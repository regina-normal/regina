
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

#include "algebra/nabeliangroup.h"
#include "manifold/nhandlebody.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "subcomplex/nlayeredsolidtorus.h"

namespace regina {

NLayeredSolidTorus* NLayeredSolidTorus::clone() const {
    NLayeredSolidTorus* ans = new NLayeredSolidTorus();
    int i,j;
    ans->nTetrahedra = nTetrahedra;
    ans->base = base;
    ans->topLevel = topLevel;
    for (i = 0; i < 6; i++) {
        ans->baseEdge[i] = baseEdge[i];
        ans->baseEdgeGroup[i] = baseEdgeGroup[i];
        ans->topEdgeGroup[i] = topEdgeGroup[i];
    }
    for (i = 0; i < 2; i++) {
        ans->baseFace[i] = baseFace[i];
        ans->topFace[i] = topFace[i];
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++)
            ans->topEdge[i][j] = topEdge[i][j];
        ans->meridinalCuts[i] = meridinalCuts[i];
    }
    return ans;
}

void NLayeredSolidTorus::transform(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) {
    unsigned i, j;
    unsigned long baseTetID = originalTri->tetrahedronIndex(base);
    unsigned long topTetID = originalTri->tetrahedronIndex(topLevel);

    // Data members nTetrahedra and meridinalCuts remain unchanged.

    // Transform edge numbers (note that -1s can also be present for topEdge[]):
    for (i = 0; i < 6; i++)
        baseEdge[i] = NEdge::edgeNumber
            [iso->facePerm(baseTetID)[NEdge::edgeVertex[baseEdge[i]][0]]]
            [iso->facePerm(baseTetID)[NEdge::edgeVertex[baseEdge[i]][1]]];

    for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++) {
            if (topEdge[i][j] < 0)
                continue;

            topEdge[i][j] = NEdge::edgeNumber
                [iso->facePerm(topTetID)[NEdge::edgeVertex[topEdge[i][j]][0]]]
                [iso->facePerm(topTetID)[NEdge::edgeVertex[topEdge[i][j]][1]]];
        }

    // Inverse arrays for edge numbers:
    for (i = 0; i < 6; i++)
        baseEdgeGroup[baseEdge[i]] = (i == 0 ? 1 : i < 3 ? 2 : 3);

    unsigned missingEdge = 0 + 1 + 2 + 3 + 4 + 5;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++)
            if (topEdge[i][j] != -1) {
                topEdgeGroup[topEdge[i][j]] = i;
                missingEdge -= topEdge[i][j];
            }
    topEdgeGroup[missingEdge] = -1;

    // Transform face numbers:
    for (i = 0; i < 2; i++) {
        baseFace[i] = iso->facePerm(baseTetID)[baseFace[i]];
        topFace[i] = iso->facePerm(topTetID)[topFace[i]];
    }

    // Transform tetrahedra:
    base = newTri->getTetrahedron(iso->tetImage(baseTetID));
    topLevel = newTri->getTetrahedron(iso->tetImage(topTetID));
}

