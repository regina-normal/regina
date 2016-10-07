
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

#include <sstream>
#include "maths/perm.h"

namespace regina {

const Perm<5>::Index Perm<5>::nPerms;
const Perm<5>::Index Perm<5>::nPerms_1;
const int Perm<5>::imageBits;

const Perm<5> Perm<5>::S5[120] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,1,2,4,3), Perm<5>(0,1,3,4,2), Perm<5>(0,1,3,2,4),
    Perm<5>(0,1,4,2,3), Perm<5>(0,1,4,3,2), Perm<5>(0,2,1,4,3), Perm<5>(0,2,1,3,4),
    Perm<5>(0,2,3,1,4), Perm<5>(0,2,3,4,1), Perm<5>(0,2,4,3,1), Perm<5>(0,2,4,1,3),
    Perm<5>(0,3,1,2,4), Perm<5>(0,3,1,4,2), Perm<5>(0,3,2,4,1), Perm<5>(0,3,2,1,4),
    Perm<5>(0,3,4,1,2), Perm<5>(0,3,4,2,1), Perm<5>(0,4,1,3,2), Perm<5>(0,4,1,2,3),
    Perm<5>(0,4,2,1,3), Perm<5>(0,4,2,3,1), Perm<5>(0,4,3,2,1), Perm<5>(0,4,3,1,2),
    Perm<5>(1,0,2,4,3), Perm<5>(1,0,2,3,4), Perm<5>(1,0,3,2,4), Perm<5>(1,0,3,4,2),
    Perm<5>(1,0,4,3,2), Perm<5>(1,0,4,2,3), Perm<5>(1,2,0,3,4), Perm<5>(1,2,0,4,3),
    Perm<5>(1,2,3,4,0), Perm<5>(1,2,3,0,4), Perm<5>(1,2,4,0,3), Perm<5>(1,2,4,3,0),
    Perm<5>(1,3,0,4,2), Perm<5>(1,3,0,2,4), Perm<5>(1,3,2,0,4), Perm<5>(1,3,2,4,0),
    Perm<5>(1,3,4,2,0), Perm<5>(1,3,4,0,2), Perm<5>(1,4,0,2,3), Perm<5>(1,4,0,3,2),
    Perm<5>(1,4,2,3,0), Perm<5>(1,4,2,0,3), Perm<5>(1,4,3,0,2), Perm<5>(1,4,3,2,0),
    Perm<5>(2,0,1,3,4), Perm<5>(2,0,1,4,3), Perm<5>(2,0,3,4,1), Perm<5>(2,0,3,1,4),
    Perm<5>(2,0,4,1,3), Perm<5>(2,0,4,3,1), Perm<5>(2,1,0,4,3), Perm<5>(2,1,0,3,4),
    Perm<5>(2,1,3,0,4), Perm<5>(2,1,3,4,0), Perm<5>(2,1,4,3,0), Perm<5>(2,1,4,0,3),
    Perm<5>(2,3,0,1,4), Perm<5>(2,3,0,4,1), Perm<5>(2,3,1,4,0), Perm<5>(2,3,1,0,4),
    Perm<5>(2,3,4,0,1), Perm<5>(2,3,4,1,0), Perm<5>(2,4,0,3,1), Perm<5>(2,4,0,1,3),
    Perm<5>(2,4,1,0,3), Perm<5>(2,4,1,3,0), Perm<5>(2,4,3,1,0), Perm<5>(2,4,3,0,1),
    Perm<5>(3,0,1,4,2), Perm<5>(3,0,1,2,4), Perm<5>(3,0,2,1,4), Perm<5>(3,0,2,4,1),
    Perm<5>(3,0,4,2,1), Perm<5>(3,0,4,1,2), Perm<5>(3,1,0,2,4), Perm<5>(3,1,0,4,2),
    Perm<5>(3,1,2,4,0), Perm<5>(3,1,2,0,4), Perm<5>(3,1,4,0,2), Perm<5>(3,1,4,2,0),
    Perm<5>(3,2,0,4,1), Perm<5>(3,2,0,1,4), Perm<5>(3,2,1,0,4), Perm<5>(3,2,1,4,0),
    Perm<5>(3,2,4,1,0), Perm<5>(3,2,4,0,1), Perm<5>(3,4,0,1,2), Perm<5>(3,4,0,2,1),
    Perm<5>(3,4,1,2,0), Perm<5>(3,4,1,0,2), Perm<5>(3,4,2,0,1), Perm<5>(3,4,2,1,0),
    Perm<5>(4,0,1,2,3), Perm<5>(4,0,1,3,2), Perm<5>(4,0,2,3,1), Perm<5>(4,0,2,1,3),
    Perm<5>(4,0,3,1,2), Perm<5>(4,0,3,2,1), Perm<5>(4,1,0,3,2), Perm<5>(4,1,0,2,3),
    Perm<5>(4,1,2,0,3), Perm<5>(4,1,2,3,0), Perm<5>(4,1,3,2,0), Perm<5>(4,1,3,0,2),
    Perm<5>(4,2,0,1,3), Perm<5>(4,2,0,3,1), Perm<5>(4,2,1,3,0), Perm<5>(4,2,1,0,3),
    Perm<5>(4,2,3,0,1), Perm<5>(4,2,3,1,0), Perm<5>(4,3,0,2,1), Perm<5>(4,3,0,1,2),
    Perm<5>(4,3,1,0,2), Perm<5>(4,3,1,2,0), Perm<5>(4,3,2,1,0), Perm<5>(4,3,2,0,1),
};

