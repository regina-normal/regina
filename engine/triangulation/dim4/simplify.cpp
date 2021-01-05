
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

bool Triangulation<4>::twoZeroMove(Triangle<4>* t, bool check, bool perform) {
    if (check) {
        if (t->isBoundary() || ! t->isValid())
            return false;
        if (t->degree() != 2)
            return false;
    }

    Pentachoron<4>* pent[2];
    Perm<5> perm[2];

    int i;
    for (i = 0; i < 2; ++i) {
        pent[i] = t->embedding(i).pentachoron();
        perm[i] = t->embedding(i).vertices();
    }

    // Lots of checks required...
    if (check) {
        // The two pentachora must be distinct.
        if (pent[0] == pent[1])
            return false;

        Edge<4>* edge[2];
        Triangle<4>* tri[2][3];
        Tetrahedron<4>* tet[2][3];

        for (i = 0; i < 2; ++i) {
            edge[i] = pent[i]->edge(
                Edge<4>::edgeNumber[perm[i][3]][perm[i][4]]);
            tri[i][0] = pent[i]->triangle(Triangle<4>::triangleNumber
                [perm[i][0]][perm[i][3]][perm[i][4]]);
            tri[i][1] = pent[i]->triangle(Triangle<4>::triangleNumber
                [perm[i][1]][perm[i][3]][perm[i][4]]);
            tri[i][2] = pent[i]->triangle(Triangle<4>::triangleNumber
                [perm[i][2]][perm[i][3]][perm[i][4]]);
            tet[i][0] = pent[i]->tetrahedron(perm[i][0]);
            tet[i][1] = pent[i]->tetrahedron(perm[i][1]);
            tet[i][2] = pent[i]->tetrahedron(perm[i][2]);
        }

        // No bad loops of edges.
        // Closed loops:
        if (edge[0] == edge[1])
            return false;
        // Bounded loops:
        if (edge[0]->isBoundary() && edge[1]->isBoundary())
            return false;

        // No bad loops of triangles.
        // Closed loops of length 1:
        for (i = 0; i < 3; ++i)
            if (tri[0][i] == tri[1][i])
                return false;
        // Closed loops of length 2:
        for (i = 0; i < 3; ++i) {
            if (tri[0][i] == tri[0][(i + 1) % 3] &&
                    tri[1][i] == tri[1][(i + 1) % 3])
                return false;
            if (tri[0][i] == tri[1][(i + 1) % 3] &&
                    tri[1][i] == tri[0][(i + 1) % 3])
                return false;
        }
        // Closed loops of length 3:
        if (tri[0][0] == tri[1][1] && tri[0][1] == tri[1][2] &&
                tri[0][2] == tri[1][0])
            return false;
        if (tri[1][0] == tri[0][1] && tri[1][1] == tri[0][2] &&
                tri[1][2] == tri[0][0])
            return false;
        for (i = 0; i < 6; ++i)
            if (tri[0][Perm<3>::S3[i][0]] == tri[0][Perm<3>::S3[i][1]] &&
                    tri[1][Perm<3>::S3[i][1]] == tri[1][Perm<3>::S3[i][2]] &&
                    tri[0][Perm<3>::S3[i][2]] == tri[1][Perm<3>::S3[i][0]])
                return false;
        // Bounded loops not already covered by the earlier edge-based test:
        for (i = 0; i < 3; ++i) {
            if (tri[0][i]->isBoundary() &&
                    tri[1][i] == tri[1][(i + 1) % 3] &&
                    tri[0][(i + 1) % 3]->isBoundary())
                return false;
            if (tri[1][i]->isBoundary() &&
                    tri[0][i] == tri[0][(i + 1) % 3] &&
                    tri[1][(i + 1) % 3]->isBoundary())
                return false;
        }
        for (i = 0; i < 6; ++i) {
            if (tri[0][Perm<3>::S3[i][0]]->isBoundary() &&
                    tri[1][Perm<3>::S3[i][0]] == tri[1][Perm<3>::S3[i][1]] &&
                    tri[0][Perm<3>::S3[i][1]] == tri[1][Perm<3>::S3[i][2]] &&
                    tri[0][Perm<3>::S3[i][2]]->isBoundary())
                return false;
            if (tri[1][Perm<3>::S3[i][0]]->isBoundary() &&
                    tri[0][Perm<3>::S3[i][0]] == tri[0][Perm<3>::S3[i][1]] &&
                    tri[1][Perm<3>::S3[i][1]] == tri[0][Perm<3>::S3[i][2]] &&
                    tri[1][Perm<3>::S3[i][2]]->isBoundary())
                return false;
        }

        // No bad loops of tetrahedra.
        // Closed loops of length 1:
        for (i = 0; i < 3; ++i)
            if (tet[0][i] == tet[1][i])
                return false;
        // Closed loops of length 2:
        for (i = 0; i < 3; ++i) {
            if (tet[0][i] == tet[0][(i + 1) % 3] &&
                    tet[1][i] == tet[1][(i + 1) % 3])
                return false;
            if (tet[0][i] == tet[1][(i + 1) % 3] &&
                    tet[1][i] == tet[0][(i + 1) % 3])
                return false;
        }
        // Bounded loops of length 2 not already covered by the earlier
        // edge-based test:
        for (i = 0; i < 3; ++i) {
            if (tet[0][i]->isBoundary() &&
                    tet[1][i] == tet[1][(i + 1) % 3] &&
                    tet[0][(i + 1) % 3]->isBoundary())
                return false;
            if (tet[1][i]->isBoundary() &&
                    tet[0][i] == tet[0][(i + 1) % 3] &&
                    tet[1][(i + 1) % 3]->isBoundary())
                return false;
        }
        // Closed and bounded loops of length 3 are all covered by the
        // following check:
        if (pent[0]->component()->size() == 2)
            return false;
    }

    if (! perform)
        return true;

    // Perform the move.
    TopologyLock lock(this);
    ChangeEventSpan span(this);

    // Unglue facets from the doomed pentachora and glue them to each other.
    Perm<5> crossover = pent[0]->adjacentGluing(perm[0][3]);
    Perm<5> gluing;
    Pentachoron<4> *top, *bottom;
    int topFacet;
    for (i = 0; i < 3; ++i) {
        top = pent[0]->adjacentPentachoron(perm[0][i]);
        bottom = pent[1]->adjacentPentachoron(perm[1][i]);

        if (! top) {
            // Bottom facet becomes boundary.
            pent[1]->unjoin(perm[1][i]);
        } else if (! bottom) {
            // Top facet becomes boundary.
            pent[0]->unjoin(perm[0][i]);
        } else {
            // Bottom and top facets join.
            topFacet = pent[0]->adjacentFacet(perm[0][i]);
            gluing = pent[1]->adjacentGluing(perm[1][i]) *
                crossover * top->adjacentGluing(topFacet);
            pent[0]->unjoin(perm[0][i]);
            pent[1]->unjoin(perm[1][i]);
            top->join(topFacet, bottom, gluing);
        }
    }

    // Finally remove and dispose of the pentachora.
    removePentachoron(pent[0]);
    removePentachoron(pent[1]);

    return true;
}

