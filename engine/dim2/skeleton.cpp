
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

    // Set this now so that any face query routines do not try to
    // recursively recompute the skeleton again.
    calculatedSkeleton_ = true;

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one face.
    if (faces_.empty())
        return;

    // Off we go!
    calculateComponents();
        // Sets:
        // - components_
        // - edges_
        // - orientable_
        // - Dim2Component::edges_
        // - Dim2Component::orientable_
        // - Dim2Face::component_
        // - Dim2Face::orientation_
        // - Dim2Face::edge_
        // - Dim2Face::edgeMapping_
        // - all Dim2Edge members except boundaryComponent_

    calculateVertices();
        // Sets:
        // - vertices_
        // - Dim2Component::vertices_
        // - Dim2Face::vertex_
        // - Dim2Face::vertexMapping_
        // - all Dim2Vertex members except boundaryComponent_

    calculateBoundary();
        // Sets:
        // - boundaryComponents_
        // - Dim2Component::boundaryComponents_
        // - Dim2 [ Edge, Vertex ]::boundaryComponent_
        // - all Dim2BoundaryComponent members
}

void Dim2Triangulation::calculateComponents() const {
    FaceIterator it;
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        (*it)->component_ = 0;
        std::fill((*it)->edge_, (*it)->edge_ + 3, static_cast<Dim2Edge*>(0));
    }

    Dim2Component* label;
    Dim2Face** stack = new Dim2Face*[faces_.size()];
    unsigned stackSize = 0;
    Dim2Face *loopFace, *face, *adjFace;
    Dim2Edge* e;
    int edge, adjEdge, adjOrientation;
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        loopFace = *it;
        if (loopFace->component_)
            continue;

        label = new Dim2Component();
        components_.push_back(label);

        // Run a depth-first search from this face to
        // completely enumerate all faces in this component.
        //
        // Since we are walking from one face to another via
        // edges, we might as well collect information on the
        // edges while we're at it.
        loopFace->component_ = label;
        label->faces_.push_back(loopFace);
        loopFace->orientation_ = 1;

        stack[0] = loopFace;
        stackSize = 1;

        while (stackSize > 0) {
            face = stack[--stackSize];

            for (edge = 0; edge < 3; ++edge) {
                // Have we already checked out this edge from the other side?
                if (face->edge_[edge])
                    continue;

                // Make a new edge, but leave the embeddings and
                // mappings until we see which side we're on.  This is so
                // that the edges *look* as though we enumerated them in
                // lexicographical order (instead of via a depth-first search
                // through each triangulation component).
                e = new Dim2Edge(face->component_);

                adjFace = face->adjacentFace(edge);
                if (adjFace) {
                    // We have an adjacent face.
                    adjEdge = face->adjacentEdge(edge);

                    face->edge_[edge] = e;
                    adjFace->edge_[adjEdge] = e;

                    // Choose an edge mapping according to which side
                    // comes "first" lexicographically.  Note that edge 2 is
                    // really edge 01, and so is "less than" edge 0, which
                    // is really edge 12.  In short, edges get ordered in
                    // reverse.
                    if (face->markedIndex() < adjFace->markedIndex() ||
                            (face->markedIndex() == adjFace->markedIndex() &&
                             edge > adjEdge)) {
                        // face comes first.
                        face->edgeMapping_[edge] = Dim2Edge::ordering[edge];
                        adjFace->edgeMapping_[adjEdge] =
                            face->adjacentGluing(edge) *
                            Dim2Edge::ordering[edge];

                        e->emb_[0] = Dim2EdgeEmbedding(face, edge);
                        e->emb_[1] = Dim2EdgeEmbedding(adjFace, adjEdge);
                    } else {
                        // adjFace comes first.
                        adjFace->edgeMapping_[adjEdge] =
                            Dim2Edge::ordering[adjEdge];
                        face->edgeMapping_[edge] =
                            adjFace->adjacentGluing(adjEdge) *
                            Dim2Edge::ordering[adjEdge];

                        e->emb_[0] = Dim2EdgeEmbedding(adjFace, adjEdge);
                        e->emb_[1] = Dim2EdgeEmbedding(face, edge);
                    }
                    e->nEmb_ = 2;

                    // Deal with orientations and connected components.
                    adjOrientation = (face->adjacentGluing(edge).sign() == 1 ?
                        - face->orientation_ : face->orientation_);
                    if (adjFace->component_) {
                        if (adjOrientation != adjFace->orientation_)
                            orientable_ = label->orientable_ = false;
                    } else {
                        // Wheee!  A face we haven't seen before.
                        adjFace->component_ = label;
                        label->faces_.push_back(adjFace);
                        adjFace->orientation_ = adjOrientation;

                        stack[stackSize++] = adjFace;
                    }
                } else {
                    // This is a boundary edge.
                    face->edge_[edge] = e;
                    face->edgeMapping_[edge] = Dim2Edge::ordering[edge];

                    e->emb_[0] = Dim2EdgeEmbedding(face, edge);
                    e->nEmb_ = 1;
                }
            }
        }
    }

    // Now run through again and number the edges (i.e., insert them
    // into the main list) in lexicographical order.  Again, edges are
    // ordered in reverse.
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        face = *it;
        for (edge = 2; edge >= 0; --edge) {
            e = face->edge_[edge];
            if (e->nEmb_ == 2 &&
                    (e->emb_[0].getFace() != face ||
                     e->emb_[0].getEdge() != edge))
                continue;

            edges_.push_back(e);
            face->component_->edges_.push_back(e);
        }
    }

    delete[] stack;
}

