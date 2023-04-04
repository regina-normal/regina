
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

/*! \file triangulation/generic/isomorphism.h
 *  \brief Internal header for combinatorial isomorphisms between
 *  triangulations of arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#ifndef __REGINA_ISOMORPHISM_H
#ifndef __DOXYGEN
#define __REGINA_ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "triangulation/facetspec.h"
#include "triangulation/alias/isomorphism.h"
#include "triangulation/forward.h"
#include "maths/perm.h"
#include "utilities/exception.h"
#include "utilities/randutils.h"
#include "utilities/tightencoding.h"
#include <sys/types.h> // for ssize_t
#include <algorithm>

namespace regina {

/**
 * Represents a combinatorial isomorphism from one <i>dim</i>-manifold
 * triangulation into another.
 *
 * In essence, a combinatorial isomorphism from triangulation \a T to
 * triangulation \a U is a one-to-one map from the simplices of \a T to the
 * simplices of \a U that allows relabelling of both the simplices and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent simplices.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map \a f
 * from the simplices of \a T to the simplices of \a U, and (ii) for each
 * simplex \a S of \a T, a permutation \a f<sub>S</sub> of the facets
 * (0,...,\a dim) of \a S, for which the following condition holds:
 *
 * - If facet \a k of simplex \a S and facet \a k' of simplex \a S'
 *   are identified in \a T, then facet \a f<sub>S</sub>(\a k) of \a f(S)
 *   and facet \a f<sub>S'</sub>(\a k') of \a f(S') are identified in \a U.
 *   Moreover, their gluing is consistent with the facet/vertex permutations;
 *   that is, there is a commutative square involving the gluing maps in
 *   \a T and \a U and the permutations \a f<sub>S</sub> and \a f<sub>S'</sub>.
 *
 * Isomorphisms can be _boundary complete_ or _boundary incomplete_.
 * A boundary complete isomorphism satisfies the additional condition:
 *
 * - If facet \a x is a boundary facet of \a T then facet \a f(x) is a
 *   boundary facet of \a U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation \a T is present as an entire component (or components) of \a U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation \a T as a subcomplex of some possibly larger component
 * (or components) of \a U.
 *
 * Note that for all types of isomorphism, triangulation \a U is allowed
 * to contain more simplices than triangulation \a T.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Isomorphism2 and Isomorphism3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the triangulations that this isomorphism
 * class works with.  This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 *
 * \ingroup generic
 */
