
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

#ifndef __REGINA_SWIFT_TEXT_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_TEXT_H
#endif

#include <sstream>
#include "packet/text.h"
#include "swift/packet.h"

namespace regina {

/**
 * A structure that holds a shared pointer to a text packet.
 *
 * It can be assumed that such a packet is never null.
 */
struct SharedText {
    private:
        std::shared_ptr<Text> packet_;

    public:
        SharedText(const SharedText&) = default;
        SharedText(SharedText&&) = default;
        SharedText& operator = (const SharedText&) = default;
        SharedText& operator = (SharedText&&) = default;

        /**
         * Creates a wrapper to the given text packet.
         *
         * \pre The given packet is not null.
         */
        SharedText(std::shared_ptr<Text> packet) :
                packet_(std::move(packet)) {
        }

        /**
         * Creates a wrapper to the given packet, which is assumed to be a
         * text packet.
         *
         * \pre The given packet is a (non-null) text packet.
         */
        SharedText(SharedPacket packet) : packet_(
                std::static_pointer_cast<Text>(packet.sharedPtr())) {
        }

        std::shared_ptr<Text> sharedPtr() const {
            return packet_;
        }

        SharedPacket asPacket() const {
            return SharedPacket(packet_);
        }

        std::string text() const {
            return packet_->text();
        }

        void setText(std::string text) {
            packet_->setText(std::move(text));
        }

        static SharedText make() {
            return std::make_shared<Text>();
        }
};

}

#endif
