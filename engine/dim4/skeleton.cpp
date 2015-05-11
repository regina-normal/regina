
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <algorithm>
#include <queue>
#include <utility>
#include "dim2/dim2triangulation.h"
#include "dim4/dim4triangulation.h"
#include "maths/permconv.h"
#include "triangulation/ntriangulation.h"

namespace regina {

void Dim4Triangulation::calculateSkeleton() const {
    TriangulationBase<4>::calculateSkeleton();

    // Triangulations are valid and non-ideal until proven otherwise.
    valid_ = true;
    ideal_ = false;

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one pentachoron.
    if (simplices_.empty())
        return;

    // Off we go!
    calculateTetrahedra();
        // Sets:
        // - tetrahedra_
        // - Dim4Component::tetrahedra_
        // - Dim4Pentachoron::tet_
        // - Dim4Pentachoron::tetMapping_
        // - all Dim4Tetrahedron members except boundaryComponent_

    calculateVertices();
        // Sets:
        // - vertices_
        // - Dim4Component::vertices_
        // - Dim4Pentachoron::vertex_
        // - all Dim4Vertex members except boundaryComponent_,
        //       link_, valid_ and ideal_

    calculateEdges();
        // Sets:
        // - edges_
        // - Dim4Component::edges_
        // - Dim4Pentachoron::edge_
        // - Dim4Pentachoron::edgeMapping_
        // - valid_ and Dim4Edge::invalid_ in the case of bad edge self-gluings
        // - all other Dim4Edge members except boundaryComponent_

    calculateTriangles();
        // Sets:
        // - triangles_
        // - Dim4Component::triangles_
        // - Dim4Pentachoron::triangle_
        // - Dim4Pentachoron::triangleMapping_
        // - valid_ and Dim4Triangle::valid_ in the case of bad face
        //   self-gluings
        // - all other Dim4Triangle members except boundaryComponent_

    calculateBoundary();
        // Sets:
        // - boundaryComponents_
        // - Dim4Component::boundaryComponents_
        // - Dim4 [ Tetrahedron, Triangle, Edge, Vertex ]::boundaryComponent_
        // - all Dim4BoundaryComponent members

    calculateVertexLinks();
        // Sets:
        // - Dim4Vertex::link_
        // - valid_ and Dim4Vertex::valid_ in the case of bad vertex links
        // - valid_ and Dim4Edge::invalid_ in the case of bad edge links
        // - ideal_, Dim4Vertex::ideal_ and Dim4Component::ideal_

    if (! valid_)
        calculateEdgeLinks();
        // Sets:
        // - Dim4Edge::link_, but only for edges with bad self-identifications

    // Recall that for 4-manifolds we restrict "ideal" to only include
    // valid triangulations.
    if (! valid_)
        ideal_ = false;
}

void Dim4Triangulation::calculateTetrahedra() const {
    PentachoronIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        std::fill((*it)->tet_, (*it)->tet_ + 5,
            static_cast<Dim4Tetrahedron*>(0));

    Dim4Pentachoron *pent, *adjPent;
    Dim4Tetrahedron* tet;
    int facet, adjFacet;

    // We process facets in lexicographical order, according to the
    // truncated permutation labels that are displayed to the user.
    // This means working through facets in the order 4,3,2,1,0.
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        pent = *it;
        for (facet = 4; facet >= 0; --facet) {
            // Have we already checked out this facet from the other side?
            if (pent->tet_[facet])
                continue;

            // A new tetrahedron!
            tet = new Dim4Tetrahedron(pent->component_);
            tetrahedra_.push_back(tet);
            pent->component_->tetrahedra_.push_back(tet);

            pent->tet_[facet] = tet;
            pent->tetMapping_[facet] = Dim4Tetrahedron::ordering[facet];

            adjPent = pent->adjacentPentachoron(facet);
            if (adjPent) {
                // We have an adjacent tetrahedron.
                adjFacet = pent->adjacentFacet(facet);

                adjPent->tet_[adjFacet] = tet;
                adjPent->tetMapping_[adjFacet] = pent->adjacentGluing(facet) *
                    Dim4Tetrahedron::ordering[facet];

                tet->emb_[0] = Dim4TetrahedronEmbedding(pent, facet);
                tet->emb_[1] = Dim4TetrahedronEmbedding(adjPent, adjFacet);
                tet->nEmb_ = 2;
            } else {
                // This is a boundary tetrahedron.
                tet->emb_[0] = Dim4TetrahedronEmbedding(pent, facet);
                tet->nEmb_ = 1;
            }
        }
    }
}

