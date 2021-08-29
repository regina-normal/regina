
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

/*! \file surfaces/xmlsurfacereader.h
 *  \brief Deals with parsing XML data for normal surface lists.
 */

#ifndef __REGINA_XMLSURFACEREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLSURFACEREADER_H
#endif

#include <optional>
#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "surfaces/normalsurfaces.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * An XML element reader that reads a single normal surface.
 *
 * \ifacespython Not present.
 */
class XMLNormalSurfaceReader : public XMLElementReader {
    private:
        std::optional<NormalSurface> surface_;
            /**< The normal surface currently being read. */
        const Triangulation<3>* tri;
            /**< The triangulation in which this surface lives. */
        NormalCoords coords;
            /**< The coordinate system used by this surface. */
        int vecEnc;
            /**< The integer encoding used for the normal surface vector,
                 or 0 if this is unknown. */
        long vecLen;
            /**< The length of the normal surface vector, or -1 if this
                 is unknown (since 0 is a valid vector length). */
        std::string name;
            /**< The optional name associated with this normal surface. */

    public:
        /**
         * Creates a new normal surface reader.
         *
         * @param newTri the triangulation in which this normal surface lives.
         * @param newCoords the coordinate system used by this normal surface.
         */
        XMLNormalSurfaceReader(const Triangulation<3>* newTri,
            NormalCoords newCoords);

        /**
         * Returns a reference to the normal surface that has been read.
         *
         * @return the normal surface, or no value if an error occurred.
         */
        std::optional<NormalSurface>& surface();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader) override;
        virtual void initialChars(const std::string& chars) override;
        virtual XMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
};

/**
 * An XML packet reader that reads a single normal surface list.
 *
 * \pre The parent XML element reader is in fact an
 * XMLTriangulationReader<3>.
 *
 * \ifacespython Not present.
 */
class XMLNormalSurfacesReader : public XMLPacketReader {
    private:
        NormalSurfaces* list;
            /**< The normal surface list currently being read. */
        const Triangulation<3>* tri;
            /**< The triangulation in which these normal surfaces live. */

    public:
        /**
         * Creates a new normal surface list reader.
         *
         * @param newTri the triangulation in which these normal surfaces live.
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLNormalSurfacesReader(const Triangulation<3>* newTri,
            XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLNormalSurfaceReader

inline XMLNormalSurfaceReader::XMLNormalSurfaceReader(
        const Triangulation<3>* newTri, NormalCoords newCoords) :
        tri(newTri), coords(newCoords), vecLen(-1), vecEnc(0) {
}

inline std::optional<NormalSurface>& XMLNormalSurfaceReader::surface() {
    return surface_;
}

// Inline functions for XMLNormalSurfacesReader

inline XMLNormalSurfacesReader::XMLNormalSurfacesReader(
        const Triangulation<3>* newTri, XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), list(0), tri(newTri) {
}

inline Packet* XMLNormalSurfacesReader::packet() {
    return list;
}

} // namespace regina

#endif

