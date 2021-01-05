
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#import "PythonConsoleController.h"
#import "ReginaHelper.h"
#import "TextHelper.h"
#import "packet/script.h"
#import "utilities/i18nutils.h"
#import "../python/gui/pythoninterpreter.h"
#import "../python/gui/pythonoutputstream.h"

// Information is displayed in dark goldenrod:
static UIColor* infoColour = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];
static UIColor* errorColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];

// Python console fonts are created on demand.
static UIFont* promptFont = nil;
static UIFont* inputFont = nil;
static UIFont* outputFont = nil;
static UIFont* entryFont = nil;
static UIFont* keyboardFont = nil;

#define KEY_PYTHON_ACCEPTED @"PythonAccepted"

/**
 * Represents different visual styles for the text that is logged in the
 * scrollable history area for a Python console.
 */
enum HistoryStyle {
    HistoryInput,
    HistoryOutput,
    HistoryInfo,
    HistoryError
};

#pragma mark - Output streams

/**
 * A protocol for objects that can act as Python output streams
 * sys.stdout and/or sys.stderr.
 */
@protocol PythonOutputStream <NSObject>
@required
/**
 * Process a chunk of data that was sent to this output stream.
 * This routine might for instance display the data to the user
 * or write it to a log file.
 *
 * You should assume that \a data is encoded in UTF-8.
 */
- (void)processOutput:(const char*)data;
@end

/**
 * A C++ wrapper around an objective-C PythonOutputStream.
 */
class PythonOutputStreamObjC : public regina::python::PythonOutputStream {
private:
    void* _object;
        /**< The Objective-C delegate that implements processOutput:. */
    
public:
    PythonOutputStreamObjC(void* object) : _object(object) {
    }
    
    virtual void processOutput(const std::string& data) {
        [(__bridge id)_object processOutput:data.c_str()];
    }
};

/**
 * An output stream that writes data in a plain font to an iOS python console.
 */
@interface PythonConsoleStdout : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

/**
 * An output stream that writes data in an alert font to an iOS python console.
 */
@interface PythonConsoleStderr : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

#pragma mark - Keyboard Buttons

@interface KeyboardButton : UIButton
@property (strong, nonatomic) NSString *toInsert;
- (id)initWithLabel:(nonnull NSString*)label
            fgColor:(nonnull UIColor*)fgColor
          fgPressed:(nullable UIColor*)fgPressed
         fgDisabled:(nullable UIColor *)fgDisabled
            bgColor:(nonnull UIColor*)bgColor
          bgPressed:(nonnull UIColor*)bgPressed;
@end

#pragma mark - Python Console

@interface PythonConsoleController () <UITextFieldDelegate> {
    PythonConsoleStdout* outputStream;
    PythonConsoleStderr* errorStream;
    bool primaryPrompt;
    NSString* lastIndent;
    CGFloat kbOffset;
}

@property (weak, nonatomic) IBOutlet UITextView *history;
@property (weak, nonatomic) IBOutlet UITextField *input;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *prompt;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *closeButton;
@property (weak, nonatomic) IBOutlet UIToolbar *toolbar;
@property (assign, nonatomic) bool working;

@property (assign, nonatomic) regina::Packet* root;
@property (assign, nonatomic) regina::Packet* item;
@property (assign, nonatomic) regina::Script* script;

- (void)appendHistory:(NSString*)text style:(HistoryStyle)style;

- (UIBarButtonItem*)textButton:(NSString*)text;
- (UIBarButtonItem*)tabButton;
- (UIBarButtonItem*)historyButton:(BOOL)past;
+ (UIBarButtonItem*)smallGap;
+ (UIBarButtonItem*)largeGap;

@end

#pragma mark - Implementation Details

@implementation PythonConsoleStdout
- (void)processOutput:(const char *)data
{
    [self.console appendHistory:[NSString stringWithUTF8String:data] style:HistoryOutput];
}
@end

@implementation PythonConsoleStderr
- (void)processOutput:(const char *)data
{
    [self.console appendHistory:[NSString stringWithUTF8String:data] style:HistoryError];
}
@end

@implementation KeyboardButton {
    UIColor* _background;
    UIColor* _pressedBackground;
}

