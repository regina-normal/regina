
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

#include <queue>
#include <utility>
#include "dim4/dim4triangulation.h"

namespace regina {

void Dim4Triangulation::calculateSkeleton() const {
    // Triangulations are valid, orientable and non-ideal until proven otherwise.
    orientable_ = true;
    valid_ = true;
    ideal_ = false;

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one pentachoron.
    if (pentachora_.empty())
        return;

    // Off we go!
    calculateComponents();
        // Sets:
        // - components_
        // - orientable_
        // - Dim4Component::pentachora_
        // - Dim4Component::orientable_
        // - Dim4Pentachoron::component_
        // - Dim4Pentachoron::orientation_

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

    calculateFaces();
        // Sets:
        // - faces_
        // - Dim4Component::faces_
        // - Dim4Pentachoron::face_
        // - Dim4Pentachoron::faceMapping_
        // - valid_ and Dim4Face::valid_ in the case of bad face self-gluings
        // - all other Dim4Face members except boundaryComponent_

    calculateBoundary();
        // Sets:
        // - boundaryComponents_
        // - Dim4Component::boundaryComponents_
        // - Dim4 [ Tetrahedron, Face, Edge, Vertex ]::boundaryComponent_
        // - all Dim4BoundaryComponent members except boundary_

    // TODO: Still missing boundary and vertex link 3-manifold triangulations.
    // Specifically, we are still missing:
    //
    // - Dim4BoundaryComponent::boundary_
    //
    // - Dim4Component::ideal_
    // - valid_ and Dim4Edge::invalid_ in the case of bad edge links
    // - Dim4Vertex::link_
    // - Dim4Vertex::valid_
    // - Dim4Vertex::ideal_

    calculatedSkeleton_ = true;
}

void Dim4Triangulation::calculateComponents() const {
    PentachoronIterator it;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it)
        (*it)->component_ = 0;

    Dim4Component* label;
    Dim4Pentachoron** stack = new Dim4Pentachoron*[pentachora_.size()];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int facet, adjOrientation;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        loopPent = *it;
        if (loopPent->component_)
            continue;

        label = new Dim4Component();
        components_.push_back(label);

        // Run a depth-first search from this pentachronon to
        // completely enumerate all pentachora in this component.
        loopPent->component_ = label;
        label->pentachora_.push_back(loopPent);
        loopPent->orientation_ = 1;

        stack[0] = loopPent;
        stackSize = 1;

        while (stackSize > 0) {
            pent = stack[--stackSize];

            for (facet = 0; facet < 5; ++facet) {
                adjPent = pent->adjacentPentachoron(facet);
                if (adjPent) {
                    adjOrientation = (pent->adjacentGluing(facet).sign() == 1 ?
                        - pent->orientation_ : pent->orientation_);
                    if (adjPent->component_) {
                        if (adjOrientation != adjPent->orientation_)
                            orientable_ = label->orientable_ = false;
                    } else {
                        adjPent->component_ = label;
                        label->pentachora_.push_back(adjPent);
                        adjPent->orientation_ = adjOrientation;

                        stack[stackSize++] = adjPent;
                    }
                }
            }
        }
    }
    delete[] stack;
}

void Dim4Triangulation::calculateTetrahedra() const {
    PentachoronIterator it;
    int loopFacet;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it)
        for (loopFacet = 0; loopFacet < 5; ++loopFacet)
            (*it)->tet_[loopFacet] = 0;

    Dim4Tetrahedron* label;
    Dim4Pentachoron *loopPent, *adjPent;
    int adjFacet;
    NPerm5 adjVertices;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        loopPent = *it;
        for (loopFacet = 4; loopFacet >= 0; --loopFacet) {
            if (loopPent->tet_[loopFacet])
                continue;

            label = new Dim4Tetrahedron(loopPent->component_);
            tetrahedra_.push_back(label);
            loopPent->component_->tetrahedra_.push_back(label);

            loopPent->tet_[loopFacet] = label;
            loopPent->tetMapping_[loopFacet] =
                Dim4Tetrahedron::ordering[loopFacet];
            label->emb_[0] = Dim4TetrahedronEmbedding(loopPent, loopFacet);
            label->nEmb_ = 1;

            adjPent = loopPent->adjacentPentachoron(loopFacet);
            if (adjPent) {
                // This tetrahedron is not on the boundary.
                adjFacet = loopPent->adjacentFacet(loopFacet);

                adjPent->tet_[adjFacet] = label;
                adjPent->tetMapping_[adjFacet] =
                    loopPent->adjacentGluing(loopFacet) *
                    loopPent->tetMapping_[loopFacet];
                label->emb_[1] =
                    Dim4TetrahedronEmbedding(adjPent, adjFacet);
                label->nEmb_ = 2;
            }
        }
    }
}

