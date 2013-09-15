
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include "enumerate/ntreetraversal.h"
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
 * In the general case, 0-efficiency must be tested for in standard
 * triangle-quad coordinates.  For example, the triangulation with
 * isosig dLQacccbnjk (which is ideal with one torus cusp) is
 * not 0-efficient, but the non-trivial sphere does not appear as a
 * vertex in quad coordinates.
 */

/**
 * Splitting surfaces must also be tested for in standard triangle-quad
 * coordinates.  See the triangulation J_{1|3,-5} (chained triangular
 * solid torus of major type) of S^3 / Q_32 x Z_3 an an example of a
 * triangulation with a splitting surface having chi=-1 that can be
 * decomposed in quad space as the sum of two vertex normal tori minus a
 * vertex link.
 */

NNormalSurface* NTriangulation::hasNonTrivialSphereOrDisc() {
    // Get the empty triangulation out of the way now.
    if (tetrahedra.empty())
        return 0;

    // Do we already know the answer?
    if (zeroEfficient.known() && zeroEfficient.value())
        return 0;

    // Use combinatorial optimisation if we can.
    if (isValid() && vertices.size() == 1) {
        // For now, just use the safe arbitrary-precision NInteger type.
        NTreeSingleSoln<LPConstraintEuler> tree(this, NS_STANDARD);
        if (tree.find()) {
            NNormalSurface* s = tree.buildSurface();
            if (! ((! s->hasRealBoundary()) &&
                    (s->getEulerCharacteristic() == 1) && s->isTwoSided()))
                return s;
            // Looks like we've found a two-sided projective plane.
            // Fall through to a full enumeration of vertex surfaces.
            delete s;
        } else
            return 0;
    }

    // Fall back to a slow-but-general method: enumerate all vertex surfaces.
    // For valid, non-ideal triangulations we can do this in quad
    // coordinates (where a non-trivial sphere or disc is guaranteed to
    // appear as a vertex surface).  Otherwise fall back to standard coords.
    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(this,
        (isValid() && ! isIdeal()) ? NS_QUAD : NS_STANDARD);
    const NNormalSurface* s;
    NNormalSurface* ans = 0;
    for (size_t i = 0; i < surfaces->getNumberOfSurfaces() && ! ans; ++i) {
        s = surfaces->getSurface(i);

        // These are vertex surfaces, so we know they must be connected.
        // Because we are either (i) using standard coordinates, or
        // (ii) working with a non-ideal triangulation; we know the
        // vertex surfaces are compact also.

        if (s->isVertexLinking())
            continue;

        // Now they are compact, connected and non-vertex-linking.
        // We just need to pick out spheres and discs.
        if (s->getEulerCharacteristic() == 2) {
            // Must be a sphere; no bounded surface has chi=2.
            ans = s->clone();
        } else if (s->getEulerCharacteristic() == 1) {
            if (s->hasRealBoundary()) {
                // Must be a disc.
                ans = s->clone();
            } else if (! s->isTwoSided()) {
                // A projective plane that doubles to a sphere.
                ans = s->doubleSurface();
            }
        }
    }

    delete surfaces;
    return ans;
}

bool NTriangulation::isZeroEfficient() {
    if (! zeroEfficient.known()) {
        if (hasTwoSphereBoundaryComponents())
            zeroEfficient = false;
        else {
            // Operate on a clone of this triangulation, to avoid
            // changing the real packet tree.
            NTriangulation clone(*this);
            NNormalSurface* s = clone.hasNonTrivialSphereOrDisc();
            if (s) {
                zeroEfficient = false;
                delete s;
            } else
                zeroEfficient = true;
        }
    }
    return zeroEfficient.value();
}

bool NTriangulation::hasSplittingSurface() {
    // Splitting surfaces must unfortunately be calculated using
    // tri-quad coordinates.
    if (splittingSurface.known())
        return splittingSurface.value();

    // Create a normal surface list.
    //
    // Work on a clone of this triangulation so we don't trigger any
    // changes to the packet tree.
    NTriangulation working(*this);
    NNormalSurfaceList* surfaces = NNormalSurfaceList::enumerate(&working,
        NS_STANDARD);

    // Run through all vertex surfaces.
    unsigned long nSurfaces = surfaces->getNumberOfSurfaces();
    const NNormalSurface* s;
    NLargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->getSurface(i);

        if (! splittingSurface.known())
            if (s->isSplitting())
                splittingSurface = true;

        // See if there is no use running through the rest of the list.
        if (splittingSurface.known())
            break;
    }

    // Done!
    if (! splittingSurface.known())
        splittingSurface = false;

    // The stack will clean things up for us automatically.
    return splittingSurface.value();
}

} // namespace regina

