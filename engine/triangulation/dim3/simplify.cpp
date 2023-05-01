
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
    // A helper routine that uses union-find to test whether a graph
    // contains cycles.  This is used by collapseEdge().
    //
    // This routine returns true if the given edge connects two distinct
    // components of the graph, or false if both endpoints of the edge
    // are already in the same component (i.e., a cycle has been created).
    bool unionFindInsert(ssize_t* parent, size_t* depth,
            size_t vtx1, size_t vtx2) {
        // Find the root of the tree containing vtx1 and vtx2.
        ssize_t top1, top2;

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
        oldTet[oldPos] = emb.simplex();
        if (check)
            if (! oldTets.insert(emb.simplex()).second)
                return false;
        if (emb.simplex()->locks_) {
            if (emb.simplex()->isLocked() ||
                    emb.simplex()->isFacetLocked(emb.vertices()[2]) ||
                    emb.simplex()->isFacetLocked(emb.vertices()[3])) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "4-4 move using a locked tetrahedron and/or facet");
            }
        }
        oldPos++;
    }

    if (! perform)
        return true;

    // Perform the 4-4 move as a 2-3 move followed by a 3-2 move.
    // Note that, by using pachner(), we also preserve orientation
    // (if the triangulation was originally oriented).
    //
    // We store the second (3-2) move using a tetrahedron-edge pair, since
    // by the time we perform it the original skeleton will be destroyed.
    //
    // The two calls to pachner() can manage any lock updates without our help.
    TopologyLock lock(*this);
    ChangeEventGroup span(*this);
    Triangle<3>* tri23 = (newAxis == 0 ?
        oldTet[0]->triangle(e->embedding(0).vertices()[2]) :
        oldTet[1]->triangle(e->embedding(1).vertices()[2]));
    int edge32 = e->embedding(3).edge();

    pachner(tri23, false, true);
    pachner(oldTet[3]->edge(edge32), false, true);

    // Done!
    return true;
}

bool Triangulation<3>::twoZeroMove(Edge<3>* e, bool check, bool perform) {
    using LockMask = Simplex<3>::LockMask;

    if (check) {
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 2)
            return false;
    }

    Tetrahedron<3>* tet[2];
    Perm<4> perm[2];
    // Note whether we need to merge facet locks opposite vertices 0,1 of e:
    bool lockExterior[2] = { false, false };

    int i = 0;
    for (auto& emb : *e) {
        tet[i] = emb.tetrahedron();
        perm[i] = emb.vertices();

        if (tet[i]->locks_) {
            // The only things that can be locked are the two exterior facets.
            if (tet[i]->isLocked()) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "2-0 move using a locked tetrahedron");
            }
            for (int v = 0; v < 2; ++v)
                if (tet[i]->isFacetLocked(perm[i][v]))
                    lockExterior[v] = true;
            for (int v = 2; v < 4; ++v)
                if (tet[i]->isFacetLocked(perm[i][v])) {
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform a "
                            "2-0 move around a locked triangle");
                }
        }

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

    // Actually perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    // Unglue faces from the doomed tetrahedra and glue them to each other.
    Perm<4> crossover = tet[0]->adjacentGluing(perm[0][2]);
    for (i=0; i<2; i++) {
        // Process the facets opposite vertex i of the edge e.
        Tetrahedron<3>* top = tet[0]->adjacentTetrahedron(perm[0][i]);
        Tetrahedron<3>* bottom = tet[1]->adjacentTetrahedron(perm[1][i]);

        if (! top) {
            // Bottom triangle becomes boundary.
            if (lockExterior[i])
                bottom->lockFacetRaw(tet[1]->adjacentFacet(perm[1][i]));
            tet[1]->unjoinRaw(perm[1][i]);
        } else if (! bottom) {
            // Top triangle becomes boundary.
            if (lockExterior[i])
                top->lockFacetRaw(tet[0]->adjacentFacet(perm[0][i]));
            tet[0]->unjoinRaw(perm[0][i]);
        } else {
            // Bottom and top triangles join.
            int topFace = tet[0]->adjacentFace(perm[0][i]);
            int bottomFace = tet[1]->adjacentFace(perm[1][i]);
            if (lockExterior[i]) {
                top->lockFacetRaw(topFace);
                bottom->lockFacetRaw(bottomFace);
            }
            Perm<4> gluing = tet[1]->adjacentGluing(perm[1][i]) *
                crossover * top->adjacentGluing(topFace);
            tet[0]->unjoinRaw(perm[0][i]);
            tet[1]->unjoinRaw(perm[1][i]);
            top->joinRaw(topFace, bottom, gluing);
        }
    }

    // Finally remove and dispose of the tetrahedra.
    removeSimplexRaw(tet[0]);
    removeSimplexRaw(tet[1]);

    return true;
}

