
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

#include "split/nsignature.h"

NSignature::NSignature(const NSignature& sig) : order(sig.order),
        label(new unsigned[2 * sig.order]), labelInv(new bool[2 * sig.order]),
        nCycles(sig.nCycles), cycleStart(new unsigned[sig.nCycles + 1]),
        cycleLen(new unsigned[sig.nCycles]), nCycleGroups(sig.nCycleGroups),
        cycleGroupStart(new unsigned[sig.nCycleGroups + 1]) {
    copy(sig.label, sig.label + 2 * sig.order, label);
    copy(sig.labelInv, sig.labelInv + 2 * sig.order, labelInv);
    copy(sig.cycleStart, sig.cycleStart + sig.nCycles + 1, cycleStart);
    copy(sig.cycleLen, sig.cycleLen + sig.nCycles, cycleLen);
    copy(sig.cycleGroupStart, sig.cycleGroupStart + sig.nCycleGroups + 1,
        cycleGroupStart);
}

NSignature* NSignature::parse(const NString& sig) {
    // TODO
    return 0;
}

NTriangulation* NSignature::triangulate() const {
    // TODO
    return 0;
}

int NSignature::cycleCmp(const NSignature& sig1, unsigned cycle1,
        unsigned start1, int dir1, unsigned* relabel1, const NSignature& sig2,
        unsigned cycle2, unsigned start2, int dir2, unsigned* relabel2) {
    unsigned len = sig1.cycleLen[cycle1];
    unsigned* arr1 = sig1.label + sig1.cycleStart[cycle1];
    unsigned* arr2 = sig2.label + sig2.cycleStart[cycle2];
    unsigned pos1 = start1;
    unsigned pos2 = start2;
    for (unsigned i = 0; i < len; i++) {
        if ((relabel1 ? relabel1[arr1[pos1]] : arr1[pos1]) <
                (relabel2 ? relabel2[arr2[pos2]] : arr2[pos2]))
            return -1;
        if ((relabel1 ? relabel1[arr1[pos1]] : arr1[pos1]) >
                (relabel2 ? relabel2[arr2[pos2]] : arr2[pos2]))
            return 1;

        if (dir1 > 0) {
            pos1++;
            if (pos1 == len)
                pos1 = 0;
        } else {
            if (pos1 == 0)
                pos1 = len - 1;
            else
                pos1--;
        }
        if (dir2 > 0) {
            pos2++;
            if (pos2 == len)
                pos2 = 0;
        } else {
            if (pos2 == 0)
                pos2 = len - 1;
            else
                pos2--;
        }
    }
    return 0;
}

void NSignature::writeCycles(std::ostream& out, const NString& cycleOpen,
        const NString& cycleClose, const NString& cycleJoin) const {
    out << cycleOpen;

    unsigned cycle = 0;
    for (unsigned pos = 0; pos < 2 * order; pos++) {
        if (cycleStart[cycle] == pos) {
            if (cycle > 0)
                out << cycleClose << cycleJoin << cycleOpen;
            cycle++;
        }
        out << char((labelInv[pos] ? 'A' : 'a') + label[pos]);
    }

    out << cycleClose;
}

