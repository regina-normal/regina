
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

#include <cppunit/extensions/HelperMacros.h>
#include "manifold/sfs.h"
#include "testsuite/subcomplex/testsubcomplex.h"

using regina::SFSFibre;
using regina::SFSpace;

class SFSTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SFSTest);

    CPPUNIT_TEST(construct);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void verifyName(SFSpace::ClassType c, size_t genus,
                size_t punctures, size_t puncturesTwisted,
                size_t reflectors, size_t reflectorsTwisted,
                std::initializer_list<SFSFibre> fibres,
                const char* expected) {
            SFSpace s(c, genus, punctures, puncturesTwisted,
                reflectors, reflectorsTwisted);
            for (const auto& f : fibres)
                s.insertFibre(f);

            std::string name = s.name();
            if (name != expected) {
                std::ostringstream msg;
                msg << "A SFSpace was constructed, and its name was "
                    "reported as " << name << " instead of "
                    << expected << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }

        void construct() {
            verifyName(SFSpace::o1, 0, 0, 0, 0, 0, {}, "S2 x S1");
            verifyName(SFSpace::o1, 1, 0, 0, 0, 0, {}, "T x S1");
            verifyName(SFSpace::bo1, 0, 1, 0, 0, 0, {}, "D x S1");
        }
};

void addSFS(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(SFSTest::suite());
}

