
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <iostream>
#include "maths/matrix2.h"

namespace regina {

Matrix2 Matrix2::inverse() const {
    long det = data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
    if (det == 1)
        return Matrix2(data_[1][1], -data_[0][1], -data_[1][0], data_[0][0]);
    else if (det == -1)
        return Matrix2(-data_[1][1], data_[0][1], data_[1][0], -data_[0][0]);
    else
        return Matrix2();
}

Matrix2& Matrix2::operator *= (const Matrix2& other) {
    long tmp00 = data_[0][0] * other.data_[0][0] +
        data_[0][1] * other.data_[1][0];
    long tmp01 = data_[0][0] * other.data_[0][1] +
        data_[0][1] * other.data_[1][1];
    long tmp10 = data_[1][0] * other.data_[0][0] +
        data_[1][1] * other.data_[1][0];
    long tmp11 = data_[1][0] * other.data_[0][1] +
        data_[1][1] * other.data_[1][1];

    data_[0][0] = tmp00;
    data_[0][1] = tmp01;
    data_[1][0] = tmp10;
    data_[1][1] = tmp11;

    return *this;
}

bool Matrix2::invert() {
    long det = data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
    if (det == 1) {
        long tmp = data_[0][0];
        data_[0][0] = data_[1][1];
        data_[1][1] = tmp;
        data_[0][1] = -data_[0][1];
        data_[1][0] = -data_[1][0];
        return true;
    } else if (det == -1) {
        long tmp = data_[0][0];
        data_[0][0] = -data_[1][1];
        data_[1][1] = -tmp;
        return true;
    } else
        return false;
}

std::strong_ordering simplerThreeWay(const Matrix2& m1, const Matrix2& m2) {
    long maxAbs1 = 0, maxAbs2 = 0;
    unsigned nZeroes1 = 0, nZeroes2 = 0;
    unsigned nNeg1 = 0, nNeg2 = 0;

    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (m1[i][j] > maxAbs1)
                maxAbs1 = m1[i][j];
            if (m1[i][j] < -maxAbs1)
                maxAbs1 = -m1[i][j];
            if (m2[i][j] > maxAbs2)
                maxAbs2 = m2[i][j];
            if (m2[i][j] < -maxAbs2)
                maxAbs2 = -m2[i][j];

            if (m1[i][j] == 0)
                nZeroes1++;
            else if (m1[i][j] < 0)
                nNeg1++;
            if (m2[i][j] == 0)
                nZeroes2++;
            else if (m2[i][j] < 0)
                nNeg2++;
        }

    if (maxAbs1 < maxAbs2)
        return std::strong_ordering::less;
    if (maxAbs1 > maxAbs2)
        return std::strong_ordering::greater;

    if (nZeroes1 > nZeroes2)
        return std::strong_ordering::less;
    if (nZeroes1 < nZeroes2)
        return std::strong_ordering::greater;

    if (nNeg1 < nNeg2)
        return std::strong_ordering::less;
    if (nNeg1 > nNeg2)
        return std::strong_ordering::greater;

    // Go lexicograhpic.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (m1[i][j] < m2[i][j])
                return std::strong_ordering::less;
            else if (m1[i][j] > m2[i][j])
                return std::strong_ordering::greater;

    // They're the same.
    return std::strong_ordering::equal;
}

std::strong_ordering simplerThreeWay(
        const Matrix2& pair1first, const Matrix2& pair1second,
        const Matrix2& pair2first, const Matrix2& pair2second) {
    long maxAbs0 = 0, maxAbs1 = 0;
    unsigned nZeroes0 = 0, nZeroes1 = 0;
    unsigned nNeg0 = 0, nNeg1 = 0;

    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (pair1first[i][j] > maxAbs0)
                maxAbs0 = pair1first[i][j];
            if (pair1first[i][j] < -maxAbs0)
                maxAbs0 = -pair1first[i][j];
            if (pair1second[i][j] > maxAbs0)
                maxAbs0 = pair1second[i][j];
            if (pair1second[i][j] < -maxAbs0)
                maxAbs0 = -pair1second[i][j];
            if (pair2first[i][j] > maxAbs1)
                maxAbs1 = pair2first[i][j];
            if (pair2first[i][j] < -maxAbs1)
                maxAbs1 = -pair2first[i][j];
            if (pair2second[i][j] > maxAbs1)
                maxAbs1 = pair2second[i][j];
            if (pair2second[i][j] < -maxAbs1)
                maxAbs1 = -pair2second[i][j];

            if (pair1first[i][j] == 0)
                nZeroes0++;
            else if (pair1first[i][j] < 0)
                nNeg0++;
            if (pair1second[i][j] == 0)
                nZeroes0++;
            else if (pair1second[i][j] < 0)
                nNeg0++;
            if (pair2first[i][j] == 0)
                nZeroes1++;
            else if (pair2first[i][j] < 0)
                nNeg1++;
            if (pair2second[i][j] == 0)
                nZeroes1++;
            else if (pair2second[i][j] < 0)
                nNeg1++;
        }

    if (maxAbs0 < maxAbs1)
        return std::strong_ordering::less;
    if (maxAbs0 > maxAbs1)
        return std::strong_ordering::greater;

    if (nZeroes0 > nZeroes1)
        return std::strong_ordering::less;
    if (nZeroes0 < nZeroes1)
        return std::strong_ordering::greater;

    if (nNeg0 < nNeg1)
        return std::strong_ordering::less;
    if (nNeg0 > nNeg1)
        return std::strong_ordering::greater;

    // Go lexicograhpic.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (pair1first[i][j] < pair2first[i][j])
                return std::strong_ordering::less;
            else if (pair1first[i][j] > pair2first[i][j])
                return std::strong_ordering::greater;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (pair1second[i][j] < pair2second[i][j])
                return std::strong_ordering::less;
            else if (pair1second[i][j] > pair2second[i][j])
                return std::strong_ordering::greater;

    // They're the same.
    return std::strong_ordering::equal;
}

} // namespace regina

