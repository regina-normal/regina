
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 2020, Robert C. Haraway, III.                           *
 *  For further details contact Robert Haraway (bobbycyiii@fastmail.com). *
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

#include <cppunit/extensions/HelperMacros.h>
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"
#include "testsuite/dim3/testtriangulation.h"


using regina::Example;
using regina::NormalSurfaces;
using regina::Triangulation;

class FaultTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FaultTest);

    CPPUNIT_TEST(separates);
    CPPUNIT_TEST(isEssentialSphere);
    CPPUNIT_TEST(isEssentialTorus);
    CPPUNIT_TEST(isSolidTorusAnnulus);
    
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
    }

    void tearDown() {
    }

    Triangulation<3>* verifyAllSeparating(Triangulation<3>* tri, 
            const std::string& triName) {
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, regina::NS_STANDARD, regina::NS_EMBEDDED_ONLY);
        bool nonseparating_known = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (!(s->surface(i)->separates())){
                nonseparating_known = true;
                break;
            }
        delete s;
        if (nonseparating_known)
            CPPUNIT_FAIL(("A surface in " + triName +
                          " is computed to be nonseparating.").c_str());
        return tri;
    }

    Triangulation<3>* verifyHasNonSeparating(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, regina::NS_STANDARD, regina::NS_EMBEDDED_ONLY);
        bool all_known_surfaces_separating = true;
        for (unsigned i = 0; i < s->size(); ++i)
            if (!(s->surface(i)->separates())){
                all_known_surfaces_separating = false;
                break;
            }
        delete s;
        if (all_known_surfaces_separating)
            CPPUNIT_FAIL(("No surfaces in " + triName +
                          " were computed to be nonseparating.").c_str());
        return tri;
    }

    void separates() {
        Triangulation<3>* tri;

        // Manifolds without nonseparating surfaces

        tri = Example<3>::threeSphere();
        delete verifyAllSeparating(tri, "Minimal 3-sphere");

        tri = Example<3>::simplicialSphere();
        delete verifyAllSeparating(tri, "Pentachoron boundary 3-sphere");

        tri = Example<3>::ball();
        delete verifyAllSeparating(tri, "One-tetrahedron ball");

        int p = 3;
        int q = 2;
        while (p < 1000){
            if (p % 2 != 0){
                tri = Example<3>::lens(p,q);
                delete verifyAllSeparating(tri, "Lens space with odd p");
            }
            p = p + q;
            q = p - q;
        }
        
        tri = Example<3>::poincareHomologySphere();
        delete verifyAllSeparating(tri, "Poincare homology sphere");

        tri = Example<3>::weeks();
        delete verifyAllSeparating(tri, "Weeks-Matveev-Fomenko manifold");


        // Manifolds with nonseparating surfaces
        
        tri = Example<3>::s2xs1();
        delete verifyHasNonSeparating(tri, "S2xS1");

        tri = Example<3>::rp2xs1();
        delete verifyHasNonSeparating(tri, "RP2xS1");

        tri = Example<3>::rp3rp3();
        delete verifyHasNonSeparating(tri, "RP3#RP3");

        tri = Example<3>::smallClosedNonOrblHyperbolic();
        delete verifyHasNonSeparating(tri, "Smallest known closed nonorientable hyperbolic");
        
        p = 3;
        q = 2;
        while (p < 1000){
            tri = Example<3>::lst(p,q);
            delete verifyHasNonSeparating(tri, "Solid torus");
            if (p % 2 == 0){
                tri = Example<3>::lens(p,q);
                delete verifyHasNonSeparating(tri, "Lens space with even p");
            }
            p = p + q;
            q = p - q;
        }

        tri = Example<3>::solidKleinBottle();
        delete verifyHasNonSeparating(tri, "Solid Klein bottle");

        tri = Example<3>::figureEight();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasNonSeparating(tri, "Figure eight");

        tri = Example<3>::trefoil();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasNonSeparating(tri, "Trefoil");

        tri = Example<3>::whiteheadLink();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasNonSeparating(tri, "Whitehead link");
        
        tri = Example<3>::gieseking();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasNonSeparating(tri, "Gieseking manifold");

        tri = Example<3>::cuspedGenusTwoTorus();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasNonSeparating(tri, "Genus two surface x I");
    }

    void isEssentialSphere() {
        CPPUNIT_FAIL("Not implemented yet");
    }

    void isEssentialTorus() {
        CPPUNIT_FAIL("Not implemented yet");
    }

    void isSolidTorusAnnulus() {
        CPPUNIT_FAIL("Not implemented yet");
    }

};

void addFaultFinding(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FaultTest::suite());
}
