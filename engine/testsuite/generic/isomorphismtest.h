
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
         * The given array \a counts should list the total number of
         * isomorphisms of size 0, 1, 2, etc., up to whatever size we are
         * willing to enumerate programatically as the test suite runs.
         *
         * Specifically, `counts[k]` should be `k! * [(dim+1)!]^k`.
         */
        static void tightEncoding(std::initializer_list<size_t> counts) {
            using Impl = TightEncodingTest<regina::Isomorphism<dim>>;

            // Exhaustive run through all small isomorphisms (we use
            // both odd and even sizes here since permutations are
            // encoded in pairs):
            size_t n = 0;
            for (size_t expect : counts) {
                SCOPED_TRACE_NUMERIC(n);
                if (n == 0) {
                    // Special-case the (unique) empty isomorphism.
                    EXPECT_EQ(expect, 1);
                    Impl::verifyTightEncoding(regina::Isomorphism<dim>(0));
                } else {
                    auto iso = regina::Isomorphism<dim>::identity(n);
                    size_t count = 0;
                    do {
                        Impl::verifyTightEncoding(iso);
                        ++count;
                        ++iso;
                    } while (! iso.isIdentity());
                    EXPECT_EQ(count, expect);
                }
                ++n;
            }

            // Cases where the isomorphism includes higher-numbered
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
