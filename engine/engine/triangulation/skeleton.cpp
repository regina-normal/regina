
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntriangulation.h"
    #include "nqueue.h"
    #include "nrational.h"
#else
    #include "engine/triangulation/ntriangulation.h"
    #include "engine/utilities/nqueue.h"
    #include "engine/utilities/nrational.h"
#endif

void NTriangulation::calculateSkeleton() {
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

void NTriangulation::calculateComponents() {
    TetrahedronIterator it(tetrahedra);
    NComponent* label;
    NTetrahedron* tet;
    while (!it.done()) {
        (*it)->component = 0;
        it++;
    }

    it.init(tetrahedra);
    while (!it.done()) {
        tet = *it;
        if (tet->component == 0) {
            label = new NComponent();
            labelComponent(tet, label, 1);
            components.addLast(label);
        }
        it++;
    }
}

void NTriangulation::labelComponent(NTetrahedron* firstTet,
        NComponent* component, int firstOrientation) {
    // Now non-recursive; uses a queue instead.
    // The queue contains tetrahedra from which we need to propagate
    //     component labelling.
    NQueue<NTetrahedron*> queue;
    
    firstTet->component = component;
    component->tetrahedra.addLast(firstTet);
    firstTet->orientation = firstOrientation;
    queue.insert(firstTet);
    
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    int yourOrientation;
    while (! queue.empty()) {
        tet = queue.remove();
        
        for (int face=0; face<4; face++) {
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
                    component->tetrahedra.addLast(adjTet);
                    adjTet->orientation = yourOrientation;
                    queue.insert(adjTet);
                }
            }
        }
    }
}

void NTriangulation::calculateVertices() {
    TetrahedronIterator it(tetrahedra);
    int vertex;
    NTetrahedron* tet;
    NVertex* label;
    while (!it.done()) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            tet->vertices[vertex] = 0;
        it++;
    }

    it.init(tetrahedra);
    while (!it.done()) {
        tet = *it;
        for (vertex=0; vertex<4; vertex++)
            if (! tet->getVertex(vertex)) {
                label = new NVertex(tet->component);
                tet->component->vertices.addLast(label);
                labelVertex(tet, vertex, label, 1);
                vertices.addLast(label);
            }
        it++;
    }
}

struct VertexState {
    NTetrahedron* tet;
    int vertex;
    
    VertexState(NTetrahedron* newTet, int newVertex) :
            tet(newTet), vertex(newVertex) {}
};

void NTriangulation::labelVertex(NTetrahedron* firstTet, int firstVertex,
        NVertex* label, int firstOrientation) {
    NQueue<VertexState*> queue;
    
    firstTet->vertices[firstVertex] = label;
    firstTet->tmpOrientation[firstVertex] = firstOrientation;
    label->embeddings.addLast(NVertexEmbedding(firstTet, firstVertex));
    queue.insert(new VertexState(firstTet, firstVertex));

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

    while (! queue.empty()) {
        current = queue.remove();
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
                    label->embeddings.addLast(NVertexEmbedding(altTet,
                        yourVertex));
                    queue.insert(new VertexState(altTet, yourVertex));
                }
            }
        }
    }
}

void NTriangulation::calculateEdges() {
    TetrahedronIterator it(tetrahedra);
    int edge;
    NTetrahedron* tet;
    NEdge* label;
    while (!it.done()) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            tet->edges[edge] = 0;
        it++;
    }

    it.init(tetrahedra);
    while (!it.done()) {
        tet = *it;
        for (edge=0; edge<6; edge++)
            if (! tet->getEdge(edge)) {
                label = new NEdge(tet->component);
                tet->component->edges.addLast(label);
                labelEdge(tet, edge, label, edgeOrdering(edge));
                edges.addLast(label);
            }
        it++;
    }
}

struct EdgeState {
    NTetrahedron* tet;
    int edge;
    int whichEndOfList;
    
    EdgeState(NTetrahedron* newTet, int newEdge, int newEnd) :
            tet(newTet), edge(newEdge), whichEndOfList(newEnd) {}
};

