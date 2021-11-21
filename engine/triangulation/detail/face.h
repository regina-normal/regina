
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

/*! \file triangulation/detail/face.h
 *  \brief Implementation details for lower-dimensional faces of triangulations.
 */

#ifndef __REGINA_FACE_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_FACE_H_DETAIL
#endif

#include "regina-core.h"
#include "core/output.h"
#include "maths/perm.h"
#include "triangulation/facenumbering.h"
#include "triangulation/alias/face.h"
#include "triangulation/alias/facenumber.h"
#include "triangulation/alias/simplex.h"
#include "triangulation/detail/strings.h"
#include "triangulation/forward.h"
#include "utilities/markedvector.h"
#include "utilities/shortarray.h"
#include "utilities/typeutils.h"
#include <deque>
#include <vector>

namespace regina::detail {

template <int dim> class TriangulationBase;

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
 *
 * \ingroup detail
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
                 <i>subdim</i>-face of the triangulation is contained. */
        Perm<dim + 1> vertices_;
            /**< Maps the vertices of the underlying <i>subdim</i>-face
                 of the triangulation to the corresponding vertex numbers of
                 \a simplex_.  See vertices() for details. */

    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present, since the C++ assignment operators
         * are not accessible to Python.
         */
        FaceEmbeddingBase();
        /**
         * Creates a new object containing the given data.
         *
         * @param simplex the top-dimensional simplex in which the
         * underlying <i>subdim</i>-face of the triangulation is contained.
         * @param vertices a mapping from the vertices of the underlying
         * <i>subdim</i>-face of the triangulation to the corresponding
         * vertex numbers of \a simplex.  See vertices() for details of how
         * this permutation should be structured.
         */
        FaceEmbeddingBase(Simplex<dim>* simplex, Perm<dim + 1> vertices);
        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbeddingBase(const FaceEmbeddingBase& cloneMe) = default;

        /**
         * Makes this a copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbeddingBase& operator = (const FaceEmbeddingBase& cloneMe) =
            default;

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
         * <tt>simplex().faceMapping<subdim>(face())</tt> (and this
         * routine is faster if you already have a FaceEmbedding).
         * See Simplex<dim>::faceMapping() for details.
         *
         * @return a mapping from the vertices of the underlying
         * <i>subdim</i>-face to the corresponding vertices of simplex().
         */
        Perm<dim+1> vertices() const;

        /**
         * Tests whether this and the given object are identical.
         *
         * Since Regina 7.0, "identical" means that both objects refer to the
         * same face of the same top-dimensional simplex, \e and have
         * the same embedding permutations as returned by vertices().
         * This is a stronger (but more natural) condition than was used in
         * Regina 6.0.1 and earlier, which merely required both objects to
         * refer to the same face number of the same top-dimensional simplex.
         *
         * @param rhs the object to compare with this.
         * @return \c true if and only if both object are identical.
         */
        bool operator == (const FaceEmbeddingBase& rhs) const;

        /**
         * Tests whether this and the given object are not identical.
         *
         * Since Regina 7.0, "identical" means that both objects refer to the
         * same face of the same top-dimensional simplex, \e and have
         * the same embedding permutations as returned by vertices().
         * This is a stronger (but more natural) condition than was used in
         * Regina 6.0.1 and earlier, which merely required both objects to
         * refer to the same face number of the same top-dimensional simplex.
         *
         * @param rhs the object to compare with this.
         * @return \c true if and only if both object are identical.
         */
        bool operator != (const FaceEmbeddingBase& rhs) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

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
        FaceEmbeddingBase(Simplex<dim>*, int);
};

#ifndef __DOXYGEN
/**
 * Internal helper class that determines the data structure used to
 * store embeddings for a face of a triangulation.
 *
 * End users should have no need to use this class, and should never
 * need to see or know about the data type that it describes.
 *
 * \ifacespython Not present.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam codim the codimension (not dimension!) of the faces under
 * consideration.  This must be between 1 and \a dim inclusive.
 *
 * \ingroup detail
 */
template <int dim, int codim>
struct FaceEmbeddingsList {
    /**
     * For most face types, we store the embeddings in an ordinary vector.
     */
    using type = std::vector<FaceEmbedding<dim, dim - codim>>;
};

template <int dim>
struct FaceEmbeddingsList<dim, 2> {
    /**
     * For codimension 2 faces, we store the embeddings in a deque.  This is
     * because the link is 1-dimensional, and the skeleton building routines
     * will want to push embeddings onto both ends of the list to maintain the
     * correct order as we follow the link around.
     */
    using type = std::deque<FaceEmbedding<dim, dim - 2>>;
};

