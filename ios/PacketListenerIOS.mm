
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

#import "PacketListenerIOS.h"
#import "packet/npacket.h"
#import "packet/npacketlistener.h"

#pragma mark C++ wrapper

class NPacketListenerObjC : public regina::NPacketListener {
private:
    void* _object;
public:
    inline NPacketListenerObjC(void* object) : _object(object) {
    }
    
    inline void packetToBeChanged(regina::NPacket* packet) {
        [(__bridge id)_object packetToBeChanged:packet];
    }
    
    inline void packetWasChanged(regina::NPacket* packet) {
        [(__bridge id)_object packetWasChanged:packet];
    }
    
    inline void packetToBeRenamed(regina::NPacket* packet) {
        [(__bridge id)_object packetToBeRenamed:packet];
    }
    
    inline void packetWasRenamed(regina::NPacket* packet) {
        [(__bridge id)_object packetWasRenamed:packet];
    }
    
    inline void packetToBeDestroyed(regina::NPacket* packet) {
        [(__bridge id)_object packetToBeDestroyed:packet];
    }
    
    inline void childToBeAdded(regina::NPacket* packet, regina::NPacket* child) {
        [(__bridge id)_object childToBeAddedTo:packet child:child];
    }
    
    inline void childWasAdded(regina::NPacket* packet, regina::NPacket* child) {
        [(__bridge id)_object childWasAddedTo:packet child:child];
    }
    
    inline void childToBeRemoved(regina::NPacket* packet, regina::NPacket* child, bool inParentDestructor) {
        [(__bridge id)_object childToBeRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
    }
    
    inline void childWasRemoved(regina::NPacket* packet, regina::NPacket* child, bool inParentDestructor) {
        [(__bridge id)_object childWasRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
    }
    
    inline void childrenToBeReordered(regina::NPacket* packet) {
        [(__bridge id)_object childrenToBeReordered:packet];
    }
    
    inline void childrenWereReordered(regina::NPacket* packet) {
        [(__bridge id)_object childrenWereReordered:packet];
    }
};

#pragma mark - Main interface

@interface PacketListenerIOS () {
    NPacketListenerObjC* _wrapper;
}
@end

@implementation PacketListenerIOS

- (id)init {
    self = [super init];
    if (self)
        _wrapper = new NPacketListenerObjC((__bridge void*)self);
    return self;
}

- (void)dealloc {
    delete _wrapper;
}

- (void)listen:(regina::NPacket*)packet {
    packet->listen(_wrapper);
}
- (void)unlisten:(regina::NPacket*)packet {
    packet->unlisten(_wrapper);
}
- (void)unlistenFromAllPackets {
    _wrapper->unregisterFromAllPackets();
}

#pragma mark Empty callbacks

- (void)packetToBeChanged:(regina::NPacket*)packet {
}
- (void)packetWasChanged:(regina::NPacket*)packet {
}
- (void)packetToBeRenamed:(regina::NPacket*)packet {
}
- (void)packetWasRenamed:(regina::NPacket*)packet {
}
- (void)packetToBeDestroyed:(regina::NPacket*)packet {
}
- (void)childToBeAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child {
}
- (void)childWasAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child {
}
- (void)childToBeRemovedFrom:(regina::NPacket*)packet child:(regina::NPacket*)child inParentDestructor:(bool)d {
}
- (void)childWasRemovedFrom:(regina::NPacket*)packet child:(regina::NPacket*)child inParentDestructor:(bool)d {
}
- (void)childrenToBeReordered:(regina::NPacket*)packet {
}
- (void)childrenWereReordered:(regina::NPacket*)packet {
}

@end
