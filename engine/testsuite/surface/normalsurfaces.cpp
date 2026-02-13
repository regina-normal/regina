
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

#include "enumerate/treetraversal.h"
#include "manifold/snappeacensusmfd.h"
#include "split/signature.h"
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testhelper.h"
#include "testexhaustive.h"

using regina::Example;
using regina::NormalAlg;
using regina::NormalCoords;
using regina::NormalList;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Triangulation;

/**
 * Run a given test over a handful of example triangulations.
 *
 * These are the same triangulations that are examined in detail in individual
 * tests below (singleTetrahedron, sphere, loopC2, etc.); see those individual
 * tests for explanations of where the triangulations come from.
 */
static void testManualCases(void (*f)(const Triangulation<3>&, const char*),
        bool ideal = true) {
    f(Triangulation<3>(), "Empty triangulation");
    f(Example<3>::ball(), "Single tetrahedron");
    f(Example<3>::sphere(), "Sphere");
    f(Example<3>::layeredLoop(2, false), "C(2)");
    f(Example<3>::layeredLoop(3, true), "C~(3)");
    f(regina::Signature("abcd.abe.c.d.e").triangulate(), "Large S3");
    f(regina::Signature("aabcd.be.c.d.e").triangulate(), "Large RP3");
    f(Triangulation<3>::fromGluings(3, {
        { 0, 0, 1, {0,1,2,3} }, { 0, 1, 2, {2,1,0,3} }, { 0, 2, 2, {1,3,2,0} },
        { 1, 1, 2, {0,3,2,1} }, { 1, 2, 2, {3,1,0,2} }}), "Twisted KxI");
    f(Triangulation<3>::rehydrate("jnnafaabcfighhihimgbpqpepbr"),
        "SFS [RP2: (2,1) (2,1) (2,1)]");

    if (ideal) {
        f(Example<3>::gieseking(), "Gieseking");
        f(Example<3>::figureEight(), "Figure eight");
    }
}

TEST(NormalSurfacesTest, defaultArgs) {
    // Make sure that enumeration using default arguments falls through to the
    // correct enumeration function, which takes list and algorithm flags.

    Triangulation<3> t = Example<3>::ball();

    NormalSurfaces l1(t, NormalCoords::Quad);
    EXPECT_EQ(l1.which(), (NormalList::Vertex | NormalList::EmbeddedOnly));

    NormalSurfaces l2(t, NormalCoords::Quad, NormalList::ImmersedSingular);
    EXPECT_EQ(l2.which(), (NormalList::Vertex | NormalList::ImmersedSingular));
}

// Use std::tuple for the free comparison operators.
using CompactProfile = std::tuple<
    regina::Integer /* euler char */,
    bool    /* orientable? */,
    bool    /* two-sided? */,
    bool    /* has real boundary? */,
    bool    /* vertex link? */,
    int     /* edge link (number of edges) */,
    size_t  /* central (number of discs) */,
    bool    /* splitting? */ >;

static std::vector<CompactProfile> sortedCompactProfiles(
        const NormalSurfaces& list) {
    std::vector<CompactProfile> found;
    found.reserve(list.size());
    for (const auto& s : list) {
        EXPECT_TRUE(s.isCompact());
        EXPECT_TRUE(s.isConnected());
        auto edgeLinks = s.isThinEdgeLink();
        found.push_back({ s.eulerChar(),
            s.isOrientable(), s.isTwoSided(), s.hasRealBoundary(),
            s.isVertexLinking(), (edgeLinks.first == nullptr ? 0 :
                edgeLinks.second == nullptr ? 1 : 2),
            s.isCentral(), s.isSplitting() });
    }
    std::sort(found.begin(), found.end());
    return found;
}

template <typename Profile, bool checkIndividualProfiles = true>
using ExpectProfiles = std::conditional_t<checkIndividualProfiles,
    std::initializer_list<Profile> /* individual profiles */,
    size_t /* number of surfaces only */>;

template <bool individualStd = true, bool individualANStd = true>
static void compareCompactProfiles(const Triangulation<3>& tri,
        ExpectProfiles<CompactProfile> expectQuad,
        ExpectProfiles<CompactProfile, individualStd> extraStd,
        ExpectProfiles<CompactProfile, individualANStd> extraANStd) {
    // extraANStd should imply extraStd.
    static_assert(individualStd || ! individualANStd);

    // We assume all surfaces should be compact and connected.
    // All hard-coded "expected solution" lists passed to this function
    // should already be in sorted order.
    std::vector<CompactProfile> expect;
    auto dest = std::back_inserter(expect);
    if constexpr (individualANStd)
        expect.reserve(expectQuad.size() + extraStd.size() + extraANStd.size());
    else if constexpr (individualStd)
        expect.reserve(expectQuad.size() + extraStd.size());
    else
        expect.reserve(expectQuad.size());

    std::copy(expectQuad.begin(), expectQuad.end(), dest);
    {
        SCOPED_TRACE("Quad coordinates");
        NormalSurfaces list(tri, NormalCoords::Quad);
        std::vector<CompactProfile> found = sortedCompactProfiles(list);
        EXPECT_EQ(found, expect);
    }
    {
        SCOPED_TRACE("Standard coordinates");
        if constexpr (individualStd) {
            auto split = expect.end();
            std::copy(extraStd.begin(), extraStd.end(), dest);
            std::inplace_merge(expect.begin(), split, expect.end());
            {
                NormalSurfaces list(tri, NormalCoords::Standard);
                std::vector<CompactProfile> found = sortedCompactProfiles(list);
                EXPECT_EQ(found, expect);
            }
        } else {
            NormalSurfaces list(tri, NormalCoords::Standard);
            EXPECT_EQ(list.size(), expect.size() + extraStd);
        }
    }
    {
        SCOPED_TRACE("Standard almost normal coordinates");
        if constexpr (individualANStd) {
            auto split = expect.end();
            std::copy(extraANStd.begin(), extraANStd.end(), dest);
            std::inplace_merge(expect.begin(), split, expect.end());
            {
                NormalSurfaces list(tri, NormalCoords::AlmostNormal);
                std::vector<CompactProfile> found = sortedCompactProfiles(list);
                EXPECT_EQ(found, expect);
            }
        } else {
            NormalSurfaces list(tri, NormalCoords::AlmostNormal);
            if constexpr (individualStd)
                EXPECT_EQ(list.size(), expect.size() + extraANStd);
            else
                EXPECT_EQ(list.size(), expect.size() + extraStd + extraANStd);
        }
    }
}