void Dim4Triangulation::calculateVertices() const {
    PentachoronIterator it;
    int loopVtx;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it)
        for (loopVtx = 0; loopVtx < 5; ++loopVtx)
            (*it)->vertex_[loopVtx] = 0;

    Dim4Vertex* label;
    typedef std::pair<Dim4Pentachoron*, int> Spec; /* (pent, vertex) */
    Spec* stack = new Spec[pentachora_.size() * 5];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int vertex, adjVertex;
    int facet;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
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
                        adjVertex = pent->adjacentGluing(facet)[vertex];

                        if (! adjPent->vertex_[adjVertex]) {
                            adjPent->vertex_[adjVertex] = label;
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
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it)
        for (loopEdge = 0; loopEdge < 10; ++loopEdge)
            (*it)->edge_[loopEdge] = 0;

    Dim4Edge* label;
    typedef std::pair<Dim4Pentachoron*, int> Spec; /* (pent, edge) */
    Spec* stack = new Spec[pentachora_.size() * 10];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int edge, adjEdge;
    int facet;
    NPerm5 adjMap;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
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
                        // formed by the images of 2, 3 and 4.  We don't
                        // actually guarantee this in the docs, but there's
                        // no harm coding it up now in case we decide we
                        // want it later on.
                        //
                        // Note this only becomes meaningful if the edge link
                        // is an orientable surface (otherwise there is no
                        // consistent way to orient these cycles at all).
                        adjEdge = pent->adjacentGluing(facet)[edge];
                        adjMap = pent->adjacentGluing(facet) *
                            pent->edgeMapping_[edge] * NPerm5(2, 3);

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

void Dim4Triangulation::calculateFaces() const {
    PentachoronIterator it;
    int loopFace;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it)
        for (loopFace = 0; loopFace < 10; ++loopFace)
            (*it)->face_[loopFace] = 0;

    Dim4Face* label;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    int face, adjFace;
    NPerm5 map, adjMap;
    int dir, exitFacet;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        loopPent = *it;
        for (loopFace = 0; loopFace < 10; ++loopFace) {
            if (loopPent->face_[loopFace])
                continue;

            label = new Dim4Face(loopPent->component_);
            faces_.push_back(label);
            loopPent->component_->faces_.push_back(label);

            // Since pentachoron faces are joined together in a loop, the
            // depth-first search is really just a straight line in either
            // direction.  We therefore do away with the usual stack and
            // just keep track of the next face to process in the current
            // direction.
            loopPent->face_[loopFace] = label;
            loopPent->faceMapping_[loopFace] = Dim4Face::ordering[loopFace];
            label->emb_.push_back(Dim4FaceEmbedding(loopPent, loopFace));

            for (dir = 0; dir < 2; ++dir) {
                // Start at the start and walk in one particular direction.
                pent = loopPent;
                face = loopFace;
                map = pent->faceMapping_[face];

                while (true) {
                    // Move through to the next pentachoron.
                    exitFacet = map[dir == 0 ? 3 : 4];
                    adjPent = pent->adjacentPentachoron(exitFacet);
                    if (! adjPent)
                        break;

                    adjMap = pent->adjacentGluing(exitFacet) * map *
                        NPerm5(3, 4);
                    adjFace = Dim4Face::faceNumber
                        [adjMap[0]][adjMap[1]][adjMap[2]];

                    if (adjPent->face_[adjFace]) {
                        // We looped right around.

                        // Check that we're not gluing the face to itself by a
                        // non-trivial mapping.
                        //
                        // Since the face link must be orientable (it is just a
                        // circle), we know adjMap[3,4] is the same as for the
                        // original mapping.  Therefore, to test whether
                        // adjMap[0,1,2] is consistent we can just compare the
                        // full permutations (which is in fact faster).
                        if (adjPent->faceMapping_[adjFace] != adjMap) {
                            // You have chosen unwisely, my son.
                            label->valid_ = false;
                            valid_ = false;
                        }
                        break;
                    }

                    // We have not yet seen this pentachoron face.  Label it.
                    adjPent->face_[adjFace] = label;
                    adjPent->faceMapping_[adjFace] = adjMap;

                    if (dir == 0)
                        label->emb_.push_back(Dim4FaceEmbedding(
                            adjPent, adjFace));
                    else
                        label->emb_.push_front(Dim4FaceEmbedding(
                            adjPent, adjFace));

                    pent = adjPent;
                    face = adjFace;
                    map = adjMap;
                }
            }
        }
    }
}

