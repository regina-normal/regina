
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __TIGHTENCODINGTEST_H
#define __TIGHTENCODINGTEST_H

#include "gtest/gtest.h"

/**
 * A class \a T that implements tight encodings (and optionally decodings) can
 * derive its test class from TightEncodingTest<T, ...>, which will give its
 * test class an inherited function verifyTightEncoding(const T&).
 *
 * The derived test class will still need to inherit from testing::Test,
 * and will still need to set up a wrapper tightEncoding() test that calls
 * verifyTightEncoding(...) for an appropriate selection of test objects.
 *
 * There are several requirements on the type \a T, including:
 *
 * - a str() function;
 * - equality tests;
 * 
 */
template <class T, bool hasDecoding = true>
class TightEncodingTest {
    public:
        static void verifyTightEncoding(const T& obj) {
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

                EXPECT_THROW({
                    enc += ' ';
                    T::tightDecoding(enc);
                }, regina::InvalidArgument);

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

