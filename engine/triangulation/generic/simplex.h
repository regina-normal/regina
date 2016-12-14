
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

/*! \file triangulation/generic/simplex.h
 *  \brief Internal header for top-dimensional simplices of triangulations of
 *  arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __SIMPLEX_H
#ifndef __DOXYGEN
#define __SIMPLEX_H
#endif

#include "triangulation/detail/simplex.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Represents a top-dimensional simplex in a <i>dim</i>-manifold triangulation.
 *
 * For example, for 3-manifolds this class represents a tetrahedron, and for
 * 2-manifolds this class represents a triangle.
 *
 * Although this is a specialisation of the Face class template, this
 * class is typically referred to using the alias Simplex<dim>.
 * For Regina's \ref stddim "standard dimensions", you can also use the
 * aliases Triangle<2>, Tetrahedron<3> and Pentachoron<4>.
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
 * specialised classes, you will need to include the corresponding
 * triangulation headers (e.g., triangulation/dim2.h for \a dim = 2, or
 * triangulation/dim3.h for \a dim = 3).
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Simplex2 and Simplex3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim>
class Face<dim, dim> : public detail::SimplexBase<dim> {
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
        Face(Triangulation<dim>* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        Face(const std::string& desc, Triangulation<dim>* tri);

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

#ifdef __DOXYGEN
// This type alias is already defined in trianguation/forward.h.
/**
 * Refers to a top-dimensional simplex in a <i>dim</i>-dimensional
 * triangulation.
 *
 * This is the preferred way to refer to a top-dimensional simplex (as
 * opposed to the more clumsy notation Face<dim, dim>).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using Simplex = Face<dim, dim>;
#endif

/*@}*/

// Inline functions for Simplex

template <int dim>
inline Face<dim, dim>::Face(Triangulation<dim>* tri) :
        detail::SimplexBase<dim>(tri) {
}

template <int dim>
inline Face<dim, dim>::Face(const std::string& desc,
        Triangulation<dim>* tri) :
        detail::SimplexBase<dim>(desc, tri) {
}

} // namespace regina

#endif

