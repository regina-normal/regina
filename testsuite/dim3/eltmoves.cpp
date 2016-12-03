
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
#include <cppunit/extensions/HelperMacros.h>
#include "manifold/simplesurfacebundle.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Edge;
using regina::Perm;
using regina::SimpleSurfaceBundle;
using regina::Tetrahedron;
using regina::Triangulation;

class ElementaryMovesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ElementaryMovesTest);

    CPPUNIT_TEST(twoZeroEdgeResult);
    CPPUNIT_TEST(twoZeroEdgeInvalid);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Tetrahedra 0 and 1 joined along faces 0 and 1 using the
        // identity permutation.
        Triangulation<3> base;

        // As for base, but with two diagonally opposite faces joined
        // also to form a solid Klein bottle.
        // This acts as a two-sided Mobius strip that can be flattened,
        // with faces 0/012 <--> 1/301.
        Triangulation<3> baseKB;

    public:
        void setUp() {
            Tetrahedron<3>* r = base.newTetrahedron();
            Tetrahedron<3>* s = base.newTetrahedron();
            r->join(0, s, Perm<4>());
            r->join(1, s, Perm<4>());

            baseKB.insertTriangulation(base);
            baseKB.tetrahedron(0)->join(2,
                baseKB.tetrahedron(1), Perm<4>(1, 2, 3, 0));
        }

        void tearDown() {
        }

        void verify20Edge(Triangulation<3>* tri, long whichEdge,
                Triangulation<3>* result, const std::string& caseName) {
            bool done = tri->twoZeroMove(tri->edge(whichEdge));
            CPPUNIT_ASSERT_MESSAGE(
                "A 2-0 edge move was incorrectly disallowed for the " +
                caseName + " case", done);
            CPPUNIT_ASSERT_MESSAGE(
                "A 2-0 edge move produced an incorrect result for the " +
                caseName + " case", tri->isIsomorphicTo(*result).get());
        }

        void verify20EdgeInvalid(Triangulation<3>* tri,
                const std::string& caseName) {
            bool found = false;
            Edge<3>* edge;
            for (unsigned long e = 0; e < tri->countEdges(); e++) {
                edge = tri->edge(e);
                if (edge->degree() == 2 && ! edge->isBoundary())
                    found = true;
                CPPUNIT_ASSERT_MESSAGE(
                    "An illegal 2-0 edge move was allowed for the " +
                    caseName + " case",
                    ! tri->twoZeroMove(tri->edge(e)));
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

                Triangulation<3> orig;
                orig.insertConstruction(5, adjOrig, gluOrig);
                Triangulation<3> result;
                result.insertConstruction(3, adjResult, gluResult);

                verify20Edge(&orig, 0, &result, "one-boundary-face");
            }

            {
                // One face boundary, two more joined in a loop.
                Triangulation<3> t;
                t.insertTriangulation(base);
                Tetrahedron<3>* tet = t.tetrahedron(0);
                tet->join(2, tet, Perm<4>(2, 3));
                Tetrahedron<3>* tet2 = t.newTetrahedron();
                t.tetrahedron(1)->join(2, tet2, Perm<4>());

                Edge<3>* e = tet->edge(Edge<3>::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-loop-tet is malformed.",
                    (! e->isBoundary()) && e->degree() == 1 &&
                        t.isOrientable());

                Triangulation<3> ball;
                ball.newTetrahedron();

                verify20Edge(&t, 3, &ball, "boundary-loop-tet");
            }

            {
                // Two boundary faces, the others attached to the top of
                // an LST.
                Triangulation<3> orig;
                orig.insertLayeredSolidTorus(4, 7);
                Tetrahedron<3>* top = orig.newTetrahedron();
                orig.tetrahedron(0)->join(2, top, Perm<4>(2, 3, 0, 1));
                orig.tetrahedron(0)->join(3, top, Perm<4>(2, 3, 0, 1));

                Triangulation<3> lst;
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

                Triangulation<3> orig;
                orig.insertConstruction(5, adj, glu);
                Triangulation<3> lst;
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

                Triangulation<3> orig;
                orig.insertConstruction(5, adj, glu);
                Triangulation<3> lens;
                lens.insertLayeredLensSpace(10, 3);
                Triangulation<3> copy(orig);

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

                Triangulation<3> orig;
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

                Triangulation<3> result;
                result.insertConstruction(6, adjResult, gluResult);

                verify20Edge(&orig, 8, &result, "internal-cross");
            }

            {
                // Two copies of baseKB glued along a single face.
                Triangulation<3> t;
                t.insertTriangulation(baseKB);
                t.insertTriangulation(baseKB);
                t.tetrahedron(0)->join(3, t.tetrahedron(2), Perm<4>());

                verify20Edge(&t, 3, &baseKB, "bdry-cross-tet");
            }
        }

        void twoZeroEdgeInvalid() {
            {
                // All four faces joined together in a simple loop.
                Triangulation<3>* s2xs1 =
                    SimpleSurfaceBundle(SimpleSurfaceBundle::S2xS1)
                    .construct();
                verify20EdgeInvalid(s2xs1, "round-loop");
                delete s2xs1;
            }

            {
                // All four faces joined together in a crossed loop.
                Triangulation<3>* s2xs1Twisted =
                    SimpleSurfaceBundle(SimpleSurfaceBundle::S2xS1_TWISTED)
                    .construct();
                verify20EdgeInvalid(s2xs1Twisted, "crossed-loop");
                delete s2xs1Twisted;
            }

            {
                // All four faces internal, but the two equatorial edges
                // both boundary.
                Triangulation<3> t;
                t.insertTriangulation(base);
                Tetrahedron<3>* p = t.newTetrahedron();
                Tetrahedron<3>* q = t.newTetrahedron();
                Tetrahedron<3>* r = t.newTetrahedron();
                Tetrahedron<3>* s = t.newTetrahedron();
                t.tetrahedron(0)->join(2, p, Perm<4>());
                t.tetrahedron(0)->join(3, q, Perm<4>());
                t.tetrahedron(1)->join(2, r, Perm<4>());
                t.tetrahedron(1)->join(3, s, Perm<4>());

                verify20EdgeInvalid(&t, "boundary-edges");
            }

            {
                // All four faces internal, and the two equatorial edges
                // internal but identified (sphere).
                Triangulation<3> t;
                t.insertTriangulation(base);
                Tetrahedron<3>* p = t.newTetrahedron();
                Tetrahedron<3>* q = t.newTetrahedron();
                Tetrahedron<3>* r = t.newTetrahedron();
                Tetrahedron<3>* s = t.newTetrahedron();
                t.tetrahedron(0)->join(2, p, Perm<4>());
                t.tetrahedron(0)->join(3, q, Perm<4>());
                t.tetrahedron(1)->join(2, r, Perm<4>());
                t.tetrahedron(1)->join(3, s, Perm<4>());
                p->join(3, r, Perm<4>());
                q->join(2, s, Perm<4>());

                Edge<3>* e = t.tetrahedron(0)->edge(
                    Edge<3>::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case identified-edges-S2 is malformed.",
                    (! e->isBoundary()) && e->degree() == 6 &&
                        t.isOrientable());

                verify20EdgeInvalid(&t, "identified-edges-S2");
            }

            {
                // All four faces internal, and the two equatorial edges
                // internal but identified (RP2).
                Triangulation<3> t;
                t.insertTriangulation(base);
                Tetrahedron<3>* p = t.newTetrahedron();
                Tetrahedron<3>* q = t.newTetrahedron();
                Tetrahedron<3>* r = t.newTetrahedron();
                Tetrahedron<3>* s = t.newTetrahedron();
                t.tetrahedron(0)->join(2, p, Perm<4>());
                t.tetrahedron(0)->join(3, q, Perm<4>());
                t.tetrahedron(1)->join(2, r, Perm<4>());
                t.tetrahedron(1)->join(3, s, Perm<4>());
                p->join(3, r, Perm<4>(0, 1));
                q->join(2, s, Perm<4>(0, 1));

                Edge<3>* e = t.tetrahedron(0)->edge(
                    Edge<3>::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case identified-edges-RP2 is malformed.",
                    (! e->isBoundary()) && e->degree() == 6 &&
                        ! t.isOrientable());

                verify20EdgeInvalid(&t, "identified-edges-RP2");
            }

            {
                // Two faces boundary, the other joined in a loop.
                Triangulation<3> t;
                t.insertTriangulation(base);
                Tetrahedron<3>* tet = t.tetrahedron(0);
                tet->join(2, tet, Perm<4>(2, 3));

                Edge<3>* e = tet->edge(Edge<3>::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-loop-boundary is malformed.",
                    (! e->isBoundary()) && e->degree() == 1 &&
                        t.isOrientable());

                verify20EdgeInvalid(&t, "boundary-loop-boundary");
            }

            {
                // Two faces boundary, the other joined in a cross.
                Triangulation<3> t;
                t.insertTriangulation(baseKB);

                Edge<3>* e = t.tetrahedron(0)->
                    edge(Edge<3>::edgeNumber[0][1]);
                CPPUNIT_ASSERT_MESSAGE(
                    "Case boundary-cross-boundary is malformed.",
                    e->isBoundary() && e->degree() == 3 &&
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

                Triangulation<3> orig;
                orig.insertConstruction(5, adj, glu);
                verify20EdgeInvalid(&orig, "boundary-opposite-wedge");
            }

            {
                // Two diagonally opposite faces boundary, the other two
                // glued to an LST boundary.  Doesn't really matter how.
                Triangulation<3> orig;
                orig.insertLayeredSolidTorus(3,4);
                orig.insertTriangulation(base);

                Tetrahedron<3>* top = orig.tetrahedron(0);
                orig.tetrahedron(3)->join(3, top, Perm<4>(1,2,0,3));
                orig.tetrahedron(4)->join(2, top, Perm<4>(0,3,2,1));

                verify20EdgeInvalid(&orig, "boundary-diag");
            }
        }
};

void addElementaryMoves(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ElementaryMovesTest::suite());
}

