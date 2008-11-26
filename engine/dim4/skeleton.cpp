
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

#include <algorithm>
#include <queue>
#include <utility>
#include "dim4/dim4triangulation.h"
#include "triangulation/ntriangulation.h"

namespace regina {

void Dim4Triangulation::calculateSkeleton() const {
    // Triangulations are valid, orientable and non-ideal until proven
    // otherwise.
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
        // - tetrahedra_
        // - orientable_
        // - Dim4Component::pentachora_
        // - Dim4Component::tetrahedra_
        // - Dim4Component::orientable_
        // - Dim4Pentachoron::component_
        // - Dim4Pentachoron::orientation_
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
        // - all Dim4BoundaryComponent members

    calculateVertexLinks(); 
        // Sets:
        // - Dim4Vertex::link_
        // - valid_ and Dim4Vertex::valid_ in the case of bad vertex links
        // - valid_ and Dim4Edge::invalid_ in the case of bad edge links
        // - ideal_, Dim4Vertex::ideal_ and Dim4Component::ideal_

    calculatedSkeleton_ = true;
}

void Dim4Triangulation::calculateComponents() const {
    PentachoronIterator it;
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        (*it)->component_ = 0;
        std::fill((*it)->tet_, (*it)->tet_ + 5,
            static_cast<Dim4Tetrahedron*>(0));
    }

    Dim4Component* label;
    Dim4Pentachoron** stack = new Dim4Pentachoron*[pentachora_.size()];
    unsigned stackSize = 0;
    Dim4Pentachoron *loopPent, *pent, *adjPent;
    Dim4Tetrahedron* tet;
    int facet, adjFacet, adjOrientation;
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
                // Have we already checked out this facet from the other side?
                if (pent->tet_[facet])
                    continue;

                // Make a new tetrahedron, but leave the embeddings and
                // mappings until we see which side we're on.  This is so
                // that the tetrahedra *look* as though we enumerated them in
                // lexicographical order (instead of via a depth-first search
                // through each triangulation component).
                tet = new Dim4Tetrahedron(pent->component_);

