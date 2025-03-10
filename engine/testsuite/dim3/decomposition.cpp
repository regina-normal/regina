
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "split/signature.h"
#include "subcomplex/standardtri.h"
#include "triangulation/example3.h"

#include "testexhaustive.h"
#include "testhelper.h"

using regina::Example;
using regina::Signature;
using regina::Triangulation;

// TODO: Move these tests into Dim3Test.

/**
 * Determines whether the given triangulation is a minimal triangulation of
 * one of the three prime manifolds with no 0-efficient triangulation.
 */
static bool isZeroEfficientException(const Triangulation<3>& tri) {
    if (tri.size() != 2)
        return false;

    std::string sig = tri.isoSig();
    return (sig == "cMcabbgqw" /* RP3 */ ||
            sig == "cMcabbjaj" /* S2xS1 */ ||
            sig == "cPcbbbajs" /* S2x~S1 */);
}

static void verifyDecomp(const Triangulation<3>& tri, const char* name,
        std::initializer_list<const char*> expectManifolds) {
    // PRE: The given triangulation does not contain an embedded two-sided
    // projective plane (i.e., summands() should always return a solution).
    //
    // PRE: The manifolds we expect to see are given in sorted order, and all
    // corresponding Manifold classes have implementations of
    // Manifold::homology() (i.e., they do not throw regina::NotImplemented).

    SCOPED_TRACE_CSTRING(name);

    auto ans = tri.summands();

    if (expectManifolds.size() == 0) {
        EXPECT_TRUE(ans.empty());
        EXPECT_TRUE(tri.homology().isTrivial());
    } else {
        std::vector<std::string> foundManifolds;
        foundManifolds.reserve(ans.size());
        regina::AbelianGroup foundH1;

        for (const auto& summand : ans) {
            if (! summand.isZeroEfficient())
                EXPECT_TRUE(isZeroEfficientException(summand));

            foundH1.addGroup(summand.homology());

            auto stdTri = regina::StandardTriangulation::recognise(summand);
            EXPECT_TRUE(stdTri);
            if (stdTri) {
                auto stdMfd = stdTri->manifold();
                EXPECT_TRUE(stdMfd);
                if (stdMfd) {
                    foundManifolds.push_back(stdMfd->name());
                    EXPECT_NO_THROW({
                        EXPECT_EQ(summand.homology(), stdMfd->homology());
                    });
                } else
                    foundManifolds.push_back(summand.isoSig());
            } else
                foundManifolds.push_back(summand.isoSig());
        }

        EXPECT_EQ(foundH1, tri.homology());

        ASSERT_EQ(foundManifolds.size(), expectManifolds.size());
        std::sort(foundManifolds.begin(), foundManifolds.end());
        auto expect = expectManifolds.begin();
        auto found = foundManifolds.begin();
        for ( ; expect != expectManifolds.end(); ++expect, ++found)
            EXPECT_EQ(*found, *expect);
    }
}

static void verifyDecompSignature(const char* sig,
        std::initializer_list<const char*> expectManifolds) {
    verifyDecomp(Signature(sig).triangulate(), sig, expectManifolds);
}

TEST(ConnectedSumDecompositionTest, threeSpheres) {
    // 3-spheres obtained from splitting surface signatures:
    verifyDecompSignature("(a)(a)", {});
    verifyDecompSignature("(ab)(a)(b)", {});
    verifyDecompSignature("(ab)(a)(b)", {});
    verifyDecompSignature("(abc)(a)(b)(c)", {});
    verifyDecompSignature("(ab)(ac)(b)(c)", {});
    verifyDecompSignature("(abcd)(a)(b)(c)(d)", {});
    verifyDecompSignature("(abc)(abd)(c)(d)", {});
    verifyDecompSignature("(abc)(acd)(b)(d)", {});
    verifyDecompSignature("(abc)(ad)(b)(c)(d)", {});
    verifyDecompSignature("(ab)(ac)(bd)(cd)", {});
    verifyDecompSignature("(ab)(ac)(bd)(c)(d)", {});
    verifyDecompSignature("(abcd)(aefg)(b)(c)(d)(e)(f)(g)", {});

    // 3-spheres obtained as Lens spaces:
    verifyDecomp(Example<3>::lens(1, 0), "L(1,0)", {});
}

