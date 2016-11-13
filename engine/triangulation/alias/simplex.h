
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

/*! \file triangulation/alias/simplex.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __ALIAS_SIMPLEX_H
#ifndef __DOXYGEN
#define __ALIAS_SIMPLEX_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"
#include <vector>

namespace regina {

/**
 * Contains helper classes that provide dimension-specific aliases for
 * generic functions.
 *
 * For example, this namespace provides aliases such as vertex() and edge()
 * for the functions face<0>() and face<1>() in the Simplex class.
 *
 * Regina's main classes (such as Triangulation, Simplex and Face) acquire
 * these aliases through inheritance.  There is typically no need for
 * end users to explicitly refer to the namespace regina::alias.
 */
namespace alias {

/**
 * \addtogroup alias Dimension-Specific Aliases
 * Dimension-specific aliases for generic code
 * @{
 */

/**
 * Helper class that provides dimension-specific aliases for size(),
 * simplices(), hasBoundaryFacets() and countBoundaryFacets(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide routines of
 * the forms <tt>size_t size() const</tt>,
 * <tt>const std::vector<Simplex<dim>*>& simplices() const</tt>,
 * <tt>bool hasBoundaryFacets() const</tt>, and
 * <tt>size_t countBoundaryFacets() const</tt>.
 *
 * The names of the aliases are determined by the dimension \a dim, and
 * the aliases are only provided for those dimensions \a dim that are
 * sufficiently small.
 */
template <class Derived, int dim>
class Simplices {
};

/**
 * Helper class that provides dimension-specific aliases for size(),
 * simplices(), hasBoundaryFacets() and countBoundaryFacets(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide routines of
 * the forms <tt>size_t size() const</tt>,
 * <tt>const std::vector<Simplex<dim>*>& simplices() const</tt>,
 * <tt>bool hasBoundaryFacets() const</tt>, and
 * <tt>size_t countBoundaryFacets() const</tt>.
 */
template <class Derived>
class Simplices<Derived, 2> {
    public:
        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        size_t countTriangles() const {
            return static_cast<const Derived*>(this)->size();
        }

        /**
         * A dimension-specific alias for simplices().
         *
         * See simplices() for further information.
         */
        const std::vector<Simplex<2>*>& triangles() const {
            return static_cast<const Derived*>(this)->simplices();
        }

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryEdges() const {
            return static_cast<const Derived*>(this)->hasBoundaryFacets();
        }

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryEdges() const {
            return static_cast<const Derived*>(this)->countBoundaryFacets();
        }
};

/**
 * Helper class that provides dimension-specific aliases for size(),
 * simplices(), hasBoundaryFacets() and countBoundaryFacets(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide routines of
 * the forms <tt>size_t size() const</tt>,
 * <tt>const std::vector<Simplex<dim>*>& simplices() const</tt>,
 * <tt>bool hasBoundaryFacets() const</tt>, and
 * <tt>size_t countBoundaryFacets() const</tt>.
 */
template <class Derived>
class Simplices<Derived, 3> {
    public:
        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        size_t countTetrahedra() const {
            return static_cast<const Derived*>(this)->size();
        }

        /**
         * A dimension-specific alias for simplices().
         *
         * See simplices() for further information.
         */
        const std::vector<Simplex<3>*>& tetrahedra() const {
            return static_cast<const Derived*>(this)->simplices();
        }

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTriangles() const {
            return static_cast<const Derived*>(this)->hasBoundaryFacets();
        }

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTriangles() const {
            return static_cast<const Derived*>(this)->countBoundaryFacets();
        }
};

/**
 * Helper class that provides dimension-specific aliases for size(),
 * simplices(), hasBoundaryFacets() and countBoundaryFacets(),
 * where reasonable.
 *
 * This is inherited by the class \a Derived, which must provide routines of
 * the forms <tt>size_t size() const</tt>,
 * <tt>const std::vector<Simplex<dim>*>& simplices() const</tt>,
 * <tt>bool hasBoundaryFacets() const</tt>, and
 * <tt>size_t countBoundaryFacets() const</tt>.
 */
template <class Derived>
class Simplices<Derived, 4> {
    public:
        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        size_t countPentachora() const {
            return static_cast<const Derived*>(this)->size();
        }

        /**
         * A dimension-specific alias for simplices().
         *
         * See simplices() for further information.
         */
        const std::vector<Simplex<4>*>& pentachora() const {
            return static_cast<const Derived*>(this)->simplices();
        }

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTetrahedra() const {
            return static_cast<const Derived*>(this)->hasBoundaryFacets();
        }

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTetrahedra() const {
            return static_cast<const Derived*>(this)->countBoundaryFacets();
        }
};

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
};

} } // namespace regina::alias

#endif

