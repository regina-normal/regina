
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

#import "Coordinates.h"
#import "NewSurfacesController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "progress/progresstracker.h"
#import "surfaces/normalsurfaces.h"

#define KEY_LAST_TYPE @"NewSurfacesType"
#define KEY_LAST_COORDS @"NewSurfacesCoords"
#define KEY_LAST_EMB @"NewSurfacesEmb"

using regina::NormalSurfaces;

static NSArray* whichText;
static NSArray* coordText;
static NSArray* embText;

@interface NewSurfacesController () {
    bool _running;
    regina::ProgressTracker _tracker;
}
@property (weak, nonatomic) IBOutlet UILabel *triangulation;
@property (weak, nonatomic) IBOutlet UISegmentedControl *whichControl;
@property (weak, nonatomic) IBOutlet UILabel *whichDesc;
@property (weak, nonatomic) IBOutlet UISegmentedControl *coordControl;
@property (weak, nonatomic) IBOutlet UILabel *coordDesc;
@property (weak, nonatomic) IBOutlet UISegmentedControl *embControl;
@property (weak, nonatomic) IBOutlet UILabel *embDesc;
@property (weak, nonatomic) IBOutlet UIProgressView *progressControl;
@property (weak, nonatomic) IBOutlet UILabel *progressLabel;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *enumerateButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *cancelButton;
- (IBAction)whichChanged:(id)sender;
- (IBAction)coordChanged:(id)sender;
- (IBAction)embChanged:(id)sender;
- (IBAction)cancel:(id)sender;
- (IBAction)enumerate:(id)sender;
+ (void)initArrays;
@end

@implementation NewSurfacesController

- (void)viewDidLoad {
    if (! whichText)
        [NewSurfacesController initArrays];
    
    if (self.spec.parent)
        _triangulation.text = [NSString stringWithUTF8String:self.spec.parent->label().c_str()];

    self.whichControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TYPE];
    self.coordControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_COORDS];
    self.embControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_EMB];

    // Update the description labels.
    [self whichChanged:nil];
    [self coordChanged:nil];
    [self embChanged:nil];
}

- (IBAction)whichChanged:(id)sender {
    _whichDesc.text = whichText[_whichControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.whichControl.selectedSegmentIndex forKey:KEY_LAST_TYPE];
}

- (IBAction)coordChanged:(id)sender {
    _coordDesc.text = coordText[_coordControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.coordControl.selectedSegmentIndex forKey:KEY_LAST_COORDS];
}

- (IBAction)embChanged:(id)sender {
    _embDesc.text = embText[_embControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.embControl.selectedSegmentIndex forKey:KEY_LAST_EMB];
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
    regina::NormalCoords coords;
    regina::NormalList which;
    regina::NormalAlg alg;
    
    bool broken = false;
    bool almostNormal = false;
    switch (_whichControl.selectedSegmentIndex) {
        case 0: which = regina::NS_VERTEX; break;
        case 1: which = regina::NS_FUNDAMENTAL; break;
        default: broken = true; break;
    }
    switch (_embControl.selectedSegmentIndex) {
        case 0: which |= regina::NS_EMBEDDED_ONLY; break;
        case 1: which |= regina::NS_IMMERSED_SINGULAR; break;
        default: broken = true; break;
    }
    switch (_coordControl.selectedSegmentIndex) {
        case 0: coords = regina::NS_STANDARD; break;
        case 1: coords = regina::NS_QUAD; break;
        case 2: coords = regina::NS_AN_STANDARD; almostNormal = true; break;
        case 3: coords = regina::NS_AN_QUAD_OCT; almostNormal = true; break;
        default: broken = true; break;
    }
    if (broken) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Selection Made"
                                                        message:@"Please select an option for each of the three parameters: (i) surface type; (ii) coordinate system; and (iii) embedded vs immersed and/or singular."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    if (almostNormal && which.has(regina::NS_IMMERSED_SINGULAR)) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Selection Not Supported"
                                                        message:@"At present, I cannot enumerate immersed and/or singular surfaces in an almost normal coordinate system.  Please select a different combination of options."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    _whichControl.enabled = NO;
    _coordControl.enabled = NO;
    _embControl.enabled = NO;
    _enumerateButton.enabled = NO;
    
    _progressControl.progress = 0.0;
    
    _progressLabel.hidden = NO;
    _progressControl.hidden = NO;

    _running = true;

    [UIApplication sharedApplication].idleTimerDisabled = YES;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        NormalSurfaces* ans =
            NormalSurfaces::enumerate((regina::Triangulation<3>*)self.spec.parent,
                                          coords, which, regina::NS_ALG_DEFAULT, &_tracker);
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
                switch (_whichControl.selectedSegmentIndex) {
                    case 0: ans->setLabel([NSString stringWithFormat:@"%@ vertex surfaces",
                                           [Coordinates adjective:coords capitalise:YES]].UTF8String); break;
                    case 1: ans->setLabel([NSString stringWithFormat:@"%@ fundamental surfaces",
                                           [Coordinates adjective:coords capitalise:YES]].UTF8String); break;
                    default: ans->setLabel("Normal surfaces"); break;
                }

                [self.spec created:ans];
            }
            [self dismissViewControllerAnimated:YES completion:nil];
        });
    });
}

+ (void)initArrays {
    whichText = [NSArray arrayWithObjects:
                 @"Vertex surfaces (extreme rays)",
                 @"Fundamental surfaces (Hilbert basis)",
                 nil];
    coordText = [NSArray arrayWithObjects:
                 @"Normal coordinates: triangles and quadrilaterals",
                 @"Normal coordinates: quadrilaterals only",
                 @"Almost normal coordinates: triangles, quadrilaterals, octagons",
                 @"Almost normal coordinates: quadrilaterals and octagons only",
                 nil];
    embText = [NSArray arrayWithObjects:
               @"Properly embedded surfaces only",
               @"Include immersed and/or singular surfaces",
               nil];
}

@end
