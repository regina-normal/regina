
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "maths/arrow.h"
#include "utilities/stringutils.h"

#include "utilities/tightencodingtest.h"

using regina::Arrow;
using regina::Integer;
using regina::Laurent;

class ArrowTest : public testing::Test {
    protected:
        Arrow zero {};
        Arrow one { { {}, {0, {1}} } };
        Arrow minusOne { { {}, {0, {-1}} } };
        Arrow two { { {}, {0, {2}} } };
        Arrow minusTwo { { {}, {0, {-2}} } };
        Arrow virtualTrefoil { { {}, {-4, {1}} },
                               { {1}, {-10, {-1,0,0,0,1}} } };
};

TEST_F(ArrowTest, output) {
    EXPECT_EQ(zero.str(), "0");
    EXPECT_EQ(one.str(), "1");
    EXPECT_EQ(minusOne.str(), "-1");
    EXPECT_EQ(two.str(), "2");
    EXPECT_EQ(minusTwo.str(), "-2");
    EXPECT_EQ(virtualTrefoil.str(), "A^-4 + (A^-6 - A^-10) K_1");
}

TEST_F(ArrowTest, tightEncoding) {
    TightEncodingTest<Arrow>::verifyTightEncoding(zero);
    TightEncodingTest<Arrow>::verifyTightEncoding(one);
    TightEncodingTest<Arrow>::verifyTightEncoding(minusOne);
    TightEncodingTest<Arrow>::verifyTightEncoding(two);
    TightEncodingTest<Arrow>::verifyTightEncoding(minusTwo);
    TightEncodingTest<Arrow>::verifyTightEncoding(virtualTrefoil);
}
