
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file triangulation/alias/isomorphism.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __REGINA_ALIAS_ISOMORPHISM_H
#ifndef __DOXYGEN
#define __REGINA_ALIAS_ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "maths/perm.h"

namespace regina {
    template <int> class Perm;
}

namespace regina::alias {

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(size_t) and facetPerm(size_t), where
 * reasonable, for isomorphisms in dimension \a dim.
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(size_t)</tt>,
 * <tt>int simpImage(size_t) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(size_t)</tt> and
 * <tt>Perm<dim+1> facetPerm(size_t) const</tt>.
 *
 * The names of the aliases are determined by the dimension \a dim,
 * and these aliases are only provided for sufficiently small \a dim.
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class IsomorphismImage {
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(size_t) and facetPerm(size_t).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(size_t)</tt>,
 * <tt>int simpImage(size_t) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(size_t)</tt> and
 * <tt>Perm<dim+1> facetPerm(size_t) const</tt>.
 *
 * \ingroup alias
 */
template <class Derived>
class IsomorphismImage<Derived, 2> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t& triImage(size_t sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t triImage(size_t sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<3>& edgePerm(size_t sourceSimp) {
            return static_cast<Derived*>(this)->facetPerm(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<3> edgePerm(size_t sourceSimp) const {
            return static_cast<const Derived*>(this)->facetPerm(sourceSimp);
        }
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(size_t) and facetPerm(size_t).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(size_t)</tt>,
 * <tt>int simpImage(size_t) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(size_t)</tt> and
 * <tt>Perm<dim+1> facetPerm(size_t) const</tt>.
 *
 * \ingroup alias
 */
template <class Derived>
class IsomorphismImage<Derived, 3> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t& tetImage(size_t sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t tetImage(size_t sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<4>& facePerm(size_t sourceSimp) {
            return static_cast<Derived*>(this)->facetPerm(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<4> facePerm(size_t sourceSimp) const {
            return static_cast<const Derived*>(this)->facetPerm(sourceSimp);
        }
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(size_t).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(size_t)</tt> and
 * <tt>int simpImage(size_t) const</tt>.
 *
 * \ingroup alias
 */
template <class Derived>
class IsomorphismImage<Derived, 4> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t& pentImage(size_t sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        ssize_t pentImage(size_t sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
};

} // namespace regina::alias

#endif

