
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <iostream>
#include "utilities/nmatrix2.h"

namespace regina {

NMatrix2 NMatrix2::inverse() const {
    long det = data[0][0] * data[1][1] - data[0][1] * data[1][0];
    if (det == 1)
        return NMatrix2(data[1][1], -data[0][1], -data[1][0], data[0][0]);
    else if (det == -1)
        return NMatrix2(-data[1][1], data[0][1], data[1][0], -data[0][0]);
    else
        return NMatrix2();
}

NMatrix2& NMatrix2::operator *= (const NMatrix2& other) {
    long tmp00 = data[0][0] * other.data[0][0] + data[0][1] * other.data[1][0];
    long tmp01 = data[0][0] * other.data[0][1] + data[0][1] * other.data[1][1];
    long tmp10 = data[1][0] * other.data[0][0] + data[1][1] * other.data[1][0];
    long tmp11 = data[1][0] * other.data[0][1] + data[1][1] * other.data[1][1];

    data[0][0] = tmp00;
    data[0][1] = tmp01;
    data[1][0] = tmp10;
    data[1][1] = tmp11;

    return *this;
}

bool NMatrix2::invert() {
    long det = data[0][0] * data[1][1] - data[0][1] * data[1][0];
    if (det == 1) {
        long tmp = data[0][0];
        data[0][0] = data[1][1];
        data[1][1] = tmp;
        data[0][1] = -data[0][1];
        data[1][0] = -data[1][0];
        return true;
    } else if (det == -1) {
        long tmp = data[0][0];
        data[0][0] = -data[1][1];
        data[1][1] = -tmp;
        return true;
    } else
        return false;
}

} // namespace regina

