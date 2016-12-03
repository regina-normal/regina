
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

#include "enumerate/treetraversal.h"
#include "triangulation/dim3.h"
#include "surfaces/normalsurfaces.h"

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

NormalSurface* Triangulation<3>::hasNonTrivialSphereOrDisc() {
    // Get the empty triangulation out of the way now.
    if (simplices_.empty())
        return 0;

    // Do we already know the answer?
    if (zeroEfficient_.known() && zeroEfficient_.value())
        return 0;

    // Use combinatorial optimisation if we can.
    if (isValid() && countVertices() == 1) {
        // For now, just use the safe arbitrary-precision Integer type.
        TreeSingleSoln<LPConstraintEuler> tree(this, NS_STANDARD);
        if (tree.find()) {
            NormalSurface* s = tree.buildSurface();
            if (! ((! s->hasRealBoundary()) &&
                    (s->eulerChar() == 1) && s->isTwoSided()))
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
    NormalSurfaces* surfaces = NormalSurfaces::enumerate(this,
        (isValid() && ! isIdeal()) ? NS_QUAD : NS_STANDARD);
    const NormalSurface* s;
    NormalSurface* ans = 0;
    for (size_t i = 0; i < surfaces->size() && ! ans; ++i) {
        s = surfaces->surface(i);

        // These are vertex surfaces, so we know they must be connected.
        // Because we are either (i) using standard coordinates, or
        // (ii) working with a non-ideal triangulation; we know the
        // vertex surfaces are compact also.

        if (s->isVertexLinking())
            continue;

        // Now they are compact, connected and non-vertex-linking.
        // We just need to pick out spheres and discs.
        if (s->eulerChar() == 2) {
            // Must be a sphere; no bounded surface has chi=2.
            ans = s->clone();
        } else if (s->eulerChar() == 1) {
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

NormalSurface* Triangulation<3>::hasOctagonalAlmostNormalSphere() {
    // Get the empty triangulation out of the way now.
    if (simplices_.empty())
        return 0;

    // Use combinatorial optimisation if we can.
    // This is good for large problems, but for small problems a full
    // enumeration is usually faster.  Still, the big problems are the
    // ones we need to be more fussy about.
    if (countVertices() == 1) {
        // For now, just use the safe arbitrary-precision Integer type.
        TreeSingleSoln<LPConstraintEuler> tree(this, NS_AN_STANDARD);
        if (tree.find()) {
            // Since our preconditions ensure the triangulation is
            // closed, orientable and 0-efficient, there are no
            // non-vertex-linking normal surfaces with positive Euler
            // characteristic.  Our optimisation asks for (Euler - #octs) > 0,
            // which then implies that our surface here is almost normal
            // with exactly 1 octagon and Euler = 2.  This is exactly
            // what we're looking for.
            NormalSurface* s = tree.buildSurface();
            return s;
        } else
            return 0;
    }

    // Fall back to a slow-but-general method: enumerate all vertex surfaces.
    // Given our preconditions, we can do this in quadrilateral-octagon
    // coordinates; for details see "Quadrilateral-octagon coordinates for
    // almost normal surfaces", B.B., Experiment. Math. 19 (2010), 285-315.
    NormalSurfaces* surfaces = NormalSurfaces::enumerate(this,
        NS_AN_QUAD_OCT);

    // Our vertex surfaces are guaranteed to be in smallest possible
    // integer coordinates, with at most one non-zero octagonal coordinate.
    const NormalSurface* s;
    NormalSurface* ans = 0;
    unsigned long tet;
    unsigned oct;
    bool found, broken;
    LargeInteger coord;
    for (size_t i = 0; i < surfaces->size() && ! ans; ++i) {
        s = surfaces->surface(i);

        // These are vertex surfaces, so we know they must be connected.
        // Because we are working with a non-ideal triangulation, we know the
        // vertex surfaces are compact.

        // Hunt for spheres with exactly one octagon.
        // Note that 1-sided projective planes are no good here,
        // since when doubled they give too many octagonal discs.
        if (s->eulerChar() == 2) {
            // Euler char = 2 implies no real boundary.
            found = false; // At least one octagon found so far?
            broken = false; // More than one octagon found so far?
            for (tet = 0; tet < simplices_.size() && ! broken; ++tet)
                for (oct = 0; oct < 3; ++oct) {
                    coord = s->octs(tet, oct);
                    if (coord > 1) {
                        broken = true;
                        break;
                    } else if (coord == 1) {
                        if (found) {
                            broken = true;
                            break;
                        } else
                            found = true;
                    }
                }
            if (found && ! broken) {
                // This is it!
                ans = s->clone();
            }
        }
    }

    delete surfaces;
    return ans;
}

bool Triangulation<3>::isZeroEfficient() {
    if (! zeroEfficient_.known()) {
        if (hasTwoSphereBoundaryComponents())
            zeroEfficient_ = false;
        else {
            // Operate on a clone of this triangulation, to avoid
            // changing the real packet tree.
            Triangulation<3> clone(*this, false);
            NormalSurface* s = clone.hasNonTrivialSphereOrDisc();
            if (s) {
                zeroEfficient_ = false;
                delete s;
            } else {
                zeroEfficient_ = true;

                // Things implied by 0-efficiency:
                if (isValid() && isClosed() && isConnected())
                    irreducible_ = true;
            }
        }
    }
    return zeroEfficient_.value();
}

bool Triangulation<3>::hasSplittingSurface() {
    // Splitting surfaces must unfortunately be calculated using
    // tri-quad coordinates.
    if (splittingSurface_.known())
        return splittingSurface_.value();

    // Create a normal surface list.
    //
    // Work on a clone of this triangulation so we don't trigger any
    // changes to the packet tree.
    Triangulation<3> working(*this, false);
    NormalSurfaces* surfaces = NormalSurfaces::enumerate(&working,
        NS_STANDARD);

    // Run through all vertex surfaces.
    unsigned long nSurfaces = surfaces->size();
    const NormalSurface* s;
    LargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = surfaces->surface(i);

        if (! splittingSurface_.known())
            if (s->isSplitting())
                splittingSurface_ = true;

        // See if there is no use running through the rest of the list.
        if (splittingSurface_.known())
            break;
    }

    // Done!
    if (! splittingSurface_.known())
        splittingSurface_ = false;

    // The stack will clean things up for us automatically.
    return splittingSurface_.value();
}

} // namespace regina

