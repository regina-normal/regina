
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

/*! \file census/ngluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of tetrahedron faces.
 */

#ifndef __NGLUINGPERMS_H
#ifndef __DOXYGEN
#define __NGLUINGPERMS_H
#endif

#include "regina-core.h"
#include "census/ngenericgluingperms.h"
#include "triangulation/facetpairing3.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of tetrahedron faces.  Given a
 * pairwise matching of faces (as described by class FacetPairing<3>), each
 * face that is matched with some other face will have an associated
 * permutation of four elements (as described by class Perm<4>).
 *
 * If a face is matched with some other face, the two associated
 * permutations in this set will be inverses.  If a face is left
 * deliberately unmatched, it will have no associated permutation in this set.
 *
 * Such a set of permutations models part of the structure of a
 * triangulation, in which each tetrahedron face that is glued to another
 * face has a corresponding gluing permutation (and the matched face has
 * the inverse gluing permutation).
 *
 * \ifacespython Not present.
 */
class REGINA_API NGluingPerms : public NGenericGluingPerms<3> {
    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        NGluingPerms(const NGluingPerms& cloneMe);

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
        NGluingPerms(std::istream& in);

    protected:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of tetrahedron faces
         * that this permutation set will complement.
         */
        NGluingPerms(const FacetPairing<3>* pairing);
};

/*@}*/

// Inline functions for NGluingPerms

inline NGluingPerms::NGluingPerms(const NGluingPerms& cloneMe) :
        NGenericGluingPerms<3>(cloneMe) {
}

inline NGluingPerms::NGluingPerms(std::istream& in) :
        NGenericGluingPerms<3>(in) {
}

inline NGluingPerms::NGluingPerms(const FacetPairing<3>* pairing) :
        NGenericGluingPerms<3>(pairing) {
}

} // namespace regina

#endif

