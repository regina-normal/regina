
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim4.h"

#include "testexhaustive.h"
#include "testhelper.h"

using regina::NormalHypersurface;
using regina::NormalHypersurfaces;
using regina::Triangulation;

static void verifyVertexLinks(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto v : tri.vertices()) {
        SCOPED_TRACE_NUMERIC(v->index());
        NormalHypersurface link = v->linkingSurface();
        EXPECT_EQ(link.isVertexLink(), v);
    }
}

TEST(LinkingHypersurfacesTest, vertexLinks) {
    runCensusAllClosed(&verifyVertexLinks);
    runCensusAllBounded(&verifyVertexLinks);
    runCensusAllNoBdry(&verifyVertexLinks);
}

static void verifyEdgeLinks(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Verify that *all* edge links look reasonable.

    regina::MatrixInt matching = regina::makeMatchingEquations(
        tri, regina::HyperCoords::Standard);

    for (auto e : tri.edges()) {
        SCOPED_TRACE_NUMERIC(e->index());
        auto [ link, thin ] = e->linkingSurface();

        EXPECT_TRUE((matching * link.vector()).isZero());

        EXPECT_EQ(link.edgeWeight(e->index()), 0);
        for (size_t i = 0; i < tri.countEdges(); ++i)
            EXPECT_LE(link.edgeWeight(i), 2);

        {
            const regina::Edge<4>* found = link.isThinEdgeLink();
            EXPECT_EQ(thin, e == found);
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

    // Check the precise coordinates for all *thin* edge links, which
    // can be separately obtained via normal hypersurface enumeration.

    NormalHypersurfaces list(tri, regina::HyperCoords::Standard);
    for (const auto& s : list) {
        SCOPED_TRACE_REGINA(s);
        const regina::Edge<4>* link = s.isThinEdgeLink();
        if (link)
            EXPECT_EQ(s, link->linkingSurface().first);
    }
}

TEST(LinkingHypersurfacesTest, edgeLinks) {
    runCensusAllClosed(&verifyEdgeLinks);
    runCensusAllBounded(&verifyEdgeLinks);
    runCensusAllNoBdry(&verifyEdgeLinks);
}

static void verifyTriangleLinks(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Verify that *all* triangle links look reasonable.

    regina::MatrixInt matching = regina::makeMatchingEquations(
        tri, regina::HyperCoords::Standard);

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

TEST(LinkingHypersurfacesTest, triangleLinks) {
    runCensusAllClosed(&verifyTriangleLinks);
    runCensusAllBounded(&verifyTriangleLinks);
    runCensusAllNoBdry(&verifyTriangleLinks);
}

static void verifyTetrahedronLinks(const Triangulation<4>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Verify that *all* triangle links look reasonable.

    regina::MatrixInt matching = regina::makeMatchingEquations(
        tri, regina::HyperCoords::Standard);

    for (auto t : tri.tetrahedra()) {
        SCOPED_TRACE_NUMERIC(t->index());
        auto [ link, thin ] = t->linkingSurface();

        EXPECT_TRUE((matching * link.vector()).isZero());

        for (int i = 0; i < 6; ++i)
            EXPECT_EQ(link.edgeWeight(t->edge(i)->index()), 0);
        for (size_t i = 0; i < tri.countEdges(); ++i)
            EXPECT_LE(link.edgeWeight(i), 2);

        {
            auto found = link.isThinTetrahedronLink();
            EXPECT_EQ(thin, t == found.first || t == found.second);
        }
        {
            auto [ allTet, nThin ] = link.isNormalTetrahedronLink();

            auto found = std::find(allTet.begin(), allTet.end(), t);
            EXPECT_NE(found, allTet.end());
            if (thin)
                EXPECT_LT(found - allTet.begin(), nThin);
            else
                EXPECT_GE(found - allTet.begin(), nThin);
        }
    }
}

TEST(LinkingHypersurfacesTest, tetrahedronLinks) {
    runCensusAllClosed(&verifyTetrahedronLinks);
    runCensusAllBounded(&verifyTetrahedronLinks);
    runCensusAllNoBdry(&verifyTetrahedronLinks);
}