- (id)initWithLabel:(NSString *)label
            fgColor:(UIColor *)fgColor
          fgPressed:(UIColor *)fgPressed
         fgDisabled:(UIColor *)fgDisabled
            bgColor:(UIColor *)bgColor
          bgPressed:(UIColor *)bgPressed {
    self = [super initWithFrame:CGRectZero];
    if (self) {
        _background = bgColor;
        _pressedBackground = bgPressed;
        self.backgroundColor = _background;
        
        self.layer.cornerRadius = 5.0;
        self.layer.shadowOffset = CGSizeMake(0, 1.0);
        self.layer.shadowRadius = 0.2;
        self.layer.shadowOpacity = 0.5;
        
        // I don't think we need to set frame or masksToBounds.
        
        [self setAttributedTitle:[[NSAttributedString alloc]
                                  initWithString:label
                                  attributes:@{NSFontAttributeName: keyboardFont,
                                               NSForegroundColorAttributeName: fgColor}]
                        forState:UIControlStateNormal];
        if (fgPressed)
            [self setAttributedTitle:[[NSAttributedString alloc]
                                      initWithString:label
                                      attributes:@{NSFontAttributeName: keyboardFont,
                                                   NSForegroundColorAttributeName: fgPressed}]
                            forState:UIControlStateHighlighted];
        if (fgDisabled)
            [self setAttributedTitle:[[NSAttributedString alloc]
                                      initWithString:label
                                      attributes:@{NSFontAttributeName: keyboardFont,
                                                   NSForegroundColorAttributeName: fgDisabled}]
                            forState:UIControlStateDisabled];
    }
    return self;
}

- (void)setHighlighted:(BOOL)highlighted {
    if (highlighted)
        self.backgroundColor = _pressedBackground;
    else
        self.backgroundColor = _background;
    
    return [super setHighlighted:highlighted];
}

@end

@implementation PythonConsoleController {
    PythonOutputStreamObjC* _outCpp;
    PythonOutputStreamObjC* _errCpp;
    
    NSMutableArray<NSString*>* _cmdHistory;
    NSUInteger _cmdHistoryPos;
    NSString* _currentLine;
    UIBarButtonItem* _pastButton;
    UIBarButtonItem* _futureButton;
    
    NSArray<UIKeyCommand*>* _keyCommands;
    
    regina::python::PythonInterpreter* _interpreter;
}

+ (void)openConsoleFromViewController:(UIViewController *)c root:(regina::Packet *)root item:(regina::Packet *)item script:(regina::Script *)script
{
    BOOL accepted = [[NSUserDefaults standardUserDefaults] boolForKey:KEY_PYTHON_ACCEPTED];
    if (! accepted) {
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Use Python?"
                                                                       message:@"Regina's Python interface is for advanced users. It gives you low-level programmable control over Regina's mathematical engine. Do you wish to continue?"
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"Continue" style:UIAlertActionStyleDefault handler:^(UIAlertAction*) {
            [[NSUserDefaults standardUserDefaults] setBool:TRUE forKey:KEY_PYTHON_ACCEPTED];
            [PythonConsoleController openConsoleFromViewController:c root:root item:item script:script];
        }]];
        [alert addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];
        [c presentViewController:alert animated:YES completion:nil];
        return;
    }

    PythonConsoleController* sheet = static_cast<PythonConsoleController*>([c.storyboard instantiateViewControllerWithIdentifier:@"pythonConsole"]);
    sheet.root = root;
    sheet.item = item;
    sheet.script = script;
    sheet.modalPresentationStyle = UIModalPresentationFullScreen;
    [c presentViewController:sheet animated:YES completion:nil];
}