void Dim2Triangulation::calculateVertices() const {
    FaceIterator it;
    int loopVtx;
    for (it = faces_.begin(); it != faces_.end(); ++it)
        for (loopVtx = 0; loopVtx < 3; ++loopVtx)
            (*it)->vertex_[loopVtx] = 0;

    Dim2Vertex* label;
    Dim2Face *loopFace, *face, *adjFace;
    int vertex, adjVertex;
    NPerm3 map, adjMap;
    int dir, exitEdge;
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        loopFace = *it;
        for (loopVtx = 2; loopVtx >= 0; --loopVtx) {
            if (loopFace->vertex_[loopVtx])
                continue;

            label = new Dim2Vertex(loopFace->component_);
            vertices_.push_back(label);
            loopFace->component_->vertices_.push_back(label);

            // Since face vertices are joined together in a loop, the
            // depth-first search is really just a straight line in either
            // direction.  We therefore do away with the usual stack and
            // just keep track of the next face to process in the current
            // direction.
            loopFace->vertex_[loopVtx] = label;
            loopFace->vertexMapping_[loopVtx] =
                NPerm3(loopVtx, (loopVtx + 1) % 3, (loopVtx + 2) % 3);
            label->emb_.push_back(Dim2VertexEmbedding(loopFace, loopVtx));

            for (dir = 0; dir < 2; ++dir) {
                // Start at the start and walk in one particular direction.
                face = loopFace;
                vertex = loopVtx;
                map = face->vertexMapping_[vertex];

                while (true) {
                    // Move through to the next face.
                    exitEdge = map[dir == 0 ? 1 : 2];
                    adjFace = face->adjacentFace(exitEdge);
                    if (! adjFace)
                        break;

                    adjMap = face->adjacentGluing(exitEdge) * map *
                        NPerm3(0, 2, 1);
                    adjVertex = adjMap[0];

                    if (adjFace->vertex_[adjVertex]) {
                        // We looped right around.
                        break;
                    }

                    // We have not yet seen this face vertex.  Label it.
                    adjFace->vertex_[adjVertex] = label;
                    adjFace->vertexMapping_[adjVertex] = adjMap;

                    if (dir == 0)
                        label->emb_.push_back(Dim2VertexEmbedding(
                            adjFace, adjVertex));
                    else
                        label->emb_.push_front(Dim2VertexEmbedding(
                            adjFace, adjVertex));

                    face = adjFace;
                    vertex = adjVertex;
                    map = adjMap;
                }
            }
        }
    }
}

void Dim2Triangulation::calculateBoundary() const {
    // Are there any boundary edges at all?
    long nBdry = 2 * edges_.size() - 3 * faces_.size();
    if (nBdry == 0)
        return;

    Dim2BoundaryComponent* label;
    EdgeIterator it;
    Dim2Face *face, *adjFace;
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

        face = edge->emb_[0].getFace();
        edgeId = edge->emb_[0].getEdge();
        vertexId = edge->emb_[0].getVertices()[0];
        vertex = face->vertex_[vertexId];
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
            if (vertexEmb.getFace() == face &&
                    vertexEmb.getVertices()[0] == vertexId &&
                    vertexEmb.getVertices()[2] == edgeId) {
                // We are currently looking at the embedding at the
                // front of the list.  Take the one at the back.
                vertexEmb = vertex->emb_.back();

                adjFace = vertexEmb.getFace();
                adjEdgeId = vertexEmb.getVertices()[1];
                adjEdge = adjFace->edge_[adjEdgeId];
                adjVertexId = vertexEmb.getVertices()[2];
            } else {
                // We must be looking at the embedding at the back
                // of the list.  Take the one at the front (which is
                // already stored in vertexEmb).
                adjFace = vertexEmb.getFace();
                adjEdgeId = vertexEmb.getVertices()[2];
                adjEdge = adjFace->edge_[adjEdgeId];
                adjVertexId = vertexEmb.getVertices()[1];

                // TODO: Sanity checking; remove this eventually.
                vertexEmb = vertex->emb_.back();
                if (! (vertexEmb.getFace() == face &&
                        vertexEmb.getVertices()[0] == vertexId &&
                        vertexEmb.getVertices()[1] == edgeId)) {
                    std::cerr << "ERROR: Something has gone terribly "
                        "wrong in computeBoundaryComponents()."
                        << std::endl;
                    ::exit(1);
                }
            }

            edge = adjEdge;
            face = adjFace;
            edgeId = adjEdgeId;
            vertexId = adjVertexId;
            vertex = face->vertex_[vertexId];
        }
    }
}

} // namespace regina

