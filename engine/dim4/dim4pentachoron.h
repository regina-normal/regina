
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4pentachoron.h
 *  \brief Deals with 4-dimensional simplices in a 4-manifold triangulation.
 */

#ifndef __DIM4PENTACHORON_H
#ifndef __DOXYGEN
#define __DIM4PENTACHORON_H
#endif

#include "regina-core.h"
#include "generic/simplex.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

template <int> class Triangulation;
typedef Triangulation<4> Dim4Triangulation;

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
 * An implementation note: the Dim4Triangulation class is responsible for
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
        Simplex(Dim4Triangulation* tri);
        /**
         * Creates a new pentachoron with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Simplex(const std::string& desc, Dim4Triangulation* tri);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<4>.
 */
typedef Simplex<4> Dim4Pentachoron;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4triangulation.h"
namespace regina {

// Inline functions for Dim4Pentachoron

inline Dim4Pentachoron* Dim4Pentachoron::adjacentPentachoron(int facet) const {
    return adjacentSimplex(facet);
}

inline Simplex<4>::Simplex(Dim4Triangulation* tri) :
        detail::SimplexBase<4>(tri) {
}

inline Simplex<4>::Simplex(const std::string& desc, Dim4Triangulation* tri) :
        detail::SimplexBase<4>(desc, tri) {
}

} // namespace regina

#endif

