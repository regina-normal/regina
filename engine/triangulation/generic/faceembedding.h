
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/generic/faceembedding.h
 *  \brief Internal header for embeddings of faces within top-dimensional
 *  simplices.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_FACEEMBEDDING_H
#ifndef __DOXYGEN
#define __REGINA_FACEEMBEDDING_H
#endif

#include "triangulation/detail/face.h"

namespace regina {

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
 * As of Regina 7.0, a FaceEmbedding can happily outlive its face - even if the
 * underlying Face object is destroyed (e.g., because the triangulation
 * changed), if you made a local copy of a FaceEmbedding beforehand then its
 * simplex(), face() and vertices() routines will continue to return the same
 * values as they did before, back when the underlying Face still existed.
 * A FaceEmbedding cannot, however, outlive its top-dimensional simplex,
 * because internally a FaceEmbedding references the Simplex object in which
 * it lives (i.e., it does not just store an integer simplex index).
 *
 * If \a dim is one of Regina's \ref stddim "standard dimensions", then
 * this template is specialised to offer additional dimension-specific aliases.
 * In order to use these specialised classes, you will need to include the
 * corresponding triangulation headers (e.g., triangulation/dim2.h for
 * \a dim = 2, or triangulation/dim3.h for \a dim = 3).
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python Python does not support templates.  Instead this class
 * can be used by appending dimensions \a dim and \a subdim as suffices
 * (e.g., FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples above).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim, int subdim>
class FaceEmbedding : public detail::FaceEmbeddingBase<dim, subdim> {
    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using `operator =`.
         *
         * \nopython This is because the C++ assignment operators are
         * not accessible to Python.
         */
        FaceEmbedding() = default;

        /**
         * Creates a new object containing the given data.
         *
         * \param simplex the top-dimensional simplex in which the
         * underlying <i>subdim</i>-face of the triangulation is contained.
         * \param vertices a mapping from the vertices of the underlying
         * <i>subdim</i>-face of the triangulation to the corresponding
         * vertex numbers of \a simplex.  See FaceEmbeddingBase::vertices()
         * for details of how this permutation should be structured.
         */
        FaceEmbedding(Simplex<dim>* simplex, Perm<dim + 1> vertices);

        /**
         * Creates a new copy of the given object.
         *
         * \param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe) = default;

        /**
         * Sets this to be a copy of the given object.
         *
         * \param cloneMe the object to copy.
         */
        FaceEmbedding& operator = (const FaceEmbedding& cloneMe) = default;

    private:
        /**
         * Explicitly disable the old (\a simplex, \a face) constructor from
         * Regina 6.0.1 and earlier.
         *
         * This is so that, if the user unintentionally calls the old
         * (\a simplex, \a face) constructor, the face argument will not be
         * silently converted to a permutation and passed to the new
         * (\a simplex, \a vertices) constructor instead.
         */
        FaceEmbedding(Simplex<dim>*, int);
};

#ifdef __APIDOCS
// This type alias is already defined in trianguation/forward.h.
/**
 * Details how a vertex of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 0>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c VertexEmbedding3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup generic
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
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c EdgeEmbedding5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup generic
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
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TriangleEmbedding12).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 3 and 15 inclusive.
 *
 * \ingroup generic
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
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TetrahedronEmbedding7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 4 and 15 inclusive.
 *
 * \ingroup generic
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
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c PentachoronEmbedding14).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 5 and 15 inclusive.
 *
 * \ingroup generic
 */
template <int dim>
using PentachoronEmbedding = FaceEmbedding<dim, 4>;
#endif // __DOXYGEN

// Inline functions for FaceEmbedding

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        Simplex<dim>* simplex, Perm<dim + 1> vertices) :
        detail::FaceEmbeddingBase<dim, subdim>(simplex, vertices) {
}

} // namespace regina

#endif

