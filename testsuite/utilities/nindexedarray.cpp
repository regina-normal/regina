
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

    CPPUNIT_TEST(basicChecks);
    CPPUNIT_TEST(constructors);
    CPPUNIT_TEST(swap);
    CPPUNIT_TEST(queries);

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

        static void arrayAssert(const char* arrayName, const char* msg,
                bool condition) {
            std::string realMsg("Array ");
            realMsg += arrayName;
            realMsg += ": ";
            realMsg += msg;
            CPPUNIT_ASSERT_MESSAGE(realMsg, condition);
        }

        static void validate(const Array& array, const char* arrayName) {
            arrayAssert(arrayName, "Failed to validate.", array.validate());
        }

        static void compare(const Array& array1, const Array& array2,
                const char* name1, const char* name2) {
            std::string realMsg("Arrays ");
            realMsg += name1;
            realMsg += " and ";
            realMsg += name2;
            realMsg += " are not identical.";

            Array::const_iterator it1 = array1.begin();
            Array::const_iterator it2 = array2.begin();

            bool ok = true;
            while (ok && it1 != array1.end() && it2 != array2.end()) {
                if (*it1 != *it2)
                    ok = false;
                it1++;
                it2++;
            }
            if (it1 != array1.end() || it2 != array2.end())
                ok = false;

            CPPUNIT_ASSERT_MESSAGE(realMsg, ok);
        }

        int* expectedElement(const Array& array,
                Array::size_type index) const {
            const Array* address = &array;

            int nUse;
            if (address == &smallUniqueArray || address == &smallMultiArray)
                nUse = nSmall;
            else if (address == &largeUniqueArray ||
                    address == &largeMultiArray)
                nUse = nLarge;
            else
                nUse = 0;

            if (address == &smallUniqueArray || address == &largeUniqueArray) {
                if (index < nUse)
                    return value + index;
            } else if (address == &smallMultiArray ||
                    address == &largeMultiArray) {
                if (index < nUse)
                    return value + index;
                index -= nUse;
                if (index < nUse)
                    return value + nUse - index - 1;
                index -= nUse;
                if (index < 3 * nUse)
                    return value + (index / 3);
            }

            return 0;
        }

        void basicChecks(const Array& array, const char* arrayName) const {
            // Validate before we start.
            validate(array, arrayName);

            // Check front, back and emptiness test.
            if (array.empty()) {
                arrayAssert(arrayName, "Empty but non-zero size.",
                    array.size() == 0);
            } else {
                arrayAssert(arrayName, "Non-empty but zero size.",
                    array.size() != 0);
                arrayAssert(arrayName, "Inconsistent front member.",
                    array.front() == *(array.begin()));
                arrayAssert(arrayName, "Inconsistent back member.",
                    array.back() == *(array.rbegin()));
            }

            // Check forward iterators.
            Array::size_type i = 0;
            for (Array::const_iterator it = array.begin();
                    it != array.end(); it++) {
                arrayAssert(arrayName,
                    "Mismatch between forward iterators and direct indexing.",
                    array[i] == *it);
                arrayAssert(arrayName,
                    "Incorrect array element found during forward iteration.",
                    *it == expectedElement(array, i));
                i++;
            }
            arrayAssert(arrayName,
                "Forward iterators do not cover exact array size.",
                i == array.size());

            // Check reverse iterators.
            i = array.size();
            for (Array::const_reverse_iterator it = array.rbegin();
                    it != array.rend(); it++) {
                arrayAssert(arrayName,
                    "Reverse iterators do not cover exact array size.",
                    i != 0);
                i--;
                arrayAssert(arrayName,
                    "Mismatch between reverse iterators and direct indexing.",
                    array[i] == *it);
                arrayAssert(arrayName,
                    "Incorrect array element found during reverse iteration.",
                    *it == expectedElement(array, i));
            }
            arrayAssert(arrayName,
                "Reverse iterators do not cover exact array size.",
                i == 0);
        }

        void basicChecks() {
            basicChecks(largeUniqueArray, "largeUniqueArray");
            basicChecks(largeMultiArray, "largeMultiArray");
            basicChecks(smallUniqueArray, "smallUniqueArray");
            basicChecks(smallMultiArray, "smallMultiArray");
            basicChecks(emptyArray, "emptyArray");
        }

        void constructors(const Array& array, const char* arrayName) const {
            Array clone(array);
            validate(clone, "new clone");
            compare(array, clone, arrayName, "new clone");

            Array copy(smallMultiArray);
            copy.push_back(value);
            copy = array;
            validate(copy, "assigned copy");
            compare(array, copy, arrayName, "assigned copy");
        }

        void constructors() {
            constructors(largeUniqueArray, "largeUniqueArray");
            constructors(largeMultiArray, "largeMultiArray");
            constructors(smallUniqueArray, "smallUniqueArray");
            constructors(smallMultiArray, "smallMultiArray");
            constructors(emptyArray, "emptyArray");

            Array fixedSize(nSmall);
            validate(fixedSize, "new small fixed size");
            arrayAssert("new small fixed size", "Has incorrect size.",
                fixedSize.size() == nSmall);

            Array manyObjects(nLarge, value);
            validate(manyObjects, "new many identical elements");
            arrayAssert("new many identical elements", "Has incorrect size.",
                manyObjects.size() == nLarge);
            for (Array::const_iterator it = manyObjects.begin();
                    it != manyObjects.end(); it++)
                arrayAssert("new many identical elements",
                    "Contains incorrect element.", *it == value);
        }

        void swap() {
            Array largeUniqueClone(largeUniqueArray);
            Array smallMultiClone(smallMultiArray);

            largeUniqueClone.swap(smallMultiClone);

            validate(smallMultiClone, "swapped smallMultiClone");
            validate(largeUniqueClone, "swapped largeUniqueClone");
            compare(smallMultiClone, largeUniqueArray,
                "swapped smallMultiClone", "largeUniqueArray");
            compare(largeUniqueClone, smallMultiArray,
                "swapped largeUniqueClone", "smallMultiArray");
        }

        void inserts() {
            // TODO: push_back, insert elt, insert chunk, insert n copies
        }

        void erasures() {
            // TODO: pop_back, erase elt, erase chunk, erase all copies
            // TODO: clear
        }

        void resizes() {
            // TODO: grow, shrink
        }

        void queries(const Array& array, const char* arrayName) const {
            Array::difference_type index;

            for (Array::const_iterator it = array.begin();
                    it != array.end(); it++) {
                index = array.index(*it);
                arrayAssert(arrayName,
                    "Returned out-of-bounds index from array element search.",
                    index >= 0 && index < array.size());
                arrayAssert(arrayName,
                    "Returned incorrect index from array element search.",
                    array[index] == *it);
            }

            arrayAssert(arrayName,
                "Returned incorrect index from non-existent element search.",
                array.index(0) == -1);
        }

        void queries() {
            queries(largeUniqueArray, "largeUniqueArray");
            queries(largeMultiArray, "largeMultiArray");
            queries(smallUniqueArray, "smallUniqueArray");
            queries(smallMultiArray, "smallMultiArray");
            queries(emptyArray, "emptyArray");
        }
};

void addNIndexedArray(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NIndexedArrayTest::suite());
}

