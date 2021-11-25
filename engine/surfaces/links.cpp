
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

#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"
#include <set>

namespace regina {

bool NormalSurface::isVertexLinking() const {
    if (! enc_.couldBeVertexLink())
        return false;

    size_t nTets = triangulation_->size();
    for (size_t tet = 0; tet < nTets; tet++) {
        for (int type = 0; type < 3; type++)
            if (quads(tet, type) != 0)
                return false;
    }
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0)
                    return false;
    return true;
}

const Vertex<3>* NormalSurface::isVertexLink() const {
    if (! enc_.couldBeVertexLink())
        return nullptr;

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& tri(*triangulation_);
    size_t nTets = tri.size();

    // Check that there are no quad/oct discs.
    for (size_t tet = 0; tet < nTets; tet++) {
        for (int type = 0; type < 3; type++)
            if (quads(tet, type) != 0)
                return nullptr;
    }
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0)
                    return nullptr;

    // It follows from the matching equations that what we have is a
    // union of vertex links.  Make sure we are linking just the one vertex.

    Vertex<3>* ans = nullptr;
    LargeInteger ansMult;

    for (size_t tet = 0; tet < nTets; tet++) {
        const Tetrahedron<3>* t = tri.tetrahedron(tet);
        for (int type = 0; type < 4; type++) {
            LargeInteger coord = triangles(tet, type);

            if (coord != 0) {
                // Some triangle discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = t->vertex(type);
                    ansMult = coord;
                } else if (ans != t->vertex(type)) {
                    // We seem to be linking more than one vertex.
                    return nullptr;
                }
            }
        }
    }

    // Either we are linking exactly one vertex (ans != null), or we
    // have the empty vector (ans == null).
    return ans;
}

std::pair<const Edge<3>*, const Edge<3>*> NormalSurface::isThinEdgeLink() const {
    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& tri(*triangulation_);
    size_t nTets = tri.size();

    // Check that there are no octagonal discs.
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0)
                    return { nullptr, nullptr };

    // Run through the quadrilateral discs and work out if there are any
    // valid candidates.
    std::set<Edge<3>*> notAns;
        /**< Edges that we know the answer *isn't*.
             We will stop updating this set once foundQuads is true. */
    bool foundQuads = false;
    const Edge<3>* ans[2];
    LargeInteger ansMultDouble;

    const Tetrahedron<3>* t;
    Edge<3>* e[6]; // { 2*link, 4*intersect }
    LargeInteger coord;
    int i;

    for (size_t tet = 0; tet < nTets; tet++) {
        t = tri.tetrahedron(tet);
        for (int type = 0; type < 3; type++) {
            coord = quads(tet, type);
            e[0] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][1]]);
            e[1] = t->edge(Edge<3>::edgeNumber[quadDefn[type][2]]
                [quadDefn[type][3]]);
            e[2] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][2]]);
            e[3] = t->edge(Edge<3>::edgeNumber[quadDefn[type][0]]
                [quadDefn[type][3]]);
            e[4] = t->edge(Edge<3>::edgeNumber[quadDefn[type][1]]
                [quadDefn[type][2]]);
            e[5] = t->edge(Edge<3>::edgeNumber[quadDefn[type][1]]
                [quadDefn[type][3]]);

            if (coord == 0) {
                // No discs in this coordinate.
                // Do we have any candidate edges yet?
                if (foundQuads) {
                    // Rule out any candidates that should have discs here.
                    for (i = 0; i < 2; i++)
                        if (ans[i] == e[0] || ans[i] == e[1])
                            ans[i] = nullptr;
                } else {
                    // Still haven't found any candidates.
                    notAns.insert(e[0]);
                    notAns.insert(e[1]);
                }
            } else {
                // Some discs in this coordinate.
                // Do we have any candidate edges yet?
                if (foundQuads) {
                    // Check consistency with our candidates.
                    if (e[0] == e[1]) {
                        // Same edge on both sides of the quad.
                        // Note that there can only be one candidate now.
                        if (e[0] == ans[0])
                            ans[1] = nullptr;
                        else if (e[0] == ans[1]) {
                            ans[0] = ans[1];
                            ans[1] = nullptr;
                        } else
                            return { nullptr, nullptr };

                        // The only possible candidate is ans[0].
                        if (ans[0] == nullptr || ansMultDouble != coord)
                            return { nullptr, nullptr };
                    } else {
                        // Different edges on both sides of the quad.
                        // Check each candidate in turn.
                        for (i = 0; i < 2; i++)
                            if (ans[i] != e[0] && ans[i] != e[1])
                                ans[i] = nullptr;
                        if (ansMultDouble != coord * 2)
                            return { nullptr, nullptr };
                    }
                } else {
                    // We've found our first and only possible candidates.
                    if (e[0] == e[1]) {
                        // Same edge on both sides of the quad.
                        if (notAns.find(e[0]) != notAns.end())
                            return { nullptr, nullptr };
                        ans[0] = e[0];
                        ans[1] = nullptr;
                        ansMultDouble = coord;
                    } else {
                        // Different edges on both sides of the quad.
                        for (i = 0; i < 2; i++) {
                            if (notAns.find(e[i]) != notAns.end())
                                ans[i] = nullptr;
                            else {
                                ans[i] = e[i];
                                ansMultDouble = coord;
                                ansMultDouble *= 2;
                            }
                        }
                    }
                    foundQuads = true;
                }

                // We now absolutely have candidates (or have exhausted
                // them all).  Check that these candidates don't
                // intersect the new quads.
                for (i = 2; i < 6; i++) {
                    if (ans[0] == e[i])
                        ans[0] = nullptr;
                    if (ans[1] == e[i])
                        ans[1] = nullptr;
                }
            }

            // Have we ruled out all the candidates we ever had?
            if (foundQuads && (! ans[0]) && (! ans[1]))
                return { nullptr, nullptr };
        }
    }

    // So did we actually find anything?
    if (! foundQuads)
        return { nullptr, nullptr };
    if ((! ans[0]) && (! ans[1]))
        return { nullptr, nullptr };

    // Finally check the triangular discs.
    Vertex<3>* v;
    bool expectZero[2];
    int j;
    for (size_t tet = 0; tet < nTets; tet++) {
        t = tri.tetrahedron(tet);
        for (int type = 0; type < 4; type++) {
            v = t->vertex(type);
            coord = triangles(tet, type);

            // Should we actually see any discs?
            for (i = 0; i < 2; i++) {
                if (! ans[i])
                    continue;

                // If the candidate edge does not touch this vertex, the
                // triangular coordinate should be 0.
                expectZero[i] = (v != ans[i]->vertex(0) &&
                    v != ans[i]->vertex(1));

                // If this triangular disc type intersects the candidate
                // edge, the coordinate should also be 0.
                if (! expectZero[i])
                    for (j = 0; j < 3; j++)
                        if (t->edge(Edge<3>::edgeNumber[type][(type+j+1) % 4])
                                == ans[i]) {
                            expectZero[i] = true;
                            break;
                        }

                // So did we get the right triangular coordinate?
                if (expectZero[i]) {
                    if (coord != 0)
                        ans[i] = nullptr;
                } else {
                    if (ansMultDouble != coord * 2)
                        ans[i] = nullptr;
                }
            }

            // Have we ruled out all possibilities?
            if ((! ans[0]) && (! ans[1]))
                return { nullptr, nullptr };
        }
    }

    // Return whatever candidates have survived.
    if (ans[0])
        return { ans[0], ans[1] };
    else
        return { ans[1], ans[0] };
}

} // namespace regina

