
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file generic/xmltrireader.h
 *  \brief Deals with parsing XML data for triangulation packets.
 */

#ifndef __XMLTRIREADER_H
#ifndef __DOXYGEN
#define __XMLTRIREADER_H
#endif

#include "regina-core.h"
#include "packet/nxmlpacketreader.h"
#include "generic/triangulation.h"
#include "utilities/stringutils.h"
#include <vector>

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * An XML packet reader that reads a single <i>dim</i>-dimensional
 * triangulation.
 *
 * \ifacespython Not present.
 */
template <int dim>
class REGINA_API XMLTriangulationReader : public NXMLPacketReader {
    static_assert(! standardDim(dim),
        "The generic implementation of XMLTriangulationReader<dim> "
        "should not be used for Regina's standard dimensions.");

    private:
        Triangulation<dim>* tri_;
            /**< The triangulation currently being read. */

    public:
        /**
         * Creates a new triangulation reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLTriangulationReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet() override;
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader) override;

    private:
        /**
         * Reads a single top-dimensional simplex with its name and gluings.
         */
        class SimplexReader : public NXMLElementReader {
            private:
                Triangulation<dim>* tri_;
                Simplex<dim>* simplex_;

            public:
                SimplexReader(Triangulation<dim>* tri, size_t whichSimplex) :
                        tri_(tri), simplex_(tri->simplices()[whichSimplex]) {
                }

                virtual void startElement(const std::string&,
                        const regina::xml::XMLPropertyDict& props,
                        NXMLElementReader*) {
                    simplex_->setDescription(props.lookup("desc"));
                }

                virtual void initialChars(const std::string& chars) {
                    std::vector<std::string> tokens;
                    if (basicTokenise(back_inserter(tokens), chars) !=
                            2 * (dim + 1))
                        return;

                    long simpIndex;
                    typename NPerm<dim + 1>::Code permCode;
                    NPerm<dim + 1> perm;
                    Simplex<dim>* adjSimp;
                    int adjFacet;
                    for (int k = 0; k <= dim; ++k) {
                        if (! valueOf(tokens[2 * k], simpIndex))
                            continue;
                        if (! valueOf(tokens[2 * k + 1], permCode))
                            continue;

                        if (simpIndex < 0 || simpIndex >=
                                static_cast<long>(tri_->size()))
                            continue;
                        if (! NPerm<dim + 1>::isPermCode(permCode))
                            continue;

                        perm.setPermCode(permCode);
                        adjSimp = tri_->simplices()[simpIndex];
                        adjFacet = perm[k];
                        if (adjSimp == simplex_ && adjFacet == k)
                            continue;
                        if (simplex_->adjacentSimplex(k))
                            continue;
                        if (adjSimp->adjacentSimplex(adjFacet))
                            continue;

                        simplex_->joinTo(k, adjSimp, perm);
                    }
                }
        };

        /**
         * Reads an entire set of top-dimensional simplices with their names
         * and gluings.
         */
        class SimplicesReader : public NXMLElementReader {
            private:
                Triangulation<dim>* tri_;
                size_t readSimplices_;

            public:
                SimplicesReader(Triangulation<dim>* tri) :
                        tri_(tri), readSimplices_(0) {
                }

                virtual void startElement(const std::string& /* tagName */,
                        const regina::xml::XMLPropertyDict& props,
                        NXMLElementReader*) {
                    long nSimplices;
                    if (valueOf(props.lookup("size"), nSimplices))
                        for ( ; nSimplices > 0; --nSimplices)
                            tri_->newSimplex();
                }

                virtual NXMLElementReader* startSubElement(
                        const std::string& subTagName,
                        const regina::xml::XMLPropertyDict&) {
                    if (subTagName == "simplex") {
                        if (readSimplices_ < tri_->size())
                            return new SimplexReader(tri_, readSimplices_++);
                        else
                            return new NXMLElementReader();
                    } else
                        return new NXMLElementReader();
                }
        };
};

/*@}*/

// Inline functions for XMLTriangulationReader

template <int dim>
inline XMLTriangulationReader<dim>::XMLTriangulationReader(
        NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), tri_(new Triangulation<dim>()) {
}

template <int dim>
inline NPacket* XMLTriangulationReader<dim>::packet() {
    return tri_;
}

template <int dim>
NXMLElementReader* XMLTriangulationReader<dim>::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "simplices")
        return new SimplicesReader(tri_);
    return new NXMLElementReader();
}

template <int dim>
inline void XMLTriangulationReader<dim>::endContentSubElement(
        const std::string&, NXMLElementReader*) {
}

} // namespace regina

#endif

