
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Alex He                                      *
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

#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testhelper.h"

using regina::Example;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::NormalCoords;
using regina::Triangulation;

static void compareBoundaryCounts(
        NormalSurfaces surfs, std::vector<size_t> expect ) {
    surfs.sort( []( NormalSurface a, NormalSurface b ){ return a < b; } );
    for (size_t i = 0; i < surfs.size(); ++i) {
        EXPECT_EQ( expect[i], surfs.surface(i).countBoundaries() );
    }
}

TEST(BoundariesTest, countBoundaries) {
    // One-tetrahedron layered solid torus
    //
    // The expected boundary-counts for this example have been checked by
    // hand.
    Triangulation<3> solidTorus = Triangulation<3>::fromIsoSig(
            "bGaj" );
    compareBoundaryCounts(
        NormalSurfaces( solidTorus, NormalCoords::Quad ),
        { 1, 1, 2 } );

    // Solid torus with an internal vertex
    //
    // The point of this example is that, if we enumerate in standard
    // coordinates, then we include a sanity check that countBoundaries()
    // returns 0 for a closed surface.
    //
    // Because this example has minimal (two-triangle) torus boundary, the
    // number of boundary components of a normal surface can be independently
    // calculated from the GCD of the normal arcs on the boundary. The
    // expected boundary-counts have been manually checked to coincide with
    // these GCDs.
    Triangulation<3> extraVertex = Triangulation<3>::fromIsoSig(
            "eLHkccddpvvo" );
    compareBoundaryCounts(
        NormalSurfaces( extraVertex, NormalCoords::Standard ),
        { 1, 1, 1, 1, 2, 1, 0, 1, 1, 2, 1, 2, 1, 2, 1, 1 } );

    // Genus-2 handlebody
    //
    // Example with more than two boundary triangles.
    //
    // The expected boundary-counts haven't been checked manually, but this
    // test at least ensures that countBoundaries() returns consistent
    // (presumably correct) answers even if the implementation is modified.
    Triangulation<3> handle2 = Triangulation<3>::fromIsoSig(
            "eHbKabdel" );
    compareBoundaryCounts(
        NormalSurfaces( handle2, NormalCoords::Quad ),
        { 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1 } );
}
