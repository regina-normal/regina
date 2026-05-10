
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

/*! \file triangulation/isosig.h
 *  \brief Defines different encodings and signature types for isomorphism
 *  signatures of triangulations.
 */

#ifndef __REGINA_ISOSIG_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_H
#endif

#include <algorithm>
#include <array>
#include "regina-core.h"
#include "concepts/maths.h"
#include "maths/perm.h"
#include "triangulation/forward.h"
#include "utilities/bitmask.h"
#include "utilities/bytesequence.h"
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim, int subdim>
requires (dim >= 1 && dim <= maxDim() && subdim >= 0 && subdim < dim)
class FaceNumbering;

template <int generation, int dim>
requires ((generation == 1 || generation == 2) && supportedDim(dim))
class IsoSigData;

/**
 * Represents an encoding that can be used for the given generation of
 * isomorphism signatures for triangulations.  Essentially, the job of an
 * encoding algorithm is to pack the gluings table for a single triangulation
 * component into a small piece of data (such as a string) that is easily
 * transported.
 *
 * An encoding should provide a type alias `Signature`, indicating the type
 * that holds the final signature (e.g., `std::string`).  In addition, it
 * should provide the following static routines:
 *
 * - `encodeEmpty()`, which encodes the empty <i>dim</i>-dimensional
 *   triangulation;
 *
 * - `encode(const IsoSigData<generation, dim>&)`, which encodes the gluings
 *   table for a single connected component of a <i>dim</i>-dimensional
 *   triangulation;
 *
 * - `length(const IsoSigData<generation, dim>&)`, which pre-computes the
 *   length of the signature that encodes a single component.
 *
 * Both encoding routines should return the type `Signature`.
 *
 * Both `encode()` and `length()` may assume that the given component is
 * non-empty, and that it uses a canonical labelling in the sense described in
 * the IsoSigData class notes.
 *
 * Note that `encode()` can be economical about what information it writes:
 * although `data.size()` will need to be encoded, it is typically not necessary
 * to encode the sizes of the various supporting bitmasks/arrays, since their
 * sizes are usually implied by their contents.
 *
 * \tparam generation the generation of signature to encode; this must be
 * either 1 or 2.
 * \tparam dim the dimension of triangulation for which we are building
 * isomorphism signatures.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <typename T, int generation, int dim>
concept IsoSigEncoding =
    requires(const IsoSigData<generation, dim> data) {
        typename T::Signature;
        requires SignatureType<typename T::Signature>;
        { T::encodeEmpty() } -> std::convertible_to<typename T::Signature>;
        { T::encode(data) } -> std::convertible_to<typename T::Signature>;
        { T::length(data) } -> std::same_as<size_t>;
    };

/**
 * Represents a signature _type_ that can be used for isomorphism signatures of
 * triangulations (both first-generation and second-generation).  Essentially,
 * the job of a signature type is to help Regina determine which labelling of a
 * triangulation is "canonical".  Different signature types will make different
 * trade-offs between factors such as speed, accessibility, backward
 * compatibility and so on, typically resulting in different notions of
 * "canonical" as a consequence.
 *
 * More specifically, a signature type is described by a class (implementing
 * this IsoSigType concept), which works with a single component \a c of a
 * <i>dim</i>-dimenensional triangulation.  The sole task of this class is to
 * iterate through a selection of combinations `(s, p)`, each of which
 * identifies a "starting simplex" of \a c and a "starting labelling" of its
 * vertices.  Here \a s is a top-dimensional simplex in \a c that will act as
 * the "starting simplex", and \a p is a permutation that maps the vertices
 * of \a s to the "starting labelling" `0,1,...,dim`.
 *
 * Second-generation signatures can be either _oriented_ or _unoriented_, and
 * a signature type should support both variants.  When building an oriented
 * signature, the permutation \a p must always be even (since the signature
 * must preserve the orientation of the triangulation component).  When building
 * an unoriented signature, any permutation \a p is allowed.
 *
 * Not all possible pairs `(s, p)` need to be offered during this iteration
 * (indeed, it is the ability to prune the candidate pairs `(s, p)` that make
 * some signature types faster than others).  However, we do require:
 *
 * - At least one candidate pair `(s, p)` must be offered.
 *
 * - Suppose we relabel the top-dimensional simplices of \a c and/or their
 *   vertices (when building an oriented signature, we insist that such a
 *   relabelling must be orientation-preserving).  Then the candidate pairs
 *   `(s, p)` that are offered will be the _same set_ as before, just modified
 *   according to this relabelling.  In other words, relabelling does not
 *   change the set of candidate pairs in any way beyond the relabelling itself.
 *
 * An instance of a signature type class acts like an iterator: it holds a
 * single candidate combination `(s, p)`.  In this sense, it must provide:
 *
 * - a class constructor `T(const Component<dim>&, bool)`, which sets the
 *   "iterator" to the first candidate pair `(s, p)` for the given component,
 *   and whose boolean argument indicates whether the signature being
 *   constructed is oriented (`true`) or unoriented (`false`);
 *
 * - a query routine `size_t simplex() const`, which returns the index of the
 *   current "starting simplex" \a s within the component \a c (this might
 *   _not_ be the index of \a s within the overall triangulation);
 *
 * - a query routine `Perm<dim+1> perm() const`, which returns the current
 *   "starting labelling" \a p (mapping the vertices of \a s to `0,1,...,dim`);
 *
 * - a routine `bool next()`, which advances this "iterator" to the next
 *   candidate pair `(s, p)`, returning `true` if this successful or `false`
 *   if there are no more candidate pairs.
 *
 * The routines `simplex()`, `perm()` and `next()` may all assume that the
 * "iterator" is describing a valid candidate pair `(s, p)`; that is, `next()`
 * has not yet returned `false`.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <typename T, int dim>
concept IsoSigType =
    std::constructible_from<T, const Component<dim>&, bool> &&
    requires(T t, const T c) {
        { c.simplex() } -> std::convertible_to<size_t>;
        { c.perm() } -> std::convertible_to<Perm<dim + 1>>;
        { t.next() } -> std::convertible_to<bool>;
    };

/**
 * Holds the combinatorial data required to reconstruct a single non-empty
 * connected component of a <i>dim</i>-dimensional triangulation, up to
 * relabelling, for use with the given generation of isomorphism signatures.
 *
 * This class offers different interfaces for first-generation versus
 * second-generation signatures, due to the differences in how these signatures
 * are constructed.  See the specialisations `IsoSigData<1, dim>` and
 * `IsoSigData<2, dim>` for further details.
 *
 * \tparam generation the generation of isomorphism signature that we are
 * working with; this must be either 1 or 2.
 * \tparam dim the dimension of triangulation with which we are working.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int generation, int dim>
requires ((generation == 1 || generation == 2) && supportedDim(dim))
class IsoSigData;

/**
 * Holds the combinatorial data required to reconstruct a single non-empty
 * connected component of a <i>dim</i>-dimensional triangulation, up to
 * relabelling, for use with first-generation isomorphism signatures as
 * returned by `Triangulation<dim>::isoSig()`.  This is a halfway point between
 * signatures and triangulations: the data is small and uses simple numerical
 * types, and is easy to encode or decode in an appropriate signature form
 * (e.g., a base64 string).
 *
 * Specifically, this data encodes a "compressed" gluings table for a
 * "canonical" labelling of a triangulation component.  Both the notions of
 * "compressed" and "canonical" involve stepping through the facets of
 * top-dimensional simplices in the usual order (simplex 0, facets `0,...,dim`;
 * then simplex 1, facets `0,...,dim`; and so on).  Some of these facets will
 * be boundary (i.e., unglued), some will be glued to "new" facets that we
 * have not yet stepped through, and some will be glued to "old" facets (i.e.,
 * representing a `(dim-1)`-face of the triangulation that we have already seen
 * from the other side).
 *
 * The notion of "canonical" differs between first-generation signatures
 * (`isoSig()`) and second-generation signatures (`neoSig()`).
 * To be _canonical_ for a first-generation signature:
 *
 * - As we step through the facets, if we reach a facet `f` that is glued to a
 *   partner `g` belonging to a top-dimensional simplex that we have not yet
 *   seen before, we require that the simplex containing `g` uses the next
 *   available simplex label, and that the gluing between `f` and `g` uses the
 *   identity permutation.  So, for example, the first facet of simplex 0 that
 *   is glued to some _other_ simplex must be glued to simplex 1 using the
 *   identity permutation.  Note that, in constrast to second-generation
 *   signatures, if the component has more than one top-dimensional simplex
 *   then this will never result in an oriented labelling.
 *
 * - Your chosen signature _type_ may impose further requirements on the
 *   choice of simplex 0 and its vertex labels; see the IsoSigType concept
 *   documentation for details.
 *
 * The first condition above ensures that, once we have chosen which
 * top-dimensional simplex will be simplex 0 and how its vertices `0,...,dim`
 * will be labelled, the labelling of the remaining simplices and their vertices
 * will be uniquely determined (and can be computed in linear time).
 *
 * Given a canonical labelling, a _compressed gluings table_ for a
 * first-generation signature holds the following information:
 *
 * - For each facet \a f that we step through in order, _ignoring_ those glued
 *   to "old" facets, we record whether \a f is: boundary (type 0); glued to a
 *   "new" facet of a previously-unseen simplex using the identity permutation
 *   (type 1); or glued to a "new" facet of a simplex that has already been seen
 *   (type 2).  This is stored in an array whose length is the total number of
 *   `(dim-1)`-faces in the triangulation component.
 *
 * - For each facet \a f of type 2, we record the top-dimensional simplex number
 *   of the facet that \a f is glued to.  This is stored in an array whose
 *   length is the total number of facets of type 2.
 *
 * - For each facet \a f of type 2, we record the ordered `S_n` index of the
 *   specific gluing permutation between \a f and its partner, again using an
 *   array whose length is the total number of facets of type 2.  Note that
 *   this is different from second-generation signatures, which use the plain
 *   (not ordered) `S_n` index.
 *
 * - Finally, if (and only if) there are any simplex or facet locks in this
 *   triangulation component, we record the lock masks of all top-dimensional
 *   simplices in order (simplex `0,1,2,...`).
 *
 * A first-generation signature _encoding_ is responsible for encoding this
 * data in its final signature form (e.g., a base64 string); see the
 * IsoSigEncoding concept documentation for details.  The final signature will
 * use the labelling that minimises this encoding, under the inherent ordering
 * of the resulting signature type (e.g., the inherent ordering on strings).
 * Note that this works differently from second-generation signatures, which
 * minimise the compressed gluings table _before_ it is encoded.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \pyname{IsoSigData1}
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
class IsoSigData<1, dim> {
    private:
        size_t size_;
            /**< The total number of top-dimensional simplices in this
                 triangulation component. */
        FixedArray<uint8_t> facetType_;
            /**< The facet types, as described above; each element of this
                 array will be 0, 1 or 2. */
        FixedArray<size_t> adjSimplex_;
            /**< For facets of type 2, the simplex numbers that the facets are
                 glued to. */
        FixedArray<typename Perm<dim + 1>::Index> adjGluing_;
            /**< For facets of type 2, the corresponding gluing permutations. */
        FixedArray<typename Simplex<dim>::LockMask> locks_;
            /**< The lock masks of all top-dimensional simplices, or the empty
                 array if there are no simplex or facet locks. */

    public:
        /**
         * Allocates space for a data set for the given triangulation component.
         *
         * The arrays that make up this data set will be initialised to the
         * correct sizes, but the individual array elements will be left
         * uninitialised.  You will need to call fillFrom() before this data
         * set can be used (i.e., before any other methods are called).
         *
         * \param component the triangulation component that we intend to
         * encode.
         */
        IsoSigData(Component<dim>* component) :
                size_(component->size()),
                facetType_(component->countFacets()),
                adjSimplex_(facetType_.size()
                    - component->countBoundaryFacets() - size_ + 1),
                adjGluing_(adjSimplex_.size()),
                locks_(component->hasLocks() ? size_ : 0) {
        }

        /**
         * Makes a new deep copy of the given data set.
         */
        IsoSigData(const IsoSigData&) = default;

        /**
         * Moves the contents of the given data set into this new data set.
         * This is a fast (constant time) operation.
         *
         * The data set that was passed will no longer be usable.
         */
        IsoSigData(IsoSigData&&) noexcept = default;

        /**
         * Returns the total number of top-dimensional simplices in the
         * connected triangulation component that this data set describes.
         *
         * \return the total number of top-dimensional simplices.
         */
        size_t size() const {
            return size_;
        }
        /**
         * Gives read-only access to the array of facet types, as described in
         * the class notes.
         *
         * This will be an array of integers 0, 1 or 2, corresponding to the
         * facets of top-dimensional simplices that we step through in order as
         * described in the class notes.  Facets that are glued to "old" partner
         * facets (i.e., facets that represent internal `(dim-1)`-faces of the
         * triangulation and have already been seen from the other side) are
         * not represented at all in this array.
         *
         * Each integer in the array indicates whether a particular facet of a
         * top-dimensional simplex is boundary (0), glued to a "new" partner
         * facet of a previously-unseen simplex (1), or glued to a "new" partner
         * facet belonging to a simplex that has already been seen before (2).
         *
         * The length of the array will be the total number of `(dim-1)`-faces
         * in the triangulation component being encoded.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of facet types.
         */
        const FixedArray<uint8_t>& facetTypes() const {
            return facetType_;
        }
        /**
         * Gives read-only access to the array of gluing destinations, as
         * described in the class notes.
         *
         * The length of this array will be the number of facets of
         * top-dimensional simplices of type 2 (as described in the class
         * notes).
         *
         * Each element of this array corresponds to some facet \a f of type 2,
         * and indicates the index of the top-dimensional simplex containing
         * the partner facet to which \a f is glued.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of gluing destinations.
         */
        const FixedArray<size_t>& adjacentSimplices() const {
            return adjSimplex_;
        }
        /**
         * Gives read-only access to the array of gluing permutations, as
         * described in the class notes.
         *
         * The length of this array will be the number of facets of
         * top-dimensional simplices of type 2 (as described in the class
         * notes).
         *
         * Each element of this array holds the ordered `S_n` index of the
         * gluing permutation for the relevant facet.  Note that, in constrast
         * to second-generation signatures, here we used the ordered `S_n`
         * index and not the plain `S_n` index.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of gluing permutations.
         */
        const FixedArray<typename Perm<dim + 1>::Index>& adjacentGluings()
                const {
            return adjGluing_;
        }
        /**
         * Gives read-only access to the array of lock masks, as described in
         * the class notes.
         *
         * If the triangulation component being encoded has no simplex or
         * facet locks, then this will be an empty array.  Otherwise it will
         * be an array whose length is the total number of top-dimensional
         * simplices in the component, and whose elements are the lock masks
         * for the individual simplices (as indicated by
         * `Simplex<dim>`::lockMask()`).
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of lock masks.
         */
        const FixedArray<typename Simplex<dim>::LockMask>& locks() const {
            return locks_;
        }
        /**
         * Indicates whether the triangulation component being encoded has any
         * simplex or facet locks.
         *
         * This will be `true` if and only if the array returned by locks() is
         * non-empty.
         *
         * \return a reference to the (possibly empty) array of lock masks.
         */
        bool hasLocks() const {
            return ! locks_.empty();
        }

        /**
         * Determines the (unique) canonical labelling of the triangulation
         * component for a particular choice of simplex 0 and vertices
         * `0,...,dim`, and fills this structure with the corresponding
         * compressed gluings table.
         *
         * See the class notes for full details on canonical labellings.  In the
         * gluings table that we build here, \a simplex will be labelled as
         * simplex 0, and its vertices `vertices[0],...,vertices[dim]` will be
         * labelled as vertices `0,...,dim`.
         *
         * This routine may be (and typically will be) called many times for
         * different choices of \a simplex and \a vertices; this is perfectly
         * safe, and each subsequent call to fillFrom() will overwrite the
         * previously-stored gluings table.
         *
         * \pre The given simplex belongs to the triangulation component that
         * was originally passed to the IsoSigData constructor.
         *
         * \pre If the isomorphism \a relabelling is non-null, then it must
         * have been constructed for \a size top-dimensional simplices, where
         * \a size is the size of the entire _triangulation_, not just the
         * component being encoded.
         *
         * \param simplex any top-dimensional simplex in the triangulation
         * component being encoded.
         * \param vertices any ordering of the vertices of the given simplex.
         * \param relabelling if non-null, this isomorphism will be filled with
         * the isomorphism from the original triangulation to the canonical
         * labelling.  If the underlying triangulation has multiple components,
         * then only the information relating to this component will be changed;
         * other simplex/vertex images in \a relabelling will be left untouched.
         */
        void fillFrom(const Simplex<dim>* simplex, Perm<dim + 1> vertices,
            Isomorphism<dim>* relabelling = nullptr);

        /**
         * Moves the contents of the given data set into this data set.
         *
         * It does not matter whether this and the given data set refer to
         * triangulation components of the same size.
         *
         * The data set that was passed will no longer be usable.
         *
         * \return a reference to this data set.
         */
        IsoSigData& operator = (IsoSigData&&) noexcept = default;

        // Disable copy assignment, since we do not want to worry about
        // reallocation if the triangulations have different sizes.
        IsoSigData& operator = (const IsoSigData&) = delete;

        /**
         * Swaps the contents of this and the given triangulation component
         * data.
         *
         * This routine will behave correctly if \a other is in fact
         * this data set.
         *
         * \param other the component data whose contents should be swapped
         * with this.
         */
        void swap(IsoSigData& other) noexcept {
            std::swap(size_, other.size_);
            facetType_.swap(other.facetType_);
            adjSimplex_.swap(other.adjSimplex_);
            adjGluing_.swap(other.adjGluing_);
            locks_.swap(other.locks_);
        }

        /**
         * Determines whether this and the given triangulation component data
         * hold identical information.
         *
         * \return \c true if and only if this and the given data are identical.
         */
        bool operator == (const IsoSigData&) const = default;
};

