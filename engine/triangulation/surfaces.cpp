
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "triangulation/ntriangulation.h"
#include "surfaces/nnormalsurfacelist.h"

namespace regina {

/**
 * When testing 0-efficiency, to prove that a normal 2-sphere must occur
 * at a vertex we use Euler characteristic arguments.  One issue that
 * arises for non-orientable 3-manifolds is whether a non-vertex normal
 * 2-sphere can be decomposed into two-sided projective planes and other
 * surfaces of non-positive Euler characteristic.  On this issue, Jaco
 * writes:
 *
 *  "Remember that in any 3-manifold, regular curves of intersection between
 *   normal surfaces are orientation preserving; thus if you add a two-sided
 *   projective plane to any other surface, the curves of intersection must
 *   be trivial curves on the projective plane - thus the result must be
 *   nonorientable."
 */

/**
 * In general, in quad space the decomposition of a normal surface S is:
 *
 *   k * S + sum of vertex links = sum of vertex normal surfaces
 *
 * If S is a 2-sphere or a disc and every vertex link has non-negative
 * Euler characteristic, it follows that at least one vertex normal surface
 * has positive Euler characteristic.  From the above comments we see
 * that this vertex normal surface must be a 2-sphere, a disc or a
 * 1-sided projective plane (which doubles to a 2-sphere).
 *
 * Thus we can test 0-efficiency in quad space if all vertex links have
 * non-negative Euler characteristic.
 */

/**
 * Splitting surfaces must alas be tested for in standard triangle-quad
 * coordinates.  See the triangulation J_{1|3,-5} (chained triangular
 * solid torus of major type) of S^3 / Q_32 x Z_3 an an example of a
 * triangulation with a splitting surface having chi=-1 that can be
 * decomposed in quad space as the sum of two vertex normal tori minus a
 * vertex link.
 */

bool NTriangulation::isZeroEfficient() {
    if (! zeroEfficient.known()) {
        if (hasTwoSphereBoundaryComponents()) {
            // We have 2-sphere boundary components.
            // No need to look through normal surfaces.
            zeroEfficient = false;
        } else if (isValid() && ! hasNegativeIdealBoundaryComponents()) {
            // We can calculate this using normal surfaces in quad space.
            calculateQuadSurfaceProperties();
        } else {
            // We have to use the slower tri-quad coordinates.
            calculateStandardSurfaceProperties();
        }
    }
    return zeroEfficient.value();
}

bool NTriangulation::hasSplittingSurface() {
    // Splitting surfaces must unfortunately be calculated using
    // tri-quad coordinates.
    if (! splittingSurface.known())
        calculateStandardSurfaceProperties();
    return splittingSurface.value();
}

void NTriangulation::calculateQuadSurfaceProperties() {
    // Create a normal surface list.
    //
    // Work on a clone of this triangulation so we don't trigger any
    // changes to the packet tree.
    NTriangulation working(*this);
    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(&working,
        NNormalSurfaceList::QUAD);

    // All we can test here is 0-efficiency.

    // Are we allowed to calculate 0-efficiency using quad coordinates?
    if ((! isValid()) || hasNegativeIdealBoundaryComponents())
        return;

    // Run through all vertex surfaces.
    unsigned long nSurfaces = surfaces->getNumberOfSurfaces();
    const NNormalSurface* s;
    NLargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->getSurface(i);

        if (! zeroEfficient.known()) {
            // Note that all vertex surfaces in quad space are
            // connected and non-vertex-linking.

            if (s->isCompact()) {
                chi = s->getEulerCharacteristic();
                if (s->hasRealBoundary()) {
                    // Hunt for discs.
                    if (chi == 1)
                        zeroEfficient = false;
                } else {
                    // Hunt for spheres.
                    if (chi == 2)
                        zeroEfficient = false;
                    else if (chi == 1 && s->isTwoSided().isFalse())
                        zeroEfficient = false;
                }
            }
        }

        // See if there is no use running through the rest of the list.
        if (zeroEfficient.known())
            break;
    }

    // Done!
    if (! zeroEfficient.known())
        zeroEfficient = true;

    // The stack will clean things up for us automatically.
}

void NTriangulation::calculateStandardSurfaceProperties() {
    // Create a normal surface list.
    //
    // Work on a clone of this triangulation so we don't trigger any
    // changes to the packet tree.
    NTriangulation working(*this);
    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(&working,
        NNormalSurfaceList::STANDARD);

    // Run through all vertex surfaces.
    unsigned long nSurfaces = surfaces->getNumberOfSurfaces();
    const NNormalSurface* s;
    NLargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->getSurface(i);

        if (! splittingSurface.known())
            if (s->isSplitting())
                splittingSurface = true;

        if (! zeroEfficient.known())
            if (! s->isVertexLinking()) {
                // No need to test for connectedness since these are
                // vertex normal surfaces.
                // No need to test for compactness since we're using
                // standard tri-quad coordinates.
                chi = s->getEulerCharacteristic();
                if (s->hasRealBoundary()) {
                    // Hunt for discs.
                    if (chi == 1)
                        zeroEfficient = false;
                } else {
                    // Hunt for spheres.
                    if (chi == 2)
                        zeroEfficient = false;
                    else if (chi == 1 && s->isTwoSided().isFalse())
                        zeroEfficient = false;
                }
            }

        // See if there is no use running through the rest of the list.
        if (zeroEfficient.known() && splittingSurface.known())
            break;
    }

    // Done!
    if (! zeroEfficient.known())
        zeroEfficient = true;
    if (! splittingSurface.known())
        splittingSurface = false;

    // The stack will clean things up for us automatically.
}

} // namespace regina

