
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include <queue>
#include <utility>
#include "dim2/dim2triangulation.h"

namespace regina {

void Dim2Triangulation::calculateSkeleton() const {
    // Triangulations are orientable until proven otherwise.
    orientable_ = true;

    // Set this now so that any triangle query routines do not try to
    // recursively recompute the skeleton again.
    calculatedSkeleton_ = true;

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one triangle.
    if (triangles_.empty())
        return;

    // Off we go!
    calculateComponents();
        // Sets:
        // - components_
        // - edges_
        // - orientable_
        // - Dim2Component::edges_
        // - Dim2Component::orientable_
        // - Dim2Triangle::component_
        // - Dim2Triangle::orientation_
        // - Dim2Triangle::edge_
        // - Dim2Triangle::edgeMapping_
        // - all Dim2Edge members except boundaryComponent_

    calculateVertices();
        // Sets:
        // - vertices_
        // - Dim2Component::vertices_
        // - Dim2Triangle::vertex_
        // - Dim2Triangle::vertexMapping_
        // - all Dim2Vertex members except boundaryComponent_

    calculateBoundary();
        // Sets:
        // - boundaryComponents_
        // - Dim2Component::boundaryComponents_
        // - Dim2 [ Edge, Vertex ]::boundaryComponent_
        // - all Dim2BoundaryComponent members
}

void Dim2Triangulation::calculateComponents() const {
    TriangleIterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        (*it)->component_ = 0;
        std::fill((*it)->edge_, (*it)->edge_ + 3, static_cast<Dim2Edge*>(0));
    }

    Dim2Component* label;
    Dim2Triangle** stack = new Dim2Triangle*[triangles_.size()];
    unsigned stackSize = 0;
    Dim2Triangle *loopTri, *tri, *adjTri;
    Dim2Edge* e;
    int edge, adjEdge, adjOrientation;
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        loopTri = *it;
        if (loopTri->component_)
            continue;

        label = new Dim2Component();
        components_.push_back(label);

        // Run a depth-first search from this triangle to
        // completely enumerate all triangles in this component.
        //
        // Since we are walking from one triangle to another via
        // edges, we might as well collect information on the
        // edges while we're at it.
        loopTri->component_ = label;
        label->triangles_.push_back(loopTri);
        loopTri->orientation_ = 1;

        stack[0] = loopTri;
        stackSize = 1;

