
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testsuite/algebra/testalgebra.h"

using regina::Integer;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;
using regina::VectorInt;

class MarkedAbelianGroupTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MarkedAbelianGroupTest);

    CPPUNIT_TEST(lst);
    CPPUNIT_TEST(kleinBottle);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void lst() {
            Triangulation<3> t = regina::Example<3>::lst(3, 5);

            MarkedAbelianGroup g = t.markedHomology();
            if (! g.isZ()) {
                std::ostringstream msg;
                msg << "LST has incorrect H1 = " << g.str() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            regina::Edge<3>* e = t.simplex(0)->edge(0);
            if (! (e->isBoundary() && e->degree() == 1)) {
                std::ostringstream msg;
                msg << "LST has the degree 1 edge in an unexpected "
                    "location.";
                CPPUNIT_FAIL(msg.str());
            }

            VectorInt snf = g.snfRep(VectorInt::unit(
                t.countEdges(), e->index()));
            if (snf.size() != 1) {
                std::ostringstream msg;
                msg << "H1(LST) gives SNF reps of the wrong size: "
                    << snf.size();
                CPPUNIT_FAIL(msg.str());
            }
            if (snf[0] != 8 && snf[0] != -8) {
                std::ostringstream msg;
                msg << "H1(LST) has the degree 1 edge with the wrong "
                    "SNF rep: " << snf[0];
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyNonCycle(const MarkedAbelianGroup& g,
                const VectorInt& v) {
            if (g.isCycle(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a cycle.";
                CPPUNIT_FAIL(msg.str());
            }
            if (g.isBoundary(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a boundary.";
                CPPUNIT_FAIL(msg.str());
            }
            if (g.cycleProjection(v) == v) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly projects "
                    << v << " onto itself via cycleProjection().";
                CPPUNIT_FAIL(msg.str());
            }
            try {
                g.snfRep(v);
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not throw an exception "
                    "when writing " << v << " in SNF coordinates.";
                CPPUNIT_FAIL(msg.str());
            } catch (const regina::InvalidArgument&) {
            }
            try {
                g.asBoundary(v);
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not throw an exception "
                    "when writing " << v << " as a boundary.";
                CPPUNIT_FAIL(msg.str());
            } catch (const regina::InvalidArgument&) {
            }
        }

        static void verifyCycleNonBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v) {
            if (! g.isCycle(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a non-cycle.";
                CPPUNIT_FAIL(msg.str());
            }
            if (g.isBoundary(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a boundary.";
                CPPUNIT_FAIL(msg.str());
            }
            if (g.cycleProjection(v) != v) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not project "
                    << v << " onto itself via cycleProjection().";
                CPPUNIT_FAIL(msg.str());
            }
            try {
                g.asBoundary(v);
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not throw an exception "
                    "when writing " << v << " as a boundary.";
                CPPUNIT_FAIL(msg.str());
            } catch (const regina::InvalidArgument&) {
            }
        }

        static void verifyCycleNonBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v, const VectorInt& snf) {
            verifyCycleNonBoundary(g, v);

            if (g.snfRep(v) != snf) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly computes "
                    "snfRep(" << v << ") as " << g.snfRep(v) << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v, const VectorInt& bdry) {
            if (! g.isCycle(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a non-cycle.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! g.isBoundary(v)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly identifies "
                    << v << " as a non-boundary.";
                CPPUNIT_FAIL(msg.str());
            }
            if (g.cycleProjection(v) != v) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not project "
                    << v << " onto itself via cycleProjection().";
                CPPUNIT_FAIL(msg.str());
            }
            if (! g.snfRep(v).isZero()) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) does not give zero snfRep() for "
                    << v << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (g.asBoundary(v) != bdry) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) incorrectly computes "
                    "asBoundary(" << v << ") as " << g.asBoundary(v) << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void kleinBottle() {
            Triangulation<2> t = Triangulation<2>::fromGluings(4, {
                { 0, 0, 1, {} },
                { 0, 1, 3, {0,2} },
                { 0, 2, 3, {} },
                { 1, 1, 2, {} },
                { 1, 2, 2, {0,1} },
                { 2, 0, 3, {} },
            });

            MatrixInt b1 = t.boundaryMap<1>();
            MatrixInt b2 = t.boundaryMap<2>();

            if (b1 != MatrixInt({{-1,0,1,0,1,1}, {1,0,-1,0,-1,-1}})) {
                std::ostringstream msg;
                msg << "Klein bottle gives the wrong boundary map "
                    "for edges: " << b1.detail();
                CPPUNIT_FAIL(msg.str());
            }
            if (b2 != MatrixInt({{1,0,0,1}, {-1,0,0,1}, {1,1,0,0},
                    {0,1,-1,0}, {0,-1,-1,0}, {0,0,1,1}})) {
                std::ostringstream msg;
                msg << "Klein bottle gives the wrong boundary map "
                    "for triangles: " << b2.detail();
                CPPUNIT_FAIL(msg.str());
            }

            MarkedAbelianGroup g = t.markedHomology();
            if (g.str() != "Z + Z_2 (Z^4 -> Z^6 -> Z^2)") {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong homology: " << g.str();
                CPPUNIT_FAIL(msg.str());
            }

            if (g.rank() != 1) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong rank: " << g.rank();
                CPPUNIT_FAIL(msg.str());
            }
            if (g.torsionRank(2) != 1) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong Z_2 rank: "
                    << g.torsionRank(2);
                CPPUNIT_FAIL(msg.str());
            }
            if (g.snfRank() != 2) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong SNF dimension: "
                    << g.snfRank();
                CPPUNIT_FAIL(msg.str());
            }
            if (g.ccRank() != 6) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong chain complex rank: "
                    << g.ccRank();
                CPPUNIT_FAIL(msg.str());
            }
            if (g.cycleRank() != 5) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong kernel rank: "
                    << g.cycleRank();
                CPPUNIT_FAIL(msg.str());
            }

            if (g.freeRep(0) != VectorInt({1,0,0,0,0,1})) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong free generator: "
                    << g.freeRep(0);
                CPPUNIT_FAIL(msg.str());
            }
            if (g.torsionRep(0) != VectorInt({1,0,0,0,1,0})) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong torsion generator: "
                    << g.torsionRep(0);
                CPPUNIT_FAIL(msg.str());
            }
            if (g.ccRep(0) != g.torsionRep(0)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong first SNF generator: "
                    << g.ccRep(0);
                CPPUNIT_FAIL(msg.str());
            }
            if (g.ccRep(1) != g.freeRep(0)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong second SNF generator: "
                    << g.ccRep(1);
                CPPUNIT_FAIL(msg.str());
            }
            if (g.ccRep({1,-1}) != g.torsionRep(0) - g.freeRep(0)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) gives the wrong "
                    "cycle representation for {1,-1}: " << g.ccRep({1,-1});
                CPPUNIT_FAIL(msg.str());
            }

            // It seems cycleProjection() is adjusting the 0th chain
            // complex coordinate to produce a cycle, though this
            // particular choice of adjustment is not guaranteed.
            for (int i = 0; i < 6; ++i) {
                auto p = g.cycleProjection(i);
                if (i == 1 || i == 3) {
                    // This edge is a cycle.
                    if (p != VectorInt::unit(6, i)) {
                        std::ostringstream msg;
                        msg << "H1(Klein bottle).cycleProjection(" << i
                            << ") gives the wrong projection: " << p;
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (! g.isCycle(VectorInt::unit(6, i))) {
                        std::ostringstream msg;
                        msg << "H1(Klein bottle) does not recognise "
                            "edge " << i << " as a cycle.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (i == 0) {
                    // This edge is not a cycle.
                    if (! p.isZero()) {
                        std::ostringstream msg;
                        msg << "H1(Klein bottle).cycleProjection(" << i
                            << ") gives the wrong projection: " << p;
                        CPPUNIT_FAIL(msg.str());
                    }
                    verifyNonCycle(g, VectorInt::unit(6, i));
                } else {
                    // This edge is not a cycle.
                    if (p != VectorInt::unit(6, i) + VectorInt::unit(6, 0)) {
                        std::ostringstream msg;
                        msg << "H1(Klein bottle).cycleProjection(" << i
                            << ") gives the wrong projection: " << p;
                        CPPUNIT_FAIL(msg.str());
                    }
                    verifyNonCycle(g, VectorInt::unit(6, i));
                }
                if (! g.isCycle(p)) {
                    std::ostringstream msg;
                    msg << "H1(Klein bottle).cycleProjection(" << i
                        << ") is not recognised as a cycle.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (g.cycleProjection({3,-1,0,0,2,-4}) !=
                    VectorInt({-2,-1,0,0,2,-4})) {
                std::ostringstream msg;
                msg << "H1(Klein bottle).cycleProjection() gives the "
                    "wrong projection for a mixed vector.";
                CPPUNIT_FAIL(msg.str());
            }

            // Examine some chains that are not cycles:
            verifyNonCycle(g, {1,0,0,0,0,0});
            verifyNonCycle(g, {1,0,0,0,-1,0});

            // Examine some cycles that are not boundaries:
            verifyCycleNonBoundary(g, {1,0,0,0,1,0}, {1,0});
            verifyCycleNonBoundary(g, {3,0,0,0,3,0}, {1,0});
            verifyCycleNonBoundary(g, {1,0,0,0,0,1}, {0,1});
            verifyCycleNonBoundary(g, {0,1,0,0,0,0}, {0,-1});
            verifyCycleNonBoundary(g, {0,3,0,0,0,0}, {0,-3});
            verifyCycleNonBoundary(g, {1,1,1,0,0,0}, {0,-2});

            // Examine some cycles that are boundaries:
            verifyBoundary(g, {0,0,0,0,0,0}, {0,0,0,0});
            verifyBoundary(g, {2,0,0,0,2,0}, {1,-1,-1,1}); // 2x Z_2 gen
            verifyBoundary(g, {4,0,0,0,4,0}, {2,-2,-2,2}); // 4x Z_2 gen
            verifyBoundary(g, {1,-1,1,0,0,0}, {1,0,0,0}); // Bdry of T0
            verifyBoundary(g, {0,0,1,0,-2,1}, {0,1,1,0}); // Bdry of T1 u T2

            for (unsigned long i = 0; i < g.cycleRank(); ++i)
                verifyCycleNonBoundary(g, g.cycleGen(i));


            MarkedAbelianGroup tor = g.torsionSubgroup();
            if (tor.unmarked().str() != "Z_2") {
                std::ostringstream msg;
                msg << "H1(Klein bottle) has wrong torsion subgroup: "
                    << tor.unmarked().str();
                CPPUNIT_FAIL(msg.str());
            }

            regina::HomMarkedAbelianGroup hom = g.torsionInclusion();
            MatrixInt m = hom.reducedMatrix();
            if (! (m.rows() == 2 && m.columns() == 1 &&
                    m.entry(0,0) == 1 && m.entry(1,0) == 0)) {
                std::ostringstream msg;
                msg << "H1(Klein bottle) has wrong torsion inclusion: "
                    << m.detail() << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }
};

void addMarkedAbelianGroup(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(MarkedAbelianGroupTest::suite());
}

