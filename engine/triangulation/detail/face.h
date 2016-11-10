
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

/*! \file triangulation/detail/face.h
 *  \brief Implementation details for lower-dimensional faces of triangulations.
 */

#ifndef __FACE_H_DETAIL
#ifndef __DOXYGEN
#define __FACE_H_DETAIL
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/perm.h"
#include "triangulation/facenumbering.h"
#include "triangulation/alias/face.h"
#include "triangulation/alias/facenumber.h"
#include "triangulation/alias/simplex.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include <deque>
#include <vector>
#include <boost/noncopyable.hpp>

namespace regina {
namespace detail {

template <int dim> class TriangulationBase;

/**
 * \weakgroup detail
 * @{
 */

/**
 * Indicates whether it is possible for a <i>subdim</i>-face of a
 * <i>dim</i>-dimensional triangulation to be invalid.
 *
 * This compile-time constant function is used to determine the first
 * template argument that should be passed to FaceValidity.
 *
 * \ifacespython Not present.
 *
 * @param dim the dimension of the underlying triangulations.
 * @param subdim the dimension of the faces in question.
 * @return \c true if such faces may be invalid, or \c false if
 * <i>subdim</i>-faces of <i>dim</i>-dimensional triangluations are
 * always valid.
 */
constexpr bool allowsInvalidFaces(int dim, int subdim) {
    return (dim >= 3 && subdim <= dim - 2);
}

/**
 * Indicates whether it is possible for a <i>subdim</i>-face of a
 * <i>dim</i>-dimensional triangulation to have a non-orientable link.
 *
 * This compile-time constant function is used to determine the
 * template argument that should be passed to FaceOrientability.
 *
 * \ifacespython Not present.
 *
 * @param dim the dimension of the underlying triangulations.
 * @param subdim the dimension of the faces in question.
 * @return \c true if such faces may have non-orientable links, or \c false if
 * the links of <i>subdim</i>-faces of <i>dim</i>-dimensional triangluations
 * are always orientable.
 */
constexpr bool allowsNonOrientableLinks(int dim, int subdim) {
    return (dim >= 3 && subdim <= dim - 3);
}

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
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceEmbeddingBase :
        public ShortOutput<FaceEmbeddingBase<dim, subdim>>,
        public alias::SimplexVoid<FaceEmbeddingBase<dim, subdim>, dim>,
        public alias::FaceNumber<FaceEmbeddingBase<dim, subdim>, subdim> {
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
         * <tt>simplex().faceMapping<subdim>(face())</tt>.
         * See Simplex<dim>::faceMapping() for details.
         *
         * @return a mapping from the vertices of the underlying
         * <i>subdim</i>-face to the corresponding vertices of simplex().
         */
        Perm<dim+1> vertices() const;

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
 * This must be between 2 and 15 inclusive.
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
         * Returns one of the ways in which this face appears within a
         * top-dimensional simplex of the underlying triangluation.
         *
         * For convenience, you can also use begin() and end() to
         * iterate through all such appearances.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where these appearances
         * are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         *
         * @param index the index of the requested appearance.  This
         * must be between 0 and degree()-1 inclusive.
         * @return details of the requested appearance.
         */
        const FaceEmbedding<dim, dim - codim>& embedding(size_t index) const;

        /**
         * A begin function for iterating through all appearances of
         * this face within the various top-dimensional simplices of the
         * underlying triangulation.
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where these appearances
         * are ordered in a way that follows the link around the face
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
         * The exception is for codimension 2, where these appearances
         * are ordered in a way that follows the link around the face
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

#ifdef __DOXYGEN
        // This is purely for the benefit of the API documentation.
        // It is commented out of the real build because the codimension-1 case
        // (the only case in which this routine exists) is specialised below.
        /**
         * Determines whether a codimension-1-face represents a dual edge in
         * the maximal forest that has been chosen for the dual 1-skeleton of
         * the triangulation.
         *
         * This routine is only available for faces of codimension 1; that is,
         * (<i>dim</i>-1)-faces of a <i>dim</i>-dimensional triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a (<i>dim</i>-1)-face of the (primal) triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies whether this (<i>dim</i>-1)-face belongs to
         * the dual forest.  In this sense it performs a similar role to
         * Simplex::facetInMaximalForest(), but this routine is typically
         * easier to use.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * @return \c true if and only if this (<i>dim</i>-1)-face represents
         * a dual edge in the maximal forest.
         */
        bool inMaximalForest() const;
#endif

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
        const FaceEmbedding<dim, dim-2>& embedding(size_t index) const;

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
        const FaceEmbedding<dim, dim-1>& embedding(size_t index) const;

        const FaceEmbedding<dim, dim-1>* begin() const;
        const FaceEmbedding<dim, dim-1>* end() const;

        const FaceEmbedding<dim, dim-1>& front() const;
        const FaceEmbedding<dim, dim-1>& back() const;

        bool inMaximalForest() const;

    protected:
        void push_back(const FaceEmbedding<dim, dim-1>& emb);
};

#endif // __DOXYGEN

/**
 * Helper class that stores whether a face is valid.
 * Every class Face<dim, subdim> inherits from this class.
 *
 * See isValid() for details on what it means for a face to be valid.
 *
 * \tparam allowsInvalid \c true when this is used for dimensions
 * (\a dim, \a subdim) in which it is possible for faces to be invalid,
 * or \c false for dimensions in which faces are always valid.  If this
 * is \c false, then this class optimises away all the implementation
 * details to leave no overhead at all.
 *
 * \tparam testLinks \c true if the underlying dimension \a dim is one
 * of Regina's \ref stddim "standard dimensions", or \c false if not.
 * In non-standard dimensions, this class uses a more limited definition
 * of validity that does not test for validity of vertex links.  See
 * isValid() for details.
 */
template <bool allowsInvalid, bool testLinks>
class FaceValidity {
    static_assert(allowsInvalid,
        "The generic FaceValidity template should only be used with "
        "allowsInvalid = true.");
    static_assert(testLinks,
        "The generic FaceValidity template should only be used with "
        "testLinks = true.");

