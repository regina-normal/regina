
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cmath>
#include <memory>
#include <cppunit/extensions/HelperMacros.h>
#include "manifold/nsimplesurfacebundle.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nexampletriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NSimpleSurfaceBundle;
using regina::NTriangulation;

class ElementaryMovesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ElementaryMovesTest);

    CPPUNIT_TEST(twoZeroEdge);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void verify20Edge(NTriangulation* tri, long whichEdge,
                NTriangulation* result, const std::string& caseName) {
            bool done = tri->twoZeroMove(tri->getEdge(whichEdge));
            CPPUNIT_ASSERT_MESSAGE(
                "A 2-0 edge move was incorrectly disallowed for the " +
                caseName + " case", done);
            CPPUNIT_ASSERT_MESSAGE(
                "A 2-0 edge move produced an incorrect result for the " +
                caseName + " case", tri->isIsomorphicTo(*result).get());
        }

        void verify20EdgeInvalid(NTriangulation* tri,
                const std::string& caseName) {
            for (long e = 0; e < tri->getNumberOfEdges(); e++)
                CPPUNIT_ASSERT_MESSAGE(
                    "An illegal 2-0 edge move was allowed for the " +
                    caseName + " case",
                    ! tri->twoZeroMove(tri->getEdge(e)));
        }

        void twoZeroEdge() {
            {
                // A one-bdry-face case that Regina 4.1 used to crash on.
                const int adjOrig[5][4] = {
                    { -1, 2, 4, 4},
                    { -1, -1, 2, 4},
                    { 0, 3, 3, 1},
                    { 2, -1, 2, 4},
                    { 1, 0, 0, 3}
                };

                const int gluOrig[5][4][4] = {
                    { { 0,0,0,0 }, { 3,0,1,2 }, { 3,0,2,1 }, { 3,0,2,1 } },
                    { { 0,0,0,0 }, { 0,0,0,0 }, { 0,1,3,2 }, { 2,1,3,0 } },
                    { { 1,2,3,0 }, { 0,2,3,1 }, { 2,1,0,3 }, { 0,1,3,2 } },
                    { { 2,1,0,3 }, { 0,0,0,0 }, { 0,3,1,2 }, { 0,1,2,3 } },
                    { { 3,1,0,2 }, { 1,3,2,0 }, { 1,3,2,0 }, { 0,1,2,3 } }
                };

                const int adjResult[3][4] = {
                    { -1, -1, 1, 1},
                    { 0, 2, 2, 0},
                    { 1, -1, 1, -1}
                };

                const int gluResult[3][4][4] = {
                    { { 0,0,0,0 }, { 0,0,0,0 }, { 0,1,3,2 }, { 1,2,3,0 } },
                    { { 3,0,1,2 }, { 0,2,3,1 }, { 2,1,0,3 }, { 0,1,3,2 } },
                    { { 2,1,0,3 }, { 0,0,0,0 }, { 0,3,1,2 }, { 0,0,0,0 } }
                };

                NTriangulation orig;
                orig.insertConstruction(5, adjOrig, gluOrig);
                NTriangulation result;
                result.insertConstruction(3, adjResult, gluResult);

                verify20Edge(&orig, 0, &result, "one-boundary-face");
            }

            {
                // All four faces joined together in a simple loop.
                NTriangulation* s2xs1 =
                    NSimpleSurfaceBundle(NSimpleSurfaceBundle::S2xS1)
                    .construct();
                verify20EdgeInvalid(s2xs1, "round-loop");
                delete s2xs1;
            }

            {
                // All four faces joined together in a crossed loop.
                NTriangulation* s2xs1Twisted =
                    NSimpleSurfaceBundle(NSimpleSurfaceBundle::S2xS1_TWISTED)
                    .construct();
                verify20EdgeInvalid(s2xs1Twisted, "crossed-loop");
                delete s2xs1Twisted;
            }
        }
};

void addElementaryMoves(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ElementaryMovesTest::suite());
}

