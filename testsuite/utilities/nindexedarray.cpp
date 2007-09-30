
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/hashutils.h"
#include "utilities/nindexedarray.h"

class NIndexedArrayTest : public CppUnit::TestFixture {
    public:
        typedef regina::NIndexedArray<int*, regina::HashPointer> Array;

    CPPUNIT_TEST_SUITE(NIndexedArrayTest);

    CPPUNIT_TEST(basicChecks);
    CPPUNIT_TEST(constructors);
    CPPUNIT_TEST(swap);
    CPPUNIT_TEST(inserts);
    CPPUNIT_TEST(erasures);
    CPPUNIT_TEST(resizes);
    CPPUNIT_TEST(queries);
    CPPUNIT_TEST(comparisons);

    CPPUNIT_TEST_SUITE_END();

    private:
        unsigned nExtra;
            /**< Slightly larger than the number of unique elements we
                 will be using in our large arrays. */
        unsigned nLarge;
            /**< The number of unique elements we will be using in
                 our large arrays. */
        unsigned nSmall;
            /**< The number of unique elements we will be using in
                 our small arrays. */
        int* value;
            /**< A list of nExtra integers, where value[i] == i.
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
            unsigned i;

            nExtra = 1010;
            nLarge = 1000;
            nSmall = 3;

            value = new int[nExtra];
            for (i = 0; i < nExtra; i++)
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

            realMsg = "Arrays ";
            realMsg += name1;
            realMsg += " and ";
            realMsg += name2;
            realMsg += " report different sizes.";

            CPPUNIT_ASSERT_MESSAGE(realMsg, array1.size() == array2.size());
        }

        int* expectedElement(const Array& array,
                Array::size_type index) const {
            const Array* address = &array;

            unsigned nUse;
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
            arrayAssert(arrayName,
                "Equality test (==) gave false negative for clone.",
                array == clone);

            Array copy(smallMultiArray);
            copy.push_back(value);
            copy = array;
            validate(copy, "assigned copy");
            compare(array, copy, arrayName, "assigned copy");
            arrayAssert(arrayName,
                "Equality test (==) gave false negative for copy.",
                array == copy);
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

        void inserts(const Array& multi, const Array& unique,
                const char* multiName) const {
            // Make a few tries at building multi from empty or unique.
            // Note that push_back() was already tested when we
            // initially constructed the member arrays.
            Array::const_reverse_iterator rit;
            unsigned i;

            Array::size_type blockSize = unique.size();

            Array try1;
            // Copies at end, front and middle:
            try1.insert(try1.end(), 3, unique.back());
            validate(try1, "inserted duplicates at end");
            try1.insert(try1.begin(), 3, unique.front());
            validate(try1, "inserted duplicates at front");
            for (rit = unique.rbegin() + 1; rit + 1 != unique.rend(); rit++) {
                try1.insert(try1.begin() + 3, 3, *rit);
                validate(try1, "inserted duplicates in middle");
            }
            // Block at front:
            try1.insert(try1.begin(), unique.begin(), unique.end());
            validate(try1, "inserted block at front");
            // Block in middle:
            try1.insert(try1.begin() + blockSize,
                unique.rbegin(), unique.rend());
            validate(try1, "inserted block in middle");

            compare(try1, multi, "first cloning attempt by insertion",
                multiName);

            Array try2(unique.begin() + 1, unique.end());
            // Element at front:
            try2.insert(try2.begin(), unique.front());
            validate(try2, "inserted element at front");
            // Block at end:
            try2.insert(try2.end(), unique.rbegin(), unique.rend());
            validate(try2, "inserted block at end");
            // Element at end and middle:
            for (rit = unique.rbegin(); rit != unique.rend(); rit++)
                for (i = 0; i < 3; i++) {
                    try2.insert(try2.begin() + blockSize + blockSize, *rit);
                    validate(try2, "inserted element at middle/end");
                }

            compare(try2, multi, "second cloning attempt by insertion",
                multiName);
        }

        void inserts() {
            inserts(largeMultiArray, largeUniqueArray, "largeMultiArray");
            inserts(smallMultiArray, smallUniqueArray, "smallMultiArray");
        }

        void erasures(const Array& multi, const Array& unique,
                const char* multiName, const char* uniqueName) const {
            // Make a few tries at building unique from multi.
            unsigned i;

            Array::size_type blockSize = unique.size();

            Array try1(multi);
            // Block from middle:
            try1.erase(try1.begin() + (2 * blockSize),
                try1.begin() + (3 * blockSize));
            validate(try1, "erased block from middle");
            arrayAssert(multiName,
                "Erased block from middle reports incorrect size.",
                try1.size() == 4 * blockSize);
            // Block from end:
            try1.erase(try1.begin() + (3 * blockSize), try1.end());
            validate(try1, "erased block from end");
            arrayAssert(multiName,
                "Erased block from end reports incorrect size.",
                try1.size() == 3 * blockSize);
            // Element from beginning:
            try1.erase(try1.begin());
            validate(try1, "erased element from beginning");
            arrayAssert(multiName,
                "Erased element from beginning reports incorrect size.",
                try1.size() == 3 * blockSize - 1);
            // Block from beginning:
            try1.erase(try1.begin(), try1.begin() + blockSize - 1);
            validate(try1, "erased block from beginning");
            arrayAssert(multiName,
                "Erased block from beginning reports incorrect size.",
                try1.size() == 2 * blockSize);
            // Pop from end:
            for (i = 0; i < 3; i++)
                try1.pop_back();
            validate(try1, "popped elements from end");
            arrayAssert(multiName,
                "Popped elements from end reports incorrect size.",
                try1.size() == 2 * blockSize - 3);
            // Element from middle and end:
            for (i = 0; i + 3 < blockSize; i++) {
                try1.erase(try1.begin() + blockSize);
                validate(try1, "erased element from middle/end");
                arrayAssert(multiName,
                    "Erased element from middle/end reports incorrect size.",
                    try1.size() == 2 * blockSize - 3 - i - 1);
            }

            compare(try1, Array(unique.rbegin(), unique.rend()),
                "cloning attempt by erasure", uniqueName);
        }

        void eraseByValue(const Array& large, const Array& small,
                const char* smallName) const {
            // Test erasing all copies of a given element.
            Array eraseTest(large);
            for (unsigned i = nSmall; i < nExtra; i++)
                eraseTest.erase(value + i);

            validate(eraseTest, "erase-by-value test");
            compare(eraseTest, small, "erase-by-value test", smallName);
        }

        void erasures() {
            erasures(largeMultiArray, largeUniqueArray,
                "largeMultiArray", "largeUniqueArray");
            erasures(smallMultiArray, smallUniqueArray,
                "smallMultiArray", "smallUniqueArray");
            eraseByValue(largeMultiArray, smallMultiArray,
                "smallMultiArray");
            eraseByValue(largeUniqueArray, smallUniqueArray,
                "smallUniqueArray");
        }

        void grow(const Array& multi, const Array& unique,
                const char* multiName, const char* uniqueName) const {
            Array cloneDefault(unique);
            Array cloneSpecific(unique);
            cloneDefault.resize(multi.size());
            cloneSpecific.resize(multi.size(), value);
            validate(cloneDefault,
                "expanded unique clone with default values");
            validate(cloneSpecific,
                "expanded unique clone with specific values");

            arrayAssert(uniqueName,
                "Default expanded clone returns incorrect size.",
                multi.size() == cloneDefault.size());
            arrayAssert(uniqueName,
                "Specific expanded clone returns incorrect size.",
                multi.size() == cloneSpecific.size());

            Array::const_iterator itDefault = cloneDefault.begin();
            Array::const_iterator itSpecific = cloneSpecific.begin();
            Array::size_type pos = 0;
            Array::value_type expected;
            while (itDefault != cloneDefault.end() &&
                    itSpecific != cloneSpecific.end()) {
                expected = expectedElement(unique, pos);
                if (expected) {
                    arrayAssert(uniqueName,
                        "Incorrect old element in default expanded clone.",
                        *itDefault == expected);
                    arrayAssert(uniqueName,
                        "Incorrect old element in specific expanded clone.",
                        *itSpecific == expected);
                } else {
                    arrayAssert(uniqueName,
                        "Incorrect new element in specific expanded clone.",
                        *itSpecific == value);
                }
                pos++;
                itDefault++;
                itSpecific++;
            }
            arrayAssert(uniqueName,
                "Default and specific expanded clones have different sizes.",
                itDefault == cloneDefault.end() &&
                itSpecific == cloneSpecific.end());
            arrayAssert(uniqueName,
                "Default and specific expanded clones have incorrect sizes.",
                pos == multi.size());
            
            arrayAssert(multiName,
                "Test == with default expanded clone gives false positive.",
                ! (multi == cloneDefault));
            arrayAssert(multiName,
                "Test == with specific expanded clone gives false positive.",
                ! (multi == cloneSpecific));
            arrayAssert(uniqueName,
                "Default and specific expanded clones test equal (==).",
                ! (cloneDefault == cloneSpecific));
        }

        void shrink(const Array& multi, const Array& unique,
                const char* uniqueName) const {
            Array clone(multi);
            clone.resize(unique.size());
            validate(clone, "shrunken multi clone");
            arrayAssert(uniqueName,
                "Shrunken multi clone returns incorrect size.",
                clone.size() == unique.size());
            compare(clone, unique, "shrunken multi clone", uniqueName);

            clone.clear();
            validate(clone, "emptied multi clone");
            arrayAssert(uniqueName,
                "Emptied multi clone returns incorrect size.",
                (clone.size() == 0) && clone.empty());
            compare(clone, emptyArray, "emptied multi clone", "emptyArray");
        }

        void resizes() {
            grow(largeMultiArray, largeUniqueArray,
                "largeMultiArray", "largeUniqueArray");
            grow(smallMultiArray, smallUniqueArray,
                "smallMultiArray", "smallUniqueArray");

            shrink(largeMultiArray, largeUniqueArray, "largeUniqueArray");
            shrink(smallMultiArray, smallUniqueArray, "smallUniqueArray");
        }

        void queries(const Array& array, const char* arrayName) const {
            Array::difference_type index;

            for (Array::const_iterator it = array.begin();
                    it != array.end(); it++) {
                index = array.index(*it);
                arrayAssert(arrayName,
                    "Returned out-of-bounds index from array element search.",
                    index >= 0 && index < (Array::difference_type)array.size());
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

        void comparisons() {
            // Note that we test == for false negatives during the
            // constructor tests.

            CPPUNIT_ASSERT_MESSAGE(
                "Test largeUniqueArray == largeMultiArray gave false positive.",
                ! (largeUniqueArray == largeMultiArray));
            CPPUNIT_ASSERT_MESSAGE(
                "Test smallUniqueArray == smallMultiArray gave false positive.",
                ! (smallUniqueArray == smallMultiArray));
            CPPUNIT_ASSERT_MESSAGE(
                "Test largeUniqueArray < largeMultiArray gave false negative.",
                largeUniqueArray < largeMultiArray);
            CPPUNIT_ASSERT_MESSAGE(
                "Test smallUniqueArray < smallMultiArray gave false negative.",
                smallUniqueArray < smallMultiArray);
            CPPUNIT_ASSERT_MESSAGE(
                "Test largeMultiArray < largeUniqueArray gave false positive.",
                ! (largeMultiArray < largeUniqueArray));
            CPPUNIT_ASSERT_MESSAGE(
                "Test smallMultiArray < smallUniqueArray gave false positive.",
                ! (smallMultiArray < smallUniqueArray));
            CPPUNIT_ASSERT_MESSAGE(
                "Test smallMultiArray < largeUniqueArray gave false negative.",
                smallMultiArray < largeUniqueArray);
        }
};

void addNIndexedArray(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NIndexedArrayTest::suite());
}