/**
 * Holds the combinatorial data required to reconstruct a single non-empty
 * connected component of a <i>dim</i>-dimensional triangulation, up to
 * relabelling, for use with second-generation isomorphism signatures as
 * returned by `Triangulation<dim>::neoSig()`.  This is a halfway point between
 * signatures and triangulations: the data is small and uses simple numerical
 * types, and is easy to encode or decode in an appropriate signature form
 * (e.g., a base64 string).
 *
 * Specifically, this data encodes a "compressed" gluings table for a
 * "canonical" labelling of a triangulation component.  Both the notions of
 * "compressed" and "canonical" involve stepping through the facets of
 * top-dimensional simplices in the usual order (simplex 0, facets `0,...,dim`;
 * then simplex 1, facets `0,...,dim`; and so on).  Some of these facets will
 * be boundary (i.e., unglued), some will be glued to "new" facets that we
 * have not yet stepped through, and some will be glued to "old" facets (i.e.,
 * representing a `(dim-1)`-face of the triangulation that we have already seen
 * from the other side).
 *
 * The notion of "canonical" differs between first-generation signatures
 * (`isoSig()`) and second-generation signatures (`neoSig()`).
 * To be _canonical_ for a second-generation signature:
 *
 * - As we step through the facets, if we reach a facet `f` that is glued to a
 *   partner `g` belonging to a top-dimensional simplex that we have not yet
 *   seen before, we require that the simplex containing `g` uses the next
 *   available simplex label, and that the gluing between `f` and `g` uses the
 *   permutation that swaps 0 with 1.  So, for example, the first facet of
 *   simplex 0 that is glued to some _other_ simplex must be glued to simplex 1
 *   using the permutation `Perm<dim+1>(0,1)`.  Note that, in contrast to
 *   first-generation signatures, if the component is orientable then this will
 *   always result in an oriented labelling.
 *
 * - Your chosen signature _type_ may impose further requirements on the
 *   choice of simplex 0 and its vertex labels; see the IsoSigType concept
 *   documentation for details.
 *
 * The first condition above ensures that, once we have chosen which
 * top-dimensional simplex will be simplex 0 and how its vertices `0,...,dim`
 * will be labelled, the labelling of the remaining simplices and their vertices
 * will be uniquely determined (and can be computed in linear time).
 *
 * Given a canonical labelling, a _compressed gluings table_ for a
 * second-generation signature holds the following information:
 *
 * - For each facet \a f that we step through in order, _ignoring_ those glued
 *   to "old" facets, we record whether \a f is: glued to a "new" facet of a
 *   previously-unseen simplex using the permutation swapping 0 and 1 (type A);
 *   or either boundary or glued to a "new" facet of a simplex that has already
 *   been seen (type B).  This is stored in a bitmask whose length is the total
 *   number of `(dim-1)`-faces in the triangulation component.  Comparing these
 *   types to the types used in first-generation signatures: type A corresponds
 *   to the old type 1, and type B combines the old types 0 and 2.
 *
 * - For each facet \a f of type B, we record the top-dimensional simplex number
 *   of the facet that \a f is glued to; if \a f is boundary then this number
 *   will be the total number of top-dimensional simplices in the component.
 *   This is stored in an array whose length is the total number of facets of
 *   type B.
 *
 * - For each non-boundary facet \a f of type B, we record the `S_n` index of
 *   the specific gluing permutation between \a f and its partner, using an
 *   array whose length is the total number of non-boundary facets of type B.
 *   Note that this is different from first-generation signatures, which use
 *   the _ordered_ `S_n` index.
 *
 * - Finally, if (and only if) there are any simplex or facet locks in this
 *   triangulation component, we record the lock masks of all top-dimensional
 *   simplices in order (simplex `0,1,2,...`).
 *
 * A second-generation signature _encoding_ is responsible for encoding this
 * data in its final signature form (e.g., a base64 string); see the
 * IsoSigEncoding concept documentation for details.  The final signature will
 * use the labelling that minimises the compressed gluings table, under the
 * inherent ordering of `IsoSigData<2, dim>`.  Note that this works differently
 * from first-generation signatures, which minimise the _encoding_ (e.g., the
 * base64 string).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \pyname{IsoSigData2}
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigData<2, dim> {
    public:
        /**
         * An unsigned integer type that can hold the index of a gluing
         * permutation in a <i>dim</i>-dimensional triangulation.  This is
         * provided to help with bitwise encodings, which read/write
         * permutations via unsigned integers.
         */
        using UnsignedPermIndex = MakeUnsigned<typename Perm<dim + 1>::Index>;

        /**
         * The number of bits needed to store the index of a gluing permutation
         * in a <i>dim</i>-dimensional triangulation.  This is _not_ the
         * bitwidth of `Perm<...>::Index`; instead it is the smallest number of
         * bits required, given the maximum possible value of such an index.
         */
        static constexpr int permBits = bitsRequired(Perm<dim + 1>::nPerms);

        /**
         * The number of bits needed to store a lock mask, which encodes all
         * simplex and/or facet locks for a single <i>dim</i>-dimensional
         * simplex.  Again, this is the smallest number of bits required
         * given the range of possible values.
         */
        static constexpr int lockBits = dim + 2;

    private:
        size_t size_;
            /**< The total number of top-dimensional simplices in this
                 triangulation component. */
        bool oriented_;
            /**< Indicates whether the labelling that this data set encodes is
                 oriented.  We cache this when we build our relabelling within
                 minimal(), since oriented_ takes linear time to compute, and
                 encoding classes may need to query it multiple times. */
        Bitmask facetType_;
            /**< The facet types, as described above.  The bits that are set
                 correspond to facets of type B; however, the bits are stored
                 in reverse order so that Bitmask::numericalComp() treats the
                 first facet as most significant. */
        FixedArray<size_t> adjSimplex_;
            /**< For facets of type B, the simplex numbers that the facets are
                 glued to, or \a size_ for boundary facets. */
        FixedArray<typename Perm<dim + 1>::Index> adjGluing_;
            /**< For non-boundary facets of type B, the `S_n` indices of
                 the corresponding gluing permutations. */
        FixedArray<typename Simplex<dim>::LockMask> locks_;
            /**< The lock masks of all top-dimensional simplices, or the empty
                 array if there are no simplex or facet locks. */

    public:
        /**
         * Chooses a canonical labelling for the given triangulation component
         * that minimises the compressed gluings table.
         *
         * The minimisation will use the inherent ordering of IsoSigData.
         *
         * \python Python does not support C++ templates.  Instead, you should
         * pass \a Type as the first runtime argument; that is,
         * `minimal(type, component, oriented, relabelling)`.
         *
         * \tparam Type an isomorphism signature type, which will be used to
         * determine which labellings are considered "canonical".
         *
         * \param component the triangulation component that this data set
         * should represent.
         * \param oriented indicates whether we are building an oriented
         * isomorphism signature (`true`), or an unoriented signature (`false`).
         * If this is `true` and if \a comp is oriented, then the relabelling
         * that is chosen will be orientation-preserving.
         * \param relabelling if non-null, this isomorphism will be filled with
         * the isomorphism from the original triangulation to the chosen
         * labelling.  If the underlying triangulation has multiple components,
         * then only the information relating to this component will be changed;
         * other simplex/vertex images in \a relabelling will be left untouched.
         * \return the data set that describes the minimal labelling, as
         * described above.
         */
        template <IsoSigType<dim> Type>
        static IsoSigData minimal(Component<dim>* component, bool oriented,
            Isomorphism<dim>* relabelling = nullptr);

        /**
         * Makes a new deep copy of the given data set.
         */
        IsoSigData(const IsoSigData&) = default;

        /**
         * Moves the contents of the given data set into this new data set.
         * This is a fast (constant time) operation.
         *
         * The data set that was passed will no longer be usable.
         */
        IsoSigData(IsoSigData&&) noexcept = default;

        /**
         * Returns the total number of top-dimensional simplices in the
         * connected triangulation component that this data set describes.
         *
         * \return the total number of top-dimensional simplices.
         */
        size_t size() const {
            return size_;
        }
        /**
         * Gives read-only access to the bitmask of facet types, as described in
         * the class notes.
         *
         * The bits correspond to the facets of top-dimensional simplices that
         * we step through as described in the class notes, but are stored in
         * _reverse_ order; this ensures that `Bitmask::numericalComp()` treats
         * the first facet (not the last) as most significant.  Facets that are
         * glued to "old" partner facets (i.e., facets that represent internal
         * `(dim-1)`-faces of the triangulation and have already been seen from
         * the other side) are not represented at all in this bitmask.
         *
         * Each bit is set if and only if the corresponding facet is of type B
         * (i.e., either boundary, or glued to a "new" partner facet belonging
         * to a simplex that has already been seen before).
         *
         * The length of the bitmask will be the total number of `(dim-1)`-faces
         * in the triangulation component being described.
         *
         * \python This routine returns a deep copy (not a reference), for
         * consistency with the array-based query routines in this class.
         *
         * \return a reference to the bitmask of facet types.
         */
        const Bitmask& facetTypes() const {
            return facetType_;
        }
        /**
         * Returns the precise length of the bitmask of facet types, as
         * returned by facetTypes().  This will be the total number of
         * `(dim-1)`-faces in the underlying triangulation component, as
         * described in the class notes.
         *
         * This routine is provided because IsoSigData does not keep a
         * reference to the component itself, and because Bitmask does not
         * encode its precise length.
         *
         * \return the length of the bitmask of facet types.
         */
        size_t countFacetBits() const {
            return adjSimplex_.size() + size_ - 1;
        }
        /**
         * Gives read-only access to the array of gluing destinations, as
         * described in the class notes.
         *
         * The length of this array will be the number of facets of
         * top-dimensional simplices of type B (as described in the class
         * notes).
         *
         * Each element of this array corresponds to some facet \a f of type B,
         * and indicates the index of the top-dimensional simplex containing
         * the partner facet to which \a f is glued, or `size()` if \a f is a
         * boundary facet.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of gluing destinations.
         */
        const FixedArray<size_t>& adjacentSimplices() const {
            return adjSimplex_;
        }
        /**
         * Gives read-only access to the array of gluing permutations, as
         * described in the class notes.
         *
         * The length of this array will be the number of non-boundary facets
         * of top-dimensional simplices of type B (as described in the class
         * notes).
         *
         * Each element of this array holds the `S_n` index of the gluing
         * permutation for the relevant facet.  Note that, in contrast to
         * first-generation signatures, here we use the plain `S_n` index
         * and not the ordered `S_n` index.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of gluing permutations.
         */
        const FixedArray<typename Perm<dim + 1>::Index>& adjacentGluings()
                const {
            return adjGluing_;
        }
        /**
         * Gives read-only access to the array of lock masks, as described in
         * the class notes.
         *
         * If the triangulation component being encoded has no simplex or
         * facet locks, then this will be an empty array.  Otherwise it will
         * be an array whose length is the total number of top-dimensional
         * simplices in the component, and whose elements are the lock masks
         * for the individual simplices (as indicated by
         * `Simplex<dim>`::lockMask()`).
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the array of lock masks.
         */
        const FixedArray<typename Simplex<dim>::LockMask>& locks() const {
            return locks_;
        }
        /**
         * Indicates whether the triangulation component being encoded has any
         * simplex or facet locks.
         *
         * This will be `true` if and only if the array returned by locks() is
         * non-empty.
         *
         * \return `true` if and only if there are simplex and/or facet locks.
         */
        bool hasLocks() const {
            return ! locks_.empty();
        }
        /**
         * Determines whether the labelling that this data set encodes is
         * oriented.
         *
         * This will be `true` if and only if all permutations in
         * adjacentGluings() are odd.
         *
         * \return `true` if and only if this labelling is oriented.
         */
        bool isOriented() const {
            return oriented_;
        }

        /**
         * Moves the contents of the given data set into this data set.
         *
         * It does not matter whether this and the given data set refer to
         * triangulation components of the same size.
         *
         * The data set that was passed will no longer be usable.
         *
         * \return a reference to this data set.
         */
        IsoSigData& operator = (IsoSigData&&) noexcept = default;

        // Disable copy assignment, since we do not want to worry about
        // reallocation if the triangulations have different sizes.
        IsoSigData& operator = (const IsoSigData&) = delete;

        /**
         * Swaps the contents of this and the given triangulation component
         * data.
         *
         * This routine will behave correctly if \a other is in fact
         * this data set.
         *
         * \param other the component data whose contents should be swapped
         * with this.
         */
        void swap(IsoSigData& other) noexcept {
            std::swap(size_, other.size_);
            std::swap(oriented_, other.oriented_);
            facetType_.swap(other.facetType_);
            adjSimplex_.swap(other.adjSimplex_);
            adjGluing_.swap(other.adjGluing_);
            locks_.swap(other.locks_);
        }

        /**
         * Determines whether this and the given triangulation component data
         * hold identical information.
         *
         * \return \c true if and only if this and the given data are identical.
         */
        bool operator == (const IsoSigData&) const = default;

        /**
         * Compares two sets of triangulation component data.  Such comparisons
         * are used by the signature algorithm when choosing a minimal canonical
         * labelling, and also when choosing an ordering of components in
         * disconnected triangulations.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the component data to compare with this.
         * \return the result of the comparison between this and the given data.
         */
        std::strong_ordering operator <=> (const IsoSigData& rhs) const {
            // We try to loosely follow the ordering used for first-gen
            // signatures.  However, we do _not_ want to do this precisely,
            // for several reasons:
            //
            // - first-generation signatures minimise the encoding and not the
            //   component data, creating awkwardness around integer widths,
            //   endianness, and the non-monotonicity of base64 integer
            //   representations, all of which make it difficult to optimise
            //   signature algorithms;
            //
            // - the first-generation facet types 0 and 2 are combined into a
            //   single second-generation facet type B, thus unavoidably
            //   changing the sort order for the list of facet types when
            //   working with triangulations with boundary.
            //
            // This comparison does not look at oriented_ at all, since what
            // matters is the relabellings that it compares, not the original
            // labelling of the component(s).
            //
            if (auto c = size_ <=> rhs.size_; c != 0)
                return c;
            if (auto c = facetType_.numericalComp(rhs.facetType_); c != 0)
                return c;
            if (auto c = std::lexicographical_compare_three_way(
                    adjSimplex_.begin(), adjSimplex_.end(),
                    rhs.adjSimplex_.begin(), rhs.adjSimplex_.end()); c != 0)
                return c;
            if (auto c = std::lexicographical_compare_three_way(
                    adjGluing_.begin(), adjGluing_.end(),
                    rhs.adjGluing_.begin(), rhs.adjGluing_.end()); c != 0)
                return c;
            return std::lexicographical_compare_three_way(
                locks_.begin(), locks_.end(),
                rhs.locks_.begin(), rhs.locks_.end());
        }

    private:
        /**
         * Allocates space for a data set for the given triangulation component.
         *
         * The bitmask included in this data set will be initialised with all
         * bits off.  All other arrays will be initialised to the correct sizes;
         * however, the individual array elements, as well as the data member
         * oriented_, will all be left uninitialised.
         *
         * \param component the triangulation component that we intend to
         * encode.
         */
        IsoSigData(Component<dim>* component) :
                size_(component->size()),
                facetType_(component->countFacets()),
                adjSimplex_(component->countFacets() - size_ + 1),
                adjGluing_(adjSimplex_.size()
                    - component->countBoundaryFacets()),
                locks_(component->hasLocks() ? size_ : 0) {
        }
};

