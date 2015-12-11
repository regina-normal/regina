
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include <queue>
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"

#include <iostream>

namespace regina {

void NTriangulation::calculateSkeleton() const {
    TriangulationBase<3>::calculateSkeleton();

    ideal_ = false;
    valid_ = true;
    standard_ = true;

#if 0
    checkPermutations();
        // Sets valid to false if gluings are mismatched (which should
        // never happen if the NTetrahedron gluing routines have been
        // used correctly)
#endif

    calculateTriangles();
        // Sets triangles, NTriangle.component
    calculateVertices();
        // Sets vertices, NVertex.component, NVertex.linkOrientable.
    calculateEdges();
        // Sets edges, NEdge.component, valid, NEdge.valid
    calculateBoundary();
        // Sets boundaryComponents, NTriangle.boundaryComponent,
        //     NEdge.boundaryComponent, NVertex.boundaryComponent,
        //     NComponent.boundaryComponents
    calculateVertexLinks();
        // Sets valid, ideal, NVertex.link,
        //     NVertex.linkEulerChar, NComponent.ideal,
        //     boundaryComponents, NVertex.boundaryComponent
}

void NTriangulation::checkPermutations() const {
    TetrahedronIterator it;

    for (it = simplices_.begin(); it != simplices_.end(); it++)
        for (int face = 0; face < 4; face++) {
            NTetrahedron * adjacent = (*it) -> adjacentTetrahedron(face);

            if (adjacent) {
                NPerm4 perm = (*it) -> adjacentGluing(face);

                NPerm4 adj_perm = adjacent -> adjacentGluing(perm[face]);

                if (!(perm*adj_perm).isIdentity()) {
                    valid_ = false;

                    // This printing statement is temporary code 
                    // to be removed once enough people have tested it
                    std::cerr << "ERROR: Permutations of adjacent faces "
                                 "do not match in skeleton.cpp" << std::endl;
                }

                if ((*it) != adjacent -> adjacentTetrahedron(perm[face])) {
                    valid_ = false;

                    // This printing statement is temporary code 
                    // to be removed once enough people have tested it
                    std::cerr << "ERROR: Adjacency relations do not match"
                                 " in skeleton.cpp" << std::endl;
                }
            }
        }
}

void NTriangulation::calculateVertices() const {
    TetrahedronIterator it;
    int vertex;
    NTetrahedron* tet;
    NVertex* label;
    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            tet->vertices_[vertex] = 0;
    }

    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            if (! tet->vertices_[vertex]) {
                label = new NVertex(tet->component_);
                tet->component_->vertices_.push_back(label);
                labelVertex(tet, vertex, label);
                FaceList<3, 0>::faces_.push_back(label);
            }
    }
}

void NTriangulation::labelVertex(NTetrahedron* firstTet, int firstVertex,
        NVertex* label) const {
    // Create a queue using simple arrays.
    // Since each tetrahedron vertex is pushed on at most once, the
    // array size does not need to be very large.

    // Note that we have >= 1 tetrahedron, since firstTet != 0.
    NTetrahedron** queueTet = new NTetrahedron*[simplices_.size() * 4];
    int* queueVtx = new int[simplices_.size() * 4];

    firstTet->vertices_[firstVertex] = label;
    firstTet->vertexMapping_[firstVertex] = NPerm4(0, firstVertex);
    firstTet->tmpOrientation_[firstVertex] = 1;
    label->push_back(NVertexEmbedding(firstTet, firstVertex));

    unsigned queueStart = 0, queueEnd = 1;
    queueTet[0] = firstTet;
    queueVtx[0] = firstVertex;

    NTetrahedron* tet;
    NTetrahedron* adjTet;
    int vertex;
    int adjVertex;
    int adjOrientation;
    int face;
    NPerm4 adjMap;

    while (queueStart < queueEnd) {
        tet = queueTet[queueStart];
        vertex = queueVtx[queueStart];
        queueStart++;

        for (face=0; face<4; face++) {
            if (face == vertex) continue;
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet) {
                // When we choose an adjacent gluing map, throw in a
                // swap to preserve the "orientation" of the cycle
                // formed by the images of 1, 2 and 3.  Note that this
                // only becomes meaningful if the vertex link is an
                // orientable surface (otherwise there is no consistent
                // way to orient these cycles at all).
                adjMap = tet->adjacentGluing(face) *
                    tet->vertexMapping_[vertex] * NPerm4(1, 2);
                adjVertex = adjMap[0];

                // We should actually be inverting NTriangle::ordering[adjVertex].
                // However, all we care about is the sign of the permutation,
                // so let's save ourselves those extra few CPU cycles.
                if ((NTriangle::ordering[adjVertex] *
                        tet->adjacentGluing(face) *
                        NTriangle::ordering[vertex]).sign() > 0)
                    adjOrientation = -(tet->tmpOrientation_[vertex]);
                else
                    adjOrientation = tet->tmpOrientation_[vertex];

                if (adjTet->vertices_[adjVertex]) {
                    if (adjTet->tmpOrientation_[adjVertex] != adjOrientation)
                        label->linkOrientable_ = false;
                } else {
                    adjTet->vertices_[adjVertex] = label;
                    adjTet->vertexMapping_[adjVertex] = adjMap;
                    adjTet->tmpOrientation_[adjVertex] = adjOrientation;
                    label->push_back(NVertexEmbedding(adjTet,
                        adjVertex));

                    queueTet[queueEnd] = adjTet;
                    queueVtx[queueEnd] = adjVertex;
                    queueEnd++;
                }
            }
        }
    }

    delete[] queueTet;
    delete[] queueVtx;
}

