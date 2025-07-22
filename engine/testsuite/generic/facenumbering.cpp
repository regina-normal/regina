
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

#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"

#include "testhelper.h"

using regina::FaceNumbering;
using regina::Perm;

/**
 * The list of triangulation dimensions to test.
 *
 * Dimensions 2..4 use specialised face numbering implementations and
 * specialised permutation classes.
 *
 * Dimensions 5..7 use generic face numbering implementations but specialised
 * permutation classes.
 *
 * Dimensions 8..15 use generic everything.
 */
using dims = std::integer_sequence<int, 2, 3, 4, 5, 6, 7, 8, 15>;

/**
 * Lists the facial dimensions to test for a triangulation of dimension dim.
 *
 * By default we use all facial dimensions 0..(dim-1), but for large dim
 * we restrict this to a smaller subset of facial dimensions to avoid the test
 * suite becoming too slow.
 */
template <int dim>
struct TestSubdims {
    using subdims = std::make_integer_sequence<int, dim>;
};
template <>
struct TestSubdims<15> {
    using subdims = std::integer_sequence<int, 0, 1, 2, 5, 8, 12, 13, 14>;
};
template <int dim>
using subdims = typename TestSubdims<dim>::subdims;

template <int dim, int subdim>
static void faceNumberDetail() {
    SCOPED_TRACE_NUMERIC(subdim);
    using Impl = FaceNumbering<dim, subdim>;

    Perm<dim + 1> rev = Perm<dim + 1>().reverse();

    for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
        SCOPED_TRACE_NUMERIC(f);
        Perm<dim + 1> ordering = FaceNumbering<dim, subdim>::ordering(f);
        EXPECT_EQ(Impl::faceNumber(ordering), f);
        if constexpr (subdim == 1) {
            EXPECT_EQ(Impl::faceNumber(ordering[0], ordering[1]), f);
            EXPECT_EQ(Impl::faceNumber(ordering[1], ordering[0]), f);
        }

        // Check that we can correctly identify face number f under many
        // possible permutations of the vertices in the face and many possible
        // permutations of the vertices not in the face.
        if constexpr (subdim == 0) {
            for (int upper = 0; upper < dim; ++upper) {
                Perm<dim> u = Perm<dim>::rot(upper);

                EXPECT_EQ(Impl::faceNumber(ordering *
                    rev * Perm<dim + 1>::extend(u) * rev), f);
                EXPECT_EQ(Impl::faceNumber(ordering *
                    rev * Perm<dim + 1>::extend(u.reverse()) * rev), f);
            }
        } else if constexpr (subdim == dim - 1) {
            for (int lower = 0; lower <= subdim; ++lower) {
                Perm<subdim + 1> l = Perm<subdim + 1>::rot(lower);

                EXPECT_EQ(Impl::faceNumber(ordering *
                    Perm<dim + 1>::extend(l)), f);
                EXPECT_EQ(Impl::faceNumber(ordering *
                    Perm<dim + 1>::extend(l.reverse())), f);
            }
        } else {
            for (int lower = 0; lower <= subdim; ++lower)
                for (int upper = 0; upper < dim - subdim; ++upper) {
                    Perm<subdim + 1> l = Perm<subdim + 1>::rot(lower);
                    Perm<dim - subdim> u = Perm<dim - subdim>::rot(upper);

                    EXPECT_EQ(Impl::faceNumber(ordering *
                        Perm<dim + 1>::extend(l) *
                        rev * Perm<dim + 1>::extend(u) * rev), f);
                    EXPECT_EQ(Impl::faceNumber(ordering *
                        Perm<dim + 1>::extend(l) *
                        rev * Perm<dim + 1>::extend(u.reverse()) * rev), f);
                    EXPECT_EQ(Impl::faceNumber(ordering *
                        Perm<dim + 1>::extend(l.reverse()) *
                        rev * Perm<dim + 1>::extend(u) * rev), f);
                    EXPECT_EQ(Impl::faceNumber(ordering *
                        Perm<dim + 1>::extend(l.reverse()) *
                        rev * Perm<dim + 1>::extend(u.reverse()) * rev), f);
                }
        }
    }
}

template <int dim, int... subdim>
static void faceNumberForDim(std::integer_sequence<int, subdim...>) {
    SCOPED_TRACE_NUMERIC(dim);
    (faceNumberDetail<dim, subdim>(), ...);
}

template <int... dim>
static void faceNumberAll(std::integer_sequence<int, dim...>) {
    (faceNumberForDim<dim>(subdims<dim>()), ...);
}

TEST(FaceNumberingTest, faceNumber) {
    faceNumberAll(dims());
}

template <int dim, int subdim>
static void orderingDetail() {
    SCOPED_TRACE_NUMERIC(subdim);

    for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
        SCOPED_TRACE_NUMERIC(f);
        Perm<dim + 1> p = FaceNumbering<dim, subdim>::ordering(f);

        // Verify that vertices of this face are listed in ascending order.
        for (int i = 0; i < subdim; ++i)
            EXPECT_LT(p[i], p[i + 1]);
    }
}

template <int dim, int... subdim>
static void orderingForDim(std::integer_sequence<int, subdim...>) {
    SCOPED_TRACE_NUMERIC(dim);
    (orderingDetail<dim, subdim>(), ...);
}

template <int... dim>
static void orderingAll(std::integer_sequence<int, dim...>) {
    (orderingForDim<dim>(subdims<dim>()), ...);
}

TEST(FaceNumberingTest, ordering) {
    orderingAll(dims());
}

template <int dim, int subdim>
static void containsVertexDetail() {
    SCOPED_TRACE_NUMERIC(subdim);
    using Impl = FaceNumbering<dim, subdim>;

    for (int f = 0; f < FaceNumbering<dim, subdim>::nFaces; ++f) {
        SCOPED_TRACE_NUMERIC(f);
        Perm<dim + 1> p = FaceNumbering<dim, subdim>::ordering(f);

        // Verify that this face contains vertices p[0..subdim] and
        // does not contain vertices p[(subdim+1)..dim].
        for (int v = 0; v <= subdim; ++v)
            EXPECT_TRUE(Impl::containsVertex(f, p[v]));

        for (int v = subdim + 1; v <= dim; ++v)
            EXPECT_FALSE(Impl::containsVertex(f, p[v]));
    }
}

template <int dim, int... subdim>
static void containsVertexForDim(std::integer_sequence<int, subdim...>) {
    SCOPED_TRACE_NUMERIC(dim);
    (containsVertexDetail<dim, subdim>(), ...);
}

template <int... dim>
static void containsVertexAll(std::integer_sequence<int, dim...>) {
    (containsVertexForDim<dim>(subdims<dim>()), ...);
}

TEST(FaceNumberingTest, containsVertex) {
    containsVertexAll(dims());
}

