
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

/*! \file generic/face.h
 *  \brief Deals with lower-dimensional faces of triangulations.
 */

#ifndef __FACE_H
#ifndef __DOXYGEN
#define __FACE_H
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/nperm.h"
#include <boost/noncopyable.hpp>

namespace regina {

template <int dim> class Simplex;
template <int dim> class Triangulation;

/**
 * \weakgroup generic
 * @{
 */

/**
 * Helper class that provides core functionality for describing how a
 * \a subdim-face of a \a dim-dimensional triangulation appears within each
 * top-dimensional simplex.
 *
 * Each such appearance is described by a FaceEmbedding<dim, subdim> object,
 * which uses this as a base class.  End users should not need to refer
 * to FaceEmbeddingBase directly.
 *
 * See the FaceEmbedding template class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * FaceEmbedding<dim, subdim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class FaceEmbeddingBase {
    static_assert(dim >= 2, "FaceEmbedding requires dimension >= 2.");
    static_assert(0 <= subdim && subdim < dim,
        "FaceEmbedding requires 0 <= subdimension < dimension.");

    private:
        Simplex<dim>* simplex_;
            /**< The top-dimensional simplex in which the underlying
             * \a subdim-face of the triangulation is contained. */
        int face_;
            /**< The corresponding face number of \a simplex_. */

    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        FaceEmbeddingBase();
        /**
         * Creates a new object containing the given data.
         *
         * @param simplex the top-dimensional simplex in which the
         * underlying \a subdim-face of the triangulation is contained.
         * @param face the corresponding face number of \a simplex.
         * This must be between 0 and (\a dim+1 choose \a subdim+1)-1 inclusive.
         */
        FaceEmbeddingBase(Simplex<dim>* simplex, int face);
        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbeddingBase(const FaceEmbeddingBase& cloneMe);

        /**
         * Makes this a copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbeddingBase& operator = (const FaceEmbeddingBase& cloneMe);

        /**
         * Returns the top-dimensional simplex in which the underlying
         * \a subdim-face of the triangulation is contained.
         *
         * @return the top-dimensional simplex.
         */
        Simplex<dim>* getSimplex() const;

        /**
         * Returns the corresponding face number of getSimplex().
         * This identifies which face of the top-dimensional simplex
         * getSimplex() refers to the underlying \a subdim-face of the
         * triangulation.
         *
         * @return the corresponding face number of the top-dimensional simplex.
         * This will be between 0 and (\a dim+1 choose \a subdim+1)-1 inclusive.
         */
        int getFace() const;

        /**
         * Maps vertices (0,...,\a subdim) of the underlying \a subdim-face
         * of the triangulation to the corresponding vertex numbers of
         * getSimplex().
         *
         * If the link of the underlying \a subdim-face is orientable,
         * then this permutation also maps (\a subdim+1, ..., \a dim) to the
         * remaining vertex numbers of getSimplex() in a manner that
         * preserves orientation as you walk through the many different
         * FaceEmbedding objects for the same underlying \a subdim-face.
         *
         * This routine returns the same permutation as
         * <tt>getSimplex().getFaceMapping<subdim>(getFace())</tt>.
         * See Simplex<dim>::getFaceMapping() for details.
         *
         * @return a mapping from the vertices of the underlying
         * \a subdim-face to the corresponding vertices of getSimplex().
         */
        NPerm<dim+1> getVertices() const;

        /**
         * Tests whether this and the given object are identical.
         * Here "identical" means that they refer to the same face of
         * the same top-dimensional simplex.
         *
         * @param rhs the object to compare with this.
         * @return \c true if and only if both object are identical.
         */
        bool operator == (const FaceEmbeddingBase& rhs) const;

        /**
         * Tests whether this and the given object are different.
         * Here "different" means that they do not refer to the same face of
         * the same top-dimensional simplex.
         *
         * @param rhs the object to compare with this.
         * @return \c true if and only if both object are identical.
         */
        bool operator != (const FaceEmbeddingBase& rhs) const;
};