/**
 * Swaps the contents of the given triangulation component data sets.
 *
 * This global routine simply calls `IsoSigData<generation, dim>::swap()`;
 * it is provided so that IsoSigData meets the C++ Swappable requirements.
 *
 * \param a the first component data set whose contents should be swapped.
 * \param b the second component data set whose contents should be swapped.
 *
 * \ingroup triangulation
 */
template <int generation, int dim>
inline void swap(IsoSigData<generation, dim>& a, IsoSigData<generation, dim>& b)
        noexcept {
    a.swap(b);
};

/**
 * Encodes both first-generation and second-generation isomorphism signatures
 * as printable strings.  This is the default encoding used by both
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::neoSig()`.
 *
 * This printable encoding represents an isomorphism signature as a
 * `std::string`, using only printable characters from the 7-bit ASCII range.
 * For first-generation signatures of triangulations with no simplex or facet
 * locks, this is the same encoding that was used for the original isomorphism
 * signatures first introduced in Regina 4.90.
 *
 * If the triangulation does have one or more simplex and/or facet locks,
 * then these locks will be encoded as part of the isomorphism signature (they
 * will appear at the end, after the encoded gluings table).  This will break
 * compatibility with signatures from Regina ≤ 7.3.1 (before locks were
 * implemented).  If the triangulation does not have any locks then no lock
 * data will encoded, and so the encoding will be remain compatible with all
 * older versions of Regina.  If you need a signature where locks are not
 * encoded (e.g., for backward compatibility), then you can use the encoding
 * IsoSigPrintableLockFree.
 *
 * See the IsoSigEncoding concept documentation for general details on
 * encodings for isomorphism signatures.
 *
 * This class is designed to be used as a template parameter for signature
 * routines in the class `Triangulation<dim>`, including `isoSig()`, `neoSig()`,
 * `isoSigDetail()`, and `neoSigDetail()`.  Typical users would have no need
 * to call any functions from this encoding class directly.
 *
 * \python To use this encoding in Python, you can just call the relevant
 * `Triangulation<dim>` signature function (e.g., `isoSig()` or `neoSig()`)
 * with no extra encoding argument, since this encoding is the default.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
class IsoSigPrintable {
    public:
        /**
         * The number of characters that we use in our encoding to
         * represent a single gluing permutation.
         * This must be large enough to encode an index into Perm<dim+1>::Sn.
         *
         * \python Since Python does not support C++ templates, this constant
         * is accessible via a function `IsoSigPrintable.charsPerPerm(dim)`.
         */
        template <int dim> requires (supportedDim(dim))
        static constexpr int charsPerPerm =
            ((regina::bitsRequired(Perm<dim+1>::nPerms) + 5) / 6);

        /**
         * The data type that this encoding uses to hold the final signature.
         */
        using Signature = std::string;

        /**
         * Encodes the isomorphism signature of the empty
         * <i>dim</i>-dimensional triangulation.
         *
         * Note that IsoSigPrintable does _not_ return an empty signature for
         * this; instead it returns the non-empty string `a`.
         *
         * \return the isomorphism signature of the empty triangulation.
         */
        static Signature encodeEmpty() {
            Base64Encoder enc;
            enc.encodeSingle(0);
            return std::move(enc).str();
        }

        /**
         * Encodes a single connected component of a <i>dim</i>-dimensional
         * triangulation as a first-generation isomorphism signature.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the given gluings table encoded as a first-generation
         * signature.
         */
        template <int dim> requires (supportedDim(dim))
        static Signature encode(const IsoSigData<1, dim>& data);

        /**
         * Precomputes the length of the first-generation isomorphism signature
         * that encodes the given connected component.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the length of the first-generation signature that encodes
         * \a data.
         */
        template <int dim> requires (supportedDim(dim))
        static size_t length(const IsoSigData<1, dim>& data);

        /**
         * Encodes a single connected component of a <i>dim</i>-dimensional
         * triangulation as a second-generation isomorphism signature.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the given gluings table encoded as a second-generation
         * signature.
         */
        template <int dim> requires (supportedDim(dim))
        static Signature encode(const IsoSigData<2, dim>& data);

        /**
         * Precomputes the length of the second-generation isomorphism signature
         * that encodes the given connected component.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the length of the second-generation signature that encodes
         * \a data.
         */
        template <int dim> requires (supportedDim(dim))
        static size_t length(const IsoSigData<2, dim>& data);

        /**
         * Identifies whether the given signature is a first-generation or
         * second-generation signature, as encoded by IsoSigPrintable.
         *
         * This routine aims to be fast, and does not verify the entire
         * signature; instead it reads just enough of the initial characters
         * to make its decision.  What this means is:
         *
         * - If the given signature _is_ a first-generation or
         *   second-generation signature as encoded by IsoSigPrintable, this
         *   routine guarantees to return 1 or 2 respectively.
         *
         * - Otherwise, there are no guarantees: this output _could_ return 0
         *   (indicating that it identified \a sig as being neither of these),
         *   or it could still return 1 or 2 (indicating that, whilst invalid,
         *   \a sig nevertheless has a prefix that _looks_ like a
         *   first-generation or second-generation signature).
         *
         * As a special case, for the empty triangulation, the first-generation
         * and second-generation signatures are identical (both are the single
         * letter `a`).  In this case, generation() will return 2.
         *
         * If you need to verify the _validity_ of a signature, this is not
         * the correct routine to use - instead you should test whether
         * `Triangulation<dim>::fromSig(sig)` throws an exception.
         *
         * This routine will also recognise signatures encoded by
         * IsoSigPrintableLockFree (since those are identical to signatures
         * encoded by IsoSigPrintable for triangulations with no simplex/facet
         * locks).
         *
         * This routine does not require knowledge of the triangulation
         * dimension.
         *
         * \param sig a printable isomorphism signature of some generation.
         * \return 1 or 2 if \a sig is a first-generation or second-generation
         * signature respectively as encoded via IsoSigPrintable, or 0 if
         * \a sig was explicitly discovered to be neither of these.
         * As described above, if \s sig is _not_ a printable isomorphism
         * signature of any generation, this routine could return any of the
         * values 0, 1 or 2.
         */
        static int generation(const std::string& sig);

        // Make this class non-constructible.
        IsoSigPrintable() = delete;
};

