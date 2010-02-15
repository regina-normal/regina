
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

/*! \file hypersurface/nxmlhypersurfacereader.h
 *  \brief Deals with parsing XML data for normal hypersurface lists.
 */

#ifndef __NXMLHYPERSURFACEREADER_H
#ifndef __DOXYGEN
#define __NXMLHYPERSURFACEREADER_H
#endif

#include "hypersurface/nnormalhypersurfacelist.h"
#include "packet/nxmlpacketreader.h"

namespace regina {

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * An XML element reader that reads a single normal hypersurface in a
 * 4-manifold triangulation.
 *
 * \ifacespython Not present.
 */
class NXMLNormalHypersurfaceReader : public NXMLElementReader {
    private:
        NNormalHypersurface* surface_;
            /**< The normal hypersurface currently being read. */
        Dim4Triangulation* tri_;
            /**< The triangulation in which this hypersurface lives. */
        int flavour_;
            /**< The flavour of coordinate system used by this hypersurface. */
        long vecLen_;
            /**< The length of corresponding normal hypersurface vector. */
        std::string name_;
            /**< The optional name associated with this normal hypersurface. */

    public:
        /**
         * Creates a new normal hypersurface reader.
         *
         * @param tri the triangulation in which this normal hypersurface lives.
         * @param flavour the flavour of coordinate system used by
         * this normal hypersurface.
         */
        NXMLNormalHypersurfaceReader(Dim4Triangulation* tri, int flavour);

        /**
         * Returns the normal hypersurface that has been read.
         *
         * @return the newly allocated normal hypersurface, or 0 if an error
         * occurred.
         */
        NNormalHypersurface* getHypersurface();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
        virtual NXMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

/**
 * An XML packet reader that reads a single normal hypersurface list.
 *
 * \pre The parent XML element reader is in fact an
 * NXMLTriangulationReader.
 *
 * \ifacespython Not present.
 */
class NXMLNormalHypersurfaceListReader : public NXMLPacketReader {
    private:
        NNormalHypersurfaceList* list_;
            /**< The normal hypersurface list currently being read. */
        Dim4Triangulation* tri_;
            /**< The triangulation in which these normal hypersurfaces live. */

    public:
        /**
         * Creates a new normal hypersurface list reader.
         *
         * @param tri the triangulation in which these normal hypersurfaces
         * live.
         */
        NXMLNormalHypersurfaceListReader(Dim4Triangulation* tri);

        virtual NPacket* getPacket();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/*@}*/

// Inline functions for NXMLNormalHypersurfaceReader

inline NXMLNormalHypersurfaceReader::NXMLNormalHypersurfaceReader(
        Dim4Triangulation* tri, int flavour) : surface_(0), tri_(tri),
        flavour_(flavour), vecLen_(-1) {
}

inline NNormalHypersurface* NXMLNormalHypersurfaceReader::getHypersurface() {
    return surface_;
}

// Inline functions for NXMLNormalHypersurfaceListReader

inline NXMLNormalHypersurfaceListReader::NXMLNormalHypersurfaceListReader(
        Dim4Triangulation* tri) : list_(0), tri_(tri) {
}

inline NPacket* NXMLNormalHypersurfaceListReader::getPacket() {
    return list_;
}

} // namespace regina

#endif

