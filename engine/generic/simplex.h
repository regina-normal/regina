
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/*! \file generic/simplex.h
 *  \brief Deals with top-dimensional simplices in a triangulation.
 */

#ifndef __SIMPLEX_H
#ifndef __DOXYGEN
#define __SIMPLEX_H
#endif

// Our #includes are a little further down - we need choose() to be
// defined before we include generic/detail/simplex.h.

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Returns the binomial coefficient \a n choose \a r.
 *
 * This routine puts no restrictions on the value of \a r.  If \a r is
 * negative or greater than \a n, then it will simply return 0.
 *
 * @param n any non-negative integer; this must be at most 16
 * (since otherwise the result may overflow).
 * @param r any integer.
 * @return \a n choose \a r.
 */
inline constexpr int choose(int n, int r) {
    return (r < 0 || r > n ? 0 :
            r == 0 || r == n ? 1 :
            choose(n-1, r) + choose(n-1, r-1));
}

} // namespace regina

#include "generic/detail/simplex.h"

namespace regina {

/**
 * Represents a top-dimensional simplex in a <i>dim</i>-manifold triangulation.
 *
 * For example, for 3-manifolds this class represents a tetrahedron, and for
 * 2-manifolds this class represents a triangle.
 *
 * Top-dimensional simplices cannot exist in isolation (without a
 * triangulation object), and they cannot be created or destroyed directly.
 * Instead, you create and destroy them via the underlying triangulation,
 * by calling routines such as Triangulation<dim>::newSimplex() or
 * Triangulation<dim>::removeSimplex().
 *
 * Amongst other things, this class is used to view and change the gluings
 * between top-dimensional simplices.  For this we number the facets and
 * vertices of each simplex 0,...,\a dim, so that facet \a i is opposite
 * vertex \a i.
 *
 * Each simplex may have an optional description.  This is typically a
 * human-readable piece of text.  Descriptions are not required, and do
 * not need to be unique.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers significant extra functionality.  In order to use these
 * specialised classes, you will need to include the corresponding headers
 * (e.g., dim2/dim2triangle.h for \a dim = 2, or triangulation/ntetrahedron.h
 * for \a dim = 3).  For convenience, there are typedefs available for these
 * specialised classes (such as Dim2Triangle and NTetrahedron respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending the dimension as a suffix
 * (e.g., Simplex2 and Simplex3 for dimensions 2 and 3).
 * The typedefs mentioned above (e.g., Dim2Triangle and NTetrahedron)
 * are also available.  Higher-dimensional classes are not available
 * in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class Simplex : public detail::SimplexBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Simplex<dim> "
        "should not be used for Regina's standard dimensions.");

    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(Triangulation<dim>* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(const std::string& desc, Triangulation<dim>* tri);

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

// Note that some of our simplex classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Simplex<2>;
template <> class Simplex<3>;

/*@}*/

// Inline functions for Simplex

template <int dim>
inline Simplex<dim>::Simplex(Triangulation<dim>* tri) :
        detail::SimplexBase<dim>(tri) {
}

template <int dim>
inline Simplex<dim>::Simplex(const std::string& desc,
        Triangulation<dim>* tri) :
        detail::SimplexBase<dim>(desc, tri) {
}

} // namespace regina

#endif

