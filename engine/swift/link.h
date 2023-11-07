
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
 * A structure that holds a raw pointer to a crossing.
 *
 * This pointer is allowed to be null.
 */
struct CrossingPtr {
    private:
        Crossing* data_;

    public:
        CrossingPtr() : data_(nullptr) {}
        CrossingPtr(Crossing* data) : data_(data) {}
        CrossingPtr(const CrossingPtr&) = default;
        CrossingPtr& operator = (const CrossingPtr&) = default;

        bool operator == (const CrossingPtr& rhs) const {
            return data_ == rhs.data_;
        }

        bool isNull() const { return ! data_; }
        size_t index() const { return data_->index(); }
        int sign() const { return data_->sign(); }
};

struct StrandRefAlt {
    private:
        StrandRef data_;

    public:
        StrandRefAlt() = default;
        StrandRefAlt(StrandRef data) : data_(data) {}
        StrandRefAlt(const StrandRefAlt&) = default;
        StrandRefAlt& operator = (const StrandRefAlt&) = default;

        bool operator == (const StrandRefAlt& rhs) const {
            return data_ == rhs.data_;
        }

        CrossingPtr crossing() const { return data_.crossing(); }
        size_t crossingIndex() const { return data_.crossing()->index(); }
        int strand() const { return data_.strand(); }
        StrandRefAlt next() const { return data_.next(); }
        StrandRefAlt prev() const { return data_.prev(); }
        bool isNull() const { return ! data_; }
        ssize_t id() const { return data_.id(); }
};

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

        Link held() const {
            return *packet_;
        }

        CrossingPtr crossing(size_t index) const {
            return packet_->crossing(index);
        }

        size_t countComponents() const {
            return packet_->countComponents();
        }

        StrandRefAlt component(size_t index) const {
            return packet_->component(index);
        }

        Laurent<Integer> bracket() const {
            return packet_->bracket();
        }

        Laurent<Integer> jones() const {
            return packet_->jones();
        }

        Laurent2<Integer> homflyAZ() const {
            return packet_->homflyAZ();
        }

        Laurent2<Integer> homflyLM() const {
            return packet_->homflyLM();
        }
};

}

#endif
