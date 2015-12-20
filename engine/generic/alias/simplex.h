
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

/* end stub */

/*! \file generic/alias/simplex.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __ALIAS_SIMPLEX_H
#ifndef __DOXYGEN
#define __ALIAS_SIMPLEX_H
#endif

#include "regina-core.h"

namespace regina {

template <int> class Simplex;

namespace alias {

/**
 * \weakgroup alias
 * @{
 */

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t),
 * where reasonable.
 *
 * This is inherited by the class \a Derived:
 *
 * - If \a useConst is \c false, then \a Derived must provide a routine of the
 *   form <tt>Simplex<dim>* simplex(size_t) const</tt>.
 * - If \a useConst is \c true, then \a Derived must provide two routines of
 *   the forms <tt>Simplex<dim>* simplex(size_t)</tt> and
 *   <tt>const Simplex<dim>* simplex(size_t) const</tt>.
 *
 * The names of the aliases are determined by the dimension \a dim, and
 * the aliases are only provided for those dimensions \a dim that are
 * sufficiently small.
 */
template <class Derived, int dim, bool useConst>
class SimplexAt {
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide a routine of
 * the form <tt>Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 2, false> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<2>* triangle(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call triangle() instead.
         */
        regina::Simplex<2>* getTriangle(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide two routines
 * of the forms <tt>Simplex<dim>* simplex(size_t)</tt> and
 * <tt>const Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 2, true> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<2>* triangle(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        const regina::Simplex<2>* triangle(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call triangle() instead.
         */
        regina::Simplex<2>* getTriangle(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call triangle() instead.
         */
        const regina::Simplex<2>* getTriangle(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide a routine of
 * the form <tt>Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 3, false> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<3>* tetrahedron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call tetrahedron() instead.
         */
        regina::Simplex<3>* getTetrahedron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide two routines
 * of the forms <tt>Simplex<dim>* simplex(size_t)</tt> and
 * <tt>const Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 3, true> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<3>* tetrahedron(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        const regina::Simplex<3>* tetrahedron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call tetrahedron() instead.
         */
        regina::Simplex<3>* getTetrahedron(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call tetrahedron() instead.
         */
        const regina::Simplex<3>* getTetrahedron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide a routine of
 * the form <tt>Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 4, false> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<4>* pentachoron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call pentachoron() instead.
         */
        regina::Simplex<4>* getPentachoron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides dimension-specific aliases for simplex(size_t).
 *
 * This is inherited by the class \a Derived, which must provide two routines
 * of the forms <tt>Simplex<dim>* simplex(size_t)</tt> and
 * <tt>const Simplex<dim>* simplex(size_t) const</tt>.
 */
template <class Derived>
class SimplexAt<Derived, 4, true> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<4>* pentachoron(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        const regina::Simplex<4>* pentachoron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call pentachoron() instead.
         */
        regina::Simplex<4>* getPentachoron(size_t index) {
            return static_cast<Derived*>(this)->simplex(index);
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call pentachoron() instead.
         */
        const regina::Simplex<4>* getPentachoron(size_t index) const {
            return static_cast<const Derived*>(this)->simplex(index);
        }
};

/**
 * Helper class that provides a dimension-specific alias for simplex(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>Simplex<dim>* simplex() const</tt>.
 *
 * The name of the alias is determined by the dimension \a dim, and
 * the alias is only provided for those dimensions \a dim that are
 * sufficiently small.
 */
template <class Derived, int dim>
class SimplexVoid {
};

/**
 * Helper class that provides a dimension-specific alias for simplex().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>Simplex<dim>* simplex() const</tt>.
 */
template <class Derived>
class SimplexVoid<Derived, 2> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<2>* triangle() const {
            return static_cast<const Derived*>(this)->simplex();
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call triangle() instead.
         */
        regina::Simplex<2>* getTriangle() const {
            return static_cast<const Derived*>(this)->simplex();
        }
};

/**
 * Helper class that provides a dimension-specific alias for simplex().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>Simplex<dim>* simplex() const</tt>.
 */
template <class Derived>
class SimplexVoid<Derived, 3> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<3>* tetrahedron() const {
            return static_cast<const Derived*>(this)->simplex();
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call tetrahedron() instead.
         */
        regina::Simplex<3>* getTetrahedron() const {
            return static_cast<const Derived*>(this)->simplex();
        }
};

/**
 * Helper class that provides a dimension-specific alias for simplex().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>Simplex<dim>* simplex() const</tt>.
 */
template <class Derived>
class SimplexVoid<Derived, 4> {
    public:
        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        regina::Simplex<4>* pentachoron() const {
            return static_cast<const Derived*>(this)->simplex();
        }

        /**
         * Deprecated dimension-specific alias for simplex().
         *
         * \deprecated Simply call pentachoron() instead.
         */
        regina::Simplex<4>* getPentachoron() const {
            return static_cast<const Derived*>(this)->simplex();
        }
};

} } // namespace regina::alias

#endif

