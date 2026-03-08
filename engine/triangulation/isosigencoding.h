
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

/*! \file triangulation/isosigencoding.h
 *  \brief Defines different encodings for isomorphism signatures.
 */

#ifndef __REGINA_ISOSIGENCODING_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIGENCODING_H
#endif

#include "regina-core.h"
#include "maths/perm.h"
#include "triangulation/forward.h"
#include "utilities/sigutils.h"

namespace regina {

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
         * \return The isomorphism signature of the empty triangulation.
         */
        static Signature emptySig();

        /**
         * Encodes a "compressed" gluings table for a single non-empty
         * connected component of a <i>dim</i>-dimensional triangulation.
         *
         * The compressed gluings table is passed into this routine via a
         * series of integers and arrays.  The encoding does not need to know
         * what these mean or where they came from; its only job is to pack
         * them into the final \a Signature format.  For example, Regina's
         * default encoding (IsoSigPrintable) uses a combination of bit-packing
         * and base64 encoding to convert the given data into a string.
         *
         * The initial \a size argument will need to be encoded; however, after
         * this it is not necessary to encode the sizes of the various arrays,
         * since these are already implicitly encoded by the array contents.
         * Specifically:
         *
         * - by using \a size and sequentially reading the contents of the
         *   \a facetAction array, it is possible for a reader to deduce the
         *   point at which the \a facetAction array ends;
         *
         * - by using \a size and the contents of the \a facetAction array,
         *   it is possible for a reader to precompute the length of the
         *   \a joinDest and \a joinGluing arrays;
         *
         * - if the \a lockMasks arrays is non-null, then its length will be
         *   the already-encoded quantity \a size.
         *
         * \python The arrays \a facetAction, \a joinDest, \a joinGluing and
         * \a lockMasks should each be passed as Python lists of integers;
         * the argument \a lockMasks may be `None`.
         * The arguments \a nFacetActions and \a nJoins are not present,
         * since Python lists already know their own sizes.
         *
         * \param size a strictly positive integer.  (This represents the
         * number of top-dimensional simplices in the component.)
         * \param nFacetActions the strictly positive size of the array
         * \a facetAction.
         * \param facetAction a non-empty array of size \a nFacetActions,
         * where each element is either 0, 1 or 2.  (This encodes which facets
         * of top-dimensional simplices are boundary, joined to a new simplex,
         * or joined to an earlier simplex.)
         * \param nJoins the non-negative size of the arrays \a joinDest and
         * \a joinGluing.
         * \param joinDest a possibly empty array of size \a nJoins, each of
         * whose elements are integers in the range `0,...,size-1` inclusive.
         * (This represents the indices of top-dimensional simplices to which
         * various gluings are being made.)
         * \param joinGluing a possibly empty array of size \a nJoins, each of
         * which is an arbitrary permutation of `dim+1` elements.  (This
         * represents various gluing permutations.)
         * \param lockMasks either a non-empty array of size \a size, each
         * of whose elements is a lock mask (representing all of the
         * simplex/facet locks in the triangulation), or else \c null if the
         * triangulation component being encoded has no locks at all (a common
         * scenario that is worth optimising for).  If this argument is
         * non-null, then at least one of the lock masks in the array will be
         * non-zero.
         * \return the given data encoded in the form of an
         * isomorphism signature.
         */
        static Signature encode(size_t size,
                size_t nFacetActions, const uint8_t* facetAction,
                size_t nJoins, const size_t* joinDest,
                const typename Perm<dim+1>::Index* joinGluing,
                const typename Simplex<dim>::LockMask* lockMasks);
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
    requires {
        typename T::Signature;
        requires ConcatenableSequence<typename T::Signature>;
        { T::emptySig() } -> std::convertible_to<typename T::Signature>;
        { T::encode((size_t)(0), (size_t)(0), (const uint8_t*)(nullptr),
            (size_t)(0), (const size_t*)(nullptr),
            (const typename Perm<dim+1>::Index*)(nullptr),
            (const typename Simplex<dim>::LockMask*)(nullptr)) } ->
            std::convertible_to<typename T::Signature>;
    };

/**
 * The default encoding to use for isomorphism signatures.
 *
 * This printable encoding is consistent with the original isomorphism
 * signatures that were implemented in Regina 4.90.  It represents an
 * isomorphism signature as a std::string, using only printable characters
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

        static Signature emptySig() {
            Base64SigEncoder enc;
            enc.encodeSingle(0);
            return std::move(enc).str();
        }

        static Signature encode(size_t size,
                size_t nFacetActions, const uint8_t* facetAction,
                size_t nJoins, const size_t* joinDest,
                const typename Perm<dim+1>::Index* joinGluing,
                const typename Simplex<dim>::LockMask* lockMasks) {
            Base64SigEncoder enc;

            int nChars = enc.encodeSize(size);
            enc.encodeTrits(facetAction, facetAction + nFacetActions);
            enc.encodeInts(joinDest, joinDest + nJoins, nChars);
            enc.encodeInts(joinGluing, joinGluing + nJoins, charsPerPerm);
            if constexpr (supportLocks) {
                if (lockMasks) {
                    // Each lock mask holds dim+2 bits.
                    enc.append(Base64SigEncoder::spare[1]);
                    for (size_t i = 0; i < size; ++i) {
                        if constexpr (dim <= 4) {
                            // We can encode <= 6 bits with 1 character.
                            enc.encodeSingle(lockMasks[i]);
                        } else if constexpr (dim <= 10) {
                            // We can encode <= 12 bits with 2 characters.
                            enc.encodeInt(lockMasks[i], 2);
                        } else {
                            static_assert(dim <= 16);
                            // We can encode <= 18 bits with 3 characters.
                            enc.encodeInt(lockMasks[i], 3);
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

} // namespace regina

#endif

