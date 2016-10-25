
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <algorithm>
#include <queue>
#include <utility>
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

namespace regina {

void Triangulation<4>::calculateSkeleton() {
    TriangulationBase<4>::calculateSkeleton();

    // Triangulations are valid and non-ideal until proven otherwise.
    ideal_ = false;

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one pentachoron.
    if (simplices_.empty())
        return;

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

    // Flesh out the details of each component.
    for (auto v : vertices())
        v->component()->vertices_.push_back(v);
    for (auto e : edges())
        e->component()->edges_.push_back(e);
    for (auto t : triangles())
        t->component()->triangles_.push_back(t);
    for (auto t : tetrahedra())
        t->component()->tetrahedra_.push_back(t);
}

void Triangulation<4>::calculateBoundary() {
    // Are there any boundary tetrahedra at all?
    long nBdry = 2 * countTetrahedra() - 5 * simplices_.size();
    if (nBdry == 0)
        return;

    // When triangulating the boundaries, we will need to be able to map
    // (tetrahedron index in 4-manifold) to (tetrahedron in 3-manifold
    // boundary).  There are probably better ways, but we'll just store
    // the (3-manifold tetrahedra) in an array of size
    // (number of 4-manifold tetrahedra).
    NTetrahedron** bdryTetAll = new NTetrahedron*[countTetrahedra()];
    std::fill(bdryTetAll, bdryTetAll + countTetrahedra(), nullptr);

    Dim4BoundaryComponent* label;
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
    for (Dim4Tetrahedron* loopTet : tetrahedra()) {
        // We only care about boundary tetrahedra that we haven't yet seen..
        if (loopTet->degree() == 2 || loopTet->boundaryComponent_)
            continue;

        label = new Dim4BoundaryComponent();
        boundaryComponents_.push_back(label);
        loopTet->component()->boundaryComponents_.push_back(label);

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
            pent = tet->front().pentachoron();
            facet = tet->front().tetrahedron();

            bdryTetAll[tet->markedIndex()] = bdryTet =
                label->boundary_->newTetrahedron();

            // Run through the vertices and edges on this tetrahedron.
            for (i = 0; i < 5; ++i)
                if (i != facet) {
                    vertex = pent->regina::detail::SimplexFaces<4, 0>::face_[i];
                    if (vertex->boundaryComponent_ != label)
                        vertex->boundaryComponent_ = label;
                }

            for (i = 0; i < 5; ++i) {
                if (i == facet)
                    continue;
                for (j = i + 1; j < 5; ++j) {
                    if (j == facet)
                        continue;

                    edge = pent->regina::detail::SimplexFaces<4, 1>::face_[
                        Dim4Edge::edgeNumber[i][j]];
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
                tri = pent->regina::detail::SimplexFaces<4, 2>::face_[
                    Dim4Edge::edgeNumber[i][facet]];
                if (! tri->boundaryComponent_)
                    tri->boundaryComponent_ = label;

                // Okay, we can be clever about this.  The current
                // boundary tetrahedron is one end of the triangle link; the
                // *adjacent* boundary tetrahedron must be at the other.
                triEmb = tri->front();
                if (triEmb.pentachoron() == pent &&
                        triEmb.vertices()[3] == i &&
                        triEmb.vertices()[4] == facet) {
                    // We are currently looking at the embedding at the
                    // front of the list.  Take the one at the back.
                    triEmb = tri->back();

                    adjPent = triEmb.pentachoron();
                    adjFacet = triEmb.vertices()[3];
                    adjTet = adjPent->regina::detail::SimplexFaces<4, 3>::face_[adjFacet];
                    j = triEmb.vertices()[4];
                } else {
                    // We must be looking at the embedding at the back
                    // of the list.  Take the one at the front (which is
                    // already stored in triEmb).
                    adjPent = triEmb.pentachoron();
                    adjFacet = triEmb.vertices()[4];
                    adjTet = adjPent->regina::detail::SimplexFaces<4, 3>::face_[adjFacet];
                    j = triEmb.vertices()[3];

                    // TODO: Sanity checking; remove this eventually.
                    triEmb = tri->back();
                    if (! (triEmb.pentachoron() == pent &&
                            triEmb.vertices()[4] == i &&
                            triEmb.vertices()[3] == facet)) {
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
                            pent->regina::detail::SimplexFaces<4, 3>::mapping_[facet].
                            preImageOf(i))) {
                        // Glue away.
                        tetTri = pent->regina::detail::SimplexFaces<4, 3>::mapping_[facet].
                            preImageOf(i);
                        adjTetTri = adjPent->regina::detail::SimplexFaces<4, 3>::mapping_
                            [adjFacet].preImageOf(j);

                        bdryTet->join(tetTri, adjBdryTet,
                            Perm<4>::contract(adjTet->triangleMapping(adjTetTri) *
                            tet->triangleMapping(tetTri).inverse()));
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
                label->boundary_->triangles().begin();
                it != label->boundary_->triangles().end(); ++it) {
            const NTriangleEmbedding& emb = (*it)->front();
            tet = label->tetrahedra_[emb.tetrahedron()->markedIndex()];
            label->triangles_.push_back(tet->triangle(emb.triangle()));
        }
        for (NTriangulation::EdgeIterator it =
                label->boundary_->edges().begin();
                it != label->boundary_->edges().end(); ++it) {
            const NEdgeEmbedding& emb = (*it)->front();
            tet = label->tetrahedra_[emb.tetrahedron()->markedIndex()];
            label->edges_.push_back(tet->edge(emb.edge()));
        }
        for (NTriangulation::VertexIterator it =
                label->boundary_->vertices().begin();
                it != label->boundary_->vertices().end(); ++it) {
            const NVertexEmbedding& emb = (*it)->front();
            tet = label->tetrahedra_[emb.tetrahedron()->markedIndex()];
            label->vertices_.push_back(tet->vertex(emb.vertex()));
        }
    }

    delete[] bdryTetAll;
}

void Triangulation<4>::calculateVertexLinks() {
    long n = simplices_.size();
    if (n == 0)
        return;

    bool foundNonSimpleLink = false;

    // Construct the vertex linking tetrahedra, and insert them into each
    // vertex link in the correct order as described by the
    // Dim4Vertex::buildLink() docs.
    NTetrahedron** tet = new NTetrahedron*[5 * n];

    for (Dim4Vertex* vertex : vertices()) {
        vertex->link_ = new NTriangulation();
        for (auto& emb : *vertex)
            tet[5 * emb.pentachoron()->index() + emb.vertex()]
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
                tet[index]->join(
                    pent->regina::detail::SimplexFaces<4, 3>::mapping_[vertexIdx].
                        preImageOf(exitFacet),
                    tet[5 * adjPentIdx + adjVertexIdx],
                    Perm<4>::contract(
                            adjPent->regina::detail::SimplexFaces<4, 3>::
                            mapping_[adjVertexIdx].inverse() *
                        pent->adjacentGluing(exitFacet) *
                        pent->regina::detail::SimplexFaces<4, 3>::mapping_[vertexIdx]));
            }
            ++index;
        }
    }

    // Look at each vertex link and see what it says about this 4-manifold
    // triangulation.
    for (Dim4Vertex* vertex : vertices()) {
        if (vertex->link_->hasBoundaryTriangles()) {
            // It's a 3-ball or nothing.
            if ((! knownSimpleLinks_) && ! vertex->link_->isBall()) {
                valid_ = vertex->component_->valid_ =  false;
                vertex->markBadLink();
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
                valid_ = vertex->component_->valid_ =  false;
                vertex->markBadLink();
                foundNonSimpleLink = true;
                boundaryComponents_.push_back(
                    vertex->boundaryComponent_ =
                    new Dim4BoundaryComponent(vertex));
            } else if ((! knownSimpleLinks_) &&
                    ! vertex->link_->isThreeSphere()) {
                // The vertex is fine but it's not a 3-sphere.
                // We have an ideal triangulation.
                ideal_ = vertex->component()->ideal_ = vertex->ideal_ = true;
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
        if (! vertex->isValid()) {
            NTriangulation::VertexIterator linkit;
            int type;
            for (linkit = vertex->link_->vertices().begin();
                    linkit != vertex->link_->vertices().end(); ++linkit) {
                type = (*linkit)->link();
                if (type != NVertex::SPHERE && type != NVertex::DISC) {
                    // This 3-manifold vertex is at the end of an
                    // invalid 4-manifold edge.

                    // Find a tetrahedron in the 3-manifold vertex link
                    // containing the bad 3-manifold vertex.
                    const NVertexEmbedding& linkemb((*linkit)->front());

                    // Find the corresponding pentachoron in the 4-manifold
                    // triangulation.
                    const Dim4VertexEmbedding& vemb(vertex->embedding(
                        linkemb.tetrahedron()->index()));

                    // We have the pentachoron (vemb.pentachoron())
                    // and one of the endpoints of the edge (vemb.vertex()).
                    // Find the other endpoint of the edge.
                    int otherEnd = vemb.pentachoron()->
                        regina::detail::SimplexFaces<4, 3>::mapping_
                        [vemb.vertex()][linkemb.vertex()];

                    // Got it!
                    vemb.pentachoron()->regina::detail::SimplexFaces<4, 1>::face_[
                        Dim4Edge::edgeNumber[vemb.vertex()][otherEnd]
                        ]->markBadLink();
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

void Triangulation<4>::calculateEdgeLinks() {
    for (Dim4Edge* e : edges())
        if (e->hasBadIdentification() && ! e->hasBadLink()) {
            // Calling buildLink() causes the edge link to be cached by
            // Dim4Edge.
            const Triangulation<2>* link = e->buildLink();
            if ((link->isClosed() && link->eulerChar() != 2) ||
                    ((! link->isClosed()) && link->eulerChar() != 1))
                e->markBadLink();
        }
}

} // namespace regina
