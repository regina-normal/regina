
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
#include "utilities/nmarkedvector.h"
#include <deque>
#include <vector>
#include <boost/noncopyable.hpp>

namespace regina {

template <int dim> class Component;
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

/**
 * Helper class for storing all the ways in which a given face of codimension
 * \a codim in a \a dim-dimensional triangulation appears within the
 * various top-dimensional simplices.
 *
 * In essence, this class provides the data structures which which a
 * Face object stores its list of corresponding FaceEmbedding objects.
 * All the routines in this class are inherited by Face, and so end
 * users should not need to refer to FaceStorage directly.
 *
 * The reason these data structrues belong to a separate class FaceStorage
 * (as opposed to be being integrated directly into Face or FaceBase) is
 * so that Regina can use different data structures for different codimensions.
 *
 * See the Face and FaceEmbedding template class notes for further information.
 *
 * \warning The second template parameter is the \e codimension of the face, not
 * the \e dimension of the face.  This means that Face<dim, subdim> inherits
 * from FaceStorage<dim, dim - codim>.  This inconsistency arises because
 * it is the codimension of the face that determines what data structures
 * we use.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Face<dim, subdim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam codim the codimension of the faces of the underlying triangulation.
 * This must be between 1 and \a dim inclusive.
 */
template <int dim, int codim>
class FaceStorage {
    static_assert(dim >= 2, "Face requires dimension >= 2.");
    static_assert(2 < codim && codim <= dim,
        "The generic FaceStorage should only be used for codimension > 2.");

    private:
        std::vector<FaceEmbedding<dim, dim - codim>> embeddings_;
            /**< The list of all occurrences of a given face within the
                 top-dimensional simplices of the underlying triangulation. */

    public:
        /**
         * Returns the degree of this face.
         * This is the number of different ways in which the face appears
         * within the various top-dimensional simplices of the underlying
         * triangulation.
         *
         * Note that if this face appears multiple times within the same
         * top-dimensional simplex, then it will be counted multiple
         * times by this routine.
         *
         * @return the degree of this face.
         */
        size_t getDegree() const;
        /**
         * Returns one of the ways in which this face appears within a
         * top-dimensional simplex of the underlying triangluation.
         *
         * For convenience, you can also use begin() and end() to
         * iterate through all such appearances.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         *
         * @param index the index of the requested appearance.  This
         * must be between 0 and getDegree()-1 inclusive.
         * @return details of the requested appearance.
         */
        const FaceEmbedding<dim, dim - codim>& getEmbedding(size_t index) const;

        /**
         * A begin function for iterating through all appearances of
         * this face within the various top-dimensional simplices of the
         * underlying triangulation.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         *
         * An iteration from begin() to end() will run through
         * getDegree() appearances in total.
         *
         * \ifacespython Not present.  However, Python users can call
         * the Python-only routine getEmbeddings(), which will return all
         * appearances (from begin() through to end()) in a Python sequence.
         *
         * @return a iterator that points to the first appearance.
         */
        typename std::vector<FaceEmbedding<dim, dim - codim>>::const_iterator
            begin() const;
        /**
         * An end function for iterating through all appearances of
         * this face within the various top-dimensional simplices of the
         * underlying triangulation.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         *
         * An iteration from begin() to end() will run through
         * getDegree() appearances in total.
         *
         * \ifacespython Not present.  However, Python users can call
         * the Python-only routine getEmbeddings(), which will return all
         * appearances (from begin() through to end()) in a Python sequence.
         *
         * @return a "beyond the end" iterator that comes immediately
         * after the last appearance.
         */
        typename std::vector<FaceEmbedding<dim, dim - codim>>::const_iterator
            end() const;

        /**
         * Returns the first appearance of this face within a top-dimensional
         * simplex of the underlying triangluation.
         *
         * This is equivalent to calling <tt>*begin()</tt>, or
         * <tt>getEmbedding(0)</tt>.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         * In particular, for a boundary face of codimension 2, both
         * front() and back() will refer to the two appearances of this
         * face on the (\a dim-1)-dimensional boundary.
         *
         * @return details of the first appearance.
         */
        const FaceEmbedding<dim, dim - codim>& front() const;
        /**
         * Returns the last appearance of this face within a top-dimensional
         * simplex of the underlying triangluation.
         *
         * This is equivalent to calling <tt>getEmbedding(getDegree()-1)</tt>.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         * In particular, for a boundary face of codimension 2, both
         * front() and back() will refer to the two appearances of this
         * face on the (\a dim-1)-dimensional boundary.
         *
         * @return details of the last appearance.
         */
        const FaceEmbedding<dim, dim - codim>& back() const;

