
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

/*! \file nsigcensus.h
 *  \brief Deals with forming a census of splitting surface signatures.
 */

#ifndef __NSIGCENSUS_H
#ifndef __DOXYGEN
#define __NSIGCENSUS_H
#endif

#include <list>
#include "split/nsignature.h"
#include "split/nsigisomorphism.h"

typedef std::list<NSigPartialIsomorphism*> NSigIsoList;
typedef void (*UseSignature)(const NSignature&, const NSigIsoList&, void *);

/**
 * Constraints:
 *
 * - Signatures are considered equivalent if they are related by:
 *   \- Relabelling;
 *   \- Rotating individual cycles;
 *   \- Inverting individual cycles;
 *   \- Reversing *all* cycles.
 *
 * - Cycles ordered by decreasing length.
 * - Cycles must have >= 50% lower case.
 * - Within this, sigs ordered lexicographically: a < A < ... < z < Z.
 *   \- Thus first occurrences of letters must be in ascending order.
 */

unsigned long formSigCensus(unsigned order, UseSignature use,
    void* useArgs = 0);

class NSigCensus {
    private:
        NSignature sig;
        unsigned nextLabel;
        unsigned* used;
        NSigIsoList* automorph;

        UseSignature use;
        void* useArgs;

        unsigned long totalFound;

    public:
        /**
         * Deallocates any memory used specifically by this structure.
         */
        ~NSigCensus();

        /**
         * Runs a complete signature census generation.  At most one
         * copy of this routine should be running at any given time for
         * a particular NSigCensus.
         *
         * @param param this parameter is ignored.
         * @return 0.
         */
        void* run(void* param);

    private:
        NSigCensus(unsigned order, UseSignature newUse, void* newUseArgs);
            // Order must be > 0.

        void clearTopAutomorphisms();
        bool extendAutomorphisms();
        void tryCycle(unsigned cycleLen, bool newCycleGroup, unsigned startPos);

    friend unsigned long formSigCensus(unsigned, UseSignature, void*);
};

// Inline functions for NSigCensus

inline NSigCensus::NSigCensus(unsigned order, UseSignature newUse,
        void* newUseArgs) : sig(order), used(new unsigned[order]),
        automorph(new NSigIsoList[order + 2]), use(newUse),
        useArgs(newUseArgs) {
}

inline NSigCensus::~NSigCensus() {
    delete[] used;
    delete[] automorph;
}

#endif

