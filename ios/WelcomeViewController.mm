
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

#import "ReginaHelper.h"
#import "WelcomeViewController.h"
#import "../testsuite/testsuite.h"
#import <cppunit/Exception.h>
#import <cppunit/Test.h>
#import <cppunit/TestFailure.h>
#import <cppunit/TestResult.h>
#import <cppunit/TextTestProgressListener.h>
#import <cppunit/ui/text/TestRunner.h>

@class TestSuiteController;

class iOSProgress : public CppUnit::TextTestProgressListener {
private:
    void* _iOSController;
    bool failed;
    
public:
    iOSProgress(void* iOSController) : TextTestProgressListener(), _iOSController(iOSController), failed(false) {
    }
    
    virtual void startTest(CppUnit::Test* test) {
        [(__bridge id)_iOSController startTest:@(truncateFixture(test->getName()).c_str())];
        failed = false;
    }
    
    virtual void addFailure(const CppUnit::TestFailure& failure) {
        if (! failed) {
            [(__bridge id)_iOSController failure:failure];
            failed = true;
        }
    }
    
    virtual void endTest(CppUnit::Test*) {
        if (! failed)
            [(__bridge id)_iOSController success];
    }
};

#pragma mark - Test suite controller

@interface TestSuiteController () {
    UIColor* testColour;
    UIColor* successColour;
    UIColor* failureColour;
    UIColor* runningColour;
    NSMutableAttributedString* runningString;
    NSMutableAttributedString* successString;
    
    int nSuccesses;
    int nFailures;
}
@property (weak, nonatomic) IBOutlet UITextView *output;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *closeButton;
@end

@implementation TestSuiteController

- (void)viewDidLoad
{
    testColour = [UIColor blackColor];
    successColour = [UIColor colorWithRed:0.0 green:0.5 blue:0.0 alpha:1.0];
    failureColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];
    runningColour = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];

    runningString = [[NSMutableAttributedString alloc] init];
    [runningString appendAttributedString:[[NSAttributedString alloc] initWithString:@"  –  "
                                                                          attributes:@{NSForegroundColorAttributeName:testColour}]];
    [runningString appendAttributedString:[[NSAttributedString alloc] initWithString:@"Running…\n"
                                                                          attributes:@{NSForegroundColorAttributeName:runningColour}]];

    successString = [[NSMutableAttributedString alloc] init];
    [successString appendAttributedString:[[NSAttributedString alloc] initWithString:@"  ✔︎\n"
                                                                          attributes:@{NSForegroundColorAttributeName:successColour}]];
}

- (void)viewWillAppear:(BOOL)animated
{
    self.closeButton.enabled = NO;
}

- (void)runTests
{
    [UIApplication sharedApplication].idleTimerDisabled = YES;

    nSuccesses = nFailures = 0;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        CppUnit::TextTestRunner runner;
        populateTests(runner);
        
        iOSProgress progress((__bridge void*)self);
        runner.eventManager().addListener(&progress);
        
        runner.run("", false, false, false);
        
        [self finished];
        [UIApplication sharedApplication].idleTimerDisabled = NO;
    });
}

- (void)startTest:(NSString *)name
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.output.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:name
                                                                                        attributes:@{NSForegroundColorAttributeName:testColour}]];
        [self.output.textStorage appendAttributedString:runningString];

        [self.output scrollRangeToVisible:NSMakeRange(self.output.textStorage.length, 0)];
    });
}

- (void)success
{
    dispatch_async(dispatch_get_main_queue(), ^{
        ++nSuccesses;

        [self.output.textStorage replaceCharactersInRange:NSMakeRange(self.output.textStorage.length - runningString.length, runningString.length)
                                     withAttributedString:successString];

        // No need to scroll, since we will start a new test (or summarise results) immediately.
    });
}

- (void)failure:(const CppUnit::TestFailure &)failure
{
    NSString* rawText = [NSString stringWithFormat:@"  ✘\n\nFailure: %s\n", failure.thrownException()->message().details().c_str()];
    NSAttributedString* formatted = [[NSAttributedString alloc] initWithString:rawText
                                                                    attributes:@{NSForegroundColorAttributeName:failureColour}];

    dispatch_async(dispatch_get_main_queue(), ^{
        ++nFailures;
        
        [self.output.textStorage replaceCharactersInRange:NSMakeRange(self.output.textStorage.length - runningString.length, runningString.length)
                                     withAttributedString:formatted];

        // No need to scroll, since we will start a new test (or summarise results) immediately.
    });
}
                   
- (void)finished
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (nFailures == 0) {
            NSString* rawText = [NSString stringWithFormat:@"\nAll %d tests passed.\n\n", nSuccesses];
            [self.output.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:rawText
                                                                                            attributes:@{NSForegroundColorAttributeName:successColour}]];
        } else {
            NSString* rawText = [NSString stringWithFormat:@"\n%d of %d tests failed.\nPlease pass this information on to the Regina developers.\n\n", nFailures, (nFailures + nSuccesses)];
            [self.output.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:rawText
                                                                                            attributes:@{NSForegroundColorAttributeName:failureColour}]];
        }
        
        [self.output scrollRangeToVisible:NSMakeRange(self.output.textStorage.length, 0)];

        self.closeButton.enabled = YES;
    });
}

- (IBAction)close:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end

#pragma mark - Welcome view controller

@interface WelcomeViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *icon;
@end

@implementation WelcomeViewController

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:r];
}

- (IBAction)runTestSuite:(id)sender
{
    UIViewController* sheet = [self.storyboard instantiateViewControllerWithIdentifier:@"testSuite"];
    [self presentViewController:sheet animated:YES completion:^{
        [static_cast<TestSuiteController*>(sheet) runTests];
    }];
}

- (IBAction)longPress:(id)sender
{
    UILongPressGestureRecognizer *press = static_cast<UILongPressGestureRecognizer*>(sender);
    if (press.state == UIGestureRecognizerStateBegan) {
        CGPoint location = [press locationInView:self.view];
        if (CGRectContainsPoint(self.icon.frame, location)) {
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
            [alert addAction:[UIAlertAction actionWithTitle:@"Run test suite"
                                                      style:UIAlertActionStyleDefault
                                                    handler:^(UIAlertAction*) {
                                                                [self runTestSuite:nil];
                                                            }]];
            [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                      style:UIAlertActionStyleCancel
                                                    handler:nil]];
            [alert setModalPresentationStyle:UIModalPresentationPopover];
            alert.popoverPresentationController.sourceView = self.view;
            alert.popoverPresentationController.sourceRect = self.icon.frame;
            alert.popoverPresentationController.permittedArrowDirections = UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown;
            [self presentViewController:alert animated:YES completion:nil];
        }
    }
}

@end
