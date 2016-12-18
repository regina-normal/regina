
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

/*! \file triangulation/dim2/triangle2.h
 *  \brief Internal header for triangular faces in a 2-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation2.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation2.h.
#include "triangulation/dim2/triangulation2.h"

#ifndef __TRIANGLE2_H
#ifndef __DOXYGEN
#define __TRIANGLE2_H
#endif

namespace regina {

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a tetrahedron within a 2-manifold triangulation.
 *
 * This class is typically referred to by the aliases Simplex<2> or
 * Triangle<2> (or, for Python users, \c Simplex2 and \c Triangle2).
 *
 * The class template Face<dim, dim> specialises the generic Face class
 * template in order to work with top-dimensional simplices in a triangulation.
 * This class Face<2, 2> specialises it further to add dimension-specific
 * aliases for dimension 2.
 *
 * See the documentation for the partial specialisation Face<dim, dim> for an
 * overview of how a top-dimensional simplex class works.
 *
 * \headerfile triangulation/dim2.h
 */
template <>
class REGINA_API Face<2, 2> : public detail::SimplexBase<2> {
    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex<2>* adjacentTriangle(int edge) const;
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
        Face(Triangulation<2>* tri);
        /**
         * Creates a new triangle with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new triangle.
         * @param tri the triangulation to which the new triangle belongs.
         */
        Face(const std::string& desc, Triangulation<2>* tri);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
        /**< Allow access to private members. */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Triangle, you should use
 * the new alias Simplex<2> (or, if you prefer, the full class name Face<2, 2>).
 */
REGINA_DEPRECATED typedef Simplex<2> Dim2Triangle;

/*@}*/

// Inline functions for Simplex<2>

inline Simplex<2>* Face<2, 2>::adjacentTriangle(int edge) const {
    return adjacentSimplex(edge);
}

inline int Face<2, 2>::adjacentEdge(int edge) const {
    return adjacentFacet(edge);
}

inline Face<2, 2>::Face(Triangulation<2>* tri) :
        detail::SimplexBase<2>(tri) {
}

inline Face<2, 2>::Face(const std::string& desc,
        Triangulation<2>* tri) : detail::SimplexBase<2>(desc, tri) {
}

} // namespace regina

#endif