    protected:
        /**
         * Internal routine to help build the skeleton of a triangulation.
         *
         * This routine pushes the given object onto the end of the
         * internal list of appearances of this face within
         * top-dimensional simplices.
         *
         * @param emb the appearance to push onto the end of the internal list.
         */
        void push_back(const FaceEmbedding<dim, dim - codim>& emb);
};

#ifndef __DOXYGEN

template <int dim>
class FaceStorage<dim, 2> {
    private:
        std::deque<FaceEmbedding<dim, dim-2>> embeddings_;

    public:
        size_t getDegree() const;
        const FaceEmbedding<dim, dim-2>& getEmbedding(size_t index) const;

        typename std::deque<FaceEmbedding<dim, dim-2>>::const_iterator
            begin() const;
        typename std::deque<FaceEmbedding<dim, dim-2>>::const_iterator
            end() const;

        const FaceEmbedding<dim, dim-2>& front() const;
        const FaceEmbedding<dim, dim-2>& back() const;

    protected:
        void push_front(const FaceEmbedding<dim, dim-2>& emb);
        void push_back(const FaceEmbedding<dim, dim-2>& emb);
};

template <int dim>
class FaceStorage<dim, 1> {
    private:
        unsigned nEmb_;
        FaceEmbedding<dim, dim-1> embeddings_[2];

    public:
        FaceStorage();

        size_t getDegree() const;
        const FaceEmbedding<dim, dim-1>& getEmbedding(size_t index) const;

        const FaceEmbedding<dim, dim-1>* begin() const;
        const FaceEmbedding<dim, dim-1>* end() const;

        const FaceEmbedding<dim, dim-1>& front() const;
        const FaceEmbedding<dim, dim-1>& back() const;

    protected:
        void push_back(const FaceEmbedding<dim, dim-1>& emb);
};

#endif // __DOXYGEN

/**
 * Helper class that provides core functionality for a \a subdim-face
 * in the skeleton of a \a dim-dimensional triangulation.
 *
 * Each \a subdim-face is described by a Face<dim, subdim> object,
 * which uses this as a base class.  End users should not need to refer
 * to FaceBase directly.
 *
 * See the Face template class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Face<dim, subdim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class FaceBase :
        public FaceStorage<dim, dim - subdim>,
        public NMarkedElement,
        public boost::noncopyable {
    private:
        Component<dim>* component_;
            /**< The component that this face belongs to. */

    public:
        /**
         * Returns the index of this face within the underlying
         * triangulation.
         *
         * This is identical to calling
         * <tt>getTriangulation()->faceIndex<subdim>(this)</tt>
         * (or <tt>getTriangulation()->vertexIndex(this)</tt> for
         * faces of dimension \a subdim = 1, and so on).
         *
         * @return the index of this face.
         */
        size_t index() const;
        /**
         * Returns the triangulation to which this face belongs.
         *
         * @return the triangulation containing this face.
         */
        Triangulation<dim>* getTriangulation() const;
        /**
         * Returns the component of the triangulation to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        Component<dim>* getComponent() const;

    protected:
        /**
         * Creates a new face.
         *
         * @param component the component of the underlying triangulation
         * to which the new face belongs.
         */
        FaceBase(Component<dim>* component);
};

/**
 * Represents a \a subdim-face in the skeleton of a \a dim-dimensional
 * triangulation.
 *
 * A given \a subdim-face \a F of the triangulation may appear many
 * times within the various top-dimensional simplices of the underlying
 * triangulation.  As an extreme example, in a 1-vertex triangulation of
 * a 3-manifold, the single vertex makes 4<i>n</i> such appearances,
 * where \a n is the total number of tetrahedra.
 *
 * Each such appearance is described by a single FaceEmbedding object.  You can
 * iterate through these appearances using begin() and end(), or using the
 * standard C++11 construct <tt>for (auto& emb : F) { ... }</tt>.
 * You can count these appearances by calling getDegree(), and you can also
 * examine them using routines such as front(), back() and getEmbedding().
 *
 * \warning Face objects are highly temporary: whenever a triangulation
 * changes, all its face objects will be deleted and new ones will be
 * created in their place.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class Face : public FaceBase<dim, subdim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Face<dim, subdim> "
        "should not be used for Regina's standard dimensions.");

    protected:
        /**
         * Creates a new face.
         *
         * @param component the component of the underlying triangulation
         * to which the new face belongs.
         */
        Face(Component<dim>* component);
};

