
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#import "PythonInterpreter.h"
#import "TextHelper.h"
#import "packet/script.h"

// Information is displayed in dark goldenrod:
static UIColor* infoColour = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];
static UIColor* errorColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];

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

@interface PythonConsoleStdout : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

@interface PythonConsoleStderr : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

@interface PythonConsoleController () <UITextFieldDelegate> {
    PythonInterpreter* python;
    PythonConsoleStdout* outputStream;
    PythonConsoleStderr* errorStream;
    UIFont* outputFont;
    UIFont* inputFont;
    UIFont* entryFont;
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

@end

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

@implementation PythonConsoleController

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
    [c presentViewController:sheet animated:YES completion:nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    primaryPrompt = true;
    lastIndent = @"";
    self.working = true;

    // Make the prompt use a fixed-width font, so that when the prompt
    // changes we do not have to worry about redoing the toolbar layout.
    [self.prompt setTitleTextAttributes:@{NSFontAttributeName: [UIFont fontWithName:@"Menlo" size:14]} forState:UIControlStateNormal];

    outputFont = [UIFont fontWithName:@"Menlo" size:12];
    inputFont = [UIFont fontWithName:@"Menlo-Bold" size:12];
    entryFont = [UIFont fontWithName:@"Menlo" size:12];

    outputStream = [[PythonConsoleStdout alloc] init];
    errorStream = [[PythonConsoleStderr alloc] init];
    outputStream.console = errorStream.console = self;

    self.input.font = entryFont;
    [self fixToolbar];
    self.input.delegate = self;

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [nc addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];

    python = [[PythonInterpreter alloc] initWithOut:outputStream err:errorStream];
    [python importRegina];
    [python runCode:"from regina import *; print regina.welcome()\n"];
    [self processOutput:"\n"];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        if (self.script) {
            [self appendHistory:@"Running script...\n" style:HistoryInfo];
            [python runScript:self.script];
            [python flush];
        }

        if (self.root) {
            if ([python setVar:"root" value:self.root]) {
                if (self.root)
                    [self appendHistory:@"The (invisible) root of the packet tree is in the variable [root].\n" style:HistoryInfo];
            } else
                [self appendHistory:@"ERROR: I could not set the [root] variable.\nPlease report this error to the authors.\n" style:HistoryError];
        }

        if (self.item) {
            if ([python setVar:"item" value:self.item]) {
                if (self.item)
                    [self appendHistory:[NSString stringWithFormat:@"The selected packet (%@) is in the variable [item].\n",
                                         [NSString stringWithUTF8String:self.item->label().c_str()]] style:HistoryInfo];
            } else
                [self appendHistory:@"ERROR: I could not set the [item] variable.\nPlease report this error to the authors.\n" style:HistoryError];
        }

        [self appendHistory:@"Ready.\n" style:HistoryInfo];

        dispatch_async(dispatch_get_main_queue(), ^{
            self.working = false;
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
    NSString* toRun = [NSString stringWithString:self.input.text];
    NSString* toLog = [NSString stringWithFormat:@"%@ %@\n", self.prompt.title, self.input.text];
    self.working = true;
    [self appendHistory:toLog style:HistoryInput];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        bool done = [python executeLine:[toRun UTF8String]];
        [python flush];

        dispatch_async(dispatch_get_main_queue(), ^{
            if (done) {
                primaryPrompt = true;
                lastIndent = @"";
            } else {
                primaryPrompt = false;

                // Extract the indentation.
                NSUInteger i = 0;
                while ((i < toRun.length) && [[NSCharacterSet whitespaceCharacterSet] characterIsMember:[toRun characterAtIndex:i]])
                    ++i;
                if (i == toRun.length) {
                    // The entire line is whitespace.  Use no indent.
                    lastIndent = @"";
                } else
                    lastIndent = [toRun substringToIndex:i];
            }

            self.working = false;
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
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
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
                         self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + kbOffset - kbSize.height);
                         if (endWasVisible && ! endWillBeVisible)
                             self.history.contentOffset = CGPointMake(self.history.contentOffset.x, self.history.contentSize.height - newHistoryHeight);
                         [self.view layoutIfNeeded]; // Ensures that the frame change is actually animated.
                         kbOffset = kbSize.height;
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
                         self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + kbOffset);
                         [self.view layoutIfNeeded]; // Ensures that the frame change is actually animated.
                         kbOffset = 0;
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
    return (! self.working);
}

@end
