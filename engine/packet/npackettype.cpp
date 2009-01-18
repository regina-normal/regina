
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "packet/packetregistry.h"

namespace regina {

// Define a task for each REGISTER_PACKET to perform.
#define REGISTER_PACKET(class, type, name)         \
    int class::getPacketType() const {             \
        return type;                               \
    }                                              \
    const int class::packetType = type;            \
    std::string class::getPacketTypeName() const { \
        return name;                               \
    }

// Import all the REGISTER_PACKET lines.
#define __PACKET_REGISTRY_BODY
#include "packet/packetregistry.h"

// Tidy up.
#undef REGISTER_PACKET
#undef __PACKET_REGISTRY_BODY

} // namespace regina
