
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
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

#ifndef __REGINA_TRIANGLE2_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGLE2_H
#endif

namespace regina {

/**
 * Represents a triangle within a 2-manifold triangulation.
 *
 * This class is typically referred to by the aliases Simplex<2> or
 * Triangle<2> (or, for Python users, \c Simplex2 and \c Triangle2).
 *
 * The class template Face<dim, dim> specialises the generic Face class
 * template in order to work with top-dimensional simplices in a triangulation.
 * This class Face<2, 2> specialises it further to add dimension-specific
 * aliases for dimension 2.
 *
 * See the generic documentation for Face<dim, dim> for an overview of how the
 * top-dimensional simplex classes work.  In Python, you can read this
 * generic documentation by looking at a higher dimension: try `help(Simplex5)`.
 *
 * Triangles do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim2.h
 *
 * \ingroup dim2
 */
template <>
class Face<2, 2> : public detail::SimplexBase<2> {
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
         * \param tri the triangulation to which the new triangle belongs.
         */
        Face(Triangulation<2>* tri);
        /**
         * Creates a new triangle whose description and locks are cloned
         * from the given triangle, and with no edges joined to anything.
         *
         * \param clone the triangle whose details should be cloned.
         * \param tri the triangulation to which the new tetrahedron belongs.
         */
        Face(const Face& clone, Triangulation<2>* tri);
        /**
         * Creates a new triangle with the given description, no locks, and
         * no edges joined to anything.
         *
         * \param desc the description to give the new triangle.
         * \param tri the triangulation to which the new triangle belongs.
         */
        Face(const std::string& desc, Triangulation<2>* tri);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
        /**< Allow access to private members. */
};

// Specialisations to improve upon generic implementations from SimplexBase
// (hide these from doxygen, which struggles with specialisations):

#ifndef __DOXYGEN
template <>
inline Edge<2>* detail::SimplexBase<2>::edge(int i, int j) const {
    return (i == j ? nullptr : edge(3 - i - j));
}
#endif

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

inline Face<2, 2>::Face(const Face& clone, Triangulation<2>* tri) :
        detail::SimplexBase<2>(clone, tri) {
}

inline Face<2, 2>::Face(const std::string& desc,
        Triangulation<2>* tri) : detail::SimplexBase<2>(desc, tri) {
}

} // namespace regina

#endif

