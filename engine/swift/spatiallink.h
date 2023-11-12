
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_SWIFT_SPATIALLINK_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_SPATIALLINK_H
#endif

#include <sstream>
#include "link/spatiallink.h"
#include "swift/packet.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a spatial link packet.
 *
 * It can be assumed that such a packet is never null.
 */
struct SharedSpatialLink {
    private:
        std::shared_ptr<PacketOf<SpatialLink>> packet_;
        int distinguisher_ = 0; /**< See modified() for why this is here. */

    public:
        SharedSpatialLink(const SharedSpatialLink&) = default;
        SharedSpatialLink(SharedSpatialLink&&) = default;
        SharedSpatialLink& operator = (const SharedSpatialLink&) = default;
        SharedSpatialLink& operator = (SharedSpatialLink&&) = default;

        /**
         * Creates a wrapper to the given spatial link packet.
         *
         * \pre The given packet is not null.
         */
        SharedSpatialLink(std::shared_ptr<PacketOf<SpatialLink>> packet) :
                packet_(std::move(packet)) {
        }

        /**
         * Creates a wrapper to the given packet, which is assumed to be a
         * spatial link packet.
         *
         * \pre The given packet is a (non-null) spatial link packet.
         */
        SharedSpatialLink(SharedPacket packet) : packet_(
                std::static_pointer_cast<PacketOf<SpatialLink>>(
                    packet.sharedPtr())) {
        }

        /**
         * Moves the given spatial link into a new packet.
         */
        SharedSpatialLink(SpatialLink link) :
                packet_(make_packet(std::move(link))) {
        }

        std::shared_ptr<SpatialLink> sharedPtr() const {
            return packet_;
        }

        SharedPacket asPacket() const {
            return SharedPacket(packet_);
        }

        /**
         * The heldCopy() function has two purposes:
         * - it returns a SpatialLink, thus giving access to the full
         *   SpatialLink API;
         * - it returns a deep copy of the packet's data, which is useful
         *   when you need a stable snapshot of an object in a multithreaded
         *   scenario.
         */
        SpatialLink heldCopy() const {
            return *packet_;
        }

        /**
         * Returns a new wrapper around the same packet.  In Swift, you can
         * set `wrapper = wrapper.modified()` to convince SwiftUI that the
         * packet has been modified (e.g., to force a refresh of the UI).
         *
         * Note: without the \a distinguisher data member, this does not work
         * (i.e., SwiftUI does _not_ refresh the UI after such a call).
         * With \a distinguisher it does work, even though \a distinguisher
         * holds the same value (the original plan was to increment it in
         * this function).  I have no idea what criterion SwiftUI is using here.
         */
        SharedSpatialLink modified() const {
            return SharedSpatialLink(packet_);
        }

        void refine() {
            packet_->refine();
        }

        void refine(int sub) {
            packet_->refine(sub);
        }
};

}

#endif
