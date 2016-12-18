
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

/*! \file triangulation/dim4/pentachoron4.h
 *  \brief Internal header for 4-dimensional simplices in a 4-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __PENTACHORON4_H
#ifndef __DOXYGEN
#define __PENTACHORON4_H
#endif

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a pentachoron (a 4-dimensional simplex) within a
 * 4-manifold triangulation.
 *
 * This class is typically referred to by the aliases Simplex<4> or
 * Pentachoron<4> (or, for Python users, \c Simplex4 and \c Pentachoron4).
 *
 * The class template Face<dim, dim> specialises the generic Face class
 * template in order to work with top-dimensional simplices in a triangulation.
 * This class Face<4, 4> specialises it further to add dimension-specific
 * aliases for dimension 4.
 *
 * See the documentation for the partial specialisation Face<dim, dim> for an
 * overview of how a top-dimensional simplex class works.
 *
 * \headerfile triangulation/dim4.h
 */
template <>
class REGINA_API Face<4, 4> : public detail::SimplexBase<4> {
    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex<4>* adjacentPentachoron(int facet) const;

    private:
        /**
         * Creates a new pentachoron with empty description and no
         * facets joined to anything.
         *
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Face(Triangulation<4>* tri);
        /**
         * Creates a new pentachoron with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Face(const std::string& desc, Triangulation<4>* tri);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Pentachoron, you should use
 * the new alias Simplex<4> (or, if you prefer, the full class name Face<4, 4>).
 */
REGINA_DEPRECATED typedef Simplex<4> Dim4Pentachoron;

/*@}*/

// Inline functions for Pentachoron<4>

inline Simplex<4>* Face<4, 4>::adjacentPentachoron(int facet) const {
    return adjacentSimplex(facet);
}

inline Face<4, 4>::Face(Triangulation<4>* tri) :
        detail::SimplexBase<4>(tri) {
}

inline Face<4, 4>::Face(const std::string& desc, Triangulation<4>* tri) :
        detail::SimplexBase<4>(desc, tri) {
}

} // namespace regina

#endif

