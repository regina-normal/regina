
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

/*! \file nsignature.h
 *  \brief Deals with signatures of splitting surfaces.
 */

#ifndef __NSIGNATURE_H
#ifndef __DOXYGEN
#define __NSIGNATURE_H
#endif

#include "shareableobject.h"

class NTriangulation;

class NSignature {
    private:
        unsigned order;
        unsigned* label;
        bool* labelInv;
        unsigned nCycles;
        unsigned* cycleStart; // Insist on sentinels.
        unsigned* cycleLen;
        unsigned nCycleGroups;
        unsigned* cycleGroupStart; // Insist on sentinels.

    public:
        NSignature(const NSignature& sig);
        virtual ~NSignature();

        static NSignature* parse(const std::string& sig); // Cannot be empty.
        NTriangulation* triangulate() const;

        static int cycleCmp(const NSignature& sig1, unsigned cycle1,
            unsigned start1, int dir1, unsigned* relabel1,
            const NSignature& sig2, unsigned cycle2, unsigned start2,
            int dir2, unsigned* relabel2);

        void writeCycles(std::ostream& out, const std::string& cycleOpen,
            const std::string& cycleClose, const std::string& cycleJoin) const;
        virtual void writeTextShort(std::ostream& out) const;

    private:
        NSignature(unsigned newOrder); // Requires newOrder > 0.

    friend class NSigPartialIsomorphism;
    friend class NSigCensus;
};

// Inline functions for NSignature

inline NSignature::NSignature(unsigned newOrder) : order(newOrder),
        label(new unsigned[2 * newOrder]), labelInv(new bool[2 * newOrder]),
        nCycles(0), cycleStart(new unsigned[2 * newOrder + 1]),
        cycleLen(new unsigned[2 * newOrder]), nCycleGroups(0),
        cycleGroupStart(new unsigned[2 * newOrder + 1]) {
    // Insert sentinels.
    cycleStart[0] = cycleGroupStart[0] = 0;
}

inline NSignature::~NSignature() {
    delete[] label;
    delete[] labelInv;
    delete[] cycleStart;
    delete[] cycleLen;
    delete[] cycleGroupStart;
}

inline void NSignature::writeTextShort(std::ostream& out) const {
    writeCycles(out, "(", ")", "");
}

#endif

