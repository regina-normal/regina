
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <algorithm>
#include "surfaces/nnormalsurface.h"
#include "surfaces/nprism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NTriangulation* NNormalSurface::cutAlong() const {
    // TODO: Actually write this routine.
    return new NTriangulation();
}

NTriangulation* NNormalSurface::crush() const {
    NTriangulation* ans = new NTriangulation(*triangulation);
    unsigned long nTet = ans->getNumberOfTetrahedra();
    if (nTet == 0)
        return new NTriangulation();

    // Work out which tetrahedra contain which quad types.
    int* quads = new int[nTet];
    long whichTet = 0;
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++) {
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
    for (whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++)
        if (quads[whichTet] == -1) {
            // We want to keep this tetrahedron, so make sure it's glued
            // up correctly.
            tet = ans->getTetrahedron(whichTet);
            for (face = 0; face < 4; face++) {
                adj = tet->adjacentTetrahedron(face);
                if (! adj)
                    continue;
                adjQuads = quads[ans->tetrahedronIndex(adj)];
                if (adjQuads == -1)
                    continue;

                // We're glued to a bad tetrahedron.  Follow around
                // until we reach a good tetrahedron or a boundary.
                adjPerm = tet->adjacentGluing(face);
                adjFace = adjPerm[face];
                while (adj && (adjQuads >= 0)) {
                    swap = NPerm(adjFace,
                        vertexSplitPartner[adjQuads][adjFace]);

                    adjFace = swap[adjFace];
                    adjPerm = adj->adjacentGluing(adjFace) *
                        swap * adjPerm;
                    adj = adj->adjacentTetrahedron(adjFace);
                    adjFace = adjPerm[face];

                    if (adj)
                        adjQuads = quads[ans->tetrahedronIndex(adj)];
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

void NNormalSurface::calculateKnownCanCrush() const {
    if (canCrush.known())
        return;

    // TODO: actually implement this routine.
    return;

    // We'll run through the prisms (defined by slicing tetrahedra along quads)
    // looking for cycles of adjacent prisms; if there are no such cycles
    // then we're fine.

    unsigned long nTet = triangulation->getNumberOfTetrahedra();

    const unsigned char UPPER = 1;
    const unsigned char LOWER = 2;
    unsigned char* seenPrism = new unsigned char[nTet];
    std::fill(seenPrism, seenPrism + nTet, 0);

    // Boolean (seenPrism[tet] & UPPER) tells whether we've already
    // processed the upper prism of tetrahedron tet; similarly for LOWER.
    // We'll define the upper prism to be the prism containing vertex 0.

    // Work out which tetrahedra contain which quad types.
    signed char* quads = new signed char[nTet];
    unsigned long tet = 0;
    for (tet = 0; tet < nTet; tet++) {
        if (getQuadCoord(tet, 0) != 0)
            quads[tet] = 0;
        else if (getQuadCoord(tet, 1) != 0)
            quads[tet] = 1;
        else if (getQuadCoord(tet, 2) != 0)
            quads[tet] = 2;
        else
            quads[tet] = -1;
    }

    // Run through each tetrahedron in turn.
    bool foundCycle = false;
    for (tet = 0; tet < nTet; tet++) {
        if (quads[tet] == -1)
            continue;

        // Process the upper prism.
        if (! (seenPrism[tet] & UPPER)) {
            // TODO
        }

        // Process the lower prism.
        if (! (seenPrism[tet] & LOWER)) {
            // TODO
        }
    }

    // Did we find a cycle of prisms?
    if (! foundCycle)
        canCrush = true;

    delete[] quads;
    delete[] seenPrism;
}

} // namespace regina

