
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file packet/npacketlistener.h
 *  \brief Deals with objects that can listen for packet events.
 */

#ifndef __NPACKETLISTENER_H
#ifndef __DOXYGEN
#define __NPACKETLISTENER_H
#endif

#include <set>
#include "regina-core.h"

namespace regina {

class NPacket;

/**
 * \weakgroup packet
 * @{
 */

/**
 * An object that can be registered to listen for packet events.
 *
 * A packet listener can be registered to listen for events on a
 * packet by calling NPacket::listen().
 *
 * Each time that one of the events listed in this class occurs,
 * the packet will call the appropriate routine for all registered
 * packet listeners.
 *
 * These events come in future/past pairs: packetToBeChanged() and
 * packetWasChanged(), childToBeAdded() and childWasAdded(), and so on.
 * These event pairs are mutually exclusive: any event will
 * cause at most one pair of routines to be called for each listener.  For
 * instance, if a packet is renamed then packetToBeRenamed() and
 * packetWasRenamed() will be called but packetToBeChanged() and
 * packetWasChanged() will not.
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
 * When using multiple threads, there are restrictions upon what any thread
 * other than the main thread may do.  If these restrictions are properly
 * adhered to, packet listeners may assume that no routines other than
 * childWasAdded() will be called from a non-main thread.  Of course it is
 * up to the multithreading code to ensure that these restrictions are in
 * fact met; see the NThread class notes for further information.
 *
 * \warning If the multithreading restrictions noted above are \e not
 * adhered to, this can result in the GUI crashing within either Qt or
 * Xlib.  Again, see the NThread class notes for further information.
 *
 * \ifacespython Not present.
 */
class REGINA_API NPacketListener {
    private:
        std::set<NPacket*> packets;
            /**< The set of packets upon which this object is currently
                 listening. */

    public:
        /**
         * Destroys this listener.  This listener will be unregistered
         * from any packets to which it is currently listening.
         */
        virtual ~NPacketListener();

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
        virtual void packetToBeChanged(NPacket* packet);
        /**
         * Called after the contents of the packet have been changed.
         * Before the contents are changed, packetToBeChanged() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetWasChanged(NPacket* packet);
        /**
         * Called before the packet label or tags are to be changed.
         * Once the label or tags are changed, packetWasRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetToBeRenamed(NPacket* packet);
        /**
         * Called after the packet label or tags have been changed.
         * Before the label or tags are changed, packetToBeRenamed() will be
         * called also.
         *
         * The default implementation of this routine is to do nothing.
         *
         * @param packet the packet being listened to.
         */
        virtual void packetWasRenamed(NPacket* packet);
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
        virtual void packetToBeDestroyed(NPacket* packet);
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
        virtual void childToBeAdded(NPacket* packet, NPacket* child);
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
        virtual void childWasAdded(NPacket* packet, NPacket* child);
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
        virtual void childToBeRemoved(NPacket* packet, NPacket* child,
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
        virtual void childWasRemoved(NPacket* packet, NPacket* child,
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
        virtual void childrenToBeReordered(NPacket* packet);

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
        virtual void childrenWereReordered(NPacket* packet);

    /**
     * Allow packets to automatically deregister listeners as they are
     * destroyed.
     */
    friend class NPacket;
};

/*@}*/

// Inline functions for NPacketListener

inline void NPacketListener::packetToBeChanged(NPacket*) {
}

inline void NPacketListener::packetWasChanged(NPacket*) {
}

inline void NPacketListener::packetToBeRenamed(NPacket*) {
}

inline void NPacketListener::packetWasRenamed(NPacket*) {
}

inline void NPacketListener::packetToBeDestroyed(NPacket*) {
}

inline void NPacketListener::childToBeAdded(NPacket*, NPacket*) {
}

inline void NPacketListener::childWasAdded(NPacket*, NPacket*) {
}

inline void NPacketListener::childToBeRemoved(NPacket*, NPacket*, bool) {
}

inline void NPacketListener::childWasRemoved(NPacket*, NPacket*, bool) {
}

inline void NPacketListener::childrenToBeReordered(NPacket*) {
}

inline void NPacketListener::childrenWereReordered(NPacket*) {
}

} // namespace regina

#endif