bool Triangulation<4>::twoZeroMove(Edge<4>* e, bool check, bool perform) {
    if (check) {
        // The follow test also implicitly ensures that the edge link is
        // a 2-sphere.  See Edge<4>::isValid() for details.
        if (e->isBoundary() || ! e->isValid())
            return false;
        if (e->degree() != 2)
            return false;
    }

    Pentachoron<4>* pent[2];
    Perm<5> perm[2];

    int i;
    for (i = 0; i < 2; ++i) {
        pent[i] = e->embedding(i).pentachoron();
        perm[i] = e->embedding(i).vertices();
    }

    if (check) {
        if (pent[0] == pent[1])
            return false;

        // No bad loops of triangles.
        Triangle<4>* tri[2];
        for (i = 0; i < 2; ++i)
            tri[i] = pent[i]->triangle(Triangle<4>::triangleNumber
                [perm[i][2]][perm[i][3]][perm[i][4]]);

        if (tri[0] == tri[1])
            return false;
        if (tri[0]->isBoundary() && tri[1]->isBoundary())
            return false;

        // No bad loops of tetrahedra.
        Tetrahedron<4>* tet[2][2];
        for (i = 0; i < 2; ++i) {
            tet[i][0] = pent[i]->tetrahedron(perm[i][0]);
            tet[i][1] = pent[i]->tetrahedron(perm[i][1]);
        }

        if (tet[0][0] == tet[1][0] || tet[0][1] == tet[1][1])
            return false;

        // The cases with two pairs of identified facets and with one
        // pair of identified facets plus one pair of boundary facets
        // are all covered by the following check.
        if (pent[0]->component()->size() == 2)
            return false;

        // Check that the pentachora are joined along all three facets.
        if (pent[0]->adjacentPentachoron(perm[0][2]) != pent[1])
            return false;
        if (pent[0]->adjacentPentachoron(perm[0][3]) != pent[1])
            return false;
        if (pent[0]->adjacentPentachoron(perm[0][4]) != pent[1])
            return false;
    }

    if (! perform)
        return true;

    // Perform the move.
    TopologyLock lock(this);
    ChangeEventSpan span(this);

    // Unglue facets from the doomed pentachora and glue them to each other.
    Perm<5> crossover = pent[0]->adjacentGluing(perm[0][2]);
    Perm<5> gluing;
    Pentachoron<4> *top, *bottom;
    int topFacet;
    for (i = 0; i < 2; ++i) {
        top = pent[0]->adjacentPentachoron(perm[0][i]);
        bottom = pent[1]->adjacentPentachoron(perm[1][i]);

        if (! top) {
            // Bottom facet becomes boundary.
            pent[1]->unjoin(perm[1][i]);
        } else if (! bottom) {
            // Top facet becomes boundary.
            pent[0]->unjoin(perm[0][i]);
        } else {
            // Bottom and top facets join.
            topFacet = pent[0]->adjacentFacet(perm[0][i]);
            gluing = pent[1]->adjacentGluing(perm[1][i]) *
                crossover * top->adjacentGluing(topFacet);
            pent[0]->unjoin(perm[0][i]);
            pent[1]->unjoin(perm[1][i]);
            top->join(topFacet, bottom, gluing);
        }
    }

    // Finally remove and dispose of the pentachora.
    removePentachoron(pent[0]);
    removePentachoron(pent[1]);

    return true;
}

