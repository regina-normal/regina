
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/generic/face.h
 *  \brief Internal header for faces of triangulations of arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_FACE_H
#ifndef __DOXYGEN
#define __REGINA_FACE_H
#endif

#include "triangulation/detail/face.h"
#include "triangulation/generic/faceembedding.h"

namespace regina {

/**
 * Represents a <i>subdim</i>-face in the skeleton of a <i>dim</i>-dimensional
 * triangulation.  There are two substantially different cases:
 *
 * - The case \a subdim \< \a dim represents a lower-dimensional face
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
 * range-based \c for loop:
 *
 * \code{.cpp}
 * for (auto& emb : face) { ... }
 * \endcode
 *
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
 * Faces do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending dimensions \a dim and \a subdim as
 * suffices (e.g., Face2_1 and Face3_0 for the two examples above).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and <i>dim</i> inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim, int subdim>
class Face : public detail::FaceBase<dim, subdim> {
    static_assert(dim == 2 || dim > 4,
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
         * \param component the component of the underlying triangulation
         * to which the new face belongs.
         */
        Face(Component<dim>* component);

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

#ifdef __APIDOCS
// This type alias is already defined in trianguation/forward.h.
/**
 * Refers to a vertex of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to a vertex of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 0>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Vertex8).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
requires (supportedDim(dim))
using Vertex = Face<dim, 0>;

/**
 * Refers to an edge of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to an edge of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 1>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Edge5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
requires (supportedDim(dim))
using Edge = Face<dim, 1>;

/**
 * Refers to a triangular face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 2, where it refers to
 * a top-dimensional simplex of a 2-dimensional triangulation.
 *
 * This is the preferred way to refer to a 2-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 2>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Triangle4).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
requires (supportedDim(dim))
using Triangle = Face<dim, 2>;

/**
 * Refers to a tetrahedral face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 3, where it refers to
 * a top-dimensional simplex of a 3-dimensional triangulation.
 *
 * This is the preferred way to refer to a 3-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 3>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c Tetrahedron7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 3 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
requires (supportedDim(dim) && dim >= 3)
using Tetrahedron = Face<dim, 3>;

/**
 * Refers to a pentachoron face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 4, where it refers to
 * a top-dimensional simplex of a 4-dimensional triangulation.
 *
 * This is the preferred way to refer to a 4-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 4>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g.,
 * \c Pentachoron9).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 4 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
requires (supportedDim(dim) && dim >= 4)
using Pentachoron = Face<dim, 4>;
#endif // __DOXYGEN

// Inline functions for Face

template <int dim, int subdim>
inline Face<dim, subdim>::Face(Component<dim>* component) :
        detail::FaceBase<dim, subdim>(component) {
}

} // namespace regina

#endif

