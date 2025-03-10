
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

#include <initializer_list>

#include "utilities/tightencodingtest.h"

/**
 * Implements tests for isomorphisms in dimension \a dim.
 *
 * Test suites can call these functions directly.  There is no need (or
 * benefit) for using inheritance of test fixture classes, other than the
 * minor convenience of not having to type out all the template parameters for
 * IsomorphismTest every time it is used.
 */
template <int dim>
class IsomorphismTest {
    public:
        /**
         * Run the given test on all isomorphisms of the given order.
         * If \a skip is non-zero, then only every (\a skip)th isomorphism
         * will be tested.
         *
         * The isomorphism will be passed to the test as a const reference
         * (since the test must not modify it directly).
         */
        template <typename Action>
        static void enumerate(size_t order, Action&& test, size_t skip = 0) {
            SCOPED_TRACE_NUMERIC(order);

            if (order == 0) {
                // Special-case the (unique) empty isomorphism.
                regina::Isomorphism<dim> iso(0);
                EXPECT_TRUE(iso.isIdentity());
                test(iso);
            } else {
                // Expected total number of isomorphisms: n! * [(dim+1)!]^n
                size_t expect = 1;
                for (size_t i = 1; i <= order; ++i)
                    expect = expect * i * regina::Perm<dim+1>::nPerms;

                auto iso = regina::Isomorphism<dim>::identity(order);
                EXPECT_TRUE(iso.isIdentity());

                size_t count = 0;
                do {
                    if (skip == 0 || count % skip == 0)
                        test(const_cast<const regina::Isomorphism<dim>&>(iso));
                    ++iso;
                    ++count;
                } while (! iso.isIdentity());

                EXPECT_EQ(count, expect);
            }
        }

        static void application(const regina::Triangulation<dim>& tri,
                size_t skip = 0) {
            enumerate(tri.size(), [&tri](const regina::Isomorphism<dim>& iso) {
                regina::Triangulation<dim> image = iso(tri);

                // Clear any computed properties that were copied across.
                image.newSimplex();
                image.removeSimplexAt(image.size() - 1);

                EXPECT_TRUE(tri.isIsomorphicTo(image));
                EXPECT_EQ(image.isValid(), tri.isValid());
                EXPECT_EQ(image.isClosed(), tri.isClosed());
                EXPECT_EQ(image.isOrientable(), tri.isOrientable());
                if constexpr (dim == 3)
                    EXPECT_EQ(image.isStandard(), tri.isStandard());
                EXPECT_EQ(image.homology(), tri.homology());
            }, skip);
        }

        static void inverse(size_t order) {
            for (int i = 0; i < 50; ++i) {
                auto iso = regina::Isomorphism<dim>::random(order);
                auto inv = iso.inverse();
                {
                    auto prod = inv * iso;
                    for (size_t j = 0; j < order; ++j) {
                        EXPECT_EQ(prod.simpImage(j), j);
                        EXPECT_EQ(prod.facetPerm(j), regina::Perm<dim+1>());
                    }
                }

                // Try the rvalue reference variant of composition.
                // (And, at the same time, compose the other way around.)
                {
                    auto prod = iso * iso.inverse();
                    for (size_t j = 0; j < order; ++j) {
                        EXPECT_EQ(prod.simpImage(j), j);
                        EXPECT_EQ(prod.facetPerm(j), regina::Perm<dim+1>());
                    }
                }
            }
        }

        static void automorphismsAndSubcomplexes(
                const regina::Triangulation<dim>& tri, size_t symmetries,
                const char* name) {
            // PRE: tri is connected and valid.
            SCOPED_TRACE_CSTRING(name);
            ASSERT_TRUE(tri.isConnected());
            ASSERT_TRUE(tri.isValid());

            regina::Triangulation<dim> sub(tri);

            EXPECT_TRUE(sub.isIsomorphicTo(tri));
            EXPECT_TRUE(sub.isContainedIn(tri));

            {
                size_t count = 0;
                sub.findAllSubcomplexesIn(tri, [&count](
                        const regina::Isomorphism<3>&){
                    ++count;
                    return false;
                });
                EXPECT_EQ(count, symmetries);
            }
            {
                size_t count = 0;
                sub.findAllIsomorphisms(tri, [&count](
                        const regina::Isomorphism<3>&){
                    ++count;
                    return false;
                });
                EXPECT_EQ(count, symmetries);
            }

            // Some of these tests cannot be run on the standalone simplex.
            bool isolated = (tri.size() == 1 &&
                tri.template countFaces<dim - 1>() == dim + 1);

            // Unglue a face of sub.
            if (! isolated) {
                for (auto f : sub.template faces<dim - 1>()) {
                    if (f->degree() == 2) {
                        auto emb = f->front();
                        emb.simplex()->unjoin(emb.face());
                        break;
                    }
                }
                EXPECT_TRUE(sub.isContainedIn(tri));
                EXPECT_FALSE(tri.isContainedIn(sub));
            }

            // Completely remove a top-dimensional simplex of sub.
            sub.removeSimplexAt(0);
            EXPECT_TRUE(sub.isContainedIn(tri));
            EXPECT_FALSE(tri.isContainedIn(sub));

            // Add back a lone simplex.
            auto simp = sub.newSimplex();
            EXPECT_TRUE(sub.isContainedIn(tri));
            if (! isolated)
                EXPECT_FALSE(tri.isContainedIn(sub));

            if constexpr (dim > 2) {
                // Make it no longer a subcomplex.
                // Do this by joining things together in a way that creates
                // an invalid edge.
                simp->join(0, simp,
                    regina::Perm<dim+1>(0, 1) * regina::Perm<dim+1>(2, 3));
                EXPECT_FALSE(sub.isContainedIn(tri));
                if (! isolated)
                    EXPECT_FALSE(tri.isContainedIn(sub));
            }
        }

        static void tightEncoding(size_t maxOrder) {
            using Impl = TightEncodingTest<regina::Isomorphism<dim>>;

            for (size_t i = 0; i <= maxOrder; ++i)
                enumerate(i, Impl::verifyTightEncoding);

            // A case where the isomorphism includes higher-numbered
            // simplex images, and also uninitialised simplex images:
            {
                regina::Isomorphism<dim> iso(2);
                iso.simpImage(0) = -1;
                iso.simpImage(1) = 3;
                iso.facetPerm(0) = regina::Perm<dim+1>::rot(dim-1);
                iso.facetPerm(1) = regina::Perm<dim+1>::rot(1);
                Impl::verifyTightEncoding(iso);
            }
        }
};
