
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/generic.h"
#include "testsuite/generic/testgeneric.h"

// For specialisations of FaceNumbering:
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

using regina::FaceNumbering;

static const int64_t increment[] = {
    // Used to iterate through permutations on n elements.
    // All of these increments are coprime with n.
    1, 1, 1, 1, 1, 1, 1, /* n=0..6 */
    11, 143, 2431, 12673, 96577, 1255501, 55190041, /* n=7..13 */
    247110827, 4200884059, 54611492767 /* n=14..16 */
};

template <int dim, int subdim>
class FaceNumberingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FaceNumberingTest);

    CPPUNIT_TEST(ordering);
    CPPUNIT_TEST(faceNumber);
    CPPUNIT_TEST(containsVertex);

    CPPUNIT_TEST_SUITE_END();

    private:
        static const int n = dim + 1;

        typedef regina::Perm<n> Perm;
        typedef typename Perm::Index Index;

        Index* idx;
        Index nIdx;

    public:
        void setUp() {
            idx = new Index[(Perm::nPerms / increment[n]) + 2];
            nIdx = 0;
            for (Index i = 0; i < Perm::nPerms; i += increment[n])
                idx[nIdx++] = i;
            if (idx[nIdx - 1] != Perm::nPerms - 1)
                idx[nIdx++] = Perm::nPerms - 1;
        }

        void tearDown() {
            delete[] idx;
        }

        void ordering() {
            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                Perm p = FaceNumbering<dim, subdim>::ordering(f);

                for (int i = 0; i < subdim; ++i)
                    if (p[i] >= p[i + 1]) {
                        std::ostringstream msg;
                        msg << "Face<" << dim << ", " << subdim
                            << "> number " << f << " gives ordering "
                            << p << ", which does not list the vertices "
                            "of the face in increasing order.";
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }

        void faceNumber() {
            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                Perm p = FaceNumbering<dim, subdim>::ordering(f);

                if (FaceNumbering<dim, subdim>::faceNumber(p) != f) {
                    std::ostringstream msg;
                    msg << "Face<" << dim << ", " << subdim
                        << "> does not have faceNumber(ordering(" << f
                        << ")) == " << f << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void containsVertex() {
            int v;
            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                Perm p = FaceNumbering<dim, subdim>::ordering(f);

                for (v = 0; v <= subdim; ++v)
                    if (! FaceNumbering<dim, subdim>::containsVertex(f, p[v])) {
                        std::ostringstream msg;
                        msg << "Face<" << dim << ", " << subdim
                            << "> number " << f << " gives ordering "
                            << p << ", but claims to not contain vertex "
                            << p[v] << '.';
                        CPPUNIT_FAIL(msg.str());
                    }

                for (v = subdim + 1; v <= dim; ++v)
                    if (FaceNumbering<dim, subdim>::containsVertex(f, p[v])) {
                        std::ostringstream msg;
                        msg << "Face<" << dim << ", " << subdim
                            << "> number " << f << " gives ordering "
                            << p << ", but claims to contain vertex "
                            << p[v] << '.';
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }
};

void addFaceNumbering(CppUnit::TextUi::TestRunner& runner) {
    // Specialised templates:
    runner.addTest(FaceNumberingTest<2, 0>::suite());
    runner.addTest(FaceNumberingTest<2, 1>::suite());
    runner.addTest(FaceNumberingTest<3, 0>::suite());
    runner.addTest(FaceNumberingTest<3, 1>::suite());
    runner.addTest(FaceNumberingTest<3, 2>::suite());
    runner.addTest(FaceNumberingTest<4, 0>::suite());
    runner.addTest(FaceNumberingTest<4, 1>::suite());
    runner.addTest(FaceNumberingTest<4, 2>::suite());
    runner.addTest(FaceNumberingTest<4, 3>::suite());

    // Generic implementations:
    runner.addTest(FaceNumberingTest<5, 0>::suite());
    runner.addTest(FaceNumberingTest<5, 1>::suite());
    runner.addTest(FaceNumberingTest<5, 2>::suite());
    runner.addTest(FaceNumberingTest<5, 3>::suite());
    runner.addTest(FaceNumberingTest<5, 4>::suite());

    runner.addTest(FaceNumberingTest<7, 0>::suite());
    runner.addTest(FaceNumberingTest<7, 1>::suite());
    runner.addTest(FaceNumberingTest<7, 2>::suite());
    runner.addTest(FaceNumberingTest<7, 3>::suite());
    runner.addTest(FaceNumberingTest<7, 4>::suite());
    runner.addTest(FaceNumberingTest<7, 5>::suite());
    runner.addTest(FaceNumberingTest<7, 6>::suite());

    runner.addTest(FaceNumberingTest<15, 0>::suite());
    runner.addTest(FaceNumberingTest<15, 1>::suite());
    runner.addTest(FaceNumberingTest<15, 2>::suite());
    runner.addTest(FaceNumberingTest<15, 5>::suite());
    runner.addTest(FaceNumberingTest<15, 8>::suite());
    runner.addTest(FaceNumberingTest<15, 12>::suite());
    runner.addTest(FaceNumberingTest<15, 13>::suite());
    runner.addTest(FaceNumberingTest<15, 14>::suite());
}

