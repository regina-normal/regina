
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"
#include "testsuite/subcomplex/testsubcomplex.h"

using regina::TxICore;
using regina::TxIDiagonalCore;
using regina::TxIParallelCore;

class TxICoreTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TxICoreTest);

    CPPUNIT_TEST(copyMove);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        template <class Core>
        void testCopyMove(const Core& core) {
            // The main point of this test is to ensure that the move
            // operations are *actually* move operations and not copies.

            const regina::Simplex<3>* s = core.core().simplex(0);

            Core a1(core);
            const regina::Simplex<3>* s1 = a1.core().simplex(0);

            if (a1.name() != core.name()) {
                std::ostringstream msg;
                msg << core.name() << ": copy constructed has the wrong name.";
                CPPUNIT_FAIL(msg.str());
            }
            if (s1 == s) {
                std::ostringstream msg;
                msg << core.name() << ": copy constructed uses "
                    "the same simplices as the source.";
                CPPUNIT_FAIL(msg.str());
            }

            Core a2(std::move(a1));
            const regina::Simplex<3>* s2 = a2.core().simplex(0);

            if (a2.name() != core.name()) {
                std::ostringstream msg;
                msg << core.name() << ": move constructed has the wrong name.";
                CPPUNIT_FAIL(msg.str());
            }
            if (s2 != s1) {
                std::ostringstream msg;
                msg << core.name() << ": move constructed does not "
                    "use the same simplices as the source.";
                CPPUNIT_FAIL(msg.str());
            }

            Core a3(a2);
            a3 = core;
            const regina::Simplex<3>* s3 = a3.core().simplex(0);

            if (a3.name() != core.name()) {
                std::ostringstream msg;
                msg << core.name() << ": copy assigned has the wrong name.";
                CPPUNIT_FAIL(msg.str());
            }
            if (s3 == s) {
                std::ostringstream msg;
                msg << core.name() << ": copy assigned uses "
                    "the same simplices as the source.";
                CPPUNIT_FAIL(msg.str());
            }

            Core a4(a2);
            a4 = std::move(a3);
            const regina::Simplex<3>* s4 = a4.core().simplex(0);

            if (a4.name() != core.name()) {
                std::ostringstream msg;
                msg << core.name() << ": move assigned has the wrong name.";
                CPPUNIT_FAIL(msg.str());
            }
            if (s4 != s3) {
                std::ostringstream msg;
                msg << core.name() << ": move assigned does not use "
                    "the same simplices as the source.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void copyMove() {
            testCopyMove(TxIParallelCore());
            testCopyMove(TxIDiagonalCore(7, 2));
        }
};

void addTxICore(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(TxICoreTest::suite());
}

