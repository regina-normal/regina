
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

/*! \file triangulation/alias/facenumber.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __ALIAS_FACENUMBER_H
#ifndef __DOXYGEN
#define __ALIAS_FACENUMBER_H
#endif

#include "regina-core.h"

namespace regina {
namespace alias {

/**
 * \weakgroup alias
 * @{
 */

/**
 * Helper class that provides a dimension-specific alias for face(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 *
 * The name of the alias is determined by the dimension \a subdim, and
 * the alias is only provided for those dimensions \a subdim that are
 * sufficiently small.
 */
template <class Derived, int subdim>
class FaceNumber {
};

/**
 * Helper class that provides a dimension-specific alias for face().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 */
template <class Derived>
class FaceNumber<Derived, 0> {
    public:
        /**
         * A dimension-specific alias for face().
         *
         * See face() for further information.
         */
        int vertex() const {
            return static_cast<const Derived*>(this)->face();
        }
};

/**
 * Helper class that provides a dimension-specific alias for face().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 */
template <class Derived>
class FaceNumber<Derived, 1> {
    public:
        /**
         * A dimension-specific alias for face().
         *
         * See face() for further information.
         */
        int edge() const {
            return static_cast<const Derived*>(this)->face();
        }
};

/**
 * Helper class that provides a dimension-specific alias for face().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 */
template <class Derived>
class FaceNumber<Derived, 2> {
    public:
        /**
         * A dimension-specific alias for face().
         *
         * See face() for further information.
         */
        int triangle() const {
            return static_cast<const Derived*>(this)->face();
        }
};

/**
 * Helper class that provides a dimension-specific alias for face().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 */
template <class Derived>
class FaceNumber<Derived, 3> {
    public:
        /**
         * A dimension-specific alias for face().
         *
         * See face() for further information.
         */
        int tetrahedron() const {
            return static_cast<const Derived*>(this)->face();
        }
};

/**
 * Helper class that provides a dimension-specific alias for face().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>int face() const</tt>.
 */
template <class Derived>
class FaceNumber<Derived, 4> {
    public:
        /**
         * A dimension-specific alias for face().
         *
         * See face() for further information.
         */
        int pentachoron() const {
            return static_cast<const Derived*>(this)->face();
        }
};

} } // namespace regina::alias

#endif

