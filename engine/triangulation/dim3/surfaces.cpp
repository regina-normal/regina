
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

#include "enumerate/treetraversal.h"
#include "packet/container.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

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

std::optional<NormalSurface> Triangulation<3>::nonTrivialSphereOrDisc() const {
    // Get the empty triangulation out of the way now.
    if (simplices_.empty())
        return std::nullopt;

    // Do we already know the answer?
    if (prop_.zeroEfficient_.has_value() && *prop_.zeroEfficient_)
        return std::nullopt;

    // Use combinatorial optimisation if we can.
    if (isValid() && countVertices() == 1) {
        // For now, just use the safe arbitrary-precision Integer type.
        TreeSingleSoln<LPConstraintEulerPositive> tree(*this, NS_STANDARD);
        if (tree.find()) {
            NormalSurface s = tree.buildSurface();
            if (! ((! s.hasRealBoundary()) &&
                    (s.eulerChar() == 1) && s.isTwoSided()))
                return s;
            // Looks like we've found a two-sided projective plane.
            // Fall through to a full enumeration of vertex surfaces.
        } else
            return std::nullopt;
    }

    // Fall back to a slow-but-general method: enumerate all vertex surfaces.
    // For valid, non-ideal triangulations we can do this in quad
    // coordinates (where a non-trivial sphere or disc is guaranteed to
    // appear as a vertex surface).  Otherwise fall back to standard coords.
    NormalSurfaces surfaces(*this,
        (isValid() && ! isIdeal()) ? NS_QUAD : NS_STANDARD);
    for (const NormalSurface& s : surfaces) {
        // These are vertex surfaces, so we know they must be connected.
        // Because we are either (i) using standard coordinates, or
        // (ii) working with a non-ideal triangulation; we know the
        // vertex surfaces are compact also.

        if (s.isVertexLinking())
            continue;

        // Now they are compact, connected and non-vertex-linking.
        // We just need to pick out spheres and discs.
        if (s.eulerChar() == 2) {
            // Must be a sphere; no bounded surface has chi=2.
            NormalSurface ans = s;
            return ans;
        } else if (s.eulerChar() == 1) {
            if (s.hasRealBoundary()) {
                // Must be a disc.
                return s;
            } else if (! s.isTwoSided()) {
                // A projective plane that doubles to a sphere.
                return s.doubleSurface();
            }
        }
    }

    return std::nullopt;
}

std::optional<NormalSurface> Triangulation<3>::octagonalAlmostNormalSphere()
        const {
    // Get the empty triangulation out of the way now.
    if (simplices_.empty())
        return std::nullopt;

    // Use combinatorial optimisation if we can.
    // This is good for large problems, but for small problems a full
    // enumeration is usually faster.  Still, the big problems are the
    // ones we need to be more fussy about.
    if (countVertices() == 1) {
        // For now, just use the safe arbitrary-precision Integer type.
        TreeSingleSoln<LPConstraintEulerPositive> tree(*this, NS_AN_STANDARD);
        if (tree.find()) {
            // Since our preconditions ensure the triangulation is
            // closed, orientable and 0-efficient, there are no
            // non-vertex-linking normal surfaces with positive Euler
            // characteristic.  Our optimisation asks for (Euler - #octs) > 0,
            // which then implies that our surface here is almost normal
            // with exactly 1 octagon and Euler = 2.  This is exactly
            // what we're looking for.
            return tree.buildSurface();
        } else
            return std::nullopt;
    }

    // Fall back to a slow-but-general method: enumerate all vertex surfaces.
    // Given our preconditions, we can do this in quadrilateral-octagon
    // coordinates; for details see "Quadrilateral-octagon coordinates for
    // almost normal surfaces", B.B., Experiment. Math. 19 (2010), 285-315.
    NormalSurfaces surfaces(*this, NS_AN_QUAD_OCT);

    // Our vertex surfaces are guaranteed to be in smallest possible
    // integer coordinates, with at most one non-zero octagonal coordinate.
    unsigned long tet;
    unsigned oct;
    bool found, broken;
    LargeInteger coord;
    for (const NormalSurface& s : surfaces) {
        // These are vertex surfaces, so we know they must be connected.
        // Because we are working with a non-ideal triangulation, we know the
        // vertex surfaces are compact.

        // Hunt for spheres with exactly one octagon.
        // Note that 1-sided projective planes are no good here,
        // since when doubled they give too many octagonal discs.
        if (s.eulerChar() == 2) {
            // Euler char = 2 implies no real boundary.
            found = false; // At least one octagon found so far?
            broken = false; // More than one octagon found so far?
            for (tet = 0; tet < simplices_.size() && ! broken; ++tet)
                for (oct = 0; oct < 3; ++oct) {
                    coord = s.octs(tet, oct);
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
                return s;
            }
        }
    }

    return std::nullopt;
}

