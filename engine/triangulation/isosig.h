
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
 *  \brief Defines different encodings and signature types for
 *  isomorphism signatures.
 */

#ifndef __REGINA_ISOSIG_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_H
#endif

#include <array>
#include "regina-core.h"
#include "maths/perm.h"
#include "triangulation/forward.h"
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim, int subdim>
requires (dim >= 1 && dim <= maxDim() && subdim >= 0 && subdim < dim)
class FaceNumbering;

/**
 * Holds all of the data required to reconstruct a single non-empty connected
 * component of a <i>dim</i>-dimensional triangulation, up to relabelling.
 * This is a halfway point between isomorphism signatures and triangulations:
 * the data is small and uses simple numerical/combinatorial types, and is easy
 * to encode or decode in an appropriate signature form (e.g., a base64 string).
 *
 * Specifically, this data encodes a "compressed" gluings table for a
 * "canonical" labelling of a triangulation component.  Both the notions of
 * "compressed" and "canonical" involve stepping through the facets of
 * top-dimensional simplices in the usual order (simplex 0, facets `0,...,dim`;
 * then simplex 1, facets `0,...,dim`; and so on).  Some of these facets will
 * be boundary (i.e., unglued), some will be glued to "new" facets that we
 * have not yet stepped through, and some will be glued to "old" facets (i.e.,
 * we have already seen the same `(dim-1)`-face from the other side).
 *
 * To be _canonical_:
 *
 * _ We require that each time we see a new facet whose gluing partner comes
 *   from a previously-unseen simplex, then that partner simplex uses the next
 *   available simplex label and the gluing uses the identity permutation.
 *   So, for example, the first facet of simplex 0 that is glued to some
 *   _other_ simplex must be glued to simplex 1 using the identity permutation.
 *
 * - The condition above ensures that, once we have chosen which top-dimensional
 *   simplex will be simplex 0 and how its vertices `0,...,dim` will be
 *   labelled, the labelling of the remaining simplices and their vertices
 *   will be uniquely determined (and can be computed in linear time).
 *
 * - Your chosen signature _type_ may impose further requirements on the
 *   choice of simplex 0 and its vertex labels; see IsoSigTypeAPI for details.
 *
 * Given a canonical labelling, a _compressed_ gluings table holds the
 * following information:
 *
 * - For each facet \a f that we step through in order, _ignoring_ those glued
 *   to "old" facets, we record whether \a f is: boundary (type 0); glued to a
 *   "new" facet of a previously-unseen simplex using the identity permutation
 *   (type 1); or glued to a "new" facet of a simplex that has been seen before
 *   (type 2).  This is stored in array whose length is the total number of
 *   `(dim-1)`-faces in the triangulation component.
 *
 * - For each facet of type 2, we record the top-dimensional simplex number
 *   that it is glued to.  This is stored in an array whose length is the
 *   total number of facets of type 2.
 *
 * - For each facet of type 2, we record the ordered `S_n` index of the
 *   specific gluing permutation, again using an array whose length is the
 *   total number of facets of type 2.
 *
 * - Finally, if (and only if) there are any simplex or facet locks in this
 *   triangulation component, we also record the lock masks of all
 *   top-dimensional simplices in order (simplex `0,1,2,...`).
 *
 * An isomorphism signature _encoding_ is responsible for encoding this data
 * in its final signature form (e.g., a base64 string), and the final
 * isomorphism signature will use the labelling that minimises this
 * encoding (under the inherent ordering of the resulting signature type).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigData {
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
         * \param comp the triangulation component that we intend to encode.
         */
        IsoSigData(Component<dim>* comp) :
                size_(comp->size()),
                facetType_(((dim+1) * size_ + comp->countBoundaryFacets()) / 2),
                adjSimplex_(facetType_.size() - comp->countBoundaryFacets()
                    - size_ + 1),
                adjGluing_(adjSimplex_.size()),
                locks_(comp->hasLocks() ? size_ : 0) {
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
         * This will be an array of integers, each taking the value 0, 1 or 2,
         * and each indicating whether a particular facet of a top-dimensional
         * simplex is boundary (0), glued to a "new" facet of a
         * previously-unseen simplex (1), or glued to a "new" facet of a
         * simplex that has been seen before (2).  Facets that are glued to
         * "old" facets (i.e., facets that represent internal `(dim-1)`-faces
         * of the triangulation and have already been seen from the other side)
         * are not represented in this array.
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
         * notes, and as indicated by the facetTypes() array).
         *
         * Each element of this array indicates the index of the top-dimensional
         * simplex to which the relevant facet is glued.
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
         * notes, and as indicated by the facetTypes() array).
         *
         * Each element of this array holds the ordered `S_n` index of the
         * gluing permutation for the relevant facet.
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
 * Swaps the contents of the given triangulation component data sets.
 *
 * This global routine simply calls `IsoSigData<dim>::swap()`; it is provided so
 * that IsoSigData meets the C++ Swappable requirements.
 *
 * \param a the first component data set whose contents should be swapped.
 * \param b the second component data set whose contents should be swapped.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
