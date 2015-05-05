
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

/*! \file generic/dimtraits.h
 *  \brief A template class that provides information on working in
 *  different dimensions.
 */

#ifndef __DIMTRAITS_H
#ifndef __DOXYGEN
#define __DIMTRAITS_H
#endif

#include "regina-core.h"

namespace regina {

template <int n> class Simplex;
template <int n> class Triangulation;

/**
 * \addtogroup generic Generic code
 * Template code to work with triangulations of arbitrary dimension.
 * @{
 */

/**
 * A template class that provides typedefs and other information about
 * working in each of the supported dimensions.
 *
 * Note that this file does not bring in all of the headers for the
 * individual types.
 *
 * \ifacespython Not present.
 */
template <int dim>
struct DimTraits {
    typedef Triangulation<dim> Triangulation;
        /**< The main data type for a <i>dim</i>-manifold triangulation. */
    typedef void FacetPairing;
        /**< The data type that represents a pairing of facets of
             top-dimensional simplices in a <i>dim</i>-manifold
             triangulation. */
};

/**
 * A template class that provides typedefs and other information about
 * faces of triangulations in each of the supported dimensions.
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangulation, and the template argument \a subdim refers to the
 * dimension of a face within such a triangulation.
 *
 * This class is defined for all values of \a subdim between 0 and \a dim
 * inclusive.
 *
 * Note that this file does not bring in all of the headers for the
 * individual types.
 *
 * \ifacespython Not present.
 */
template <int dim, int subdim>
struct FaceTraits {
    typedef void Face;
        /**< The main data type for a </i>subdim</i>-dimensional face in a
             <i>dim</i>-manifold triangulation. */
};

#ifndef __DOXYGEN

class Dim2Edge;
class Dim2EdgePairing;
class Dim2Vertex;

template <>
struct DimTraits<2> {
    typedef Triangulation<2> Triangulation;
    typedef Dim2EdgePairing FacetPairing;
};

class NEdge;
class NTriangle;
class NTriangulation;
class NFacePairing;
class NVertex;

template <>
struct DimTraits<3> {
    typedef NTriangulation Triangulation;
    typedef NFacePairing FacetPairing;
};

template <>
struct FaceTraits<2, 0> {
    typedef Dim2Vertex Face;
};

template <>
struct FaceTraits<2, 1> {
    typedef Dim2Edge Face;
};

template <>
struct FaceTraits<2, 2> {
    typedef Simplex<2> Face;
};

template <>
struct FaceTraits<3, 0> {
    typedef NVertex Face;
};

template <>
struct FaceTraits<3, 1> {
    typedef NEdge Face;
};

template <>
struct FaceTraits<3, 2> {
    typedef NTriangle Face;
};

template <>
struct FaceTraits<3, 3> {
    typedef Simplex<3> Face;
};
#endif

} // namespace regina

#endif

