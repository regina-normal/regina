
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file packet/packetlistener.h
 *  \brief Deals with objects that can listen for packet events.
 */

#ifndef __PACKETLISTENER_H
#ifndef __DOXYGEN
#define __PACKETLISTENER_H
#endif

#include <set>
#include "regina-core.h"

namespace regina {

class Packet;

/**
 * \weakgroup packet
 * @{
 */

/**
 * An object that can be registered to listen for packet events.
 *
 * A packet listener can be registered to listen for events on a
 * packet by calling Packet::listen().
 *
 * Each time that one of the events listed in this class occurs,
 * the packet will call the appropriate routine for all registered
 * packet listeners.
 *
 * These events come in future/past pairs: packetToBeChanged() and
 * packetWasChanged(), childToBeAdded() and childWasAdded(), and so on.
 * These event pairs are mutually exclusive: any event will
 * cause at most one pair of routines to be called for each
 * (packet, listener) pair.  For instance, if a packet is renamed then
 * packetToBeRenamed() and packetWasRenamed() will be called but
 * packetToBeChanged() and packetWasChanged() will not.
 *
 * As a special case, when a packet is destroyed there is only the future
 * event packetToBeDestroyed() with no matching "past" event, since \e after
 * the packet has been destroyed the set of listeners is no longer available.
 *
 * No guarantees are made as to the order in which the different packet
 * listeners are notified of an event.
 *
 * When a listener is destroyed, it is automatically unregistered
 * from any packets to which it is currently listening.  Similarly, when
 * a packet is destroyed all listeners are automatically unregistered.
 *
 * \warning At the time of writing (admittedly long ago now), Qt has only
 * limited support for multithreading.  When working with an existing packet
 * tree in a new thread (not the main thread), the \e only modification that
 * you may make is to insert new packets.  Modifications of any other type
 * (such as changing, renaming, deleting or reordering existing packets)
 * could lead to a crash within Qt or Xlib when running the GUI.
 * Of course, a new thread may create, modify and delete its own temporary
 * packet trees as it chooses (and it may in fact insert them into a
 * pre-existing packet tree once all modifications are completed).
 * Assuming these restrictions are respected, packet listeners may assume
 * that no routines other than childWasAdded() will be called from a non-main
 * thread.
 *
 * \ifacespython Not present.
 */
class REGINA_API PacketListener {
    private:
        std::set<Packet*> packets;
            /**< The set of packets upon which this object is currently
                 listening. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Destroys this listener.  This listener will be unregistered
         * from any packets to which it is currently listening.
         */
        virtual ~PacketListener();

        /*@}*/
        /**
         * \name Packet Listener Interface
         */
        /*@{*/

        /**
         * Unregisters this listener from any packets to which it is
         * currently listening.
         */
        void unregisterFromAllPackets();

