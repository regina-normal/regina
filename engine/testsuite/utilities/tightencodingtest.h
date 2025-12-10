
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#ifndef __TIGHTENCODINGTEST_H
#define __TIGHTENCODINGTEST_H

#include "testhelper.h"

/**
 * Implements tests for tight encodings (and optionally decodings) of objects
 * of type \a T.  This type \a T must provide equality tests and a str()
 * function that returns a std::string.
 *
 * Test suites can call these functions directly.  There is no need (or
 * benefit) for using inheritance of test fixture classes, other than the
 * minor convenience of not having to type out the template parameters for
 * TightEncodingTest every time it is used.
 */
template <typename T, bool hasDecoding = true>
class TightEncodingTest {
    // Ensure that str() returns a std::string.
    static_assert(
        std::is_same_v<decltype(std::declval<T>().str()), std::string>,
        "TightEncodingTest<T> requires T::str() to return a std::string.");

    public:
        static void verifyTightEncoding(const T& obj) {
            SCOPED_TRACE_REGINA(obj);
            std::string enc = obj.tightEncoding();

            {
                std::ostringstream out;
                obj.tightEncode(out);
                EXPECT_EQ(enc, out.str());
            }

            for (char c : enc) {
                EXPECT_GE(c, 33);
                EXPECT_LE(c, 126);
            }

            if constexpr (hasDecoding) {
                EXPECT_NO_THROW({
                    T dec = T::tightDecoding(enc);
                    EXPECT_EQ(dec, obj);
                });

                EXPECT_NO_THROW({
                    std::istringstream input(enc);
                    T dec = T::tightDecode(input);
                    EXPECT_EQ(dec, obj);
                });

                // Strings being decoded cannot have trailing whitespace.
                EXPECT_THROW({
                    T::tightDecoding(enc + ' ');
                }, regina::InvalidArgument);

                // Streams being decoded should ignore (and not consume)
                // any trailing characters.
                EXPECT_NO_THROW({
                    std::istringstream input(enc + "x y z");
                    T dec = T::tightDecode(input);
                    EXPECT_EQ(dec, obj);

                    char c;
                    input >> c;
                    ASSERT_TRUE(input);
                    EXPECT_EQ(c, 'x');
                });
            }
        }
};

#endif

