
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file packet/text.h
 *  \brief Contains a packet representing a text string.
 */

#ifndef __REGINA_TEXT_H
#ifndef __DOXYGEN
#define __REGINA_TEXT_H
#endif

#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

class Text;

/**
 * A packet representing a text string.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the text content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * \ingroup packet
 */
class Text : public Packet {
    REGINA_PACKET(PACKET_TEXT, "Text")

    private:
        std::string text_;
            /**< The text string stored in this packet. */
    public:
        /**
         * Initialises the packet to the empty string.
         */
        Text() = default;

        /**
         * Initialises the packet to the given string.
         *
         * @param text the new value for the packet.
         */
        Text(std::string text);

        /**
         * Creates a new copy of the given text packet.
         *
         * Like all packet types, this only copies the text content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * it will not clone the given packet's children, and it will not
         * insert the new packet into any packet tree).
         */
        Text(const Text&) = default;

        /**
         * Sets this to be a copy of the given text packet.
         *
         * Like all packet types, this only copies the text content, not
         * the packet infrastructure (e.g., it will not copy the packet label,
         * or change this packet's location in any packet tree).
         *
         * @param src the text packet whose contents should be copied.
         * @return a reference to this packet.
         */
        Text& operator = (const Text& src);

        /**
         * Swaps the contents of this and the given text packet.
         *
         * Like all packet types, this only swaps the text content, not
         * the packet infrastructure (e.g., it will not swap packet labels,
         * or change either packet's location in any packet tree).
         *
         * @param other the text packet whose contents should be swapped
         * with this.
         */
        void swap(Text& other);

        /**
         * Returns the string stored in the packet.
         *
         * @return the stored string.
         */
        const std::string& text() const;

        /**
         * Sets the packet data to the given string.
         *
         * @param text the new value for the packet.
         */
        void setText(std::string text);

        void writeTextShort(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given text packets.
 *
 * This global routine simply calls Text::swap(); it is provided so that
 * Text meets the C++ Swappable requirements.
 *
 * @param a the first text packet whose contents should be swapped.
 * @param b the second text packet whose contents should be swapped.
 *
 * \ingroup packet
 */
void swap(Text& a, Text& b);

// Inline functions for Text

inline Text::Text(std::string text) : text_(std::move(text)) {
}

inline Text& Text::operator = (const Text& src) {
    ChangeEventSpan span(*this);
    text_ = src.text_;
    return *this;
}

inline void Text::swap(Text& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);
    text_.swap(other.text_);
}

inline const std::string& Text::text() const {
    return text_;
}

inline void Text::setText(std::string text) {
    if (text_ == text)
        return; // No change event fired.

    ChangeEventSpan span(*this);
    text_ = std::move(text);
}

inline void Text::writeTextShort(std::ostream& o) const {
    o << "Text packet";
}

inline void Text::writeTextLong(std::ostream& o) const {
    o << text_ << '\n';
}

inline std::shared_ptr<Packet> Text::internalClonePacket() const {
    return std::make_shared<Text>(text_);
}

inline void swap(Text& a, Text& b) {
    a.swap(b);
}

} // namespace regina

#endif