inline void swap(IsoSigData<dim>& a, IsoSigData<dim>& b) noexcept {
    a.swap(b);
};

#ifdef __APIDOCS
/**
 * A documentation-only class describing the expected behaviour of isomorphism
 * signature encodings.
 *
 * Regina supports different _encodings_ for isomorphism signatures of
 * triangulations.  Essentially, the job of an encoding algorithm is to pack
 * a "compressed" gluings table into a small piece of data (such as a string)
 * that can be easily saved and/or passed around.
 *
 * This IsoSigEncodingAPI class is a documentation-only class (it is not
 * actually built into Regina).  Its purpose is to describe in detail the
 * tasks that an isomorphism signature encoding is expected to perform, and the
 * interface that the corresponding C++ class should provide.
 *
 * All encoding classes provide their functionality through static members and
 * static routines: they do not contain any member data, and it is unnecessary
 * (but harmless) to construct them.  Instead encoding classes are typically
 * used as C++ template arguments for functions such as
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::isoSigDetail()`.
 *
 * \python Whilst Regina's encoding classes are available, it is rare that you
 * would need to access these directly through Python.  Instead, to use an
 * isomorphism signature encoding class, you would typically call a modified
 * form of `Triangulation<dim>::isoSig()` or
 * `Triangulation<dim>::isoSigDetail()`  See `Triangulation<dim>::isoSig()`
 * for further details.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigEncodingAPI {
    public:
        /**
         * The data type that this encoding uses to hold the final
         * isomorphism signature.
         *
         * This API documentation shows `std::string` as an example, but this
         * may be any type that adheres to the concept ConcatenableSequence.
         */
        using Signature = std::string;

        /**
         * Encodes the isomorphism signature of the empty
         * <i>dim</i>-dimensional triangulation.
         *
         * Note that this would typically _not_ be an empty signature.
         * For example, under Regina's default encoding, the signature for
         * the empty triangulation is the non-empty string `a`.
         *
         * \return the isomorphism signature of the empty triangulation.
         */
        static Signature encodeEmpty();

        /**
         * Encodes a "compressed" gluings table for a single non-empty
         * connected component of a <i>dim</i>-dimensional triangulation.
         *
         * The job of this routine is to pack the information stored in
         * `IsoSigData<dim>` into the final \a Signature format.  For example,
         * Regina's default encoding (IsoSigPrintable) uses a combination of
         * bit-packing and base64 encoding to convert the data into a string.
         *
         * The size `data.size()` will need to be encoded; however, after this
         * it is not necessary to encode the sizes of the various arrays,
         * since these are already implied by the array contents.  Specifically:
         *
         * - by using `data.size()` and sequentially reading the contents of
         *   `data.facetTypes()`, it is possible for a reader to deduce the
         *   point at which the array `data.facetTypes()` ends;
         *
         * - by using `data.size()` and the contents of `data.facetTypes()`,
         *   it is possible for a reader to precompute the length of the arrays
         *   `data.adjacentSimplices()` and `data.adjacentGluings()`;
         *
         * - if the array `data.locks()` is non-empty, then its length will be
         *   the already-encoded quantity `data.size()`.
         *
         * \param data the compressed gluings table to be encoded.
         * \return the given gluings table encoded in the form of an
         * isomorphism signature.
         */
        static Signature encode(const IsoSigData<dim>& data);
};
#endif // __APIDOCS