+ (void)setupFonts {
    promptFont = [UIFont fontWithName:@"Menlo" size:14];
    outputFont = [UIFont fontWithName:@"Menlo" size:12];
    inputFont = [UIFont fontWithName:@"Menlo-Bold" size:12];
    entryFont = outputFont;
    keyboardFont = [UIFont fontWithName:@"Menlo" size:18];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    if (! promptFont)
        [PythonConsoleController setupFonts];
    
    _cmdHistory = [[NSMutableArray<NSString*> alloc] init];
    _cmdHistoryPos = 0;
    _currentLine = [NSString string];
    
    // Give an extra row on the keyboard containing buttons useful for programmers.
    _pastButton = [self historyButton:YES];
    _futureButton = [self historyButton:NO];
    _pastButton.enabled = NO;
    _futureButton.enabled = NO;
    
    _keyCommands = @[[UIKeyCommand keyCommandWithInput:@"\t"
                                         modifierFlags:0
                                                action:@selector(tabPressed)],
                     [UIKeyCommand keyCommandWithInput:UIKeyInputUpArrow
                                         modifierFlags:0
                                                action:@selector(historyPast)],
                     [UIKeyCommand keyCommandWithInput:UIKeyInputDownArrow
                                         modifierFlags:0
                                                action:@selector(historyFuture)],
                     [UIKeyCommand keyCommandWithInput:@"d"
                                         modifierFlags:UIKeyModifierControl
                                                action:@selector(close:)],
                     [UIKeyCommand keyCommandWithInput:@"w"
                                         modifierFlags:UIKeyModifierCommand
                                                action:@selector(close:)]];

    UIToolbar* bar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 50)];
    bar.barStyle = UIBarStyleDefault;
    bar.translucent = YES;
    bar.items = @[[self tabButton], [PythonConsoleController largeGap],

                  [self textButton:@"2"], [PythonConsoleController smallGap],
                  [self textButton:@"3"], [PythonConsoleController smallGap],
                  [self textButton:@"4"], [PythonConsoleController largeGap],
                  
                  [self textButton:@"("], [PythonConsoleController smallGap],
                  [self textButton:@")"], [PythonConsoleController smallGap],
                  [self textButton:@"["], [PythonConsoleController smallGap],
                  [self textButton:@"]"], [PythonConsoleController smallGap],
                  [self textButton:@"<"], [PythonConsoleController smallGap],
                  [self textButton:@">"], [PythonConsoleController largeGap],
                  
                  [self textButton:@"+"], [PythonConsoleController smallGap],
                  [self textButton:@"-"], [PythonConsoleController smallGap],
                  [self textButton:@"*"], [PythonConsoleController smallGap],
                  [self textButton:@"/"], [PythonConsoleController smallGap],
                  [self textButton:@"="], [PythonConsoleController largeGap],

                  [self textButton:@":"], [PythonConsoleController smallGap],
                  [self textButton:@";"], [PythonConsoleController smallGap],
                  [self textButton:@"\""], [PythonConsoleController largeGap],
                  
                  _pastButton, [PythonConsoleController smallGap], _futureButton];
    
    [bar sizeToFit];
    self.input.inputAccessoryView = bar;
}

- (void)viewWillAppear:(BOOL)animated
{
    primaryPrompt = true;
    lastIndent = @"";
    self.working = true;

    // Make the prompt use a fixed-width font, so that when the prompt
    // changes we do not have to worry about redoing the toolbar layout.
    [self.prompt setTitleTextAttributes:@{NSFontAttributeName: promptFont} forState:UIControlStateNormal];
    // The prompt should really have all interaction disabled.
    // Until we work out how to do this properly, at least make sure it keeps
    // the fixed-width font when the user presses it (on iOS 13 this makes it
    // go grey).
    [self.prompt setTitleTextAttributes:@{NSFontAttributeName: promptFont} forState:UIControlStateHighlighted];

    // Remove the suggestions bar above the keyboard, which will be empty anyway.
    UITextInputAssistantItem* item = [self inputAssistantItem];
    item.leadingBarButtonGroups = @[];
    item.trailingBarButtonGroups = @[];
    
    outputStream = [[PythonConsoleStdout alloc] init];
    errorStream = [[PythonConsoleStderr alloc] init];
    outputStream.console = errorStream.console = self;

    self.input.font = entryFont;
    [self fixToolbar];
    self.input.delegate = self;

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [nc addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];

    _outCpp = new PythonOutputStreamObjC((__bridge void*)outputStream);
    _errCpp = new PythonOutputStreamObjC((__bridge void*)errorStream);
    
    _interpreter = new regina::python::PythonInterpreter(*_outCpp, *_errCpp);

    _interpreter->importRegina();
    _interpreter->executeLine("from regina import *; print(regina.welcome())\n");
    [self processOutput:"\n"];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        if (self.script) {
            [self appendHistory:@"Running script...\n" style:HistoryInfo];
            self->_interpreter->runScript(self.script);
        }

        if (self.root) {
            if (self->_interpreter->setVar("root", self.root)) {
                if (self.root)
                    [self appendHistory:@"The (invisible) root of the packet tree is in the variable [root].\n" style:HistoryInfo];
            } else
                [self appendHistory:@"ERROR: I could not set the [root] variable.\nPlease report this error to the authors.\n" style:HistoryError];
        }

        if (self.item) {
            if (self->_interpreter->setVar("item", self.item)) {
                if (self.item)
                    [self appendHistory:[NSString stringWithFormat:@"The selected packet (%@) is in the variable [item].\n",
                                         [NSString stringWithUTF8String:self.item->label().c_str()]] style:HistoryInfo];
            } else
                [self appendHistory:@"ERROR: I could not set the [item] variable.\nPlease report this error to the authors.\n" style:HistoryError];
        }

        [self appendHistory:@"Ready.\n" style:HistoryInfo];

        dispatch_async(dispatch_get_main_queue(), ^{
            self.working = false;
            
            if (self->_interpreter->exitAttempted())
                [self close:nil];
            else
                [self.input becomeFirstResponder];
        });
    });
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc removeObserver:self name:UIKeyboardWillShowNotification object:nil];
    [nc removeObserver:self name:UIKeyboardWillHideNotification object:nil];
}

