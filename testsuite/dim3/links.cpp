
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "hypersurface/normalhypersurfaces.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"

#include "testsuite/exhaustive.h"
#include "testsuite/dim3/testdim3.h"
#include "testsuite/dim4/testdim4.h"

using regina::Edge;
using regina::NormalHypersurface;
using regina::NormalHypersurfaces;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Tetrahedron;
using regina::Triangle;
using regina::Triangulation;
using regina::Vertex;

class LinkingSurfacesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkingSurfacesTest);

    CPPUNIT_TEST(vertexLinks3);
    CPPUNIT_TEST(edgeLinks3);
    CPPUNIT_TEST(triangleLinks3);
    CPPUNIT_TEST(vertexLinks4);
    CPPUNIT_TEST(edgeLinks4);
    CPPUNIT_TEST(triangleLinks4);
    CPPUNIT_TEST(tetrahedronLinks4);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        static void testVertexLinks3(const Triangulation<3>& tri,
                const char* name) {
            for (auto v : tri.vertices()) {
                NormalSurface link = v->linkingSurface();
                if (link.isVertexLink() != v) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", vertex " << v->index() << ": linking surface "
                        << link.vector() << " is not recognised as the "
                        "correct vertex link.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void vertexLinks3() {
            runCensusAllClosed(&testVertexLinks3);
            runCensusAllBounded(&testVertexLinks3);
            runCensusAllIdeal(&testVertexLinks3);
        }

        static void testEdgeLinks3(const Triangulation<3>& tri,
                const char* name) {
            // Verify that *all* edge links look reasonable.

            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::NS_STANDARD);

            for (auto e : tri.edges()) {
                auto [ link, thin ] = e->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", edge " << e->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (auto e2 : tri.edges()) {
                    auto w = link.edgeWeight(e2->index());
                    if (w > 2) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " has an edge weight "
                            "greater than 2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (e2 == e && w != 0) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " meets the edge that "
                            "it is supposed to link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                /*
                auto found = link.isThinEdgeLink();
                if (found.first) {
                    if (found.first != e && found.second != e) {
                        // This failure *could* be triggered if a surface
                        // is simultaneously a thin edge link and also a
                        // thick edge link.  Indeed, this is possible:
                        // see for instance isosig dLQbcbcaefv.
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " is recognised as a "
                            "thin edge link, but not of the edge in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                */

                {
                    auto [ allEdges, nThin] = link.isNormalEdgeLink();

                    auto found = std::find(allEdges.begin(), allEdges.end(), e);
                    if (found == allEdges.end()) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " is not recognised as a "
                            "normalised edge link of the edge in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (thin) {
                        if (found - allEdges.begin() >= nThin) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a non-thin normalised "
                                "link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (found - allEdges.begin() < nThin) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin normalised "
                                "link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
                {
                    auto found = link.isThinEdgeLink();
                    if (thin) {
                        if (e != found.first && e != found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is not recognised as a "
                                "thin edge link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (e == found.first || e == found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin edge link of the "
                                "edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }

            // Check the precise coordinates for all *thin* edge links,
            // which can be separately obtained via normal surface enumeration.

            NormalSurfaces list(tri, regina::NS_STANDARD);
            for (const auto& s : list) {
                auto link = s.isThinEdgeLink();
                if (link.first) {
                    if (! s.isTwoSided()) {
                        if (s + s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated one-sided thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else  {
                        if (s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }

                    if (link.second) {
                        // If the surface is the thin link of two
                        // distinct edges then it must be two-sided.
                        if (s != link.second->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.second->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
        }

        void edgeLinks3() {
            runCensusAllClosed(&testEdgeLinks3);
            runCensusAllBounded(&testEdgeLinks3);
            runCensusAllIdeal(&testEdgeLinks3);

            // A case where several interesting things happen.
            {
                Triangulation<3> tri =
                    Triangulation<3>::fromIsoSig("dLQbcbcaefv");

                // There are five edges.
                // - Edges 2, 3, 4 all have thin edge links.
                // - Edge 0 has a thick edge link that is the same as the
                //   *thin* edge link from edge 4.
                // - Edge 1 has a link that normalises away to nothing.

                // We unroll what should be a for loop here, since
                // NormalSurface does not have a default constructor.
                std::pair<NormalSurface, bool> link[5] {
                    tri.edge(0)->linkingSurface(),
                    tri.edge(1)->linkingSurface(),
                    tri.edge(2)->linkingSurface(),
                    tri.edge(3)->linkingSurface(),
                    tri.edge(4)->linkingSurface() };

                for (int i = 2; i < 5; ++i) {
                    if ((! link[i].second) ||
                            (link[i].first.isThinEdgeLink().first !=
                            tri.edge(i))) {
                        std::ostringstream msg;
                        msg << "Triangulation dLQbcbcaefv, edge " << i << ": "
                            "linking surface is not a thin edge link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                if (link[0].second || link[0].first != link[4].first) {
                    std::ostringstream msg;
                    msg << "Triangulation dLQbcbcaefv, edge 0: "
                        "linking surface is not think and also the "
                        "thin link of edge 4, as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (link[1].second || ! link[1].first.isEmpty()) {
                    std::ostringstream msg;
                    msg << "Triangulation dLQbcbcaefv, edge 1: "
                        "linking surface is not thick and empty, as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void testTriangleLinks3(const Triangulation<3>& tri,
                const char* name) {
            // Verify that *all* triangle links look reasonable.

            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::NS_STANDARD);

            for (auto t : tri.triangles()) {
                auto [ link, thin ] = t->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", triangle " << t->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (auto e : tri.edges()) {
                    auto w = link.edgeWeight(e->index());
                    if (w > 2) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", triangle " << t->index() << ": linking surface "
                            << link.vector() << " has an edge weight "
                            "greater than 2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if ((e == t->edge(0) || e == t->edge(1) || e == t->edge(2))
                            && w != 0) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", triangle " << t->index() << ": linking surface "
                            << link.vector() << " meets an edge of the "
                            "triangle that it is supposed to link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                {
                    auto [ allTri, nThin] = link.isNormalTriangleLink();

                    auto found = std::find(allTri.begin(), allTri.end(), t);
                    if (found == allTri.end()) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", triangle " << t->index() << ": linking surface "
                            << link.vector() << " is not recognised as a "
                            "normalised link of the triangle in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (thin) {
                        if (found - allTri.begin() >= nThin) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", triangle " << t->index()
                                << ": linking surface " << link.vector()
                                << " is incorrectly "
                                "recognised as a non-thin normalised "
                                "link of the triangle in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (found - allTri.begin() < nThin) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", triangle " << t->index()
                                << ": linking surface " << link.vector()
                                << " is incorrectly "
                                "recognised as a thin normalised "
                                "link of the triangle in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
                /*
                {
                    auto found = link.isThinEdgeLink();
                    if (thin) {
                        if (e != found.first && e != found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is not recognised as a "
                                "thin edge link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (e == found.first || e == found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin edge link of the "
                                "edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
                */
            }
        }

        void triangleLinks3() {
            runCensusAllClosed(&testTriangleLinks3);
            runCensusAllBounded(&testTriangleLinks3);
            runCensusAllIdeal(&testTriangleLinks3);
        }

        static void testVertexLinks4(const Triangulation<4>& tri,
                const char* name) {
            for (auto v : tri.vertices()) {
                NormalHypersurface link = v->linkingSurface();
                if (link.isVertexLink() != v) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", vertex " << v->index() << ": linking hypersurface "
                        << link.vector() << " is not recognised as the "
                        "correct vertex link.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void vertexLinks4() {
            runCensusAllClosed(&testVertexLinks4);
            runCensusAllBounded(&testVertexLinks4);
            runCensusAllNoBdry(&testVertexLinks4);
        }

        static void testEdgeLinks4(const Triangulation<4>& tri,
                const char* name) {
            // Verify that *all* edge links look reasonable.

            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::HS_STANDARD);

            for (auto e : tri.edges()) {
                auto [ link, thin ] = e->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", edge " << e->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (auto e2 : tri.edges()) {
                    auto w = link.edgeWeight(e2->index());
                    if (w > 2) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " has an edge weight "
                            "greater than 2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (e2 == e && w != 0) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " meets the edge that "
                            "it is supposed to link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                /*
                {
                    auto found = link.isNormalEdgeLink();
                    if (std::find(found.begin(), found.end(), e) ==
                            found.end()) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " is not recognised as a "
                            "normalised edge link of the edge in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                */
                {
                    const Edge<4>* found = link.isThinEdgeLink();
                    if (thin) {
                        if (e != found) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is not recognised as a "
                                "thin edge link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (e == found) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin edge link of the "
                                "edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }

            // Check the precise coordinates for all *thin* edge links, which
            // can be separately obtained via normal hypersurface enumeration.

            NormalHypersurfaces list(tri, regina::HS_STANDARD);
            for (const auto& s : list) {
                const Edge<4>* link = s.isThinEdgeLink();
                if (link) {
                    if (s != link->linkingSurface().first) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << link->index() <<
                            ": linking surface does not match the "
                            "enumerated thin edge link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void edgeLinks4() {
            runCensusAllClosed(&testEdgeLinks4);
            runCensusAllBounded(&testEdgeLinks4);
            runCensusAllNoBdry(&testEdgeLinks4);
        }

        static void testTriangleLinks4(const Triangulation<4>& tri,
                const char* name) {
            // Verify that *all* triangle links look reasonable.

            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::HS_STANDARD);

            for (auto t : tri.triangles()) {
                auto [ link, thin ] = t->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", triangle " << t->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (auto e : tri.edges()) {
                    auto w = link.edgeWeight(e->index());
                    if (w > 2) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", triangle " << t->index() << ": linking surface "
                            << link.vector() << " has an edge weight "
                            "greater than 2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if ((e == t->edge(0) || e == t->edge(1) || e == t->edge(2))
                            && w != 0) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", triangle " << t->index() << ": linking surface "
                            << link.vector() << " meets an edge of the "
                            "triangle that it is supposed to link.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                /*
                {
                    auto found = link.isNormalEdgeLink();
                    if (std::find(found.begin(), found.end(), e) ==
                            found.end()) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " is not recognised as a "
                            "normalised edge link of the edge in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                {
                    auto found = link.isThinEdgeLink();
                    if (thin) {
                        if (e != found.first && e != found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is not recognised as a "
                                "thin edge link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (e == found.first || e == found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin edge link of the "
                                "edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
                */
            }

            /*
            // Check the precise coordinates for all *thin* triangle links,
            // which can be separately obtained via normal surface enumeration.

            NormalSurfaces list(tri, regina::NS_STANDARD);
            for (const auto& s : list) {
                auto link = s.isThinEdgeLink();
                if (link.first) {
                    if (! s.isTwoSided()) {
                        if (s + s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated one-sided thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else  {
                        if (s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }

                    if (link.second) {
                        // If the surface is the thin link of two
                        // distinct edges then it must be two-sided.
                        if (s != link.second->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.second->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
            */
        }

        void triangleLinks4() {
            runCensusAllClosed(&testTriangleLinks4);
            runCensusAllBounded(&testTriangleLinks4);
            runCensusAllNoBdry(&testTriangleLinks4);
        }

        static void testTetrahedronLinks4(const Triangulation<4>& tri,
                const char* name) {
            // Verify that *all* triangle links look reasonable.

            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::HS_STANDARD);

            for (auto t : tri.tetrahedra()) {
                auto [ link, thin ] = t->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", tetrahedron " << t->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (auto e : tri.edges()) {
                    auto w = link.edgeWeight(e->index());
                    if (w > 2) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name << ", tetrahedron "
                            << t->index() << ": linking surface "
                            << link.vector() << " has an edge weight "
                            "greater than 2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (w != 0)
                        for (int i = 0; i < 6; ++i)
                            if (e == t->edge(i)) {
                                std::ostringstream msg;
                                msg << "Triangulation " << name <<
                                    ", tetrahedron " << t->index() <<
                                    ": linking surface " << link.vector() <<
                                    " meets an edge of the tetrahedron "
                                    "that it is supposed to link.";
                                CPPUNIT_FAIL(msg.str());
                            }
                }

                /*
                {
                    auto found = link.isNormalEdgeLink();
                    if (std::find(found.begin(), found.end(), e) ==
                            found.end()) {
                        std::ostringstream msg;
                        msg << "Triangulation " << name <<
                            ", edge " << e->index() << ": linking surface "
                            << link.vector() << " is not recognised as a "
                            "normalised edge link of the edge in question.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                {
                    auto found = link.isThinEdgeLink();
                    if (thin) {
                        if (e != found.first && e != found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is not recognised as a "
                                "thin edge link of the edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (e == found.first || e == found.second) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << e->index() << ": linking surface "
                                << link.vector() << " is incorrectly "
                                "recognised as a thin edge link of the "
                                "edge in question.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
                */
            }

            /*
            // Check the precise coordinates for all *thin* triangle links,
            // which can be separately obtained via normal surface enumeration.

            NormalSurfaces list(tri, regina::NS_STANDARD);
            for (const auto& s : list) {
                auto link = s.isThinEdgeLink();
                if (link.first) {
                    if (! s.isTwoSided()) {
                        if (s + s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated one-sided thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else  {
                        if (s != link.first->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.first->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }

                    if (link.second) {
                        // If the surface is the thin link of two
                        // distinct edges then it must be two-sided.
                        if (s != link.second->linkingSurface().first) {
                            std::ostringstream msg;
                            msg << "Triangulation " << name <<
                                ", edge " << link.second->index() <<
                                ": linking surface does not match the "
                                "enumerated thin edge link.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
            */
        }

        void tetrahedronLinks4() {
            runCensusAllClosed(&testTetrahedronLinks4);
            runCensusAllBounded(&testTetrahedronLinks4);
            runCensusAllNoBdry(&testTetrahedronLinks4);
        }
};

void addLinkingSurfaces(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkingSurfacesTest::suite());
}

