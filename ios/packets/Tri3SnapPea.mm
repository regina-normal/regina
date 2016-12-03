
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
#import "TextHelper.h"
#import "Tri3ViewController.h"
#import "Tri3SnapPea.h"
#import "snappea/snappeatriangulation.h"
#import "triangulation/dim3.h"

@interface Tri3SnapPea ()
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UIButton *convert;
@property (weak, nonatomic) IBOutlet UILabel *convertLabel;
@property (weak, nonatomic) IBOutlet UIButton *convertIcon;

@property (strong, nonatomic) Tri3ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<3>* packet;
@end

@implementation Tri3SnapPea

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri3ViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;
    [self reloadPacket];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header lockIcon:self.lockIcon];

    regina::SnapPeaTriangulation snappeaTri(*self.packet);

    if (snappeaTri.isNull()) {
        self.solnType.attributedText = self.volume.attributedText = [TextHelper dimString:@"Unavailable for this triangulation"];

        self.convert.hidden = YES;
        self.convertLabel.hidden = YES;
        self.convertIcon.hidden = YES;
    } else {
        switch (snappeaTri.solutionType()) {
            case regina::SnapPeaTriangulation::not_attempted:
                self.solnType.text = @"Not attempted"; break;
            case regina::SnapPeaTriangulation::geometric_solution:
                self.solnType.text = @"Tetrahedra positively oriented"; break;
            case regina::SnapPeaTriangulation::nongeometric_solution:
                self.solnType.text = @"Contains flat or negative tetrahedra"; break;
            case regina::SnapPeaTriangulation::flat_solution:
                self.solnType.text = @"All tetrahedra flat"; break;
            case regina::SnapPeaTriangulation::degenerate_solution:
                self.solnType.text = @"Contains degenerate tetrahedra"; break;
            case regina::SnapPeaTriangulation::other_solution:
                self.solnType.text = @"Unrecognised solution type"; break;
            case regina::SnapPeaTriangulation::no_solution:
                self.solnType.text = @"No solution found"; break;
            case regina::SnapPeaTriangulation::externally_computed:
                self.solnType.text = @"Externally computed"; break;
            default:
                self.solnType.text = @"ERROR (invalid solution type)"; break;
        }

        int places;
        double ans = snappeaTri.volume(places);

        if (snappeaTri.volumeZero()) {
            // Zero is within the margin of error, and this margin of
            // error is small.  Report it as zero, with the exact result
            // beneath.
            self.volume.text = [NSString stringWithFormat:@"Possibly zero\n(calculated %.9lf,\nest. %d places accuracy)",
                                ans, places];
        } else {
            self.volume.text = [NSString stringWithFormat:@"%.9lf\n(est. %d places accuracy)",
                                ans, places];
        }

        self.convert.hidden = NO;
        self.convertLabel.hidden = NO;
        self.convertIcon.hidden = NO;
    }
}

- (IBAction)toSnapPea:(id)sender {
    regina::SnapPeaTriangulation* s = new regina::SnapPeaTriangulation(*self.packet);
    s->setLabel(self.packet->label());

    self.packet->insertChildLast(s);
    [ReginaHelper viewPacket:s];
}

@end
