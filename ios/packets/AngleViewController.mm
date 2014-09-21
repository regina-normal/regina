
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

#import "AngleViewController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "angle/nanglestructure.h"
#import "angle/nanglestructurelist.h"
#import "triangulation/ntriangulation.h"

static UIColor* yesColour = [UIColor colorWithRed:0.0 green:0.5 blue:0.0 alpha:1.0];
static UIColor* noColour = [UIColor colorWithRed:0.5 green:0.0 blue:0.0 alpha:1.0];

#pragma mark - Angle structure view controller

@interface AngleViewController () <PacketDelegate> {
    PacketListenerIOS* _triListener;
}
@property (weak, nonatomic) IBOutlet UILabel *countAndType;
@property (weak, nonatomic) IBOutlet UILabel *span;
@property (weak, nonatomic) IBOutlet UIButton *triangulation;
@property (weak, nonatomic) IBOutlet UIScrollView *angles;
@end

@implementation AngleViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    regina::NAngleStructureList* a = static_cast<regina::NAngleStructureList*>(self.packet);

    unsigned long count = a->getNumberOfStructures();
    if (a->isTautOnly()) {
        if (count == 0)
            self.countAndType.text = @"No taut angle structures";
        else if (count == 1)
            self.countAndType.text = @"1 taut angle structure";
        else
            self.countAndType.text = [NSString stringWithFormat:@"%ld taut angle structures", count];
    } else {
        if (count == 0)
            self.countAndType.text = @"No vertex angle structures";
        else if (count == 1)
            self.countAndType.text = @"1 vertex angle structure";
        else
            self.countAndType.text = [NSString stringWithFormat:@"%ld vertex angle structures", count];
    }

    NSAttributedString* strictText = (a->spansStrict() ?
                                      [[NSAttributedString alloc]
                                       initWithString:@"Strict"
                                       attributes:@{NSForegroundColorAttributeName: yesColour}] :
                                      [[NSAttributedString alloc]
                                       initWithString:@"No strict"
                                       attributes:@{NSForegroundColorAttributeName: noColour}]);
    NSAttributedString* tautText = (a->spansTaut() ?
                                    [[NSAttributedString alloc]
                                     initWithString:@"Taut"
                                     attributes:@{NSForegroundColorAttributeName: yesColour}] :
                                    [[NSAttributedString alloc]
                                     initWithString:@"No taut"
                                     attributes:@{NSForegroundColorAttributeName: noColour}]);

    NSMutableAttributedString* spanText = [[NSMutableAttributedString alloc] initWithString:@"Span includes: "];
    [spanText appendAttributedString:strictText];
    [spanText appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
    [spanText appendAttributedString:tautText];
    self.span.attributedText = spanText;

    [self updateTriangulationButton];
    // Continue to update the button text if the triangulation is renamed.
    _triListener = [PacketListenerIOS listenerWithPacket:a->getTriangulation() delegate:self listenChildren:NO];

    // TODO: Set up the table.
}

- (void)dealloc
{
    [_triListener permanentlyUnlisten];
}

- (IBAction)openTriangulation:(id)sender {
    regina::NPacket* show = static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation();
    [[ReginaHelper tree] navigateToPacket:show->getTreeParent()];

    // We can't select this normal surface list in the tree, since the pop action is animated and
    // will not have finished by this point.
    [ReginaHelper viewPacket:static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation()];
}

- (void)updateTriangulationButton
{
    regina::NPacket* tri = static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation();
    NSString* triName = [NSString stringWithUTF8String:tri->getPacketLabel().c_str()];
    if (triName.length == 0)
        triName = @"(Unnamed)";
    [self.triangulation setTitle:triName forState:UIControlStateNormal];
}

#pragma mark - Packet listener

- (void)packetWasRenamed:(regina::NPacket *)packet
{
    if (packet == static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation())
        [self updateTriangulationButton];
}

@end
