
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

/*! \file triangulation/alias/isomorphism.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __ALIAS_ISOMORPHISM_H
#ifndef __DOXYGEN
#define __ALIAS_ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "maths/perm.h"

namespace regina {

template <int> class Perm;

namespace alias {

/**
 * \weakgroup alias
 * @{
 */

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(unsigned) and facetPerm(unsigned), where
 * reasonable, for isomorphisms in dimension \a dim.
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(unsigned)</tt>,
 * <tt>int simpImage(unsigned) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(unsigned)</tt> and
 * <tt>Perm<dim+1> facetPerm(unsigned) const</tt>.
 *
 * The names of the aliases are determined by the dimension \a dim,
 * and these aliases are only provided for sufficiently small \a dim.
 */
template <class Derived, int dim>
class IsomorphismImage {
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(unsigned) and facetPerm(unsigned).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(unsigned)</tt>,
 * <tt>int simpImage(unsigned) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(unsigned)</tt> and
 * <tt>Perm<dim+1> facetPerm(unsigned) const</tt>.
 */
template <class Derived>
class IsomorphismImage<Derived, 2> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& triImage(unsigned sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int triImage(unsigned sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<3>& edgePerm(unsigned sourceSimp) {
            return static_cast<Derived*>(this)->facetPerm(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<3> edgePerm(unsigned sourceSimp) const {
            return static_cast<const Derived*>(this)->facetPerm(sourceSimp);
        }
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(unsigned) and facetPerm(unsigned).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(unsigned)</tt>,
 * <tt>int simpImage(unsigned) const</tt>,
 * <tt>Perm<dim+1>& facetPerm(unsigned)</tt> and
 * <tt>Perm<dim+1> facetPerm(unsigned) const</tt>.
 */
template <class Derived>
class IsomorphismImage<Derived, 3> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& tetImage(unsigned sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int tetImage(unsigned sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<4>& facePerm(unsigned sourceSimp) {
            return static_cast<Derived*>(this)->facetPerm(sourceSimp);
        }
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        Perm<4> facePerm(unsigned sourceSimp) const {
            return static_cast<const Derived*>(this)->facetPerm(sourceSimp);
        }
};

/**
 * Helper class that provides dimension-specific aliases for both const and
 * non-const versions of simpImage(unsigned).
 *
 * This is inherited by the class \a Derived, which must provide functions
 * of the form
 * <tt>int& simpImage(unsigned)</tt> and
 * <tt>int simpImage(unsigned) const</tt>.
 */
template <class Derived>
class IsomorphismImage<Derived, 4> {
    public:
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& pentImage(unsigned sourceSimp) {
            return static_cast<Derived*>(this)->simpImage(sourceSimp);
        }
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int pentImage(unsigned sourceSimp) const {
            return static_cast<const Derived*>(this)->simpImage(sourceSimp);
        }
};

} } // namespace regina::alias

#endif

