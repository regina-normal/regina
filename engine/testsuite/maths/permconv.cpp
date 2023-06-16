
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "maths/perm.h"
#include "utilities/typeutils.h"

#include "testhelper.h"

using regina::Perm;

TEST(PermConvTest, identity) {
    regina::for_constexpr<2, 9>([](auto from) {
        SCOPED_TRACE_NUMERIC(from);

        regina::for_constexpr<from + 1, 10>([from](auto to) {
            SCOPED_TRACE_NUMERIC(to);
            // Note: from is not constexpr inside the lambda, but from.value is.
            using Low = Perm<from.value>;
            using High = Perm<to>;

            for (typename Low::Index i = 0; i < Low::nPerms; ++i) {
                Low p = Low::orderedSn[i];
                Low q = Low::contract(High::extend(p));
                EXPECT_EQ(p, q);
                EXPECT_EQ(p.str(), q.str());
            }

            {
                int done = 0;
                for (typename High::Index i = 0; i < High::nPerms; ++i) {
                    High p = High::orderedSn[i];

                    bool id = true;
                    for (int j = Low::degree; j < High::degree; ++j)
                        if (p[j] != j) {
                            id = false;
                            break;
                        }
                    if (! id)
                        continue;

                    High q = High::extend(Low::contract(p));
                    EXPECT_EQ(p, q);
                    EXPECT_EQ(p.str(), q.str());

                    High r = p;
                    r.clear(Low::degree);
                    EXPECT_EQ(p, r);
                    EXPECT_EQ(p.str(), r.str());

                    ++done;
                }
                // Does it look like we tested everything that is fixed on
                // [from..to)?
                EXPECT_EQ(done, Low::nPerms);
            }
        });
    });
}

TEST(PermConvTest, strings) {
    regina::for_constexpr<2, 9>([](auto from) {
        SCOPED_TRACE_NUMERIC(from);

        regina::for_constexpr<from + 1, 10>([from](auto to) {
            SCOPED_TRACE_NUMERIC(to);
            // Note: from is not constexpr inside the lambda, but from.value is.
            using Low = Perm<from.value>;
            using High = Perm<to>;

            for (typename Low::Index i = 0; i < Low::nPerms; ++i) {
                Low p = Low::orderedSn[i];
                std::string s1 = p.str();
                for (int j = Low::degree; j < High::degree; ++j)
                    s1 += (j < 10 ? ('0' + j) : ('a' + j - 10));
                std::string s2 = High::extend(p).str();
                EXPECT_EQ(s1, s2);
            }

            {
                int done = 0;
                for (typename High::Index i = 0; i < High::nPerms; ++i) {
                    High p = High::orderedSn[i];

                    bool id = true;
                    for (int j = Low::degree; j < High::degree; ++j)
                        if (p[j] != j) {
                            id = false;
                            break;
                        }
                    if (! id)
                        continue;

                    std::string s1 = p.str();
                    std::string s2 = Low::contract(p).str();
                    for (int j = Low::degree; j < High::degree; ++j)
                        s2 += (j < 10 ? ('0' + j) : ('a' + j - 10));
                    EXPECT_EQ(s1, s2);
                    ++done;
                }
                // Does it look like we tested everything that is fixed on
                // [from..to)?
                EXPECT_EQ(done, Low::nPerms);
            }
        });
    });
}