NLayeredSolidTorus* NLayeredSolidTorus::formsLayeredSolidTorusBase(
        NTetrahedron* tet) {
    int baseFace1;
    int baseFace2 = -1;
    NPerm basePerm;
    bool okay;
    int i, j;
    for (baseFace1 = 0; baseFace1 < 3; baseFace1++)
        if (tet->adjacentTetrahedron(baseFace1) == tet) {
            // This tetrahedron is glued to itself.
            baseFace2 = tet->adjacentFace(baseFace1);
            basePerm = tet->adjacentGluing(baseFace1);
            okay = true;
            for (i = 0; i < 4; i++) {
                if (i == baseFace1 || i == baseFace2)
                    continue;
                // No vertex should be glued to itself!
                if (basePerm[i] == i) {
                    okay = false;
                    break;
                }
            }
            if (okay && basePerm[baseFace2] != baseFace1)
                break;
            else
                baseFace2 = -1;
        }

    if (baseFace2 == -1)
        return 0;

    // We have a layered solid torus!!
    // Fill in the details for the bottom layer.
    NLayeredSolidTorus* ans = new NLayeredSolidTorus();
    ans->nTetrahedra = 1;
    ans->base = tet;
    ans->baseFace[0] = baseFace1;
    ans->baseFace[1] = baseFace2;
    ans->topFace[0] = basePerm[baseFace2];
    ans->topFace[1] = basePerm[ans->topFace[0]];
    ans->baseEdge[0] = NEdge::edgeNumber[baseFace1][baseFace2];
    ans->baseEdge[1] = NEdge::edgeNumber[ans->topFace[1]][baseFace2];
    ans->baseEdge[2] = NEdge::edgeNumber[ans->topFace[0]][baseFace1];
    ans->baseEdge[3] = NEdge::edgeNumber[ans->topFace[0]][baseFace2];
    ans->baseEdge[4] = NEdge::edgeNumber[ans->topFace[0]][ans->topFace[1]];
    ans->baseEdge[5] = NEdge::edgeNumber[ans->topFace[1]][baseFace1];
    for (i = 0; i < 6; i++)
        ans->baseEdgeGroup[ans->baseEdge[i]] = (i == 0 ? 1 : i < 3 ? 2 : 3);

    ans->topLevel = tet;
    ans->meridinalCuts[0] = 1;
    ans->meridinalCuts[1] = 2;
    ans->meridinalCuts[2] = 3;
    ans->topEdge[0][0] = ans->baseEdge[5];
    ans->topEdge[0][1] = ans->baseEdge[3];
    ans->topEdge[1][0] = ans->baseEdge[1];
    ans->topEdge[1][1] = ans->baseEdge[2];
    ans->topEdge[2][0] = ans->baseEdge[0];
    ans->topEdge[2][1] = -1;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++)
            if (ans->topEdge[i][j] != -1)
                ans->topEdgeGroup[ans->topEdge[i][j]] = i;
    ans->topEdgeGroup[ans->baseEdge[4]] = -1;

    // Now run through and look for layers to add to the torus.
    int adjFace[2]; // Faces of adjacent tetrahedron glued to the torus.
    int adjEdge; // Layering edge of the adjacent tetrahedron.
    int layerOnEdge[2]; // The two edges of the current top tetrahedron
                        // corresponding to adjEdge.
    int newTopEdge; // New boundary edge of degree 1 in the torus.
    NPerm adjPerm[2];
    int layerOnGroup;
    while (true) {
        // Is there a new layer?
        tet = ans->topLevel->adjacentTetrahedron(ans->topFace[0]);
        if (tet == 0 || tet == ans->topLevel ||
                tet != ans->topLevel->adjacentTetrahedron(ans->topFace[1]))
            break;

        // There is a new tetrahedron glued to both torus boundary faces.
        adjPerm[0] = ans->topLevel->adjacentGluing(
            ans->topFace[0]);
        adjPerm[1] = ans->topLevel->adjacentGluing(
            ans->topFace[1]);
        if (adjPerm[0].sign() != adjPerm[1].sign())
            break;

        // See what the new boundary edge would be.
        adjFace[0] = ans->topLevel->adjacentFace(ans->topFace[0]);
        adjFace[1] = ans->topLevel->adjacentFace(ans->topFace[1]);
        newTopEdge = NEdge::edgeNumber[adjFace[0]][adjFace[1]];
        adjEdge = 5 - newTopEdge;

        // See which edges of the current top tetrahedron are being
        // layered upon.
        layerOnEdge[0] = NEdge::edgeNumber
            [adjPerm[0].preImageOf(NEdge::edgeVertex[adjEdge][0])]
            [adjPerm[0].preImageOf(NEdge::edgeVertex[adjEdge][1])];
        layerOnEdge[1] = NEdge::edgeNumber
            [adjPerm[1].preImageOf(NEdge::edgeVertex[adjEdge][0])]
            [adjPerm[1].preImageOf(NEdge::edgeVertex[adjEdge][1])];
        if (layerOnEdge[0] != layerOnEdge[1] &&
                layerOnEdge[0] + layerOnEdge[1] != 5)
            break;

        // We have a new layer!

        // Before changing anything else, rearrange the topEdge and
        // meridinalCuts arrays.
        layerOnGroup = ans->topEdgeGroup[layerOnEdge[0]];
        switch(layerOnGroup) {
            case 0:
                // p q r  ->  q r q+r
                ans->meridinalCuts[0] = ans->meridinalCuts[1];
                ans->meridinalCuts[1] = ans->meridinalCuts[2];
                ans->meridinalCuts[2] = ans->meridinalCuts[0] +
                    ans->meridinalCuts[1];
                ans->followEdge(0, 1);
                ans->followEdge(1, 2);
                ans->topEdge[2][0] = newTopEdge;
                ans->topEdge[2][1] = -1;
                break;
            case 1:
                // p q r  ->  p r p+r
                ans->meridinalCuts[1] = ans->meridinalCuts[2];
                ans->meridinalCuts[2] = ans->meridinalCuts[0] +
                    ans->meridinalCuts[1];
                ans->followEdge(0, 0);
                ans->followEdge(1, 2);
                ans->topEdge[2][0] = newTopEdge;
                ans->topEdge[2][1] = -1;
                break;
            case 2:
                if (ans->meridinalCuts[1] - ans->meridinalCuts[0] <
                        ans->meridinalCuts[0]) {
                    // p q r  ->  q-p p q
                    ans->meridinalCuts[2] = ans->meridinalCuts[1];
                    ans->meridinalCuts[1] = ans->meridinalCuts[0];
                    ans->meridinalCuts[0] = ans->meridinalCuts[2] -
                        ans->meridinalCuts[1];
                    ans->followEdge(2, 1);
                    ans->followEdge(1, 0);
                    ans->topEdge[0][0] = newTopEdge;
                    ans->topEdge[0][1] = -1;
                } else {
                    // p q r  ->  p q-p q
                    ans->meridinalCuts[2] = ans->meridinalCuts[1];
                    ans->meridinalCuts[1] = ans->meridinalCuts[2] -
                        ans->meridinalCuts[0];
                    ans->followEdge(2, 1);
                    ans->followEdge(0, 0);
                    ans->topEdge[1][0] = newTopEdge;
                    ans->topEdge[1][1] = -1;
                }
                break;
        }

        ans->topFace[0] = NEdge::edgeVertex[adjEdge][0];
        ans->topFace[1] = NEdge::edgeVertex[adjEdge][1];

        // Massage the indices in topEdge to match topFace.
        for (i = 0; i < 3; i++) {
            // Make sure ans->topEdge[i][0] is in face ans->topFace[0].
            if (ans->topFace[0] == NEdge::edgeVertex[ans->topEdge[i][0]][0] ||
                    ans->topFace[0] ==
                    NEdge::edgeVertex[ans->topEdge[i][0]][1]) {
                j = ans->topEdge[i][0];
                ans->topEdge[i][0] = ans->topEdge[i][1];
                ans->topEdge[i][1] = j;
            }
        }

        ans->topLevel = tet;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 2; j++)
                if (ans->topEdge[i][j] != -1)
                    ans->topEdgeGroup[ans->topEdge[i][j]] = i;
        ans->topEdgeGroup[adjEdge] = -1;
        ans->nTetrahedra++;
    }

    return ans;
}

