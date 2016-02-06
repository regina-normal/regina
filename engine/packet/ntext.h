
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file packet/ntext.h
 *  \brief Contains a packet representing a text string.
 */

#ifndef __NTEXT_H
#ifndef __DOXYGEN
#define __NTEXT_H
#endif

#include "regina-core.h"
#include "packet/npacket.h"

namespace regina {

class NXMLPacketReader;
class NText;

/**
 * \weakgroup packet
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct PacketInfo<PACKET_TEXT> {
    typedef NText Class;
    inline static const char* name() {
        return "Text";
    }
};
#endif

/**
 * A packet representing a text string.
 */
class REGINA_API NText : public NPacket {
    REGINA_PACKET(NText, PACKET_TEXT)

    private:
        std::string text_;
            /**< The text string stored in this packet. */
    public:
        /**
         * Initialises the packet to the empty string.
         */
        NText();

        /**
         * Initialises the packet to the given string.
         *
         * @param newText the new value for the packet.
         */
        NText(const std::string& newText);

        /**
         * Initialises the packet to the given string.
         *
         * @param newText the new value for the packet.
         */
        NText(const char* newText);

        /**
         * Returns the string stored in the packet.
         *
         * @return the stored string.
         */
        const std::string& text() const;

        /**
         * Deprecated routine that returns the string stored in the packet.
         *
         * \deprecated This routine has been renamed to text().
         * See the text() documentation for further details.
         */
        REGINA_DEPRECATED const std::string& getText() const;

        /**
         * Sets the packet data to the given string.
         *
         * @param newText the new value for the packet.
         */
        void setText(const std::string& newText);

        /**
         * Sets the packet data to the given string.
         *
         * @param newText the new value for the packet.
         */
        void setText(const char* newText);

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* xmlReader(NPacket* parent,
            NXMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NText

inline NText::NText() {
}

inline NText::NText(const std::string& newText) : text_(newText) {
}

inline NText::NText(const char* newText) : text_(newText) {
}

inline const std::string& NText::text() const {
    return text_;
}

inline const std::string& NText::getText() const {
    return text_;
}

inline void NText::setText(const std::string& newText) {
    if (text_ == newText)
        return; // No change event fired.

    ChangeEventSpan span(this);
    text_ = newText;
}

inline void NText::setText(const char* newText) {
    if (text_ == newText)
        return; // No change event fired.

    ChangeEventSpan span(this);
    text_ = newText;
}

inline void NText::writeTextShort(std::ostream& o) const {
    o << "Text packet";
}

inline void NText::writeTextLong(std::ostream& o) const {
    o << text_ << '\n';
}

inline bool NText::dependsOnParent() const {
    return false;
}

inline NPacket* NText::internalClonePacket(NPacket*) const {
    return new NText(text_);
}

} // namespace regina

#endif

