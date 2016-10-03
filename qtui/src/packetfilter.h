
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file packetfilter.h
 *  \brief Provides a means of filtering desirable packets from a group.
 */

#ifndef __PACKETFILTER_H
#define __PACKETFILTER_H

#include "packet/packet.h"

/**
 * Represents a means by which packets can be either accepted or
 * rejected.  Different acceptance algorithms should correspond to
 * different subclasses of PacketFilter.
 */
class PacketFilter {
    public:
        /**
         * Destructor.
         */
        virtual ~PacketFilter();

        /**
         * Should the given packet be accepted according to this
         * particular acceptance algorithm?
         */
        virtual bool accept(regina::Packet* packet) = 0;
};

/**
 * A packet filter that accepts all packets.
 */
class AllPacketsFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::Packet* packet);
};

/**
 * A packet filter that accepts only packets that can be happily
 * separated from their parents.
 */
class StandaloneFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::Packet* packet);
};

/**
 * A packet filter that only accepts packets of a single fixed packet type.
 *
 * The template argument T must be one of the available packet types.
 * The acceptance test will be performed by calling
 * Packet::type() upon each packet being questioned.
 */
template <class T>
class SingleTypeFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::Packet* packet) {
            return (packet->type() == T::typeID);
        }
};

/**
 * A packet filter that only accepts packets of one of two fixed packet types.
 *
 * The template arguments S and T must each be one of the available packet
 * types.  The acceptance test will be performed by calling
 * Packet::type() upon each packet being questioned.
 */
template <class S, class T>
class TwoTypeFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::Packet* packet) {
            int type = packet->type();
            return (type == S::typeID || type == T::typeID);
        }
};

/**
 * A packet filter that only accepts packets of either a given packet type
 * or any of its subclasses.
 *
 * The template argument T must be one of the available packet types.
 * The acceptance test will be performed by calling dynamic_cast<T*>
 * upon each packet being questioned.
 */
template <class T>
class SubclassFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::Packet* packet) {
            return dynamic_cast<T*>(packet);
        }
};

inline PacketFilter::~PacketFilter() {
}

inline bool AllPacketsFilter::accept(regina::Packet*) {
    return true;
}

inline bool StandaloneFilter::accept(regina::Packet* packet) {
    return ! packet->dependsOnParent();
}

#endif

