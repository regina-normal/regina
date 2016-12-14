
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

#import "NewAnglesController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "angle/anglestructures.h"
#import "progress/progresstracker.h"

#define KEY_LAST_TAUT @"NewAnglesTaut"

using regina::AngleStructures;

static NSArray* whichText;

@interface NewAnglesController () {
    bool _running;
    regina::ProgressTracker _tracker;
}
@property (weak, nonatomic) IBOutlet UILabel *triangulation;
@property (weak, nonatomic) IBOutlet UISegmentedControl *whichControl;
@property (weak, nonatomic) IBOutlet UILabel *whichDesc;
@property (weak, nonatomic) IBOutlet UIProgressView *progressControl;
@property (weak, nonatomic) IBOutlet UILabel *progressLabel;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *enumerateButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *cancelButton;
- (IBAction)whichChanged:(id)sender;
- (IBAction)cancel:(id)sender;
- (IBAction)enumerate:(id)sender;
+ (void)initArrays;
@end

@implementation NewAnglesController

- (void)viewDidLoad {
    if (! whichText)
        [NewAnglesController initArrays];
    
    if (self.spec.parent)
        _triangulation.text = [NSString stringWithUTF8String:self.spec.parent->label().c_str()];

    self.whichControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TAUT];

    // Update the description labels.
    [self whichChanged:nil];
}

- (IBAction)whichChanged:(id)sender {
    _whichDesc.text = whichText[_whichControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.whichControl.selectedSegmentIndex forKey:KEY_LAST_TAUT];
}

- (IBAction)cancel:(id)sender {
    if (_running) {
        _cancelButton.enabled = NO;
        _progressLabel.text = @"Cancelling...";
        _tracker.cancel();
    } else
        [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)updateProgress {
    _progressControl.progress = _tracker.percent() / 100;
}

- (IBAction)enumerate:(id)sender {
    bool tautOnly = (_whichControl.selectedSegmentIndex == 1);
    
    _whichControl.enabled = NO;
    _enumerateButton.enabled = NO;
    
    _progressControl.progress = 0.0;
    
    _progressLabel.hidden = NO;
    _progressControl.hidden = NO;

    _running = true;

    [UIApplication sharedApplication].idleTimerDisabled = YES;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        AngleStructures* ans =
            AngleStructures::enumerate((regina::Triangulation<3>*)self.spec.parent, tautOnly, &_tracker);
        while (! _tracker.isFinished()) {
            if (_tracker.percentChanged()) {
                // This operation blocks until the UI is updated:
                [self performSelectorOnMainThread:@selector(updateProgress) withObject:nil waitUntilDone:YES];
            }
            [NSThread sleepForTimeInterval:0.1];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [UIApplication sharedApplication].idleTimerDisabled = NO;
            
            if (_tracker.isCancelled()) {
                delete ans;
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Enumeration Cancelled"
                                                                message:nil
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
            } else {
                if (tautOnly)
                    ans->setLabel("Taut angle structures");
                else
                    ans->setLabel("Vertex angle structures");
                [self.spec created:ans];
            }
            [self dismissViewControllerAnimated:YES completion:nil];
        });
    });
}

+ (void)initArrays {
    whichText = [NSArray arrayWithObjects:
                 @"All vertices of the angle structure solution polytope",
                 @"All taut angle structures",
                 nil];
}

@end