bool Triangulation<3>::twoZeroMove(Vertex<3>* v, bool check, bool perform) {
    using LockMask = Simplex<3>::LockMask;

    if (check) {
        if (v->linkType() != Vertex<3>::SPHERE)
            return false;
        if (v->degree() != 2)
            return false;
    }

    Tetrahedron<3>* tet[2];
    int vertex[2];
    bool lockExterior = false;

    int i = 0;
    for (auto& emb : *v) {
        tet[i] = emb.tetrahedron();
        vertex[i] = emb.vertex();

        if (tet[i]->locks_) {
            // The only thing that can be locked is the exterior facet.
            if (tet[i]->locks_ != (LockMask(1) << vertex[i])) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "2-0 move using a locked tetrahedron and/or facet");
            }
            // Remember that, when we perform the move, the two merged
            // exterior facets need to be locked from both sides.
            lockExterior = true;
        }

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

    // Actually perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    // Unglue faces from the doomed tetrahedra and glue them to each
    // other.
    Tetrahedron<3>* top = tet[0]->adjacentTetrahedron(vertex[0]);
    Tetrahedron<3>* bottom = tet[1]->adjacentTetrahedron(vertex[1]);

    if (! top) {
        if (lockExterior)
            bottom->lockFacetRaw(tet[1]->adjacentFacet(vertex[1]));
        tet[1]->unjoinRaw(vertex[1]);
    } else if (! bottom) {
        if (lockExterior)
            top->lockFacetRaw(tet[0]->adjacentFacet(vertex[0]));
        tet[0]->unjoinRaw(vertex[0]);
    } else {
        int topFace = tet[0]->adjacentFace(vertex[0]);
        int bottomFace = tet[1]->adjacentFace(vertex[1]);
        if (lockExterior) {
            top->lockFacetRaw(topFace);
            bottom->lockFacetRaw(bottomFace);
        }
        Perm<4> crossover = tet[0]->adjacentGluing(vertex[0] == 0 ? 1 : 0);
        Perm<4> gluing = tet[1]->adjacentGluing(vertex[1]) *
            crossover * top->adjacentGluing(topFace);
        tet[0]->unjoinRaw(vertex[0]);
        tet[1]->unjoinRaw(vertex[1]);
        top->joinRaw(topFace, bottom, gluing);
    }

    // Finally remove and dispose of the tetrahedra.
    removeSimplexRaw(tet[0]);
    removeSimplexRaw(tet[1]);

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
    Perm<4> bottomToTop = oldTet->adjacentGluing(oldVertices[edgeEnd]);
    int topGlued[2];
    Edge<3>* flatEdge[2];
    for (int i=0; i<2; i++) {
        topGlued[i] = bottomToTop[oldVertices[i + 2]];
        flatEdge[i] = top->edge(
            Edge<3>::edgeNumber[topGlued[i]][bottomToTop[oldVertices[edgeEnd]]]);
    }

    int bottomFace = oldVertices[otherEdgeEnd]; // face of oldTet
    int topFace = bottomToTop[bottomFace]; // face of top

    using LockMask = Simplex<3>::LockMask;
    if (oldTet->locks_) {
        // The only lock that *is* allowed in oldTet is the bottom face.
        if (oldTet->locks_ != (LockMask(1) << oldVertices[otherEdgeEnd])) {
            if (check)
                return false;
            if (perform)
                throw LockViolation("An attempt was made to perform a "
                    "2-1 move using a locked tetrahedron and/or facet");
        }
    }
    if (top->isLocked()) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform a "
                "2-1 move using a locked tetrahedron");
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

    // Go ahead and perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    // First glue together the two faces that will be flattened.
    Tetrahedron<3>* adjTet[2];
    adjTet[0] = top->adjacentTetrahedron(topGlued[0]);
    adjTet[1] = top->adjacentTetrahedron(topGlued[1]);

    if (! adjTet[0]) {
        // We are merging a boundary triangle with a non-boundary triangle.
        if (top->isFacetLocked(topGlued[0]))
            adjTet[1]->lockFacetRaw(top->adjacentFacet(topGlued[1]));
        top->unjoinRaw(topGlued[1]);
    } else if (! adjTet[1]) {
        // We are merging a boundary triangle with a non-boundary triangle.
        if (top->isFacetLocked(topGlued[1]))
            adjTet[0]->lockFacetRaw(top->adjacentFacet(topGlued[0]));
        top->unjoinRaw(topGlued[0]);
    } else {
        // We are merging two internal triangles.
        int adjFace[2];
        adjFace[0] = top->adjacentFacet(topGlued[0]);
        adjFace[1] = top->adjacentFacet(topGlued[1]);

        if (top->isFacetLocked(topGlued[0]))
            adjTet[1]->lockFacetRaw(adjFace[1]);
        if (top->isFacetLocked(topGlued[1]))
            adjTet[0]->lockFacetRaw(adjFace[0]);

        Perm<4> gluing = top->adjacentGluing(topGlued[1])
            * Perm<4>(topGlued[0], topGlued[1])
            * adjTet[0]->adjacentGluing(adjFace[0]);
        top->unjoinRaw(topGlued[0]);
        top->unjoinRaw(topGlued[1]);
        adjTet[0]->joinRaw(adjFace[0], adjTet[1], gluing);
    }

    // Now make the new tetrahedron and glue it to itself.
    Tetrahedron<3>* newTet = newSimplexRaw();
    newTet->joinRaw(2, newTet, {2,3});

    // Glue the new tetrahedron into the remaining structure.
    Perm<4> bottomFacePerm = oldVertices * Perm<4>(edgeEnd, otherEdgeEnd, 2, 3);
    if (oldTet->adjacentTetrahedron(bottomFace) == top) {
        // The top of the new tetrahedron must be glued to the bottom.
        if (top->isFacetLocked(topFace) || oldTet->isFacetLocked(bottomFace))
            newTet->locks_ = 3; // Locks facets 0 and 1 of the new tetrahedron
        Perm<4> gluing = bottomFacePerm.inverse() *
            top->adjacentGluing(topFace) * bottomToTop *
            bottomFacePerm * Perm<4>(0,1);
        top->unjoinRaw(topFace);
        newTet->joinRaw(0, newTet, gluing);
    } else {
        Tetrahedron<3>* adjTop = top->adjacentTetrahedron(topFace);
        Tetrahedron<3>* adjBottom = oldTet->adjacentTetrahedron(bottomFace);

        if (bottomFacePerm.sign() < 0) {
            // Switch vertices 2,3 in newTet so we can preserve orientation.
            bottomFacePerm = bottomFacePerm * Perm<4>(2, 3);
        }

        if (adjTop) {
            Perm<4> topGluing = top->adjacentGluing(topFace) *
                bottomToTop * bottomFacePerm * Perm<4>(0,1);
            if (top->isFacetLocked(topFace))
                newTet->locks_ |= 1; // Lock facet 0 of the new tetrahedron
            top->unjoinRaw(topFace);
            newTet->joinRaw(0, adjTop, topGluing);
        }
        if (adjBottom) {
            Perm<4> bottomGluing = oldTet->adjacentGluing(bottomFace) *
                bottomFacePerm;
            if (oldTet->isFacetLocked(bottomFace))
                newTet->locks_ |= 2; // Lock facet 1 of the new tetrahedron
            oldTet->unjoinRaw(bottomFace);
            newTet->joinRaw(1, adjBottom, bottomGluing);
        }
    }

    // Finally remove and dispose of the unwanted tetrahedra.
    removeSimplexRaw(oldTet);
    removeSimplexRaw(top);

    return true;
}

