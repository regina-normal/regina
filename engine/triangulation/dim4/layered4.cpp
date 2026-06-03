
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <array>
#include <sstream>
#include <vector>
#include "triangulation/dim4.h"

namespace regina {

namespace {

struct BoundaryFacet4 {
    Pentachoron<4>* pentachoron;
    int newFacet;
    Perm<5> roles;
};

Triangle<4>* edgeTriangle(const BoundaryFacet4& facet,
        int sideVertex) {
    return facet.pentachoron->triangle(Triangle<4>::triangleNumber[
        facet.roles[0]][facet.roles[1]][facet.roles[sideVertex]]);
}

Triangle<4>* sharedEdgeTriangle(const BoundaryFacet4& lhs,
        const BoundaryFacet4& rhs) {
    for (int i = 2; i < 5; ++i) {
        if (i == lhs.newFacet)
            continue;
        Triangle<4>* candidate = edgeTriangle(lhs, i);
        for (int j = 2; j < 5; ++j)
            if (j != rhs.newFacet && candidate == edgeTriangle(rhs, j))
                return candidate;
    }
    return nullptr;
}

Perm<5> edgeFacetGluing(const BoundaryFacet4& facet,
        const BoundaryFacet4& prev, const BoundaryFacet4& next,
        int newFacet, int prevFacet, int nextFacet) {
    Triangle<4>* sharedPrev = sharedEdgeTriangle(facet, prev);
    Triangle<4>* sharedNext = sharedEdgeTriangle(facet, next);
    if (! sharedPrev || ! sharedNext || sharedPrev == sharedNext)
        throw InvalidArgument(
            "layerOn() could not consistently label the boundary edge link");

    int sideToPrev = -1;
    int sideToNext = -1;
    for (int i = 2; i < 5; ++i) {
        if (i == facet.newFacet)
            continue;
        Triangle<4>* tri = edgeTriangle(facet, i);
        if (tri == sharedPrev)
            sideToPrev = i;
        else if (tri == sharedNext)
            sideToNext = i;
    }

    if (sideToPrev < 0 || sideToNext < 0)
        throw InvalidArgument(
            "layerOn() could not consistently label the boundary edge link");

    std::array<int, 5> image {};
    image[0] = facet.roles[0];
    image[1] = facet.roles[1];
    image[newFacet] = facet.roles[facet.newFacet];
    image[nextFacet] = facet.roles[sideToPrev];
    image[prevFacet] = facet.roles[sideToNext];

    return Perm<5>(image);
}

std::vector<BoundaryFacet4> boundaryFacets(Edge<4>* edge) {
    std::vector<BoundaryFacet4> ans;
    for (const auto& emb : *edge) {
        Pentachoron<4>* pent = emb.pentachoron();
        Perm<5> roles = emb.vertices();

        for (int v = 2; v < 5; ++v) {
            int facet = roles[v];
            if (pent->adjacentPentachoron(facet))
                continue;

            Tetrahedron<4>* tet = pent->tetrahedron(facet);
            bool seen = false;
            for (const BoundaryFacet4& old : ans)
                if (old.pentachoron->tetrahedron(old.roles[old.newFacet])
                        == tet) {
                    seen = true;
                    break;
                }
            if (! seen)
                ans.push_back({ pent, v, roles });
        }
    }
    return ans;
}

std::vector<BoundaryFacet4> boundaryFacetAppearances(Edge<4>* edge) {
    std::vector<BoundaryFacet4> ans;
    for (const auto& emb : *edge) {
        Pentachoron<4>* pent = emb.pentachoron();
        Perm<5> roles = emb.vertices();

        for (int v = 2; v < 5; ++v)
            if (! pent->adjacentPentachoron(roles[v]))
                ans.push_back({ pent, v, roles });
    }
    return ans;
}

std::vector<BoundaryFacet4> orderedBoundaryFacets(Edge<4>* edge,
        size_t expectedDegree) {
    std::vector<BoundaryFacet4> facets = boundaryFacets(edge);
    if (facets.size() != expectedDegree) {
        std::ostringstream msg;
        msg << "layerOn() requires boundary degree " << expectedDegree
            << " (the given edge has boundary degree " << facets.size()
            << " and 4-dimensional degree " << edge->degree() << ')';
        throw InvalidArgument(msg.str());
    }

    std::vector<std::vector<int>> adj(expectedDegree);
    for (size_t i = 0; i < expectedDegree; ++i)
        for (size_t j = i + 1; j < expectedDegree; ++j)
            if (sharedEdgeTriangle(facets[i], facets[j])) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }

    for (size_t i = 0; i < expectedDegree; ++i)
        if (adj[i].size() != 2)
            throw InvalidArgument(
                "layerOn() requires the boundary tetrahedra around the edge "
                "to form a cycle");

    std::vector<BoundaryFacet4> ordered;
    int prev = -1;
    int curr = 0;
    for (size_t i = 0; i < expectedDegree; ++i) {
        ordered.push_back(facets[curr]);
        int next = -1;
        for (int candidate : adj[curr])
            if (candidate != prev) {
                next = candidate;
                break;
            }
        prev = curr;
        curr = next;
    }

    if (curr != 0)
        throw InvalidArgument(
            "layerOn() requires the boundary tetrahedra around the edge "
            "to form a cycle");

