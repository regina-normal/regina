
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"
#include "testsuite/dim3/testtriangulation.h"

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
        void setUp() {
        }

        void tearDown() {
        }

        void verifyNotIncompressible(const NormalSurface* s,
                const std::string& triName) {
            if (s->isIncompressible()) {
                CPPUNIT_FAIL(("A compressible surface in " + triName +
                    " is recognised as incompressible.").c_str());
            }
        }

        Triangulation<3>* verifyAllNotIncompressible(Triangulation<3>* tri,
                const std::string& triName) {
            NormalSurfaces* s = NormalSurfaces::enumerate(tri,
                regina::NS_STANDARD, regina::NS_EMBEDDED_ONLY);
            for (unsigned i = 0; i < s->size(); ++i)
                verifyNotIncompressible(s->surface(i), triName);
            delete s;
            return tri;
        }

        Triangulation<3>* verifyHasIncompressible(Triangulation<3>* tri,
                const std::string& triName) {
            NormalSurfaces* s = NormalSurfaces::enumerate(tri,
                regina::NS_STANDARD, regina::NS_EMBEDDED_ONLY);
            for (unsigned i = 0; i < s->size(); ++i)
                if (s->surface(i)->isIncompressible()) {
                    delete s;
                    return tri;
                }

            delete s;
            CPPUNIT_FAIL(("The incompressible surface was not found in "
                + triName + ".").c_str());
            return tri;
        }

        void isIncompressible() {
            Triangulation<3>* tri;

            // Try the 3-sphere (no incompressible surfaces).
            tri = Example<3>::threeSphere();
            delete verifyAllNotIncompressible(tri, "3-sphere");

            // Try RP3 (no incompressible surfaces, since we
            // always work with the double cover of a 1-sided surface).
            tri = new Triangulation<3>();
            tri->insertLayeredLensSpace(2, 1);
            delete verifyAllNotIncompressible(tri, "RP3 (1 vtx)");

            tri = new Triangulation<3>();
            tri->insertLayeredLoop(2, false);
            delete verifyAllNotIncompressible(tri, "RP3 (2 vtx)");

            // Try some other non-Haken manifolds.
            tri = Example<3>::poincareHomologySphere();
            delete verifyAllNotIncompressible(tri,
                "Poincare homology sphere");

            // Try some SFSs that should be Haken.
            tri = Triangulation<3>::fromIsoSig("gLALQbccefffemkbemi");
            delete verifyHasIncompressible(tri,
                "SFS [S2: (2,1) (2,1) (2,1) (2,-1)]");

            tri = Triangulation<3>::fromIsoSig("gvLQQedfedffrwawrhh");
            delete verifyHasIncompressible(tri, "T x S1");

            tri = Triangulation<3>::fromIsoSig("gvLQQcdefeffnwnpkhe");
            delete verifyHasIncompressible(tri, "SFS [T: (1,1)]");
        }

        Triangulation<3>* verifyHasCompressingDisc(Triangulation<3>* t,
                const std::string& triName) {
            if (! t->hasCompressingDisc()) {
                CPPUNIT_FAIL(("The compressing disc was not found in "
                    + triName + ".").c_str());
            }
            return t;
        }

        Triangulation<3>* verifyNoCompressingDisc(Triangulation<3>* t,
                const std::string& triName) {
            if (t->hasCompressingDisc()) {
                CPPUNIT_FAIL(("A compressing disc was found in "
                    + triName + " even though none should exist.").c_str());
            }
            return t;
        }

        void hasCompressingDisc() {
            Triangulation<3>* tri;
            Tetrahedron<3>* tet[4];
            
            tri = new Triangulation<3>();
            tri->insertLayeredSolidTorus(1, 2);
            delete verifyHasCompressingDisc(tri, "LST(1,2,3)");

            tri = new Triangulation<3>();
            tri->insertLayeredSolidTorus(3, 4);
            delete verifyHasCompressingDisc(tri, "LST(3,4,7)");

            tri = new Triangulation<3>();
            delete verifyNoCompressingDisc(tri, "Empty triangulation");

            tri = new Triangulation<3>();
            tri->newTetrahedron();
            delete verifyNoCompressingDisc(tri, "Standalone tetrahedron");

            tri = new Triangulation<3>();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[0]->join(0, tet[1], Perm<4>());
            tet[0]->join(1, tet[1], Perm<4>());
            tet[0]->join(2, tet[1], Perm<4>());
            delete verifyNoCompressingDisc(tri, "Triangular pillow");

            tri = new Triangulation<3>();
            tet[0] = tri->newTetrahedron();
            tet[0]->join(0, tet[0], Perm<4>(3, 1, 2, 0));
            delete verifyNoCompressingDisc(tri, "Snapped tetrahedron");

            tri = new Triangulation<3>();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[2] = tri->newTetrahedron();
            tet[3] = tri->newTetrahedron();
            tet[0]->join(2, tet[0], Perm<4>(0,2));
            tet[0]->join(1, tet[1], Perm<4>(2,0,1,3));
            tet[1]->join(2, tet[2], Perm<4>());
            tet[1]->join(1, tet[2], Perm<4>(2,0,1,3));
            tet[2]->join(1, tet[3], Perm<4>(2,0,1,3));
            tet[3]->join(2, tet[3], Perm<4>(1,2));
            delete verifyNoCompressingDisc(tri, "4-tetrahedron ball");

            tri = Example<3>::figureEight();
            tri->idealToFinite();
            tri->intelligentSimplify();
            delete verifyNoCompressingDisc(tri, "Figure 8 Knot Complement");

            tri = Triangulation<3>::fromIsoSig("eHucabdhs");
            delete verifyHasCompressingDisc(tri, "Solid genus two torus");

            tri = Triangulation<3>::fromIsoSig(
                "tbLGburuGuqHbKgqGacdjmpqsrqbkltl");
            delete verifyHasCompressingDisc(tri, "Solid genus seven torus");
        }
};

void addIncompressible(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(IncompressibleTest::suite());
}

