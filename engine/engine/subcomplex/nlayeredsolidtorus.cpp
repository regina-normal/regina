
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/ntetrahedron.h"
#include "triangulation/nedge.h"
#include "subcomplex/nlayeredsolidtorus.h"

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

NLayeredSolidTorus* NLayeredSolidTorus::isLayeredSolidTorusBase(
        NTetrahedron* tet) {
    int baseFace1;
    int baseFace2 = -1;
    NPerm basePerm;
    bool okay;
    int i, j;
    for (baseFace1 = 0; baseFace1 < 3; baseFace1++)
        if (tet->getAdjacentTetrahedron(baseFace1) == tet) {
            // This tetrahedron is glued to itself.
            baseFace2 = tet->getAdjacentFace(baseFace1);
            basePerm = tet->getAdjacentTetrahedronGluing(baseFace1);
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
    ans->baseEdge[0] = edgeNumber[baseFace1][baseFace2];
    ans->baseEdge[1] = edgeNumber[ans->topFace[1]][baseFace2];
    ans->baseEdge[2] = edgeNumber[ans->topFace[0]][baseFace1];
    ans->baseEdge[3] = edgeNumber[ans->topFace[0]][baseFace2];
    ans->baseEdge[4] = edgeNumber[ans->topFace[0]][ans->topFace[1]];
    ans->baseEdge[5] = edgeNumber[ans->topFace[1]][baseFace1];
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
    int newTopEdge; // New boundary edge of degree 1 in the torus.
    NPerm adjPerm[2];
    int layerOnGroup;
    while (true) {
        // Is there a new layer?
        tet = ans->topLevel->getAdjacentTetrahedron(ans->topFace[0]);
        if (tet == 0 || tet == ans->topLevel ||
                tet != ans->topLevel->getAdjacentTetrahedron(ans->topFace[1]))
            break;
        // There is a new tetrahedron glued to both torus boundary faces.
        // If both face gluings preserve orientation, this is a layering.
        adjPerm[0] = ans->topLevel->getAdjacentTetrahedronGluing(
            ans->topFace[0]);
        adjPerm[1] = ans->topLevel->getAdjacentTetrahedronGluing(
            ans->topFace[1]);
        if (adjPerm[0].sign() != adjPerm[1].sign())
            break;

        // We have a new layer!
        adjFace[0] = ans->topLevel->getAdjacentFace(ans->topFace[0]);
        adjFace[1] = ans->topLevel->getAdjacentFace(ans->topFace[1]);
        newTopEdge = edgeNumber[adjFace[0]][adjFace[1]];
        adjEdge = 5 - newTopEdge;

        // On which edge are we doing the layering?
        layerOnGroup = ans->topEdgeGroup[edgeNumber
                [adjPerm[0].preImageOf(edgeStart[adjEdge])]
                [adjPerm[0].preImageOf(edgeEnd[adjEdge])]];

        // Before changing anything else, rearrange the topEdge and
        // meridinalCuts arrays.
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

        ans->topFace[0] = edgeStart[adjEdge];
        ans->topFace[1] = edgeEnd[adjEdge];

        // Massage the indices in topEdge to match topFace.
        for (i = 0; i < 3; i++) {
            // Make sure ans->topEdge[i][0] is in face ans->topFace[0].
            if (ans->topFace[0] == edgeStart[ans->topEdge[i][0]] ||
                    ans->topFace[0] == edgeEnd[ans->topEdge[i][0]]) {
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

void NLayeredSolidTorus::followEdge(int destGroup, int sourceGroup) {
    int pos;
    NPerm adjPerm;
    for (int i = 1; i >= 0; i--) {
        pos = (topEdge[sourceGroup][i] == -1 ? 0 : i);
        adjPerm = topLevel->getAdjacentTetrahedronGluing(topFace[i]);
        topEdge[destGroup][i] = edgeNumber
            [adjPerm[edgeStart[topEdge[sourceGroup][pos]]]]
            [adjPerm[edgeEnd[topEdge[sourceGroup][pos]]]];
    }
}
