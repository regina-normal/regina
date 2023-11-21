
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

#ifndef __REGINA_SWIFT_PACKET_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_PACKET_H
#endif

#include <sstream>
#include "packet/packet.h"
#include "utilities/memstream.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a packet, but where the packet
 * member functions are accessible directly (without dereferencing).
 *
 * The _only_ member functions that are safe to call on a null packet are
 * isNull() and id().
 */
struct SharedPacket {
    private:
        std::shared_ptr<Packet> packet_;

    public:
        SharedPacket() = default;
        SharedPacket(const SharedPacket&) = default;
        SharedPacket(SharedPacket&&) = default;
        SharedPacket& operator = (const SharedPacket&) = default;
        SharedPacket& operator = (SharedPacket&&) = default;

        /**
         * Creates a wrapper to the given packet, which may be null.
         */
        SharedPacket(std::shared_ptr<Packet> packet) :
                packet_(std::move(packet)) {
        }

        std::shared_ptr<Packet> sharedPtr() const {
            return packet_;
        }

        // TODO: How to access bool() in swift?
        bool isNull() const {
            return ! packet_;
        }

        PacketType type() const {
            return packet_->type();
        }

        std::string typeName() const {
            return packet_->typeName();
        }

        std::string humanLabel() const {
            return packet_->humanLabel();
        }

        std::string adornedLabel(const std::string& adornment) const {
            return packet_->adornedLabel(adornment);
        }

        void setLabel(std::string label) {
            packet_->setLabel(std::move(label));
        }

        SharedPacket firstChild() const {
            return packet_->firstChild();
        }

        SharedPacket lastChild() const {
            return packet_->lastChild();
        }

        SharedPacket prevSibling() const {
            return packet_->prevSibling();
        }

        SharedPacket nextSibling() const {
            return packet_->nextSibling();
        }

        size_t countChildren() const {
            return packet_->countChildren();
        }

        void prepend(SharedPacket child) {
            packet_->prepend(std::move(child.packet_));
        }

        void append(SharedPacket child) {
            packet_->append(std::move(child.packet_));
        }

        /**
         * Reads an entire packet tree from the given buffer in memory.
         *
         * On error, this returns a null packet.
         */
        static SharedPacket open(const char* buffer, size_t size) {
            mem_istream s(buffer, buffer + size);
            return regina::open(s);
        }

        /**
         * Saves to a string (not a file), in an uncompressed XML format.
         *
         * TODO: Can we refine this to invoke fewer deep copies of the entire
         * packet tree on the route from C++ to Swift to filesystem?
         */
        std::string save() const {
            std::ostringstream s;
            packet_->save(s, false);
            // Once we have C++20 we can use std::move(s).str() instead.
            return s.str();
        }

        /**
         * Return an integer that uniquely idenfies this packet in the engine.
         * This can be used to make packet Identifiable in Swift (for example).
         *
         * For now we hard-code the return type as int64_t, since in practice
         * the SwiftUI will only be built on 64-bit architectures (and there
         * is a static assert to ensure that 64 bits exactly what we need).
         *
         * If this is a null packet, then the ID returned will be 0.
         */
        int64_t id() const {
            static_assert(sizeof(Packet*) == sizeof(int64_t));
            return (packet_ ? reinterpret_cast<int64_t>(packet_.get()) : 0);
        }
};

}

#endif
