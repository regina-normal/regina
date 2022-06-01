
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "triangulation/dim3.h"
#include "maths/matrix.h"

namespace regina {

const AbelianGroup& Triangulation<3>::homologyRel() const {
    if (prop_.H1Rel_.has_value())
        return *prop_.H1Rel_;

    if (! isValid())
        throw FailedPrecondition(
            "homologyRel() requires a valid triangulation");

    if (countBoundaryComponents() == 0)
        return *(prop_.H1Rel_ = homology());

    // Calculate the relative first homology wrt the boundary.

    // Find a maximal forest in the 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    std::set<Edge<3>*> forest = maximalForestInSkeleton(false);

    // Build a presentation matrix.
    // Each non-boundary not-in-forest edge is a generator.
    // Each non-boundary triangle is a relation.
    size_t nBdryVertices = 0;
    size_t nBdryEdges = 0;
    size_t nClosedComponents = 0;
    for (auto bc : boundaryComponents()) {
        nBdryVertices += bc->countVertices();
        nBdryEdges += bc->countEdges();
    }
    for (Component<3>* c : components())
        if (c->isClosed())
            nClosedComponents++;
    size_t nGens = (countEdges() - nBdryEdges)
        + nClosedComponents - (countVertices() - nBdryVertices);
    size_t nRels = countTriangles() - countBoundaryFacets();
    MatrixInt pres(nRels, nGens);

    // Find out which edge corresponds to which generator.
    ssize_t* genIndex = new ssize_t[countEdges()];
    size_t i = 0;
    for (Edge<3>* e : edges()) {
        if (e->isBoundary())
            genIndex[e->index()] = -1;
        else if (forest.count(e))
            genIndex[e->index()] = -1;
        else
            genIndex[e->index()] = i++;
    }

    // Run through each triangle and put the relations in the matrix.
    Tetrahedron<3>* currTet;
    Perm<4> currTetVertices;
    ssize_t edgeGenIndex;
    i = 0;
    int triEdge, currEdgeStart, currEdgeEnd, currEdge;
    for (Triangle<3>* f : triangles()) {
        if (! f->isBoundary()) {
            // Put in the relation corresponding to this triangle.
            currTet = f->front().tetrahedron();
            currTetVertices = f->front().vertices();
            for (triEdge = 0; triEdge < 3; triEdge++) {
                currEdgeStart = currTetVertices[triEdge];
                currEdgeEnd = currTetVertices[(triEdge + 1) % 3];
                // Examine the edge from vertex edgeStart to edgeEnd
                // in tetrahedron currTet.
                currEdge = Edge<3>::edgeNumber[currEdgeStart][currEdgeEnd];
                edgeGenIndex = genIndex[currTet->edge(currEdge)->index()];
                if (edgeGenIndex >= 0) {
                    if (currTet->edgeMapping(currEdge)[0] == currEdgeStart)
                        pres.entry(i, edgeGenIndex) += 1;
                    else
                        pres.entry(i, edgeGenIndex) -= 1;
                }
            }
            i++;
        }
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    return prop_.H1Rel_.emplace(std::move(pres));
}

const AbelianGroup& Triangulation<3>::homologyBdry() const {
    if (prop_.H1Bdry_.has_value())
        return *prop_.H1Bdry_;

    if (! isValid())
        throw FailedPrecondition(
            "homologyBdry() requires a valid triangulation");

    // Run through the individual boundary components and add the
    // appropriate pieces to the homology group.
    size_t rank = 0;
    size_t z2rank = 0;

    // Ensure that the skeleton has been calculated.
    ensureSkeleton();

    for (auto bc : boundaryComponents()) {
        if (bc->isOrientable()) {
            rank += (2 - bc->eulerChar());
        } else {
            rank += (1 - bc->eulerChar());
            z2rank++;
        }
    }

    // Build the group and tidy up.
    AbelianGroup ans(rank);
    for (size_t i = 0; i < z2rank; ++i)
        ans.addTorsion(2);
    return *(prop_.H1Bdry_ = std::move(ans));
}

} // namespace regina

