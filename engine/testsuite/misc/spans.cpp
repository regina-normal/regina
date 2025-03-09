
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

#include "link/link.h"
#include "link/examplelink.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "gtest/gtest.h"

TEST(SpansTest, clearAllProperties) {
    // Verify that operations that should clear *all* cached properties
    // in fact do so.
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string sig = t.isoSig();
        regina::AbelianGroup hom = t.homology();

        t.insertLayeredSolidTorus(2, 3);
        EXPECT_NE(t.isoSig(), sig); // ensure the triangulation changed

        EXPECT_NE(t.homology(), hom);
    }
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string sig = t.isoSig();
        regina::AbelianGroup hom = t.homology();

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());

        t.connectedSumWith(regina::Example<3>::lens(3, 1));
        EXPECT_NE(t.isoSig(), sig); // ensure the triangulation changed

        EXPECT_NE(t.homology(), hom);
        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_FALSE(t.isIrreducible());
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        std::string brief = k.brief();

        EXPECT_FALSE(k.knowsJones());
        auto jones = k.jones();
        EXPECT_TRUE(k.knowsJones());

        k.reflect();
        EXPECT_NE(k.brief(), brief); // ensure the link changed

        EXPECT_FALSE(k.knowsJones());
        EXPECT_NE(k.jones(), jones);
        EXPECT_TRUE(k.knowsJones());
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        std::string brief = k.brief();

        EXPECT_FALSE(k.knowsJones());
        auto jones = k.jones();
        EXPECT_TRUE(k.knowsJones());

        k.resolve(k.crossing(0));
        EXPECT_NE(k.brief(), brief); // ensure the link changed

        EXPECT_FALSE(k.knowsJones());
        EXPECT_NE(k.jones(), jones);
        EXPECT_TRUE(k.knowsJones());
    }
}

TEST(SpansTest, preserveTopology) {
    // Verify that operations that should preserve cached *topological*
    // properties but clear all other cached properties in fact do so.
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string sig = t.isoSig();
        regina::AbelianGroup hom = t.homology();

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());

        EXPECT_TRUE(t.pachner(t.triangle(0)));
        EXPECT_NE(t.isoSig(), sig); // ensure the triangulation changed

        EXPECT_TRUE(t.knowsIrreducible()); // should still be cached
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_EQ(t.homology(), hom);
    }
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string sig = t.isoSig();
        regina::AbelianGroup hom = t.homology();

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());

        EXPECT_TRUE(t.move44(t.edge(0), 1));
        EXPECT_NE(t.isoSig(), sig); // ensure the triangulation changed

        EXPECT_TRUE(t.knowsIrreducible()); // should still be cached
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_EQ(t.homology(), hom);
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        std::string brief = k.brief();

        EXPECT_FALSE(k.knowsJones());
        auto jones = k.jones();
        EXPECT_TRUE(k.knowsJones());

        EXPECT_TRUE(k.r1(k.component(0), 1, 1));
        EXPECT_NE(k.brief(), brief); // ensure the link changed

        EXPECT_TRUE(k.knowsJones()); // should still be cached
        EXPECT_EQ(k.jones(), jones);
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        std::string brief = k.brief();

        EXPECT_FALSE(k.knowsJones());
        auto jones = k.jones();
        EXPECT_TRUE(k.knowsJones());

        k.rotate();
        EXPECT_NE(k.brief(), brief); // ensure the link changed

        EXPECT_TRUE(k.knowsJones()); // should still be cached
        EXPECT_EQ(k.jones(), jones);
    }
}

TEST(SpansTest, preserveAllProperties) {
    // Verify that operations that should preserve *all* cached properties
    // in fact do so.
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string enc = t.tightEncoding();
        regina::AbelianGroup hom = t.homology();

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());

        t.reorderBFS(true);
        EXPECT_NE(t.tightEncoding(), enc); // ensure the triangulation changed

        EXPECT_TRUE(t.knowsIrreducible()); // should still be cached
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_EQ(t.homology(), hom);
    }
    {
        regina::Triangulation<3> t = regina::Example<3>::lens(3, 1);
        regina::Triangulation<3> u = regina::Example<3>::lens(0, 1);
        std::string tSig = t.isoSig();
        std::string uSig = u.isoSig();
        EXPECT_NE(tSig, uSig);

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_FALSE(u.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_FALSE(u.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());
        EXPECT_TRUE(u.knowsIrreducible());

        t.swap(u);
        EXPECT_EQ(t.isoSig(), uSig);
        EXPECT_EQ(u.isoSig(), tSig);

        EXPECT_TRUE(t.knowsIrreducible()); // should still be cached
        EXPECT_TRUE(u.knowsIrreducible()); // should still be cached
        EXPECT_FALSE(t.isIrreducible());
        EXPECT_TRUE(u.isIrreducible());
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        regina::Link w = regina::ExampleLink::whitehead();
        std::string kBrief = k.brief();
        std::string wBrief = w.brief();
        EXPECT_NE(kBrief, wBrief);

        EXPECT_FALSE(k.knowsJones());
        EXPECT_FALSE(w.knowsJones());
        auto kJones = k.jones();
        auto wJones = w.jones();
        EXPECT_TRUE(k.knowsJones());
        EXPECT_TRUE(w.knowsJones());

        k.swap(w);
        EXPECT_EQ(k.brief(), wBrief);
        EXPECT_EQ(w.brief(), kBrief);

        EXPECT_TRUE(k.knowsJones()); // should still be cached
        EXPECT_TRUE(w.knowsJones()); // should still be cached
        EXPECT_EQ(k.jones(), wJones);
        EXPECT_EQ(w.jones(), kJones);
    }
}

TEST(SpansTest, clearTopologyLock) {
    // Verify that ChangeAndClearSpan<ChangeType::PreserveTopology> actually
    // clears the topology lock when it is destroyed.
    {
        regina::Triangulation<3> t = regina::Example<3>::weeks();
        std::string sig = t.isoSig();
        regina::AbelianGroup hom = t.homology();

        EXPECT_FALSE(t.knowsIrreducible());
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_TRUE(t.knowsIrreducible());

        EXPECT_TRUE(t.pachner(t.triangle(0))); // preserves topology
        EXPECT_NE(t.isoSig(), sig); // ensure the triangulation changed

        EXPECT_TRUE(t.knowsIrreducible()); // should still be cached
        EXPECT_TRUE(t.isIrreducible());
        EXPECT_EQ(t.homology(), hom);

        t.connectedSumWith(regina::Example<3>::lens(3, 1)); // changes topology

        EXPECT_FALSE(t.knowsIrreducible()); // should no longer be cached
        EXPECT_FALSE(t.isIrreducible());
        EXPECT_NE(t.isoSig(), sig);
        EXPECT_NE(t.homology(), hom);
    }
    {
        regina::Link k = regina::ExampleLink::trefoil();
        std::string brief = k.brief();

        EXPECT_FALSE(k.knowsJones());
        auto jones = k.jones();
        EXPECT_TRUE(k.knowsJones());

        EXPECT_TRUE(k.r1(k.component(0), 1, 1)); // preserves topology
        EXPECT_NE(k.brief(), brief); // ensure the link changed

        EXPECT_TRUE(k.knowsJones()); // should still be cached
        EXPECT_EQ(k.jones(), jones);

        k.changeAll(); // changes topology

        EXPECT_FALSE(k.knowsJones()); // should no longer be cached
        EXPECT_NE(k.jones(), jones);
        EXPECT_TRUE(k.knowsJones());
        EXPECT_NE(k.brief(), brief);
    }
}