NLayeredSolidTorus* NLayeredSolidTorus::formsLayeredSolidTorusTop(
        NTetrahedron* tet, unsigned topFace1, unsigned topFace2) {
    NTetrahedron* top = tet;
    NTetrahedron* next;
    NPerm cross1, cross2;
    NPerm canon1, canon2;
    NFacePair pair = NFacePair(topFace1, topFace2).complement();
    NPerm vRoles(pair.upper(), topFace1, topFace2, pair.lower());
    NPerm topRoles(vRoles);
    NPerm nextRoles;
    long w = 1, x = 0, y = 0, z = 1;
    long w_, x_, y_, z_;
    int rotation;
    unsigned long nTets = 1;

    NPerm rot180(3, 2, 1, 0);

    while (true) {
        // INVARIANT:
        //
        // We are currently looking at tetrahedron tet.
        // The faces of tet closest to the top of the layered solid
        // torus are vRoles[1] and vRoles[2].  The faces we have not yet
        // looked at are vRoles[0] and vRoles[3].
        //
        // Denote directed edges a = vRoles[01], b = vRoles[02], and
        // similarly let p = topRoles[01], q = topRoles[02] (where topRoles
        // was the original permutation vRoles for the original top-level
        // tetrahedron top).  Then these edges are related as follows:
        //
        //     p == w.a + x.b
        //     q == y.a + z.b
        //
        // The count nTets reflects the number of tetrahedra seen so
        // far, including this one.

        // Verify that both new faces go to the same tetrahedron (which
        // exists).
        next = tet->adjacentTetrahedron(vRoles[0]);
        if (! next)
            return 0;
        if (next != tet->adjacentTetrahedron(vRoles[3]))
            return 0;

        // Are we folding over?
        cross1 = tet->adjacentGluing(vRoles[0]);
        cross2 = tet->adjacentGluing(vRoles[3]);
        if (next == tet && cross1[vRoles[0]] == vRoles[3]) {
            // Could be.  Certainly faces vRoles[0,3] are joined to
            // each other.  This is our final iteration -- either it
            // works or it doesn't.

            // Find the permutation that maps canonical vertices 123 to 012.
            canon1 = vRoles.inverse() * cross1 * vRoles;

            // Run through the three orientation-preserving permutations.
            // Note that canon1[0] == 3.
            if (canon1 == NPerm(3, 1, 2, 0)) {
                // Tetrahedron is folded shut over edge vRoles[12].
                // This does not give an LST(3,2,1) base, so we are not
                // interested.
                return 0;
            } else if (canon1 == NPerm(3, 0, 1, 2)) {
                rotation = 1;
                // a, b have weights 1, 2.
            } else if (canon1 == NPerm(3, 2, 0, 1)) {
                rotation = 2;
                // a, b have weights 2, 1.
            } else {
                // We have an orientation-reversing permutation.
                return 0;
            }

            // We got one!
            NLayeredSolidTorus* ans = new NLayeredSolidTorus();
            ans->nTetrahedra = nTets;

            ans->base = tet;
            ans->baseFace[0] = vRoles[3]; // Face vRoles[012]
            ans->baseFace[1] = vRoles[0]; // Face vRoles[123]
            ans->baseEdge[0] = NEdge::edgeNumber[vRoles[0]][vRoles[3]];
            if (rotation == 1) {
                ans->baseEdge[1] = NEdge::edgeNumber[vRoles[0]][vRoles[2]];
                ans->baseEdge[2] = NEdge::edgeNumber[vRoles[1]][vRoles[3]];

                ans->baseEdge[3] = NEdge::edgeNumber[vRoles[0]][vRoles[1]];
                ans->baseEdge[4] = NEdge::edgeNumber[vRoles[1]][vRoles[2]];
                ans->baseEdge[5] = NEdge::edgeNumber[vRoles[2]][vRoles[3]];
            } else {
                ans->baseEdge[1] = NEdge::edgeNumber[vRoles[0]][vRoles[1]];
                ans->baseEdge[2] = NEdge::edgeNumber[vRoles[2]][vRoles[3]];

                ans->baseEdge[3] = NEdge::edgeNumber[vRoles[0]][vRoles[2]];
                ans->baseEdge[4] = NEdge::edgeNumber[vRoles[2]][vRoles[1]];
                ans->baseEdge[5] = NEdge::edgeNumber[vRoles[1]][vRoles[3]];
            }
            ans->baseEdgeGroup[ans->baseEdge[0]] = 1;
            ans->baseEdgeGroup[ans->baseEdge[1]] = 2;
            ans->baseEdgeGroup[ans->baseEdge[2]] = 2;
            ans->baseEdgeGroup[ans->baseEdge[3]] = 3;
            ans->baseEdgeGroup[ans->baseEdge[4]] = 3;
            ans->baseEdgeGroup[ans->baseEdge[5]] = 3;

            long cuts01, cuts13, cuts30;
            if (rotation == 1) {
                // (a,b) == (1,2).
                cuts01 = w + 2 * x;  // w.a + x.b
                cuts13 = y + 2 * z;  // y.a + z.b
            } else {
                // (a,b) == (2,1).
                cuts01 = 2 * w + x;  // w.a + x.b
                cuts13 = 2 * y + z;  // y.a + z.b
            }
            cuts30 = - cuts01 - cuts13;

            if (cuts01 < 0) cuts01 = -cuts01;
            if (cuts13 < 0) cuts13 = -cuts13;
            if (cuts30 < 0) cuts30 = -cuts30;

            ans->topLevel = top;
            ans->topFace[0] = topRoles[2]; // Face topRoles[013]
            ans->topFace[1] = topRoles[1]; // Face topRoles[023]

            // Run through all six possible orderings.
            int group01, group13, group30;
            if (cuts01 <= cuts13) {
                // 01 13
                if (cuts13 <= cuts30) {
                    // 01 13 30
                    group01 = 0; group13 = 1; group30 = 2;
                } else if (cuts30 <= cuts01) {
                    // 30 01 13
                    group30 = 0; group01 = 1; group13 = 2;
                } else {
                    // 01 30 13
                    group01 = 0; group30 = 1; group13 = 2;
                }
            } else {
                // 13 01
                if (cuts30 <= cuts13) {
                    // 30 13 01
                    group30 = 0; group13 = 1; group01 = 2;
                } else if (cuts01 <= cuts30) {
                    // 13 01 30
                    group13 = 0; group01 = 1; group30 = 2;
                } else {
                    // 13 30 01
                    group13 = 0; group30 = 1; group01 = 2;
                }
            }
            ans->meridinalCuts[group01] = cuts01;
            ans->meridinalCuts[group13] = cuts13;
            ans->meridinalCuts[group30] = cuts30;
            ans->topEdge[group01][0] =
                NEdge::edgeNumber[topRoles[0]][topRoles[1]];
            ans->topEdge[group01][1] =
                NEdge::edgeNumber[topRoles[2]][topRoles[3]];
            ans->topEdge[group13][0] =
                NEdge::edgeNumber[topRoles[1]][topRoles[3]];
            ans->topEdge[group13][1] =
                NEdge::edgeNumber[topRoles[0]][topRoles[2]];
            ans->topEdge[group30][0] =
                NEdge::edgeNumber[topRoles[3]][topRoles[0]];
            ans->topEdge[group30][1] = -1;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[0]][topRoles[1]]] =
                group01;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[2]][topRoles[3]]] =
                group01;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[1]][topRoles[3]]] =
                group13;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[0]][topRoles[2]]] =
                group13;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[3]][topRoles[0]]] =
                group30;
            ans->topEdgeGroup[NEdge::edgeNumber[topRoles[1]][topRoles[2]]] = -1;

            // All done!
            return ans;
        }

        // We're looking for an entirely new tetrahedron.
        // Make sure we're not looping back in a cycle or anything kinky.
        if (next == tet || next == top)
            return 0;

        // Set up nextRoles so that faces tet/vRoles[0,3] are joined to
        // faces next/nextRoles[1,2] respectively.
        pair = NFacePair(cross1[vRoles[0]], cross2[vRoles[3]]).complement();
        nextRoles = NPerm(pair.upper(), cross1[vRoles[0]], cross2[vRoles[3]],
            pair.lower());

        // Find the mapping between the canonical 0123 as described by
        // vRoles and the canonical 0123 as described by nextRoles.
        // There are two such mappings, for the gluings on faces
        // vRoles[0,3] respectively.
        canon1 = nextRoles.inverse() * cross1 * vRoles;
        canon2 = nextRoles.inverse() * cross2 * vRoles;

        // Make sure it's actually a layering, i.e., canon1 and canon2 are
        // compatible.
        if (rot180 * canon1 * rot180 != canon2)
            return 0;

        // Update the matrix [ a,b | c,d ].
        // It seems sanest to take cases based on the six possible
        // permutations.  Use canon2, which starts at face 3 (012).
        // Note that canon2[3] == 2.
        // Old a, b : 01, 02.
        // New a, b : 01, 13.
        if (canon2 == NPerm(0, 1, 3, 2)) {
            // 012 -> 013.
            // old a = a
            // old b = a+b
            // p = w.(old_a) + x.(old_b) = (w+x).a + x.b
            // q = y.(old_a) + z.(old_b) = (y+z).a + z.b
            w_ = w + x;
            x_ = x;
            y_ = y + z;
            z_ = z;
        } else if (canon2 == NPerm(0, 3, 1, 2)) {
            // 012 -> 031.
            // old a = a+b
            // old b = a
            // p = w.(old_a) + x.(old_b) = (w+x).a + w.b
            // q = y.(old_a) + z.(old_b) = (y+z).a + y.b
            w_ = w + x;
            x_ = w;
            y_ = y + z;
            z_ = y;
        } else if (canon2 == NPerm(1, 0, 3, 2)) {
            // 012 -> 103.
            // old a = -a
            // old b = b
            // p = w.(old_a) + x.(old_b) = -w.a + x.b
            // q = y.(old_a) + z.(old_b) = -y.a + z.b
            w_ = -w;
            x_ = x;
            y_ = -y;
            z_ = z;
        } else if (canon2 == NPerm(1, 3, 0, 2)) {
            // 012 -> 130.
            // old a = b
            // old b = -a
            // p = w.(old_a) + x.(old_b) = -x.a + w.b
            // q = y.(old_a) + z.(old_b) = -z.a + y.b
            w_ = -x;
            x_ = w;
            y_ = -z;
            z_ = y;
        } else if (canon2 == NPerm(3, 0, 1, 2)) {
            // 012 -> 301.
            // old a = -(a+b)
            // old b = -b
            // p = w.(old_a) + x.(old_b) = -w.a - (w+x).b
            // q = y.(old_a) + z.(old_b) = -y.a - (y+z).b
            w_ = -w;
            x_ = -w - x;
            y_ = -y;
            z_ = -y - z;
        } else if (canon2 == NPerm(3, 1, 0, 2)) {
            // 012 -> 310.
            // old a = -b
            // old b = -(a+b)
            // p = w.(old_a) + x.(old_b) = -x.a - (w+x).b
            // q = y.(old_a) + z.(old_b) = -z.a - (y+z).b
            w_ = -x;
            x_ = -w - x;
            y_ = -z;
            z_ = -y - z;
        } else {
            // Impossible.  We should never get to this point.
            std::cerr << "ERROR: Bad permutation canon2." << std::endl;
            return 0;
        }

        w = w_; x = x_; y = y_; z = z_;

        // Adjust the other variables in preparation for the next loop
        // iteration.
        tet = next;
        vRoles = nextRoles;
        nTets++;
    }

    // The loop has no break so we should never get here, but what the hell.
    return 0;
}