void Dim4Triangulation::calculateVertices() const {
    PentachoronIterator it;
    int loopVtx;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        for (loopVtx = 0; loopVtx < 5; ++loopVtx)
            (*it)->vertex_[loopVtx] = 0;

    Dim4Vertex* label;
    typedef std::pair<Dim4Pentachoron*, int> Spec; /* (pent, vertex) */
    Spec* stack = new Spec[simplices_.size() * 5];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int vertex, adjVertex;
    int facet;
    NPerm5 adjMap;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        loopPent = *it;
        for (loopVtx = 0; loopVtx < 5; ++loopVtx) {
            if (loopPent->vertex_[loopVtx])
                continue;

            label = new Dim4Vertex(loopPent->component_);
            vertices_.push_back(label);
            loopPent->component_->vertices_.push_back(label);

            // Run a depth-first search around this vertex to completely
            // enumerate all identifications.
            loopPent->vertex_[loopVtx] = label;
            loopPent->vertexMapping_[loopVtx] = NPerm5(0, loopVtx);
            label->emb_.push_back(Dim4VertexEmbedding(loopPent, loopVtx));

            stack[0].first = loopPent;
            stack[0].second = loopVtx;
            stackSize = 1;

            while (stackSize > 0) {
                --stackSize;
                pent = stack[stackSize].first;
                vertex = stack[stackSize].second;

                for (facet = 0; facet < 5; ++facet) {
                    if (facet == vertex)
                        continue;
                    adjPent = pent->adjacentPentachoron(facet);
                    if (adjPent) {
                        // When we choose an adjacent gluing map, throw in a
                        // swap to preserve the "orientation" of the tetrahedron
                        // formed by the images of 1, 2, 3 and 4.  Note that
                        // this only becomes meaningful if the vertex link is
                        // an orientable 3-manifold (otherwise there is no
                        // consistent way to orient these tetrahedra at all).
                        adjMap = pent->adjacentGluing(facet) *
                            pent->vertexMapping_[vertex] * NPerm5(1, 2);
                        adjVertex = adjMap[0];

                        if (! adjPent->vertex_[adjVertex]) {
                            adjPent->vertex_[adjVertex] = label;
                            adjPent->vertexMapping_[adjVertex] = adjMap;
                            label->emb_.push_back(Dim4VertexEmbedding(adjPent,
                                adjVertex));

                            stack[stackSize].first = adjPent;
                            stack[stackSize].second = adjVertex;
                            ++stackSize;
                        }
                    }
                }
            }
        }
    }
    delete [] stack;
}

