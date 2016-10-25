
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

namespace regina {

void Triangulation<2>::calculateSkeleton() {
    TriangulationBase<2>::calculateSkeleton();

    // Get rid of the empty triangulation now, so that all the helper routines
    // can happily assume at least one triangle.
    if (simplices_.empty())
        return;

    calculateBoundary();
        // Sets:
        // - boundaryComponents_
        // - Dim2Component::boundaryComponents_
        // - Dim2 [ Edge, Vertex ]::boundaryComponent_
        // - all Dim2BoundaryComponent members

    // Flesh out the details of each component.
    for (auto v : vertices())
        v->component()->vertices_.push_back(v);
    for (auto e : edges())
        e->component()->edges_.push_back(e);
}

void Triangulation<2>::calculateBoundary() {
    // Are there any boundary edges at all?
    long nBdry = 2 * countEdges() - 3 * simplices_.size();
    if (nBdry == 0)
        return;

    Dim2BoundaryComponent* label;
    Dim2Triangle *tri, *adjTri;
    int edgeId, adjEdgeId;
    int vertexId, adjVertexId;
    Dim2Edge *adjEdge;
    Dim2Vertex* vertex;
    Dim2VertexEmbedding vertexEmb;

    for (Dim2Edge* edge : edges()) {
        // We only care about boundary edges that we haven't yet seen..
        if (edge->degree() == 2 || edge->boundaryComponent_)
            continue;

        label = new Dim2BoundaryComponent();
        boundaryComponents_.push_back(label);
        edge->component()->boundaryComponents_.push_back(label);

        // Loop around from this boundary edge to
        // completely enumerate all edges in this boundary component.

        tri = edge->front().triangle();
        edgeId = edge->front().edge();
        vertexId = edge->front().vertices()[0];
        vertex = tri->regina::detail::SimplexFaces<2, 0>::face_[vertexId];
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
            vertexEmb = vertex->front();
            if (vertexEmb.triangle() == tri &&
                    vertexEmb.vertices()[0] == vertexId &&
                    vertexEmb.vertices()[2] == edgeId) {
                // We are currently looking at the embedding at the
                // front of the list.  Take the one at the back.
                vertexEmb = vertex->back();

                adjTri = vertexEmb.triangle();
                adjEdgeId = vertexEmb.vertices()[1];
                adjEdge = adjTri->regina::detail::SimplexFaces<2, 1>::face_[adjEdgeId];
                adjVertexId = vertexEmb.vertices()[2];
            } else {
                // We must be looking at the embedding at the back
                // of the list.  Take the one at the front (which is
                // already stored in vertexEmb).
                adjTri = vertexEmb.triangle();
                adjEdgeId = vertexEmb.vertices()[2];
                adjEdge = adjTri->regina::detail::SimplexFaces<2, 1>::face_[adjEdgeId];
                adjVertexId = vertexEmb.vertices()[1];

                // TODO: Sanity checking; remove this eventually.
                vertexEmb = vertex->back();
                if (! (vertexEmb.triangle() == tri &&
                        vertexEmb.vertices()[0] == vertexId &&
                        vertexEmb.vertices()[1] == edgeId)) {
                    std::cerr << "ERROR: Something has gone terribly "
                        "wrong in computeBoundaryComponents()."
                        << std::endl;
                    ::exit(1);
                }
            }

            edge = adjEdge;
            tri = adjTri;
            edgeId = adjEdgeId;
            vertexId = adjVertexId;
            vertex = tri->regina::detail::SimplexFaces<2, 0>::face_[vertexId];
        }
    }
}

} // namespace regina

