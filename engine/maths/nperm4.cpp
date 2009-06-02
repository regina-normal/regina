
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "maths/nperm4.h"
#include "triangulation/nedge.h" // deprecated
#include "triangulation/nface.h" // deprecated

namespace regina {

const NPerm4 NPerm4::S4[24] = {
    NPerm4(0,1,2,3), NPerm4(0,1,3,2), NPerm4(0,2,3,1), NPerm4(0,2,1,3),
    NPerm4(0,3,1,2), NPerm4(0,3,2,1), NPerm4(1,0,3,2), NPerm4(1,0,2,3),
    NPerm4(1,2,0,3), NPerm4(1,2,3,0), NPerm4(1,3,2,0), NPerm4(1,3,0,2),
    NPerm4(2,0,1,3), NPerm4(2,0,3,1), NPerm4(2,1,3,0), NPerm4(2,1,0,3),
    NPerm4(2,3,0,1), NPerm4(2,3,1,0), NPerm4(3,0,2,1), NPerm4(3,0,1,2),
    NPerm4(3,1,0,2), NPerm4(3,1,2,0), NPerm4(3,2,1,0), NPerm4(3,2,0,1)
};

const NPerm4* allPermsS4 = NPerm4::S4;

const unsigned NPerm4::invS4[24] = {
    0, 1, 4, 3,
    2, 5, 6, 7,
    12, 19, 18, 13,
    8, 11, 20, 15,
    16, 23, 10, 9,
    14, 21, 22, 17
};

const unsigned* allPermsS4Inv = NPerm4::invS4;

const NPerm4 NPerm4::orderedS4[24] = {
    NPerm4(0,1,2,3), NPerm4(0,1,3,2), NPerm4(0,2,1,3), NPerm4(0,2,3,1),
    NPerm4(0,3,1,2), NPerm4(0,3,2,1), NPerm4(1,0,2,3), NPerm4(1,0,3,2),
    NPerm4(1,2,0,3), NPerm4(1,2,3,0), NPerm4(1,3,0,2), NPerm4(1,3,2,0),
    NPerm4(2,0,1,3), NPerm4(2,0,3,1), NPerm4(2,1,0,3), NPerm4(2,1,3,0),
    NPerm4(2,3,0,1), NPerm4(2,3,1,0), NPerm4(3,0,1,2), NPerm4(3,0,2,1),
    NPerm4(3,1,0,2), NPerm4(3,1,2,0), NPerm4(3,2,0,1), NPerm4(3,2,1,0)
};

const NPerm4* orderedPermsS4 = NPerm4::orderedS4;

const NPerm4 NPerm4::S3[6] = {
    NPerm4(0,1,2,3), NPerm4(0,2,1,3),
    NPerm4(1,2,0,3), NPerm4(1,0,2,3),
    NPerm4(2,0,1,3), NPerm4(2,1,0,3)
};

const NPerm4* allPermsS3 = NPerm4::S3;

const unsigned NPerm4::invS3[6] = {
    0, 1,
    4, 3,
    2, 5
};

const unsigned* allPermsS3Inv = NPerm4::invS3;

const NPerm4 NPerm4::orderedS3[6] = {
    NPerm4(0,1,2,3), NPerm4(0,2,1,3),
    NPerm4(1,0,2,3), NPerm4(1,2,0,3),
    NPerm4(2,0,1,3), NPerm4(2,1,0,3)
};

const NPerm4* orderedPermsS3 = NPerm4::orderedS3;

const NPerm4 NPerm4::S2[2] = {
    NPerm4(0,1,2,3), NPerm4(1,0,2,3)
};

const NPerm4* allPermsS2 = NPerm4::S2;

const unsigned NPerm4::invS2[2] = {
    0, 1
};

const unsigned* allPermsS2Inv = NPerm4::invS2;

bool NPerm4::isPermCode(unsigned char code) {
    unsigned char mask = 0;
    for (int i = 0; i < 4; i++)
        mask |= (1 << ((code >> (2 * i)) & 3));
            // mask |= (1 << imageOf(i));
    return (mask == 15);
}

int NPerm4::sign() const {
    // The code is a little non-obvious, but this routine needs to be
    // streamlined since it gets called a _lot_ during census generation.
    unsigned char matches = 0;
    if ((code & 0x03) == 0x00)
        ++matches;
    if ((code & 0x0c) == 0x04)
        ++matches;
    if ((code & 0x30) == 0x20)
        ++matches;
    if ((code & 0xc0) == 0xc0)
        ++matches;

    if (matches == 4)
        return 1;
    if (matches == 2)
        return -1;
    if (matches == 1)
        return 1;
    if (    code == 0xb1 /* 2301 */ ||
            code == 0x1b /* 0123 */ ||
            code == 0x4e /* 1032 */)
        return 1;
    return -1;
}

int NPerm4::compareWith(const NPerm4& other) const {
    for (int i = 0; i < 4; i++) {
        if (imageOf(i) < other.imageOf(i))
            return -1;
        if (imageOf(i) > other.imageOf(i))
            return 1;
    }
    return 0;
}

std::string NPerm4::toString() const {
    char ans[5];
    for (int i = 0; i < 4; i++)
        ans[i] = '0' + imageOf(i);
    ans[4] = 0;

    return ans;
}

std::string NPerm4::trunc2() const {
    char ans[3];
    ans[0] = '0' + imageOf(0);
    ans[1] = '0' + imageOf(1);
    ans[2] = 0;
    return ans;
}

std::string NPerm4::trunc3() const {
    char ans[4];
    ans[0] = '0' + imageOf(0);
    ans[1] = '0' + imageOf(1);
    ans[2] = '0' + imageOf(2);
    ans[3] = 0;
    return ans;
}

int NPerm4::orderedS4Index() const {
    return 6 * imageOf(0) +
           2 * (imageOf(1) - (imageOf(1) > imageOf(0) ? 1 : 0)) +
               (imageOf(2) > imageOf(3) ? 1 : 0);
}

int NPerm4::S4Index() const {
    // S4 is almost the same as orderedS4, except that some pairs
    // S4[2i] <--> S4[2i+1] have been swapped to ensure that all
    // permutations S4[2i] are even and all permutations S4[2i+1] are odd.
    int retval = orderedS4Index();

    // Interchange all pairs 4i+2 <--> 4i+3.
    if (retval & 2)
        retval ^= 1;

    return retval;
}

NPerm4 faceOrdering(int face) {
    switch(face) {
        case 0:
            return NPerm4(1,2,3,0);
        case 1:
            return NPerm4(0,2,3,1);
        case 2:
            return NPerm4(0,1,3,2);
        case 3:
            return NPerm4(0,1,2,3);
    }
    return NPerm4();
}

NPerm4 edgeOrdering(int edge) {
    switch(edge) {
        case 0:
            return NPerm4(0,1,2,3);
        case 1:
            return NPerm4(0,2,3,1);
        case 2:
            return NPerm4(0,3,1,2);
        case 3:
            return NPerm4(1,2,0,3);
        case 4:
            return NPerm4(1,3,2,0);
        case 5:
            return NPerm4(2,3,0,1);
    }
    return NPerm4();
}

std::string faceDescription(int face) {
    // deprecated
    return NFace::ordering[face].trunc3();
}

std::string edgeDescription(int edge) {
    // deprecated
    return NEdge::ordering[edge].trunc2();
}

} // namespace regina

