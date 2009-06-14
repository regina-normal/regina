
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

namespace regina {

/*
bool NTriangulation::openBook(NFace* f, bool check, bool perform) {
    const NFaceEmbedding& emb = f->getEmbedding(0);
    NTetrahedron* tet = emb.getTetrahedron();
    NPerm4 vertices = emb.getVertices();

    // Check that the face has exactly two boundary edges.
    // Note that this will imply that the face joins two tetrahedra.
    if (check) {
        int fVertex = -1;
        int nBdry = 0;
        if (tet->getEdge(NEdge::edgeNumber[vertices[0]][vertices[1]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 2;
        if (tet->getEdge(NEdge::edgeNumber[vertices[1]][vertices[2]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 0;
        if (tet->getEdge(NEdge::edgeNumber[vertices[2]][vertices[0]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 1;

        if (nBdry != 2)
            return false;
        if (tet->getVertex(vertices[fVertex])->getLink() != NVertex::DISC)
            return false;
        if (! f->getEdge(fVertex)->isValid())
            return false;
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    tet->unjoin(emb.getFace());
    gluingsHaveChanged();
    return true;
}

bool NTriangulation::closeBook(NEdge* e, bool check, bool perform) {
    if (check) {
        if (! e->isBoundary())
            return false;
    }

    // Find the two faces on either side of edge e.
    const NEdgeEmbedding& front = e->getEmbeddings().front();
    const NEdgeEmbedding& back = e->getEmbeddings().back();

    NTetrahedron* t0 = front.getTetrahedron();
    NTetrahedron* t1 = back.getTetrahedron();
    NPerm4 p0 = front.getVertices();
    NPerm4 p1 = back.getVertices();

    if (check) {
        if (t0->getFace(p0[3]) == t1->getFace(p1[2]))
            return false;
        if (t0->getVertex(p0[2]) == t1->getVertex(p1[3]))
            return false;
        if (t0->getVertex(p0[2])->getLink() != NVertex::DISC ||
               t1->getVertex(p1[3])->getLink() != NVertex::DISC)
            return false;

        NEdge* e1 = t0->getEdge(NEdge::edgeNumber[p0[0]][p0[2]]);
        NEdge* e2 = t0->getEdge(NEdge::edgeNumber[p0[1]][p0[2]]);
        NEdge* f1 = t1->getEdge(NEdge::edgeNumber[p1[0]][p1[3]]);
        NEdge* f2 = t1->getEdge(NEdge::edgeNumber[p1[1]][p1[3]]);

        if (e1 == f1 || e2 == f2)
            return false;
        if (e1 == e2 && f1 == f2)
            return false;
        if (e1 == f2 && f1 == e2)
            return false;
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a block since this is so simple.

    t0->joinTo(p0[3], t1, p1 * NPerm4(2, 3) * p0.inverse());
    gluingsHaveChanged();
    return true;
}
*/

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