/**
 * Represents an encoding that can be used for isomorphism signatures
 * of triangulations.  Essentially, the job of an encoding algorithm is to
 * pack the gluings table into a small piece of data (such as a string)
 * that is easily transported.
 *
 * See IsoSigEncodingAPI for further information, including a thorough
 * description of how an encoding class is expected to behave.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <typename T, int dim>
concept IsoSigEncoding =
    requires(const IsoSigData<dim> data) {
        typename T::Signature;
        requires ConcatenableSequence<typename T::Signature>;
        { T::encodeEmpty() } -> std::convertible_to<typename T::Signature>;
        { T::encode(data) } -> std::convertible_to<typename T::Signature>;
    };

/**
 * The default encoding to use for isomorphism signatures.
 *
 * This printable encoding is consistent with the original isomorphism
 * signatures that were implemented in Regina 4.90.  It represents an
 * isomorphism signature as a `std::string`, using only printable characters
 * from the 7-bit ASCII range.
 *
 * As of Regina 7.4, if \a supportLocks is \c true (the default), then this
 * encoding will also encode any simplex and/or facet locks into the
 * isomorphism signature.  If \a supportLocks is \c false, and/or if the
 * triangulation in question does not actually have any simplex and/or facet
 * locks, then the resulting signature will be the same as produced by earlier
 * versions of Regina, before locks were implemented.
 *
 * See the IsoSigEncodingAPI documentation for details on all member functions.
 *
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates.  For encodings that do support
 * locks (the default), Python users can just append the dimension as a suffix
 * (e.g., IsoSigPrintable2 and IsoSigPrintable3 for dimensions 2 and 3).
 * For encodings that do not support locks, Python users should use the
 * type aliases IsoSigPrintableLockFree2, IsoSigPrintableLockFree3, and so on.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim, bool supportLocks = true>
requires (supportedDim(dim))
class IsoSigPrintable {
    public:
        /**
         * The number of characters that we use in our encoding to
         * represent a single gluing permutation.
         * This must be large enough to encode an index into Perm<dim+1>::Sn.
         */
        static constexpr int charsPerPerm =
            ((regina::bitsRequired(Perm<(dim)+1>::nPerms) + 5) / 6);

        using Signature = std::string;

        static Signature encodeEmpty() {
            Base64SigEncoder enc;
            enc.encodeSingle(0);
            return std::move(enc).str();
        }

        static Signature encode(const IsoSigData<dim>& data) {
            Base64SigEncoder enc;

            int nChars = enc.encodeSize(data.size());
            enc.encodeTrits(data.facetTypes());
            enc.encodeInts(data.adjacentSimplices(), nChars);
            enc.encodeInts(data.adjacentGluings(), charsPerPerm);
            if constexpr (supportLocks) {
                if (data.hasLocks()) {
                    // Each lock mask holds dim+2 bits.
                    enc.append(Base64SigEncoder::spare[1]);
                    for (auto mask : data.locks()) {
                        if constexpr (dim <= 4) {
                            // We can encode <= 6 bits with 1 character.
                            enc.encodeSingle(mask);
                        } else if constexpr (dim <= 10) {
                            // We can encode <= 12 bits with 2 characters.
                            enc.encodeInt(mask, 2);
                        } else {
                            static_assert(dim <= 16);
                            // We can encode <= 18 bits with 3 characters.
                            enc.encodeInt(mask, 3);
                        }
                    }
                }
            }

            return std::move(enc).str();
        }

        // Make this class non-constructible.
        IsoSigPrintable() = delete;
};

