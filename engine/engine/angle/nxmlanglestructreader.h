
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file nxmlanglestructreader.h
 *  \brief Deals with parsing XML data for angle structure lists.
 */

#ifndef __NXMLANGLESTRUCTREADER_H
#ifndef __DOXYGEN
#define __NXMLANGLESTRUCTREADER_H
#endif

#include "packet/nxmlpacketreader.h"
#include "angle/nanglestructurelist.h"

namespace regina {

/**
 * \weakgroup angle
 * @{
 */

/**
 * An XML element reader that reads a single angle structure.
 *
 * \ifaces Not present.
 */
class NXMLAngleStructureReader : public NXMLElementReader {
    private:
        NAngleStructure* angles;
            /**< The angle structure currently being read. */
        NTriangulation* tri;
            /**< The triangulation on which this angle structure is placed. */
        long vecLen;
            /**< The length of corresponding angle structure vector. */

    public:
        /**
         * Creates a new angle structure reader.
         *
         * @param newTri the triangulation on which this angle structure lies.
         */
        NXMLAngleStructureReader(NTriangulation* newTri);

        /**
         * Returns the angle structure that has been read.
         *
         * @return the newly allocated angle structure, or 0 if an error
         * occurred.
         */
        NAngleStructure* getStructure();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
        virtual NXMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

/**
 * An XML packet reader that reads a single angle structure list.
 *
 * \pre The parent XML element reader is in fact an
 * NXMLTriangulationReader.
 *
 * \ifaces Not present.
 */
class NXMLAngleStructureListReader : public NXMLPacketReader {
    private:
        NAngleStructureList* list;
            /**< The angle structure list currently being read. */
        NTriangulation* tri;
            /**< The triangulation on which these angle structures
                 are placed. */

    public:
        /**
         * Creates a new angle structure list reader.
         *
         * @param newTri the triangulation on which these angle
         * structures are placed.
         */
        NXMLAngleStructureListReader(NTriangulation* newTri);

        virtual NPacket* getPacket();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/*@}*/

// Inline functions for NXMLAngleStructureReader

inline NXMLAngleStructureReader::NXMLAngleStructureReader(
        NTriangulation* newTri) : angles(0), tri(newTri), vecLen(-1) {
}

inline NAngleStructure* NXMLAngleStructureReader::getStructure() {
    return angles;
}

// Inline functions for NXMLAngleStructureListReader

inline NXMLAngleStructureListReader::NXMLAngleStructureListReader(
        NTriangulation* newTri) : list(new NAngleStructureList()),
        tri(newTri) {
}

inline NPacket* NXMLAngleStructureListReader::getPacket() {
    return list;
}

} // namespace regina

#endif

