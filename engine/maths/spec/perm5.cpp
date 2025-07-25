
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <sstream>
#include "maths/perm.h"

namespace regina {

std::string Perm<5>::str() const {
    char ans[6];
    for (int i = 0; i < 5; i++)
        ans[i] = static_cast<char>('0' + imageTable[code2_][i]);
    ans[5] = 0;

    return ans;
}

std::string Perm<5>::trunc(int len) const {
    char ans[6];
    for (int i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + imageTable[code2_][i]);
    ans[len] = 0;
    return ans;
}

std::string Perm<5>::trunc2() const {
    char ans[3];
    ans[0] = static_cast<char>('0' + imageTable[code2_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code2_][1]);
    ans[2] = 0;
    return ans;
}

std::string Perm<5>::trunc3() const {
    char ans[4];
    ans[0] = static_cast<char>('0' + imageTable[code2_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code2_][1]);
    ans[2] = static_cast<char>('0' + imageTable[code2_][2]);
    ans[3] = 0;
    return ans;
}

std::string Perm<5>::trunc4() const {
    char ans[5];
    ans[0] = static_cast<char>('0' + imageTable[code2_][0]);
    ans[1] = static_cast<char>('0' + imageTable[code2_][1]);
    ans[2] = static_cast<char>('0' + imageTable[code2_][2]);
    ans[3] = static_cast<char>('0' + imageTable[code2_][3]);
    ans[4] = 0;
    return ans;
}

} // namespace regina