/**
 * An encoding for isomorphism signatures that ignores simplex and/or facet
 * locks.
 *
 * This is exactly the same as the printable encoding that was used with
 * Regina 7.3.x and earlier, before locks were implemented.  Like
 * IsoSigPrintable, this encoding represents an isomorphism signature as a
 * std::string using only printable characters from the 7-bit ASCII range.
 *
 * See the IsoSigEncodingAPI documentation for details on all member functions.
 *
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates.  Instead this class can be used
 * by appending the dimension as a suffix (e.g., IsoSigPrintableLockFree2 and
 * IsoSigPrintableLockFree3 for dimensions 2 and 3).
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using IsoSigPrintableLockFree = IsoSigPrintable<dim, false>;

#ifdef __APIDOCS
/**
 * A documentation-only class describing the expected behaviour of
 * types of isomorphism signatures.
 *
 * Regina supports different _types_ of isomomorphism signatures of
 * triangulations.  Essentially, the job of a signature type is to help Regina
 * determine which labelling of a triangulation is "canonical".  Different
 * signature types will make different trade-offs between factors such as
 * speed, accessibility, backward compatibility and so on, typically resulting
 * in different notions of "canonical" as a consequence.
 *
 * This IsoSigTypeAPI class is a documentation-only class (it is not
 * actually built into Regina).  Its purpose is to describe in detail the
 * tasks that an isomorphism signature type is expected to perform, and the
 * interface that the corresponding C++ class should provide.
 *
 * A signature type class such as this works with a single component \a c of
 * a <i>dim</i>-dimenensional triangulation.  The sole task of a type class
 * is to iterate through a selection of combinations `(s, p)`, each
 * of which identifies a "starting simplex" and a "starting labelling"
 * of its vertices.  Here \a s is a top-dimensional simplex in \a c that
 * will act as the "starting simplex", and \a p is a permutation that maps
 * the vertices of \a s to the "starting labelling" `0,1,...,dim`.
 *
 * The properties that any signature type must satisfy are:
 *
 * - The selection of combinations `(s, p)` is always non-empty.
 *
 * - If we reorder the top-dimensional simplices of \a c and/or relabel
 *   their individual vertices, then the combinations `(s, p)` that
 *   this type class produces will be the same set, but modified
 *   according to this reordering/relabelling.  In other words, the
 *   starting simplices and their starting labellings can in theory
 *   be completely deduced from an _unlabelled_ triangulation component.
 *
 * An instance of a signature type class is like an iterator: it holds a
 * single candidate combination `(s, p)`.  The constructor must
 * initialise the instance to store the first candidate combination; you
 * can then query the current combination by calling simplex() and perm(),
 * and you can advance to the next combination by calling next().
 *
 * End users should typically not need to create instances of isomorphism
 * signature type classes.  Instead you would typically use such classes as
 * C++ template arguments for functions such as
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::isoSigDetail()`.
 *
 * \python Whilst Regina's signature type classes are available, it is rare
 * that you would need to access these directly through Python.  Instead, to use
 * an isomorphism signature type, you would typically call a modified form of
 * `Triangulation<dim>::isoSig()` or `Triangulation<dim>::isoSigDetail()`.
 * See `Triangulation<dim>::isoSig()` for further details.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigTypeAPI {
    public:
        /**
         * Initialises this object to iterate through candidate
         * "starting simplices" \a s and "starting labellings" \a p for the
         * given triangulation component.  See the IsoSigTypeAPI class
         * documentation for details.
         *
         * This object will initially be set to hold the first candidate pair
         * `(s, p)`.
         *
         * \param comp the triangulation component that we are examining.
         */
        IsoSigTypeAPI(const Component<dim>& comp);

        /**
         * Returns the current starting simplex \a s.
         *
         * See the IsoSigTypeAPI class documentation for further details.
         *
         * \pre This object is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned \c false.
         *
         * \return the index of the current starting simplex with
         * respect to the triangulation component under consideration.
         * Note that, for a disconnected triangulation, this is _not_
         * necessarily the same as Simplex::index() (which gives the
         * index with respect to the overall triangulation).
         */
        size_t simplex() const;

        /**
         * Returns the current starting labelling \a p of the vertices
         * of the current starting simplex.
         *
         * See the IsoSigTypeAPI class documentation for further details.
         *
         * \pre This object is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned \c false.
         *
         * \return the starting labelling, given as a permutation that
         * maps the current vertex labels of the starting simplex \a s
         * to the "canonical" labels `0,1,...,dim`.
         */
        Perm<dim + 1> perm() const;

        /**
         * Advances this object to the next candidate pair `(s, p)`.
         *
         * See the IsoSigTypeAPI class documentation for further details.
         *
         * \pre This object is holding a valid candidate pair `(s, p)`;
         * that is, next() has not yet returned \c false.
         *
         * \return \c true if this was successful, or \c false if there
         * is no next candidate pair (i.e., the current candidate pair
         * is the last).
         */
        bool next();
};
#endif // __APIDOCS

/**
 * Represents a signature type that can be used for isomorphism signatures of
 * triangulations.  Essentially, the job of a signature type is to help Regina
 * determine which labelling of a triangulation is "canonical".
 *
 * See IsoSigTypeAPI for further information, including a thorough description
 * of how a signature type class is expected to behave.
 *
 * \apinotfinal
 *
 * \ingroup triangulation
 */
template <typename T, int dim>
concept IsoSigType =
    std::constructible_from<T, const Component<dim>&> &&
    requires(T t, const T c) {
        { c.simplex() } -> std::convertible_to<size_t>;
        { c.perm() } -> std::convertible_to<Perm<dim + 1>>;
        { t.next() } -> std::convertible_to<bool>;
    };

