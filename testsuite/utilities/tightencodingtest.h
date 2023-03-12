
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

/**
 * A class \a T that implements tight encodings (and optionally decodings) can
 * derive its test class from TightEncodingTest<T, ...>, which will give its
 * test class an inherited function verifyTightEncoding(const T&).
 *
 * The derived test class will still need to inherit from CppUnit::TestFixture,
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
                if (enc != out.str()) {
                    std::ostringstream msg;
                    msg << "Object " << obj.str()
                        << " has inconsistent tightEncoding() vs "
                            "tightEncode(): " << enc << ' ' << out.str();
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (char c : enc)
                if (c < 33 || c > 126) {
                    std::ostringstream msg;
                    msg << "Object " << obj.str()
                        << " has non-printable character "
                        << static_cast<int>(c) << " in its tight encoding.";
                    CPPUNIT_FAIL(msg.str());
                }

            if constexpr (hasDecoding) {
                try {
                    T dec = T::tightDecoding(enc);
                    if (dec != obj) {
                        std::ostringstream msg;
                        msg << "The tight encoding for object " << obj.str()
                            << " does not decode as a string to the same "
                            "object.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidArgument&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for object " << obj.str()
                        << " does not decode as a string at all.";
                    CPPUNIT_FAIL(msg.str());
                }

                try {
                    std::istringstream input(enc);
                    T dec = T::tightDecode(input);
                    if (dec != obj) {
                        std::ostringstream msg;
                        msg << "The tight encoding for object " << obj.str()
                            << " does not decode as an input stream "
                            "to the same object.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidArgument&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for object " << obj.str()
                        << " does not decode as an input stream at all.";
                    CPPUNIT_FAIL(msg.str());
                }

                try {
                    enc += ' ';
                    T::tightDecoding(enc);

                    std::ostringstream msg;
                    msg << "The tight encoding for object " << obj.str()
                        << " decodes as a string with trailing whitespace "
                        "(which it should not).";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::InvalidArgument&) {
                }

                try {
                    std::istringstream input(enc + "x y z");
                    T dec = T::tightDecode(input);
                    if (dec != obj) {
                        std::ostringstream msg;
                        msg << "The tight encoding for object " << obj.str()
                            << " does not decode as an input stream "
                            "with trailing characters to the same object.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    char c;
                    input >> c;
                    if ((! input) || (c != 'x')) {
                        std::ostringstream msg;
                        msg << "The tight encoding for object " << obj.str()
                            << " consumes trailing characters when "
                            "decoding as an input stream.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidInput&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for object " << obj.str()
                        << " does not decode as an input stream "
                        "with trailing characters at all.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }
};

#endif

