
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ntext.h
 *  \brief Contains a packet representing a text string.
 */

#ifndef __NTEXT_H
#ifndef __DOXYGEN
#define __NTEXT_H
#endif

#include "packet/npacket.h"

namespace regina {

class NXMLPacketReader;

/**
 * \weakgroup packet
 * @{
 */

/**
 * A packet representing a text string.
 */
class NText : public NPacket {
    private:
        std::string text;
            /**< The text string stored in this packet. */
    public:
        static const int packetType;

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
        const std::string& getText() const;

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

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual void writePacket(NFile& out) const;
        static NText* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NText

inline NText::NText() {
}

inline NText::NText(const std::string& newText) : text(newText) {
}

inline NText::NText(const char* newText) : text(newText) {
}

inline const std::string& NText::getText() const {
    return text;
}

inline void NText::setText(const std::string& newText) {
    text = newText;
    fireChangedEvent();
}

inline void NText::setText(const char* newText) {
    text = newText;
    fireChangedEvent();
}

inline void NText::writeTextShort(std::ostream& o) const {
    o << "Text packet";
}

inline void NText::writeTextLong(std::ostream& o) const {
    o << text << '\n';
}

inline bool NText::dependsOnParent() const {
    return false;
}

inline NPacket* NText::internalClonePacket(NPacket*) const {
    return new NText(text);
}

} // namespace regina

#endif

