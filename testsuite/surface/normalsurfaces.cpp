
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

#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include "enumerate/treetraversal.h"
#include "packet/container.h"
#include "split/signature.h"
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/surface/testsurface.h"

using regina::AbelianGroup;
using regina::BoolSet;
using regina::BoundaryComponent;
using regina::Container;
using regina::Edge;
using regina::Example;
using regina::LargeInteger;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Packet;
using regina::Perm;
using regina::Signature;
using regina::Tetrahedron;
using regina::Triangulation;
using regina::Vector;

using regina::NS_STANDARD;
using regina::NS_QUAD;
using regina::NS_QUAD_CLOSED;
using regina::NS_AN_STANDARD;
using regina::NS_AN_QUAD_OCT;
using regina::NS_AN_QUAD_OCT_CLOSED;

using regina::NS_VERTEX;
using regina::NS_FUNDAMENTAL;

using regina::NS_VERTEX_DD;
using regina::NS_VERTEX_TREE;
using regina::NS_VERTEX_STD_DIRECT;
using regina::NS_VERTEX_VIA_REDUCED;

using regina::NS_HILBERT_DUAL;
using regina::NS_HILBERT_PRIMAL;

class NormalSurfacesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NormalSurfacesTest);

    CPPUNIT_TEST(defaultArgs);
    CPPUNIT_TEST(standardEmpty);
    CPPUNIT_TEST(standardOneTet);
    CPPUNIT_TEST(standardGieseking);
    CPPUNIT_TEST(standardFigure8);
    CPPUNIT_TEST(standardS3);
    CPPUNIT_TEST(standardLoopC2);
    CPPUNIT_TEST(standardLoopCtw3);
    CPPUNIT_TEST(standardLargeS3);
    CPPUNIT_TEST(standardLargeRP3);
    CPPUNIT_TEST(standardTwistedKxI);
    CPPUNIT_TEST(standardNorSFS);
    CPPUNIT_TEST(quadEmpty);
    CPPUNIT_TEST(quadOneTet);
    CPPUNIT_TEST(quadGieseking);
    CPPUNIT_TEST(quadFigure8);
    CPPUNIT_TEST(quadS3);
    CPPUNIT_TEST(quadLoopC2);
    CPPUNIT_TEST(quadLoopCtw3);
    CPPUNIT_TEST(quadLargeS3);
    CPPUNIT_TEST(quadLargeRP3);
    CPPUNIT_TEST(quadTwistedKxI);
    CPPUNIT_TEST(quadNorSFS);
    CPPUNIT_TEST(almostNormalEmpty);
    CPPUNIT_TEST(almostNormalOneTet);
    CPPUNIT_TEST(almostNormalGieseking);
    CPPUNIT_TEST(almostNormalFigure8);
    CPPUNIT_TEST(almostNormalS3);
    CPPUNIT_TEST(almostNormalLoopC2);
    CPPUNIT_TEST(almostNormalLoopCtw3);
    CPPUNIT_TEST(almostNormalLargeS3);
    CPPUNIT_TEST(almostNormalLargeRP3);
    CPPUNIT_TEST(almostNormalTwistedKxI);
    CPPUNIT_TEST(largeDimensionsStandard);
    CPPUNIT_TEST(largeDimensionsQuad);
    CPPUNIT_TEST(largeDimensionsAlmostNormal);
    CPPUNIT_TEST(standardQuadConversionsConstructed);
    CPPUNIT_TEST(standardQuadConversionsCensus);
    CPPUNIT_TEST(standardANQuadOctConversionsConstructed);
    CPPUNIT_TEST(standardANQuadOctConversionsCensus);
    CPPUNIT_TEST(treeVsDDCensus<NS_QUAD>);
    CPPUNIT_TEST(treeVsDDCensus<NS_STANDARD>);
    CPPUNIT_TEST(treeVsDDCensus<NS_AN_QUAD_OCT>);
    CPPUNIT_TEST(treeVsDDCensus<NS_AN_STANDARD>);
    CPPUNIT_TEST(treeVsDDCensus<NS_QUAD_CLOSED>);
    CPPUNIT_TEST(treeVsDDCensus<NS_AN_QUAD_OCT_CLOSED>);
    CPPUNIT_TEST(eulerZero);
    CPPUNIT_TEST(fundPrimalVsDual<NS_QUAD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_STANDARD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_AN_QUAD_OCT>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_AN_STANDARD>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_QUAD_CLOSED>);
    CPPUNIT_TEST(fundPrimalVsDual<NS_AN_QUAD_OCT_CLOSED>);
    CPPUNIT_TEST(disjointConstructed);
    CPPUNIT_TEST(disjointCensus);
    CPPUNIT_TEST(cutAlongConstructed);
    CPPUNIT_TEST(cutAlongCensus);
    CPPUNIT_TEST(removeOctsConstructed);
    CPPUNIT_TEST(removeOctsCensus);
    CPPUNIT_TEST(copyMove);

    CPPUNIT_TEST_SUITE_END();

    private:
        Triangulation<3> empty;
            /**< An empty triangulation. */
        Triangulation<3> oneTet;
            /**< A one-tetrahedron ball. */
        Triangulation<3> figure8;
            /**< The figure eight knot complement. */
        Triangulation<3> gieseking;
            /**< The Gieseking manifold. */
        Triangulation<3> S3;
            /**< A one-tetrahedron two-vertex 3-sphere. */
        Triangulation<3> loopC2;
            /**< An untwisted layered loop of length 2. */
        Triangulation<3> loopCtw3;
            /**< A twisted layered loop of length 3. */
        Triangulation<3> largeS3;
            /**< A 3-vertex 5-tetrahedron triangulation of the 3-sphere. */
        Triangulation<3> largeRP3;
            /**< A 2-vertex 5-tetrahedron triangulation of real
                 projective space. */
        Triangulation<3> twistedKxI;
            /**< A 3-tetrahedron non-orientable twisted I-bundle over the
                 Klein bottle. */
        Triangulation<3> norSFS;
            /**< A 9-tetrahedron triangulation of the space
                 SFS [RP2: (2,1) (2,1) (2,1)].  Specifically, this is
                 triangulation #5 of this space from the non-orientable
                 census as it was shipped with Regina 4.5. */


    public:
        void setUp() override {
            // The empty triangulation has no face identifications at all.

            oneTet.newTetrahedron();

            // Use pre-coded triangulations where we can.
            figure8 = Example<3>::figureEight();

            gieseking = Example<3>::gieseking();

            // Layered loops can be constructed automatically.
            S3 = Example<3>::layeredLoop(1, false);

            loopC2 = Example<3>::layeredLoop(2, false);

            loopCtw3 = Example<3>::layeredLoop(3, true);

            // Some non-minimal triangulations can be generated from
            // splitting surfaces.
            largeS3 = Signature("abcd.abe.c.d.e").triangulate();

            largeRP3 = Signature("aabcd.be.c.d.e").triangulate();

            // A 3-tetrahedron non-orientable twisted I-bundle over the
            // Klein bottle is described in Chapter 3 of Burton's PhD thesis.
            auto [r, s, t] = twistedKxI.newTetrahedra<3>();
            r->join(0, s, Perm<4>(0, 1, 2, 3));
            r->join(1, t, Perm<4>(2, 1, 0, 3));
            r->join(2, t, Perm<4>(1, 3, 2, 0));
            s->join(1, t, Perm<4>(0, 3, 2, 1));
            s->join(2, t, Perm<4>(3, 1, 0, 2));

            // Build the 9-tetrahedron SFS from its dehydration string;
            // obscure but painless at least.
            norSFS = Triangulation<3>::rehydrate("jnnafaabcfighhihimgbpqpepbr");
        }

        void tearDown() override {
        }

        void defaultArgs() {
            Triangulation<3> t(oneTet);

            // Make sure that enumeration using default arguments
            // falls through to the correct enumeration function, which takes
            // NormalFlags and NormalAlg.

            NormalSurfaces l1(t, NS_QUAD);
            if (l1.which() != (NS_VERTEX | regina::NS_EMBEDDED_ONLY)) {
                std::ostringstream msg;
                msg << "Enumeration with default (flags, algorithm) gave "
                    "incorrect flags " << l1.which().intValue() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NormalSurfaces l2(t, NS_QUAD, regina::NS_IMMERSED_SINGULAR);
            if (l2.which() != (NS_VERTEX | regina::NS_IMMERSED_SINGULAR)) {
                std::ostringstream msg;
                msg << "Enumeration with default algorithm gave "
                    "incorrect flags " << l2.which().intValue() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testSize(const NormalSurfaces& list,
                const char* listType, unsigned long expectedSize,
                const char* triName) {
            std::ostringstream msg;
            msg << "Number of " << listType << " for " << triName
                << " should be " << expectedSize << ", not "
                << list.size() << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(),
                list.size() == expectedSize);
        }

        void testSurface(const NormalSurface& surface, const char* triName,
                const char* surfaceName, int euler, bool connected,
                bool orient, bool twoSided, bool compact, bool realBdry,
                bool vertexLink, unsigned edgeLink,
                size_t central, bool splitting) {
            // Begin with the compactness test so we know which other
            // tests may be performed.
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be "
                    << (compact ? "compact." : "non-compact.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isCompact() == compact);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have Euler char. " << euler << ", not "
                    << surface.eulerChar() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.eulerChar() == euler);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (connected)
                    msg << "connected.";
                else
                    msg << "disconnected.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isConnected() == connected);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (orient)
                    msg << "orientable.";
                else
                    msg << "non-orientable.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isOrientable() == orient);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be ";
                if (twoSided)
                    msg << "2-sided.";
                else
                    msg << "1-sided.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isTwoSided() == twoSided);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have "
                    << (realBdry ? "real boundary." : "no real boundary.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.hasRealBoundary() == realBdry);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! vertexLink)
                    msg << "not ";
                msg << "be vertex linking.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isVertexLinking() == vertexLink);
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

                std::pair<const regina::Edge<3>*, const regina::Edge<3>*> links
                    = surface.isThinEdgeLink();
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
                    surface.isCentral() == central);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! splitting)
                    msg << "not ";
                msg << "be a splitting surface.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface.isSplitting() == splitting);
            }
        }

        void countCompactSurfaces(const NormalSurfaces& list,
                const char* surfaceType,
                unsigned long expectedCount, int euler,
                bool connected, bool orient, bool twoSided,
                bool realBdry, bool vertexLink, unsigned edgeLink,
                size_t central, bool splitting,
                const char* triName) {
            unsigned long tot = 0;

            for (const NormalSurface& s : list) {
                if (s.eulerChar() == euler &&
                        s.isConnected() == connected &&
                        s.isOrientable() == orient &&
                        s.isTwoSided() == twoSided &&
                        s.hasRealBoundary() == realBdry &&
                        s.isVertexLinking() == vertexLink &&
                        s.isCentral() == central &&
                        s.isSplitting() == splitting) {
                    std::pair<const regina::Edge<3>*, const regina::Edge<3>*> links
                        = s.isThinEdgeLink();
                    unsigned linkCount;
                    if (links.first == 0)
                        linkCount = 0;
                    else if (links.second == 0)
                        linkCount = 1;
                    else
                        linkCount = 2;

                    if (linkCount == edgeLink)
                        tot++;
                }
            }

            std::ostringstream msg;
            msg << "Number of " << surfaceType << " in " << triName
                    << " should be " << expectedCount << ", not " << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        template <typename T>
        static bool lexLess(const Vector<T>* a, const Vector<T>* b) {
            for (unsigned i = 0; i < a->size(); ++i) {
                if ((*a)[i] < (*b)[i])
                    return true;
                if ((*a)[i] > (*b)[i])
                    return false;
            }
            return false;
        }

        static bool identical(const NormalSurfaces& lhs,
                const NormalSurfaces& rhs) {
            if (lhs.size() != rhs.size())
                return false;

            unsigned long n = lhs.size();
            if (n == 0)
                return true;

            auto* lhsRaw = new const Vector<LargeInteger>*[n];
            auto* rhsRaw = new const Vector<LargeInteger>*[n];

            unsigned long i;
            for (i = 0; i < n; ++i) {
                lhsRaw[i] = &lhs.surface(i).vector();
                rhsRaw[i] = &rhs.surface(i).vector();
            }

            std::sort(lhsRaw, lhsRaw + n, lexLess<LargeInteger>);
            std::sort(rhsRaw, rhsRaw + n, lexLess<LargeInteger>);

            bool ok = true;
            for (i = 0; i < n; ++i)
                if (*(lhsRaw[i]) != *(rhsRaw[i])) {
                    ok = false;
                    break;
                }

            delete[] lhsRaw;
            delete[] rhsRaw;
            return ok;
        }

        void standardEmpty() {
            NormalSurfaces list(empty, NS_STANDARD);

            testSize(list, "standard normal surfaces", 0, "Empty");
        }

        void quadEmpty() {
            NormalSurfaces list(empty, NS_QUAD);

            testSize(list, "quad normal surfaces", 0, "Empty");
        }

        void almostNormalEmpty() {
            NormalSurfaces list(empty, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 0, "Empty");
        }

        void standardOneTet() {
            NormalSurfaces list(oneTet, NS_STANDARD);

            testSize(list, "standard normal surfaces", 7, "Lone tetrahedron");
            countCompactSurfaces(list, "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */,
                "Lone tetrahedron");
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "Lone tetrahedron");
        }

        void quadOneTet() {
            NormalSurfaces list(oneTet, NS_QUAD);

            testSize(list, "quad normal surfaces", 3, "Lone tetrahedron");
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "Lone tetrahedron");
        }

        void almostNormalOneTet() {
            NormalSurfaces list(oneTet, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 10,
                "Lone tetrahedron");
            countCompactSurfaces(list, "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */,
                "Lone tetrahedron");
            countCompactSurfaces(list, "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "Lone tetrahedron");
            countCompactSurfaces(list, "octagonal discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */,
                "Lone tetrahedron");
        }

        void standardGieseking() {
            NormalSurfaces list(gieseking, NS_STANDARD);

            testSize(list, "standard normal surfaces", 1, "Gieseking");
            testSurface(list.surface(0), "Gieseking",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
        }

        void quadGieseking() {
            NormalSurfaces list(gieseking, NS_QUAD);

            testSize(list, "quad normal surfaces", 0, "Gieseking");
        }

        void almostNormalGieseking() {
            NormalSurfaces list(gieseking, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 1, "Gieseking");
            testSurface(list.surface(0), "Gieseking",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
        }

        void standardFigure8() {
            NormalSurfaces list(figure8, NS_STANDARD);

            testSize(list, "standard normal surfaces", 1, "Figure eight");
            testSurface(list.surface(0), "Figure eight",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
        }

        void quadFigure8() {
            NormalSurfaces list(figure8, NS_QUAD);

            testSize(list, "quad normal surfaces", 4, "Figure eight");
            for (const NormalSurface& s : list)
                testSurface(s, "Figure eight", "spun surface",
                    0 /* euler, N/A */, false /* connected, N/A */,
                    false /* orient, N/A */, false /* two-sided, N/A */,
                    false /* compact */, false /* realBdry */,
                    false /* vertex link */, 0 /* edge link */,
                    0 /* central */, false /* splitting */);
        }

        void almostNormalFigure8() {
            NormalSurfaces list(figure8, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 1,
                "Figure eight");
            testSurface(list.surface(0), "Figure eight",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
        }

        void standardS3() {
            NormalSurfaces list(S3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 3, "S3");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "S3");
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "S3");
        }

        void quadS3() {
            NormalSurfaces list(S3, NS_QUAD);

            testSize(list, "quad normal surfaces", 1, "S3");
            countCompactSurfaces(list,
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "S3");
        }

        void almostNormalS3() {
            NormalSurfaces list(S3, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 4, "S3");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "S3");
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */,
                "S3");
            countCompactSurfaces(list,
                "standard almost normal central 2-spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */,
                "S3");
        }

        void standardLoopC2() {
            NormalSurfaces list(loopC2, NS_STANDARD);

            testSize(list, "standard normal surfaces", 5, "C(2)");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "C(2)");
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
            countCompactSurfaces(list,
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
        }

        void quadLoopC2() {
            NormalSurfaces list(loopC2, NS_QUAD);

            testSize(list, "quad normal surfaces", 3, "C(2)");
            countCompactSurfaces(list,
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
            countCompactSurfaces(list,
                "quad normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
        }

        void almostNormalLoopC2() {
            NormalSurfaces list(loopC2, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 5, "C(2)");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "C(2)");
            countCompactSurfaces(list,
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
            countCompactSurfaces(list,
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */,
                "C(2)");
        }

        void standardLoopCtw3() {
            NormalSurfaces list(loopCtw3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 5, "C~(3)");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "C~(3)");
            countCompactSurfaces(list,
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "C~(3)");
            countCompactSurfaces(list,
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */,
                "C~(3)");
        }

        void quadLoopCtw3() {
            NormalSurfaces list(loopCtw3, NS_QUAD);

            testSize(list, "quad normal surfaces", 4, "C~(3)");
            countCompactSurfaces(list,
                "quad normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "C~(3)");
            countCompactSurfaces(list,
                "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */,
                "C~(3)");
        }

        void almostNormalLoopCtw3() {
            NormalSurfaces list(loopCtw3, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 5, "C~(3)");
            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "C~(3)");
            countCompactSurfaces(list,
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "C~(3)");
            countCompactSurfaces(list,
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */,
                "C~(3)");
        }

        void standardLargeS3() {
            NormalSurfaces list(largeS3, NS_STANDARD);

            testSize(list, "standard normal surfaces", 15, "Large S3");
            countCompactSurfaces(list,
                "standard normal vertex linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal vertex linking non-central spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                2 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal edge linking non-central tori", 2,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal edge linking central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                5 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal miscellaneous spheres", 3,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal miscellaneous tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "standard normal splitting genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                5 /* central */, true /* splitting */,
                "Large S3");
        }

        void quadLargeS3() {
            NormalSurfaces list(largeS3, NS_QUAD);

            testSize(list, "quad normal surfaces", 4, "Large S3");
            countCompactSurfaces(list,
                "quad normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "quad normal edge linking non-central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
            countCompactSurfaces(list,
                "quad normal miscellaneous spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large S3");
        }

        void almostNormalLargeS3() {
            NormalSurfaces list(largeS3, NS_AN_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 27, "Large S3");
        }

        void standardLargeRP3() {
            NormalSurfaces list(largeRP3, NS_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 29, "Large RP3");
        }

        void quadLargeRP3() {
            NormalSurfaces list(largeRP3, NS_QUAD);

            testSize(list, "quad normal surfaces", 5, "Large RP3");
            countCompactSurfaces(list,
                "quad normal edge linking non-central spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large RP3");
            countCompactSurfaces(list,
                "quad normal edge linking non-central tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large RP3");
            countCompactSurfaces(list,
                "quad normal miscellaneous spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large RP3");
            countCompactSurfaces(list,
                "quad normal miscellaneous projective planes", 1,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Large RP3");
        }

        void almostNormalLargeRP3() {
            NormalSurfaces list(largeRP3, NS_AN_STANDARD);

            // Bleh.  Too messy.  Just count them.
            testSize(list, "standard normal surfaces", 59, "Large RP3");
        }

        void standardTwistedKxI() {
            NormalSurfaces list(twistedKxI, NS_STANDARD);

            testSize(list, "standard normal surfaces", 8, "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
        }

        void quadTwistedKxI() {
            NormalSurfaces list(twistedKxI, NS_QUAD);

            testSize(list, "quad normal surfaces", 6, "Twisted KxI");
            countCompactSurfaces(list,
                "quad normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "quad normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "quad normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "quad normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "quad normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
        }

        void almostNormalTwistedKxI() {
            NormalSurfaces list(twistedKxI, NS_AN_STANDARD);

            testSize(list, "standard almost normal surfaces", 13,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, non-orbl)", 2,
                -1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, orbl)", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
            countCompactSurfaces(list,
                "standard almost normal surfaces "
                    "(chi=-2, 1-sided, non-orbl)", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                "Twisted KxI");
        }

        void standardNorSFS() {
            NormalSurfaces list(norSFS, NS_STANDARD);

            testSize(list, "standard normal surfaces", 25,
                "SFS [RP2: (2,1) (2,1) (2,1)]");

            countCompactSurfaces(list,
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal two-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal one-sided genus two Klein bottles", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "standard normal central one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void quadNorSFS() {
            NormalSurfaces list(norSFS, NS_QUAD);

            testSize(list, "quad normal surfaces", 21,
                "SFS [RP2: (2,1) (2,1) (2,1)]");

            countCompactSurfaces(list,
                "quad normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
            countCompactSurfaces(list,
                "quad normal one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */,
                    "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void testStandardLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop = Example<3>::layeredLoop(len, true);
            NormalSurfaces list(loop, NS_STANDARD);

            std::ostringstream label;
            label << "the twisted layered loop C~(" << len << ")";
            const std::string name = label.str();

            // For standard normal and almost normal coordinates we just
            // count the surfaces (as opposed to in quad space, where we can
            // describe the surfaces precisely, with proof).

            // The following pattern has been observed experimentally.
            // For the purposes of the test suite I'm happy to assume it
            // holds in general; certainly it has been verified for all
            // the cases that we actually test here.
            unsigned long curr, prev, tmp;
            if (len == 1)
                curr = 2;
            else if (len == 2)
                curr = 4;
            else {
                curr = 4;
                prev = 2;
                for (unsigned counted = 2; counted < len; ++counted) {
                    tmp = curr + prev - 1;
                    prev = curr;
                    curr = tmp;
                }
            }

            testSize(list, "standard normal surfaces", curr, name.c_str());
        }

        void testQuadLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop = Example<3>::layeredLoop(len, true);
            NormalSurfaces list(loop, NS_QUAD);

            std::ostringstream label;
            label << "the twisted layered loop C~(" << len << ")";
            const std::string name = label.str();

            // It is easy to prove in general that C~(len) has precisely
            // (len + 1) vertex surfaces, as described by the following tests.
            testSize(list, "quad normal surfaces", len + 1, name.c_str());
            countCompactSurfaces(list, "quad normal edge linking tori", len,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */,
                name.c_str());
            countCompactSurfaces(list, "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                len /* central */, true /* splitting */,
                name.c_str());
        }

        void testAlmostNormalLoopCtwGeneric(unsigned len) {
            Triangulation<3> loop = Example<3>::layeredLoop(len, true);
            NormalSurfaces list(loop, NS_AN_STANDARD);

            std::ostringstream label;
            label << "the twisted layered loop C~(" << len << ")";
            const std::string name = label.str();

            // For standard normal and almost normal coordinates we just
            // count the surfaces (as opposed to in quad space, where we can
            // describe the surfaces precisely, with proof).

            // The following pattern has been observed experimentally.
            // For the purposes of the test suite I'm happy to assume it
            // holds in general; certainly it has been verified for all
            // the cases that we actually test here.
            unsigned long curr, prev, tmp, currgap, prevgap;
            if (len < 7) {
                switch (len) {
                    case 1: curr = 3; break;
                    case 2: curr = 4; break;
                    case 3: curr = 5; break;
                    case 4: curr = 12; break;
                    case 5: curr = 12; break;
                    case 6: curr = 25; break;
                }
            } else {
                prev = 12; prevgap = 2;
                curr = 25; currgap = 1;

                for (unsigned counted = 6; counted < len; ++counted) {
                    tmp = curr + prev + currgap - 1;
                    prev = curr;
                    curr = tmp;

                    tmp = currgap + prevgap;
                    prevgap = currgap;
                    currgap = tmp;
                }
            }

            testSize(list, "standard almost normal surfaces", curr,
                name.c_str());
        }

        void largeDimensionsStandard() {
            testStandardLoopCtwGeneric(4);
            testStandardLoopCtwGeneric(8);
            testStandardLoopCtwGeneric(12);
        }

        void largeDimensionsQuad() {
            testQuadLoopCtwGeneric(5);
            testQuadLoopCtwGeneric(10);
            testQuadLoopCtwGeneric(20);
            testQuadLoopCtwGeneric(30);
            testQuadLoopCtwGeneric(40);
            testQuadLoopCtwGeneric(50);
        }

        void largeDimensionsAlmostNormal() {
            testAlmostNormalLoopCtwGeneric(3);
            testAlmostNormalLoopCtwGeneric(6);
            testAlmostNormalLoopCtwGeneric(9);
            testAlmostNormalLoopCtwGeneric(12);
            testAlmostNormalLoopCtwGeneric(15);
        }

        static void verifyConversions(const Triangulation<3>& tri,
                const char* name) {
            NormalSurfaces stdDirect(tri, NS_STANDARD, NS_VERTEX,
                NS_VERTEX_STD_DIRECT);
            NormalSurfaces stdConv(tri, NS_STANDARD, NS_VERTEX,
                NS_VERTEX_VIA_REDUCED);
            if ((! tri.isEmpty()) &&
                    (stdDirect.algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                    ! stdDirect.algorithm().has(NS_VERTEX_STD_DIRECT))) {
                std::ostringstream msg;
                msg << "Direct enumeration in standard coordinates gives "
                    "incorrect algorithm flags for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (tri.isValid() && ! tri.isIdeal()) {
                if ((! tri.isEmpty()) &&
                        (stdConv.algorithm().has(NS_VERTEX_STD_DIRECT) ||
                        ! stdConv.algorithm().has(NS_VERTEX_VIA_REDUCED))) {
                    std::ostringstream msg;
                    msg << "Quad-to-standard conversion gives "
                        "incorrect algorithm flags for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                // Ideal or invalid triangluations should use the standard
                // enumeration process regardless of what the user requested.
                if ((! tri.isEmpty()) &&
                        (stdConv.algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                        ! stdConv.algorithm().has(NS_VERTEX_STD_DIRECT))) {
                    std::ostringstream msg;
                    msg << "Quad-to-standard conversion request was "
                        "incorrectly granted for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (! identical(stdDirect, stdConv)) {
                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard coordinates for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }

            // Only test standard-to-quad if the preconditions for
            // NS_CONV_STD_TO_REDUCED hold.
            if (tri.isValid() && ! tri.isIdeal()) {
                NormalSurfaces quadDirect(tri, NS_QUAD);
                NormalSurfaces quadConv(stdDirect,
                    regina::NS_CONV_STD_TO_REDUCED);
                if (! identical(quadDirect, quadConv)) {
                    std::ostringstream msg;
                    msg << "Direct enumeration vs conversion gives different "
                        "surfaces in quadrilateral coordinates for "
                        << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyConversionsAN(const Triangulation<3>& tri,
                const char* name) {
            NormalSurfaces stdDirect(tri, NS_AN_STANDARD, NS_VERTEX,
                NS_VERTEX_STD_DIRECT);
            NormalSurfaces stdConv(tri, NS_AN_STANDARD, NS_VERTEX,
                NS_VERTEX_VIA_REDUCED);
            if ((! tri.isEmpty()) &&
                    (stdDirect.algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                    ! stdDirect.algorithm().has(NS_VERTEX_STD_DIRECT))) {
                std::ostringstream msg;
                msg << "Direct enumeration in standard AN coordinates gives "
                    "incorrect algorithm flags for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (tri.isValid() && ! tri.isIdeal()) {
                if ((! tri.isEmpty()) &&
                        (stdConv.algorithm().has(NS_VERTEX_STD_DIRECT) ||
                        ! stdConv.algorithm().has(NS_VERTEX_VIA_REDUCED))) {
                    std::ostringstream msg;
                    msg << "Quad-oct-to-standard-AN conversion gives "
                        "incorrect algorithm flags for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                // Ideal or invalid triangluations should use the standard
                // enumeration process regardless of what the user requested.
                if ((! tri.isEmpty()) &&
                        (stdConv.algorithm().has(NS_VERTEX_VIA_REDUCED) ||
                        ! stdConv.algorithm().has(NS_VERTEX_STD_DIRECT))) {
                    std::ostringstream msg;
                    msg << "Quad-oct-to-standard-AN conversion request was "
                        "incorrectly granted for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (! identical(stdDirect, stdConv)) {
                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard almost normal coordinates for "
                        << name << '.';
                CPPUNIT_FAIL(msg.str());
            }

            // Only test standard-to-quad if the preconditions for
            // NS_CONV_STD_TO_REDUCED hold.
            if (tri.isValid() && ! tri.isIdeal()) {
                NormalSurfaces quadDirect(tri, NS_AN_QUAD_OCT);
                NormalSurfaces quadConv(stdDirect,
                    regina::NS_CONV_STD_TO_REDUCED);
                if (! identical(quadDirect, quadConv)) {
                    std::ostringstream msg;
                    msg << "Direct enumeration vs conversion gives different "
                        "surfaces in quadrilateral-octagon coordinates for "
                        << name << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void standardQuadConversionsConstructed() {
            verifyConversions(empty, "Empty");
            verifyConversions(oneTet, "Lone tetrahedron");
            verifyConversions(S3, "S3");
            verifyConversions(loopC2, "C(2)");
            verifyConversions(loopCtw3, "C~(3)");
            verifyConversions(largeS3, "Large S3");
            verifyConversions(largeRP3, "Large RP3");
            verifyConversions(twistedKxI, "Twisted KxI");
            verifyConversions(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void standardQuadConversionsCensus() {
            runCensusMinClosed(&verifyConversions);
            runCensusAllClosed(&verifyConversions);
            runCensusAllBounded(&verifyConversions);
            runCensusAllIdeal(&verifyConversions);
        }

        void standardANQuadOctConversionsConstructed() {
            verifyConversionsAN(empty, "Empty");
            verifyConversionsAN(oneTet, "Lone tetrahedron");
            verifyConversionsAN(S3, "S3");
            verifyConversionsAN(loopC2, "C(2)");
            verifyConversionsAN(loopCtw3, "C~(3)");
            verifyConversionsAN(largeS3, "Large S3");
            verifyConversionsAN(largeRP3, "Large RP3");
            verifyConversionsAN(twistedKxI, "Twisted KxI");
            verifyConversionsAN(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void standardANQuadOctConversionsCensus() {
            runCensusMinClosed(verifyConversionsAN);
            runCensusAllClosed(verifyConversionsAN);
            runCensusAllBounded(verifyConversionsAN);
            runCensusAllIdeal(verifyConversionsAN);
        }

        template <regina::NormalCoords coords>
        static void verifyTreeVsDD(const Triangulation<3>& tri,
                const char* name) {
            std::unique_ptr<NormalSurfaces> dd;
            std::unique_ptr<NormalSurfaces> tree;

            try {
                dd = std::make_unique<NormalSurfaces>(tri, coords, NS_VERTEX,
                    NS_VERTEX_DD | NS_VERTEX_STD_DIRECT);
            } catch (const regina::InvalidArgument&) {
            } catch (const regina::UnsolvedCase&) {
            }
            try {
                tree = std::make_unique<NormalSurfaces>(tri, coords, NS_VERTEX,
                    NS_VERTEX_TREE | NS_VERTEX_STD_DIRECT);
            } catch (const regina::InvalidArgument&) {
            } catch (const regina::UnsolvedCase&) {
            }

            if (dd && ! tree) {
                std::ostringstream msg;
                msg << "Enumeration in coordinate system "
                    << coords << " fails via tree traversal "
                    "but not double description for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (tree && ! dd) {
                std::ostringstream msg;
                msg << "Enumeration in coordinate system "
                    << coords << " fails via double description "
                    "but not tree traversal for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! dd) {
                if (coords != NS_QUAD_CLOSED &&
                        coords != NS_AN_QUAD_OCT_CLOSED) {
                    // Enumeration should not fail in this coordinate system.
                    std::ostringstream msg;
                    msg << "Enumeration in coordinate system "
                        << coords << " fails for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                } else
                    return;
            }
            if ((! tri.isEmpty()) &&
                    (dd->algorithm().has(NS_VERTEX_TREE) ||
                    ! dd->algorithm().has(NS_VERTEX_DD))) {
                std::ostringstream msg;
                msg << "Double description enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if ((! tri.isEmpty()) &&
                    (tree->algorithm().has(NS_VERTEX_DD) ||
                    ! tree->algorithm().has(NS_VERTEX_TREE))) {
                std::ostringstream msg;
                msg << "Tree traversal enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(*dd, *tree)) {
                std::ostringstream msg;
                msg << "Double description vs tree enumeration in "
                    "coordinate system " << coords << " gives different "
                    "surfaces for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <regina::NormalCoords coords>
        void treeVsDDCensus() {
            runCensusMinClosed(verifyTreeVsDD<coords>);
            runCensusAllClosed(verifyTreeVsDD<coords>);
            runCensusAllBounded(verifyTreeVsDD<coords>);
            runCensusAllIdeal(verifyTreeVsDD<coords>);
        }

        void verifyEulerZeroNoPositive(Triangulation<3>& tri,
                const char* name) {
            // Preconditions for tree traversal:
            if (tri.isEmpty()) {
                std::ostringstream msg;
                msg << "Chi=0: triangulation is empty: " << name;
                CPPUNIT_FAIL(msg.str());
            }

            // Collect all vertex surfaces with the chi=0 constraint.
            std::vector<Vector<LargeInteger>*> eulerZero;
            regina::TreeEnumeration<regina::LPConstraintEulerZero> tree(
                tri, NS_STANDARD);
            while (tree.next()) {
                NormalSurface s = tree.buildSurface();
                if (s.eulerChar() != 0) {
                    std::ostringstream msg;
                    msg << "Chi=0: custom list contains a surface with "
                        "chi = " << s.eulerChar() << ": " << name;
                    CPPUNIT_FAIL(msg.str());
                }
                eulerZero.push_back(new Vector<LargeInteger>(s.vector()));
            }

            // Collect *all* vertex surfaces in the normal way, and extract
            // only those with chi=0.
            std::vector<const Vector<LargeInteger>*> filtered;
            NormalSurfaces all(tri, NS_STANDARD);
            for (const NormalSurface& s : all)
                if (s.eulerChar() == 0)
                    filtered.push_back(&s.vector());

            // Ensure that every vertex surface with chi=0 was picked up
            // in our custom list.

            std::sort(eulerZero.begin(), eulerZero.end(), lexLess<LargeInteger>);
            std::sort(filtered.begin(), filtered.end(), lexLess<LargeInteger>);

            auto customIt = eulerZero.begin();
            auto allIt = filtered.begin();

            while (allIt != filtered.end()) {
                // We are looking for *allIt in our custom list.
                if (customIt == eulerZero.end()) {
                    std::ostringstream msg;
                    msg << "Chi=0: custom list is missing a vertex surface: "
                        << name;
                    CPPUNIT_FAIL(msg.str());
                }
                if (**customIt == **allIt) {
                    ++customIt;
                    ++allIt;
                    continue;
                }
                // Assume *customIt is one of the extra (non-vertex) surfaces
                // that we picked up by adding chi=0 as a new constraint.
                ++customIt;
            }

            for (auto ray : eulerZero)
                delete ray;
        }

        void eulerZero() {
            verifyEulerZeroNoPositive(oneTet, "Lone tetrahedron");
            verifyEulerZeroNoPositive(figure8, "Figure eight");
            verifyEulerZeroNoPositive(gieseking, "Gieseking");
            verifyEulerZeroNoPositive(S3, "S3");
            verifyEulerZeroNoPositive(loopC2, "C(2)");
            verifyEulerZeroNoPositive(loopCtw3, "C~(3)");
            verifyEulerZeroNoPositive(largeS3, "Large S3");
            verifyEulerZeroNoPositive(largeRP3, "Large RP3");
            verifyEulerZeroNoPositive(twistedKxI, "Twisted KxI");
            verifyEulerZeroNoPositive(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        template <regina::NormalCoords coords>
        static void verifyFundPrimalVsDual(const Triangulation<3>& tri,
                const char* name) {
            std::unique_ptr<NormalSurfaces> primal;
            std::unique_ptr<NormalSurfaces> dual;

            try {
                primal = std::make_unique<NormalSurfaces>(tri, coords,
                    NS_FUNDAMENTAL, NS_HILBERT_PRIMAL);
            } catch (const regina::InvalidArgument&) {
            } catch (const regina::UnsolvedCase&) {
            }
            try {
                dual = std::make_unique<NormalSurfaces>(tri, coords,
                    NS_FUNDAMENTAL, NS_HILBERT_DUAL);
            } catch (const regina::InvalidArgument&) {
            } catch (const regina::UnsolvedCase&) {
            }

            if (primal && ! dual) {
                std::ostringstream msg;
                msg << "Hilbert basis enumeration in coordinate system "
                    << coords << " fails via dual method "
                    "but not primal method for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (dual && ! primal) {
                std::ostringstream msg;
                msg << "Hilbert basis enumeration in coordinate system "
                    << coords << " fails via primal method "
                    "but not dual method for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! primal) {
                if (coords != NS_QUAD_CLOSED &&
                        coords != NS_AN_QUAD_OCT_CLOSED) {
                    // Enumeration should not fail in this coordinate system.
                    std::ostringstream msg;
                    msg << "Hilbert basis enumeration in coordinate system "
                        << coords << " fails for " << name << '.';
                    CPPUNIT_FAIL(msg.str());
                } else
                    return;
            }
            if ((! tri.isEmpty()) &&
                    (primal->algorithm().has(NS_HILBERT_DUAL) ||
                    ! primal->algorithm().has(NS_HILBERT_PRIMAL))) {
                std::ostringstream msg;
                msg << "Primal Hilbert basis enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if ((! tri.isEmpty()) &&
                    (dual->algorithm().has(NS_HILBERT_PRIMAL) ||
                    ! dual->algorithm().has(NS_HILBERT_DUAL))) {
                std::ostringstream msg;
                msg << "Dual Hilbert basis enumeration in coordinate system "
                    << coords << " gives incorrect algorithm flags for "
                    << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(*primal, *dual)) {
                std::ostringstream msg;
                msg << "Primal vs dual Hilbert basis enumeration in "
                    "coordinate system " << coords << " gives different "
                    "surfaces for " << name << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <regina::NormalCoords coords>
        void fundPrimalVsDual() {
            runCensusMinClosed(verifyFundPrimalVsDual<coords>, true);
            runCensusAllClosed(verifyFundPrimalVsDual<coords>, true);
            runCensusAllBounded(verifyFundPrimalVsDual<coords>, true);
            runCensusAllIdeal(verifyFundPrimalVsDual<coords>, true);
        }

        static void testDisjoint(const Triangulation<3>& tri,
                const char* name) {
            NormalSurfaces list(tri, NS_AN_STANDARD);
            unsigned long n = list.size();

            unsigned long i, j;
            std::pair<const Edge<3>*, const Edge<3>*> edges;
            unsigned long edge;

            for (i = 0; i < n; ++i) {
                const NormalSurface& s = list.surface(i);

                // For some types of surfaces we know exactly what it
                // should be disjoint from.
                if (s.isVertexLinking()) {
                    // Vertex links are disjoint from everything.
                    for (j = 0; j < n; ++j) {
                        const NormalSurface& t = list.surface(j);
                        if (! s.disjoint(t)) {
                            std::ostringstream msg;
                            msg << "Surface #" << i << " for " << name
                                << " is a vertex link "
                                "and therefore should be disjoint from "
                                "surface #" << j << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                } else if ((edges = s.isThinEdgeLink()).first) {
                    // A thin edge link is disjoint from (i) all vertex
                    // links, and (ii) all surfaces that do not meet the
                    // relevant edge (except the edge link itself, if it
                    // is 1-sided).
                    edge = edges.first->index();

                    for (j = 0; j < n; ++j) {
                        // Deal with (s, s) later.
                        if (j == i)
                            continue;

                        const NormalSurface& t = list.surface(j);
                        if (t.isVertexLinking()) {
                            if (! s.disjoint(t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i << " for " << name
                                    << " is a thin edge link and therefore "
                                    "should be disjoint from surface #" << j
                                    << ", which is a vertex link.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else if (t.edgeWeight(edge) == 0) {
                            if (! s.disjoint(t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i << " for " << name
                                    << " is a thin edge link and therefore "
                                    "should be disjoint from surface #" << j
                                    << ", which does not meet the "
                                    "corresponding edge.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (s.disjoint(t)) {
                                std::ostringstream msg;
                                msg << "Surface #" << i <<
                                    " is a thin edge link and therefore "
                                    "should not be disjoint from surface #"
                                    << j << ", which meets the "
                                    "corresponding edge.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }

                // Ensure that the surface is disjoint from itself
                // iff it is two-sided.
                if (s.isTwoSided() && ! s.disjoint(s)) {
                    std::ostringstream msg;
                    msg << "Surface #" << i << " for " << name
                        << " is two-sided and therefore should be "
                        "disjoint from itself.";
                    CPPUNIT_FAIL(msg.str());
                } else if ((! s.isTwoSided()) && s.disjoint(s)) {
                    std::ostringstream msg;
                    msg << "Surface #" << i << " for " << name
                        << " is one-sided and therefore should not be "
                        "disjoint from itself.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void disjointConstructed() {
            testDisjoint(oneTet, "Lone tetrahedron");
            testDisjoint(figure8, "Figure eight");
            testDisjoint(gieseking, "Gieseking");
            testDisjoint(S3, "S3");
            testDisjoint(loopC2, "C(2)");
            testDisjoint(loopCtw3, "C~(3)");
            testDisjoint(largeS3, "Large S3");
            testDisjoint(largeRP3, "Large RP3");
            testDisjoint(twistedKxI, "Twisted KxI");
            testDisjoint(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void disjointCensus() {
            runCensusAllClosed(&testDisjoint);
            runCensusAllBounded(&testDisjoint);
            runCensusAllIdeal(&testDisjoint);
        }

        /**
         * PRE: tri is valid with only one component, and all vertex
         * links are spheres or discs.
         */
        static bool mightBeTwistedProduct(const Triangulation<3>& tri) {
            if (tri.countBoundaryComponents() != 1)
                return false;

            // Check the relationship between H1 and H1Bdry.
            // We must have one of:
            //  -  H1 = (2g)Z, H1Bdry = (4g-2)Z;
            //  -  H1 = Z_2 + (g-1)Z, H1Bdry = Z_2 + (2g-3)Z;
            //  -  H1 = Z_2 + (g-1)Z, H1Bdry = (2g-2)Z;
            AbelianGroup h1 = tri.homology();
            const AbelianGroup& bdry = tri.homologyBdry();

            if (h1.countInvariantFactors() == 0) {
                // Must have H1 = (2g)Z.
                if (bdry.countInvariantFactors() != 0)
                    return false;
                if (bdry.rank() != 2 * h1.rank() - 2)
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
         * PRE: tri is valid with only one component, and all vertex
         * links are spheres or discs.
         */
        static bool mightBeUntwistedProduct(const Triangulation<3>& tri) {
            if (tri.countBoundaryComponents() != 2)
                return false;

            // Check that both boundary components are homeomorphic.
            BoundaryComponent<3>* b0 = tri.boundaryComponent(0);
            BoundaryComponent<3>* b1 = tri.boundaryComponent(1);

            if (b0->eulerChar() != b1->eulerChar())
                return false;
            if (b0->isOrientable() && ! b1->isOrientable())
                return false;
            if (b1->isOrientable() && ! b0->isOrientable())
                return false;

            // Check that H1 is of the form (k)Z or Z_2 + (k)Z, and that
            // H1Bdry = 2 H1.
            AbelianGroup h1 = tri.homology();
            AbelianGroup bdry = tri.homologyBdry();

            if (h1.countInvariantFactors() == 0) {
                // Must have H1 = (k)Z.
                if (bdry.rank() != 2 * h1.rank())
                    return false;
                if (bdry.countInvariantFactors() != 0)
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

        // Check whether the boundary of the given triangulation *might*
        // be equal to (i) the surface s, (ii) two copies of the surface s,
        // or (iii) a double cover of the surface s.
        // Increment the relevant counters accordingly.
        static void checkBoundaryType(const NormalSurface& s,
                const Triangulation<3>& tri, unsigned& foundS,
                unsigned& foundTwoCopies, unsigned& foundDoubleCover) {
            if (tri.countBoundaryComponents() == 1) {
                const BoundaryComponent<3>* b = tri.boundaryComponent(0);

                if (s.eulerChar() == b->eulerChar()
                        && s.isOrientable() == b->isOrientable())
                    ++foundS;
                if (s.eulerChar() * 2 == b->eulerChar() &&
                        (b->isOrientable() || ! s.isOrientable()))
                    ++foundDoubleCover;
            } else if (tri.countBoundaryComponents() == 2) {
                const BoundaryComponent<3>* b0 = tri.boundaryComponent(0);
                const BoundaryComponent<3>* b1 = tri.boundaryComponent(1);

                if (
                        s.eulerChar() == b0->eulerChar() &&
                        s.eulerChar() == b1->eulerChar() &&
                        s.isOrientable() == b0->isOrientable() &&
                        s.isOrientable() == b1->isOrientable())
                    ++foundTwoCopies;
            }
        }

        /**
         * PRE: tri is valid and has only one component.
         */
        static void testCutAlong(const Triangulation<3>& tri,
                const char* name) {
            NormalSurfaces list(tri, NS_STANDARD);

            bool separating;
            unsigned long expected;

            // We use the fact that each normal surface is connected.
            for (const NormalSurface& s : list) {
                Triangulation<3> t = s.cutAlong();
                t.intelligentSimplify();

                Triangulation<3> tDouble = (s * 2).cutAlong();
                tDouble.intelligentSimplify();

                auto comp = t.triangulateComponents();
                auto compDouble = tDouble.triangulateComponents();

                separating = (s.isTwoSided() && comp.size() > 1);

                expected = (separating ? 2 : 1);
                if (comp.size() != expected) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for " << name
                        << " gives " << comp.size() << " component(s), not "
                        << expected << " as expected.";
                    CPPUNIT_FAIL(msg.str());
                }

                expected = (separating ? 3 : 2);
                if (compDouble.size() != expected) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for " << name
                        << " gives " << compDouble.size() <<
                        " component(s), not " << expected << " as expected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! t.isValid()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for " << name
                        << " gives an invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! tDouble.isValid()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for " << name
                        << " gives an invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isIdeal() && ! t.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for "
                        << name << " (which is ideal)"
                        << " gives a non-ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (tri.isIdeal() && ! tDouble.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for "
                        << name << " (which is ideal)"
                        << " gives a non-ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if ((! tri.isIdeal()) && t.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for "
                        << name << " (which is not ideal)"
                        << " gives an ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if ((! tri.isIdeal()) && tDouble.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for "
                        << name << " (which is not ideal)"
                        << " gives an ideal triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isOrientable() && ! t.isOrientable()) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for "
                        << name << " (which is orientable)"
                        << " gives a non-orientable triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (tri.isOrientable() && ! tDouble.isOrientable()) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for "
                        << name << " (which is orientable)"
                        << " gives a non-orientable triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (const Triangulation<3>& c : comp)
                    if (! c.hasBoundaryTriangles()) {
                        std::ostringstream msg;
                        msg << "Cutting along surface for " << name
                            << " gives a component with no boundary triangles.";
                        CPPUNIT_FAIL(msg.str());
                    }
                for (const Triangulation<3>& c : compDouble)
                    if (! c.hasBoundaryTriangles()) {
                        std::ostringstream msg;
                        msg << "Cutting along double surface for " << name
                            << " gives a component with no boundary triangles.";
                        CPPUNIT_FAIL(msg.str());
                    }

                // The remaining tests only work for closed triangulations.
                if (! tri.isClosed())
                    continue;

                // Check the boundaries of components of t.
                unsigned expectS, expectTwoCopies, expectDoubleCover;
                unsigned foundS, foundTwoCopies, foundDoubleCover;
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
                if (t.countBoundaryComponents() !=
                        expectS + 2 * expectTwoCopies + expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for " << name
                        << " gives the wrong number of boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
                foundS = foundTwoCopies = foundDoubleCover = 0;
                for (const Triangulation<3>& c : comp)
                    checkBoundaryType(s, c,
                        foundS, foundTwoCopies, foundDoubleCover);
                if (foundS < expectS || foundTwoCopies < expectTwoCopies ||
                        foundDoubleCover < expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along surface for " << name
                        << " gives boundary components of the wrong type.";
                    CPPUNIT_FAIL(msg.str());
                }

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
                if (tDouble.countBoundaryComponents() !=
                        expectS + 2 * expectTwoCopies + expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for " << name
                        << " gives the wrong number of boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
                foundS = foundTwoCopies = foundDoubleCover = 0;
                for (const Triangulation<3>& c : compDouble)
                    checkBoundaryType(s, c,
                        foundS, foundTwoCopies, foundDoubleCover);
                if (foundS < expectS || foundTwoCopies < expectTwoCopies ||
                        foundDoubleCover < expectDoubleCover) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for " << name
                        << " gives boundary components of the wrong type.";
                    CPPUNIT_FAIL(msg.str());
                }

                // Look for the product piece when cutting along the
                // double surface.
                bool found = false;
                for (const Triangulation<3>& c : compDouble) {
                    if (s.isTwoSided()) {
                        if (mightBeUntwistedProduct(c)) {
                            found = true;
                            break;
                        }
                    } else {
                        if (mightBeTwistedProduct(c)) {
                            found = true;
                            break;
                        }
                    }
                }
                if (! found) {
                    std::ostringstream msg;
                    msg << "Cutting along double surface for " << name
                        << " does not yield a product piece as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void cutAlongConstructed() {
            testCutAlong(oneTet, "Lone tetrahedron");
            testCutAlong(figure8, "Figure eight");
            testCutAlong(gieseking, "Gieseking");
            testCutAlong(S3, "S3");
            testCutAlong(loopC2, "C(2)");
            testCutAlong(loopCtw3, "C~(3)");
            testCutAlong(largeS3, "Large S3");
            testCutAlong(largeRP3, "Large RP3");
            testCutAlong(twistedKxI, "Twisted KxI");
            testCutAlong(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void cutAlongCensus() {
            runCensusAllClosed(&testCutAlong, true);
            runCensusAllBounded(&testCutAlong, true);
            runCensusAllIdeal(&testCutAlong, true);
        }

        static void testRemoveOctsInCoords(regina::NormalCoords coords,
                const Triangulation<3>& tri, const char* name) {
            NormalSurfaces list(tri, coords);

            for (const NormalSurface& s : list) {
                NormalSurface noOct = s.removeOcts();

                if (noOct.encoding().storesOctagons()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " gives a surface vector that still encodes "
                        "octagons.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! noOct.normal()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " gives a surface that still contains octagons.";
                    CPPUNIT_FAIL(msg.str());
                }

                // Internally, the no-octagon variants should always be
                // stored using the standard matching equations.
                regina::MatrixInt matching = regina::makeMatchingEquations(
                    noOct.triangulation(), NS_STANDARD);
                if (noOct.vector().size() != matching.columns()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " gives a surface vector of the incorrect size.";
                    CPPUNIT_FAIL(msg.str());
                }
                // For non-compact surfaces we should test the quad
                // matching equations, but for now we leave it.
                // The standard matching equations will fail because
                // infinity - infinity != 0.
                if (s.isCompact())
                    if (! (matching * noOct.vector()).isZero()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " gives a surface vector that fails the "
                            "standard matching equations.";
                        CPPUNIT_FAIL(msg.str());
                    }
                if (noOct.isOrientable() != s.isOrientable()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks orientability.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.isTwoSided() != s.isTwoSided()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks two-sidedness.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.isCompact() != s.isCompact()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks compactness.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.isConnected() != s.isConnected()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks connectedness.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.hasRealBoundary() != s.hasRealBoundary()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks the existence of real boundary.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.eulerChar() != s.eulerChar()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (noOct.countBoundaries() != s.countBoundaries()) {
                    std::ostringstream msg;
                    msg << "Removing octagons in surface " << s
                        << " for " << name
                        << " breaks the number of boundary curves.";
                    CPPUNIT_FAIL(msg.str());
                }

                const Triangulation<3>& retri = noOct.triangulation();
                if (std::addressof(retri) != std::addressof(tri)) {
                    if (tri.isClosed() != retri.isClosed()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " yields a triangulation that breaks "
                            "closedness.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (tri.isOrientable() != retri.isOrientable()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " yields a triangulation that breaks "
                            "orientability.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (tri.countComponents() !=
                            retri.countComponents()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " yields a triangulation that breaks "
                            "the number of components.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (tri.countBoundaryComponents() !=
                            retri.countBoundaryComponents()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " yields a triangulation that breaks "
                            "the number of boundary components.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (tri.homology() != retri.homology()) {
                        std::ostringstream msg;
                        msg << "Removing octagons in surface " << s
                            << " for " << name
                            << " yields a triangulation that breaks "
                            "homology.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        static void testRemoveOcts(const Triangulation<3>& tri,
                const char* name) {
            testRemoveOctsInCoords(NS_AN_STANDARD, tri, name);
            testRemoveOctsInCoords(NS_AN_QUAD_OCT, tri, name);
        }

        void removeOctsConstructed() {
            testRemoveOcts(oneTet, "Lone tetrahedron");
            testRemoveOcts(figure8, "Figure eight");
            testRemoveOcts(gieseking, "Gieseking");
            testRemoveOcts(S3, "S3");
            testRemoveOcts(loopC2, "C(2)");
            testRemoveOcts(loopCtw3, "C~(3)");
            testRemoveOcts(largeS3, "Large S3");
            testRemoveOcts(largeRP3, "Large RP3");
            testRemoveOcts(twistedKxI, "Twisted KxI");
            testRemoveOcts(norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void removeOctsCensus() {
            runCensusAllClosed(&testRemoveOcts, true);
            runCensusAllBounded(&testRemoveOcts, true);
            runCensusAllIdeal(&testRemoveOcts, true);
        }

        void testCopyMove(const Triangulation<3>& tri, const char* name) {
            // The main point of this test is to ensure that the move
            // operations are *actually* move operations and not copies.
            //
            // We assume here that std::vector's move operations
            // preserve the addresses of the underlying objects.
            // I don't think this is required by the standard, but I'm
            // also not aware of any implementation that doesn't do this,
            // and I can't think of a better (and still non-intrusive)
            // way to ensure that the move was a "real" move.

            const NormalSurfaces a(tri, NS_STANDARD);
            if (a.size() == 0) {
                std::ostringstream msg;
                msg << name << ": copy/move test requires a non-empty list.";
                CPPUNIT_FAIL(msg.str());
            }

            NormalSurfaces a1(a);

            if (a1.detail() != a.detail()) {
                std::ostringstream msg;
                msg << name << ": copy constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }
            if (a.size() == 0) {
                std::ostringstream msg;
                msg << name << ": copy constructed empties the original.";
                CPPUNIT_FAIL(msg.str());
            }

            NormalSurfaces a2(std::move(a1));

            if (a2.detail() != a.detail()) {
                std::ostringstream msg;
                msg << name << ": move constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }
            // The std::vector move constructor promises to empty the source.
            if (a1.size() != 0) {
                std::ostringstream msg;
                msg << name << ": move constructed does not "
                    "empty the original.";
                CPPUNIT_FAIL(msg.str());
            }

            NormalSurfaces a3(Example<3>::s2xs1(), NS_STANDARD);
            a3 = a;

            if (a3.detail() != a.detail()) {
                std::ostringstream msg;
                msg << name << ": copy assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }
            if (a.size() == 0) {
                std::ostringstream msg;
                msg << name << ": copy assigned empties the original.";
                CPPUNIT_FAIL(msg.str());
            }

            NormalSurfaces a4(Example<3>::s2xs1(), NS_STANDARD);
            a4 = std::move(a3);

            if (a4.detail() != a.detail()) {
                std::ostringstream msg;
                msg << name << ": move assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }
            /*
            // I'm not sure how to verify that the source was moved from
            // and not copied, in a way that does not make assumptions
            // about the standard library implementation...
            if (a3.size() != 0) {
                std::ostringstream msg;
                msg << name << ": move assigned does not "
                    "empty the original.";
                CPPUNIT_FAIL(msg.str());
            }
            */
        }

        void copyMove() {
            testCopyMove(Example<3>::poincare(), "Poincare homology sphere");
            testCopyMove(Example<3>::weeks(), "Weeks manifold");
            testCopyMove(Example<3>::whiteheadLink(), "Whitehead");
        }
};

void addNormalSurfaces(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NormalSurfacesTest::suite());
}