const Perm<5>* Perm<5>::Sn = Perm<5>::S5;

const Perm<5> Perm<5>::orderedS5[120] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,1,2,4,3), Perm<5>(0,1,3,2,4), Perm<5>(0,1,3,4,2),
    Perm<5>(0,1,4,2,3), Perm<5>(0,1,4,3,2), Perm<5>(0,2,1,3,4), Perm<5>(0,2,1,4,3),
    Perm<5>(0,2,3,1,4), Perm<5>(0,2,3,4,1), Perm<5>(0,2,4,1,3), Perm<5>(0,2,4,3,1),
    Perm<5>(0,3,1,2,4), Perm<5>(0,3,1,4,2), Perm<5>(0,3,2,1,4), Perm<5>(0,3,2,4,1),
    Perm<5>(0,3,4,1,2), Perm<5>(0,3,4,2,1), Perm<5>(0,4,1,2,3), Perm<5>(0,4,1,3,2),
    Perm<5>(0,4,2,1,3), Perm<5>(0,4,2,3,1), Perm<5>(0,4,3,1,2), Perm<5>(0,4,3,2,1),
    Perm<5>(1,0,2,3,4), Perm<5>(1,0,2,4,3), Perm<5>(1,0,3,2,4), Perm<5>(1,0,3,4,2),
    Perm<5>(1,0,4,2,3), Perm<5>(1,0,4,3,2), Perm<5>(1,2,0,3,4), Perm<5>(1,2,0,4,3),
    Perm<5>(1,2,3,0,4), Perm<5>(1,2,3,4,0), Perm<5>(1,2,4,0,3), Perm<5>(1,2,4,3,0),
    Perm<5>(1,3,0,2,4), Perm<5>(1,3,0,4,2), Perm<5>(1,3,2,0,4), Perm<5>(1,3,2,4,0),
    Perm<5>(1,3,4,0,2), Perm<5>(1,3,4,2,0), Perm<5>(1,4,0,2,3), Perm<5>(1,4,0,3,2),
    Perm<5>(1,4,2,0,3), Perm<5>(1,4,2,3,0), Perm<5>(1,4,3,0,2), Perm<5>(1,4,3,2,0),
    Perm<5>(2,0,1,3,4), Perm<5>(2,0,1,4,3), Perm<5>(2,0,3,1,4), Perm<5>(2,0,3,4,1),
    Perm<5>(2,0,4,1,3), Perm<5>(2,0,4,3,1), Perm<5>(2,1,0,3,4), Perm<5>(2,1,0,4,3),
    Perm<5>(2,1,3,0,4), Perm<5>(2,1,3,4,0), Perm<5>(2,1,4,0,3), Perm<5>(2,1,4,3,0),
    Perm<5>(2,3,0,1,4), Perm<5>(2,3,0,4,1), Perm<5>(2,3,1,0,4), Perm<5>(2,3,1,4,0),
    Perm<5>(2,3,4,0,1), Perm<5>(2,3,4,1,0), Perm<5>(2,4,0,1,3), Perm<5>(2,4,0,3,1),
    Perm<5>(2,4,1,0,3), Perm<5>(2,4,1,3,0), Perm<5>(2,4,3,0,1), Perm<5>(2,4,3,1,0),
    Perm<5>(3,0,1,2,4), Perm<5>(3,0,1,4,2), Perm<5>(3,0,2,1,4), Perm<5>(3,0,2,4,1),
    Perm<5>(3,0,4,1,2), Perm<5>(3,0,4,2,1), Perm<5>(3,1,0,2,4), Perm<5>(3,1,0,4,2),
    Perm<5>(3,1,2,0,4), Perm<5>(3,1,2,4,0), Perm<5>(3,1,4,0,2), Perm<5>(3,1,4,2,0),
    Perm<5>(3,2,0,1,4), Perm<5>(3,2,0,4,1), Perm<5>(3,2,1,0,4), Perm<5>(3,2,1,4,0),
    Perm<5>(3,2,4,0,1), Perm<5>(3,2,4,1,0), Perm<5>(3,4,0,1,2), Perm<5>(3,4,0,2,1),
    Perm<5>(3,4,1,0,2), Perm<5>(3,4,1,2,0), Perm<5>(3,4,2,0,1), Perm<5>(3,4,2,1,0),
    Perm<5>(4,0,1,2,3), Perm<5>(4,0,1,3,2), Perm<5>(4,0,2,1,3), Perm<5>(4,0,2,3,1),
    Perm<5>(4,0,3,1,2), Perm<5>(4,0,3,2,1), Perm<5>(4,1,0,2,3), Perm<5>(4,1,0,3,2),
    Perm<5>(4,1,2,0,3), Perm<5>(4,1,2,3,0), Perm<5>(4,1,3,0,2), Perm<5>(4,1,3,2,0),
    Perm<5>(4,2,0,1,3), Perm<5>(4,2,0,3,1), Perm<5>(4,2,1,0,3), Perm<5>(4,2,1,3,0),
    Perm<5>(4,2,3,0,1), Perm<5>(4,2,3,1,0), Perm<5>(4,3,0,1,2), Perm<5>(4,3,0,2,1),
    Perm<5>(4,3,1,0,2), Perm<5>(4,3,1,2,0), Perm<5>(4,3,2,0,1), Perm<5>(4,3,2,1,0),
};