void Dim4Triangulation::calculateBoundary() const {
    // Are there any boundary tetrahedra at all?
    long nBdry = 2 * tetrahedra_.size() - 5 * pentachora_.size();
    if (nBdry == 0)
        return;

    Dim4BoundaryComponent* label;
    TetrahedronIterator it;
    Dim4Tetrahedron *loopTet;
    Dim4Tetrahedron** stack = new Dim4Tetrahedron*[nBdry];
    unsigned stackSize = 0;
    Dim4Pentachoron *pent;
    int facet;
    Dim4Vertex* vertex;
    Dim4Edge* edge;
    Dim4Face* face;
    int i, j;
    for (it = tetrahedra_.begin(); it != tetrahedra_.end(); ++it) {
        loopTet = *it;

        // We only care about boundary tetrahedra that we haven't yet seen..
        if (loopTet->nEmb_ == 2 || loopTet->boundaryComponent_)
            continue;

        label = new Dim4BoundaryComponent();
        boundaryComponents_.push_back(label);
        loopTet->component_->boundaryComponents_.push_back(label);

        // Run a depth-first search from this boundary tetrahedron to
        // completely enumerate all tetrahedra in this boundary component.
        loopTet->boundaryComponent_ = label;
        label->tetrahedra_.push_back(loopTet);

        stack[0] = loopTet;
        stackSize = 1;

        while (stackSize > 0) {
            --stackSize;
            pent = stack[stackSize]->emb_[0].getPentachoron();
            facet = stack[stackSize]->emb_[0].getTetrahedron();

            // Run through the vertices and edges on this tetrahedron.
            for (i = 0; i < 5; ++i)
                if (i != facet) {
                    vertex = pent->vertex_[i];
                    if (! vertex->boundaryComponent_) {
                        vertex->boundaryComponent_ = label;
                        label->vertices_.push_back(vertex);
                    }
                }

            for (i = 0; i < 5; ++i) {
                if (i == facet)
                    continue;
                for (j = i + 1; j < 5; ++j) {
                    if (j == facet)
                        continue;

                    edge = pent->edge_[Dim4Edge::edgeNumber[i][j]];
                    if (! edge->boundaryComponent_) {
                        edge->boundaryComponent_ = label;
                        label->edges_.push_back(edge);
                    }
                }
            }

            // Now run through the faces of this tetrahedron, and follow
            // through them to adjacent tetrahedra.
            for (i = 0; i < 5; ++i) {
                if (i == facet)
                    continue;

                // Examine the face opposite vertices (i, facet).
                // This the face opposite the edge joining vertices (i, facet).
                face = pent->face_[Dim4Edge::edgeNumber[i][facet]];
                if (! face->boundaryComponent_) {
                    face->boundaryComponent_ = label;
                    label->faces_.push_back(face);
                }

                // TODO: Gaaaa.  Pipped at the post.  This gets ugly in higher
                // dimensions.

                // TODO: For each adjacent tetrahedron that we find:
                // adjTet->boundaryComponent = label;
                // label->tetrahedra_.push_back(adjTet);
                // stack[stackSize++] = adjTet;
            }
        }
    }
}

} // namespace regina
