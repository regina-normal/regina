
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "link/tangle.h"
#include "utilities/randutils.h"

// Affects the number of random type III moves attempted during simplification.
#define COEFF_TYPE_3 20

namespace regina {

bool Link::intelligentSimplify() {
    bool changed;

    { // Begin scope for change event block.
        ChangeEventSpan span(this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // Clone to work with when we might want to roll back changes.
        Link* use;

        // Variables used for selecting random moves.
        std::vector<std::pair<Crossing*, int>> type3Available;
        std::pair<Crossing*, int> type3Choice;

        size_t type3Attempts, type3Cap;

        int side;

        while (true) {
            // Clone the link and start making moves that might
            // or might not lead to a simplification.
            // If we've already simplified then there's no need to use a
            // separate clone since we won't need to undo further changes.
            use = (changed ? this : new Link(*this, false));

            // Make random type III Reidemeister moves.
            type3Attempts = type3Cap = 0;
            while (true) {
                // Calculate the list of available type III moves.
                type3Available.clear();
                for (Crossing* c : use->crossings_)
                    for (side = 0; side < 2; ++side)
                        if (use->r3(c, side, true, false))
                            type3Available.push_back(std::make_pair(c, side));

                // Increment type3Cap if needed.
                if (type3Cap < COEFF_TYPE_3 * type3Available.size())
                    type3Cap = COEFF_TYPE_3 * type3Available.size();

                // Have we tried enough type III moves?
                if (type3Attempts >= type3Cap)
                    break;

                // Perform a random type III move on the clone.
                type3Choice = type3Available[
                    RandomEngine::rand(type3Available.size())];
                use->r3(type3Choice.first, type3Choice.second, false, true);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimum(true)) {
                    // We have successfully simplified!
                    // Start all over again.
                    type3Attempts = type3Cap = 0;
                } else
                    ++type3Attempts;
            }

            // Sync the real link with the clone if appropriate.
            if (use != this) {
                // At this point, changed == false.
                if (use->size() < size()) {
                    // The type III moves were successful; accept them.
                    swapContents(*use);
                    changed = true;
                }
                delete use;
            }

            // Type III moves will help us no more for now.
            break;
        }
    } // End scope for change event span.

    return changed;
}

bool Link::simplifyToLocalMinimum(bool perform) {
    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    { // Begin scope for change event span.
        ChangeEventSpan span(this);

        while (changedNow) {
            changedNow = false;

            // Look for type I or II Reidemeister moves.
            for (Crossing* c : crossings_) {
                if (r1(c, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
                if (r2(c, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }
        }
    } // End scope for change event span.

    return changed;
}

bool Tangle::simplifyToLocalMinimum(bool perform) {
    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    while (changedNow) {
        changedNow = false;

        // Look for type I or II Reidemeister moves.
        for (Crossing* c : crossings_) {
            if (r1(c, true, perform)) {
                changedNow = changed = true;
                break;
            }
            if (r2(c, true, perform)) {
                changedNow = changed = true;
                break;
            }
        }
        if (changedNow) {
            if (perform)
                continue;
            else
                return true;
        }
    }

    return changed;
}

} // namespace regina

