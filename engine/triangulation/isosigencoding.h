
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
 * This class is designed to be used as a template parameter for
 * Triangulation<dim>::isoSig() and Triangulation<dim>::isoSigDetail().
 * Typical users would have no need to create objects of this class or
 * call any of its functions directly.
 *
 * \python Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g.,
 * IsoSigPrintable2 and IsoSigPrintable3 for dimensions 2 and 3).
 *
 * \ingroup triangulation
 */
template <int dim>
class IsoSigPrintable : public Base64SigEncoding {
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
        static constexpr unsigned charsPerPerm =
            ((regina::bitsRequired(Perm<(dim)+1>::nPerms) + 5) / 6);

        /**
         * Returns the isomorphism signature of an empty
         * <i>dim</i>-dimensional triangulation.
         */
        static Signature emptySig() {
            char c[2] = { encodeSingle(0), 0 };
            return c;
        }

        /**
         * Encodes data for a single connected component of a
         * <i>dim</i>-dimensional triangulation.
         *
         * The description consists of several arrays, describing facets of
         * the top-dimensional simplices, as well as the ways that these
         * facets are glued together.  Which array elements represent
         * which facets/gluings is an implementation detail; the purpose
         * of this routine is simply to encode the given information.
         * See the isoSig() implementation for further details.
         *
         * \python The arrays \a facetAction, \a joinDest and
         * \a joinGluing should each be passed as Python lists of integers.
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
         * \return the encoding of the component being described.
         */
        static Signature encode(size_t size,
            size_t nFacetActions, const uint8_t* facetAction,
            size_t nJoins, const size_t* joinDest,
            const typename Perm<dim+1>::Index* joinGluing);

        // Make this class non-constructible.
        IsoSigPrintable() = delete;
};

} // namespace regina

#endif