bool Triangulation<4>::twoZeroMove(Vertex<4>* v, bool check, bool perform) {
    if (check) {
        // For a valid vertex, the link must be a 3-ball or a closed 3-manifold.
        // Moreover: *both* ideal and invalid vertices are considered to
        // be on the boundary.
        // Therefore, if a vertex is non-boundary, its link must be a 3-sphere.
        if (v->isBoundary())
            return false;
        if (v->degree() != 2)
            return false;
    }

    // Note: there is only one 2-tetrahedron 3-sphere triangulation
    // where each tetrahedron is glued to the other along all four faces
    // (and this is the vertex link we are looking for).

    Simplex<4>* pent[2];
    int vertex[2];

    int i = 0;
    for (auto& emb : *v) {
        pent[i] = emb.pentachoron();
        vertex[i] = emb.vertex();
        i++;
    }

    if (check) {
        if (pent[0] == pent[1])
            return false;

        Tetrahedron<4>* tetrahedron[2];
        for (i = 0; i < 2; i++)
            tetrahedron[i] = pent[i]->tetrahedron(vertex[i]);
        if (tetrahedron[0] == tetrahedron[1])
            return false;
        if (tetrahedron[0]->isBoundary() && tetrahedron[1]->isBoundary())
            return false;

        // Check that the pentachora are joined along all four tetrahedra.
        for (i = 0; i < 5; ++i) {
            if (i == vertex[0])
                continue;
            if (pent[0]->adjacentSimplex(i) != pent[1])
                return false;
        }
    }

    if (! perform)
        return true;

    // Actually perform the move.
    TopologyLock lock(this);
    ChangeEventSpan span(this);

    // Unglue faces from the doomed pentachora and glue them to each other.
    Pentachoron<4>* top = pent[0]->adjacentPentachoron(vertex[0]);
    Pentachoron<4>* bottom = pent[1]->adjacentPentachoron(vertex[1]);

    if (! top) {
        pent[1]->unjoin(vertex[1]);
    } else if (! bottom) {
        pent[0]->unjoin(vertex[0]);
    } else {
        Perm<5> crossover;
        crossover = pent[0]->adjacentGluing(vertex[0] == 0 ? 1 : 0);
        int topFacet = pent[0]->adjacentFacet(vertex[0]);
        Perm<5> gluing = pent[1]->adjacentGluing(vertex[1]) *
            crossover * top->adjacentGluing(topFacet);
        pent[0]->unjoin(vertex[0]);
        pent[1]->unjoin(vertex[1]);
        top->join(topFacet, bottom, gluing);
    }

    // Finally remove and dispose of the pentachora.
    removeSimplex(pent[0]);
    removeSimplex(pent[1]);

    return true;
}
bool Triangulation<4>::openBook(Tetrahedron<4>* t, bool check, bool perform) {
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
    // Don't bother with a change event block since this is so simple.
    TopologyLock lock(this);
    pent->unjoin(emb.tetrahedron());

    return true;
}

bool Triangulation<4>::shellBoundary(Pentachoron<4>* p,
        bool check, bool perform) {
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
    // Don't bother with a change event block since this is so simple.
    TopologyLock lock(this);
    removePentachoron(p);

    return true;
}