    return ordered;
}

bool hasBoundary32(Edge<4>* edge) {
    if (! edge)
        return false;
    if (! edge->isBoundary())
        return false;
    if (! edge->boundaryComponent() || ! edge->boundaryComponent()->isReal())
        return false;

    // Count boundary tetrahedron appearances, not just distinct
    // Tetrahedron<4>* objects.  A single boundary tetrahedron can contain the
    // same global Edge<4> more than once; this gives multiple edge
    // appearances in the induced boundary triangulation, and is not a legal
    // 3-2 move.
    std::vector<BoundaryFacet4> facets = boundaryFacetAppearances(edge);
    if (facets.size() != 3)
        return false;

    // The 3D Pachner preconditions require three distinct boundary
    // tetrahedra around the edge.
    for (int i = 0; i < 3; ++i) {
        Tetrahedron<4>* tetI =
            facets[i].pentachoron->tetrahedron(
                facets[i].roles[facets[i].newFacet]);
        for (int j = 0; j < i; ++j)
            if (tetI == facets[j].pentachoron->tetrahedron(
                    facets[j].roles[facets[j].newFacet]))
                return false;
    }

    // In a legal boundary 3-2 move, the edge link is the standard 3-cycle.
    // Thus each pair of boundary tetrahedra must share one of the boundary
    // triangles containing the edge, and these three shared triangles must be
    // distinct.
    Triangle<4>* shared[3] {
        sharedEdgeTriangle(facets[0], facets[1]),
        sharedEdgeTriangle(facets[0], facets[2]),
        sharedEdgeTriangle(facets[1], facets[2])
    };

    for (int i = 0; i < 3; ++i) {
        if (! shared[i])
            return false;
        for (int j = 0; j < i; ++j)
            if (shared[i] == shared[j])
                return false;
    }

    return true;
}

} // namespace

// namespace regina {

Pentachoron<4>* Triangulation<4>::layerOn(Triangle<4>* triangle) {
    if (! triangle)
        throw InvalidArgument("layerOn() requires a non-null triangle");
    if (! triangle->isBoundary())
        throw InvalidArgument("layerOn() requires a boundary triangle");
    if (! triangle->boundaryComponent()->isReal())
        throw InvalidArgument("layerOn() requires real boundary");

    // Locate the two boundary tetrahedra.
    // For codimension-2 boundary faces, front() and back() are the two
    // appearances at the ends of the link path.
    Pentachoron<4>* pent1 = triangle->front().pentachoron();
    Pentachoron<4>* pent2 = triangle->back().pentachoron();

    Perm<5> roles1 = triangle->front().vertices();
    Perm<5> roles2 = triangle->back().vertices();

    if (pent1->tetrahedron(roles1[4]) == pent2->tetrahedron(roles2[3]))
        throw InvalidArgument("layerOn() requires a triangle between two "
            "distinct boundary tetrahedra");
    if (pent1->isFacetLocked(roles1[4]) || pent2->isFacetLocked(roles2[3]))
        throw LockViolation("An attempt was made to layer on a locked facet");

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    Pentachoron<4>* newPent = newSimplexRaw();

    newPent->joinRaw(4, pent1, roles1);
    newPent->joinRaw(3, pent2, roles2);

    return newPent;
}

Pentachoron<4>* Triangulation<4>::layerOn(Edge<4>* edge) {
    if (! edge)
        throw InvalidArgument("layerOn() requires a non-null edge");
    if (! edge->isBoundary())
        throw InvalidArgument("layerOn() requires a boundary edge");
    if (! edge->boundaryComponent()->isReal())
        throw InvalidArgument("layerOn() requires real boundary");
    if (! hasBoundary32(edge))
        throw InvalidArgument(
            "layerOn() requires a legal 3-2 move in the boundary");

    std::vector<BoundaryFacet4> facets = orderedBoundaryFacets(edge, 3);

    for (const BoundaryFacet4& facet : facets)
        if (facet.pentachoron->isFacetLocked(facet.roles[facet.newFacet]))
            throw LockViolation(
                "An attempt was made to layer on a locked facet");

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    Pentachoron<4>* newPent = newSimplexRaw();
    for (int i = 0; i < 3; ++i) {
        int newFacet = i + 2;
        int prevFacet = ((i + 2) % 3) + 2;
        int nextFacet = ((i + 1) % 3) + 2;
        newPent->joinRaw(i + 2, facets[i].pentachoron,
            edgeFacetGluing(facets[i], facets[(i + 2) % 3],
                facets[(i + 1) % 3], newFacet, prevFacet, nextFacet));
    }

    return newPent;
}

bool Triangulation<4>::layer44(Edge<4>* edge, int axis) {
    if (axis != 0 && axis != 1)
        return false;
    if (! edge)
        return false;
    if (! edge->isBoundary())
        return false;
    if (! edge->boundaryComponent()->isReal())
        return false;

    std::vector<BoundaryFacet4> facets;
    try {
        facets = orderedBoundaryFacets(edge, 4);
    } catch (const InvalidArgument&) {
        return false;
    }

    Triangle<4>* tri23 = (axis == 0 ?
        sharedEdgeTriangle(facets[0], facets[1]) :
        sharedEdgeTriangle(facets[1], facets[2]));
    if (! tri23)
        return false;

    Pentachoron<4>* edgePent = facets[3].pentachoron;
    int edge32 = Edge<4>::edgeNumber[facets[3].roles[0]][facets[3].roles[1]];

    try {
        PacketChangeGroup span(*this);
        layerOn(tri23);
        layerOn(edgePent->edge(edge32));
        return true;
    } catch (const InvalidArgument&) {
        return false;
    } catch (const LockViolation&) {
        return false;
    }
}

} // namespace regina
