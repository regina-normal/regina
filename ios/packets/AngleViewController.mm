
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

#pragma mark - Angle structure view controller

@interface AngleViewController ()
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

    // TODO: Use attributed text for this.
    if (a->spansStrict()) {
        if (a->spansTaut())
            self.span.text = @"Span includes: Strict, Taut";
        else
            self.span.text = @"Span includes: Strict, NO Taut";
    } else {
        if (a->spansTaut())
            self.span.text = @"Span includes: NO Strict, Taut";
        else
            self.span.text = @"Span includes: NO Strict, NO Taut";
    }

    // TODO: Listen for triangulation renames.
    NSString* triName = [NSString stringWithUTF8String:a->getTriangulation()->getPacketLabel().c_str()];
    [self.triangulation setTitle:triName forState:UIControlStateNormal];

    // TODO: Set up the table.
}

- (IBAction)openTriangulation:(id)sender {
    regina::NPacket* show = static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation();
    PacketTreeController* tree = [ReginaHelper tree];
    if (tree.node == show)
        [tree.navigationController popViewControllerAnimated:YES]; // TODO: Generalise.

    // We can't select this normal surface list in the tree, since the pop action is animated and
    // will not have finished by this point.
    [ReginaHelper viewPacket:static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation()];
}

@end