- (void)dealloc
{
    NSLog(@"Python interpreter being deallocated");
    
    // Delete C++ resources that are not managed by ARC.
    delete _interpreter;
    delete _outCpp;
    delete _errCpp;
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext>  __nonnull context) {
        [self fixToolbar];
    } completion:^(id<UIViewControllerTransitionCoordinatorContext>  __nonnull context) {
    }];
}

- (void)setWorking:(bool)working
{
    // Must be called from the main thread.
    if (working) {
        self.input.attributedText = [TextHelper dimString:@"Processing..."];
        _working = true;

        self.closeButton.enabled = false;
        self.prompt.title = @"   ";
    } else {
        self.prompt.title = (primaryPrompt ? @">>>" : @"...");
        self.closeButton.enabled = true;
        self.input.enabled = true;

        _working = false;
        self.input.text = lastIndent;
        self.input.textColor = [UIColor blackColor]; // Override the grey from "Processing".
    }

    [self fixToolbar];
}

- (IBAction)execute:(id)sender
{
    // Must be called from the main thread.
    [_cmdHistory addObject:self.input.text];
    _cmdHistoryPos = _cmdHistory.count;
    _pastButton.enabled = YES;
    _futureButton.enabled = NO;
    
    NSString* toRun = [NSString stringWithString:self.input.text];
    NSString* toLog = [NSString stringWithFormat:@"%@ %@\n", self.prompt.title, self.input.text];
    self.working = true;
    [self appendHistory:toLog style:HistoryInput];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        bool done = self->_interpreter->executeLine([toRun UTF8String]);
        self->_interpreter->flush();

        dispatch_async(dispatch_get_main_queue(), ^{
            if (done) {
                self->primaryPrompt = true;
                self->lastIndent = @"";
            } else {
                self->primaryPrompt = false;

                // Extract the indentation.
                NSUInteger i = 0;
                while ((i < toRun.length) && [[NSCharacterSet whitespaceCharacterSet] characterIsMember:[toRun characterAtIndex:i]])
                    ++i;
                if (i == toRun.length) {
                    // The entire line is whitespace.  Use no indent.
                    self->lastIndent = @"";
                } else
                    self->lastIndent = [toRun substringToIndex:i];
            }

            self.working = false;
            
            if (self->_interpreter->exitAttempted())
                [self close:nil];
            else
                [self.input becomeFirstResponder];
        });
    });
}

- (IBAction)close:(id)sender
{
    // Must be called from the main thread.
    if (! self.working)
        [self dismissViewControllerAnimated:YES completion:nil];
}

- (NSArray<UIKeyCommand *> *)keyCommands {
    return _keyCommands;
}

- (NSString*)tabCompletion:(NSString*)textToComplete {
    // We only send the last "word", where a word starts with a character or
    // underscore, and only contains letters, numbers, underscores and the dot.
    NSRegularExpression* re = [[NSRegularExpression alloc]
                               initWithPattern:@"[\\p{Ll}\\p{Lu}\\p{Lt}\\p{Lo}_][\\w_.]*$"
                               options:0
                               error:nil];
    NSTextCheckingResult* match = [re firstMatchInString:textToComplete
                                                 options:0
                                                   range:NSMakeRange(0, textToComplete.length)];
    
    if (match.numberOfRanges == 0) {
        // Nothing to complete.
        return nil;
    }
    
    NSString* word = [textToComplete substringWithRange:match.range];

    regina::python::PrefixCompleter comp;
    int ans = _interpreter->complete(word.UTF8String, comp);
    if (ans < 0) {
        NSLog(@"ERROR: Completion failed");
        return nil;
    } else if (ans == 0) {
        // No completions for this word.
        return nil;
    } else {
        // Note: PrefixCompleter does understand unicode, and computes the
        // longest common prefix in terms of UTF-8 unicode characters.
        NSString* completion = [NSString stringWithUTF8String:comp.prefix().c_str()];
        
        // When returning the completion, ignore the initial word that was matched.
        if (completion.length >= word.length)
            return [completion substringFromIndex:word.length];
        else {
            // Something broke: we did not even get the initial matching word.
            NSLog(@"ERROR: Completion does not include original matched word");
            return nil;
        }
    }
}