    private:
        enum {
            INVALID_IDENTIFICATION = 1,
                /**< Signifies that the face is identified with itself
                     under a non-identity permutation. */
            INVALID_LINK = 2
                /**< Signifies that the link of the face does not obey
                     the rules laid out by isValid(). */
        };

        unsigned invalid_;
            /**< 0 if this face is valid, or a bitwise combination of
                 INVALID_IDENTIFICATION and/or INVALID_LINK if this face is
                 invalid. */

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
         * 2. if the face does not have an appropriate link.  Here the
         *    meaning of "appropriate" depends upon the type of face:
         *    - for a face that belongs to some boundary facet(s) of the
         *      triangulation, its link must be a topological ball;
         *    - for a vertex that does not belong to any boundary facets,
         *      its link must be a closed (\a dim - 1)-manifold;
         *    - for a (\a subdim &ge; 1)-face that does not belong to any
         *      boundary facets, its link must be a topological sphere.
         *
         * Condition (1) is tested for all dimensions \a subdim and \a dim.
         * Condition (2) is more difficult, since it relies on undecidable
         * problems.  As a result, (2) is \e only tested when \a dim is one
         * of Regina's \ref stddim "standard dimensions".
         *
         * If this face is invalid, then it is possible to find out why.
         * In non-standard dimensions, this must mean that the face fails
         * condition (1) above.  In standard dimensions, you can call the
         * functions hasBadIdentification() and/or hasBadLink() to determine
         * whether the failure is due to conditions (1) or (2) respectively.
         *
         * @return for standard dimensions \a dim, returns \c true if and only
         * if this face is valid according to both conditions (1) and (2) above;
         * for non-standard dimensions \a dim, returns \c true if and only if
         * this face is valid according to condition (1).
         */
        bool isValid() const;

