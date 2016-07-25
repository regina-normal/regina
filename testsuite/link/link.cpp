
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#include "link/examplelink.h"
#include "link/link.h"
#include "maths/nlaurent.h"
#include "triangulation/ntriangulation.h"

#include "testsuite/link/testlink.h"

using regina::ExampleLink;
using regina::Link;
using regina::NTriangulation;

class LinkTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkTest);

    CPPUNIT_TEST(components);
    CPPUNIT_TEST(jones);
    CPPUNIT_TEST(complement);

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
            unlink2_r2->setLabel("Unlink (2 components via R1+R1)");

            hopf = ExampleLink::hopf();

            whitehead = ExampleLink::whitehead();

            borromean = ExampleLink::borromean();

            // Um.  How to pass an empty initialiser list that lets the
            // compiler deduce the type?
            // trefoil_unknot0 = Link::fromData(
            //     { +1, +1, +1 }, { 1, -2, 3, -1, 2, -3 }, { });
            trefoil_unknot0 = Link::fromJenkins(
                "2   6 0 1 1 -1 2 1 0 -1 1 1 2 -1   0   0 1 1 1 2 1");
            trefoil_unknot0->setLabel("Trefoil U unknot (separate)");

            trefoil_unknot1 = Link::fromData(
                { +1, +1, -1, +1 }, { 1, -2, 4, -1, 2, -4 }, { -3, 3 });
            trefoil_unknot1->setLabel("Trefoil U unknot (separate + twist)");

            trefoil_unknot_overlap = Link::fromData(
                { -1, +1, +1, +1, +1 }, { 2, -3, -4, -1, 5, -2, 3, -5 },
                { 4, 1 });
            trefoil_unknot_overlap->setLabel("Trefoil U unknot (with R2)");
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
            regina::NLaurent<regina::NInteger>* found = l->jones();
            s << *found;
            delete found;

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

        void testComplementBasic(Link* l) {
            NTriangulation* c = l->complement();

            if (c->countComponents() != 1) {
                std::ostringstream msg;
                msg << l->label() << " complement: expected 1 component, "
                    "found " << c->countComponents() << ".";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }

            size_t ideal = 0;
            for (auto v : c->vertices()) {
                regina::NVertex::LinkType t = v->link();
                if (t == regina::NVertex::TORUS)
                    ++ideal;
                else if (t != regina::NVertex::SPHERE) {
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
            NTriangulation* c = l->complement();
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
            NTriangulation* c = l->complement();
            if (! c->isSolidTorus()) {
                std::ostringstream msg;
                msg << l->label() << " complement: not a solid torus "
                    "as expected.";
                delete c;
                CPPUNIT_FAIL(msg.str());
            }
            delete c;
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

            // TODO: Test hyperbolicity for Whitehead & Borromean.
            // TODO: Algebraic tests for the separable links.
        }
};

void addLink(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkTest::suite());
}

