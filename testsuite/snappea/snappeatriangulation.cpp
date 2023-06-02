
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

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "link/examplelink.h"
#include "link/link.h"
#include "maths/matrix.h"
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/snappea/testsnappea.h"

using regina::SnapPeaTriangulation;
using regina::Triangulation;

class SnapPeaTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SnapPeaTriangulationTest);

    CPPUNIT_TEST(stability);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            SnapPeaTriangulation::disableKernelMessages();
        }

        void tearDown() override {
        }

        static void testStability(const Triangulation<3>& tri, const char*) {
            // Just make sure SnapPea can work with the triangulation
            // without crashing.
            try {
                SnapPeaTriangulation s(tri);
                s.volume();
                s.randomize();
                s.volume();
                Triangulation<3> t(s);
            } catch (const regina::SnapPeaIsNull&) {
            }
        }

        void stability() {
            runCensusAllNoBdry(&testStability);
        }
};

void addSnapPeaTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(SnapPeaTriangulationTest::suite());
}