        /**
         * Determines if this face is identified with itself under a
         * non-identity permutation.  For example, if this face is an
         * edge then this routine tests whether the edge is identified
         * with itself in reverse.
         *
         * Such a face will always be marked as invalid.  Note that,
         * for \ref stddim "standard dimensions" \a dim, there are other
         * types of invalid faces also.  See isValid() for a full
         * discussion of what it means for a face to be valid.
         *
         * @return \c true if and only if this face is identified with
         * itself under a non-identity permutation.
         */
        bool hasBadIdentification() const;

        /**
         * Determines if this face does not have an appropriate link.
         * See condition (2) in the documentation for isValid() for a
         * full description of what "appropriate" means.
         *
         * This routine is only available where \a dim is one of Regina's
         * \ref stddim "standard dimensions", since testing this
         * condition in arbitrary dimensions is undecidable.  For higher
         * dimensions \a dim, this routine is not present.
         *
         * A face whose link is not appropriate will always be marked as
         * invalid.  Note that there are other types of invalid faces also.
         * See isValid() for a full discussion of what it means for a
         * face to be valid.
         *
         * @return \c true if and only if the link of this face is not
         * appropriate.
         */
        bool hasBadLink() const;

    protected:
        /**
         * Initialises this face as valid.
         */
        FaceValidity();

        /**
         * Marks this face as having a non-identity self-identification.
         */
        void markBadIdentification();

        /**
         * Marks this face as having a bad link.
         */
        void markBadLink();
};

/**
 * Helper class that stores whether a face is valid.
 * See the general FaceValidity template notes for further details.
 *
 * This specialisation is used for dimensions in which faces are never
 * invalid.  It optimises away all the implementation details (since
 * there is nothing to store and nothing to compute).
 */
template <bool testLinks>
class FaceValidity<false, testLinks> {
    public:
        /**
         * Always returns \c true.
         *
         * In general, this routine determines whether a face is valid.
         * However, this particular class is used for dimensions in which
         * faces are always valid, and so this routine returns \c true always.
         *
         * See FaceValidity<true, true>::isValid() for a general
         * discussion on what it means for a face to be valid.
         *
         * @return \c true.
         */
        bool isValid() const;

    protected:
        /**
         * Marks this face as having a non-identity self-identification.
         *
         * This routine should never be called, since this specialisation
         * of FaceValidity is for dimensions in which faces are always valid.
         *
         * It is provided to support dimension-agnostic code, but its
         * implementation does nothing.
         */
        void markBadIdentification();

        /**
         * Marks this face as having a bad link.
         *
         * This routine should never be called, since this specialisation
         * of FaceValidity is for dimensions in which faces are always valid.
         *
         * It is provided to support dimension-agnostic code, but its
         * implementation does nothing.
         */
        void markBadLink();
};

/**
 * Helper class that stores whether a face is valid.
 * See the general FaceValidity template notes for further details.
 *
 * This specialisation is used for \ref stddim "non-standard dimensions",
 * where the definition of validity is weaker: we do test faces for bad
 * self-identifications, but we do not test faces for bad links.
 *
 * See FaceValidity<true, true>::isValid() for a full discussion of what
 * it means for a face to be valid, and how this differs between
 * standard and non-standard dimensions.
 */
template <>
class FaceValidity<true, false> {
    private:
        bool valid_;
            /**< Is this face valid? */