template <int dim>
struct FaceEmbeddingsList<dim, 1> {
    /**
     * For codimension 1 faces, there are always either one or two embeddings.
     * We therefore use a cheap stack-based array of maximum size 2.
     */
    using type = ShortArray<FaceEmbedding<dim, dim - 1>, 2>;
};
#endif // __DOXYGEN

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
 * Neither this class nor the "end user" class Face<dim> support
 * value semantics: they cannot be copied, swapped, or manually constructed.
 * Their memory is managed by the Triangulation class, and their locations
 * in memory define them.  See Face<dim> for further details.
 *
 * \ifacespython This base class is not present, but the "end user" class
 * Face<dim, subdim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 *
 * \ingroup detail
 */
template <int dim, int subdim>
class FaceBase :
        public FaceNumbering<dim, subdim>,
        public MarkedElement,
        public alias::FaceOfSimplex<FaceBase<dim, subdim>, dim, subdim - 1>,
        public Output<Face<dim, subdim>> {
    static_assert(dim >= 2, "Face requires dimension >= 2.");

    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 triangulation containing this face. */
        static constexpr int subdimension = subdim;
            /**< A compile-time constant that gives the dimension of this
                 face. */

        static constexpr bool allowsNonOrientableLinks = (subdim <= dim - 3);
            /**< Indicates whether it is possible for a face of this dimension
                 to have a non-orientable link. */
        static constexpr bool allowsInvalidFaces =
                (dim >= 3 && subdim <= dim - 2);
            /**< Indicates whether it is possible for a face of this dimension
                 to be invalid. */

    private:
        /**
         * Possible values of \a whyInvalid_.
         * These can be combined using bitwise OR.
         */
        enum Validity {
            VALID = 0,
                /**< Signifies that this face is valid. */
            INVALID_IDENTIFICATION = 1,
                /**< Signifies that the face is identified with itself
                     under a non-identity permutation. */
            INVALID_LINK = 2
                /**< Signifies that the link of the face does not obey
                     the rules laid out by isValid(). */
        };

        typename FaceEmbeddingsList<dim, dim - subdim>::type embeddings_;
            /**< The list of all occurrences of this face within the
                 top-dimensional simplices of the underlying triangulation. */
        Component<dim>* component_;
            /**< The component that this face belongs to. */
        BoundaryComponent<dim>* boundaryComponent_;
            /**< The boundary component that this face is a part of,
                 or \c null if this face is internal. */
        EnableIf<allowsNonOrientableLinks, bool, true> linkOrientable_;
            /**< Is the link of this face orientable? */
        EnableIf<allowsInvalidFaces && standardDim(dim), unsigned, VALID>
                whyInvalid_;
            /**< Indicates whether this face is valid and, if it is not,
                 lists all the reasons why.  This should be a bitwise
                 combination of Validity constants. */
        EnableIf<allowsInvalidFaces && ! standardDim(dim), bool, true> valid_;
            /**< Is this face valid?  This is for use in non-standard
                 dimensions, where we only test for one type of validity
                 (bad self-identifications). */

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
         * @return a reference to the triangulation containing this face.
         */
        Triangulation<dim>& triangulation() const;
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
         * @return the boundary component containing this face, or \c null if
         * this face does not lie entirely within the boundary of the
         * triangulation.
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
         * For convenience, you can also use begin() and end() to iterate
         * through all such appearances, or equivalently you can iterate
         * over the lightweight object returned by embeddings().
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
        const FaceEmbedding<dim, subdim>& embedding(size_t index) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all of the ways in which this face appears within a
         * top-dimensional simplex of the underlying triangluation.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  The elements of the list will be
         * read-only objects of type FaceEmbedding<dim, subdim>.  For
         * example, your code might look like:
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (const auto& emb : face->embeddings()) { ... }
         * \endcode
         *
         * In most cases, the ordering of appearances is arbitrary.
         * The exception is for codimension 2, where these appearances
         * are ordered in a way that follows the link around the face
         * (which in codimension 2 is always a path or a cycle).
         *
         * Using embeddings() is equivalent to iterating over the face
         * itself.  It generates a tiny amount of extra overhead, but also
         * may be considered more readable.  In particular, the code
         * above is equivalent to both of the following alternatives:
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (const auto& emb : *face) { ... }
         * \endcode
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (size_t i = 0; i < face->degree(); ++i) {
         *     const auto& emb = face->embedding(i);
         *     ...
         * }
         * \endcode
         *
         * @return access to the list of all appearances of this face
         * within a top-dimensional simplex of the underlying triangulation.
         */
        auto embeddings() const;

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
         * The type that is returned will be a lightweight iterator type,
         * guaranteed to satisfy the C++ LegacyRandomAccessIterator requirement.
         * The precise C++ type of the iterator is subject to change, so
         * C++ users should use \c auto (just like this declaration does).
         *
         * Iterating from begin() to end() (that is, iterating directly over a
         * face) is equivalent to iterating over embeddings().  Iterating
         * directly over a face generates a tiny bit less overhead, but
         * you may also find it to be less readable.  In particular, the
         * following three blocks of code are all equivalent:
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (const auto& emb : *face) { ... }
         * \endcode
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (const auto& emb : face->embeddings()) { ... }
         * \endcode
         *
         * \code{.cpp}
         * Face<dim, subdim>* face = ...;
         * for (size_t i = 0; i < face->degree(); ++i) {
         *     const auto& emb = face->embedding(i);
         *     ...
         * }
         * \endcode
         *
         * \ifacespython Not present.  Python users can still iterate over
         * embeddings(); however, be aware that embeddings() returns an
         * entire list of appearances, not a lightweight object.
         *
         * @return a iterator that points to the first appearance.
         */
        auto begin() const;
        /**
         * An end function for iterating through all appearances of
         * this face within the various top-dimensional simplices of the
         * underlying triangulation.
         *
         * See begin() for full details on how this iteration works, and what
         * other alternatives are available.
         *
         * The type that is returned will be a lightweight iterator type,
         * guaranteed to satisfy the C++ LegacyRandomAccessIterator requirement.
         * The precise C++ type of the iterator is subject to change, so
         * C++ users should use \c auto (just like this declaration does).
         *
         * \ifacespython Not present.  Python users can still iterate over
         * embeddings(); however, be aware that embeddings() returns an
         * entire list of appearances, not a lightweight object.
         *
         * @return a "beyond the end" iterator that comes immediately
         * after the last appearance.
         */
        auto end() const;

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
        const FaceEmbedding<dim, subdim>& front() const;
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
        const FaceEmbedding<dim, subdim>& back() const;

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
         * \pre The facial dimension \a subdim is precisely <i>dim</i>-1.
         *
         * @return \c true if and only if this (<i>dim</i>-1)-face represents
         * a dual edge in the maximal forest.
         */
        bool inMaximalForest() const;

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
         * A face whose link is not appropriate will always be marked as
         * invalid.  Note that there are other types of invalid faces also.
         * See isValid() for a full discussion of what it means for a
         * face to be valid.
         *
         * \pre The dimension \a dim is one of Regina's
         * \ref stddim "standard dimensions".  Any attempt to use this routine
         * in higher dimensions \a dim will result in a compile-time error.
         * This is because testing for bad links in higher dimensions can
         * require solutions to problems that are proven to be undecidable.
         *
         * @return \c true if the link of this face is not appropriate (thereby
         * making the face invalid), or \c false if the link is appropriate.
         */
        bool hasBadLink() const;

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
         * \ifacespython Not present; use str() instead.
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
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        // Make this class non-copyable.
        FaceBase(const FaceBase&) = delete;
        FaceBase& operator = (const FaceBase&) = delete;

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

// Inline functions for FaceEmbeddingBase

template <int dim, int subdim>
inline FaceEmbeddingBase<dim, subdim>::FaceEmbeddingBase() : simplex_(nullptr) {
}

template <int dim, int subdim>
inline FaceEmbeddingBase<dim, subdim>::FaceEmbeddingBase(
        Simplex<dim>* simplex, Perm<dim + 1> vertices) :
        simplex_(simplex), vertices_(vertices) {
}

template <int dim, int subdim>
inline Simplex<dim>* FaceEmbeddingBase<dim, subdim>::simplex() const {
    return simplex_;
}

template <int dim, int subdim>
inline int FaceEmbeddingBase<dim, subdim>::face() const {
    return FaceNumbering<dim, subdim>::faceNumber(vertices_);
}

template <int dim, int subdim>
inline Perm<dim+1> FaceEmbeddingBase<dim, subdim>::vertices() const {
    return vertices_;
}

template <int dim, int subdim>
inline bool FaceEmbeddingBase<dim, subdim>::operator == (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ == rhs.simplex_) && (vertices_ == rhs.vertices_));
}