void Dim4Triangulation::calculateEdges() const {
    PentachoronIterator it;
    int loopEdge;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        for (loopEdge = 0; loopEdge < 10; ++loopEdge)
            (*it)->edge_[loopEdge] = 0;

    Dim4Edge* label;
    typedef std::pair<Dim4Pentachoron*, int> Spec; /* (pent, edge) */
    Spec* stack = new Spec[simplices_.size() * 10];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int edge, adjEdge;
    int facet;
    NPerm5 adjMap;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        loopPent = *it;
        for (loopEdge = 0; loopEdge < 10; ++loopEdge) {
            if (loopPent->edge_[loopEdge])
                continue;

            label = new Dim4Edge(loopPent->component_);
            edges_.push_back(label);
            loopPent->component_->edges_.push_back(label);

            // Run a depth-first search around this edge to completely
            // enumerate all identifications.
            loopPent->edge_[loopEdge] = label;
            loopPent->edgeMapping_[loopEdge] = Dim4Edge::ordering[loopEdge];
            label->emb_.push_back(Dim4EdgeEmbedding(loopPent, loopEdge));

            stack[0].first = loopPent;
            stack[0].second = loopEdge;
            stackSize = 1;

            while (stackSize > 0) {
                --stackSize;
                pent = stack[stackSize].first;
                edge = stack[stackSize].second;

                for (facet = 0; facet < 5; ++facet) {
                    // We are only interested in facets that contain this edge.
                    // Recall that the facet number is also the number of the
                    // only vertex *missing* from this facet.
                    if (facet == Dim4Edge::edgeVertex[edge][0] ||
                            facet == Dim4Edge::edgeVertex[edge][1])
                        continue;

                    adjPent = pent->adjacentPentachoron(facet);
                    if (adjPent) {
                        // When we choose an adjacent gluing map, throw in a
                        // swap to preserve the "orientation" of the cycle
                        // formed by the images of 2, 3 and 4.  Note that this
                        // only becomes meaningful if the edge link is an
                        // orientable surface (otherwise there is no
                        // consistent way to orient these cycles at all).
                        adjMap = pent->adjacentGluing(facet) *
                            pent->edgeMapping_[edge] * NPerm5(2, 3);
                        adjEdge = Dim4Edge::edgeNumber[adjMap[0]][adjMap[1]];

                        if (adjPent->edge_[adjEdge]) {
                            // We have a bad self-identification!
                            if (adjPent->edgeMapping_[adjEdge][0] !=
                                    adjMap[0]) {
                                label->invalid_ |=
                                    Dim4Edge::INVALID_IDENTIFICATION;
                                valid_ = false;
                            }
                        } else {
                            adjPent->edge_[adjEdge] = label;
                            adjPent->edgeMapping_[adjEdge] = adjMap;
                            label->emb_.push_back(Dim4EdgeEmbedding(adjPent,
                                adjEdge));

                            stack[stackSize].first = adjPent;
                            stack[stackSize].second = adjEdge;
                            ++stackSize;
                        }
                    }
                }
            }
        }
    }
    delete [] stack;
}

void Dim4Triangulation::calculateTriangles() const {
    PentachoronIterator it;
    int loopTriangle;
    for (it = simplices_.begin(); it != simplices_.end(); ++it)
        for (loopTriangle = 0; loopTriangle < 10; ++loopTriangle)
            (*it)->triangle_[loopTriangle] = 0;

    Dim4Triangle* label;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int tri, adjTri;
    NPerm5 map, adjMap;
    int dir, exitFacet;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        loopPent = *it;
        for (loopTriangle = 9; loopTriangle >= 0; --loopTriangle) {
            if (loopPent->triangle_[loopTriangle])
                continue;

            label = new Dim4Triangle(loopPent->component_);
            triangles_.push_back(label);
            loopPent->component_->triangles_.push_back(label);

            // Since pentachoron triangles are joined together in a loop, the
            // depth-first search is really just a straight line in either
            // direction.  We therefore do away with the usual stack and
            // just keep track of the next triangle to process in the current
            // direction.
            loopPent->triangle_[loopTriangle] = label;
            loopPent->triangleMapping_[loopTriangle] =
                Dim4Triangle::ordering[loopTriangle];
            label->emb_.push_back(Dim4TriangleEmbedding(loopPent,
                loopTriangle));

            for (dir = 0; dir < 2; ++dir) {
                // Start at the start and walk in one particular direction.
                pent = loopPent;
                tri = loopTriangle;
                map = pent->triangleMapping_[tri];

                while (true) {
                    // Move through to the next pentachoron.
                    exitFacet = map[dir == 0 ? 3 : 4];
                    adjPent = pent->adjacentPentachoron(exitFacet);
                    if (! adjPent)
                        break;

                    adjMap = pent->adjacentGluing(exitFacet) * map *
                        NPerm5(3, 4);
                    adjTri = Dim4Triangle::triangleNumber
                        [adjMap[0]][adjMap[1]][adjMap[2]];

                    if (adjPent->triangle_[adjTri]) {
                        // We looped right around.

                        // Check that we're not gluing the triangle to itself
                        // by a non-trivial mapping.
                        //
                        // Since the triangle link must be orientable (it is
                        // just a circle), we know adjMap[3,4] is the same as
                        // for the original mapping.  Therefore, to test whether
                        // adjMap[0,1,2] is consistent we can just compare the
                        // full permutations (which is in fact faster).
                        if (adjPent->triangleMapping_[adjTri] != adjMap) {
                            // You have chosen unwisely, my son.
                            label->valid_ = false;
                            valid_ = false;
                        }
                        break;
                    }

                    // We have not yet seen this pentachoron triangle.
                    // Label it.
                    adjPent->triangle_[adjTri] = label;
                    adjPent->triangleMapping_[adjTri] = adjMap;

                    if (dir == 0)
                        label->emb_.push_back(Dim4TriangleEmbedding(
                            adjPent, adjTri));
                    else
                        label->emb_.push_front(Dim4TriangleEmbedding(
                            adjPent, adjTri));

                    pent = adjPent;
                    tri = adjTri;
                    map = adjMap;
                }
            }
        }
    }
}

