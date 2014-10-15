
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#import <cppunit/Test.h>
#import <cppunit/TestResult.h>
#import <cppunit/TextTestProgressListener.h>
#import <cppunit/ui/text/TestRunner.h>

class iOSProgress : public CppUnit::TextTestProgressListener {
private:
    bool failed;
    
public:
    iOSProgress() : TextTestProgressListener(), failed(false) {
    }
    
    virtual void startTest(CppUnit::Test* test) {
        NSLog(@(truncateFixture(test->getName()).c_str()));
        failed = false;
    }
    
    virtual void addFailure(const CppUnit::TestFailure&) {
        if (! failed) {
            NSLog(@"FAILED.");
            failed = true;
        }
    }
    
    virtual void endTest(CppUnit::Test*) {
        if (! failed)
            NSLog(@"ok.");
    }
};

void runAllTests() {
    NSLog(@"Regina calculation engine test suite");
    
    CppUnit::TextTestRunner runner;
    iOSProgress progress;
    
    populateTests(runner);
    
    runner.eventManager().addListener(&progress);
    runner.run("", false, true, false);
}

#pragma mark - Test suite controller

@interface TestSuiteController ()
@property (weak, nonatomic) IBOutlet UITextView *output;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *closeButton;
@end

@implementation TestSuiteController

- (void)viewWillAppear:(BOOL)animated
{
    self.closeButton.enabled = NO;
}

- (void)runTests
{
    // TODO.
    self.closeButton.enabled = YES;
}

- (IBAction)close:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end

#pragma mark - Welcome view controller

@interface WelcomeViewController () <UIActionSheetDelegate>
@property (weak, nonatomic) IBOutlet UIImageView *icon;
@end

@implementation WelcomeViewController

- (void)viewDidLoad
{
    UILongPressGestureRecognizer *r = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:r];
}

- (void)runTestSuite
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
            if ([ReginaHelper ios8]) {
                UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
                [alert addAction:[UIAlertAction actionWithTitle:@"Run test suite"
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction*) {
                                                            [self runTestSuite];
                                                        }]];
                [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                          style:UIAlertActionStyleCancel
                                                        handler:nil]];
                [alert setModalPresentationStyle:UIModalPresentationPopover];
                alert.popoverPresentationController.sourceView = self.view;
                alert.popoverPresentationController.sourceRect = self.icon.frame;
                [self presentViewController:alert animated:YES completion:nil];
            } else {
                UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                                   delegate:self
                                                          cancelButtonTitle:@"Cancel"
                                                     destructiveButtonTitle:nil
                                                          otherButtonTitles:@"Run test suite", nil];
                [sheet showFromRect:self.icon.frame inView:self.view animated:YES];
            }
        }
    }
}

#pragma mark Action Sheet

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    if (buttonIndex != actionSheet.cancelButtonIndex)
        [self runTestSuite];
}

@end
