
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file nxmltrireader.h
 *  \brief Deals with parsing XML data for triangulation packets.
 */

#ifndef __NXMLTRIREADER_H
#ifndef __DOXYGEN
#define __NXMLTRIREADER_H
#endif

#include "packet/nxmlpacketreader.h"
#include "triangulation/ntriangulation.h"

namespace regina {

/**
 * An XML packet reader that reads a single triangulation.
 *
 * \ifaces Not present.
 */
class NXMLTriangulationReader : public NXMLPacketReader {
    private:
        NTriangulation* tri;
            /**< The triangulation currently being read. */

    public:
        /**
         * Creates a new triangulation reader.
         */
        NXMLTriangulationReader();

        virtual NPacket* getPacket();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

// Inline functions for NXMLTriangulationReader

inline NXMLTriangulationReader::NXMLTriangulationReader() :
        tri(new NTriangulation()) {
}

inline NPacket* NXMLTriangulationReader::getPacket() {
    return tri;
}

} // namespace regina

#endif