TEST(NormalSurfacesTest, empty) {
    SCOPED_TRACE("Empty triangulation");

    Triangulation<3> tri;
    EXPECT_EQ(NormalSurfaces(tri, NormalCoords::Standard).size(), 0);
    EXPECT_EQ(NormalSurfaces(tri, NormalCoords::Quad).size(), 0);
    EXPECT_EQ(NormalSurfaces(tri, NormalCoords::AlmostNormal).size(), 0);
}

TEST(NormalSurfacesTest, singleTetrahedron) {
    SCOPED_TRACE("Single tetrahedron");

    Triangulation<3> tri = Example<3>::ball();
    compareCompactProfiles(tri, {
        { 1, true, true, true, false, 2, 1, true },
        { 1, true, true, true, false, 2, 1, true },
        { 1, true, true, true, false, 2, 1, true }
    }, {
        { 1, true, true, true, true, 0, 1, false },
        { 1, true, true, true, true, 0, 1, false },
        { 1, true, true, true, true, 0, 1, false },
        { 1, true, true, true, true, 0, 1, false }
    }, {
        { 1, true, true, true, false, 0, 1, false },
        { 1, true, true, true, false, 0, 1, false },
        { 1, true, true, true, false, 0, 1, false },
    });
}

TEST(NormalSurfacesTest, sphere) {
    // This is a one-tetrahedron, two-vertex three-sphere.
    SCOPED_TRACE("Sphere");

    Triangulation<3> tri = Example<3>::layeredLoop(1, false);
    compareCompactProfiles(tri, {
        { 0, true, true, false, false, 2, 1, true }
    }, {
        { 2, true, true, false, true, 0, 0, false },
        { 2, true, true, false, true, 0, 0, false }
    }, {
        { 2, true, true, false, false, 0, 1, false }
    });
}

TEST(NormalSurfacesTest, loopC2) {
    SCOPED_TRACE("C(2)");

    Triangulation<3> tri = Example<3>::layeredLoop(2, false);
    compareCompactProfiles(tri, {
        { 0, true, true, false, false, 2, 2, true },
        { 1, false, false, false, false, 1, 2, true },
        { 1, false, false, false, false, 1, 2, true }
    }, {
        { 2, true, true, false, true, 0, 0, false },
        { 2, true, true, false, true, 0, 0, false }
    }, {
        // No additional almost normal surfaces.
    });
}

TEST(NormalSurfacesTest, loopCtw3) {
    SCOPED_TRACE("C~(3)");

    Triangulation<3> tri = Example<3>::layeredLoop(3, true);
    compareCompactProfiles(tri, {
        { 0, false, false, false, false, 1, 3, true },
        { 0, true, true, false, false, 1, 0, false },
        { 0, true, true, false, false, 1, 0, false },
        { 0, true, true, false, false, 1, 0, false }
    }, {
        { 2, true, true, false, true, 0, 0, false }
    }, {
        // No additional almost normal surfaces.
    });
}

TEST(NormalSurfacesTest, largeS3) {
    // A three-vertex five-tetrahedron triangulation of the three-sphere.
    SCOPED_TRACE("Large S3");

    // Too many almost normal surfaces to check individually.
    Triangulation<3> tri = regina::Signature("abcd.abe.c.d.e").triangulate();
    compareCompactProfiles<true, false>(tri, {
        { 0, true, true, false, false, 1, 0, false },
        { 2, true, true, false, false, 0, 0, false },
        { 2, true, true, false, false, 1, 0, false },
        { 2, true, true, false, false, 1, 0, false }
    }, {
        { -2, true, true, false, false, 0, 5, true },
        { 0, true, true, false, false, 0, 0, false },
        { 0, true, true, false, false, 0, 0, false },
        { 0, true, true, false, false, 0, 0, false },
        { 0, true, true, false, false, 1, 0, false },
        { 0, true, true, false, false, 1, 5, false },
        { 2, true, true, false, false, 0, 0, false },
        { 2, true, true, false, false, 0, 0, false },
        { 2, true, true, false, true, 0, 0, false },
        { 2, true, true, false, true, 0, 0, false },
        { 2, true, true, false, true, 0, 2, false }
    }, 12);
}

TEST(NormalSurfacesTest, largeRP3) {
    // A two-vertex five-tetrahedron triangulation of real projective space.
    SCOPED_TRACE("Large RP3");

    // Too many standard and almost normal surfaces to check individually.
    Triangulation<3> tri = regina::Signature("aabcd.be.c.d.e").triangulate();
    compareCompactProfiles<false, false>(tri, {
        { 0, true, true, false, false, 1, 0, false },
        { 1, false, false, false, false, 0, 0, false },
        { 2, true, true, false, false, 0, 0, false },
        { 2, true, true, false, false, 1, 0, false },
        { 2, true, true, false, false, 1, 0, false }
    }, 24, 30);
}