bool Triangulation<3>::zeroTwoMove(
        EdgeEmbedding<3> e0, int t0, EdgeEmbedding<3> e1, int t1,
        bool check, bool perform ) {
    Edge<3>* e = e0.tetrahedron()->edge(e0.edge());

    if (check) {
        if (e != e1.tetrahedron()->edge(e1.edge()))
            return false;
        if (t0 < 2 || t0 > 3 || t1 < 2 || t1 > 3)
            return false;
        if (! e->isValid())
            return false;
    }

    if (e0.simplex()->isFacetLocked(e0.vertices()[t0]) ||
            e1.simplex()->isFacetLocked(e1.vertices()[t1])) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform a "
                "0-2 move using a locked triangle");
    }

    if (! perform)
        return true;

    // Work out how to glue in the two new tetrahedra.
    EdgeEmbedding<3> emb[2] = {e0, e1};
    int t[2] = {t0, t1};
    Perm<4> ident = Perm<4>();
    Perm<4> trans = Perm<4>(2, 3);
    int simTemp[2][2] = {};
    Tetrahedron<3>* sim[2][2];
    Perm<4> ver[2][2];

    // Rather than separately handling all the corner cases when the 0-2
    // move involves a boundary triangle, we will temporarily glue in up to
    // two extra tetrahedra so that we can just perform the move as if it
    // involves only non-boundary triangles. To do this, we first need to
    // work out how our edge e meets the boundary.
    size_t deg = e->degree();
    bool bdy[2] = {
        e0.tetrahedron()->triangle( e0.vertices()[t0] )->isBoundary(),
        e1.tetrahedron()->triangle( e1.vertices()[t1] )->isBoundary()
    };
    Tetrahedron<3>* bdySim[2];
    Perm<4> bdyVer[2];
    bool distinct;
    if ( bdy[0] or bdy[1] ) {
        bdySim[0] = e->embedding(0).simplex();
        bdyVer[0] = e->embedding(0).vertices();
        bdySim[1] = e->embedding( deg - 1 ).simplex();
        bdyVer[1] = e->embedding( deg - 1 ).vertices();
        distinct = ( bdySim[0]->triangle( bdyVer[0][3] ) !=
                bdySim[1]->triangle( bdyVer[1][2] ) );
    }
    Perm<4> tempGlu[2];
    int tempFace[2];
    for ( int k : {0, 1} ) {
        if ( bdyVer[k].sign() > 0 ) {
            tempGlu[k] = bdyVer[k] * trans;
            tempFace[k] = 2 + k;
        } else {
            tempGlu[k] = bdyVer[k];
            tempFace[k] = 3 - k;
        }
    }

    for ( int i : {0, 1} ) {
        if ( bdy[i] ) {
            if ( t[i] == 2 ) {
                sim[i][0] = bdySim[1];
                ver[i][0] = bdyVer[1];
                simTemp[i][1] = 2;
                ver[i][1] = (
                        (bdyVer[1].sign() > 0) ? ident : trans );
            } else {
                sim[i][1] = bdySim[0];
                ver[i][1] = bdyVer[0];
                simTemp[i][0] = 1;
                ver[i][0] = (
                        (bdyVer[0].sign() > 0) ? ident : trans );
            }
        } else {
            if ( t[i] == 2 ) {
                sim[i][0] = emb[i].simplex();
                ver[i][0] = emb[i].vertices();
                sim[i][1] = sim[i][0]->adjacentSimplex(
                        ver[i][0][2] );
                ver[i][1] = sim[i][0]->adjacentGluing(
                        ver[i][0][2] ) * ver[i][0] * trans;
            } else {
                sim[i][1] = emb[i].simplex();
                ver[i][1] = emb[i].vertices();
                sim[i][0] = sim[i][1]->adjacentSimplex(
                        ver[i][1][3] );
                ver[i][0] = sim[i][1]->adjacentGluing(
                        ver[i][1][3] ) * ver[i][1] * trans;
            }
        }
    }

    // Actually perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    auto tet = newSimplicesRaw<2>();

    // Temporary tetrahedra for handling boundary triangles.
    Tetrahedron<3>* temp[2];
    if ( bdy[0] or bdy[1] ) {
        temp[0] = newSimplexRaw();
        temp[0]->joinRaw( tempFace[0], bdySim[0], tempGlu[0] );
        if ( distinct ) {
            temp[1] = newSimplexRaw();
            temp[1]->joinRaw( tempFace[1], bdySim[1], tempGlu[1] );
        } else {
            temp[1] = temp[0];
            for ( int i : {0, 1} ) {
                if ( bdy[i] and t[i] == 2 ) {
                    ver[i][1] = bdySim[1]->adjacentGluing( bdyVer[1][2] )
                        * ver[i][0] * trans;
                }
            }
        }
        for ( int i : {0, 1} ) {
            for ( int j : {0, 1} ) {
                if ( simTemp[i][j] > 0 ) {
                    sim[i][j] = temp[ simTemp[i][j] - 1 ];
                }
            }
        }
    }

    // We use the orient permutation to ensure that if this triangulation
    // was originally oriented, then this orientation will be preserved by
    // the 0-2 move.`
    Perm<4> orient = ( (ver[0][0].sign() > 0) ? trans : ident );
    Perm<4> gluing = sim[0][0]->adjacentGluing( ver[0][0][2] );
    for ( int i : {0, 1} ) {
        sim[i][0]->unjoinRaw( ver[i][0][2] );
    }
    tet[0]->joinRaw( orient[2], sim[0][0], ver[0][0] * orient );
    for ( int i : {0, 1} ) {
        tet[0]->joinRaw( i, tet[1], trans );
    }
    if ( sim[0][1] == sim[1][0] and ver[0][1][3] == ver[1][0][2] ) {
        tet[1]->joinRaw( orient[2], sim[1][0], ver[1][0] * orient );
        tet[1]->joinRaw( orient[3], tet[0],
                trans * orient * ver[1][0].inverse() * gluing *
                ver[0][0] * orient * trans );
    } else if ( sim[0][1] == sim[1][1] and ver[0][1][3] == ver[1][1][3] ) {
        tet[0]->joinRaw( orient[3], sim[1][1], ver[1][1] * orient );
        tet[1]->joinRaw( orient[3], tet[1],
                trans * orient * ver[1][1].inverse() * gluing *
                ver[0][0] * orient * trans );
    } else {
        tet[1]->joinRaw( orient[3], sim[0][1], ver[0][1] * orient );
        tet[1]->joinRaw( orient[2], sim[1][0], ver[1][0] * orient );
        tet[0]->joinRaw( orient[3], sim[1][1], ver[1][1] * orient );
    }
    if ( bdy[0] or bdy[1] ) {
        removeSimplexRaw( temp[0] );
        if ( distinct ) {
            removeSimplexRaw( temp[1] );
        }
    }

    // Done!
    return true;
}

