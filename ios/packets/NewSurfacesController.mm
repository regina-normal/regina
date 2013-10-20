
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

#import "NewSurfacesController.h"
#import "progress/nprogresstracker.h"
#import "surfaces/nnormalsurfacelist.h"

NSArray* whichText;
NSArray* coordText;
NSArray* embText;

// TODO: Put together a proper coordinates class, and so on.

@interface NewSurfacesController () {
    bool _running;
    regina::NProgressTracker _tracker;
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
    
    if (self.createBeneath)
        _triangulation.text = [NSString stringWithUTF8String:self.createBeneath->getPacketLabel().c_str()];
    
    // TODO: Check for invalid parent.
    // Update the description labels.
    [self whichChanged:nil];
    [self coordChanged:nil];
    [self embChanged:nil];
}

- (IBAction)whichChanged:(id)sender {
    _whichDesc.text = whichText[_whichControl.selectedSegmentIndex];
}

- (IBAction)coordChanged:(id)sender {
    _coordDesc.text = coordText[_coordControl.selectedSegmentIndex];
}

- (IBAction)embChanged:(id)sender {
    _embDesc.text = embText[_embControl.selectedSegmentIndex];
}

- (IBAction)cancel:(id)sender {
    // TODO: Get this working.
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)enumerate:(id)sender {
    // TODO: Make this test more robust against GUI changes.
    if (_coordControl.selectedSegmentIndex >= 2 && _embControl.selectedSegmentIndex == 1) {
        // TODO: Bail.
        return;
    }
    
    _running = true;
    
    _whichControl.enabled = NO;
    _coordControl.enabled = NO;
    _embControl.enabled = NO;
    _enumerateButton.enabled = NO;
    
    _progressControl.progress = 0.0;
    
    _progressLabel.hidden = NO;
    _progressControl.hidden = NO;

    // TODO: Run!
    // TODO: Fix arguments.
    regina::NNormalSurfaceList* ans = regina::NNormalSurfaceList::enumerate(                                                            (regina::NTriangulation*)self.createBeneath, regina::NS_STANDARD,
        regina::NS_VERTEX, regina::NS_ALG_DEFAULT, &_tracker);
    
    while (! _tracker.isFinished()) {
        if (_tracker.percentChanged())
            _progressControl.progress = _tracker.percent();
        [NSThread sleepForTimeInterval:0.1];
    }
    
    ans->setPacketLabel("Normal surfaces");
    [self dismissViewControllerAnimated:YES completion:nil];
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