- (IBAction)tabPressed {
    UITextRange* selection = [self.input selectedTextRange];
    if (! selection) {
        NSLog(@"ERROR: Cannot fetch cursor position");
        // Pretend the cursor is at the end of the input.
        selection = [self.input textRangeFromPosition:self.input.endOfDocument toPosition:self.input.endOfDocument];
    }
    [self tabPressedOnRange:selection];
}

- (void)tabPressedOnRange:(UITextRange*)selection {
    // If the cursor is sitting immediately after non-whitespace, attempt tab completion.
    // First fetch everything *before* the cursor.
    // We will try to tab complete at the end of this block.
    UITextRange* prefixRange = [self.input textRangeFromPosition:self.input.beginningOfDocument toPosition:selection.start];
    NSString* prefix = [self.input textInRange:prefixRange];
    
    NSUInteger len = prefix.length;
    if (len > 0) {
        unichar last = [self.input.text characterAtIndex:(len - 1)];
        if (! [[NSCharacterSet whitespaceCharacterSet] characterIsMember:last]) {
            // Try to tab complete the prefix.
            NSString* completion = [self tabCompletion:prefix];
            if (completion)
                [self.input replaceRange:selection withText:completion];
            return;
        }
    }
    
    // Just insert an actual tab (but using spaces).
    [self.input replaceRange:selection withText:@"    "];
}

- (IBAction)historyPast {
    if (_cmdHistoryPos == _cmdHistory.count)
        _currentLine = self.input.text;
    if (_cmdHistoryPos == 0) {
        // Do nothing.
        // The enabled status of the button should stop us from ever reaching here.
    } else {
        --_cmdHistoryPos;
        self.input.text = _cmdHistory[_cmdHistoryPos];
        // Setting the text seems to move the cursor to the end-of-line automatically.
        _futureButton.enabled = YES;
        if (_cmdHistoryPos == 0)
            _pastButton.enabled = NO;
    }
}

- (IBAction)historyFuture {
    if (_cmdHistoryPos == _cmdHistory.count) {
        // Do nothing.
        // The enabled status of the button should stop us from ever reaching here.
    } else {
        ++_cmdHistoryPos;
        if (_cmdHistoryPos == _cmdHistory.count) {
            self.input.text = _currentLine;
            _futureButton.enabled = NO;
        } else
            self.input.text = _cmdHistory[_cmdHistoryPos];
        _pastButton.enabled = YES;
    }
}

- (IBAction)inputFromButton:(id)sender {
    [self.input insertText:((KeyboardButton*)sender).toInsert];
}

- (void)appendHistory:(NSString*)text style:(HistoryStyle)style
{
    // Can be called from any thread.
    dispatch_async(dispatch_get_main_queue(), ^{
        switch (style) {
            case HistoryInput:
                [self.history.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:text attributes:@{NSFontAttributeName: inputFont}]];
                break;
            case HistoryOutput:
                [self.history.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:text attributes:@{NSFontAttributeName: outputFont}]];
                break;
            case HistoryInfo:
                [self.history.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:text attributes:@{NSFontAttributeName: outputFont, NSForegroundColorAttributeName: infoColour}]];
                break;
            case HistoryError:
                [self.history.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:text attributes:@{NSFontAttributeName: outputFont, NSForegroundColorAttributeName: errorColour}]];
                break;
        }

        [self.history scrollRangeToVisible:NSMakeRange(self.history.textStorage.length, 0)];
    });
}

- (void)processOutput:(const char *)data
{
    // Can be called from any thread.
    [self appendHistory:[NSString stringWithUTF8String:data] style:HistoryOutput];
}

- (void)fixToolbar
{
    // Why is 135 the magic number?  I'm not entirely sure... but it lays things out correctly. *sigh*
    self.input.frame = CGRectMake(0, 0, self.toolbar.frame.size.width - self.closeButton.width - self.prompt.width - 135, self.input.frame.size.height);
}

