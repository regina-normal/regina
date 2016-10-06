
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

const Perm<4>::Index Perm<4>::nPerms;
const Perm<4>::Index Perm<4>::nPerms_1;

const Perm<4> Perm<4>::S4[24] = {
    Perm<4>((Perm<4>::Code)0),
                Perm<4>(1),  Perm<4>(2),  Perm<4>(3),  Perm<4>(4),  Perm<4>(5),
    Perm<4>(6),  Perm<4>(7),  Perm<4>(8),  Perm<4>(9),  Perm<4>(10), Perm<4>(11),
    Perm<4>(12), Perm<4>(13), Perm<4>(14), Perm<4>(15), Perm<4>(16), Perm<4>(17),
    Perm<4>(18), Perm<4>(19), Perm<4>(20), Perm<4>(21), Perm<4>(22), Perm<4>(23)
};

const Perm<4>* Perm<4>::Sn = Perm<4>::S4;

const unsigned Perm<4>::invS4[24] = {
    0, 1, 4, 3,
    2, 5, 6, 7,
    12, 19, 18, 13,
    8, 11, 20, 15,
    16, 23, 10, 9,
    14, 21, 22, 17
};

const unsigned Perm<4>::revS4[24] = {
    22, 17, 10, 21,
    14, 9, 16, 23,
    18, 5, 2, 13,
    20, 11, 4, 19,
    6, 1, 8, 15,
    12, 3, 0, 7
};

const unsigned* Perm<4>::invSn = Perm<4>::invS4;

const Perm<4> Perm<4>::orderedS4[24] = {
    Perm<4>((Perm<4>::Code)0),  Perm<4>(1),  Perm<4>(3),  Perm<4>(2),
    Perm<4>(4),  Perm<4>(5),  Perm<4>(7),  Perm<4>(6),
    Perm<4>(8),  Perm<4>(9),  Perm<4>(11), Perm<4>(10),
    Perm<4>(12), Perm<4>(13), Perm<4>(15), Perm<4>(14),
    Perm<4>(16), Perm<4>(17), Perm<4>(19), Perm<4>(18),
    Perm<4>(20), Perm<4>(21), Perm<4>(23), Perm<4>(22)
};

const Perm<4>* Perm<4>::orderedSn = Perm<4>::orderedS4;

const Perm<4> Perm<4>::S3[6] = {
    Perm<4>(0,1,2,3), Perm<4>(0,2,1,3),
    Perm<4>(1,2,0,3), Perm<4>(1,0,2,3),
    Perm<4>(2,0,1,3), Perm<4>(2,1,0,3)
};

const Perm<4>* Perm<4>::Sn_1 = Perm<4>::S3;

const Perm<4> Perm<4>::orderedS3[6] = {
    Perm<4>(0,1,2,3), Perm<4>(0,2,1,3),
    Perm<4>(1,0,2,3), Perm<4>(1,2,0,3),
    Perm<4>(2,0,1,3), Perm<4>(2,1,0,3)
};

const Perm<4> Perm<4>::S2[2] = {
    Perm<4>(0,1,2,3), Perm<4>(1,0,2,3)
};

const Perm<4>::Code Perm<4>::imageTable[24][4] = {
    { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 2, 3, 1 }, { 0, 2, 1, 3 },
    { 0, 3, 1, 2 }, { 0, 3, 2, 1 }, { 1, 0, 3, 2 }, { 1, 0, 2, 3 },
    { 1, 2, 0, 3 }, { 1, 2, 3, 0 }, { 1, 3, 2, 0 }, { 1, 3, 0, 2 },
    { 2, 0, 1, 3 }, { 2, 0, 3, 1 }, { 2, 1, 3, 0 }, { 2, 1, 0, 3 },
    { 2, 3, 0, 1 }, { 2, 3, 1, 0 }, { 3, 0, 2, 1 }, { 3, 0, 1, 2 },
    { 3, 1, 0, 2 }, { 3, 1, 2, 0 }, { 3, 2, 1, 0 }, { 3, 2, 0, 1 }
};

const Perm<4>::Code Perm<4>::productTable[24][24] = {
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

const Perm<4>::Code Perm<4>::swapTable[4][4] = {
    {  0, 7, 15, 21 },
    {  7, 0,  3,  5 },
    { 15, 3,  0,  1 },
    { 21, 5,  1,  0 }
};

Perm<4>::Perm(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1) {
    int image[4];
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    code_ = static_cast<Code>(S4Index(image[0], image[1], image[2], image[3]));
}

Perm<4>::Perm(const int* a, const int* b) {
    int image[4];
    image[a[0]] = b[0];
    image[a[1]] = b[1];
    image[a[2]] = b[2];
    image[a[3]] = b[3];
    code_ = static_cast<Code>(S4Index(image[0], image[1], image[2], image[3]));
}

bool Perm<4>::isPermCode(Code code) {
    unsigned mask = 0;
    for (int i = 0; i < 4; i++)
        mask |= (1 << ((code >> (2 * i)) & 3));
            // mask |= (1 << imageOf(i));
    return (mask == 15);
}

std::string Perm<4>::str() const {
    char ans[5];
    for (int i = 0; i < 4; i++)
        ans[i] = static_cast<char>('0' + imageTable[code_][i]);
    ans[4] = 0;

    return ans;
}

std::string Perm<4>::trunc(unsigned len) const {
    char ans[5];
    for (unsigned i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + imageTable[code_][i]);
    ans[len] = 0;
    return ans;
}

std::string Perm<4>::trunc2() const {
    char ans[3];
    ans[0] = static_cast<char>('0' + imageTable[code_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code_][1]);
    ans[2] = 0;
    return ans;
}

std::string Perm<4>::trunc3() const {
    char ans[4];
    ans[0] = static_cast<char>('0' + imageTable[code_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code_][1]);
    ans[2] = static_cast<char>('0' + imageTable[code_][2]);
    ans[3] = 0;
    return ans;
}

} // namespace regina