        while (stackSize > 0) {
            tri = stack[--stackSize];

            for (edge = 0; edge < 3; ++edge) {
                // Have we already checked out this edge from the other side?
                if (tri->edge_[edge])
                    continue;

                // Make a new edge, but leave the embeddings and
                // mappings until we see which side we're on.  This is so
                // that the edges *look* as though we enumerated them in
                // lexicographical order (instead of via a depth-first search
                // through each triangulation component).
                e = new Dim2Edge(tri->component_);

                adjTri = tri->adjacentTriangle(edge);
                if (adjTri) {
                    // We have an adjacent triangle.
                    adjEdge = tri->adjacentEdge(edge);

                    tri->edge_[edge] = e;
                    adjTri->edge_[adjEdge] = e;

                    // Choose an edge mapping according to which side
                    // comes "first" lexicographically.  Note that edge 2 is
                    // really edge 01, and so is "less than" edge 0, which
                    // is really edge 12.  In short, edges get ordered in
                    // reverse.
                    if (tri->markedIndex() < adjTri->markedIndex() ||
                            (tri->markedIndex() == adjTri->markedIndex() &&
                             edge > adjEdge)) {
                        // tri comes first.
                        tri->edgeMapping_[edge] = Dim2Edge::ordering[edge];
                        adjTri->edgeMapping_[adjEdge] =
                            tri->adjacentGluing(edge) *
                            Dim2Edge::ordering[edge];

                        e->emb_[0] = Dim2EdgeEmbedding(tri, edge);
                        e->emb_[1] = Dim2EdgeEmbedding(adjTri, adjEdge);
                    } else {
                        // adjTri comes first.
                        adjTri->edgeMapping_[adjEdge] =
                            Dim2Edge::ordering[adjEdge];
                        tri->edgeMapping_[edge] =
                            adjTri->adjacentGluing(adjEdge) *
                            Dim2Edge::ordering[adjEdge];

                        e->emb_[0] = Dim2EdgeEmbedding(adjTri, adjEdge);
                        e->emb_[1] = Dim2EdgeEmbedding(tri, edge);
                    }
                    e->nEmb_ = 2;

                    // Deal with orientations and connected components.
                    adjOrientation = (tri->adjacentGluing(edge).sign() == 1 ?
                        - tri->orientation_ : tri->orientation_);
                    if (adjTri->component_) {
                        if (adjOrientation != adjTri->orientation_)
                            orientable_ = label->orientable_ = false;
                    } else {
                        // Wheee!  A triangle we haven't seen before.
                        adjTri->component_ = label;
                        label->triangles_.push_back(adjTri);
                        adjTri->orientation_ = adjOrientation;

                        stack[stackSize++] = adjTri;
                    }
                } else {
                    // This is a boundary edge.
                    tri->edge_[edge] = e;
                    tri->edgeMapping_[edge] = Dim2Edge::ordering[edge];

                    e->emb_[0] = Dim2EdgeEmbedding(tri, edge);
                    e->nEmb_ = 1;
                }
            }
        }
    }

    // Now run through again and number the edges (i.e., insert them
    // into the main list) in lexicographical order.  Again, edges are
    // ordered in reverse.
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        tri = *it;
        for (edge = 2; edge >= 0; --edge) {
            e = tri->edge_[edge];
            if (e->nEmb_ == 2 &&
                    (e->emb_[0].getTriangle() != tri ||
                     e->emb_[0].getEdge() != edge))
                continue;

            edges_.push_back(e);
            tri->component_->edges_.push_back(e);
        }
    }

    delete[] stack;
}

void Dim2Triangulation::calculateVertices() const {
    TriangleIterator it;
    int loopVtx;
    for (it = triangles_.begin(); it != triangles_.end(); ++it)
        for (loopVtx = 0; loopVtx < 3; ++loopVtx)
            (*it)->vertex_[loopVtx] = 0;

    Dim2Vertex* label;
    Dim2Triangle *loopTri, *tri, *adjTri;
    int vertex, adjVertex;
    NPerm3 map, adjMap;
    int dir, exitEdge;
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        loopTri = *it;
        for (loopVtx = 2; loopVtx >= 0; --loopVtx) {
            if (loopTri->vertex_[loopVtx])
                continue;

            label = new Dim2Vertex(loopTri->component_);
            vertices_.push_back(label);
            loopTri->component_->vertices_.push_back(label);

            // Since triangle vertices are joined together in a loop, the
            // depth-first search is really just a straight line in either
            // direction.  We therefore do away with the usual stack and
            // just keep track of the next triangle to process in the current
            // direction.
            loopTri->vertex_[loopVtx] = label;
            loopTri->vertexMapping_[loopVtx] =
                NPerm3(loopVtx, (loopVtx + 1) % 3, (loopVtx + 2) % 3);
            label->emb_.push_back(Dim2VertexEmbedding(loopTri, loopVtx));

            for (dir = 0; dir < 2; ++dir) {
                // Start at the start and walk in one particular direction.
                tri = loopTri;
                vertex = loopVtx;
                map = tri->vertexMapping_[vertex];

                while (true) {
                    // Move through to the next triangle.
                    exitEdge = map[dir == 0 ? 1 : 2];
                    adjTri = tri->adjacentTriangle(exitEdge);
                    if (! adjTri)
                        break;

                    adjMap = tri->adjacentGluing(exitEdge) * map *
                        NPerm3(0, 2, 1);
                    adjVertex = adjMap[0];

                    if (adjTri->vertex_[adjVertex]) {
                        // We looped right around.
                        break;
                    }

                    // We have not yet seen this triangle vertex.  Label it.
                    adjTri->vertex_[adjVertex] = label;
                    adjTri->vertexMapping_[adjVertex] = adjMap;

                    if (dir == 0)
                        label->emb_.push_back(Dim2VertexEmbedding(
                            adjTri, adjVertex));
                    else
                        label->emb_.push_front(Dim2VertexEmbedding(
                            adjTri, adjVertex));

                    tri = adjTri;
                    vertex = adjVertex;
                    map = adjMap;
                }
            }
        }
    }
}

