
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
#include "maths/numbertheory.h"

class NumberTheoryTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NumberTheoryTest);

    CPPUNIT_TEST(reducedMod);

    CPPUNIT_TEST_SUITE_END();

    private:
        long lEvenLarge;
            /**< Large even number; twice this fits in a signed long. */
        long lOddLarge;
            /**< Large odd number; twice this fits in a signed long. */
        long lEvenMed;
            /**< Medium even number; this squared fits in a signed long. */
        long lOddMed;
            /**< Medium odd number; this squared fits in a signed long. */

    public:
        void setUp() {
            lEvenLarge = 1000000000;
            lOddLarge = 1000000001;
            lEvenMed = 40000;
            lOddMed = 40001;
        }

        void tearDown() {
        }

        void reducedMod() {
            // 0 mod n:
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(0, 1));
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(0, lEvenLarge));

            // n mod 1:
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(1, 1));
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(-1, 1));
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(lEvenLarge, 1));
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(-lEvenLarge, 1));

            // Halfway tests:
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(16, 2));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::reducedMod(17, 2));
            CPPUNIT_ASSERT_EQUAL((long)0, regina::reducedMod(-16, 2));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::reducedMod(-17, 2));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::reducedMod(16, 3));
            CPPUNIT_ASSERT_EQUAL((long)(-1), regina::reducedMod(17, 3));
            CPPUNIT_ASSERT_EQUAL((long)(-1), regina::reducedMod(-16, 3));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::reducedMod(-17, 3));

            long lEvenMod = lEvenMed * 2;
            long lEvenHalf = (lEvenMed * lEvenMed) - lEvenMed;
            long lEvenAnsHalf = lEvenMed;
            long lOddMod = lOddMed;
            long lOddBelowHalf = (lOddMed * lOddMed) - ((lOddMed + 1) / 2);
            long lOddAnsBelowHalf = lOddMed / 2;
            CPPUNIT_ASSERT_EQUAL(lEvenAnsHalf - 1,
                regina::reducedMod(lEvenHalf - 1, lEvenMod));
            CPPUNIT_ASSERT_EQUAL(lEvenAnsHalf,
                regina::reducedMod(lEvenHalf, lEvenMod));
            CPPUNIT_ASSERT_EQUAL(-(lEvenAnsHalf - 1),
                regina::reducedMod(lEvenHalf + 1, lEvenMod));
            CPPUNIT_ASSERT_EQUAL(-(lEvenAnsHalf - 1),
                regina::reducedMod(-(lEvenHalf - 1), lEvenMod));
            CPPUNIT_ASSERT_EQUAL(lEvenAnsHalf,
                regina::reducedMod(-lEvenHalf, lEvenMod));
            CPPUNIT_ASSERT_EQUAL(lEvenAnsHalf - 1,
                regina::reducedMod(-(lEvenHalf + 1), lEvenMod));
            CPPUNIT_ASSERT_EQUAL(lOddAnsBelowHalf,
                regina::reducedMod(lOddBelowHalf, lOddMod));
            CPPUNIT_ASSERT_EQUAL(-lOddAnsBelowHalf,
                regina::reducedMod(lOddBelowHalf + 1, lOddMod));
            CPPUNIT_ASSERT_EQUAL(-lOddAnsBelowHalf,
                regina::reducedMod(-lOddBelowHalf, lOddMod));
            CPPUNIT_ASSERT_EQUAL(lOddAnsBelowHalf,
                regina::reducedMod(-(lOddBelowHalf + 1), lOddMod));

            // Examples from documentation:
            CPPUNIT_ASSERT_EQUAL((long)4, regina::reducedMod(4, 10));
            CPPUNIT_ASSERT_EQUAL((long)(-4), regina::reducedMod(6, 10));
        }
};

CPPUNIT_TEST_SUITE_REGISTRATION(NumberTheoryTest);