const Perm<5>* Perm<5>::orderedSn = Perm<5>::orderedS5;

const unsigned Perm<5>::invS5[120] = {
     0,  1,  4,  3,  2,  5,  6,  7, 12, 19, 18, 13,
     8, 11, 20, 15, 16, 23, 10,  9, 14, 21, 22, 17,
    24, 25, 26, 29, 28, 27, 48, 49, 96, 73, 72, 97,
    52, 51, 74, 99,100, 77, 50, 53, 98, 75, 76,101,
    30, 31, 42, 37, 36, 43, 54, 55, 78,103,102, 79,
    60, 67,108, 85, 90,115, 66, 61, 84,109,114, 91,
    34, 33, 38, 45, 46, 41, 56, 59,104, 81, 82,107,
    68, 63, 86,111,116, 93, 64, 71,112, 89, 94,119,
    32, 35, 44, 39, 40, 47, 58, 57, 80,105,106, 83,
    62, 69,110, 87, 92,117, 70, 65, 88,113,118, 95
};

const unsigned* Perm<5>::invSn = Perm<5>::invS5;

const Perm<5> Perm<5>::S4[24] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,1,3,2,4), Perm<5>(0,2,3,1,4), Perm<5>(0,2,1,3,4),
    Perm<5>(0,3,1,2,4), Perm<5>(0,3,2,1,4), Perm<5>(1,0,3,2,4), Perm<5>(1,0,2,3,4),
    Perm<5>(1,2,0,3,4), Perm<5>(1,2,3,0,4), Perm<5>(1,3,2,0,4), Perm<5>(1,3,0,2,4),
    Perm<5>(2,0,1,3,4), Perm<5>(2,0,3,1,4), Perm<5>(2,1,3,0,4), Perm<5>(2,1,0,3,4),
    Perm<5>(2,3,0,1,4), Perm<5>(2,3,1,0,4), Perm<5>(3,0,2,1,4), Perm<5>(3,0,1,2,4),
    Perm<5>(3,1,0,2,4), Perm<5>(3,1,2,0,4), Perm<5>(3,2,1,0,4), Perm<5>(3,2,0,1,4)
};

const Perm<5>* Perm<5>::Sn_1 = Perm<5>::S4;

