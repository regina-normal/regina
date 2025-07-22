
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

#include "testexhaustive.h"
#include "testhelper.h"

using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Triangulation;

static void verifyVertexLinks(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto v : tri.vertices()) {
        SCOPED_TRACE_NUMERIC(v->index());
        NormalSurface link = v->linkingSurface();
        EXPECT_EQ(link.isVertexLink(), v);
    }
}

TEST(LinkingSurfacesTest, vertexLinks) {
    runCensusAllClosed(&verifyVertexLinks);
    runCensusAllBounded(&verifyVertexLinks);
    runCensusAllIdeal(&verifyVertexLinks);
}

static void verifyEdgeLinks(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Verify that *all* edge links look reasonable.

    regina::MatrixInt matching = regina::makeMatchingEquations(
        tri, regina::NormalCoords::Standard);

    for (auto e : tri.edges()) {
        SCOPED_TRACE_NUMERIC(e->index());
        auto [ link, thin ] = e->linkingSurface();

        EXPECT_TRUE((matching * link.vector()).isZero());

        EXPECT_EQ(link.edgeWeight(e->index()), 0);
        for (size_t i = 0; i < tri.countEdges(); ++i)
            EXPECT_LE(link.edgeWeight(i), 2);

        {
            auto found = link.isThinEdgeLink();
            EXPECT_EQ(thin, e == found.first || e == found.second);
            // Note: it is possible for found to be non-null even if thin is
            // false - that is, a surface can simultaneously be both a thin
            // and thick edge link.  See for instance isosig dLQbcbcaefv.
        }
        {
            auto [ allEdges, nThin ] = link.isNormalEdgeLink();

            auto found = std::find(allEdges.begin(), allEdges.end(), e);
            EXPECT_NE(found, allEdges.end());
            if (thin)
                EXPECT_LT(found - allEdges.begin(), nThin);
            else
                EXPECT_GE(found - allEdges.begin(), nThin);
        }
    }

    // Check the precise coordinates for all *thin* edge links,
    // which can be separately obtained via normal surface enumeration.

    NormalSurfaces list(tri, regina::NormalCoords::Standard);
    for (const auto& s : list) {
        SCOPED_TRACE_REGINA(s);
        auto link = s.isThinEdgeLink();
        if (link.first) {
            if (s.isTwoSided()) {
                EXPECT_EQ(s, link.first->linkingSurface().first);
                if (link.second)
                    EXPECT_EQ(s, link.second->linkingSurface().first);
            } else {
                // A one-sided surface can only link a single edge.
                EXPECT_EQ(s + s, link.first->linkingSurface().first);
                EXPECT_EQ(link.second, nullptr);
            }
        }
    }
}

TEST(LinkingSurfacesTest, edgeLinks) {
    runCensusAllClosed(&verifyEdgeLinks);
    runCensusAllBounded(&verifyEdgeLinks);
    runCensusAllIdeal(&verifyEdgeLinks);

    // A case where several interesting things happen.
    {
        Triangulation<3> tri = Triangulation<3>::fromIsoSig("dLQbcbcaefv");

        // There are five edges.
        // - Edges 2, 3, 4 all have thin edge links.
        // - Edge 0 has a thick edge link that is the same as the
        //   *thin* edge link from edge 4.
        // - Edge 1 has a link that normalises away to nothing.

        // We unroll what should be a for loop here, since NormalSurface does
        // not have a default constructor.
        std::pair<NormalSurface, bool> link[5] {
            tri.edge(0)->linkingSurface(),
            tri.edge(1)->linkingSurface(),
            tri.edge(2)->linkingSurface(),
            tri.edge(3)->linkingSurface(),
            tri.edge(4)->linkingSurface() };

        EXPECT_FALSE(link[0].second);
        EXPECT_EQ(link[0].first, link[4].first);

        EXPECT_FALSE(link[1].second);
        EXPECT_TRUE(link[1].first.isEmpty());

        for (int i = 2; i < 5; ++i) {
            EXPECT_TRUE(link[i].second);
            EXPECT_EQ(link[i].first.isThinEdgeLink().first, tri.edge(i));
        }
    }
}

static void verifyTriangleLinks(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Verify that *all* triangle links look reasonable.

    regina::MatrixInt matching = regina::makeMatchingEquations(
        tri, regina::NormalCoords::Standard);

    for (auto t : tri.triangles()) {
        SCOPED_TRACE_NUMERIC(t->index());
        auto [ link, thin ] = t->linkingSurface();

        EXPECT_TRUE((matching * link.vector()).isZero());

        for (int i = 0; i < 3; ++i)
            EXPECT_EQ(link.edgeWeight(t->edge(i)->index()), 0);
        for (size_t i = 0; i < tri.countEdges(); ++i)
            EXPECT_LE(link.edgeWeight(i), 2);

        {
            auto found = link.isThinTriangleLink();
            EXPECT_EQ(thin, t == found.first || t == found.second);
        }
        {
            auto [ allTri, nThin ] = link.isNormalTriangleLink();

            auto found = std::find(allTri.begin(), allTri.end(), t);
            EXPECT_NE(found, allTri.end());
            if (thin)
                EXPECT_LT(found - allTri.begin(), nThin);
            else
                EXPECT_GE(found - allTri.begin(), nThin);
        }
    }
}

TEST(LinkingSurfacesTest, triangleLinks) {
    runCensusAllClosed(&verifyTriangleLinks);
    runCensusAllBounded(&verifyTriangleLinks);
    runCensusAllIdeal(&verifyTriangleLinks);
}
