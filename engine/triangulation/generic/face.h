
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

/*! \file triangulation/generic/face.h
 *  \brief Internal header for faces of triangulations of arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __FACE_H
#ifndef __DOXYGEN
#define __FACE_H
#endif

#include "triangulation/detail/face.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Details how a <i>subdim</i>-face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * For small-dimensional faces, this class is typically described using
 * dimension-specific type aliases: VertexEmbedding<dim>, EdgeEmbedding<dim>,
 * TriangleEmbedding<dim>, TetrahedronEmbedding<dim> and
 * PentachoronEmbedding<dim> refer to the cases
 * \a subdim = 0, 1, 2, 3 and 4 respectively.
 *
 * For a <i>dim</i>-dimensional triangulation \a T, each <i>subdim</i>-face
 * \a F typically belongs to many top-dimensional simplices of \a T,
 * and therefore has many associated FaceEmbedding objects.  These individual
 * FaceEmbedding objects correspond to the top-dimensional simplices of the
 * link of \a F (which is a (\a dim - \a subdim - 1)-dimensional triangulation).
 *
 * If \a dim is one of Regina's \ref stddim "standard dimensions", then
 * this template is specialised to offer additional dimension-specific aliases.
 * In order to use these specialised classes, you will need to include the
 * corresponding triangulation headers (e.g., triangulation/dim2.h for
 * \a dim = 2, or triangulation/dim3.h for \a dim = 3).
 *
 * \ifacespython Python does not support templates.  Instead this class
 * can be used by appending dimensions \a dim and \a subdim as suffices
 * (e.g., FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples above).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim, int subdim>
class FaceEmbedding : public detail::FaceEmbeddingBase<dim, subdim> {
    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        FaceEmbedding();

        /**
         * Creates a new object containing the given data.
         *
         * @param simplex the top-dimensional simplex in which the
         * underlying <i>subdim</i>-face of the triangulation is contained.
         * @param face the corresponding face number of \a simplex.
         * This must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         */
        FaceEmbedding(Simplex<dim>* simplex, int face);

        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe);
};

/**
 * Represents a <i>subdim</i>-face in the skeleton of a <i>dim</i>-dimensional
 * triangulation.  There are two substantially different cases:
 *
 * - The case \a subdim &lt; \a dim represents a lower-dimensional face
 *   in a triangulation.  This is implemented by the generic class
 *   template (as documented here), and is specialised for Regina's
 *   \ref stddim "standard dimensions" (as discussed below).
 *
 * - The case \a subdim = \a dim represents a top-dimensional simplex in
 *   a triangulation.  This class has a very different interface, and is
 *   implemented by the partial specialisation Face<dim, dim> (and again
 *   specialised further for \ref stddim "standard dimensions").   This
 *   case is typically referred to using the type alias Simplex<dim>, to
 *   make the distinction clear.  See the documentation for the specialisation
 *   Face<dim, dim> for details on the interface that it provides.
 *
 * For small-dimensional faces, this class is typically described using
 * dimension-specific type aliases: Vertex<dim>, Edge<dim>, Triangle<dim>,
 * Tetrahedron<dim> and Pentachoron<dim> refer to the cases
 * \a subdim = 0, 1, 2, 3 and 4 respectively.
 *
 * A given <i>subdim</i>-face \a F of the triangulation may appear many
 * times within the various top-dimensional simplices of the underlying
 * triangulation.  As an extreme example, in a 1-vertex triangulation of
 * a 3-manifold, the single vertex makes 4<i>n</i> such appearances,
 * where \a n is the total number of tetrahedra.
 *
 * Each such appearance is described by a single FaceEmbedding object.  You can
 * iterate through these appearances using begin() and end(), or using a
 * C++11 range-based \c for loop: <tt>for (auto& emb : F) { ... }</tt>.
 * You can count these appearances by calling degree(), and you can also
 * examine them using routines such as front(), back() and embedding().
 *
 * \warning Face objects are highly temporary: whenever a triangulation
 * changes, all its face objects will be deleted and new ones will be
 * created in their place.
 *
 * For some types of faces in dimensions \a dim = 3 and 4, this template is
 * specialised to offer additional functionality.  In order to use these
 * specialised classes, you will need to include the corresponding triangulation
 * headers (triangulation/dim3.h or triangulation/dim4.h respectively).
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending dimensions \a dim and \a subdim as
 * suffices (e.g., Face2_1 and Face3_0 for the two examples above).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and <i>dim</i> inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim, int subdim>
class Face : public detail::FaceBase<dim, subdim> {
    static_assert(dim == 2 || dim > 4 ||
        (dim == 3 && subdim == 1) ||
        (dim == 4 && subdim >= 2 && subdim <= 3),
        "The generic implementation of Face<dim, subdim> "
        "should not be used for those face class that are specialised "
        "in Regina's standard dimensions.");
    static_assert(subdim < dim,
        "The generic implementation of Face<dim, subdim> "
        "should only be used for the case subdim < dim.");

    protected:
        /**
         * Creates a new face.
         *
         * @param component the component of the underlying triangulation
         * to which the new face belongs.
         */
        Face(Component<dim>* component);

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