/**
 * Encodes both first-generation and second-generation isomorphism signatures
 * as printable strings, ignoring any simplex and/or facet locks.
 *
 * Like IsoSigPrintable, this encoding represents an isomorphism signature as a
 * `std::string` using only printable characters from the 7-bit ASCII range.
 * For first-generation signatures, this is exactly the same as the printable
 * encoding that was used with Regina ≤ 7.3.x, before locks were implemented.
 *
 * See the IsoSigEncoding concept documentation for general details on
 * encodings for isomorphism signatures.
 *
 * This class is designed to be used as a template parameter for signature
 * routines in the class `Triangulation<dim>`, including `isoSig()`, `neoSig()`,
 * `isoSigDetail()`, and `neoSigDetail()`.  Typical users would have no need
 * to call any functions from this encoding class directly.
 *
 * \python To use this encoding in Python, pass IsoSigPrintableLockFree as a
 * runtime argument to the relevant `Triangulation<dim>` signature function
 * (e.g., `isoSig()` or `neoSig()`).
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
class IsoSigPrintableLockFree {
    public:
        /**
         * The number of characters that we use in our encoding to
         * represent a single gluing permutation.
         * This must be large enough to encode an index into Perm<dim+1>::Sn.
         *
         * \python Since Python does not support C++ templates, this constant
         * is accessible via a function
         * `IsoSigPrintableLockFree.charsPerPerm(dim)`.
         */
        template <int dim> requires (supportedDim(dim))
        static constexpr int charsPerPerm = IsoSigPrintable::charsPerPerm<dim>;

        /**
         * The data type that this encoding uses to hold the final signature.
         */
        using Signature = std::string;

        /**
         * Encodes the isomorphism signature of the empty
         * <i>dim</i>-dimensional triangulation.
         *
         * Like IsoSigPrintable, this does _not_ return an empty signature for
         * this; instead it returns the non-empty string `a`.
         *
         * \return the isomorphism signature of the empty triangulation.
         */
        static Signature encodeEmpty() {
            Base64Encoder enc;
            enc.encodeSingle(0);
            return std::move(enc).str();
        }

        /**
         * Encodes a single connected component of a <i>dim</i>-dimensional
         * triangulation as a first-generation isomorphism signature.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the given gluings table encoded as a first-generation
         * signature.
         */
        template <int dim> requires (supportedDim(dim))
        static Signature encode(const IsoSigData<1, dim>& data);

        /**
         * Precomputes the length of the first-generation isomorphism signature
         * that encodes the given connected component.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the length of the first-generation signature that encodes
         * \a data.
         */
        template <int dim> requires (supportedDim(dim))
        static size_t length(const IsoSigData<1, dim>& data);

        /**
         * Encodes a single connected component of a <i>dim</i>-dimensional
         * triangulation as a second-generation isomorphism signature.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the given gluings table encoded as a second-generation
         * signature.
         */
        template <int dim> requires (supportedDim(dim))
        static Signature encode(const IsoSigData<2, dim>& data);

        /**
         * Precomputes the length of the second-generation isomorphism signature
         * that encodes the given connected component.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the length of the second-generation signature that encodes
         * \a data.
         */
        template <int dim> requires (supportedDim(dim))
        static size_t length(const IsoSigData<2, dim>& data);

        // Make this class non-constructible.
        IsoSigPrintableLockFree() = delete;
};