    public:
        /**
         * Determines if this face is valid.
         *
         * This class is used for \ref stddim "non-standard dimensions",
         * where a face is defined to be valid if and only if it is not
         * identified with itself under a non-identity permutation.
         * For example, an edge is valid if and only if it is not identified
         * with itself in reverse, and a triangle is valid if and only
         * if it is is not identified with itself under a non-trivial
         * rotation or reflection.
         *
         * Note that the definition of validity is richer in Regina's
         * standard dimensions, where we also consider the topology of
         * the link of a face.  See FaceValidity<true, true> for a full
         * discussion of what it means for a face to be valid, and how
         * this differs between standard and non-standard dimensions.
         *
         * @return \c true if and only if this face is not identified
         * with itself under a non-identity permutation.
         */
        bool isValid() const;

        /**
         * Determines if this face is identified with itself under a
         * non-identity permutation.  For example, if this face is an
         * edge then this routine tests whether the edge is identified
         * with itself in reverse.
         *
         * For this class, hasBadIdentification() returns \c true if and
         * only if isValid() returns \c false.  This is in contrast to
         * \ref stddim "standard dimensions", where there are other types of
         * invalid faces also.  See FaceValidity<true, true>::isValid() for
         * a full discussion of what it means for a face to be valid, and how
         * this differs between standard and non-standard dimensions.
         *
         * @return \c true if and only if this face is identified with
         * itself under a non-identity permutation.
         */
        bool hasBadIdentification() const;

    protected:
        /**
         * Initialises this face as valid.
         */
        FaceValidity();

