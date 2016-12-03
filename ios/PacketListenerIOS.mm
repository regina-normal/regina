
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
#import "packet/packet.h"
#import "packet/packetlistener.h"

#pragma mark C++ wrapper

class PacketListenerObjC : public regina::PacketListener {
private:
    void* _object;
    regina::Packet* _listenChildrenOf;
public:
    inline PacketListenerObjC(void* object, regina::Packet* listenChildrenOf) : _object(object), _listenChildrenOf(listenChildrenOf) {
    }
    
    inline void packetToBeChanged(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeChanged:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object packetToBeChanged:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object packetToBeChanged:packet];
                });
        }
    }
    
    inline void packetWasChanged(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetWasChanged:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object packetWasChanged:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object packetWasChanged:packet];
                });
        }
    }
    
    inline void packetToBeRenamed(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeRenamed:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object packetToBeRenamed:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object packetToBeRenamed:packet];
                });
        }
    }
    
    inline void packetWasRenamed(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetWasRenamed:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object packetWasRenamed:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object packetWasRenamed:packet];
                });
        }
    }
    
    inline void packetToBeDestroyed(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(packetToBeDestroyed:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object packetToBeDestroyed:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object packetToBeDestroyed:packet];
                });
        }
    }
    
    inline void childToBeAdded(regina::Packet* packet, regina::Packet* child) {
        if (packet == _listenChildrenOf)
            child->listen(this);
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeAddedTo:child:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childToBeAddedTo:packet child:child];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childToBeAddedTo:packet child:child];
                });
        }
    }
    
    inline void childWasAdded(regina::Packet* packet, regina::Packet* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasAddedTo:child:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childWasAddedTo:packet child:child];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childWasAddedTo:packet child:child];
                });
        }
    }
    
    inline void childToBeRemoved(regina::Packet* packet, regina::Packet* child, bool inParentDestructor) {
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeRemovedFrom:child:inParentDestructor:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childToBeRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childToBeRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
                });
        }
    }
    
    inline void childWasRemoved(regina::Packet* packet, regina::Packet* child, bool inParentDestructor) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasRemovedFrom:child:inParentDestructor:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childWasRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childWasRemovedFrom:packet child:child inParentDestructor:inParentDestructor];
                });
        }
    }
    
    inline void childrenToBeReordered(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(childrenToBeReordered:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childrenToBeReordered:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childrenToBeReordered:packet];
                });
        }
    }
    
    inline void childrenWereReordered(regina::Packet* packet) {
        if ([(__bridge id)_object respondsToSelector:@selector(childrenWereReordered:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childrenWereReordered:packet];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childrenWereReordered:packet];
                });
        }
    }

    inline void childToBeRenamed(regina::Packet* packet, regina::Packet* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childToBeRenamed:child:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childToBeRenamed:packet child:child];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childToBeRenamed:packet child:child];
                });
        }
    }
    
    inline void childWasRenamed(regina::Packet* packet, regina::Packet* child) {
        if ([(__bridge id)_object respondsToSelector:@selector(childWasRenamed:child:)]) {
            if ([NSThread isMainThread])
                [(__bridge id)_object childWasRenamed:packet child:child];
            else
                dispatch_async(dispatch_get_main_queue(), ^{
                    [(__bridge id)_object childWasRenamed:packet child:child];
                });
        }
    }
};

#pragma mark - Main interface

@interface PacketListenerIOS () {
    PacketListenerObjC* _wrapper;
}
@end

@implementation PacketListenerIOS

- (id)initWithPacket:(regina::Packet *)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren
{
    self = [super init];
    if (self) {
        _wrapper = new PacketListenerObjC((__bridge void*)delegate, listenChildren ? packet : 0);
        packet->listen(_wrapper);

        if (listenChildren)
            for (regina::Packet* p = packet->firstChild(); p; p = p->nextSibling())
                p->listen(_wrapper);
    }
    return self;
}

+ (id)listenerWithPacket:(regina::Packet *)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren
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