TEST(ConnectedSumDecompositionTest, nonZeroEfficient) {
    // Prime manifolds with no zero-efficient triangulation:
    verifyDecompSignature("(aab)(b)", { "RP3" });
    verifyDecompSignature("(ab)(ab)", { "RP3" });
    verifyDecompSignature("(aabcb)(c)", { "RP3" });
    verifyDecompSignature("(aabc)(b)(c)", { "RP3" });
    verifyDecompSignature("(aabcdcb)(d)", { "RP3" });
    verifyDecompSignature("(aabcdb)(c)(d)", { "RP3" });
    verifyDecompSignature("(aabcd)(b)(c)(d)", { "RP3" });
    verifyDecompSignature("(aabc)(bd)(c)(d)", { "RP3" });
    verifyDecompSignature("(abac)(bd)(cd)", { "RP3" });
    verifyDecompSignature("(abac)(bd)(c)(d)", { "RP3" });
    verifyDecompSignature("(abcd)(ac)(bd)", { "RP3" });
    verifyDecompSignature("(aab)(bc)(cd)(d)", { "RP3" });
    verifyDecompSignature("(abc)(ab)(cd)(d)", { "RP3" });
    verifyDecompSignature("(abc)(ad)(bd)(c)", { "RP3" });
    verifyDecompSignature("(abac)(b)(c)", { "S2 x S1" });
    verifyDecompSignature("(abacdc)(b)(d)", { "S2 x S1" });
    verifyDecompSignature("(abcabd)(c)(d)", { "S2 x S1" });
    verifyDecompSignature("(abacd)(b)(c)(d)", { "S2 x S1" });
    verifyDecompSignature("(aabc)(bd)(cd)", { "S2 x S1" });
    verifyDecompSignature("(abacde)(cf)(fg)(b)(d)(e)(g)", { "S2 x S1" });
    verifyDecompSignature("(abc)(abc)", { "L(3,1)" });
    verifyDecompSignature("(abc)(acb)", { "L(3,1)" });
}

TEST(ConnectedSumDecompositionTest, prime) {
    verifyDecompSignature("(aa)", { "L(4,1)" });
    verifyDecompSignature("(aabb)", { "L(8,3)" });
    verifyDecompSignature("(abab)", { "S3/Q8" });
    verifyDecompSignature("(aabccb)", { "L(12,5)" });
    verifyDecompSignature("(abcabc)", { "S3/Q12" });
    verifyDecompSignature("(aab)(bcc)", { "L(6,1)" });
    verifyDecompSignature("(aab)(bc)(c)", { "L(4,1)" });
    verifyDecompSignature("(ab)(ac)(bc)", { "L(4,1)" });
    verifyDecompSignature("(aabcddcb)", { "L(16,7)" });
    verifyDecompSignature("(abcdabcd)", { "S3/Q16" });
    verifyDecompSignature("(aabbc)(cd)(d)", { "L(8,3)" });
    verifyDecompSignature("(aabcb)(cdd)", { "L(14,3)" });
    verifyDecompSignature("(aabcb)(cd)(d)", { "L(8,3)" });
    verifyDecompSignature("(ababc)(cd)(d)", { "S3/Q8" });
    verifyDecompSignature("(abac)(bdcd)", { "L(4,1)" });
    verifyDecompSignature("(abac)(bcd)(d)", { "L(4,1)" });
    verifyDecompSignature("(abac)(bdd)(c)", { "L(4,1)" });
    verifyDecompSignature("(abcd)(abcd)", { "L(4,1)" });
    verifyDecompSignature("(abcd)(adcb)", { "L(4,1)" });
    verifyDecompSignature("(aab)(bcd)(c)(d)", { "L(4,1)" });
    verifyDecompSignature("(abc)(abd)(cd)", { "L(8,3)" });
    verifyDecompSignature("(abc)(acd)(bd)", { "S3/Q8" });
    verifyDecompSignature("(abcdefgh)(abcdefgh)", { "L(8,1)" });

    // And of course the Poincare homology sphere(S3/P120).
    // We'll build this a few different ways.

    verifyDecomp(Example<3>::augTriSolidTorus(2, -1, 3, 1, 5, -4),
        "Poincare homology sphere (aug I)", { "S3/P120" });
    verifyDecomp(Example<3>::augTriSolidTorus(2, -1, 3, -2, 5, 1),
        "Poincare homology sphere (aug II)", { "S3/P120" });

    // Poincare homology sphere as a plugged triangular solid torus:
    verifyDecomp(Triangulation<3>::fromGluings(5, {
        { 0, 0, 4, {1,0,2,3} }, { 0, 1, 3, {0,2,3,1} }, { 0, 2, 1, {0,1,3,2} },
        { 0, 3, 2, {2,1,3,0} }, { 1, 0, 3, {1,3,2,0} }, { 1, 1, 2, {0,2,3,1} },
        { 1, 2, 4, {2,1,0,3} }, { 2, 1, 4, {0,2,3,1} }, { 2, 3, 3, {3,1,2,0} },
        { 3, 3, 4, {0,1,2,3} }
    }), "Poincare homology sphere (plugged)", { "S3/P120" });
}