void Dim4Triangulation::calculateBoundary() const {
    // Are there any boundary tetrahedra at all?
    long nBdry = 2 * tetrahedra_.size() - 5 * simplices_.size();
    if (nBdry == 0)
        return;

    // When triangulating the boundaries, we will need to be able to map
    // (tetrahedron index in 4-manifold) to (tetrahedron in 3-manifold
    // boundary).  There are probably better ways, but we'll just store
    // the (3-manifold tetrahedra) in an array of size
    // (number of 4-manifold tetrahedra).
    NTetrahedron** bdryTetAll = new NTetrahedron*[tetrahedra_.size()];
    std::fill(bdryTetAll, bdryTetAll + tetrahedra_.size(),
        static_cast<NTetrahedron*>(0));

    Dim4BoundaryComponent* label;
    TetrahedronIterator it;
    Dim4Tetrahedron *loopTet;
    std::queue<Dim4Tetrahedron*> queue;
    Dim4Pentachoron *pent, *adjPent;
    int facet, adjFacet;
    Dim4Vertex* vertex;
    Dim4Edge* edge;
    Dim4Triangle* tri;
    Dim4TriangleEmbedding triEmb;
    Dim4Tetrahedron *tet, *adjTet;
    int tetTri, adjTetTri;
    NTetrahedron *bdryTet, *adjBdryTet;
    int i, j;
    for (it = tetrahedra_.begin(); it != tetrahedra_.end(); ++it) {
        loopTet = *it;

        // We only care about boundary tetrahedra that we haven't yet seen..
        if (loopTet->nEmb_ == 2 || loopTet->boundaryComponent_)
            continue;

        label = new Dim4BoundaryComponent();
        boundaryComponents_.push_back(label);
        loopTet->component_->boundaryComponents_.push_back(label);

        label->boundary_ = new NTriangulation();

        // Run a breadth-first search from this boundary tetrahedron to
        // completely enumerate all tetrahedra in this boundary component.
        //
        // Because we use a first-in-first-out queue, we know that
        // tetrahedra are added to the boundary triangulation in the
        // same order as they are added to the list label->tetrahedra_.
        loopTet->boundaryComponent_ = label;
        label->tetrahedra_.push_back(loopTet);

        queue.push(loopTet);

        while (! queue.empty()) {
            tet = queue.front();
            queue.pop();
            pent = tet->emb_[0].getPentachoron();
            facet = tet->emb_[0].getTetrahedron();

            bdryTetAll[tet->markedIndex()] = bdryTet =
                label->boundary_->newTetrahedron();

            // Run through the vertices and edges on this tetrahedron.
            for (i = 0; i < 5; ++i)
                if (i != facet) {
                    vertex = pent->vertex_[i];
                    if (vertex->boundaryComponent_ != label)
                        vertex->boundaryComponent_ = label;
                }

            for (i = 0; i < 5; ++i) {
                if (i == facet)
                    continue;
                for (j = i + 1; j < 5; ++j) {
                    if (j == facet)
                        continue;

                    edge = pent->edge_[Dim4Edge::edgeNumber[i][j]];
                    if (edge->boundaryComponent_ != label)
                        edge->boundaryComponent_ = label;
                }
            }

            // Now run through the triangles of this tetrahedron, and follow
            // through them to adjacent tetrahedra.
            for (i = 0; i < 5; ++i) {
                if (i == facet)
                    continue;

                // Examine the triangle opposite vertices (i, facet).  This is
                // the triangle opposite the edge joining vertices (i, facet).
                tri = pent->triangle_[Dim4Edge::edgeNumber[i][facet]];
                if (! tri->boundaryComponent_)
                    tri->boundaryComponent_ = label;

                // Okay, we can be clever about this.  The current
                // boundary tetrahedron is one end of the triangle link; the
                // *adjacent* boundary tetrahedron must be at the other.
                triEmb = tri->emb_.front();
                if (triEmb.getPentachoron() == pent &&
                        triEmb.getVertices()[3] == i &&
                        triEmb.getVertices()[4] == facet) {
                    // We are currently looking at the embedding at the
                    // front of the list.  Take the one at the back.
                    triEmb = tri->emb_.back();

                    adjPent = triEmb.getPentachoron();
                    adjFacet = triEmb.getVertices()[3];
                    adjTet = adjPent->tet_[adjFacet];
                    j = triEmb.getVertices()[4];
                } else {
                    // We must be looking at the embedding at the back
                    // of the list.  Take the one at the front (which is
                    // already stored in triEmb).
                    adjPent = triEmb.getPentachoron();
                    adjFacet = triEmb.getVertices()[4];
                    adjTet = adjPent->tet_[adjFacet];
                    j = triEmb.getVertices()[3];

                    // TODO: Sanity checking; remove this eventually.
                    triEmb = tri->emb_.back();
                    if (! (triEmb.getPentachoron() == pent &&
                            triEmb.getVertices()[4] == i &&
                            triEmb.getVertices()[3] == facet)) {
                        std::cerr << "ERROR: Something has gone terribly "
                            "wrong in computeBoundaryComponents()."
                            << std::endl;
                        ::exit(1);
                    }
                }

                // Glue the corresponding boundary tetrahedra if both
                // are ready to go.
                adjBdryTet = bdryTetAll[adjTet->markedIndex()];
                if (adjBdryTet) {
                    // We might have the same tetrahedron joined to
                    // itself; make sure we only glue in one direction.
                    if (! bdryTet->adjacentTetrahedron(
                            pent->tetMapping_[facet].preImageOf(i))) {
                        // Glue away.
                        tetTri = pent->tetMapping_[facet].preImageOf(i);
                        adjTetTri = adjPent->tetMapping_[adjFacet].
                            preImageOf(j);

                        bdryTet->joinTo(tetTri, adjBdryTet,
                            perm5to4(adjTet->getTriangleMapping(adjTetTri) *
                            tet->getTriangleMapping(tetTri).inverse()));
                    }
                }

                // Push the adjacent tetrahedron onto the queue for
                // processing.
                if (! adjTet->boundaryComponent_) {
                    adjTet->boundaryComponent_ = label;
                    label->tetrahedra_.push_back(adjTet);
                    queue.push(adjTet);
                }
            }
        }

        // This boundary 3-manifold triangulation is complete.

        // Now run through the vertices, edges and triangles of the
        // 3-manifold triangulation and insert the corresponding 4-D
        // objects into the boundary component lists in the *same* order.
        for (NTriangulation::TriangleIterator it =
                label->boundary_->getTriangles().begin();
                it != label->boundary_->getTriangles().end(); ++it) {
            const NTriangleEmbedding& emb = (*it)->getEmbedding(0);
            tet = label->tetrahedra_[emb.getTetrahedron()->markedIndex()];
            label->triangles_.push_back(tet->getTriangle(emb.getTriangle()));
        }
        for (NTriangulation::EdgeIterator it =
                label->boundary_->getEdges().begin();
                it != label->boundary_->getEdges().end(); ++it) {
            const NEdgeEmbedding& emb = (*it)->getEmbedding(0);
            tet = label->tetrahedra_[emb.getTetrahedron()->markedIndex()];
            label->edges_.push_back(tet->getEdge(emb.getEdge()));
        }
        for (NTriangulation::VertexIterator it =
                label->boundary_->getVertices().begin();
                it != label->boundary_->getVertices().end(); ++it) {
            const NVertexEmbedding& emb = (*it)->getEmbedding(0);
            tet = label->tetrahedra_[emb.getTetrahedron()->markedIndex()];
            label->vertices_.push_back(tet->getVertex(emb.getVertex()));
        }
    }

    delete[] bdryTetAll;
}

