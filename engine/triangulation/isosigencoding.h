
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
#include "utilities/sigutils.h"

namespace regina {

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
 * \ingroup triangulation
 */
template <int dim, bool supportLocks = true>
class IsoSigPrintable {
    public:
        /**
         * The data type used to store an isomorphism signature.
         */
        using Signature = std::string;

        /**
         * The number of characters that we use in our encoding to
         * represent a single gluing permutation.
         * This must be large enough to encode an index into Perm<dim+1>::Sn.
         */
        static constexpr int charsPerPerm =
            ((regina::bitsRequired(Perm<(dim)+1>::nPerms) + 5) / 6);

        /**
         * Returns the isomorphism signature of an empty
         * <i>dim</i>-dimensional triangulation.
         */
        static Signature emptySig() {
            Base64SigEncoder enc;
            enc.encodeSingle(0);
            return std::move(enc).str();
        }

        /**
         * Encodes data for a single connected component of a
         * <i>dim</i>-dimensional triangulation.
         *
         * The description consists of several arrays, describing facets of
         * the top-dimensional simplices, as well as the ways that these
         * facets are glued together and any simplex and/or facet locks.
         * Which array elements represent which facets/gluings is an
         * implementation detail; the purpose of this routine is simply to
         * encode the given information.  See the isoSig() implementation for
         * further details.
         *
         * \python The arrays \a facetAction, \a joinDest, \a joinGluing and
         * \a lockMasks should each be passed as Python lists of integers;
         * the argument \a lockMasks may be `None`.
         * The arguments \a nFacetActions and \a nJoins are not present,
         * since Python lists already know their own sizes.
         *
         * \param size the number of top-dimensional simplices in the component.
         * \param nFacetActions the size of the array \a facetAction.
         * \param facetAction an array of size \a nFacetActions, where
         * each element is either 0, 1 or 2, respectively representing
         * a boundary facet, a facet joined to a new simplex, or a facet
         * joined to a simplex that has already been seen.
         * \param nJoins the size of the arrays \a joinDest and \a joinGluing.
         * \param joinDest an array whose elements are indices of
         * top-dimensional simplices to which gluings are being made.
         * \param joinGluing an array of gluing permutations.
         * \param lockMasks an array that holds the lock masks for all
         * top-dimensional simplices in the component, as returned by
         * `Simplex<dim>::lockMask()`.  This array should have length \a size.
         * If no simplices have any locks at all, then this argument must
         * be \c null.
         * \return the encoding of the component being described.
         */
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
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates.  Instead this class can be used
 * by appending the dimension as a suffix (e.g., IsoSigPrintableLockFree2 and
 * IsoSigPrintableLockFree3 for dimensions 2 and 3).
 *
 * \ingroup triangulation
 */
template <int dim>
using IsoSigPrintableLockFree = IsoSigPrintable<dim, false>;

} // namespace regina

#endif

