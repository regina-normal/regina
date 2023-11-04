
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

/*! \file swift/packet.h
 *  \brief Provides glue to make packet-related classes and functions
 *  available to Swift.
 *
 *  The specific problems that we aim to address here are:
 *
 *  - Swift/C++ interoperability appears to offer only limited support
 *    for std::shared_ptr, and in particular does not seem able to access the
 *    members of the pointee.
 *
 *  - Swift/C++ appears to struggle with C++ inheritance trees that use
 *    abstract and/or non-copyable base classes (such as regina::Packet).
 *
 *  It is entirely possible that the Swift/C++ has clean solutions to these
 *  problems, in which case Ben would _really_ love an email to this effect.
 */

#ifndef __REGINA_SWIFT_PACKET_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_PACKET_H
#endif

#include <sstream>
#include "packet/container.h"
#include "utilities/memstream.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a packet, but where the packet
 * member functions are accessible directly (without dereferencing).
 */
struct SharedPacket {
    private:
        std::shared_ptr<Packet> packet_;

    public:
        SharedPacket(std::shared_ptr<Packet> packet) :
                packet_(std::move(packet)) {
        }

        // TODO: How to access bool() in swift?
        bool isNull() const {
            return ! packet_;
        }

        PacketType type() const {
            return packet_->type();
        }

        std::string humanLabel() const {
            return packet_->humanLabel();
        }

        void setLabel(std::string label) {
            packet_->setLabel(std::move(label));
        }

        SharedPacket firstChild() const {
            return packet_->firstChild();
        }

        SharedPacket nextSibling() const {
            return packet_->nextSibling();
        }

        size_t countChildren() const {
            return packet_->countChildren();
        }

        void prepend(SharedPacket child) {
            packet_->prepend(child.packet_);
        }

        void append(SharedPacket child) {
            packet_->append(child.packet_);
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
            return s.str();
        }

        /**
         * Return an integer that uniquely idenfies this packet in the engine.
         * This can be used to make packet Identifiable in Swift (for example).
         *
         * For now we hard-code the return type as int64_t, since in practice
         * the SwiftUI will only be built on 64-bit architectures (and there
         * is a static assert to ensure that 64 bits exactly what we need).
         */
        int64_t id() const {
            static_assert(sizeof(Packet*) == sizeof(int64_t));
            return reinterpret_cast<int64_t>(packet_.get());
        }

        static SharedPacket makeContainer() {
            return SharedPacket(std::make_shared<Container>());
        }
};

}

#endif
