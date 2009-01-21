
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include "census/ncensus.h"
#include "packet/ncontainer.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/surfaces/testsurfaces.h"

using regina::NBoolSet;
using regina::NCensus;
using regina::NContainer;
using regina::NExampleTriangulation;
using regina::NNormalSurface;
using regina::NNormalSurfaceList;
using regina::NNormalSurfaceVector;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTriangulation;

class NNormalSurfaceListTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NNormalSurfaceListTest);

    CPPUNIT_TEST(standardEmpty);
    CPPUNIT_TEST(standardOneTet);
    CPPUNIT_TEST(standardGieseking);
    CPPUNIT_TEST(standardFigure8);
    CPPUNIT_TEST(standardS3);
    CPPUNIT_TEST(standardLoopC2);
    CPPUNIT_TEST(standardLoopCtw3);
    CPPUNIT_TEST(standardTwistedKxI);
    CPPUNIT_TEST(standardNorSFS);
    CPPUNIT_TEST(quadEmpty);
    CPPUNIT_TEST(quadOneTet);
    CPPUNIT_TEST(quadGieseking);
    CPPUNIT_TEST(quadFigure8);
    CPPUNIT_TEST(quadS3);
    CPPUNIT_TEST(quadLoopC2);
    CPPUNIT_TEST(quadLoopCtw3);
    CPPUNIT_TEST(quadTwistedKxI);
    CPPUNIT_TEST(quadNorSFS);
    CPPUNIT_TEST(almostNormalEmpty);
    CPPUNIT_TEST(almostNormalOneTet);
    CPPUNIT_TEST(almostNormalGieseking);
    CPPUNIT_TEST(almostNormalFigure8);
    CPPUNIT_TEST(almostNormalS3);
    CPPUNIT_TEST(almostNormalLoopC2);
    CPPUNIT_TEST(almostNormalLoopCtw3);
    CPPUNIT_TEST(almostNormalTwistedKxI);
    CPPUNIT_TEST(largeDimensionsStandard);
    CPPUNIT_TEST(largeDimensionsQuad);
    CPPUNIT_TEST(largeDimensionsAlmostNormal);
    CPPUNIT_TEST(standardQuadConversionsConstructed);
    CPPUNIT_TEST(standardQuadConversionsCensus);
    CPPUNIT_TEST(standardANQuadOctConversionsConstructed);
    CPPUNIT_TEST(standardANQuadOctConversionsCensus);

    CPPUNIT_TEST_SUITE_END();

    private:
        NTriangulation empty;
            /**< An empty triangulation. */
        NTriangulation oneTet;
            /**< A one-tetrahedron ball. */
        NTriangulation figure8;
            /**< The figure eight knot complement. */
        NTriangulation gieseking;
            /**< The Gieseking manifold. */
        NTriangulation S3;
            /**< A one-tetrahedron two-vertex 3-sphere. */
        NTriangulation loopC2;
            /**< An untwisted layered loop of length 2. */
        NTriangulation loopCtw3;
            /**< A twisted layered loop of length 3. */
        NTriangulation twistedKxI;
            /**< A 3-tetrahedron non-orientable twisted I-bundle over the
                 Klein bottle. */
        NTriangulation norSFS;
            /**< A 9-tetrahedron triangulation of the space
                 SFS [RP2: (2,1) (2,1) (2,1)].  Specifically, this is
                 triangulation #5 of this space from the non-orientable
                 census as it was shipped with Regina 4.5. */


    public:
        void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            delete source;
        }

        void setUp() {
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;

            // The one-tetrahedron ball has no face identifications at all.
            oneTet.addTetrahedron(new NTetrahedron());

            // Use pre-coded triangulations where we can.
            copyAndDelete(figure8,
                NExampleTriangulation::figureEightKnotComplement());
            copyAndDelete(gieseking, NExampleTriangulation::gieseking());

            // Layered loops can be constructed automatically.
            S3.insertLayeredLoop(1, false);
            loopC2.insertLayeredLoop(2, false);
            loopCtw3.insertLayeredLoop(3, true);

            // A 3-tetrahedron non-orientable twisted I-bundle over the
            // Klein bottle is described in Chapter 3 of Benjamin
            // Burton's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            t = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 1, 2, 3));
            r->joinTo(1, t, NPerm(2, 1, 0, 3));
            r->joinTo(2, t, NPerm(1, 3, 2, 0));
            s->joinTo(1, t, NPerm(0, 3, 2, 1));
            s->joinTo(2, t, NPerm(3, 1, 0, 2));
            twistedKxI.addTetrahedron(r);
            twistedKxI.addTetrahedron(s);
            twistedKxI.addTetrahedron(t);

            // Build the 9-tetrahedron SFS from its dehydration string;
            // obscure but painless at least.
            norSFS.insertRehydration("jnnafaabcfighhihimgbpqpepbr");
        }

        void tearDown() {
        }

        void testSize(NNormalSurfaceList* list, const char* triName,
                const char* listType, unsigned long expectedSize) {
            std::ostringstream msg;
            msg << "Number of " << listType << " for " << triName
                << " should be " << expectedSize << ", not "
                << list->getNumberOfSurfaces() << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(),
                list->getNumberOfSurfaces() == expectedSize);
        }

        void testSurface(const NNormalSurface* surface, const char* triName,
                const char* surfaceName, int euler, bool connected,
                bool orient, bool twoSided, bool compact, bool realBdry,
                bool vertexLink, unsigned edgeLink,
                unsigned long central, bool splitting) {
            // Begin with the compactness test so we know which other
            // tests may be performed.
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should be "
                    << (compact ? "compact." : "non-compact.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isCompact() == compact);
            }
            if (compact) {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have Euler char. " << euler << ", not "
                    << surface->getEulerCharacteristic() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->getEulerCharacteristic() == euler);
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
                    surface->isConnected() == connected);
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
                    surface->isOrientable() == orient);
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
                    surface->isTwoSided() == twoSided);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should have "
                    << (realBdry ? "real boundary." : "no real boundary.");

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->hasRealBoundary() == realBdry);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! vertexLink)
                    msg << "not ";
                msg << "be vertex linking.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isVertexLinking() == vertexLink);
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

                std::pair<const regina::NEdge*, const regina::NEdge*> links
                    = surface->isThinEdgeLink();
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
                    surface->isCentral() == central);
            }
            {
                std::ostringstream msg;
                msg << "Surface [" << surfaceName << "] for " << triName
                    << " should ";
                if (! splitting)
                    msg << "not ";
                msg << "be a splitting surface.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    surface->isSplitting() == splitting);
            }
        }

        void countCompactSurfaces(const NNormalSurfaceList* list,
                const char* triName, const char* surfaceType,
                unsigned long expectedCount, int euler,
                bool connected, bool orient, bool twoSided,
                bool realBdry, bool vertexLink, unsigned edgeLink,
                unsigned long central, bool splitting) {
            unsigned long tot = 0;
            unsigned long size = list->getNumberOfSurfaces();

            const NNormalSurface* s;
            for (unsigned long i = 0; i < size; i++) {
                s = list->getSurface(i);

                if (s->getEulerCharacteristic() == euler &&
                        s->isConnected() == connected &&
                        s->isOrientable() == orient &&
                        s->isTwoSided() == twoSided &&
                        s->hasRealBoundary() == realBdry &&
                        s->isVertexLinking() == vertexLink &&
                        s->isCentral() == central &&
                        s->isSplitting() == splitting) {
                    std::pair<const regina::NEdge*, const regina::NEdge*> links
                        = s->isThinEdgeLink();
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
                    << " should be " << expectedCount << ", not "
                    << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        static bool lexLess(const NNormalSurfaceVector* a,
                const NNormalSurfaceVector* b) {
            for (unsigned i = 0; i < a->size(); ++i) {
                if ((*a)[i] < (*b)[i])
                    return true;
                if ((*a)[i] > (*b)[i])
                    return false;
            }
            return false;
        }

        static bool identical(const NNormalSurfaceList* lhs,
                const NNormalSurfaceList* rhs) {
            if (lhs->getNumberOfSurfaces() != rhs->getNumberOfSurfaces())
                return false;

            unsigned long n = lhs->getNumberOfSurfaces();
            if (n == 0)
                return true;

            typedef const NNormalSurfaceVector* VecPtr;
            VecPtr* lhsRaw = new VecPtr[n];
            VecPtr* rhsRaw = new VecPtr[n];

            unsigned long i;
            for (i = 0; i < n; ++i) {
                lhsRaw[i] = lhs->getSurface(i)->rawVector();
                rhsRaw[i] = rhs->getSurface(i)->rawVector();
            }

            std::sort(lhsRaw, lhsRaw + n, lexLess);
            std::sort(rhsRaw, rhsRaw + n, lexLess);

            bool ok = true;
            for (i = 0; i < n; ++i)
                if (! (*(lhsRaw[i]) == *(rhsRaw[i]))) {
                    ok = false;
                    break;
                }

            delete[] lhsRaw;
            delete[] rhsRaw;
            return ok;
        }

        static void verifyConversions(NTriangulation* tri,
                const char* triName = 0) {
            const char* useName = (triName ? triName :
                tri->getPacketLabel().c_str());

            if (tri->isIdeal() || ! tri->isValid()) {
                std::ostringstream msg;
                msg << "Cannot verify normal conversion routines for "
                    << useName << ", which is either ideal or invalid.";
                CPPUNIT_FAIL(msg.str());
            }

            NNormalSurfaceList* stdDirect = NNormalSurfaceList::
                enumerateStandardDirect(tri);
            NNormalSurfaceList* quadDirect = NNormalSurfaceList::enumerate(
                tri, NNormalSurfaceList::QUAD);

            NNormalSurfaceList* stdConv = quadDirect->quadToStandard();
            NNormalSurfaceList* quadConv = stdDirect->standardToQuad();

            // Compare the surfaces in each list coordinate by coordinate.
            if (! identical(stdDirect, stdConv)) {
                /**
                 * If something goes wrong, uncomment this block for a
                 * detailed dump of the triangulation and the two lists
                 * of surfaces.
                 *
                std::cerr << std::endl;
                std::cerr << stdDirect->toStringLong();
                std::cerr << std::endl;
                std::cerr << stdConv->toStringLong();
                std::cerr << std::endl;
                std::cerr << tri->toStringLong();
                 */

                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard coordinates for "
                        << useName << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(quadDirect, quadConv)) {
                /**
                 * If something goes wrong, uncomment this block for a
                 * detailed dump of the triangulation and the two lists
                 * of surfaces.
                 *
                std::cerr << std::endl;
                std::cerr << quadDirect->toStringLong();
                std::cerr << std::endl;
                std::cerr << quadConv->toStringLong();
                std::cerr << std::endl;
                std::cerr << tri->toStringLong();
                 */

                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in quadrilateral coordinates for "
                        << useName << '.';
                CPPUNIT_FAIL(msg.str());
            }

            delete stdDirect;
            delete quadDirect;
            delete stdConv;
            delete quadConv;
        }

        static void verifyConversionsAN(NTriangulation* tri,
                const char* triName = 0) {
            const char* useName = (triName ? triName :
                tri->getPacketLabel().c_str());

            if (tri->isIdeal() || ! tri->isValid()) {
                std::ostringstream msg;
                msg << "Cannot verify almost normal conversion routines for "
                    << useName << ", which is either ideal or invalid.";
                CPPUNIT_FAIL(msg.str());
            }

            NNormalSurfaceList* stdANDirect = NNormalSurfaceList::
                enumerateStandardANDirect(tri);
            NNormalSurfaceList* quadOctDirect = NNormalSurfaceList::enumerate(
                tri, NNormalSurfaceList::QUAD_OCT);

            NNormalSurfaceList* stdANConv =
                quadOctDirect->quadOctToStandardAN();
            NNormalSurfaceList* quadOctConv =
                stdANDirect->standardANToQuadOct();

            // Compare the surfaces in each list coordinate by coordinate.
            if (! identical(stdANDirect, stdANConv)) {
                /**
                 * If something goes wrong, uncomment this block for a
                 * detailed dump of the triangulation and the two lists
                 * of surfaces.
                 *
                std::cerr << std::endl;
                std::cerr << stdANDirect->toStringLong();
                std::cerr << std::endl;
                std::cerr << stdANConv->toStringLong();
                std::cerr << std::endl;
                std::cerr << tri->toStringLong();
                 */

                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in standard almost normal coordinates for "
                        << useName << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (! identical(quadOctDirect, quadOctConv)) {
                /**
                 * If something goes wrong, uncomment this block for a
                 * detailed dump of the triangulation and the two lists
                 * of surfaces.
                 *
                std::cerr << std::endl;
                std::cerr << quadOctDirect->toStringLong();
                std::cerr << std::endl;
                std::cerr << quadOctConv->toStringLong();
                std::cerr << std::endl;
                std::cerr << tri->toStringLong();
                 */

                std::ostringstream msg;
                msg << "Direct enumeration vs conversion gives different "
                    "surfaces in quadrilateral-octagon coordinates for "
                        << useName << '.';
                CPPUNIT_FAIL(msg.str());
            }

            delete stdANDirect;
            delete quadOctDirect;
            delete stdANConv;
            delete quadOctConv;
        }

        static bool verifyConversionsCensus(NTriangulation* tri,
                void* triName) {
            verifyConversions(tri, static_cast<const char*>(triName));
            return false;
        }

        static bool verifyConversionsANCensus(NTriangulation* tri,
                void* triName) {
            verifyConversionsAN(tri, static_cast<const char*>(triName));
            return false;
        }

        void standardEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::STANDARD);

            testSize(list, "the empty triangulation",
                "standard normal surfaces", 0);

            delete list;
        }

        void quadEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::QUAD);

            testSize(list, "the empty triangulation",
                "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalEmpty() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &empty, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the empty triangulation",
                "standard almost normal surfaces", 0);

            delete list;
        }

        void standardOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::STANDARD);

            testSize(list, "a single tetrahedron",
                "standard normal surfaces", 7);
            countCompactSurfaces(list, "a single tetrahedron",
                "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::QUAD);

            testSize(list, "a single tetrahedron",
                "quad normal surfaces", 3);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalOneTet() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &oneTet, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "a single tetrahedron",
                "standard almost normal surfaces", 10);
            countCompactSurfaces(list, "a single tetrahedron",
                "triangular discs", 4,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "quadrilateral discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list, "a single tetrahedron",
                "octagonal discs", 3,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::STANDARD);

            testSize(list, "the Gieseking manifold",
                "standard normal surfaces", 1);
            testSurface(list->getSurface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::QUAD);

            testSize(list, "the Gieseking manifold",
                "quad normal surfaces", 0);

            delete list;
        }

        void almostNormalGieseking() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &gieseking, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the Gieseking manifold",
                "standard almost normal surfaces", 1);
            testSurface(list->getSurface(0), "the Gieseking manifold",
                "vertex link",
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::STANDARD);

            testSize(list, "the figure eight knot complement",
                "standard normal surfaces", 1);
            testSurface(list->getSurface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::QUAD);

            testSize(list, "the figure eight knot complement",
                "quad normal surfaces", 4);
            for (unsigned long i = 0; i < list->getNumberOfSurfaces(); i++)
                testSurface(list->getSurface(i),
                    "the figure eight knot complement", "spun surface",
                    0 /* euler, N/A */, 0 /* connected, N/A */,
                    0 /* orient, N/A */, 0 /* two-sided, N/A */,
                    false /* compact */, false /* realBdry */,
                    false /* vertex link */, 0 /* edge link */,
                    0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalFigure8() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &figure8, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the figure eight knot complement",
                "standard almost normal surfaces", 1);
            testSurface(list->getSurface(0), "the figure eight knot complement",
                "vertex link",
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* compact */, false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::STANDARD);

            testSize(list, "the 3-sphere",
                "standard normal surfaces", 3);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void quadS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::QUAD);

            testSize(list, "the 3-sphere",
                "quad normal surfaces", 1);
            countCompactSurfaces(list, "the 3-sphere",
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalS3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &S3, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the 3-sphere",
                "standard almost normal surfaces", 4);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                1 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the 3-sphere",
                "standard almost normal central 2-spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                1 /* central */, false /* splitting */);

            delete list;
        }

        void standardLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::STANDARD);

            testSize(list, "the untwisted layered loop C(2)",
                "standard normal surfaces", 5);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::QUAD);

            testSize(list, "the untwisted layered loop C(2)",
                "quad normal surfaces", 3);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "quad normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "quad normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopC2() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopC2, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the untwisted layered loop C(2)",
                "standard almost normal surfaces", 5);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal vertex linking spheres", 2,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal double-edge linking tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 2 /* edge link */,
                2 /* central */, true /* splitting */);
            countCompactSurfaces(list, "the untwisted layered loop C(2)",
                "standard normal splitting projective planes", 2,
                1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                2 /* central */, true /* splitting */);

            delete list;
        }

        void standardLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::STANDARD);

            testSize(list, "the twisted layered loop C~(3)",
                "standard normal surfaces", 5);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void quadLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::QUAD);

            testSize(list, "the twisted layered loop C~(3)",
                "quad normal surfaces", 4);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "quad normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void almostNormalLoopCtw3() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loopCtw3, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "the twisted layered loop C~(3)",
                "standard almost normal surfaces", 5);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal edge linking tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, "the twisted layered loop C~(3)",
                "standard normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                3 /* central */, true /* splitting */);

            delete list;
        }

        void standardTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::STANDARD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal surfaces", 8);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void quadTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::QUAD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal surfaces", 6);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "quad normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void almostNormalTwistedKxI() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &twistedKxI, NNormalSurfaceList::AN_STANDARD);

            testSize(list, "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces", 13);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal vertex linking discs", 1,
                1 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal thin edge-linking annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal splitting punctured tori", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, true /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 1-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal central 2-sided Mobius bands", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                3 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided Mobius bands", 2,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard normal generic 1-sided annuli", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, non-orbl)", 2,
                -1 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-1, 1-sided, orbl)", 1,
                -1 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "a 3-tetrahedron non-orientable twisted KxI",
                "standard almost normal surfaces "
                    "(chi=-2, 1-sided, non-orbl)", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                true /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void standardNorSFS() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &norSFS, NNormalSurfaceList::STANDARD);

            testSize(list, "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal surfaces", 25);

            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal vertex linking spheres", 1,
                2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                true /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal two-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal one-sided genus two Klein bottles", 2,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "standard normal central one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);

            delete list;
        }

        void quadNorSFS() {
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &norSFS, NNormalSurfaceList::QUAD);

            testSize(list, "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal surfaces", 21);

            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal thin edge-linking Klein bottles", 6,
                0 /* euler */, true /* connected */,
                false /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal central(8) one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                8 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal central(6) one-sided Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                6 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal miscellaneous one-sided Klein bottles", 4,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal central(9) one-sided tori", 1,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                9 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal miscellaneous one-sided tori", 3,
                0 /* euler */, true /* connected */,
                true /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal two-sided genus two tori", 1,
                -2 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list,
                "SFS [RP2: (2,1) (2,1) (2,1)]",
                "quad normal one-sided genus two Klein bottles", 1,
                -2 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 0 /* edge link */,
                0 /* central */, false /* splitting */);

            delete list;
        }

        void testStandardLoopCtwGeneric(unsigned len) {
            NTriangulation loop;
            loop.insertLayeredLoop(len, true);
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loop, NNormalSurfaceList::STANDARD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";

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

            testSize(list, name.str().c_str(), "standard normal surfaces",
                curr);
        }

        void testQuadLoopCtwGeneric(unsigned len) {
            NTriangulation loop;
            loop.insertLayeredLoop(len, true);
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loop, NNormalSurfaceList::QUAD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";

            // It is easy to prove in general that C~(len) has precisely
            // (len + 1) vertex surfaces, as described by the following tests.
            testSize(list, name.str().c_str(), "quad normal surfaces", len + 1);
            countCompactSurfaces(list, name.str().c_str(),
                "quad normal edge linking tori", len,
                0 /* euler */, true /* connected */,
                true /* orient */, true /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                0 /* central */, false /* splitting */);
            countCompactSurfaces(list, name.str().c_str(),
                "quad normal splitting Klein bottles", 1,
                0 /* euler */, true /* connected */,
                false /* orient */, false /* two-sided */,
                false /* realBdry */,
                false /* vertex link */, 1 /* edge link */,
                len /* central */, true /* splitting */);

            delete list;
        }

        void testAlmostNormalLoopCtwGeneric(unsigned len) {
            NTriangulation loop;
            loop.insertLayeredLoop(len, true);
            NNormalSurfaceList* list = NNormalSurfaceList::enumerate(
                &loop, NNormalSurfaceList::AN_STANDARD);

            std::ostringstream name;
            name << "the twisted layered loop C~(" << len << ")";

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

            testSize(list, name.str().c_str(),
                "standard almost normal surfaces", curr);
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

        void standardQuadConversionsConstructed() {
            verifyConversions(&empty, "the empty triangulation");
            verifyConversions(&oneTet, "a single tetrahedron");
            verifyConversions(&S3, "the 3-sphere");
            verifyConversions(&loopC2, "the untwisted layered loop C(2)");
            verifyConversions(&loopCtw3, "the twisted layered loop C~(3)");
            verifyConversions(&twistedKxI,
                "a 3-tetrahedron non-orientable twisted KxI");
            verifyConversions(&norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void standardQuadConversionsCensus() {
            NContainer* parent = new NContainer();

            // Potentially minimal closed compact triangulations, 5 tetrahedra.
            NCensus::formCensus(parent, 5,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sFalse /* bounded */,
                -1,
                NCensus::PURGE_NON_MINIMAL_PRIME | NCensus::PURGE_P2_REDUCIBLE,
                &verifyConversionsCensus,
                const_cast<char*>(
                    "possibly-minimal closed compact census triangulation"));

            // All closed compact triangulations, 4 tetrahedra.
            NCensus::formCensus(parent, 4,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sFalse /* bounded */,
                -1, 0, &verifyConversionsCensus,
                const_cast<char*>("closed compact census triangulation"));

            // All bounded compact triangulations, 3 tetrahedra.
            NCensus::formCensus(parent, 3,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sTrue /* bounded */,
                -1, 0, &verifyConversionsCensus,
                const_cast<char*>("bounded compact census triangulation"));

            delete parent;
        }

        void standardANQuadOctConversionsConstructed() {
            verifyConversionsAN(&empty, "the empty triangulation");
            verifyConversionsAN(&oneTet, "a single tetrahedron");
            verifyConversionsAN(&S3, "the 3-sphere");
            verifyConversionsAN(&loopC2, "the untwisted layered loop C(2)");
            verifyConversionsAN(&loopCtw3, "the twisted layered loop C~(3)");
            verifyConversionsAN(&twistedKxI,
                "a 3-tetrahedron non-orientable twisted KxI");
            verifyConversionsAN(&norSFS, "SFS [RP2: (2,1) (2,1) (2,1)]");
        }

        void standardANQuadOctConversionsCensus() {
            NContainer* parent = new NContainer();

            // Potentially minimal closed compact triangulations, 5 tetrahedra.
            NCensus::formCensus(parent, 5,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sFalse /* bounded */,
                -1,
                NCensus::PURGE_NON_MINIMAL_PRIME | NCensus::PURGE_P2_REDUCIBLE,
                &verifyConversionsANCensus,
                const_cast<char*>(
                    "possibly-minimal closed compact census triangulation"));

            // All closed compact triangulations, 4 tetrahedra.
            NCensus::formCensus(parent, 4,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sFalse /* bounded */,
                -1, 0, &verifyConversionsANCensus,
                const_cast<char*>("closed compact census triangulation"));

            // All bounded compact triangulations, 3 tetrahedra.
            NCensus::formCensus(parent, 3,
                NBoolSet::sTrue /* finite */,
                NBoolSet::sBoth /* orientable */,
                NBoolSet::sTrue /* bounded */,
                -1, 0, &verifyConversionsANCensus,
                const_cast<char*>("bounded compact census triangulation"));

            delete parent;
        }
};

void addNNormalSurfaceList(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NNormalSurfaceListTest::suite());
}

