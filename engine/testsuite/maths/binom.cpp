
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

#include "maths/binom.h"

#include "testhelper.h"

using regina::binomSmall;
using regina::binomMedium;

TEST(BinomialTest, relations) {
    static constexpr int maxMedium = 61;

    for (int n = 0; n <= maxMedium; ++n) {
        SCOPED_TRACE_NUMERIC(n);

        EXPECT_EQ(binomMedium(n, 0), 1);
        EXPECT_EQ(binomMedium(n, n), 1);
        if (n > 0) {
            EXPECT_EQ(binomMedium(n, 1), n);
            EXPECT_EQ(binomMedium(n, n-1), n);

            for (int k = 1; k < n; ++k)
                EXPECT_EQ(binomMedium(n, k),
                    binomMedium(n-1, k-1) + binomMedium(n-1, k));
        }
    }
}
