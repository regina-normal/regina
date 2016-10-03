
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

#import <UIKit/UIKit.h>
#import "EditableTableViewController.h"
#import "ReginaDocument.h"
#import "packet/packettype.h"

namespace regina {
    class Packet;
}

@class NewPacketSpec;

@interface PacketTreeController : EditableTableViewController

@property (assign, nonatomic, readonly) regina::Packet* node;

- (void)newDocument;
- (void)openDocument:(ReginaDocument*)doc;
- (void)openSubtree:(regina::Packet*)p;

/**
 * Should only need to be called from the internal packet listener.
 */
- (void)refreshPackets;

/**
 * Safely does nothing if \a p is not in the list of children.
 * Must not pass \c null.
 * Refuses to select a container packet, but regardless will scroll
 * down to ensure that the packet is visible in the table.
 */
- (BOOL)selectPacket:(regina::Packet*)p;

- (void)newPacket:(regina::PacketType)type;

/**
 * Currently, will do nothing unless this involves a single push or a single pop.
 * Safe if dest is 0.
 */
- (void)navigateToPacket:(regina::Packet*)dest;

@end
