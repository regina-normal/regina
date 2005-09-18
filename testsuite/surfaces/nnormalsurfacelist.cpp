
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <cppunit/extensions/HelperMacros.h>
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/surfaces/testsurfaces.h"

using regina::NExampleTriangulation;
using regina::NNormalSurface;
using regina::NNormalSurfaceList;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTriangulation;

class NNormalSurfaceListTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NNormalSurfaceListTest);

    CPPUNIT_TEST(standardEmpty);
    CPPUNIT_TEST(standardOneTet);
    CPPUNIT_TEST(standardGieseking);
    CPPUNIT_TEST(standardFigure8);
    CPPUNIT_TEST(standardS3);
    CPPUNIT_TEST(standardLoopC2);
    CPPUNIT_TEST(standardLoopCtw3);
    CPPUNIT_TEST(standardTwistedKxI);
    CPPUNIT_TEST(quadEmpty);
    CPPUNIT_TEST(quadOneTet);
    CPPUNIT_TEST(quadGieseking);
    CPPUNIT_TEST(quadFigure8);
    CPPUNIT_TEST(quadS3);
    CPPUNIT_TEST(quadLoopC2);
    CPPUNIT_TEST(quadLoopCtw3);
    CPPUNIT_TEST(quadTwistedKxI);
    CPPUNIT_TEST(almostNormalEmpty);
    CPPUNIT_TEST(almostNormalOneTet);
    CPPUNIT_TEST(almostNormalGieseking);
    CPPUNIT_TEST(almostNormalFigure8);
    CPPUNIT_TEST(almostNormalS3);
    CPPUNIT_TEST(almostNormalLoopC2);
    CPPUNIT_TEST(almostNormalLoopCtw3);
    CPPUNIT_TEST(almostNormalTwistedKxI);

    CPPUNIT_TEST_SUITE_END();

    private:
        NTriangulation empty;
            /**< An empty triangulation. */
        NTriangulation oneTet;
            /**< A one-tetrahedron ball. */
        NTriangulation figure8;
            /**< The figure eight knot complement. */
        NTriangulation gieseking;
            /**< The Gieseking manifold. */
        NTriangulation S3;
            /**< A one-tetrahedron two-vertex 3-sphere. */
        NTriangulation loopC2;
            /**< An untwisted layered loop of length 2. */
        NTriangulation loopCtw3;
            /**< A twisted layered loop of length 3. */
        NTriangulation twistedKxI;
            /**< A 3-tetrahedron non-orientable twisted I-bundle over the
                 Klein bottle. */

    public:
        void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            delete source;
        }

        void setUp() {
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;

            // The one-tetrahedron ball has no face identifications at all.
            oneTet.addTetrahedron(new NTetrahedron());

            // Use pre-coded triangulations where we can.
            copyAndDelete(figure8,
                NExampleTriangulation::figureEightKnotComplement());
            copyAndDelete(gieseking, NExampleTriangulation::gieseking());

            // Layered loops can be constructed automatically.
            S3.insertLayeredLoop(1, false);
            loopC2.insertLayeredLoop(2, false);
            loopCtw3.insertLayeredLoop(3, true);

            // A 3-tetrahedron non-orientable twisted I-bundle over the
            // Klein bottle is described in Chapter 3 of Benjamin
            // Burton's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            t = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 1, 2, 3));
            r->joinTo(1, t, NPerm(2, 1, 0, 3));
            r->joinTo(2, t, NPerm(1, 3, 2, 0));
            s->joinTo(1, t, NPerm(0, 3, 2, 1));
            s->joinTo(2, t, NPerm(3, 1, 0, 2));
            twistedKxI.addTetrahedron(r);
            twistedKxI.addTetrahedron(s);
            twistedKxI.addTetrahedron(t);
        }

        void tearDown() {
        }

        void testSize(NNormalSurfaceList* list, const char* triName,
                const char* listType, unsigned long expectedSize) {
            std::ostringstream msg;
            msg << "Number of " << listType << " for " << triName
                << " should be " << expectedSize << ", not "
                << list->getNumberOfSurfaces() << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(),
                list->getNumberOfSurfaces() == expectedSize);
        }

        void testSurface(const NNormalSurface* surface, const char* triName,
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
                    << surface->getEulerCharacteristic() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->getEulerCharacteristic() == euler);
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

                std::pair<const regina::NEdge*, const regina::NEdge*> links
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

        void countCompactSurfaces(const NNormalSurfaceList* list,
                const char* triName, const char* surfaceType,
                unsigned long expectedCount, int euler,
                bool connected, bool orient, bool twoSided,
                bool realBdry, bool vertexLink, unsigned edgeLink,
                unsigned long central, bool splitting) {
            unsigned long tot = 0;
            unsigned long size = list->getNumberOfSurfaces();

            const NNormalSurface* s;
            for (unsigned long i = 0; i < size; i++) {
                s = list->getSurface(i);

                if (s->getEulerCharacteristic() == euler &&
                        s->isConnected() == connected &&
                        s->isOrientable() == orient &&
                        s->isTwoSided() == twoSided &&
                        s->hasRealBoundary() == realBdry &&
                        s->isVertexLinking() == vertexLink &&
                        s->isCentral() == central &&
                        s->isSplitting() == splitting) {
                    std::pair<const regina::NEdge*, const regina::NEdge*> links
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
            msg << "Number of " << surfaceType << " in " << triName
                    << " should be " << expectedCount << ", not "
                    << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        void standardEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::STANDARD);

            testSize(list, "the empty triangulation",
                "standard normal surfaces", 0);

            delete list;
        }

        void quadEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::QUAD);

            testSize(list, "the empty triangulation",
                "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the empty triangulation",
                "standard almost normal surfaces", 0);

            delete list;
        }

        void standardOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::STANDARD);

            testSize(list, "a single tetrahedron",
                "standard normal surfaces", 7);
            countCompactSurfaces(list, "a single tetrahedron",
                "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::QUAD);

            testSize(list, "a single tetrahedron",
                "quad normal surfaces", 3);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "a single tetrahedron",
                "standard almost normal surfaces", 10);
            countCompactSurfaces(list, "a single tetrahedron",
                "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "octahedral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::STANDARD);

            testSize(list, "the Gieseking manifold",
                "standard normal surfaces", 1);
            testSurface(list->getSurface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::QUAD);

            testSize(list, "the Gieseking manifold",
                "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the Gieseking manifold",
                "standard almost normal surfaces", 1);
            testSurface(list->getSurface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::STANDARD);

            testSize(list, "the figure eight knot complement",
                "standard normal surfaces", 1);
            testSurface(list->getSurface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::QUAD);

            testSize(list, "the figure eight knot complement",
                "quad normal surfaces", 4);
            for (unsigned long i = 0; i < list->getNumberOfSurfaces(); i++)
                testSurface(list->getSurface(i),
                    "the figure eight knot complement", "spun surface",
                    0 /* euler, N/A */, 0 /* connected, N/A */,
                    0 /* orient, N/A */, 0 /* two-sided, N/A */,
                    false /* compact */, false /* realBdry */,
                    false /* vertex link */, 0 /* edge link */,
                    0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the figure eight knot complement",
                "standard almost normal surfaces", 1);
            testSurface(list->getSurface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::STANDARD);

            testSize(list, "the 3-sphere",
                "standard normal surfaces", 3);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::QUAD);

            testSize(list, "the 3-sphere",
                "quad normal surfaces", 1);
            countCompactSurfaces(list, "the 3-sphere",
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the 3-sphere",
                "standard almost normal surfaces", 4);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard almost normal central 2-spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::STANDARD);

            testSize(list, "the untwisted layered loop C(2)",
                "standard normal surfaces", 5);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::QUAD);

            testSize(list, "the untwisted layered loop C(2)",
                "quad normal surfaces", 3);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "quad normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the untwisted layered loop C(2)",
                "standard almost normal surfaces", 5);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void standardLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::STANDARD);

            testSize(list, "the twisted layered loop C~(3)",
                "standard normal surfaces", 5);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::QUAD);

            testSize(list, "the twisted layered loop C~(3)",
                "quad normal surfaces", 4);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "quad normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the twisted layered loop C~(3)",
                "standard almost normal surfaces", 5);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void standardTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::STANDARD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal surfaces", 8);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::QUAD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal surfaces", 6);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces", 13);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, non-orbl)", 2,
                -1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, orbl)", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-2, 1-sided, non-orbl)", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }
};

void addNNormalSurfaceList(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NNormalSurfaceListTest::suite());
}

