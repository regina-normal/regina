
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

#include <set>
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

namespace regina {

namespace {
    // A helper routine that uses union-find to test whether a graph
    // contains cycles.  This is used by Triangulation<4>::collapseEdge().
    //
    // This routine returns true if the given edge connects two distinct
    // components of the graph, or false if both endpoints of the edge
    // are already in the same component (i.e., a cycle has been created).
    //
    // This routine is a direct clone of the corresponding routine for
    // Triangulation<3>::collapseEdge().  It would be nice to reuse this
    // routine instead of duplicate it, but that is for another day.
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

bool Triangulation<4>::fourFourMove( Edge<4>* e, bool check, bool perform ) {
    const Triangulation<2>& edgeLink = e->buildLink();
    Isomorphism<4> linkInc = e->buildLinkInclusion();

    if (check) {
        // e should meet four distinct pentachora.
        if (e->degree() != 4) {
            return false;
        }
        std::set< Pentachoron<4>* > pentSet;
        for ( auto& emb : *e ) {
            if ( not pentSet.insert( emb.pentachoron() ).second ) {
                return false;
            }
        }

        // The link of e should be a 2-2 move away from being combinatorially
        // isomorphic to the boundary of a tetrahedron.
        if ( edgeLink.isoSig() != "eLPbddaaa" ) {
            return false;
        }
    }

    for (auto& emb : *e) {
        if (emb.simplex()->locks_) {
            if (emb.simplex()->isLocked() ||
                    emb.simplex()->isFacetLocked(emb.vertices()[2]) ||
                    emb.simplex()->isFacetLocked(emb.vertices()[3]) ||
                    emb.simplex()->isFacetLocked(emb.vertices()[4])) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "4-4 move using a locked pentachoron and/or facet");
            }
        }
    }

    if (! perform)
        return true;

    // Perform the 4-4 move as a 2-4 move followed by a 4-2 move.
    // Note that we use pachner(), which ensures that we preserve orientation
    // (if the triangulation was originally oriented).
    //
    // The calls to pachner() will also manage any lock updates.

    // Start by working out where the 2-4 and 4-2 moves should take place.
    Vertex<2>* topVert = nullptr;
    for ( int i = 0; i < 3; ++i ) {
        if ( edgeLink.triangle(0)->vertex( i )->degree() == 2 ) {
            topVert = edgeLink.triangle(0)->vertex( i );
            break;
        }
    }
    // Location of the (first) 2-4 move.
    size_t linkFront = topVert->embedding(0).triangle()->index();
    int vertFront = topVert->embedding(0).vertex();
    Pentachoron<4>* frontPent = pentachoron( linkInc.pentImage(linkFront) );
    Tetrahedron<4>* tet24 = frontPent->tetrahedron(
            linkInc.facetPerm( linkFront )[ vertFront ] );

    // Location of the (second) 4-2 move.
    // We record this as a pentachoron-edge combination, since by the time we
    // perform this second move the original skeleton will have been destroyed.
    size_t linkBack = topVert->embedding(1).triangle()->index();
    Pentachoron<4>* backPent = pentachoron( linkInc.pentImage(linkBack) );
    int edge42;
    for ( auto& emb : *e ) {
        if ( emb.simplex() == backPent ) {
            edge42 = emb.edge();
            break;
        }
    }

    PacketChangeGroup span(*this);

    pachner(tet24, regina::unprotected);
    pachner(backPent->edge(edge42), regina::unprotected);

    // Done!
    return true;
}

