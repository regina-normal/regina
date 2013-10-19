
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

#import "PacketManagerIOS.h"
#import "packet/npacket.h"

@implementation PacketManagerIOS

+ (UIImage*)iconFor:(regina::NPacket *)p {
    switch (p->getPacketType()) {
        case regina::PACKET_ANGLESTRUCTURELIST:
            return [UIImage imageNamed:@"Angles"];
            break;
        case regina::PACKET_CONTAINER:
            return [UIImage imageNamed:@"Container"];
            break;
        case regina::PACKET_DIM2TRIANGULATION:
            return [UIImage imageNamed:@"Dim2Triangulation"];
            break;
        case regina::PACKET_NORMALSURFACELIST:
            return [UIImage imageNamed:@"Surfaces"];
            break;
        case regina::PACKET_PDF:
            return [UIImage imageNamed:@"PDF"];
            break;
        case regina::PACKET_SCRIPT:
            return [UIImage imageNamed:@"Script"];
            break;
        case regina::PACKET_SURFACEFILTER:
            return [UIImage imageNamed:@"Filter"];
            break;
        case regina::PACKET_TEXT:
            return [UIImage imageNamed:@"Text"];
            break;
        case regina::PACKET_TRIANGULATION:
            return [UIImage imageNamed:@"Triangulation"];
            break;
        default:
            return nil;
    }
}

+ (NSString *)segueFor:(regina::NPacket *)p {
    switch (p->getPacketType()) {
        case regina::PACKET_TEXT: return @"embedText";
        case regina::PACKET_TRIANGULATION: return @"embedTriangulation";
        default: return @"embedDefault";
    }
}

@end
