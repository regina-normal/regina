
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
#import "ReginaHelper.h"
#import "HyperViewController.h"
#import "hypersurface/normalhypersurfaces.h"
#import "triangulation/dim4.h"

#pragma mark - Hypersurfaces view controller

@implementation HypersurfacesViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setSelectedImages:@[@"Tab-Summary-Bold",
                              @"Tab-Coords-Bold",
                              @"Tab-Compat-Bold",
                              @"Tab-Matching-Bold"]];
    [self registerDefaultKey:@"ViewHypersurfacesTab"];
}

- (void)updateHeader:(UILabel *)summary coords:(UILabel *)coords tri:(UIButton *)tri
{
    regina::HyperList which = self.packet->which();

    NSString *sEmb, *sType;

    if (which.has(regina::HS_EMBEDDED_ONLY))
        sEmb = @"embedded";
    else if (which.has(regina::HS_IMMERSED_SINGULAR))
        sEmb = @"embedded / immersed / singular";
    else
        sEmb = @"unknown";

    if (which.has(regina::HS_VERTEX))
        sType = @"vertex";
    else if (which.has(regina::HS_FUNDAMENTAL))
        sType = @"fundamental";
    else if (which.has(regina::HS_CUSTOM))
        sType = @"custom";
    else if (which.has(regina::HS_LEGACY))
        sType = @"legacy";
    else
        sType = @"unknown";

    if (self.packet->size() == 0)
        summary.text = [NSString stringWithFormat:@"No %@, %@ hypersurfaces", sType, sEmb];
    else if (self.packet->size() == 1)
        summary.text = [NSString stringWithFormat:@"1 %@, %@ hypersurface", sType, sEmb];
    else
        summary.text = [NSString stringWithFormat:@"%ld %@, %@ hypersurfaces",
                        self.packet->size(), sType, sEmb];

    coords.text = [NSString stringWithFormat:@"Enumerated in %@ coordinates", [HyperCoordinates name:self.packet->coords() capitalise:NO]];
    [self updateTriangulationButton:tri];
}

- (void)updateTriangulationButton:(UIButton*)button
{
    const regina::Triangulation<4>& tri = self.packet->triangulation();
    NSString* triName;
    if (tri.isReadOnlySnapshot())
        triName = @"(Private Copy)";
    else {
        triName = [NSString stringWithUTF8String:tri.label().c_str()];
        if (triName.length == 0)
            triName = @"(Unnamed)";
    }

    // Regarding the triangulation button:
    // If we just call setTitle:, then then button flashes annoyingly the first time we switch to each tab.
    // The following two-step method fixes this.
    button.titleLabel.text = triName;
    [button setTitle:triName forState:UIControlStateNormal];
}

@end

#pragma mark - Hypersurfaces tab

@interface HypersurfacesTab () <PacketDelegate> {
    PacketListenerIOS* _triListener;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *coords;
@property (weak, nonatomic) IBOutlet UIButton *tri;
@end

@implementation HypersurfacesTab

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<HypersurfacesViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.packet = self.viewer.packet;

    [_triListener permanentlyUnlisten];

    const regina::Triangulation<4>& tri = self.packet->triangulation();
    if (! tri.isReadOnlySnapshot())
        _triListener = [PacketListenerIOS listenerWithPacket:std::addressof(tri) delegate:self listenChildren:NO];
}

- (void)dealloc
{
    [_triListener permanentlyUnlisten];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header coords:self.coords tri:self.tri];
}

- (IBAction)openTriangulation:(id)sender {
    const regina::Triangulation<4>& tri = self.packet->triangulation();
    if (tri.isReadOnlySnapshot()) {
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Triangulation is Private"
                                                                       message:@"This list stores its own private copy of the triangulation, since the original has changed or been deleted.\n\nWould you like me to make a new copy that you can view and edit?\n\nThis list will continue to use its own private copy, so you can edit or delete your new copy as you please."
                                                                        preferredStyle:UIAlertControllerStyleAlert];
                [alert addAction:[UIAlertAction actionWithTitle:@"Create Copy"
                                                          style:UIAlertActionStyleDefault
                                                        handler:^(UIAlertAction* action) {
                                                            regina::Triangulation<4>* copy = new regina::Triangulation<4>(tri);
                                                            copy->setLabel(self.packet->adornedLabel("Triangulation"));
                                                            self.packet->insertChildLast(copy);

                                                            [ReginaHelper viewPacket:copy];
                                                        }]];
                [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                          style:UIAlertActionStyleCancel
                                                        handler:^(UIAlertAction * action) {}]];
                [self presentViewController:alert animated:YES completion:nil];
    } else {
        [ReginaHelper viewPacket:std::addressof(tri)];
    }
}

- (void)packetWasRenamed:(regina::Packet&)packet
{
    // Assume it is the underlying triangulation.
    [self updateTriangulationButton:self.tri];
}

- (void)packetWasChanged:(regina::Packet&)packet
{
    if (packet != std::addressof(self.packet->triangulation())) {
        // Our list has switched to use a local snapshot of the triangulation.
        // The triangulation will be read-only from now on.
        [self permanentlyUnlisten];
        [self updateTriangulationButton:self.tri];
    }
}

- (void)packetBeingDestroyed:(regina::PacketShell)packet
{
    // Assume it is the underlying triangulation.
    [self updateTriangulationButton:self.tri];
}

@end