        /**
         * Called before the contents of the packet are to be changed.
         * Once the contents are changed, packetWasChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetToBeChanged(Packet* packet);
        /**
         * Called after the contents of the packet have been changed.
         * Before the contents are changed, packetToBeChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetWasChanged(Packet* packet);
        /**
         * Called before the packet label or tags are to be changed.
         * Once the label or tags are changed, packetWasRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @see childToBeRenamed()
         */
        virtual void packetToBeRenamed(Packet* packet);
        /**
         * Called after the packet label or tags have been changed.
         * Before the label or tags are changed, packetToBeRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @see childWasRenamed()
         */
        virtual void packetWasRenamed(Packet* packet);
        /**
         * Called before the packet is about to be destroyed.  Note that
         * there is no matching function called \e after the
         * packet is destroyed, since the set of listeners will no
         * longer be available at that stage.
         *
         * When an entire packet subtree is to be destroyed, child packets
         * will notify their listeners of the impending destruction
         * before parent packets will.
         *
         * Note that the packet will forcibly unregister this listener
         * immediately \e before packetToBeDestroyed() is called, to avoid
         * any unpleasant consequences if this listener should also try to
         * unregister itself.  This means that, by the time this routine is
         * called, this listener will no longer be registered with the
         * packet in question (and any attempt to unregister it again
         * will be harmless).
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetToBeDestroyed(Packet* packet);
        /**
         * Called before a child packet is to be inserted directly beneath
         * the packet.
         * Once the child is removed, childWasAdded() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet to be added.
         */
        virtual void childToBeAdded(Packet* packet, Packet* child);
        /**
         * Called after a child packet has been inserted directly beneath
         * the packet.
         * Before this child is added, childToBeAdded() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet that was added.
         */
        virtual void childWasAdded(Packet* packet, Packet* child);
        /**
         * Called before a child packet is to be removed from directly beneath
         * the packet.  Note that the child packet may be about to be
         * destroyed (although this destruction will not have happened yet).
         * Once the child is removed, childWasRemoved() will be
         * called also.
         *
         * Note also that this packet (the parent) may have already
         * entered its destructor (which removes and destroys all child
         * packets as a matter of course).  In this situation it may be
         * unsafe to query or update this packet, and so the third argument
         * \a inParentDestructor is provided to indicate such a situation.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet to be removed.
         * @param inParentDestructor set to \c true if the parent packet
         * is in fact being destroyed, and the child was simply removed
         * as part of the standard subtree destruction.
         */
        virtual void childToBeRemoved(Packet* packet, Packet* child,
            bool inParentDestructor);
        /**
         * Called after a child packet has been removed from directly beneath
         * the packet.  Note that the child packet may be about to be
         * destroyed (although this destruction will not have happened yet).
         * Before this child is removed, childToBeRemoved() will be
         * called also.
         *
         * Note also that this packet (the parent) may have already
         * entered its destructor (which removes and destroys all child
         * packets as a matter of course).  In this situation it may be
         * unsafe to query or update this packet, and so the third argument
         * \a inParentDestructor is provided to indicate such a situation.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet that was removed.
         * @param inParentDestructor set to \c true if the parent packet
         * is in fact being destroyed, and the child was simply removed
         * as part of the standard subtree destruction.
         */
        virtual void childWasRemoved(Packet* packet, Packet* child,
            bool inParentDestructor);
        /**
         * Called before the child packets directly beneath the packet
         * are to be reordered.
         * Once the reordering is done, childrenWereReordered() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void childrenToBeReordered(Packet* packet);

        /**
         * Called after the child packets directly beneath the packet
         * have been reordered.
         * Before this reordering is done, childrenToBeReordered() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void childrenWereReordered(Packet* packet);
        /**
         * Called before one of this packet's immediate children has its
         * label or tags changed.
         * Before this change, childToBeRenamed() will be called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet to be renamed.
         * @see packetToBeRenamed()
         */
        virtual void childToBeRenamed(Packet* packet, Packet* child);
        /**
         * Called after one of this packet's immediate children has its
         * label or tags changed.
         * Before this change, childToBeRenamed() will be called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         * @param child the child packet that was renamed.
         * @see packetWasRenamed()
         */
        virtual void childWasRenamed(Packet* packet, Packet* child);

        /*@}*/

    /**
     * Allow packets to automatically deregister listeners as they are
     * destroyed.
     */
    friend class Packet;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPacketListener has now been renamed to PacketListener.
 */
REGINA_DEPRECATED typedef PacketListener NPacketListener;

/*@}*/

// Inline functions for PacketListener

inline void PacketListener::packetToBeChanged(Packet*) {
}

inline void PacketListener::packetWasChanged(Packet*) {
}

inline void PacketListener::packetToBeRenamed(Packet*) {
}

inline void PacketListener::packetWasRenamed(Packet*) {
}

inline void PacketListener::packetToBeDestroyed(Packet*) {
}

inline void PacketListener::childToBeAdded(Packet*, Packet*) {
}

inline void PacketListener::childWasAdded(Packet*, Packet*) {
}

inline void PacketListener::childToBeRemoved(Packet*, Packet*, bool) {
}

inline void PacketListener::childWasRemoved(Packet*, Packet*, bool) {
}

inline void PacketListener::childrenToBeReordered(Packet*) {
}

inline void PacketListener::childrenWereReordered(Packet*) {
}

inline void PacketListener::childToBeRenamed(Packet*, Packet*) {
}

inline void PacketListener::childWasRenamed(Packet*, Packet*) {
}

} // namespace regina

#endif