#ifdef __DOXYGEN
// This type alias is already defined in trianguation/forward.h.
/**
 * Details how a vertex of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 0>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c VertexEmbedding3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using VertexEmbedding = FaceEmbedding<dim, 0>;

/**
 * Details how a edge of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 1>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c EdgeEmbedding5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using EdgeEmbedding = FaceEmbedding<dim, 1>;

/**
 * Details how a triangular face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 2>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TriangleEmbedding12).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 3 and 15 inclusive.
 */
template <int dim>
using TriangleEmbedding = FaceEmbedding<dim, 2>;

/**
 * Details how a tetrahedral face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 3>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TetrahedronEmbedding7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 4 and 15 inclusive.
 */
template <int dim>
using TetrahedronEmbedding = FaceEmbedding<dim, 3>;

/**
 * Details how a pentachoron face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 4>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c PentachoronEmbedding14).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 5 and 15 inclusive.
 */
template <int dim>
using PentachoronEmbedding = FaceEmbedding<dim, 4>;

/**
 * Refers to a vertex of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to a vertex of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 0>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Vertex8).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using Vertex = Face<dim, 0>;

/**
 * Refers to an edge of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to an edge of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 1>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Edge5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using Edge = Face<dim, 1>;

/**
 * Refers to a triangular face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 2, where it refers to
 * a top-dimensional simplex of a 2-dimensional triangulation.
 *
 * This is the preferred way to refer to a 2-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 2>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Triangle4).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
using Triangle = Face<dim, 2>;

/**
 * Refers to a tetrahedral face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 3, where it refers to
 * a top-dimensional simplex of a 3-dimensional triangulation.
 *
 * This is the preferred way to refer to a 3-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 3>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c Tetrahedron7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 3 and 15 inclusive.
 */
template <int dim>
using Tetrahedron = Face<dim, 3>;

/**
 * Refers to a pentachoron face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 4, where it refers to
 * a top-dimensional simplex of a 4-dimensional triangulation.
 *
 * This is the preferred way to refer to a 4-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 4>).
 *
 * \ifacespython Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g.,
 * \c Pentachoron9).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 4 and 15 inclusive.
 */
template <int dim>
using Pentachoron = Face<dim, 4>;
#endif // __DOXYGEN

/*@}*/

// Inline functions for FaceEmbedding

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding() {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        Simplex<dim>* simplex, int face) :
        detail::FaceEmbeddingBase<dim, subdim>(simplex, face) {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        const FaceEmbedding& cloneMe) :
        detail::FaceEmbeddingBase<dim, subdim>(cloneMe) {
}

// Inline functions for Face

template <int dim, int subdim>
inline Face<dim, subdim>::Face(Component<dim>* component) :
        detail::FaceBase<dim, subdim>(component) {
}

} // namespace regina

#endif

