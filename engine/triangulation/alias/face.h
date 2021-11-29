
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file triangulation/alias/face.h
 *  \brief Provides dimension-specific aliases for dimension-agnostic routines.
 */

#ifndef __REGINA_ALIAS_FACE_H
#ifndef __DOXYGEN
#define __REGINA_ALIAS_FACE_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"

namespace regina {
    template <int> class Perm;
}

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
namespace regina::alias {

/**
 * \defgroup alias Dimension-Specific Aliases
 * Dimension-specific aliases for generic code
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
 *
 * \ingroup alias
 */
template <class Derived, int dim, int maxdim = dim - 1>
class FaceOfTriangulation : public FaceOfTriangulation<Derived, dim, 3> {
    static_assert(maxdim >= 4);

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
         * A dimension-specific alias for face<4>().
         *
         * See face() for further information.
         */
        Face<dim, 4>* pentachoron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<4>(i);
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
 *
 * \ingroup alias
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
         * A dimension-specific alias for face<3>().
         *
         * See face() for further information.
         */
        Face<dim, 3>* tetrahedron(size_t i) const {
            return static_cast<const Derived*>(this)->template face<3>(i);
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
 *
 * \ingroup alias
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
         * A dimension-specific alias for face<2>().
         *
         * See face() for further information.
         */
        Face<dim, 2>* triangle(size_t i) const {
            return static_cast<const Derived*>(this)->template face<2>(i);
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
 *
 * \ingroup alias
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
         * A dimension-specific alias for face<1>().
         *
         * See face() for further information.
         */
        Face<dim, 1>* edge(size_t i) const {
            return static_cast<const Derived*>(this)->template face<1>(i);
        }
};

/**
 * Helper class that provides dimension-specific aliases for
 * countFaces<0>() and face<0>(size_t).
 *
 * This is inherited by the class \a Derived, which must provide template
 * functions of the form <tt>size_t countFaces<subdim>()</tt>
 * and <tt>Face<dim, subdim>* face<subdim>(size_t) const</tt>,
 * for facial dimension \a subdim = 0.
 *
 * \ingroup alias
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
         * A dimension-specific alias for face<0>().
         *
         * See face() for further information.
         */
        Face<dim, 0>* vertex(size_t i) const {
            return static_cast<const Derived*>(this)->template face<0>(i);
        }
};

/**
 * Helper class that provides a dimension-specific alias for
 * faces<subdim>(), where reasonable, for facial dimensions
 * \a subdim &le; \a maxdim.
 *
 * This is inherited by the class \a Derived, which must provide a template
 * function of the form <tt>faces<subdim>() const</tt>,
 * again for all facial dimensions \a subdim &le; \a maxdim.
 * This function should return a const reference to whatever data type
 * is used by \a Derived to store its <tt>subdim</tt>-dimensional faces.
 *
 * The names of the aliases are determined by the facial dimensions \a subdim,
 * and these aliases are only provided for sufficiently small \a subdim.
 *
 * \ingroup alias
 */
template <class Derived, int dim, int maxdim = dim - 1>
class FacesOfTriangulation : public FacesOfTriangulation<Derived, dim, 3> {
    static_assert(maxdim >= 4);

    public:
        /**
         * A dimension-specific alias for faces<4>().
         *
         * See faces() for further information.
         */
        auto pentachora() const {
            return static_cast<const Derived*>(this)->template faces<4>();
        }
};


/**
 * Helper class that provides dimension-specific aliases for faces<0,...,3>().
 *
 * This is inherited by the class \a Derived, which must provide a template
 * function of the form <tt>faces<subdim>() const</tt>,
 * for facial dimensions \a subdim &le; 3.
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class FacesOfTriangulation<Derived, dim, 3> :
        public FacesOfTriangulation<Derived, dim, 2> {
    public:
        /**
         * A dimension-specific alias for faces<3>().
         *
         * See faces() for further information.
         */
        auto tetrahedra() const {
            return static_cast<const Derived*>(this)->template faces<3>();
        }
};

/**
 * Helper class that provides dimension-specific aliases for faces<0,1,2>().
 *
 * This is inherited by the class \a Derived, which must provide a template
 * function of the form <tt>faces<subdim>() const</tt>,
 * for facial dimensions \a subdim &le; 2.
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class FacesOfTriangulation<Derived, dim, 2> :
        public FacesOfTriangulation<Derived, dim, 1> {
    public:
        /**
         * A dimension-specific alias for faces<2>().
         *
         * See faces() for further information.
         */
        auto triangles() const {
            return static_cast<const Derived*>(this)->template faces<2>();
        }
};

/**
 * Helper class that provides dimension-specific aliases for faces<0,1>().
 *
 * This is inherited by the class \a Derived, which must provide a template
 * function of the form <tt>faces<subdim>() const</tt>,
 * for facial dimensions \a subdim &le; 1.
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class FacesOfTriangulation<Derived, dim, 1> :
        public FacesOfTriangulation<Derived, dim, 0> {
    public:
        /**
         * A dimension-specific alias for faces<1>().
         *
         * See faces() for further information.
         */
        auto edges() const {
            return static_cast<const Derived*>(this)->template faces<1>();
        }
};

/**
 * Helper class that provides a dimension-specific alias for faces<0>().
 *
 * This is inherited by the class \a Derived, which must provide a template
 * function of the form <tt>faces<subdim>() const</tt>,
 * for facial dimension \a subdim = 0.
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class FacesOfTriangulation<Derived, dim, 0> {
    public:
        /**
         * A dimension-specific alias for faces<0>().
         *
         * See faces() for further information.
         */
        auto vertices() const {
            return static_cast<const Derived*>(this)->template faces<0>();
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
 *
 * \ingroup alias
 */
template <class Derived, int dim>
class SimplexVoid {
};

/**
 * Helper class that provides a dimension-specific alias for simplex().
 *
 * This is inherited by the class \a Derived, which must provide a
 * routine of the form <tt>Simplex<dim>* simplex() const</tt>.
 *
 * \ingroup alias
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
 *
 * \ingroup alias
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
 *
 * \ingroup alias
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

} // namespace regina::alias

#endif

