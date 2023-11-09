
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

#ifndef __REGINA_SWIFT_CONTAINER_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_CONTAINER_H
#endif

#include <sstream>
#include "packet/container.h"
#include "swift/packet.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a container packet.
 *
 * It can be assumed that such a packet is never null.
 */
struct SharedContainer {
    private:
        std::shared_ptr<Container> packet_;

    public:
        SharedContainer(const SharedContainer&) = default;
        SharedContainer(SharedContainer&&) = default;
        SharedContainer& operator = (const SharedContainer&) = default;
        SharedContainer& operator = (SharedContainer&&) = default;

        /**
         * Creates a wrapper to the given container packet.
         *
         * \pre The given packet is not null.
         */
        SharedContainer(std::shared_ptr<Container> packet) :
                packet_(std::move(packet)) {
        }

        /**
         * Creates a wrapper to the given packet, which is assumed to be a
         * container packet.
         *
         * \pre The given packet is a (non-null) container packet.
         */
        SharedContainer(SharedPacket packet) : packet_(
                std::static_pointer_cast<Container>(packet.sharedPtr())) {
        }

        std::shared_ptr<Container> sharedPtr() const {
            return packet_;
        }

        SharedPacket asPacket() const {
            return SharedPacket(packet_);
        }

        static SharedContainer make() {
            return std::make_shared<Container>();
        }
};

}

#endif
