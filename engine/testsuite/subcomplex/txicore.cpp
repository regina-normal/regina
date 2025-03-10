
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"

#include "testhelper.h"

using regina::TxICore;
using regina::TxIDiagonalCore;
using regina::TxIParallelCore;

template <class Core>
static void testCopyMove(const Core& core) {
    SCOPED_TRACE("core = " + core.name());

    // The main point of this test is to ensure that the move
    // operations are *actually* move operations and not copies.

    const regina::Simplex<3>* s = core.core().simplex(0);

    Core a1(core);
    const regina::Simplex<3>* s1 = a1.core().simplex(0);

    EXPECT_EQ(a1.name(), core.name());
    EXPECT_NE(s1, s); // copy construction should use different simplices

    Core a2(std::move(a1));
    const regina::Simplex<3>* s2 = a2.core().simplex(0);

    EXPECT_EQ(a2.name(), core.name());
    EXPECT_EQ(s2, s1); // move construction should use the same simplices

    Core a3(a2);
    a3 = core;
    const regina::Simplex<3>* s3 = a3.core().simplex(0);

    EXPECT_EQ(a3.name(), core.name());
    EXPECT_NE(s3, s); // copy assignment should use different simplices

    Core a4(a2);
    a4 = std::move(a3);
    const regina::Simplex<3>* s4 = a4.core().simplex(0);

    EXPECT_EQ(a4.name(), core.name());
    EXPECT_EQ(s4, s3); // move assignment should use the same simplices
}

TEST(TxICoreTest, copyMove) {
    testCopyMove(TxIParallelCore());
    testCopyMove(TxIDiagonalCore(7, 2));
}