const Perm<5> Perm<5>::orderedS4[24] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,1,3,2,4), Perm<5>(0,2,1,3,4), Perm<5>(0,2,3,1,4),
    Perm<5>(0,3,1,2,4), Perm<5>(0,3,2,1,4), Perm<5>(1,0,2,3,4), Perm<5>(1,0,3,2,4),
    Perm<5>(1,2,0,3,4), Perm<5>(1,2,3,0,4), Perm<5>(1,3,0,2,4), Perm<5>(1,3,2,0,4),
    Perm<5>(2,0,1,3,4), Perm<5>(2,0,3,1,4), Perm<5>(2,1,0,3,4), Perm<5>(2,1,3,0,4),
    Perm<5>(2,3,0,1,4), Perm<5>(2,3,1,0,4), Perm<5>(3,0,1,2,4), Perm<5>(3,0,2,1,4),
    Perm<5>(3,1,0,2,4), Perm<5>(3,1,2,0,4), Perm<5>(3,2,0,1,4), Perm<5>(3,2,1,0,4)
};

const Perm<5> Perm<5>::S3[6] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,2,1,3,4),
    Perm<5>(1,2,0,3,4), Perm<5>(1,0,2,3,4),
    Perm<5>(2,0,1,3,4), Perm<5>(2,1,0,3,4)
};

const Perm<5> Perm<5>::orderedS3[6] = {
    Perm<5>(0,1,2,3,4), Perm<5>(0,2,1,3,4),
    Perm<5>(1,0,2,3,4), Perm<5>(1,2,0,3,4),
    Perm<5>(2,0,1,3,4), Perm<5>(2,1,0,3,4)
};

const Perm<5> Perm<5>::S2[2] = {
    Perm<5>(0,1,2,3,4), Perm<5>(1,0,2,3,4)
};

bool Perm<5>::isPermCode(Code code) {
    unsigned mask = 0;
    for (int i = 0; i < 5; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 31);
}

int Perm<5>::sign() const {
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

int Perm<5>::compareWith(const Perm<5>& other) const {
    for (int i = 0; i < 5; i++) {
        if (imageOf(i) < other.imageOf(i))
            return -1;
        if (imageOf(i) > other.imageOf(i))
            return 1;
    }
    return 0;
}

std::string Perm<5>::str() const {
    char ans[6];
    for (int i = 0; i < 5; i++)
        ans[i] = static_cast<char>('0' + imageOf(i));
    ans[5] = 0;

    return ans;
}

std::string Perm<5>::trunc(unsigned len) const {
    char ans[6];
    for (unsigned i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + imageOf(i));
    ans[len] = 0;
    return ans;
}

std::string Perm<5>::trunc2() const {
    char ans[3];
    ans[0] = static_cast<char>('0' + imageOf(0));
    ans[1] = static_cast<char>('0' + imageOf(1));
    ans[2] = 0;
    return ans;
}

std::string Perm<5>::trunc3() const {
    char ans[4];
    ans[0] = static_cast<char>('0' + imageOf(0));
    ans[1] = static_cast<char>('0' + imageOf(1));
    ans[2] = static_cast<char>('0' + imageOf(2));
    ans[3] = 0;
    return ans;
}

std::string Perm<5>::trunc4() const {
    char ans[5];
    ans[0] = static_cast<char>('0' + imageOf(0));
    ans[1] = static_cast<char>('0' + imageOf(1));
    ans[2] = static_cast<char>('0' + imageOf(2));
    ans[3] = static_cast<char>('0' + imageOf(3));
    ans[4] = 0;
    return ans;
}

/**
 * Returns the number n such that Perm<5>::orderedS5[n] == *this.
 */
int Perm<5>::orderedS5Index() const {
    return 24*imageOf(0) +
           6*( imageOf(1)-(   (imageOf(1) > imageOf(0)) ? 1 : 0) ) +
           2*( imageOf(2)-( ( (imageOf(2) > imageOf(1)) ? 1 : 0) +
                            ( (imageOf(2) > imageOf(0)) ? 1 : 0)
                          ) ) +
             ( (imageOf(3) > imageOf(4)) ? 1 : 0 );
}

/**
 * Returns the number n such that Perm<5>::S5[n] == *this.
 */
int Perm<5>::S5Index() const {
    // S5 is almost the same as orderedS5, except that some pairs
    // S5[2i] <--> S5[2i+1] have been swapped to ensure that all
    // permutations S5[2i] are even and all permutations S5[2i+1] are odd.
    int retval = orderedS5Index();

    // Flip between 2i <--> 2i+1 if and only if
    // one but not both of (retval / 2) and (retval / 24) is even.
    // Here we use (retval >> 1), which is equivalent to (retval / 2).
    if (((retval >> 1) ^ (retval / 24)) & 1)
        retval ^= 1;

    return retval;
}

} // namespace regina