// Note that some of our face-related classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Face<2, 1>;
template <> class Face<2, 0>;
template <> class Face<3, 2>;
template <> class Face<3, 1>;
template <> class Face<3, 0>;

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

// Inline functions for FaceStorage

template <int dim, int codim>
inline size_t FaceStorage<dim, codim>::getDegree() const {
    return embeddings_.size();
}

template <int dim, int codim>
inline const FaceEmbedding<dim, dim - codim>& FaceStorage<dim, codim>::
        getEmbedding(size_t index) const {
    return embeddings_[index];
}

template <int dim, int codim>
inline typename std::vector<FaceEmbedding<dim, dim - codim>>::const_iterator
        FaceStorage<dim, codim>::begin() const {
    return embeddings_.begin();
}

template <int dim, int codim>
inline typename std::vector<FaceEmbedding<dim, dim - codim>>::const_iterator
        FaceStorage<dim, codim>::end() const {
    return embeddings_.end();
}

template <int dim, int codim>
inline const FaceEmbedding<dim, dim - codim>& FaceStorage<dim, codim>::front()
         const {
    return embeddings_.front();
}

template <int dim, int codim>
inline const FaceEmbedding<dim, dim - codim>& FaceStorage<dim, codim>::back()
         const {
    return embeddings_.back();
}

template <int dim, int codim>
inline void FaceStorage<dim, codim>::push_back(
        const FaceEmbedding<dim, dim - codim>& emb) {
    embeddings_.push_back(emb);
}

template <int dim>
inline FaceStorage<dim, 1>::FaceStorage() : nEmb_(0) {
}

template <int dim>
inline size_t FaceStorage<dim, 1>::getDegree() const {
    return nEmb_;
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>& FaceStorage<dim, 1>::
        getEmbedding(size_t index) const {
    return embeddings_[index];
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>* FaceStorage<dim, 1>::begin() const {
    return embeddings_;
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>* FaceStorage<dim, 1>::end() const {
    return embeddings_ + nEmb_;
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>& FaceStorage<dim, 1>::front() const {
    return *embeddings_;
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>& FaceStorage<dim, 1>::back() const {
    return *(embeddings_ + nEmb_ - 1);
}

template <int dim>
inline void FaceStorage<dim, 1>::push_back(
        const FaceEmbedding<dim, dim-1>& emb) {
    embeddings_[nEmb_++] = emb;
}

template <int dim>
inline size_t FaceStorage<dim, 2>::getDegree() const {
    return embeddings_.size();
}

template <int dim>
inline const FaceEmbedding<dim, dim-2>& FaceStorage<dim, 2>::
        getEmbedding(size_t index) const {
    return embeddings_[index];
}

template <int dim>
inline typename std::deque<FaceEmbedding<dim, dim-2>>::const_iterator
        FaceStorage<dim, 2>::begin() const {
    return embeddings_.begin();
}

template <int dim>
inline typename std::deque<FaceEmbedding<dim, dim-2>>::const_iterator
        FaceStorage<dim, 2>::end() const {
    return embeddings_.end();
}

template <int dim>
inline const FaceEmbedding<dim, dim-2>& FaceStorage<dim, 2>::front() const {
    return embeddings_.front();
}

template <int dim>
inline const FaceEmbedding<dim, dim-2>& FaceStorage<dim, 2>::back() const {
    return embeddings_.back();
}

template <int dim>
inline void FaceStorage<dim, 2>::push_front(
        const FaceEmbedding<dim, dim-2>& emb) {
    embeddings_.push_front(emb);
}

template <int dim>
inline void FaceStorage<dim, 2>::push_back(
        const FaceEmbedding<dim, dim-2>& emb) {
    embeddings_.push_back(emb);
}

// Inline functions for FaceBase

template <int dim, int subdim>
inline size_t FaceBase<dim, subdim>::index() const {
    return markedIndex();
}

template <int dim, int subdim>
inline Triangulation<dim>* FaceBase<dim, subdim>::getTriangulation() const {
    return FaceStorage<dim, dim - subdim>::front().getSimplex()->
        getTriangulation();
}

template <int dim, int subdim>
inline Component<dim>* FaceBase<dim, subdim>::getComponent() const {
    return component_;
}

template <int dim, int subdim>
inline FaceBase<dim, subdim>::FaceBase(Component<dim>* component) :
        component_(component) {
}

// Inline functions for Face

template <int dim, int subdim>
inline Face<dim, subdim>::Face(Component<dim>* component) :
        FaceBase<dim, subdim>(component) {
}

} // namespace regina

#endif

