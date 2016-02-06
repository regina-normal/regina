
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurface* NNormalSurface::findNonTrivialSphere(NTriangulation* tri) {
    if (! tri->hasBoundaryTriangles()) {
        // Switch to the optimised (and non-deprecated) verison of this routine.
        // Here we use the fact that, with no boundary triangles, there
        // cannot be any normal discs.
        return tri->hasNonTrivialSphereOrDisc();
    }

    // The older, deprecated code follows.

    // If the triangulation is already known to be 0-efficient, there
    // are no non-trivial normal 2-spheres.
    if (tri->knowsZeroEfficient() && tri->isZeroEfficient())
        return 0;

    // Construct the vertex normal surfaces and look for any spheres
    // or 1-sided projective planes.
    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(tri,
        NS_STANDARD);
    unsigned long nSurfaces = surfaces->size();

    const NNormalSurface* s;
    NLargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->surface(i);

        // No need to test for connectedness since these are vertex surfaces.
        if (s->isCompact() && (! s->hasRealBoundary()) &&
                (! s->isVertexLinking())) {
            chi = s->eulerChar();
            if (chi == 2 || (chi == 1 && ! s->isTwoSided())) {
                // It's a non-trivial 2-sphere!
                // Clone the surface for our return value.
                NNormalSurface* ans = (chi == 1 ? s->doubleSurface() :
                    s->clone());

                surfaces->makeOrphan();
                delete surfaces;
                return ans;
            }
        }
    }

    // Nothing was found.
    // Therefore there cannot be any non-trivial 2-spheres at all.
    surfaces->makeOrphan();
    delete surfaces;
    return 0;
}

NNormalSurface* NNormalSurface::findVtxOctAlmostNormalSphere(
        NTriangulation* tri, bool quadOct) {
    // This routine is deprecated.
    // However, stick with this implementation instead of running the
    // faster NTriangulation::hasOctagonalAlmostNormalSphere(), since
    // the docs for this routine insist on a vertex surface in a
    // particular coordinate system.

    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(tri, quadOct ?
        NS_AN_QUAD_OCT : NS_AN_STANDARD);
    unsigned long nSurfaces = surfaces->size();
    unsigned long nTets = tri->size();

    // Note that our surfaces are guaranteed to be in smallest possible
    // integer coordinates.
    // We are also guaranteed at most one non-zero octagonal coordinate.

    // Note that in this search a 1-sided projective plane is no good,
    // since when doubled it gives too many octagonal discs.

    const NNormalSurface* s;
    unsigned long tet;
    int oct;
    NLargeInteger octCoord;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->surface(i);

        // No need to test for connectedness since these are vertex surfaces.
        // No need to test for vertex links since we're about to test
        // for octagons.
        if (s->isCompact() && (! s->hasRealBoundary())) {
            if (s->eulerChar() == 2) {
                // Test for the existence of precisely one octagon.
                for (tet = 0; tet < nTets; tet++)
                    for (oct = 0; oct < 3; oct++)
                        if ((octCoord = s->octs(tet, oct)) > 0) {
                            // We found our one and only non-zero
                            // octagonal coordinate.
                            if (octCoord > 1) {
                                // Too many octagons.  Move along.
                                // Bail out of all our loops.
                                tet = nTets; // To escape tet loop
                                break; // To escape oct loop
                            } else {
                                // This is our almost normal 2-sphere!
                                // Clone the surface for our return value.
                                NNormalSurface* ans = s->clone();

                                surfaces->makeOrphan();
                                delete surfaces;
                                return ans;
                            }
                        }
                // Either too many octagons or none at all.
                // On to the next surface.
            }
        }
    }

    // Nothing was found.
    // Therefore there cannot be any non-trivial 2-spheres at all.
    surfaces->makeOrphan();
    delete surfaces;
    return 0;
}

} // namespace regina

