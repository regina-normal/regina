
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

#include <algorithm>
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

    // A helper routine that uses union-find to test whether a graph
    // contains cycles.  This is used by collapseEdge().
    //
    // This routine returns true if the given edge connects two distinct
    // components of the graph, or false if both endpoints of the edge
    // are already in the same component (i.e., a cycle has been created).
    bool unionFindInsert(long* parent, long* depth, long vtx1, long vtx2) {
        // Find the root of the tree containing vtx1 and vtx2.
        long top1, top2;

        for (top1 = vtx1; parent[top1] >= 0; top1 = parent[top1])
            ;
        for (top2 = vtx2; parent[top2] >= 0; top2 = parent[top2])
            ;

        // Are both vertices in the same component?
        if (top1 == top2)
            return false;

        // Join the two components.
        // Insert the shallower tree beneath the deeper tree.
        if (depth[top1] < depth[top2]) {
            parent[top1] = top2;
        } else {
            parent[top2] = top1;
            if (depth[top1] == depth[top2])
                ++depth[top1];
        }
        return true;
    }
}

bool NTriangulation::threeTwoMove(NEdge* e, bool check, bool perform) {
    const std::deque<NEdgeEmbedding>& embs = e->getEmbeddings();
    if (check) {
        if (e->isBoundary() || ! e->isValid())
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
        if (e->isBoundary() || ! e->isValid())
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
        if (e->isBoundary() || ! e->isValid())
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
        if (v->getLink() != NVertex::SPHERE)
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
    NTetrahedron* top = tet[0]->adjacentTetrahedron(vertex[0]);
    NTetrahedron* bottom = tet[1]->adjacentTetrahedron(vertex[1]);

    if (! top) {
        tet[1]->unjoin(vertex[1]);
    } else if (! bottom) {
        tet[0]->unjoin(vertex[0]);
    } else {
        NPerm crossover;
        if (vertex[0] == 0)
            crossover = tet[0]->adjacentGluing(1);
        else
            crossover = tet[0]->adjacentGluing(0);
        int topFace = tet[0]->adjacentFace(vertex[0]);
        NPerm gluing = tet[1]->adjacentGluing(vertex[1]) *
            crossover * top->adjacentGluing(topFace);
        tet[0]->unjoin(vertex[0]);
        tet[1]->unjoin(vertex[1]);
        top->joinTo(topFace, bottom, gluing);
    }

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
        if (e->isBoundary() || ! e->isValid())
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
    adjTet[0] = top->adjacentTetrahedron(topGlued[0]);
    adjTet[1] = top->adjacentTetrahedron(topGlued[1]);

    if (! adjTet[0])
        top->unjoin(topGlued[1]);
    else if (! adjTet[1])
        top->unjoin(topGlued[0]);
    else {
        int adjFace[2];
        adjFace[0] = top->adjacentFace(topGlued[0]);
        adjFace[1] = top->adjacentFace(topGlued[1]);

        NPerm gluing = top->adjacentGluing(topGlued[1])
            * NPerm(topGlued[0], topGlued[1])
            * adjTet[0]->adjacentGluing(adjFace[0]);
        top->unjoin(topGlued[0]);
        top->unjoin(topGlued[1]);
        adjTet[0]->joinTo(adjFace[0], adjTet[1], gluing);
    }

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

        if (adjTop) {
            NPerm topGluing = top->adjacentGluing(topFace) *
                bottomToTop * bottomFacePerm * NPerm(0,1);
            top->unjoin(topFace);
            newTet->joinTo(0, adjTop, topGluing);
        }
        if (adjBottom) {
            NPerm bottomGluing = oldTet->adjacentGluing(bottomFace) *
                bottomFacePerm;
            oldTet->unjoin(bottomFace);
            newTet->joinTo(1, adjBottom, bottomGluing);
        }
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
        if (! f->getEdge(fVertex)->isValid())
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
    NPerm p0 = front.getVertices();
    NPerm p1 = back.getVertices();

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

    #ifdef DEBUG
    std::cerr << "Performing close book move\n";
    #endif

    // Actually perform the move.
    // Don't bother with a block since this is so simple.

    t0->joinTo(p0[3], t1, p1 * NPerm(2, 3) * p0.inverse());
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
        int i, j;
        int bdry[4];
        for (i=0; i<4; i++)
            if (t->getFace(i)->isBoundary())
                bdry[nBdry++] = i;
        if (nBdry < 1 || nBdry > 3)
            return false;
        if (nBdry == 1) {
            if (t->getVertex(bdry[0])->isBoundary())
                return false;

            NEdge* internal[3];
            j = 0;
            for (i = 0; i < 4; ++i)
                if (i != bdry[0])
                    internal[j++] = t->getEdge(NEdge::edgeNumber[bdry[0]][i]);

            if (! (internal[0]->isValid() &&
                    internal[1]->isValid() &&
                    internal[2]->isValid()))
                return false;

            if (internal[0] == internal[1] ||
                    internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        } else if (nBdry == 2) {
            i = NEdge::edgeNumber[bdry[0]][bdry[1]];
            if (t->getEdge(i)->isBoundary())
                return false;
            if (! t->getEdge(i)->isValid())
                return false;
            if (t->adjacentTetrahedron(NEdge::edgeVertex[5 - i][0]) == t)
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
        // CHECK 0: The tetrahedra around the edge must be distinct.
        // We check this as follows:
        //
        // - None of the faces containing edge e must contain e twice.
        //   We throw this into check 2 below (see point [0a]).
        //
        // - The only remaining bad case is where a tetrahedron contains
        //   e as two opposite edges.  In this case one can prove that
        //   we have a bad chain of bigons, which will be picked up in
        //   check 2 below.

        // CHECK 1: Can we collapse the edge to a point (creating bigons and
        // pillows with bigon boundaries)?

        // The vertices must be distinct.
        if (e->getVertex(0) == e->getVertex(1))
            return false;

        // If both vertices are in the boundary then we must be collapsing a
        // boundary edge, and both vertices must have plain old disc links.
        // Recall that ideal vertices return isBoundary() == true.
        if (e->getVertex(0)->isBoundary() && e->getVertex(1)->isBoundary()) {
            if (! e->isBoundary())
                return false;
            if (e->getVertex(0)->getLink() != NVertex::DISC)
                return false;
            if (e->getVertex(1)->getLink() != NVertex::DISC)
                return false;
        }

        // CHECK 2: Can we flatten each bigon to an edge (leaving
        // triangular pillows behind)?
        //
        // This is trickier.  Even if every individual bigon is okay, we
        // don't want a _chain_ of bigons together to crush a sphere or
        // projective plane.
        //
        // The way we do this is as follows.  Consider each NEdge* to be
        // a vertex of some graph G, and consider each bigon to be an edge
        // in this graph G.  The vertices at either end of the edge in G
        // are the (NEdge*)s that bound the bigon.
        //
        // We can happily flatten each bigon if and only if the graph G
        // contains no cycles.  We shall test this using union-find,
        // which should have log-linear complexity.
        //
        // We deal with boundary edges and invalid edges as follows.
        // All boundary and/or invalid edges become the *same* vertex in
        // the graph G.  This means, for instance, that a bigon joining two
        // distinct boundary edges is not allowed.  Invalid edges are
        // included here because each invalid edge contains a projective
        // plane cusp at its centre.
        //
        // If edge e is itself a boundary edge, things become more
        // interesting again.  In this case, the two *boundary* bigons
        // are not subject to the same restrictions -- crushing bigons
        // along the boundary does no harm, *unless* the boundary bigon
        // edges themselves form a cycle.  This is essentially the same
        // dilemma as before but one dimension down.  We can detect this
        // because it implies either:
        //
        // - two edges of the same bigon are identified, and hence the
        //   two vertices of edge e are identified (which has already
        //   been disallowed in check 1 above);
        //
        // - the four edges of the two boundary bigons are identified in
        //   pairs, which means the entire boundary component consists
        //   of the two bigons and nothing else.
        //
        // What does this mean in a practical sense?  If edge e is a
        // boundary edge, we:
        //
        // - verify that the boundary component has more than two faces;
        //
        // - then ignore both boundary bigons from here onwards.
        //
        // Quite pleasant to deal with in the end.
        if (e->isBoundary())
            if (e->getBoundaryComponent()->getNumberOfFaces() == 2)
                return false;

        {
            long nEdges = edges.size();

            // The parent of each edge in the union-find tree, or -1 if
            // an edge is at the root of a tree.
            //
            // This array is indexed by edge number in the triangulation.
            // Although we might not use many of these edges, it's fast
            // and simple.  The "unified boundary" is assigned the edge
            // number nEdges.
            long* parent = new long[nEdges + 1];
            std::fill(parent, parent + nEdges + 1, -1);

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nEdges + 1];
            std::fill(depth, depth + nEdges + 1, 0);

            NEdge *upper, *lower;
            long id1, id2;

            // Run through all faces containing e.
            it = embs.begin();

            for ( ; it != embs.end(); ++it) {
                tet = it->getTetrahedron();
                p = it->getVertices();

                upper = tet->getEdge(NEdge::edgeNumber[p[0]][p[2]]);
                lower = tet->getEdge(NEdge::edgeNumber[p[1]][p[2]]);

                if (upper == e || lower == e) {
                    // [0a]: Check 0 fails (see explanation earlier).
                    delete[] depth;
                    delete[] parent;
                    return false;
                }

                // Now that we've run check 0, skip the first (boundary)
                // face if e is a boundary edge.  We will skip the
                // last boundary face automatically, since for a boundary
                // edge there are k+1 faces but only k embeddings.
                //
                // We do not need to worry about missing check 0 for
                // the last boundary face, since if it fails there then
                // it must also fail for the first.
                if (e->isBoundary() && it == embs.begin())
                    continue;

                id1 = ((upper->isBoundary() || ! upper->isValid()) ?
                    nEdges : upper->markedIndex());
                id2 = ((lower->isBoundary() || ! lower->isValid()) ?
                    nEdges : lower->markedIndex());

                // This bigon joins nodes id1 and id2 in the graph G.
                if (! unionFindInsert(parent, depth, id1, id2)) {
                    delete[] depth;
                    delete[] parent;
                    return false;
                }
            }

            // No bad chains of bigons!
            delete[] depth;
            delete[] parent;
        }

        // CHECK 3: Can we flatten each triangular pillow to a face?
        //
        // Again, even if each individual pillow is okay, we don't want
        // a chain of pillows together to completely crush away a
        // 3-manifold component.
        //
        // This means no cycles of pillows, and no chains of pillows
        // that run from boundary to boundary.
        //
        // Test this in the same way that we tested edges.  It's kind of
        // overkill, since each vertex in the corresponding graph G will
        // have degree <= 2, but it's fast so we'll do it.
        {
            long nFaces = faces.size();

            // The parent of each face in the union-find tree, or -1 if
            // a face is at the root of a tree.
            //
            // This array is indexed by face number in the triangulation.
            // The "unified boundary" is assigned the face number nFaces.
            long* parent = new long[nFaces + 1];
            std::fill(parent, parent + nFaces + 1, -1);

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nFaces + 1];
            std::fill(depth, depth + nFaces + 1, 0);

            NFace *upper, *lower;
            long id1, id2;

            for (it = embs.begin(); it != embs.end(); ++it) {
                tet = it->getTetrahedron();
                p = it->getVertices();

                upper = tet->getFace(p[0]);
                lower = tet->getFace(p[1]);

                id1 = (upper->isBoundary() ? nFaces : upper->markedIndex());
                id2 = (lower->isBoundary() ? nFaces : lower->markedIndex());

                // This pillow joins nodes id1 and id2 in the graph G.
                if (! unionFindInsert(parent, depth, id1, id2)) {
                    delete[] depth;
                    delete[] parent;
                    return false;
                }
            }

            // No bad chains of bigons!
            delete[] depth;
            delete[] parent;
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

void NTriangulation::reorderTetrahedraBFS(bool reverse) {
    unsigned n = getNumberOfTetrahedra();
    if (n == 0)
        return;

    ChangeEventBlock block(this);

    // Run a breadth-first search over all tetrahedra.
    NTetrahedron** ordered = new NTetrahedron*[n];
    bool* used = new bool[n];

    std::fill(used, used + n, false);
    unsigned filled = 0;    /* Placed in the ordered array. */
    unsigned processed = 0; /* All neighbours placed in the ordered array. */
    unsigned nextTet = 0;   /* Used to search for connected components. */

    unsigned i;
    NTetrahedron *tet, *adj;
    while (processed < n) {
        if (filled == processed) {
            // Look for the next connected component.
            while (used[nextTet])
                ++nextTet;

            ordered[filled++] = tetrahedra[nextTet];
            used[nextTet] = true;
            ++nextTet;
        }

        tet = ordered[processed];

        // Add all neighbours of tet to the queue.
        for (i = 0; i < 4; ++i)
            if ((adj = tet->adjacentTetrahedron(i)))
                if (! used[adj->markedIndex()]) {
                    ordered[filled++] = adj;
                    used[adj->markedIndex()] = true;
                }

        ++processed;
    }

    // Flush the tetrahedra from the triangulation, and reinsert them in
    // the order in which they were found during the breadth-first search.
    tetrahedra.clear();

    if (reverse) {
        for (i = n; i > 0; )
            addTetrahedron(ordered[--i]);
    } else {
        for (i = 0; i < n; )
            addTetrahedron(ordered[i++]);
    }

    delete[] used;
    delete[] ordered;
}

} // namespace regina