- (void)keyboardWillShow:(NSNotification*)notification
{
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
    NSTimeInterval duration = [[notification.userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    NSInteger animationCurve = [[notification.userInfo objectForKey:UIKeyboardAnimationCurveUserInfoKey] integerValue];

    // Do we need to scroll the text view?
    CGFloat newHistoryHeight = self.history.frame.size.height + kbOffset - kbSize.height;
    bool endWasVisible = (self.history.contentSize.height - self.history.contentOffset.y <= self.history.frame.size.height);
    bool endWillBeVisible = (self.history.contentSize.height - self.history.contentOffset.y <= newHistoryHeight);

    [UIView animateWithDuration:duration
                          delay:0
                        options:UIViewAnimationOptions(animationCurve << 16)
                     animations:^{
                         self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + self->              kbOffset - kbSize.height);
                         if (endWasVisible && ! endWillBeVisible)
                             self.history.contentOffset = CGPointMake(self.history.contentOffset.x, self.history.contentSize.height - newHistoryHeight);
                         [self.view layoutIfNeeded]; // Ensures that the frame change is actually animated.
                         self->kbOffset = kbSize.height;
                     }
                     completion:^(BOOL finished){}];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    NSTimeInterval duration = [[notification.userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    NSInteger animationCurve = [[notification.userInfo objectForKey:UIKeyboardAnimationCurveUserInfoKey] integerValue];

    [UIView animateWithDuration:duration
                          delay:0
                        options:UIViewAnimationOptions(animationCurve << 16)
                     animations:^{
                         self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + self->kbOffset);
                         [self.view layoutIfNeeded]; // Ensures that the frame change is actually animated.
                         self->kbOffset = 0;
                     }
                     completion:^(BOOL finished){}];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self execute:self];
    return YES;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
    return (! self.working);
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if (self.working)
        return NO;
    
    if ([string isEqualToString:@"\t"]) {
        // Intercept tab presses.
        // Here we rereoute them to either tab completion or
        // inserting a sequence of spaces, depending on context.
        UITextPosition* from = [textField positionFromPosition:textField.beginningOfDocument offset:range.location];
        UITextPosition* to = [textField positionFromPosition:from offset:range.length];
        [self tabPressedOnRange:[textField textRangeFromPosition:from toPosition:to]];
        return NO;
    }
    return YES;
}

- (UIBarButtonItem*)textButton:(NSString *)text {
    KeyboardButton *view = [[KeyboardButton alloc] initWithLabel:text
                                                         fgColor:[UIColor blackColor]
                                                       fgPressed:nil
                                                      fgDisabled:nil
                                                         bgColor:[UIColor whiteColor]
                                                       bgPressed:[UIColor lightGrayColor]];
    view.toInsert = text;
    [view addTarget:self action:@selector(inputFromButton:) forControlEvents:UIControlEventPrimaryActionTriggered];
    
    return [[UIBarButtonItem alloc] initWithCustomView:view];
}

- (UIBarButtonItem*)tabButton {
    KeyboardButton *view = [[KeyboardButton alloc] initWithLabel:@" Tab "
                                                         fgColor:[UIColor blackColor]
                                                       fgPressed:nil
                                                      fgDisabled:nil
                                                         bgColor:[UIColor lightGrayColor]
                                                       bgPressed:[UIColor whiteColor]];
    [view addTarget:self action:@selector(tabPressed) forControlEvents:UIControlEventPrimaryActionTriggered];
    
    return [[UIBarButtonItem alloc] initWithCustomView:view];
}

- (UIBarButtonItem*)historyButton:(BOOL)past {
    KeyboardButton* view;
    if (past) {
        view = [[KeyboardButton alloc] initWithLabel:@"⬆︎"
                                             fgColor:[UIColor blackColor]
                                           fgPressed:nil
                                          fgDisabled:[UIColor grayColor]
                                             bgColor:[UIColor lightGrayColor]
                                           bgPressed:[UIColor whiteColor]];
        [view addTarget:self action:@selector(historyPast) forControlEvents:UIControlEventPrimaryActionTriggered];
    } else {
        view = [[KeyboardButton alloc] initWithLabel:@"⬇︎"
                                             fgColor:[UIColor blackColor]
                                           fgPressed:nil
                                          fgDisabled:[UIColor grayColor]
                                             bgColor:[UIColor lightGrayColor]
                                           bgPressed:[UIColor whiteColor]];
        [view addTarget:self action:@selector(historyFuture) forControlEvents:UIControlEventPrimaryActionTriggered];
    }
    
    return [[UIBarButtonItem alloc] initWithCustomView:view];
}

+ (UIBarButtonItem *)smallGap {
    UIBarButtonItem* item = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
    item.width = 5;
    return item;
}

+ (UIBarButtonItem *)largeGap {
    return [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
}

@end
