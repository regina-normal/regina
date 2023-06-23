
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

#include "triangulation/example3.h"

#include "generic/isomorphismtest.h"

using regina::Example;

TEST(Isomorphism3Test, application) {
    IsomorphismTest<3>::application(Example<3>::rp2xs1(), 11);
}

TEST(Isomorphism3Test, inverse) {
    IsomorphismTest<3>::inverse(5);
}

TEST(Isomorphism3Test, automorphismsAndSubcomplexes) {
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::ball(), 24, "Ball");
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::lens(8, 1), 4, "L(8,1)");
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::lens(13, 3), 2, "L(13,3)");
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::layeredLoop(5, true), 20, "C~(5)");
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::layeredLoop(5, false), 20, "C(5)");

    // A case with no non-trivial symmetries.
    IsomorphismTest<3>::automorphismsAndSubcomplexes(
        Example<3>::augTriSolidTorus(3, -1, 5, -3, 2, -1), 1, "A(3,-1 | 5,-3)");
}

TEST(Isomorphism3Test, tightEncoding) {
    IsomorphismTest<3>::tightEncoding(3);
}
