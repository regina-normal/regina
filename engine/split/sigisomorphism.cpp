
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

#include <algorithm>
#include "split/sigisomorphism.h"

namespace regina {

SigPartialIsomorphism::SigPartialIsomorphism(
        const SigPartialIsomorphism& iso) : nLabels(iso.nLabels),
        nCycles(iso.nCycles),
        labelImage(iso.nLabels ? new unsigned[iso.nLabels] : 0),
        cyclePreImage(iso.nCycles ? new unsigned[iso.nCycles] : 0),
        cycleStart(iso.nCycles ? new unsigned[iso.nCycles] : 0), dir(iso.dir) {
    if (iso.nLabels)
        std::copy(iso.labelImage, iso.labelImage + iso.nLabels, labelImage);
    if (iso.nCycles) {
        std::copy(iso.cyclePreImage, iso.cyclePreImage + iso.nCycles,
            cyclePreImage);
        std::copy(iso.cycleStart, iso.cycleStart + iso.nCycles, cycleStart);
    }
}

SigPartialIsomorphism::SigPartialIsomorphism(
        const SigPartialIsomorphism& base,
        unsigned newLabels, unsigned newCycles) : nLabels(newLabels),
        nCycles(newCycles),
        labelImage(newLabels ? new unsigned[newLabels] : 0),
        cyclePreImage(newCycles ? new unsigned[newCycles] : 0),
        cycleStart(newCycles ? new unsigned[newCycles] : 0), dir(base.dir) {
    if (base.nLabels)
        std::copy(base.labelImage, base.labelImage + base.nLabels, labelImage);
    if (base.nCycles) {
        std::copy(base.cyclePreImage, base.cyclePreImage + base.nCycles,
            cyclePreImage);
        std::copy(base.cycleStart, base.cycleStart + base.nCycles, cycleStart);
    }
}

void SigPartialIsomorphism::makeCanonical(const Signature& sig,
        unsigned fromCycleGroup) {
    unsigned fromCycle, toCycle;
    unsigned c, i;
    unsigned cycleLen;
    unsigned start1, start2;

    // Deal with each cycle group separately.
    for ( ; sig.cycleGroupStart[fromCycleGroup] < nCycles; fromCycleGroup++) {
        fromCycle = sig.cycleGroupStart[fromCycleGroup];
        toCycle = sig.cycleGroupStart[fromCycleGroup + 1];
        if (toCycle > nCycles)
            toCycle = nCycles;
        if (fromCycle >= toCycle)
            continue;

        // Determine where each cycle should start.
        cycleLen = sig.cycleStart[fromCycle + 1] - sig.cycleStart[fromCycle];
        for (c = fromCycle; c < toCycle; c++) {
            start1 = start2 = cycleLen;
            for (i = 0; i < cycleLen; i++)
                if (start1 == cycleLen ||
                        labelImage[sig.label[sig.cycleStart[c] + i]] <
                        labelImage[sig.label[sig.cycleStart[c] + start1]]) {
                    start1 = i;
                    start2 = cycleLen;
                } else if (labelImage[sig.label[sig.cycleStart[c] + i]] ==
                        labelImage[sig.label[sig.cycleStart[c] + start1]])
                    start2 = i;
            if (start2 == cycleLen)
                cycleStart[c] = start1;
            else {
                // Two possible starting points; we must choose between them.
                if (Signature::cycleCmp(sig, c, start1, dir, labelImage,
                        sig, c, start2, dir, labelImage) <= 0)
                    cycleStart[c] = start1;
                else
                    cycleStart[c] = start2;
            }
        }

        // At this point we now know where each cycle starts under the new
        // labelling.  It's now time to determine in which order the cycles
        // should be presented.
        for (c = fromCycle; c < toCycle; c++)
            cyclePreImage[c] = c;
        std::sort(cyclePreImage + fromCycle, cyclePreImage + toCycle,
            ShorterCycle(sig, *this));
    }
}

int SigPartialIsomorphism::compareWith(const Signature& sig,
        const SigPartialIsomorphism* other, unsigned fromCycleGroup) const {
    int result;
    for (unsigned c = sig.cycleGroupStart[fromCycleGroup]; c < nCycles; c++) {
        if (other)
            result = Signature::cycleCmp(sig, cyclePreImage[c],
                cycleStart[cyclePreImage[c]], dir, labelImage,
                sig, other->cyclePreImage[c],
                other->cycleStart[other->cyclePreImage[c]], other->dir,
                other->labelImage);
        else
            result = Signature::cycleCmp(sig, cyclePreImage[c],
                cycleStart[cyclePreImage[c]], dir, labelImage,
                sig, c, 0, 1, 0);
        if (result < 0)
            return -1;
        if (result > 0)
            return 1;
    }

    return 0;
}

} // namespace regina

