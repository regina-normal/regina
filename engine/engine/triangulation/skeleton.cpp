
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <queue>
#include "triangulation/ntriangulation.h"
#include "utilities/nrational.h"

namespace regina {

void NTriangulation::calculateSkeleton() const {
    ideal = false;
    valid = true;
    orientable = true;
    standard = true;

    calculateComponents();
        // Sets components, orientable, NComponent.orientable,
        //     NTetrahedron.component
    calculateFaces();
        // Sets faces, NFace.component
    calculateVertices();
        // Sets vertices, NVertex.component
    calculateEdges();
        // Sets edges, NEdge.component, valid, NEdge.valid
    calculateBoundary();
        // Sets boundaryComponents, NFace.boundaryComponent,
        //     NEdge.boundaryComponent, NVertex.boundaryComponent,
        //     NComponent.boundaryComponents
    calculateVertexLinks();
        // Sets valid, ideal, NVertex.link, NVertex.linkOrientable,
        //     NVertex.linkEulerCharacteristic, NComponent.ideal,
        //     boundaryComponents, NVertex.boundaryComponent

    calculatedSkeleton = true;
}

void NTriangulation::calculateComponents() const {
    TetrahedronIterator it;
    NComponent* label;
    NTetrahedron* tet;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++)
        (*it)->component = 0;

    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        if (tet->component == 0) {
            label = new NComponent();
            labelComponent(tet, label, 1);
            components.push_back(label);
        }
    }
}

void NTriangulation::labelComponent(NTetrahedron* firstTet,
        NComponent* component, int firstOrientation) const {
    // Now non-recursive; uses a queue instead.
    // The queue contains tetrahedra from which we need to propagate
    //     component labelling.
    std::queue<NTetrahedron*> tetQueue;

    firstTet->component = component;
    component->tetrahedra.push_back(firstTet);
    firstTet->orientation = firstOrientation;
    tetQueue.push(firstTet);

    NTetrahedron* tet;
    NTetrahedron* adjTet;
    int face;
    int yourOrientation;
    while (! tetQueue.empty()) {
        tet = tetQueue.front();
        tetQueue.pop();

        for (face=0; face<4; face++) {
            adjTet = tet->getAdjacentTetrahedron(face);
            if (adjTet) {
                yourOrientation = (tet->getAdjacentTetrahedronGluing(face).
                    sign() == 1 ? -tet->orientation : tet->orientation);
                if (adjTet->component) {
                    if (yourOrientation != adjTet->orientation) {
                        orientable = false;
                        component->orientable = false;
                    }
                } else {
                    adjTet->component = component;
                    component->tetrahedra.push_back(adjTet);
                    adjTet->orientation = yourOrientation;
                    tetQueue.push(adjTet);
                }
            }
        }
    }
}

void NTriangulation::calculateVertices() const {
    TetrahedronIterator it;
    int vertex;
    NTetrahedron* tet;
    NVertex* label;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            tet->vertices[vertex] = 0;
    }

    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            if (! tet->getVertex(vertex)) {
                label = new NVertex(tet->component);
                tet->component->vertices.push_back(label);
                labelVertex(tet, vertex, label, 1);
                vertices.push_back(label);
            }
    }
}

namespace {
    struct VertexState {
        NTetrahedron* tet;
        int vertex;

        VertexState(NTetrahedron* newTet, int newVertex) :
                tet(newTet), vertex(newVertex) {}
    };
}