void NTriangulation::calculateEdges() const {
    TetrahedronIterator it;
    int edge;
    NTetrahedron* tet;
    NEdge* label;
    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            tet->edges_[edge] = 0;
    }

    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            if (! tet->edges_[edge]) {
                label = new NEdge(tet->component_);
                tet->component_->edges_.push_back(label);
                labelEdge(tet, edge, label);
                FaceList<3, 1>::faces_.push_back(label);
            }
    }
}

void NTriangulation::labelEdge(NTetrahedron* firstTet, int firstEdge,
        NEdge* label) const {
    // Since tetrahedron edges are joined together in a loop, the depth-first
    // search is really just a straight line in either direction.
    // We therefore do away with the usual stack/queue and just keep track
    // of the next edge to process in the current direction.

    firstTet->edges_[firstEdge] = label;
    firstTet->edgeMapping_[firstEdge] = NEdge::ordering[firstEdge];
    label->push_back(NEdgeEmbedding(firstTet, firstEdge));

    // The last tetrahedron edge that was successfully processed.
    NTetrahedron* tet;
    NPerm4 tetVertices;

    int exitFace;
    NPerm4 exitPerm;

    // The next tetrahedron edge around from this.
    NTetrahedron* nextTet;
    int nextEdge;
    NPerm4 nextVertices;

    for (int dir = 0; dir < 2; dir++) {
        // Start at the start and walk in one particular direction.
        tet = firstTet;
        tetVertices = tet->edgeMapping_[firstEdge];

        while (true) {
            // Move through to the next tetrahedron.
            exitFace = tetVertices[dir == 0 ? 2 : 3];
            nextTet = tet->adjacentTetrahedron(exitFace);
            if (! nextTet)
                break;

            exitPerm = tet->adjacentGluing(exitFace);
            nextVertices = exitPerm * tetVertices * NPerm4(2, 3);
            nextEdge = NEdge::edgeNumber[nextVertices[0]][nextVertices[1]];

            if (nextTet->edges_[nextEdge]) {
                // We looped right around.
                // Check that we're not labelling the edge in reverse.
                if (nextTet->edgeMapping_[nextEdge][0] != nextVertices[0]) {
                    // The edge is being labelled in reverse!
                    label->valid_ = false;
                    valid_ = false;
                }
                break;
            }

            // We have a new tetrahedron edge; this needs to be labelled.
            nextTet->edges_[nextEdge] = label;
            nextTet->edgeMapping_[nextEdge] = nextVertices;

            if (dir == 0)
                label->push_back(NEdgeEmbedding(nextTet, nextEdge));
            else
                label->push_front(NEdgeEmbedding(nextTet, nextEdge));

            tet = nextTet;
            tetVertices = nextVertices;
        }
    }
}

