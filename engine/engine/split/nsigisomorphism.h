
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nsigisomorphism.h
 *  \brief Deals with full and partial isomorphisms of splitting surface
 *  signatures.
 */

#ifndef __NSIGISOMORPHISM_H
#ifndef __DOXYGEN
#define __NSIGISOMORPHISM_H
#endif

#include "split/nsignature.h"

class NSigPartialIsomorphism {
    private:
        unsigned nLabels;
        unsigned nCycles;

        unsigned* labelImage;
        unsigned* cyclePreImage;
        unsigned* cycleStart; // Refers to the original cycle index.

        int dir;

    public:
        NSigPartialIsomorphism(int newDir);
        NSigPartialIsomorphism(const NSigPartialIsomorphism& iso);
        ~NSigPartialIsomorphism();

        /**
         * Find the canonical representation of the cycles under the
         * relabelling represented here.
         */
        void makeCanonical(const NSignature& sig, unsigned fromCycleGroup = 0);
        int compareWith(const NSignature& sig,
            const NSigPartialIsomorphism* other,
            unsigned fromCycleGroup = 0) const;

    private:
        NSigPartialIsomorphism(const NSigPartialIsomorphism& base,
            unsigned newLabels, unsigned newCycles);

        friend struct ShorterCycle {
            const NSignature& sig;
            const NSigPartialIsomorphism& iso;

            ShorterCycle(const NSignature& newSig,
                    const NSigPartialIsomorphism& newIso);
            bool operator () (unsigned cycle1, unsigned cycle2) const;
        };

    friend class NSigCensus;
};

// Inline functions for NSigPartialIsomorphism

inline NSigPartialIsomorphism::NSigPartialIsomorphism(int newDir) : nLabels(0),
        nCycles(0), labelImage(0), cyclePreImage(0), cycleStart(0),
        dir(newDir) {
}

inline NSigPartialIsomorphism::~NSigPartialIsomorphism() {
    if (labelImage) delete[] labelImage;
    if (cyclePreImage) delete[] cyclePreImage;
    if (cycleStart) delete[] cycleStart;
}

inline NSigPartialIsomorphism::ShorterCycle::ShorterCycle(
        const NSignature& newSig, const NSigPartialIsomorphism& newIso) :
        sig(newSig), iso(newIso) {
}

inline bool NSigPartialIsomorphism::ShorterCycle::operator ()
        (unsigned cycle1, unsigned cycle2) const {
    return (NSignature::cycleCmp(sig, cycle1, iso.cycleStart[cycle1],
        iso.dir, iso.labelImage, sig, cycle2, iso.cycleStart[cycle2],
        iso.dir, iso.labelImage) < 0);
}

#endif

