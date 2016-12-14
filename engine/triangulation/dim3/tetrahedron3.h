
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

/*! \file triangulation/dim3/tetrahedron3.h
 *  \brief Internal header for tetrahedra in a 3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __TETRAHEDRON3_H
#ifndef __DOXYGEN
#define __TETRAHEDRON3_H
#endif

namespace regina {

/**
 * \weakgroup dim3
 * @{
 */

/**
 * Represents a tetrahedron within a 3-manifold triangulation.
 *
 * This class is typically referred to by the aliases Simplex<3> or
 * Tetrahedron<3> (or, for Python users, \c Simplex3 and \c Tetrahedron3).
 *
 * The class template Face<dim, dim> specialises the generic Face class
 * template in order to work with top-dimensional simplices in a triangulation.
 * This class Face<3, 3> specialises it further to add dimension-specific
 * aliases for dimension 3.
 *
 * See the documentation for the partial specialisation Face<dim, dim> for an
 * overview of how a top-dimensional simplex class works.
 *
 * \headerfile triangulation/dim3.h
 */
template <>
class REGINA_API Face<3, 3> : public detail::SimplexBase<3> {
    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex<3>* adjacentTetrahedron(int face) const;
        /**
         * A dimension-specific alias for adjacentFacet().
         *
         * See adjacentFacet() for further information.
         */
        int adjacentFace(int face) const;

    private:
        /**
         * Creates a new tetrahedron with empty description and no
         * faces joined to anything.
         *
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        Face(Triangulation<3>* tri);
        /**
         * Creates a new tetrahedron with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new tetrahedron.
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        Face(const std::string& desc, Triangulation<3>* tri);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
        /**< Allow access to private members. */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NTetrahedron, you should use
 * the new alias Simplex<3> (or, if you prefer, the full class name Face<3, 3>).
 */
REGINA_DEPRECATED typedef Simplex<3> NTetrahedron;

/*@}*/

// Inline functions for Simplex<3>

inline Simplex<3>* Face<3, 3>::adjacentTetrahedron(int face) const {
    return adjacentSimplex(face);
}

inline int Face<3, 3>::adjacentFace(int face) const {
    return adjacentFacet(face);
}

inline Face<3, 3>::Face(Triangulation<3>* tri) : detail::SimplexBase<3>(tri) {
}

inline Face<3, 3>::Face(const std::string& desc, Triangulation<3>* tri) :
        detail::SimplexBase<3>(desc, tri) {
}

} // namespace regina

#endif