bool Triangulation<4>::collapseEdge(Edge<4>* e, bool check, bool perform) {
    // Find the pentachora to remove.
    Perm<5> p;

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
            long nEdges = countEdges();

            // The parent of each edge in the union-find tree, or -1 if
            // an edge is at the root of a tree.
            //
            // This array is indexed by edge number in the triangulation.
            // Although we might not use many of these edges, it's fast
            // and simple.  The "unified boundary" is assigned the edge
            // number nEdges.
            long* parent = new long[nEdges + 1];

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nEdges + 1];

            Edge<4> *upper, *lower;
            long id1, id2;
            int i;

            if (e->isBoundary()) {
                // Search for cycles in boundary bigons.

                std::fill(parent, parent + nEdges + 1, -1);
                std::fill(depth, depth + nEdges + 1, 0);

                // Run through all boundary triangles containing e.
                for (Triangle<4>* triangle : triangles()) {
                    if (! triangle->isBoundary())
                        continue;

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
                            upper->markedIndex(), lower->markedIndex())) {
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
            long nTriangles = countTriangles();

            // The parent of each triangle in the union-find tree, or -1 if
            // a triangle is at the root of a tree.
            //
            // This array is indexed by triangle number in the triangulation.
            // Although we might not use many of these triangles, it's fast
            // and simple.  The "unified boundary" is assigned the triangle
            // number nTriangles.
            long* parent = new long[nTriangles + 1];

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nTriangles + 1];

            Triangle<4> *upper, *lower;
            long id1, id2;
            int i;

            if (e->isBoundary()) {
                // Search for cycles in boundary pillows.

                std::fill(parent, parent + nTriangles + 1, -1);
                std::fill(depth, depth + nTriangles + 1, 0);

                // Run through all boundary tetrahedra containing e.
                for (Tetrahedron<4>* tet : tetrahedra()) {
                    if (! tet->isBoundary())
                        continue;

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
                            upper->markedIndex(), lower->markedIndex())) {
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

                for (i = 0; i < 6; ++i)
                    if (tet->edge(i) == e)
                        break;
                if (i == 6)
                    continue;

                // This tetrahedron contains edge e (specifically, as edge i
                // of this tetrahedron).

                upper = tet->triangle(Edge<3>::edgeVertex[i][0]);
                lower = tet->triangle(Edge<3>::edgeVertex[i][1]);

                id1 = ((upper->isBoundary() || ! upper->isValid()) ?
                    nTriangles : upper->markedIndex());
                id2 = ((lower->isBoundary() || ! lower->isValid()) ?
                    nTriangles : lower->markedIndex());

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
            long nTets = countTetrahedra();

            // The parent of each tetrahedron in the union-find tree,
            // or -1 if a tetrahedron is at the root of a tree.
            //
            // This array is indexed by tetrahedron number in the triangulation.
            // The "unified boundary" is assigned the triangle number nTets.
            long* parent = new long[nTets + 1];
            std::fill(parent, parent + nTets + 1, -1);

            // The depth of each subtree in the union-find tree.
            long* depth = new long[nTets + 1];
            std::fill(depth, depth + nTets + 1, 0);

            Tetrahedron<4> *upper, *lower;
            long id1, id2;

            for (auto& emb : *e) {
                upper = emb.simplex()->tetrahedron(emb.vertices()[0]);
                lower = emb.simplex()->tetrahedron(emb.vertices()[1]);

                id1 = (upper->isBoundary() ? nTets : upper->index());
                id2 = (lower->isBoundary() ? nTets : lower->index());

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

    if (! perform)
        return true;

    // Perform the move.
    TopologyLock lock(this);
    ChangeEventSpan span(this);
    Perm<5> topPerm, botPerm;
    Pentachoron<4> *top, *bot;

    // Clone the edge embeddings because we cannot rely on skeletal
    // objects once we start changing the triangulation.
    unsigned nEmbs = e->degree();
    Pentachoron<4>** embPent = new Pentachoron<4>*[nEmbs];
    Perm<5>* embVert = new Perm<5>[nEmbs];

    unsigned i = 0;
    for (auto& emb : *e) {
        embPent[i] = emb.simplex();
        embVert[i] = emb.vertices();
        ++i;
    }

    for (i = 0; i < nEmbs; ++i) {
        top = embPent[i]->adjacentPentachoron(embVert[i][0]);
        topPerm = embPent[i]->adjacentGluing(embVert[i][0]);
        bot = embPent[i]->adjacentPentachoron(embVert[i][1]);
        botPerm = embPent[i]->adjacentGluing(embVert[i][1]);

        embPent[i]->isolate();
        if (top && bot)
            top->join(topPerm[embVert[i][0]], bot,
                botPerm * Perm<5>(embVert[i][0], embVert[i][1]) *
                topPerm.inverse());

        removePentachoron(embPent[i]);
    }

    delete[] embPent;
    delete[] embVert;

    return true;
}

} // namespace regina
