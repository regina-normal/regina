
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

#import "DetailViewController.h"
#import "NewPacketController.h"
#import "PacketManagerIOS.h"
#import "PacketTreeController.h"
#import "packet/ncontainer.h"
#import "packet/npacket.h"
#import "packet/ntext.h"

@implementation PacketManagerIOS

+ (UIImage*)iconFor:(regina::NPacket *)p {
    if (! p->getTreeParent())
        return [UIImage imageNamed:@"Document"];

    switch (p->getPacketType()) {
        case regina::PACKET_ANGLESTRUCTURELIST:
            return [UIImage imageNamed:@"Angles"];
        case regina::PACKET_CONTAINER:
            return [UIImage imageNamed:@"Container"];
        case regina::PACKET_DIM2TRIANGULATION:
            return [UIImage imageNamed:@"Dim2Triangulation"];
        case regina::PACKET_NORMALSURFACELIST:
            return [UIImage imageNamed:@"Surfaces"];
        case regina::PACKET_PDF:
            return [UIImage imageNamed:@"PDF"];
        case regina::PACKET_SCRIPT:
            return [UIImage imageNamed:@"Script"];
        case regina::PACKET_SNAPPEATRIANGULATION:
            return [UIImage imageNamed:@"SnapPea"];
        case regina::PACKET_SURFACEFILTER:
            return [UIImage imageNamed:@"Filter"];
        case regina::PACKET_TEXT:
            return [UIImage imageNamed:@"Text"];
        case regina::PACKET_TRIANGULATION:
            return [UIImage imageNamed:@"Triangulation"];
        default:
            return nil;
    }
}

+ (NSString *)segueFor:(regina::NPacket *)p {
    switch (p->getPacketType()) {
        case regina::PACKET_PDF: return @"embedPDF";
        case regina::PACKET_SCRIPT: return @"embedScript";
        case regina::PACKET_TEXT: return @"embedText";
        case regina::PACKET_TRIANGULATION: return @"embedTriangulation";
        default: return @"embedDefault";
    }
}

+ (void)newPacket:(NewPacketSpec*)spec {
    switch (spec.type) {
        case regina::PACKET_CONTAINER:
        {
            // We can do this immediately, no input required.
            regina::NContainer* c = new regina::NContainer();
            c->setPacketLabel("Container");
            spec.tree.node->insertChildLast(c);
            [spec.tree viewPacket:c];
            break;
        }
        case regina::PACKET_TEXT:
        {
            // We can do this immediately, no input required.
            regina::NText* t = new regina::NText();
            t->setPacketLabel("Text");
            t->setText("Type your text here.");
            spec.tree.node->insertChildLast(t);
            [spec.tree viewPacket:t];
            break;
        }
        case regina::PACKET_TRIANGULATION:
            [spec.tree performSegueWithIdentifier:@"newTriangulation" sender:spec];
            break;
        case regina::PACKET_DIM2TRIANGULATION:
            [spec.tree performSegueWithIdentifier:@"newDim2Triangulation" sender:spec];
            break;
        case regina::PACKET_NORMALSURFACELIST:
            [spec.tree performSegueWithIdentifier:@"newSurfaces" sender:spec];
            break;
        case regina::PACKET_ANGLESTRUCTURELIST:
            [spec.tree performSegueWithIdentifier:@"newAngles" sender:spec];
            break;
        case regina::PACKET_SNAPPEATRIANGULATION:
            [spec.tree performSegueWithIdentifier:@"newSnapPea" sender:spec];
            break;
        case regina::PACKET_SURFACEFILTER:
            [spec.tree performSegueWithIdentifier:@"newFilter" sender:spec];
            break;
        case regina::PACKET_PDF:
        case regina::PACKET_SCRIPT:
            // We don't create these in the iOS version of Regina.
            break;
    }
}

@end
