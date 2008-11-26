
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

#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    // Mapping from vertices (0,1,2) of each external face of a new tetrahedron
    //     to the vertices of this new tetrahedron in a 3-2 move.
    // Each new tetrahedron has its vertices numbered so that the corresponding
    //     face embedding permutation for the internal face is the identity.
    // Also, threeTwoVertices[i] refers to face i of the new tetrahedron for
    //     each i.
    const NPerm threeTwoVertices[3] = {
        NPerm(3,1,2,0), NPerm(3,2,0,1), NPerm(3,0,1,2)
    };

    // Mapping from vertices (0,1,2) of each external face of a new tetrahedron
    //     to the vertices of this new tetrahedron in a 2-3 move.
    // Each new tetrahedron has its vertices numbered so that the corresponding
    //     edge embedding permutation for the internal edge is the identity.
    // Also, twoThreeVertices[i] refers to face i of the new tetrahedron for
    //     each i.
    const NPerm twoThreeVertices[2] = {
        NPerm(1,2,3,0), NPerm(0,2,3,1)
    };
}

bool NTriangulation::threeTwoMove(NEdge* e, bool check, bool perform) {
    const std::deque<NEdgeEmbedding>& embs = e->getEmbeddings();
    if (check) {
        if (e->isBoundary())
            return false;
        if (embs.size() != 3)
            return false;
    }

    // Find the unwanted tetrahedra.
    NTetrahedron* oldTet[3];
    NPerm oldVertexPerm[3];
    stdhash::hash_set<NTetrahedron*, HashPointer> oldTets;
    int oldPos = 0;
    for (std::deque<NEdgeEmbedding>::const_iterator it = embs.begin();
            it != embs.end(); it++) {
        oldTet[oldPos] =(*it).getTetrahedron();
        if (check)
            if (! oldTets.insert(oldTet[oldPos]).second)
                return false;
        oldVertexPerm[oldPos] = (*it).getVertices();
        oldPos++;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 3-2 move\n";
    #endif

    // Perform the move.
    ChangeEventBlock block(this);
    int oldPos2, newPos, newPos2;

    // Allocate the new tetrahedra.
    NTetrahedron* newTet[2];
    for (newPos = 0; newPos < 2; newPos++)
        newTet[newPos] = new NTetrahedron();

    // Find the gluings from (0,1,2) of the new tetrahedron faces
    // to the vertices of the old tetrahedra.
    NPerm gluings[2][3];
    for (oldPos = 0; oldPos < 3; oldPos++)
        for (newPos = 0; newPos < 2; newPos++)
            gluings[newPos][oldPos] = oldVertexPerm[oldPos] *
                twoThreeVertices[newPos];

    // Find the tetrahedra to which the old tetrahedron faces are glued,
    // store the gluings from (0,1,2) of the new tetrahedron faces to the
    // vertices of these adjacent tetrahedra, and unjoin the tetrahedra.
    NTetrahedron* adjTet[2][3];
    int adjFace;
    int oldFace;

    for (oldPos = 0; oldPos < 3; oldPos++)
        for (newPos = 0; newPos < 2; newPos++) {
            // Note that gluings[n][o][3] == oldVertexPerm[o][n], since
            // twoThreeVertices[i][3] == i.
            // oldFace = gluings[newPos][oldPos][3];
            oldFace = oldVertexPerm[oldPos][newPos];
            adjTet[newPos][oldPos] =
                oldTet[oldPos]->adjacentTetrahedron(oldFace);
            if (adjTet[newPos][oldPos]) {
                for (oldPos2 = 0; oldPos2 < 3; oldPos2++) {
                    if (adjTet[newPos][oldPos] == oldTet[oldPos2]) {
                        adjFace = oldTet[oldPos]->adjacentFace(oldFace);
                        for (newPos2 = 0; newPos2 < 2; newPos2++)
                            // if (gluings[newPos2][oldPos2][3] == adjFace) {
                            if (oldVertexPerm[oldPos2][newPos2] == adjFace) {
                                // Face oldFace of oldTet[oldPos] is glued to
                                // face adjFace of oldTet[oldPos2] and should be
                                // glued to face oldPos2 of newTet[newPos2].
                                if ((oldPos2 < oldPos) ||
                                        (oldPos2 == oldPos &&
                                        newPos2 < newPos)) {
                                    // We've already seen this gluing from
                                    // the other direction and
                                    // gluings[newPos2][oldPos2] has already
                                    // been modified.  We'll have to leave
                                    // this gluing to be made from the
                                    // other direction.
                                    adjTet[newPos][oldPos] = 0;
                                } else {
                                    adjTet[newPos][oldPos] = newTet[newPos2];
                                    gluings[newPos][oldPos] =
                                        threeTwoVertices[oldPos2]
                                        * gluings[newPos2][oldPos2].inverse()
                                        * oldTet[oldPos]->
                                            adjacentGluing(oldFace)
                                        * gluings[newPos][oldPos];
                                }
                                break;
                            }
                        break;
                    }
                }
                if (oldPos2 >= 3)
                    gluings[newPos][oldPos] =
                        oldTet[oldPos]->adjacentGluing(oldFace)
                        * gluings[newPos][oldPos];
                oldTet[oldPos]->unjoin(oldFace);
            }
        }

    // Remove the old tetrahedra from the triangulation.
    for (oldPos = 0; oldPos < 3; oldPos++)
        delete removeTetrahedron(oldTet[oldPos]);

    // Insert the new tetrahedra into the triangulation.
    for (newPos = 0; newPos < 2; newPos++)
        addTetrahedron(newTet[newPos]);

    // Glue the faces of the new tetrahedra.
    for (oldPos = 0; oldPos < 3; oldPos++)
        for (newPos = 0; newPos < 2; newPos++)
            if (adjTet[newPos][oldPos])
                newTet[newPos]->joinTo(oldPos, adjTet[newPos][oldPos],
                    gluings[newPos][oldPos] *
                    threeTwoVertices[oldPos].inverse());
    newTet[0]->joinTo(3, newTet[1], NPerm());

    // Tidy up.
    gluingsHaveChanged();
    return true;
}

bool NTriangulation::twoThreeMove(NFace* f, bool check, bool perform) {
    if (check) {
        if (f->getNumberOfEmbeddings() != 2)
            return false;
        // We now know that the given face is not on the boundary.
    }

    // Find the unwanted tetrahedra.
    NTetrahedron* oldTet[2];
    NPerm oldVertexPerm[2];
    int oldPos;
    for (oldPos = 0; oldPos < 2; oldPos++) {
        oldTet[oldPos] = f->getEmbedding(oldPos).getTetrahedron();
        oldVertexPerm[oldPos] = f->getEmbedding(oldPos).getVertices();
    }

    if (check)
        if (oldTet[0] == oldTet[1])
            return false;

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-3 move\n";
    #endif

    // Actually perform the move.
    ChangeEventBlock block(this);
    int oldPos2, newPos, newPos2;

    // Allocate the new tetrahedra.
    NTetrahedron* newTet[3];
    for (newPos = 0; newPos < 3; newPos++)
        newTet[newPos] = new NTetrahedron();

    // Find the gluings from (0,1,2) of the new tetrahedron faces
    // to the vertices of the old tetrahedra.
    NPerm gluings[3][2];
    for (oldPos = 0; oldPos < 2; oldPos++)
        for (newPos = 0; newPos < 3; newPos++)
            gluings[newPos][oldPos] = oldVertexPerm[oldPos] *
                threeTwoVertices[newPos];

    // Find the tetrahedra to which the old tetrahedron faces are glued,
    // store the gluings from (0,1,2) of the new tetrahedron faces to the
    // vertices of these adjacent tetrahedra, and unjoin the tetrahedra.
    NTetrahedron* adjTet[3][2];
    int adjFace;
    int oldFace;

    for (oldPos = 0; oldPos < 2; oldPos++)
        for (newPos = 0; newPos < 3; newPos++) {
            // Note that gluings[n][o][3] == oldVertexPerm[o][n], since
            // threeTwoVertices[i][3] == i.
            // oldFace = gluings[newPos][oldPos][3];
            oldFace = oldVertexPerm[oldPos][newPos];
            adjTet[newPos][oldPos] =
                oldTet[oldPos]->adjacentTetrahedron(oldFace);
            if (adjTet[newPos][oldPos]) {
                for (oldPos2 = 0; oldPos2 < 2; oldPos2++) {
                    if (adjTet[newPos][oldPos] == oldTet[oldPos2]) {
                        adjFace = oldTet[oldPos]->adjacentFace(oldFace);
                        for (newPos2 = 0; newPos2 < 3; newPos2++)
                            // if (gluings[newPos2][oldPos2][3] == adjFace) {
                            if (oldVertexPerm[oldPos2][newPos2] == adjFace) {
                                // Face oldFace of oldTet[oldPos] is glued to
                                // face adjFace of oldTet[oldPos2] and should be
                                // glued to face oldPos2 of newTet[newPos2].
                                if ((oldPos2 < oldPos) ||
                                        (oldPos2 == oldPos &&
                                        newPos2 < newPos)) {
                                    // We've already seen this gluing from
                                    // the other direction and
                                    // gluings[newPos2][oldPos2] has already
                                    // been modified.  We'll have to leave
                                    // this gluing to be made from the
                                    // other direction.
                                    adjTet[newPos][oldPos] = 0;
                                } else {
                                    adjTet[newPos][oldPos] = newTet[newPos2];
                                    gluings[newPos][oldPos] =
                                        twoThreeVertices[oldPos2]
                                        * gluings[newPos2][oldPos2].inverse()
                                        * oldTet[oldPos]->
                                            adjacentGluing(oldFace)
                                        * gluings[newPos][oldPos];
                                }
                                break;
                            }
                        break;
                    }
                }
                if (oldPos2 >= 2)
                    gluings[newPos][oldPos] =
                        oldTet[oldPos]->adjacentGluing(oldFace)
                        * gluings[newPos][oldPos];
                oldTet[oldPos]->unjoin(oldFace);
            }
        }

    // Remove the old tetrahedra from the triangulation.
    for (oldPos = 0; oldPos < 2; oldPos++)
        delete removeTetrahedron(oldTet[oldPos]);

    // Insert the new tetrahedra into the triangulation.
    for (newPos = 0; newPos < 3; newPos++)
        addTetrahedron(newTet[newPos]);

    // Glue the faces of the new tetrahedra.
    for (oldPos = 0; oldPos < 2; oldPos++)
        for (newPos = 0; newPos < 3; newPos++)
            if (adjTet[newPos][oldPos])
                newTet[newPos]->joinTo(oldPos, adjTet[newPos][oldPos],
                    gluings[newPos][oldPos] *
                    twoThreeVertices[oldPos].inverse());
    NPerm internalPerm = NPerm(0,1,3,2);
    newTet[0]->joinTo(2, newTet[1], internalPerm);
    newTet[1]->joinTo(2, newTet[2], internalPerm);
    newTet[2]->joinTo(2, newTet[0], internalPerm);

    // Tidy up.
    gluingsHaveChanged();
    return true;
}

bool NTriangulation::fourFourMove(NEdge* e, int newAxis, bool check,
        bool perform) {
    const std::deque<NEdgeEmbedding>& embs = e->getEmbeddings();
    if (check) {
        if (e->isBoundary())
            return false;
        if (embs.size() != 4)
            return false;
    }

    // Find the unwanted tetrahedra.
    NTetrahedron* oldTet[4];
    stdhash::hash_set<NTetrahedron*, HashPointer> oldTets;
    int oldPos = 0;
    for (std::deque<NEdgeEmbedding>::const_iterator it = embs.begin();
            it != embs.end(); it++) {
        oldTet[oldPos] =(*it).getTetrahedron();
        if (check)
            if (! oldTets.insert(oldTet[oldPos]).second)
                return false;
        oldPos++;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 4-4 move\n";
    #endif

    // Perform the 4-4 move as a 2-3 move followed by a 3-2 move.
    ChangeEventBlock block(this);
    NFace* face23 = (newAxis == 0 ?
        oldTet[0]->getFace(embs[0].getVertices()[2]) :
        oldTet[1]->getFace(embs[1].getVertices()[2]));
    int edge32 = embs[3].getEdge();

    twoThreeMove(face23, false, true);
    calculateSkeleton();
    threeTwoMove(oldTet[3]->getEdge(edge32), false, true);

    // Tidy up.  Note that gluingsHaveChanged() was already called by
    // twoThreeMove() and threeTwoMove().
    return true;
}

bool NTriangulation::twoZeroMove(NEdge* e, bool check, bool perform) {
    if (check) {
        if (e->isBoundary())
            return false;
        if (e->getNumberOfEmbeddings() != 2)
            return false;
    }

    NTetrahedron* tet[2];
    NPerm perm[2];

    int i = 0;
    for (std::deque<NEdgeEmbedding>::const_iterator it =
            e->getEmbeddings().begin(); it != e->getEmbeddings().end(); it++) {
        tet[i] = (*it).getTetrahedron();
        perm[i] = (*it).getVertices();
        i++;
    }

    if (check)
        if (tet[0] == tet[1])
            return false;

    if (check) {
        NEdge* edge[2];
        NFace* face[2][2];
            // face[i][j] will be on tetrahedron i opposite vertex j of the
            // internal edge.
        for (i=0; i<2; i++) {
            edge[i] = tet[i]->getEdge(
                NEdge::edgeNumber[perm[i][2]][perm[i][3]]);
            face[i][0] = tet[i]->getFace(perm[i][0]);
            face[i][1] = tet[i]->getFace(perm[i][1]);
        }

        if (edge[0] == edge[1])
            return false;
        if (edge[0]->isBoundary() && edge[1]->isBoundary())
            return false;
        if (face[0][0] == face[1][0])
            return false;
        if (face[0][1] == face[1][1])
            return false;

        // The cases with two pairs of identified faces and with one
        // pair of identified faces plus one pair of boundary faces are
        // all covered by the following check.
        if (tet[0]->getComponent()->getNumberOfTetrahedra() == 2)
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-0 move about edge\n";
    #endif

    // Actually perform the move.
    ChangeEventBlock block(this);

    // Unglue faces from the doomed tetrahedra and glue them to each
    // other.
    NPerm crossover = tet[0]->adjacentGluing(perm[0][2]);
    NPerm gluing;
    NTetrahedron* top;
    NTetrahedron* bottom;
    int topFace;
    for (i=0; i<2; i++) {
        top = tet[0]->adjacentTetrahedron(perm[0][i]);
        bottom = tet[1]->adjacentTetrahedron(perm[1][i]);

        if (! top) {
            // Bottom face becomes boundary.
            tet[1]->unjoin(perm[1][i]);
        } else if (! bottom) {
            // Top face becomes boundary.
            tet[0]->unjoin(perm[0][i]);
        } else {
            // Bottom and top faces join.
            topFace = tet[0]->adjacentFace(perm[0][i]);
            gluing = tet[1]->adjacentGluing(perm[1][i]) *
                crossover * top->adjacentGluing(topFace);
            tet[0]->unjoin(perm[0][i]);
            tet[1]->unjoin(perm[1][i]);
            top->joinTo(topFace, bottom, gluing);
        }
    }

    // Finally remove and dispose of the tetrahedra.
    delete removeTetrahedron(tet[0]);
    delete removeTetrahedron(tet[1]);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool NTriangulation::twoZeroMove(NVertex* v, bool check, bool perform) {
    if (check) {
        if (v->isBoundary())
            return false;
        if (v->getNumberOfEmbeddings() != 2)
            return false;
    }

    NTetrahedron* tet[2];
    int vertex[2];

    std::vector<NVertexEmbedding>::const_iterator it;
    int i = 0;
    for (it = v->getEmbeddings().begin(); it != v->getEmbeddings().end();
            it++) {
        tet[i] = (*it).getTetrahedron();
        vertex[i] = (*it).getVertex();
        i++;
    }

    if (check) {
        if (tet[0] == tet[1])
            return false;

        NFace* face[2];
        for (i = 0; i < 2; i++)
            face[i] = tet[i]->getFace(vertex[i]);
        if (face[0] == face[1])
            return false;
        if (face[0]->isBoundary() && face[1]->isBoundary())
            return false;

        // Check that the tetrahedra are joined along all three faces.
        for (i = 0; i < 4; i++) {
            if (i == vertex[0])
                continue;
            if (tet[0]->adjacentTetrahedron(i) != tet[1])
                return false;
        }
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-0 move about vertex\n";
    #endif

    // Actually perform the move.
    ChangeEventBlock block(this);

    // Unglue faces from the doomed tetrahedra and glue them to each
    // other.
    NPerm crossover;
    if (vertex[0] == 0)
        crossover = tet[0]->adjacentGluing(1);
    else
        crossover = tet[0]->adjacentGluing(0);
    NTetrahedron* top = tet[0]->adjacentTetrahedron(vertex[0]);
    NTetrahedron* bottom = tet[1]->adjacentTetrahedron(vertex[1]);
    int topFace = tet[0]->adjacentFace(vertex[0]);
    NPerm gluing = tet[1]->adjacentGluing(vertex[1]) *
        crossover * top->adjacentGluing(topFace);
    tet[0]->unjoin(vertex[0]);
    tet[1]->unjoin(vertex[1]);
    top->joinTo(topFace, bottom, gluing);

    // Finally remove and dispose of the tetrahedra.
    delete removeTetrahedron(tet[0]);
    delete removeTetrahedron(tet[1]);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool NTriangulation::twoOneMove(NEdge* e, int edgeEnd,
        bool check, bool perform) {
    // edgeEnd is the end opposite where the action is.
    if (check) {
        if (e->isBoundary())
            return false;
        if (e->getNumberOfEmbeddings() != 1)
            return false;
    }

    const NEdgeEmbedding& emb = e->getEmbeddings().front();
    NTetrahedron* oldTet = emb.getTetrahedron();
    NPerm oldVertices = emb.getVertices();

    NTetrahedron* top = oldTet->adjacentTetrahedron(oldVertices[edgeEnd]);
    int otherEdgeEnd = 1 - edgeEnd;

    if (check) {
        if (! top)
            return false;
        if (oldTet->getVertex(oldVertices[edgeEnd])->isBoundary() &&
                oldTet->getVertex(oldVertices[otherEdgeEnd])->isBoundary())
            return false;
    }

    NFace* centreFace = oldTet->getFace(oldVertices[edgeEnd]);
    NFace* bottomFace = oldTet->getFace(oldVertices[otherEdgeEnd]);
    NPerm bottomToTop =
        oldTet->adjacentGluing(oldVertices[edgeEnd]);
    int topGlued[2];
    NEdge* flatEdge[2];
    int i;
    for (i=0; i<2; i++) {
        topGlued[i] = bottomToTop[oldVertices[i + 2]];
        flatEdge[i] = top->getEdge(
            NEdge::edgeNumber[topGlued[i]][bottomToTop[oldVertices[edgeEnd]]]);
    }

    if (check) {
        if (centreFace == bottomFace)
            return false;
        if (flatEdge[0] == flatEdge[1])
            return false;
        if (flatEdge[0]->isBoundary() && flatEdge[1]->isBoundary())
            return false;
        // This next test should follow from the two edges being distinct,
        // but we'll do it anyway.
        if (top->getFace(topGlued[0]) == top->getFace(topGlued[1]))
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-1 move\n";
    #endif

    // Go ahead and perform the move.
    ChangeEventBlock block(this);

    // First glue together the two faces that will be flattened.
    NTetrahedron* adjTet[2];
    int adjFace[2];
    for (i=0; i<2; i++) {
        adjTet[i] = top->adjacentTetrahedron(topGlued[i]);
        adjFace[i] = top->adjacentFace(topGlued[i]);
    }
    NPerm gluing = top->adjacentGluing(topGlued[1])
        * NPerm(topGlued[0], topGlued[1])
        * adjTet[0]->adjacentGluing(adjFace[0]);
    top->unjoin(topGlued[0]);
    top->unjoin(topGlued[1]);
    adjTet[0]->joinTo(adjFace[0], adjTet[1], gluing);

    // Now make the new tetrahedron and glue it to itself.
    NTetrahedron* newTet = new NTetrahedron();
    addTetrahedron(newTet);
    newTet->joinTo(2, newTet, NPerm(2,3));

    // Glue the new tetrahedron into the remaining structure.
    if (oldTet->adjacentTetrahedron(oldVertices[otherEdgeEnd]) == top) {
        // The top of the new tetrahedron must be glued to the bottom.
        int topFace = bottomToTop[oldVertices[otherEdgeEnd]];
        NPerm bottomFacePerm = NPerm(oldVertices[edgeEnd],
            oldVertices[otherEdgeEnd], oldVertices[2], oldVertices[3]);
        NPerm gluing = bottomFacePerm.inverse() *
            top->adjacentGluing(topFace) * bottomToTop *
            bottomFacePerm * NPerm(0,1);
        top->unjoin(topFace);
        newTet->joinTo(0, newTet, gluing);
    } else {
        int bottomFace = oldVertices[otherEdgeEnd];
        int topFace = bottomToTop[bottomFace];
        NTetrahedron* adjTop = top->adjacentTetrahedron(topFace);
        NTetrahedron* adjBottom = oldTet->adjacentTetrahedron(bottomFace);
        NPerm bottomFacePerm = NPerm(oldVertices[edgeEnd],
            oldVertices[otherEdgeEnd], oldVertices[2], oldVertices[3]);
        NPerm bottomGluing = oldTet->adjacentGluing(bottomFace) *
            bottomFacePerm;
        NPerm topGluing = top->adjacentGluing(topFace) *
            bottomToTop * bottomFacePerm * NPerm(0,1);
        top->unjoin(topFace);
        oldTet->unjoin(bottomFace);
        newTet->joinTo(0, adjTop, topGluing);
        newTet->joinTo(1, adjBottom, bottomGluing);
    }

    // Finally remove and dispose of the unwanted tetrahedra.
    delete removeTetrahedron(oldTet);
    delete removeTetrahedron(top);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool NTriangulation::openBook(NFace* f, bool check, bool perform) {
    const NFaceEmbedding& emb = f->getEmbedding(0);
    NTetrahedron* tet = emb.getTetrahedron();
    NPerm vertices = emb.getVertices();

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
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing open book move\n";
    #endif

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    tet->unjoin(emb.getFace());
    gluingsHaveChanged();
    return true;
}

bool NTriangulation::shellBoundary(NTetrahedron* t,
        bool check, bool perform) {
    // To perform the move we don't even need a skeleton.
    if (check) {
        if (! calculatedSkeleton)
            calculateSkeleton();

        int nBdry = 0;
        int face;
        int bdry[4];
        for (face=0; face<4; face++)
            if (t->getFace(face)->isBoundary()) {
                bdry[nBdry] = face;
                nBdry++;
            }
        if (nBdry < 1 || nBdry > 3)
            return false;
        if (nBdry == 1) {
            if (t->getVertex(bdry[0])->isBoundary())
                return false;
        } else if (nBdry == 2) {
            int edge = NEdge::edgeNumber[bdry[0]][bdry[1]];
            if (t->getEdge(edge)->isBoundary())
                return false;
            if (t->adjacentTetrahedron(NEdge::edgeVertex[5 - edge][0]) == t)
                return false;
        }
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing shell boundary move\n";
    #endif

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    removeTetrahedron(t);
    return true;
}

bool NTriangulation::collapseEdge(NEdge* e, bool check, bool perform) {
    // Find the tetrahedra to remove.
    const std::deque<NEdgeEmbedding>& embs = e->getEmbeddings();

    std::deque<NEdgeEmbedding>::const_iterator it;
    NTetrahedron* tet = 0;
    NPerm p;

    if (check) {
        // Check if the vertices are distinct.
        if (e->getVertex(0) == e->getVertex(1))
            return false;

        // Cannot have both vertices ideal.
        if (e->getVertex(0)->isIdeal() && e->getVertex(1)->isIdeal())
            return false;

        // Cannot have an ideal vertex and one in the boundary.
        if ((e->getVertex(0)->isIdeal() && e->getVertex(1)->isBoundary()) ||
                (e->getVertex(1)->isIdeal() && e->getVertex(0)->isBoundary()))
            return false;

        // If both of the vertices are in the boundary then so must the edge.
        if (e->getVertex(0)->isBoundary() && e->getVertex(1)->isBoundary() &&
                ! e->isBoundary())
            return false;

        // Make sure no tetrahedron has a top and bottom face in the boundary.
        for (it = embs.begin(); it != embs.end(); it++) {
            tet = (*it).getTetrahedron();
            p = (*it).getVertices();
            if (! (tet->adjacentTetrahedron(p[0]) ||
                    tet->adjacentTetrahedron(p[1])))
                return false;
        }

        // The tetrahedra around the edge must be distinct.
        {
            stdhash::hash_set<NTetrahedron*, HashPointer> adjTets;
            for (it = embs.begin(); it != embs.end(); it++)
                if (! adjTets.insert((*it).getTetrahedron()).second)
                    return false;
        }

        // The edges meeting either the north or south poles must be distinct.
        // Note this condition is stronger than necessary and will be
        // weakened later.
        {
            stdhash::hash_set<NEdge*, HashPointer> tetEdges;
            for (it = embs.begin(); it != embs.end(); it++) {
                tet = (*it).getTetrahedron();
                p = (*it).getVertices();
                if (! (tetEdges.insert(tet->getEdge(
                            NEdge::edgeNumber[p[0]][p[2]])).second &&
                        tetEdges.insert(tet->getEdge(
                            NEdge::edgeNumber[p[1]][p[2]])).second))
                    return false;
            }

            // Note that tet and p currently correpond to the final
            // edge embedding.
            if (e->isBoundary())
                if (! (tetEdges.insert(tet->getEdge(
                            NEdge::edgeNumber[p[0]][p[3]])).second &&
                        tetEdges.insert(tet->getEdge(
                            NEdge::edgeNumber[p[1]][p[3]])).second))
                    return false;
        }
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing edge collapse move\n";
    #endif

    // Perform the move.
    ChangeEventBlock block(this);
    NPerm topPerm, botPerm;
    NTetrahedron *top, *bot;

    // Clone the edge embeddings because we cannot rely on skeletal
    // objects once we start changing the triangulation.
    std::deque<NEdgeEmbedding> embClones(embs);

    for (it = embClones.begin(); it != embClones.end(); it++) {
        tet = (*it).getTetrahedron();
        p = (*it).getVertices();

        top = tet->adjacentTetrahedron(p[0]);
        topPerm = tet->adjacentGluing(p[0]);
        bot = tet->adjacentTetrahedron(p[1]);
        botPerm = tet->adjacentGluing(p[1]);

        tet->isolate();
        if (top && bot)
            top->joinTo(topPerm[p[0]], bot,
                botPerm * NPerm(p[0], p[1]) * topPerm.inverse());

        delete removeTetrahedron(tet);
    }

    return true;
}

} // namespace regina
