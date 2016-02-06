
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

#include <cstring>
#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/base64.h"

class Base64Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Base64Test);

    CPPUNIT_TEST(encodeAndDecode);
    CPPUNIT_TEST(invalidEncodings);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void verifyEncDec(const char* dat, size_t len) {
            char* enc;
            char* dec;

            size_t encLen = regina::base64Encode(dat, len, &enc);
            if (! enc) {
                CPPUNIT_FAIL("Failed to encode to base64.");
                return;
            }
            if (strlen(enc) != encLen) {
                CPPUNIT_FAIL("Base64 encoded string has incorrect length.");
                delete[] enc;
                return;
            }
            // Remove the terminating null, which should not matter for
            // decryption.
            enc[encLen] = 'x';
            size_t decLen;
            bool res = regina::base64Decode(enc, encLen, &dec, &decLen);

            if (! res) {
                std::ostringstream msg;
                enc[encLen] = 0; // so we can print the encoded string
                msg << "Failed to decode base64: " << enc;
                delete[] enc;
                delete[] dec; // should be null anyway
                CPPUNIT_FAIL(msg.str());
                return;
            }
            if (decLen != len) {
                std::ostringstream msg;
                enc[encLen] = 0; // so we can print the encoded string
                msg << "Decoding to base64 gives incorrect length: " << enc;
                delete[] enc;
                delete[] dec;
                CPPUNIT_FAIL(msg.str());
                return;
            }
            for (size_t i = 0; i < decLen; ++i)
                if (dat[i] != dec[i]) {
                    std::ostringstream msg;
                    enc[encLen] = 0; // so we can print the encoded string
                    msg << "Decoding to base64 gives incorrect data: " << enc;
                    delete[] enc;
                    delete[] dec;
                    CPPUNIT_FAIL(msg.str());
                    return;
                }
            delete[] enc;
            delete[] dec;
        }

        void encodeAndDecode() {
            verifyEncDec("foobar", 6);
            verifyEncDec("foobar", 3);
            verifyEncDec("foo\0bar\0baz\0", 12);
            verifyEncDec("", 0);
            verifyEncDec("!", 1);
            verifyEncDec("!_", 2);
            verifyEncDec("!_:", 3);
            verifyEncDec("!_:)", 4);
            verifyEncDec("sadjfl8q34jr9awj;ljfap98q2up[]!@~|$", 35);
            verifyEncDec("sadjfl8q34jr9awj;ljfap98q2up[]!@~|$/", 36);
            verifyEncDec("sadjfl8q34jr9awj;ljfap98q2up[]!@~|$/\t", 37);
            verifyEncDec("sadjfl8q34jr9awj;ljfap98q2up[]!@~|$/\t\n", 38);
        }

        void verifyInvalid(const char* enc, size_t validChars) {
            char* dec;
            size_t decLen;
            bool res = regina::base64Decode(enc, strlen(enc), &dec, &decLen);
            if (dec) {
                std::ostringstream msg;
                msg << "Invalid base64 string was decoded: " << enc;
                delete[] dec;
                CPPUNIT_FAIL(msg.str());
                return;
            }
            /** In base64.h we indicate that decLen is left undefined.
            if (decLen) {
                std::ostringstream msg;
                msg << "Invalid base64 string reported "
                    "non-zero decoded length: " << enc;
                CPPUNIT_FAIL(msg.str());
                return;
            }
            **/
            if (res) {
                std::ostringstream msg;
                msg << "Invalid base64 string reported "
                    "a valid decoding: " << enc;
                CPPUNIT_FAIL(msg.str());
                return;
            }

            // Try the variant of base64Decode that decodes as much as it can.
            size_t expectDecLen = (validChars / 4) * 3;
            if (validChars % 4 > 1)
                expectDecLen += ((validChars % 4) - 1);

            dec = new char[expectDecLen + 10];
            decLen = expectDecLen + 10;
            res = regina::base64Decode(enc, strlen(enc), dec, &decLen);
            if (res) {
                std::ostringstream msg;
                msg << "Invalid base64 string reported "
                    "a valid decoding: " << enc;
                delete[] dec;
                CPPUNIT_FAIL(msg.str());
                return;
            }
            if (decLen != expectDecLen) {
                std::ostringstream msg;
                msg << "Invalid base64 string reported "
                    "incorrect decoded length of "
                    << decLen << ", not " << expectDecLen << ": " << enc;
                delete[] dec;
                CPPUNIT_FAIL(msg.str());
                return;
            }
            delete[] dec;
        }

        void invalidEncodings() {
            verifyInvalid("AbCplo=6", 6);
            verifyInvalid("AbCpl===", 5);
            verifyInvalid("AbCp====", 4);
            verifyInvalid("abde fghi", 4);
            verifyInvalid("abde\nfghi", 4);
            verifyInvalid("abde\tfghi", 4);
        }
};

void addBase64(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Base64Test::suite());
}

