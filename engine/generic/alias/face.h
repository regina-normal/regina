
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

/*! \file generic/alias/facenumber.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __ALIAS_FACE_H
#ifndef __DOXYGEN
#define __ALIAS_FACE_H
#endif

#include "regina-core.h"

namespace regina {

template <int, int> class Face;

namespace alias {

/**
 * \weakgroup alias
 * @{
 */

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<subdim>() and face<subdim>(size_t), where reasonable,
 * for facial dimensions \a subdim &le; \a maxdim.
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * again for all facial dimensions \a subdim &le; \a maxdim.
 *
 * The names of the aliases are determined by the facial dimensions \a subdim,
 * and these aliases are only provided for sufficiently small \a subdim.
 */
template <class Derived, int dim, int maxdim = dim - 1>
class FaceOfTriangulation :
        public FaceOfTriangulation<Derived, dim, maxdim - 1> {
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0>() and face<0>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for facial dimension \a subdim = 0.
 */
template <class Derived, int dim>
class FaceOfTriangulation<Derived, dim, 0> {
    public:
        /**
         * A dimension-specific alias for countFaces<0>().
         *
         * See countFaces() for further information.
         */
        size_t countVertices() const {
            return static_cast<const Derived*>(this)->template countFaces<0>();
        }

        /**
         * Deprecated dimension-specific alias for countFaces<0>().
         *
         * \deprecated Simply call countVertices() instead.
         */
        size_t getNumberOfVertices() const {
            return static_cast<const Derived*>(this)->template countFaces<0>();
        }

        /**
         * A dimension-specific alias for face<0>().
         *
         * See face() for further information.
         */
        Face<dim, 0>* vertex(size_t i) const {
            return static_cast<const Derived*>(this)->template face<0>(i);
        }

        /**
         * Deprecated dimension-specific alias for face<0>().
         *
         * \deprecated Simply call vertex() instead.
         */
        Face<dim, 0>* getVertex(size_t i) const {
            return static_cast<const Derived*>(this)->template face<0>(i);
        }
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0,1>() and face<0,1>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for all facial dimensions \a subdim &le; 1.
 */
template <class Derived, int dim>
class FaceOfTriangulation<Derived, dim, 1> :
        public FaceOfTriangulation<Derived, dim, 0> {
    public:
        /**
         * A dimension-specific alias for countFaces<1>().
         *
         * See countFaces() for further information.
         */
        size_t countEdges() const {
            return static_cast<const Derived*>(this)->template countFaces<1>();
        }

        /**
         * Deprecated dimension-specific alias for countFaces<1>().
         *
         * \deprecated Simply call countEdges() instead.
         */
        size_t getNumberOfEdges() const {
            return static_cast<const Derived*>(this)->template countFaces<1>();
        }

        /**
         * A dimension-specific alias for face<1>().
         *
         * See face() for further information.
         */
        Face<dim, 1>* edge(size_t i) const {
            return static_cast<const Derived*>(this)->template face<1>(i);
        }

        /**
         * Deprecated dimension-specific alias for face<1>().
         *
         * \deprecated Simply call edge() instead.
         */
        Face<dim, 1>* getEdge(size_t i) const {
            return static_cast<const Derived*>(this)->template face<1>(i);
        }
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0,1,2>() and face<0,1,2>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for all facial dimensions \a subdim &le; 2.
 */
template <class Derived, int dim>
class FaceOfTriangulation<Derived, dim, 2> :
        public FaceOfTriangulation<Derived, dim, 1> {
    public:
        /**
         * A dimension-specific alias for countFaces<2>().
         *
         * See countFaces() for further information.
         */
        size_t countTriangles() const {
            return static_cast<const Derived*>(this)->template countFaces<2>();
        }

        /**
         * Deprecated dimension-specific alias for countFaces<2>().
         *
         * \deprecated Simply call countTriangles() instead.
         */
        size_t getNumberOfTriangles() const {
            return static_cast<const Derived*>(this)->template countFaces<2>();
        }

        /**
         * A dimension-specific alias for face<2>().
         *
         * See face() for further information.
         */
        Face<dim, 2>* triangle(size_t i) const {
            return static_cast<const Derived*>(this)->template face<2>(i);
        }

        /**
         * Deprecated dimension-specific alias for face<2>().
         *
         * \deprecated Simply call triangle() instead.
         */
        Face<dim, 2>* getTriangle(size_t i) const {
            return static_cast<const Derived*>(this)->template face<2>(i);
        }
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0,...,3>() and face<0,...,3>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for all facial dimensions \a subdim &le; 3.
 */
template <class Derived, int dim>
class FaceOfTriangulation<Derived, dim, 3> :
        public FaceOfTriangulation<Derived, dim, 2> {
    public:
        /**
         * A dimension-specific alias for countFaces<3>().
         *
         * See countFaces() for further information.
         */
        size_t countTetrahedra() const {
            return static_cast<const Derived*>(this)->template countFaces<3>();
        }

        /**
         * Deprecated dimension-specific alias for countFaces<3>().
         *
         * \deprecated Simply call countTetrahedra() instead.
         */
        size_t getNumberOfTetrahedra() const {
            return static_cast<const Derived*>(this)->template countFaces<3>();
        }

        /**
         * A dimension-specific alias for face<3>().
         *
         * See face() for further information.
         */
        Face<dim, 3>* tetrahedron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<3>(i);
        }

        /**
         * Deprecated dimension-specific alias for face<3>().
         *
         * \deprecated Simply call tetrahedron() instead.
         */
        Face<dim, 3>* getTetrahedron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<3>(i);
        }
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0,...,4>() and face<0,...,4>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for all facial dimensions \a subdim &le; 4.
 */
template <class Derived, int dim>
class FaceOfTriangulation<Derived, dim, 4> :
        public FaceOfTriangulation<Derived, dim, 3> {
    public:
        /**
         * A dimension-specific alias for countFaces<4>().
         *
         * See countFaces() for further information.
         */
        size_t countPentachora() const {
            return static_cast<const Derived*>(this)->template countFaces<4>();
        }

        /**
         * Deprecated dimension-specific alias for countFaces<4>().
         *
         * \deprecated Simply call countPentachora() instead.
         */
        size_t getNumberOfPentachora() const {
            return static_cast<const Derived*>(this)->template countFaces<4>();
        }

        /**
         * A dimension-specific alias for face<4>().
         *
         * See face() for further information.
         */
        Face<dim, 4>* pentachoron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<4>(i);
        }

        /**
         * Deprecated dimension-specific alias for face<4>().
         *
         * \deprecated Simply call pentachoron() instead.
         */
        Face<dim, 4>* getPentachoron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<4>(i);
        }
};

} } // namespace regina::alias

#endif