NLayeredSolidTorus* NLayeredSolidTorus::isLayeredSolidTorus(NComponent* comp) {
    // Start with some basic property checks.
    if (! comp->isOrientable())
        return 0;
    if (comp->getNumberOfBoundaryComponents() != 1)
        return 0;
    if (comp->getBoundaryComponent(0)->getNumberOfFaces() != 2)
        return 0;

    NFaceEmbedding f0 = comp->getBoundaryComponent(0)->getFace(0)->
        getEmbedding(0);
    NFaceEmbedding f1 = comp->getBoundaryComponent(0)->getFace(1)->
        getEmbedding(0);

    NTetrahedron* top = f0.getTetrahedron();
    if (f1.getTetrahedron() != top)
        return 0;

    // We have precisely one boundary component, which consists of two
    // faces belonging to the same tetrahedron.

    // Follow the adjacent tetrahedra down to what should be the base
    // tetrahedron.  Don't worry about gluing permutations for now.

    // Run a full search.

    // We use formsLayeredSolidTorusBase(), which works out the full structure
    // for us.  It would be faster to just follow straight down from
    // the top level tetrahedron (which we already know), but this would
    // also require us to code up the entire structure again.

    NFacePair underFaces = NFacePair(f0.getFace(), f1.getFace()).complement();
    NTetrahedron* currTet = top;
    NTetrahedron* nextTet;
    while (1) {
        // INV: Thus far we have seen a chain of tetrahedra, with each
        // tetrahedron glued to the next along two faces.

        // See where the next two faces lead.
        // Note that they cannot lead back to a previous tetrahedron,
        // since each previous tetrahedron already has all four faces
        // accounted for (thus showing this loop will terminate).
        // They also cannot be boundary faces, since there are only two
        // boundary faces and these have already been seen.
        nextTet = currTet->adjacentTetrahedron(underFaces.lower());
        if (nextTet != currTet->adjacentTetrahedron(underFaces.upper()))
            return 0;

        // They both lead to the same adjacent tetrahedron.
        // Have we reached the end?
        if (nextTet == currTet)
            break;

        // No; we have simply moved on to the next tetrahedron.
        underFaces = NFacePair(currTet->adjacentFace(underFaces.lower()),
            currTet->adjacentFace(underFaces.upper())).complement();
        currTet = nextTet;
    }

    // Here we are at the bottom.  Now check the individual permutations
    // and fill in the structural details.
    return formsLayeredSolidTorusBase(currTet);
}