void NTriangulation::labelVertex(NTetrahedron* firstTet, int firstVertex,
        NVertex* label, int firstOrientation) {
    std::queue<VertexState*> vtxQueue;

    firstTet->vertices[firstVertex] = label;
    firstTet->tmpOrientation[firstVertex] = firstOrientation;
    label->embeddings.push_back(NVertexEmbedding(firstTet, firstVertex));
    vtxQueue.push(new VertexState(firstTet, firstVertex));

    VertexState* current;
    NTetrahedron* tet;
    NTetrahedron* altTet;
    int vertex;
    int yourVertex;
    int yourOrientation;
    int yourFace;
    int face;
    NPerm myFaceOrientation;
    NPerm yourFaceOrientation;
    NPerm faceCycle(1,2,0,3);

    while (! vtxQueue.empty()) {
        current = vtxQueue.front();
        vtxQueue.pop();
        tet = current->tet;
        vertex = current->vertex;
        delete current;

        for (face=0; face<4; face++) {
            if (face == vertex) continue;
            altTet = tet->getAdjacentTetrahedron(face);
            if (altTet) {
                yourVertex = tet->getAdjacentTetrahedronGluing(face)[vertex];
                yourFace = tet->getAdjacentFace(face);
                myFaceOrientation = faceOrdering(vertex);
                while (myFaceOrientation[2] != face)
                    myFaceOrientation = myFaceOrientation * faceCycle;
                yourFaceOrientation = faceOrdering(yourVertex);
                while (yourFaceOrientation[2] != yourFace)
                    yourFaceOrientation = yourFaceOrientation * faceCycle;
                if (tet->getAdjacentTetrahedronGluing(face)[
                        myFaceOrientation[0]] == yourFaceOrientation[0])
                    yourOrientation = -(tet->tmpOrientation[vertex]);
                else
                    yourOrientation = tet->tmpOrientation[vertex];
                if (altTet->getVertex(yourVertex)) {
                    if (altTet->tmpOrientation[yourVertex] !=
                            yourOrientation)
                        label->linkOrientable = false;
                } else {
                    altTet->vertices[yourVertex] = label;
                    altTet->tmpOrientation[yourVertex] = yourOrientation;
                    label->embeddings.push_back(NVertexEmbedding(altTet,
                        yourVertex));
                    vtxQueue.push(new VertexState(altTet, yourVertex));
                }
            }
        }
    }
}

void NTriangulation::calculateEdges() const {
    TetrahedronIterator it;
    int edge;
    NTetrahedron* tet;
    NEdge* label;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            tet->edges[edge] = 0;
    }

    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            if (! tet->getEdge(edge)) {
                label = new NEdge(tet->component);
                tet->component->edges.push_back(label);
                labelEdge(tet, edge, label, edgeOrdering(edge));
                edges.push_back(label);
            }
    }
}

namespace {
    struct EdgeState {
        NTetrahedron* tet;
        int edge;
        int whichEndOfList;

        EdgeState(NTetrahedron* newTet, int newEdge, int newEnd) :
                tet(newTet), edge(newEdge), whichEndOfList(newEnd) {}
    };
}

void NTriangulation::labelEdge(NTetrahedron* firstTet, int firstEdge,
        NEdge* label, const NPerm& firstTetVertices) const {
    std::queue<EdgeState*> edgeQueue;

    firstTet->edges[firstEdge] = label;
    firstTet->edgeMapping[firstEdge] = firstTetVertices;
    label->embeddings.push_back(NEdgeEmbedding(firstTet, firstEdge));
    edgeQueue.push(new EdgeState(firstTet, firstEdge, 0));

    EdgeState* current;
    NTetrahedron* tet;
    NTetrahedron* altTet;
    NPerm perm;
    int edge;
    int yourEdge;
    NPerm tetVertices;
    int whichEndOfList;
    int face;
    while (! edgeQueue.empty()) {
        current = edgeQueue.front();
        edgeQueue.pop();
        tet = current->tet;
        edge = current->edge;
        whichEndOfList = current->whichEndOfList;
        tetVertices = tet->edgeMapping[edge];
        delete current;

        for (face=0; face<4; face++) {
            if (face != edgeStart[edge] && face != edgeEnd[edge]) {
                altTet = tet->getAdjacentTetrahedron(face);
                if (altTet) {
                    perm = tet->getAdjacentTetrahedronGluing(face);
                    yourEdge =
                        edgeNumber[perm[edgeStart[edge]]][perm[edgeEnd[edge]]];
                    if (altTet->getEdge(yourEdge)) {
                        // We need to check we're not labelling the edge in
                        // reverse.
                        if (((altTet->getEdgeMapping(yourEdge).inverse())
                            * perm * tetVertices)[0] != 0) {
                                // The edge is being labelled in reverse!
                                label->valid = false;
                                valid = false;
                            }
                    } else {
                        // We need to label this new tetrahedron's edge.
                        if (whichEndOfList == 0) {
                            if (tetVertices[2] == face)
                                whichEndOfList = 1;
                            else
                                whichEndOfList = -1;
                        }
                        altTet->edges[yourEdge] = label;
                        altTet->edgeMapping[yourEdge] =
                            perm * tetVertices * NPerm(2,3);
                        if (whichEndOfList >= 0)
                            label->embeddings.push_back(
                                NEdgeEmbedding(altTet, yourEdge));
                        else
                            label->embeddings.push_front(
                                NEdgeEmbedding(altTet, yourEdge));
                        edgeQueue.push(new EdgeState(altTet, yourEdge,
                            whichEndOfList));
                        // If more than one adjacent tetrahedron shares this edge,
                        // we will put the other tetrahedron at the other end of
                        // the list in the corresponding NEdge embedding list.
                        // This will ensure that all tetrahedra about a boundary
                        // edge appear in the order in the NEdge embedding list in
                        // which they are glued.
                        //
                        // The NEdge embedding list will be filled as in the
                        // following example:  7-6-5-0-1-2-3-4, where the numbers
                        // indicate in which order the tetrahedra are visited by
                        // labelEdge().
                        whichEndOfList = -whichEndOfList;
                    }
                }
            }
        }
    }
}