TEST(NormalSurfacesTest, twistedKxI) {
    // A three-tetrahedron non-orientable twisted I-bundle over the Klein
    // bottle.  For the triangulation, see Chapter 3 of Burton's PhD thesis.
    SCOPED_TRACE("Twisted KxI");

    Triangulation<3> tri = Triangulation<3>::fromGluings(3, {
        { 0, 0, 1, {0,1,2,3} }, { 0, 1, 2, {2,1,0,3} }, { 0, 2, 2, {1,3,2,0} },
        { 1, 1, 2, {0,3,2,1} }, { 1, 2, 2, {3,1,0,2} }});
    compareCompactProfiles(tri, {
        { 0, false, false, false, false, 0, 3, false },
        { 0, false, false, true, false, 0, 0, false },
        { 0, false, false, true, false, 0, 0, false },
        { 0, false, true, true, false, 0, 3, false },
        { 0, true, false, true, false, 0, 0, false },
        { 0, true, true, true, false, 1, 0, false }
    }, {
        { -1, true, false, true, false, 0, 3, true },
        { 1, true, true, true, true, 0, 0, false }
    }, {
        { -2, false, false, true, false, 0, 0, false },
        { -2, false, false, true, false, 0, 0, false },
        { -1, false, false, true, false, 0, 0, false },
        { -1, false, false, true, false, 0, 0, false },
        { -1, true, false, true, false, 0, 0, false }
    });
}

TEST(NormalSurfacesTest, norSFS) {
    // A 9-tetrahedron triangulation of the space SFS [RP2: (2,1) (2,1) (2,1)].
    // Specifically, this is triangulation #5 of this space from the
    // non-orientable census as it was shipped with Regina 4.5.
    SCOPED_TRACE("Twisted KxI");

    Triangulation<3> tri = Triangulation<3>::rehydrate(
        "jnnafaabcfighhihimgbpqpepbr");

    compareCompactProfiles<true, false>(tri, {
        { -2, false, false, false, false, 0, 0, false },
        { -2, true, true, false, false, 0, 0, false },
        { 0, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, 0, 6, false },
        { 0, false, false, false, false, 0, 8, false },
        { 0, false, false, false, false, 0, 8, false },
        { 0, false, false, false, false, 0, 8, false },
        { 0, false, false, false, false, 0, 8, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, false, true, false, false, 1, 0, false },
        { 0, true, false, false, false, 0, 0, false },
        { 0, true, false, false, false, 0, 0, false },
        { 0, true, false, false, false, 0, 0, false },
        { 0, true, false, false, false, 0, 9, false }
    }, {
        { -2, false, false, false, false, 0, 0, false },
        { -2, false, false, false, false, 0, 9, false },
        { -2, false, true, false, false, 0, 0, false },
        { 2, true, true, false, true, 0, 0, false },
    }, 36);
}

// Use std::tuple for the free comparison operators.
using NonCompactProfile = std::tuple<
    regina::Integer /* euler char, or always 0 for non-compact surfaces */,
    bool    /* orientable?, or always false for non-compact surfaces */,
    bool    /* two-sided?, or always false for non-compact surfaces */,
    bool    /* compact? */,
    bool    /* has real boundary? */,
    bool    /* vertex link? */,
    int     /* edge link (number of edges) */,
    size_t  /* central (number of discs) */,
    bool    /* splitting? */ >;

static std::vector<NonCompactProfile> sortedNonCompactProfiles(
        const NormalSurfaces& list) {
    std::vector<NonCompactProfile> found;
    found.reserve(list.size());
    for (const auto& s : list) {
        auto edgeLinks = s.isThinEdgeLink();
        if (s.isCompact()) {
            EXPECT_TRUE(s.isConnected());
            found.push_back({ s.eulerChar(),
                s.isOrientable(), s.isTwoSided(),
                true, s.hasRealBoundary(),
                s.isVertexLinking(), (edgeLinks.first == nullptr ? 0 :
                    edgeLinks.second == nullptr ? 1 : 2),
                s.isCentral(), s.isSplitting() });
        } else {
            // Several tests we cannot perform for non-compact surfaces.
            found.push_back({ 0, false, false, false, s.hasRealBoundary(),
                s.isVertexLinking(), (edgeLinks.first == nullptr ? 0 :
                    edgeLinks.second == nullptr ? 1 : 2),
                s.isCentral(), s.isSplitting() });
        }
    }
    std::sort(found.begin(), found.end());
    return found;
}

static void compareNonCompactProfiles(const Triangulation<3>& tri,
        std::initializer_list<NonCompactProfile> expectQuad,
        std::initializer_list<NonCompactProfile> expectStd,
        std::initializer_list<NonCompactProfile> extraANStd) {
    // We assume all surfaces should be compact and connected.
    // All hard-coded "expected solution" lists passed to this function
    // should already be in sorted order.
    {
        std::vector<NonCompactProfile> expect;
        auto dest = std::back_inserter(expect);
        expect.reserve(expectQuad.size());

        std::copy(expectQuad.begin(), expectQuad.end(), dest);

        SCOPED_TRACE("Quad coordinates");
        NormalSurfaces list(tri, NormalCoords::Quad);
        std::vector<NonCompactProfile> found = sortedNonCompactProfiles(list);
        EXPECT_EQ(found, expect);
    }

    {
        std::vector<NonCompactProfile> expect;
        auto dest = std::back_inserter(expect);
        expect.reserve(expectStd.size() + extraANStd.size());

        std::copy(expectStd.begin(), expectStd.end(), dest);
        {
            SCOPED_TRACE("Standard coordinates");
            NormalSurfaces list(tri, NormalCoords::Standard);
            std::vector<NonCompactProfile> found =
                sortedNonCompactProfiles(list);
            EXPECT_EQ(found, expect);
        }

        auto split = expect.end();
        std::copy(extraANStd.begin(), extraANStd.end(), dest);
        std::inplace_merge(expect.begin(), split, expect.end());
        {
            SCOPED_TRACE("Standard almost normal coordinates");
            NormalSurfaces list(tri, NormalCoords::AlmostNormal);
            std::vector<NonCompactProfile> found =
                sortedNonCompactProfiles(list);
            EXPECT_EQ(found, expect);
        }
    }
}

TEST(NormalSurfacesTest, gieseking) {
    SCOPED_TRACE("Gieseking");

    Triangulation<3> tri = Example<3>::gieseking();
    compareNonCompactProfiles(tri, {
        // No quad normal surfaces at all.
    }, {
        { 0, false, true, true, false, true, 0, 0, false }
    }, {
        // No additional almost normal surfaces.
    });
}

