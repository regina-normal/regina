
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

/*! \file ncontainer.h
 *  \brief Contains a packet whose entire life purpose is to contain
 *  other packets.
 */

#ifndef __NCONTAINER_H
#ifndef __DOXYGEN
#define __NCONTAINER_H
#endif

#include "packet/npacket.h"

namespace regina {

class NXMLPacketReader;

/**
 * \weakgroup packet
 * @{
 */

/**
 * A packet that simply contains other packets.  Such
 * a packet contains no real data.
 */
class NContainer : public NPacket {
    public:
        static const int packetType;

        /**
         * Default constructor.
         */
        NContainer();

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;

        virtual void writeTextShort(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual void writePacket(NFile& out) const;
        static NContainer* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;
    
    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NContainer

inline NContainer::NContainer() {
}

inline void NContainer::writeTextShort(std::ostream& o) const {
    o << "Container";
}

inline void NContainer::writePacket(NFile&) const {
}

inline bool NContainer::dependsOnParent() const {
    return false;
}

inline NPacket* NContainer::internalClonePacket(NPacket*) const {
    return new NContainer();
}

inline void NContainer::writeXMLPacketData(std::ostream&) const {
}

} // namespace regina

#endif

