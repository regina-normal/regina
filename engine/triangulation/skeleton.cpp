
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

#include <queue>
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"

#include <iostream>

namespace regina {

void NTriangulation::calculateSkeleton() {
    TriangulationBase<3>::calculateSkeleton();

    ideal_ = false;
    standard_ = true;

#if 0
    checkPermutations();
        // Sets valid to false if gluings are mismatched (which should
        // never happen if the NTetrahedron gluing routines have been
        // used correctly)
#endif

    calculateBoundary();
        // Sets boundaryComponents, NTriangle.boundaryComponent,
        //     NEdge.boundaryComponent, NVertex.boundaryComponent,
        //     NComponent.boundaryComponents
    calculateVertexLinks();
        // Sets valid, ideal, NVertex.link,
        //     NVertex.linkEulerChar, NComponent.ideal,
        //     boundaryComponents, NVertex.boundaryComponent

    // Flesh out the details of each component.
    for (auto v : vertices())
        v->component()->vertices_.push_back(v);
    for (auto e : edges())
        e->component()->edges_.push_back(e);
    for (auto t : triangles())
        t->component()->triangles_.push_back(t);
}

void NTriangulation::checkPermutations() {
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

void NTriangulation::calculateBoundary() {
    // Sets boundaryComponents, NTriangle.boundaryComponent,
    //     NEdge.boundaryComponent, NVertex.boundaryComponent,
    //     NComponent.boundaryComponents
    NBoundaryComponent* label;

    for (NTriangle* triangle : triangles()) {
        if (triangle->degree() < 2)
            if (triangle->boundaryComponent_ == 0) {
                label = new NBoundaryComponent();
                label->orientable_ = true;
                labelBoundaryTriangle(triangle, label);
                boundaryComponents_.push_back(label);
                triangle->component()->boundaryComponents_.push_back(label);
            }
    }
}

void NTriangulation::labelBoundaryTriangle(NTriangle* firstTriangle,
        NBoundaryComponent* label) {
    std::queue<NTriangle*> triangleQueue;

    const NTriangleEmbedding& emb = firstTriangle->front();
    firstTriangle->boundaryComponent_ = label;
    label->triangles_.push_back(firstTriangle);
    emb.tetrahedron()->tmpOrientation_[emb.triangle()] = 1;
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
        tet = triangle->front().tetrahedron();
        tetFace = triangle->front().triangle();
        tetVertices = tet->regina::detail::SimplexFaces<3, 2>::mapping_[tetFace];

        // Run through the vertices.
        for (i=0; i<3; i++) {
            vertex = tet->regina::detail::SimplexFaces<3, 0>::face_[tetVertices[i]];
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
                edge = tet->regina::detail::SimplexFaces<3, 1>::face_[
                    NEdge::edgeNumber[tetVertices[i]][tetVertices[j]]];
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
                nextTriangle =
                    nextTet->regina::detail::SimplexFaces<3, 2>::face_[nextFaceNumber];
                // Find the expected orientation of the next triangle.
                yourOrientation =
                    (nextTet->regina::detail::SimplexFaces<3, 2>::mapping_[nextFaceNumber].
                        inverse() * nextFacePerm * switchPerm *
                        tet->regina::detail::SimplexFaces<3, 2>::mapping_[tetFace])
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

void NTriangulation::calculateVertexLinks() {
    // Begin by calculating Euler characteristics.
    // Here we use the formula:  chi = (2 v_int + v_bdry - f) / 2, which
    // is easily proven with a little arithmetic.

    // Note that NVertex::linkEulerChar is initialised to 0 in
    // the NVertex constructor.

    // Begin by calculating (2 v_int + v_bdry) for each vertex link.
    NVertex* end0;
    NVertex* end1;
    NTetrahedron* tet;
    for (NEdge* e : edges()) {
        // Try to compute e->vertex(0) and e->vertex(1), but
        // without calling e->vertex() which will recursively try to
        // recompute the skeleton.
        const NEdgeEmbedding& emb = e->front();
        tet = emb.tetrahedron();
        end0 = tet->regina::detail::SimplexFaces<3, 0>::face_[tet->regina::detail::SimplexFaces<3, 1>::mapping_
            [emb.edge()][0]];
        end1 = tet->regina::detail::SimplexFaces<3, 0>::face_[tet->regina::detail::SimplexFaces<3, 1>::mapping_
            [emb.edge()][1]];

        if (e->isBoundary()) {
            // Contribute to v_bdry.
            end0->linkEulerChar_++;
            if (e->isValid())
                end1->linkEulerChar_++;
        } else {
            // Contribute to 2 v_int.
            end0->linkEulerChar_ += 2;
            if (e->isValid())
                end1->linkEulerChar_ += 2;
        }
    }

    // Run through each vertex and finalise Euler characteristic, link
    // and more.

    for (NVertex* vertex : vertices()) {
        // Fix the Euler characteristic (subtract f, divide by two).
        vertex->linkEulerChar_ = (vertex->linkEulerChar_
            - static_cast<long>(vertex->degree())) / 2;

        if (vertex->isBoundary()) {
            // We haven't added ideal vertices to the boundary list yet,
            // so this must be real boundary.
            if (vertex->linkEulerChar_ == 1)
                vertex->link_ = NVertex::DISC;
            else {
                vertex->link_ = NVertex::NON_STANDARD_BDRY;
                vertex->markBadLink();
                valid_ = vertex->component_->valid_ = false;
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
                vertex->component()->ideal_ = true;

                NBoundaryComponent* bc = new NBoundaryComponent(vertex);
                bc->orientable_ = vertex->isLinkOrientable();
                vertex->boundaryComponent_ = bc;
                boundaryComponents_.push_back(bc);
                vertex->component()->boundaryComponents_.push_back(bc);
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
        if ((*it)->eulerChar() == 2)
            localTwoSphereBoundaryComponents = true;
        else if ((*it)->isIdeal() && (*it)->eulerChar() < 0)
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
