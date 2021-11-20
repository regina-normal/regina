
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

#include <cmath>
#include <iomanip>
#include <unistd.h>
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
#define TREFOIL_SIGS { "cPcbbbadh", "cPcbbbadu", "dLQbcbcdlcj", "dLQbcbcdlcn", "dLQabccbrwj", "eLAkbbcddainqv" }
#define FIG8_SIGS { "cPcbbbiht" }

class LinkTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkTest);

    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(components);
    CPPUNIT_TEST(linking);
    CPPUNIT_TEST(writhe);
    CPPUNIT_TEST(selfFrame);
    CPPUNIT_TEST(parallel);
    CPPUNIT_TEST(jones);
    CPPUNIT_TEST(homfly);
    CPPUNIT_TEST(complement);
    CPPUNIT_TEST(r1Count);
    CPPUNIT_TEST(r2Count);
    CPPUNIT_TEST(r3Count);
    CPPUNIT_TEST(r123Perform);
    CPPUNIT_TEST(resolve);
    CPPUNIT_TEST(knotSig);
    CPPUNIT_TEST(dt);
    CPPUNIT_TEST(gauss);
    CPPUNIT_TEST(orientedGauss);
    CPPUNIT_TEST(pdCode);
    CPPUNIT_TEST(rewrite);
    CPPUNIT_TEST(swapping);
    CPPUNIT_TEST(group);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * The empty link:
         */
        Link empty;

        /**
         * Knots:
         */
        Link unknot0, unknot1, unknot3, unknotMonster, unknotGordian;
        Link trefoilLeft, trefoilRight, figureEight;
        Link trefoil_r1x2, trefoil_r1x6, figureEight_r1x2;
        Link conway, kinoshitaTerasaka, gst;

        /**
         * Links:
         */
        Link unlink2_0, unlink3_0;
        Link unlink2_r2, unlink2_r1r1;
        Link hopf, whitehead, borromean;
        Link trefoil_unknot0, trefoil_unknot1, trefoil_unknot_overlap;
        Link adams6_28; // Figure 6.28 from Adams

        /**
         * Composite knots:
         */
        Link rht_rht, rht_lht;

    public:
        void setUp() override {
            empty = Link();

            unknot0 = Link(1);

            unknot1 = Link::fromData({ 1 }, { 1, -1 });

            unknot3 = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });

            unknotMonster = ExampleLink::monster();

            unknotGordian = ExampleLink::gordian();

            trefoilLeft = ExampleLink::trefoilLeft();

            trefoilRight = ExampleLink::trefoilRight();

            figureEight = ExampleLink::figureEight();

            trefoil_r1x2 = ExampleLink::trefoilRight();
            {
                Crossing* c = trefoil_r1x2.crossing(0);
                trefoil_r1x2.r1(c->upper(), 0, -1, false, true);
                trefoil_r1x2.r1(c->lower(), 1, +1, false, true);
            }

            trefoil_r1x6 = ExampleLink::trefoilRight();
            {
                Crossing* c[3];
                c[0] = trefoil_r1x6.crossing(0);
                c[1] = trefoil_r1x6.crossing(1);
                c[2] = trefoil_r1x6.crossing(2);
                trefoil_r1x6.r1(c[0]->upper(), 0, -1, false, true);
                trefoil_r1x6.r1(c[0]->lower(), 1, -1, false, true);
                trefoil_r1x6.r1(c[1]->upper(), 1, +1, false, true);
                trefoil_r1x6.r1(c[1]->lower(), 0, +1, false, true);
                trefoil_r1x6.r1(c[2]->upper(), 0, +1, false, true);
                trefoil_r1x6.r1(c[2]->lower(), 0, -1, false, true);
            }

            figureEight_r1x2 = ExampleLink::figureEight();
            {
                Crossing* c = figureEight_r1x2.crossing(3);
                figureEight_r1x2.r1(c->upper(), 0, -1, false, true);
                figureEight_r1x2.r1(c->lower(), 1, +1, false, true);
            }

            conway = ExampleLink::conway();

            kinoshitaTerasaka = ExampleLink::kinoshitaTerasaka();

            gst = ExampleLink::gst();

            unlink2_0 = Link(2);

            unlink3_0 = Link(3);

            unlink2_r2 = Link::fromData({ 1, -1 }, { 1, 2 }, { -1, -2 });

            unlink2_r1r1 = Link::fromData({ -1, 1 }, { 1, -1 }, { -2, 2 });

            hopf = ExampleLink::hopf();

            whitehead = ExampleLink::whitehead();

            borromean = ExampleLink::borromean();

            trefoil_unknot0 = Link::fromData({ 1, 1, 1 },
                { 1, -2, 3, -1, 2, -3 }, { 0 });

            trefoil_unknot1 = Link::fromData(
                { +1, +1, -1, +1 }, { 1, -2, 4, -1, 2, -4 }, { -3, 3 });

            trefoil_unknot_overlap = Link::fromData(
                { -1, +1, +1, +1, +1 }, { 2, -3, -4, -1, 5, -2, 3, -5 },
                { 4, 1 });

            adams6_28 = Link::fromData({ +1, +1, -1, -1, +1, +1 },
                { -2, 1, -5, 6 }, { 2, -3, 4, -6, 5, -4, 3, -1 });

            rht_rht = ExampleLink::trefoilRight();
            rht_rht.composeWith(trefoilRight);

            rht_lht = ExampleLink::trefoilRight();
            rht_lht.composeWith(trefoilLeft);
        }

        void tearDown() override {
        }

        void sanity(const Link& l, const std::string& name) {
            for (Crossing* c : l.crossings()) {
                StrandRef lower(c, 0);
                StrandRef upper(c, 1);
                if (lower.next().prev() != lower ||
                        upper.next().prev() != upper) {
                    std::ostringstream msg;
                    msg << name << ": inconsistent next/prev links.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksIdentical(const Link& a, const Link& b) {
            if (a.size() != b.size())
                return false;
            if (a.countComponents() != b.countComponents())
                return false;
            if (a.brief() != b.brief())
                return false;
            return true;
        }

        void testCopyMove(const Link& l, const char* name) {
            Crossing* c0 = (l.size() > 0 ? l.crossing(0) : nullptr);

            Link copy(l);
            if (! looksIdentical(copy, l)) {
                std::ostringstream msg;
                msg << name << ": copy constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Crossing* c1 = (copy.size() > 0 ? copy.crossing(0) : nullptr);
            if (c1 == c0 && l.size() > 0) {
                std::ostringstream msg;
                msg << name << ": copy constructed uses the same crossings.";
                CPPUNIT_FAIL(msg.str());
            }

            Link move(std::move(copy));
            if (! looksIdentical(move, l)) {
                std::ostringstream msg;
                msg << name << ": move constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Crossing* c2 = (move.size() > 0 ? move.crossing(0) : nullptr);
            if (c2 != c1) {
                std::ostringstream msg;
                msg << name << ": move constructed does not use the "
                    "same crossings.";
                CPPUNIT_FAIL(msg.str());
            }

            Link copyAss(2); // A two-component unlink
            copyAss = l;
            if (! looksIdentical(copyAss, l)) {
                std::ostringstream msg;
                msg << name << ": copy assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Crossing* c3 = (copyAss.size() > 0 ? copyAss.crossing(0) : nullptr);
            if (c3 == c0 && l.size() > 0) {
                std::ostringstream msg;
                msg << name << ": copy assigned uses the same crossings.";
                CPPUNIT_FAIL(msg.str());
            }

            Link moveAss(2); // A two-component unlink
            moveAss = std::move(copyAss);
            if (! looksIdentical(moveAss, l)) {
                std::ostringstream msg;
                msg << name << ": move assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Crossing* c4 = (moveAss.size() > 0 ? moveAss.crossing(0) : nullptr);
            if (c4 != c3) {
                std::ostringstream msg;
                msg << name << ": move assigned does not use the "
                    "same crossings.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void copyMove() {
            testCopyMove(empty, "Empty");
            testCopyMove(unknot0, "Unknot (0 crossings)");
            testCopyMove(unknot1, "Unknot (1 crossing)");
            testCopyMove(unknot3, "Unknot (3 crossings)");
            testCopyMove(unknotMonster, "Monster unknot");
            testCopyMove(unknotGordian, "Gordian unknot");
            testCopyMove(trefoilLeft, "LH Trefoil");
            testCopyMove(trefoilRight, "RH Trefoil");
            testCopyMove(trefoil_r1x2, "Trefoil with 2 R1s");
            testCopyMove(trefoil_r1x6, "Trefoil with 6 R1s");
            testCopyMove(figureEight, "Figure eight");
            testCopyMove(figureEight_r1x2, "Figure eight with 2 R1s");
            testCopyMove(conway, "Conway knot");
            testCopyMove(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            testCopyMove(gst, "GST");
            testCopyMove(unlink2_0, "Unlink (2 components)");
            testCopyMove(unlink3_0, "Unlink (3 components)");
            testCopyMove(unlink2_r2, "Unlink (2 components via R2)");
            testCopyMove(unlink2_r1r1, "Unlink (2 components via R1+R1)");
            testCopyMove(hopf, "Hopf link");
            testCopyMove(whitehead, "Whitehead link");
            testCopyMove(borromean, "Borromean rings");
            testCopyMove(trefoil_unknot0, "Trefoil U unknot (separate)");
            testCopyMove(trefoil_unknot1,
                "Trefoil U unknot (separate + twist)");
            testCopyMove(trefoil_unknot_overlap, "Trefoil U unknot (with R2)");
            testCopyMove(rht_rht, "RH Trefoil # RH Trefoil");
            testCopyMove(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void testComponents(const Link& l, size_t expected, const char* name) {
            if (l.countComponents() != expected) {
                std::ostringstream msg;
                msg << name << ": expected " << expected
                    << " components, found " << l.countComponents() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void components() {
            testComponents(empty, 0, "Empty");
            testComponents(unknot0, 1, "Unknot (0 crossings)");
            testComponents(unknot1, 1, "Unknot (1 crossing)");
            testComponents(unknot3, 1, "Unknot (3 crossings)");
            testComponents(unknotMonster, 1, "Monster unknot");
            testComponents(unknotGordian, 1, "Gordian unknot");
            testComponents(trefoilLeft, 1, "LH Trefoil");
            testComponents(trefoilRight, 1, "RH Trefoil");
            testComponents(trefoil_r1x2, 1, "Trefoil with 2 R1s");
            testComponents(trefoil_r1x6, 1, "Trefoil with 6 R1s");
            testComponents(figureEight, 1, "Figure eight");
            testComponents(figureEight_r1x2, 1, "Figure eight with 2 R1s");
            testComponents(conway, 1, "Conway knot");
            testComponents(kinoshitaTerasaka, 1, "Kinoshita-Terasaka knot");
            testComponents(gst, 1, "GST");
            testComponents(unlink2_0, 2, "Unlink (2 components)");
            testComponents(unlink3_0, 3, "Unlink (3 components)");
            testComponents(unlink2_r2, 2, "Unlink (2 components via R2)");
            testComponents(unlink2_r1r1, 2, "Unlink (2 components via R1+R1)");
            testComponents(hopf, 2, "Hopf link");
            testComponents(whitehead, 2, "Whitehead link");
            testComponents(borromean, 3, "Borromean rings");
            testComponents(trefoil_unknot0, 2, "Trefoil U unknot (separate)");
            testComponents(trefoil_unknot1, 2,
                "Trefoil U unknot (separate + twist)");
            testComponents(trefoil_unknot_overlap, 2,
                "Trefoil U unknot (with R2)");
            testComponents(rht_rht, 1, "RH Trefoil # RH Trefoil");
            testComponents(rht_lht, 1, "RH Trefoil # LH Trefoil");
        }

        void testLinking(const Link& l, long expected, const char* name) {
            long ans = l.linking();
            if (ans != expected) {
                std::ostringstream msg;
                msg << name << ": expected lk = " << expected
                    << ", computed " << ans << " instead.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void linking() {
            testLinking(empty, 0, "Empty");
            testLinking(unknot0, 0, "Unknot (0 crossings)");
            testLinking(unknot1, 0, "Unknot (1 crossing)");
            testLinking(unknot3, 0, "Unknot (3 crossings)");
            testLinking(unknotMonster, 0, "Monster unknot");
            testLinking(unknotGordian, 0, "Gordian unknot");
            testLinking(trefoilLeft, 0, "LH Trefoil");
            testLinking(trefoilRight, 0, "RH Trefoil");
            testLinking(trefoil_r1x2, 0, "Trefoil with 2 R1s");
            testLinking(trefoil_r1x6, 0, "Trefoil with 6 R1s");
            testLinking(figureEight, 0, "Figure eight");
            testLinking(figureEight_r1x2, 0, "Figure eight with 2 R1s");
            testLinking(conway, 0, "Conway knot");
            testLinking(kinoshitaTerasaka, 0, "Kinoshita-Terasaka knot");
            testLinking(gst, 0, "GST");
            testLinking(unlink2_0, 0, "Unlink (2 components)");
            testLinking(unlink3_0, 0, "Unlink (3 components)");
            testLinking(unlink2_r2, 0, "Unlink (2 components via R2)");
            testLinking(unlink2_r1r1, 0, "Unlink (2 components via R1+R1)");
            testLinking(hopf, 1, "Hopf link");
            testLinking(whitehead, 0, "Whitehead link");
            testLinking(borromean, 0, "Borromean rings");
            testLinking(trefoil_unknot0, 0, "Trefoil U unknot (separate)");
            testLinking(trefoil_unknot1, 0,
                "Trefoil U unknot (separate + twist)");
            testLinking(trefoil_unknot_overlap, 0,
                "Trefoil U unknot (with R2)");
            testLinking(rht_rht, 0, "RH Trefoil # RH Trefoil");
            testLinking(rht_lht, 0, "RH Trefoil # LH Trefoil");
        }

        void verifyWrithe(const Link& l, const char* name) {
            long sum = 0;
            for (long c = 0; c < l.countComponents(); ++c)
                sum += l.writheOfComponent(c);
            if (sum + 2 * l.linking() != l.writhe()) {
                std::ostringstream msg;
                msg << name << ": sum of component writhes + "
                    "2 * linking number != writhe.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void writhe() {
            verifyWrithe(unknot0, "Unknot (0 crossings)");
            verifyWrithe(unknot1, "Unknot (1 crossing)");
            verifyWrithe(unknot3, "Unknot (3 crossings)");
            verifyWrithe(unknotMonster, "Monster unknot");
            verifyWrithe(unknotGordian, "Gordian unknot");
            verifyWrithe(trefoilLeft, "LH Trefoil");
            verifyWrithe(trefoilRight, "RH Trefoil");
            verifyWrithe(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyWrithe(trefoil_r1x6, "Trefoil with 6 R1s");
            verifyWrithe(figureEight, "Figure eight");
            verifyWrithe(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyWrithe(conway, "Conway knot");
            verifyWrithe(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyWrithe(gst, "GST");
            verifyWrithe(rht_rht, "RH Trefoil # RH Trefoil");
            verifyWrithe(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void verifySelfFrame(const Link& l, const char* name) {
            Link framed(l);
            framed.selfFrame();
            long framedWrithe = framed.writhe();
            if (framedWrithe != 0) {
                std::ostringstream msg;
                msg << name << ": self-framed version has writhe "
                    << framedWrithe << ", not 0.";
                CPPUNIT_FAIL(msg.str());
            }

            if (l.size() < 20) {
                auto jones = l.jones();
                auto framedJones = framed.jones();
                if (jones != framedJones) {
                    std::ostringstream msg;
                    msg << name << ": self-framed version has different "
                        "Jones polynomial (" << jones << " != "
                        << framedJones << ").";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void selfFrame() {
            verifySelfFrame(unknot0, "Unknot (0 crossings)");
            verifySelfFrame(unknot1, "Unknot (1 crossing)");
            verifySelfFrame(unknot3, "Unknot (3 crossings)");
            verifySelfFrame(unknotMonster, "Monster unknot");
            verifySelfFrame(unknotGordian, "Gordian unknot");
            verifySelfFrame(trefoilLeft, "LH Trefoil");
            verifySelfFrame(trefoilRight, "RH Trefoil");
            verifySelfFrame(trefoil_r1x2, "Trefoil with 2 R1s");
            verifySelfFrame(trefoil_r1x6, "Trefoil with 6 R1s");
            verifySelfFrame(figureEight, "Figure eight");
            verifySelfFrame(figureEight_r1x2, "Figure eight with 2 R1s");
            verifySelfFrame(conway, "Conway knot");
            verifySelfFrame(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifySelfFrame(gst, "GST");
            verifySelfFrame(rht_rht, "RH Trefoil # RH Trefoil");
            verifySelfFrame(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void testParallel(const Link& l, const char* name) {
            long writhe = l.writhe();
            long linking = l.linking();

            // Compute the sum of writhe and |writhe| for each individual
            // component.  We do this in quadratic time, so the code is simple
            // enough to be sure it's right.
            long writheSame = 0, absWritheSame = 0;
            for (auto start : l.components()) {
                if (! start)
                    continue;

                long writeComp = 0;
                StrandRef s = start;
                do {
                    StrandRef t = s;
                    for (++t; t != start; ++t) {
                        if (t.crossing() == s.crossing()) {
                            writeComp += t.crossing()->sign();
                            break;
                        }
                    }
                    ++s;
                } while (s != start);

                writheSame += writeComp;
                absWritheSame += (writeComp >= 0 ? writeComp : -writeComp);
            }

            for (int k = 0; k <= 3; ++k) {
                Link p = l.parallel(k, regina::FRAMING_BLACKBOARD);
                if (p.countComponents() != k * l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", blackboard) "
                        "has the wrong number of components.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.size() != k * k * l.size()) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", blackboard) "
                        "has the wrong number of crossings.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.writhe() != k * k * writhe) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", blackboard) "
                        "has the wrong writhe.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.linking() != k * k * linking +
                        (k * (k-1) * writheSame) / 2) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", blackboard) "
                        "has the wrong linking number.";
                    CPPUNIT_FAIL(msg.str());
                }

                p = l.parallel(k, regina::FRAMING_SEIFERT);
                if (p.countComponents() != k * l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", seifert) "
                        "has the wrong number of components.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.size() != k * k * l.size() +
                        k * (k-1) * absWritheSame) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", seifert) "
                        "has the wrong number of crossings.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.writhe() != k * k * writhe - k * (k-1) * writheSame) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", seifert) "
                        "has the wrong writhe.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.linking() != k * k * linking) {
                    std::ostringstream msg;
                    msg << name << ": parallel(" << k << ", seifert) "
                        "has the wrong linking number.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void parallel() {
            testParallel(empty, "Empty");
            testParallel(unknot0, "Unknot (0 crossings)");
            testParallel(unknot1, "Unknot (1 crossing)");
            testParallel(unknot3, "Unknot (3 crossings)");
            testParallel(unknotMonster, "Monster unknot");
            testParallel(unknotGordian, "Gordian unknot");
            testParallel(trefoilLeft, "LH Trefoil");
            testParallel(trefoilRight, "RH Trefoil");
            testParallel(trefoil_r1x2, "Trefoil with 2 R1s");
            testParallel(trefoil_r1x6, "Trefoil with 6 R1s");
            testParallel(figureEight, "Figure eight");
            testParallel(figureEight_r1x2, "Figure eight with 2 R1s");
            testParallel(conway, "Conway knot");
            testParallel(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            testParallel(gst, "GST");
            testParallel(unlink2_0, "Unlink (2 components)");
            testParallel(unlink3_0, "Unlink (3 components)");
            testParallel(unlink2_r2, "Unlink (2 components via R2)");
            testParallel(unlink2_r1r1, "Unlink (2 components via R1+R1)");
            testParallel(hopf, "Hopf link");
            testParallel(whitehead, "Whitehead link");
            testParallel(borromean, "Borromean rings");
            testParallel(trefoil_unknot0, "Trefoil U unknot (separate)");
            testParallel(trefoil_unknot1,
                "Trefoil U unknot (separate + twist)");
            testParallel(trefoil_unknot_overlap, "Trefoil U unknot (with R2)");
            testParallel(rht_rht, "RH Trefoil # RH Trefoil");
            testParallel(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void testJones(const Link& l, const char* expected, const char* name) {
            // Since we are computing the Jones polynomial multiple times
            // (using different algorithms), we work with clones of l
            // that do not clone any already-computed properties.

            if (l.size() <= 40) {
                // The naive algorithm iterates through 2^n states.
                // If n > 40 then we don't have a chance.
                std::ostringstream s1;
                s1 << Link(l, false).jones(regina::ALG_NAIVE);

                if (s1.str() != expected) {
                    std::ostringstream msg;
                    msg << name << ": expected V(link) = " << expected
                        << ", found " << s1.str() << " using naive algorithm.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            std::ostringstream s2;
            s2 << Link(l, false).jones(regina::ALG_TREEWIDTH);

            if (s2.str() != expected) {
                std::ostringstream msg;
                msg << name << ": expected V(link) = " << expected
                    << ", found " << s2.str() << " using treewidth algorithm.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void jones() {
            testJones(empty, "0", "Empty");
            testJones(unknot0, "1", "Unknot (0 crossings)");
            testJones(unknot1, "1", "Unknot (1 crossing)");
            testJones(unknot3, "1", "Unknot (3 crossings)");
            testJones(unknotMonster, "1", "Monster unknot");
            // The Gordian unknot is too large to compute V(link).
            testJones(trefoilLeft, "x^-2 + x^-6 - x^-8", "LH Trefoil");
            testJones(trefoilRight, "-x^8 + x^6 + x^2", "RH Trefoil");
            testJones(trefoil_r1x2, "-x^8 + x^6 + x^2", "Trefoil with 2 R1s");
            testJones(trefoil_r1x6, "-x^8 + x^6 + x^2", "Trefoil with 6 R1s");
            testJones(figureEight, "x^4 - x^2 + 1 - x^-2 + x^-4",
                "Figure eight");
            testJones(figureEight_r1x2, "x^4 - x^2 + 1 - x^-2 + x^-4",
                "Figure eight with 2 R1s");
            testJones(unlink2_0, "-x - x^-1", "Unlink (2 components)");
            testJones(unlink3_0, "x^2 + 2 + x^-2", "Unlink (3 components)");
            testJones(unlink2_r2, "-x - x^-1", "Unlink (2 components via R2)");
            testJones(unlink2_r1r1, "-x - x^-1",
                "Unlink (2 components via R1+R1)");
            testJones(hopf, "-x^5 - x", "Hopf link");
            testJones(whitehead, "-x^3 + x - 2 x^-1 + x^-3 - 2 x^-5 + x^-7",
                "Whitehead link");
            testJones(borromean,
                "-x^6 + 3 x^4 - 2 x^2 + 4 - 2 x^-2 + 3 x^-4 - x^-6",
                "Borromean rings");
            testJones(trefoil_unknot0, "x^9 - x^5 - x^3 - x",
                "Trefoil U unknot (separate)");
            testJones(trefoil_unknot1, "x^9 - x^5 - x^3 - x",
                "Trefoil U unknot (separate + twist)");
            testJones(trefoil_unknot_overlap, "x^9 - x^5 - x^3 - x",
                "Trefoil U unknot (with R2)");
            testJones(rht_rht, "x^16 - 2 x^14 + x^12 - 2 x^10 + 2 x^8 + x^4",
                "RH Trefoil # RH Trefoil");
            testJones(rht_lht, "-x^6 + x^4 - x^2 + 3 - x^-2 + x^-4 - x^-6",
                "RH Trefoil # LH Trefoil");

            // The following polynomials were computed using Regina,
            // using the naive algorithm (ALG_NAIVE).
            // This means that they have not been verified independently;
            // here they are simply being used to ensure that different
            // algorithms give the same results, and also that nothing breaks
            // in a subsequent release.
            testJones(conway, "-x^8 + 2 x^6 - 2 x^4 + 2 x^2 + x^-4 - 2 x^-6 + 2 x^-8 - 2 x^-10 + x^-12",
                "Conway knot");
            testJones(kinoshitaTerasaka, "-x^8 + 2 x^6 - 2 x^4 + 2 x^2 + x^-4 - 2 x^-6 + 2 x^-8 - 2 x^-10 + x^-12",
                "Kinoshita-Terasaka knot");

            // The following polynomials were computed using Regina,
            // via the treewidth-based algorithm (ALG_TREEWIDTH).
            // This is because they are too large for the naive
            // algorithm to handle.
            testJones(gst, "-x^32 + 3 x^30 - 4 x^28 + 3 x^26 - x^24 + x^22 - x^20 + x^18 - x^16 + 2 x^14 - 4 x^12 + 3 x^10 - x^8 - 3 x^6 + 5 x^4 - 5 x^2 + 5 - 3 x^-2 + 3 x^-4 - x^-6 + x^-12 - x^-14",
                "GST");
        }

        void testHomflyAZ(const Link& l, bool reverse, const char* expected,
                const char* name) {
            // Since we are computing the HOMFLY polynomial multiple times
            // (using different algorithms), we work with clones of l
            // that do not clone any already-computed properties.

            std::ostringstream s1;
            {
                Link use(l, false);
                if (reverse)
                    use.reverse();
                s1 << use.homflyAZ(regina::ALG_BACKTRACK);
            }

            if (s1.str() != expected) {
                std::ostringstream msg;
                msg << name << ": expected homflyAZ(link) = " << expected
                    << ", found " << s1.str() << " using backtrack algorithm";
                if (reverse)
                    msg << " with reversed link";
                msg << ".";
                CPPUNIT_FAIL(msg.str());
            }

            std::ostringstream s2;
            {
                Link use(l, false);
                if (reverse)
                    use.reverse();
                s2 << use.homflyAZ(regina::ALG_TREEWIDTH);
            }

            if (s2.str() != expected) {
                std::ostringstream msg;
                msg << name << ": expected homflyAZ(link) = " << expected
                    << ", found " << s2.str() << " using treewidth algorithm";
                if (reverse)
                    msg << " with reversed link";
                msg << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testHomflyAZ(const Link& l, const char* expected,
                const char* name) {
            testHomflyAZ(l, false, expected, name);
            testHomflyAZ(l, true, expected, name);
        }

        void testHomflyLM(const Link& l, bool reverse, const char* expected,
                const char* name) {
            std::ostringstream s1;
            {
                Link use(l, false);
                if (reverse)
                    use.reverse();
                s1 << use.homflyLM(regina::ALG_BACKTRACK);
            }

            if (s1.str() != expected) {
                std::ostringstream msg;
                msg << name << ": expected homflyLM(link) = " << expected
                    << ", found " << s1.str() << " using backtrack algorithm";
                if (reverse)
                    msg << " with reversed link";
                msg << ".";
                CPPUNIT_FAIL(msg.str());
            }

            std::ostringstream s2;
            {
                Link use(l, false);
                if (reverse)
                    use.reverse();
                s2 << use.homflyLM(regina::ALG_TREEWIDTH);
            }

            if (s2.str() != expected) {
                std::ostringstream msg;
                msg << name << ": expected homflyLM(link) = " << expected
                    << ", found " << s2.str() << " using treewidth algorithm";
                if (reverse)
                    msg << " with reversed link";
                msg << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testHomflyLM(const Link& l, const char* expected,
                const char* name) {
            testHomflyLM(l, false, expected, name);
            testHomflyLM(l, true, expected, name);
        }

        void homfly() {
            testHomflyAZ(empty, "0", "Empty");
            testHomflyLM(empty, "0", "Empty");

            testHomflyAZ(unknot0, "1", "Unknot (0 crossings)");
            testHomflyLM(unknot0, "1", "Unknot (0 crossings)");
            testHomflyAZ(unknot1, "1", "Unknot (1 crossing)");
            testHomflyLM(unknot1, "1", "Unknot (1 crossing)");
            testHomflyAZ(unknot3, "1", "Unknot (3 crossings)");
            testHomflyLM(unknot3, "1", "Unknot (3 crossings)");
            testHomflyAZ(unknotMonster, "1", "Monster unknot");
            testHomflyLM(unknotMonster, "1", "Monster unknot");
            // The Gordian unknot is surely too large for this.

            testHomflyLM(unlink2_0, "-x y^-1 - x^-1 y^-1",
                "Unlink (2 components)");
            testHomflyLM(unlink3_0, "x^2 y^-2 + 2 y^-2 + x^-2 y^-2",
                "Unlink (3 components)");
            testHomflyLM(unlink2_r2, "-x y^-1 - x^-1 y^-1",
                "Unlink (2 components via R2)");
            testHomflyLM(unlink2_r1r1, "-x y^-1 - x^-1 y^-1",
                "Unlink (2 components via R1+R1)");

            testHomflyLM(trefoilLeft, "-x^4 + x^2 y^2 - 2 x^2",
                "LH Trefoil");
            testHomflyLM(trefoilRight, "x^-2 y^2 - 2 x^-2 - x^-4",
                "RH Trefoil");
            testHomflyAZ(trefoilRight, "x^-2 y^2 + 2 x^-2 - x^-4",
                "RH Trefoil");
            testHomflyLM(trefoil_r1x2, "x^-2 y^2 - 2 x^-2 - x^-4",
                "Trefoil with 2 R1s");
            testHomflyLM(trefoil_r1x6, "x^-2 y^2 - 2 x^-2 - x^-4",
                "Trefoil with 6 R1s");

            testHomflyLM(figureEight, "-x^2 + y^2 - 1 - x^-2", "Figure eight");
            testHomflyLM(figureEight_r1x2, "-x^2 + y^2 - 1 - x^-2",
                "Figure eight with 2 R1s");

            testHomflyLM(hopf, "-x^-1 y + x^-1 y^-1 + x^-3 y^-1", "Hopf link");

            testHomflyLM(trefoil_unknot0,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
                "Trefoil U unknot (separate)");
            testHomflyLM(trefoil_unknot1,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
                "Trefoil U unknot (separate + twist)");
            testHomflyLM(trefoil_unknot_overlap,
                "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
                "Trefoil U unknot (with R2)");

            // This is different from Adams' claim regarding the HOMFLY
            // polynomial of this link.  But... Adams does get his arithmetic
            // wrong elsewhere, and a calculation by hand using the Skein
            // relation agrees with the polynomial below.
            testHomflyLM(adams6_28,
                "x y - x^-1 y^3 + x^-1 y + 2 x^-3 y - x^-3 y^-1 - x^-5 y^-1",
                "Adams Fig. 6.28");

            testHomflyLM(rht_rht,
                "x^-4 y^4 - 4 x^-4 y^2 + 4 x^-4 - 2 x^-6 y^2 + 4 x^-6 + x^-8",
                "RH Trefoil # RH Trefoil");
            testHomflyLM(rht_lht,
                "-x^2 y^2 + 2 x^2 + y^4 - 4 y^2 + 5 - x^-2 y^2 + 2 x^-2",
                "RH Trefoil # LH Trefoil");

            // The following polynomials were computed using Regina,
            // using Kauffman's skein template algorithm (ALG_BACKTRACK).
            // This means that they have not been verified independently;
            // here they are simply being used to ensure that different
            // algorithms give the same results, and also that nothing breaks
            // in a subsequent release.
            testHomflyLM(whitehead,
                "x^3 y - x y^3 + 2 x y - x y^-1 + x^-1 y - x^-1 y^-1",
                "Whitehead link");
            testHomflyLM(borromean,
                "-x^2 y^2 + x^2 y^-2 + y^4 - 2 y^2 + 2 y^-2 - x^-2 y^2 + x^-2 y^-2",
                "Borromean rings");
            testHomflyLM(conway,
                "x^4 y^4 - 3 x^4 y^2 + 2 x^4 - x^2 y^6 + 6 x^2 y^4 - 11 x^2 y^2 + 6 x^2 - y^6 + 6 y^4 - 11 y^2 + 7 + x^-2 y^4 - 3 x^-2 y^2 + 2 x^-2",
                "Conway knot");
            testHomflyLM(kinoshitaTerasaka,
                "x^4 y^4 - 3 x^4 y^2 + 2 x^4 - x^2 y^6 + 6 x^2 y^4 - 11 x^2 y^2 + 6 x^2 - y^6 + 6 y^4 - 11 y^2 + 7 + x^-2 y^4 - 3 x^-2 y^2 + 2 x^-2",
                "Kinoshita-Terasaka knot");

            // On my machine, GST takes around 25 seconds for ALG_TREEWIDTH,
            // and 88 seconds for ALG_NAIVE.  This is probably still a
            // touch too slow to put in the test suite.
            // testHomflyLM(gst,
            //     "-x^4 y^8 + 6 x^4 y^6 - 11 x^4 y^4 + 8 x^4 y^2 - 2 x^4 - x^2 y^12 + 10 x^2 y^10 - 35 x^2 y^8 + 49 x^2 y^6 - 21 x^2 y^4 - 7 x^2 y^2 + 5 x^2 + y^14 - 12 y^12 + 53 y^10 - 102 y^8 + 67 y^6 + 36 y^4 - 63 y^2 + 21 - x^-2 y^16 + 16 x^-2 y^14 - 104 x^-2 y^12 + 355 x^-2 y^10 - 685 x^-2 y^8 + 744 x^-2 y^6 - 422 x^-2 y^4 + 100 x^-2 y^2 - 3 x^-2 + x^-4 y^18 - 18 x^-4 y^16 + 137 x^-4 y^14 - 575 x^-4 y^12 + 1457 x^-4 y^10 - 2296 x^-4 y^8 + 2233 x^-4 y^6 - 1279 x^-4 y^4 + 385 x^-4 y^2 - 45 x^-4 + x^-6 y^18 - 17 x^-6 y^16 + 122 x^-6 y^14 - 484 x^-6 y^12 + 1168 x^-6 y^10 - 1776 x^-6 y^8 + 1698 x^-6 y^6 - 978 x^-6 y^4 + 304 x^-6 y^2 - 38 x^-6 - x^-8 y^16 + 14 x^-8 y^14 - 79 x^-8 y^12 + 233 x^-8 y^10 - 393 x^-8 y^8 + 392 x^-8 y^6 - 228 x^-8 y^4 + 71 x^-8 y^2 - 9 x^-8");

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

        void testComplementBasic(const Link& l, const char* name) {
            Triangulation<3> c = l.complement();

            if (c.countComponents() != 1) {
                std::ostringstream msg;
                msg << name << " complement: expected 1 component, "
                    "found " << c.countComponents() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            size_t ideal = 0;
            for (auto v : c.vertices()) {
                regina::Vertex<3>::LinkType t = v->linkType();
                if (t == regina::Vertex<3>::TORUS)
                    ++ideal;
                else if (t != regina::Vertex<3>::SPHERE) {
                    std::ostringstream msg;
                    msg << name << " complement: "
                        "contains a vertex with unexpected link type.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (ideal != l.countComponents()) {
                std::ostringstream msg;
                msg << name << " complement: expected "
                    << l.countComponents() << " ideal vertices, "
                    "found " << ideal << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testComplementS3(const Link& l, const char* name) {
            if (! l.complement().isSphere()) {
                std::ostringstream msg;
                msg << name << " complement: not a 3-sphere "
                    "as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testComplementUnknot(const Link& l, const char* name) {
            if (! l.complement().isSolidTorus()) {
                std::ostringstream msg;
                msg << name << " complement: not a solid torus as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testComplementSig(const Link& l,
                std::initializer_list<const char*> expected, const char* name) {
            std::string sig = l.complement().isoSig();

            for (auto e : expected)
                if (sig == e)
                    return;

            std::ostringstream msg;
            msg << name << " complement: isosig is " << sig << ", not ";
            auto e = expected.begin();
            msg << *e;
            for (++e; e != expected.end(); ++e)
                msg << " / " << *e;
            msg << " as expected.";
            CPPUNIT_FAIL(msg.str());
        }

        void testComplementCensus(const Link& l, std::string prefix,
                const char* name) {
            std::string sig = l.complement().isoSig();

            auto hits = regina::Census::lookup(sig);
            for (const auto& hit : hits) {
                if (hit.name().substr(0, prefix.size()) == prefix)
                    return;
            }

            std::ostringstream msg;
            msg << name << " complement: isosig " << sig
                << " was not identified in the census as " << prefix
                << "... .";
            CPPUNIT_FAIL(msg.str());
        }

        void testComplementFree(const Link& l, unsigned nGen,
                const char* name) {
            const regina::GroupPresentation fg =
                l.complement().fundamentalGroup();
            if (fg.countGenerators() != nGen || fg.countRelations() != 0) {
                std::ostringstream msg;
                msg << name << " complement: fundamental group is "
                    "not recognised as free on " << nGen
                    << " generators as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testComplementFreeAbelian(const Link& l, unsigned nGen,
                const char* name) {
            std::ostringstream expected;
            expected << nGen << " Z";

            const regina::GroupPresentation fg =
                l.complement().fundamentalGroup();
            if (fg.recogniseGroup() != expected.str()) {
                std::ostringstream msg;
                msg << name << " complement: fundamental group is "
                    "not recognised as free abelian on " << nGen
                    << " generators as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void testComplementTrefoilUnknot(const Link& l, const char* name) {
            // Find a separating sphere in the complement.
            regina::NormalSurfaces vtx(l.complement(), regina::NS_STANDARD);
            for (const regina::NormalSurface& s : vtx) {
                if (s.eulerChar() != 2)
                    continue;
                // s must be a 2-sphere.

                Triangulation<3> cut = s.cutAlong();
                cut.finiteToIdeal(); // Fills the sphere boundaries with balls.
                if (cut.isConnected()) {
                    // Should never happen...
                    std::ostringstream msg;
                    msg << name << " complement contains a "
                        "non-separating 2-sphere.";
                    CPPUNIT_FAIL(msg.str());
                }

                cut.intelligentSimplify();
                auto comp = cut.triangulateComponents();

                if ((sigMatches(comp[0].isoSig(), TREFOIL_SIGS) &&
                         comp[1].isSolidTorus()) ||
                        (sigMatches(comp[1].isoSig(), TREFOIL_SIGS) &&
                         comp[0].isSolidTorus())) {
                    // We have the correct manifold!
                    return;
                }
            }

            std::ostringstream msg;
            msg << name << " complement could not be recognised "
                "as coming from (trefoil U unknot).";
            CPPUNIT_FAIL(msg.str());
        }

        void complement() {
            testComplementBasic(empty, "Empty");
            testComplementBasic(unknot0, "Unknot (0 crossings)");
            testComplementBasic(unknot1, "Unknot (1 crossing)");
            testComplementBasic(unknot3, "Unknot (3 crossings)");
            testComplementBasic(unknotMonster, "Monster unknot");
            testComplementBasic(unknotGordian, "Gordian unknot");
            testComplementBasic(trefoilLeft, "LH Trefoil");
            testComplementBasic(trefoilRight, "RH Trefoil");
            testComplementBasic(trefoil_r1x2, "Trefoil with 2 R1s");
            testComplementBasic(trefoil_r1x6, "Trefoil with 6 R1s");
            testComplementBasic(figureEight, "Figure eight");
            testComplementBasic(figureEight_r1x2, "Figure eight with 2 R1s");
            testComplementBasic(conway, "Conway knot");
            testComplementBasic(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            testComplementBasic(gst, "GST");
            testComplementBasic(unlink2_0, "Unlink (2 components)");
            testComplementBasic(unlink3_0, "Unlink (3 components)");
            testComplementBasic(unlink2_r2, "Unlink (2 components via R2)");
            testComplementBasic(unlink2_r1r1,
                "Unlink (2 components via R1+R1)");
            testComplementBasic(hopf, "Hopf link");
            testComplementBasic(whitehead, "Whitehead link");
            testComplementBasic(borromean, "Borromean rings");
            testComplementBasic(trefoil_unknot0, "Trefoil U unknot (separate)");
            testComplementBasic(trefoil_unknot1,
                "Trefoil U unknot (separate + twist)");
            testComplementBasic(trefoil_unknot_overlap,
                "Trefoil U unknot (with R2)");
            testComplementBasic(rht_rht, "RH Trefoil # RH Trefoil");
            testComplementBasic(rht_lht, "RH Trefoil # LH Trefoil");

            testComplementS3(empty, "Empty");
            testComplementUnknot(unknot0, "Unknot (0 crossings)");
            testComplementUnknot(unknot1, "Unknot (1 crossing)");
            testComplementUnknot(unknot3, "Unknot (3 crossings)");
            testComplementUnknot(unknotMonster, "Monster unknot");
            // Too large! - testComplementUnknot(unknotGordian);

            // For some knots and links, it is reasonable to assume that
            // intelligentSimplify() will reach a minimal triangulation.
            //
            // In some cases there are too many minimal triangulations
            // to list here, and so we use a census lookup.
            // cPcbbbadh 
            testComplementSig(trefoilLeft, TREFOIL_SIGS, "LH Trefoil");
            testComplementSig(trefoilRight, TREFOIL_SIGS, "RH Trefoil");
            testComplementSig(trefoil_r1x2, TREFOIL_SIGS, "Trefoil with 2 R1s");
            testComplementSig(trefoil_r1x6, TREFOIL_SIGS, "Trefoil with 6 R1s");
            testComplementSig(figureEight, FIG8_SIGS, "Figure eight");
            testComplementSig(figureEight_r1x2, FIG8_SIGS,
                "Figure eight with 2 R1s");
            testComplementCensus(whitehead, "m129 :", "Whitehead link");
            testComplementCensus(borromean, "t12067 :", "Borromean rings");

            // For unlink complements, we can test that the fundamental
            // group is free.
            testComplementFree(unlink2_0, 2, "Unlink (2 components)");
            testComplementFree(unlink3_0, 3, "Unlink (3 components)");
            testComplementFree(unlink2_r2, 2, "Unlink (2 components via R2)");
            testComplementFree(unlink2_r1r1, 2,
                "Unlink (2 components via R1+R1)");

            // For the Hopf link, we can test that the fundamental group
            // is free abelian.
            testComplementFreeAbelian(hopf, 2, "Hopf link");

            // A specialised test for the complement of (trefoil U unknot).
            testComplementTrefoilUnknot(trefoil_unknot0,
                "Trefoil U unknot (separate)");
            testComplementTrefoilUnknot(trefoil_unknot1,
                "Trefoil U unknot (separate + twist)");
            testComplementTrefoilUnknot(trefoil_unknot_overlap,
                "Trefoil U unknot (with R2)");
        }

        void verifyCountR1Up(Link& link, size_t expected, const char* name) {
            size_t total = 0;

            int cr, strand, side, sign;
            for (side = 0; side < 2; ++side)
                for (sign = -1; sign <= 1; sign += 2) {
                    if (link.r1(StrandRef(), side, sign, true, false))
                        ++total;

                    for (cr = 0; cr < link.size(); ++cr)
                        for (strand = 0; strand < 2; ++strand)
                            if (link.r1(link.crossing(cr)->strand(strand),
                                    side, sign, true, false))
                                ++total;
                }

            if (total != expected) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R1 moves "
                    "up, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR1Down(Link& link, size_t expected, const char* name) {
            size_t total = 0;

            if (link.r1(nullptr, true, false))
                ++total;

            int cr;
            for (cr = 0; cr < link.size(); ++cr)
                if (link.r1(link.crossing(cr), true, false))
                    ++total;

            if (total != expected) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R1 moves "
                    "down, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR2Up(Link& link, size_t expected, const char* name) {
            size_t total = 0;

            int cr1, cr2, str1, str2, side1, side2;
            for (side1 = 0; side1 < 2; ++side1)
                for (side2 = 0; side2 < 2; ++side2) {
                    if (link.r2(StrandRef(), side1, StrandRef(), side2,
                            true, false))
                        ++total;

                    for (cr1 = 0; cr1 < link.size(); ++cr1)
                        for (str1 = 0; str1 < 2; ++str1) {
                            if (link.r2(StrandRef(), side1,
                                    link.crossing(cr1)->strand(str1), side2,
                                    true, false))
                                ++total;
                            if (link.r2(link.crossing(cr1)->strand(str1),
                                    side1, StrandRef(), side2,
                                    true, false))
                                ++total;

                            for (cr2 = 0; cr2 < link.size(); ++cr2)
                                for (str2 = 0; str2 < 2; ++str2)
                                    if (link.r2(
                                            link.crossing(cr1)->strand(str1),
                                            side1,
                                            link.crossing(cr2)->strand(str2),
                                            side2, true, false))
                                        ++total;
                        }
                }

            if (total != expected) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R2 moves "
                    "up, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR2Down(Link& link, size_t expected, const char* name) {
            verifyCountR2Down(link, expected, 2 * expected, name);
        }

        void verifyCountR2Down(Link& link, size_t expectedCr,
                size_t expectedStr, const char* name) {
            // Most of the time, expectedStr == expectedCr * 2.
            //
            // However, this can differ in the case of an unknotted loop
            // placed on top of another strand - here there are 2 moves
            // by crossing, but only 3 moves by strand (all of which
            // produce identical results).

            int cr, strand;

            size_t total = 0;

            if (link.r2(nullptr, true, false))
                ++total;

            for (cr = 0; cr < link.size(); ++cr)
                if (link.r2(link.crossing(cr), true, false))
                    ++total;

            if (total != expectedCr) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R2 moves "
                    "down by crossing, not " << expectedCr << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            total = 0;

            if (link.r2(StrandRef(), true, false))
                ++total;

            for (cr = 0; cr < link.size(); ++cr)
                for (strand = 0; strand < 2; ++strand)
                    if (link.r2(link.crossing(cr)->strand(strand),
                            true, false))
                        ++total;

            if (total != expectedStr) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R2 moves "
                    "down by strand, not " << expectedStr << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyCountR3(Link& link, size_t expected, const char* name) {
            int cr, strand, side;

            size_t total = 0;
            for (side = 0; side < 2; ++side) {
                if (link.r3(nullptr, side, true, false))
                    ++total;

                for (cr = 0; cr < link.size(); ++cr)
                    if (link.r3(link.crossing(cr), side, true, false))
                        ++total;
            }

            if (total != expected) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R3 moves "
                    "by crossing, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            total = 0;
            for (side = 0; side < 2; ++side) {
                if (link.r3(StrandRef(), side, true, false))
                    ++total;

                for (cr = 0; cr < link.size(); ++cr)
                    for (strand = 0; strand < 2; ++strand)
                        if (link.r3(link.crossing(cr)->strand(strand),
                                side, true, false))
                            ++total;
            }

            if (total != expected * 3) {
                std::ostringstream msg;
                msg << name << " offers " << total << " R3 moves "
                    "by strand, not " << expected << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void r1Count() {
            verifyCountR1Up(empty, 0, "Empty");
            verifyCountR1Down(empty, 0, "Empty");

            verifyCountR1Up(unknot0, 4, "Unknot (0 crossings)");
            verifyCountR1Down(unknot0, 0, "Unknot (0 crossings)");

            verifyCountR1Up(unknot1, 8, "Unknot (1 crossing)");
            verifyCountR1Down(unknot1, 1, "Unknot (1 crossing)");

            verifyCountR1Up(unknot3, 24, "Unknot (3 crossings)");
            verifyCountR1Down(unknot3, 0, "Unknot (3 crossings)");

            verifyCountR1Up(trefoilLeft, 24, "LH Trefoil");
            verifyCountR1Down(trefoilLeft, 0, "LH Trefoil");

            verifyCountR1Up(trefoilRight, 24, "RH Trefoil");
            verifyCountR1Down(trefoilRight, 0, "RH Trefoil");

            verifyCountR1Up(trefoil_r1x2, 40, "Trefoil with 2 R1s");
            verifyCountR1Down(trefoil_r1x2, 2, "Trefoil with 2 R1s");

            verifyCountR1Up(trefoil_r1x6, 72, "Trefoil with 6 R1s");
            verifyCountR1Down(trefoil_r1x6, 6, "Trefoil with 6 R1s");

            verifyCountR1Up(figureEight, 32, "Figure eight");
            verifyCountR1Down(figureEight, 0, "Figure eight");

            verifyCountR1Up(figureEight_r1x2, 48, "Figure eight with 2 R1s");
            verifyCountR1Down(figureEight_r1x2, 2, "Figure eight with 2 R1s");

            verifyCountR1Up(conway, 88, "Conway knot");
            verifyCountR1Down(conway, 0, "Conway knot");

            verifyCountR1Up(kinoshitaTerasaka, 88, "Kinoshita-Terasaka knot");
            verifyCountR1Down(kinoshitaTerasaka, 0, "Kinoshita-Terasaka knot");

            verifyCountR1Up(gst, 384, "GST");
            verifyCountR1Down(gst, 0, "GST");

            verifyCountR1Up(unlink2_0, 4, "Unlink (2 components)");
            verifyCountR1Down(unlink2_0, 0, "Unlink (2 components)");

            verifyCountR1Up(unlink3_0, 4, "Unlink (3 components)");
            verifyCountR1Down(unlink3_0, 0, "Unlink (3 components)");

            verifyCountR1Up(unlink2_r2, 16, "Unlink (2 components via R2)");
            verifyCountR1Down(unlink2_r2, 0, "Unlink (2 components via R2)");

            verifyCountR1Up(unlink2_r1r1, 16,
                "Unlink (2 components via R1+R1)");
            verifyCountR1Down(unlink2_r1r1, 2,
                "Unlink (2 components via R1+R1)");

            verifyCountR1Up(hopf, 16, "Hopf link");
            verifyCountR1Down(hopf, 0, "Hopf link");

            verifyCountR1Up(whitehead, 40, "Whitehead link");
            verifyCountR1Down(whitehead, 0, "Whitehead link");

            verifyCountR1Up(borromean, 48, "Borromean rings");
            verifyCountR1Down(borromean, 0, "Borromean rings");

            verifyCountR1Up(trefoil_unknot0, 28, "Trefoil U unknot (separate)");
            verifyCountR1Down(trefoil_unknot0, 0,
                "Trefoil U unknot (separate)");

            verifyCountR1Up(trefoil_unknot1, 32,
                "Trefoil U unknot (separate + twist)");
            verifyCountR1Down(trefoil_unknot1, 1,
                "Trefoil U unknot (separate + twist)");

            verifyCountR1Up(trefoil_unknot_overlap, 40,
                "Trefoil U unknot (with R2)");
            verifyCountR1Down(trefoil_unknot_overlap, 0,
                "Trefoil U unknot (with R2)");

            verifyCountR1Up(adams6_28, 48, "Adams Fig. 6.28");
            verifyCountR1Down(adams6_28, 0, "Adams Fig. 6.28");
        }

        void r2Count() {
            verifyCountR2Up(empty, 0, "Empty");
            verifyCountR2Down(empty, 0, "Empty");

            verifyCountR2Up(unknot0, 0, "Unknot (0 crossings)");
            verifyCountR2Down(unknot0, 0, "Unknot (0 crossings)");

            verifyCountR2Up(unknot1, 2, "Unknot (1 crossing)");
            verifyCountR2Down(unknot1, 0, "Unknot (1 crossing)");

            verifyCountR2Up(unknot3, 18, "Unknot (3 crossings)");
            verifyCountR2Down(unknot3, 2, "Unknot (3 crossings)");

            verifyCountR2Up(trefoilLeft, 18, "LH Trefoil");
            verifyCountR2Down(trefoilLeft, 0, "LH Trefoil");

            verifyCountR2Up(trefoilRight, 18, "RH Trefoil");
            verifyCountR2Down(trefoilRight, 0, "RH Trefoil");

            verifyCountR2Up(trefoil_r1x2, 58, "Trefoil with 2 R1s");
            verifyCountR2Down(trefoil_r1x2, 0, "Trefoil with 2 R1s");

            verifyCountR2Up(trefoil_r1x6, 160, "Trefoil with 6 R1s");
            verifyCountR2Down(trefoil_r1x6, 0, "Trefoil with 6 R1s");

            verifyCountR2Up(figureEight, 28, "Figure eight");
            verifyCountR2Down(figureEight, 0, "Figure eight");

            verifyCountR2Up(figureEight_r1x2, 66, "Figure eight with 2 R1s");
            verifyCountR2Down(figureEight_r1x2, 0, "Figure eight with 2 R1s");

            verifyCountR2Up(conway, 120, "Conway knot");
            verifyCountR2Down(conway, 0, "Conway knot");

            verifyCountR2Up(kinoshitaTerasaka, 118, "Kinoshita-Terasaka knot");
            verifyCountR2Down(kinoshitaTerasaka, 0, "Kinoshita-Terasaka knot");

            verifyCountR2Up(gst, 612, "GST");
            verifyCountR2Down(gst, 0, "GST");

            verifyCountR2Up(unlink2_0, 4, "Unlink (2 components)");
            verifyCountR2Down(unlink2_0, 0, "Unlink (2 components)");

            verifyCountR2Up(unlink3_0, 4, "Unlink (3 components)");
            verifyCountR2Down(unlink3_0, 0, "Unlink (3 components)");

            verifyCountR2Up(unlink2_r2, 8, "Unlink (2 components via R2)");
            verifyCountR2Down(unlink2_r2, 2, "Unlink (2 components via R2)");

            verifyCountR2Up(unlink2_r1r1, 36,
                "Unlink (2 components via R1+R1)");
            verifyCountR2Down(unlink2_r1r1, 0,
                "Unlink (2 components via R1+R1)");

            verifyCountR2Up(hopf, 8, "Hopf link");
            verifyCountR2Down(hopf, 0, "Hopf link");

            verifyCountR2Up(whitehead, 40, "Whitehead link");
            verifyCountR2Down(whitehead, 0, "Whitehead link");

            verifyCountR2Up(borromean, 48, "Borromean rings");
            verifyCountR2Down(borromean, 0, "Borromean rings");

            verifyCountR2Up(trefoil_unknot0, 66, "Trefoil U unknot (separate)");
            verifyCountR2Down(trefoil_unknot0, 0,
                "Trefoil U unknot (separate)");

            verifyCountR2Up(trefoil_unknot1, 116,
                "Trefoil U unknot (separate + twist)");
            verifyCountR2Down(trefoil_unknot1, 0,
                "Trefoil U unknot (separate + twist)");

            verifyCountR2Up(trefoil_unknot_overlap, 46,
                "Trefoil U unknot (with R2)");
            verifyCountR2Down(trefoil_unknot_overlap, 2, 3,
                "Trefoil U unknot (with R2)");

            verifyCountR2Up(adams6_28, 54, "Adams Fig. 6.28");
            verifyCountR2Down(adams6_28, 0, "Adams Fig. 6.28");
        }

        void r3Count() {
            verifyCountR3(empty, 0, "Empty");
            verifyCountR3(unknot0, 0, "Unknot (0 crossings)");
            verifyCountR3(unknot1, 0, "Unknot (1 crossing)");
            verifyCountR3(unknot3, 2, "Unknot (3 crossings)");
            verifyCountR3(trefoilLeft, 0, "LH Trefoil");
            verifyCountR3(trefoilRight, 0, "RH Trefoil");
            verifyCountR3(trefoil_r1x2, 0, "Trefoil with 2 R1s");
            verifyCountR3(trefoil_r1x6, 0, "Trefoil with 6 R1s");
            verifyCountR3(figureEight, 0, "Figure eight");
            verifyCountR3(figureEight_r1x2, 0, "Figure eight with 2 R1s");
            verifyCountR3(conway, 0, "Conway knot");
            verifyCountR3(kinoshitaTerasaka, 0, "Kinoshita-Terasaka knot");
            verifyCountR3(gst, 10, "GST");
            verifyCountR3(unlink2_0, 0, "Unlink (2 components)");
            verifyCountR3(unlink3_0, 0, "Unlink (3 components)");
            verifyCountR3(unlink2_r2, 0, "Unlink (2 components via R2)");
            verifyCountR3(unlink2_r1r1, 0, "Unlink (2 components via R1+R1)");
            verifyCountR3(hopf, 0, "Hopf link");
            verifyCountR3(whitehead, 0, "Whitehead link");
            verifyCountR3(borromean, 0, "Borromean rings");
            verifyCountR3(trefoil_unknot0, 0, "Trefoil U unknot (separate)");
            verifyCountR3(trefoil_unknot1, 0,
                "Trefoil U unknot (separate + twist)");
            verifyCountR3(trefoil_unknot_overlap, 0,
                "Trefoil U unknot (with R2)");
            verifyCountR3(adams6_28, 0, "Adams Fig. 6.28");
        }

        void verifyR1Down(const Link& l, int crossing,
                const char* briefResult, const std::string& name) {
            Link clone(l, false);

            if (! clone.r1(clone.crossing(crossing))) {
                std::ostringstream msg;
                msg << name << ": R1(" << crossing << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R1(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR1Up(const Link& l, int crossing, int strand,
                int side, int sign, const char* briefResult,
                const std::string& name) {
            Link clone(l, false);

            StrandRef s;
            if (crossing >= 0)
                s = clone.crossing(crossing)->strand(strand);

            if (! clone.r1(s, side, sign)) {
                std::ostringstream msg;
                msg << name << ": R1(" << s << ", " << side << ", "
                    << sign << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R1(" << s << ", " << side << ", "
                    << sign << ") gives " << clone.brief() << ", not "
                    << briefResult << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Down(const Link& l, int crossing,
                const char* briefResult, const std::string& name) {
            Link clone(l, false);

            if (! clone.r2(clone.crossing(crossing))) {
                std::ostringstream msg;
                msg << name << ": R2(" << crossing << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R2(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Down(const Link& l, int crossing, int strand,
                const char* briefResult, const std::string& name) {
            Link clone(l, false);

            if (! clone.r2(clone.crossing(crossing)->strand(strand))) {
                std::ostringstream msg;
                msg << name << ": R2("
                    << (strand ? '^' : '_') << crossing
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R2("
                    << (strand ? '^' : '_') << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR2Up(const Link& l, int upperCrossing, int upperStrand,
                int upperSide, int lowerCrossing, int lowerStrand,
                int lowerSide, const char* briefResult,
                const std::string& name) {
            Link clone(l, false);

            StrandRef upper, lower;
            if (upperCrossing >= 0)
                upper = clone.crossing(upperCrossing)->strand(upperStrand);
            if (lowerCrossing >= 0)
                lower = clone.crossing(lowerCrossing)->strand(lowerStrand);

            if (! clone.r2(upper, upperSide, lower, lowerSide)) {
                std::ostringstream msg;
                msg << name << ": R2(" << upper << ", " << upperSide
                    << ", " << lower << ", " << lowerSide
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R2(" << upper << ", " << upperSide
                    << ", " << lower << ", " << lowerSide << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR3(const Link& l, int crossing, int side,
                const char* briefResult, const std::string& name) {
            Link clone(l, false);

            if (! clone.r3(clone.crossing(crossing), side)) {
                std::ostringstream msg;
                msg << name << ": R3(" << crossing << ", "
                    << side << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R3(" << crossing << ", "
                    << side << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyR3(const Link& l, int crossing, int strand, int side,
                const char* briefResult, const std::string& name) {
            Link clone(l, false);

            if (! clone.r3(clone.crossing(crossing)->strand(strand), side)) {
                std::ostringstream msg;
                msg << name << ": R3("
                    << (strand ? '^' : '_') << crossing << ", " << side
                    << ") is not allowed.";
                CPPUNIT_FAIL(msg.str());
            }

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": R3(" << (strand ? '^' : '_')
                    << crossing << ", " << side << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void r123Perform() {
            Link l;
            std::string label;

            l = Link::fromData({ -1 }, { 1, -1 });
            label = "One twist";
            verifyR1Down(l, 0, "( )", label);

            l = Link::fromData({ 1, -1 }, { -1, 1, 2, -2 });
            label = "Two twists (a)";
            verifyR1Down(l, 0, "- ( ^0 _0 )", label);
            verifyR2Down(l, 0, "( )", label);
            verifyR2Down(l, 0, 1, "( )", label);
            verifyR2Down(l, 1, 0, "( )", label);

            l = Link::fromData({ 1, -1 }, { 1, 2, -2, -1 });
            label = "Two twists (b)";
            verifyR1Down(l, 0, "- ( ^0 _0 )", label);
            verifyR2Down(l, 0, "( )", label);
            verifyR2Down(l, 0, 1, "( )", label);
            verifyR2Down(l, 1, 0, "( )", label);

            l = Link::fromData({ 1, -1 }, { 2, -2, -1, 1 });
            label = "Two twists (c)";
            verifyR1Down(l, 0, "- ( ^0 _0 )", label);
            verifyR2Down(l, 0, "( )", label);
            verifyR2Down(l, 0, 1, "( )", label);
            verifyR2Down(l, 1, 0, "( )", label);

            l = Link::fromData({ 1, -1 }, { -2, -1, 1, 2 });
            label = "Two twists (d)";
            verifyR1Down(l, 0, "- ( _0 ^0 )", label);
            verifyR2Down(l, 0, "( )", label);
            verifyR2Down(l, 0, 1, "( )", label);
            verifyR2Down(l, 1, 0, "( )", label);

            l = Link::fromData({ 1, -1 }, { 1, 2 }, { -2, -1 });
            label = "Overlapping loops";
            verifyR2Down(l, 0, "( ) ( )", label);
            verifyR2Down(l, 1, "( ) ( )", label);
            verifyR2Down(l, 0, 0, "( ) ( )", label);
            verifyR2Down(l, 0, 1, "( ) ( )", label);
            verifyR2Down(l, 1, 0, "( ) ( )", label);
            verifyR2Down(l, 1, 1, "( ) ( )", label);

            l = Link::fromData({ -1, 1, -1 }, { -1, 1, 3, 2 }, { -2, -3 });
            label = "Loop overlapping twist (a)";
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
            verifyR3(l, 0, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
            verifyR3(l, 0, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
            verifyR3(l, 1, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
            verifyR3(l, 1, 0, 1, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);

            l = Link::fromData({ -1, 1, -1 }, { 3, 2, -1, 1 }, { -2, -3 });
            label = "Loop overlapping twist (b)";
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
            verifyR3(l, 0, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
            verifyR3(l, 0, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
            verifyR3(l, 1, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
            verifyR3(l, 1, 0, 1, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);

            l = Link::fromData({ -1, 1, -1 }, { 2, -1, 1, 3 }, { -2, -3 });
            label = "Loop overlapping twist (c)";
            verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
            verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
            verifyR3(l, 0, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
            verifyR3(l, 0, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
            verifyR3(l, 1, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
            verifyR3(l, 1, 0, 1, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);

            l = Link::fromData({ 1, -1, 1 }, { 1, -1, -3, -2 }, { 2, 3 });
            label = "Loop overlapping twist (d)";
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR3(l, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
            verifyR3(l, 0, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);

            l = Link::fromData({ 1, -1, 1 }, { -3, -2, 1, -1 }, { 2, 3 });
            label = "Loop overlapping twist (e)";
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR3(l, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
            verifyR3(l, 0, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);

            l = Link::fromData({ 1, -1, 1 }, { -2, 1, -1, -3 }, { 2, 3 });
            label = "Loop overlapping twist (f)";
            verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
            verifyR3(l, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
            verifyR3(l, 0, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
            verifyR3(l, 1, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);

            l = Link::fromData({ 1, -1, 1, -1, 1, -1 }, { 5, 4 },
                { 6, -6, -5, -3, 1, -1, -2, 2, 3, -4 });
            label = "Three triangles (a)";
            verifyR3(l, 4, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
            verifyR3(l, 4, 1, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
            verifyR3(l, 5, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
            verifyR3(l, 3, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
            verifyR3(l, 4, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
            verifyR3(l, 4, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
            verifyR3(l, 4, 0, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
            verifyR3(l, 2, 1, 1,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
            verifyR3(l, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
            verifyR3(l, 1, 1, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
            verifyR3(l, 2, 0, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
            verifyR3(l, 0, 0, 0,
                "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);

            l = Link::fromData({ -1, +1, -1, +1, -1, +1 }, { -5, -4 },
                { -6, 6, 5, 3, -1, 1, 2, -2, -3, 4 });
            label = "Three triangles (b)";
            verifyR3(l, 5, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
            verifyR3(l, 4, 0, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
            verifyR3(l, 5, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
            verifyR3(l, 3, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
            verifyR3(l, 4, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
            verifyR3(l, 4, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
            verifyR3(l, 4, 1, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
            verifyR3(l, 2, 0, 1,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
            verifyR3(l, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
            verifyR3(l, 1, 0, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
            verifyR3(l, 2, 1, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
            verifyR3(l, 0, 1, 0,
                "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);

            l = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
            label = "Bad trefoil (a)";
            verifyR2Down(l, 2, "+ ( _0 ^0 )", label);
            verifyR2Down(l, 2, 1, "+ ( _0 ^0 )", label);
            verifyR2Down(l, 2, 0, "+ ( _0 ^0 )", label);
            verifyR3(l, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 1, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 2, 0, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 0, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);

            l = Link::fromData({ 1, 1, -1 }, { -3, -1, 2, 3, 1, -2 });
            label = "Bad trefoil (b)";
            verifyR2Down(l, 2, "+ ( ^0 _0 )", label);
            verifyR2Down(l, 2, 1, "+ ( ^0 _0 )", label);
            verifyR2Down(l, 2, 0, "+ ( ^0 _0 )", label);
            verifyR3(l, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 1, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 2, 0, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 0, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);

            l = Link::fromData({ -1, -1, 1 }, { 1, -2, -3, -1, 2, 3 });
            label = "Bad trefoil (c)";
            verifyR2Down(l, 2, "- ( _0 ^0 )", label);
            verifyR2Down(l, 2, 1, "- ( _0 ^0 )", label);
            verifyR2Down(l, 2, 0, "- ( _0 ^0 )", label);
            verifyR3(l, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 1, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 2, 0, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
            verifyR3(l, 0, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);

            l = Link::fromData({ -1, -1, 1 }, { -3, -1, 2, 3, 1, -2 });
            label = "Bad trefoil (d)";
            verifyR2Down(l, 2, "- ( ^0 _0 )", label);
            verifyR2Down(l, 2, 1, "- ( ^0 _0 )", label);
            verifyR2Down(l, 2, 0, "- ( ^0 _0 )", label);
            verifyR3(l, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 1, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 2, 0, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
            verifyR3(l, 0, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);

            l = Link::fromData({ -1, 1, 1, -1}, { 1, 2, -4, -3, -2, -1, 3, 4 });
            label = "Bad figure eight (a)";
            verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 2, "-+ ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 2, 1, "-+ ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 3, 0, "-+ ( ^0 ^1 _1 _0 )", label);
            verifyR3(l, 0, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
            verifyR3(l, 0, 1, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
            verifyR3(l, 0, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
            verifyR3(l, 2, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);

            l = Link::fromData({ -1, 1, 1, -1}, { 2, -4, -3, -2, -1, 3, 4, 1 });
            label = "Bad figure eight (b)";
            verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )", label);

            l = Link::fromData({ -1, 1, 1, -1}, { -2, -1, 3, 4, 1, 2, -4, -3 });
            label = "Bad figure eight (c)";
            verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )", label);

            l = Link::fromData({ -1, 1, 1, -1}, { -1, 3, 4, 1, 2, -4, -3, -2 });
            label = "Bad figure eight (d)";
            verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )", label);
            verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )", label);

            l = Link::fromData({ 1, -1, -1, 1}, { -1, -2, 4, 3, 2, 1, -3, -4 });
            label = "Bad figure eight (e)";
            verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 3, "+- ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 2, 0, "+- ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 3, 1, "+- ( _0 _1 ^1 ^0 )", label);
            verifyR3(l, 2, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
            verifyR3(l, 0, 0, 1, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
            verifyR3(l, 0, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
            verifyR3(l, 2, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);

            l = Link::fromData({ 1, -1, -1, 1}, { -2, 4, 3, 2, 1, -3, -4, -1 });
            label = "Bad figure eight (f)";
            verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )", label);

            l = Link::fromData({ 1, -1, -1, 1}, { 2, 1, -3, -4, -1, -2, 4, 3 });
            label = "Bad figure eight (g)";
            verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )", label);

            l = Link::fromData({ 1, -1, -1, 1}, { 1, -3, -4, -1, -2, 4, 3, 2 });
            label = "Bad figure eight (h)";
            verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )", label);
            verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )", label);

            l = Link::fromData({ -1, 1, -1, -1, -1 },
                { -2, -3, 4, -5, 1, 2, 3, -4, 5, -1 });
            label = "Excessive trefoil (a)";
            verifyR2Down(l, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
            verifyR2Down(l, 0, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
            verifyR2Down(l, 0, 1, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
            verifyR2Down(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);
            verifyR2Down(l, 1, 0, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);
            verifyR2Down(l, 1, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);

            l = Link::fromData({ 1, -1, 1, 1, 1 },
                { 2, 3, -4, 5, -1, -2, -3, 4, -5, 1 });
            label = "Excessive trefoil (b)";
            verifyR2Down(l, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
            verifyR2Down(l, 0, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
            verifyR2Down(l, 0, 1, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
            verifyR2Down(l, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);
            verifyR2Down(l, 1, 0, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);
            verifyR2Down(l, 1, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);

            l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
                { 1, -1, -2, 7, 6, 5, 4, 3, -5, -6, -7, 8, -8, 2 });
            label = "Dangling twists (a)";
            verifyR1Down(l, 0,
                "-+--+-- ( _1 _2 ) ( _0 ^5 ^4 ^3 ^2 ^1 _3 _4 _5 ^6 _6 ^0 )",
                label);
            verifyR1Down(l, 7,
                "+-+--+- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^4 ^3 ^2 _4 _5 _6 ^1 )",
                label);
            verifyR2Down(l, 1,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
            verifyR2Down(l, 1, 1,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
            verifyR2Down(l, 0, 0,
                "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
            verifyR2Down(l, 6,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 6, 1,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 5, 0,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 5,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 5, 1,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 4, 0,
                "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 3,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 3, 1,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
            verifyR2Down(l, 2, 0,
                "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
            verifyR3(l, 4, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
                label);
            verifyR3(l, 4, 1, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
                label);
            verifyR3(l, 2, 0, 1,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
                label);
            verifyR3(l, 2, 1, 0,
                "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
                label);

            l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
                { 6, 5, 4, 3, -5, -6, -7, 8, -8, 2, 1, -1, -2, 7 });
            label = "Dangling twists (b)";
            verifyR2Down(l, 6,
                "+-+--- ( _2 _3 ) ( ^4 ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 )", label);
            verifyR2Down(l, 5,
                "+-+--- ( _2 _3 ) ( ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 ^4 )", label);

            l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
                { -1, 1, 2, -7, -6, -5, -4, -3, 5, 6, 7, -8, 8, -2 });
            label = "Dangling twists (c)";
            verifyR1Down(l, 0,
                "+-++-++ ( ^1 ^2 ) ( ^0 _5 _4 _3 _2 _1 ^3 ^4 ^5 _6 ^6 _0 )",
                label);
            verifyR1Down(l, 7,
                "-+-++-+ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _4 _3 _2 ^4 ^5 ^6 _1 )",
                label);
            verifyR2Down(l, 0,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
            verifyR2Down(l, 0, 1,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
            verifyR2Down(l, 1, 0,
                "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
            verifyR2Down(l, 5,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 5, 1,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 6, 0,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 4,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 4, 1,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 5, 0,
                "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 2,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 2, 1,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
            verifyR2Down(l, 3, 0,
                "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
            verifyR3(l, 2, 1,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
                label);
            verifyR3(l, 2, 1, 1,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
                label);
            verifyR3(l, 2, 0, 0,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
                label);
            verifyR3(l, 4, 0, 0,
                "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
                label);

            l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
                { -6, -5, -4, -3, 5, 6, 7, -8, 8, -2, -1, 1, 2, -7 });
            label = "Dangling twists (d)";
            verifyR2Down(l, 5,
                "-+-+++ ( ^2 ^3 ) ( _4 _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 )", label);
            verifyR2Down(l, 4,
                "-+-+++ ( ^2 ^3 ) ( _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 _4 )", label);

            l = Link::fromData({ 1, 1, -1, -1, -1 },
                { 0 }, { -1, 2, -4, 3, -2, 1, -3, 4, 5, -5 }, { 0 }, { 0 });
            label = "Figure eight with twist and three unknots";
            verifyR1Up(l, -1, 0, 0, -1,
                "++---- ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, -1, 0, 0, 1,
                "++---+ ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, -1, 0, 1, -1,
                "++---- ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, -1, 0, 1, 1,
                "++---+ ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 4, 1, 0, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 4, 1, 0, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 4, 1, 1, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 4, 1, 1, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 1, 0, 0, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 1, 0, 0, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 1, 0, 1, -1,
                "++---- ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR1Up(l, 1, 0, 1, 1,
                "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
                label);
            // Note: for R2, the implementation always adds the two new
            // crossings in the order (+, -).
            verifyR2Up(l, -1, 0, 0, -1, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )",
                label);
            verifyR2Up(l, -1, 0, 0, -1, 0, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )",
                label);
            verifyR2Up(l, -1, 0, 1, -1, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )",
                label);
            verifyR2Up(l, -1, 0, 1, -1, 0, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )",
                label);
            verifyR2Up(l, -1, 0, 0, 4, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _6 _5 ) ( ) ( )",
                label);
            verifyR2Up(l, -1, 0, 1, 4, 0, 0, "++---+- "
                "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _5 _6 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 0, 0, -1, 0, 0, "++---+- "
                "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^5 ^6 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 0, 0, -1, 0, 1, "++---+- "
                "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^6 ^5 ) ( ) ( )",
                label);
            verifyR2Up(l, -1, 0, 0, 4, 1, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, -1, 0, 1, 4, 1, 1, "++---+- "
                "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 1, 1, -1, 0, 0, "++---+- "
                "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 1, 1, -1, 0, 1, "++---+- "
                "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 0, 0, 3, 1, 0, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^5 ^6 ) ( ) ( )",
                label);
            verifyR2Up(l, 3, 1, 0, 4, 0, 0, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _6 _5 ) ( ) ( )",
                label);
            verifyR2Up(l, 4, 1, 1, 1, 0, 1, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 _5 _6 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 1, 0, 1, 4, 1, 1, "++---+- "
                "( ) ( _0 ^1 _3 ^2 _1 ^6 ^5 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 2, 0, 0, 1, 1, 1, "++---+- "
                "( ) ( _0 ^1 _6 _5 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _4 ) ( ) ( )",
                label);
            verifyR2Up(l, 1, 1, 1, 2, 0, 0, "++---+- "
                "( ) ( _0 ^1 ^5 ^6 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 _4 ) ( ) ( )",
                label);
        }

        void verifyResolve(const Link& l, int crossing,
                const char* briefResult, const char* name) {
            Link clone(l, false);

            clone.resolve(clone.crossing(crossing));

            sanity(clone, name);

            if (clone.brief() != briefResult) {
                std::ostringstream msg;
                msg << name << ": resolve(" << crossing << ") gives "
                    << clone.brief() << ", not " << briefResult
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void resolve() {
            Link l;

            l = Link::fromData({ +1 }, { 1, -1 });
            verifyResolve(l, 0, "( ) ( )", "One twist (a)");

            l = Link::fromData({ +1 }, { -1, 1 });
            verifyResolve(l, 0, "( ) ( )", "One twist (b)");

            l = Link::fromData({ -1 }, { 1, -1 });
            verifyResolve(l, 0, "( ) ( )", "One twist (c)");

            l = Link::fromData({ -1 }, { -1, 1 });
            verifyResolve(l, 0, "( ) ( )", "One twist (d)");

            l = Link::fromData({ -1, +1, -1, -1 },
                { 4, -1, 2, -2, 3, -4, 1, -3 });
            verifyResolve(l, 1, "--- ( ^2 _0 ^1 _2 ^0 _1 ) ( )",
                "Trefoil with + twist (a)");

            l = Link::fromData({ -1, +1, -1, -1 },
                { 2, -2, 3, -4, 1, -3, 4, -1 });
            verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
                "Trefoil with + twist (b)");

            l = Link::fromData({ -1, +1, -1, -1 },
                { -2, 3, -4, 1, -3, 4, -1, 2 });
            verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
                "Trefoil with + twist (c)");

            l = Link::fromData({ +1, -1, +1, +1 },
                { 4, -1, -2, 2, 3, -4, 1, -3 });
            verifyResolve(l, 1, "+++ ( ^2 _0 ^1 _2 ^0 _1 ) ( )",
                "Trefoil with - twist (a)");

            l = Link::fromData({ +1, -1, +1, +1 },
                { -2, 2, 3, -4, 1, -3, 4, -1 });
            verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
                "Trefoil with - twist (b)");

            l = Link::fromData({ +1, -1, +1, +1 },
                { 2, 3, -4, 1, -3, 4, -1, -2 });
            verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
                "Trefoil with - twist (c)");

            l = Link::fromData({ +1, +1, -1, -1 },
                { 3, -1, 2, -3, 4, -2, 1, -4 });
            verifyResolve(l, 2, "++- ( _0 ^1 ) ( ^2 _1 ^0 _2 )",
                "Figure eight (a)");

            l = Link::fromData({ +1, +1, -1, -1 },
                { -3, 4, -2, 1, -4, 3, -1, 2 });
            verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )",
                "Figure eight (b)");

            l = Link::fromData({ +1, +1, -1, -1 },
                { 2, -3, 4, -2, 1, -4, 3, -1 });
            verifyResolve(l, 2, "++- ( ^1 _0 ) ( ^2 _1 ^0 _2 )",
                "Figure eight (c)");

            l = Link::fromData({ +1, +1, -1, -1 },
                { 4, -2, 1, -4, 3, -1, 2, -3 });
            verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )",
                "Figure eight (d)");

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { -3, 4 });
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
                "Figure eight with link (a)");

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { 4, -3 });
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
                "Figure eight with link (b)");

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { -3, 4 });
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
                "Figure eight with link (c)");

            l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
                { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { 4, -3 });
            verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
                "Figure eight with link (d)");
        }

        void verifyKnotSig(const Link& l, bool reflect, bool reverse,
                const char* name) {
            std::string sig = l.knotSig(reflect, reverse);
            if (sig.empty()) {
                std::ostringstream msg;
                msg << name << ": empty knotSig.";
                CPPUNIT_FAIL(msg.str());
            }

            {
                Link alt(l, false);
                alt.rotate();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << name << ": rotation gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (reflect) {
                Link alt(l, false);
                alt.reflect();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << name << ": reflection gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if (reverse) {
                Link alt(l, false);
                alt.reverse();
                std::string altSig = alt.knotSig(reflect, reverse);
                if (altSig != sig) {
                    std::ostringstream msg;
                    msg << name << ": reversal gives different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            try {
                Link recon = Link::fromKnotSig(sig);

                if (recon.size() != l.size()) {
                    std::ostringstream msg;
                    msg << name << ": knotSig reconstruction has "
                        "different size.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.countComponents() != l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": knotSig reconstruction has "
                        "different number of components.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.knotSig(reflect, reverse) != sig) {
                    std::ostringstream msg;
                    msg << name << ": knotSig reconstruction has "
                        "different knotSig.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (l.size() <= 20 && ! reflect) {
                    if (recon.jones() != l.jones()) {
                        std::ostringstream msg;
                        msg << name << ": knotSig reconstruction has "
                            "different Jones polynomial.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": cannot reconstruct from knotSig.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyKnotSig(const Link& l, const char* name) {
            verifyKnotSig(l, true, true, name);
            verifyKnotSig(l, true, false, name);
            verifyKnotSig(l, false, true, name);
            verifyKnotSig(l, false, false, name);
        }

        void verifyKnotSig(const Link& l, bool reflect, bool reverse,
                const char* expect, const char* name) {
            std::string sig = l.knotSig(reflect, reverse);
            if (sig != expect) {
                std::ostringstream msg;
                msg << name << ": knotSig("
                    << (reflect ? 't' : 'f') << ", "
                    << (reverse ? 't' : 'f')
                    << ") is " << sig << ", not " << expect << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void knotSig() {
            verifyKnotSig(unknot0, "Unknot (0 crossings)");
            verifyKnotSig(unknot1, "Unknot (1 crossing)");
            verifyKnotSig(unknot3, "Unknot (3 crossings)");
            verifyKnotSig(unknotMonster, "Monster unknot");
            verifyKnotSig(unknotGordian, "Gordian unknot");
            verifyKnotSig(trefoilLeft, "LH Trefoil");
            verifyKnotSig(trefoilRight, "RH Trefoil");
            verifyKnotSig(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyKnotSig(trefoil_r1x6, "Trefoil with 6 R1s");
            verifyKnotSig(figureEight, "Figure eight");
            verifyKnotSig(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyKnotSig(conway, "Conway knot");
            verifyKnotSig(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyKnotSig(gst, "GST");
            verifyKnotSig(rht_rht, "RH Trefoil # RH Trefoil");
            verifyKnotSig(rht_lht, "RH Trefoil # LH Trefoil");

            verifyKnotSig(trefoilRight, true, true, "dabcabcv-", "RH Trefoil");
            verifyKnotSig(trefoilRight, false, true, "dabcabcv-", "RH Trefoil");
            verifyKnotSig(trefoilLeft, true, true, "dabcabcv-", "LH Trefoil");
            verifyKnotSig(trefoilLeft, false, true, "dabcabcva", "LH Trefoil");

            // A link where all four boolean options give different sigs.
            Link l = Link::fromOrientedGauss(
                "-<6 +>3 -<5 +>2 -<4 -<1 +>1 +>5 -<3 +>6 -<2 +>4");
            verifyKnotSig(l, true,  true,  "gaabcdefbcfedPQ--",
                "Antisymmetric knot");
            verifyKnotSig(l, true,  false, "gaabcdefdcbefPQ--",
                "Antisymmetric knot");
            verifyKnotSig(l, false, true,  "gaabcdefbcfedPQaa",
                "Antisymmetric knot");
            verifyKnotSig(l, false, false, "gaabcdefdcbefPQaa",
                "Antisymmetric knot");
        }

        void verifyDT(const Link& l, bool alpha, const char* name) {
            std::string code = l.dt(alpha);

            try {
                Link recon = Link::fromDT(code);

                if (recon.size() != l.size()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has "
                        "different size.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.countComponents() != l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has "
                        "different number of components.";
                    CPPUNIT_FAIL(msg.str());
                }

                // For prime knots, the only possible ambiguity is reflection.
                if (recon.knotSig(false) != l.knotSig(false))
                    recon.reflect();

                if (recon.knotSig(false) != l.knotSig(false)) {
                    std::ostringstream msg;
                    msg << name <<
                        ": reconstruction has different knot signature.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (l.size() <= 20) {
                    if (recon.homfly() != l.homfly()) {
                        std::ostringstream msg;
                        msg << name << ": reconstruction has "
                            "different HOMFLY-PT polynomial.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": cannot reconstruct from code.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyDT(const Link& l, const char* name) {
            if (l.size() <= 26)
                verifyDT(l, true, name);
            verifyDT(l, false, name);
        }

        void dt() {
            verifyDT(unknot0, "Unknot (0 crossings)");
            verifyDT(unknot1, "Unknot (1 crossing)");
            verifyDT(unknot3, "Unknot (3 crossings)");
            verifyDT(unknotMonster, "Monster unknot");
            verifyDT(unknotGordian, "Gordian unknot");
            verifyDT(trefoilLeft, "LH Trefoil");
            verifyDT(trefoilRight, "RH Trefoil");
            verifyDT(figureEight, "Figure eight");
            verifyDT(conway, "Conway knot");
            verifyDT(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyDT(gst, "GST");
            // Luckily works despite ambiguity with composite knots:
            verifyDT(rht_lht, "RH Trefoil # LH Trefoil");
            // Broken by ambiguity with composite knots:
            // verifyDT(trefoil_r1x2, "Trefoil with 2 R1s");
            // verifyDT(trefoil_r1x6, "Trefoil with 6 R1s");
            // verifyDT(figureEight_r1x2, "Figure eight with 2 R1s");
            // verifyDT(rht_rht, "RH Trefoil # RH Trefoil");
        }

        void verifyGauss(const Link& l, const char* name) {
            std::string code = l.gauss();

            try {
                Link recon = Link::fromGauss(code);

                if (recon.size() != l.size()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has different size.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.countComponents() != l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has "
                        "different number of components.";
                    CPPUNIT_FAIL(msg.str());
                }

                // For prime knots, the only possible ambiguity is reflection.
                if (recon.knotSig(false) != l.knotSig(false))
                    recon.reflect();

                if (recon.knotSig(false) != l.knotSig(false)) {
                    std::ostringstream msg;
                    msg << name <<
                        ": reconstruction has different knot signature.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (l.size() <= 20) {
                    if (recon.homfly() != l.homfly()) {
                        std::ostringstream msg;
                        msg << name << ": reconstruction has "
                            "different HOMFLY-PT polynomial.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": cannot reconstruct from code.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void gauss() {
            verifyGauss(unknot0, "Unknot (0 crossings)");
            verifyGauss(unknot1, "Unknot (1 crossing)");
            verifyGauss(unknot3, "Unknot (3 crossings)");
            verifyGauss(unknotMonster, "Monster unknot");
            verifyGauss(unknotGordian, "Gordian unknot");
            verifyGauss(trefoilLeft, "LH Trefoil");
            verifyGauss(trefoilRight, "RH Trefoil");
            verifyGauss(figureEight, "Figure eight");
            verifyGauss(conway, "Conway knot");
            verifyGauss(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyGauss(gst, "GST");
            // Luckily works despite ambiguity with composite knots:
            verifyGauss(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyGauss(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyGauss(rht_rht, "RH Trefoil # RH Trefoil");
            // Broken by ambiguity with composite knots:
            // verifyGauss(trefoil_r1x6, "Trefoil with 6 R1s");
            // verifyGauss(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void verifyOrientedGauss(const Link& l, const char* name) {
            std::string code = l.orientedGauss();

            try {
                Link recon = Link::fromOrientedGauss(code);

                if (recon.size() != l.size()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has different size.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.countComponents() != l.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has "
                        "different number of components.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.orientedGauss() != code) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has different code.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (l.size() <= 20) {
                    if (recon.homfly() != l.homfly()) {
                        std::ostringstream msg;
                        msg << name << ": reconstruction has "
                            "different HOMFLY-PT polynomial.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": cannot reconstruct from code.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void orientedGauss() {
            verifyOrientedGauss(unknot0, "Unknot (0 crossings)");
            verifyOrientedGauss(unknot1, "Unknot (1 crossing)");
            verifyOrientedGauss(unknot3, "Unknot (3 crossings)");
            verifyOrientedGauss(unknotMonster, "Monster unknot");
            verifyOrientedGauss(unknotGordian, "Gordian unknot");
            verifyOrientedGauss(trefoilLeft, "LH Trefoil");
            verifyOrientedGauss(trefoilRight, "RH Trefoil");
            verifyOrientedGauss(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyOrientedGauss(trefoil_r1x6, "Trefoil with 6 R1s");
            verifyOrientedGauss(figureEight, "Figure eight");
            verifyOrientedGauss(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyOrientedGauss(conway, "Conway knot");
            verifyOrientedGauss(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyOrientedGauss(gst, "GST");
            verifyOrientedGauss(rht_rht, "RH Trefoil # RH Trefoil");
            verifyOrientedGauss(rht_lht, "RH Trefoil # LH Trefoil");
        }

        void verifyPDCode(const Link& link, const Link& expect,
                const char* name) {
            // The PD code will throw away zero-crossing components;
            // the "expect" argument should be the resulting diagram.
            std::string code = link.pd();

            try {
                Link recon = Link::fromPD(code);

                if (recon.size() != expect.size()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has incorrect size.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.countComponents() != expect.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has "
                        "different number of components.";
                    CPPUNIT_FAIL(msg.str());
                }
                for (size_t i = 0; i < recon.countComponents(); ++i) {
                    if (recon.writheOfComponent(i) !=
                            expect.writheOfComponent(i)) {
                        std::ostringstream msg;
                        msg << name << ": reconstruction has "
                            "different writhe for component " << i << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                if (recon.writhe() != expect.writhe()) {
                    std::ostringstream msg;
                    msg << name << ": reconstruction has different writhe.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (recon.linking() != expect.linking()) {
                    std::ostringstream msg;
                    msg << name <<
                        ": reconstruction has different linking number.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (expect.size() <= 20) {
                    if (recon.homfly() != expect.homfly()) {
                        std::ostringstream msg;
                        msg << name << ": reconstruction has "
                            "different HOMFLY-PT polynomial.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": cannot reconstruct from code.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyPDCode(const Link& link, const char* name) {
            verifyPDCode(link, link, name);
        }

        void pdCode() {
            // The empty link:
            verifyPDCode(empty, "Empty");

            // Single-component knots:
            verifyPDCode(unknot1, "Unknot (1 crossing)");
            verifyPDCode(unknot3, "Unknot (3 crossings)");
            verifyPDCode(unknotMonster, "Monster unknot");
            verifyPDCode(unknotGordian, "Gordian unknot");
            verifyPDCode(trefoilLeft, "LH Trefoil");
            verifyPDCode(trefoilRight, "RH Trefoil");
            verifyPDCode(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyPDCode(trefoil_r1x6, "Trefoil with 6 R1s");
            verifyPDCode(figureEight, "Figure eight");
            verifyPDCode(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyPDCode(conway, "Conway knot");
            verifyPDCode(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyPDCode(gst, "GST");
            verifyPDCode(rht_rht, "RH Trefoil # RH Trefoil");
            verifyPDCode(rht_lht, "RH Trefoil # LH Trefoil");

            // Links with multiple components:
            verifyPDCode(unlink2_r2, "Unlink (2 components via R2)");
            verifyPDCode(unlink2_r1r1, "Unlink (2 components via R1+R1)");
            verifyPDCode(hopf, "Hopf link");
            verifyPDCode(whitehead, "Whitehead link");
            verifyPDCode(borromean, "Borromean rings");
            verifyPDCode(trefoil_unknot1,
                "Trefoil U unknot (separate + twist)");
            verifyPDCode(trefoil_unknot_overlap, "Trefoil U unknot (with R2)");
            verifyPDCode(adams6_28, "Adams Fig. 6.28");

            // Cases where the PD code throws away zero-crossing components:
            verifyPDCode(unknot0, empty, "Unknot (0 crossings)");
            verifyPDCode(unlink2_0, empty, "Unlink (2 components)");
            verifyPDCode(unlink3_0, empty, "Unlink (3 components)");
            verifyPDCode(trefoil_unknot0, trefoilRight,
                "Trefoil U unknot (separate)");
        }

        void verifyRewrite(const Link& link, int height, int threads,
                bool track, size_t count, const char* name) {
            // For now, this should only be called for knots that are
            // equivalent to their mirror image.

            size_t tot = 0;
            bool broken = false;

            regina::ProgressTrackerOpen* tracker = nullptr;
            if (track)
                tracker = new regina::ProgressTrackerOpen();

            bool result = link.rewrite(height, threads, tracker,
                    [&tot, &broken, &link](const Link& alt) {
                        ++tot;
                        if (alt.jones() != link.jones()) {
                            broken = true; return true;
                        }
                        return false;
                    });

            if (result != broken) {
                std::ostringstream msg;
                msg << name << ": rewrite() return value differs from "
                    "action return values.";
                CPPUNIT_FAIL(msg.str());
            }
            if (broken) {
                std::ostringstream msg;
                msg << name << ": rewrite() changed the link.";
                CPPUNIT_FAIL(msg.str());
            }
            if (count == 0) {
                std::cerr << name << " -> " << tot << std::endl;
                return;
            }
            if (tot != count) {
                std::ostringstream msg;
                msg << name << ": rewrite() with height "
                    << height << " gave " << tot
                    << " diagram(s) instead of " << count << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyRewrite(const Link& link, int height, size_t count,
                const char* name) {
            // Single-threaded, no tracker:
            verifyRewrite(link, height, 1, false, count, name);
            // Multi-threaded, with and without tracker:
            verifyRewrite(link, height, 2, false, count, name);
            verifyRewrite(link, height, 2, true, count, name);
        }

        void rewrite() {
            // The counts here were computed using Regina 6.0 in
            // single-threaded mode.
            //
            // The expected counts should always be positive, so passing
            // an expected count of 0 will be treated as a request to display
            // the number of triangulations that were actually found.
            //
            verifyRewrite(figureEight, 0, 1, "Figure eight");
            verifyRewrite(figureEight, 1, 8, "Figure eight");
            verifyRewrite(figureEight, 2, 137, "Figure eight");
            verifyRewrite(figureEight, 3, 2401, "Figure eight");
            verifyRewrite(unknot3, 0, 22, "Unknot (3 crossings)");
            verifyRewrite(unknot3, 1, 131, "Unknot (3 crossings)");
            verifyRewrite(unknot3, 2, 998, "Unknot (3 crossings)");
            verifyRewrite(rht_lht, 0, 1, "RH Trefoil # LH Trefoil");
            verifyRewrite(rht_lht, 1, 35, "RH Trefoil # LH Trefoil");
            verifyRewrite(rht_lht, 2, 1131, "RH Trefoil # LH Trefoil");
            verifyRewrite(figureEight_r1x2, 0, 137, "Figure eight with 2 R1s");
            verifyRewrite(figureEight_r1x2, 1, 2401, "Figure eight with 2 R1s");
            verifyRewrite(figureEight_r1x2, 2, 44985, "Figure eight with 2 R1s");
        }

        void swapping() {
            Link a = ExampleLink::trefoilLeft();
            Link b = ExampleLink::figureEight();

            a.jones();
            b.jones();

            swap(a, b);

            if (a.size() != 4) {
                CPPUNIT_FAIL("swap() did not swap crossings correctly.");
            }
            if (a.jones() != figureEight.jones()) {
                CPPUNIT_FAIL("swap() did not swap properties correctly.");
            }

            std::iter_swap(&a, &b);

            if (a.size() != 3) {
                CPPUNIT_FAIL(
                    "std::iter_swap() did not swap crossings correctly.");
            }
            if (a.jones() != trefoilLeft.jones()) {
                CPPUNIT_FAIL(
                    "std::iter_swap() did not swap properties correctly.");
            }
        }

        template <int index>
        std::vector<std::string> covers(const regina::GroupPresentation& g) {
            std::vector<std::string> ans;
            g.enumerateCovers<index>([&](const regina::GroupPresentation& c) {
                ans.push_back(c.abelianisation().str());
            });

            std::sort(ans.begin(), ans.end());
            return ans;
        }

        bool lookIsomorphic(const regina::GroupPresentation& a,
                const regina::GroupPresentation& b) {
            // Here we assume that both groups have been simplified.

            // For trivial, cyclic or free groups, we expect the
            // presentations to be identical.
            if (a.countGenerators() == 0 || b.countGenerators() == 0)
                return a.countGenerators() == b.countGenerators();

            if (a.countGenerators() == 1 || b.countGenerators() == 1)
                return a.countGenerators() == b.countGenerators() &&
                    a.relations() == b.relations();

            if (a.countRelations() == 0 || b.countRelations() == 0)
                return a.countRelations() == b.countRelations() &&
                    a.countGenerators() == b.countGenerators();

            // Both groups have >= 2 generators and >= 1 relation.

            // Check the abelian invariants and some low-index covers.
            if (a.abelianisation() != b.abelianisation())
                return false;
            if (covers<2>(a) != covers<2>(b))
                return false;
            if (covers<3>(a) != covers<3>(b))
                return false;
            if (covers<4>(a) != covers<4>(b))
                return false;
            if (covers<5>(a) != covers<5>(b))
                return false;

            return true;
        }

        void verifyGroup(const Link& link, const char* name) {
            regina::GroupPresentation fromLink = link.group();
            regina::GroupPresentation fromComp =
                link.complement().fundamentalGroup();

            if (! lookIsomorphic(fromLink, fromComp)) {
                std::ostringstream msg;
                msg << name << ": link group does not appear "
                    "isomorphic to the complement's fundamental group.\n"
                    << fromLink.detail() << fromComp.detail() << std::endl;
                CPPUNIT_FAIL(msg.str());
            }

            if (fromLink.abelianRank() != link.countComponents()) {
                std::ostringstream msg;
                msg << name << ": link group has abelian rank "
                    << fromLink.abelianRank() << " instead of the "
                    "expected " << link.countComponents() << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }

        void group() {
            // The empty link:
            verifyGroup(empty, "Empty");

            // Single-component knots:
            verifyGroup(unknot0, "Unknot (0 crossings)");
            verifyGroup(unknot1, "Unknot (1 crossing)");
            verifyGroup(unknot3, "Unknot (3 crossings)");
            verifyGroup(unknotMonster, "Monster unknot");
            // verifyGroup(unknotGordian, "Gordian unknot");
            verifyGroup(trefoilLeft, "LH Trefoil");
            verifyGroup(trefoilRight, "RH Trefoil");
            verifyGroup(trefoil_r1x2, "Trefoil with 2 R1s");
            verifyGroup(trefoil_r1x6, "Trefoil with 6 R1s");
            verifyGroup(figureEight, "Figure eight");
            verifyGroup(figureEight_r1x2, "Figure eight with 2 R1s");
            verifyGroup(conway, "Conway knot");
            verifyGroup(kinoshitaTerasaka, "Kinoshita-Terasaka knot");
            verifyGroup(gst, "GST");
            verifyGroup(rht_rht, "RH Trefoil # RH Trefoil");
            verifyGroup(rht_lht, "RH Trefoil # LH Trefoil");

            // Links with multiple components:
            verifyGroup(unlink2_0, "Unlink (2 components)");
            verifyGroup(unlink3_0, "Unlink (3 components)");
            verifyGroup(unlink2_r2, "Unlink (2 components via R2)");
            verifyGroup(unlink2_r1r1, "Unlink (2 components via R1+R1)");
            verifyGroup(hopf, "Hopf link");
            verifyGroup(whitehead, "Whitehead link");
            verifyGroup(borromean, "Borromean rings");
            verifyGroup(trefoil_unknot0, "Trefoil U unknot (separate)");
            verifyGroup(trefoil_unknot1, "Trefoil U unknot (separate + twist)");
            verifyGroup(trefoil_unknot_overlap, "Trefoil U unknot (with R2)");
            verifyGroup(adams6_28, "Adams Fig. 6.28");
        }
};

void addLink(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkTest::suite());
}

