
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

namespace regina {
    class NPacket;
}

/**
 * A protocol that mirrors Regina's C++ NPacketListener interface.
 *
 * Objects that wish to respond to changes to the packet tree should
 * implement the relevant methods from this protocol.
 */
@protocol PacketDelegate <NSObject>
@optional
- (void)packetToBeChanged:(regina::NPacket*)packet;
- (void)packetWasChanged:(regina::NPacket*)packet;
- (void)packetToBeRenamed:(regina::NPacket*)packet;
- (void)packetWasRenamed:(regina::NPacket*)packet;
- (void)packetToBeDestroyed:(regina::NPacket*)packet;
- (void)childToBeAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child;
- (void)childWasAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child;
- (void)childToBeRemovedFrom:(regina::NPacket*)packet child:(regina::NPacket*)child inParentDestructor:(bool)d;
- (void)childWasRemovedFrom:(regina::NPacket*)packet child:(regina::NPacket*)child inParentDestructor:(bool)d;
- (void)childrenToBeReordered:(regina::NPacket*)packet;
- (void)childrenWereReordered:(regina::NPacket*)packet;
- (void)childToBeRenamed:(regina::NPacket*)packet child:(regina::NPacket*)child;
- (void)childWasRenamed:(regina::NPacket*)packet child:(regina::NPacket*)child;
@end

/**
 * An object that listens for changes to a packet tree and responds
 * by calling the corresponding methods for a given delegate object.
 *
 * This class essentially acts as a bridge between the C++ NPacketListener code
 * and the Objective-C protocol mechanisms.
 *
 * \warning This class does not manage lifespans at all.  You must ensure that,
 * if the listener might outlive the delegate object, then the method
 * permanentlyUnlisten: is called before the delegate is destroyed.
 */
@interface PacketListenerIOS : NSObject

/**
 * Initialises a new packet listener, listening to the given packet and
 * sending events to the given delegate object.  See
 * listenerWithPacket:delegate:listenChildren: for further details.
 */
- (id)initWithPacket:(regina::NPacket*)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren;

/**
 * Creates a new packet listener, listening to the given packet and
 * sending events to the given delegate object.
 *
 * @param listenChildren If \c YES, then this will always listen to not only
 * the given packet but also all of its immediate children (but not descendants at a
 * deeper level in the packet tree).  This behaviour will be maintained automatically as
 * packets are added and/or removed from the packet tree.
 */
+ (id)listenerWithPacket:(regina::NPacket*)packet delegate:(id<PacketDelegate>)delegate listenChildren:(BOOL)listenChildren;

/**
 * Unregisters this listener from all packets that it is currently listening to.
 * This operation cannot be undone.
 */
- (void)permanentlyUnlisten;

@end