bool Triangulation<3>::isZeroEfficient() const {
    if (! prop_.zeroEfficient_.has_value()) {
        if (hasTwoSphereBoundaryComponents())
            prop_.zeroEfficient_ = false;
        else if (nonTrivialSphereOrDisc()) {
            prop_.zeroEfficient_ = false;
        } else {
            prop_.zeroEfficient_ = true;

            // Things implied by 0-efficiency:
            if (isValid() && isClosed() && isConnected())
                prop_.irreducible_ = true;
        }
    }
    return *prop_.zeroEfficient_;
}

bool Triangulation<3>::hasSplittingSurface() const {
    if (prop_.splittingSurface_.has_value())
        return *prop_.splittingSurface_;

    if (isEmpty())
        return *(prop_.splittingSurface_ = false);

    // In the main loop of this procedure, we assume the triangulation is
    // connected.  If it isn't connected, we see instead if each component
    // has a splitting surface.

    if (!isConnected()) {
        for (const Triangulation<3>& comp : triangulateComponents())
            if (! comp.hasSplittingSurface())
                return *(prop_.splittingSurface_ = false);
        return *(prop_.splittingSurface_ = true);
    }

    // Now we can assume the triangulation is connected.

    // We keep track of whether an edge has been assumed to be disjoint
    // or not from a putative splitting surface.
    enum EdgeState {
        EDGE_UNKNOWN = 0,
        EDGE_DISJOINT = 1,
        EDGE_INTERSECTING = 2
    };
    auto* state = new EdgeState[countEdges()];

    // We also keep track of each edge e that is not yet assumed disjoint but
    // that is a candidate for this assumption.
    std::deque<Edge<3>*> candidate_disjoint;

    // At the outset, we may regard any edge as a candidate.
    // We will do so for each of the three edges in a triangle of the
    // triangulation.
    // The triangulation is connected.
    // So these exhaust the possibilities for a splitting surface.
    auto tri = triangle(0);

    for (int i = 0; i < 3; i++){
        candidate_disjoint.clear();
        std::fill(state, state + countEdges(), EDGE_UNKNOWN);

        // Outset
        candidate_disjoint.push_back(tri->edge(i));

        // Main inner loop
        bool broken = false;
        while (!candidate_disjoint.empty()){
            Edge<3>* e = candidate_disjoint.front();
            candidate_disjoint.pop_front();

            for (auto& emb : *e){
                Tetrahedron<3>* tet_e = emb.simplex();
                Perm<4> v_perm = emb.vertices();

                // The splitting surface should intersect the other edges
                // of the candidate edge in the given embedding meeting
                // the candidate edge---lateral edges.
                Edge<3>* lat02 = tet_e->edge(v_perm[0],v_perm[2]);
                Edge<3>* lat03 = tet_e->edge(v_perm[0],v_perm[3]);
                Edge<3>* lat12 = tet_e->edge(v_perm[1],v_perm[2]);
                Edge<3>* lat13 = tet_e->edge(v_perm[1],v_perm[3]);
                state[lat02->index()] = EDGE_INTERSECTING;
                state[lat03->index()] = EDGE_INTERSECTING;
                state[lat12->index()] = EDGE_INTERSECTING;
                state[lat13->index()] = EDGE_INTERSECTING;
            }

            // Now we check for a local obstruction to a splitting surface
            // opposite e.
            if (state[e->index()] == EDGE_INTERSECTING) {
                broken = true;
                break;
            } else {
                state[e->index()] = EDGE_DISJOINT;
                // Regard the edges opposite e as candidates if they're not
                // already assumed disjoint.
                for (auto& emb : *e) {
                    Tetrahedron<3>* tet_e = emb.simplex();
                    Perm<4> v_perm = emb.vertices();
                    Edge<3>* opp = tet_e->edge(v_perm[2],v_perm[3]);
                    if (state[opp->index()] == EDGE_INTERSECTING) {
                        broken = true;
                        break;
                    }
                    if (state[opp->index()] != EDGE_DISJOINT)
                        candidate_disjoint.push_back(opp);
                }
            }
        } // End main inner loop

        if (! broken) {
            // We partitioned the edges into disjoint and intersecting,
            // with two opposite disjoint edges per tetrahedron.
            // Thus there is a splitting surface.
            delete[] state;
            return *(prop_.splittingSurface_ = true);
        }
    } // End search for splitting surfaces along each edge of tri.

    // We found no splitting surfaces; there is none.
    delete[] state;
    return *(prop_.splittingSurface_ = false);
}

} // namespace regina

