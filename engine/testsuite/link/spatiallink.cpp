
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "link/examplelink.h"
#include "link/spatiallink.h"

#include "testhelper.h"

using regina::ExampleLink;
using regina::SpatialLink;

static void verifyIteration(const SpatialLink& link, const char* name) {
    // The node iteration code is non-trivial: verify that things appear to be
    // working correctly.

    size_t size = link.size();
    auto range = link.range();

    size_t found = 0;
    auto it = link.beginNodes();
    while (it != link.endNodes()) {
        EXPECT_GE(it->x, range.first.x);
        EXPECT_GE(it->y, range.first.y);
        EXPECT_GE(it->z, range.first.z);
        EXPECT_LE(it->x, range.second.x);
        EXPECT_LE(it->y, range.second.y);
        EXPECT_LE(it->z, range.second.z);

        ++found;
        ++it;
    }
    EXPECT_EQ(found, size);

    found = 0;
    while (it != link.beginNodes()) {
        --it;
        EXPECT_GE(it->x, range.first.x);
        EXPECT_GE(it->y, range.first.y);
        EXPECT_GE(it->z, range.first.z);
        EXPECT_LE(it->x, range.second.x);
        EXPECT_LE(it->y, range.second.y);
        EXPECT_LE(it->z, range.second.z);

        ++found;
    }
    EXPECT_EQ(found, size);
}

TEST(SpatialLinkTest, iteration) {
    verifyIteration(SpatialLink(), "Empty link");
    verifyIteration(ExampleLink::cubicalUnknot(), "Cubical unknot");
    verifyIteration(ExampleLink::spatialTrefoil(), "Trefoil");
    verifyIteration(ExampleLink::spatialHopf(), "Hopf link");
    verifyIteration(ExampleLink::spatialBorromean(), "Borromean rings");
}

TEST(SpatialLinkTest, defaultRadius) {
    // These three links use the RNG radius: the values here were computed
    // using Regina 8.0.
    EXPECT_NEAR(ExampleLink::spatialTrefoil().defaultRadius(), 0.2475, 0.001);
    EXPECT_NEAR(ExampleLink::spatialHopf().defaultRadius(), 0.9186, 0.001);
    EXPECT_NEAR(ExampleLink::spatialBorromean().defaultRadius(), 2.0810, 0.001);

    // For this link, the RNG radius fails and we end up falling back to a
    // simple heuristic using the bounding box.
    EXPECT_DOUBLE_EQ(ExampleLink::cubicalUnknot().defaultRadius(), 0.35);

    // For the empty link, the radius is irrelevant.  Regina returns 1.
    EXPECT_DOUBLE_EQ(SpatialLink().defaultRadius(), 1.0);
}