void NTriangulation::calculateFaces() const {
    TetrahedronIterator it;
    int face;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NFace* label;
    NPerm adjVertices;
    int adjFace;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++)
            tet->faces[face] = 0;
    }

    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (face=3; face>=0; face--)
            if (! tet->getFace(face)) {
                label = new NFace(tet->component);
                tet->component->faces.push_back(label);
                tet->faces[face] = label;
                tet->faceMapping[face] = faceOrdering(face);
                label->embeddings[0] = new NFaceEmbedding(tet, face);
                label->nEmbeddings = 1;
                adjTet = tet->getAdjacentTetrahedron(face);
                if (adjTet) {
                    // Face is not on the boundary.
                    adjFace = tet->getAdjacentFace(face);
                    adjVertices = (tet->getAdjacentTetrahedronGluing(face))*
                        (label->embeddings[0]->getVertices());
                    adjTet->faces[adjFace] = label;
                    adjTet->faceMapping[adjFace] = adjVertices;
                    label->embeddings[1] = new NFaceEmbedding(adjTet, adjFace);
                    label->nEmbeddings = 2;
                }
                faces.push_back(label);
            }
    }
}

void NTriangulation::calculateBoundary() const {
    // Sets boundaryComponents, NFace.boundaryComponent,
    //     NEdge.boundaryComponent, NVertex.boundaryComponent,
    //     NComponent.boundaryComponents
    FaceIterator it;
    NFace* face;
    NBoundaryComponent* label;

    for (it = faces.begin(); it != faces.end(); it++) {
        face = *it;
        if (face->nEmbeddings < 2)
            if (face->boundaryComponent == 0) {
                label = new NBoundaryComponent();
                label->orientable = true;
                labelBoundaryFace(face, label, 1);
                boundaryComponents.push_back(label);
                face->component->boundaryComponents.push_back(label);
            }
    }
}

void NTriangulation::labelBoundaryFace(NFace* firstFace,
        NBoundaryComponent* label, int firstOrientation) const {
    std::queue<NFace*> faceQueue;
    NFaceEmbedding* emb;

    emb = firstFace->embeddings[0];
    firstFace->boundaryComponent = label;
    label->faces.push_back(firstFace);
    emb->getTetrahedron()->tmpOrientation[emb->getFace()] = firstOrientation;
    faceQueue.push(firstFace);

    NTetrahedron* tet;
    NPerm tetVertices;
    int tetFace;
    int i,j;
    NVertex* vertex;
    NEdge* edge;

    NFace* face;
    NFace* nextFace;
    int nextFaceNumber;
    NPerm nextFacePerm;
    NTetrahedron* nextTet;
    int followFromFace;
    NPerm switchPerm;
    int yourOrientation;

    while (! faceQueue.empty()) {
        face = faceQueue.front();
        faceQueue.pop();

        // Run through the edges and vertices on this face.
        emb = face->embeddings[0];
        tet = emb->getTetrahedron();
        tetVertices = emb->getVertices();
        tetFace = emb->getFace();

        // Run through the vertices.
        for (i=0; i<3; i++) {
            vertex = tet->getVertex(tetVertices[i]);
            if (vertex->boundaryComponent != label) {
                vertex->boundaryComponent = label;
                label->vertices.push_back(vertex);
            }
        }

        // Run through the edges.
        for (i=0; i<3; i++)
            for (j=i+1; j<3; j++) {
                edge = tet->getEdge(edgeNumber[tetVertices[i]][tetVertices[j]]);
                if (! (edge->boundaryComponent)) {
                    edge->boundaryComponent = label;
                    label->edges.push_back(edge);
                }

                // Label the adjacent boundary face with the same label.
                followFromFace = 6 - tetVertices[i] - tetVertices[j] - tetFace;
                switchPerm = NPerm(followFromFace, tetFace);
                nextFaceNumber = followFromFace;
                nextFacePerm = NPerm();
                nextTet = tet;
                while (nextTet->getAdjacentTetrahedron(nextFaceNumber)) {
                    nextFacePerm = nextTet->getAdjacentTetrahedronGluing(
                        nextFaceNumber) * nextFacePerm * switchPerm;
                    nextTet = nextTet->getAdjacentTetrahedron(nextFaceNumber);
                    nextFaceNumber = nextFacePerm[followFromFace];
                }
                nextFace = nextTet->getFace(nextFaceNumber);
                // Find the expected orientation of the next face.
                yourOrientation =
                    (nextTet->getFaceMapping(nextFaceNumber).inverse() *
                    nextFacePerm * switchPerm * tet->getFaceMapping(tetFace))
                    .sign() == 1 ?  -tet->tmpOrientation[tetFace] :
                    tet->tmpOrientation[tetFace];
                if (nextFace->boundaryComponent) {
                    // Check the orientation.
                    if (yourOrientation !=
                            nextTet->tmpOrientation[nextFaceNumber])
                        label->orientable = false;
                } else {
                    // Add this adjacent face to the queue.
                    nextFace->boundaryComponent = label;
                    label->faces.push_back(nextFace);
                    nextTet->tmpOrientation[nextFaceNumber] = yourOrientation;
                    faceQueue.push(nextFace);
                }
            }
    }
}

