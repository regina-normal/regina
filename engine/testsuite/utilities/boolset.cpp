
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
#include <cctype>
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

TEST_F(BoolSetTest, byteCode) {
    EXPECT_EQ(bsTrue.byteCode(), 1);
    EXPECT_EQ(bsFalse.byteCode(), 2);
    EXPECT_EQ(bsBoth.byteCode(), 3);
    EXPECT_EQ(bsNone.byteCode(), 0);

    for (const auto x : cases) {
        EXPECT_EQ(BoolSet::fromByteCode(x.byteCode()), x);

        for (const auto y : cases) {
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setByteCode(y.byteCode()));
                EXPECT_EQ(tmp, y);
            }
        }

        BoolSet tmp = x;
        EXPECT_FALSE(tmp.setByteCode(-1));
        EXPECT_FALSE(tmp.setByteCode(4));
        EXPECT_EQ(tmp, x);
    }
}

TEST_F(BoolSetTest, comparisons) {
    for (const auto x : cases) {
        auto xc = x.byteCode();
        for (const auto y : cases) {
            auto yc = y.byteCode();
            EXPECT_EQ(x == y, xc == yc);
            EXPECT_EQ(x != y, xc != yc);
            EXPECT_EQ(x <= y, (xc & yc) == xc);
            EXPECT_EQ(x >= y, (xc & yc) == yc);
            EXPECT_EQ(x < y, xc != yc && (xc & yc) == xc);
            EXPECT_EQ(x > y, xc != yc && (xc & yc) == yc);
        }
    }
}

TEST_F(BoolSetTest, constructors) {
    EXPECT_EQ(BoolSet(), bsNone);
    EXPECT_EQ(BoolSet(true), bsTrue);
    EXPECT_EQ(BoolSet(false), bsFalse);

    for (const auto x : cases)
        EXPECT_EQ(BoolSet(x), x);

    for (int t = 0; t < 2; ++t)
        for (int f = 0; f < 2; ++f) {
            BoolSet tmp(static_cast<bool>(t), static_cast<bool>(f));
            EXPECT_EQ(tmp.hasTrue(), static_cast<bool>(t));
            EXPECT_EQ(tmp.hasFalse(), static_cast<bool>(f));
        }
}

TEST_F(BoolSetTest, assignment) {
    for (const auto x : cases) {
        {
            BoolSet tmp = x;
            tmp = true;
            EXPECT_EQ(tmp, bsTrue);
        }
        {
            BoolSet tmp = x;
            tmp = false;
            EXPECT_EQ(tmp, bsFalse);
        }

        for (const auto y : cases) {
            BoolSet tmp = x;
            tmp = y;
            EXPECT_EQ(tmp, y);
        }
    }
}

TEST_F(BoolSetTest, simpleTests) {
    EXPECT_FALSE(bsNone.full());
    EXPECT_FALSE(bsNone.hasTrue());
    EXPECT_FALSE(bsNone.hasFalse());
    EXPECT_FALSE(bsNone.contains(true));
    EXPECT_FALSE(bsNone.contains(false));

    EXPECT_FALSE(bsTrue.full());
    EXPECT_TRUE(bsTrue.hasTrue());
    EXPECT_FALSE(bsTrue.hasFalse());
    EXPECT_TRUE(bsTrue.contains(true));
    EXPECT_FALSE(bsTrue.contains(false));

    EXPECT_FALSE(bsFalse.full());
    EXPECT_FALSE(bsFalse.hasTrue());
    EXPECT_TRUE(bsFalse.hasFalse());
    EXPECT_FALSE(bsFalse.contains(true));
    EXPECT_TRUE(bsFalse.contains(false));

    EXPECT_TRUE(bsBoth.full());
    EXPECT_TRUE(bsBoth.hasTrue());
    EXPECT_TRUE(bsBoth.hasFalse());
    EXPECT_TRUE(bsBoth.contains(true));
    EXPECT_TRUE(bsBoth.contains(false));
}

TEST_F(BoolSetTest, simpleOperations) {
    for (const auto x : cases) {
        {
            BoolSet tmp = x;
            tmp.clear();
            EXPECT_EQ(tmp, bsNone);
        }
        {
            BoolSet tmp = x;
            tmp.fill();
            EXPECT_EQ(tmp, bsBoth);
        }
        {
            BoolSet tmp = x;
            tmp.insertTrue();
            EXPECT_EQ(tmp, x.hasFalse() ? bsBoth : bsTrue);
        }
        {
            BoolSet tmp = x;
            tmp.insertFalse();
            EXPECT_EQ(tmp, x.hasTrue() ? bsBoth : bsFalse);
        }
        {
            BoolSet tmp = x;
            tmp.removeTrue();
            EXPECT_EQ(tmp, x.hasFalse() ? bsFalse : bsNone);
        }
        {
            BoolSet tmp = x;
            tmp.removeFalse();
            EXPECT_EQ(tmp, x.hasTrue() ? bsTrue : bsNone);
        }
    }
}

TEST_F(BoolSetTest, logicalOperations) {
    for (const auto x : cases) {
        EXPECT_EQ((~x).byteCode() + x.byteCode(), 3);

        for (const auto y : cases) {
            EXPECT_EQ((x | y).byteCode(), x.byteCode() | y.byteCode());
            EXPECT_EQ((x & y).byteCode(), x.byteCode() & y.byteCode());
            EXPECT_EQ((x ^ y).byteCode(), x.byteCode() ^ y.byteCode());

            {
                BoolSet tmp = x;
                tmp |= y;
                EXPECT_EQ(tmp.byteCode(), x.byteCode() | y.byteCode());
            }
            {
                BoolSet tmp = x;
                tmp &= y;
                EXPECT_EQ(tmp.byteCode(), x.byteCode() & y.byteCode());
            }
            {
                BoolSet tmp = x;
                tmp ^= y;
                EXPECT_EQ(tmp.byteCode(), x.byteCode() ^ y.byteCode());
            }
        }
    }
}

TEST_F(BoolSetTest, stringCode) {
    EXPECT_STREQ(bsTrue.stringCode(), "T-");
    EXPECT_STREQ(bsFalse.stringCode(), "-F");
    EXPECT_STREQ(bsBoth.stringCode(), "TF");
    EXPECT_STREQ(bsNone.stringCode(), "--");

    for (const auto x : cases) {
        for (const auto y : cases) {
            std::string code = y.stringCode();
            ASSERT_EQ(code.size(), 2);
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setStringCode(code));
                EXPECT_EQ(tmp, y);
            }
            code[0] = std::tolower(code[0]);
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setStringCode(code));
                EXPECT_EQ(tmp, y);
            }
            code[1] = std::tolower(code[1]);
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setStringCode(code));
                EXPECT_EQ(tmp, y);
            }
            code[0] = std::toupper(code[0]);
            {
                BoolSet tmp = x;
                EXPECT_TRUE(tmp.setStringCode(code));
                EXPECT_EQ(tmp, y);
            }
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
