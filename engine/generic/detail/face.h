
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

/*! \file generic/detail/face.h
 *  \brief Implementation details for lower-dimensional faces of triangulations.
 */

#ifndef __FACE_H_DETAIL
#ifndef __DOXYGEN
#define __FACE_H_DETAIL
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"
#include <deque>
#include <vector>
#include <boost/noncopyable.hpp>

namespace regina {

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
template <int, int> class Face;
template <int, int> class FaceEmbedding;

namespace detail {

template <int dim> class TriangulationBase;

/**
 * \weakgroup detail
 * @{
 */

/**
 * Helper class that provides core functionality for describing how a
 * <i>subdim</i>-face of a <i>dim</i>-dimensional triangulation appears within
 * each top-dimensional simplex.
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
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceEmbeddingBase : public ShortOutput<FaceEmbeddingBase<dim, subdim>> {
    static_assert(dim >= 2, "FaceEmbedding requires dimension >= 2.");
    static_assert(0 <= subdim && subdim < dim,
        "FaceEmbedding requires 0 <= subdimension < dimension.");

    private:
        Simplex<dim>* simplex_;
            /**< The top-dimensional simplex in which the underlying
             * <i>subdim</i>-face of the triangulation is contained. */
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
         * underlying <i>subdim</i>-face of the triangulation is contained.
         * @param face the corresponding face number of \a simplex.
         * This must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
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
         * <i>subdim</i>-face of the triangulation is contained.
         *
         * @return the top-dimensional simplex.
         */
        Simplex<dim>* simplex() const;
        /**
         * Deprecated routine that returns the top-dimensional simplex
         * in which the underlying <i>subdim</i>-face of the triangulation
         * is contained.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplex() for further details.
         */
        Simplex<dim>* getSimplex() const;

        /**
         * Returns the corresponding face number of simplex().
         * This identifies which face of the top-dimensional simplex
         * simplex() refers to the underlying <i>subdim</i>-face of the
         * triangulation.
         *
         * @return the corresponding face number of the top-dimensional simplex.
         * This will be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         */
        int face() const;
        /**
         * Deprecated routine that returns the corresponding face number
         * of simplex().
         *
         * \deprecated Simply call face() instead.
         *
         * See face() for further details.
         */
        int getFace() const;

        /**
         * Maps vertices (0,...,\a subdim) of the underlying <i>subdim</i>-face
         * of the triangulation to the corresponding vertex numbers of
         * simplex().
         *
         * If the link of the underlying <i>subdim</i>-face is orientable,
         * then this permutation also maps (<i>subdim</i>+1, ..., \a dim) to
         * the remaining vertex numbers of simplex() in a manner that
         * preserves orientation as you walk through the many different
         * FaceEmbedding objects for the same underlying <i>subdim</i>-face.
         *
         * This routine returns the same permutation as
         * <tt>simplex().getFaceMapping<subdim>(face())</tt>.
         * See Simplex<dim>::getFaceMapping() for details.
         *
         * @return a mapping from the vertices of the underlying
         * <i>subdim</i>-face to the corresponding vertices of simplex().
         */
        NPerm<dim+1> vertices() const;
        /**
         * Deprecated routine that maps vertices (0,...,\a subdim) of the
         * underlying <i>subdim</i>-face of the triangulation to the
         * corresponding vertex numbers of simplex().
         *
         * \deprecated Simply call vertices() instead.
         *
         * See vertices() for further details.
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

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
};

/**
 * Helper class for storing all the ways in which a given face of codimension
 * \a codim in a <i>dim</i>-dimensional triangulation appears within the
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
        size_t degree() const;
        /**
         * Deprecated routine that returns the degree of this face.
         *
         * \deprecated Simply call degree() instead.
         *
         * See degree() for further details.
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
         * must be between 0 and degree()-1 inclusive.
         * @return details of the requested appearance.
         */
        const FaceEmbedding<dim, dim - codim>& embedding(size_t index) const;
        /**
         * Deprecated routine that returns one of the ways in which this
         * face appears within a top-dimensional simplex of the underlying
         * triangulation.
         *
         * \deprecated Simply call embedding() instead.
         *
         * See embedding() for further details.
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
         * degree() appearances in total.
         *
         * \ifacespython Not present.  However, Python users can call
         * the Python-only routine embeddings(), which will return all
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
         * degree() appearances in total.
         *
         * \ifacespython Not present.  However, Python users can call
         * the Python-only routine embeddings(), which will return all
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
         * <tt>embedding(0)</tt>.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         * In particular, for a boundary face of codimension 2, both
         * front() and back() will refer to the two appearances of this
         * face on the (<i>dim</i>-1)-dimensional boundary.
         *
         * @return details of the first appearance.
         */
        const FaceEmbedding<dim, dim - codim>& front() const;
        /**
         * Returns the last appearance of this face within a top-dimensional
         * simplex of the underlying triangluation.
         *
         * This is equivalent to calling <tt>embedding(degree()-1)</tt>.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where the appearances of
         * a face are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         * In particular, for a boundary face of codimension 2, both
         * front() and back() will refer to the two appearances of this
         * face on the (<i>dim</i>-1)-dimensional boundary.
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
        size_t degree() const;
        size_t getDegree() const;
        const FaceEmbedding<dim, dim-2>& embedding(size_t index) const;
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

        size_t degree() const;
        size_t getDegree() const;
        const FaceEmbedding<dim, dim-1>& embedding(size_t index) const;
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
 * Helper class that stores whether a face is valid.
 * Every class Face<dim, subdim> inherits from this class.
 *
 * This class takes a template argument to allow optimisation for those
 * dimensions in which faces are always valid.  In particular, if
 * \a allowsInvalid is \c false, then this class assumes that all faces
 * will always be valid, and it optimises away all the implementation
 * details to leave no overhead at all.
 *
 * See FaceValidity<true>::isValid() for details on what it means for a
 * face to be valid.
 *
 * \tparam allowsInvalid \c true when this is used for dimensions in which
 * faces could potentially be invalid, or \c false when this is used for
 * dimensions in which faces are always valid.
 */
template <bool allowsInvalid>
class FaceValidity {
    static_assert(allowsInvalid,
        "The generic FaceValidity template should only be used with "
        "allowsInvalid = true.");

