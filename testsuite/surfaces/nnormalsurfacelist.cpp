
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cppunit/extensions/HelperMacros.h>
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/surfaces/testsurfaces.h"

using regina::NNormalSurface;
using regina::NNormalSurfaceList;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTriangulation;

class NNormalSurfaceListTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NNormalSurfaceListTest);

    CPPUNIT_TEST(standardGieseking);
    CPPUNIT_TEST(quadGieseking);
    CPPUNIT_TEST(almostNormalGieseking);
    CPPUNIT_TEST(standardFigure8);
    CPPUNIT_TEST(quadFigure8);
    CPPUNIT_TEST(almostNormalFigure8);

    CPPUNIT_TEST_SUITE_END();

    private:
        NTriangulation figure8;
            /**< The figure eight knot complement. */
        NTriangulation gieseking;
            /**< The Gieseking manifold. */

    public:
        void setUp() {
            NTetrahedron* r;
            NTetrahedron* s;

            // The two-tetrahedron figure eight knot complement is
            // described at the beginning of chapter 8 of Richard
            // Rannard's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            r->joinTo(0, s, NPerm(1, 3, 0, 2));
            r->joinTo(1, s, NPerm(2, 0, 3, 1));
            r->joinTo(2, s, NPerm(0, 3, 2, 1));
            r->joinTo(3, s, NPerm(2, 1, 0, 3));
            figure8.addTetrahedron(r);
            figure8.addTetrahedron(s);

            // The Gieseking manifold is simple enough; it has only one
            // tetrahedron.
            r = new NTetrahedron();
            r->joinTo(0, r, NPerm(1, 2, 0, 3));
            r->joinTo(2, r, NPerm(0, 2, 3, 1));
            gieseking.addTetrahedron(r);
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
                const char* surfaceName, int euler, int connected,
                int orient, int twoSided, bool compact, bool realBdry,
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
                if (connected == 1)
                    msg << "connected.";
                else if (connected == -1)
                    msg << "disconnected.";
                else
                    msg << "of unknown connectedness.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isConnected() == connected);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (orient == 1)
                    msg << "orientable.";
                else if (orient == -1)
                    msg << "non-orientable.";
                else
                    msg << "of unknown orientability.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isOrientable() == orient);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (twoSided == 1)
                    msg << "2-sided.";
                else if (twoSided == -1)
                    msg << "1-sided.";
                else
                    msg << "of unknown sidedness.";

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

        void standardGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::STANDARD);

            testSize(list, "the Gieseking manifold",
                "standard normal surfaces", 1);
            testSurface(list->getSurface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, 1 /* connected */,
                -1 /* orient */, 1 /* two-sided */,
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
                0 /* euler */, 1 /* connected */,
                -1 /* orient */, 1 /* two-sided */,
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
                0 /* euler */, 1 /* connected */,
                1 /* orient */, 1 /* two-sided */,
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
                0 /* euler */, 1 /* connected */,
                1 /* orient */, 1 /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }
};

void addNNormalSurfaceList(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NNormalSurfaceListTest::suite());
}