template <int dim, int subdim>
inline bool FaceEmbeddingBase<dim, subdim>::operator != (
        const FaceEmbeddingBase& rhs) const {
    return ((simplex_ != rhs.simplex_) || (vertices_ != rhs.vertices_));
}

template <int dim, int subdim>
inline void FaceEmbeddingBase<dim, subdim>::writeTextShort(std::ostream& out)
        const {
    if constexpr (subdim == 0)
        out << simplex_->index() << " (" << vertices_[0] << ')';
    else
        out << simplex_->index() << " ("
            << vertices_.trunc(subdim + 1) << ')';
}

// Inline functions for FaceBase

template <int dim, int subdim>
inline size_t FaceBase<dim, subdim>::index() const {
    return markedIndex();
}

template <int dim, int subdim>
inline Triangulation<dim>& FaceBase<dim, subdim>::triangulation() const {
    return front().simplex()->triangulation();
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
inline size_t FaceBase<dim, subdim>::degree() const {
    return embeddings_.size();
}

template <int dim, int subdim>
inline const FaceEmbedding<dim, subdim>& FaceBase<dim, subdim>::embedding(
        size_t index) const {
    return embeddings_[index];
}

template <int dim, int subdim>
inline auto FaceBase<dim, subdim>::embeddings() const {
    return ListView(embeddings_);
}

template <int dim, int subdim>
inline auto FaceBase<dim, subdim>::begin() const {
    return embeddings_.begin();
}

template <int dim, int subdim>
inline auto FaceBase<dim, subdim>::end() const {
    return embeddings_.end();
}

template <int dim, int subdim>
inline const FaceEmbedding<dim, subdim>& FaceBase<dim, subdim>::front() const {
    return embeddings_.front();
}

template <int dim, int subdim>
inline const FaceEmbedding<dim, subdim>& FaceBase<dim, subdim>::back() const {
    return embeddings_.back();
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::inMaximalForest() const {
    static_assert(dim == subdim + 1,
        "FaceBase::inMaximalForest() is only available for faces of "
        "codimension 1");
    return embeddings_.front().simplex()->facetInMaximalForest(
        embeddings_.front().face());
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::isLinkOrientable() const {
    if constexpr (allowsNonOrientableLinks)
        return linkOrientable_.value;
    else
        return true;
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::isValid() const {
    if constexpr (! allowsInvalidFaces)
        return true;
    else if constexpr (standardDim(dim))
        return ! whyInvalid_.value;
    else
        return valid_.value;
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::hasBadIdentification() const {
    if constexpr (! allowsInvalidFaces)
        return false;
    else if constexpr (standardDim(dim))
        return (whyInvalid_.value & INVALID_IDENTIFICATION);
    else
        return ! valid_.value;
}

template <int dim, int subdim>
inline bool FaceBase<dim, subdim>::hasBadLink() const {
    static_assert(standardDim(dim), "The routine hasBadLink() is only available in Regina's standard dimensions.");
    if constexpr (! allowsInvalidFaces)
        return false;
    else
        return (whyInvalid_.value & INVALID_LINK);
}

template <int dim, int subdim>
template <int lowerdim>
inline Face<dim, lowerdim>* FaceBase<dim, subdim>::face(int f) const {
    // Let S be the dim-simplex corresponding to the first embedding,
    // i.e., this->front().
    // Let face f of this subdim-face correspond to face inSimp of S.

    int inSimp = (
        // If lowerdim = 0, the general formula can be simplified.
        lowerdim == 0 ? front().vertices()[f] :
        FaceNumbering<dim, lowerdim>::faceNumber(
            front().vertices() * Perm<dim + 1>::extend(
                FaceNumbering<subdim, lowerdim>::ordering(f))));
    return front().simplex()->template face<lowerdim>(inSimp);
}

template <int dim, int subdim>
template <int lowerdim>
Perm<dim + 1> FaceBase<dim, subdim>::faceMapping(int f) const {
    // Let S be the dim-simplex corresponding to the first embedding,
    // i.e., this->front().
    // Let face f of this subdim-face correspond to face inSimp of S.

    int inSimp = (
        // If lowerdim = 0, the general formula can be simplified.
        lowerdim == 0 ?  front().vertices()[f] :
        FaceNumbering<dim, lowerdim>::faceNumber(
            front().vertices() * Perm<dim + 1>::extend(
                FaceNumbering<subdim, lowerdim>::ordering(f))));

    // Get the images of 0,...,lowerdim correct:
    Perm<dim + 1> p = front().vertices().inverse() *
        front().simplex()->template faceMapping<lowerdim>(inSimp);

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
        component_(component), boundaryComponent_(nullptr) {
}

template <int dim, int subdim>
inline void FaceBase<dim, subdim>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << Strings<subdim>::face;
    if (subdim < dim - 1)
        out << " of degree " << degree();
}

template <int dim, int subdim>
void FaceBase<dim, subdim>::writeTextLong(std::ostream& out) const {
    static_cast<const Face<dim, subdim>*>(this)->writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (const auto& emb : *this)
        out << "  " << emb << std::endl;
}

} // namespace regina::detail

#endif

