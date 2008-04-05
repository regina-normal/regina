
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include <cppunit/extensions/HelperMacros.h>
#include "manifold/nsimplesurfacebundle.h"
#include "triangulation/nedge.h"
#include "triangulation/nperm.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nexampletriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NEdge;
using regina::NPerm;
using regina::NSimpleSurfaceBundle;
using regina::NTetrahedron;
using regina::NTriangulation;

class ElementaryMovesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ElementaryMovesTest);

    CPPUNIT_TEST(twoZeroEdgeResult);
    CPPUNIT_TEST(twoZeroEdgeInvalid);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Tetrahedra 0 and 1 joined along faces 0 and 1 using the
        // identity permutation.
        NTriangulation base;

        // As for base, but with two diagonally opposite faces joined
        // also to form a solid Klein bottle.
        // This acts as a two-sided Mobius strip that can be flattened,
        // with faces 0/012 <--> 1/301.
        NTriangulation baseKB;

    public:
        void setUp() {
            NTetrahedron* r = new NTetrahedron();
            NTetrahedron* s = new NTetrahedron();
            r->joinTo(0, s, NPerm());
            r->joinTo(1, s, NPerm());
            base.addTetrahedron(r);
            base.addTetrahedron(s);

            baseKB.insertTriangulation(base);
            baseKB.getTetrahedron(0)->joinTo(2,
                baseKB.getTetrahedron(1), NPerm(1, 2, 3, 0));

            baseKB.gluingsHaveChanged();
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
            bool found = false;
            NEdge* edge;
            for (unsigned long e = 0; e < tri->getNumberOfEdges(); e++) {
                edge = tri->getEdge(e);
                if (edge->getDegree() == 2 && ! edge->isBoundary())
                    found = true;
                CPPUNIT_ASSERT_MESSAGE(
                    "An illegal 2-0 edge move was allowed for the " +
                    caseName + " case",
                    ! tri->twoZeroMove(tri->getEdge(e)));
            }
            CPPUNIT_ASSERT_MESSAGE(
                "No degree two edge was found for the " + caseName + " case.",
                found);
        }

        void twoZeroEdgeResult() {
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
                // One face boundary, two more joined in a loop.
                NTriangulation t;
                t.insertTriangulation(base);
                NTetrahedron* tet = t.getTetrahedron(0);
                tet->joinTo(2, tet, NPerm(2, 3));
                NTetrahedron* tet2 = new NTetrahedron();
                t.getTetrahedron(1)->joinTo(2, tet2, NPerm());
                t.addTetrahedron(tet2);

                // Force a recalculation of the skeleton, since
                // NTetrahedron routines such as getEdge() are oblivious
                // to the encompassing triangulation.  There has got to
                // be a better way (TODO).  We do this several times
                // here in this file.
                t.getNumberOfEdges();

                NEdge* e = tet->getEdge(regina::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-loop-tet is malformed.",
                    (! e->isBoundary()) && e->getDegree() == 1 &&
                        t.isOrientable());

                NTriangulation ball;
                ball.addTetrahedron(new NTetrahedron());

                verify20Edge(&t, 3, &ball, "boundary-loop-tet");
            }

            {
                // Two boundary faces, the others attached to the top of
                // an LST.
                NTriangulation orig;
                orig.insertLayeredSolidTorus(4, 7);
                NTetrahedron* top = new NTetrahedron();
                orig.getTetrahedron(0)->joinTo(2, top, NPerm(2, 3, 0, 1));
                orig.getTetrahedron(0)->joinTo(3, top, NPerm(2, 3, 0, 1));
                orig.addTetrahedron(top);

                NTriangulation lst;
                lst.insertLayeredSolidTorus(3, 4);

                verify20Edge(&orig, 0, &lst, "boundary-layer");
            }

            {
                // Wedged in between two adjacent internal faces in an
                // LST(3,4,7).
                const int adj[5][4] = {
                    { 1, 1, -1, -1},
                    { 4, 4, 0, 0},
                    { 2, 2, 3, 3},
                    { 4, 4, 2, 2},
                    { 3, 3, 1, 1}
                };

                const int glu[5][4][4] = {
                    { { 2,1,3,0 }, { 0,3,1,2 }, { 0,0,0,0 }, { 0,0,0,0 } },
                    { { 3,1,2,0 }, { 0,2,1,3 }, { 3,1,0,2 }, { 0,2,3,1 } },
                    { { 1,2,3,0 }, { 3,0,1,2 }, { 0,1,2,3 }, { 0,1,2,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 0,1,2,3 }, { 0,1,2,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 0,2,1,3 }, { 3,1,2,0 } }
                };

                NTriangulation orig;
                orig.insertConstruction(5, adj, glu);
                NTriangulation lst;
                lst.insertLayeredSolidTorus(3, 4);

                verify20Edge(&orig, 5, &lst, "internal-flat");
            }

            {
                // Wedged into the tip of a layered L(10,3), with two
                // faces joined in a loop.
                const int adj[5][4] = {
                    { 1, 1, 3, 3},
                    { 2, 2, 0, 0},
                    { 2, 2, 1, 1},
                    { 4, 4, 0, 0},
                    { 3, 3, 4, 4}
                };

                const int glu[5][4][4] = {
                    { { 2,1,3,0 }, { 0,3,1,2 }, { 0,1,2,3 }, { 0,1,2,3 } },
                    { { 3,1,2,0 }, { 0,2,1,3 }, { 3,1,0,2 }, { 0,2,3,1 } },
                    { { 1,2,3,0 }, { 3,0,1,2 }, { 0,2,1,3 }, { 3,1,2,0 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 0,1,2,3 }, { 0,1,2,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 1,2,3,0 }, { 3,0,1,2 } }
                };

                NTriangulation orig;
                orig.insertConstruction(5, adj, glu);
                NTriangulation lens;
                lens.insertLayeredLensSpace(10, 3);
                NTriangulation copy(orig);

                verify20Edge(&orig, 5, &lens, "internal-loop-twist");
                verify20Edge(&copy, 0, &lens, "internal-flat-lens");
            }

            {
                // A hand-constructed example formed by squeezing baseKB
                // into a two-sided Mobius band face in K(iii | 1,0 | 0,1)
                // from the 6-tetrahedron non-orientable census.
                const int adjOrig[8][4] = {
                    { 1, 1, 2, 2},
                    { 0, 0, 6, 3},
                    { 0, 0, 7, 4},
                    { 1, 4, 5, 5},
                    { 2, 3, 5, 5},
                    { 3, 3, 4, 4},
                    { 7, 7, 7, 1},
                    { 6, 6, 2, 6}
                };

                const int gluOrig[8][4][4] = {
                    { { 0,2,1,3 }, { 2,1,3,0 }, { 1,3,0,2 }, { 3,0,2,1 } },
                    { { 0,2,1,3 }, { 3,1,0,2 }, { 2,0,3,1 }, { 2,3,1,0 } },
                    { { 2,0,3,1 }, { 1,3,2,0 }, { 1,3,2,0 }, { 2,3,1,0 } },
                    { { 3,2,0,1 }, { 0,1,2,3 }, { 3,1,0,2 }, { 2,0,3,1 } },
                    { { 3,2,0,1 }, { 0,1,2,3 }, { 1,3,2,0 }, { 0,2,1,3 } },
                    { { 2,1,3,0 }, { 1,3,0,2 }, { 3,0,2,1 }, { 0,2,1,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 1,2,3,0 }, { 1,3,0,2 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 3,0,2,1 }, { 3,0,1,2 } }
                };

                NTriangulation orig;
                orig.insertConstruction(8, adjOrig, gluOrig);

                const int adjResult[6][4] = {
                    { 1, 1, 2, 2},
                    { 0, 0, 2, 3},
                    { 0, 0, 1, 4},
                    { 1, 4, 5, 5},
                    { 2, 3, 5, 5},
                    { 3, 3, 4, 4}
                };

                const int gluResult[6][4][4] = {
                    { { 0,2,1,3 }, { 2,1,3,0 }, { 1,3,0,2 }, { 3,0,2,1 } },
                    { { 0,2,1,3 }, { 3,1,0,2 }, { 0,1,2,3 }, { 2,3,1,0 } },
                    { { 2,0,3,1 }, { 1,3,2,0 }, { 0,1,2,3 }, { 2,3,1,0 } },
                    { { 3,2,0,1 }, { 0,1,2,3 }, { 3,1,0,2 }, { 2,0,3,1 } },
                    { { 3,2,0,1 }, { 0,1,2,3 }, { 1,3,2,0 }, { 0,2,1,3 } },
                    { { 2,1,3,0 }, { 1,3,0,2 }, { 3,0,2,1 }, { 0,2,1,3 } }
                };

                NTriangulation result;
                result.insertConstruction(6, adjResult, gluResult);

                verify20Edge(&orig, 8, &result, "internal-cross");
            }

            {
                // Two copies of baseKB glued along a single face.
                NTriangulation t;
                t.insertTriangulation(baseKB);
                t.insertTriangulation(baseKB);
                t.getTetrahedron(0)->joinTo(3, t.getTetrahedron(2), NPerm());
                t.gluingsHaveChanged();

                verify20Edge(&t, 3, &baseKB, "bdry-cross-tet");
            }
        }

        void twoZeroEdgeInvalid() {
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

            {
                // All four faces internal, but the two equatorial edges
                // both boundary.
                NTriangulation t;
                t.insertTriangulation(base);
                NTetrahedron* p = new NTetrahedron();
                NTetrahedron* q = new NTetrahedron();
                NTetrahedron* r = new NTetrahedron();
                NTetrahedron* s = new NTetrahedron();
                t.getTetrahedron(0)->joinTo(2, p, NPerm());
                t.getTetrahedron(0)->joinTo(3, q, NPerm());
                t.getTetrahedron(1)->joinTo(2, r, NPerm());
                t.getTetrahedron(1)->joinTo(3, s, NPerm());
                t.addTetrahedron(p);
                t.addTetrahedron(q);
                t.addTetrahedron(r);
                t.addTetrahedron(s);

                verify20EdgeInvalid(&t, "boundary-edges");
            }

            {
                // All four faces internal, and the two equatorial edges
                // internal but identified (sphere).
                NTriangulation t;
                t.insertTriangulation(base);
                NTetrahedron* p = new NTetrahedron();
                NTetrahedron* q = new NTetrahedron();
                NTetrahedron* r = new NTetrahedron();
                NTetrahedron* s = new NTetrahedron();
                t.getTetrahedron(0)->joinTo(2, p, NPerm());
                t.getTetrahedron(0)->joinTo(3, q, NPerm());
                t.getTetrahedron(1)->joinTo(2, r, NPerm());
                t.getTetrahedron(1)->joinTo(3, s, NPerm());
                p->joinTo(3, r, NPerm());
                q->joinTo(2, s, NPerm());
                t.addTetrahedron(p);
                t.addTetrahedron(q);
                t.addTetrahedron(r);
                t.addTetrahedron(s);

                // Ugly hack to regenerate skeleton, see notes above.
                t.getNumberOfEdges();

                NEdge* e = t.getTetrahedron(0)->getEdge(
                    regina::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case identified-edges-S2 is malformed.",
                    (! e->isBoundary()) && e->getDegree() == 6 &&
                        t.isOrientable());

                verify20EdgeInvalid(&t, "identified-edges-S2");
            }

            {
                // All four faces internal, and the two equatorial edges
                // internal but identified (RP2).
                NTriangulation t;
                t.insertTriangulation(base);
                NTetrahedron* p = new NTetrahedron();
                NTetrahedron* q = new NTetrahedron();
                NTetrahedron* r = new NTetrahedron();
                NTetrahedron* s = new NTetrahedron();
                t.getTetrahedron(0)->joinTo(2, p, NPerm());
                t.getTetrahedron(0)->joinTo(3, q, NPerm());
                t.getTetrahedron(1)->joinTo(2, r, NPerm());
                t.getTetrahedron(1)->joinTo(3, s, NPerm());
                p->joinTo(3, r, NPerm(0, 1));
                q->joinTo(2, s, NPerm(0, 1));
                t.addTetrahedron(p);
                t.addTetrahedron(q);
                t.addTetrahedron(r);
                t.addTetrahedron(s);

                // Ugly hack to regenerate skeleton, see notes above.
                t.getNumberOfEdges();

                NEdge* e = t.getTetrahedron(0)->getEdge(
                    regina::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case identified-edges-RP2 is malformed.",
                    (! e->isBoundary()) && e->getDegree() == 6 &&
                        ! t.isOrientable());

                verify20EdgeInvalid(&t, "identified-edges-RP2");
            }

            {
                // Two faces boundary, the other joined in a loop.
                NTriangulation t;
                t.insertTriangulation(base);
                NTetrahedron* tet = t.getTetrahedron(0);
                tet->joinTo(2, tet, NPerm(2, 3));
                t.gluingsHaveChanged();

                // Ugly hack to regenerate skeleton, see notes above.
                t.getNumberOfEdges();

                NEdge* e = tet->getEdge(regina::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-loop-boundary is malformed.",
                    (! e->isBoundary()) && e->getDegree() == 1 &&
                        t.isOrientable());

                verify20EdgeInvalid(&t, "boundary-loop-boundary");
            }

            {
                // Two faces boundary, the other joined in a cross.
                NTriangulation t;
                t.insertTriangulation(baseKB);

                // Ugly hack to regenerate skeleton, see notes above.
                t.getNumberOfEdges();

                NEdge* e = t.getTetrahedron(0)->
                    getEdge(regina::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-cross-boundary is malformed.",
                    e->isBoundary() && e->getDegree() == 3 &&
                        ! t.isOrientable());

                verify20EdgeInvalid(&t, "boundary-cross-boundary");
            }

            {
                // Two opposite faces boundary, the other two wedged
                // inside an LST(3,4,7).
                const int adj[5][4] = {
                    { 1, 1, -1, -1},
                    { 4, 2, 0, 0},
                    { 2, 2, 1, 3},
                    { 4, 4, -1, 2},
                    { 3, 3, -1, 1}
                };

                const int glu[5][4][4] = {
                    { { 2,1,3,0 }, { 0,3,1,2 }, { 0,0,0,0 }, { 0,0,0,0 } },
                    { { 3,1,2,0 }, { 0,2,1,3 }, { 3,1,0,2 }, { 0,2,3,1 } },
                    { { 1,2,3,0 }, { 3,0,1,2 }, { 0,2,1,3 }, { 0,1,2,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 0,0,0,0 }, { 0,1,2,3 } },
                    { { 0,1,2,3 }, { 0,1,2,3 }, { 0,0,0,0 }, { 3,1,2,0 } }
                };

                NTriangulation orig;
                orig.insertConstruction(5, adj, glu);
                verify20EdgeInvalid(&orig, "boundary-opposite-wedge");
            }

            {
                // Two diagonally opposite faces boundary, the other two
                // glued to an LST boundary.  Doesn't really matter how.
                NTriangulation orig;
                orig.insertLayeredSolidTorus(3,4);
                orig.insertTriangulation(base);

                NTetrahedron* top = orig.getTetrahedron(0);
                orig.getTetrahedron(3)->joinTo(3, top, NPerm(1,2,0,3));
                orig.getTetrahedron(4)->joinTo(2, top, NPerm(0,3,2,1));
                orig.gluingsHaveChanged();

                verify20EdgeInvalid(&orig, "boundary-diag");
            }
        }
};

void addElementaryMoves(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ElementaryMovesTest::suite());
}

