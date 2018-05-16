
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "census/census.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "packet/container.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

#include "testsuite/link/testlink.h"

using regina::Crossing;
using regina::ExampleLink;
using regina::Link;
using regina::Triangulation;
using regina::StrandRef;

// Isomorphism signatures for various knot/link complements that regina's
// simplification heuristics are found to reduce to in practice.
#define TREFOIL_SIGS { "cPcbbbadh", "cPcbbbadu", "dLQbcbcdlcj", "dLQbcbcdlcn" }
#define FIG8_SIGS { "cPcbbbiht" }

class LinkTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkTest);

    CPPUNIT_TEST(components);
    CPPUNIT_TEST(jones);
    CPPUNIT_TEST(homfly);
    CPPUNIT_TEST(complement);
    CPPUNIT_TEST(r1Count);
    CPPUNIT_TEST(r2Count);
    CPPUNIT_TEST(r3Count);
    CPPUNIT_TEST(r123Perform);
    CPPUNIT_TEST(resolve);
    CPPUNIT_TEST(knotSig);
    CPPUNIT_TEST(rewrite);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * The empty link:
         */
        Link *empty;

        /**
         * Knots:
         */
        Link *unknot0, *unknot1, *unknot3, *unknotGordian;
        Link *trefoilLeft, *trefoilRight, *figureEight;

        /**
         * Links:
         */
        Link *unlink2_0, *unlink3_0;
        Link *unlink2_r2, *unlink2_r1r1;
        Link *hopf, *whitehead, *borromean;
        Link *trefoil_unknot0, *trefoil_unknot1, *trefoil_unknot_overlap;
        Link *adams6_28; // Figure 6.28 from Adams

    public:
        void setUp() {
            empty = new Link();
            empty->setLabel("Empty link");

            unknot0 = new Link(1);
            unknot0->setLabel("Unknot (0 crossings)");

            unknot1 = Link::fromData({ 1 }, { 1, -1 });
            unknot1->setLabel("Unknot (1 crossing)");

            unknot3 = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
            unknot3->setLabel("Unknot (3 crossings)");

            unknotGordian = ExampleLink::gordian();

            trefoilLeft = ExampleLink::trefoilLeft();

            trefoilRight = ExampleLink::trefoilRight();

            figureEight = ExampleLink::figureEight();

            unlink2_0 = new Link(2);
            unlink2_0->setLabel("Unlink (2 components)");

            unlink3_0 = new Link(3);
            unlink3_0->setLabel("Unlink (3 components)");

            unlink2_r2 = Link::fromData({ 1, -1 }, { 1, 2 }, { -1, -2 });
            unlink2_r2->setLabel("Unlink (2 components via R2)");

            unlink2_r1r1 = Link::fromData({ -1, 1 }, { 1, -1 }, { -2, 2 });
            unlink2_r1r1->setLabel("Unlink (2 components via R1+R1)");

            hopf = ExampleLink::hopf();

            whitehead = ExampleLink::whitehead();

            borromean = ExampleLink::borromean();

            trefoil_unknot0 = Link::fromData({ 1, 1, 1 },
                { 1, -2, 3, -1, 2, -3 }, { 0 });
            trefoil_unknot0->setLabel("Trefoil U unknot (separate)");

            trefoil_unknot1 = Link::fromData(
                { +1, +1, -1, +1 }, { 1, -2, 4, -1, 2, -4 }, { -3, 3 });
            trefoil_unknot1->setLabel("Trefoil U unknot (separate + twist)");

            trefoil_unknot_overlap = Link::fromData(
                { -1, +1, +1, +1, +1 }, { 2, -3, -4, -1, 5, -2, 3, -5 },
                { 4, 1 });
            trefoil_unknot_overlap->setLabel("Trefoil U unknot (with R2)");

            adams6_28 = Link::fromData({ +1, +1, -1, -1, +1, +1 },
                { -2, 1, -5, 6 }, { 2, -3, 4, -6, 5, -4, 3, -1 });
            adams6_28->setLabel("Adams, Figure 6.28");
        }

        void tearDown() {
            delete empty;
            delete unknot0;
            delete unknot1;
            delete unknot3;
            delete unknotGordian;
            delete trefoilLeft;
            delete trefoilRight;
            delete figureEight;
            delete unlink2_0;
            delete unlink3_0;
            delete unlink2_r2;
            delete unlink2_r1r1;
            delete hopf;
            delete whitehead;
            delete borromean;
            delete trefoil_unknot0;
            delete trefoil_unknot1;
            delete trefoil_unknot_overlap;
            delete adams6_28;
        }

        void sanity(Link* l) {
            Crossing* c;
            for (size_t i = 0; i < l->size(); ++i) {
                c = l->crossing(i);

                StrandRef lower(c, 0);
                StrandRef upper(c, 1);
                if (lower.next().prev() != lower ||
                        upper.next().prev() != upper) {
                    std::ostringstream msg;
                    msg << l->label() << ": inconsistent next/prev links.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void testComponents(Link* l, size_t expected) {
            if (l->countComponents() != expected) {
                std::ostringstream msg;
                msg << l->label() << ": expected " << expected
                    << " components, found " << l->countComponents() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void components() {
            testComponents(empty, 0);
            testComponents(unknot0, 1);
            testComponents(unknot1, 1);
            testComponents(unknot3, 1);
            testComponents(unknotGordian, 1);
            testComponents(trefoilLeft, 1);
            testComponents(trefoilRight, 1);
            testComponents(figureEight, 1);
            testComponents(unlink2_0, 2);
            testComponents(unlink3_0, 3);
            testComponents(unlink2_r2, 2);
            testComponents(unlink2_r1r1, 2);
            testComponents(hopf, 2);
            testComponents(whitehead, 2);
            testComponents(borromean, 3);
            testComponents(trefoil_unknot0, 2);
            testComponents(trefoil_unknot1, 2);
            testComponents(trefoil_unknot_overlap, 2);
        }

        void testJones(Link* l, const char* expected) {
            std::ostringstream s;
            s << l->jones();

            if (s.str() != expected) {
                std::ostringstream msg;
                msg << l->label() << ": expected V(link) = " << expected
                    << ", found " << s.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void jones() {
            testJones(empty, "0");
            testJones(unknot0, "1");
            testJones(unknot1, "1");
            testJones(unknot3, "1");
            // The Gordian unknot is too large to compute V(link).
            testJones(trefoilLeft, "x^-2 + x^-6 - x^-8");
            testJones(trefoilRight, "-x^8 + x^6 + x^2");
            testJones(figureEight, "x^4 - x^2 + 1 - x^-2 + x^-4");
            testJones(unlink2_0, "-x - x^-1");
            testJones(unlink3_0, "x^2 + 2 + x^-2");
            testJones(unlink2_r2, "-x - x^-1");
            testJones(unlink2_r1r1, "-x - x^-1");
            testJones(hopf, "-x^5 - x");
            testJones(whitehead, "-x^3 + x - 2 x^-1 + x^-3 - 2 x^-5 + x^-7");
            testJones(borromean,
                "-x^6 + 3 x^4 - 2 x^2 + 4 - 2 x^-2 + 3 x^-4 - x^-6");
            testJones(trefoil_unknot0, "x^9 - x^5 - x^3 - x");
            testJones(trefoil_unknot1, "x^9 - x^5 - x^3 - x");
            testJones(trefoil_unknot_overlap, "x^9 - x^5 - x^3 - x");
        }

        void testHomflyAZ(Link* l, const char* expected) {
            std::ostringstream s;
            s << l->homflyAZ();

            if (s.str() != expected) {
                std::ostringstream msg;
                msg << l->label() << ": expected homflyAZ(link) = " << expected
                    << ", found " << s.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testHomflyLM(Link* l, const char* expected) {
            std::ostringstream s;
            s << l->homflyLM();

            if (s.str() != expected) {
                std::ostringstream msg;
                msg << l->label() << ": expected homflyLM(link) = " << expected
                    << ", found " << s.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void homfly() {
            testHomflyAZ(empty, "0");
            testHomflyLM(empty, "0");

            testHomflyAZ(unknot0, "1");
            testHomflyLM(unknot0, "1");
            testHomflyAZ(unknot1, "1");
            testHomflyLM(unknot1, "1");
            testHomflyAZ(unknot3, "1");
            testHomflyLM(unknot3, "1");
            // The Gordian unknot is surely too large for this.

            testHomflyLM(unlink2_0, "-x y^-1 - x^-1 y^-1");
            testHomflyLM(unlink3_0, "x^2 y^-2 + 2 y^-2 + x^-2 y^-2");
            testHomflyLM(unlink2_r2, "-x y^-1 - x^-1 y^-1");
            testHomflyLM(unlink2_r1r1, "-x y^-1 - x^-1 y^-1");

            testHomflyLM(trefoilLeft, "-x^4 + x^2 y^2 - 2 x^2");
            testHomflyLM(trefoilRight, "x^-2 y^2 - 2 x^-2 - x^-4");
            testHomflyAZ(trefoilRight, "x^-2 y^2 + 2 x^-2 - x^-4");

            testHomflyLM(figureEight, "-x^2 + y^2 - 1 - x^-2");
            testHomflyLM(hopf, "-x^-1 y + x^-1 y^-1 + x^-3 y^-1");

            testHomflyLM(trefoil_unknot0,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1");
            testHomflyLM(trefoil_unknot1,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1");
            testHomflyLM(trefoil_unknot_overlap,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1");

            // This is different from Adams' claim regarding the HOMFLY
            // polynomial of this link.  But... Adams does get his arithmetic
            // wrong elsewhere, and a calculation by hand using the Skein
            // relation agrees with the polynomial below.
            testHomflyLM(adams6_28,
                "x y - x^-1 y^3 + x^-1 y + 2 x^-3 y - x^-3 y^-1 - x^-5 y^-1");

            // TODO: whitehead, borromean

            // TODO: Verify that L # M means * for HOMFLY/Jones

            // TODO: Verify that HOMFLY gives Jones by:
            //   * l = it^-1, m = i(t^-1/2 - t^1/2)
            //   * a = t^-1, z = t^1/2 - t^-1/2
        }

        bool sigMatches(const std::string& sig,
                std::initializer_list<const char*> expected) {
            for (auto e : expected)
                if (sig == e)
                    return true;
            return false;
        }

        void testComplementBasic(Link* l) {
            Triangulation<3>* c = l->complement();

            if (c->countComponents() != 1) {
                std::ostringstream msg;
                msg << l->label() << " complement: expected 1 component, "
                    "found " << c->countComponents() << ".";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }

            size_t ideal = 0;
            for (auto v : c->vertices()) {
                regina::Vertex<3>::LinkType t = v->link();
                if (t == regina::Vertex<3>::TORUS)
                    ++ideal;
                else if (t != regina::Vertex<3>::SPHERE) {
                    std::ostringstream msg;
                    msg << l->label() << " complement: "
                        "contains a vertex with unexpected link type.";
                    delete c;
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (ideal != l->countComponents()) {
                std::ostringstream msg;
                msg << l->label() << " complement: expected "
                    << l->countComponents() << " ideal vertices, "
                    "found " << ideal << ".";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }

            delete c;
        }

        void testComplementS3(Link* l) {
            Triangulation<3>* c = l->complement();
            if (! c->isThreeSphere()) {
                std::ostringstream msg;
                msg << l->label() << " complement: not a 3-sphere "
                    "as expected.";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }
            delete c;
        }

        void testComplementUnknot(Link* l) {
            Triangulation<3>* c = l->complement();
            if (! c->isSolidTorus()) {
                std::ostringstream msg;
                msg << l->label() << " complement: not a solid torus "
                    "as expected.";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }
            delete c;
        }

        void testComplementSig(Link* l,
                std::initializer_list<const char*> expected) {
            Triangulation<3>* c = l->complement();
            std::string sig = c->isoSig();
            delete c;

            for (auto e : expected)
                if (sig == e)
                    return;

            std::ostringstream msg;
            msg << l->label() << " complement: isosig is " << sig << ", not ";
            auto e = expected.begin();
            msg << *e;
            for (++e; e != expected.end(); ++e)
                msg << " / " << *e;
            msg << " as expected.";
            CPPUNIT_FAIL(msg.str());
        }

        void testComplementCensus(Link* l, std::string prefix) {
            Triangulation<3>* c = l->complement();
            std::string sig = c->isoSig();
            delete c;

            regina::CensusHits* hits = regina::Census::lookup(sig);
            const regina::CensusHit* hit = hits->first();
            while (hit) {
                if (hit->name().substr(0, prefix.size()) == prefix) {
                    delete hits;
                    return;
                }
                hit = hit->next();
            }

            delete hits;

            std::ostringstream msg;
            msg << l->label() << " complement: isosig " << sig
                << " was not identified in the census as " << prefix
                << "... .";
            CPPUNIT_FAIL(msg.str());
        }

        void testComplementFree(Link* l, unsigned nGen) {
            Triangulation<3>* c = l->complement();
            const regina::GroupPresentation& fg = c->fundamentalGroup();
            if (fg.countGenerators() != nGen || fg.countRelations() != 0) {
                std::ostringstream msg;
                msg << l->label() << " complement: fundamental group is "
                    "not recognised as free on " << nGen
                    << " generators as expected.";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }
            delete c;
        }

        void testComplementFreeAbelian(Link* l, unsigned nGen) {
            std::ostringstream expected;
            expected << nGen << " Z";

            Triangulation<3>* c = l->complement();
            const regina::GroupPresentation& fg = c->fundamentalGroup();
            if (fg.recogniseGroup() != expected.str()) {
                std::ostringstream msg;
                msg << l->label() << " complement: fundamental group is "
                    "not recognised as free abelian on " << nGen
                    << " generators as expected.";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }
            delete c;
        }

        void testComplementTrefoilUnknot(Link* l) {
            Triangulation<3>* c = l->complement();

            // Find a separating sphere.
            regina::NormalSurfaces* vtx =
                regina::NormalSurfaces::enumerate(c, regina::NS_STANDARD);
            const regina::NormalSurface* s;
            for (size_t i = 0; i < vtx->size(); ++i) {
                s = vtx->surface(i);
                if (s->eulerChar() != 2)
                    continue;
                // s must be a 2-sphere.

                Triangulation<3>* cut = s->cutAlong();
                cut->finiteToIdeal(); // Fills the sphere boundaries with balls.
                if (cut->isConnected()) {
                    // Should never happen...
                    delete cut;
                    delete c;
                    std::ostringstream msg;
                    msg << l->label() << " complement contains a "
                        "non-separating 2-sphere.";
                    CPPUNIT_FAIL(msg.str());
                }

                regina::Container parent;
                cut->intelligentSimplify();
                cut->splitIntoComponents(&parent);

                Triangulation<3>* c1 = static_cast<Triangulation<3>*>(
                    parent.firstChild());
                Triangulation<3>* c2 = static_cast<Triangulation<3>*>(
                    parent.lastChild());

                if ((sigMatches(c1->isoSig(), TREFOIL_SIGS) &&
                         c2->isSolidTorus()) ||
                        (sigMatches(c2->isoSig(), TREFOIL_SIGS) &&
                         c1->isSolidTorus())) {
                    // We have the correct manifold!
                    delete cut;
                    delete c;
                    return;
                }

                // c1 and c2 will be deleted automatically as parent goes out
                // of scope.
                delete cut;
            }

            // Note: vtx will be deleted automatically with c.
            delete c;
            std::ostringstream msg;
            msg << l->label() << " complement could not be recognised "
                "as coming from (trefoil U unknot).";
            CPPUNIT_FAIL(msg.str());
        }

        void complement() {
            testComplementBasic(empty);
            testComplementBasic(unknot0);
            testComplementBasic(unknot1);
            testComplementBasic(unknot3);
            testComplementBasic(unknotGordian);
            testComplementBasic(trefoilLeft);
            testComplementBasic(trefoilRight);
            testComplementBasic(figureEight);
            testComplementBasic(unlink2_0);
            testComplementBasic(unlink3_0);
            testComplementBasic(unlink2_r2);
            testComplementBasic(unlink2_r1r1);
            testComplementBasic(hopf);
            testComplementBasic(whitehead);
            testComplementBasic(borromean);
            testComplementBasic(trefoil_unknot0);
            testComplementBasic(trefoil_unknot1);
            testComplementBasic(trefoil_unknot_overlap);

            testComplementS3(empty);
            testComplementUnknot(unknot0);
            testComplementUnknot(unknot1);
            testComplementUnknot(unknot3);
            // Too large! - testComplementUnknot(unknotGordian);

            // For some knots and links, it is reasonable to assume that
            // intelligentSimplify() will reach a minimal triangulation.
            //
            // In some cases there are too many minimal triangulations
            // to list here, and so we use a census lookup.
            // cPcbbbadh 
            testComplementSig(trefoilLeft, TREFOIL_SIGS);
            testComplementSig(trefoilRight, TREFOIL_SIGS);
            testComplementSig(figureEight, FIG8_SIGS);
            testComplementCensus(whitehead, "m129 :");
            testComplementCensus(borromean, "t12067 :");

            // For unlink complements, we can test that the fundamental
            // group is free.
            testComplementFree(unlink2_0, 2);
            testComplementFree(unlink3_0, 3);
            testComplementFree(unlink2_r2, 2);
            testComplementFree(unlink2_r1r1, 2);

            // For the Hopf link, we can test that the fundamental group
            // is free abelian.
            testComplementFreeAbelian(hopf, 2);

            // A specialised test for the complement of (trefoil U unknot).
            testComplementTrefoilUnknot(trefoil_unknot0);
            testComplementTrefoilUnknot(trefoil_unknot1);
            testComplementTrefoilUnknot(trefoil_unknot_overlap);
        }

        void verifyCountR1Up(Link* link, size_t expected) {
            size_t total = 0;

            int cr, strand, side, sign;
            for (side = 0; side < 2; ++side)
                for (sign = -1; sign <= 1; sign += 2) {
                    if (link->r1(StrandRef(), side, sign, 1, 0))
                        ++total;

                    for (cr = 0; cr < link->size(); ++cr)
                        for (strand = 0; strand < 2; ++strand)
                            if (link->r1(link->crossing(cr)->strand(strand),
                                    side, sign, 1, 0))
                                ++total;
                }

            if (total != expected) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R1 moves "
                    "up, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR1Down(Link* link, size_t expected) {
            size_t total = 0;

            if (link->r1(nullptr, 1, 0))
                ++total;

            int cr;
            for (cr = 0; cr < link->size(); ++cr)
                if (link->r1(link->crossing(cr), 1, 0))
                    ++total;

            if (total != expected) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R1 moves "
                    "down, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR2Up(Link* link, size_t expected) {
            size_t total = 0;

            int cr1, cr2, str1, str2, side1, side2;
            for (side1 = 0; side1 < 2; ++side1)
                for (side2 = 0; side2 < 2; ++side2) {
                    if (link->r2(StrandRef(), side1, StrandRef(), side2, 1, 0))
                        ++total;

                    for (cr1 = 0; cr1 < link->size(); ++cr1)
                        for (str1 = 0; str1 < 2; ++str1) {
                            if (link->r2(StrandRef(), side1,
                                    link->crossing(cr1)->strand(str1), side2,
                                    1, 0))
                                ++total;
                            if (link->r2(link->crossing(cr1)->strand(str1),
                                    side1, StrandRef(), side2,
                                    1, 0))
                                ++total;

                            for (cr2 = 0; cr2 < link->size(); ++cr2)
                                for (str2 = 0; str2 < 2; ++str2)
                                    if (link->r2(
                                            link->crossing(cr1)->strand(str1),
                                            side1,
                                            link->crossing(cr2)->strand(str2),
                                            side2, 1, 0))
                                        ++total;
                        }
                }

            if (total != expected) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R2 moves "
                    "up, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR2Down(Link* link, size_t expected) {
            verifyCountR2Down(link, expected, 2 * expected);
        }

        void verifyCountR2Down(Link* link, size_t expectedCr,
                size_t expectedStr) {
            // Most of the time, expectedStr == expectedCr * 2.
            //
            // However, this can differ in the case of an unknotted loop
            // placed on top of another strand - here there are 2 moves
            // by crossing, but only 3 moves by strand (all of which
            // produce identical results).

            int cr, strand;

            size_t total = 0;

            if (link->r2(nullptr, 1, 0))
                ++total;

            for (cr = 0; cr < link->size(); ++cr)
                if (link->r2(link->crossing(cr), 1, 0))
                    ++total;

            if (total != expectedCr) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R2 moves "
                    "down by crossing, not " << expectedCr << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            total = 0;

            if (link->r2(StrandRef(), 1, 0))
                ++total;

            for (cr = 0; cr < link->size(); ++cr)
                for (strand = 0; strand < 2; ++strand)
                    if (link->r2(link->crossing(cr)->strand(strand), 1, 0))
                        ++total;

            if (total != expectedStr) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R2 moves "
                    "down by strand, not " << expectedStr << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR3(Link* link, size_t expected) {
            int cr, strand, side;

            size_t total = 0;
            for (side = 0; side < 2; ++side) {
                if (link->r3(nullptr, side, 1, 0))
                    ++total;

                for (cr = 0; cr < link->size(); ++cr)
                    if (link->r3(link->crossing(cr), side, 1, 0))
                        ++total;
            }

            if (total != expected) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R3 moves "
                    "by crossing, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            total = 0;
            for (side = 0; side < 2; ++side) {
                if (link->r3(StrandRef(), side, 1, 0))
                    ++total;

                for (cr = 0; cr < link->size(); ++cr)
                    for (strand = 0; strand < 2; ++strand)
                        if (link->r3(link->crossing(cr)->strand(strand),
                                side, 1, 0))
                            ++total;
            }

            if (total != expected * 3) {
                std::ostringstream msg;
                msg << link->label() << " offers " << total << " R3 moves "
                    "by strand, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void r1Count() {
            verifyCountR1Up(empty, 0);
            verifyCountR1Down(empty, 0);

            verifyCountR1Up(unknot0, 4);
            verifyCountR1Down(unknot0, 0);

            verifyCountR1Up(unknot1, 8);
            verifyCountR1Down(unknot1, 1);

            verifyCountR1Up(unknot3, 24);
            verifyCountR1Down(unknot3, 0);

            verifyCountR1Up(trefoilLeft, 24);
            verifyCountR1Down(trefoilLeft, 0);

            verifyCountR1Up(trefoilRight, 24);
            verifyCountR1Down(trefoilRight, 0);

            verifyCountR1Up(figureEight, 32);
            verifyCountR1Down(figureEight, 0);

            verifyCountR1Up(unlink2_0, 4);
            verifyCountR1Down(unlink2_0, 0);

            verifyCountR1Up(unlink3_0, 4);
            verifyCountR1Down(unlink3_0, 0);

            verifyCountR1Up(unlink2_r2, 16);
            verifyCountR1Down(unlink2_r2, 0);

            verifyCountR1Up(unlink2_r1r1, 16);
            verifyCountR1Down(unlink2_r1r1, 2);

            verifyCountR1Up(hopf, 16);
            verifyCountR1Down(hopf, 0);

            verifyCountR1Up(whitehead, 40);
            verifyCountR1Down(whitehead, 0);

            verifyCountR1Up(borromean, 48);
            verifyCountR1Down(borromean, 0);

            verifyCountR1Up(trefoil_unknot0, 28);
            verifyCountR1Down(trefoil_unknot0, 0);

            verifyCountR1Up(trefoil_unknot1, 32);
            verifyCountR1Down(trefoil_unknot1, 1);

            verifyCountR1Up(trefoil_unknot_overlap, 40);
            verifyCountR1Down(trefoil_unknot_overlap, 0);

            verifyCountR1Up(adams6_28, 48);
            verifyCountR1Down(adams6_28, 0);
        }

        void r2Count() {
            verifyCountR2Up(empty, 0);
            verifyCountR2Down(empty, 0);

            verifyCountR2Up(unknot0, 0);
            verifyCountR2Down(unknot0, 0);

            verifyCountR2Up(unknot1, 2);
            verifyCountR2Down(unknot1, 0);

            verifyCountR2Up(unknot3, 18);
            verifyCountR2Down(unknot3, 2);

            verifyCountR2Up(trefoilLeft, 18);
            verifyCountR2Down(trefoilLeft, 0);

            verifyCountR2Up(trefoilRight, 18);
            verifyCountR2Down(trefoilRight, 0);

            verifyCountR2Up(figureEight, 28);
            verifyCountR2Down(figureEight, 0);

            verifyCountR2Up(unlink2_0, 4);
            verifyCountR2Down(unlink2_0, 0);

            verifyCountR2Up(unlink3_0, 4);
            verifyCountR2Down(unlink3_0, 0);

            verifyCountR2Up(unlink2_r2, 8);
            verifyCountR2Down(unlink2_r2, 2);

            verifyCountR2Up(unlink2_r1r1, 36);
            verifyCountR2Down(unlink2_r1r1, 0);

            verifyCountR2Up(hopf, 8);
            verifyCountR2Down(hopf, 0);

            verifyCountR2Up(whitehead, 40);
            verifyCountR2Down(whitehead, 0);

            verifyCountR2Up(borromean, 48);
            verifyCountR2Down(borromean, 0);

            verifyCountR2Up(trefoil_unknot0, 66);
            verifyCountR2Down(trefoil_unknot0, 0);

            verifyCountR2Up(trefoil_unknot1, 116);
            verifyCountR2Down(trefoil_unknot1, 0);

            verifyCountR2Up(trefoil_unknot_overlap, 46);
            verifyCountR2Down(trefoil_unknot_overlap, 2, 3);

            verifyCountR2Up(adams6_28, 54);
            verifyCountR2Down(adams6_28, 0);
        }

        void r3Count() {
            verifyCountR3(empty, 0);
            verifyCountR3(unknot0, 0);
            verifyCountR3(unknot1, 0);
            verifyCountR3(unknot3, 2);
            verifyCountR3(trefoilLeft, 0);
            verifyCountR3(trefoilRight, 0);
            verifyCountR3(figureEight, 0);
            verifyCountR3(unlink2_0, 0);
            verifyCountR3(unlink3_0, 0);
            verifyCountR3(unlink2_r2, 0);
            verifyCountR3(unlink2_r1r1, 0);
            verifyCountR3(hopf, 0);
            verifyCountR3(whitehead, 0);
            verifyCountR3(borromean, 0);
            verifyCountR3(trefoil_unknot0, 0);
            verifyCountR3(trefoil_unknot1, 0);
            verifyCountR3(trefoil_unknot_overlap, 0);
            verifyCountR3(adams6_28, 0);
        }

        void verifyR1Down(const Link* l, int crossing,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            if (! clone.r1(clone.crossing(crossing))) {
                std::ostringstream msg;
                msg << l->label() << ": R1(" << crossing << ") is "
                    "not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R1(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR1Up(const Link* l, int crossing, int strand,
                int side, int sign, const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            StrandRef s;
            if (crossing >= 0)
                s = clone.crossing(crossing)->strand(strand);

            if (! clone.r1(s, side, sign)) {
                std::ostringstream msg;
                msg << l->label() << ": R1(" << s << ", " << side << ", "
                    << sign << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R1(" << s << ", " << side << ", "
                    << sign << ") gives " << clone.brief() << ", not "
                    << briefResult << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Down(const Link* l, int crossing,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            if (! clone.r2(clone.crossing(crossing))) {
                std::ostringstream msg;
                msg << l->label() << ": R2(" << crossing << ") is "
                    "not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R2(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Down(const Link* l, int crossing, int strand,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            if (! clone.r2(clone.crossing(crossing)->strand(strand))) {
                std::ostringstream msg;
                msg << l->label() << ": R2("
                    << (strand ? '^' : '_') << crossing
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R2("
                    << (strand ? '^' : '_') << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Up(const Link* l, int upperCrossing, int upperStrand,
                int upperSide, int lowerCrossing, int lowerStrand,
                int lowerSide, const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            StrandRef upper, lower;
            if (upperCrossing >= 0)
                upper = clone.crossing(upperCrossing)->strand(upperStrand);
            if (lowerCrossing >= 0)
                lower = clone.crossing(lowerCrossing)->strand(lowerStrand);

            if (! clone.r2(upper, upperSide, lower, lowerSide)) {
                std::ostringstream msg;
                msg << l->label() << ": R2(" << upper << ", " << upperSide
                    << ", " << lower << ", " << lowerSide
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R2(" << upper << ", " << upperSide
                    << ", " << lower << ", " << lowerSide << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR3(const Link* l, int crossing, int side,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            if (! clone.r3(clone.crossing(crossing), side)) {
                std::ostringstream msg;
                msg << l->label() << ": R3(" << crossing << ", "
                    << side << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R3(" << crossing << ", "
                    << side << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR3(const Link* l, int crossing, int strand, int side,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            if (! clone.r3(clone.crossing(crossing)->strand(strand), side)) {
                std::ostringstream msg;
                msg << l->label() << ": R3("
                    << (strand ? '^' : '_') << crossing << ", " << side
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": R3(" << (strand ? '^' : '_')
                    << crossing << ", " << side << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void r123Perform() {
            Link* l;

            l = Link::fromData({ -1 }, { 1, -1 });
            l->setLabel("One twist");
            verifyR1Down(l, 0, "( )");
            delete l;

            l = Link::fromData({ 1, -1 }, { -1, 1, 2, -2 });
            l->setLabel("Two twists (a)");
            verifyR1Down(l, 0, "- ( ^0 _0 )");
            verifyR2Down(l, 0, "( )");
            verifyR2Down(l, 0, 1, "( )");
            verifyR2Down(l, 1, 0, "( )");
            delete l;

            l = Link::fromData({ 1, -1 }, { 1, 2, -2, -1 });
            l->setLabel("Two twists (b)");
            verifyR1Down(l, 0, "- ( ^0 _0 )");
            verifyR2Down(l, 0, "( )");
            verifyR2Down(l, 0, 1, "( )");
            verifyR2Down(l, 1, 0, "( )");
            delete l;

            l = Link::fromData({ 1, -1 }, { 2, -2, -1, 1 });
            l->setLabel("Two twists (c)");
            verifyR1Down(l, 0, "- ( ^0 _0 )");
            verifyR2Down(l, 0, "( )");
            verifyR2Down(l, 0, 1, "( )");
            verifyR2Down(l, 1, 0, "( )");
            delete l;

            l = Link::fromData({ 1, -1 }, { -2, -1, 1, 2 });
            l->setLabel("Two twists (d)");
            verifyR1Down(l, 0, "- ( _0 ^0 )");
            verifyR2Down(l, 0, "( )");
            verifyR2Down(l, 0, 1, "( )");
            verifyR2Down(l, 1, 0, "( )");
            delete l;

            l = Link::fromData({ 1, -1 }, { 1, 2 }, { -2, -1 });
            l->setLabel("Overlapping loops");
            verifyR2Down(l, 0, "( ) ( )");
            verifyR2Down(l, 1, "( ) ( )");
            verifyR2Down(l, 0, 0, "( ) ( )");
            verifyR2Down(l, 0, 1, "( ) ( )");
            verifyR2Down(l, 1, 0, "( ) ( )");
            verifyR2Down(l, 1, 1, "( ) ( )");
            delete l;

            l = Link::fromData({ -1, 1, -1 }, { -1, 1, 3, 2 }, { -2, -3 });
            l->setLabel("Loop overlapping twist (a)");
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )");
            verifyR3(l, 0, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
            verifyR3(l, 0, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
            verifyR3(l, 1, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
            verifyR3(l, 1, 0, 1, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
            delete l;

            l = Link::fromData({ -1, 1, -1 }, { 3, 2, -1, 1 }, { -2, -3 });
            l->setLabel("Loop overlapping twist (b)");
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )");
            verifyR3(l, 0, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
            verifyR3(l, 0, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
            verifyR3(l, 1, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
            verifyR3(l, 1, 0, 1, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
            delete l;

            l = Link::fromData({ -1, 1, -1 }, { 2, -1, 1, 3 }, { -2, -3 });
            l->setLabel("Loop overlapping twist (c)");
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )");
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )");
            verifyR3(l, 0, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
            verifyR3(l, 0, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
            verifyR3(l, 1, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
            verifyR3(l, 1, 0, 1, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
            delete l;

            l = Link::fromData({ 1, -1, 1 }, { 1, -1, -3, -2 }, { 2, 3 });
            l->setLabel("Loop overlapping twist (d)");
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )");
            verifyR3(l, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
            verifyR3(l, 0, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
            verifyR3(l, 1, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
            verifyR3(l, 1, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
            delete l;

            l = Link::fromData({ 1, -1, 1 }, { -3, -2, 1, -1 }, { 2, 3 });
            l->setLabel("Loop overlapping twist (e)");
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )");
            verifyR3(l, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
            verifyR3(l, 0, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
            verifyR3(l, 1, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
            verifyR3(l, 1, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
            delete l;

            l = Link::fromData({ 1, -1, 1 }, { -2, 1, -1, -3 }, { 2, 3 });
            l->setLabel("Loop overlapping twist (f)");
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )");
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )");
            verifyR3(l, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
            verifyR3(l, 0, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
            verifyR3(l, 1, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
            verifyR3(l, 1, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
            delete l;

            l = Link::fromData({ 1, -1, 1, -1, 1, -1 }, { 5, 4 },
                { 6, -6, -5, -3, 1, -1, -2, 2, 3, -4 });
            l->setLabel("Three triangles (a)");
            verifyR3(l, 4, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
            verifyR3(l, 4, 1, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
            verifyR3(l, 5, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
            verifyR3(l, 3, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
            verifyR3(l, 4, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
            verifyR3(l, 4, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
            verifyR3(l, 4, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
            verifyR3(l, 2, 1, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
            verifyR3(l, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
            verifyR3(l, 1, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
            verifyR3(l, 2, 0, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
            verifyR3(l, 0, 0, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
            delete l;

            l = Link::fromData({ -1, +1, -1, +1, -1, +1 }, { -5, -4 },
                { -6, 6, 5, 3, -1, 1, 2, -2, -3, 4 });
            l->setLabel("Three triangles (b)");
            verifyR3(l, 5, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
            verifyR3(l, 4, 0, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
            verifyR3(l, 5, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
            verifyR3(l, 3, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
            verifyR3(l, 4, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
            verifyR3(l, 4, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
            verifyR3(l, 4, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
            verifyR3(l, 2, 0, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
            verifyR3(l, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
            verifyR3(l, 1, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
            verifyR3(l, 2, 1, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
            verifyR3(l, 0, 1, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
            delete l;

            l = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
            l->setLabel("Bad trefoil (a)");
            verifyR2Down(l, 2, "+ ( _0 ^0 )");
            verifyR2Down(l, 2, 1, "+ ( _0 ^0 )");
            verifyR2Down(l, 2, 0, "+ ( _0 ^0 )");
            verifyR3(l, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 1, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 2, 0, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 0, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
            delete l;

            l = Link::fromData({ 1, 1, -1 }, { -3, -1, 2, 3, 1, -2 });
            l->setLabel("Bad trefoil (b)");
            verifyR2Down(l, 2, "+ ( ^0 _0 )");
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 )");
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 )");
            verifyR3(l, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 1, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 2, 0, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 0, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
            delete l;

            l = Link::fromData({ -1, -1, 1 }, { 1, -2, -3, -1, 2, 3 });
            l->setLabel("Bad trefoil (c)");
            verifyR2Down(l, 2, "- ( _0 ^0 )");
            verifyR2Down(l, 2, 1, "- ( _0 ^0 )");
            verifyR2Down(l, 2, 0, "- ( _0 ^0 )");
            verifyR3(l, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 1, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 2, 0, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
            verifyR3(l, 0, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
            delete l;

            l = Link::fromData({ -1, -1, 1 }, { -3, -1, 2, 3, 1, -2 });
            l->setLabel("Bad trefoil (d)");
            verifyR2Down(l, 2, "- ( ^0 _0 )");
            verifyR2Down(l, 2, 1, "- ( ^0 _0 )");
            verifyR2Down(l, 2, 0, "- ( ^0 _0 )");
            verifyR3(l, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 1, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 2, 0, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
            verifyR3(l, 0, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
            delete l;

            l = Link::fromData({ -1, 1, 1, -1}, { 1, 2, -4, -3, -2, -1, 3, 4 });
            l->setLabel("Bad figure eight (a)");
            verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )");
            verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )");
            verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )");
            verifyR2Down(l, 2, "-+ ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 2, 1, "-+ ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 3, 0, "-+ ( ^0 ^1 _1 _0 )");
            verifyR3(l, 0, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
            verifyR3(l, 0, 1, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
            verifyR3(l, 0, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
            verifyR3(l, 2, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
            delete l;

            l = Link::fromData({ -1, 1, 1, -1}, { 2, -4, -3, -2, -1, 3, 4, 1 });
            l->setLabel("Bad figure eight (b)");
            verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )");
            verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )");
            verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )");
            delete l;

            l = Link::fromData({ -1, 1, 1, -1}, { -2, -1, 3, 4, 1, 2, -4, -3 });
            l->setLabel("Bad figure eight (c)");
            verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )");
            delete l;

            l = Link::fromData({ -1, 1, 1, -1}, { -1, 3, 4, 1, 2, -4, -3, -2 });
            l->setLabel("Bad figure eight (d)");
            verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )");
            verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )");
            delete l;

            l = Link::fromData({ 1, -1, -1, 1}, { -1, -2, 4, 3, 2, 1, -3, -4 });
            l->setLabel("Bad figure eight (e)");
            verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )");
            verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )");
            verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )");
            verifyR2Down(l, 3, "+- ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 2, 0, "+- ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 3, 1, "+- ( _0 _1 ^1 ^0 )");
            verifyR3(l, 2, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
            verifyR3(l, 0, 0, 1, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
            verifyR3(l, 0, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
            verifyR3(l, 2, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
            delete l;

            l = Link::fromData({ 1, -1, -1, 1}, { -2, 4, 3, 2, 1, -3, -4, -1 });
            l->setLabel("Bad figure eight (f)");
            verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )");
            verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )");
            verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )");
            delete l;

            l = Link::fromData({ 1, -1, -1, 1}, { 2, 1, -3, -4, -1, -2, 4, 3 });
            l->setLabel("Bad figure eight (g)");
            verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )");
            delete l;

            l = Link::fromData({ 1, -1, -1, 1}, { 1, -3, -4, -1, -2, 4, 3, 2 });
            l->setLabel("Bad figure eight (h)");
            verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )");
            verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )");
            delete l;

            l = Link::fromData({ -1, 1, -1, -1, -1 },
                { -2, -3, 4, -5, 1, 2, 3, -4, 5, -1 });
            l->setLabel("Excessive trefoil (a)");
            verifyR2Down(l, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
            verifyR2Down(l, 0, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
            verifyR2Down(l, 0, 1, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
            verifyR2Down(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
            verifyR2Down(l, 1, 0, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
            verifyR2Down(l, 1, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
            delete l;

            l = Link::fromData({ 1, -1, 1, 1, 1 },
                { 2, 3, -4, 5, -1, -2, -3, 4, -5, 1 });
            l->setLabel("Excessive trefoil (b)");
            verifyR2Down(l, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
            verifyR2Down(l, 0, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
            verifyR2Down(l, 0, 1, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
            verifyR2Down(l, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
            verifyR2Down(l, 1, 0, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
            verifyR2Down(l, 1, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
            delete l;

            l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
                { 1, -1, -2, 7, 6, 5, 4, 3, -5, -6, -7, 8, -8, 2 });
            l->setLabel("Dangling twists (a)");
            verifyR1Down(l, 0,
                "-+--+-- ( _1 _2 ) ( _0 ^5 ^4 ^3 ^2 ^1 _3 _4 _5 ^6 _6 ^0 )");
            verifyR1Down(l, 7,
                "+-+--+- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^4 ^3 ^2 _4 _5 _6 ^1 )");
            verifyR2Down(l, 1,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
            verifyR2Down(l, 1, 1,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
            verifyR2Down(l, 0, 0,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
            verifyR2Down(l, 6,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 6, 1,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 5, 0,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 5,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 5, 1,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 4, 0,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 3,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 3, 1,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
            verifyR2Down(l, 2, 0,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
            verifyR3(l, 4, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
            verifyR3(l, 4, 1, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
            verifyR3(l, 2, 0, 1,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
            verifyR3(l, 2, 1, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
            delete l;

            l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
                { 6, 5, 4, 3, -5, -6, -7, 8, -8, 2, 1, -1, -2, 7 });
            l->setLabel("Dangling twists (b)");
            verifyR2Down(l, 6,
                "+-+--- ( _2 _3 ) ( ^4 ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 )");
            verifyR2Down(l, 5,
                "+-+--- ( _2 _3 ) ( ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 ^4 )");
            delete l;

            l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
                { -1, 1, 2, -7, -6, -5, -4, -3, 5, 6, 7, -8, 8, -2 });
            l->setLabel("Dangling twists (c)");
            verifyR1Down(l, 0,
                "+-++-++ ( ^1 ^2 ) ( ^0 _5 _4 _3 _2 _1 ^3 ^4 ^5 _6 ^6 _0 )");
            verifyR1Down(l, 7,
                "-+-++-+ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _4 _3 _2 ^4 ^5 ^6 _1 )");
            verifyR2Down(l, 0,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
            verifyR2Down(l, 0, 1,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
            verifyR2Down(l, 1, 0,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
            verifyR2Down(l, 5,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 5, 1,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 6, 0,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 4,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 4, 1,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 5, 0,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 2,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 2, 1,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
            verifyR2Down(l, 3, 0,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
            verifyR3(l, 2, 1,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
            verifyR3(l, 2, 1, 1,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
            verifyR3(l, 2, 0, 0,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
            verifyR3(l, 4, 0, 0,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
            delete l;

            l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
                { -6, -5, -4, -3, 5, 6, 7, -8, 8, -2, -1, 1, 2, -7 });
            l->setLabel("Dangling twists (d)");
            verifyR2Down(l, 5,
                "-+-+++ ( ^2 ^3 ) ( _4 _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 )");
            verifyR2Down(l, 4,
                "-+-+++ ( ^2 ^3 ) ( _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 _4 )");
            delete l;

            l = Link::fromData({ 1, 1, -1, -1, -1 },
                { 0 }, { -1, 2, -4, 3, -2, 1, -3, 4, 5, -5 }, { 0 }, { 0 });
            l->setLabel("Figure eight with twist and three unknots");
            verifyR1Up(l, -1, 0, 0, -1,
                "++---- ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, -1, 0, 0, 1,
                "++---+ ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, -1, 0, 1, -1,
                "++---- ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, -1, 0, 1, 1,
                "++---+ ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, 4, 1, 0, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )");
            verifyR1Up(l, 4, 1, 0, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )");
            verifyR1Up(l, 4, 1, 1, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )");
            verifyR1Up(l, 4, 1, 1, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )");
            verifyR1Up(l, 1, 0, 0, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, 1, 0, 0, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, 1, 0, 1, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            verifyR1Up(l, 1, 0, 1, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
            // Note: for R2, the implementation always adds the two new
            // crossings in the order (+, -).
            verifyR2Up(l, -1, 0, 0, -1, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
            verifyR2Up(l, -1, 0, 0, -1, 0, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
            verifyR2Up(l, -1, 0, 1, -1, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
            verifyR2Up(l, -1, 0, 1, -1, 0, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
            verifyR2Up(l, -1, 0, 0, 4, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _6 _5 ) ( ) ( )");
            verifyR2Up(l, -1, 0, 1, 4, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _5 _6 ) ( ) ( )");
            verifyR2Up(l, 4, 0, 0, -1, 0, 0, "++---+- "
                "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^5 ^6 ) ( ) ( )");
            verifyR2Up(l, 4, 0, 0, -1, 0, 1, "++---+- "
                "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^6 ^5 ) ( ) ( )");
            verifyR2Up(l, -1, 0, 0, 4, 1, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ) ( ) ( )");
            verifyR2Up(l, -1, 0, 1, 4, 1, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");
            verifyR2Up(l, 4, 1, 1, -1, 0, 0, "++---+- "
                "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 ) ( ) ( )");
            verifyR2Up(l, 4, 1, 1, -1, 0, 1, "++---+- "
                "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
            verifyR2Up(l, 4, 0, 0, 3, 1, 0, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^5 ^6 ) ( ) ( )");
            verifyR2Up(l, 3, 1, 0, 4, 0, 0, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _6 _5 ) ( ) ( )");
            verifyR2Up(l, 4, 1, 1, 1, 0, 1, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 _5 _6 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
            verifyR2Up(l, 1, 0, 1, 4, 1, 1, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^6 ^5 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");
            verifyR2Up(l, 2, 0, 0, 1, 1, 1, "++---+- "
                "( ) ( _0 ^1 _6 _5 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _4 ) ( ) ( )");
            verifyR2Up(l, 1, 1, 1, 2, 0, 0, "++---+- "
                "( ) ( _0 ^1 ^5 ^6 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 _4 ) ( ) ( )");
            delete l;
        }

        void verifyResolve(const Link* l, int crossing,
                const char* briefResult) {
            Link clone(*l, false);
            clone.setLabel(l->label());

            clone.resolve(clone.crossing(crossing));

            sanity(&clone);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << l->label() << ": resolve(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void resolve() {
            Link* l;

            l = Link::fromData({ +1 }, { 1, -1 });
            l->setLabel("One twist (a)");
            verifyResolve(l, 0, "( ) ( )");
            delete l;

            l = Link::fromData({ +1 }, { -1, 1 });
            l->setLabel("One twist (b)");
            verifyResolve(l, 0, "( ) ( )");
            delete l;

            l = Link::fromData({ -1 }, { 1, -1 });
            l->setLabel("One twist (c)");
            verifyResolve(l, 0, "( ) ( )");
            delete l;

            l = Link::fromData({ -1 }, { -1, 1 });
            l->setLabel("One twist (d)");
            verifyResolve(l, 0, "( ) ( )");
            delete l;

            l = Link::fromData({ -1, +1, -1, -1 },
                { 4, -1, 2, -2, 3, -4, 1, -3 });
            l->setLabel("Trefoil with + twist (a)");
            verifyResolve(l, 1, "--- ( ^2 _0 ^1 _2 ^0 _1 ) ( )");
            delete l;

            l = Link::fromData({ -1, +1, -1, -1 },
                { 2, -2, 3, -4, 1, -3, 4, -1 });
            l->setLabel("Trefoil with + twist (b)");
            verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
            delete l;

            l = Link::fromData({ -1, +1, -1, -1 },
                { -2, 3, -4, 1, -3, 4, -1, 2 });
            l->setLabel("Trefoil with + twist (c)");
            verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
            delete l;

            l = Link::fromData({ +1, -1, +1, +1 },
                { 4, -1, -2, 2, 3, -4, 1, -3 });
            l->setLabel("Trefoil with - twist (a)");
            verifyResolve(l, 1, "+++ ( ^2 _0 ^1 _2 ^0 _1 ) ( )");
            delete l;

            l = Link::fromData({ +1, -1, +1, +1 },
                { -2, 2, 3, -4, 1, -3, 4, -1 });
            l->setLabel("Trefoil with - twist (b)");
            verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
            delete l;

            l = Link::fromData({ +1, -1, +1, +1 },
                { 2, 3, -4, 1, -3, 4, -1, -2 });
            l->setLabel("Trefoil with - twist (c)");
            verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
            delete l;

            l = Link::fromData({ +1, +1, -1, -1 },
                { 3, -1, 2, -3, 4, -2, 1, -4 });
            l->setLabel("Figure eight (a)");
            verifyResolve(l, 2, "++- ( _0 ^1 ) ( ^2 _1 ^0 _2 )");
            delete l;

            l = Link::fromData({ +1, +1, -1, -1 },
                { -3, 4, -2, 1, -4, 3, -1, 2 });
            l->setLabel("Figure eight (b)");
            verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )");
            delete l;

            l = Link::fromData({ +1, +1, -1, -1 },
                { 2, -3, 4, -2, 1, -4, 3, -1 });
            l->setLabel("Figure eight (c)");
            verifyResolve(l, 2, "++- ( ^1 _0 ) ( ^2 _1 ^0 _2 )");
            delete l;

            l = Link::fromData({ +1, +1, -1, -1 },
                { 4, -2, 1, -4, 3, -1, 2, -3 });
            l->setLabel("Figure eight (d)");
            verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )");
            delete l;

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { -3, 4 });
            l->setLabel("Figure eight with link (a)");
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
            delete l;

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { 4, -3 });
            l->setLabel("Figure eight with link (b)");
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
            delete l;

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { -3, 4 });
            l->setLabel("Figure eight with link (c)");
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
            delete l;

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { 4, -3 });
            l->setLabel("Figure eight with link (d)");
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
            delete l;
        }

        void verifyKnotSig(const Link* l, bool reflect, bool reverse) {
            std::string sig = l->knotSig(reflect, reverse);
            if (sig.empty()) {
                std::ostringstream msg;
                msg << l->label() << ": empty knotSig.";
                CPPUNIT_FAIL(msg.str());
            }

            {
                Link alt(*l, false);
                alt.rotate();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << l->label() << ": rotation gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (reflect) {
                Link alt(*l, false);
                alt.reflect();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << l->label() << ": reflection gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (reverse) {
                Link alt(*l, false);
                alt.reverse();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << l->label() << ": reversal gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Link* recon = Link::fromKnotSig(sig);
            if (! recon) {
                std::ostringstream msg;
                msg << l->label() << ": cannot reconstruct from knotSig.";
                CPPUNIT_FAIL(msg.str());
            }
            if (recon->size() != l->size()) {
                std::ostringstream msg;
                msg << l->label() << ": knotSig reconstruction has "
                    "different size.";
                CPPUNIT_FAIL(msg.str());
            }
            if (recon->countComponents() != l->countComponents()) {
                std::ostringstream msg;
                msg << l->label() << ": knotSig reconstruction has "
                    "different number of components.";
                CPPUNIT_FAIL(msg.str());
            }
            if (recon->knotSig(reflect, reverse) != sig) {
                std::ostringstream msg;
                msg << l->label() << ": knotSig reconstruction has "
                    "different knotSig.";
                CPPUNIT_FAIL(msg.str());
            }
            if (l->size() <= 12 && ! reflect) {
                if (recon->jones() != l->jones()) {
                    std::ostringstream msg;
                    msg << l->label() << ": knotSig reconstruction has "
                        "different Jones polynomial.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            delete recon;
        }

        void verifyKnotSig(const Link* l) {
            verifyKnotSig(l, true, true);
            verifyKnotSig(l, true, false);
            verifyKnotSig(l, false, true);
            verifyKnotSig(l, false, false);
        }

        void verifyKnotSig(const Link* l, bool reflect, bool reverse,
                const char* expect) {
            std::string sig = l->knotSig(reflect, reverse);
            if (sig != expect) {
                std::ostringstream msg;
                msg << l->label() << ": knotSig("
                    << (reflect ? 't' : 'f') << ", "
                    << (reverse ? 't' : 'f')
                    << ") is " << sig << ", not " << expect << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void knotSig() {
            verifyKnotSig(unknot0);
            verifyKnotSig(unknot1);
            verifyKnotSig(unknot3);
            verifyKnotSig(unknotGordian);
            verifyKnotSig(trefoilLeft);
            verifyKnotSig(trefoilRight);
            verifyKnotSig(figureEight);

            verifyKnotSig(trefoilRight, true, true, "dabcabcv-");
            verifyKnotSig(trefoilRight, false, true, "dabcabcv-");
            verifyKnotSig(trefoilLeft, true, true, "dabcabcv-");
            verifyKnotSig(trefoilLeft, false, true, "dabcabcva");

            // A link where all four boolean options give different sigs.
            Link* l = Link::fromOrientedGauss(
                "-<6 +>3 -<5 +>2 -<4 -<1 +>1 +>5 -<3 +>6 -<2 +>4");
            l->setLabel("Antisymmetric knot");
            verifyKnotSig(l, true,  true,  "gaabcdefbcfedPQ--");
            verifyKnotSig(l, true,  false, "gaabcdefdcbefPQ--");
            verifyKnotSig(l, false, true,  "gaabcdefbcfedPQaa");
            verifyKnotSig(l, false, false, "gaabcdefdcbefPQaa");
            delete l;
        }

        void verifyRewrite(const Link* l, int height) {
            // For now, this should only be called for knots that are
            // equivalent to their mirror image.
            if (l->rewrite(height, 1 /* threads */, nullptr,
                    [l](const Link& alt) {
                        return (alt.jones() != l->jones());
                    })) {
                std::ostringstream msg;
                msg << l->label() << ": rewrite() produced a different "
                    "jones polynomial.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void rewrite() {
            verifyRewrite(figureEight, 1);
            verifyRewrite(figureEight, 2);
        }
};

void addLink(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkTest::suite());
}

