
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"

#define NO_EDGES std::make_pair(static_cast<NEdge*>(0), static_cast<NEdge*>(0))

namespace regina {

bool NNormalSurfaceVector::isVertexLinking(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 3; type++)
            if (getQuadCoord(tet, type, triang) != 0)
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return false;
    return true;
}

const NVertex* NNormalSurfaceVector::isVertexLink(NTriangulation* triang)
        const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;

    // Check that there are no quad/oct discs.
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 3; type++)
            if (getQuadCoord(tet, type, triang) != 0)
                return 0;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return 0;

    // Now examine the triangle to see if we link only a single vertex.
    stdhash::hash_set<NVertex*, HashPointer> notAns;
        /**< We will ignore notAns once ans != 0. */
    NVertex* ans = 0;
    NLargeInteger ansMult;

    NTetrahedron* t;
    NVertex* v;
    NLargeInteger coord;

    for (tet = 0; tet < nTets; tet++) {
        t = triang->getTetrahedron(tet);
        for (type = 0; type < 4; type++) {
            v = t->getVertex(type);
            coord = getTriangleCoord(tet, type, triang);

            if (coord == 0) {
                // No discs in this coordinate.
                // Do we have a candidate vertex yet?
                if (! ans) {
                    // Still haven't found a candidate.
                    notAns.insert(v);
                } else if (ans == v) {
                    // This is our only candidate vertex.
                    return 0;
                }
            }

            if (coord != 0) {
                // Some discs in this coordinate.
                // Do we have a candidate vertex?
                if (! ans) {
                    // We've found our first and only possible candidate.
                    if (notAns.count(v))
                        return 0;
                    ans = v;
                    ansMult = coord;
                } else if (ans == v) {
                    // This vertex matches our current candidate.
                    if (ansMult != coord)
                        return 0;
                } else {
                    // This vertex does not match our current candidate.
                    return 0;
                }
            } else {
                // No discs in this coordinate.
                if (ans == v)
                    return 0;
            }
        }
    }

    return ans;
}

std::pair<const NEdge*, const NEdge*> NNormalSurfaceVector::isThinEdgeLink(
        NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;

    // Check that there are no octahedral discs.
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return NO_EDGES;

    // Run through the quadrilateral discs and work out if there are any
    // valid candidates.
    stdhash::hash_set<NEdge*, HashPointer> notAns;
        /**< We will ignore notAns once ans != 0. */
    bool foundQuads = false;
    const NEdge* ans[2];
    NLargeInteger ansMultDouble;

    NTetrahedron* t;
    NEdge* e[6]; // { 2*link, 4*intersect }
    NLargeInteger coord;
    int i;

    for (tet = 0; tet < nTets; tet++) {
        t = triang->getTetrahedron(tet);
        for (type = 0; type < 3; type++) {
            coord = getQuadCoord(tet, type, triang);
            e[0] = t->getEdge(edgeNumber[vertexSplitDefn[type][0]]
                [vertexSplitDefn[type][1]]);
            e[1] = t->getEdge(edgeNumber[vertexSplitDefn[type][2]]
                [vertexSplitDefn[type][3]]);
            e[2] = t->getEdge(edgeNumber[vertexSplitDefn[type][0]]
                [vertexSplitDefn[type][2]]);
            e[3] = t->getEdge(edgeNumber[vertexSplitDefn[type][0]]
                [vertexSplitDefn[type][3]]);
            e[4] = t->getEdge(edgeNumber[vertexSplitDefn[type][1]]
                [vertexSplitDefn[type][2]]);
            e[5] = t->getEdge(edgeNumber[vertexSplitDefn[type][1]]
                [vertexSplitDefn[type][3]]);

            if (coord == 0) {
                // No discs in this coordinate.
                // Do we have any candidate edges yet?
                if (foundQuads) {
                    // Rule out any candidates that should have discs here.
                    for (i = 0; i < 2; i++)
                        if (ans[i] == e[0] || ans[i] == e[1])
                            ans[i] = 0;
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
                            ans[1] = 0;
                        else if (e[0] == ans[1]) {
                            ans[0] = ans[1];
                            ans[1] = 0;
                        } else
                            return NO_EDGES;

                        // The only possible candidate is ans[0].
                        if (ans[0] == 0 || ansMultDouble != coord)
                            return NO_EDGES;
                    } else {
                        // Different edges on both sides of the quad.
                        // Check each candidate in turn.
                        for (i = 0; i < 2; i++)
                            if (ans[i] != e[0] && ans[i] != e[1])
                                ans[i] = 0;
                        if (ansMultDouble != coord * 2)
                            return NO_EDGES;
                    }
                } else {
                    // We've found our first and only possible candidates.
                    if (e[0] == e[1]) {
                        // Same edge on both sides of the quad.
                        if (notAns.count(e[0]))
                            return NO_EDGES;
                        ans[0] = e[0];
                        ans[1] = 0;
                        ansMultDouble = coord;
                    } else {
                        // Different edges on both sides of the quad.
                        for (i = 0; i < 2; i++) {
                            if (notAns.count(e[i]))
                                ans[i] = 0;
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
                        ans[0] = 0;
                    if (ans[1] == e[i])
                        ans[1] = 0;
                }
            }

            // Have we ruled out all the candidates we ever had?
            if (foundQuads && (! ans[0]) && (! ans[1]))
                return NO_EDGES;
        }
    }

    // So did we actually find anything?
    if (! foundQuads)
        return NO_EDGES;
    if ((! ans[0]) && (! ans[1]))
        return NO_EDGES;

    // Finally check the triangular discs.
    NVertex* v;
    bool expectZero[2];
    int j;
    for (tet = 0; tet < nTets; tet++) {
        t = triang->getTetrahedron(tet);
        for (type = 0; type < 4; type++) {
            v = t->getVertex(type);
            coord = getTriangleCoord(tet, type, triang);

            // Should we actually see any discs?
            for (i = 0; i < 2; i++) {
                if (! ans[i])
                    continue;

                // If the candidate edge does not touch this vertex, the
                // triangular coordinate should be 0.
                expectZero[i] = (v != ans[i]->getVertex(0) &&
                    v != ans[i]->getVertex(1));

                // If this triangular disc type intersects the candidate
                // edge, the coordinate should also be 0.
                if (! expectZero[i])
                    for (j = 0; j < 3; j++)
                        if (t->getEdge(edgeNumber[type][(type + j + 1) % 4])
                                == ans[i]) {
                            expectZero[i] = true;
                            break;
                        }

                // So did we get the right triangular coordinate?
                if (expectZero[i]) {
                    if (coord != 0)
                        ans[i] = 0;
                } else {
                    if (ansMultDouble != coord * 2)
                        ans[i] = 0;
                }
            }

            // Have we ruled out all possibilities?
            if ((! ans[0]) && (! ans[1]))
                return NO_EDGES;
        }
    }

    // Return whatever candidates have survived.
    if (ans[0])
        return std::make_pair(ans[0], ans[1]);
    else
        return std::make_pair(ans[1], ans[0]);
}

} // namespace regina