/**
 * Encodes isomorphism signatures using small-memory byte-based encodings.
 * This encoding method is _only_ available for second-generation signatures,
 * not first-generation signatures.
 *
 * This encoding algorithm is intended for scenarios where memory needs to be
 * kept as small as possible.  It uses all eight bits per byte (as opposed to
 * IsoSigPrintable, which only encodes six bits per byte but creates a
 * printable string as a result).
 *
 * Any simplex and/or facet locks will be encoded as part of the signature.
 *
 * See the IsoSigEncoding concept documentation for general details on
 * encodings for isomorphism signatures.
 *
 * This class is designed to be used as a template parameter for
 * second-generation signature routines in the class `Triangulation<dim>`,
 * including `neoSig()` and `neoSigDetail()`.  Typical users would have no need
 * to call any functions from this encoding class directly.
 *
 * \python To use this encoding in Python, pass IsoSigBinary as a runtime
 * argument to the relevant `Triangulation<dim>` signature function
 * (e.g., `neoSig()` or `neoSigDetail()`).
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
class IsoSigBinary {
    public:
        /**
         * The data type that this encoding uses to hold the final signature.
         */
        using Signature = ByteSequence;

        /**
         * Encodes the isomorphism signature of the empty triangulation.
         *
         * For IsoSigBinary (unlike Regina's string-based encodings), this
         * will simply be an empty sequence.
         *
         * \return the isomorphism signature of the empty triangulation.
         */
        static Signature encodeEmpty() {
            return {};
        }

        /**
         * Encodes a single connected component of a <i>dim</i>-dimensional
         * triangulation as a second-generation isomorphism signature.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the given gluings table encoded as a second-generation
         * signature.
         */
        template <int dim> requires (supportedDim(dim))
        static Signature encode(const IsoSigData<2, dim>& data);

        /**
         * Precomputes the length of the second-generation isomorphism signature
         * that encodes the given connected component.
         *
         * \pre The given component is non-empty, and uses a canonical labelling
         * in the sense described in the IsoSigData class notes.
         *
         * \param data the compressed gluings table for the component to encode.
         * \return the length of the second-generation signature that encodes
         * \a data.
         */
        template <int dim> requires (supportedDim(dim))
        static size_t length(const IsoSigData<2, dim>& data);

        /**
         * Re-encodes the given binary signature as a string-based
         * second-generation signature (using the IsoSigPrintable encoding),
         * which uses only printable characters from the 7-bit ASCII range.
         *
         * Calling `printable(sig)` is significantly more efficient than calling
         * `Triangulation<dim>::fromSig(sig).neoSig()` (with an appropriate
         * orientation argument if necessary), and should give the same result.
         *
         * If \a sig is an oriented signature, then this re-encoding will
         * preserve the orientation.
         *
         * \pre The argument \a sig is indeed a second-generation isomorphism
         * signature of a <i>dim</i/>-dimensional triangulation, encoded via
         * IsoSigBinary.  This will _not_ be checked thoroughly (though some
         * minimal checks will be done).
         *
         * \python Python does not support C++ templates.  Instead, the
         * dimension \a dim should be passed as a second runtime argument;
         * for example, `IsoSigBinary.asString(sig, 3)`.
         *
         * \exception InvalidArgument It was detected that \a sig was not a
         * valid second-generation isomorphism signature of a
         * <i>dim</i/>-dimensional triangulation encoded via IsoSigBinary.
         * Again, this will not be checked thoroughly; this exception will
         * only be thrown if the violation is sufficiently obvious that it is
         * picked up during the re-encoding process.
         *
         * \param sig the second-generation signature of some
         * <i>dim</i/>-dimensional triangulation, encoded as a byte sequence
         * using the IsoSigBinary encoding.
         * \return the second-generation signature of the same triangulation,
         * encoded as a printable string using the IsoSigPrintable encoding.
         */
        template <int dim> requires (supportedDim(dim))
        static std::string asString(const ByteSequence& sig);

        // Make this class non-constructible.
        IsoSigBinary() = delete;
};

