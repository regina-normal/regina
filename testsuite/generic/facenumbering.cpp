
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/generic.h"
#include "testsuite/generic/testgeneric.h"

// For specialisations of FaceNumbering:
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

using regina::FaceNumbering;
using regina::Perm;

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
        static constexpr int n = dim + 1;

        using Index = typename Perm<dim + 1>::Index;

        Index* idx;
        Index nIdx;

    public:
        void setUp() override {
            idx = new Index[(Perm<dim + 1>::nPerms / increment[n]) + 2];
            nIdx = 0;
            for (Index i = 0; i < Perm<dim + 1>::nPerms; i += increment[n])
                idx[nIdx++] = i;
            if (idx[nIdx - 1] != Perm<dim + 1>::nPerms - 1)
                idx[nIdx++] = Perm<dim + 1>::nPerms - 1;
        }

        void tearDown() override {
            delete[] idx;
        }

        void ordering() {
            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                Perm<dim + 1> p = FaceNumbering<dim, subdim>::ordering(f);

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

        void verifyFaceNumber(int face, Perm<dim + 1> reorder) {
            Perm<dim + 1> p =
                FaceNumbering<dim, subdim>::ordering(face) * reorder;

            if (FaceNumbering<dim, subdim>::faceNumber(p) != face) {
                std::ostringstream msg;
                msg << "Face<" << dim << ", " << subdim
                    << "> does not have faceNumber(ordering(" << face
                    << ")) == " << face
                    << " with reordering " << reorder << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void faceNumber() {
            Perm<dim + 1> rev = Perm<dim + 1>().reverse();

            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                // Check that we can correctly identify face number f
                // under many possible permutations of the vertices
                // in the face and many possible permutations of the vertices
                // not in the face.
                if constexpr (subdim == 0) {
                    for (int upper = 0; upper < dim; ++upper) {
                        Perm<dim> u = Perm<dim>::rot(upper);

                        verifyFaceNumber(f,
                            rev * Perm<dim + 1>::extend(u) * rev);
                        verifyFaceNumber(f,
                            rev * Perm<dim + 1>::extend(u.reverse()) * rev);
                    }
                    verifyFaceNumber(f, Perm<dim + 1>());
                } else if constexpr (subdim == dim - 1) {
                    for (int lower = 0; lower <= subdim; ++lower) {
                        Perm<subdim + 1> l = Perm<subdim + 1>::rot(lower);

                        verifyFaceNumber(f,
                            Perm<dim + 1>::extend(l));
                        verifyFaceNumber(f,
                            Perm<dim + 1>::extend(l.reverse()));
                    }
                } else {
                    for (int lower = 0; lower <= subdim; ++lower)
                        for (int upper = 0; upper < dim - subdim; ++upper) {
                            Perm<subdim + 1> l = Perm<subdim + 1>::rot(lower);
                            Perm<dim - subdim> u =
                                Perm<dim - subdim>::rot(upper);

                            verifyFaceNumber(f,
                                Perm<dim + 1>::extend(l) *
                                rev * Perm<dim + 1>::extend(u) * rev);
                            verifyFaceNumber(f,
                                Perm<dim + 1>::extend(l) *
                                rev * Perm<dim + 1>::extend(u.reverse()) * rev);
                            verifyFaceNumber(f,
                                Perm<dim + 1>::extend(l.reverse()) *
                                rev * Perm<dim + 1>::extend(u) * rev);
                            verifyFaceNumber(f,
                                Perm<dim + 1>::extend(l.reverse()) *
                                rev * Perm<dim + 1>::extend(u.reverse()) * rev);
                        }
                }
            }
        }

        void containsVertex() {
            int v;
            for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
                Perm<dim + 1> p = FaceNumbering<dim, subdim>::ordering(f);

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

    // Generic face numbering but specialised permutation templates:
    runner.addTest(FaceNumberingTest<5, 0>::suite());
    runner.addTest(FaceNumberingTest<5, 1>::suite());
    runner.addTest(FaceNumberingTest<5, 2>::suite());
    runner.addTest(FaceNumberingTest<5, 3>::suite());
    runner.addTest(FaceNumberingTest<5, 4>::suite());

    runner.addTest(FaceNumberingTest<6, 0>::suite());
    runner.addTest(FaceNumberingTest<6, 1>::suite());
    runner.addTest(FaceNumberingTest<6, 2>::suite());
    runner.addTest(FaceNumberingTest<6, 3>::suite());
    runner.addTest(FaceNumberingTest<6, 4>::suite());
    runner.addTest(FaceNumberingTest<6, 5>::suite());

    runner.addTest(FaceNumberingTest<7, 0>::suite());
    runner.addTest(FaceNumberingTest<7, 1>::suite());
    runner.addTest(FaceNumberingTest<7, 2>::suite());
    runner.addTest(FaceNumberingTest<7, 3>::suite());
    runner.addTest(FaceNumberingTest<7, 4>::suite());
    runner.addTest(FaceNumberingTest<7, 5>::suite());
    runner.addTest(FaceNumberingTest<7, 6>::suite());

    // Generic face numbering *and* permutation templates:
    runner.addTest(FaceNumberingTest<8, 0>::suite());
    runner.addTest(FaceNumberingTest<8, 1>::suite());
    runner.addTest(FaceNumberingTest<8, 2>::suite());
    runner.addTest(FaceNumberingTest<8, 3>::suite());
    runner.addTest(FaceNumberingTest<8, 4>::suite());
    runner.addTest(FaceNumberingTest<8, 5>::suite());
    runner.addTest(FaceNumberingTest<8, 6>::suite());
    runner.addTest(FaceNumberingTest<8, 7>::suite());

    runner.addTest(FaceNumberingTest<15, 0>::suite());
    runner.addTest(FaceNumberingTest<15, 1>::suite());
    runner.addTest(FaceNumberingTest<15, 2>::suite());
    runner.addTest(FaceNumberingTest<15, 5>::suite());
    runner.addTest(FaceNumberingTest<15, 8>::suite());
    runner.addTest(FaceNumberingTest<15, 12>::suite());
    runner.addTest(FaceNumberingTest<15, 13>::suite());
    runner.addTest(FaceNumberingTest<15, 14>::suite());
}