TEST(NormalSurfacesTest, figureEight) {
    SCOPED_TRACE("Figure eight");

    Triangulation<3> tri = Example<3>::figureEight();
    compareNonCompactProfiles(tri, {
        { 0, false, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, false, 0, 0, false },
        { 0, false, false, false, false, false, 0, 0, false }
    }, {
        { 0, true, true, true, false, true, 0, 0, false }
    }, {
        // No additional almost normal surfaces.
    });
}

static void verifyLoopCtw(size_t len) {
    // Verify the vertex surfaces in C~(len) in several coordinate systems.
    // Requires len > 0.
    SCOPED_TRACE("Twisted layered loop");
    SCOPED_TRACE_NUMERIC(len);
    Triangulation<3> loop = Example<3>::layeredLoop(len, true);

    /**
     * It is easy to prove that the quad vertex surfaces are precisely:
     * - len x edge-linking tori;
     * - 1 x splitting, edge-linking Klein bottle.
     */
    {
        NormalSurfaces list(loop, NormalCoords::Quad);
        EXPECT_EQ(list.size(), len + 1);

        bool foundKB = false;
        for (const auto& s : list) {
            EXPECT_EQ(s.eulerChar(), 0);
            EXPECT_TRUE(s.isConnected());
            EXPECT_TRUE(s.isCompact());
            EXPECT_FALSE(s.hasRealBoundary());
            EXPECT_FALSE(s.isVertexLinking());

            auto edgeLinks = s.isThinEdgeLink();
            EXPECT_NE(edgeLinks.first, nullptr);
            EXPECT_EQ(edgeLinks.second, nullptr);

            if (s.isOrientable()) {
                // Should be an edge-linking torus.
                EXPECT_TRUE(s.isTwoSided());
                EXPECT_FALSE(s.isSplitting());
                EXPECT_EQ(s.isCentral(), 0);
            } else {
                // Should be the (unique) splitting, edge-linking Klein bottle.
                EXPECT_FALSE(foundKB);
                foundKB = true;

                EXPECT_FALSE(s.isTwoSided());
                EXPECT_TRUE(s.isSplitting());
                EXPECT_EQ(s.isCentral(), len);
            }
        }
        EXPECT_TRUE(foundKB);
    }

    // In standard and normal coordinates, the output size becomes exponential.
    // We should scale down our ambitions a little from here on.
    if (len > 15)
        return;

    /*
     * In standard and almost normal coordinates, the following patterns are
     * observed experimentally (and I am happy to assume them for the purposes
     * of the test suite):
     *
     * Standard coordinates: OEIS sequence #A001612:
     * - count(1,2) = (2,4); count(n > 2) = count(n-1) + count(n-2) - 1
     * Equivalently:
     * - count(n > 0) = fibonacci(n+1) + fibonacci(n-1) + 1
     *
     * Standard almost normal coordinates = standard solutions plus:
     * - diff(1,2,3) = (1,0,0)
     * - diff(n > 3) = n * fibonacci(n-5) [where fibonacci(-1) = 1]
     */

    // Build a table of Fibonacci numbers.  We could do this in constant space
    // (we only need indices n-5 and n±1), but we go with simpler code instead.
    auto* fib = new size_t[len + 3]; // fib[i] holds fibonacci(i-1)
    fib[0] = 1; fib[1] = 0;
    for (size_t i = 2; i < len + 3; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];
    size_t expectStd = fib[len] + fib[len + 2] + 1;
    size_t extraANStd = (len > 3 ? (len * fib[len - 4]) : len == 1 ? 1 : 0);
    delete[] fib;

    {
        NormalSurfaces list(loop, NormalCoords::Standard);
        EXPECT_EQ(list.size(), expectStd);
    }
    {
        NormalSurfaces list(loop, NormalCoords::AlmostNormal);
        EXPECT_EQ(list.size(), expectStd + extraANStd);
    }
}

TEST(NormalSurfacesTest, largeTriangulations) {
    verifyLoopCtw(3);
    verifyLoopCtw(6);
    verifyLoopCtw(9);
    verifyLoopCtw(12);
    verifyLoopCtw(15);
    verifyLoopCtw(20);
    verifyLoopCtw(30);
    verifyLoopCtw(40);
    verifyLoopCtw(50);
}

template <bool almostNormal>
static void verifyConversions(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    static constexpr NormalCoords standardCoords = (almostNormal ?
        NormalCoords::AlmostNormal : NormalCoords::Standard);
    static constexpr NormalCoords reducedCoords = (almostNormal ?
        NormalCoords::QuadOct : NormalCoords::Quad);

    bool conversionSupported = tri.isValid() && ! tri.isIdeal();

    NormalSurfaces stdDirect(tri, standardCoords, NormalList::Vertex,
        NormalAlg::VertexStandardDirect);
    NormalSurfaces stdConv(tri, standardCoords, NormalList::Vertex,
        NormalAlg::VertexViaReduced);

    if (! tri.isEmpty()) {
        EXPECT_FALSE(stdDirect.algorithm().has(NormalAlg::VertexViaReduced));
        EXPECT_TRUE(stdDirect.algorithm().has(NormalAlg::VertexStandardDirect));

        if (conversionSupported) {
            EXPECT_FALSE(stdConv.algorithm().has(NormalAlg::VertexStandardDirect));
            EXPECT_TRUE(stdConv.algorithm().has(NormalAlg::VertexViaReduced));
        } else {
            EXPECT_TRUE(stdConv.algorithm().has(NormalAlg::VertexStandardDirect));
            EXPECT_FALSE(stdConv.algorithm().has(NormalAlg::VertexViaReduced));
        }
    }

    EXPECT_EQ(stdDirect, stdConv);

    if (conversionSupported) {
        NormalSurfaces quadDirect(tri, reducedCoords);
        NormalSurfaces quadConv(stdDirect,
            regina::NormalTransform::ConvertStandardToReduced);
        EXPECT_EQ(quadDirect, quadConv);
    }
}

