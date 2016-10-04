
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

#import "SnapPeaViewController.h"
#import "TextHelper.h"
#import "snappea/snappeatriangulation.h"

@implementation SnapPeaViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setSelectedImages:@[@"Tab-Cusps-Bold",
                              @"Tab-Gluings-Bold",
                              @"Tab-Skeleton-Bold",
                              @"Tab-Graph-Bold",
                              [NSNull null],
                              @"Tab-Composition-Bold",
                              @"Tab-Recognition-Bold"]];
    [self registerDefaultKey:@"ViewSnapPeaTab"];
}

- (void)updateHeader:(UILabel *)summary volume:(UILabel *)volume solnType:(UILabel *)solnType
{
    if (self.packet->isNull()) {
        summary.text = @"Null triangulation (no SnapPea data)";
        volume.text = @"";
        solnType.text = @"";
        return;
    }
    if (self.packet->isEmpty()) {
        summary.text = @"Empty triangulation";
        volume.text = @"";
        solnType.text = @"";
        return;
    }

    if (! self.packet->isValid()) {
        summary.attributedText = [TextHelper badString:@"Invalid triangulation"];
        volume.text = @"";
        solnType.text = @"";
        return;
    }

    NSMutableString* msg;

    if (self.packet->isOrientable())
        msg = [NSMutableString stringWithString:@"Orientable, "];
    else
        msg = [NSMutableString stringWithString:@"Non-orientable, "];

    unsigned nFilled = self.packet->countFilledCusps();
    unsigned nComplete = self.packet->countCompleteCusps();

    if (nFilled + nComplete == 1) {
        [msg appendString:@"1 cusp, "];
        if (nFilled)
            [msg appendString:@"filled"];
        else
            [msg appendString:@"complete"];
    } else if (nFilled == 0) {
        [msg appendFormat:@"%d cusps, all complete", nComplete];
    } else if (nComplete == 0) {
        [msg appendFormat:@"%d cusps, all filled", nFilled];
    } else {
        [msg appendFormat:@"%d cusps, %d filled", (nFilled + nComplete), nFilled];
    }
    summary.text = msg;

    // Volume and solution type:
    switch (self.packet->solutionType()) {
        case regina::SnapPeaTriangulation::not_attempted:
            volume.text = @"Solution not attempted";
            solnType.text= @"";
            break;
        case regina::SnapPeaTriangulation::geometric_solution:
            if (self.packet->volumeZero())
                volume.text = @"Volume approximately zero";
            else
                volume.text = [NSString stringWithFormat:@"Volume: %lf", self.packet->volume()];
            solnType.text = @"Tetrahedra positively oriented";
            break;
        case regina::SnapPeaTriangulation::nongeometric_solution:
            if (self.packet->volumeZero())
                volume.text = @"Volume approximately zero";
            else
                volume.text = [NSString stringWithFormat:@"Volume: %lf", self.packet->volume()];
            solnType.text = @"Contains flat or negative tetrahedra";
            break;
        case regina::SnapPeaTriangulation::flat_solution:
            if (self.packet->volumeZero())
                volume.text = @"Volume approximately zero";
            else
                volume.text = [NSString stringWithFormat:@"Volume: %lf", self.packet->volume()];
            solnType.text = @"All tetrahedra flat";
            break;
        case regina::SnapPeaTriangulation::degenerate_solution:
            if (self.packet->volumeZero())
                volume.text = @"Volume approximately zero";
            else
                volume.text = [NSString stringWithFormat:@"Volume: %lf", self.packet->volume()];
            solnType.text = @"Contains degenerate tetrahedra";
            break;
        case regina::SnapPeaTriangulation::other_solution:
            volume.text = @"Unrecognised solution type";
            solnType.text = @"";
            break;
        case regina::SnapPeaTriangulation::no_solution:
            volume.text = @"No solution found";
            solnType.text = @"";
            break;
        case regina::SnapPeaTriangulation::externally_computed:
            volume.text = @"Externally computed";
            solnType.text = @"";
            break;
        default:
            volume.attributedText = [TextHelper badString:@"Invalid solution type"];
            solnType.text = @"";
            break;
    }
}

@end
