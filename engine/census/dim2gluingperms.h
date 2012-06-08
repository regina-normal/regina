
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file census/dim2gluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of triangle edges.
 */

#ifndef __DIM2GLUINGPERMS_H
#ifndef __DOXYGEN
#define __DIM2GLUINGPERMS_H
#endif

#include "regina-core.h"
#include "census/dim2edgepairing.h"
#include "census/ngenericgluingperms.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of triangle edges.  Given a
 * pairwise matching of edges (as described by class Dim2EdgePairing), each
 * edge that is matched with some other edge will have an associated
 * permutation of three elements (as described by class NPerm3).
 *
 * If an edge is matched with some other edge, the two associated
 * permutations in this set will be inverses.  If an edge is left
 * deliberately unmatched, it will have no associated permutation in this set.
 *
 * Such a set of permutations models part of the structure of a 2-manifold
 * triangulation, in which each triangle edge that is glued to another
 * edge has a corresponding gluing permutation (and the matched edge has
 * the inverse gluing permutation).
 *
 * \ifacespython Not present.
 */
class REGINA_API Dim2GluingPerms : public NGenericGluingPerms<2> {
    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        Dim2GluingPerms(const Dim2GluingPerms& cloneMe);

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
        Dim2GluingPerms(std::istream& in);

    protected:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by Dim2EdgePairing::isCanonical().  Note that all edge pairings
         * constructed by Dim2EdgePairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of triangle edges
         * that this permutation set will complement.
         */
        Dim2GluingPerms(const Dim2EdgePairing* pairing);
};

/*@}*/

// Inline functions for Dim2GluingPerms

inline Dim2GluingPerms::Dim2GluingPerms(const Dim2GluingPerms& cloneMe) :
        NGenericGluingPerms<2>(cloneMe) {
}

inline Dim2GluingPerms::Dim2GluingPerms(std::istream& in) :
        NGenericGluingPerms<2>(in) {
}

inline Dim2GluingPerms::Dim2GluingPerms(const Dim2EdgePairing* pairing) :
        NGenericGluingPerms<2>(pairing) {
}

} // namespace regina

#endif

