
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

#import "SurfacesCompat.h"
#import "SurfacesViewController.h"
#import "surfaces/nnormalsurfacelist.h"

#define KEY_LAST_TYPE @"SurfacesCompatType"

static NSArray* typeText = @[@"Do surfaces have compatible quadrilateral and/or octagon types?",
                             @"Can surfaces be made globally disjoint?"];

@interface SurfacesCompat ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *type;
@property (weak, nonatomic) IBOutlet UILabel *typeExpln;
@property (weak, nonatomic) IBOutlet UIView *grid;
@end

@implementation SurfacesCompat

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    self.type.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TYPE];
    [self typeChanged:nil];

    [self reloadPacket];
}

- (void)reloadPacket
{
    [super reloadPacket];
    // TODO: Implement.
}

- (IBAction)typeChanged:(id)sender {
    self.typeExpln.text = typeText[self.type.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.type.selectedSegmentIndex forKey:KEY_LAST_TYPE];
    
    [self reloadPacket];
}

@end
