
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cppunit/extensions/HelperMacros.h>
#include "angle/nanglestructurelist.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/angle/testangle.h"

using regina::NAngleStructure;
using regina::NAngleStructureList;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTriangulation;

class NAngleStructureListTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NAngleStructureListTest);

    CPPUNIT_TEST(empty);
    CPPUNIT_TEST(gieseking);
    CPPUNIT_TEST(figure8);
    CPPUNIT_TEST(loopC2);

    CPPUNIT_TEST_SUITE_END();

    private:
        NTriangulation triEmpty;
            /**< An empty triangulation. */
        NTriangulation triGieseking;
            /**< The Gieseking manifold. */
        NTriangulation triFigure8;
            /**< The figure eight knot complement. */
        NTriangulation triLoopC2;
            /**< An untwisted layered loop of length 2. */

    public:
        void setUp() {
            NTetrahedron* r;
            NTetrahedron* s;

            // The two-tetrahedron figure eight knot complement is
            // described at the beginning of chapter 8 of Richard
            // Rannard's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            r->joinTo(0, s, NPerm(1, 3, 0, 2));
            r->joinTo(1, s, NPerm(2, 0, 3, 1));
            r->joinTo(2, s, NPerm(0, 3, 2, 1));
            r->joinTo(3, s, NPerm(2, 1, 0, 3));
            triFigure8.addTetrahedron(r);
            triFigure8.addTetrahedron(s);

            // The Gieseking manifold is simple enough; it has only one
            // tetrahedron.
            r = new NTetrahedron();
            r->joinTo(0, r, NPerm(1, 2, 0, 3));
            r->joinTo(2, r, NPerm(0, 2, 3, 1));
            triGieseking.addTetrahedron(r);

            // Layered loops can be constructed automatically.
            triLoopC2.insertLayeredLoop(2, false);
        }

        void tearDown() {
        }

        void testSize(NAngleStructureList* list, const char* triName,
                unsigned long expectedSize,
                bool allowStrict, bool allowTaut) {
            {
                std::ostringstream msg;
                msg << "Number of angle structures for " << triName
                    << " should be " << expectedSize << ", not "
                    << list->getNumberOfStructures() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->getNumberOfStructures() == expectedSize);
            }
            {
                std::ostringstream msg;
                msg << "Angle structures for " << triName
                    << " should ";
                if (! allowStrict)
                    msg << "not ";
                msg << "support strict angle structures.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->allowsStrict() == allowStrict);
            }
            {
                std::ostringstream msg;
                msg << "Angle structures for " << triName
                    << " should ";
                if (! allowTaut)
                    msg << "not ";
                msg << "support taut angle structures.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->allowsTaut() == allowTaut);
            }
        }

        void countStructures(const NAngleStructureList* list,
                const char* triName, unsigned long expectedCount,
                bool strict, bool taut) {
            unsigned long tot = 0;
            unsigned long size = list->getNumberOfStructures();

            const NAngleStructure* s;
            for (unsigned long i = 0; i < size; i++) {
                s = list->getStructure(i);

                if (s->isStrict() == strict && s->isTaut() == taut)
                    tot++;
            }

            std::ostringstream msg;
            msg << "Number of ";
            if (strict)
                msg << "strict ";
            else if (taut)
                msg << "taut ";
            else
                msg << "generic ";
            msg << "angle structures on " << triName << " should be "
                << expectedCount << ", not " << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        void empty() {
            NAngleStructureList* list = NAngleStructureList::enumerate(
                &triEmpty);

            testSize(list, "the empty triangulation", 1, true, true);

            delete list;
        }

        void gieseking() {
            NAngleStructureList* list = NAngleStructureList::enumerate(
                &triGieseking);

            testSize(list, "the Gieseking manifold", 3, true, true);
            countStructures(list, "the Gieseking manifold", 3,
                false /* strict */, true /* taut */);

            delete list;
        }

        void figure8() {
            NAngleStructureList* list = NAngleStructureList::enumerate(
                &triFigure8);

            testSize(list, "the figure eight knot complement", 5, true, true);
            countStructures(list, "the figure eight knot complement", 3,
                false /* strict */, true /* taut */);
            countStructures(list, "the figure eight knot complement", 2,
                false /* strict */, false /* taut */);

            delete list;
        }

        void loopC2() {
            NAngleStructureList* list = NAngleStructureList::enumerate(
                &triLoopC2);

            testSize(list, "the untwisted layered loop C(2)", 0, false, false);

            delete list;
        }
};

void addNAngleStructureList(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NAngleStructureListTest::suite());
}