void NLayeredSolidTorus::followEdge(int destGroup, int sourceGroup) {
    int pos;
    NPerm adjPerm;
    for (int i = 1; i >= 0; i--) {
        pos = (topEdge[sourceGroup][i] == -1 ? 0 : i);
        adjPerm = topLevel->adjacentGluing(topFace[i]);
        topEdge[destGroup][i] = NEdge::edgeNumber
            [adjPerm[NEdge::edgeVertex[topEdge[sourceGroup][pos]][0]]]
            [adjPerm[NEdge::edgeVertex[topEdge[sourceGroup][pos]][1]]];
    }
}

NManifold* NLayeredSolidTorus::getManifold() const {
    return new NHandlebody(1, true);
}

NAbelianGroup* NLayeredSolidTorus::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank();
    return ans;
}

NTriangulation* NLayeredSolidTorus::flatten(const NTriangulation* original,
        int mobiusBandBdry) const {
    // Create a new triangulation and identify the top-level and
    // base tetrahedra.
    NTriangulation* ans = new NTriangulation(*original);

    NTetrahedron* newTop = ans->getTetrahedron(
        original->tetrahedronIndex(topLevel));
    NTetrahedron* newBase = ans->getTetrahedron(
        original->tetrahedronIndex(base));

    NPacket::ChangeEventBlock block(ans);

    // Reglue the top faces before deleting the layered solid torus.
    NTetrahedron* adj0 = newTop->adjacentTetrahedron(topFace[0]);
    NTetrahedron* adj1 = newTop->adjacentTetrahedron(topFace[1]);

    if (adj0 && adj1 && (adj0 != newTop)) {
        // A permutation for each adjacent tetrahedron.
        // These permutations map:
        //   1,2 -> vertices corresponding to top edge group 0
        //   0,2 -> vertices corresponding to top edge group 1
        //   0,1 -> vertices corresponding to top edge group 2

        // Start by representing vertices of this tetrahedron instead.
        NPerm groups0 = NPerm(
            6 - NEdge::edgeVertex[topEdge[0][0]][0] -
                NEdge::edgeVertex[topEdge[0][0]][1] - topFace[0],
            6 - NEdge::edgeVertex[topEdge[1][0]][0] -
                NEdge::edgeVertex[topEdge[1][0]][1] - topFace[0],
            6 - NEdge::edgeVertex[topEdge[2][0]][0] -
                NEdge::edgeVertex[topEdge[2][0]][1] - topFace[0],
            topFace[0]);

        NFacePair underFaces = NFacePair(topFace[0], topFace[1]).complement();
        NPerm groups1 = NPerm(topFace[0], topFace[1]) *
            NPerm(underFaces.lower(), underFaces.upper()) * groups0;

        // Move these to vertices of the adjacent tetrahedra.
        groups0 = newTop->adjacentGluing(topFace[0]) * groups0;
        groups1 = newTop->adjacentGluing(topFace[1]) * groups1;

        // And do the regluing.
        adj0->unjoin(groups0[3]);
        adj1->unjoin(groups1[3]);

        adj0->joinTo(groups0[3], adj1, groups1 *
            NPerm((mobiusBandBdry + 1) % 3, (mobiusBandBdry + 2) % 3) *
            groups0.inverse());
    }

    // Delete the layered solid torus tetrahedra.
    NTetrahedron* curr;
    NTetrahedron* next;
    NFacePair currBdryFaces;
    NPerm adjPerm;

    curr = newBase;
    currBdryFaces = NFacePair(baseFace[0], baseFace[1]).complement();
    while (curr) {
        next = curr->adjacentTetrahedron(currBdryFaces.lower());

        currBdryFaces = NFacePair(curr->adjacentFace(currBdryFaces.lower()),
            curr->adjacentFace(currBdryFaces.upper())).complement();
        delete ans->removeTetrahedron(curr);
        curr = next;
    }

    // And we're done.
    return ans;
}

} // namespace regina
