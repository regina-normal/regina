
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

Perm<7>::Code2 (*Perm<7>::products_)[5040] = nullptr;
int* Perm<7>::orders_ = nullptr;
Perm<7>::Code2 (*Perm<7>::powers_)[11] = nullptr;

namespace {
    std::mutex precomputeMutex;
}

void Perm<7>::precompute() {
    std::lock_guard<std::mutex> lock(precomputeMutex);
    if (! products_) {
        products_ = new Code2[5040][5040];
        for (Code2 i = 0; i < 5040; ++i)
            for (Code2 j = 0; j < 5040; ++j)
                products_[i][j] = (Perm<7>(i) * Perm<7>(j)).code2_;

        orders_ = new int[5040];
        powers_ = new Code2[5040][11];
        for (Code2 i = 0; i < 5040; ++i) {
            int ord = 1;
            Code2 pow = i;
            while (pow != 0) {
                // INV: pow is the S7 index of S7[i]^ord.
                powers_[i][ord] = pow;
                pow = products_[i][pow];
                ++ord;
            }
            orders_[i] = ord;
        }
    }
}

std::string Perm<7>::str() const {
    char ans[8];
    for (int i = 0; i < 7; i++)
        ans[i] = static_cast<char>('0' + (*this)[i]);
    ans[7] = 0;

    return ans;
}

std::string Perm<7>::trunc(int len) const {
    char ans[8];
    for (int i = 0; i < len; ++i)
        ans[i] = static_cast<char>('0' + (*this)[i]);
    ans[len] = 0;
    return ans;
}

} // namespace regina

