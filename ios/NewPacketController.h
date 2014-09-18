
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

#import <Foundation/Foundation.h>
#import "packet/packettype.h"

namespace regina {
    class NPacket;
}

@class PacketTreeController;

@interface NewPacketSpec : NSObject

@property (assign, nonatomic, readonly) regina::PacketType type;
@property (assign, nonatomic, readonly) regina::NPacket* parent;
@property (assign, nonatomic) BOOL viewOnCreation;

// Note: parent will be 0 if no suitable parent could be found.
// Test this via parent, or hasParentWithAlert.
- (id)initWithType:(regina::PacketType)type tree:(PacketTreeController*)tree;
+ (id)specWithType:(regina::PacketType)type tree:(PacketTreeController*)tree;

// PRE: The given parent is suitable and non-null.
- (id)initWithType:(regina::PacketType)type parent:(regina::NPacket*)parent;
+ (id)specWithType:(regina::PacketType)type parent:(regina::NPacket*)parent;

- (BOOL)hasParentWithAlert;

- (void)created:(regina::NPacket*)result;

@end

@interface NewPacketController : UIViewController

@property (strong, nonatomic) NewPacketSpec* spec;

@end
