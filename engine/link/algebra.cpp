
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
        if (other.type_ == '-') {
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

void Tangle::negate() {
    if (type_ == '-') {
        type_ = '|';
    } else if (type_ == '|') {
        type_ = '-';
    } else {
        // type 'x'
        reverse(1);
        std::swap(end_[1][0], end_[1][1]);
    }

    for (Crossing* c : crossings_)
        c->sign_ = - c->sign_;
}

void Tangle::endForCorner(int corner, int& string, int& end) {
    switch (corner) {
        case 1:
            string = 0; end = 0; break;
        case 2:
            switch (type_) {
                case '|': string = 1; end = 0; break;
                case '-': string = 0; end = 1; break;
                case 'x': string = 1; end = 1; break;
            }
            break;
        case 3:
            switch (type_) {
                case '|': string = 0; end = 1; break;
                case '-':
                case 'x': string = 1; end = 0; break;
            }
            break;
        case 4:
            switch (type_) {
                case '|':
                case '-': string = 1; end = 1; break;
                case 'x': string = 0; end = 1; break;
            }
            break;
    }
}

int Tangle::cornerForEnd(int string, int end) {
    if (string == 0) {
        if (end == 0) {
            return 1;
        } else {
            switch (type_) {
                case '|': return 3;
                case '-': return 2;
                case 'x': return 4;
            }
        }
    } else {
        if (end == 0) {
            switch (type_) {
                case '|': return 2;
                case '-':
                case 'x': return 3;
            }
        } else {
            switch (type_) {
                case '|':
                case '-': return 4;
                case 'x': return 2;
            }
        }
    }
    // Should never reach here.
    return 0;
}

void Tangle::box(const Tangle& topLeft, const Tangle& topRight,
        const Tangle& bottomLeft, const Tangle& bottomRight) {
    // Sanity testing:
    if (type_ == '|' && topLeft.type_ == '-' && bottomLeft.type_ == '-') {
        std::cerr << "box(): cannot create closed 3-cycle" << std::endl;
        return;
    }
    if (type_ == '|' && topRight.type_ == '-' && bottomRight.type_ == '-') {
        std::cerr << "box(): cannot create closed 3-cycle" << std::endl;
        return;
    }
    if (type_ == '-' && topLeft.type_ == '|' && topRight.type_ == '|') {
        std::cerr << "box(): cannot create closed 3-cycle" << std::endl;
        return;
    }
    if (type_ == '-' && bottomLeft.type_ == '|' && bottomRight.type_ == '|') {
        std::cerr << "box(): cannot create closed 3-cycle" << std::endl;
        return;
    }
    if (topLeft.type_ == '-' && topRight.type_ == '-' &&
            bottomLeft.type_ == '-' && bottomRight.type_ == '-') {
        std::cerr << "box(): cannot create closed 6-cycle" << std::endl;
        return;
    }
    if (topLeft.type_ == '|' && topRight.type_ == '|' &&
            bottomLeft.type_ == '|' && bottomRight.type_ == '|') {
        std::cerr << "box(): cannot create closed 6-cycle" << std::endl;
        return;
    }
    if (topLeft.type_ == '-' && topRight.type_ == '-' && type_ == '-' &&
            bottomLeft.type_ == 'x' && bottomRight.type_ == 'x') {
        std::cerr << "box(): cannot create closed 5-cycle" << std::endl;
        return;
    }
    if (bottomLeft.type_ == '-' && bottomRight.type_ == '-' && type_ == '-' &&
            topLeft.type_ == 'x' && topRight.type_ == 'x') {
        std::cerr << "box(): cannot create closed 5-cycle" << std::endl;
        return;
    }
    if (topLeft.type_ == '|' && bottomLeft.type_ == '|' && type_ == '|' &&
            topRight.type_ == 'x' && bottomRight.type_ == 'x') {
        std::cerr << "box(): cannot create closed 5-cycle" << std::endl;
        return;
    }
    if (topRight.type_ == '|' && bottomRight.type_ == '|' && type_ == '|' &&
            topLeft.type_ == 'x' && bottomLeft.type_ == 'x') {
        std::cerr << "box(): cannot create closed 5-cycle" << std::endl;
        return;
    }
    if (topLeft.type_ == 'x' && topRight.type_ == 'x' &&
            bottomLeft.type_ == 'x' && bottomRight.type_ == 'x') {
        std::cerr << "box(): cannot create closed 4-cycle" << std::endl;
        return;
    }
    if (type_ == 'x' && topLeft.type_ == 'x' &&
            topRight.type_ == '|' && bottomLeft.type_ == '-') {
        std::cerr << "box(): cannot create closed 4-cycle" << std::endl;
        return;
    }
    if (type_ == 'x' && topRight.type_ == 'x' &&
            topLeft.type_ == '|' && bottomRight.type_ == '-') {
        std::cerr << "box(): cannot create closed 4-cycle" << std::endl;
        return;
    }
    if (type_ == 'x' && bottomLeft.type_ == 'x' &&
            bottomRight.type_ == '|' && topLeft.type_ == '-') {
        std::cerr << "box(): cannot create closed 4-cycle" << std::endl;
        return;
    }
    if (type_ == 'x' && bottomRight.type_ == 'x' &&
            bottomLeft.type_ == '|' && topRight.type_ == '-') {
        std::cerr << "box(): cannot create closed 4-cycle" << std::endl;
        return;
    }

    // Clone the arguments, which as a side-effect also clones their crossings.
    Tangle* arg[5];
    arg[0] = this;
    arg[1] = new Tangle(topLeft);
    arg[2] = new Tangle(topRight);
    arg[3] = new Tangle(bottomLeft);
    arg[4] = new Tangle(bottomRight);

    int i, j;
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 2; ++j)
            if (! arg[i]->end_[j][0]) {
                std::cerr << "box(): cannot use strings without crossings"
                    << std::endl;
                for (i = 1; i < 5; ++i)
                    delete arg[i];
                return;
            }

    char finalType;
    StrandRef finalStart[2];
    StrandRef finalEnd[2];

    int which, corner, adj, adjCorner;
    int subString, subEnd, adjString, adjEnd;
    for (int string = 0; string < 2; ++string) {
        // Keep track of which corner we are at in which sub-tangle:
        // - which is the index into arg[];
        // - corner is: 1 2
        //              3 4
        // - subString and subEnd indicate which end of which string
        //   this corner represents.

        // Trace the ith string of the final tangle.
        if (string == 0) {
            which = corner = 1;
        } else {
            if (finalType == '|')
                which = corner = 2;
            else
                which = corner = 3;
        }

        // Find the start point of this string.
        arg[which]->endForCorner(corner, subString, subEnd);
        finalStart[string] = arg[which]->end_[subString][subEnd];

        // Follow this string to its other end in the current sub-tangle.
        if (subEnd == 1)
            arg[which]->reverse(subString);
        subEnd ^= 1;
        corner = arg[which]->cornerForEnd(subString, subEnd);

        while (corner != which) {
            // We need to connect this to an adjacent sub-tangle.
            if (which == 0) {
                adj = corner;
                adjCorner = 5 - corner;
            } else if (which + corner == 5) {
                adj = 0;
                adjCorner = which;
            } else {
                adj = corner;
                adjCorner = which;
            }

            arg[adj]->endForCorner(adjCorner, adjString, adjEnd);

            if (adjEnd == 1)
                arg[adj]->reverse(adjString);

            // Make the join.
            Link::join(arg[which]->end_[subString][subEnd],
                arg[adj]->end_[adjString][adjEnd]);

            // Move into the next sub-tangle and follow its string to
            // the other end.
            which = adj;
            subString = adjString;
            subEnd = adjEnd ^ 1;
            corner = arg[which]->cornerForEnd(subString, subEnd);
        }

        // We have now exited the overall tangle.
        if (string == 0) {
            // Determine the final tangle type.
            if (which == 2)
                finalType = '-';
            else if (which == 3)
                finalType = '|';
            else
                finalType = 'x';
        }
        finalEnd[string] = arg[which]->end_[subString][subEnd];
    }

    // Transfer all crossings from all clones to this.
    // As in add(), we are abusing the MarkedVector API slightly here.
    for (i = 1; i < 5; ++i) {
        for (Crossing* c : arg[i]->crossings_)
            crossings_.push_back(c);
        arg[i]->crossings_.clear();
        delete arg[i];
    }

    type_ = finalType;
    for (i = 0; i < 2; ++i) {
        end_[i][0] = finalStart[i];
        end_[i][1] = finalEnd[i];
    }
}

Link* Tangle::numClosure() const {
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

Link* Tangle::denClosure() const {
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

