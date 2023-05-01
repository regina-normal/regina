
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#import "triangulation/dim3.h"

#define KEY_LAST_TYPE @"NewSurfacesType"
#define KEY_LAST_NORMAL @"NewSurfacesNormal"
#define KEY_LAST_COORDS @"NewSurfacesCoordsV2"
#define KEY_LAST_EMB @"NewSurfacesEmb"

using regina::NormalSurfaces;

static NSArray* whichText;
static NSArray* normalText;
static NSArray* coordTextNormal;
static NSArray* coordTextAlmostNormal;
static NSArray* embText;

@interface NewSurfacesController () {
    bool _running;
    regina::ProgressTracker _tracker;
    NSArray* coordText;
}
@property (weak, nonatomic) IBOutlet UILabel *triangulation;
@property (weak, nonatomic) IBOutlet UISegmentedControl *whichControl;
@property (weak, nonatomic) IBOutlet UILabel *whichDesc;
@property (weak, nonatomic) IBOutlet UISegmentedControl *normalControl;
@property (weak, nonatomic) IBOutlet UILabel *normalDesc;
@property (weak, nonatomic) IBOutlet UISegmentedControl *coordControl;
@property (weak, nonatomic) IBOutlet UILabel *coordDesc;
@property (weak, nonatomic) IBOutlet UISegmentedControl *embControl;
@property (weak, nonatomic) IBOutlet UILabel *embDesc;
@property (weak, nonatomic) IBOutlet UIProgressView *progressControl;
@property (weak, nonatomic) IBOutlet UILabel *progressLabel;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *enumerateButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *cancelButton;
- (IBAction)whichChanged:(id)sender;
- (IBAction)normalChanged:(id)sender;
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
    self.normalControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_NORMAL];
    self.coordControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_COORDS];
    self.embControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_EMB];

    // Update the description labels.
    // Note that normalChanged *must* be called before coordChanged, since
    // normalChanged sets the pointer coordText (which coordChanged uses).
    [self whichChanged:nil];
    [self normalChanged:nil];
    [self coordChanged:nil];
    [self embChanged:nil];
}

- (IBAction)whichChanged:(id)sender {
    _whichDesc.text = whichText[_whichControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.whichControl.selectedSegmentIndex forKey:KEY_LAST_TYPE];
}

- (IBAction)normalChanged:(id)sender {
    _normalDesc.text = normalText[_normalControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.normalControl.selectedSegmentIndex forKey:KEY_LAST_NORMAL];

    switch (_normalControl.selectedSegmentIndex) {
        case 0:
            [_coordControl setTitle:@"Quad" forSegmentAtIndex:1];
            coordText = coordTextNormal;
            break;
        case 1:
            [_coordControl setTitle:@"Quad-oct" forSegmentAtIndex:1];
            coordText = coordTextAlmostNormal;
            break;
    }

    // The coordinate system description might need to change also.
    [self coordChanged:nil];
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
    bool almostNormal;
    
    bool broken = false;
    switch (_whichControl.selectedSegmentIndex) {
        case 0: which = regina::NS_VERTEX; break;
        case 1: which = regina::NS_FUNDAMENTAL; break;
        default: broken = true; break;
    }
    switch (_normalControl.selectedSegmentIndex) {
        case 0: almostNormal = false; break;
        case 1: almostNormal = true; break;
        default: broken = true; break;
    }
    switch (_embControl.selectedSegmentIndex) {
        case 0: which |= regina::NS_EMBEDDED_ONLY; break;
        case 1: which |= regina::NS_IMMERSED_SINGULAR; break;
        default: broken = true; break;
    }
    switch (_coordControl.selectedSegmentIndex) {
        case 0: coords = (almostNormal ? regina::NS_AN_STANDARD : regina::NS_STANDARD); break;
        case 1: coords = (almostNormal ? regina::NS_AN_QUAD_OCT : regina::NS_QUAD); break;
        case 2: coords = (almostNormal ? regina::NS_AN_QUAD_OCT_CLOSED : regina::NS_QUAD_CLOSED); break;
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

    regina::Triangulation<3>* tri = (regina::Triangulation<3>*)self.spec.parent;

    if ((coords == regina::NS_QUAD_CLOSED || coords == regina::NS_AN_QUAD_OCT_CLOSED) && ! (
            tri->countVertices() == 1 &&
            tri->vertex(0)->linkType() == regina::Vertex<3>::TORUS &&
            tri->isOriented())) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Selection Not Supported"
                                                        message:@"At present, closed quad and closed quad-oct coordinates are only available for oriented ideal triangulations with one torus cusp and no other boundary components or internal vertices."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    _whichControl.enabled = NO;
    _normalControl.enabled = NO;
    _coordControl.enabled = NO;
    _embControl.enabled = NO;
    _enumerateButton.enabled = NO;
    
    _progressControl.progress = 0.0;
    
    _progressLabel.hidden = NO;
    _progressControl.hidden = NO;

    _running = true;

    [UIApplication sharedApplication].idleTimerDisabled = YES;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        NormalSurfaces* ans = nullptr;
        try {
            ans = new NormalSurfaces(tri, coords, which, regina::NS_ALG_DEFAULT, &_tracker);
        } catch (const regina::NoMatchingEquations&) {
        }
        while (! _tracker.isFinished()) {
            if (_tracker.percentChanged()) {
                // This operation blocks until the UI is updated:
                [self performSelectorOnMainThread:@selector(updateProgress) withObject:nil waitUntilDone:YES];
            }
            [NSThread sleepForTimeInterval:0.1];
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [UIApplication sharedApplication].idleTimerDisabled = NO;

            if (! ans) {
                UIAlertView *alert;
                if (coords == regina::NS_QUAD_CLOSED || coords == regina::NS_AN_QUAD_OCT_CLOSED) {
                    alert = [[UIAlertView alloc] initWithTitle:@"Enumeration Failed"
                                                                    message:@"I could not complete the normal surface enumeration. This could be because SnapPea was unable to construct the slope equations, or because it tried to retriangulate when doing so. Please report this to the Regina developers."
                                                                   delegate:nil
                                                          cancelButtonTitle:@"Close"
                                                          otherButtonTitles:nil];
                } else {
                    alert = [[UIAlertView alloc] initWithTitle:@"Enumeration Failed"
                                                                    message:@"I could not complete the normal surface enumeration.  Please report this to the Regina developers."
                                                                   delegate:nil
                                                          cancelButtonTitle:@"Close"
                                                          otherButtonTitles:nil];
                }
                [alert show];
            } else if (_tracker.isCancelled()) {
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
                    default: // Should never happen:
                        ans->setLabel("Normal surfaces"); break;
                }

                tri->insertChildLast(ans);
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
    normalText = [NSArray arrayWithObjects:
                  @"Normal surfaces: only triangles and/or quadrilaterals",
                  @"Almost normal surfaces: one non-zero octagon type allowed",
                  nil];
    coordTextNormal = [NSArray arrayWithObjects:
                 @"Standard coordinates: triangles and quadrilaterals",
                 @"Quad coordinates: quadrilaterals only",
                 @"Closed quad coordinates: closed (non-spun) surfaces only",
                 nil];
    coordTextAlmostNormal = [NSArray arrayWithObjects:
                 @"Standard coordinates: triangles, quadrilaterals and octagons",
                 @"Quad-oct coordinates: quadrilaterals and octagons only",
                 @"Closed quad-oct coordinates: closed (non-spun) surfaces only",
                 nil];

    embText = [NSArray arrayWithObjects:
               @"Properly embedded surfaces only",
               @"Include immersed and/or singular surfaces",
               nil];
}

@end