TEST(ConnectedSumDecompositionTest, composite) {
    verifyDecompSignature("(aabccd)(b)(d)", { "RP3", "RP3" });
    verifyDecompSignature("(abacbd)(cd)", { "RP3", "RP3" });
    verifyDecompSignature("(aabcdecb)(dfeg)(fg)", { "L(12,5)", "RP3" });
    verifyDecompSignature("(aabbc)(cdef)(egg)(d)(f)", { "L(4,1)", "L(8,3)" });
    verifyDecompSignature("(ababc)(cdef)(egg)(d)(f)", { "L(4,1)", "S3/Q8" });
    verifyDecompSignature("(abcd)(acbe)(dfeg)(f)(g)", { "L(3,1)", "S2 x S1" });
    verifyDecompSignature("(abcd)(adce)(befg)(f)(g)", { "L(3,1)", "RP3" });
    verifyDecompSignature("(abcde)(aff)(cgg)(b)(d)(e)", { "L(4,1)", "L(4,1)" });
    verifyDecompSignature("(abcde)(adf)(bfg)(ce)(g)", { "RP3", "S2 x S1" });
    verifyDecompSignature("(ababc)(cde)(dfg)(fg)(e)", { "RP3", "S3/Q8" });
    verifyDecompSignature("(abcdef)(acegg)(bdf)", { "L(3,1)", "L(4,1)" });
    verifyDecompSignature("(abacde)(dffgg)(b)(c)(e)", { "L(8,3)", "S2 x S1" });
    verifyDecompSignature("(aabccdeffeg)(b)(d)(g)", { "RP3", "RP3", "RP3" });
    verifyDecompSignature("(aabcde)(cfg)(dgf)(b)(e)", { "RP3", "RP3", "RP3" });
}

static void verifyDecompGeneral(const Triangulation<3>& tri, const char* name) {
    // PRE: tri is valid, closed and connected.
    SCOPED_TRACE_CSTRING(name);
    ASSERT_TRUE(tri.isValid());
    ASSERT_TRUE(tri.isClosed());
    ASSERT_TRUE(tri.isConnected());

    std::vector<Triangulation<3>> ans;
    try {
        ans = tri.summands();
    } catch (const regina::UnsolvedCase&) {
        // The routine reported an embedded two-sided projective plane.
        EXPECT_FALSE(tri.isOrientable());
        return;
    }

    regina::AbelianGroup h1;
    bool foundNor = false;
    for (const Triangulation<3>& term : ans) {
        if (! term.isOrientable())
            foundNor = true;
        if (! term.isZeroEfficient())
            EXPECT_TRUE(isZeroEfficientException(term));
        EXPECT_FALSE(term.isSphere());
        h1.addGroup(term.homology());
    }
    EXPECT_EQ(h1, tri.homology());
    EXPECT_EQ(foundNor, ! tri.isOrientable());
}

TEST(ConnectedSumDecompositionTest, census) {
    runCensusAllClosed(&verifyDecompGeneral);
    runCensusMinClosed(&verifyDecompGeneral);
}