bool Triangulation<3>::zeroTwoMove(
        Edge<3>* e, size_t t0, size_t t1,
        bool check, bool perform ) {
    size_t deg = e->degree();
    if ( check ) {
        if ( e->isBoundary() and ( t0 > deg or t1 > deg ) ) {
            return false;
        } else if ( t0 >= deg or t1 >= deg ) {
            return false;
        }
    }
    size_t t[2] = {t0, t1};
    EdgeEmbedding<3> emb[2];
    int tri[2];
    for ( int i : {0, 1} ) {
        if ( t[i] == deg ) {
            emb[i] = e->embedding( deg - 1 );
            tri[i] = 2;
        } else {
            emb[i] = e->embedding( t[i] );
            tri[i] = 3;
        }
    }
    return zeroTwoMove( emb[0], tri[0], emb[1], tri[1], check, perform );
}

bool Triangulation<3>::zeroTwoMove(
        Triangle<3>* t0, int e0, Triangle<3>* t1, int e1,
        bool check, bool perform ) {
    Triangle<3>* t[2] = {t0, t1};
    int e[2] = {e0, e1};
    EdgeEmbedding<3> emb[2];
    int tri[2];
    for (int i = 0; i < 2; ++i) {
        TriangleEmbedding<3> te = t[i]->embedding(0);
        Perm<4> ve = te.vertices();
        emb[i] = EdgeEmbedding<3>(
                te.simplex(),
                te.simplex()->faceMapping<1>(
                    FaceNumbering<3,1>::faceNumber(
                        ve * Perm<4>( 2, e[i] ) ) ) );
        tri[i] = ( (emb[i].vertices()[2] == ve[3]) ? 2 : 3 );
    }
    return zeroTwoMove( emb[0], tri[0], emb[1], tri[1], check, perform );
}

