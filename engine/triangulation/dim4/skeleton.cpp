
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

    calculateVertexLinks();
        // Sets:
        // - Vertex<4>::link_
        // - valid_ and Vertex<4>::valid_ in the case of bad vertex links
        // - valid_ and Edge<4>::invalid_ in the case of bad edge links
        // - ideal_, Vertex<4>::ideal_ and Component<4>::ideal_

    if (! valid_)
        calculateEdgeLinks();
        // Sets:
        // - Edge<4>::link_, but only for edges with bad self-identifications

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

void Triangulation<4>::calculateVertexLinks() {
    long n = simplices_.size();
    if (n == 0)
        return;

    bool foundNonSimpleLink = false;

    // Construct the vertex linking tetrahedra, and insert them into each
    // vertex link in the correct order as described by the
    // Vertex<4>::buildLink() docs.
    Tetrahedron<3>** tet = new Tetrahedron<3>*[5 * n];

    for (Vertex<4>* vertex : vertices()) {
        vertex->link_ = new Triangulation<3>();
        for (auto& emb : *vertex)
            tet[5 * emb.pentachoron()->index() + emb.vertex()]
                = vertex->link_->newTetrahedron();
    }

    // Now glue the tetrahedra together correctly.
    Pentachoron<4> *pent, *adjPent;
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
    for (Vertex<4>* vertex : vertices()) {
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
                vertex->boundaryComponent_ = new BoundaryComponent<4>();
                vertex->boundaryComponent_->orientable_ =
                    vertex->isLinkOrientable();
                vertex->boundaryComponent_->push_back(vertex);
                boundaryComponents_.push_back(vertex->boundaryComponent_);
            } else if ((! knownSimpleLinks_) &&
                    ! vertex->link_->isThreeSphere()) {
                // The vertex is fine but it's not a 3-sphere.
                // We have an ideal triangulation.
                ideal_ = vertex->component()->ideal_ = vertex->ideal_ = true;
                foundNonSimpleLink = true;
                vertex->boundaryComponent_ = new BoundaryComponent<4>();
                vertex->boundaryComponent_->orientable_ =
                    vertex->isLinkOrientable();
                vertex->boundaryComponent_->push_back(vertex);
                boundaryComponents_.push_back(vertex->boundaryComponent_);
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
            Triangulation<3>::VertexIterator linkit;
            int type;
            for (linkit = vertex->link_->vertices().begin();
                    linkit != vertex->link_->vertices().end(); ++linkit) {
                type = (*linkit)->link();
                if (type != Vertex<3>::SPHERE && type != Vertex<3>::DISC) {
                    // This 3-manifold vertex is at the end of an
                    // invalid 4-manifold edge.

                    // Find a tetrahedron in the 3-manifold vertex link
                    // containing the bad 3-manifold vertex.
                    const VertexEmbedding<3>& linkemb((*linkit)->front());

                    // Find the corresponding pentachoron in the 4-manifold
                    // triangulation.
                    const VertexEmbedding<4>& vemb(vertex->embedding(
                        linkemb.tetrahedron()->index()));

                    // We have the pentachoron (vemb.pentachoron())
                    // and one of the endpoints of the edge (vemb.vertex()).
                    // Find the other endpoint of the edge.
                    int otherEnd = vemb.pentachoron()->
                        regina::detail::SimplexFaces<4, 3>::mapping_
                        [vemb.vertex()][linkemb.vertex()];

                    // Got it!
                    vemb.pentachoron()->regina::detail::SimplexFaces<4, 1>::face_[
                        Edge<4>::edgeNumber[vemb.vertex()][otherEnd]
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
    for (Edge<4>* e : edges())
        if (e->hasBadIdentification() && ! e->hasBadLink()) {
            // Calling buildLink() causes the edge link to be cached by
            // Edge<4>.
            const Triangulation<2>* link = e->buildLink();
            if ((link->isClosed() && link->eulerChar() != 2) ||
                    ((! link->isClosed()) && link->eulerChar() != 1))
                e->markBadLink();
        }
}

} // namespace regina