void NTriangulation::labelEdge(NTetrahedron* firstTet, int firstEdge,
        NEdge* label, const NPerm& firstTetVertices) {
    NQueue<EdgeState*> queue;
    
    firstTet->edges[firstEdge] = label;
    firstTet->edgeMapping[firstEdge] = firstTetVertices;
    label->embeddings.addLast(NEdgeEmbedding(firstTet, firstEdge));
    queue.insert(new EdgeState(firstTet, firstEdge, 0));

    EdgeState* current;
    NTetrahedron* tet;
    NTetrahedron* altTet;
    NPerm perm;
    int edge;
    int yourEdge;
    NPerm tetVertices;
    int whichEndOfList;
    int face;
    while (! queue.empty()) {
        current = queue.remove();
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
                            label->embeddings.addLast(
                                NEdgeEmbedding(altTet, yourEdge));
                        else
                            label->embeddings.addFirst(
                                NEdgeEmbedding(altTet, yourEdge));
                        queue.insert(new EdgeState(altTet, yourEdge,
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

void NTriangulation::calculateFaces() {
    TetrahedronIterator it(tetrahedra);
    int face;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NFace* label;
    NPerm adjVertices;
    int adjFace;
    while (!it.done()) {
        tet = *it;
        for (face=0; face<4; face++)
            tet->faces[face] = 0;
        it++;
    }

    it.init(tetrahedra);
    while (!it.done()) {
        tet = *it;
        for (face=3; face>=0; face--)
            if (! tet->getFace(face)) {
                label = new NFace(tet->component);
                tet->component->faces.addLast(label);
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
                faces.addLast(label);
            }
        it++;
    }
}

void NTriangulation::calculateBoundary() {
    // Sets boundaryComponents, NFace.boundaryComponent,
    //     NEdge.boundaryComponent, NVertex.boundaryComponent,
    //     NComponent.boundaryComponents
    FaceIterator it(faces);
    NFace* face;
    NBoundaryComponent* label;

    while (! it.done()) {
        face = *it;
        if (face->nEmbeddings < 2)
            if (face->boundaryComponent == 0) {
                label = new NBoundaryComponent();
                label->orientable = true;
                labelBoundaryFace(face, label, 1);
                boundaryComponents.addLast(label);
                face->component->boundaryComponents.addLast(label);
            }
        it++;
    }
}

void NTriangulation::labelBoundaryFace(NFace* firstFace,
        NBoundaryComponent* label, int firstOrientation) {
    NQueue<NFace*> queue;
    NFaceEmbedding* emb;
    
    emb = firstFace->embeddings[0];
    firstFace->boundaryComponent = label;
    label->faces.addLast(firstFace);
    emb->getTetrahedron()->tmpOrientation[emb->getFace()] = firstOrientation;
    queue.insert(firstFace);

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

    while (! queue.empty()) {
        face = queue.remove();
        
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
                label->vertices.addLast(vertex);
            }
        }
        
        // Run through the edges.
        for (i=0; i<3; i++)
            for (j=i+1; j<3; j++) {
                edge = tet->getEdge(edgeNumber[tetVertices[i]][tetVertices[j]]);
                if (! (edge->boundaryComponent)) {
                    edge->boundaryComponent = label;
                    label->edges.addLast(edge);
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
                    label->faces.addLast(nextFace);
                    nextTet->tmpOrientation[nextFaceNumber] = yourOrientation;
                    queue.insert(nextFace);
                }
            }
    }
}

void NTriangulation::calculateVertexLinks() {
    // Runs through each vertex and sets link accordingly.
    VertexIterator it(vertices);
    NVertex* vertex;
    NDynamicArrayIterator<NVertexEmbedding> embit;
    NTetrahedron* tet;
    int tetVertex, secondVertex;

    NRational v;
    long f, twiceE;
    while (!it.done()) {
        vertex = *it;

        // Calculate number of faces, edges and vertices in the link.
        f = vertex->getEmbeddings().size();
        twiceE = 3 * f;
        v = 0;
        
        embit.init(vertex->getEmbeddings());
        while (! embit.done()) {
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
                boundaryComponents.addLast(bc);
                vertex->component->boundaryComponents.addLast(bc);
            }
        }

        it++;
    }
}