void Dim4Triangulation::calculateVertexLinks() const {
    long n = simplices_.size();
    if (n == 0)
        return;

    bool foundNonSimpleLink = false;

    // Construct the vertex linking tetrahedra, and insert them into each
    // vertex link in the correct order as described by the
    // Dim4Vertex::getLink() docs.
    NTetrahedron** tet = new NTetrahedron*[5 * n];

    Dim4Vertex* vertex;
    VertexIterator vit;
    std::vector<Dim4VertexEmbedding>::const_iterator embit;
    for (vit = vertices_.begin(); vit != vertices_.end(); ++vit) {
        vertex = *vit;
        vertex->link_ = new NTriangulation();
        for (embit = vertex->emb_.begin(); embit != vertex->emb_.end();
                ++embit)
            tet[5 * embit->getPentachoron()->markedIndex() + embit->getVertex()]
                = vertex->link_->newTetrahedron();
    }

    // Now glue the tetrahedra together correctly.
    Dim4Pentachoron *pent, *adjPent;
    long pentIdx, adjPentIdx;
    int vertexIdx, adjVertexIdx;
    int exitFacet, adjFacet;

    long index = 0; // Index into the tet[] array.
    for (pentIdx = 0; pentIdx < n; ++pentIdx) {
        pent = simplices_[pentIdx];
        for (vertexIdx = 0; vertexIdx < 5; ++vertexIdx) {
            // Glue this piece of vertex link to any adjacent pieces of
            // vertex link.
            for (exitFacet = 0; exitFacet < 5; ++exitFacet) {
                if (exitFacet == vertexIdx)
                    continue;

                adjPent = pent->adjacentPentachoron(exitFacet);
                if (! adjPent)
                    continue;

                // Make sure we perform each gluing in one direction only.
                adjPentIdx = adjPent->markedIndex();
                if (adjPentIdx > pentIdx)
                    continue;
                adjFacet = pent->adjacentFacet(exitFacet);
                if (adjPentIdx == pentIdx && adjFacet > exitFacet)
                    continue;

                // This tetrahedron is adjacent to a previously-seen
                // tetrahedron.  Make the gluing.
                adjVertexIdx = pent->adjacentGluing(exitFacet)[vertexIdx];
                tet[index]->joinTo(
                    pent->tetMapping_[vertexIdx].preImageOf(exitFacet),
                    tet[5 * adjPentIdx + adjVertexIdx],
                    perm5to4(adjPent->tetMapping_[adjVertexIdx].inverse() *
                        pent->adjacentGluing(exitFacet) *
                        pent->tetMapping_[vertexIdx]));
            }
            ++index;
        }
    }

    // Look at each vertex link and see what it says about this 4-manifold
    // triangulation.
    for (vit = vertices_.begin(); vit != vertices_.end(); ++vit) {
        vertex = *vit;

        if (vertex->link_->hasBoundaryTriangles()) {
            // It's a 3-ball or nothing.
            if ((! knownSimpleLinks_) && ! vertex->link_->isBall()) {
                valid_ = vertex->valid_ = false;
                foundNonSimpleLink = true;
                // The vertex belongs to some pentachoron with boundary
                // tetrahedra, and so already belongs to a boundary component.
            }
        } else {
            // The vertex link has no boundary triangles, which means this
            // vertex is not part of any boundary tetrahedra.
            // Let's see what we've got.
            if ((! vertex->link_->isValid()) || vertex->link_->isIdeal()) {
                // Bapow.
                valid_ = vertex->valid_ = false;
                foundNonSimpleLink = true;
                boundaryComponents_.push_back(
                    vertex->boundaryComponent_ =
                    new Dim4BoundaryComponent(vertex));
            } else if ((! knownSimpleLinks_) &&
                    ! vertex->link_->isThreeSphere()) {
                // The vertex is fine but it's not a 3-sphere.
                // We have an ideal triangulation.
                ideal_ = vertex->component_->ideal_ = vertex->ideal_ = true;
                foundNonSimpleLink = true;
                boundaryComponents_.push_back(
                    vertex->boundaryComponent_ =
                    new Dim4BoundaryComponent(vertex));
            }
            // The only case not covered is a 3-sphere link, where we
            // have nothing to do.
        }

        // Hunt down invalid edge links.
        // If an edge has an invalid link, then we can follow this through
        // to the vertex linking 3-manifold at the endpoint of the edge,
        // where we will find that this 3-manifold has a corresponding
        // invalid vertex link.
        // As an exception, edges with reverse self-identifications might also
        // have invalid links, but these might not translate up to the vertex
        // link (e.g., a projective plane edge link might become the
        // spherical double cover at the vertex link).  We detect these
        // cases separately under calculateEdgeLinks() below.
        if (! vertex->valid_) {
            NTriangulation::VertexIterator linkit;
            int type;
            for (linkit = vertex->link_->getVertices().begin();
                    linkit != vertex->link_->getVertices().end(); ++linkit) {
                type = (*linkit)->getLink();
                if (type != NVertex::SPHERE && type != NVertex::DISC) {
                    // This 3-manifold vertex is at the end of an
                    // invalid 4-manifold edge.

                    // Find a tetrahedron in the 3-manifold vertex link
                    // containing the bad 3-manifold vertex.
                    const NVertexEmbedding& linkemb((*linkit)->getEmbedding(0));

                    // Find the corresponding pentachoron in the 4-manifold
                    // triangulation.
                    const Dim4VertexEmbedding& vemb(vertex->getEmbedding(
                        vertex->link_->tetrahedronIndex(
                            linkemb.getTetrahedron())));

                    // We have the pentachoron (vemb.getPentachoron())
                    // and one of the endpoints of the edge (vemb.getVertex()).
                    // Find the other endpoint of the edge.
                    int otherEnd = vemb.getPentachoron()->
                        tetMapping_[vemb.getVertex()][linkemb.getVertex()];

                    // Got it!
                    vemb.getPentachoron()->edge_[
                        Dim4Edge::edgeNumber[vemb.getVertex()][otherEnd]
                        ]->invalid_ |= Dim4Edge::INVALID_LINK;
                }
            }
        }
    }

    delete[] tet;

    // If every vertex link was a 3-sphere or 3-ball, remember this for
    // future optimisations.
    if (! foundNonSimpleLink)
        knownSimpleLinks_ = true;
}

void Dim4Triangulation::calculateEdgeLinks() const {
    for (EdgeIterator it = edges_.begin(); it != edges_.end(); ++it)
        if (((*it)->invalid_ & Dim4Edge::INVALID_IDENTIFICATION) &&
                ! ((*it)->invalid_ & Dim4Edge::INVALID_LINK)) {
            // Calling buildLink() causes the edge link to be cached by
            // Dim4Edge.
            const Dim2Triangulation* link = (*it)->buildLink();
            if ((link->isClosed() && link->getEulerChar() != 2) ||
                    ((! link->isClosed()) && link->getEulerChar() != 1))
                (*it)->invalid_ |= Dim4Edge::INVALID_LINK;
        }
}

} // namespace regina
