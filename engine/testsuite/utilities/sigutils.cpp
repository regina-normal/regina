
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

#include <array>
#include "utilities/bitmask.h"
#include "utilities/sigutils.h"

#include "testhelper.h"

using regina::Bitmask;
using regina::BitSigEncoder;
using regina::ByteSequence;

TEST(SigUtilsTest, bitEncoder) {
    {
        BitSigEncoder enc;
        EXPECT_EQ(std::move(enc).bytes(), ByteSequence());
    }
    {
        BitSigEncoder enc;
        for (bool b : { false, true, false, true, true, false, false, true,
                        true, false, false, true, true })
            enc.encodeBit(b);
        EXPECT_EQ(std::move(enc).bytes(), ByteSequence({ 0x9a, 0x19 }));
    }
    {
        BitSigEncoder enc;
        enc.encodeBits(3, unsigned(0x0002));
        enc.encodeBits(14, unsigned(0x3576)); // spans three encoded bytes
        EXPECT_EQ(std::move(enc).bytes(),
            ByteSequence({ 0xb2, 0xab, 0x01 }));
    }
    EXPECT_THROW({
        BitSigEncoder enc;
        enc.encodeBits(3, unsigned(0x0002));
        enc.encodeBits(14, unsigned(0x4576));
    }, regina::InvalidArgument);
    {
        BitSigEncoder enc;
        enc.encodeBits(3, unsigned(0x0002));
        {
            Bitmask mask(14);
            std::array indices { 1, 2, 4, 5, 6, 8, 10, 12, 13 };
            mask.set(indices.begin(), indices.end(), true);
            enc.encodeBits(14, mask);
        }
        EXPECT_EQ(std::move(enc).bytes(),
            ByteSequence({ 0xb2, 0xab, 0x01 }));
    }
}

