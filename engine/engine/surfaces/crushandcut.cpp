
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NTriangulation* NNormalSurface::cutAlong() {
    // TODO: Actually write this routine.
    return new NTriangulation();
}

NTriangulation* NNormalSurface::crush() {
    NTriangulation* ans = new NTriangulation(*triangulation);
    unsigned long nTet = ans->getNumberOfTetrahedra();
    if (nTet == 0)
        return new NTriangulation();

    // Work out which tetrahedra contain which quad types.
    int* quads = new int[nTet];
    long whichTet = 0;
    for (whichTet = 0; whichTet < (long)nTet; whichTet++) {
        if (getQuadCoord(whichTet, 0) != 0)
            quads[whichTet] = 0;
        else if (getQuadCoord(whichTet, 1) != 0)
            quads[whichTet] = 1;
        else if (getQuadCoord(whichTet, 2) != 0)
            quads[whichTet] = 2;
        else
            quads[whichTet] = -1;
    }

    // Run through and fix the tetrahedron gluings.
    NTetrahedron* tet;
    NTetrahedron* adj;
    int adjQuads;
    NPerm adjPerm;
    NPerm swap;
    int face, adjFace;
    for (whichTet = 0; whichTet < (long)nTet; whichTet++)
        if (quads[whichTet] == -1) {
            // We want to keep this tetrahedron, so make sure it's glued
            // up correctly.
            tet = ans->getTetrahedron(whichTet);
            for (face = 0; face < 4; face++) {
                adj = tet->getAdjacentTetrahedron(face);
                if (! adj)
                    continue;
                adjQuads = quads[ans->getTetrahedronIndex(adj)];
                if (adjQuads == -1)
                    continue;

                // We're glued to a bad tetrahedron.  Follow around
                // until we reach a good tetrahedron or a boundary.
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                adjFace = adjPerm[face];
                while (adj && (adjQuads >= 0)) {
                    swap = NPerm(adjFace,
                        vertexSplitPartner[adjQuads][adjFace]);

                    adjFace = swap[adjFace];
                    adjPerm = adj->getAdjacentTetrahedronGluing(adjFace) *
                        swap * adjPerm;
                    adj = adj->getAdjacentTetrahedron(adjFace);
                    adjFace = adjPerm[face];

                    if (adj)
                        adjQuads = quads[ans->getTetrahedronIndex(adj)];
                }

                // Reglue the tetrahedron face accordingly.
                tet->unjoin(face);
                if (! adj)
                    continue;

                // We haven't yet unglued the face of adj since there is
                // at least one bad tetrahedron between tet and adj.
                adj->unjoin(adjFace);
                tet->joinTo(face, adj, adjPerm);
            }
        }
    
    // Delete unwanted tetrahedra.
    for (whichTet = nTet - 1; whichTet >= 0; whichTet--)
        if (quads[whichTet] >= 0)
            ans->removeTetrahedronAt(whichTet);

    delete[] quads;
    return ans;
}

} // namespace regina

