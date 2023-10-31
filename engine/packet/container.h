
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file packet/container.h
 *  \brief Contains a packet whose entire life purpose is to contain
 *  other packets.
 */

#ifndef __REGINA_CONTAINER_H
#ifndef __DOXYGEN
#define __REGINA_CONTAINER_H
#endif

#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

class Container;

/**
 * A packet that simply contains other packets.  Such
 * a packet contains no real data.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps, but only
 * for consistency with the other packet types.  Such copy/swap operations
 * are pointless for container packets since they do not copy/swap the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet tree,
 * or event listeners), and containers have no "real" content of their own.
 *
 * \ingroup packet
 */
class Container : public Packet {
    REGINA_PACKET(PacketType::Container, "Container")

    public:
        /**
         * Default constructor.
         */
        Container() = default;

        /**
         * Constructs a new container with the given packet label.
         *
         * This constructor is (for example) helpful when you are
         * building a complex packet tree to save to a Regina data file,
         * and you are using containers to organise the data in this tree.
         *
         * \param label the packet label for this new container.
         */
        Container(const std::string& label);

        /**
         * Copy constructor that does nothing.
         *
         * This is only here for consistency with the other packet types.
         * Like all packet types, this copy constructor does not copy any of
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         */
        Container(const Container&) = default;

        /**
         * Copy assignment operator that does nothing.
         *
         * This is only here for consistency with the other packet types.
         * Like all packet types, this assignment operator does not copy any of
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * \return a reference to this packet.
         */
        Container& operator = (const Container&) = default;

        /**
         * Swap function that does nothing.
         *
         * This is only here for consistency with the other packet types.
         * Like all packet types, this operation does not swap any of
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         */
        void swap(Container&) {}

        void writeTextShort(std::ostream& out) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swap function for container packets that does nothing.
 *
 * This is only here for consistency with the other packet types.
 * For container packets, the swap operation does nothing since containers
 * have no "real" content of their own.  See the member function
 * Container::swap() for further explanation.
 *
 * \ingroup packet
 */
inline void swap(Container&, Container&) {}

// Inline functions for Container

inline Container::Container(const std::string& label) {
    setLabel(label);
}

inline void Container::writeTextShort(std::ostream& o) const {
    if (! firstChild())
        o << "Empty container";
    else {
        size_t c = countChildren();
        size_t d = countDescendants();
        o << "Container with "
            << c << (c == 1 ? " child" : " children") << ", "
            << d << (d == 1 ? " descendant" : " descendants");
    }
}

inline std::shared_ptr<Packet> Container::internalClonePacket() const {
    return std::make_shared<Container>();
}

inline void Container::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    writeXMLHeader(out, "container", format, anon, refs, true);
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "container", format);
}

} // namespace regina

#endif

