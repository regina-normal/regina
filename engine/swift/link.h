
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

#ifndef __REGINA_SWIFT_LINK_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_LINK_H
#endif

#include <sstream>
#include "link/link.h"
#include "swift/packet.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a link packet.
 *
 * It can be assumed that such a packet is never null.
 */
struct SharedLink {
    private:
        std::shared_ptr<PacketOf<Link>> packet_;

    public:
        SharedLink(const SharedLink&) = default;
        SharedLink(SharedLink&&) = default;
        SharedLink& operator = (const SharedLink&) = default;
        SharedLink& operator = (SharedLink&&) = default;

        /**
         * Creates a wrapper to the given link packet.
         *
         * \pre The given packet is not null.
         */
        SharedLink(std::shared_ptr<PacketOf<Link>> packet) : packet_(
                std::move(packet)) {
        }

        /**
         * Creates a wrapper to the given packet, which is assumed to be a
         * link packet.
         *
         * \pre The given packet is a (non-null) link packet.
         */
        SharedLink(SharedPacket packet) : packet_(
                std::static_pointer_cast<PacketOf<Link>>(packet.sharedPtr())) {
        }

        /**
         * Moves the given link into a new packet.
         */
        SharedLink(Link link) : packet_(make_packet(std::move(link))) {
        }

        std::shared_ptr<Link> sharedPtr() const {
            return packet_;
        }

        SharedPacket asPacket() const {
            return SharedPacket(packet_);
        }
};

}

#endif