                adjPent = pent->adjacentPentachoron(facet);
                if (adjPent) {
                    // We have an adjacent tetrahedron.
                    adjFacet = pent->adjacentFacet(facet);

                    pent->tet_[facet] = tet;
                    adjPent->tet_[adjFacet] = tet;

                    // Choose a tetrahedron mapping according to which side
                    // comes "first" lexicographically.  Note that facet 4 is
                    // really facet 0123, and so is "less than" facet 0, which
                    // is really facet 1234.  In short, facets get ordered in
                    // reverse.
                    if (pent->markedIndex() < adjPent->markedIndex() ||
                            (pent->markedIndex() == adjPent->markedIndex() &&
                             facet > adjFacet)) {
                        // pent comes first.
                        pent->tetMapping_[facet] =
                            Dim4Tetrahedron::ordering[facet];
                        adjPent->tetMapping_[adjFacet] =
                            pent->adjacentGluing(facet) *
                            Dim4Tetrahedron::ordering[facet];

                        tet->emb_[0] = Dim4TetrahedronEmbedding(pent, facet);
                        tet->emb_[1] = Dim4TetrahedronEmbedding(
                            adjPent, adjFacet);
                    } else {
                        // adjPent comes first.
                        adjPent->tetMapping_[adjFacet] =
                            Dim4Tetrahedron::ordering[adjFacet];
                        pent->tetMapping_[facet] =
                            adjPent->adjacentGluing(adjFacet) *
                            Dim4Tetrahedron::ordering[adjFacet];

                        tet->emb_[0] = Dim4TetrahedronEmbedding(
                            adjPent, adjFacet);
                        tet->emb_[1] = Dim4TetrahedronEmbedding(pent, facet);
                    }
                    tet->nEmb_ = 2;

                    // Deal with orientations and connected components.
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
                } else {
                    // This is a boundary tetrahedron.
                    pent->tet_[facet] = tet;
                    pent->tetMapping_[facet] = Dim4Tetrahedron::ordering[facet];

                    tet->emb_[0] = Dim4TetrahedronEmbedding(pent, facet);
                    tet->nEmb_ = 1;
                }
            }
        }
    }

    // Now run through again and number the tetrahedra (i.e., insert them
    // into the main list) in lexicographical order.  Again, facets are
    // ordered in reverse.
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        pent = *it;
        for (facet = 4; facet >= 0; --facet) {
            tet = pent->tet_[facet];
            if (tet->nEmb_ == 2 &&
                    (tet->emb_[0].getPentachoron() != pent ||
                     tet->emb_[0].getTetrahedron() != facet))
                continue;

            tetrahedra_.push_back(tet);
            pent->component_->tetrahedra_.push_back(tet);
        }
    }

    delete[] stack;
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
        for (loopFace = 9; loopFace >= 0; --loopFace) {
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
    Dim4Tetrahedron** stack = new Dim4Tetrahedron*[nBdry];
    unsigned stackSize = 0;
    Dim4Pentachoron *pent, *adjPent;
    int facet, adjFacet;
    Dim4Vertex* vertex;
    Dim4Edge* edge;
    Dim4Face* face;
    Dim4FaceEmbedding faceEmb;
    Dim4Tetrahedron *tet, *adjTet;
    int tetFace, adjTetFace;
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

        // Run a depth-first search from this boundary tetrahedron to
        // completely enumerate all tetrahedra in this boundary component.
        loopTet->boundaryComponent_ = label;
        label->tetrahedra_.push_back(loopTet);

        stack[0] = loopTet;
        stackSize = 1;

        while (stackSize > 0) {
            tet = stack[--stackSize];
            pent = tet->emb_[0].getPentachoron();
            facet = tet->emb_[0].getTetrahedron();

            bdryTetAll[tet->markedIndex()] = bdryTet = new NTetrahedron();

            // Run through the vertices and edges on this tetrahedron.
            for (i = 0; i < 5; ++i)
                if (i != facet) {
                    vertex = pent->vertex_[i];
                    if (vertex->boundaryComponent_ != label) {
                        // Note that a vertex in an invalid
                        // triangulation might end up in more than one
                        // boundary component.  Push it into all of the
                        // relevant boundary components' lists.
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
                    if (edge->boundaryComponent_ != label) {
                        // Likewise, an edge in an invalid triangulation
                        // might end up in more than one boundary component.
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

                // Examine the face opposite vertices (i, facet).  This is
                // the face opposite the edge joining vertices (i, facet).
                face = pent->face_[Dim4Edge::edgeNumber[i][facet]];
                if (! face->boundaryComponent_) {
                    face->boundaryComponent_ = label;
                    label->faces_.push_back(face);
                }

                // Okay, we can be clever about this.  The current
                // boundary tetrahedron is one end of the face link; the
                // *adjacent* boundary tetrahedron must be at the other.
                faceEmb = face->emb_.front();
                if (faceEmb.getPentachoron() == pent &&
                        faceEmb.getVertices()[3] == i &&
                        faceEmb.getVertices()[4] == facet) {
                    // We are currently looking at the embedding at the
                    // front of the list.  Take the one at the back.
                    faceEmb = face->emb_.back();

                    adjPent = faceEmb.getPentachoron();
                    adjFacet = faceEmb.getVertices()[3];
                    adjTet = adjPent->tet_[adjFacet];
                    j = faceEmb.getVertices()[4];
                } else {
                    // We must be looking at the embedding at the back
                    // of the list.  Take the one at the front (which is
                    // already stored in faceEmb).
                    adjPent = faceEmb.getPentachoron();
                    adjFacet = faceEmb.getVertices()[4];
                    adjTet = adjPent->tet_[adjFacet];
                    j = faceEmb.getVertices()[3];

                    // TODO: Sanity checking; remove this eventually.
                    faceEmb = face->emb_.back();
                    if (! (faceEmb.getPentachoron() == pent &&
                            faceEmb.getVertices()[4] == i &&
                            faceEmb.getVertices()[3] == facet)) {
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
                        tetFace = pent->tetMapping_[facet].preImageOf(i);
                        adjTetFace = adjPent->tetMapping_[adjFacet].
                            preImageOf(j);

                        bdryTet->joinTo(tetFace, adjBdryTet,
                            (adjTet->getFaceMapping(adjTetFace) *
                             tet->getFaceMapping(tetFace).inverse()).asPerm4());
                    }
                }

                // Push the adjacent tetrahedron onto the stack for
                // processing.
                if (! adjTet->boundaryComponent_) {
                    adjTet->boundaryComponent_ = label;
                    label->tetrahedra_.push_back(adjTet);
                    stack[stackSize++] = adjTet;
                }
            }

            label->boundary_->addTetrahedron(bdryTetAll[tet->markedIndex()]);
        }
    }

    delete[] bdryTetAll;
}

void Dim4Triangulation::calculateVertexLinks() const {
    long n = pentachora_.size();

    NTetrahedron** tet = new NTetrahedron*[5 * n]; // Pieces of vertex link.
    long index = 0; // Index into the tet[] array.

    for (index = 0; index < 5 * n; ++index)
        tet[index] = new NTetrahedron();

    Dim4Pentachoron *pent, *adjPent;
    long pentIdx, adjPentIdx;
    int vertexIdx, adjVertexIdx;
    int exitFacet, adjFacet;

    index = 0;
    for (pentIdx = 0; pentIdx < n; ++pentIdx) {
        pent = pentachora_[pentIdx];
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
                    (adjPent->tetMapping_[adjVertexIdx].inverse() *
                        pent->adjacentGluing(exitFacet) *
                        pent->tetMapping_[vertexIdx]).asPerm4());
            }
            ++index;
        }
    }

    // Have each vertex claim its own pieces of vertex link, and in the
    // correct order as described by the Dim4Vertex::getLink() docs.
    Dim4Vertex* vertex;
    VertexIterator vit;
    std::vector<Dim4VertexEmbedding>::const_iterator embit;
    for (vit = vertices_.begin(); vit != vertices_.end(); ++vit) {
        vertex = *vit;

        // Build a 3-manifold triangulation out of all the little pieces of
        // vertex link.
        vertex->link_ = new NTriangulation();
        for (embit = vertex->emb_.begin(); embit != vertex->emb_.end();
                ++embit)
            vertex->link_->addTetrahedron(tet[
                5 * embit->getPentachoron()->markedIndex() +
                embit->getVertex()]);

        // Look at the vertex link and see what it says about this 4-manifold
        // triangulation.
        if (vertex->link_->hasBoundaryFaces()) {
            // It's a 3-ball or nothing.
            if (! vertex->link_->isBall()) {
                valid_ = vertex->valid_ = false;
                // The vertex belongs to some pentachoron with boundary
                // tetrahedra, and so already belongs to a boundary component.
            }
        } else {
            // The vertex link has no boundary faces, which means this
            // vertex is not part of any boundary tetrahedra.
            // Let's see what we've got.
            if ((! vertex->link_->isValid()) || vertex->link_->isIdeal()) {
                // Bapow.
                valid_ = vertex->valid_ = false;
                boundaryComponents_.push_back(
                    vertex->boundaryComponent_ =
                    new Dim4BoundaryComponent(vertex));
            } else if (! vertex->link_->isThreeSphere()) {
                // The vertex is fine but it's not a 3-sphere.
                // We have an ideal triangulation.
                ideal_ = vertex->component_->ideal_ = vertex->ideal_ = true;
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
}

} // namespace regina
