
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

/*! \file angle/xmlanglestructreader.h
 *  \brief Deals with parsing XML data for angle structure lists.
 */

#ifndef __XMLANGLESTRUCTREADER_H
#ifndef __DOXYGEN
#define __XMLANGLESTRUCTREADER_H
#endif

#include "regina-core.h"
#include "packet/xmlpacketreader.h"
#include "angle/anglestructures.h"

namespace regina {

/**
 * \weakgroup angle
 * @{
 */

/**
 * An XML element reader that reads a single angle structure.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLAngleStructureReader : public XMLElementReader {
    private:
        AngleStructure* angles;
            /**< The angle structure currently being read. */
        Triangulation<3>* tri;
            /**< The triangulation on which this angle structure is placed. */
        long vecLen;
            /**< The length of corresponding angle structure vector. */

    public:
        /**
         * Creates a new angle structure reader.
         *
         * @param newTri the triangulation on which this angle structure lies.
         */
        XMLAngleStructureReader(Triangulation<3>* newTri);

        /**
         * Returns the angle structure that has been read.
         *
         * @return the newly allocated angle structure, or 0 if an error
         * occurred.
         */
        AngleStructure* structure();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
        virtual XMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

/**
 * An XML packet reader that reads a single angle structure list.
 *
 * \pre The parent XML element reader is in fact an
 * XMLTriangulationReader<3>.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLAngleStructuresReader : public XMLPacketReader {
    private:
        AngleStructures* list;
            /**< The angle structure list currently being read. */
        Triangulation<3>* tri;
            /**< The triangulation on which these angle structures
                 are placed. */

    public:
        /**
         * Creates a new angle structure list reader.
         *
         * @param newTri the triangulation on which these angle
         * structures are placed.
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLAngleStructuresReader(Triangulation<3>* newTri,
            XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLAngleStructureReader

inline XMLAngleStructureReader::XMLAngleStructureReader(
        Triangulation<3>* newTri) : angles(0), tri(newTri), vecLen(-1) {
}

inline AngleStructure* XMLAngleStructureReader::structure() {
    return angles;
}

// Inline functions for XMLAngleStructuresReader

inline XMLAngleStructuresReader::XMLAngleStructuresReader(
        Triangulation<3>* newTri, XMLTreeResolver& resolver) :
        XMLPacketReader(resolver),
        list(new AngleStructures(false)), tri(newTri) {
}

inline Packet* XMLAngleStructuresReader::packet() {
    return list;
}

} // namespace regina

#endif