/**
 * A slow isomorphism signature type that is consistent with the original
 * isomorphism signatures that were first introduced in Regina 4.90.
 *
 * This signature type is slow to use and is not recommended for use in new
 * projects.  It is still offered because it reproduces the first-generation
 * signatures from Regina ≤ 7.x.  For backward compatibility, it remains the
 * default signature type for the first-generation signature routines
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::isoSigDetail()`.
 *
 * The IsoSigType concept documentation explains how an isomorphism signature
 * type acts as an "iterator" through candidate pairs `(s, p)`, where \a s is a
 * "starting simplex" within some triangulation component \a c, and \a p
 * describes a "starting labelling" of its vertices.  This signature type is
 * trivial: it considers _all_ possible pairs `(s, p)`, without any pruning.
 *
 * This class is designed to be used as a template parameter for signature
 * routines in the class `Triangulation<dim>`, including `isoSig()`, `neoSig()`,
 * `isoSigDetail()`, and `neoSigDetail()`.  Typical users would have no need
 * to create objects of this class or call any of its functions directly.
 *
 * \python Python does not support C++ templates.  To access this class,
 * append the dimension as a suffix (e.g., IsoSigClassic3 for dimension 3).
 * To _use_ this signature type in Python: for first-generation signatures this
 * type is the default, so just call the `Triangulation<dim>` signature function
 * with no extra arguments (e.g., `isoSig()` or `isoSigDetail()`); for
 * second-generation signatures, pass this class as a runtime argument (e.g.,
 * `neoSig(False, IsoSigPrintable, IsoSigClassic3)`.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigClassic {
    private:
        bool oriented_;
            /**< Indicates whether we are creating an oriented signature. */
        size_t size_;
            /**< The number of top-dimensional simplices in the
                 triangulation component that we are working with. */
        size_t simp_;
            /**< Identifies the current top-dimensional "starting simplex" \a s.
                 This index is relative to the component (not the overall
                 triangulation). */
        typename Perm<dim + 1>::Index perm_;
            /**< Identifies the current "starting labelling" \a p.
                 This is an index into `Perm<dim + 1>::Sn`.  Note that this
                 has changed as of Regina 8.0; in Regina ≤ 7.x it was an
                 index into `Perm<dim + 1>::orderedSn` instead. */

    public:
        /**
         * Initialises this "iterator" to the first candidate pair `(s, p)`
         * for the given triangulation component.
         *
         * \param component the triangulation component that we are examining.
         * \param oriented `true` if you are creating an oriented signature,
         * or `false` if you are creating an unoriented signature.
         */
        IsoSigClassic(const Component<dim>& component, bool oriented) :
                oriented_(oriented), size_(component.size()),
                simp_(0), perm_(0) {
        }

        /**
         * Returns the index of the current starting simplex \a s within the
         * relevant triangulation component.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return the index within the component of the starting simplex \a s.
         */
        size_t simplex() const {
            return simp_;
        }

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex \a s.  This maps the vertices
         * of \a s to the "canonical" labels `0,1,...,dim`.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return the starting labelling \a p.
         */
        Perm<dim + 1> perm() const {
            return Perm<dim + 1>::Sn[perm_];
        }

        /**
         * Advances this "iterator" to the next candidate pair `(s, p)`.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return \c true if this was successful, or \c false if there
         * are no more candidate pairs.
         */
        bool next() {
            if (oriented_)
                perm_ += 2;
            else
                ++perm_;

            if (perm_ == Perm<dim + 1>::nPerms) {
                perm_ = 0;
                if (++simp_ == size_)
                    return false;
            }
            return true;
        }

        // Make this class non-copyable and non-assignable, since users
        // should not be creating objects of this class on their own.
        IsoSigClassic(const IsoSigClassic&) = delete;
        IsoSigClassic& operator = (const IsoSigClassic&) = delete;
};

