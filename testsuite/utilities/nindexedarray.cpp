
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/census/testcensus.h"
#include "utilities/hashutils.h"
#include "utilities/nindexedarray.h"

class NIndexedArrayTest : public CppUnit::TestFixture {
    public:
        typedef regina::NIndexedArray<int*, regina::HashPointer> Array;

    CPPUNIT_TEST_SUITE(NIndexedArrayTest);

    // CPPUNIT_TEST();

    CPPUNIT_TEST_SUITE_END();

    private:
        int nLarge;
            /**< The number of unique elements we will be using in
                 our large arrays. */
        int nSmall;
            /**< The number of unique elements we will be using in
                 our small arrays. */
        int* value;
            /**< A list of nLarge integers, where value[i] == i.
                 Note that we will actually store the consecutive memory
                 addresses of these integers in the various arrays. */

        Array largeUniqueArray;
            /**< A large array in which no element appears twice.
                 This will contain the value list in order. */
        Array largeMultiArray;
            /**< A large array in which each element appears several times.
                 This will contain the value list forwards, then backwards,
                 then forwards with each element used three times
                 consecutively, coming to a grand total of 5 * nLarge
                 elements. */
        Array smallUniqueArray;
            /**< A small array in which no element appears twice.
                 This will contain the value list in order. */
        Array smallMultiArray;
            /**< A small array in which each element appears several times.
                 This will contain the value list forwards, then backwards,
                 then forwards with each element used three times
                 consecutively, coming to a grand total of 5 * nSmall
                 elements. */
        Array emptyArray;
            /**< An array containing no elements at all. */

    public:
        void setUp() {
            int i;

            nLarge = 1000;
            nSmall = 3;

            value = new int[nLarge];
            for (i = 0; i < nLarge; i++)
                value[i] = i;

            for (i = 0; i < nLarge; i++)
                largeUniqueArray.push_back(value + i);

            for (i = 0; i < nLarge; i++)
                largeMultiArray.push_back(value + i);
            for (i = 0; i < nLarge; i++)
                largeMultiArray.push_back(value + nLarge - i - 1);
            for (i = 0; i < nLarge; i++) {
                largeMultiArray.push_back(value + i);
                largeMultiArray.push_back(value + i);
                largeMultiArray.push_back(value + i);
            }

            for (i = 0; i < nSmall; i++)
                smallUniqueArray.push_back(value + i);

            for (i = 0; i < nSmall; i++)
                smallMultiArray.push_back(value + i);
            for (i = 0; i < nSmall; i++)
                smallMultiArray.push_back(value + nSmall - i - 1);
            for (i = 0; i < nSmall; i++) {
                smallMultiArray.push_back(value + i);
                smallMultiArray.push_back(value + i);
                smallMultiArray.push_back(value + i);
            }
        }

        void tearDown() {
            delete[] value;
        }
};

void addNIndexedArray(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NIndexedArrayTest::suite());
}

