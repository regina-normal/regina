
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/snappea/testsnappea.h"

using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

class NSnapPeaTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NSnapPeaTriangulationTest);

    CPPUNIT_TEST(volume);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * Triangulations from "A Census of Cusped Hyperbolic 3-Manifolds",
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999, pp 321--332.
         */

        // Orientable 3-manifolds:
        NTriangulation m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;

        // Non-orientable 3-manifolds:
        NTriangulation n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;

    public:
        void setUp() {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            NSnapPeaTriangulation::disableKernelMessages();

            m2_1.insertRehydration("cabbbbaei");
            m2_2.insertRehydration("cabbbbapt");
            m3_9.insertRehydration("dagacccfwkn");
            m4_52.insertRehydration("ebdbcdddaqhie");
            m4_1_2.insertRehydration("eahbcdddhsssj");
            m4_4_2.insertRehydration("ebdbcdddddddx");

            n1_1.insertRehydration("baaaade");
            n2_1.insertRehydration("cabbbbabw");
            n2_1_2.insertRehydration("cabbbbcdw");
            n4_14.insertRehydration("eahdccddakfhq");
            n4_9_2.insertRehydration("ebdbcdddcemre");
            n4_1_2_1.insertRehydration("eahbcdddjxxxj");
        }

        void tearDown() {
        }

        void testVolume(NTriangulation& tri, const char* triName,
                double vol, unsigned places) {
            // Verify the volume to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            NSnapPeaTriangulation s(tri);
            {
                std::ostringstream msg;
                msg << "Triangulation " << triName <<
                    " could not be represented in SnapPea format.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ! s.isNull());
            }

            int precision;
            double foundVol = s.volume(precision);
            {
                std::ostringstream msg;
                msg << "Volume for " << triName <<
                    " has a precision of " << precision
                    << " places, which is less than the desired "
                    << places << " places.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    precision >= static_cast<int>(places));
            }

            double epsilon = 0.5;
            for (unsigned i = 0; i < places; i++)
                epsilon /= 10;

            {
                std::ostringstream msg;
                msg << "Volume for " << triName << " should be "
                    << std::setprecision(
                        places + static_cast<int>(ceil(log10(vol))))
                    << vol << ", not "
                    << std::setprecision(
                        precision + static_cast<int>(ceil(log10(foundVol))))
                    << foundVol << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    foundVol <= vol + epsilon && foundVol >= vol - epsilon);
            }
        }

        void volume() {
            testVolume(m2_1,   "M 2_1",   2.0298832128, 9);
            testVolume(m2_2,   "M 2_2",   2.0298832128, 9);
            testVolume(m3_9,   "M 3_9",   2.9441064867, 9);
            testVolume(m4_52,  "M 4_52",  4.0597664256, 9);
            testVolume(m4_1_2, "M 4_1^2", 3.6638623767, 9);
            testVolume(m4_4_2, "M 4_4^2", 4.0597664256, 9);

            testVolume(n1_1,     "N 1_1",     1.0149416064, 9);
            testVolume(n2_1,     "N 2_1",     1.8319311884, 9);
            testVolume(n2_1_2,   "N 2_1^2",   2.0298832128, 9);
            testVolume(n4_14,    "N 4_14",    3.9696478012, 9);
            testVolume(n4_9_2,   "N 4_9^2",   4.0597664256, 9);
            testVolume(n4_1_2_1, "N 4_1^2,1", 3.6638623767, 9);
        }
};

void addNSnapPeaTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NSnapPeaTriangulationTest::suite());
}

