
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

/*! \file triangulation/xmltrireader.h
 *  \brief Deals with parsing XML data for triangulation packets.
 */

#ifndef __XMLTRIREADER_H
#ifndef __DOXYGEN
#define __XMLTRIREADER_H
#endif

#include "triangulation/detail/xmltrireader.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * An XML packet reader that reads a single <i>dim</i>-dimensional
 * triangulation.
 *
 * In some dimensions this template is specialised so that it can read in
 * additional properties of the triangulation.  In order to use these
 * specialised classes, you will need to include the corresponding headers
 * (e.g., triangulation/xmltrireader3.h for \a dim = 3).
 *
 * \ifacespython Not present.
 *
 * \tparam dim The dimension of the triangulation being read.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class XMLTriangulationReader :
        public detail::XMLTriangulationReaderBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of XMLTriangulationReader<dim> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * Creates a new triangulation reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLTriangulationReader(XMLTreeResolver& resolver);

        /**
         * Returns an XML element reader for the given optional property of a
         * <i>dim</i>-dimensional triangulation.
         *
         * If \a subTagName names an XML element that describes an optional
         * property of a triangulation (such as \c H1 or \c fundgroup for
         * 3-manifold triangulations), then this function should return
         * a corresponding element reader.
         *
         * Otherwise this function should return a new XMLElementReader,
         * which will cause the XML element to be ignored.
         *
         * @param subTagName the name of the XML subelement opening tag.
         * @param subTagProps the properties associated with the
         * subelement opening tag.
         * @return a newly created element reader that will be used to
         * parse the subelement.  This class should not take care of the
         * new reader's destruction; that will be done by the parser.
         */
        XMLElementReader* startPropertySubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
};

// Note that some of our classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class XMLTriangulationReader<2>;
template <> class XMLTriangulationReader<3>;
template <> class XMLTriangulationReader<4>;

/*@}*/

// Inline functions for XMLTriangulationReader

template <int dim>
inline XMLTriangulationReader<dim>::XMLTriangulationReader(
        XMLTreeResolver& resolver) :
        detail::XMLTriangulationReaderBase<dim>(resolver) {
}

template <int dim>
inline XMLElementReader* XMLTriangulationReader<dim>::startPropertySubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new XMLElementReader();
}

} // namespace regina

#endif

