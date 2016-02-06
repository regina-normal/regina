
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

#import "PacketListenerIOS.h"
#import "packet/npacket.h"
#import "packet/npacketlistener.h"

#pragma mark C++ wrapper

class NPacketListenerObjC : public regina::NPacketListener {
private:
    void* _object;
    regina::NPacket* _listenChildrenOf;
public:
    inline NPacketListenerObjC(void* object, regina::NPacket* listenChildrenOf) : _object(object), _listenChildrenOf(listenChildrenOf) {
    }
    
    inline void packetToBeChanged(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeChanged:)])
            [(__bridge id)_object packetToBeChanged:packet];
    }
    
    inline void packetWasChanged(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetWasChanged:)])
            [(__bridge id)_object packetWasChanged:packet];
    }
    
    inline void packetToBeRenamed(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeRenamed:)])
            [(__bridge id)_object packetToBeRenamed:packet];
    }
    
    inline void packetWasRenamed(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetWasRenamed:)])
            [(__bridge id)_object packetWasRenamed:packet];
    }
    
    inline void packetToBeDestroyed(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeDestroyed:)])
            [(__bridge id)_object packetToBeDestroyed:packet];
    }
    
    inline void childToBeAdded(regina::NPacket* packet, regina::NPacket* child) {
        if (packet == _listenChildrenOf)
            child->listen(this);
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeAddedTo:child:)])
            [(__bridge id)_object childToBeAddedTo:packet child:child];
    }
    
    inline void childWasAdded(regina::NPacket* packet, regina::NPacket* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasAddedTo:child:)])
            [(__bridge id)_object childWasAddedTo:packet child:child];
    }
    
    inline void childToBeRemoved(regina::NPacket* packet, regina::NPacket* child, bool inParentDestructor) {
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeRemovedFrom:child:inParentDestructor:)])
            [(__bridge id)_object childToBeRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
    }
    
    inline void childWasRemoved(regina::NPacket* packet, regina::NPacket* child, bool inParentDestructor) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasRemovedFrom:child:inParentDestructor:)])
            [(__bridge id)_object childWasRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
    }
    
    inline void childrenToBeReordered(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(childrenToBeReordered:)])
            [(__bridge id)_object childrenToBeReordered:packet];
    }
    
    inline void childrenWereReordered(regina::NPacket* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(childrenWereReordered:)])
            [(__bridge id)_object childrenWereReordered:packet];
    }

    inline void childToBeRenamed(regina::NPacket* packet, regina::NPacket* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeRenamed:child:)])
            [(__bridge id)_object childToBeRenamed:packet child:child];
    }
    
    inline void childWasRenamed(regina::NPacket* packet, regina::NPacket* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasRenamed:child:)])
            [(__bridge id)_object childWasRenamed:packet child:child];
    }
};

#pragma mark - Main interface

@interface PacketListenerIOS () {
    NPacketListenerObjC* _wrapper;
}
@end

@implementation PacketListenerIOS

- (id)initWithPacket:(regina::NPacket *)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren
{
    self = [super init];
    if (self) {
        _wrapper = new NPacketListenerObjC((__bridge void*)delegate, listenChildren ? packet : 0);
        packet->listen(_wrapper);

        if (listenChildren)
            for (regina::NPacket* p = packet->firstChild(); p; p = p->nextSibling())
                p->listen(_wrapper);
    }
    return self;
}

+ (id)listenerWithPacket:(regina::NPacket *)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren
{
    return [[PacketListenerIOS alloc] initWithPacket:packet delegate:delegate listenChildren:listenChildren];
}

- (void)permanentlyUnlisten
{
    // This implicitly calls packet->unlisten(_wrapper) on every packet that
    // we are listening to.
    delete _wrapper;
    _wrapper = 0;
}

- (void)dealloc
{
    delete _wrapper;
}

@end
