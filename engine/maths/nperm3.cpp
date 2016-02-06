
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
#include "maths/nperm3.h"

namespace regina {

const NPerm<3>::Index NPerm<3>::nPerms;
const NPerm<3>::Index NPerm<3>::nPerms_1;

const NPerm<3> NPerm<3>::S3[6] = {
    NPerm<3>((NPerm<3>::Code)0), NPerm<3>(1), NPerm<3>(2),
    NPerm<3>(3), NPerm<3>(4), NPerm<3>(5)
};

const NPerm<3>* NPerm<3>::Sn = NPerm<3>::S3;

const unsigned NPerm<3>::invS3[6] = {
    0, 1, 4, 3, 2, 5
};

const unsigned NPerm<3>::revS3[6] = {
    5, 2, 1, 4, 3, 0
};

const unsigned* NPerm<3>::invSn = NPerm<3>::invS3;

const NPerm<3> NPerm<3>::orderedS3[6] = {
    NPerm<3>(code012), NPerm<3>(code021),
    NPerm<3>(code102), NPerm<3>(code120),
    NPerm<3>(code201), NPerm<3>(code210)
};

const NPerm<3>* NPerm<3>::orderedSn = NPerm<3>::orderedS3;

const NPerm<3> NPerm<3>::S2[2] = {
    NPerm<3>(code012), NPerm<3>(code102)
};

const NPerm<3>* NPerm<3>::Sn_1 = NPerm<3>::S2;

const NPerm<3>::Code NPerm<3>::imageTable[6][3] = {
    { 0, 1, 2 },
    { 0, 2, 1 },
    { 1, 2, 0 },
    { 1, 0, 2 },
    { 2, 0, 1 },
    { 2, 1, 0 }
};

const NPerm<3>::Code NPerm<3>::productTable[6][6] = {
    { 0, 1, 2, 3, 4, 5 },
    { 1, 0, 5, 4, 3, 2 },
    { 2, 3, 4, 5, 0, 1 },
    { 3, 2, 1, 0, 5, 4 },
    { 4, 5, 0, 1, 2, 3 },
    { 5, 4, 3, 2, 1, 0 }
};

NPerm<3>::NPerm(const int* a, const int* b) {
    int image[3];
    image[a[0]] = b[0];
    image[a[1]] = b[1];
    image[a[2]] = b[2];

    switch (image[0]) {
        case 0:
            code_ = static_cast<Code>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(image[1] == 0 ? 4 : 5); break;
    }
}

std::string NPerm<3>::str() const {
    char ans[4];
    ans[0] = static_cast<char>('0' + imageTable[code_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code_][1]);
    ans[2] = static_cast<char>('0' + imageTable[code_][2]);
    ans[3] = 0;
    return ans;
}

std::string NPerm<3>::trunc(unsigned len) const {
    char ans[4];
    for (unsigned i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + imageTable[code_][i]);
    ans[len] = 0;
    return ans;
}

std::string NPerm<3>::trunc2() const {
    char ans[3];
    ans[0] = static_cast<char>('0' + imageTable[code_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code_][1]);
    ans[2] = 0;
    return ans;
}

} // namespace regina

