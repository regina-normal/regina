
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

#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include "packet/container.h"
#include "split/signature.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/surfaces/testsurfaces.h"

using regina::AbelianGroup;
using regina::BoolSet;
using regina::BoundaryComponent;
using regina::Container;
using regina::Edge;
using regina::Example;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::NormalSurfaceVector;
using regina::Packet;
using regina::Perm;
using regina::Ray;
using regina::Signature;
using regina::Tetrahedron;
using regina::Triangulation;

using regina::NS_STANDARD;
using regina::NS_QUAD;
using regina::NS_AN_STANDARD;
using regina::NS_AN_QUAD_OCT;

using regina::NS_VERTEX;
using regina::NS_FUNDAMENTAL;

using regina::NS_VERTEX_DD;
using regina::NS_VERTEX_TREE;
using regina::NS_VERTEX_STD_DIRECT;
using regina::NS_VERTEX_VIA_REDUCED;

using regina::NS_HILBERT_DUAL;
using regina::NS_HILBERT_PRIMAL;

class NormalSurfacesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NormalSurfacesTest);

    CPPUNIT_TEST(defaultArgs);
    CPPUNIT_TEST(standardEmpty);
    CPPUNIT_TEST(standardOneTet);
    CPPUNIT_TEST(standardGieseking);
    CPPUNIT_TEST(standardFigure8);
    CPPUNIT_TEST(standardS3);
    CPPUNIT_TEST(standardLoopC2);
    CPPUNIT_TEST(standardLoopCtw3);
    CPPUNIT_TEST(standardLargeS3);
    CPPUNIT_TEST(standardLargeRP3);
    CPPUNIT_TEST(standardTwistedKxI);
    CPPUNIT_TEST(standardNorSFS);
    CPPUNIT_TEST(quadEmpty);
    CPPUNIT_TEST(quadOneTet);
    CPPUNIT_TEST(quadGieseking);
    CPPUNIT_TEST(quadFigure8);
    CPPUNIT_TEST(quadS3);
    CPPUNIT_TEST(quadLoopC2);
    CPPUNIT_TEST(quadLoopCtw3);
    CPPUNIT_TEST(quadLargeS3);
    CPPUNIT_TEST(quadLargeRP3);
    CPPUNIT_TEST(quadTwistedKxI);
    CPPUNIT_TEST(quadNorSFS);
    CPPUNIT_TEST(almostNormalEmpty);
    CPPUNIT_TEST(almostNormalOneTet);
    CPPUNIT_TEST(almostNormalGieseking);
    CPPUNIT_TEST(almostNormalFigure8);
    CPPUNIT_TEST(almostNormalS3);
    CPPUNIT_TEST(almostNormalLoopC2);
    CPPUNIT_TEST(almostNormalLoopCtw3);
    CPPUNIT_TEST(almostNormalLargeS3);
    CPPUNIT_TEST(almostNormalLargeRP3);
    CPPUNIT_TEST(almostNormalTwistedKxI);
    CPPUNIT_TEST(largeDimensionsStandard);
    CPPUNIT_TEST(largeDimensionsQuad);
    CPPUNIT_TEST(largeDimensionsAlmostNormal);
    CPPUNIT_TEST(standardQuadConversionsConstructed);
    CPPUNIT_TEST(standardQuadConversionsCensus);
    CPPUNIT_TEST(standardANQuadOctConversionsConstructed);
    CPPUNIT_TEST(standardANQuadOctConversionsCensus);
    CPPUNIT_TEST(treeVsDDCensus<NS_QUAD>);
    CPPUNIT_TEST(treeVsDDCensus<NS_STANDARD>);
    CPPUNIT_TEST(treeVsDDCensus<NS_AN_QUAD_OCT>);
    CPPUNIT_TEST(treeVsDDCensus<NS_AN_STANDARD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_QUAD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_STANDARD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_AN_QUAD_OCT>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_AN_STANDARD>);
    CPPUNIT_TEST(disjointConstructed);
    CPPUNIT_TEST(disjointCensus);
    CPPUNIT_TEST(cutAlongConstructed);
    CPPUNIT_TEST(cutAlongCensus);

    CPPUNIT_TEST_SUITE_END();

    private:
        Triangulation<3> empty;
            /**< An empty triangulation. */
        Triangulation<3> oneTet;
            /**< A one-tetrahedron ball. */
        Triangulation<3> figure8;
            /**< The figure eight knot complement. */
        Triangulation<3> gieseking;
            /**< The Gieseking manifold. */
        Triangulation<3> S3;
            /**< A one-tetrahedron two-vertex 3-sphere. */
        Triangulation<3> loopC2;
            /**< An untwisted layered loop of length 2. */
        Triangulation<3> loopCtw3;
            /**< A twisted layered loop of length 3. */
        Triangulation<3> largeS3;
            /**< A 3-vertex 5-tetrahedron triangulation of the 3-sphere. */
        Triangulation<3> largeRP3;
            /**< A 2-vertex 5-tetrahedron triangulation of real
                 projective space. */
        Triangulation<3> twistedKxI;
            /**< A 3-tetrahedron non-orientable twisted I-bundle over the
                 Klein bottle. */
        Triangulation<3> norSFS;
            /**< A 9-tetrahedron triangulation of the space
                 SFS [RP2: (2,1) (2,1) (2,1)].  Specifically, this is
                 triangulation #5 of this space from the non-orientable
                 census as it was shipped with Regina 4.5. */


    public:
        void copyAndDelete(Triangulation<3>& dest, Triangulation<3>* source) {
            dest.insertTriangulation(*source);
            delete source;
        }

        void generateFromSig(Triangulation<3>& tri, const std::string& sigStr) {
            Signature* sig = Signature::parse(sigStr);
            if (sig == 0)
                return;

            Triangulation<3>* triNew = sig->triangulate();
            delete sig;
            if (triNew == 0)
                return;

            tri.insertTriangulation(*triNew);
            delete triNew;
        }

        void setUp() {
            Tetrahedron<3>* r;
            Tetrahedron<3>* s;
            Tetrahedron<3>* t;

            // Some triangulations have no face identifications at all.
            empty.setLabel("Empty");

            oneTet.newTetrahedron();
            oneTet.setLabel("Lone tetrahedron");

            // Use pre-coded triangulations where we can.
            copyAndDelete(figure8,
                Example<3>::figureEight());
            figure8.setLabel("Figure eight knot complement");

            copyAndDelete(gieseking, Example<3>::gieseking());
            gieseking.setLabel("Gieseking manifold");

            // Layered loops can be constructed automatically.
            S3.insertLayeredLoop(1, false);
            S3.setLabel("S3");

            loopC2.insertLayeredLoop(2, false);
            loopC2.setLabel("C(2)");

            loopCtw3.insertLayeredLoop(3, true);
            loopCtw3.setLabel("C~(3)");

            // Some non-minimal triangulations can be generated from
            // splitting surfaces.
            generateFromSig(largeS3, "abcd.abe.c.d.e");
            largeS3.setLabel("Large S3");

            generateFromSig(largeRP3, "aabcd.be.c.d.e");
            largeRP3.setLabel("Large RP3");

            // A 3-tetrahedron non-orientable twisted I-bundle over the
            // Klein bottle is described in Chapter 3 of Burton's PhD thesis.
            r = twistedKxI.newTetrahedron();
            s = twistedKxI.newTetrahedron();
            t = twistedKxI.newTetrahedron();
            r->join(0, s, Perm<4>(0, 1, 2, 3));
            r->join(1, t, Perm<4>(2, 1, 0, 3));
            r->join(2, t, Perm<4>(1, 3, 2, 0));
            s->join(1, t, Perm<4>(0, 3, 2, 1));
            s->join(2, t, Perm<4>(3, 1, 0, 2));
            twistedKxI.setLabel("Twisted KxI");

            // Build the 9-tetrahedron SFS from its dehydration string;
            // obscure but painless at least.
            norSFS.insertRehydration("jnnafaabcfighhihimgbpqpepbr");
            norSFS.setLabel("SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void tearDown() {
        }

        void defaultArgs() {
            Triangulation<3> t(oneTet);
            NormalSurfaces* l;

            // Make sure that calls to enumerate() using default arguments
            // fall through to the correct enumerate() function, which takes
            // NormalFlags and NormalAlg.

            l = NormalSurfaces::enumerate(&t, regina::NS_QUAD);
            if (l->which() != (regina::NS_VERTEX | regina::NS_EMBEDDED_ONLY)) {
                std::ostringstream msg;
                msg << "Enumeration with default (flags, algorithm) gave "
                    "incorrect flags " << l->which().intValue() << ".";
                CPPUNIT_FAIL(msg.str());
            }
            delete l;

            l = NormalSurfaces::enumerate(&t, regina::NS_QUAD,
                regina::NS_IMMERSED_SINGULAR);
            if (l->which() !=
                    (regina::NS_VERTEX | regina::NS_IMMERSED_SINGULAR)) {
                std::ostringstream msg;
                msg << "Enumeration with default algorithm gave "
                    "incorrect flags " << l->which().intValue() << ".";
                CPPUNIT_FAIL(msg.str());
            }
            delete l;
        }

        void testSize(NormalSurfaces* list,
                const char* listType, unsigned long expectedSize) {
            std::ostringstream msg;
            msg << "Number of " << listType << " for "
                << list->triangulation()->label()
                << " should be " << expectedSize << ", not "
                << list->size() << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(),
                list->size() == expectedSize);
        }

        void testSurface(const NormalSurface* surface, const char* triName,
                const char* surfaceName, int euler, bool connected,
                bool orient, bool twoSided, bool compact, bool realBdry,
                bool vertexLink, unsigned edgeLink,
                unsigned long central, bool splitting) {
            // Begin with the compactness test so we know which other
            // tests may be performed.
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be "
                    << (compact ? "compact." : "non-compact.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isCompact() == compact);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have Euler char. " << euler << ", not "
                    << surface->eulerChar() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->eulerChar() == euler);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (connected)
                    msg << "connected.";
                else
                    msg << "disconnected.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isConnected() == connected);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (orient)
                    msg << "orientable.";
                else
                    msg << "non-orientable.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isOrientable() == orient);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (twoSided)
                    msg << "2-sided.";
                else
                    msg << "1-sided.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isTwoSided() == twoSided);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have "
                    << (realBdry ? "real boundary." : "no real boundary.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->hasRealBoundary() == realBdry);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! vertexLink)
                    msg << "not ";
                msg << "be vertex linking.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isVertexLinking() == vertexLink);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (edgeLink == 0)
                    msg << "not be thin edge linking.";
                else if (edgeLink == 1)
                    msg << "be the (thin) link of one edge.";
                else
                    msg << "be the (thin) link of two edges.";

                std::pair<const regina::Edge<3>*, const regina::Edge<3>*> links
                    = surface->isThinEdgeLink();
                unsigned ans;
                if (links.first == 0)
                    ans = 0;
                else if (links.second == 0)
                    ans = 1;
                else
                    ans = 2;

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ans == edgeLink);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (central == 0)
                    msg << "not be a central surface.";
                else
                    msg << "be a central surface with " << central
                        << " disc(s).";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isCentral() == central);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! splitting)
                    msg << "not ";
                msg << "be a splitting surface.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isSplitting() == splitting);
            }
        }

        void countCompactSurfaces(const NormalSurfaces* list,
                const char* surfaceType,
                unsigned long expectedCount, int euler,
                bool connected, bool orient, bool twoSided,
                bool realBdry, bool vertexLink, unsigned edgeLink,
                unsigned long central, bool splitting) {
            unsigned long tot = 0;
            unsigned long size = list->size();

            const NormalSurface* s;
            for (unsigned long i = 0; i < size; i++) {
                s = list->surface(i);

                if (s->eulerChar() == euler &&
                        s->isConnected() == connected &&
                        s->isOrientable() == orient &&
                        s->isTwoSided() == twoSided &&
                        s->hasRealBoundary() == realBdry &&
                        s->isVertexLinking() == vertexLink &&
                        s->isCentral() == central &&
                        s->isSplitting() == splitting) {
                    std::pair<const regina::Edge<3>*, const regina::Edge<3>*> links
                        = s->isThinEdgeLink();
                    unsigned linkCount;
                    if (links.first == 0)
                        linkCount = 0;
                    else if (links.second == 0)
                        linkCount = 1;
                    else
                        linkCount = 2;

                    if (linkCount == edgeLink)
                        tot++;
                }
            }

            std::ostringstream msg;
            msg << "Number of " << surfaceType << " in "
                    << list->triangulation()->label()
                    << " should be " << expectedCount << ", not "
                    << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        static bool lexLess(const Ray* a, const Ray* b) {
            for (unsigned i = 0; i < a->size(); ++i) {
                if ((*a)[i] < (*b)[i])
                    return true;
                if ((*a)[i] > (*b)[i])
                    return false;
            }
            return false;
        }

        static bool identical(const NormalSurfaces* lhs,
                const NormalSurfaces* rhs) {
            if (lhs->size() != rhs->size())
                return false;

            unsigned long n = lhs->size();
            if (n == 0)
                return true;

            typedef const Ray* VecPtr;
            VecPtr* lhsRaw = new VecPtr[n];
            VecPtr* rhsRaw = new VecPtr[n];

            unsigned long i;
            for (i = 0; i < n; ++i) {
                lhsRaw[i] = &lhs->surface(i)->rawVector();
                rhsRaw[i] = &rhs->surface(i)->rawVector();
            }

            std::sort(lhsRaw, lhsRaw + n, lexLess);
            std::sort(rhsRaw, rhsRaw + n, lexLess);

            bool ok = true;
            for (i = 0; i < n; ++i)
                if (! (*(lhsRaw[i]) == *(rhsRaw[i]))) {
                    ok = false;
                    break;
                }

            delete[] lhsRaw;
            delete[] rhsRaw;
            return ok;
        }

        void standardEmpty() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &empty, NS_STANDARD);

            testSize(list, "standard normal surfaces", 0);

            delete list;
        }

        void quadEmpty() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &empty, NS_QUAD);

            testSize(list, "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalEmpty() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &empty, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 0);

            delete list;
        }

        void standardOneTet() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &oneTet, NS_STANDARD);

            testSize(list, "standard normal surfaces", 7);
            countCompactSurfaces(list, "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadOneTet() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &oneTet, NS_QUAD);

            testSize(list, "quad normal surfaces", 3);
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalOneTet() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &oneTet, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 10);
            countCompactSurfaces(list, "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list, "octagonal discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardGieseking() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &gieseking, NS_STANDARD);

            testSize(list, "standard normal surfaces", 1);
            testSurface(list->surface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadGieseking() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &gieseking, NS_QUAD);

            testSize(list, "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalGieseking() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &gieseking, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 1);
            testSurface(list->surface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardFigure8() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &figure8, NS_STANDARD);

            testSize(list, "standard normal surfaces", 1);
            testSurface(list->surface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadFigure8() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &figure8, NS_QUAD);

            testSize(list, "quad normal surfaces", 4);
            for (unsigned long i = 0; i < list->size(); i++)
                testSurface(list->surface(i),
                    "the figure eight knot complement", "spun surface",
                    0 /* euler, N/A */, 0 /* connected, N/A */,
                    0 /* orient, N/A */, 0 /* two-sided, N/A */,
                    false /* compact */, false /* realBdry */,
                    false /* vertex link */, 0 /* edge link */,
                    0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalFigure8() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &figure8, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 1);
            testSurface(list->surface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &S3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 3);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &S3, NS_QUAD);

            testSize(list, "quad normal surfaces", 1);
            countCompactSurfaces(list,
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &S3, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 4);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "standard almost normal central 2-spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardLoopC2() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopC2, NS_STANDARD);

            testSize(list, "standard normal surfaces", 5);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopC2() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopC2, NS_QUAD);

            testSize(list, "quad normal surfaces", 3);
            countCompactSurfaces(list,
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "quad normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopC2() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopC2, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 5);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void standardLoopCtw3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopCtw3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 5);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopCtw3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopCtw3, NS_QUAD);

            testSize(list, "quad normal surfaces", 4);
            countCompactSurfaces(list,
                "quad normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopCtw3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loopCtw3, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 5);
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void standardLargeS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeS3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 15);
            countCompactSurfaces(list,
                "standard normal vertex linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal vertex linking non-central spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                2 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal edge linking non-central tori", 2,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal edge linking central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                5 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal miscellaneous spheres", 3,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal miscellaneous tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                5 /* central */, true /* splitting */);

            delete list;
        }

        void quadLargeS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeS3, NS_QUAD);

            testSize(list, "quad normal surfaces", 4);
            countCompactSurfaces(list,
                "quad normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal edge linking non-central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal miscellaneous spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalLargeS3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeS3, NS_AN_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 27);

            delete list;
        }

        void standardLargeRP3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeRP3, NS_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 29);

            delete list;
        }

        void quadLargeRP3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeRP3, NS_QUAD);

            testSize(list, "quad normal surfaces", 5);
            countCompactSurfaces(list,
                "quad normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal edge linking non-central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal miscellaneous spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal miscellaneous projective planes", 1,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalLargeRP3() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &largeRP3, NS_AN_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 59);

            delete list;
        }

        void standardTwistedKxI() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &twistedKxI, NS_STANDARD);

            testSize(list, "standard normal surfaces", 8);
            countCompactSurfaces(list,
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadTwistedKxI() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &twistedKxI, NS_QUAD);

            testSize(list, "quad normal surfaces", 6);
            countCompactSurfaces(list,
                "quad normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalTwistedKxI() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &twistedKxI, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 13);
            countCompactSurfaces(list,
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, non-orbl)", 2,
                -1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, orbl)", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-2, 1-sided, non-orbl)", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardNorSFS() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &norSFS, NS_STANDARD);

            testSize(list, "standard normal surfaces", 25);

            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal two-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal one-sided genus two Klein bottles", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "standard normal central one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);

            delete list;
        }

        void quadNorSFS() {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &norSFS, NS_QUAD);

            testSize(list, "quad normal surfaces", 21);

            countCompactSurfaces(list,
                "quad normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "quad normal one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void testStandardLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop;
            loop.insertLayeredLoop(len, true);
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loop, NS_STANDARD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";
            loop.setLabel(name.str());

            // For standard normal and almost normal coordinates we just
            // count the surfaces (as opposed to in quad space, where we can
            // describe the surfaces precisely, with proof).

            // The following pattern has been observed experimentally.
            // For the purposes of the test suite I'm happy to assume it
            // holds in general; certainly it has been verified for all
            // the cases that we actually test here.
            unsigned long curr, prev, tmp;
            if (len == 1)
                curr = 2;
            else if (len == 2)
                curr = 4;
            else {
                curr = 4;
                prev = 2;
                for (unsigned counted = 2; counted < len; ++counted) {
                    tmp = curr + prev - 1;
                    prev = curr;
                    curr = tmp;
                }
            }

            testSize(list, "standard normal surfaces", curr);
        }

        void testQuadLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop;
            loop.insertLayeredLoop(len, true);
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loop, NS_QUAD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";
            loop.setLabel(name.str());

            // It is easy to prove in general that C~(len) has precisely
            // (len + 1) vertex surfaces, as described by the following tests.
            testSize(list, "quad normal surfaces", len + 1);
            countCompactSurfaces(list, "quad normal edge linking tori", len,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                len /* central */, true /* splitting */);

            delete list;
        }

        void testAlmostNormalLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop;
            loop.insertLayeredLoop(len, true);
            NormalSurfaces* list = NormalSurfaces::enumerate(
                &loop, NS_AN_STANDARD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";
            loop.setLabel(name.str());

            // For standard normal and almost normal coordinates we just
            // count the surfaces (as opposed to in quad space, where we can
            // describe the surfaces precisely, with proof).

            // The following pattern has been observed experimentally.
            // For the purposes of the test suite I'm happy to assume it
            // holds in general; certainly it has been verified for all
            // the cases that we actually test here.
            unsigned long curr, prev, tmp, currgap, prevgap;
            if (len < 7) {
                switch (len) {
                    case 1: curr = 3; break;
                    case 2: curr = 4; break;
                    case 3: curr = 5; break;
                    case 4: curr = 12; break;
                    case 5: curr = 12; break;
                    case 6: curr = 25; break;
                }
            } else {
                prev = 12; prevgap = 2;
                curr = 25; currgap = 1;

                for (unsigned counted = 6; counted < len; ++counted) {
                    tmp = curr + prev + currgap - 1;
                    prev = curr;
                    curr = tmp;

                    tmp = currgap + prevgap;
                    prevgap = currgap;
                    currgap = tmp;
                }
            }

            testSize(list, "standard almost normal surfaces", curr);
        }

        void largeDimensionsStandard() {
            testStandardLoopCtwGeneric(4);
            testStandardLoopCtwGeneric(8);
            testStandardLoopCtwGeneric(12);
        }

        void largeDimensionsQuad() {
            testQuadLoopCtwGeneric(5);
            testQuadLoopCtwGeneric(10);
            testQuadLoopCtwGeneric(20);
            testQuadLoopCtwGeneric(30);
            testQuadLoopCtwGeneric(40);
            testQuadLoopCtwGeneric(50);
        }

        void largeDimensionsAlmostNormal() {
            testAlmostNormalLoopCtwGeneric(3);
            testAlmostNormalLoopCtwGeneric(6);
            testAlmostNormalLoopCtwGeneric(9);
            testAlmostNormalLoopCtwGeneric(12);
            testAlmostNormalLoopCtwGeneric(15);
        }

        static void verifyConversions(Triangulation<3>* tri) {
            std::unique_ptr<NormalSurfaces> stdDirect(
                NormalSurfaces::enumerate(
                tri, NS_STANDARD, NS_VERTEX, NS_VERTEX_STD_DIRECT));
            std::unique_ptr<NormalSurfaces> stdConv(
                NormalSurfaces::enumerate(
                tri, NS_STANDARD, NS_VERTEX, NS_VERTEX_VIA_REDUCED));
            if ((! tri->isEmpty()) &&
                    (stdDirect->algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                    ! stdDirect->algorithm().has(NS_VERTEX_STD_DIRECT))) {
                std::ostringstream msg;
                msg << "Direct enumeration in standard coordinates gives "
                    "incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (tri->isValid() && ! tri->isIdeal()) {
                if ((! tri->isEmpty()) &&
                        (stdConv->algorithm().has(NS_VERTEX_STD_DIRECT) ||
                        ! stdConv->algorithm().has(NS_VERTEX_VIA_REDUCED))) {
                    std::ostringstream msg;
                    msg << "Quad-to-standard conversion gives "
                        "incorrect algorithm flags for "
                        << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                // Ideal or invalid triangluations should use the standard
                // enumeration process regardless of what the user requested.
                if ((! tri->isEmpty()) &&
                        (stdConv->algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                        ! stdConv->algorithm().has(NS_VERTEX_STD_DIRECT))) {
                    std::ostringstream msg;
                    msg << "Quad-to-standard conversion request was "
                        "incorrectly granted for "
                        << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (! identical(stdDirect.get(), stdConv.get())) {
                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard coordinates for "
                        << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            // Only test standard-to-quad if the preconditions for
            // standardToQuad() hold.
            if (tri->isValid() && ! tri->isIdeal()) {
                std::unique_ptr<NormalSurfaces> quadDirect(
                    NormalSurfaces::enumerate(tri, NS_QUAD));
                std::unique_ptr<NormalSurfaces> quadConv(
                    stdDirect->standardToQuad());
                if (! identical(quadDirect.get(), quadConv.get())) {
                    std::ostringstream msg;
                    msg << "Direct enumeration vs conversion gives different "
                        "surfaces in quadrilateral coordinates for "
                            << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyConversionsAN(Triangulation<3>* tri) {
            std::unique_ptr<NormalSurfaces> stdDirect(
                NormalSurfaces::enumerate(
                tri, NS_AN_STANDARD, NS_VERTEX, NS_VERTEX_STD_DIRECT));
            std::unique_ptr<NormalSurfaces> stdConv(
                NormalSurfaces::enumerate(
                tri, NS_AN_STANDARD, NS_VERTEX, NS_VERTEX_VIA_REDUCED));
            if ((! tri->isEmpty()) &&
                    (stdDirect->algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                    ! stdDirect->algorithm().has(NS_VERTEX_STD_DIRECT))) {
                std::ostringstream msg;
                msg << "Direct enumeration in standard AN coordinates gives "
                    "incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (tri->isValid() && ! tri->isIdeal()) {
                if ((! tri->isEmpty()) &&
                        (stdConv->algorithm().has(NS_VERTEX_STD_DIRECT) ||
                        ! stdConv->algorithm().has(NS_VERTEX_VIA_REDUCED))) {
                    std::ostringstream msg;
                    msg << "Quad-oct-to-standard-AN conversion gives "
                        "incorrect algorithm flags for "
                        << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                // Ideal or invalid triangluations should use the standard
                // enumeration process regardless of what the user requested.
                if ((! tri->isEmpty()) &&
                        (stdConv->algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                        ! stdConv->algorithm().has(NS_VERTEX_STD_DIRECT))) {
                    std::ostringstream msg;
                    msg << "Quad-oct-to-standard-AN conversion request was "
                        "incorrectly granted for "
                        << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (! identical(stdDirect.get(), stdConv.get())) {
                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard almost normal coordinates for "
                        << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            // Only test standard-to-quad if the preconditions for
            // standardToQuad() hold.
            if (tri->isValid() && ! tri->isIdeal()) {
                std::unique_ptr<NormalSurfaces> quadDirect(
                    NormalSurfaces::enumerate(tri, NS_AN_QUAD_OCT));
                std::unique_ptr<NormalSurfaces> quadConv(
                    stdDirect->standardANToQuadOct());
                if (! identical(quadDirect.get(), quadConv.get())) {
                    std::ostringstream msg;
                    msg << "Direct enumeration vs conversion gives different "
                        "surfaces in quadrilateral-octagon coordinates for "
                            << tri->label() << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void standardQuadConversionsConstructed() {
            verifyConversions(&empty);
            verifyConversions(&oneTet);
            verifyConversions(&S3);
            verifyConversions(&loopC2);
            verifyConversions(&loopCtw3);
            verifyConversions(&largeS3);
            verifyConversions(&largeRP3);
            verifyConversions(&twistedKxI);
            verifyConversions(&norSFS);
        }

        void standardQuadConversionsCensus() {
            runCensusMinClosed(&verifyConversions);
            runCensusAllClosed(&verifyConversions);
            runCensusAllBounded(&verifyConversions);
            runCensusAllIdeal(&verifyConversions);
        }

        void standardANQuadOctConversionsConstructed() {
            verifyConversionsAN(&empty);
            verifyConversionsAN(&oneTet);
            verifyConversionsAN(&S3);
            verifyConversionsAN(&loopC2);
            verifyConversionsAN(&loopCtw3);
            verifyConversionsAN(&largeS3);
            verifyConversionsAN(&largeRP3);
            verifyConversionsAN(&twistedKxI);
            verifyConversionsAN(&norSFS);
        }

        void standardANQuadOctConversionsCensus() {
            runCensusMinClosed(verifyConversionsAN);
            runCensusAllClosed(verifyConversionsAN);
            runCensusAllBounded(verifyConversionsAN);
            runCensusAllIdeal(verifyConversionsAN);
        }

        template <regina::NormalCoords coords>
        static void verifyTreeVsDD(Triangulation<3>* tri) {
            NormalSurfaces* dd = NormalSurfaces::enumerate(
                tri, coords, NS_VERTEX, NS_VERTEX_DD | NS_VERTEX_STD_DIRECT);
            NormalSurfaces* tree = NormalSurfaces::enumerate(
                tri, coords, NS_VERTEX, NS_VERTEX_TREE | NS_VERTEX_STD_DIRECT);
            if ((! tri->isEmpty()) &&
                    (dd->algorithm().has(NS_VERTEX_TREE) ||
                    ! dd->algorithm().has(NS_VERTEX_DD))) {
                std::ostringstream msg;
                msg << "Double description enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if ((! tri->isEmpty()) &&
                    (tree->algorithm().has(NS_VERTEX_DD) ||
                    ! tree->algorithm().has(NS_VERTEX_TREE))) {
                std::ostringstream msg;
                msg << "Tree traversal enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(dd, tree)) {
                std::ostringstream msg;
                msg << "Double description vs tree enumeration in "
                    "coordinate system " << coords << " gives different "
                    "surfaces for " << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            delete dd;
            delete tree;
        }

        template <regina::NormalCoords coords>
        void treeVsDDCensus() {
            runCensusMinClosed(verifyTreeVsDD<coords>);
            runCensusAllClosed(verifyTreeVsDD<coords>);
            runCensusAllBounded(verifyTreeVsDD<coords>);
            runCensusAllIdeal(verifyTreeVsDD<coords>);
        }

        template <regina::NormalCoords coords>
        static void verifyFundPrimalVsDual(Triangulation<3>* tri) {
            NormalSurfaces* primal = NormalSurfaces::enumerate(
                tri, coords, NS_FUNDAMENTAL, NS_HILBERT_PRIMAL);
            NormalSurfaces* dual = NormalSurfaces::enumerate(
                tri, coords, NS_FUNDAMENTAL, NS_HILBERT_DUAL);
            if ((! tri->isEmpty()) &&
                    (primal->algorithm().has(NS_HILBERT_DUAL) ||
                    ! primal->algorithm().has(NS_HILBERT_PRIMAL))) {
                std::ostringstream msg;
                msg << "Primal Hilbert basis enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if ((! tri->isEmpty()) &&
                    (dual->algorithm().has(NS_HILBERT_PRIMAL) ||
                    ! dual->algorithm().has(NS_HILBERT_DUAL))) {
                std::ostringstream msg;
                msg << "Dual Hilbert basis enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(primal, dual)) {
                std::ostringstream msg;
                msg << "Primal vs dual Hilbert basis enumeration in "
                    "coordinate system " << coords << " gives different "
                    "surfaces for " << tri->label() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            delete primal;
            delete dual;
        }

        template <regina::NormalCoords coords>
        void fundPrimalVsDual() {
            runCensusMinClosed(verifyFundPrimalVsDual<coords>, true);
            runCensusAllClosed(verifyFundPrimalVsDual<coords>, true);
            runCensusAllBounded(verifyFundPrimalVsDual<coords>, true);
            runCensusAllIdeal(verifyFundPrimalVsDual<coords>, true);
        }

        static void testDisjoint(Triangulation<3>* tri) {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                tri, NS_AN_STANDARD);
            unsigned long n = list->size();

            unsigned long i, j;
            const NormalSurface *s, *t;
            std::pair<const Edge<3>*, const Edge<3>*> edges;
            unsigned long edge;

            for (i = 0; i < n; ++i) {
                s = list->surface(i);

                // For some types of surfaces we know exactly what it
                // should be disjoint from.
                if (s->isVertexLinking()) {
                    // Vertex links are disjoint from everything.
                    for (j = 0; j < n; ++j) {
                        t = list->surface(j);
                        if (! s->disjoint(*t)) {
                            std::ostringstream msg;
                            msg << "Surface #" << i << " for "
                                << tri->label()
                                << " is a vertex link "
                                "and therefore should be disjoint from "
                                "surface #" << j << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                } else if ((edges = s->isThinEdgeLink()).first) {
                    // A thin edge link is disjoint from (i) all vertex
                    // links, and (ii) all surfaces that do not meet the
                    // relevant edge (except the edge link itself, if it
                    // is 1-sided).
                    edge = edges.first->index();

                    for (j = 0; j < n; ++j) {
                        // Deal with (s, s) later.
                        if (j == i)
                            continue;

                        t = list->surface(j);
                        if (t->isVertexLinking()) {
                            if (! s->disjoint(*t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i << " for "
                                    << tri->label()
                                    << " is a thin edge link and therefore "
                                    "should be disjoint from surface #" << j
                                    << ", which is a vertex link.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else if (t->edgeWeight(edge) == 0) {
                            if (! s->disjoint(*t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i << " for "
                                    << tri->label()
                                    << " is a thin edge link and therefore "
                                    "should be disjoint from surface #" << j
                                    << ", which does not meet the "
                                    "corresponding edge.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (s->disjoint(*t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i <<
                                    " is a thin edge link and therefore "
                                    "should not be disjoint from surface #"
                                    << j << ", which meets the "
                                    "corresponding edge.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }

                // Ensure that the surface is disjoint from itself
                // iff it is two-sided.
                if (s->isTwoSided() && ! s->disjoint(*s)) {
                    std::ostringstream msg;
                    msg << "Surface #" << i << " for "
                        << tri->label()
                        << " is two-sided and therefore should be "
                        "disjoint from itself.";
                    CPPUNIT_FAIL(msg.str());
                } else if ((! s->isTwoSided()) && s->disjoint(*s)) {
                    std::ostringstream msg;
                    msg << "Surface #" << i << " for "
                        << tri->label()
                        << " is one-sided and therefore should not be "
                        "disjoint from itself.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            delete list;
        }

        void disjointConstructed() {
            testDisjoint(&oneTet);
            testDisjoint(&figure8);
            testDisjoint(&gieseking);
            testDisjoint(&S3);
            testDisjoint(&loopC2);
            testDisjoint(&loopCtw3);
            testDisjoint(&largeS3);
            testDisjoint(&largeRP3);
            testDisjoint(&twistedKxI);
            testDisjoint(&norSFS);
        }

        void disjointCensus() {
            runCensusAllClosed(&testDisjoint);
            runCensusAllBounded(&testDisjoint);
            runCensusAllIdeal(&testDisjoint);
        }

        /**
         * PRE: tri is valid with only one component, and all vertex
         * links are spheres or discs.
         */
        static bool mightBeTwistedProduct(const Triangulation<3>* tri) {
            if (tri->countBoundaryComponents() != 1)
                return false;

            // Check the relationship between H1 and H1Bdry.
            // We must have one of:
            //  -  H1 = (2g)Z, H1Bdry = (4g-2)Z;
            //  -  H1 = Z_2 + (g-1)Z, H1Bdry = Z_2 + (2g-3)Z;
            //  -  H1 = Z_2 + (g-1)Z, H1Bdry = (2g-2)Z;
            const AbelianGroup& h1 = tri->homology();
            const AbelianGroup& bdry = tri->homologyBdry();

            if (h1.countInvariantFactors() == 0) {
                // Must have H1 = (2g)Z.
                if (bdry.countInvariantFactors() != 0)
                    return false;
                if (bdry.rank() != 2 * h1.rank() - 2)
                    return false;
            } else if (h1.countInvariantFactors() == 1) {
                // Must have H1 = Z_2 + (g-1)Z.
                if (h1.invariantFactor(0) != 2)
                    return false;

                if (bdry.countInvariantFactors() == 0) {
                    if (bdry.rank() != 2 * h1.rank())
                        return false;
                } else {
                    if (bdry.countInvariantFactors() != 1)
                        return false;
                    if (bdry.invariantFactor(0) != 2)
                        return false;
                    if (bdry.rank() != 2 * h1.rank() - 1)
                        return false;
                }
            } else
                return false;

            // Check that H1Rel is just Z_2.
            if (! tri->homologyRel().isZn(2))
                return false;

            return true;
        }

        /**
         * PRE: tri is valid with only one component, and all vertex
         * links are spheres or discs.
         */
        static bool mightBeUntwistedProduct(const Triangulation<3>* tri) {
            if (tri->countBoundaryComponents() != 2)
                return false;

            // Check that both boundary components are homeomorphic.
            BoundaryComponent<3>* b0 = tri->boundaryComponent(0);
            BoundaryComponent<3>* b1 = tri->boundaryComponent(1);

            if (b0->eulerChar() != b1->eulerChar())
                return false;
            if (b0->isOrientable() && ! b1->isOrientable())
                return false;
            if (b1->isOrientable() && ! b0->isOrientable())
                return false;

            // Check that H1 is of the form (k)Z or Z_2 + (k)Z, and that
            // H1Bdry = 2 H1.
            const AbelianGroup& h1 = tri->homology();
            const AbelianGroup& bdry = tri->homologyBdry();

            if (h1.countInvariantFactors() == 0) {
                // Must have H1 = (k)Z.
                if (bdry.rank() != 2 * h1.rank())
                    return false;
                if (bdry.countInvariantFactors() != 0)
                    return false;
            } else if (h1.countInvariantFactors() == 1) {
                // Must have H1 = Z_2 + (k)Z.
                if (h1.invariantFactor(0) != 2)
                    return false;
                if (bdry.rank() != 2 * h1.rank())
                    return false;
                if (bdry.countInvariantFactors() != 2)
                    return false;
                if (bdry.invariantFactor(0) != 2)
                    return false;
                if (bdry.invariantFactor(1) != 2)
                    return false;
            } else
                return false;

            // Check that H1Rel is just Z.
            if (! tri->homologyRel().isZ())
                return false;

            return true;
        }

        // Check whether the boundary of the given triangulation *might*
        // be equal to (i) the surface s, (ii) two copies of the surface s,
        // or (iii) a double cover of the surface s.
        // Increment the relevant counters accordingly.
        static void checkBoundaryType(const NormalSurface* s,
                const Triangulation<3>* tri, unsigned& foundS,
                unsigned& foundTwoCopies, unsigned& foundDoubleCover) {
            if (tri->countBoundaryComponents() == 1) {
                const BoundaryComponent<3>* b = tri->boundaryComponent(0);

                if (s->eulerChar() == b->eulerChar()
                        && s->isOrientable() == b->isOrientable())
                    ++foundS;
                if (s->eulerChar() * 2 == b->eulerChar() &&
                        (b->isOrientable() || ! s->isOrientable()))
                    ++foundDoubleCover;
            } else if (tri->countBoundaryComponents() == 2) {
                const BoundaryComponent<3>* b0 = tri->boundaryComponent(0);
                const BoundaryComponent<3>* b1 = tri->boundaryComponent(1);

                if (
                        s->eulerChar() == b0->eulerChar() &&
                        s->eulerChar() == b1->eulerChar() &&
                        s->isOrientable() == b0->isOrientable() &&
                        s->isOrientable() == b1->isOrientable())
                    ++foundTwoCopies;
            }
        }

        /**
         * PRE: tri is valid and has only one component.
         */
        static void testCutAlong(Triangulation<3>* tri) {
            NormalSurfaces* list = NormalSurfaces::enumerate(
                tri, NS_STANDARD);
            unsigned long n = list->size();

            const NormalSurface *s;
            std::unique_ptr<Triangulation<3>> t;
            std::unique_ptr<Container> comp;
            unsigned long nComp;

            std::unique_ptr<NormalSurface> sDouble;
            std::unique_ptr<Triangulation<3>> tDouble;
            std::unique_ptr<Container> compDouble;
            unsigned long nCompDouble;

            bool separating;

            unsigned long expected;
            Packet* p;

            // We use the fact that each normal surface is connected.
            for (unsigned long i = 0; i < n; ++i) {
                s = list->surface(i);
                t.reset(s->cutAlong());
                t->intelligentSimplify();
                comp.reset(new Container());
                nComp = t->splitIntoComponents(comp.get(), false);

                sDouble.reset(s->doubleSurface());
                tDouble.reset(sDouble->cutAlong());
                tDouble->intelligentSimplify();
                compDouble.reset(new Container());
                nCompDouble = tDouble->splitIntoComponents(compDouble.get(),
                    false);

                separating = (s->isTwoSided() && nComp > 1);

                expected = (separating ? 2 : 1);
                if (nComp != expected) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i << " for "
                        << tri->label()
                        << " gives " << nComp << " component(s), not "
                        << expected << " as expected.";
                    CPPUNIT_FAIL(msg.str());
                }

                expected = (separating ? 3 : 2);
                if (nCompDouble != expected) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for " << tri->label()
                        << " gives " << nCompDouble << " component(s), not "
                        << expected << " as expected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! t->isValid()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i << " for "
                        << tri->label()
                        << " gives an invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! tDouble->isValid()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for " << tri->label()
                        << " gives an invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isIdeal() && ! t->isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i
                        << " for "
                        << tri->label() << " (which is ideal)"
                        << " gives a non-ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (tri->isIdeal() && ! tDouble->isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for "
                        << tri->label() << " (which is ideal)"
                        << " gives a non-ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if ((! tri->isIdeal()) && t->isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i
                        << " for "
                        << tri->label() << " (which is not ideal)"
                        << " gives an ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if ((! tri->isIdeal()) && tDouble->isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for "
                        << tri->label() << " (which is not ideal)"
                        << " gives an ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isOrientable() && ! t->isOrientable()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i
                        << " for "
                        << tri->label() << " (which is orientable)"
                        << " gives a non-orientable triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (tri->isOrientable() && ! tDouble->isOrientable()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for "
                        << tri->label() << " (which is orientable)"
                        << " gives a non-orientable triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (p = comp->firstChild(); p; p = p->nextSibling())
                    if (! static_cast<Triangulation<3>*>(p)->
                            hasBoundaryTriangles()) {
                        std::ostringstream msg;
                        msg << "Cutting along surface #" << i
                            << " for " << tri->label()
                            << " gives a component with no boundary triangles.";
                        CPPUNIT_FAIL(msg.str());
                    }
                for (p = compDouble->firstChild(); p; p = p->nextSibling())
                    if (! static_cast<Triangulation<3>*>(p)->
                            hasBoundaryTriangles()) {
                        std::ostringstream msg;
                        msg << "Cutting along double surface #" << i
                            << " for " << tri->label()
                            << " gives a component with no boundary triangles.";
                        CPPUNIT_FAIL(msg.str());
                    }

                // The remaining tests only work for closed triangulations.
                if (! tri->isClosed())
                    continue;

                // Check the boundaries of components of t.
                unsigned expectS, expectTwoCopies, expectDoubleCover;
                unsigned foundS, foundTwoCopies, foundDoubleCover;
                if (separating) {
                    expectS = 2;
                    expectTwoCopies = 0;
                    expectDoubleCover = 0;
                } else if (s->isTwoSided()) {
                    expectS = 0;
                    expectTwoCopies = 1;
                    expectDoubleCover = 0;
                } else {
                    expectS = 0;
                    expectTwoCopies = 0;
                    expectDoubleCover = 1;
                }
                if (t->countBoundaryComponents() !=
                        expectS + 2 * expectTwoCopies + expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i << " for "
                        << tri->label()
                        << " gives the wrong number of boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
                foundS = foundTwoCopies = foundDoubleCover = 0;
                for (p = comp->firstChild(); p; p = p->nextSibling())
                    checkBoundaryType(s, static_cast<Triangulation<3>*>(p),
                        foundS, foundTwoCopies, foundDoubleCover);
                if (foundS < expectS || foundTwoCopies < expectTwoCopies ||
                        foundDoubleCover < expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along surface #" << i << " for "
                        << tri->label()
                        << " gives boundary components of the wrong type.";
                    CPPUNIT_FAIL(msg.str());
                }

                // Check the boundaries of components of tDouble.
                if (separating) {
                    expectS = 2;
                    expectTwoCopies = 1;
                    expectDoubleCover = 0;
                } else if (s->isTwoSided()) {
                    expectS = 0;
                    expectTwoCopies = 2;
                    expectDoubleCover = 0;
                } else {
                    expectS = 0;
                    expectTwoCopies = 0;
                    expectDoubleCover = 2;
                }
                if (tDouble->countBoundaryComponents() !=
                        expectS + 2 * expectTwoCopies + expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for " << tri->label()
                        << " gives the wrong number of boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
                foundS = foundTwoCopies = foundDoubleCover = 0;
                for (p = compDouble->firstChild(); p; p = p->nextSibling())
                    checkBoundaryType(s, static_cast<Triangulation<3>*>(p),
                        foundS, foundTwoCopies, foundDoubleCover);
                if (foundS < expectS || foundTwoCopies < expectTwoCopies ||
                        foundDoubleCover < expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for " << tri->label()
                        << " gives boundary components of the wrong type.";
                    CPPUNIT_FAIL(msg.str());
                }

                // Look for the product piece when cutting along the
                // double surface.
                for (p = compDouble->firstChild(); p; p = p->nextSibling()) {
                    if (s->isTwoSided()) {
                        if (mightBeUntwistedProduct(
                                static_cast<Triangulation<3>*>(p)))
                            break;
                    } else {
                        if (mightBeTwistedProduct(
                                static_cast<Triangulation<3>*>(p)))
                            break;
                    }
                }
                if (! p) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface #" << i
                        << " for " << tri->label()
                        << " does not yield a product piece as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            delete list;
        }

        void cutAlongConstructed() {
            testCutAlong(&oneTet);
            testCutAlong(&figure8);
            testCutAlong(&gieseking);
            testCutAlong(&S3);
            testCutAlong(&loopC2);
            testCutAlong(&loopCtw3);
            testCutAlong(&largeS3);
            testCutAlong(&largeRP3);
            testCutAlong(&twistedKxI);
            testCutAlong(&norSFS);
        }

        void cutAlongCensus() {
            runCensusAllClosed(&testCutAlong, true);
            runCensusAllBounded(&testCutAlong, true);
            runCensusAllIdeal(&testCutAlong, true);
        }
};

void addNormalSurfaces(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NormalSurfacesTest::suite());
}