template <int dim>
class Isomorphism :
        public Output<Isomorphism<dim>>,
        public TightEncodable<Isomorphism<dim>>,
        public alias::IsomorphismImage<Isomorphism<dim>, dim> {
    static_assert(dim >= 2, "Isomorphism requires dimension >= 2.");

    protected:
        size_t size_;
            /**< The number of simplices in the source triangulation. */
        ssize_t* simpImage_;
            /**< Stores the simplex of the destination triangulation that
                 each simplex of the source triangulation maps to.
                 This array has size size_. */
        Perm<dim+1>* facetPerm_;
            /**< The permutation applied to the facets of each
                 source simplex.  This array has size size_. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \python For Python users, the images of the simplices and
         * their vertices must be set using setSimpImage() and setFacetPerm()
         * instead.
         *
         * \param nSimplices the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(size_t nSimplices);
        /**
         * Creates a copy of the given isomorphism.
         *
         * \param src the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& src);
        /**
         * Moves the given isomorphism into this new isomorphism.
         * This is a fast (constant time) operation.
         *
         * The isomorphism that is passed (\a src) will no longer be usable.
         *
         * \param src the isomorphism to move.
         */
        Isomorphism(Isomorphism&& src) noexcept;
        /**
         * Destroys this isomorphism.
         */
        ~Isomorphism();
        /**
         * Copies the given isomorphism into this isomorphism.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if they do then this isomorphism will be
         * resized as a result.
         *
         * This operator induces a deep copy of \a src.
         *
         * \param src the isomorphism to copy.
         * \return a reference to this isomorphism.
         */
        Isomorphism& operator = (const Isomorphism& src);
        /**
         * Moves the given isomorphism into this isomorphism.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if they do then this isomorphism will be
         * resized as a result.
         *
         * The isomorphism that is passed (\a src) will no longer be usable.
         *
         * \param src the isomorphism to move.
         * \return a reference to this isomorphism.
         */
        Isomorphism& operator = (Isomorphism&& src) noexcept;

        /**
         * Swaps the contents of this and the given isomorphism.
         *
         * It does not matter if this and the given isomorphism use different
         * numbers of simplices; if so then they will be adjusted accordingly.
         *
         * \param other the isomorphism whose contents are to be swapped with
         * this.
         */
        void swap(Isomorphism& other) noexcept;

        /**
         * Returns the number of simplices in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of simplices in the
         * destination triangulation.
         *
         * \python This is also used to implement the Python special
         * method __len__().
         *
         * \return the number of simplices in the source triangulation.
         */
        size_t size() const;

        /**
         * Returns a read-write reference to the image of the given source
         * simplex under this isomorphism.
         *
         * If the dimension \a dim is 2, 3 or 4, then you can also access
         * this image through the dimension-specific alias triImage(),
         * tetImage() or pentImage() respectively.
         *
         * This image is stored using type \c ssize_t, not \c size_t,
         * and so you can safely use the special value -1 as a marker for an
         * image that is unknown or not yet initialised.
         *
         * \nopython For Python users, simpImage() is a read-only function
         * that returns by value.  To edit the isomorphism, use the
         * Python-only routine setSimpImage() instead.
         *
         * \param sourceSimp the index of the source simplex; this must
         * be between 0 and `size()-1` inclusive.
         * \return a reference to the index of the destination simplex
         * that the source simplex maps to.
         */
        ssize_t& simpImage(size_t sourceSimp);
        /**
         * Determines the image of the given source simplex under
         * this isomorphism.
         *
         * If the dimension \a dim is 2, 3 or 4, then you can also access
         * this image through the dimension-specific alias triImage(),
         * tetImage() or pentImage() respectively.
         *
         * \param sourceSimp the index of the source simplex; this must
         * be between 0 and `size()-1` inclusive.
         * \return the index of the destination simplex
         * that the source simplex maps to.
         */
        ssize_t simpImage(size_t sourceSimp) const;
#ifdef __APIDOCS
        /**
         * Python-only routine that sets the image of the given source
         * simplex to the given value under this isomorphism.
         *
         * If the dimension \a dim is 2, 3 or 4, then you can also set
         * this image through the dimension-specific alias setTriImage(),
         * setTetImage() or setPentImage() respectively.
         *
         * Simplex images are stored using type \c ssize_t, not \c size_t,
         * and so you can safely use the special value -1 as a marker for an
         * image that is unknown or not yet initialised.
         *
         * \nocpp For C++ users, simpImage() is used for both reading and
         * writing: just write `simpImage(sourceSimp) = image`.
         *
         * \param sourceSimp the index of the source simplex; this must
         * be between 0 and `size()-1` inclusive.
         * \param image the index of the new destination simplex that the
         * source simplex should map to.
         */
        void setSimpImage(size_t sourceSimp, ssize_t image);
#endif
        /**
         * Returns a read-write reference to the permutation that is
         * applied to the (\a dim + 1) facets of the given source simplex
         * under this isomorphism.
         * Facet \a i of source simplex \a sourceSimp will be mapped to
         * facet `facetPerm(sourceSimp)[i]` of simplex
         * `simpImage(sourceSimp)`.
         *
         * If the dimension \a dim is 2 or 3, then you can also access
         * this permutation through the dimension-specific alias
         * edgePerm() or facePerm() respectively.
         *
         * \nopython For Python users, facetPerm() is a read-only function
         * that returns by value.  To edit the isomorphism, use the
         * Python-only routine setFacetPerm() instead.
         *
         * \param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * `size()-1` inclusive.
         * \return a read-write reference to the permutation applied to the
         * facets of the source simplex.
         */
        Perm<dim+1>& facetPerm(size_t sourceSimp);
        /**
         * Determines the permutation that is applied to the (\a dim + 1)
         * facets of the given source simplex under this isomorphism.
         * Facet \a i of source simplex \a sourceSimp will be mapped to
         * face `facetPerm(sourceSimp)[i]` of simplex
         * `simpImage(sourceSimp)`.
         *
         * If the dimension \a dim is 2 or 3, then you can also access
         * this permutation through the dimension-specific alias
         * edgePerm() or facePerm() respectively.
         *
         * \param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * `size()-1` inclusive.
         * \return the permutation applied to the facets of the
         * source simplex.
         */
        Perm<dim+1> facetPerm(size_t sourceSimp) const;
#ifdef __APIDOCS
        /**
         * Python-only routine that sets the permutation that is applied to
         * the (\a dim + 1) facets of the given source simplex under this
         * isomorphism.  Facet \a i of source simplex \a sourceSimp will be
         * mapped to facet `perm[i]` of simplex `simpImage(sourceSimp)`.
         *
         * If the dimension \a dim is 2 or 3, then you can also set
         * this permutation through the dimension-specific alias
         * setEdgePerm() or setFacePerm() respectively.
         *
         * \nocpp For C++ users, facetPerm() is used for both reading and
         * writing: just write `facetPerm(sourceSimp) = perm`.
         *
         * \param sourceSimp the index of the source simplex containing
         * the original (\a dim + 1) facets; this must be between 0 and
         * `size()-1` inclusive.
         * \param perm the new permutation that should be applied to the facets
         * of the source simplex.
         */
        void setFacetPerm(size_t sourceSimp, Perm<dim+1> perm);
#endif
        /**
         * Determines the image of the given source simplex facet
         * under this isomorphism.  This operator returns by value:
         * it cannot be used to alter the isomorphism.
         *
         * \param source the given source simplex facet; this must
         * be one of the (\a dim + 1) facets of one of the size()
         * simplices in the source triangulation.
         * \return the image of the source simplex facet under this
         * isomorphism.
         */
        FacetSpec<dim> operator [] (const FacetSpec<dim>& source) const;

        /**
         * Determines whether or not this is an identity isomorphism.
         *
         * In an identity isomorphism, each simplex image is itself,
         * and within each simplex the facet/vertex permutation is
         * the identity permutation.
         *
         * \return \c true if this is an identity isomorphism, or
         * \c false otherwise.
         */
        bool isIdentity() const;

        /**
         * Applies this isomorphism to the given triangulation, and
         * returns the result as a new triangulation.
         *
         * An isomorphism represents a combinatorial map from a triangulation
         * \a T to a triangulation \a U.  This routine treats the given
         * triangulation as the domain \a T, and returns the corresponding
         * range \a U.  The given triangulation \a T is not modified in any way.
         *
         * In more detail: A new triangulation \a U is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from \a T to \a U.  That is, \a T and \a U will be
         * combinatorially isomorphic triangulations, and this isomorphism
         * describes the mapping from the simplices of \a T and their facets
         * to the simplices of \a U and their facets.
         *
         * If the given triangulation \a T has any locks on top-dimensional
         * simplices and/or their facets, then the resulting triangulation \a U
         * will have matching locks that have been carried through the
         * isomorphism correctly (i.e., the locks will be copied over to the
         * appropriate destination simplices and/or facets).
         *
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \todo Lock the topological properties of the underlying manifold,
         * to avoid recomputing them after the isomorphism is applied.
         *
         * \exception InvalidArgument The number of top-dimensional simplices
         * in the given triangulation is not equal to size() for this
         * isomorphism.
         *
         * \param tri the triangulation to which this isomorphism
         * should be applied.
         * \return the new isomorphic triangulation.
         */
        Triangulation<dim> operator ()(const Triangulation<dim>& tri) const;

        /**
         * Returns the image of the given facet-of-simplex under this
         * isomorphism.
         *
         * Specifically:
         *
         * - If `f.simp` is in the range 0,1,...,size()-1 inclusive
         *   (i.e., \a f denotes a facet of an actual top-dimensional simplex),
         *   then this routine will return an object denoting facet
         *   `facetPerm(f.facet)` of simplex `simpImage(f.simp)`.
         *
         * - If `f.simp` is negative (i.e., \a f takes a
         *   before-the-start value), or if `f.simp` is at least size()
         *   (i.e., \a f takes a boundary or past-the-end value), then this
         *   routine will return \a f unchanged (but see the precondition
         *   below).
         *
         * \pre If this isomorphism maps a smaller triangulation into a larger
         * triangulation (in particular, if the simplex images under this
         * isomorphism are not just some reordering of 0,1,...,size()-1),
         * then \a f must not denote a boundary or past-the-end value.
         * This is because a boundary or past-the-end value is encoded
         * by using a past-the-end value of `FacetSpec::simp`.
         * If this isomorphism maps into a larger triangulation then this
         * past-the-end simplex number would need to change, but the
         * isomorphism does not actually know what the new value of
         * `FacetSpec::simp` should be.
         *
         * \param f the facet-of-simplex which should be transformed by
         * this isomorphism.
         * \return the image of \a f under this isomorphism.
         */
        FacetSpec<dim> operator ()(const FacetSpec<dim>& f) const;

        /**
         * Applies this isomorphism to the given facet pairing, and
         * returns the result as a new facet pairing.
         *
         * Although the Isomorphism class was designed to represent
         * mappings between isomorphic triangulations, it can just as well
         * describe mappings between isomorphic facet pairings.
         * In particular, if \a iso represents this isomorphism and if \a p
         * were the facet pairing of some triangulation \a tri, then
         * `iso(p)` would be the facet pairing for the triangulation
         * `iso(tri)`.  Of course, this routine works directly with
         * the facet pairing, and does not actually construct any
         * triangulations at all.
         *
         * This routine behaves correctly even if some facets of \a p are
         * unmatched (i.e., if \a p models a triangulation with boundary
         * facets).
         *
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \exception InvalidArgument The number of top-dimensional simplices
         * described by the given facet pairing is not equal to size() for
         * this isomorphism.
         *
         * \param p the facet pairing to which this isomorphism should be
         * applied.
         * \return the new isomorphic facet pairing.
         */
        FacetPairing<dim> operator ()(const FacetPairing<dim>& p) const;

        /**
         * Deprecated routine that applies this isomorphism to the given
         * triangulation, and returns the result as a new triangulation.
         *
         * If the given triangulation has any locks on top-dimensional
         * simplices and/or their facets, then the triangulation that is
         * returned will have matching locks that have been carried through
         * the isomorphism correctly (i.e., the locks will be copied over
         * to the appropriate destination simplices and/or facets).
         *
         * \deprecated If this isomorphism is \a iso, then this routine
         * is equivalent to calling `iso(tri)`.  See the bracket
         * operator for further details.
         *
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \exception InvalidArgument The number of top-dimensional simplices
         * in the given triangulation is not equal to size() for this
         * isomorphism.
         *
         * \param tri the triangulation to which this isomorphism
         * should be applied.
         * \return the new isomorphic triangulation.
         */
        [[deprecated]] Triangulation<dim> apply(const Triangulation<dim>& tri)
            const;

        /**
         * Deprecated routine that applies this isomorphism to the given
         * triangulation, modifying the given triangulation directly.
         *
         * If the given triangulation has any locks on top-dimensional
         * simplices and/or their facets, then these locks will be carried
         * through the isomorphism correctly (i.e., the locks will be moved
         * to the appropriate destination simplices and/or facets).
         *
         * \deprecated If this isomorphism is \a iso, then this routine
         * is equivalent to calling `tri = iso(tri)`.  See the bracket
         * operator for further details.
         *
         * \pre The simplex images are precisely 0,1,...,size()-1 in some
         * order (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * \exception InvalidArgument The number of top-dimensional simplices
         * in the given triangulation is not equal to size() for this
         * isomorphism.
         *
         * \param tri the triangulation to which this isomorphism
         * should be applied.
         */
        [[deprecated]] void applyInPlace(Triangulation<dim>& tri) const;

        /**
         * Returns the composition of this isomorphism with the given
         * isomorphism.
         *
         * This follows the same order convention as Regina's permutation
         * classes: the composition `a * b` first applies the right-hand
         * isomorphism \a b, and then the left-hand isomorphism \a a.
         *
         * \pre The source triangulation for this isomorphism (the left-hand
         * side) is at least as large as the destination triangulation
         * for \a rhs (the right-hand side).  In other words, the maximum
         * value of `rhs.simpImage(i)` over all \a i must be less than
         * `this->size()`.
         *
         * \return the composition of both isomorphisms.
         */
        Isomorphism operator * (const Isomorphism& rhs) const;

        /**
         * Returns the composition of this isomorphism with the given
         * isomorphism.
         *
         * This follows the same order convention as Regina's permutation
         * classes: the composition `a * b` first applies the right-hand
         * isomorphism \a b, and then the left-hand isomorphism \a a.
         *
         * \pre The source triangulation for this isomorphism (the left-hand
         * side) is at least as large as the destination triangulation
         * for \a rhs (the right-hand side).  In other words, the maximum
         * value of `rhs.simpImage(i)` over all \a i must be less than
         * `this->size()`.
         *
         * \return the composition of both isomorphisms.
         */
        Isomorphism operator * (Isomorphism&& rhs) const;

        /**
         * Returns the inverse of this isomorphism.
         *
         * \pre The destination triangulation has precisely the same
         * number of simplices as the source triangulation.  In other words,
         * there are no "gaps" in the simplex images: the values
         * `simpImage(0)`, ..., `simpImage(size()-1)` must be
         * a permutation of 0, ..., `size()-1`.
         *
         * \return the inverse isomorphism.
         */
        Isomorphism inverse() const;

        /**
         * A preincrement operator that changes this to be the next
         * isomorphism in an iteration through all possible isomorphisms
         * of this size.
         *
         * The order of iteration is lexicographical, by the sequence of
         * simplex images and then by the sequence of facet permutations.
         * Facet permutations, in turn, are ordered by their indices in
         * the array Perm<dim>::Sn.
         *
         * In particular, the identity isomorphism is the first in such an
         * iteration.  If this isomorphism is the _last_ in such an
         * iteration, then this operator will "wrap around" and set this
         * to the identity.
         *
         * \pre The class Perm<dim+1> supports the preincrement operator;
         * currently this means that \a dim must be at most 6.
         *
         * \python This routine is named inc() since Python does
         * not support the increment operator.  Unlike other Regina
         * classes, here inc() wraps the preincrement operator (not the
         * postincrement operator), since the postincrement operator is
         * significantly more expensive.  To avoid confusion, the python
         * inc() function returns \c None (not this isomorphism).
         *
         * \return a reference to this isomorphism after the increment.
         */
        Isomorphism<dim>& operator ++();

        /**
         * A postincrement operator that changes this to be the next
         * isomorphism in an iteration through all possible isomorphisms
         * of this size.
         *
         * The order of iteration is lexicographical, by the sequence of
         * simplex images and then by the sequence of facet permutations.
         * Facet permutations, in turn, are ordered by their indices in
         * the array Perm<dim>::Sn.
         *
         * In particular, the identity isomorphism is the first in such an
         * iteration.  If this isomorphism is the _last_ in such an
         * iteration, then this operator will "wrap around" and set this
         * to the identity.
         *
         * \warning Since the postincrement operator returns an
         * isomorphism by value, it is significantly more expensive than
         * the preincrement operator (since it involves a deep copy of a
         * large object).  You should use the preincrement operator unless
         * you actually need a copy of the old value of this isomorphism.
         *
         * \pre The class Perm<dim+1> supports the preincrement operator;
         * currently this means that \a dim must be at most 6.
         *
         * \nopython The preincrement operator is present in Python as the
         * member function inc().  (Note that this is different from other
         * Regina classes, where inc() typically wraps the postincrement
         * operator instead.  See the preincrement documentation for details.)
         *
         * \return a copy of this isomorphism before the increment took place.
         */
        Isomorphism<dim> operator ++(int);

        /**
         * Writes the tight encoding of this isomorphism to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Reconstructs an isomorphism from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input routine _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of an isomorphism on <i>dim</i>-dimensional
         * triangulations.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for an isomorphism on <i>dim</i>-dimensional triangulations.
         * \return the isomorphism represented by the given tight encoding.
         */
        static Isomorphism tightDecode(std::istream& input);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines whether this and the given isomorphism are identical.
         *
         * Two isomorphisms are considered _identical_ if they act on the
         * same number of top-dimensional simplices, and all destination
         * simplex numbers and facet permutations are the same for both
         * isomorphisms.
         *
         * In particular it is only the simplex, facet and vertex _labels_
         * that matter: an isomorphism does not refer to a specific
         * triangulation, and there is no sense in which the two isomorphisms
         * need to act on the same triangulations and/or point to the same
         * destination Simplex objects.
         *
         * It is safe to compare isomorphisms of different sizes (in
         * which case this routine will return \c false).
         *
         * \param other the isomorphism to compare with this.
         * \return \c true if and only if this and the given isomorphism
         * are identical.
         */
        bool operator == (const Isomorphism& other) const;

        /**
         * Determines whether this and the given isomorphism are not identical.
         *
         * Two isomorphisms are considered _identical_ if they act on the
         * same number of top-dimensional simplices, and all destination
         * simplex numbers and facet permutations are the same for both
         * isomorphisms.
         *
         * In particular it is only the simplex, facet and vertex _labels_
         * that matter: an isomorphism does not refer to a specific
         * triangulation, and there is no sense in which the two isomorphisms
         * need to act on the same triangulations and/or point to the same
         * destination Simplex objects.
         *
         * It is safe to compare isomorphisms of different sizes (in
         * which case this routine will return \c true).
         *
         * \param other the isomorphism to compare with this.
         * \return \c true if and only if this and the given isomorphism
         * are not identical.
         */
        bool operator != (const Isomorphism& other) const;

        /**
         * Returns the identity isomorphism for the given number of simplices.
         * This isomorphism sends every simplex and every vertex to itself.
         *
         * \param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * \return the identity isomorphism.
         */
        static Isomorphism<dim> identity(size_t nSimplices);

        /**
         * Returns a random isomorphism for the given number of simplices.
         * This isomorphism will reorder simplices
         * 0 to `nSimplices-1` in a random fashion, and for
         * each simplex a random permutation of its (\a dim + 1) vertices
         * will be selected.
         *
         * All possible isomorphisms for the given number of simplices are
         * equally likely.
         *
         * This routine is thread-safe, and uses RandomEngine for its
         * random number generation.
         *
         * \param nSimplices the number of simplices that the new
         * isomorphism should operate upon.
         * \param even if \c true, then every simplex will have its
         * vertices permuted with an even permutation.  This means that,
         * if the random isomorphism is applied to an oriented triangulation,
         * it will preserve the orientation.
         * \return the new random isomorphism.
         */
        static Isomorphism<dim> random(size_t nSimplices, bool even = false);
};