/**
 * The default signature type to use for isomorphism signatures of
 * triangulations.
 *
 * This signature type is slower to use than others; its main benefit is that
 * it is consistent with the original isomorphism signatures that were
 * implemented in Regina 4.90.
 *
 * See the IsoSigTypeAPI documentation for details on all member functions,
 * and on how isomorphism signature types work in general.
 *
 * This classic signature type is trivial: it considers _all_ possible
 * "starting simplices" \a s, and all `(dim+1)!` possible
 * "starting labellings" \a p.
 *
 * This class is designed to be used as a template parameter for
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::isoSigDetail()`.
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g.,
 * IsoSigClassic2 and IsoSigClassic3 for dimensions 2 and 3).
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class IsoSigClassic {
    private:
        size_t size_;
            /**< The number of top-dimensional simplices in the
                 triangulation component that we are working with. */
        size_t simp_;
            /**< Identifies the current top-dimensional "starting simplex" \a s.
                 This index is relative to the component (not the overall
                 triangulation). */
        typename Perm<dim + 1>::Index perm_;
            /**< Identifies the current "starting labelling" \a p.
                 This is an index into Perm<dim + 1>::orderedSn. */

    public:
        IsoSigClassic(const Component<dim>& comp) :
                size_(comp.size()), simp_(0), perm_(0) {
        }

        size_t simplex() const {
            return simp_;
        }

        Perm<dim + 1> perm() const {
            return Perm<dim + 1>::orderedSn[perm_];
        }

        bool next() {
            if (++perm_ == Perm<dim + 1>::nPerms) {
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
 * Defines an alternate type of isomorphism signature based on degree
 * sequences of <i>subdim</i>-faces.
 *
 * This is a "proof of concept" type that shows how you might speed up
 * isomorphism signature computations.  It requires that the signature
 * choose a starting simplex whose set of <i>subdim</i>-face degrees is
 * lexicographically minimal amongst all top-dimensional simplices.
 * The hope is that this eliminates a large number of potential starting
 * simplices without adding an enormous amount of computational overhead.
 *
 * See the IsoSigTypeAPI documentation for details on all member functions,
 * and on how isomorphism signature types work in general.
 *
 * This class is designed to be used as a template parameter for
 * `Triangulation<dim>::isoSig()` and `Triangulation<dim>::isoSigDetail()`.
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates, and there are far
 * too many of these classes to wrap.  Currently Python supports only
 * the cases where \a subdim is 1 or <i>dim</i>-2, using the type aliases
 * IsoSigEdgeDegrees and IsoSigRidgeDegrees respectively (these cover
 * all the variants that Regina currently uses internally).
 * You can access the corresponding classes by appending the appending the
 * dimension as a suffix to the type alias (e.g., you can use
 * IsoSigEdgeDegrees3 to work with edge degrees in 3-manifold triangulations).
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

        size_t simp_;
            /**< Identifies the current top-dimensional "starting simplex" \a s.
                 This index is relative to the component (not the overall
                 triangulation). */
        typename Perm<dim + 1>::Index perm_;
            /**< Identifies the current "starting labelling" \a p.
                 This is an index into Perm<dim + 1>::orderedSn. */

    public:
        IsoSigDegrees(const Component<dim>& comp) :
                marks_(comp.size()), perm_(0) {
            // We set smallest_ to the first simplex with minimal subdim-face
            // degrees (which will then be the initial choice of starting
            // simplex simp_).
            smallest_ = 0;
            for (size_t i = 0; i < marks_.size(); ++i) {
                marks_[i].init(*(comp.simplex(i)));
                if (i > 0 && marks_[i] < marks_[smallest_])
                    smallest_ = i;
            }

            simp_ = smallest_;
        }

        size_t simplex() const {
            return simp_;
        }

        Perm<dim + 1> perm() const {
            return Perm<dim + 1>::orderedSn[perm_];
        }

        bool next() {
            if (++perm_ == Perm<dim + 1>::nPerms) {
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
 * Defines an alternate type of isomorphism signature based on edge degree
 * sequences.  See IsoSigDegrees for further discussion.
 *
 * \python Python does not support templates.  You can access these
 * classes by appending the appending the dimension as a suffix (e.g., use
 * IsoSigEdgeDegrees3 to use edge degrees in 3-manifold triangulations).
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using IsoSigEdgeDegrees = IsoSigDegrees<dim, 1>;

/**
 * Defines an alternate type of isomorphism signature based on degree
 * sequences of (<i>dim</i>-2)-faces.  See IsoSigDegrees for further discussion.
 *
 * \python Python does not support templates.  You can access these
 * classes by appending the appending the dimension as a suffix (e.g., use
 * IsoSigRidgeDegrees4 to use triangle degrees in 4-manifold triangulations).
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using IsoSigRidgeDegrees = IsoSigDegrees<dim, dim - 2>;

} // namespace regina

#endif

