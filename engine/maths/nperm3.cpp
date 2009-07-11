
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
#include "maths/nperm3.h"

namespace regina {

const NPerm3 NPerm3::S3[6] = {
    NPerm3(0), NPerm3(1), NPerm3(2), NPerm3(3), NPerm3(4), NPerm3(5)
};

const int NPerm3::invS3[6] = {
    0, 1, 4, 3, 2, 5
};

const NPerm3 NPerm3::orderedS3[6] = {
    NPerm3(code012), NPerm3(code021),
    NPerm3(code102), NPerm3(code120),
    NPerm3(code201), NPerm3(code210)
};

const unsigned char NPerm3::imageTable[6][3] = {
    { 0, 1, 2 },
    { 0, 2, 1 },
    { 1, 2, 0 },
    { 1, 0, 2 },
    { 2, 0, 1 },
    { 2, 1, 0 }
};

const unsigned char NPerm3::productTable[6][6] = {
    { 0, 1, 2, 3, 4, 5 },
    { 1, 0, 5, 4, 3, 2 },
    { 2, 3, 4, 5, 0, 1 },
    { 3, 2, 1, 0, 5, 4 },
    { 4, 5, 0, 1, 2, 3 },
    { 5, 4, 3, 2, 1, 0 }
};

std::string NPerm3::toString() const {
    char ans[4];
    ans[0] = '0' + static_cast<char>(imageTable[code_][0]);
    ans[1] = '0' + static_cast<char>(imageTable[code_][1]);
    ans[2] = '0' + static_cast<char>(imageTable[code_][2]);
    ans[3] = 0;
    return ans;
}

std::string NPerm3::trunc2() const {
    char ans[3];
    ans[0] = '0' + static_cast<char>(imageTable[code_][0]);
    ans[1] = '0' + static_cast<char>(imageTable[code_][1]);
    ans[2] = 0;
    return ans;
}

} // namespace regina

