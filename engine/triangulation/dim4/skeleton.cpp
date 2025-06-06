
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one pentachoron.
    if (simplices_.empty())
        return;

    calculateVertexLinks();
        // Sets:
        // - Vertex<4>::realBoundary_
        // - Vertex<4>::link_, but only where necessary
        // - valid_ and Vertex<4>::valid_ in the case of bad vertex links
        // - valid_ and Edge<4>::invalid_ in the case of bad edge links
        // - Vertex<4>::ideal_ and Component<4>::ideal_
        // - vertexLinkSummary_

    if (! valid_)
        calculateEdgeLinks();
        // Sets:
        // - Edge<4>::link_, but only for edges with bad self-identifications

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

    for (auto bc : boundaryComponents())
        if (bc->isReal())
            for (auto v : bc->vertices())
                v->realBoundary_ = true;

    // Look at each vertex link and see what it says about this 4-manifold
    // triangulation.
    long foundIdeal = 0; // -1 if we ever find an invalid vertex
    long remaining = countVertices();
    for (Vertex<4>* vertex : vertices()) {
        if (vertex->realBoundary_) {
            // This vertex belongs to one or more boundary tetrahedra.
            // If the link is not a 3-ball then this vertex is invalid.
            // In particular, if vertexLinkSummary_ >= 0 then all vertices
            // are valid and therefore this must be a 3-ball.
            if (vertexLinkSummary_ < 0 && ! vertex->buildLink().isBall()) {
                valid_ = vertex->component_->valid_ =  false;
                vertex->whyInvalid_.value |= Vertex<4>::INVALID_LINK;
                foundIdeal = -1;
                // The vertex belongs to some pentachoron with boundary
                // tetrahedra, and so already belongs to a boundary component.
            }
        } else {
            // This vertex is not part of any boundary tetrahedra.
            // Let's see what we've got.
            // Note: the first test below is for invalid vertices, and so
            // if vertexLinkSummary_ >= 0 we can skip that test entirely.
            if (vertexLinkSummary_ < 0 &&
                    ((! vertex->buildLink().isValid()) ||
                     vertex->buildLink().isIdeal())) {
                // Bapow.
                valid_ = vertex->component_->valid_ =  false;
                vertex->whyInvalid_.value |= Vertex<4>::INVALID_LINK;
                foundIdeal = -1;
                vertex->boundaryComponent_ = new BoundaryComponent<4>();
                ++nBoundaryFaces_[0];
                vertex->boundaryComponent_->orientable_ =
                    vertex->isLinkOrientable();
                vertex->boundaryComponent_->push_back(vertex);
                boundaryComponents_.push_back(vertex->boundaryComponent_);
                vertex->component_->boundaryComponents_.push_back(
                    vertex->boundaryComponent_);
            } else {
                // If we have a 3-sphere link then there is nothing to do.
                // Otherwise we have a non-sphere closed 3-manifold, and
                // we get an ideal vertex.
                //
                // Note: if vertexLinkSummary_ >= 0 then all vertices
                // are guaranteed to be valid, and so we can assume that
                // foundIdeal >= 0 also.
                if (
                        // Every remaining vertex must be ideal:
                        (vertexLinkSummary_ >= 0 &&
                            foundIdeal + remaining == vertexLinkSummary_) ||
                        // Either we don't know how many ideal vertices
                        // to expect, or we know that some but not all
                        // remaining vertices are ideal (and in either
                        // case we must explicitly test isSphere()):
                        ((vertexLinkSummary_ < 0 ||
                                foundIdeal < vertexLinkSummary_) &&
                            ! vertex->buildLink().isSphere())) {
                    // We have an ideal vertex.
                    vertex->component()->ideal_ = vertex->ideal_ = true;
                    if (foundIdeal >= 0)
                        ++foundIdeal;
                    vertex->boundaryComponent_ = new BoundaryComponent<4>();
                    ++nBoundaryFaces_[0];
                    vertex->boundaryComponent_->orientable_ =
                        vertex->isLinkOrientable();
                    vertex->boundaryComponent_->push_back(vertex);
                    boundaryComponents_.push_back(vertex->boundaryComponent_);
                    vertex->component_->boundaryComponents_.push_back(
                        vertex->boundaryComponent_);
                }
            }
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
            for (Vertex<3>* v : vertex->buildLink().vertices()) {
                auto type = v->linkType();
                if (type != Vertex<3>::Link::Sphere &&
                        type != Vertex<3>::Link::Disc) {
                    // This 3-manifold vertex is at the end of an
                    // invalid 4-manifold edge.

                    // Find a tetrahedron in the 3-manifold vertex link
                    // containing the bad 3-manifold vertex.
                    const VertexEmbedding<3>& linkemb(v->front());

                    // Find the corresponding pentachoron in the 4-manifold
                    // triangulation.
                    const VertexEmbedding<4>& vemb(vertex->embedding(
                        linkemb.tetrahedron()->index()));

                    // We have the pentachoron (vemb.pentachoron())
                    // and one of the endpoints of the edge (vemb.vertex()).
                    // Find the other endpoint of the edge.
                    int otherEnd = std::get<3>(vemb.pentachoron()->mappings_)
                        [vemb.vertex()][linkemb.vertex()];

                    // Got it!
                    std::get<1>(vemb.pentachoron()->faces_)[
                        Edge<4>::edgeNumber[vemb.vertex()][otherEnd]
                        ]->whyInvalid_.value |= Edge<4>::INVALID_LINK;
                }
            }
        }

        --remaining;
    }

    vertexLinkSummary_ = foundIdeal;
}

void Triangulation<4>::calculateEdgeLinks() {
    for (Edge<4>* e : edges())
        if (e->hasBadIdentification() && ! e->hasBadLink()) {
            // Calling buildLink() causes the edge link to be cached by
            // Edge<4>.
            const Triangulation<2>& link = e->buildLink();
            if ((link.isClosed() && link.eulerChar() != 2) ||
                    ((! link.isClosed()) && link.eulerChar() != 1))
                e->whyInvalid_.value |= Edge<4>::INVALID_LINK;
        }
}

void Triangulation<4>::cloneSkeleton(const Triangulation& src) {
    TriangulationBase<4>::cloneSkeleton(src);

    // Leave Vertex::link_ and Edge::link_ as built-on-demand for now.

    vertexLinkSummary_ = src.vertexLinkSummary_;
    {
        auto me = vertices().begin();
        auto you = src.vertices().begin();
        for ( ; me != vertices().end(); ++me, ++you) {
            (*me)->ideal_ = (*you)->ideal_;
        }
    }
    {
        auto me = components_.begin();
        auto you = src.components_.begin();
        for ( ; me != components_.end(); ++me, ++you) {
            (*me)->ideal_ = (*you)->ideal_;

            for (auto f : (*you)->vertices_)
                (*me)->vertices_.push_back(vertex(f->index()));
            for (auto f : (*you)->edges_)
                (*me)->edges_.push_back(edge(f->index()));
            for (auto f : (*you)->triangles_)
                (*me)->triangles_.push_back(triangle(f->index()));
            for (auto f : (*you)->tetrahedra_)
                (*me)->tetrahedra_.push_back(tetrahedron(f->index()));
        }
    }
}

} // namespace regina