/**
 * A faster isomorphism signature type based on degree sequences of
 * <i>subdim</i>-faces.
 *
 * The signature type `IsoSigDegrees<dim, dim-2>` (also available under the
 * alias `IsoSigRidgeDegrees<dim>`) is recommended for new projects, and is
 * the default signature type for the second-generation signature routines
 * `Triangulation<dim>::neoSig()` and `Triangulation<dim>::neoSigDetail()`.
 *
 * The IsoSigType concept documentation explains how an isomorphism signature
 * type acts as an "iterator" through candidate pairs `(s, p)`, where \a s is a
 * "starting simplex" within some triangulation component \a c, and \a p
 * describes a "starting labelling" of its vertices.  This signature type only
 * considers starting simplices \a s with a lexicographically minimal
 * <i>subdim</i>-face degree sequence.  The hope is that this eliminates a
 * large number of potential starting simplices without adding an enormous
 * amount of computational overhead, thereby speeding up the overall
 * isomorphism signature computation.
 *
 * This class is designed to be used as a template parameter for signature
 * routines in the class `Triangulation<dim>`, including `isoSig()`, `neoSig()`,
 * `isoSigDetail()`, and `neoSigDetail()`.  Typical users would have no need
 * to create objects of this class or call any of its functions directly.
 *
 * \python Python does not support C++ templates, and there are far too many of
 * these classes to wrap.  Currently Python supports only the cases where
 * \a subdim is 1 or `dim-2`, using the type aliases IsoSigEdgeDegrees and
 * IsoSigRidgeDegrees respectively.  To access one of these classes, append the
 * dimension as a suffix to the relevant type alias (e.g., IsoSigEdgeDegrees3,
 * or IsoSigRidgeDegrees4).  To _use_ this signature type in Python, pass the
 * type alias as a runtime argument to the `Triangulation<dim>` signature
 * function (e.g., `neoSig(False, IsoSigPrintable, IsoSigEdgeDegrees4)`.
 * As an exception, if you are building a second-generation signature and you
 * wish to use IsoSigRidgeDegrees (or IsoSigEdgeDegrees in dimension three,
 * which is the same type), you do not need any extra arguments since this
 * signature type is the default.
 *
 * \ingroup triangulation
 */
