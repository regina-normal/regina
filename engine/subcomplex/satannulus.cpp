
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

#include "maths/matrix2.h"
#include "subcomplex/satannulus.h"
#include "triangulation/dim3.h"

namespace regina {

unsigned SatAnnulus::meetsBoundary() const {
    unsigned ans = 0;
    if (! tet[0]->adjacentTetrahedron(roles[0][3]))
        ans++;
    if (! tet[1]->adjacentTetrahedron(roles[1][3]))
        ans++;
    return ans;
}

void SatAnnulus::switchSides() {
    unsigned which, face;
    for (which = 0; which < 2; which++) {
        face = roles[which][3];
        roles[which] = tet[which]->adjacentGluing(face) *
            roles[which];
        tet[which] = tet[which]->adjacentTetrahedron(face);
    }
}

bool SatAnnulus::isAdjacent(const SatAnnulus& other, bool* refVert,
        bool* refHoriz) const {
    if (other.meetsBoundary())
        return false;

    // See what is actually attached to the given annulus.
    SatAnnulus opposite(other);
    opposite.switchSides();

    if (opposite.tet[0] == tet[0] && opposite.tet[1] == tet[1]) {
        // Could be a match without horizontal reflection.

        if (opposite.roles[0] == roles[0] && opposite.roles[1] == roles[1]) {
            // Perfect match.
            if (refVert) *refVert = false;
            if (refHoriz) *refHoriz = false;
            return true;
        }

        if (opposite.roles[0] == roles[0] * Perm<4>(0, 1) &&
                opposite.roles[1] == roles[1] * Perm<4>(0, 1)) {
            // Match with vertical reflection.
            if (refVert) *refVert = true;
            if (refHoriz) *refHoriz = false;
            return true;
        }
    }

    if (opposite.tet[0] == tet[1] && opposite.tet[1] == tet[0]) {
        // Could be a match with horizontal reflection.

        if (opposite.roles[0] == roles[1] * Perm<4>(0, 1) &&
                opposite.roles[1] == roles[0] * Perm<4>(0, 1)) {
            // Match with horizontal reflection.
            if (refVert) *refVert = false;
            if (refHoriz) *refHoriz = true;
            return true;
        }

        if (opposite.roles[0] == roles[1] && opposite.roles[1] == roles[0]) {
            // Match with both reflections.
            if (refVert) *refVert = true;
            if (refHoriz) *refHoriz = true;
            return true;
        }
    }

    // No match.
    return false;
}

bool SatAnnulus::isJoined(const SatAnnulus& other, Matrix2& matching) const {
    if (other.meetsBoundary())
        return false;

    // See what is actually attached to the given annulus.
    SatAnnulus opposite(other);
    opposite.switchSides();

    bool swapTriangles;
    Perm<4> roleMap; // Maps this 0/1/2 roles -> opposite 0/1/2 roles.
    if (opposite.tet[0] == tet[0] &&
            opposite.tet[1] == tet[1] &&
            opposite.roles[0][3] == roles[0][3] &&
            opposite.roles[1][3] == roles[1][3]) {
        swapTriangles = false;

        roleMap = opposite.roles[0].inverse() * roles[0];
        if (roleMap != opposite.roles[1].inverse() * roles[1])
            return false;
    } else if (opposite.tet[0] == tet[1] &&
            opposite.tet[1] == tet[0] &&
            opposite.roles[0][3] == roles[1][3] &&
            opposite.roles[1][3] == roles[0][3]) {
        swapTriangles = true;

        roleMap = opposite.roles[1].inverse() * roles[0];
        if (roleMap != opposite.roles[0].inverse() * roles[1])
            return false;
    } else
        return false;

    // It's a match.  We just need to work out the matching matrix.
    if        (roleMap == Perm<4>(0, 1, 2, 3)) {
        matching = Matrix2(1, 0, 0, 1);
    } else if (roleMap == Perm<4>(1, 2, 0, 3)) {
        matching = Matrix2(-1, 1, -1, 0);
    } else if (roleMap == Perm<4>(2, 0, 1, 3)) {
        matching = Matrix2(0, -1, 1, -1);
    } else if (roleMap == Perm<4>(0, 2, 1, 3)) {
        matching = Matrix2(0, 1, 1, 0);
    } else if (roleMap == Perm<4>(2, 1, 0, 3)) {
        matching = Matrix2(1, -1, 0, -1);
    } else if (roleMap == Perm<4>(1, 0, 2, 3)) {
        matching = Matrix2(-1, 0, -1, 1);
    }
    if (swapTriangles)
        matching.negate();

    return true;
}

bool SatAnnulus::isTwoSidedTorus() const {
    // Check that the edges are identified in opposite pairs and that we
    // have no duplicates.
    Edge<3>* e01 = tet[0]->edge(Edge<3>::edgeNumber[roles[0][0]][roles[0][1]]);
    Edge<3>* e02 = tet[0]->edge(Edge<3>::edgeNumber[roles[0][0]][roles[0][2]]);
    Edge<3>* e12 = tet[0]->edge(Edge<3>::edgeNumber[roles[0][1]][roles[0][2]]);

    if (e01 != tet[1]->edge(Edge<3>::edgeNumber[roles[1][0]][roles[1][1]]))
        return false;
    if (e02 != tet[1]->edge(Edge<3>::edgeNumber[roles[1][0]][roles[1][2]]))
        return false;
    if (e12 != tet[1]->edge(Edge<3>::edgeNumber[roles[1][1]][roles[1][2]]))
        return false;

    if (e01 == e02 || e02 == e12 || e12 == e01)
        return false;

    // Verify that edges are consistently oriented, and that the
    // orientations of the edge links indicate a two-sided torus.
    Perm<4> map0, map1;
    int a, b, x, y;
    for (int i = 0; i < 3; i++) {
        // Examine edges corresponding to annulus markings a & b.
        // We also set x & y as the complement of {a,b} in {0,1,2,3}.
        switch (i) {
            case 0: a = 0; b = 1; x = 2; y = 3; break;
            case 1: a = 0; b = 2; x = 1; y = 3; break;
            case 2: a = 1; b = 2; x = 0; y = 3; break;
        }

        // Get mappings from tetrahedron edge roles to annulus vertex roles.
        map0 = roles[0].inverse() * tet[0]->edgeMapping(
            Edge<3>::edgeNumber[roles[0][a]][roles[0][b]]);
        map1 = roles[1].inverse() * tet[1]->edgeMapping(
            Edge<3>::edgeNumber[roles[1][a]][roles[1][b]]);

        // We should have {a,b} -> {a,b} and {x,y} -> {x,y} for each map.

        // Make sure that the two annulus edges are oriented in the same way
        // (i.e., (a,b) <-> (b,a)), and that the edge link runs in opposite
        // directions through the annulus on each side (i.e., (x,y) <-> (y,x)).
        if (map0 != Perm<4>(a, b) * Perm<4>(x, y) * map1)
            return false;
    }

    // No unpleasantries.
    return true;
}

void SatAnnulus::transform(const Triangulation<3>* originalTri,
        const Isomorphism<3>* iso, Triangulation<3>* newTri) {
    unsigned which;
    unsigned long tetID;
    for (which = 0; which < 2; which++) {
        tetID = tet[which]->index();
        tet[which] = newTri->tetrahedron(iso->tetImage(tetID));
        roles[which] = iso->facePerm(tetID) * roles[which];
    }
}

void SatAnnulus::attachLST(Triangulation<3>* tri, long alpha, long beta) const {
    // Save ourselves headaches later.  Though this should never happen;
    // see the preconditions.
    if (alpha == 0)
        return;

    // Normalise to alpha positive.
    if (alpha < 0) {
        alpha = -alpha;
        beta = -beta;
    }

    // Pull out the degenerate case.
    if (alpha == 2 && beta == 1) {
        tet[0]->join(roles[0][3], tet[1],
            roles[1] * Perm<4>(0, 1) * roles[0].inverse());
        return;
    }

    // Insert a real layered solid torus.  How we do this depends on
    // relative signs and orderings.
    long diag = alpha - beta;

    // Our six possibilities are:
    //
    // 0 <= -diag  <   alpha <= beta:
    // 0 <   alpha <= -diag  <  beta:
    // 0 <   diag  <=  beta  <  alpha:
    // 0 <=  beta  <   diag  <= alpha:
    // 0 <  -beta  <=  alpha <  diag
    // 0 <   alpha <  -beta  <  diag

    // We can give the vertices of the tetrahedra "cut labels" as
    // follows (where the LST has parameters 0 <= cuts0 <= cuts1 <= cuts2):
    //
    //         cuts0
    //       *-------*
    //       |2  1 / |
    //       |    / 0|
    // cuts1 |   /   | cuts1
    //       |0 /    |
    //       | / 1  2|
    //       *-------*
    //         cuts0

    long cuts0, cuts1;
    Perm<4> cutsToRoles; // Maps cut labels to annulus vertex roles.
    if (alpha <= beta) {
        if (-diag < alpha) {
            // 0 <= -diag  <   alpha <= beta:
            cuts0 = -diag;
            cuts1 = alpha;
            cutsToRoles = Perm<4>(0, 2, 1, 3);
        } else {
            // 0 <   alpha <= -diag  <  beta:
            cuts0 = alpha;
            cuts1 = -diag;
            cutsToRoles = Perm<4>(2, 0, 1, 3);
        }
    } else if (0 <= beta) {
        if (diag <= beta) {
            // 0 <   diag  <=  beta  <  alpha:
            cuts0 = diag;
            cuts1 = beta;
            cutsToRoles = Perm<4>(0, 1, 2, 3);
        } else {
            // 0 <=  beta  <   diag  <= alpha:
            cuts0 = beta;
            cuts1 = diag;
            cutsToRoles = Perm<4>(1, 0, 2, 3);
        }
    } else {
        if (-beta <= alpha) {
            // 0 <  -beta  <=  alpha <  diag
            cuts0 = -beta;
            cuts1 = alpha;
            cutsToRoles = Perm<4>(1, 2, 0, 3);
        } else {
            // 0 <   alpha <  -beta  <  diag
            cuts0 = alpha;
            cuts1 = -beta;
            cutsToRoles = Perm<4>(2, 1, 0, 3);
        }
    }

    Tetrahedron<3>* lst = tri->insertLayeredSolidTorus(cuts0, cuts1);

    // The boundary of the new LST sits differently for the special
    // cases (0,1,1) and (1,1,2); see the insertLayeredSolidTorus()
    // documentation for details.
    if (cuts1 == 1) {
        lst->join(3, tet[0], roles[0] * cutsToRoles * Perm<4>(1, 2, 0, 3));
        lst->join(2, tet[1], roles[1] * cutsToRoles * Perm<4>(2, 1, 3, 0));
    } else {
        lst->join(3, tet[0], roles[0] * cutsToRoles * Perm<4>(0, 1, 2, 3));
        lst->join(2, tet[1], roles[1] * cutsToRoles * Perm<4>(1, 0, 3, 2));
    }
}

} // namespace regina

