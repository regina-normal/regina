
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

/*! \file census/dim4gluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of pentachoron facets.
 */

#ifndef __DIM4GLUINGPERMS_H
#ifndef __DOXYGEN
#define __DIM4GLUINGPERMS_H
#endif

#include "regina-core.h"
#include "census/ngenericgluingperms.h"
#include "dim4/dim4facetpairing.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of pentachoron facets.  Given a
 * pairwise matching of facets (as described by class Dim4FacetPairing), each
 * facet that is matched with some other facet will have an associated
 * permutation of five elements (as described by class NPerm<5>).
 *
 * If a facet is matched with some other facet, the two associated
 * permutations in this set will be inverses.  If a facet is left
 * deliberately unmatched, it will have no associated permutation in this set.
 *
 * Such a set of permutations models part of the structure of a 4-manifold
 * triangulation, in which each pentachoron facet that is glued to another
 * facet has a corresponding gluing permutation (and the matched facet has
 * the inverse gluing permutation).
 *
 * \ifacespython Not present.
 */
class REGINA_API Dim4GluingPerms : public NGenericGluingPerms<4> {
    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        Dim4GluingPerms(const Dim4GluingPerms& cloneMe);

        /**
         * Reads a new set of gluing permutations from the given input
         * stream.  This routine reads data in the format written by
         * dumpData().
         *
         * If the data found in the input stream is invalid or
         * incorrectly formatted, the routine inputError() will return
         * \c true but the contents of this object will be otherwise
         * undefined.
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        Dim4GluingPerms(std::istream& in);

    protected:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of pentachoron facets
         * that this permutation set will complement.
         */
        Dim4GluingPerms(const Dim4FacetPairing* pairing);
};

/*@}*/

// Inline functions for Dim4GluingPerms

inline Dim4GluingPerms::Dim4GluingPerms(const Dim4GluingPerms& cloneMe) :
        NGenericGluingPerms<4>(cloneMe) {
}

inline Dim4GluingPerms::Dim4GluingPerms(std::istream& in) :
        NGenericGluingPerms<4>(in) {
}

inline Dim4GluingPerms::Dim4GluingPerms(const Dim4FacetPairing* pairing) :
        NGenericGluingPerms<4>(pairing) {
}

} // namespace regina

#endif

