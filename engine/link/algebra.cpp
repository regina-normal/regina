
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
    // Sanity testing:
    if (type_ == '|' && other.type_ == '|') {
        std::cerr << "add(): cannot add two vertical tangles" << std::endl;
        return;
    }

    // Make a clone of other, which as a side-effect also clones the crossings.
    Tangle clone(other);

    if (type_ == '|') {
        if (other.type_ == '=') {
            // Type (|| =).
            clone.reverse(0);

            // Sections to join:
            //   clone.end_[0][1] -> clone.end_[0][0]
            //   end_[1][0] -> end_[1][1]
            //   clone.end_[1][0] -> clone.end_[1][1]

            if (! clone.end_[0][1])
                clone.end_[0][1] = end_[1][0];
            else if (! end_[1][1])
                end_[1][1] = clone.end_[0][0];
            else
                Link::join(clone.end_[0][0], end_[1][0]);

            // Sections to join:
            //   clone.end_[0][1] -> end_[1][1]
            //   clone.end_[1][0] -> clone.end_[1][1]

            if (! clone.end_[0][1])
                clone.end_[0][1] = clone.end_[1][0];
            else if (! clone.end_[1][1])
                clone.end_[1][1] = end_[1][1];
            else
                Link::join(end_[1][1], clone.end_[1][0]);

            // Final rightmost string:
            //   clone.end_[0][1] -> clone.end_[1][1]

            end_[1][0] = clone.end_[0][1];
            end_[1][1] = clone.end_[1][1];
        } else {
            // Type (|| x).
            reverse(1);
            clone.reverse(1);

            // Sections to join:
            //   clone.end_[1][1] -> clone.end_[1][0]
            //   end_[1][1] -> end_[1][0]
            //   clone.end_[0][0] -> clone.end_[0][1]

            if (! clone.end_[1][1])
                clone.end_[1][1] = end_[1][1];
            else if (! end_[1][0])
                end_[1][0] = clone.end_[1][0];
            else
                Link::join(clone.end_[1][0], end_[1][1]);

            // Sections to join:
            //   clone.end_[1][1] -> end_[1][0]
            //   clone.end_[0][0] -> clone.end_[0][1]

            if (! clone.end_[1][1])
                clone.end_[1][1] = clone.end_[0][0];
            else if (! clone.end_[0][1])
                clone.end_[0][1] = end_[1][0];
            else
                Link::join(end_[1][0], clone.end_[0][0]);

            // Final rightmost string:
            //   clone.end_[1][1] -> clone.end_[0][1]

            end_[1][0] = clone.end_[1][1];
            end_[1][1] = clone.end_[0][1];
        }
    } else if (other.type_ == '|') {
        // Either (= ||) or (x ||).
        reverse(1);
        if (type_ == 'x') {
            // Type (x ||).
            clone.reverse(0);

            // Sections to join:
            //   end_[0][0] -> end_[0][1]
            //   clone.end_[0][1] -> clone.end_[0][0]
            //   end_[1][1] -> end_[1][0]

            if (! end_[0][0])
                end_[0][0] = clone.end_[0][1];
            else if (! clone.end_[0][0])
                clone.end_[0][0] = end_[0][1];
            else
                Link::join(end_[0][1], clone.end_[0][1]);

            // Sections to join:
            //   end_[0][0] -> clone.end_[0][0]
            //   end_[1][1] -> end_[1][0]

            if (! end_[0][0])
                end_[0][0] = end_[1][1];
            else if (! end_[1][0])
                end_[1][0] = clone.end_[0][0];
            else
                Link::join(clone.end_[0][0], end_[1][1]);

            // Final leftmost string:
            //   end_[0][0] -> end_[1][0]
        } else {
            // Type (= ||).

            // Sections to join:
            //   end_[0][0] -> end_[0][1]
            //   clone.end_[0][0] -> clone.end_[0][1]
            //   end_[1][1] -> end_[1][0]

            if (! end_[0][0])
                end_[0][0] = clone.end_[0][0];
            else if (! clone.end_[0][1])
                clone.end_[0][1] = end_[0][1];
            else
                Link::join(end_[0][1], clone.end_[0][0]);

            // Sections to join:
            //   end_[0][0] -> clone.end_[0][1]
            //   end_[1][1] -> end_[1][0]

            if (! end_[0][0])
                end_[0][0] = end_[1][1];
            else if (! end_[1][0])
                end_[1][0] = clone.end_[0][1];
            else
                Link::join(clone.end_[0][1], end_[1][1]);

            // Final leftmost string:
            //   end_[0][0] -> end_[1][0]
        }

        end_[0][1] = end_[1][0];
        end_[1][0] = clone.end_[1][0];
        end_[1][1] = clone.end_[1][1];

        type_ = '|';
    } else {
        // Either (= =), (= x), (x =) or (x x).
        // In all four cases, we can keep the all string orientations the same.

        // Do we join strings 0,1 of this to strings 0,1 of other, or do
        // the strings switch?
        int cross = (type_ == '-' ? 0 : 1);

        // Sections to join:
        //   end_[0][0] -> end_[0][1]
        //   clone.end_[cross][0] -> clone.end_[cross][1]
        // and:
        //   end_[1][0] -> end_[1][1]
        //   clone.end_[cross ^ 1][0] -> clone.end_[cross ^ 1][1]

        if (! end_[0][0])
            end_[0][0] = clone.end_[cross][0];
        else if (! clone.end_[cross][1])
            clone.end_[cross][1] = end_[0][1];
        else
            Link::join(end_[0][1], clone.end_[cross][0]);

        if (! end_[1][0])
            end_[1][0] = clone.end_[cross ^ 1][0];
        else if (! clone.end_[cross ^ 1][1])
            clone.end_[cross ^ 1][1] = end_[1][1];
        else
            Link::join(end_[1][1], clone.end_[cross ^ 1][0]);

        // Final upper and lower strings:
        //   end_[0][0] -> clone.end_[cross][1]
        // and:
        //   end_[1][0] -> clone.end_[cross ^ 1][1]

        end_[0][1] = clone.end_[cross][1];
        end_[1][1] = clone.end_[cross ^ 1][1];

        if (other.type_ == 'x')
            type_ = (type_ == '-' ? 'x' : '-');
    }

    // Transfer all crossings from clone to this.
    // We are abusing the MarkedVector API slightly here (since an object
    // must not belong to more than one MarkedVector at a time), but the
    // implementation of MarkedVector does makes this correct, and we
    // avoid having to make a temporary array of crossings instead. :/
    for (Crossing* c : clone.crossings_)
        crossings_.push_back(c);
    clone.crossings_.clear();
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
            if (clone.end_[i][0])
                Link::join(clone.end_[i][1], clone.end_[i][0]);
            ans->components_.push_back(clone.end_[i][0]);
        }
    } else if (type_ == 'x') {
        // We have just one component, and the orientations of the two
        // strings of the tangle are consistent.
        //
        // Note that, in the diagonal case, both strings *must* meet at
        // least one crossing.
        assert(clone.end_[0][0] && clone.end_[1][0]);

        for (i = 0; i < 2; ++i)
            Link::join(clone.end_[i][1], clone.end_[i ^ 1][0]);
        ans->components_.push_back(clone.end_[0][0]);
    } else {
        // The vertical case.
        // We have just one component, and the orientations of the two
        // strings of the tangle are inconsistent.
        if (clone.end_[0][0] && clone.end_[1][0]) {
            clone.reverse(1);
            for (i = 0; i < 2; ++i)
                Link::join(clone.end_[i ^ 1][i], clone.end_[i][i]);
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[0][0]) {
            // Just connect the ends of the left-hand string.
            Link::join(clone.end_[0][1], clone.end_[0][0]);
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[1][0]) {
            // Just connect the ends of the right-hand string.
            Link::join(clone.end_[1][1], clone.end_[1][0]);
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
            if (clone.end_[i][0])
                Link::join(clone.end_[i][1], clone.end_[i][0]);
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
        for (i = 0; i < 2; ++i)
            Link::join(clone.end_[i ^ 1][i], clone.end_[i][i]);
        ans->components_.push_back(clone.end_[0][0]);
    } else {
        // The horizontal case.
        // We have just one component, and the orientations of the two
        // strings of the tangle are inconsistent.
        if (clone.end_[0][0] && clone.end_[1][0]) {
            clone.reverse(1);
            for (i = 0; i < 2; ++i)
                Link::join(clone.end_[i ^ 1][i], clone.end_[i][i]);
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[0][0]) {
            // Just connect the ends of the top string.
            Link::join(clone.end_[0][1], clone.end_[0][0]);
            ans->components_.push_back(clone.end_[0][0]);
        } else if (clone.end_[1][0]) {
            // Just connect the ends of the bottom string.
            Link::join(clone.end_[1][1], clone.end_[1][0]);
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

