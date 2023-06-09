
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

#include <cmath>
#include <memory>
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testhelper.h"

using regina::Example;
using regina::NormalSurfaces;
using regina::Triangulation;

TEST(IncompressibleTest, isHaken) {
    // Some non-Haken prime manifolds:
    EXPECT_FALSE(Example<3>::threeSphere().isHaken());
    EXPECT_FALSE(Example<3>::lens(2, 1).isHaken());
    EXPECT_FALSE(Example<3>::poincare().isHaken());

    // Some Haken Seifert fibred spaces and surface bundles:
    Triangulation<3> sfsSphere = Triangulation<3>::fromIsoSig(
        "gLALQbccefffemkbemi"); // SFS [S2: (2,1) (2,1) (2,1) (2,-1)]
    Triangulation<3> sfsTorus = Triangulation<3>::fromIsoSig(
        "gvLQQcdefeffnwnpkhe"); // SFS [T: (1,1)]
    Triangulation<3> bundle = Triangulation<3>::fromIsoSig(
        "gvLQQedfedffrwawrhh"); // T x S1

    EXPECT_TRUE(sfsSphere.isHaken());
    EXPECT_TRUE(sfsTorus.isHaken());
    EXPECT_TRUE(bundle.isHaken());

    // The Hakenness test always returns false for reducible manifolds.
    EXPECT_FALSE(Example<3>::lens(0, 1).isHaken());
    {
        Triangulation<3> connSum(sfsSphere);
        connSum.connectedSumWith(bundle);
        EXPECT_FALSE(connSum.isHaken());
    }
}

static bool hasIncompressibleSurface(const Triangulation<3>& tri) {
    NormalSurfaces s(tri, regina::NS_STANDARD, regina::NS_EMBEDDED_ONLY);
    for (const auto& f : s)
        if (f.isIncompressible())
            return true;
    return false;
}

TEST(IncompressibleTest, isIncompressible) {
    // Simple cases;
    EXPECT_FALSE(hasIncompressibleSurface(Example<3>::threeSphere()));

    // RP^3 should have no incompressible surface, since isIncompressible()
    // always works with the double cover of a 1-sided surface.
    EXPECT_FALSE(hasIncompressibleSurface(Example<3>::lens(2, 1)));
    EXPECT_FALSE(hasIncompressibleSurface(Example<3>::layeredLoop(2, false)));

    // Some other non-Haken manifolds:
    EXPECT_FALSE(hasIncompressibleSurface(Example<3>::poincare()));

    // Some Haken Seifert fibred spaces and surface bundles:
    EXPECT_TRUE(hasIncompressibleSurface(Triangulation<3>::fromIsoSig(
        "gLALQbccefffemkbemi"))); // SFS [S2: (2,1) (2,1) (2,1) (2,-1)]
    EXPECT_TRUE(hasIncompressibleSurface(Triangulation<3>::fromIsoSig(
        "gvLQQcdefeffnwnpkhe"))); // SFS [T: (1,1)]
    EXPECT_TRUE(hasIncompressibleSurface(Triangulation<3>::fromIsoSig(
        "gvLQQedfedffrwawrhh"))); // T x S1
}

TEST(IncompressibleTest, hasCompressingDisc) {
    // Trivial cases:
    EXPECT_FALSE(Triangulation<3>().hasCompressingDisc());

    // Balls:
    EXPECT_FALSE(Example<3>::ball().hasCompressingDisc());
    // ... a snapped tetrahedron:
    EXPECT_FALSE(Triangulation<3>::fromGluings(1, {
        { 0, 0, 0, {3,1,2,0} }}).hasCompressingDisc());
    // ... a triangular pillow:
    EXPECT_FALSE(Triangulation<3>::fromGluings(2, {
        { 0, 0, 1, {} },
        { 0, 1, 1, {} },
        { 0, 2, 1, {} }}).hasCompressingDisc());
    // ... a 4-tetrahedron ball:
    EXPECT_FALSE(Triangulation<3>::fromGluings(4, {
        { 0, 2, 0, {0,2} },
        { 0, 1, 1, {2,0,1,3} },
        { 1, 2, 2, {} },
        { 1, 1, 2, {2,0,1,3} },
        { 2, 1, 3, {2,0,1,3} },
        { 3, 2, 3, {1,2} }}).hasCompressingDisc());

    // Orientable handlebodies:
    EXPECT_TRUE(Example<3>::lst(1, 2).hasCompressingDisc());
    EXPECT_TRUE(Example<3>::lst(3, 4).hasCompressingDisc());
    EXPECT_TRUE(Triangulation<3>::fromIsoSig(
        "eHucabdhs").hasCompressingDisc()); // Genus 2
    EXPECT_TRUE(Triangulation<3>::fromIsoSig(
        "tbLGburuGuqHbKgqGacdjmpqsrqbkltl").hasCompressingDisc()); // Genus 7

    // Hyperbolic manifolds:
    {
        Triangulation<3> tri = Example<3>::figureEight();
        tri.idealToFinite();
        tri.intelligentSimplify();
        EXPECT_FALSE(tri.hasCompressingDisc());
    }
}