    private:
        bool valid_;
            /**< Is this face valid?  See isValid() for what this means. */

    public:
        /**
         * Determines if this face is valid.
         *
         * There are several conditions that might make a <i>subdim</i>-face
         * of a <i>dim</i>-dimensional triangulation invalid:
         *
         * 1. if the face is identified with itself under a non-identity
         *    permutation (e.g., an edge is identified with itself in
         *    reverse, or a triangle is identified with itself under a
         *    rotation);
         * 2. if the face lies in the boundary of the triangulation but
         *    its link is not a (\a dim - \a subdim - 1)-ball;
         * 3. if the face is internal and not a vertex, and its link is
         *    not a (\a dim - \a subdim - 1)-sphere.
         *
         * Condition (1) is tested for all dimensions \a subdim and \a dim.
         * Conditions (2) and (3) are more difficult, since they rely on
         * undecidable problems.  As a result, they are \e only tested
         * when \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * @return when \a dim is one of Regina's standard dimensions,
         * this returns \c true if and only if this face is valid
         * according to all conditions (1), (2) and (3) above;
         * otherwise this returns \c true if and only if this face is
         * valid according to condition (1) only.
         */
        bool isValid() const;

    protected:
        /**
         * Initialises this face as valid.
         */
        FaceValidity();

        /**
         * Marks this face as invalid.
         */
        void markInvalid();
};

/**
 * Helper class that stores whether a face is valid.
 * See the general FaceValidity template notes for further details.
 *
 * This specialisation is used for dimensions in which faces are never
 * invalid.  It optimises away all the implementation details (since
 * there is nothing to store and nothing to compute).
 */
template <>
class FaceValidity<false> {
    public:
        /**
         * Determines if this face is valid.
         * See FaceValidity<true>::isValid() for a discussion on what
         * this means.
         *
         * This routine always returns \c true, since this specialisation
         * of FaceValidity is for dimensions in which faces are always valid.
         *
         * @return \c true.
         */
        bool isValid() const;

    protected:
        /**
         * Marks this face as invalid.
         *
         * This routine should never be called, since this specialisation
         * of FaceValidity is for dimensions in which faces are always valid.
         *
         * It is provided to support dimension-agnostic code, but its
         * implementation does nothing.
         */
        void markInvalid();
};

/**
 * Helper class that stores whether the link of a face is orientable.
 * Every class Face<dim, subdim> inherits from this class.
 *
 * This class takes a template argument to allow optimisation for those
 * dimensions in which face links are always orientable.  In particular, if
 * \a allowsNonorientable is \c false, then this class assumes that all faces
 * will always have orientable links, and it optimises away all the
 * implementation details to leave no overhead at all.
 *
 * \tparam allowsNonorientable \c true when this is used for dimensions in
 * which face links could potentially be non-orientable, or \c false when
 * this is used for dimensions in which face links are always orientable.
 */
template <bool allowsNonorientable>
class FaceOrientability {
    static_assert(allowsNonorientable,
        "The generic FaceOrientability template should only be used with "
        "allowsNonorientable = true.");

