
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "manifold/nmanifold.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/subcomplex/testsubcomplex.h"

using regina::NManifold;
using regina::NStandardTriangulation;
using regina::NTriangulation;

class NStandardTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NStandardTriangulationTest);

    CPPUNIT_TEST(recognition);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void testRecognition(const char* dehydration,
                const char* triName, const char* mfdName) {
            NTriangulation t;
            if (! t.insertRehydration(dehydration)) {
                std::ostringstream msg;
                msg << "The standard triangulation " << triName
                    << " could not be constructed from its dehydration.";
                CPPUNIT_FAIL(msg.str());
            }

            NStandardTriangulation* std =
                NStandardTriangulation::isStandardTriangulation(&t);
            if (! std) {
                std::ostringstream msg;
                msg << "The standard triangulation " << triName
                    << " was not recognised at all.";
                CPPUNIT_FAIL(msg.str());
            }
            if (std->getName() != triName) {
                std::ostringstream msg;
                msg << "The standard triangulation " << triName
                    << " was instead recognised as "
                    << std->getName() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NManifold* mfd = std->getManifold();
            if (! mfd) {
                std::ostringstream msg;
                msg << "The 3-manifold for the standard triangulation "
                    << triName << " was not recognised at all.";
                CPPUNIT_FAIL(msg.str());
            }
            if (mfd->getName() != mfdName) {
                std::ostringstream msg;
                msg << "The 3-manifold for the standard triangulation "
                    << triName << " was recognised as " << mfd->getName()
                    << ", not the expected " << mfdName << ".";
                CPPUNIT_FAIL(msg.str());
            }

            delete mfd;
            delete std;
        }

        void recognition() {
            // Closed orientable triangulations:
            testRecognition("baaaawr", "C(1)", "S3");
            testRecognition("baaaawf", "L(1,0)", "S3");
            testRecognition("cabbbbxxt", "L'(3,1)", "L(3,1)");
            testRecognition("cacabbwxf", "L(3,1) (1)", "L(3,1)");
            testRecognition("cacabbfxn", "L(3,1) (2)", "L(3,1)");
            testRecognition("fkkaaabcdeefaqhan", "L(29,8)", "L(29,8)");
            testRecognition("fffaabcdeeeaaaakn", "C~(5)", "S3/Q20");
            testRecognition("fjgaaacddeeffthon", "A(3,-2 | 3,-1)", "S3/P'72");
            testRecognition("fdfaabcedeeaategk", "J(2 | 2,-1)", "S3/P'72");
            testRecognition("fbnaabcedeemgxbdu", "X(2 | 2,-1)", "S3/D40");
            testRecognition("fapaaceddeegngoon", "P'(0)", "S3/P48");
            testRecognition("fapaadecedenbokbo", "P(0)", "S3/P120");
            testRecognition("jhnafaabdgfghhiiihkagrkvskr",
                "B(T7:1 | -1,0 | -3,-1)", "T x I / [ -4,-3 | -1,-1 ]");
            testRecognition("jlkijaaabdefghhiifanavrumvb",
                "Blocked SFS [Tri, Tri, LST(1, 2, 3), LST(1, 3, 4)]",
                "SFS [RP2/n2: (3,1) (4,3)]");
            testRecognition("jnnagaabcgfihhihimgtsonkjgr",
                "Blocked SFS [Tri, Tri, Tri, Mob(v)]", "SFS [T: (2,3)]");
            testRecognition("jfkinaaabdeehghiifhgtqitrtn",
                "Blocked SFS Pair [Tri, LST(2, 3, 5), Mob(v) | "
                    "Tri, LST(1, 2, 3), Mob(v)]",
                "SFS [D: (2,1) (2,1)] U/m SFS [D: (2,1) (5,2)], "
                    "m = [ 1,1 | 0,1 ]");
            testRecognition("jofdiaabcceffhiiiedkgiilpsw",
                "Blocked SFS Triple [Tri, Mob(v), Mob(v) | Tri, Mob(v) | "
                    "Tri, Mob(v), Mob(v)]",
                "SFS [D: (2,1) (2,1)] U/m SFS [A: (2,1)] U/n "
                    "SFS [D: (2,1) (2,1)], m = [ 0,1 | 1,0 ], "
                    "n = [ 1,1 | 1,0 ]");

            // Closed non-orientable triangulations:
            testRecognition("dafbcccxbqg", "N(3,2)", "RP2 x S1");
            testRecognition("ihnabbdghgfhhgakaxdneel",
                "B(T6:1 | -3,-2 | -2,-1)", "T x I / [ 3,2 | 2,1 ]");
            testRecognition("icoaladddggghhfbwabwaxf",
                "Blocked SFS [Ref(2), LST(1, 2, 3), LST(1, 2, 3)]",
                "SFS [D_: (3,1) (3,2)]");
            testRecognition("kcoelabadddgggijjjfbwabwafehe",
                "Blocked SFS [Tri, Ref(2), LST(1, 2, 3)]",
                "SFS [M_/n2: (3,1)]");
            testRecognition("kcomlaaadddgggjijjfbwabwabrwa",
                "Blocked SFS [Ref(1), Ref(2), LST(1, 2, 3)]",
                "SFS [A=: (3,1)]");
            testRecognition("kjgmlaaacddfhhijjjffthmvooloh",
                "Blocked SFS Pair [Tri, LST(1, 2, 3), Mob(v) | "
                    "Tri, Ref(1), Mob(v)]",
                "SFS [D: (2,1) (3,1)] U/m SFS [A_: (2,1)], "
                    "m = [ 0,1 | 1,0 ]");

            // Ideal triangulations:
            testRecognition("cabbbbteh",
                "SnapPea m004", "Figure eight knot complement");
        }
};

void addNStandardTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NStandardTriangulationTest::suite());
}