void NTriangulation::calculateTriangles() const {
    TetrahedronIterator it;
    int face;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NTriangle* label;
    NPerm4 adjVertices;
    int adjFace;
    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++)
            tet->triangles_[face] = 0;
    }

    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        tet = *it;
        for (face=3; face>=0; face--)
            if (! tet->triangles_[face]) {
                label = new NTriangle(tet->component_);
                tet->component_->triangles_.push_back(label);
                tet->triangles_[face] = label;
                tet->triMapping_[face] = NTriangle::ordering[face];
                label->push_back(NTriangleEmbedding(tet, face));
                adjTet = tet->adjacentTetrahedron(face);
                if (adjTet) {
                    // Triangle is not on the boundary.
                    adjFace = tet->adjacentFace(face);
                    adjVertices = (tet->adjacentGluing(face))*
                        tet->triMapping_[face];
                    adjTet->triangles_[adjFace] = label;
                    adjTet->triMapping_[adjFace] = adjVertices;
                    label->push_back(NTriangleEmbedding(adjTet, adjFace));
                }
                FaceList<3, 2>::faces_.push_back(label);
            }
    }
}

void NTriangulation::calculateBoundary() const {
    // Sets boundaryComponents, NTriangle.boundaryComponent,
    //     NEdge.boundaryComponent, NVertex.boundaryComponent,
    //     NComponent.boundaryComponents
    NBoundaryComponent* label;

    for (NTriangle* triangle : getTriangles()) {
        if (triangle->degree() < 2)
            if (triangle->boundaryComponent_ == 0) {
                label = new NBoundaryComponent();
                label->orientable_ = true;
                labelBoundaryTriangle(triangle, label);
                boundaryComponents_.push_back(label);
                triangle->getComponent()->boundaryComponents_.push_back(label);
            }
    }
}

void NTriangulation::labelBoundaryTriangle(NTriangle* firstTriangle,
        NBoundaryComponent* label) const {
    std::queue<NTriangle*> triangleQueue;

    const NTriangleEmbedding& emb = firstTriangle->front();
    firstTriangle->boundaryComponent_ = label;
    label->triangles_.push_back(firstTriangle);
    emb.getTetrahedron()->tmpOrientation_[emb.getTriangle()] = 1;
    triangleQueue.push(firstTriangle);

    NTetrahedron* tet;
    NPerm4 tetVertices;
    int tetFace;
    int i,j;
    NVertex* vertex;
    NEdge* edge;

    NTriangle* triangle;
    NTriangle* nextTriangle;
    int nextFaceNumber;
    NPerm4 nextFacePerm;
    NTetrahedron* nextTet;
    int followFromFace;
    NPerm4 switchPerm;
    int yourOrientation;

    while (! triangleQueue.empty()) {
        triangle = triangleQueue.front();
        triangleQueue.pop();

        // Run through the edges and vertices on this triangle.
        tet = triangle->front().getTetrahedron();
        tetFace = triangle->front().getTriangle();
        tetVertices = tet->triMapping_[tetFace];

        // Run through the vertices.
        for (i=0; i<3; i++) {
            vertex = tet->vertices_[tetVertices[i]];
            if (vertex->boundaryComponent_ != label) {
                // A vertex in an invalid triangulation might end up in
                // more than one boundary component.  Push it into all
                // of the relevant boundary components' lists.
                vertex->boundaryComponent_ = label;
                label->vertices_.push_back(vertex);
            }
        }

        // Run through the edges.
        for (i=0; i<3; i++)
            for (j=i+1; j<3; j++) {
                edge = tet->edges_[NEdge::edgeNumber[tetVertices[i]]
                    [tetVertices[j]]];
                if (! (edge->boundaryComponent_)) {
                    edge->boundaryComponent_ = label;
                    label->edges_.push_back(edge);
                }

                // Label the adjacent boundary triangle with the same label.
                followFromFace = 6 - tetVertices[i] - tetVertices[j] - tetFace;
                switchPerm = NPerm4(followFromFace, tetFace);
                nextFaceNumber = followFromFace;
                nextFacePerm = NPerm4();
                nextTet = tet;
                while (nextTet->adjacentTetrahedron(nextFaceNumber)) {
                    nextFacePerm = nextTet->adjacentGluing(
                        nextFaceNumber) * nextFacePerm * switchPerm;
                    nextTet = nextTet->adjacentTetrahedron(nextFaceNumber);
                    nextFaceNumber = nextFacePerm[followFromFace];
                }
                nextTriangle = nextTet->triangles_[nextFaceNumber];
                // Find the expected orientation of the next triangle.
                yourOrientation =
                    (nextTet->triMapping_[nextFaceNumber].inverse() *
                    nextFacePerm * switchPerm * tet->triMapping_[tetFace])
                    .sign() == 1 ? -tet->tmpOrientation_[tetFace] :
                    tet->tmpOrientation_[tetFace];
                if (nextTriangle->boundaryComponent_) {
                    // Check the orientation.
                    if (yourOrientation !=
                            nextTet->tmpOrientation_[nextFaceNumber])
                        label->orientable_ = false;
                } else {
                    // Add this adjacent triangle to the queue.
                    nextTriangle->boundaryComponent_ = label;
                    label->triangles_.push_back(nextTriangle);
                    nextTet->tmpOrientation_[nextFaceNumber] = yourOrientation;
                    triangleQueue.push(nextTriangle);
                }
            }
    }
}