void Dim2Triangulation::calculateBoundary() const {
    // Are there any boundary edges at all?
    long nBdry = 2 * edges_.size() - 3 * triangles_.size();
    if (nBdry == 0)
        return;

    Dim2BoundaryComponent* label;
    EdgeIterator it;
    Dim2Triangle *tri, *adjTri;
    int edgeId, adjEdgeId;
    int vertexId, adjVertexId;
    Dim2Edge *edge, *adjEdge;
    Dim2Vertex* vertex;
    Dim2VertexEmbedding vertexEmb;

    for (it = edges_.begin(); it != edges_.end(); ++it) {
        edge = *it;

        // We only care about boundary edges that we haven't yet seen..
        if (edge->nEmb_ == 2 || edge->boundaryComponent_)
            continue;

        label = new Dim2BoundaryComponent();
        boundaryComponents_.push_back(label);
        edge->component_->boundaryComponents_.push_back(label);

        // Loop around from this boundary edge to
        // completely enumerate all edges in this boundary component.

        tri = edge->emb_[0].getTriangle();
        edgeId = edge->emb_[0].getEdge();
        vertexId = edge->emb_[0].getVertices()[0];
        vertex = tri->vertex_[vertexId];
        while (true) {
            if (! edge->boundaryComponent_) {
                edge->boundaryComponent_ = label;
                label->edges_.push_back(edge);

                vertex->boundaryComponent_ = label;
                label->vertices_.push_back(vertex);
            } else {
                // We've looped right around.
                break;
            }

            // Find the next edge along the boundary.

            // We can be clever about this.  The current
            // boundary edge is one end of the vertex link; the
            // *adjacent* boundary edge must be at the other.
            vertexEmb = vertex->emb_.front();
            if (vertexEmb.getTriangle() == tri &&
                    vertexEmb.getVertices()[0] == vertexId &&
                    vertexEmb.getVertices()[2] == edgeId) {
                // We are currently looking at the embedding at the
                // front of the list.  Take the one at the back.
                vertexEmb = vertex->emb_.back();

                adjTri = vertexEmb.getTriangle();
                adjEdgeId = vertexEmb.getVertices()[1];
                adjEdge = adjTri->edge_[adjEdgeId];
                adjVertexId = vertexEmb.getVertices()[2];
            } else {
                // We must be looking at the embedding at the back
                // of the list.  Take the one at the front (which is
                // already stored in vertexEmb).
                adjTri = vertexEmb.getTriangle();
                adjEdgeId = vertexEmb.getVertices()[2];
                adjEdge = adjTri->edge_[adjEdgeId];
                adjVertexId = vertexEmb.getVertices()[1];

                // TODO: Sanity checking; remove this eventually.
                vertexEmb = vertex->emb_.back();
                if (! (vertexEmb.getTriangle() == tri &&
                        vertexEmb.getVertices()[0] == vertexId &&
                        vertexEmb.getVertices()[1] == edgeId)) {
                    std::cerr << "ERROR: Something has gone terribly "
                        "wrong in computeBoundaryComponents()."
                        << std::endl;
                    ::exit(1);
                }
            }

            edge = adjEdge;
            tri = adjTri;
            edgeId = adjEdgeId;
            vertexId = adjVertexId;
            vertex = tri->vertex_[vertexId];
        }
    }
}

} // namespace regina

