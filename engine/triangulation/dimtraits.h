
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file triangulation/dimtraits.h
 *  \brief A template class that provides information on working in
 *  different dimensions.
 */

#ifndef __DIMTRAITS_H
#ifndef __DOXYGEN
#define __DIMTRAITS_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A template class that provides typedefs and other information about
 * working in each of the supported dimensions.
 *
 * Note that this file does not bring in all of the headers for the
 * individual types.
 */
template <int dim>
struct DimTraits {
    typedef void Triangulation;
        /**< The main data type for a \a dim-manifold triangulation. */
    typedef void Simplex;
        /**< The data type for a top-dimensional simplex in a
             \a dim-manifold triangulation. */
    typedef void Isomorphism;
        /**< The data type for an isomorphism between two
             \a dim-manifold triangulations. */
    typedef void FacetSpec;
        /**< The lightweight data type that specifies an individual
             facet of an individual top-dimensional simplex in a
             \a dim-manifold triangulation. */
    typedef void FacetPairing;
        /**< The data type that represents a pairing of facets of
             top-dimensional simplices in a \a dim-manifold triangulation. */
    typedef void Perm;
        /**< The permutation type used to describe gluings between
             top-dimensional simplices in a \a dim-manifold triangulation. */
};

#ifndef __DOXYGEN
class Dim2Triangulation;
class Dim2Triangle;
class Dim2Isomorphism;
class Dim2EdgePairing;
class NPerm3;

template <>
struct DimTraits<2> {
    typedef Dim2Triangulation Triangulation;
    typedef Dim2Triangle Simplex;
    typedef Dim2Isomorphism Isomorphism;
    typedef Dim2EdgePairing FacetPairing;
    typedef NPerm3 Perm;
};

class NTriangulation;
class NTetrahedron;
class NIsomorphism;
class NFacePairing;
class NPerm4;

template <>
struct DimTraits<3> {
    typedef NTriangulation Triangulation;
    typedef NTetrahedron Simplex;
    typedef NIsomorphism Isomorphism;
    typedef NFacePairing FacetPairing;
    typedef NPerm4 Perm;
};
#endif

} // namespace regina

#endif

