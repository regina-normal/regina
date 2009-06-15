
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

#include "dim4/dim4triangulation.h"
#include "triangulation/nedge.h"

namespace regina {

bool Dim4Triangulation::openBook(Dim4Tetrahedron* t, bool check, bool perform) {
    const Dim4TetrahedronEmbedding& emb = t->getEmbedding(0);
    Dim4Pentachoron* pent = emb.getPentachoron();

    // Check that the face has exactly two boundary edges.
    // Note that this will imply that the face joins two tetrahedra.
    if (check) {
        int i;
        for (i = 0; i < 4; ++i)
            if (! t->getVertex(i)->isValid())
                return false;
        for (i = 0; i < 6; ++i)
            if (! t->getEdge(i)->isValid())
                return false;
        for (i = 0; i < 4; ++i)
            if (! t->getFace(i)->isValid())
                return false;

        NPerm5 vertices = emb.getVertices();

        int nBdry = 0;
        int bdryFace[4];

        for (i = 0; i < 4; ++i)
            if (t->getFace(i)->isBoundary())
                bdryFace[nBdry++] = i;

        if (nBdry < 1 || nBdry > 3)
            return false;

        if (nBdry == 2) {
            // Remaining edge is non-boundary.
            int edge = NEdge::edgeNumber[bdryFace[0]][bdryFace[1]];
            if (t->getEdge(edge)->isBoundary())
                return false;

            // Remaining two faces are not identified.
            if (t->getFace(NEdge::edgeVertex[5 - edge][0]) ==
                    t->getFace(NEdge::edgeVertex[5 - edge][1]))
                return false;
        } else if (nBdry == 1) {
            // Remaining vertex is non-boundary.
            if (t->getVertex(bdryFace[0])->isBoundary())
                return false;

            // No two of the remaining three edges are identified.
            Dim4Edge* internal[3];
            internal[0] = t->getEdge(
                NEdge::edgeNumber[bdryFace[0]][(bdryFace[0] + 1) % 4]);
            internal[1] = t->getEdge(
                NEdge::edgeNumber[bdryFace[0]][(bdryFace[0] + 2) % 4]);
            internal[2] = t->getEdge(
                NEdge::edgeNumber[bdryFace[0]][(bdryFace[0] + 3) % 4]);

            if (internal[0] == internal[1] || internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        }
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    pent->unjoin(emb.getTetrahedron());
    gluingsHaveChanged();
    return true;
}

bool Dim4Triangulation::shellBoundary(Dim4Pentachoron* p,
        bool check, bool perform) {
    // To perform the move we don't even need a skeleton.
    if (check) {
        if (! calculatedSkeleton_)
            calculateSkeleton();

        // All edges and faces must be valid.
        int i;
        for (i = 0; i < 10; ++i)
            if (! p->getEdge(i)->isValid())
                return false;
        for (i = 0; i < 10; ++i)
            if (! p->getFace(i)->isValid())
                return false;

        // Precisely 1, 2, 3 or 4 boundary facets.
        int nBdry = 0;
        int bdry[5];
        for (i = 0; i < 5; ++i)
            if (p->getTetrahedron(i)->isBoundary())
                bdry[nBdry++] = i;
        if (nBdry < 1 || nBdry > 4)
            return false;

        if (nBdry == 1) {
            // Opposite vertex not in boundary.
            if (p->getVertex(bdry[0])->isBoundary())
                return false;

            // No two of the remaining four edges identified.
            Dim4Edge* internal[4];
            int j = 0;
            for (i = 0; i < 5; ++i)
                if (i != bdry[0])
                    internal[j++] = p->getEdge(
                        Dim4Edge::edgeNumber[bdry[0]][i]);

            for (i = 0; i < 4; ++i)
                for (j = i + 1; j < 4; ++j)
                    if (internal[i] == internal[j])
                        return false;
        } else if (nBdry == 2) {
            // Opposite edge not in boundary.
            i = Dim4Edge::edgeNumber[bdry[0]][bdry[1]];
            if (p->getEdge(i)->isBoundary())
                return false;

            // No two of the remaining three faces identified.
            Dim4Face* internal[3];
            int j = 0;
            for (i = 0; i < 5; ++i)
                if (i != bdry[0] && i != bdry[1])
                    internal[j++] = p->getFace(
                        Dim4Face::faceNumber[bdry[0]][bdry[1]][i]);

            if (internal[0] == internal[1] ||
                    internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        } else if (nBdry == 3) {
            // Opposite face not in boundary.
            i = Dim4Face::faceNumber[bdry[0]][bdry[1]][bdry[2]];
            if (p->getFace(i)->isBoundary())
                return false;

            // Remaining two facets not identified.
            if (p->adjacentPentachoron(Dim4Edge::edgeVertex[i][0]) == p)
                return false;
        }
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    removePentachoron(p);
    return true;
}

} // namespace regina