bool Triangulation<4>::openBook(Tetrahedron<4>* t, bool check, bool perform) {
    if (t->isLocked()) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform an "
                "open book move using a locked tetrahedron");
    }

    const TetrahedronEmbedding<4>& emb = t->front();
    Pentachoron<4>* pent = emb.pentachoron();

    // Check that the triangle has exactly two boundary edges.
    // Note that this will imply that the triangle joins two tetrahedra.
    if (check) {
        int i;
        for (i = 0; i < 4; ++i)
            if (! t->vertex(i)->isValid())
                return false;
        for (i = 0; i < 6; ++i)
            if (! t->edge(i)->isValid())
                return false;
        for (i = 0; i < 4; ++i)
            if (! t->triangle(i)->isValid())
                return false;

        int nBdry = 0;
        int bdryTriangle[4];

        for (i = 0; i < 4; ++i)
            if (t->triangle(i)->isBoundary())
                bdryTriangle[nBdry++] = i;

        if (nBdry < 1 || nBdry > 3)
            return false;

        if (nBdry == 2) {
            // Remaining edge is non-boundary.
            int edge = Edge<3>::edgeNumber[bdryTriangle[0]][bdryTriangle[1]];
            if (t->edge(edge)->isBoundary())
                return false;

            // Remaining two triangles are not identified.
            if (t->triangle(Edge<3>::edgeVertex[5 - edge][0]) ==
                    t->triangle(Edge<3>::edgeVertex[5 - edge][1]))
                return false;
        } else if (nBdry == 1) {
            // Remaining vertex is non-boundary.
            if (t->vertex(bdryTriangle[0])->isBoundary())
                return false;

            // No two of the remaining three edges are identified.
            Edge<4>* internal[3];
            internal[0] = t->edge(
                Edge<3>::edgeNumber[bdryTriangle[0]][(bdryTriangle[0] + 1) % 4]);
            internal[1] = t->edge(
                Edge<3>::edgeNumber[bdryTriangle[0]][(bdryTriangle[0] + 2) % 4]);
            internal[2] = t->edge(
                Edge<3>::edgeNumber[bdryTriangle[0]][(bdryTriangle[0] + 3) % 4]);

            if (internal[0] == internal[1] || internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        }
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // Don't bother with a change event group: this is very simple, and
    // we will already get our change management bookkeeping via unjoin().
    // We should however declare a topology lock here, since unjoin() does not
    // know that the topology will be preserved.
    TopologyLock lock(*this);
    pent->unjoin(emb.tetrahedron());

    return true;
}

bool Triangulation<4>::shellBoundary(Pentachoron<4>* p,
        bool check, bool perform) {
    if (p->isLocked()) {
        if (check)
            return false;
        if (perform)
            throw LockViolation("An attempt was made to perform a "
                "boundary shelling move on a locked pentachoron");
    }
    for (int i = 0; i < 5; ++i)
        if ((! p->adjacentSimplex(i)) && p->isFacetLocked(i)) {
            if (check)
                return false;
            if (perform)
                throw LockViolation("An attempt was made to perform a "
                    "boundary shelling move that would remove a "
                    "locked boundary tetrahedron");
        }

    // To perform the move we don't even need a skeleton.
    if (check) {
        ensureSkeleton();

        // All edges and triangles must be valid.
        int i;
        for (i = 0; i < 10; ++i)
            if (! p->edge(i)->isValid())
                return false;
        for (i = 0; i < 10; ++i)
            if (! p->triangle(i)->isValid())
                return false;

        // Precisely 1, 2, 3 or 4 boundary facets.
        int nBdry = 0;
        int bdry[5];
        for (i = 0; i < 5; ++i)
            if (p->tetrahedron(i)->isBoundary())
                bdry[nBdry++] = i;
        if (nBdry < 1 || nBdry > 4)
            return false;

        if (nBdry == 1) {
            // Opposite vertex not in boundary.
            if (p->vertex(bdry[0])->isBoundary())
                return false;

            // No two of the remaining four edges identified.
            Edge<4>* internal[4];
            int j = 0;
            for (i = 0; i < 5; ++i)
                if (i != bdry[0])
                    internal[j++] = p->edge(Edge<4>::edgeNumber[bdry[0]][i]);

            for (i = 0; i < 4; ++i)
                for (j = i + 1; j < 4; ++j)
                    if (internal[i] == internal[j])
                        return false;
        } else if (nBdry == 2) {
            // Opposite edge not in boundary.
            i = Edge<4>::edgeNumber[bdry[0]][bdry[1]];
            if (p->edge(i)->isBoundary())
                return false;

            // No two of the remaining three triangles identified.
            Triangle<4>* internal[3];
            int j = 0;
            for (i = 0; i < 5; ++i)
                if (i != bdry[0] && i != bdry[1])
                    internal[j++] = p->triangle(
                        Triangle<4>::triangleNumber[bdry[0]][bdry[1]][i]);

            if (internal[0] == internal[1] ||
                    internal[1] == internal[2] ||
                    internal[2] == internal[0])
                return false;
        } else if (nBdry == 3) {
            // Opposite triangle not in boundary.
            i = Triangle<4>::triangleNumber[bdry[0]][bdry[1]][bdry[2]];
            if (p->triangle(i)->isBoundary())
                return false;

            // Remaining two facets not identified.
            if (p->adjacentPentachoron(Edge<4>::edgeVertex[i][0]) == p)
                return false;
        }
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // The following ChangeAndClearSpan is essential, since we use the
    // "raw" routine removeSimplexRaw() below.  This is because
    // the facets on the internal side of the shelling _are_ allowed
    // to be locked, and we do not want to throw an exception because of this.
    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    removeSimplexRaw(p);
    return true;
}

bool Triangulation<4>::collapseEdge(Edge<4>* e, bool check, bool perform) {
    // Find the pentachora to remove.
    if (check) {
        // We need a valid edge before we test anything else.
        // From this we know that the edge link is a disc or sphere.
        if (! e->isValid())
            return false;

        // CHECK 0: The pentachora around the edge must be distinct.
        // We check this as follows:
        //
        // - None of the triangles containing edge e must contain e twice.
        //   We throw this into check 2 below (see points [0a] and [0b]).
        //
        // - The only remaining bad cases involve some tetrahedron with
        //   e as two opposite edges.  In this case one can prove that
        //   we have a bad chain of bigons, which will be picked up in
        //   check 2 below.  This works both for internal tetrahedra
        //   (which give a bad chain of internal bigons) and boundary
        //   tetrahedra (which give a bad chain of boundary bigons).

        // CHECK 1: Can we collapse the edge to a point (which in turn
        // collapses triangles to bigons and so on up the dimensions)?

        // The vertices must be distinct.
        if (e->vertex(0) == e->vertex(1))
            return false;

        // If both vertices are in the boundary then we must be collapsing a
        // boundary edge, and both vertices must have plain old ball links.
        // Recall that ideal vertices return isBoundary() == true.
        if (e->vertex(0)->isBoundary() && e->vertex(1)->isBoundary()) {
            if (! e->isBoundary())
                return false;

            // Since e is a boundary edge, both vertex links are bounded
            // 3-manifolds.  This means that the vertex links are balls
            // if and only if the vertices are valid.
            if (! e->vertex(0)->isValid())
                return false;
            if (! e->vertex(1)->isValid())
                return false;
        }

        // CHECK 2: Triangles containing the edge have now become bigons.
        // Can we flatten each bigon to an edge (leaving behind
        // triangular pillows behind and so on up the dimensions)?
        //
        // This is trickier.  Even if every individual bigon is okay, we
        // don't want a _chain_ of bigons together to crush a sphere or
        // projective plane.
        //
        // The way we do this is as follows.  Consider each Edge<4>* to be
        // a vertex of some graph G, and consider each bigon to be an edge
        // in this graph G.  The vertices at either end of the edge in G
        // are the (Edge<4>*)s that bound the bigon.
        //
        // We can happily flatten each bigon if and only if the graph G
        // contains no cycles.  We shall test this using union-find,
        // which should have log-linear complexity.
        //
        // We deal with boundary edges and invalid edges as follows.
        // All boundary and/or invalid edges become the *same* vertex in
        // the graph G.  This means, for instance, that a bigon joining two
        // distinct boundary edges is not allowed.
        //
        // If edge e is itself a boundary edge, things become more
        // interesting again.  In this case, the *boundary* bigons
        // are not subject to the same restrictions -- crushing bigons
        // along the boundary does no harm, *unless* the boundary bigon
        // edges themselves form a cycle.  This is essentially the same
        // dilemma as before but one dimension down, and we must test this
        // separately.
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

            // The depth of each subtree in the union-find tree.
            auto* depth = new size_t[nEdges + 1];

            Edge<4> *upper, *lower;

            if (e->isBoundary()) {
                // Search for cycles in boundary bigons.

                std::fill(parent, parent + nEdges + 1, -1);
                std::fill(depth, depth + nEdges + 1, 0);

                // Run through all boundary triangles containing e.
                for (Triangle<4>* triangle : triangles()) {
                    if (! triangle->isBoundary())
                        continue;

                    int i;
                    for (i = 0; i < 3; ++i)
                        if (triangle->edge(i) == e)
                            break;
                    if (i == 3)
                        continue;

                    // This triangle contains edge e (specifically, as edge i
                    // of this triangle).

                    upper = triangle->edge((i + 1) % 3);
                    lower = triangle->edge((i + 2) % 3);

                    if (upper == e || lower == e) {
                        // [0a]: Check 0 fails; this triangle contains edge e
                        // more than once.
                        delete[] depth;
                        delete[] parent;
                        return false;
                    }

                    // This bigon joins nodes id1 and id2 in the graph G.
                    if (! unionFindInsert(parent, depth,
                            upper->index(), lower->index())) {
                        delete[] depth;
                        delete[] parent;
                        return false;
                    }
                }

                // All looks good on the boundary.
            }

            // Search for cycles in internal bigons.

            std::fill(parent, parent + nEdges + 1, -1);
            std::fill(depth, depth + nEdges + 1, 0);

            // Run through all internal triangles containing e.
            for (Triangle<4>* triangle : triangles()) {
                if (triangle->isBoundary())
                    continue;

                int i;
                for (i = 0; i < 3; ++i)
                    if (triangle->edge(i) == e)
                        break;
                if (i == 3)
                    continue;

                // This triangle contains edge e (specifically, as edge i
                // of this triangle).

                upper = triangle->edge((i + 1) % 3);
                lower = triangle->edge((i + 2) % 3);

                if (upper == e || lower == e) {
                    // [0b]: Check 0 fails; this triangle contains edge e
                    // more than once.
                    delete[] depth;
                    delete[] parent;
                    return false;
                }

                size_t id1 = ((upper->isBoundary() || ! upper->isValid()) ?
                    nEdges : upper->index());
                size_t id2 = ((lower->isBoundary() || ! lower->isValid()) ?
                    nEdges : lower->index());

                // This bigon joins nodes id1 and id2 in the graph G.
                if (! unionFindInsert(parent, depth, id1, id2)) {
                    delete[] depth;
                    delete[] parent;
                    return false;
                }
            }

            // All looks good internally also.

            // No bad chains of bigons!
            delete[] depth;
            delete[] parent;
        }

        // CHECK 3: Tetrahedra containing the edge have now become
        // triangular pillows.  Can we flatten each pillow to a triangle
        // (leaving behind "tetrahedral 4-pillows" in higher dimensions)?
        //
        // We deal with this the same way we deal with flattening bigons
        // to edges.  Again, we must treat internal pillows and
        // boundary pillows separately.
        {
            size_t nTriangles = countTriangles();

            // The parent of each triangle in the union-find tree, or -1 if
            // a triangle is at the root of a tree.
            //
            // This array is indexed by triangle number in the triangulation.
            // Although we might not use many of these triangles, it's fast
            // and simple.  The "unified boundary" is assigned the triangle
            // number nTriangles.
            auto* parent = new ssize_t[nTriangles + 1];

            // The depth of each subtree in the union-find tree.
            auto* depth = new size_t[nTriangles + 1];

            Triangle<4> *upper, *lower;

            if (e->isBoundary()) {
                // Search for cycles in boundary pillows.

                std::fill(parent, parent + nTriangles + 1, -1);
                std::fill(depth, depth + nTriangles + 1, 0);

                // Run through all boundary tetrahedra containing e.
                for (Tetrahedron<4>* tet : tetrahedra()) {
                    if (! tet->isBoundary())
                        continue;

                    int i;
                    for (i = 0; i < 6; ++i)
                        if (tet->edge(i) == e)
                            break;
                    if (i == 6)
                        continue;

                    // This tetrahedron contains edge e (specifically, as
                    // edge i of this tetrahedron).

                    upper = tet->triangle(Edge<3>::edgeVertex[i][0]);
                    lower = tet->triangle(Edge<3>::edgeVertex[i][1]);

                    if (! unionFindInsert(parent, depth,
                            upper->index(), lower->index())) {
                        delete[] depth;
                        delete[] parent;
                        return false;
                    }
                }

                // All looks good on the boundary.
            }

            // Search for cycles in internal pillows.

            std::fill(parent, parent + nTriangles + 1, -1);
            std::fill(depth, depth + nTriangles + 1, 0);

            // Run through all internal tetrahedra containing e.
            for (Tetrahedron<4>* tet : tetrahedra()) {
                if (tet->isBoundary())
                    continue;

                int i;
                for (i = 0; i < 6; ++i)
                    if (tet->edge(i) == e)
                        break;
                if (i == 6)
                    continue;

                // This tetrahedron contains edge e (specifically, as edge i
                // of this tetrahedron).

                upper = tet->triangle(Edge<3>::edgeVertex[i][0]);
                lower = tet->triangle(Edge<3>::edgeVertex[i][1]);

                size_t id1 = ((upper->isBoundary() || ! upper->isValid()) ?
                    nTriangles : upper->index());
                size_t id2 = ((lower->isBoundary() || ! lower->isValid()) ?
                    nTriangles : lower->index());

                // This pillow joins nodes id1 and id2 in the graph G.
                if (! unionFindInsert(parent, depth, id1, id2)) {
                    delete[] depth;
                    delete[] parent;
                    return false;
                }
            }

            // All looks good internally also.

            // No bad chains of bigons!
            delete[] depth;
            delete[] parent;
        }

        // CHECK 4: Can we flatten each tetrahedral 4-pillow to a tetrahedron?
        //
        // Again, even if each individual pillow is okay, we don't want
        // a chain of pillows together to completely crush away a
        // 4-manifold component.
        //
        // This means no cycles of pillows, and no chains of pillows
        // that run from boundary to boundary.
        //
        // Test this in the same way that we tested edges.  It's kind of
        // overkill, since each vertex in the corresponding graph G will
        // have degree <= 2, but it's fast so we'll do it.
        {
            size_t nTets = countTetrahedra();

            // The parent of each tetrahedron in the union-find tree,
            // or -1 if a tetrahedron is at the root of a tree.
            //
            // This array is indexed by tetrahedron number in the triangulation.
            // The "unified boundary" is assigned the triangle number nTets.
            auto* parent = new ssize_t[nTets + 1];
            std::fill(parent, parent + nTets + 1, -1);

            // The depth of each subtree in the union-find tree.
            auto* depth = new size_t[nTets + 1];
            std::fill(depth, depth + nTets + 1, 0);

            Tetrahedron<4> *upper, *lower;

            for (auto& emb : *e) {
                upper = emb.simplex()->tetrahedron(emb.vertices()[0]);
                lower = emb.simplex()->tetrahedron(emb.vertices()[1]);

                size_t id1 = (upper->isBoundary() ? nTets : upper->index());
                size_t id2 = (lower->isBoundary() ? nTets : lower->index());

                // This 4-pillow joins nodes id1 and id2 in the graph G.
                if (! unionFindInsert(parent, depth, id1, id2)) {
                    delete[] depth;
                    delete[] parent;
                    return false;
                }
            }

            // No bad chains of 4-pillows!
            delete[] depth;
            delete[] parent;
        }
    }

    // Finally, we search for potential lock violations:
    size_t idx = 0;
    for (auto& emb : *e) {
        if (emb.simplex()->locks_) {
            if (emb.simplex()->isLocked()) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform an "
                        "edge collapse that would remove a locked pentachoron");
            }
            for (int i = 2; i <= 4; ++i)
                if (emb.simplex()->isFacetLocked(emb.vertices()[i])) {
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform an "
                            "edge collapse that would remove a locked "
                            "tetrahedron");
                }
        }
        ++idx;
    }

    if (! perform)
        return true;

    // Perform the move.
    // The following ChangeAndClearSpan is essential, since we use
    // "raw" routines (removeSimplexRaw, joinRaw, etc.) below.
    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // Clone the edge embeddings because we cannot rely on skeletal
    // objects once we start changing the triangulation.
    auto* embs = new EdgeEmbedding<4>[e->degree()];
    std::copy(e->begin(), e->end(), embs);

    for (size_t i = 0; i < e->degree(); ++i) {
        const auto& emb = embs[i];

        Simplex<4>* top = emb.simplex()->adjacentPentachoron(emb.vertices()[0]);
        Perm<5> topPerm = emb.simplex()->adjacentGluing(emb.vertices()[0]);
        Simplex<4>* bot = emb.simplex()->adjacentPentachoron(emb.vertices()[1]);
        Perm<5> botPerm = emb.simplex()->adjacentGluing(emb.vertices()[1]);

        if (emb.simplex()->locks_) {
            if (bot && emb.simplex()->isFacetLocked(emb.vertices()[0]))
                bot->lockFacetRaw(botPerm[emb.vertices()[1]]);
            if (top && emb.simplex()->isFacetLocked(emb.vertices()[1]))
                top->lockFacetRaw(topPerm[emb.vertices()[0]]);
        }

        removeSimplexRaw(emb.simplex());

        if (top && bot)
            top->joinRaw(topPerm[emb.vertices()[0]], bot,
                botPerm * Perm<5>(emb.vertices()[0], emb.vertices()[1]) *
                topPerm.inverse());
    }

    delete[] embs;
    return true;
}