void NTriangulation::calculateVertexLinks() const {
    // Runs through each vertex and sets link accordingly.
    VertexIterator it;
    NVertex* vertex;
    std::vector<NVertexEmbedding>::const_iterator embit;
    NTetrahedron* tet;
    int tetVertex, secondVertex;

    NRational v;
    long f, twiceE;
    for (it = vertices.begin(); it != vertices.end(); it++) {
        vertex = *it;

        // Calculate number of faces, edges and vertices in the link.
        f = vertex->getEmbeddings().size();
        twiceE = 3 * f;
        v = 0;

        embit = vertex->getEmbeddings().begin();
        while (embit != vertex->getEmbeddings().end()) {
            const NVertexEmbedding& emb = (*embit);
            tet = emb.getTetrahedron();
            tetVertex = emb.getVertex();

            for (secondVertex = 0; secondVertex < 4; secondVertex++) {
                if (secondVertex == tetVertex)
                    continue;
                v += NRational(1, tet->getEdge(edgeNumber[tetVertex]
                    [secondVertex])->getEmbeddings().size());
                if (tet->getFace(secondVertex)->isBoundary())
                    twiceE++;
            }

            embit++;
        }

        // Find Euler characteristic.
        vertex->linkEulerCharacteristic = f - (twiceE / 2) +
            v.getNumerator().longValue();

        if (vertex->isBoundary()) {
            // We haven't added ideal vertices to the boundary list yet,
            // so this must be real boundary.
            if (vertex->linkEulerCharacteristic == 1)
                vertex->link = NVertex::DISC;
            else {
                vertex->link = NVertex::NON_STANDARD_BDRY;
                valid = false;
                standard = false;
            }
        } else {
            if (vertex->linkEulerCharacteristic == 2)
                vertex->link = NVertex::SPHERE;
            else {
                if (vertex->linkEulerCharacteristic == 0)
                    vertex->link = (vertex->isLinkOrientable() ?
                        NVertex::TORUS : NVertex::KLEIN_BOTTLE);
                else {
                    vertex->link = NVertex::NON_STANDARD_CUSP;
                    standard = false;
                }

                ideal = true;
                vertex->component->ideal = true;

                NBoundaryComponent* bc = new NBoundaryComponent(vertex);
                bc->orientable = vertex->isLinkOrientable();
                vertex->boundaryComponent = bc;
                boundaryComponents.push_back(bc);
                vertex->component->boundaryComponents.push_back(bc);
            }
        }
    }
}

void NTriangulation::calculateBoundaryProperties() const {
    // Make sure the skeleton has been calculated!
    if (! calculatedSkeleton)
        calculateSkeleton();

    bool localTwoSphereBoundaryComponents = false;
    bool localNegativeIdealBoundaryComponents = false;

    for (BoundaryComponentIterator it = boundaryComponents.begin();
            it != boundaryComponents.end(); it++) {
        if ((*it)->getEulerCharacteristic() == 2)
            localTwoSphereBoundaryComponents = true;
        else if ((*it)->isIdeal() && (*it)->getEulerCharacteristic() < 0)
            localNegativeIdealBoundaryComponents = true;

        // Stop the search if we've found everything we're looking for.
        if (localTwoSphereBoundaryComponents &&
                localNegativeIdealBoundaryComponents)
            break;
    }

    twoSphereBoundaryComponents = localTwoSphereBoundaryComponents;
    negativeIdealBoundaryComponents = localNegativeIdealBoundaryComponents;
}

} // namespace regina
