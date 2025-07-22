
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

#include <cstring>
#include <string>
#include "utilities/base64.h"

#include "testhelper.h"

static void verifyEncDec(const char* dat, size_t len) {
    SCOPED_TRACE_CSTRING(dat);

    char* enc;
    char* dec;

    size_t encLen = regina::base64Encode(dat, len, &enc);
    ASSERT_NE(enc, nullptr);
    EXPECT_EQ(strlen(enc), encLen);

    SCOPED_TRACE_CSTRING(enc);

    // Remove the terminating null, which should not matter for decryption.
    enc[encLen] = 'x';
    size_t decLen;
    bool res = regina::base64Decode(enc, encLen, &dec, &decLen);
    EXPECT_TRUE(res);
    EXPECT_EQ(decLen, len);
    for (size_t i = 0; i < decLen; ++i)
        EXPECT_EQ(dat[i], dec[i]);

    delete[] enc;
    delete[] dec;
}

TEST(Base64Test, encodeAndDecode) {
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

static void verifyInvalid(const char* enc, size_t validChars) {
    SCOPED_TRACE_CSTRING(enc);

    char* dec;
    size_t decLen;

    bool res = regina::base64Decode(enc, strlen(enc), &dec, &decLen);
    EXPECT_EQ(dec, nullptr);
    // In base64.h we indicate that decLen is left undefined.
    // EXPECT_EQ(decLen, 0);
    EXPECT_FALSE(res);
    delete[] dec;

    // Try the variant of base64Decode that decodes as much as it can.
    size_t expectDecLen = (validChars / 4) * 3;
    if (validChars % 4 > 1)
        expectDecLen += ((validChars % 4) - 1);

    dec = new char[expectDecLen + 10];
    decLen = expectDecLen + 10;
    res = regina::base64Decode(enc, strlen(enc), dec, &decLen);
    EXPECT_FALSE(res);
    EXPECT_EQ(decLen, expectDecLen);
    delete[] dec;
}

TEST(Base64Test, invalidEncodings) {
    verifyInvalid("AbCplo=6", 6);
    verifyInvalid("AbCpl===", 5);
    verifyInvalid("AbCp====", 4);
    verifyInvalid("abde fghi", 4);
    verifyInvalid("abde\nfghi", 4);
    verifyInvalid("abde\tfghi", 4);
};