bool Triangulation<3>::openBook(Triangle<3>* f, bool check, bool perform) {
    if (f->isLocked()) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform an "
                "open book move using a locked triangle");
    }

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
        if (tet->vertex(vertices[fVertex])->linkType() != Vertex<3>::DISC)
            return false;
        if (! f->edge(fVertex)->isValid())
            return false;
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a change event group: this is very simple, and
    // we will already get a ChangeEventSpan via unjoin().
    TopologyLock lock(*this);
    tet->unjoin(emb.triangle());
    return true;
}

bool Triangulation<3>::closeBook(Edge<3>* e, bool check, bool perform) {
    if (check) {
        if (! e->isBoundary())
            return false;
        if (e->boundaryComponent()->countTriangles() <= 2)
            return false;
    }

    // Find the two triangles on either side of edge e.
    const EdgeEmbedding<3>& front = e->front();
    const EdgeEmbedding<3>& back = e->back();

    Tetrahedron<3>* t0 = front.tetrahedron();
    Tetrahedron<3>* t1 = back.tetrahedron();
    Perm<4> p0 = front.vertices();
    Perm<4> p1 = back.vertices();

    if (t0->isFacetLocked(p0[3]) || t1->isFacetLocked(p1[2])) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform a "
                "close book move using a locked boundary triangle");
    }

    if (check) {
        if (t0->vertex(p0[2]) == t1->vertex(p1[3]))
            return false;
        if (t0->vertex(p0[2])->linkType() != Vertex<3>::DISC ||
               t1->vertex(p1[3])->linkType() != Vertex<3>::DISC)
            return false;
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a change event group: this is very simple, and
    // we will already get a ChangeEventSpan via join().
    TopologyLock lock(*this);
    t0->join(p0[3], t1, p1 * Perm<4>(2, 3) * p0.inverse());
    return true;
}