bool Triangulation<4>::snapEdge(Edge<4>* e, bool check, bool perform) {
    if (check &&
            ((e->vertex(0) == e->vertex(1)) ||
            (e->vertex(0)->isBoundary() && e->vertex(1)->isBoundary())))
        return false;
    if (! perform)
        return true;

    // Our plan is to find a tetrahedron containing e, and then insert four
    // pentachora in its place.
    Pentachoron<4>* open = e->front().pentachoron();
    Perm<5> vertices = e->front().vertices();
    Pentachoron<4>* adj = open->adjacentPentachoron(vertices[2]);
    Perm<5> glue = open->adjacentGluing(vertices[2]);
    bool locked = open->isFacetLocked(vertices[2]);

    // Actually perform the move.
    // The following ChangeAndClearSpan is essential, since we use
    // "raw" routines (newSimplicesRaw, joinRaw, etc.) below.
    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // The four pentachora that we insert together form a "pinched 4-ball".
    // Combinatorially, the boundary of this pinched 4-ball is isomorphic to
    // the 3-sphere that forms the boundary of a tetrahedral pillow; however,
    // two adjacent boundary edges a and b in this pinched 4-ball are pinched
    // together to form a single edge whose link becomes an annulus. We insert
    // this pinched 4-ball into the opened-up tetrahedron in such a way that
    // edges a, b and e together bound a triangle in the 3-sphere that used to
    // form the boundary of the pinched 4-ball (this is possible because edges
    // a and b are adjacent). For our purposes, the most important consequence
    // of this is that the endpoints of e will become snapped together.

    auto p = newSimplicesRaw<4>();
    p[0]->joinRaw(0, p[1], {3, 4});
    p[0]->joinRaw(2, p[1], {0, 2, 4, 1, 3});
    p[0]->joinRaw(3, p[2], {3, 4});
    p[0]->joinRaw(4, p[2], {3, 4});
    p[1]->joinRaw(1, p[2], {1, 2});
    p[1]->joinRaw(2, p[3], {3, 4});
    p[1]->joinRaw(3, p[3], {3, 4});
    p[2]->joinRaw(0, p[3], {3, 4});
    p[2]->joinRaw(1, p[3], {3, 4});

    // The boundary tetrahedra of this auxiliary structure are p[0]: 0234 and
    // p[3]: 0214.
    // The edges that glue to p[0]: 02, p[0]: 03, p[0]: 04 or p[0]: 23 will
    // remain (topologically) unaffected.
    // The edges that glue to p[0]: 24 and p[0]: 34 will be snapped together.

    // A note for oriented triangulations: Simplex::faceMapping() guarantees
    // that e->front().vertices() has a sign equal to the orientation of the
    // relevant pentachoron, which for an oriented triangulation is always 1.
    // Therefore all of the gluings that we make here use odd gluing
    // permutations, and hence the orientation is preserved.

    open->unjoinRaw(vertices[2]);
    p[0]->joinRaw(1, open, vertices * Perm<5>(3, 2, 0, 1, 4));
    p[3]->joinRaw(3, adj, glue * vertices * Perm<5>(3, 1, 0, 2, 4));

    // If the tetrahedron that we popped open was locked, we will (arbitrarily)
    // choose to move the lock to the tetrahedron that still belongs to open
    // (as opposed to the ex-partner tetrahedron belonging to adj).
    if (locked) {
        // The lock is already present from open's side.
        // Remove it from adj's side, and put it where it needs to be in p[0].
        adj->unlockFacetRaw(glue[vertices[2]]);
        p[0]->lockFacetRaw(1);
    }

    // Done!
    return true;
}

} // namespace regina
