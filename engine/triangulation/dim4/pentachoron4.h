
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
 * This is a specialisation of the generic Simplex class template; see the
 * generic Simplex documentation for an overview of how this class works.
 *
 * This 4-dimensional specialisation contains some extra functionality.
 * In particular, each pentachoron stores additional details on how this
 * pentachoron and its sub-faces integrate into the overall skeletal
 * structure of the triangulation.
 *
 * An implementation note: the Triangulation<4> class is responsible for
 * creating, maintaining and destroying this extra skeletal information.
 */
template <>
class REGINA_API Simplex<4> : public detail::SimplexBase<4> {
    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex* adjacentPentachoron(int facet) const;

    private:
        /**
         * Creates a new pentachoron with empty description and no
         * facets joined to anything.
         *
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Simplex(Triangulation<4>* tri);
        /**
         * Creates a new pentachoron with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Simplex(const std::string& desc, Triangulation<4>* tri);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<4>.
 */
typedef Simplex<4> Dim4Pentachoron;

/*@}*/

// Inline functions for Dim4Pentachoron

inline Dim4Pentachoron* Dim4Pentachoron::adjacentPentachoron(int facet) const {
    return adjacentSimplex(facet);
}

inline Simplex<4>::Simplex(Triangulation<4>* tri) :
        detail::SimplexBase<4>(tri) {
}

inline Simplex<4>::Simplex(const std::string& desc, Triangulation<4>* tri) :
        detail::SimplexBase<4>(desc, tri) {
}

} // namespace regina

#endif

