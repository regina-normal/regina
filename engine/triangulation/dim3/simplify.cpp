
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

#include <algorithm>
#include <set>
#include "triangulation/dim3.h"

namespace regina {

namespace {
    // Mapping from vertices (0,1,2) of each external triangular face of a new
    //     tetrahedron to the vertices of this new tetrahedron in a 3-2 move.
    // Each new tetrahedron has its vertices numbered so that the corresponding
    //     embedding permutation for the internal triangle is the identity.
    // Also, threeTwoVertices[i] refers to face i of the new tetrahedron for
    //     each i.
    const Perm<4> threeTwoVertices[3] = {
        Perm<4>(3,1,2,0), Perm<4>(3,2,0,1), Perm<4>(3,0,1,2)
    };

    // Mapping from vertices (0,1,2) of each external triangular face of a new
    //     tetrahedron to the vertices of this new tetrahedron in a 2-3 move.
    // Each new tetrahedron has its vertices numbered so that the corresponding
    //     embedding permutation for the internal edge is the identity.
    // Also, twoThreeVertices[i] refers to face i of the new tetrahedron for
    //     each i.
    const Perm<4> twoThreeVertices[2] = {
        Perm<4>(1,2,3,0), Perm<4>(0,2,3,1)
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

bool Triangulation<3>::threeTwoMove(Edge<3>* e, bool check, bool perform) {
    if (check) {
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 3)
            return false;
    }

    // Find the unwanted tetrahedra.
    Tetrahedron<3>* oldTet[3];
    Perm<4> oldVertexPerm[3];
    std::set<Tetrahedron<3>*> oldTets;
    int oldPos = 0;
    for (auto& emb : *e) {
        oldTet[oldPos] = emb.tetrahedron();
        if (check)
            if (! oldTets.insert(oldTet[oldPos]).second)
                return false;
        oldVertexPerm[oldPos] = emb.vertices();
        oldPos++;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 3-2 move\n";
    #endif

    // Perform the move.
    ChangeEventSpan span(this);
    int oldPos2, newPos, newPos2;

    // Create the new tetrahedra.
    Tetrahedron<3>* newTet[2];
    for (newPos = 0; newPos < 2; newPos++)
        newTet[newPos] = newTetrahedron();

    // Find the gluings from (0,1,2) of the new tetrahedron faces
    // to the vertices of the old tetrahedra.
    Perm<4> gluings[2][3];
    for (oldPos = 0; oldPos < 3; oldPos++)
        for (newPos = 0; newPos < 2; newPos++)
            gluings[newPos][oldPos] = oldVertexPerm[oldPos] *
                twoThreeVertices[newPos];

    // Find the tetrahedra to which the old tetrahedron faces are glued,
    // store the gluings from (0,1,2) of the new tetrahedron faces to the
    // vertices of these adjacent tetrahedra, and unjoin the tetrahedra.
    Tetrahedron<3>* adjTet[2][3];
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
        removeTetrahedron(oldTet[oldPos]);

    // Glue the faces of the new tetrahedra.
    for (oldPos = 0; oldPos < 3; oldPos++)
        for (newPos = 0; newPos < 2; newPos++)
            if (adjTet[newPos][oldPos])
                newTet[newPos]->join(oldPos, adjTet[newPos][oldPos],
                    gluings[newPos][oldPos] *
                    threeTwoVertices[oldPos].inverse());
    newTet[0]->join(3, newTet[1], Perm<4>());

    // Done!.
    return true;
}

bool Triangulation<3>::twoThreeMove(Triangle<3>* f, bool check, bool perform) {
    if (check) {
        if (f->degree() != 2)
            return false;
        // We now know that the given triangle is not on the boundary.
    }

    // Find the unwanted tetrahedra.
    Tetrahedron<3>* oldTet[2];
    Perm<4> oldVertexPerm[2];
    int oldPos;
    for (oldPos = 0; oldPos < 2; oldPos++) {
        oldTet[oldPos] = f->embedding(oldPos).tetrahedron();
        oldVertexPerm[oldPos] = f->embedding(oldPos).vertices();
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
    ChangeEventSpan span(this);
    int oldPos2, newPos, newPos2;

    // Allocate the new tetrahedra.
    Tetrahedron<3>* newTet[3];
    for (newPos = 0; newPos < 3; newPos++)
        newTet[newPos] = newTetrahedron();

    // Find the gluings from (0,1,2) of the new tetrahedron faces
    // to the vertices of the old tetrahedra.
    Perm<4> gluings[3][2];
    for (oldPos = 0; oldPos < 2; oldPos++)
        for (newPos = 0; newPos < 3; newPos++)
            gluings[newPos][oldPos] = oldVertexPerm[oldPos] *
                threeTwoVertices[newPos];

    // Find the tetrahedra to which the old tetrahedron faces are glued,
    // store the gluings from (0,1,2) of the new tetrahedron faces to the
    // vertices of these adjacent tetrahedra, and unjoin the tetrahedra.
    Tetrahedron<3>* adjTet[3][2];
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
        removeTetrahedron(oldTet[oldPos]);

    // Glue the faces of the new tetrahedra.
    for (oldPos = 0; oldPos < 2; oldPos++)
        for (newPos = 0; newPos < 3; newPos++)
            if (adjTet[newPos][oldPos])
                newTet[newPos]->join(oldPos, adjTet[newPos][oldPos],
                    gluings[newPos][oldPos] *
                    twoThreeVertices[oldPos].inverse());
    Perm<4> internalPerm = Perm<4>(0,1,3,2);
    newTet[0]->join(2, newTet[1], internalPerm);
    newTet[1]->join(2, newTet[2], internalPerm);
    newTet[2]->join(2, newTet[0], internalPerm);

    // Done!
    return true;
}

bool Triangulation<3>::oneFourMove(Tetrahedron<3>* tet, bool /* check */,
        bool perform) {
    if ( !perform )
        return true; // You can always do this move.

    ChangeEventSpan span(this);

    // Before we unglue, record how the adjacent tetrahedra are glued to tet.
    Tetrahedron<3>* adjTet[4];
    Perm<4> adjGlue[4];
    unsigned i, j;
    for (i=0; i<4; i++) {
        adjTet[i] = tet->adjacentTetrahedron(i);
        if (adjTet[i])
            adjGlue[i] = tet->adjacentGluing(i);
    }

    // Unglue the old tetrahedron.
    tet->isolate();

    // The new tetrahedra.
    // Face i of the old tetrahedron will become a facet of newTet[i].
    // Vertex i of newTet[i] will become the new internal vertex, and
    // the other three vertices of newTet[i] will keep the same vertex numbers
    // that they had in the old tetrahedron.
    Tetrahedron<3>* newTet[4];
    for (i = 0; i < 4; ++i)
        newTet[i] = newTetrahedron();

    // Glue the new tetrahedra to each other internally.
    for (i = 0; i < 4; ++i)
        for (j = i + 1; j < 4; ++j)
            newTet[i]->join(j, newTet[j], Perm<4>(i, j));

    // Attach the new tetrahedra to the old triangulation.
    for (i = 0; i < 4; ++i) {
        if (adjTet[i] == tet) {
            // The old tetrahedron was glued to itself.

            // We might have already made this gluing from the other side:
            if (newTet[i]->adjacentTetrahedron(i))
                continue;

            // Nope, do it now.
            newTet[i]->join(i, newTet[adjGlue[i][i]], adjGlue[i]);
        } else if (adjTet[i]) {
            // The old tetrahedron was glued elsewhere.
            newTet[i]->join(i, adjTet[i], adjGlue[i]);
        }
    }

    // Delete the old tetrahedron.
    removeTetrahedron(tet);

    // All done!
    return true;
}

bool Triangulation<3>::fourFourMove(Edge<3>* e, int newAxis, bool check,
        bool perform) {
    if (check) {
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 4)
            return false;
    }

    // Find the unwanted tetrahedra.
    Tetrahedron<3>* oldTet[4];
    std::set<Tetrahedron<3>*> oldTets;
    int oldPos = 0;
    for (auto& emb : *e) {
        oldTet[oldPos] = emb.tetrahedron();
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
    ChangeEventSpan span(this);
    Triangle<3>* tri23 = (newAxis == 0 ?
        oldTet[0]->triangle(e->embedding(0).vertices()[2]) :
        oldTet[1]->triangle(e->embedding(1).vertices()[2]));
    int edge32 = e->embedding(3).edge();

    twoThreeMove(tri23, false, true);
    threeTwoMove(oldTet[3]->edge(edge32), false, true);

    // Done!
    return true;
}

bool Triangulation<3>::twoZeroMove(Edge<3>* e, bool check, bool perform) {
    if (check) {
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 2)
            return false;
    }

    Tetrahedron<3>* tet[2];
    Perm<4> perm[2];

    int i = 0;
    for (auto& emb : *e) {
        tet[i] = emb.tetrahedron();
        perm[i] = emb.vertices();
        i++;
    }

    if (check)
        if (tet[0] == tet[1])
            return false;

    if (check) {
        Edge<3>* edge[2];
        Triangle<3>* triangle[2][2];
            // triangle[i][j] will be on tetrahedron i opposite vertex j of the
            // internal edge.
        for (i=0; i<2; i++) {
            edge[i] = tet[i]->edge(Edge<3>::edgeNumber[perm[i][2]][perm[i][3]]);
            triangle[i][0] = tet[i]->triangle(perm[i][0]);
            triangle[i][1] = tet[i]->triangle(perm[i][1]);
        }

        if (edge[0] == edge[1])
            return false;
        if (edge[0]->isBoundary() && edge[1]->isBoundary())
            return false;
        if (triangle[0][0] == triangle[1][0])
            return false;
        if (triangle[0][1] == triangle[1][1])
            return false;

        // The cases with two pairs of identified triangles and with one
        // pair of identified triangles plus one pair of boundary triangles are
        // all covered by the following check.
        if (tet[0]->component()->size() == 2)
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-0 move about edge\n";
    #endif

    // Actually perform the move.
    ChangeEventSpan span(this);

    // Unglue faces from the doomed tetrahedra and glue them to each
    // other.
    Perm<4> crossover = tet[0]->adjacentGluing(perm[0][2]);
    Perm<4> gluing;
    Tetrahedron<3>* top;
    Tetrahedron<3>* bottom;
    int topFace;
    for (i=0; i<2; i++) {
        top = tet[0]->adjacentTetrahedron(perm[0][i]);
        bottom = tet[1]->adjacentTetrahedron(perm[1][i]);

        if (! top) {
            // Bottom triangle becomes boundary.
            tet[1]->unjoin(perm[1][i]);
        } else if (! bottom) {
            // Top triangle becomes boundary.
            tet[0]->unjoin(perm[0][i]);
        } else {
            // Bottom and top triangles join.
            topFace = tet[0]->adjacentFace(perm[0][i]);
            gluing = tet[1]->adjacentGluing(perm[1][i]) *
                crossover * top->adjacentGluing(topFace);
            tet[0]->unjoin(perm[0][i]);
            tet[1]->unjoin(perm[1][i]);
            top->join(topFace, bottom, gluing);
        }
    }

    // Finally remove and dispose of the tetrahedra.
    removeTetrahedron(tet[0]);
    removeTetrahedron(tet[1]);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool Triangulation<3>::twoZeroMove(Vertex<3>* v, bool check, bool perform) {
    if (check) {
        if (v->link() != Vertex<3>::SPHERE)
            return false;
        if (v->degree() != 2)
            return false;
    }

    Tetrahedron<3>* tet[2];
    int vertex[2];

    int i = 0;
    for (auto& emb : *v) {
        tet[i] = emb.tetrahedron();
        vertex[i] = emb.vertex();
        i++;
    }

    if (check) {
        if (tet[0] == tet[1])
            return false;

        Triangle<3>* triangle[2];
        for (i = 0; i < 2; i++)
            triangle[i] = tet[i]->triangle(vertex[i]);
        if (triangle[0] == triangle[1])
            return false;
        if (triangle[0]->isBoundary() && triangle[1]->isBoundary())
            return false;

        // Check that the tetrahedra are joined along all three triangles.
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
    ChangeEventSpan span(this);

    // Unglue faces from the doomed tetrahedra and glue them to each
    // other.
    Tetrahedron<3>* top = tet[0]->adjacentTetrahedron(vertex[0]);
    Tetrahedron<3>* bottom = tet[1]->adjacentTetrahedron(vertex[1]);

    if (! top) {
        tet[1]->unjoin(vertex[1]);
    } else if (! bottom) {
        tet[0]->unjoin(vertex[0]);
    } else {
        Perm<4> crossover;
        if (vertex[0] == 0)
            crossover = tet[0]->adjacentGluing(1);
        else
            crossover = tet[0]->adjacentGluing(0);
        int topFace = tet[0]->adjacentFace(vertex[0]);
        Perm<4> gluing = tet[1]->adjacentGluing(vertex[1]) *
            crossover * top->adjacentGluing(topFace);
        tet[0]->unjoin(vertex[0]);
        tet[1]->unjoin(vertex[1]);
        top->join(topFace, bottom, gluing);
    }

    // Finally remove and dispose of the tetrahedra.
    removeTetrahedron(tet[0]);
    removeTetrahedron(tet[1]);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool Triangulation<3>::twoOneMove(Edge<3>* e, int edgeEnd,
        bool check, bool perform) {
    // edgeEnd is the end opposite where the action is.
    if (check) {
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 1)
            return false;
    }

    const EdgeEmbedding<3>& emb = e->front();
    Tetrahedron<3>* oldTet = emb.tetrahedron();
    Perm<4> oldVertices = emb.vertices();

    Tetrahedron<3>* top = oldTet->adjacentTetrahedron(oldVertices[edgeEnd]);
    int otherEdgeEnd = 1 - edgeEnd;

    if (check)
        if (! top)
            return false;

    Triangle<3>* centreTri = oldTet->triangle(oldVertices[edgeEnd]);
    Triangle<3>* bottomTri = oldTet->triangle(oldVertices[otherEdgeEnd]);
    Perm<4> bottomToTop =
        oldTet->adjacentGluing(oldVertices[edgeEnd]);
    int topGlued[2];
    Edge<3>* flatEdge[2];
    int i;
    for (i=0; i<2; i++) {
        topGlued[i] = bottomToTop[oldVertices[i + 2]];
        flatEdge[i] = top->edge(
            Edge<3>::edgeNumber[topGlued[i]][bottomToTop[oldVertices[edgeEnd]]]);
    }

    if (check) {
        if (centreTri == bottomTri)
            return false;
        if (flatEdge[0] == flatEdge[1])
            return false;
        if (flatEdge[0]->isBoundary() && flatEdge[1]->isBoundary())
            return false;
        // This next test should follow from the two edges being distinct,
        // but we'll do it anyway.
        if (top->triangle(topGlued[0]) == top->triangle(topGlued[1]))
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing 2-1 move\n";
    #endif

    // Go ahead and perform the move.
    ChangeEventSpan span(this);

    // First glue together the two faces that will be flattened.
    Tetrahedron<3>* adjTet[2];
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

        Perm<4> gluing = top->adjacentGluing(topGlued[1])
            * Perm<4>(topGlued[0], topGlued[1])
            * adjTet[0]->adjacentGluing(adjFace[0]);
        top->unjoin(topGlued[0]);
        top->unjoin(topGlued[1]);
        adjTet[0]->join(adjFace[0], adjTet[1], gluing);
    }

    // Now make the new tetrahedron and glue it to itself.
    Tetrahedron<3>* newTet = newTetrahedron();
    newTet->join(2, newTet, Perm<4>(2,3));

    // Glue the new tetrahedron into the remaining structure.
    if (oldTet->adjacentTetrahedron(oldVertices[otherEdgeEnd]) == top) {
        // The top of the new tetrahedron must be glued to the bottom.
        int topFace = bottomToTop[oldVertices[otherEdgeEnd]];
        Perm<4> bottomFacePerm = Perm<4>(oldVertices[edgeEnd],
            oldVertices[otherEdgeEnd], oldVertices[2], oldVertices[3]);
        Perm<4> gluing = bottomFacePerm.inverse() *
            top->adjacentGluing(topFace) * bottomToTop *
            bottomFacePerm * Perm<4>(0,1);
        top->unjoin(topFace);
        newTet->join(0, newTet, gluing);
    } else {
        int bottomFace = oldVertices[otherEdgeEnd];
        int topFace = bottomToTop[bottomFace];
        Tetrahedron<3>* adjTop = top->adjacentTetrahedron(topFace);
        Tetrahedron<3>* adjBottom = oldTet->adjacentTetrahedron(bottomFace);

        Perm<4> bottomFacePerm = Perm<4>(oldVertices[edgeEnd],
            oldVertices[otherEdgeEnd], oldVertices[2], oldVertices[3]);

        if (adjTop) {
            Perm<4> topGluing = top->adjacentGluing(topFace) *
                bottomToTop * bottomFacePerm * Perm<4>(0,1);
            top->unjoin(topFace);
            newTet->join(0, adjTop, topGluing);
        }
        if (adjBottom) {
            Perm<4> bottomGluing = oldTet->adjacentGluing(bottomFace) *
                bottomFacePerm;
            oldTet->unjoin(bottomFace);
            newTet->join(1, adjBottom, bottomGluing);
        }
    }

    // Finally remove and dispose of the unwanted tetrahedra.
    removeTetrahedron(oldTet);
    removeTetrahedron(top);

    // Tidy up.
    // Properties have already been cleared in removeTetrahedron().
    return true;
}

bool Triangulation<3>::openBook(Triangle<3>* f, bool check, bool perform) {
    const TriangleEmbedding<3>& emb = f->front();
    Tetrahedron<3>* tet = emb.tetrahedron();
    Perm<4> vertices = emb.vertices();

    // Check that the triangle has exactly two boundary edges.
    // Note that this will imply that the triangle joins two tetrahedra.
    if (check) {
        int fVertex = -1;
        int nBdry = 0;
        if (tet->edge(Edge<3>::edgeNumber[vertices[0]][vertices[1]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 2;
        if (tet->edge(Edge<3>::edgeNumber[vertices[1]][vertices[2]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 0;
        if (tet->edge(Edge<3>::edgeNumber[vertices[2]][vertices[0]])->
                isBoundary())
            nBdry++;
        else
            fVertex = 1;

        if (nBdry != 2)
            return false;
        if (tet->vertex(vertices[fVertex])->link() != Vertex<3>::DISC)
            return false;
        if (! f->edge(fVertex)->isValid())
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing open book move\n";
    #endif

    // Actually perform the move.
    // Don't bother with a block since this is so simple.
    tet->unjoin(emb.triangle());
    return true;
}

bool Triangulation<3>::closeBook(Edge<3>* e, bool check, bool perform) {
    if (check) {
        if (! e->isBoundary())
            return false;
    }

    // Find the two triangles on either side of edge e.
    const EdgeEmbedding<3>& front = e->front();
    const EdgeEmbedding<3>& back = e->back();

    Tetrahedron<3>* t0 = front.tetrahedron();
    Tetrahedron<3>* t1 = back.tetrahedron();
    Perm<4> p0 = front.vertices();
    Perm<4> p1 = back.vertices();

    if (check) {
        if (t0->triangle(p0[3]) == t1->triangle(p1[2]))
            return false;
        if (t0->vertex(p0[2]) == t1->vertex(p1[3]))
            return false;
        if (t0->vertex(p0[2])->link() != Vertex<3>::DISC ||
               t1->vertex(p1[3])->link() != Vertex<3>::DISC)
            return false;

        Edge<3>* e1 = t0->edge(Edge<3>::edgeNumber[p0[0]][p0[2]]);
        Edge<3>* e2 = t0->edge(Edge<3>::edgeNumber[p0[1]][p0[2]]);
        Edge<3>* f1 = t1->edge(Edge<3>::edgeNumber[p1[0]][p1[3]]);
        Edge<3>* f2 = t1->edge(Edge<3>::edgeNumber[p1[1]][p1[3]]);

        if (e1 == e2 && f1 == f2)
            return false;
    }

    if (! perform)
        return true;

    #ifdef DEBUG
    std::cerr << "Performing close book move\n";
    #endif

    // Actually perform the move.
    // Don't bother with a block since this is so simple.

    t0->join(p0[3], t1, p1 * Perm<4>(2, 3) * p0.inverse());
    return true;
}

bool Triangulation<3>::shellBoundary(Tetrahedron<3>* t,
        bool check, bool perform) {
    // To perform the move we don't even need a skeleton.
    if (check) {
        ensureSkeleton();

        int nBdry = 0;
        int i, j;
        int bdry[4];
        for (i=0; i<4; i++)
            if (t->triangle(i)->isBoundary())
                bdry[nBdry++] = i;
        if (nBdry < 1 || nBdry > 3)
            return false;
        if (nBdry == 1) {
            if (t->vertex(bdry[0])->isBoundary())
                return false;

            Edge<3>* internal[3];
            j = 0;
            for (i = 0; i < 4; ++i)
                if (i != bdry[0])
                    internal[j++] = t->edge(Edge<3>::edgeNumber[bdry[0]][i]);

            if (! (internal[0]->isValid() &&
                    internal[1]->isValid() &&
                    internal[2]->isValid()))
                return false;

            if (internal[0] == internal[1] ||
                    internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        } else if (nBdry == 2) {
            i = Edge<3>::edgeNumber[bdry[0]][bdry[1]];
            if (t->edge(i)->isBoundary())
                return false;
            if (! t->edge(i)->isValid())
                return false;
            if (t->adjacentTetrahedron(Edge<3>::edgeVertex[5 - i][0]) == t)
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

bool Triangulation<3>::collapseEdge(Edge<3>* e, bool check, bool perform) {
    // Find the tetrahedra to remove.
    std::deque<EdgeEmbedding<3>>::const_iterator it;
    Tetrahedron<3>* tet = 0;
    Perm<4> p;

    if (check) {
        // Note: We never check whether the edge is valid, but this
        // comes automatically from the other tests.  In particular, an
        // invalid edge must join the same vertex to itself.

        // CHECK 0: The tetrahedra around the edge must be distinct.
        // We check this as follows:
        //
        // - None of the triangles containing edge e must contain e twice.
        //   We throw this into check 2 below (see point [0a]).
        //
        // - The only remaining bad case is where a tetrahedron contains
        //   e as two opposite edges.  In this case one can prove that
        //   we have a bad chain of bigons, which will be picked up in
        //   check 2 below.

        // CHECK 1: Can we collapse the edge to a point (creating bigons and
        // pillows with bigon boundaries)?

        // The vertices must be distinct.
        if (e->vertex(0) == e->vertex(1))
            return false;

        // If both vertices are in the boundary then we must be collapsing a
        // boundary edge, and both vertices must have plain old disc links.
        // Recall that ideal vertices return isBoundary() == true.
        if (e->vertex(0)->isBoundary() && e->vertex(1)->isBoundary()) {
            if (! e->isBoundary())
                return false;
            if (e->vertex(0)->link() != Vertex<3>::DISC)
                return false;
            if (e->vertex(1)->link() != Vertex<3>::DISC)
                return false;
        }

        // CHECK 2: Can we flatten each bigon to an edge (leaving
        // triangular pillows behind)?
        //
        // This is trickier.  Even if every individual bigon is okay, we
        // don't want a _chain_ of bigons together to crush a sphere or
        // projective plane.
        //
        // The way we do this is as follows.  Consider each Edge<3>* to be
        // a vertex of some graph G, and consider each bigon to be an edge
        // in this graph G.  The vertices at either end of the edge in G
        // are the (Edge<3>*)s that bound the bigon.
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
        // - verify that the boundary component has more than two triangles;
        //
        // - then ignore both boundary bigons from here onwards.
        //
        // Quite pleasant to deal with in the end.
        if (e->isBoundary())
            if (e->boundaryComponent()->countTriangles() == 2)
                return false;

        {
            size_t nEdges = countEdges();

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

            Edge<3> *upper, *lower;
            long id1, id2;

            // Run through all triangles containing e.
            it = e->begin();

            for ( ; it != e->end(); ++it) {
                tet = it->tetrahedron();
                p = it->vertices();

                upper = tet->edge(Edge<3>::edgeNumber[p[0]][p[2]]);
                lower = tet->edge(Edge<3>::edgeNumber[p[1]][p[2]]);

                if (upper == e || lower == e) {
                    // [0a]: Check 0 fails (see explanation earlier).
                    delete[] depth;
                    delete[] parent;
                    return false;
                }

                // Now that we've run check 0, skip the first (boundary)
                // triangle if e is a boundary edge.  We will skip the
                // last boundary triangle automatically, since for a boundary
                // edge there are k+1 triangles but only k embeddings.
                //
                // We do not need to worry about missing check 0 for
                // the last boundary triangle, since if it fails there then
                // it must also fail for the first.
                if (e->isBoundary() && it == e->begin())
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

        // CHECK 3: Can we flatten each triangular pillow to a triangle?
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
            size_t nTriangles = countTriangles();

            // The parent of each triangle in the union-find tree, or -1 if
            // a triangle is at the root of a tree.
            //
            // This array is indexed by triangle number in the triangulation.
            // The "unified boundary" is assigned the triangle number
            // nTriangles.
            long* parent = new long[nTriangles + 1];
            std::fill(parent, parent + nTriangles + 1, -1);

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nTriangles + 1];
            std::fill(depth, depth + nTriangles + 1, 0);

            Triangle<3> *upper, *lower;
            long id1, id2;

            for (it = e->begin(); it != e->end(); ++it) {
                tet = it->tetrahedron();
                p = it->vertices();

                upper = tet->triangle(p[0]);
                lower = tet->triangle(p[1]);

                id1 = (upper->isBoundary() ? nTriangles : upper->markedIndex());
                id2 = (lower->isBoundary() ? nTriangles : lower->markedIndex());

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
    ChangeEventSpan span(this);
    Perm<4> topPerm, botPerm;
    Tetrahedron<3> *top, *bot;

    // Clone the edge embeddings because we cannot rely on skeletal
    // objects once we start changing the triangulation.
    size_t degree = e->degree();
    Tetrahedron<3>** embTet = new Tetrahedron<3>*[degree];
    Perm<4>* embVertices = new Perm<4>[degree];

    unsigned i;
    for (i = 0, it = e->begin(); it != e->end(); ++i, ++it) {
        embTet[i] = (*it).tetrahedron();
        embVertices[i] = (*it).vertices();
    }

    for (i = 0; i < degree; ++i) {
        top = embTet[i]->adjacentTetrahedron(embVertices[i][0]);
        topPerm = embTet[i]->adjacentGluing(embVertices[i][0]);
        bot = embTet[i]->adjacentTetrahedron(embVertices[i][1]);
        botPerm = embTet[i]->adjacentGluing(embVertices[i][1]);

        embTet[i]->isolate();
        if (top && bot)
            top->join(topPerm[embVertices[i][0]], bot,
                botPerm * Perm<4>(embVertices[i][0], embVertices[i][1]) *
                topPerm.inverse());

        removeTetrahedron(embTet[i]);
    }

    delete[] embVertices;
    delete[] embTet;
    return true;
}

void Triangulation<3>::reorderTetrahedraBFS(bool reverse) {
    unsigned long n = size();
    if (n == 0)
        return;

    ChangeEventSpan span(this);

    // Run a breadth-first search over all tetrahedra.
    Tetrahedron<3>** ordered = new Tetrahedron<3>*[n];
    bool* used = new bool[n];

    std::fill(used, used + n, false);
    unsigned long filled = 0;    /* Placed in ordered[]. */
    unsigned long processed = 0; /* All neighbours placed in ordered[]. */
    unsigned long nextTet = 0;   /* Used to search for connected components. */

    unsigned i;
    Tetrahedron<3> *tet, *adj;
    while (processed < n) {
        if (filled == processed) {
            // Look for the next connected component.
            while (used[nextTet])
                ++nextTet;

            ordered[filled++] = simplices_[nextTet];
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
    simplices_.clear();

    unsigned long j;
    if (reverse) {
        for (j = n; j > 0; )
            simplices_.push_back(ordered[--j]);
    } else {
        for (j = 0; j < n; )
            simplices_.push_back(ordered[j++]);
    }

    delete[] used;
    delete[] ordered;
}

} // namespace regina
