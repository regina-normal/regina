
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <array>
#include "utilities/boolset.h"

#include "testhelper.h"

using regina::BoolSet;

class BoolSetTest : public testing::Test {
    protected:
        static constexpr BoolSet bsTrue { true };
        static constexpr BoolSet bsFalse { false };
        static constexpr BoolSet bsBoth { true, true };
        static constexpr BoolSet bsNone { };

        static constexpr std::array<BoolSet, 4> cases { BoolSet(false, false),
            BoolSet(false, true), BoolSet(true, false), BoolSet(true, true) };
};

TEST_F(BoolSetTest, stringCode) {
    EXPECT_STREQ(bsTrue.stringCode(), "T-");
    EXPECT_STREQ(bsFalse.stringCode(), "-F");
    EXPECT_STREQ(bsBoth.stringCode(), "TF");
    EXPECT_STREQ(bsNone.stringCode(), "--");

    for (auto x : cases) {
        for (auto y : cases) {
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setStringCode(y.stringCode()));
                EXPECT_EQ(tmp, y);
            }
            // TODO: Play with lower-case
        }

        BoolSet tmp = x;
        EXPECT_FALSE(tmp.setStringCode("FT"));
        EXPECT_FALSE(tmp.setStringCode("T- "));
        EXPECT_FALSE(tmp.setStringCode(" T-"));
        EXPECT_FALSE(tmp.setStringCode("T "));
        EXPECT_FALSE(tmp.setStringCode(" F"));
        EXPECT_FALSE(tmp.setStringCode(" "));
        EXPECT_FALSE(tmp.setStringCode(""));
        EXPECT_EQ(tmp, x);
    }
}

