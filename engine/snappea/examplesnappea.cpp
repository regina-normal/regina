
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"

namespace {
}

namespace regina {

SnapPeaTriangulation* ExampleSnapPea::figureEight() {
    SnapPeaTriangulation* ans = new SnapPeaTriangulation(
"% Triangulation\n"
"m004\n"
"geometric_solution  2.02988321\n"
"oriented_manifold\n"
"CS_unknown\n"
"\n"
"1 0\n"
"    torus   0.000000000000   0.000000000000\n"
"\n"
"2\n"
"   1    1    1    1 \n"
" 0132 1230 2310 2103\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0 -1  1  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  0  1  1  0 -1  0  0  1  0 -1 -1  0  1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   0    0    0    0 \n"
" 0132 3201 3012 2103\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0  0  0  0  0  1  0 -1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  0  1 -1  0  1  0  1  0  0 -1  0  1 -1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n");

    ans->setLabel("Figure eight knot complement");
    return ans;
}

SnapPeaTriangulation* ExampleSnapPea::trefoil() {
    SnapPeaTriangulation* ans = new SnapPeaTriangulation(
"% Triangulation\n"
"Trefoil\n"
"flat_solution  0.00000000\n"
"oriented_manifold\n"
"CS_unknown\n"
"\n"
"1 0\n"
"    torus   0.000000000000   0.000000000000\n"
"\n"
"2\n"
"   1    1    1    1 \n"
" 2031 0132 0132 3120\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0 -1  1  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  1  0 -1 -1  0  0  1  3 -2  0 -1 -1  1  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.000000000000\n"
"\n"
"   0    0    0    0 \n"
" 3120 0132 1302 0132\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0 -1  1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  1  0 -1  0  1  0  1 -1  0  0  1  2 -3  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  2.000000000000   0.000000000000\n");

    ans->setLabel("Trefoil knot complement");
    return ans;
}

SnapPeaTriangulation* ExampleSnapPea::whiteheadLink() {
    SnapPeaTriangulation* ans = new SnapPeaTriangulation(
"% Triangulation\n"
"m129\n"
"geometric_solution  3.66386238\n"
"oriented_manifold\n"
"CS_unknown\n"
"\n"
"2 0\n"
"    torus   0.000000000000   0.000000000000\n"
"    torus   0.000000000000   0.000000000000\n"
"\n"
"4\n"
"   1    2    3    1 \n"
" 0132 0132 0132 3201\n"
"   0    1    0    1 \n"
"  0  1 -1  0 -1  0  1  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  0  1  0  0 -1  1 -1  0  0  1  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  1.000000000000   1.000000000000\n"
"\n"
"   0    0    3    2 \n"
" 0132 2310 3120 3120\n"
"   0    1    1    0 \n"
"  0  0  0  0  1  0 -1  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  1  0  0  0  0  0  0 -1  0  1  1 -1  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.500000000000\n"
"\n"
"   1    0    3    3 \n"
" 3120 0132 0213 3120\n"
"   0    1    1    0 \n"
"  0 -1  1  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  1  0 -1  0  0 -1  1 -1  0  0  1  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.500000000000\n"
"\n"
"   2    2    1    0 \n"
" 3120 0213 3120 0132\n"
"   0    1    1    0 \n"
"  0 -1  0  1  0  0  1 -1  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0 -1  0  0  1 -1  1  0  0  1  0 -1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.500000000000\n");

    ans->setLabel("Whitehead link complement");
    return ans;
}

SnapPeaTriangulation* ExampleSnapPea::gieseking() {
    SnapPeaTriangulation* ans = new SnapPeaTriangulation(
"% Triangulation\n"
"m000\n"
"geometric_solution  1.01494161\n"
"nonorientable_manifold\n"
"CS_unknown\n"
"\n"
"0 1\n"
"    Klein   0.000000000000   0.000000000000\n"
"\n"
"1\n"
"   0    0    0    0 \n"
" 1320 3021 2130 3102\n"
"   0    0    0    0 \n"
"  0  0 -1  1 -1  0  1  0  1  0  0 -1  0 -1  1  0\n"
"  0  0  1 -1  1  0  0 -1  0 -1  0  1  0  1 -1  0\n"
"  0  0  0  0  0  0  0  0  1 -1  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  1 -1  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n");

    ans->setLabel("Gieseking manifold");
    return ans;
}

SnapPeaTriangulation* ExampleSnapPea::x101() {
    SnapPeaTriangulation* ans = new SnapPeaTriangulation(
"% Triangulation\n"
"x101\n"
"geometric_solution  5.07470803\n"
"nonorientable_manifold\n"
"CS_unknown\n"
"\n"
"0 1\n"
"    Klein   0.000000000000   0.000000000000\n"
"\n"
"6\n"
"   1    2    3    4 \n"
" 0132 0132 0132 0132\n"
"   0    0    0    0 \n"
"  0 -1  0  1  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  1 -1  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   0    5    2    2 \n"
" 0132 0132 1032 1302\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0  0 -1  0  1  0  0  0  0\n"
"  0 -1  0  1  0  0  0  0  0  1  0 -1  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   5    0    1    1 \n"
" 0132 0132 2031 1032\n"
"   0    0    0    0 \n"
"  0  1 -1  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0 -1  1  0  1  0 -1  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   4    4    5    0 \n"
" 1302 1032 0123 0132\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  1 -1  0\n"
"  0 -1  0  1  0  0  0  0  0  0  0  0 -1  0  1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0 -1  1  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0 -1  1  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   3    3    0    5 \n"
" 1032 2031 0132 0123\n"
"   0    0    0    0 \n"
"  0  0 -1  1  1  0  0 -1  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0 -1  1  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0 -1  1  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0 -1  1  0  0  0  0  0  0\n"
"  0.500000000000   0.866025403784\n"
"\n"
"   2    1    3    4 \n"
" 0132 0132 0123 0123\n"
"   0    0    0    0 \n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  1 -1  0\n"
"  0  1  0 -1 -1  0  0  1  0  0  0  0  0 -1  1  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
"  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0\n"
" -1.000000000000  -0.000000000000\n");

    ans->setLabel("x101");
    return ans;
}

}
