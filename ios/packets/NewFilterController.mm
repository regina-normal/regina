
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

#import "NewFilterController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "surfaces/surfacefilter.h"

#define KEY_LAST_TYPE @"NewFilterType"

static NSArray* whichText;

@interface NewFilterController ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *whichControl;
@property (weak, nonatomic) IBOutlet UILabel *whichDesc;
- (IBAction)whichChanged:(id)sender;
- (IBAction)cancel:(id)sender;
- (IBAction)create:(id)sender;
+ (void)initArrays;
@end

@implementation NewFilterController

- (void)viewDidLoad {
    if (! whichText)
        [NewFilterController initArrays];

    self.whichControl.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TYPE];
    
    // Update the description labels.
    [self whichChanged:nil];
}

- (IBAction)whichChanged:(id)sender {
    self.whichDesc.text = whichText[self.whichControl.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.whichControl.selectedSegmentIndex forKey:KEY_LAST_TYPE];
}

- (IBAction)cancel:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)create:(id)sender {
    regina::Packet* ans;
    switch (self.whichControl.selectedSegmentIndex) {
        case 0:
            ans = new regina::SurfaceFilterProperties;
            ans->setLabel("Filter");
            break;
        case 1:
            ans = new regina::SurfaceFilterCombination;
            ans->setLabel("Combination filter");
            break;
        default:
            return;
    }
    self.spec.parent->insertChildLast(ans);
    [self.spec created:ans];
    [self dismissViewControllerAnimated:YES completion:nil];
}

+ (void)initArrays {
    whichText = [NSArray arrayWithObjects:
                 @"Filter normal surfaces by their properties, such as orientability or Euler characteristic",
                 @"Build a more complex filter by combining simpler filters, using boolean AND / OR",
                 nil];
}

@end
