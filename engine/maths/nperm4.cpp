
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
    NPerm4(0),  NPerm4(1),  NPerm4(2),  NPerm4(3),  NPerm4(4),  NPerm4(5),
    NPerm4(6),  NPerm4(7),  NPerm4(8),  NPerm4(9),  NPerm4(10), NPerm4(11),
    NPerm4(12), NPerm4(13), NPerm4(14), NPerm4(15), NPerm4(16), NPerm4(17),
    NPerm4(18), NPerm4(19), NPerm4(20), NPerm4(21), NPerm4(22), NPerm4(23)
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
    NPerm4(0),  NPerm4(1),  NPerm4(3),  NPerm4(2),
    NPerm4(4),  NPerm4(5),  NPerm4(7),  NPerm4(6),
    NPerm4(8),  NPerm4(9),  NPerm4(11), NPerm4(10),
    NPerm4(12), NPerm4(13), NPerm4(15), NPerm4(14),
    NPerm4(16), NPerm4(17), NPerm4(19), NPerm4(18),
    NPerm4(20), NPerm4(21), NPerm4(23), NPerm4(22)
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

const unsigned char NPerm4::imageTable[24][4] = {
    { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 2, 3, 1 }, { 0, 2, 1, 3 },
    { 0, 3, 1, 2 }, { 0, 3, 2, 1 }, { 1, 0, 3, 2 }, { 1, 0, 2, 3 },
    { 1, 2, 0, 3 }, { 1, 2, 3, 0 }, { 1, 3, 2, 0 }, { 1, 3, 0, 2 },
    { 2, 0, 1, 3 }, { 2, 0, 3, 1 }, { 2, 1, 3, 0 }, { 2, 1, 0, 3 },
    { 2, 3, 0, 1 }, { 2, 3, 1, 0 }, { 3, 0, 2, 1 }, { 3, 0, 1, 2 },
    { 3, 1, 0, 2 }, { 3, 1, 2, 0 }, { 3, 2, 1, 0 }, { 3, 2, 0, 1 }
};

const unsigned char NPerm4::productTable[24][24] = {
    // Generated using an older version of Regina in which products were
    // computed (not simply looked up from a dictionary like the one below).
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 },
    { 1,0,5,4,3,2,7,6,11,10,9,8,19,18,21,20,23,22,13,12,15,14,17,16 },
    { 2,3,4,5,0,1,12,13,16,17,14,15,18,19,22,23,20,21,6,7,8,9,10,11 },
    { 3,2,1,0,5,4,13,12,15,14,17,16,7,6,9,8,11,10,19,18,23,22,21,20 },
    { 4,5,0,1,2,3,18,19,20,21,22,23,6,7,10,11,8,9,12,13,16,17,14,15 },
    { 5,4,3,2,1,0,19,18,23,22,21,20,13,12,17,16,15,14,7,6,11,10,9,8 },
    { 6,7,10,11,8,9,0,1,4,5,2,3,20,21,18,19,22,23,14,15,12,13,16,17 },
    { 7,6,9,8,11,10,1,0,3,2,5,4,15,14,13,12,17,16,21,20,19,18,23,22 },
    { 8,9,6,7,10,11,14,15,12,13,16,17,0,1,2,3,4,5,20,21,22,23,18,19 },
    { 9,8,11,10,7,6,15,14,17,16,13,12,21,20,23,22,19,18,1,0,3,2,5,4 },
    { 10,11,8,9,6,7,20,21,22,23,18,19,14,15,16,17,12,13,0,1,4,5,2,3 },
    { 11,10,7,6,9,8,21,20,19,18,23,22,1,0,5,4,3,2,15,14,17,16,13,12 },
    { 12,13,14,15,16,17,2,3,0,1,4,5,8,9,6,7,10,11,22,23,18,19,20,21 },
    { 13,12,17,16,15,14,3,2,5,4,1,0,23,22,19,18,21,20,9,8,7,6,11,10 },
    { 14,15,16,17,12,13,8,9,10,11,6,7,22,23,20,21,18,19,2,3,0,1,4,5 },
    { 15,14,13,12,17,16,9,8,7,6,11,10,3,2,1,0,5,4,23,22,21,20,19,18 },
    { 16,17,12,13,14,15,22,23,18,19,20,21,2,3,4,5,0,1,8,9,10,11,6,7 },
    { 17,16,15,14,13,12,23,22,21,20,19,18,9,8,11,10,7,6,3,2,5,4,1,0 },
    { 18,19,22,23,20,21,4,5,2,3,0,1,16,17,12,13,14,15,10,11,6,7,8,9 },
    { 19,18,21,20,23,22,5,4,1,0,3,2,11,10,7,6,9,8,17,16,13,12,15,14 },
    { 20,21,18,19,22,23,10,11,6,7,8,9,4,5,0,1,2,3,16,17,14,15,12,13 },
    { 21,20,23,22,19,18,11,10,9,8,7,6,17,16,15,14,13,12,5,4,1,0,3,2 },
    { 22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1 },
    { 23,22,19,18,21,20,17,16,13,12,15,14,5,4,3,2,1,0,11,10,9,8,7,6 }
};

const unsigned char NPerm4::swapTable[4][4] = {
    {  0, 7, 15, 21 },
    {  7, 0,  3,  5 },
    { 15, 3,  0,  1 },
    { 21, 5,  1,  0 }
};

NPerm4::NPerm4(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1) {
    int image[4];
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    code_ = static_cast<unsigned char>(
        S4Index(image[0], image[1], image[2], image[3]));
}

bool NPerm4::isPermCode(unsigned char code) {
    unsigned char mask = 0;
    for (int i = 0; i < 4; i++)
        mask |= (1 << ((code >> (2 * i)) & 3));
            // mask |= (1 << imageOf(i));
    return (mask == 15);
}

std::string NPerm4::toString() const {
    char ans[5];
    for (int i = 0; i < 4; i++)
        ans[i] = '0' + static_cast<char>(imageTable[code_][i]);
    ans[4] = 0;

    return ans;
}

std::string NPerm4::trunc2() const {
    char ans[3];
    ans[0] = '0' + static_cast<char>(imageTable[code_][0]);
    ans[1] = '0' + static_cast<char>(imageTable[code_][1]);
    ans[2] = 0;
    return ans;
}

std::string NPerm4::trunc3() const {
    char ans[4];
    ans[0] = '0' + static_cast<char>(imageTable[code_][0]);
    ans[1] = '0' + static_cast<char>(imageTable[code_][1]);
    ans[2] = '0' + static_cast<char>(imageTable[code_][2]);
    ans[3] = 0;
    return ans;
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