void NTriangulation::calculateVertexLinks() const {
    // Begin by calculating Euler characteristics.
    // Here we use the formula:  chi = (2 v_int + v_bdry - f) / 2, which
    // is easily proven with a little arithmetic.

    // Note that NVertex::linkEulerChar is initialised to 0 in
    // the NVertex constructor.

    // Begin by calculating (2 v_int + v_bdry) for each vertex link.
    NVertex* end0;
    NVertex* end1;
    NTetrahedron* tet;
    for (NEdge* e : getEdges()) {
        // Try to compute e->getVertex(0) and e->getVertex(1), but
        // without calling e->getVertex() which will recursively try to
        // recompute the skeleton.
        const NEdgeEmbedding& emb = e->front();
        tet = emb.getTetrahedron();
        end0 = tet->vertices_[tet->edgeMapping_[emb.getEdge()][0]];
        end1 = tet->vertices_[tet->edgeMapping_[emb.getEdge()][1]];

        if (e->isBoundary()) {
            // Contribute to v_bdry.
            end0->linkEulerChar_++;
            if (e->valid_)
                end1->linkEulerChar_++;
        } else {
            // Contribute to 2 v_int.
            end0->linkEulerChar_ += 2;
            if (e->valid_)
                end1->linkEulerChar_ += 2;
        }
    }

    // Run through each vertex and finalise Euler characteristic, link
    // and more.

    for (NVertex* vertex : getVertices()) {
        // Fix the Euler characteristic (subtract f, divide by two).
        vertex->linkEulerChar_ = (vertex->linkEulerChar_
            - static_cast<long>(vertex->getDegree())) / 2;

        if (vertex->isBoundary()) {
            // We haven't added ideal vertices to the boundary list yet,
            // so this must be real boundary.
            if (vertex->linkEulerChar_ == 1)
                vertex->link_ = NVertex::DISC;
            else {
                vertex->link_ = NVertex::NON_STANDARD_BDRY;
                valid_ = false;
                standard_ = false;
            }
        } else {
            if (vertex->linkEulerChar_ == 2)
                vertex->link_ = NVertex::SPHERE;
            else {
                if (vertex->linkEulerChar_ == 0)
                    vertex->link_ = (vertex->isLinkOrientable() ?
                        NVertex::TORUS : NVertex::KLEIN_BOTTLE);
                else {
                    vertex->link_ = NVertex::NON_STANDARD_CUSP;
                    standard_ = false;
                }

                ideal_ = true;
                vertex->getComponent()->ideal_ = true;

                NBoundaryComponent* bc = new NBoundaryComponent(vertex);
                bc->orientable_ = vertex->isLinkOrientable();
                vertex->boundaryComponent_ = bc;
                boundaryComponents_.push_back(bc);
                vertex->getComponent()->boundaryComponents_.push_back(bc);
            }
        }
    }
}

void NTriangulation::calculateBoundaryProperties() const {
    // Make sure the skeleton has been calculated!
    ensureSkeleton();

    bool localTwoSphereBoundaryComponents = false;
    bool localNegativeIdealBoundaryComponents = false;

    for (BoundaryComponentIterator it = boundaryComponents_.begin();
            it != boundaryComponents_.end(); it++) {
        if ((*it)->getEulerChar() == 2)
            localTwoSphereBoundaryComponents = true;
        else if ((*it)->isIdeal() && (*it)->getEulerChar() < 0)
            localNegativeIdealBoundaryComponents = true;

        // Stop the search if we've found everything we're looking for.
        if (localTwoSphereBoundaryComponents &&
                localNegativeIdealBoundaryComponents)
            break;
    }

    twoSphereBoundaryComponents_ = localTwoSphereBoundaryComponents;
    negativeIdealBoundaryComponents_ = localNegativeIdealBoundaryComponents;
}

} // namespace regina