/**
 * Swaps the contents of the given isomorphisms.
 *
 * This global routine simply calls Isomorphism<dim>::swap(); it is provided
 * so that Isomorphism<dim> meets the C++ Swappable requirements.
 *
 * \param a the first isomorphism whose contents should be swapped.
 * \param b the second isomorphism whose contents should be swapped.
 *
 * \ingroup generic
 */
template <int dim>
void swap(Isomorphism<dim>& a, Isomorphism<dim>& b) noexcept;

// Inline functions for Isomorphism

template <int dim>
inline Isomorphism<dim>::Isomorphism(size_t nSimplices) :
        size_(nSimplices),
        simpImage_(new ssize_t[nSimplices]),
        facetPerm_(new Perm<dim+1>[nSimplices]) {
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(const Isomorphism<dim>& src) :
        size_(src.size_),
        simpImage_(new ssize_t[src.size_]),
        facetPerm_(new Perm<dim+1>[src.size_]) {
    // We use src.size_, not size_, since otherwise gcc12 issues
    // loud warnings about copying data from an empty range in the case where
    // src is empty.  (The logic does work, but the compiler doesn't realise).
    std::copy(src.simpImage_, src.simpImage_ + src.size_, simpImage_);
    std::copy(src.facetPerm_, src.facetPerm_ + src.size_, facetPerm_);
}

template <int dim>
inline Isomorphism<dim>::Isomorphism(Isomorphism<dim>&& src) noexcept:
        size_(src.size_),
        simpImage_(src.simpImage_),
        facetPerm_(src.facetPerm_) {
    src.simpImage_ = nullptr;
    src.facetPerm_ = nullptr;
}

template <int dim>
inline Isomorphism<dim>::~Isomorphism() {
    delete[] simpImage_;
    delete[] facetPerm_;
}

template <int dim>
Isomorphism<dim>& Isomorphism<dim>::operator = (const Isomorphism<dim>& src) {
    // std::copy() exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (size_ != src.size_) {
        delete[] simpImage_;
        delete[] facetPerm_;

        size_ = src.size_;

        simpImage_ = new ssize_t[size_];
        facetPerm_ = new Perm<dim+1>[size_];
    }

    // We use src.size_, not size_, since otherwise gcc12 issues
    // loud warnings about copying data from an empty range in the case where
    // src is empty.  (The logic does work, but the compiler doesn't realise).
    std::copy(src.simpImage_, src.simpImage_ + src.size_, simpImage_);
    std::copy(src.facetPerm_, src.facetPerm_ + src.size_, facetPerm_);
    return *this;
}