    private:
        bool linkOrientable_;
            /**< Is the link of this face orientable? */

    public:
        /**
         * Determines if the link of this face is orientable.
         *
         * This routine is fast: it uses pre-computed information, and
         * does not need to build a full triangulation of the link.
         *
         * \warning If this face is identified with itself under a
         * non-identity permutation (which makes the face invalid), then
         * the return value of this routine is undefined.
         *
         * @return \c true if and only if the link is orientable.
         */
        bool isLinkOrientable() const;

    protected:
        /**
         * Initialises the link of this face as orientable.
         */
        FaceOrientability();

        /**
         * Marks the link of this face as non-orientable.
         */
        void markLinkNonorientable();
};

/**
 * Helper class that stores whether the link of a face is orientable.
 * See the general FaceOrientability template notes for further details.
 *
 * This specialisation is used for dimensions in which links of faces are
 * always orientable.  It optimises away all the implementation details
 * (since there is nothing to store and nothing to compute).
 */
template <>
class FaceOrientability<false> {
    public:
        /**
         * Determines if the link of this face is orientable.
         *
         * This routine always returns \c true, since this specialisation
         * of FaceOrientability is for dimensions in which links of faces
         * are always orientable.
         *
         * @return \c true.
         */
        bool isLinkOrientable() const;

    protected:
        /**
         * Marks the link of this face as non-orientable.
         *
         * This routine should never be called, since this specialisation
         * of FaceOrientability is for dimensions in which links of faces
         * are always orientable.
         *
         * It is provided to support dimension-agnostic code, but its
         * implementation does nothing.
         */
        void markLinkNonorientable();
};

/**
 * Specifies how <i>subdim</i>-faces are numbered within a
 * <i>dim</i>-dimensional simplex.
 *
 * Every class Face<dim, subdim> inherits from this class.
 * End users should not need to reference this class directly - you can
 * just call the inherited routines in Face<dim, subdim>, such as
 * Face<dim, subdim>::ordering(), Face<dim, subdim>::faceNumber(), and so on.
 *
 * The routines in this class have been put into this separate FaceNumbering
 * class (instead of FaceBase or Face) so that they can be specialised
 * (and therefore optimised) in Regina's \ref stddim "standard dimensions".
 */
template <int dim, int subdim>
class FaceNumbering {
    public:
        /**
         * Given a <i>subdim</i>-face number within a <i>dim</i>-dimensional
         * simplex, returns the corresponding canonical ordering of the
         * simplex vertices.
         *
         * If this canonical ordering is \a c, then \a c[0,...,\a subdim]
         * will be the vertices of the given face in increasing numerical
         * order.  That is, \a c[0] &lt; ... &lt; \a c[\a subdim].
         * For faces of dimension \a subdim &le; <i>dim</i>-2, the remaining
         * images \a c[(\a subdim + 1),...,\a dim] will be chosen to make
         * the permutation even.
         *
         * Note that this is \e not the same permutation as returned by
         * Simplex<dim>::faceMapping<subdim>():
         *
         * - ordering() is a static function, which returns the same permutation
         *   for the same face number, regardless of which <i>dim</i>-simplex
         *   we are looking at.  The images of 0,...,\a subdim will always
         *   appear in increasing order, and (for dimensions
         *   \a subdim &le; <i>dim</i>-2) the permutation will always be even.
         *
         * - faceMapping() examines the underlying face \a F of the
         *   triangulation and, across all appearances of \a F in different
         *   <i>dim</i>-simplices: (i) chooses the images of 0,...,\a subdim to
         *   map to the same respective vertices of \a F; and (ii) chooses the
         *   images of (\a subdim + 1),...,\a dim to maintain a "consistent
         *   orientation" constraint.
         *
         * @param face identifies which <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex to query.  This must be between
         * 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         * @return the corresponding canonical ordering of the simplex vertices.
         */
        static NPerm<dim + 1> ordering(unsigned face);

