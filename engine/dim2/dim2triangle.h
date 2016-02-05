
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

/*! \file dim2/dim2triangle.h
 *  \brief Deals with triangular faces in a 2-manifold triangulation.
 */

#ifndef __DIM2TRIANGLE_H
#ifndef __DOXYGEN
#define __DIM2TRIANGLE_H
#endif

#include "regina-core.h"
#include "generic/simplex.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

template <int> class Triangulation;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a triangle within a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Simplex class template; see the
 * Simplex documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each triangle stores additional details on how this
 * triangle and its sub-faces integrate into the overall skeletal structure
 * of the triangulation.
 *
 * An implementation note: the Dim2Triangulation class is responsible for
 * creating, maintaining and destroying this extra skeletal information.
 */
template <>
class REGINA_API Simplex<2> : public detail::SimplexBase<2> {
    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex* adjacentTriangle(int edge) const;
        /**
         * A dimension-specific alias for adjacentFacet().
         *
         * See adjacentFacet() for further information.
         */
        int adjacentEdge(int edge) const;

    private:
        /**
         * Creates a new triangle with empty description and no
         * edges joined to anything.
         *
         * @param tri the triangulation to which the new triangle belongs.
         */
        Simplex(Dim2Triangulation* tri);
        /**
         * Creates a new triangle with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new triangle.
         * @param tri the triangulation to which the new triangle belongs.
         */
        Simplex(const std::string& desc, Dim2Triangulation* tri);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<2>.
 */
typedef Simplex<2> Dim2Triangle;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangulation.h"
namespace regina {

// Inline functions for Simplex<2>

inline Simplex<2>* Simplex<2>::adjacentTriangle(int edge) const {
    return adjacentSimplex(edge);
}

inline int Simplex<2>::adjacentEdge(int edge) const {
    return adjacentFacet(edge);
}

inline Simplex<2>::Simplex(Dim2Triangulation* tri) :
        detail::SimplexBase<2>(tri) {
}

inline Simplex<2>::Simplex(const std::string& desc,
        Dim2Triangulation* tri) : detail::SimplexBase<2>(desc, tri) {
}

} // namespace regina

#endif

