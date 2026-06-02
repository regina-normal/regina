
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include <array>
#include "utilities/bitmask.h"
#include "utilities/sigutils.h"

#include "testhelper.h"

using regina::Base64BitDecoder;
using regina::Base64BitEncoder;
using regina::Bitmask;
using regina::BitDecoder;
using regina::BitEncoder;
using regina::ByteSequence;

TEST(SigUtilsTest, bitEncoder) {
    {
        BitEncoder enc;
        auto bytes = std::move(enc).bytes();
        EXPECT_EQ(bytes, ByteSequence());

        BitDecoder dec(bytes.begin(), bytes.end());
        EXPECT_TRUE(dec.noMoreBits());
    }
    {
        BitEncoder enc;
        std::array write { false, true, false, true, true, false, false, true,
                           true, false, false, true, true };
        for (bool b : write)
            enc.encodeBit(b);
        auto bytes = std::move(enc).bytes();
        EXPECT_EQ(bytes, ByteSequence({ 0x9a, 0x19 }));

        BitDecoder dec(bytes.begin(), bytes.end());
        EXPECT_FALSE(dec.maybeDone());
        std::array<bool, 13> read;
        for (bool& b : read)
            b = dec.decodeBit();
        EXPECT_EQ(read, write);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
    {
        BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);
        enc.encodeInt(unsigned(0x3576), 14); // spans three encoded bytes
        auto bytes = std::move(enc).bytes();
        EXPECT_EQ(bytes, ByteSequence({ 0xb2, 0xab, 0x01 }));

        BitDecoder dec(bytes.begin(), bytes.end());
        EXPECT_FALSE(dec.maybeDone());
        EXPECT_EQ(dec.decodeInt<unsigned>(3), 0x0002);
        EXPECT_EQ(dec.decodeInt<unsigned>(14), 0x3576);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
    EXPECT_THROW({
        BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);
        enc.encodeInt(unsigned(0x4576), 14);
    }, regina::InvalidArgument);
    {
        BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);

        Bitmask mask(14);
        std::array indices { 1, 2, 4, 5, 6, 8, 10, 12, 13 };
        mask.set(indices.begin(), indices.end(), true);
        enc.encodeBitmask(mask, 14);

        auto bytes = std::move(enc).bytes();
        EXPECT_EQ(bytes, ByteSequence({ 0xb2, 0xab, 0x01 }));

        BitDecoder dec(bytes.begin(), bytes.end());
        EXPECT_FALSE(dec.maybeDone());
        EXPECT_EQ(dec.decodeInt<unsigned>(3), 0x0002);
        EXPECT_EQ(dec.decodeBitmask(14), mask);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
    {
        Base64BitEncoder enc;
        auto str = std::move(enc).str();
        EXPECT_EQ(str, "");

        Base64BitDecoder dec(str.begin(), str.end());
        EXPECT_TRUE(dec.noMoreBits());
    }
    {
        Base64BitEncoder enc;
        std::array write { false, true, false, true, true, false, false, true,
                           true, false, false, true, true };
        for (bool b : write)
            enc.encodeBit(b);
        auto str = std::move(enc).str();
        EXPECT_EQ(str, "AMb");

        Base64BitDecoder dec(str.begin(), str.end());
        EXPECT_FALSE(dec.maybeDone());
        std::array<bool, 13> read;
        for (bool& b : read)
            b = dec.decodeBit();
        EXPECT_EQ(read, write);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
    {
        Base64BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);
        enc.encodeInt(unsigned(0x3576), 14); // spans three encoded chars
        auto str = std::move(enc).str();
        EXPECT_EQ(str, "YUA");

        Base64BitDecoder dec(str.begin(), str.end());
        EXPECT_FALSE(dec.maybeDone());
        EXPECT_EQ(dec.decodeInt<unsigned>(3), 0x0002);
        EXPECT_EQ(dec.decodeInt<unsigned>(14), 0x3576);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
    EXPECT_THROW({
        Base64BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);
        enc.encodeInt(unsigned(0x4576), 14);
    }, regina::InvalidArgument);
    {
        Base64BitEncoder enc;
        enc.encodeInt(unsigned(0x0002), 3);

        Bitmask mask(14);
        std::array indices { 1, 2, 4, 5, 6, 8, 10, 12, 13 };
        mask.set(indices.begin(), indices.end(), true);
        enc.encodeBitmask(mask, 14);

        auto str = std::move(enc).str();
        EXPECT_EQ(str, "YUA");

        Base64BitDecoder dec(str.begin(), str.end());
        EXPECT_FALSE(dec.maybeDone());
        EXPECT_EQ(dec.decodeInt<unsigned>(3), 0x0002);
        EXPECT_EQ(dec.decodeBitmask(14), mask);
        EXPECT_TRUE(dec.maybeDone());
        EXPECT_FALSE(dec.noMoreBits());
    }
}