TEST(NormalSurfacesTest, standardQuadConversions) {
    // Include a couple of ideal triangulations, but not a whole census of
    // them, since conversion is not supported for ideal triangulations.
    testManualCases(verifyConversions<false>);

    runCensusMinClosed(&verifyConversions<false>);
    runCensusAllClosed(&verifyConversions<false>);
    runCensusAllBounded(&verifyConversions<false>);
    // runCensusAllIdeal(&verifyConversions<false>);
}

TEST(NormalSurfacesTest, standardANQuadOctConversions) {
    // Include a couple of ideal triangulations, but not a whole census of
    // them, since conversion is not supported for ideal triangulations.
    testManualCases(verifyConversions<true>);

    runCensusMinClosed(&verifyConversions<true>);
    runCensusAllClosed(&verifyConversions<true>);
    runCensusAllBounded(&verifyConversions<true>);
    // runCensusAllIdeal(&verifyConversions<true>);
}

template <NormalCoords coords>
static void verifyTreeVsDD(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    std::unique_ptr<NormalSurfaces> dd;
    std::unique_ptr<NormalSurfaces> tree;

    try {
        dd = std::make_unique<NormalSurfaces>(tri, coords, NormalList::Vertex,
            NormalAlg::VertexDD | NormalAlg::VertexStandardDirect);
    } catch (const regina::InvalidArgument&) {
    } catch (const regina::UnsolvedCase&) {
    }
    try {
        tree = std::make_unique<NormalSurfaces>(tri, coords, NormalList::Vertex,
            NormalAlg::VertexTree | NormalAlg::VertexStandardDirect);
    } catch (const regina::InvalidArgument&) {
    } catch (const regina::UnsolvedCase&) {
    }

    EXPECT_EQ(bool(dd), bool(tree));

    if (! dd) {
        // Enumeration failed.
        EXPECT_FALSE(tree);
        if constexpr (coords != NormalCoords::QuadClosed &&
                coords != NormalCoords::QuadOctClosed)
            ADD_FAILURE() << "Enumeration should not fail in this "
                "coordinate system";
    } else {
        // Enumeration should have succeeded.
        ASSERT_TRUE(tree);

        if (! tri.isEmpty()) {
            EXPECT_TRUE(dd->algorithm().has(NormalAlg::VertexDD));
            EXPECT_FALSE(dd->algorithm().has(NormalAlg::VertexTree));

            EXPECT_FALSE(tree->algorithm().has(NormalAlg::VertexDD));
            EXPECT_TRUE(tree->algorithm().has(NormalAlg::VertexTree));
        }

        EXPECT_EQ(*dd, *tree);
    }
}

template <NormalCoords coords>
static void treeVsDDDetail() {
    SCOPED_TRACE_NUMERIC(static_cast<int>(coords));

    runCensusMinClosed(verifyTreeVsDD<coords>);
    runCensusAllClosed(verifyTreeVsDD<coords>);
    runCensusAllBounded(verifyTreeVsDD<coords>);
    runCensusAllIdeal(verifyTreeVsDD<coords>);
}

TEST(NormalSurfacesTest, treeVsDD) {
    treeVsDDDetail<NormalCoords::Quad>();
    treeVsDDDetail<NormalCoords::Standard>();
    treeVsDDDetail<NormalCoords::QuadOct>();
    treeVsDDDetail<NormalCoords::AlmostNormal>();
    treeVsDDDetail<NormalCoords::QuadClosed>();
    treeVsDDDetail<NormalCoords::QuadOctClosed>();
}

template <NormalCoords coords>
static void verifyFundAlgorithms(const Triangulation<3>& tri,
        const char* name, NormalAlg alg1, NormalAlg alg2) {
    SCOPED_TRACE_CSTRING(name);

    std::unique_ptr<NormalSurfaces> surfaces1;
    std::unique_ptr<NormalSurfaces> surfaces2;

    try {
        surfaces1 = std::make_unique<NormalSurfaces>(tri, coords,
            NormalList::Fundamental, alg1);
    } catch (const regina::InvalidArgument&) {
    } catch (const regina::UnsolvedCase&) {
    }
    try {
        surfaces2 = std::make_unique<NormalSurfaces>(tri, coords,
            NormalList::Fundamental, alg2);
    } catch (const regina::InvalidArgument&) {
    } catch (const regina::UnsolvedCase&) {
    }

    EXPECT_EQ(bool(surfaces1), bool(surfaces2));

    if (! surfaces1) {
        // Enumeration failed.
        EXPECT_FALSE(surfaces2);
        if constexpr (coords != NormalCoords::QuadClosed &&
                coords != NormalCoords::QuadOctClosed)
            ADD_FAILURE() << "Enumeration should not fail in this "
                "coordinate system";
    } else {
        // Enumeration should have succeeded.
        ASSERT_TRUE(surfaces2);

        if (! tri.isEmpty()) {
            EXPECT_TRUE(surfaces1->algorithm().has(alg1));
            EXPECT_FALSE(surfaces1->algorithm().has(alg2));

            EXPECT_FALSE(surfaces2->algorithm().has(alg1));
            EXPECT_TRUE(surfaces2->algorithm().has(alg2));
        }

        EXPECT_EQ(*surfaces1, *surfaces2);
    }
}

template <NormalCoords coords>
static void fundPrimalVsDualDetail() {
    SCOPED_TRACE_NUMERIC(static_cast<int>(coords));

    auto cmp = [](const Triangulation<3>& tri, const char* name) {
        verifyFundAlgorithms<coords>(tri, name,
            NormalAlg::HilbertPrimal, NormalAlg::HilbertDual);
    };
    runCensusMinClosed(cmp, true);
    runCensusAllClosed(cmp, true);
    runCensusAllBounded(cmp, true);
    runCensusAllIdeal(cmp, true);
}

