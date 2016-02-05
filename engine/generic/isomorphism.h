
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

/*! \file generic/isomorphism.h
 *  \brief Deals with combinatorial isomorphisms between triangulations.
 */

#ifndef __ISOMORPHISM_H
#ifndef __DOXYGEN
#define __ISOMORPHISM_H
#endif

#include "generic/detail/isomorphism.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one <i>dim</i>-manifold
 * triangulation into another.
 *
 * In essence, a combinatorial isomorphism from triangulation \a T to
 * triangulation \a U is a one-to-one map from the simplices of \a T to the
 * simplices of \a U that allows relabelling of both the simplices and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent simplices.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map \a f
 * from the simplices of \a T to the simplices of \a U, and (ii) for each
 * simplex \a S of \a T, a permutation \a f<sub>S</sub> of the facets
 * (0,...,\a dim) of \a S, for which the following condition holds:
 *
 *   - If facet \a k of simplex \a S and facet \a k' of simplex \a S'
 *     are identified in \a T, then facet \a f<sub>S</sub>(\a k) of \a f(S)
 *     and facet \a f<sub>S'</sub>(\a k') of \a f(S') are identified in \a U.
 *     Moreover, their gluing is consistent with the facet/vertex permutations;
 *     that is, there is a commutative square involving the gluing maps in
 *     \a T and \a U and the permutations \a f<sub>S</sub> and
 *     \a f<sub>S'</sub>.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If facet \a x is a boundary facet of \a T then facet \a f(x) is a
 *     boundary facet of \a U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation \a T is present as an entire component (or components) of \a U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation \a T as a subcomplex of some possibly larger component
 * (or components) of \a U.
 *
 * Note that for all types of isomorphism, triangulation \a U is allowed
 * to contain more simplices than triangulation \a T.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers some extra functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers
 * (e.g., dim2/dim2isomorphism.h for \a dim = 2, or
 * triangulation/nisomorphism.h for \a dim = 3).  For convenience, there
 * are typedefs available for these specialised classes (such as
 * Dim2Isomorphism and NIsomorphism respectively).
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Isomorphism2 and Isomorphism3 for dimensions 2 and 3).
 * The typedefs mentioned above for standard dimensions
 * (e.g., Dim2Isomorphism and NIsomorphism) are also available.
 *
 * \tparam dim The dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class Isomorphism : public detail::IsomorphismBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Isomorphism<dim> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \ifacespython Not present.
         *
         * @param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nSimplices);
        /**
         * Creates a copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& copy);
};

// Note that some of our isomorphism classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Isomorphism<2>;
template <> class Isomorphism<3>;
template <> class Isomorphism<4>;

/*@}*/

// Inline functions for Isomorphism

template <int dim>
inline Isomorphism<dim>::Isomorphism(unsigned nSimplices) :
        detail::IsomorphismBase<dim>(nSimplices) {
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(const Isomorphism& copy) :
        detail::IsomorphismBase<dim>(copy) {
}

} // namespace regina

#endif

