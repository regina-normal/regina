
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "tangle.h"
#include <cassert>

namespace regina {

void Tangle::add(const Tangle& other) {
    // TODO
}

Link* Tangle::numClosure() {
    Link* ans = new Link();

    // Make a clone of this tangle, which as a side-effect also clones
    // the crossings.
    Tangle clone(*this);

    int i;

    if (type_ == '-') {
        // We obtain two components.
        for (i = 0; i < 2; ++i) {
            if (clone.end_[i][0]) {
                clone.end_[i][1].crossing()->next_[clone.end_[i][1].strand()] =
                    clone.end_[i][0];
                clone.end_[i][0].crossing()->prev_[clone.end_[i][0].strand()] =
                    clone.end_[i][1];
            }
            ans->components_.push_back(clone.end_[i][0]);
        }
    } else if (type_ == 'x') {
        // We have just one component, and the orientations of the two
        // strings of the tangle are consistent.
        //
        // Note that, in the diagonal case, both strings *must* meet at
        // least one crossing.
        assert(clone.end_[0][0] && clone.end_[1][0]);

        for (i = 0; i < 2; ++i) {
            clone.end_[i][1].crossing()->next_[clone.end_[i][1].strand()] =
                clone.end_[i ^ 1][0];
            clone.end_[i][0].crossing()->prev_[clone.end_[i][0].strand()] =
                clone.end_[i ^ 1][1];
        }
        ans->components_.push_back(clone.end_[0][0]);
    } else {
        // The vertical case.
        // We have just one component, and the orientations of the two
        // strings of the tangle are inconsistent.
        if (clone.end_[0][0] && clone.end_[1][0]) {
            clone.reverse(1);
            for (i = 0; i < 2; ++i) {
                clone.end_[i][i].crossing()->prev_[clone.end_[i][i].strand()] =
                    clone.end_[i ^ 1][i];
                clone.end_[i ^ 1][i].crossing()->next_[clone.end_[i ^ 1][i].
                    strand()] = clone.end_[i][i];
            }
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[0][0]) {
            // Just connect the ends of the left-hand string.
            clone.end_[0][1].crossing()->next_[clone.end_[0][1].strand()] =
                clone.end_[0][0];
            clone.end_[0][0].crossing()->prev_[clone.end_[0][0].strand()] =
                clone.end_[0][1];
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[1][0]) {
            // Just connect the ends of the right-hand string.
            clone.end_[1][1].crossing()->next_[clone.end_[1][1].strand()] =
                clone.end_[1][0];
            clone.end_[1][0].crossing()->prev_[clone.end_[1][0].strand()] =
                clone.end_[1][1];
            ans->components_.push_back(clone.end_[1][0]);
        } else {
            ans->components_.push_back(StrandRef());
        }
    }

    // Transfer all the crossings directly to the new link.
    ans->crossings_.swap(clone.crossings_);

    return ans;
}

Link* Tangle::denClosure() {
    Link* ans = new Link();

    // Make a clone of this tangle, which as a side-effect also clones
    // the crossings.
    Tangle clone(*this);

    int i;

    if (type_ == '|') {
        // We obtain two components.
        for (i = 0; i < 2; ++i) {
            if (clone.end_[i][0]) {
                clone.end_[i][1].crossing()->next_[clone.end_[i][1].strand()] =
                    clone.end_[i][0];
                clone.end_[i][0].crossing()->prev_[clone.end_[i][0].strand()] =
                    clone.end_[i][1];
            }
            ans->components_.push_back(clone.end_[i][0]);
        }
    } else if (type_ == 'x') {
        // We have just one component, and the orientations of the two
        // strings of the tangle are inconsistent.
        //
        // Note that, in the diagonal case, both strings *must* meet at
        // least one crossing.
        assert(clone.end_[0][0] && clone.end_[1][0]);

        clone.reverse(1);
        for (i = 0; i < 2; ++i) {
            clone.end_[i][i].crossing()->prev_[clone.end_[i][i].strand()] =
                clone.end_[i ^ 1][i];
            clone.end_[i ^ 1][i].crossing()->next_[clone.end_[i ^ 1][i].
                strand()] = clone.end_[i][i];
        }
        ans->components_.push_back(clone.end_[0][0]);
    } else {
        // The horizontal case.
        // We have just one component, and the orientations of the two
        // strings of the tangle are inconsistent.
        if (clone.end_[0][0] && clone.end_[1][0]) {
            clone.reverse(1);
            for (i = 0; i < 2; ++i) {
                clone.end_[i][i].crossing()->prev_[clone.end_[i][i].strand()] =
                    clone.end_[i ^ 1][i];
                clone.end_[i ^ 1][i].crossing()->next_[clone.end_[i ^ 1][i].
                    strand()] = clone.end_[i][i];
            }
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[0][0]) {
            // Just connect the ends of the left-hand string.
            clone.end_[0][1].crossing()->next_[clone.end_[0][1].strand()] =
                clone.end_[0][0];
            clone.end_[0][0].crossing()->prev_[clone.end_[0][0].strand()] =
                clone.end_[0][1];
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[1][0]) {
            // Just connect the ends of the right-hand string.
            clone.end_[1][1].crossing()->next_[clone.end_[1][1].strand()] =
                clone.end_[1][0];
            clone.end_[1][0].crossing()->prev_[clone.end_[1][0].strand()] =
                clone.end_[1][1];
            ans->components_.push_back(clone.end_[1][0]);
        } else {
            ans->components_.push_back(StrandRef());
        }
    }

    // Transfer all the crossings directly to the new link.
    ans->crossings_.swap(clone.crossings_);

    return ans;
}

} // namespace regina