        /**
         * Marks this face as having a non-identity self-identification.
         */
        void markBadIdentification();
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
 * Helper class that indicates what data type \a Base uses to store its
 * list of <i>subdim</i>-faces.
 *
 * Typically \a Base will represent a triangulation, or one of its components
 * or boundary components.  The typedef FaceListHolder<Base, subdim>::Holder
 * represents the type that \a Base uses to store its <i>subdim</i>-faces.
 *
 * \a Base should have a template function faces<subdim>() that returns a
 * const reference to this type.  That is, Base::faces<subdim> should have
 * return type <tt>const FaceListHolder<Base, subdim>::Holder&</tt>.
 *
 * For each such class \a Base, this FaceListHolder template should be
 * specialised in the same header where \a Base is defined.  For all
 * other types \a T, the template class FaceListHolder<T, subdim> will
 * remain undefined.
 */
template <class Base, int subdim>
struct FaceListHolder;

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
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceBase :
        public FaceStorage<dim, dim - subdim>,
        public FaceValidity<allowsInvalidFaces(dim, subdim), standardDim(dim)>,
        public FaceOrientability<allowsNonOrientableLinks(dim, subdim)>,
        public FaceNumbering<dim, subdim>,
        public MarkedElement,
        public alias::FaceOfSimplex<FaceBase<dim, subdim>, dim, subdim - 1>,
        public Output<Face<dim, subdim>>,
        public boost::noncopyable {
    private:
        Component<dim>* component_;
            /**< The component that this face belongs to. */
        BoundaryComponent<dim>* boundaryComponent_;
            /**< The boundary component that this face is a part of,
                 or 0 if this face is internal. */

    public:
        /**
         * Returns the index of this face within the underlying
         * triangulation.
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
         * Returns the component of the triangulation to which this
         * face belongs.
         *
         * @return the component containing this face.
         */
        Component<dim>* component() const;
        /**
         * Returns the boundary component of the triangulation to which
         * this face belongs.
         *
         * See the note in the BoundaryComponent overview regarding
         * what happens if the link of the face itself has more than one
         * boundary component.  Note that such a link makes both the
         * face and the underlying triangulation invalid.
         *
         * For dimensions in which ideal and/or invalid vertices are
         * both possible and recognised: an ideal vertex will have its own
         * individual boundary component to which it belongs, and so will
         * an invalid vertex boundary component if the invalid vertex does
         * not already belong to some real boundary component.
         *
         * @return the boundary component containing this face, or 0 if this
         * face does not lie entirely within the boundary of the triangulation.
         */
        BoundaryComponent<dim>* boundaryComponent() const;
        /**
         * Determines if this face lies entirely on the boundary of the
         * triangulation.
         *
         * For dimensions in which ideal and/or invalid vertices are
         * both possible and recognised: both ideal and invalid vertices
         * are considered to be on the boundary.
         *
         * @return \c true if and only if this face lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Returns the <i>lowerdim</i>-face of the underlying triangulation
         * that appears as the given <i>lowerdim</i>-dimensional subface
         * of this face.
         *
         * The argument \a face must represent a <i>lowerdim</i>-face
         * number within a <i>subdim</i>-simplex.  This <i>lowerdim</i>-face
         * number will be interpreted with respect to the inherent labelling
         * (0, ..., \a subdim) of the vertices of this <i>subdim</i>-face.
         * See FaceEmbedding<dim, subdim>::vertices() for details on how
         * these map to the vertex numbers of the <i>dim</i>-dimensional
         * simplices that contain this face in the overall triangulation.
         *
         * See FaceNumbering<subdim, lowerdim> for the conventions of how
         * <i>lowerdim</i>-faces are numbered within a <i>subdim</i>-simplex.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>face(lowerdim, face)</tt>; that is, the template parameter
         * \a lowerdim becomes the first argument of the function.
         *
         * @param face the <i>lowerdim</i>-face of this <i>subdim</i>-face to
         * examine.  This should be between 0 and
         * (<i>subdim</i>+1 choose <i>lowerdim</i>+1)-1 inclusive.
         * @return the corresponding <i>lowerdim</i>-face of the triangulation.
         */
        template <int lowerdim>
        Face<dim, lowerdim>* face(int face) const;

        /**
         * Examines the given <i>lowerdim</i>-dimensional subface of this face,
         * and returns the mapping between the underlying <i>lowerdim</i>-face
         * of the triangulation and the individual vertices of this face.
         *
         * The argument \a face must represent a <i>lowerdim</i>-face
         * number within a <i>subdim</i>-simplex.  This <i>lowerdim</i>-face
         * number will be interpreted with respect to the inherent labelling
         * (0, ..., \a subdim) of the vertices of this <i>subdim</i>-face.
         * See FaceEmbedding<dim, subdim>::vertices() for details on how
         * these map to the vertex numbers of the <i>dim</i>-dimensional
         * simplices that contain this face in the overall triangulation.
         *
         * Let \a F denote this <i>subdim</i>-face of the triangulation, and
         * let \a L denote the <i>lowerdim</i>-face of the triangulation that
         * corresponds to the given subface of \a F.  Then the
         * permutation returned by this routine maps the vertex numbers
         * (0, ..., \a lowerdim) of \a L to the corresponding vertex numbers
         * of \a F.  This is with respect to the inherent labellings
         * (0, ..., \a lowerdim) and (0, ..., \a subdim) of the vertices of
         * \a L and \a F respectively.
         *
         * In particular, if this routine returns the permutation \a p,
         * then the images \a p[0,...,\a lowerdim] will be some permutation
         * of the vertices
         * Face<subdim, lowerdim>::ordering(face)[0,...,\a lowerdim].
         *
         * This routine differs from Simplex<dim>::faceMapping<lowerdim>()
         * in how it handles the images of (<i>lowerdim</i>+1, ..., \a dim):
         *
         * - This routine will map (<i>lowerdim</i>+1, ..., \a subdim) to the
         *   remaining vertices of this face in an arbitrary order, and will map
         *   (<i>subdim</i>+1, ..., \a dim) to (<i>subdim</i>+1, ..., \a dim)
         *   again in an arbitrary order.
         *
         * - In contrast, Simplex<dim>::faceMapping<lowerdim>() chooses
         *   the images of (<i>lowerdim</i>+1, ..., \a dim) to satisfy
         *   an additional orientability constraint.
         *
         * See FaceNumbering<subdim, lowerdim> for the conventions of how
         * <i>lowerdim</i>-faces are numbered within a <i>subdim</i>-simplex.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>faceMapping(lowerdim, face)</tt>; that is, the template
         * parameter \a lowerdim becomes the first argument of the function.
         *
         * @param face the <i>lowerdim</i>-face of this <i>subdim</i>-face to
         * examine.  This should be between 0 and
         * (<i>subdim</i>+1 choose <i>lowerdim</i>+1)-1 inclusive.
         * @return a mapping from the vertices of the underlying
         * <i>lowerdim</i>-face of the triangulation to the vertices of
         * this <i>subdim</i>-face.
         */
        template <int lowerdim>
        Perm<dim + 1> faceMapping(int face) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * The class Face<dim, subdim> may safely override this function,
         * since the output routines cast down to Face<dim, subdim>
         * before calling it.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * The class Face<dim, subdim> may safely override this function,
         * since the output routines cast down to Face<dim, subdim>
         * before calling it.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * Creates a new face.  The face will be initialised as belong
         * to no boundary component.
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
inline int FaceEmbeddingBase<dim, subdim>::face() const {
    return face_;
}

template <int dim, int subdim>
inline Perm<dim+1> FaceEmbeddingBase<dim, subdim>::vertices() const {
    return simplex_->template faceMapping<subdim>(face_);
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
inline const FaceEmbedding<dim, dim - codim>& FaceStorage<dim, codim>::
        embedding(size_t index) const {
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

// We hid the specialisations FaceStorage<dim, {1,2}> from doxygen, so hide
// its implementation as well.
#ifndef __DOXYGEN
template <int dim>
inline FaceStorage<dim, 1>::FaceStorage() : nEmb_(0) {
}

template <int dim>
inline size_t FaceStorage<dim, 1>::degree() const {
    return nEmb_;
}

template <int dim>
inline bool FaceStorage<dim, 1>::inMaximalForest() const {
    return embeddings_->simplex()->facetInMaximalForest(embeddings_->face());
}

template <int dim>
inline const FaceEmbedding<dim, dim-1>& FaceStorage<dim, 1>::
        embedding(size_t index) const {
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
inline const FaceEmbedding<dim, dim-2>& FaceStorage<dim, 2>::
        embedding(size_t index) const {
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
#endif // ! __DOXYGEN

// Inline functions for FaceValidity

// We hide the specialised FaceValidity<true, true> implementations from
// doxygen, since doxygen is confused by the fact that it doesn't see a
// corresponding specialised FaceValidity<true, true> class.
#ifndef __DOXYGEN
template <>
inline FaceValidity<true, true>::FaceValidity() : invalid_(0) {
}

template <>
inline bool FaceValidity<true, true>::isValid() const {
    return ! invalid_;
}

template <>
inline bool FaceValidity<true, true>::hasBadIdentification() const {
    return (invalid_ & INVALID_IDENTIFICATION);
}

template <>
inline bool FaceValidity<true, true>::hasBadLink() const {
    return (invalid_ & INVALID_LINK);
}

template <>
inline void FaceValidity<true, true>::markBadIdentification() {
    invalid_ |= INVALID_IDENTIFICATION;
}

template <>
inline void FaceValidity<true, true>::markBadLink() {
    invalid_ |= INVALID_LINK;
}
#endif // ! __DOXYGEN

template <bool testLinks>
inline bool FaceValidity<false, testLinks>::isValid() const {
    return true;
}

template <bool testLinks>
inline void FaceValidity<false, testLinks>::markBadIdentification() {
}

template <bool testLinks>
inline void FaceValidity<false, testLinks>::markBadLink() {
}

inline FaceValidity<true, false>::FaceValidity() : valid_(true) {
}

inline bool FaceValidity<true, false>::isValid() const {
    return valid_;
}

inline bool FaceValidity<true, false>::hasBadIdentification() const {
    return ! valid_;
}

inline void FaceValidity<true, false>::markBadIdentification() {
    valid_ = false;
}

// Inline functions for FaceOrientability

// We hide the specialised FaceOrientability<true> implementations from
// doxygen, since doxygen is confused by the fact that it doesn't see a
// corresponding specialised FaceOrientability<true> class.
#ifndef __DOXYGEN
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
#endif // ! __DOXYGEN

inline bool FaceOrientability<false>::isLinkOrientable() const {
    return true;
}

inline void FaceOrientability<false>::markLinkNonorientable() {
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
inline Component<dim>* FaceBase<dim, subdim>::component() const {
    return component_;
}

template <int dim, int subdim>
inline BoundaryComponent<dim>* FaceBase<dim, subdim>::boundaryComponent()
        const {
    return boundaryComponent_;
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::isBoundary() const {
    return boundaryComponent_;
}

template <int dim, int subdim>
template <int lowerdim>
inline Face<dim, lowerdim>* FaceBase<dim, subdim>::face(int f) const {
    // Let S be the dim-simplex corresponding to the first embedding,
    // i.e., this->front().
    // Let face f of this subdim-face correspond to face inSimp of S.

    int inSimp = (
        // If lowerdim = 0, the general formula can be simplified.
        lowerdim == 0 ?
        FaceStorage<dim, dim - subdim>::front().vertices()[f] :
        FaceNumbering<dim, lowerdim>::faceNumber(
            FaceStorage<dim, dim - subdim>::front().vertices() *
            Perm<dim + 1>::extend(
                FaceNumbering<subdim, lowerdim>::ordering(f))));
    return FaceStorage<dim, dim - subdim>::front().simplex()->
        template face<lowerdim>(inSimp);
}

template <int dim, int subdim>
template <int lowerdim>
Perm<dim + 1> FaceBase<dim, subdim>::faceMapping(int f) const {
    // Let S be the dim-simplex corresponding to the first embedding,
    // i.e., this->front().
    // Let face f of this subdim-face correspond to face inSimp of S.

    int inSimp = (
        // If lowerdim = 0, the general formula can be simplified.
        lowerdim == 0 ?
        FaceStorage<dim, dim - subdim>::front().vertices()[f] :
        FaceNumbering<dim, lowerdim>::faceNumber(
            FaceStorage<dim, dim - subdim>::front().vertices() *
            Perm<dim + 1>::extend(
                FaceNumbering<subdim, lowerdim>::ordering(f))));

    // Get the images of 0,...,lowerdim correct:
    Perm<dim + 1> p =
        FaceStorage<dim, dim - subdim>::front().vertices().inverse() *
        FaceStorage<dim, dim - subdim>::front().simplex()->
            template faceMapping<lowerdim>(inSimp);

    // Ensure the images of lowerdim+1,...,dim are correct also.
    for (unsigned i = subdim + 1; i <= dim; ++i)
        if (p[i] != i) {
            // Suppose p maps x -> i -> y.
            //
            // Since i > subdim, we must have x > lowerdim.
            // Therefore y is not one of the vertices of our lowerdim-face.
            //
            // Change p to map x -> y and i -> i.
            //
            p = Perm<dim + 1>(p[i], i) * p;
        }

    return p;
}

template <int dim, int subdim>
inline FaceBase<dim, subdim>::FaceBase(Component<dim>* component) :
        component_(component), boundaryComponent_(0) {
}

template <int dim, int subdim>
inline void FaceBase<dim, subdim>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << Strings<subdim>::face;
    if (subdim < dim - 1)
        out << " of degree " << FaceStorage<dim, dim - subdim>::degree();
}

template <int dim, int subdim>
void FaceBase<dim, subdim>::writeTextLong(std::ostream& out) const {
    static_cast<const Face<dim, subdim>*>(this)->writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (const auto& emb : *this)
        out << "  " << emb << std::endl;
}

} } // namespace regina::detail

#endif