bool Triangulation<3>::shellBoundary(Tetrahedron<3>* t,
        bool check, bool perform) {
    if (t->isLocked()) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform a "
                "boundary shelling move on a locked tetrahedron");
    }
    for (int i = 0; i < 4; ++i)
        if ((! t->adjacentSimplex(i)) && t->isFacetLocked(i)) {
            if (check)
                return false;
            if (perform)
                throw LockViolation("An attempt was made to perform a "
                    "boundary shelling move that would remove a "
                    "locked boundary triangle");
        }

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

    // Actually perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use the "raw" routines removeSimplexRaw() below.  This is
    // because the facets on the internal side of the shelling _are_ allowed
    // to be locked, and we do not want to throw an exception because of this.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    removeSimplexRaw(t);
    return true;
}

bool Triangulation<3>::collapseEdge(Edge<3>* e, bool check, bool perform) {
    // Find the tetrahedra to remove.
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
            if (e->vertex(0)->linkType() != Vertex<3>::DISC)
                return false;
            if (e->vertex(1)->linkType() != Vertex<3>::DISC)
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
            auto* parent = new ssize_t[nEdges + 1];
            std::fill(parent, parent + nEdges + 1, -1);

            // The depth of each subtree in the union-find tree.
            auto* depth = new size_t[nEdges + 1];
            std::fill(depth, depth + nEdges + 1, 0);

            // Run through all triangles containing e.
            auto it = e->begin();

            for ( ; it != e->end(); ++it) {
                Tetrahedron<3>* tet = it->tetrahedron();
                Perm<4> p = it->vertices();

                Edge<3>* upper = tet->edge(Edge<3>::edgeNumber[p[0]][p[2]]);
                Edge<3>* lower = tet->edge(Edge<3>::edgeNumber[p[1]][p[2]]);

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

                size_t id1 = ((upper->isBoundary() || ! upper->isValid()) ?
                    nEdges : upper->markedIndex());
                size_t id2 = ((lower->isBoundary() || ! lower->isValid()) ?
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
            auto* parent = new ssize_t[nTriangles + 1];
            std::fill(parent, parent + nTriangles + 1, -1);

            // The depth of each subtree in the union-find tree.
            auto* depth = new size_t[nTriangles + 1];
            std::fill(depth, depth + nTriangles + 1, 0);

            for (auto& emb : *e) {
                Triangle<3>* upper = emb.simplex()->triangle(emb.vertices()[0]);
                Triangle<3>* lower = emb.simplex()->triangle(emb.vertices()[1]);

                size_t id1 = (upper->isBoundary() ? nTriangles :
                    upper->markedIndex());
                size_t id2 = (lower->isBoundary() ? nTriangles :
                    lower->markedIndex());

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

    // Finally, we search for potential lock violations, and also record any
    // locks on the exterior of the region that we need to preserve and merge.
    auto* lockExterior = new bool[e->degree()];
    std::fill(lockExterior, lockExterior + e->degree(), false);

    size_t idx = 0;
    for (auto& emb : *e) {
        if (emb.simplex()->locks_) {
            if (emb.simplex()->isLocked()) {
                delete[] lockExterior;
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform an "
                        "edge collapse that would remove a locked tetrahedron");
            }
            if (emb.simplex()->isFacetLocked(emb.vertices()[0]) ||
                    emb.simplex()->isFacetLocked(emb.vertices()[1])) {
                // These are the two exterior facets, which will be merged.
                lockExterior[idx] = true;
            }
            for (int i = 2; i <= 3; ++i)
                if (emb.simplex()->isFacetLocked(emb.vertices()[i])) {
                    delete[] lockExterior;
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform an "
                            "edge collapse that would remove a locked "
                            "triangle");
                }
        }
        ++idx;
    }

    if (! perform) {
        delete[] lockExterior;
        return true;
    }

    // Perform the move.
    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (removeSimplexRaw, joinRaw, etc.) below.
    TopologyLock lock(*this);
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    // Clone the edge embeddings because we cannot rely on skeletal
    // objects once we start changing the triangulation.
    auto* embs = new EdgeEmbedding<3>[e->degree()];
    std::copy(e->begin(), e->end(), embs);

    for (size_t i = 0; i < e->degree(); ++i) {
        const auto& emb = embs[i];

        Simplex<3>* top = emb.simplex()->adjacentTetrahedron(emb.vertices()[0]);
        Perm<4> topPerm = emb.simplex()->adjacentGluing(emb.vertices()[0]);
        Simplex<3>* bot = emb.simplex()->adjacentTetrahedron(emb.vertices()[1]);
        Perm<4> botPerm = emb.simplex()->adjacentGluing(emb.vertices()[1]);

        emb.simplex()->isolateRaw();
        if (top && bot)
            top->joinRaw(topPerm[emb.vertices()[0]], bot,
                botPerm * Perm<4>(emb.vertices()[0], emb.vertices()[1]) *
                topPerm.inverse());

        if (lockExterior[i]) {
            if (top)
                top->lockFacetRaw(topPerm[emb.vertices()[0]]);
            if (bot)
                bot->lockFacetRaw(botPerm[emb.vertices()[1]]);
        }

        removeSimplexRaw(emb.simplex());
    }

    delete[] embs;
    delete[] lockExterior;
    return true;
}

void Triangulation<3>::pinchEdge(Edge<3>* e) {
    if (e->isBoundary())
        throw InvalidArgument("pinchEdge() requires an internal edge");

    // Find a triangular face containing e (this will be the face that
    // connects e->front() with e->back()).
    // Our plan is to insert two tetrahedra in its place.
    Tetrahedron<3>* open = e->front().tetrahedron();
    Perm<4> vertices = e->front().vertices();
    bool locked = open->isFacetLocked(vertices[3]);

    // The following takeSnapshot() and ChangeAndClearSpan are essential,
    // since we use "raw" routines (newSimplicesRaw, joinRaw, etc.) below.
    takeSnapshot();
    ChangeAndClearSpan span(*this);

    // The two tetrahedra that we insert together form a pinched ball.
    // By a "pinched ball", this means a 3-ball in which some internal curve
    // joining two distinct boundary points is collapsed to a point, whose
    // link then becomes an annulus.
    //
    // Combinatorially, the boundary of this pinched ball is isomorphic to the
    // boundary of a triangular pillow: two of the vertices of the pillow
    // correspond to opposite sides of the pinch point, and the third vertex
    // of the pillow is some other vertex on the boundary of the pinched ball.
    //
    // We insert this pillow into the opened-up triangular face, so that
    // the two endpoints of edge e get glued into the opposite sides of the
    // pinch point.  The result, topologically, is that we have (1) created
    // a new internal curve c which is parallel to e and whose endpoints are
    // the same as e's, and then (2) collapsed this curve c to a point.
    // Since e is an internal edge (a precondition of this routine),
    // this is topologically the same as collapsing e itself.

    auto [t0, t1] = newSimplicesRaw<2>();
    t0->joinRaw(0, t1, {1, 2});
    t0->joinRaw(3, t1, {0, 1});
    t1->joinRaw(1, t1, {1, 2});

    // The boundary triangles of this auxiliary structure are t0: 013 / 023.
    // Whatever vertex is glued to t0: 3 will be (topologically) unaffected.
    // Whatever vertices glue to t0: 0 and t0: 1=2 will have their links
    // joined by a connected sum.

    // A note for oriented triangulations: Simplex::faceMapping() guarantees
    // that e->front().vertices() has a sign equal to the orientation of the
    // relevant tetrahedron, which for an oriented triangulation is always 1.
    // Therefore all of the gluings that we make here use odd gluing
    // permutations, and so the orientation is preserved.

    Tetrahedron<3>* adj = open->adjacentTetrahedron(vertices[3]);
    Perm<4> glue = open->adjacentGluing(vertices[3]);
    open->unjoinRaw(vertices[3]);
    t0->joinRaw(1, adj, glue * vertices * Perm<4>(0, 3, 1, 2));
    t0->joinRaw(2, open, vertices * Perm<4>(2, 3));

    // If the triangle that we popped open was locked, we will (arbitrarily)
    // choose to move the lock to the triangle that still belongs to open
    // (as opposed to the ex-partner triangle belonging to adj).
    if (locked) {
        // The lock is already present from open's side.
        // Remove it from adj's side, and put it where it needs to be in t0.
        adj->unlockFacetRaw(glue[vertices[3]]);
        t0->lockFacetRaw(2);
    }
}

} // namespace regina
