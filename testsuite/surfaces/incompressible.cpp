
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <cmath>
#include <memory>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NExampleTriangulation;
using regina::NNormalSurface;
using regina::NNormalSurfaceList;
using regina::NPerm4;
using regina::NTetrahedron;
using regina::NTriangulation;

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

        void verifyNotIncompressible(const NNormalSurface* s,
                const std::string& triName) {
            if (s->isIncompressible()) {
                CPPUNIT_FAIL(("A compressible surface in " + triName +
                    " is recognised as incompressible.").c_str());
            }
        }

        NTriangulation* verifyAllNotIncompressible(NTriangulation* tri,
                const std::string& triName) {
            NNormalSurfaceList* s = NNormalSurfaceList::enumerate(tri,
                NNormalSurfaceList::STANDARD, true);
            for (unsigned i = 0; i < s->getNumberOfSurfaces(); ++i)
                verifyNotIncompressible(s->getSurface(i), triName);
            delete s;
            return tri;
        }

        NTriangulation* verifyHasIncompressible(NTriangulation* tri,
                const std::string& triName) {
            NNormalSurfaceList* s = NNormalSurfaceList::enumerate(tri,
                NNormalSurfaceList::STANDARD, true);
            for (unsigned i = 0; i < s->getNumberOfSurfaces(); ++i)
                if (s->getSurface(i)->isIncompressible()) {
                    delete s;
                    return tri;
                }

            delete s;
            CPPUNIT_FAIL(("The incompressible surface was not found in "
                + triName + ".").c_str());
            return tri;
        }

        void isIncompressible() {
            NTriangulation* tri;

            // Try the 3-sphere (no incompressible surfaces).
            tri = NExampleTriangulation::threeSphere();
            delete verifyAllNotIncompressible(tri, "3-sphere");

            // Try RP3 (no incompressible surfaces, since we
            // always work with the double cover of a 1-sided surface).
            tri = new NTriangulation();
            tri->insertLayeredLensSpace(2, 1);
            delete verifyAllNotIncompressible(tri, "RP3 (1 vtx)");

            tri = new NTriangulation();
            tri->insertLayeredLoop(2, false);
            delete verifyAllNotIncompressible(tri, "RP3 (2 vtx)");

            // Try some other non-Haken manifolds.
            tri = NExampleTriangulation::poincareHomologySphere();
            delete verifyAllNotIncompressible(tri,
                "Poincare homology sphere");

            // Try some SFSs that should be Haken.
            tri = NTriangulation::fromIsoSig("gLALQbccefffemkbemi");
            delete verifyHasIncompressible(tri,
                "SFS [S2: (2,1) (2,1) (2,1) (2,-1)]");

            tri = NTriangulation::fromIsoSig("gvLQQedfedffrwawrhh");
            delete verifyHasIncompressible(tri, "T x S1");

            tri = NTriangulation::fromIsoSig("gvLQQcdefeffnwnpkhe");
            delete verifyHasIncompressible(tri, "SFS [T: (1,1)]");
        }

        NTriangulation* verifyHasCompressingDisc(NTriangulation* t,
                const std::string& triName) {
            if (! t->hasCompressingDisc()) {
                CPPUNIT_FAIL(("The compressing disc was not found in "
                    + triName + ".").c_str());
            }
            return t;
        }

        NTriangulation* verifyNoCompressingDisc(NTriangulation* t,
                const std::string& triName) {
            if (t->hasCompressingDisc()) {
                CPPUNIT_FAIL(("A compressing disc was found in "
                    + triName + " even though none should exist.").c_str());
            }
            return t;
        }

        void hasCompressingDisc() {
            NTriangulation* tri;
            NTetrahedron* tet[4];
            
            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 2);
            delete verifyHasCompressingDisc(tri, "LST(1,2,3)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(3, 4);
            delete verifyHasCompressingDisc(tri, "LST(3,4,7)");

            tri = new NTriangulation();
            delete verifyNoCompressingDisc(tri, "Empty triangulation");

            tri = new NTriangulation();
            tri->newTetrahedron();
            delete verifyNoCompressingDisc(tri, "Standalone tetrahedron");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[1], NPerm4());
            tet[0]->joinTo(1, tet[1], NPerm4());
            tet[0]->joinTo(2, tet[1], NPerm4());
            delete verifyNoCompressingDisc(tri, "Triangular pillow");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[0], NPerm4(3, 1, 2, 0));
            delete verifyNoCompressingDisc(tri, "Snapped tetrahedron");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[2] = tri->newTetrahedron();
            tet[3] = tri->newTetrahedron();
            tet[0]->joinTo(2, tet[0], NPerm4(0,2));
            tet[0]->joinTo(1, tet[1], NPerm4(2,0,1,3));
            tet[1]->joinTo(2, tet[2], NPerm4());
            tet[1]->joinTo(1, tet[2], NPerm4(2,0,1,3));
            tet[2]->joinTo(1, tet[3], NPerm4(2,0,1,3));
            tet[3]->joinTo(2, tet[3], NPerm4(1,2));
            delete verifyNoCompressingDisc(tri, "4-tetrahedron ball");

            tri = NExampleTriangulation::figureEightKnotComplement();
            tri->idealToFinite();
            tri->intelligentSimplify();
            delete verifyNoCompressingDisc(tri, "Figure 8 Knot Complement");

            tri = NTriangulation::fromIsoSig("eHucabdhs");
            delete verifyHasCompressingDisc(tri, "Solid genus two torus");

            tri = NTriangulation::fromIsoSig(
                "tbLGburuGuqHbKgqGacdjmpqsrqbkltl");
            delete verifyHasCompressingDisc(tri, "Solid genus seven torus");
        }
};

void addIncompressible(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(IncompressibleTest::suite());
}

