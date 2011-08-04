
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"
#include <algorithm>

namespace regina {

Dim4Pentachoron::Dim4Pentachoron(Dim4Triangulation* tri) : tri_(tri) {
    std::fill(adj_, adj_ + 5, static_cast<Dim4Pentachoron*>(0));
}

Dim4Pentachoron::Dim4Pentachoron(const std::string& desc,
        Dim4Triangulation* tri) : desc_(desc), tri_(tri) {
    std::fill(adj_, adj_ + 5, static_cast<Dim4Pentachoron*>(0));
}

bool Dim4Pentachoron::hasBoundary() const {
    for (int i=0; i<5; ++i)
        if (adj_[i] == 0)
            return true;
    return false;
}

void Dim4Pentachoron::joinTo(int myFacet, Dim4Pentachoron* you, NPerm5 gluing) {
    adj_[myFacet] = you;
    adjPerm_[myFacet] = gluing;
    int yourFacet = gluing[myFacet];
    you->adj_[yourFacet] = this;
    you->adjPerm_[yourFacet] = gluing.inverse();

    tri_->clearAllProperties();
    tri_->fireChangedEvent();
}

Dim4Pentachoron* Dim4Pentachoron::unjoin(int myFacet) {
    Dim4Pentachoron* you = adj_[myFacet];
    int yourFacet = adjPerm_[myFacet][myFacet];
    you->adj_[yourFacet] = 0;
    adj_[myFacet] = 0;

    tri_->clearAllProperties();
    tri_->fireChangedEvent();

    return you;
}

void Dim4Pentachoron::isolate() {
    for (int i=0; i<5; ++i)
        if (adj_[i])
            unjoin(i);
}

Dim4Component* Dim4Pentachoron::getComponent() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return component_;
}

Dim4Vertex* Dim4Pentachoron::getVertex(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertex_[vertex];
}

Dim4Edge* Dim4Pentachoron::getEdge(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edge_[edge];
}

Dim4Face* Dim4Pentachoron::getFace(int face) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return face_[face];
}

Dim4Tetrahedron* Dim4Pentachoron::getTetrahedron(int tet) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return tet_[tet];
}

NPerm5 Dim4Pentachoron::getVertexMapping(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertexMapping_[vertex];
}

NPerm5 Dim4Pentachoron::getEdgeMapping(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edgeMapping_[edge];
}

NPerm5 Dim4Pentachoron::getFaceMapping(int face) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return faceMapping_[face];
}

NPerm5 Dim4Pentachoron::getTetrahedronMapping(int tet) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return tetMapping_[tet];
}

int Dim4Pentachoron::orientation() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return orientation_;
}

} // namespace regina

