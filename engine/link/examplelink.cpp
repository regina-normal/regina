
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "link/examplelink.h"
#include "link/link.h"

namespace regina {

Link* ExampleLink::unknot() {
    Link* ans = new Link(1);
    ans->setLabel("Unknot");
    return ans;
}

Link* ExampleLink::trefoilLeft() {
    Link* ans = Link::fromData({ -1, -1, -1 }, { 1, -2, 3, -1, 2, -3 });
    ans->setLabel("Left-hand trefoil");
    return ans;
}

Link* ExampleLink::trefoilRight() {
    Link* ans = Link::fromData({ +1, +1, +1 }, { 1, -2, 3, -1, 2, -3 });
    ans->setLabel("Right-hand trefoil");
    return ans;
}

Link* ExampleLink::trefoil() {
    Link* ans = Link::fromData({ +1, +1, +1 }, { 1, -2, 3, -1, 2, -3 });
    ans->setLabel("Trefoil");
    return ans;
}

Link* ExampleLink::figureEight() {
    Link* ans = Link::fromData({ +1, +1, -1, -1 },
        { -1, 2, -3, 4, -2, 1, -4, 3});
    ans->setLabel("Figure eight knot");
    return ans;
}

Link* ExampleLink::hopf() {
    Link* ans = Link::fromData({ +1, +1 }, { 1, -2 }, { -1, 2 });
    ans->setLabel("Hopf link");
    return ans;
}

Link* ExampleLink::whitehead() {
    Link* ans = Link::fromData({ -1, -1, +1, +1, -1 },
        { 1, -2, 5, -4, 3, -5 }, { -1, 2, -3, 4 });
    ans->setLabel("Whitehead link");
    return ans;
}

Link* ExampleLink::borromean() {
    Link* ans = Link::fromData({ +1, -1, +1, -1, +1, -1 },
        { 1, -2, 4, -5 }, { 3, -4, 6, -1 }, { 5, -6, 2, -3 });
    ans->setLabel("Borromean rings");
    return ans;
}

Link* ExampleLink::monster() {
    Link* ans = Link::fromData({ -1, -1, +1, -1, +1, +1, +1, +1, +1, -1 },
        { 1, -2, 3, 4, 5, -6, 7, -8, 9, -3, 10, -1, 2, -10, -4, -9, 8, -5,
          6, -7 });
    ans->setLabel("Monster unknot");
    return ans;
}

Link* ExampleLink::kinoshitaTerasaka() {
    Link* ans = Link::fromData({ -1, -1, +1, -1, -1, +1, -1, +1, +1, +1, -1 },
        { 1, -2, 3, 4, -5, -6, 2, -7, -8, 9, -4, 5, -10, 8, -9, 10, 11,
          -1, 6, -3, 7, -11 });
    ans->setLabel("Kinoshita-Terasaka knot");
    return ans;
}

Link* ExampleLink::conway() {
    Link* ans = Link::fromData({ +1, -1, -1, -1, -1, -1, -1, +1, +1, +1, +1 },
        { 1, -2, 3, 4, -5, -6, 7, -1, 8, -3, 6, -7, 2, -8, -9, 10, -11,
          9, -4, 5, -10, 11 });
    ans->setLabel("Conway knot");
    return ans;
}

Link* ExampleLink::gordian() {
    Link* ans = Link::fromData(
        { +1, -1, +1, +1, -1, +1, +1, -1, -1, +1, +1, -1, +1, +1, +1,
          -1, -1, -1, -1, -1, -1, +1, -1, -1, -1, -1, -1, -1, -1, -1,
          -1, -1, -1, +1, -1, +1, -1, +1, +1, -1, +1, +1, -1, -1, -1,
          -1, +1, +1, -1, -1, +1, -1, +1, -1, -1, -1, -1, -1, +1, -1,
          -1, +1, -1, -1, +1, -1, -1, -1, -1, -1, -1, +1, +1, +1, -1,
          +1, +1, +1, +1, -1, -1, +1, -1, +1, +1, +1, -1, -1, +1, -1,
          -1, +1, -1, +1, +1, +1, +1, +1, -1, +1, +1, +1, +1, -1, -1,
          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, +1, -1, -1,
          +1, -1, -1, -1, -1, -1, +1, -1, -1, -1, -1, -1, -1, +1, -1,
          +1, +1, +1, -1, +1, -1 },
        { -1, -2, 3, -4, 5, 6, 7, 8, -9, -10, 11, 12, 13, -14, 15, -16, 17,
          18, -19, 20, -21, -7, 22, 23, 24, -25, -26, -27, 28, -29, 30, 31,
          -32, -33, -34, 35, 36, -37, -38, -39, -40, -41, 42, 43, -44, 45,
          46, 47, -48, -49, -50, 38, -51, 52, -53, -54, -55, -42, -56, 57, 58,
          -46, -59, -60, -61, -62, 63, -64, -65, -66, -67, 68, -35, -69, 70,
          -71, -72, 73, 29, -74, -31, -75, -76, 77, 69, -36, -68, -78, -79,
          -80, 64, -63, -81, -82, 60, -83, -47, 84, -57, -85, -43, 55, -86,
          -87, -52, -88, 37, -89, 49, -90, -84, -58, -45, 44, 85, 56, -91,
          -92, 39, 51, 88, -70, -77, -93, 33, -94, 74, -30, -73, 95, 27, -96,
          -97, 98, -23, 99, -8, 21, 100, -101, -18, 102, -103, -15, -104, 105,
          -12, 106, -107, 9, -99, -22, 108, -5, -109, 110, 2, -111, -112, 113,
          -114, -28, -95, 72, 115, -116, 75, 32, 94, 117, 78, -118, 66, -119,
          120, 121, 83, 59, 10, 107, -122, -123, 124, 103, 16, 125, -110, -3,
          126, 127, -98, -24, 54, 86, -128, 40, 92, 129, 90, 48, -121, -130,
          62, 81, -131, 104, 14, 132, -133, -124, -134, 19, 101, 135, -6,
          -108, -127, 136, 137, 114, -113, -138, 139, -126, 4, 109, -135,
          -100, -20, 140, 123, 133, -132, -13, -105, 131, 141, 79, 118, 67,
          89, 50, -129, 91, 41, 128, 87, 53, 25, 97, -136, -139, 111, 1, -125,
          -17, -102, 134, -140, 122, -106, -11, 82, 61, 130, -120, 119, 65,
          80, -141, -117, 34, 93, 76, 116, -115, 71, 26, 96, -137, 138, 112 });
    ans->setLabel("Gordian unknot");
    return ans;
}

Link* ExampleLink::torus(int p, int q) {
    Link* ans = new Link;
    ans->insertTorusLink(p, q);
    return ans;
}

Link* ExampleLink::gst() {
    Link* ans = Link::fromData(
        { -1, -1, -1, +1, +1, -1, -1, -1, -1, -1, +1, -1, +1, +1, +1, -1,
          -1, -1, +1, -1, +1, +1, +1, -1, +1, -1, +1, +1, +1, -1, -1, +1,
          -1, -1, +1, +1, +1, +1, +1, +1, -1, -1, -1, -1, +1, +1, +1, +1 },
        { -1, -2, -3, -4, 5, 6, 7, 8, -9, -10, 11, -12,
          -13, -14, 15, 16, 2, -7, 17, 9, -18, 19, -20, -21,
          -22, 23, 24, 1, -8, -17, 10, 18, 25, -26, -27, -28,
          29, 30, -31, -32, -33, 27, 21, 13, 34, -5, 4, -15,
          -23, -29, 32, 35, 36, -37, -38, 33, 28, 22, 14, -34,
          -39, -11, -19, -25, 40, -41, -42, 43, 44, 31, -30, -24,
          -16, 3, -6, 39, 12, 20, 26, -40, 45, 46, -43, -36,
          37, 42, -46, -47, 48, -44, -35, 38, 41, -45, 47, -48 });
    ans->setLabel("Gompf-Scharlemann-Thompson");
    return ans;
}

} // namespace regina