template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
class IsoSigDegrees {
    private:
        static constexpr int nFaces = FaceNumbering<dim, subdim>::nFaces;
            /**< The number of subdim-faces in each top-dimensional simplex. */

        /**
         * Holds the subdim-face degree sequence for a single top-dimensional
         * simplex.  The degrees are sorted; that is, we forget all
         * information about which degree corresponds to which subdim-face.
         */
        struct SimplexMarking {
            std::array<size_t, nFaces> degree;
                /**< The sorted list of subdim-face degrees. */

            /**
             * Computes and stores the sorted subdim-face degree sequence
             * for the given top-dimensional simplex.
             */
            void init(const Simplex<dim>& simplex) {
                for (int i = 0; i < nFaces; ++i)
                    degree[i] = simplex.template face<subdim>(i)->degree();
                std::sort(degree.begin(), degree.end());
            }

            /**
             * Tests whether this and the given sorted degree sequence
             * are identical.
             */
            bool operator == (const SimplexMarking&) const = default;

            /**
             * Lexicographically compares this with the given sorted
             * degree sequence.
             */
            std::strong_ordering operator <=> (const SimplexMarking&) const =
                default;
        };

        FixedArray<SimplexMarking> marks_;
            /**< The sorted subdim-face degree sequence of every top-dimensional
                 simplex in the component we are working with. */
        size_t smallest_;
            /**< A top-dimensional simplex with the lexicographically smallest
                 degree sequence.  Like \a simp_, this index is relative
                 to the component (not the overall triangulation).  If there
                 are many simplices with the same smallest degree sequence
                 then this denotes the one with smallest index. */

        bool oriented_;
            /**< Indicates whether we are creating an oriented signature. */
        size_t simp_;
            /**< Identifies the current top-dimensional "starting simplex" \a s.
                 This index is relative to the component (not the overall
                 triangulation). */
        typename Perm<dim + 1>::Index perm_;
            /**< Identifies the current "starting labelling" \a p.
                 This is an index into `Perm<dim + 1>::Sn`.  Note that this
                 has changed as of Regina 8.0; in Regina ≤ 7.x it was an
                 index into `Perm<dim + 1>::orderedSn` instead. */

    public:
        /**
         * Initialises this "iterator" to the first candidate pair `(s, p)`
         * for the given triangulation component.
         *
         * \param component the triangulation component that we are examining.
         * \param oriented `true` if you are creating an oriented signature,
         * or `false` if you are creating an unoriented signature.
         */
        IsoSigDegrees(const Component<dim>& component, bool oriented) :
                marks_(component.size()), oriented_(oriented), perm_(0) {
            // We set smallest_ to the first simplex with minimal subdim-face
            // degrees (which will then be the initial choice of starting
            // simplex simp_).
            smallest_ = 0;
            for (size_t i = 0; i < marks_.size(); ++i) {
                marks_[i].init(*(component.simplex(i)));
                if (i > 0 && marks_[i] < marks_[smallest_])
                    smallest_ = i;
            }

            simp_ = smallest_;
        }

        /**
         * Returns the index of the current starting simplex \a s within the
         * relevant triangulation component.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return the index within the component of the starting simplex \a s.
         */
        size_t simplex() const {
            return simp_;
        }

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex \a s.  This maps the vertices
         * of \a s to the "canonical" labels `0,1,...,dim`.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return the starting labelling \a p.
         */
        Perm<dim + 1> perm() const {
            return Perm<dim + 1>::Sn[perm_];
        }

        /**
         * Advances this "iterator" to the next candidate pair `(s, p)`.
         *
         * \pre This "iterator" is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned `false`.
         *
         * \return \c true if this was successful, or \c false if there
         * are no more candidate pairs.
         */
        bool next() {
            if (oriented_)
                perm_ += 2;
            else
                ++perm_;

            if (perm_ == Perm<dim + 1>::nPerms) {
                perm_ = 0;
                // Advance to the next simplex with minimal degree sequence.
                for (++simp_; simp_ < marks_.size() &&
                        ! (marks_[simp_] == marks_[smallest_]); ++simp_)
                    ;
                return simp_ < marks_.size();
            }
            return true;
        }

        // Make this class non-copyable and non-assignable, since users
        // should not be creating objects of this class on their own.
        IsoSigDegrees(const IsoSigDegrees&) = delete;
        IsoSigDegrees& operator = (const IsoSigDegrees&) = delete;
};

/**
 * A faster isomorphism signature type based on edge degree sequences.
 * See IsoSigDegrees for further discussion.
 *
 * \python Python does not support C++ templates.  To access this type alias,
 * append the dimension as a suffix (e.g., IsoSigEdgeDegrees4 for dimension 4).
 * To _use_ this signature type in Python, pass this class as a runtime argument
 * to the relevant `Triangulation<dim>` signature function (e.g.,
 * `neoSig(False, IsoSigPrintable, IsoSigEdgeDegrees4)`.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using IsoSigEdgeDegrees = IsoSigDegrees<dim, 1>;

/**
 * A faster isomorphism signature type based on degree sequences of
 * (`dim-2`)-faces.  See IsoSigDegrees for further discussion.
 *
 * This signature type is recommended for new projects, and is the default for
 * the second-generation signature routines `Triangulation<dim>::neoSig()` and
 * `Triangulation<dim>::neoSigDetail()`.
 *
 * \python Python does not support C++ templates.  To access this type alias,
 * append the dimension as a suffix (e.g., IsoSigRidgeDegrees4 for dimension 4).
 * To _use_ this signature type in Python: for second-generation signatures this
 * type is the default, so just call the `Triangulation<dim>` signature function
 * with no extra arguments (e.g., `neoSig()` or `neoSigDetail()`); for
 * first-generation signatures, pass this class as a runtime argument (e.g.,
 * `isoSig(IsoSigPrintable, IsoSigRidgeDegrees4)`.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using IsoSigRidgeDegrees = IsoSigDegrees<dim, dim - 2>;

} // namespace regina

#endif

