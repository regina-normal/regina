
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
#include "split/sigcensus.h"
#include "utilities/memutils.h"

namespace regina {

unsigned long formSigCensus(unsigned order, UseSignature use, void* useArgs) {
    SigCensus census(order, use, useArgs);
    census.run(0);
    return census.totalFound;
}

void* SigCensus::run(void*) {
    // Initialisations.
    sig.nCycles = 0;
    sig.nCycleGroups = 0;
    nextLabel = 0;
    std::fill(used, used + sig.order_, 0);
    totalFound = 0;

    // Try creating a first cycle.
    extendAutomorphisms();
    for (unsigned i = 2 * sig.order_; i > 0; i--)
        tryCycle(i, true, 0);
    clearTopAutomorphisms();

    return 0;
}

void SigCensus::clearTopAutomorphisms() {
    if (! automorph[sig.nCycleGroups].empty()) {
        for_each(automorph[sig.nCycleGroups].begin(),
            automorph[sig.nCycleGroups].end(),
            FuncDelete<SigPartialIsomorphism>());
        automorph[sig.nCycleGroups].clear();
    }
}

bool SigCensus::extendAutomorphisms() {
    if (sig.nCycleGroups == 0) {
        automorph[0].push_back(new SigPartialIsomorphism(1));
        automorph[0].push_back(new SigPartialIsomorphism(-1));
        return true;
    }

    SigPartialIsomorphism* iso;
    unsigned firstLabel;
    int result;
    unsigned i;
    std::list<SigPartialIsomorphism*>::const_iterator it;
    for (it = automorph[sig.nCycleGroups - 1].begin();
            it != automorph[sig.nCycleGroups - 1].end(); it++) {
        // Try extending this automorphism.
        iso = new SigPartialIsomorphism(**it, nextLabel, sig.nCycles);
        firstLabel = (*it)->nLabels;

        if (firstLabel == nextLabel) {
            iso->makeCanonical(sig, sig.nCycleGroups - 1);
            result = iso->compareWith(sig, 0, sig.nCycleGroups - 1);
            if (result == 0)
                automorph[sig.nCycleGroups].push_back(iso);
            else {
                delete iso;
                if (result < 0)
                    return false;
            }
        } else {
            for (i = firstLabel; i < nextLabel; i++)
                iso->labelImage[i] = i;
            do {
                iso->makeCanonical(sig, sig.nCycleGroups - 1);
                result = iso->compareWith(sig, 0, sig.nCycleGroups - 1);
                if (result < 0) {
                    delete iso;
                    return false;
                }
                else if (result == 0)
                    automorph[sig.nCycleGroups].push_back(
                        new SigPartialIsomorphism(*iso));
            } while (std::next_permutation(iso->labelImage + firstLabel,
                    iso->labelImage + nextLabel));
            delete iso;
        }
    }
    return true;
}

void SigCensus::tryCycle(unsigned cycleLen, bool newCycleGroup,
        unsigned startPos) {
    // Are we finished?
    if (startPos == 2 * sig.order_) {
        totalFound++;
        use(sig, automorph[sig.nCycleGroups], useArgs);
        return;
    }

    // Prepare the signature for the forthcoming cycle.
    sig.nCycles++;
    if (newCycleGroup)
        sig.nCycleGroups++;

    // Insert the cycleStart sentinel.
    unsigned endPos = startPos + cycleLen;
    sig.cycleStart[sig.nCycles] = endPos;

    // We won't insert the cycleGroupStart sentinel until we know where
    // the group will finish.

    // Generate all possibilities for this cycle.
    unsigned tryPos = startPos;
    sig.label[tryPos] = 0;

    unsigned lowerBnd, upperBnd;
    bool avoid;
    unsigned i;
    while(true) {
        if (tryPos == endPos) {
            // Found a complete cycle.
            avoid = false;
            if (startPos == 0 && used[sig.label[startPos]] == 2) {
                // We run the risk of having a cycle that could be
                // made lexicographically smaller simply by rotating it.
                i = 1;
                while (sig.label[startPos + i] != sig.label[startPos])
                    i++;
                if (Signature::cycleCmp(sig, sig.nCycles - 1, 0, 1, 0,
                        sig, sig.nCycles - 1, i, 1, 0) > 0)
                    avoid = true;
            }

            if (! avoid) {
                if (endPos == 2 * sig.order_) {
                    // Found a complete cycle set.
                    sig.cycleGroupStart[sig.nCycleGroups] = sig.nCycles;
                    if (extendAutomorphisms())
                        tryCycle(0, true, endPos);
                    clearTopAutomorphisms();
                } else {
                    // Move on to create the next cycle.
                    // The next cycle will have length i.
                    if (endPos + cycleLen <= 2 * sig.order_)
                        tryCycle(cycleLen, false, endPos);
                    sig.cycleGroupStart[sig.nCycleGroups] = sig.nCycles;
                    if (extendAutomorphisms())
                        for (i = (endPos + cycleLen - 1 <= 2 * sig.order_ ?
                                cycleLen - 1 : 2 * sig.order_ - endPos);
                                i > 0; i--)
                            tryCycle(i, true, endPos);
                    clearTopAutomorphisms();
                }
            }

            // Step back again.
            tryPos--;
            used[sig.label[tryPos]]--;
            if (sig.label[tryPos] == nextLabel - 1 &&
                    used[sig.label[tryPos]] == 0)
                nextLabel--;
            sig.label[tryPos]++;
        } else {
            // Find the next viable possibility for this position.
            if (tryPos == startPos) {
                if (newCycleGroup)
                    lowerBnd = 0;
                else
                    lowerBnd = sig.label[startPos - cycleLen];
                upperBnd = (startPos == 0 ? 1 : nextLabel);
            } else {
                lowerBnd = (startPos == 0 ? sig.label[startPos] :
                    sig.label[startPos] + 1);
                upperBnd = nextLabel + 1;
            }
            if (upperBnd >= sig.order_)
                upperBnd = sig.order_;

            if (sig.label[tryPos] < lowerBnd)
                sig.label[tryPos] = lowerBnd;
            while (sig.label[tryPos] < upperBnd) {
                if (used[sig.label[tryPos]] < 2)
                    break;
                sig.label[tryPos]++;
            }

            if (sig.label[tryPos] >= upperBnd) {
                // We've run out of ideas for this position.
                // Step back and undo the previous position.
                if (tryPos == startPos)
                    break;
                tryPos--;
                used[sig.label[tryPos]]--;
                if (sig.label[tryPos] == nextLabel - 1 &&
                        used[sig.label[tryPos]] == 0)
                    nextLabel--;
                sig.label[tryPos]++;
            } else {
                // We've found a value to try.
                used[sig.label[tryPos]]++;
                if (sig.label[tryPos] == nextLabel)
                    nextLabel++;
                tryPos++;
                sig.label[tryPos] = 0;
            }
        }
    }

    sig.nCycles--;
    if (newCycleGroup)
        sig.nCycleGroups--;
}

} // namespace regina

