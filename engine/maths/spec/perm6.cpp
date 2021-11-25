
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

#include <mutex>
#include <sstream>
#include "maths/perm.h"

namespace regina {

Perm<6>::Code2 (*Perm<6>::products_)[720] = nullptr;

namespace {
    std::mutex precomputeMutex;
}

void Perm<6>::precompute() {
    std::lock_guard<std::mutex> lock(precomputeMutex);
    if (! products_) {
        products_ = new Code2[720][720];
        for (Code2 i = 0; i < 720; ++i)
            for (Code2 j = 0; j < 720; ++j)
                products_[i][j] = (Perm<6>(i) * Perm<6>(j)).code2_;
    }
}

std::string Perm<6>::str() const {
    char ans[7];
    for (int i = 0; i < 6; i++)
        ans[i] = static_cast<char>('0' + (*this)[i]);
    ans[6] = 0;

    return ans;
}

std::string Perm<6>::trunc(unsigned len) const {
    char ans[7];
    for (unsigned i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + (*this)[i]);
    ans[len] = 0;
    return ans;
}

} // namespace regina