template <int dim>
Isomorphism<dim>& Isomorphism<dim>::operator = (Isomorphism<dim>&& src)
        noexcept{
    size_ = src.size_;
    std::swap(simpImage_, src.simpImage_);
    std::swap(facetPerm_, src.facetPerm_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <int dim>
void Isomorphism<dim>::swap(Isomorphism<dim>& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(simpImage_, other.simpImage_);
    std::swap(facetPerm_, other.facetPerm_);
}

template <int dim>
inline size_t Isomorphism<dim>::size() const {
    return size_;
}

template <int dim>
inline ssize_t& Isomorphism<dim>::simpImage(size_t sourceSimp) {
    return simpImage_[sourceSimp];
}

template <int dim>
inline ssize_t Isomorphism<dim>::simpImage(size_t sourceSimp) const {
    return simpImage_[sourceSimp];
}

template <int dim>
inline Perm<dim+1>& Isomorphism<dim>::facetPerm(size_t sourceSimp) {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline Perm<dim+1> Isomorphism<dim>::facetPerm(size_t sourceSimp) const {
    return facetPerm_[sourceSimp];
}

template <int dim>
inline FacetSpec<dim> Isomorphism<dim>::operator [] (
        const FacetSpec<dim>& source) const {
    return FacetSpec<dim>(simpImage_[source.simp],
        facetPerm_[source.simp][source.facet]);
}

template <int dim>
bool Isomorphism<dim>::isIdentity() const {
    for (size_t p = 0; p < size_; ++p) {
        if (simpImage_[p] != static_cast<ssize_t>(p))
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

template <int dim>
Triangulation<dim> Isomorphism<dim>::operator ()(
        const Triangulation<dim>& original) const {
    if (original.size() != size_)
        throw InvalidArgument("Isomorphism::operator() was given "
            "a triangulation of the wrong size");

    if (size_ == 0)
        return Triangulation<dim>();

    Triangulation<dim> ans;
    auto* tet = new Simplex<dim>*[size_];

    // Ensure only one event pair is fired in this sequence of changes.
    typename Triangulation<dim>::ChangeEventSpan span(ans);
    for (size_t t = 0; t < size_; t++)
        tet[t] = ans.newSimplex();

    const Simplex<dim> *src, *adj;
    Simplex<dim> *dest;

    for (size_t t = 0; t < size_; t++) {
        src = original.simplex(t);
        dest = tet[simpImage_[t]];

        dest->setDescription(src->description());
        if (src->isLocked())
            dest->lock();
    }

    for (size_t t = 0; t < size_; t++) {
        src = original.simplex(t);
        for (int f = 0; f <= dim; f++)
            if ((adj = src->adjacentSimplex(f))) {
                // We have an adjacent simplex.
                size_t adjTetIndex = adj->index();
                Perm<dim+1> gluingPerm = src->adjacentGluing(f);

                // Make the gluing (and any necessary locks) from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f)) {
                    dest = tet[simpImage_[t]];
                    dest->join(facetPerm_[t][f], tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
                    if (src->isFacetLocked(f))
                        dest->lockFacet(facetPerm_[t][f]);
                }
            } else {
                if (src->isFacetLocked(f))
                    tet[simpImage_[t]]->lockFacet(facetPerm_[t][f]);
            }
    }

    delete[] tet;
    return ans;
}

template <int dim>
inline FacetSpec<dim> Isomorphism<dim>::operator ()(const FacetSpec<dim>& f)
        const {
    if (f.simp >= 0 && f.simp < static_cast<ssize_t>(size_)) {
        return FacetSpec<dim>(simpImage_[f.simp], facetPerm_[f.simp][f.facet]);
    } else {
        // Past-the-end or before-the-start values should be left alone.
        return f;
    }
}

template <int dim>
FacetPairing<dim> Isomorphism<dim>::operator ()(
        const FacetPairing<dim>& p) const {
    if (p.size() != size_)
        throw InvalidArgument("Isomorphism::operator() was given "
            "a facet pairing of the wrong size");

    FacetPairing<dim> ans(size_);

    for (FacetSpec<dim> f(0, 0); f.simp < static_cast<ssize_t>(size_); ++f)
        ans.dest((*this)(f)) = (*this)(p[f]);

    return ans;
}

template <int dim>
inline Triangulation<dim> Isomorphism<dim>::apply(
        const Triangulation<dim>& original) const {
    return (*this)(original);
}

template <int dim>
inline void Isomorphism<dim>::applyInPlace(Triangulation<dim>& tri) const {
    tri = (*this)(tri);
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::operator * (const Isomorphism& rhs) const {
    Isomorphism<dim> ans(rhs.size_);
    for (size_t i = 0; i < rhs.size_; ++i) {
        ans.simpImage_[i] = simpImage_[rhs.simpImage_[i]];
        ans.facetPerm_[i] = facetPerm_[rhs.simpImage_[i]] * rhs.facetPerm_[i];
    }
    return ans;
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::operator * (Isomorphism&& rhs) const {
    // We will construct the result by overwriting rhs.
    for (size_t i = 0; i < rhs.size_; ++i) {
        rhs.facetPerm_[i] = facetPerm_[rhs.simpImage_[i]] * rhs.facetPerm_[i];
        rhs.simpImage_[i] = simpImage_[rhs.simpImage_[i]];
    }
    return std::move(rhs);
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::inverse() const {
    Isomorphism<dim> ans(size_);
    for (size_t i = 0; i < size_; ++i) {
        ans.simpImage_[simpImage_[i]] = i;
        ans.facetPerm_[simpImage_[i]] = facetPerm_[i].inverse();
    }
    return ans;
}

template <int dim>
Isomorphism<dim>& Isomorphism<dim>::operator ++() {
    static_assert(dim <= 6,
        "Currently the Isomorphism<dim> pre/postincrement operators "
        "are only available for dimensions dim <= 6.");

    if (size_ == 0)
        return *this;

    for (ssize_t i = size_ - 1; i >= 0; --i)
        if (! (++facetPerm_[i]).isIdentity())
            return *this;

    std::next_permutation(simpImage_, simpImage_ + size_);
    return *this;
}

template <int dim>
inline Isomorphism<dim> Isomorphism<dim>::operator ++(int) {
    Isomorphism<dim> prev(*this);
    ++(*this);
    return prev;
}

template <int dim>
void Isomorphism<dim>::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeIndex(out, size_);
    for (size_t i = 0; i < size_; ++i)
        regina::detail::tightEncodeIndex(out, simpImage_[i]);
    if constexpr (dim == 2) {
        // We can fit two permutations per character here.
        for (size_t i = 0; i < size_; i += 2) {
            if (i + 1 == size_)
                regina::detail::tightEncodeIndex(out, static_cast<unsigned>(
                    facetPerm_[i].SnIndex()));
            else
                regina::detail::tightEncodeIndex(out, static_cast<unsigned>(
                    facetPerm_[i].SnIndex() + 6 * facetPerm_[i+1].SnIndex()));
        }
    } else {
        for (size_t i = 0; i < size_; ++i)
            facetPerm_[i].tightEncode(out);
    }
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::tightDecode(std::istream& input) {
    auto n = regina::detail::tightDecodeIndex<size_t>(input);
    Isomorphism ans(n);

    // We don't check the values of simpImage_[...], since we want to
    // support the negative "unknown" placeholder value for simpImage_[...].
    // Perm<dim+1>::tightDecode() will check the validity of the permutations
    // that are read.
    for (size_t i = 0; i < n; ++i)
        ans.simpImage_[i] = regina::detail::tightDecodeIndex<ssize_t>(input);
    if constexpr (dim == 2) {
        for (size_t i = 0; i < n; i += 2) {
            auto p = regina::detail::tightDecodeIndex<unsigned>(input);
            if (i + 1 == n) {
                if (p >= 6)
                    throw InvalidInput("The tight encoding contains "
                        "invalid permutations");
                ans.facetPerm_[i] = Perm<dim+1>::Sn[p];
            } else {
                if (p >= 36)
                    throw InvalidInput("The tight encoding contains "
                        "invalid permutations");
                ans.facetPerm_[i] = Perm<dim+1>::Sn[p % 6];
                ans.facetPerm_[i+1] = Perm<dim+1>::Sn[p / 6];
            }
        }
    } else {
        for (size_t i = 0; i < n; ++i)
            ans.facetPerm_[i] = Perm<dim+1>::tightDecode(input);
    }

    return ans;
}

template <int dim>
inline void Isomorphism<dim>::writeTextShort(std::ostream& out) const {
    for (size_t i = 0; i < size_; ++i) {
        if (i > 0)
            out << ", ";
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ')';
    }
}

template <int dim>
inline void Isomorphism<dim>::writeTextLong(std::ostream& out) const {
    for (size_t i = 0; i < size_; ++i)
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ")\n";
}

template <int dim>
inline bool Isomorphism<dim>::operator == (const Isomorphism& other) const {
    return size_ == other.size_ &&
        std::equal(simpImage_, simpImage_ + size_, other.simpImage_) &&
        std::equal(facetPerm_, facetPerm_ + size_, other.facetPerm_);
}

template <int dim>
inline bool Isomorphism<dim>::operator != (const Isomorphism& other) const {
    return ! ((*this) == other);
}

template <int dim>
inline Isomorphism<dim> Isomorphism<dim>::identity(size_t nSimplices) {
    Isomorphism<dim> id(nSimplices);
    for (size_t i = 0; i < nSimplices; ++i)
        id.simpImage_[i] = i;
    return id;
}

template <int dim>
Isomorphism<dim> Isomorphism<dim>::random(size_t nSimplices, bool even) {
    Isomorphism<dim> ans(nSimplices);

    // Prepare the destination simplices.
    size_t i;
    for (i = 0; i < nSimplices; ++i)
        ans.simpImage_[i] = i;

    // Construct the RandomEngine as late as possible, because it grabs
    // a mutex lock.
    RandomEngine engine;

    // Randomly choose the destination simplices.
    std::shuffle(ans.simpImage_, ans.simpImage_ + nSimplices, engine.engine());

    // Randomly choose the individual permutations.
    for (i = 0; i < nSimplices; ++i)
        ans.facetPerm_[i] = Perm<dim+1>::rand(engine.engine(), even);

    return ans;
}

template <int dim>
inline void swap(Isomorphism<dim>& a, Isomorphism<dim>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