TEST(NormalSurfacesTest, fundPrimalVsDual) {
    fundPrimalVsDualDetail<NormalCoords::Quad>();
    fundPrimalVsDualDetail<NormalCoords::Standard>();
    fundPrimalVsDualDetail<NormalCoords::QuadOct>();
    fundPrimalVsDualDetail<NormalCoords::AlmostNormal>();
    fundPrimalVsDualDetail<NormalCoords::QuadClosed>();
    fundPrimalVsDualDetail<NormalCoords::QuadOctClosed>();
}

TEST(NormalSurfacesTest, fundDualVsCD) {
    // The Contejean-Devie algorithm is very slow, and people should not be
    // using it.  Here we are just giving a proof-of-life that it still works,
    // but on very small triangulations.
    // In all of the following examples, there are more fundamental surfaces
    // than vertex surfaces.
    verifyFundAlgorithms<NormalCoords::Standard>(
        Example<3>::lst(1, 4), "LST(1,4,5)",
        NormalAlg::HilbertDual, NormalAlg::HilbertCD);
    verifyFundAlgorithms<NormalCoords::Quad>(
        Triangulation<3>::fromIsoSig("dLQbccchhfo"), "m009",
        NormalAlg::HilbertDual, NormalAlg::HilbertCD);
}

static void verifyEulerConstraints(const Triangulation<3>& tri,
        const char* name) {
    // Verify that tree traversal works correctly with a chi=0 constraint.
    SCOPED_TRACE_CSTRING(name);

    // Tree traversal requires a non-empty triangulation.
    if (tri.isEmpty())
        return;

    // Build a sorted list of all vertex surfaces enumerated using the
    // chi=0 constraint.
    std::vector<NormalSurface> eulerZero;
    regina::TreeEnumeration<regina::LPConstraintEulerZero> tree(
        tri, NormalCoords::Standard);
    while (tree.next()) {
        NormalSurface s = tree.buildSurface();
        EXPECT_EQ(s.eulerChar(), 0);
        eulerZero.push_back(std::move(s));
    }
    std::sort(eulerZero.begin(), eulerZero.end());

    // Build all vertex surfaces with no constraints on Euler characteristic.
    // Every vertex surface with chi=0 should appear in eulerZero (but note
    // that eulerZero could contain some extra surfaces also).
    NormalSurfaces noConstraints(tri, NormalCoords::Standard);
    for (const NormalSurface& s : noConstraints)
        if (s.eulerChar() == 0)
            EXPECT_TRUE(std::binary_search(
                eulerZero.begin(), eulerZero.end(), s));
}

TEST(NormalSurfacesTest, eulerConstraints) {
    testManualCases(verifyEulerConstraints);
}

static void verifyDisjoint(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    NormalSurfaces list(tri, NormalCoords::AlmostNormal);

    std::pair<const regina::Edge<3>*, const regina::Edge<3>*> edges;
    for (const auto& s : list) {
        // For some types of surfaces we know exactly what it should be
        // disjoint from.
        if (s.isVertexLinking()) {
            // Vertex links are disjoint from everything.
            for (const auto& t : list)
                EXPECT_TRUE(s.disjoint(t));
        } else if ((edges = s.isThinEdgeLink()).first) {
            // A thin edge link is disjoint from (i) all vertex links, and
            // (ii) all surfaces that do not meet the relevant edge (except
            // the edge link itself, if it is 1-sided).
            size_t edge = edges.first->index();

            for (const auto& t : list) {
                // Deal with (s, s) later.
                if (std::addressof(s) == std::addressof(t))
                    continue;

                if (t.isVertexLinking())
                    EXPECT_TRUE(s.disjoint(t));
                else if (t.edgeWeight(edge) == 0)
                    EXPECT_TRUE(s.disjoint(t));
                else
                    EXPECT_FALSE(s.disjoint(t));
            }
        }

        // Ensure that the surface is disjoint from itself iff it is two-sided.
        EXPECT_EQ(s.isTwoSided(), s.disjoint(s));
    }
}

TEST(NormalSurfacesTest, disjoint) {
    testManualCases(verifyDisjoint);

    runCensusAllClosed(verifyDisjoint);
    runCensusAllBounded(verifyDisjoint);
    runCensusAllIdeal(verifyDisjoint);

    // There was a bug in older versions of Regina where calling components()
    // a second time on a disconnected surface s = (s1 u ... u sk) would return
    // {s}, not {s1, ..., sk}, due to incorrect use of the cached connectedness
    // property.  Make sure this bug does not return.
    {
        Triangulation<3> t = Example<3>::poincare();
        auto s1 = t.vertex(0)->linkingSurface();
        auto s2 = t.edge(0)->linkingSurface().first;
        auto s = s1 + s2;

        std::vector<NormalSurface> c;

        EXPECT_NO_THROW({ c = s.components(); });
        ASSERT_EQ(c.size(), 2);
        EXPECT_EQ(c[0], s1);
        EXPECT_EQ(c[1], s2);

        // Here's where it used to break - when components() was called for
        // the second time.
        EXPECT_NO_THROW({ c = s.components(); });
        ASSERT_EQ(c.size(), 2);
        EXPECT_EQ(c[0], s1);
        EXPECT_EQ(c[1], s2);
    }
}

/**
 * Returns \c false if tri is definitely not a twisted product Sx~I, or
 * \c true if it might be.
 *
 * Assumes tri is valid, connected, and has no ideal vertices.
 */
