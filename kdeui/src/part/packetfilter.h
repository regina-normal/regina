
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file packetfilter.h
 *  \brief Provides a means of filtering desirable packets from a group.
 */

#ifndef __PACKETFILTER_H
#define __PACKETFILTER_H

#include "packet/npacket.h"

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
        virtual bool accept(regina::NPacket* packet) = 0;
};

/**
 * A packet filter that accepts all packets.
 */
class AllPacketsFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::NPacket* packet);
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
        virtual bool accept(regina::NPacket* packet);
};

/**
 * A packet filter that only accepts packets of a single fixed packet type.
 *
 * The template argument T must be one of the available packet types.
 * The acceptance test will be performed by calling
 * NPacket::getPacketType() upon each packet being questioned.
 */
template <class T>
class SingleTypeFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::NPacket* packet) {
            return (packet->getPacketType() == T::packetType);
        }
};

/**
 * A packet filter that only accepts packets of one of two fixed packet types.
 *
 * The template arguments S and T must each be one of the available packet
 * types.  The acceptance test will be performed by calling
 * NPacket::getPacketType() upon each packet being questioned.
 */
template <class S, class T>
class TwoTypeFilter : public PacketFilter {
    public:
        /**
         * PacketFilter overrides.
         */
        virtual bool accept(regina::NPacket* packet) {
            int type = packet->getPacketType();
            return (type == S::packetType || type == T::packetType);
        }
};

inline PacketFilter::~PacketFilter() {
}

inline bool AllPacketsFilter::accept(regina::NPacket*) {
    return true;
}

inline bool StandaloneFilter::accept(regina::NPacket* packet) {
    return ! packet->dependsOnParent();
}

#endif