/**
 * Details how a \a subdim-face of a \a dim-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * For a \a dim-dimensional triangulation \a T, each \a subdim-face \a F
 * typically belongs to many top-dimensional simplices of \a T,
 * and therefore has many associated FaceEmbedding objects.  These individual
 * FaceEmbedding objects correspond to the top-dimensional simplices of the
 * link of \a F (which is a (\a dim - \a subdim - 1)-dimensional triangulation).
 *
 * If \a dim is one of Regina's \ref stddim "standard dimensions", then
 * this template is specialised to offer additional dimension-specific
 * aliases.  In order to use these specialised classes, you will need to
 * include the corresponding headers (e.g., dim2/dim2edge.h for
 * (\a dim, \a subdim) = (2, 1), or triangulation/nvertex.h for
 * (\a dim, \a subdim) = (3, 0)).  For convenience, there are typedefs
 * for these specialised classes (such as Dim2EdgeEmbedding and
 * NVertexEmbedding respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending dimensions \a dim and \a subdim as
 * suffices (e.g., FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples
 * above).  The typedefs mentioned above (e.g., Dim2EdgeEmbedding and
 * NVertexEmbedding) are also available.  Higher-dimensional classes are
 * not available in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class FaceEmbedding : public FaceEmbeddingBase<dim, subdim> {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceEmbedding<dim, subdim> "
        "should not be used for Regina's standard dimensions.");

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
         * underlying \a subdim-face of the triangulation is contained.
         * @param face the corresponding face number of \a simplex.
         * This must be between 0 and (\a dim+1 choose \a subdim+1)-1 inclusive.
         */
        FaceEmbedding(Simplex<dim>* simplex, int face);

        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe);
};

// Note that some of our face-related classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class FaceEmbedding<2, 1>;
template <> class FaceEmbedding<2, 0>;
template <> class FaceEmbedding<3, 2>;
template <> class FaceEmbedding<3, 1>;
template <> class FaceEmbedding<3, 0>;

/*@}*/

// Inline functions for FaceEmbeddingBase

template <int dim, int subdim>
inline FaceEmbeddingBase<dim, subdim>::FaceEmbeddingBase() :
        simplex_(0), face_(0) {
}

template <int dim, int subdim>
inline FaceEmbeddingBase<dim, subdim>::FaceEmbeddingBase(
        Simplex<dim>* simplex, int face) :
        simplex_(simplex), face_(face) {
}

template <int dim, int subdim>
inline FaceEmbeddingBase<dim, subdim>::FaceEmbeddingBase(
        const FaceEmbeddingBase& cloneMe) :
        simplex_(cloneMe.simplex_), face_(cloneMe.face_) {
}

template <int dim, int subdim>
FaceEmbeddingBase<dim, subdim>& FaceEmbeddingBase<dim, subdim>::operator = (
        const FaceEmbeddingBase& cloneMe) {
    simplex_ = cloneMe.simplex_;
    face_ = cloneMe.face_;
    return *this;
}

template <int dim, int subdim>
Simplex<dim>* FaceEmbeddingBase<dim, subdim>::getSimplex() const {
    return simplex_;
}

template <int dim, int subdim>
int FaceEmbeddingBase<dim, subdim>::getFace() const {
    return face_;
}

template <int dim, int subdim>
NPerm<dim+1> FaceEmbeddingBase<dim, subdim>::getVertices() const {
    return simplex_->template getFaceMapping<subdim>(face_);
}

template <int dim, int subdim>
bool FaceEmbeddingBase<dim, subdim>::operator == (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ == rhs.simplex_) && (face_ == rhs.face_));
}

template <int dim, int subdim>
bool FaceEmbeddingBase<dim, subdim>::operator != (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ != rhs.simplex_) || (face_ != rhs.face_));
}

// Inline functions for FaceEmbedding

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding() {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        Simplex<dim>* simplex, int face) :
        FaceEmbeddingBase<dim, subdim>(simplex, face) {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        const FaceEmbedding& cloneMe) :
        FaceEmbeddingBase<dim, subdim>(cloneMe) {
}

} // namespace regina

#endif