static bool mightBeTwistedProduct(const Triangulation<3>& tri) {
    if (tri.countBoundaryComponents() != 1)
        return false;

    // Check the relationship between H1 and H1Bdry.
    // We must have one of:
    //  -  H1 = (2g)Z, H1Bdry = (4g-2)Z;
    //  -  H1 = Z_2 + (g-1)Z, H1Bdry = Z_2 + (2g-3)Z;
    //  -  H1 = Z_2 + (g-1)Z, H1Bdry = (2g-2)Z;
    regina::AbelianGroup h1 = tri.homology();
    const regina::AbelianGroup& bdry = tri.homologyBdry();

    if (h1.countInvariantFactors() == 0) {
        // Must have H1 = (2g)Z.
        if (! bdry.isFree(2 * h1.rank() - 2))
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
    if (! tri.homologyRel().isZn(2))
        return false;

    return true;
}

/**
 * Returns \c false if tri is definitely not a product SxI, or \c true if it
 * might be.
 *
 * Assumes tri is valid, connected, and has no ideal vertices.
 */
static bool mightBeUntwistedProduct(const Triangulation<3>& tri) {
    if (tri.countBoundaryComponents() != 2)
        return false;

    // Check that both boundary components are homeomorphic.
    regina::BoundaryComponent<3>* b0 = tri.boundaryComponent(0);
    regina::BoundaryComponent<3>* b1 = tri.boundaryComponent(1);

    if (b0->eulerChar() != b1->eulerChar())
        return false;
    if (b0->isOrientable() != b1->isOrientable())
        return false;

    // Check that H1 is of the form (k)Z or Z_2 + (k)Z, and that
    // H1Bdry = 2 H1.
    regina::AbelianGroup h1 = tri.homology();
    regina::AbelianGroup bdry = tri.homologyBdry();

    if (h1.countInvariantFactors() == 0) {
        // Must have H1 = (k)Z.
        if (! bdry.isFree(2 * h1.rank()))
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
    if (! tri.homologyRel().isZ())
        return false;

    return true;
}

/**
 * Checks whether the boundary of the given triangulation *might* be equal to
 * (i) the surface s, (ii) two copies of the surface s, or (iii) a connected
 * double cover of the surface s.  Increments the relevant counters accordingly.
 */
static void checkBoundaryType(const NormalSurface& s,
        const Triangulation<3>& tri, size_t& foundS, size_t& foundTwoCopies,
        size_t& foundDoubleCover) {
    if (tri.countBoundaryComponents() == 1) {
        const regina::BoundaryComponent<3>* b = tri.boundaryComponent(0);

        if (s.eulerChar() == b->eulerChar()
                && s.isOrientable() == b->isOrientable())
            ++foundS;
        if (s.eulerChar() * 2 == b->eulerChar() &&
                (b->isOrientable() || ! s.isOrientable()))
            ++foundDoubleCover;
    } else if (tri.countBoundaryComponents() == 2) {
        const regina::BoundaryComponent<3>* b0 = tri.boundaryComponent(0);
        const regina::BoundaryComponent<3>* b1 = tri.boundaryComponent(1);

        if (
                s.eulerChar() == b0->eulerChar() &&
                s.eulerChar() == b1->eulerChar() &&
                s.isOrientable() == b0->isOrientable() &&
                s.isOrientable() == b1->isOrientable())
            ++foundTwoCopies;
    }
}

static void verifyCutAlong(const Triangulation<3>& tri, const char* name) {
    // PRE: tri is valid and connected.
    SCOPED_TRACE_CSTRING(name);

    ASSERT_TRUE(tri.isValid());
    ASSERT_TRUE(tri.isConnected());

    NormalSurfaces list(tri, NormalCoords::AlmostNormal);

    // We use the fact that all vertex surfaces are connected.
    for (const NormalSurface& s : list) {
        Triangulation<3> t = s.cutAlong();
        t.simplify();

        Triangulation<3> tDouble = (s * 2).cutAlong();
        tDouble.simplify();

        auto comp = t.triangulateComponents();
        auto compDouble = tDouble.triangulateComponents();

        bool separating = (s.isTwoSided() && comp.size() > 1);

        EXPECT_EQ(comp.size(), (separating ? 2 : 1));
        EXPECT_EQ(compDouble.size(), (separating ? 3 : 2));

        EXPECT_TRUE(t.isValid());
        EXPECT_TRUE(tDouble.isValid());
        EXPECT_TRUE(t.isIdeal() == tri.isIdeal());
        EXPECT_TRUE(tDouble.isIdeal() == tri.isIdeal());
        if (tri.isOrientable()) {
            EXPECT_TRUE(t.isOrientable());
            EXPECT_TRUE(tDouble.isOrientable());
        }

        for (const Triangulation<3>& c : comp)
            EXPECT_TRUE(c.hasBoundaryTriangles());
        for (const Triangulation<3>& c : compDouble)
            EXPECT_TRUE(c.hasBoundaryTriangles());

        // The remaining tests only work for closed triangulations.
        if (! tri.isClosed())
            continue;

        // TODO: Simplify this part of the testing.
        // Check the boundaries of components of t.
        size_t expectS, expectTwoCopies, expectDoubleCover;
        size_t foundS, foundTwoCopies, foundDoubleCover;
        if (separating) {
            expectS = 2;
            expectTwoCopies = 0;
            expectDoubleCover = 0;
        } else if (s.isTwoSided()) {
            expectS = 0;
            expectTwoCopies = 1;
            expectDoubleCover = 0;
        } else {
            expectS = 0;
            expectTwoCopies = 0;
            expectDoubleCover = 1;
        }
        EXPECT_EQ(t.countBoundaryComponents(),
                expectS + 2 * expectTwoCopies + expectDoubleCover);
        foundS = foundTwoCopies = foundDoubleCover = 0;
        for (const Triangulation<3>& c : comp)
            checkBoundaryType(s, c, foundS, foundTwoCopies, foundDoubleCover);
        EXPECT_GE(foundS, expectS);
        EXPECT_GE(foundTwoCopies, expectTwoCopies);
        EXPECT_GE(foundDoubleCover, expectDoubleCover);

        // Check the boundaries of components of tDouble.
        if (separating) {
            expectS = 2;
            expectTwoCopies = 1;
            expectDoubleCover = 0;
        } else if (s.isTwoSided()) {
            expectS = 0;
            expectTwoCopies = 2;
            expectDoubleCover = 0;
        } else {
            expectS = 0;
            expectTwoCopies = 0;
            expectDoubleCover = 2;
        }
        EXPECT_EQ(tDouble.countBoundaryComponents(),
                expectS + 2 * expectTwoCopies + expectDoubleCover);
        foundS = foundTwoCopies = foundDoubleCover = 0;
        for (const Triangulation<3>& c : compDouble)
            checkBoundaryType(s, c, foundS, foundTwoCopies, foundDoubleCover);
        EXPECT_GE(foundS, expectS);
        EXPECT_GE(foundTwoCopies, expectTwoCopies);
        EXPECT_GE(foundDoubleCover, expectDoubleCover);

        // Look for the product piece when cutting along the double surface.
        bool foundProduct = false;
        for (const Triangulation<3>& c : compDouble) {
            if (s.isTwoSided()) {
                // Look for a potential product SxI.
                if (mightBeUntwistedProduct(c)) {
                    foundProduct = true;
                    break;
                }
            } else {
                // Look for a potential twisted product Sx~I
                if (mightBeTwistedProduct(c)) {
                    foundProduct = true;
                    break;
                }
            }
        }
        EXPECT_TRUE(foundProduct);
    }
}

TEST(NormalSurfacesTest, cutAlong) {
    testManualCases(verifyCutAlong);

    runCensusAllClosed(verifyCutAlong, true);
    runCensusAllBounded(verifyCutAlong, true);
    runCensusAllIdeal(verifyCutAlong, true);
}

static void verifyRemoveOctsDetail(NormalCoords coords,
        const Triangulation<3>& tri) {
    SCOPED_TRACE_NUMERIC(static_cast<int>(coords));

    for (const NormalSurface& s : NormalSurfaces(tri, coords)) {
        NormalSurface noOct = s.removeOcts();

        EXPECT_FALSE(noOct.encoding().storesOctagons());
        EXPECT_TRUE(noOct.normal());

        // Internally, the no-octagon variants should always be
        // stored using the standard matching equations.
        regina::MatrixInt matching = regina::makeMatchingEquations(
            noOct.triangulation(), NormalCoords::Standard);
        ASSERT_EQ(noOct.vector().size(), matching.columns());
        // For non-compact surfaces we should test the quad matching equations,
        // but for now we leave it.  The standard matching equations will fail
        // because infinity - infinity != 0.
        if (s.isCompact())
            EXPECT_TRUE((matching * noOct.vector()).isZero());

        // Properties that are available for all surfaces:
        EXPECT_EQ(noOct.isCompact(), s.isCompact());
        EXPECT_EQ(noOct.hasRealBoundary(), s.hasRealBoundary());

        // Properties that are only available for compact surfaces:
        if (s.isCompact()) {
            EXPECT_EQ(noOct.isOrientable(), s.isOrientable());
            EXPECT_EQ(noOct.isTwoSided(), s.isTwoSided());
            EXPECT_EQ(noOct.isConnected(), s.isConnected());
            EXPECT_EQ(noOct.eulerChar(), s.eulerChar());
            EXPECT_EQ(noOct.countBoundaries(), s.countBoundaries());
        }

        if (s.octPosition()) {
            // We should have retriangulated.
            const Triangulation<3>& retri = noOct.triangulation();
            EXPECT_NE(std::addressof(retri), std::addressof(tri));

            // Verify that the retriangulation looks correct.
            EXPECT_EQ(tri.isClosed(), retri.isClosed());
            EXPECT_EQ(tri.isOrientable(), retri.isOrientable());
            EXPECT_EQ(tri.countComponents(), retri.countComponents());
            EXPECT_EQ(tri.countBoundaryComponents(),
                retri.countBoundaryComponents());
            EXPECT_EQ(tri.homology(), retri.homology());
        } else {
            // We should not have retriangulated.
            EXPECT_EQ(std::addressof(noOct.triangulation()),
                std::addressof(tri));
        }
    }
}

void verifyRemoveOcts(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    verifyRemoveOctsDetail(NormalCoords::AlmostNormal, tri);
    verifyRemoveOctsDetail(NormalCoords::QuadOct, tri);
}

TEST(NormalSurfacesTest, removeOcts) {
    testManualCases(verifyRemoveOcts);

    runCensusAllClosed(verifyRemoveOcts, true);
    runCensusAllBounded(verifyRemoveOcts, true);
    runCensusAllIdeal(verifyRemoveOcts, true);
}

static void testCopyMove(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // The main point of this test is to ensure that the move
    // operations are *actually* move operations and not copies.
    //
    // We assume here that std::vector's move operations
    // preserve the addresses of the underlying objects.
    // I don't think this is required by the standard, but I'm
    // also not aware of any implementation that doesn't do this,
    // and I can't think of a better (and still non-intrusive)
    // way to ensure that the move was a "real" move.

    const NormalSurfaces a(tri, NormalCoords::Standard);
    ASSERT_NE(a.size(), 0);

    NormalSurfaces a1(a);
    EXPECT_EQ(a1.detail(), a.detail());
    EXPECT_NE(a.size(), 0);

    NormalSurfaces a2(std::move(a1));
    EXPECT_EQ(a2.detail(), a.detail());
    // The std::vector move constructor promises to empty the source.
    EXPECT_EQ(a1.size(), 0);

    NormalSurfaces a3(Example<3>::s2xs1(), NormalCoords::Standard);
    a3 = a;
    EXPECT_EQ(a3.detail(), a.detail());
    EXPECT_NE(a.size(), 0);

    NormalSurfaces a4(Example<3>::s2xs1(), NormalCoords::Standard);
    a4 = std::move(a3);
    EXPECT_EQ(a4.detail(), a.detail());
    /*
    // I'm not sure how to verify that the source was moved from
    // and not copied, in a way that does not make assumptions
    // about the standard library implementation...
    EXPECT_EQ(a3.size(), 0);
    */
}

TEST(NormalSurfacesTest, copyMove) {
    // Try this with something closed and something ideal.
    testCopyMove(Example<3>::poincare(), "Poincare homology sphere");
    testCopyMove(Example<3>::whitehead(), "Whitehead");
}