        /**
         * Identifies which <i>subdim</i>-face in a <i>dim</i>-dimensional
         * simplex is represented by the first (\a subdim + 1) elements of the
         * given permutation.
         *
         * In other words, this routine identifies which <i>subdim</i>-face
         * number within a <i>dim</i>-dimensional simplex spans vertices
         * <tt>vertices[0, ..., \a subdim]</tt>.
         *
         * @param vertices a permutation whose first (\a subdim + 1)
         * elements represent some vertex numbers in a <i>dim</i>-simplex.
         * @return the corresponding <i>subdim</i>-face number in the
         * <i>dim</i>-simplex.  This will be between 0 and
         * (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         */
        static unsigned faceNumber(const NPerm<dim + 1>& vertices);

        /**
         * Tests whether the given <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex contains the given vertex
         * of the simplex.
         *
         * @param face a <i>subdim</i>-face number in a <i>dim</i>-simplex;
         * this must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         * @param vertex a vertex number in a <i>dim</i>-simplex; this must be
         * between 0 and \a dim inclusive.
         * @return \c true if and only if the given <i>subdim</i>-face
         * contains the given vertex.
         */
        static bool containsVertex(unsigned face, unsigned vertex);
};

// Note that FaceNumbering is specialised in standard dimensions.
// Do not explicitly drag in the specialised headers for now.
template <> class FaceNumbering<2, 1>;
template <> class FaceNumbering<2, 0>;
template <> class FaceNumbering<3, 2>;
template <> class FaceNumbering<3, 1>;
template <> class FaceNumbering<3, 0>;

/**
 * Helper class that provides core functionality for a <i>subdim</i>-face
 * in the skeleton of a <i>dim</i>-dimensional triangulation.
 *
 * Each <i>subdim</i>-face is described by a Face<dim, subdim> object,
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
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceBase :
        public FaceStorage<dim, dim - subdim>,
        public FaceValidity<dim >= 3 && subdim <= dim - 2>,
        public FaceOrientability<dim >= 3 && subdim <= dim - 3>,
        public FaceNumbering<dim, subdim>,
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
         * <tt>triangulation()->faceIndex<subdim>(this)</tt>
         * (or <tt>triangulation()->vertexIndex(this)</tt> for
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
        Triangulation<dim>* triangulation() const;
        /**
         * Deprecated routine that returns the triangulation to which
         * this face belongs.
         *
         * \deprecated Simply call triangulation() instead.
         *
         * See triangulation() for further details.
         */
        Triangulation<dim>* getTriangulation() const;
        /**
         * Returns the component of the triangulation to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        Component<dim>* component() const;
        /**
         * Deprecated routine that returns the component of the
         * triangulation to which this face belongs.
         *
         * \deprecated Simply call component() instead.
         *
         * See component() for further details.
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

    friend class Triangulation<dim>;
    friend class TriangulationBase<dim>;
};

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
inline FaceEmbeddingBase<dim, subdim>& FaceEmbeddingBase<dim, subdim>::
        operator = (const FaceEmbeddingBase& cloneMe) {
    simplex_ = cloneMe.simplex_;
    face_ = cloneMe.face_;
    return *this;
}

template <int dim, int subdim>
inline Simplex<dim>* FaceEmbeddingBase<dim, subdim>::simplex() const {
    return simplex_;
}

template <int dim, int subdim>
inline Simplex<dim>* FaceEmbeddingBase<dim, subdim>::getSimplex() const {
    return simplex_;
}

template <int dim, int subdim>
inline int FaceEmbeddingBase<dim, subdim>::face() const {
    return face_;
}

template <int dim, int subdim>
inline int FaceEmbeddingBase<dim, subdim>::getFace() const {
    return face_;
}

template <int dim, int subdim>
inline NPerm<dim+1> FaceEmbeddingBase<dim, subdim>::vertices() const {
    return simplex_->template getFaceMapping<subdim>(face_);
}

template <int dim, int subdim>
inline NPerm<dim+1> FaceEmbeddingBase<dim, subdim>::getVertices() const {
    return vertices();
}

template <int dim, int subdim>
inline bool FaceEmbeddingBase<dim, subdim>::operator == (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ == rhs.simplex_) && (face_ == rhs.face_));
}

template <int dim, int subdim>
inline bool FaceEmbeddingBase<dim, subdim>::operator != (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ != rhs.simplex_) || (face_ != rhs.face_));
}

template <int dim, int subdim>
inline void FaceEmbeddingBase<dim, subdim>::writeTextShort(std::ostream& out)
        const {
    // It seems C++ will not let us do a partial specialisation for dim = 0.
    // Let's see if the compiler can optimise this code instead, given that
    // the if(...) test is a compile-time constant.
    if (subdim == 0)
        out << simplex_->index() << " (" << face_ << ')';
    else
        out << simplex_->index() << " ("
            << vertices().trunc(subdim + 1) << ')';
}

// Inline functions for FaceStorage

template <int dim, int codim>
inline size_t FaceStorage<dim, codim>::degree() const {
    return embeddings_.size();
}

template <int dim, int codim>
inline size_t FaceStorage<dim, codim>::getDegree() const {
    return embeddings_.size();
}

template <int dim, int codim>
inline const FaceEmbedding<dim, dim - codim>& FaceStorage<dim, codim>::
        embedding(size_t index) const {
    return embeddings_[index];
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
inline size_t FaceStorage<dim, 1>::degree() const {
    return nEmb_;
}

template <int dim>
inline size_t FaceStorage<dim, 1>::getDegree() const {
    return nEmb_;
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>& FaceStorage<dim, 1>::
        embedding(size_t index) const {
    return embeddings_[index];
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
inline size_t FaceStorage<dim, 2>::degree() const {
    return embeddings_.size();
}

template <int dim>
inline size_t FaceStorage<dim, 2>::getDegree() const {
    return embeddings_.size();
}

template <int dim>
inline const FaceEmbedding<dim, dim-2>& FaceStorage<dim, 2>::
        embedding(size_t index) const {
    return embeddings_[index];
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

// Inline functions for FaceValidity

template <>
inline FaceValidity<true>::FaceValidity() : valid_(true) {
}

template <>
inline bool FaceValidity<true>::isValid() const {
    return valid_;
}

template <>
inline void FaceValidity<true>::markInvalid() {
    valid_ = false;
}

inline bool FaceValidity<false>::isValid() const {
    return true;
}

inline void FaceValidity<false>::markInvalid() {
}

// Inline functions for FaceOrientability

template <>
inline FaceOrientability<true>::FaceOrientability() : linkOrientable_(true) {
}

template <>
inline bool FaceOrientability<true>::isLinkOrientable() const {
    return linkOrientable_;
}

template <>
inline void FaceOrientability<true>::markLinkNonorientable() {
    linkOrientable_ = false;
}

inline bool FaceOrientability<false>::isLinkOrientable() const {
    return true;
}

inline void FaceOrientability<false>::markLinkNonorientable() {
}

// Inline functions for FaceNumbering

template <int dim, int subdim>
inline NPerm<dim + 1> FaceNumbering<dim, subdim>::ordering(unsigned face) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBase<dim, subdim>::ordering() "
        "should not be used for Regina's standard dimensions.");
    // TODO: Implement ordering().
    return NPerm<dim + 1>();
}

template <int dim, int subdim>
inline unsigned FaceNumbering<dim, subdim>::faceNumber(
        const NPerm<dim + 1>& vertices) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBase<dim, subdim>::faceNumber() "
        "should not be used for Regina's standard dimensions.");
    // TODO: Implement faceNumber().
    return 0;
}

template <int dim, int subdim>
inline bool FaceNumbering<dim, subdim>::containsVertex(unsigned face,
        unsigned vertex) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBase<dim, subdim>::containsVertex() "
        "should not be used for Regina's standard dimensions.");
    // TODO: Implement containsVertex().
    return false;
}

// Inline functions for FaceBase

template <int dim, int subdim>
inline size_t FaceBase<dim, subdim>::index() const {
    return markedIndex();
}

template <int dim, int subdim>
inline Triangulation<dim>* FaceBase<dim, subdim>::triangulation() const {
    return FaceStorage<dim, dim - subdim>::front().simplex()->triangulation();
}

template <int dim, int subdim>
inline Triangulation<dim>* FaceBase<dim, subdim>::getTriangulation() const {
    return triangulation();
}

template <int dim, int subdim>
inline Component<dim>* FaceBase<dim, subdim>::component() const {
    return component_;
}

template <int dim, int subdim>
inline Component<dim>* FaceBase<dim, subdim>::getComponent() const {
    return component_;
}

template <int dim, int subdim>
inline FaceBase<dim, subdim>::FaceBase(Component<dim>* component) :
        component_(component) {
}

} } // namespace regina::detail

#endif

