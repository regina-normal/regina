
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"
#include "testsuite/dim3/testdim3.h"

using regina::Example;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Perm;
using regina::Tetrahedron;
using regina::Triangulation;

class IncompressibleTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(IncompressibleTest);

    CPPUNIT_TEST(isIncompressible);
    CPPUNIT_TEST(hasCompressingDisc);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void verifyNotIncompressible(const NormalSurface& s,
                const std::string& triName) {
            if (s.isIncompressible()) {
                CPPUNIT_FAIL(("A compressible surface in " + triName +
                    " is recognised as incompressible.").c_str());
            }
        }

        void verifyAllNotIncompressible(const Triangulation<3>& tri,
                const std::string& triName) {
            for (const NormalSurface& f :
                    NormalSurfaces(tri, regina::NS_STANDARD,
                    regina::NS_EMBEDDED_ONLY))
                verifyNotIncompressible(f, triName);
        }

        void verifyHasIncompressible(const Triangulation<3>& tri,
                const std::string& triName) {
            NormalSurfaces s(tri, regina::NS_STANDARD,
                regina::NS_EMBEDDED_ONLY);
            for (const NormalSurface& f : s)
                if (f.isIncompressible())
                    return;

            CPPUNIT_FAIL(("The incompressible surface was not found in "
                + triName + ".").c_str());
        }

        void isIncompressible() {
            // Try the 3-sphere (no incompressible surfaces).
            verifyAllNotIncompressible(Example<3>::threeSphere(), "3-sphere");

            // Try RP3 (no incompressible surfaces, since we
            // always work with the double cover of a 1-sided surface).
            {
                Triangulation<3> tri = Example<3>::lens(2, 1);
                verifyAllNotIncompressible(tri, "RP3 (1 vtx)");
            }

            {
                Triangulation<3> tri = Example<3>::layeredLoop(2, false);
                verifyAllNotIncompressible(tri, "RP3 (2 vtx)");
            }

            // Try some other non-Haken manifolds.
            verifyAllNotIncompressible(Example<3>::poincare(),
                "Poincare homology sphere");

            // Try some SFSs that should be Haken.
            verifyHasIncompressible(
                Triangulation<3>::fromIsoSig("gLALQbccefffemkbemi"),
                "SFS [S2: (2,1) (2,1) (2,1) (2,-1)]");

            verifyHasIncompressible(
                Triangulation<3>::fromIsoSig("gvLQQedfedffrwawrhh"),
                "T x S1");

            verifyHasIncompressible(
                Triangulation<3>::fromIsoSig("gvLQQcdefeffnwnpkhe"),
                "SFS [T: (1,1)]");
        }

        void verifyHasCompressingDisc(const Triangulation<3>& t,
                const std::string& triName) {
            if (! t.hasCompressingDisc()) {
                CPPUNIT_FAIL(("The compressing disc was not found in "
                    + triName + ".").c_str());
            }
        }

        void verifyNoCompressingDisc(const Triangulation<3>& t,
                const std::string& triName) {
            if (t.hasCompressingDisc()) {
                CPPUNIT_FAIL(("A compressing disc was found in "
                    + triName + " even though none should exist.").c_str());
            }
        }

        void hasCompressingDisc() {
            verifyHasCompressingDisc(Example<3>::lst(1, 2), "LST(1,2,3)");
            verifyHasCompressingDisc(Example<3>::lst(3, 4), "LST(3,4,7)");

            {
                Triangulation<3> tri;
                verifyNoCompressingDisc(tri, "Empty triangulation");
            }

            {
                Triangulation<3> tri;
                tri.newTetrahedron();
                verifyNoCompressingDisc(tri, "Standalone tetrahedron");
            }

            {
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<2>();
                tet[0]->join(0, tet[1], Perm<4>());
                tet[0]->join(1, tet[1], Perm<4>());
                tet[0]->join(2, tet[1], Perm<4>());
                verifyNoCompressingDisc(tri, "Triangular pillow");
            }

            {
                Triangulation<3> tri;
                auto tet = tri.newTetrahedron();
                tet->join(0, tet, Perm<4>(3, 1, 2, 0));
                verifyNoCompressingDisc(tri, "Snapped tetrahedron");
            }

            {
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<4>();
                tet[0]->join(2, tet[0], Perm<4>(0,2));
                tet[0]->join(1, tet[1], Perm<4>(2,0,1,3));
                tet[1]->join(2, tet[2], Perm<4>());
                tet[1]->join(1, tet[2], Perm<4>(2,0,1,3));
                tet[2]->join(1, tet[3], Perm<4>(2,0,1,3));
                tet[3]->join(2, tet[3], Perm<4>(1,2));
                verifyNoCompressingDisc(tri, "4-tetrahedron ball");
            }

            {
                Triangulation<3> f8 = Example<3>::figureEight();
                f8.idealToFinite();
                f8.intelligentSimplify();
                verifyNoCompressingDisc(f8, "Figure 8 Knot Complement");
            }

            verifyHasCompressingDisc(Triangulation<3>::fromIsoSig(
                "eHucabdhs"), "Solid genus two torus");

            verifyHasCompressingDisc(Triangulation<3>::fromIsoSig(
                "tbLGburuGuqHbKgqGacdjmpqsrqbkltl"), "Solid genus seven torus");
        }
};

void addIncompressible(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(IncompressibleTest::suite());
}

