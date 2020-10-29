
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
#include <sstream>
#include "manifold/sfs.h"
#include "surfaces/normalcoords.h"
#include "surfaces/normalflags.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"
#include "testsuite/dim3/testtriangulation.h"


using regina::Example;
using regina::NormalListFlags;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::NS_EMBEDDED_ONLY;
using regina::NS_FUNDAMENTAL;
using regina::NS_QUAD;
using regina::NS_VERTEX;
using regina::SFSFibre;
using regina::SFSpace;
using regina::Triangulation;

using std::ostringstream;

NormalListFlags NS_VERTEX_EMBEDDED = (NormalListFlags)(NS_VERTEX & NS_EMBEDDED_ONLY);

class FaultTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FaultTest);

    CPPUNIT_TEST(separates);
    CPPUNIT_TEST(isEssentialSphere);
    CPPUNIT_TEST(isEssentialKleinBottle);
    CPPUNIT_TEST(isEssentialTorus);
    CPPUNIT_TEST(isSolidTorusAnnulus);
    
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
    }

    void tearDown() {
    }

    
    Triangulation<3>* verifyAllVertexSeparating(Triangulation<3>* tri, 
            const std::string& triName) {
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
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

    Triangulation<3>* verifyHasVertexNonSeparating(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
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

    /*
    Triangulation<3>* verifyAllFundamentalSeparating(Triangulation<3>* tri,
            const std::string& triName) {
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
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
    */

    Triangulation<3>* verifyHasFundamentalNonSeparating(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_FUNDAMENTAL);
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

    /**
     * All we can conclude from S not being a vertex surface
     * is that there is some equation of the form
     *
     *     nS + T = X + Y
     *
     * for some natural n > 0, trivial surface T, and normal surfaces X, Y.
     * Even if there is a nonseparating surface,
     * we cannot conclude that some vertex of its support is also nonseparating,
     * because the above n might be even, in which case nS is separating.
     * So if there is a nonseparating surface in a triangulation,
     * nevertheless there might not be a nonseparating vertex-normal such surface.
     *
     * For example, L(34,13) has H_2(L(34,13),Z_2) = Z_2.
     * So L(34,13) admits a nonseparating surface.
     * But fLAMcbcbdeehxwqhr = L(34,13) has no nonseparating quad-vertex surface.
     *
     * However, there must be a nonseparating fundamental normal such surface.
     * For instance, in fLAMcbcbdeehxwqhr there is a fundamental P2#P2#P2.
     *
     * Thus we should have backup tests for manifolds
     * with no Z second homology but with Z_2 second homology.
     */
    
    void separates() {
        Triangulation<3>* tri;

        // Manifolds without nonseparating surfaces

        tri = Example<3>::threeSphere();
        delete verifyAllVertexSeparating(tri, "Minimal 3-sphere");

        tri = Example<3>::simplicialSphere();
        delete verifyAllVertexSeparating(tri, "Pentachoron boundary 3-sphere");

        tri = Example<3>::ball();
        delete verifyAllVertexSeparating(tri, "One-tetrahedron ball");

        int p = 3;
        int q = 2;
        while (p <= 34){
            if (p % 2 != 0){
                tri = Example<3>::lens(p,q);
                ostringstream oss;
                oss << "L(" << p << "," << q << ")" ;
                delete verifyAllVertexSeparating(tri, oss.str());
            }
            p = p + q;
            q = p - q;
        }
        
        tri = Example<3>::poincareHomologySphere();
        delete verifyAllVertexSeparating(tri, "Poincare homology sphere");

        tri = Example<3>::weeks();
        delete verifyAllVertexSeparating(tri, "Weeks-Matveev-Fomenko manifold");

        // Manifolds with nonseparating surfaces
        
        tri = Example<3>::s2xs1();
        delete verifyHasVertexNonSeparating(tri, "S2xS1");

        tri = Example<3>::rp2xs1();
        delete verifyHasVertexNonSeparating(tri, "RP2xS1");

        tri = Example<3>::rp3rp3();
        delete verifyHasVertexNonSeparating(tri, "RP3#RP3");

        tri = Example<3>::smallClosedNonOrblHyperbolic();
        delete verifyHasVertexNonSeparating(tri, "Smallest known closed nonorientable hyperbolic");
        
        p = 3;
        q = 2;
        while (p <= 34){
            tri = Example<3>::lst(p,q);
            delete verifyHasVertexNonSeparating(tri, "Solid torus");
            if (p % 2 == 0){
                tri = Example<3>::lens(p,q);
                ostringstream oss;
                oss << "L(" << p << "," << q << ")" ;
                delete verifyHasFundamentalNonSeparating(tri, oss.str());
            }
            p = p + q;
            q = p - q;
        }

        tri = Example<3>::solidKleinBottle();
        delete verifyHasVertexNonSeparating(tri, "Solid Klein bottle");

        tri = Example<3>::figureEight();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexNonSeparating(tri, "Figure eight");

        tri = Example<3>::trefoil();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexNonSeparating(tri, "Trefoil");

        tri = Example<3>::whiteheadLink();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexNonSeparating(tri, "Whitehead link");
        
        tri = Example<3>::gieseking();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexNonSeparating(tri, "Gieseking manifold");

        tri = Example<3>::cuspedGenusTwoTorus();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexNonSeparating(tri, "Genus two surface x I");
    }

    
    Triangulation<3>* verifyAllVertexSpheresTrivial(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_sphere = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (s->surface(i)->isEssentialSphere()){
                found_essential_sphere = true;
                break;
            }
        delete s;
        if (found_essential_sphere)
            CPPUNIT_FAIL(("The irreducible manifold " + triName +
                          " was computed to have an essential sphere.").c_str());
        return tri;
    }

    Triangulation<3>* verifyHasVertexEssentialSphere(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_sphere = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (s->surface(i)->isEssentialSphere()){
                found_essential_sphere = true;
                break;
            }
        delete s;
        if (!found_essential_sphere)
            CPPUNIT_FAIL(("No vertex essential spheres were found in " +
                          triName + ".").c_str());
        return tri;
    }

    
    void isEssentialSphere() {
        Triangulation<3>* tri;

        // Closed irreducible manifolds
        tri = Example<3>::threeSphere();
        delete verifyAllVertexSpheresTrivial(tri, "Minimal 3-sphere");
        tri = Example<3>::simplicialSphere();
        delete verifyAllVertexSpheresTrivial(tri, "Pentachoron boundary 3-sphere");
        tri = Example<3>::poincareHomologySphere();
        delete verifyAllVertexSpheresTrivial(tri, "Poincare homology sphere");
        tri = Example<3>::weeks();
        delete verifyAllVertexSpheresTrivial(tri, "Weeks-Matveev-Fomenko manifold");
        int p = 3;
        int q = 2;
        while (p < 1000){
            tri = Example<3>::lens(p,q);
            ostringstream oss;
            oss << "L(" << p << "," << q << ")";
            delete verifyAllVertexSpheresTrivial(tri, oss.str());
            p = p + q;
            q = p - q;
        }

        // Bounded irreducible manifolds
        tri = Example<3>::ball();
        delete verifyAllVertexSpheresTrivial(tri, "One tetrahedron ball");
        tri = Example<3>::cuspedGenusTwoTorus();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(
            tri, "Trivial I-bundle over genus two surface");
        tri = Example<3>::figureEight();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(tri, "Figure eight knot exterior");
        tri = Example<3>::trefoil();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(tri, "Trefoil knot exterior");
        tri = Example<3>::whiteheadLink();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(tri, "Whitehead link exterior");
        tri = Example<3>::gieseking();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(tri, "Gieseking manifold");
        tri = Example<3>::solidKleinBottle();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexSpheresTrivial(tri, "Solid Klein bottle");
        p = 3;
        q = 2;
        while (p < 1000){
            tri = Example<3>::lst(p,q);
            delete verifyAllVertexSpheresTrivial(tri, "Solid torus");
            p = p + q;
            q = p - q;
        }

        // The prime reducible manifold S^2 x S^1
        tri = Example<3>::s2xs1();
        delete verifyHasVertexEssentialSphere(tri, "S2xS1");
        
        // RP3#RP3 has a reducing sphere,
        // but in the given triangulation, there is no fundamental reducing sphere.
        // Instead, there are two fundamental embedded one-sided RP2s
        // that double to reducing spheres.

        tri = Example<3>::rp3rp3();
        delete verifyAllVertexSpheresTrivial(tri, "RP3#RP3");
        
        p = 3;
        q = 2;
        while (p < 100){
            tri = Example<3>::lens(p,q);
            tri->connectedSumWith(*tri);
            tri->intelligentSimplify();
            ostringstream oss;
            oss << "L(" << p << "," << q << ")";
            delete verifyHasVertexEssentialSphere(tri, oss.str() + " # " + oss.str());
            p = p + q;
            q = p - q;
        }
    }
    
    Triangulation<3>* verifyAllVertexKleinBottlesInessential(Triangulation<3>* tri, 
            const std::string& triName){
        Triangulation<3>* working = new Triangulation<3>(tri->isoSig());
        NormalSurfaces* s = NormalSurfaces::enumerate(
            working, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_klein_bottle = false;
        unsigned i = 0;
        for (; i < s->size(); ++i){
            const NormalSurface* surf = s->surface(i);
            if (surf->isEssentialKleinBottle()){
                found_essential_klein_bottle = true;
                break;
            }
        }
        delete s;
        if (found_essential_klein_bottle)
            CPPUNIT_FAIL((triName +
                          " has an \"essential Klein bottle.\"").c_str());
        return tri;
    }
    
    Triangulation<3>* verifyHasVertexEssentialKleinBottle(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_klein_bottle = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (s->surface(i)->isEssentialKleinBottle()){
                found_essential_klein_bottle = true;
                break;
            }
        delete s;
        if (!found_essential_klein_bottle)
            CPPUNIT_FAIL(("No vertex essential Klein bottle found in "
                          + triName + ".").c_str());
        return tri;
    }

    
    Triangulation<3>* smallSFS(long alpha0, long beta0,
                   long alpha1, long beta1,
                   long alpha2, long beta2){
        const SFSFibre* fiber0 = new SFSFibre(alpha0, beta0);
        const SFSFibre* fiber1 = new SFSFibre(alpha1, beta1);
        const SFSFibre* fiber2 = new SFSFibre(alpha2, beta2);
        SFSpace* sfs = new SFSpace(
            SFSpace::o1, 0, 0, 0, 0, 0);
        sfs->insertFibre(*fiber0);
        sfs->insertFibre(*fiber1);
        sfs->insertFibre(*fiber2);
        Triangulation<3>* sfs_tri = sfs->construct();
        delete sfs;
        delete fiber0;
        delete fiber1;
        delete fiber2;
        return sfs_tri;
    }

    void isEssentialKleinBottle() {
        Triangulation<3>* tri;

        // Manifolds with no essential Klein bottles
        
        tri = Example<3>::threeSphere();
        delete verifyAllVertexKleinBottlesInessential(tri, "Minimal 3-sphere");

        tri = Example<3>::simplicialSphere();
        delete verifyAllVertexKleinBottlesInessential(tri, "Pentachoron boundary 3-sphere");

        tri = Example<3>::ball();
        delete verifyAllVertexKleinBottlesInessential(tri, "One-tetrahedron ball");

        int p = 3;
        int q = 2;
        ostringstream oss;
        while (p <= 34){
            tri = Example<3>::lst(p,q);
            delete verifyAllVertexKleinBottlesInessential(tri, "Solid torus");

            tri = Example<3>::lens(p,q);
            oss << "L(" << p << "," << q << ")" ;
            delete verifyAllVertexKleinBottlesInessential(tri, oss.str());
            oss.clear();
            oss.str("");

            tri = smallSFS(2,1,2,1,p,-q);
            oss << "SFS [S2, (2,1,2,1," << p << "," << q << ")]";
            delete verifyAllVertexKleinBottlesInessential(tri, oss.str() );
            oss.clear();
            oss.str("");

            p = p + q;
            q = p - q;
        }
        
        tri = Example<3>::poincareHomologySphere();
        delete verifyAllVertexKleinBottlesInessential(tri, "Poincare homology sphere");

        tri = Example<3>::weeks();
        delete verifyAllVertexKleinBottlesInessential(tri, "Weeks-Matveev-Fomenko manifold");

        tri = Example<3>::s2xs1();
        delete verifyAllVertexKleinBottlesInessential(tri, "S2xS1");

        tri = Example<3>::rp2xs1();
        delete verifyAllVertexKleinBottlesInessential(tri, "RP2xS1");

        tri = Example<3>::rp3rp3();
        delete verifyAllVertexKleinBottlesInessential(tri, "RP3#RP3");

        tri = Example<3>::smallClosedNonOrblHyperbolic();
        delete verifyAllVertexKleinBottlesInessential(tri, "Smallest known closed nonorientable hyperbolic");

        tri = Example<3>::solidKleinBottle();
        delete verifyAllVertexKleinBottlesInessential(tri, "Solid Klein bottle");

        tri = Example<3>::figureEight();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexKleinBottlesInessential(tri, "Figure eight");

        tri = Example<3>::trefoil();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexKleinBottlesInessential(tri, "Trefoil");

        tri = Example<3>::whiteheadLink();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexKleinBottlesInessential(tri, "Whitehead link");
        
        tri = Example<3>::gieseking();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexKleinBottlesInessential(tri, "Gieseking manifold");

        tri = Example<3>::cuspedGenusTwoTorus();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexKleinBottlesInessential(tri, "Genus two surface x I");

        // Manifolds with essential Klein bottles

        tri = new Triangulation<3>("gLALQbcceffflpkktua");
        delete verifyHasVertexEssentialKleinBottle(tri, "Doubled twisted I-bundle over K2");

        tri = new Triangulation<3>("oLLLAAzLMQccefffggjlmnmmnnkkwawpwwjraakru");
        delete verifyHasVertexEssentialKleinBottle(tri, "Doubled Gieseking manifold");
    }
    
    Triangulation<3>* verifyAllVertexToriInessential(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_torus = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (s->surface(i)->isEssentialTorus()){
                found_essential_torus = true;
                break;
            }
        delete s;
        if (found_essential_torus)
            CPPUNIT_FAIL(("The atoroidal manifold " + triName +
                          " was computed to have an essential torus.").c_str());
        return tri;
    }

    Triangulation<3>* verifyHasVertexEssentialTorus(Triangulation<3>* tri, 
            const std::string& triName){
        NormalSurfaces* s = NormalSurfaces::enumerate(
            tri, NS_QUAD, NS_VERTEX_EMBEDDED);
        bool found_essential_torus = false;
        for (unsigned i = 0; i < s->size(); ++i)
            if (s->surface(i)->isEssentialTorus()){
                found_essential_torus = true;
                break;
            }
        delete s;
        if (!found_essential_torus)
            CPPUNIT_FAIL(("No essential tori were found in the toroidal manifold " +
                          triName + ".").c_str());
        return tri;
    }
    
    void isEssentialTorus() {
        Triangulation<3>* tri;

        // Simple closed atoroidal manifolds
        tri = Example<3>::threeSphere();
        delete verifyAllVertexToriInessential(tri, "Minimal 3-sphere");
        tri = Example<3>::simplicialSphere();
        delete verifyAllVertexToriInessential(tri, "Pentachoron boundary 3-sphere");
        tri = Example<3>::poincareHomologySphere();
        delete verifyAllVertexToriInessential(tri, "Poincare homology sphere");
        tri = Example<3>::weeks();
        delete verifyAllVertexToriInessential(tri, "Weeks-Matveev-Fomenko manifold");
        int p = 3;
        int q = 2;
        while (p < 1000){
            tri = Example<3>::lens(p,q);
            delete verifyAllVertexToriInessential(tri, "Fibonacci lens space");
            tri = smallSFS(2,1,2,1,p,-q);
            ostringstream oss;
            oss << "SFS [S2: (2,1)(2,1)(" << p << "," << -q << ")]";
            delete verifyAllVertexToriInessential(tri, oss.str());
            p = p + q;
            q = p - q;
        }

        // Small Seifert fibered spaces...
#define VERIFY_SMALL_SFS_ATOROIDAL(a0,b0,a1,b1,a2,b2) \
        tri = smallSFS(a0,b0,a1,b1,a2,b2);           \
        tri->intelligentSimplify();               \
        delete verifyAllVertexToriInessential(tri, "SFS [S2: (a0,b0) (a1,b1) (a2,b2)]")

        // ... of positive orbifold Euler characteristic ...
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,3,-2);
        
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,3,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,4,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,5,-4);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,3,-1);

        /*
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,3,1);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,3,2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,4,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,5,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,3,2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,4,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,4,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,5,-2);
        */

        // ...and of negative orbifold Euler characteristic....
        /*
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,7,-6);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,7,-5);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,7,-5);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,7,-4);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,7,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,8,-5);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,1,8,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,7,-5);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,7,-4);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,7,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,7,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,8,-5);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,8,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,3,2,8,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,4,1,5,-4);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,4,1,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,4,1,5,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,4,3,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,4,3,5,-2);
        */
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,5,2,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,5,2,5,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(2,1,5,3,5,-2);

        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,1,4,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,1,4,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,1,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,1,5,-2);
        /*
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,2,4,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,2,4,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,2,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,1,3,2,5,-2);
        VERIFY_SMALL_SFS_ATOROIDAL(3,2,3,2,4,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,2,3,2,4,-1);
        VERIFY_SMALL_SFS_ATOROIDAL(3,2,3,2,5,-3);
        VERIFY_SMALL_SFS_ATOROIDAL(3,2,3,2,5,-2);
        */
        
        // Bounded atoroidal manifolds
        tri = Example<3>::ball();
        delete verifyAllVertexToriInessential(tri, "One tetrahedron ball");
        tri = Example<3>::cuspedGenusTwoTorus();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(
            tri, "Trivial I-bundle over genus two surface");
        tri = Example<3>::figureEight();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(tri, "Figure eight knot exterior");
        tri = Example<3>::trefoil();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(tri, "Trefoil knot exterior");
        tri = Example<3>::whiteheadLink();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(tri, "Whitehead link exterior");
        tri = Example<3>::gieseking();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(tri, "Gieseking manifold");
        tri = Example<3>::solidKleinBottle();
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyAllVertexToriInessential(tri, "Solid Klein bottle");
        p = 3;
        q = 2;
        while (p < 1000){
            tri = Example<3>::lst(p,q);
            delete verifyAllVertexToriInessential(tri, "Solid torus");
            p = p + q;
            q = p - q;
        }

        // Toroidal manifolds
        tri = new Triangulation<3>("uLLvPAPAzzvQPQccdeghiihjjlmqspstrstthshgbvrndhakecbcqvndm");
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexEssentialTorus(tri, "Doubled figure eight knot exterior");

        tri = new Triangulation<3>("iLALzQcbccefhgghlpkkucjjs");
        tri->idealToFinite();
        tri->intelligentSimplify();
        delete verifyHasVertexEssentialTorus(tri, "Doubled trefoil knot exterior");
    }

    void isSolidTorusAnnulus() {
        CPPUNIT_FAIL("Not implemented yet");
    }

};

void addFaultFinding(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FaultTest::suite());
}
