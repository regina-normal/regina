
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

/*! \file nxmlsurfacereader.h
 *  \brief Deals with parsing XML data for normal surface lists.
 */

#ifndef __NXMLSURFACEREADER_H
#ifndef __DOXYGEN
#define __NXMLSURFACEREADER_H
#endif

#include "packet/nxmlpacketreader.h"
#include "surfaces/nnormalsurfacelist.h"

namespace regina {

/**
 * An XML element reader that reads a single normal surface.
 *
 * \ifaces Not present.
 */
class NXMLNormalSurfaceReader : public NXMLElementReader {
    private:
        NNormalSurface* surface;
            /**< The normal surface currently being read. */
        NTriangulation* tri;
            /**< The triangulation in which this surface lives. */
        int flavour;
            /**< The flavour of coordinate system used by this surface. */
        long vecLen;
            /**< The length of corresponding normal surface vector. */
        std::string name;
            /**< The optional name associated with this normal surface. */

    public:
        /**
         * Creates a new normal surface reader.
         *
         * @param newTri the triangulation in which this normal surface lives.
         * @param newFlavour the flavour of coordinate system used by
         * this normal surface.
         */
        NXMLNormalSurfaceReader(NTriangulation* newTri, int newFlavour);

        /**
         * Returns the normal surface that has been read.
         *
         * @return the newly allocated normal surface, or 0 if an error
         * occurred.
         */
        NNormalSurface* getSurface();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
        virtual NXMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

/**
 * An XML packet reader that reads a single normal surface list.
 *
 * \pre The parent XML element reader is in fact an
 * NXMLTriangulationReader.
 *
 * \ifaces Not present.
 */
class NXMLNormalSurfaceListReader : public NXMLPacketReader {
    private:
        NNormalSurfaceList* list;
            /**< The normal surface list currently being read. */
        NTriangulation* tri;
            /**< The triangulation in which these normal surfaces live. */

    public:
        /**
         * Creates a new normal surface list reader.
         *
         * @param newTri the triangulation in which these normal surfaces
         * live.
         */
        NXMLNormalSurfaceListReader(NTriangulation* newTri);

        virtual NPacket* getPacket();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

// Inline functions for NXMLNormalSurfaceReader

inline NXMLNormalSurfaceReader::NXMLNormalSurfaceReader(
        NTriangulation* newTri, int newFlavour) : surface(0), tri(newTri),
        flavour(newFlavour), vecLen(-1) {
}

inline NNormalSurface* NXMLNormalSurfaceReader::getSurface() {
    return surface;
}

// Inline functions for NXMLNormalSurfaceListReader

inline NXMLNormalSurfaceListReader::NXMLNormalSurfaceListReader(
        NTriangulation* newTri) : list(0), tri(newTri) {
}

inline NPacket* NXMLNormalSurfaceListReader::getPacket() {
    return list;
}

} // namespace regina

#endif

