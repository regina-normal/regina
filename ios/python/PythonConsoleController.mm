
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

// TODO: Make keyboard permanently visible?
// TODO: Make sure the UI DTRT when packets are deleted, etc.

// Information is displayed in dark goldenrod:
static UIColor* infoColour = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];
static UIColor* errorColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];

@interface LongTextField : UITextField
- (CGSize)sizeThatFits:(CGSize)size;
@end

@implementation LongTextField
- (CGSize)sizeThatFits:(CGSize)size
{
    return size;
}
@end

@interface PythonConsoleStdout : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

@implementation PythonConsoleStdout
- (void)processOutput:(const char *)data
{
    [self.console appendHistory:[NSString stringWithUTF8String:data] style:HistoryOutput];
}
@end

@interface PythonConsoleStderr : NSObject<PythonOutputStream>
@property (weak, nonatomic) PythonConsoleController* console;
@end

@implementation PythonConsoleStderr
- (void)processOutput:(const char *)data
{
    [self.console appendHistory:[NSString stringWithUTF8String:data] style:HistoryError];
}
@end

@interface PythonConsoleController () <UITextFieldDelegate> {
    PythonInterpreter* python;
    PythonConsoleStdout* outputStream;
    PythonConsoleStderr* errorStream;
    UIFont* outputFont;
    UIFont* inputFont;
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

@end

@implementation PythonConsoleController

- (void)viewWillAppear:(BOOL)animated
{
    primaryPrompt = true;
    lastIndent = @"";
    self.working = true;

    outputFont = [UIFont fontWithName:@"Menlo" size:12];
    inputFont = [UIFont fontWithName:@"Menlo-Bold" size:12];

    outputStream = [[PythonConsoleStdout alloc] init];
    errorStream = [[PythonConsoleStderr alloc] init];
    outputStream.console = errorStream.console = self;

    [self fixToolbar];
    self.input.delegate = self;

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [nc addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];

    python = [[PythonInterpreter alloc] initWithOut:outputStream err:errorStream];
    [python importRegina];
    [python runCode:"print regina.welcome()\n"];
    [self processOutput:"\n"];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        if (self.script) {
            [self appendHistory:@"Running script...\n" style:HistoryInfo];
            [python runScript:self.script];
            [python flush];
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
        _working = true;
        self.input.enabled = false;
        self.closeButton.enabled = false;
        self.prompt.title = @"     ";
        self.input.text = @"Processing...";
    } else {
        self.prompt.title = (primaryPrompt ? @">>>" : @"...");
        self.input.text = lastIndent;
        self.closeButton.enabled = true;
        self.input.enabled = true;
        _working = false;
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
    // TODO: Animate correctly.
    CGSize kbSize = [[[notification userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    NSTimeInterval duration = [[notification.userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    NSInteger animationCurve = [[notification.userInfo objectForKey:UIKeyboardAnimationCurveUserInfoKey] integerValue];

    [UIView animateWithDuration:duration
                          delay:0
                        options:UIViewAnimationOptions(animationCurve << 16)
                     animations:^{
                         self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + kbOffset - kbSize.height);
                         kbOffset = kbSize.height;
                     }
                     completion:^(BOOL finished){}];
}

- (void)keyboardWillHide:(NSNotification*)notification
{
    // TODO: Animate
    self.view.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height + kbOffset);
    kbOffset = 0;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self execute:self];
    return YES;
}

@end
