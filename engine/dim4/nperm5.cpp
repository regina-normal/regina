
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <sstream>
#include "dim4/nperm5.h"

namespace regina {

bool NPerm5::isPermCode(unsigned code) {
    unsigned mask = 0;
    for (int i = 0; i < 5; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 31);
}

int NPerm5::sign() const {
    // Try to streamline this routine.

    // Count the number of elements that map to themselves.
    unsigned matches = 0;
    if ((code & 7) == 0)
        ++matches;
    if ((code & (7 << 3)) == (1 << 3))
        ++matches;
    if ((code & (7 << 6)) == (2 << 6))
        ++matches;
    if ((code & (7 << 9)) == (3 << 9))
        ++matches;
    if ((code & (7 << 12)) == (4 << 12))
        ++matches;

    if (matches == 5)
        return 1;
    if (matches == 3)
        return -1;
    if (matches == 2)
        return 1;

    // We have at most one fixed point.
    // Now count the number of order two points.
    unsigned two = 0;

    if (((code >> (3 * (code & 7))) & 7) == 0)
        ++two;
    if (((code >> (3 * ((code >> 3) & 7))) & 7) == 1)
        ++two;
    if (((code >> (3 * ((code >> 6) & 7))) & 7) == 2)
        ++two;
    if (((code >> (3 * ((code >> 9) & 7))) & 7) == 3)
        ++two;
    if (((code >> (3 * ((code >> 12) & 7))) & 7) == 4)
        ++two;

    if (matches == 1) {
        // We have one fixed point, which means we have a permutation
        // of four elements that leaves nothing fixed.
        // This means either (a b)(c d) or (a b c d).
        if (two == 5)
            return 1;
        return -1;
    }

    // We have no fixed points, which means we have either
    // (a b)(c d e) or (a b c d e).
    if (two == 0)
        return 1;
    return -1;
}

int NPerm5::compareWith(const NPerm5& other) const {
    for (int i = 0; i < 5; i++) {
        if (imageOf(i) < other.imageOf(i))
            return -1;
        if (imageOf(i) > other.imageOf(i))
            return 1;
    }
    return 0;
}

std::string NPerm5::toString() const {
    char ans[6];
    for (int i = 0; i < 5; i++)
        ans[i] = '0' + imageOf(i);
    ans[5] = 0;

    return ans;
}

} // namespace regina

