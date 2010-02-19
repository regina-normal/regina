
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

#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurface.h"

namespace regina {

bool NNormalHypersurfaceVector::isVertexLinking(Dim4Triangulation* triang)
        const {
    unsigned long nPents = triang->getNumberOfPentachora();
    unsigned long pent;
    int type;
    for (pent = 0; pent < nPents; pent++) {
        for (type = 0; type < 10; type++)
            if (getPrismCoord(pent, type, triang) != 0)
                return false;
    }
    return true;
}

const Dim4Vertex* NNormalHypersurfaceVector::isVertexLink(
        Dim4Triangulation* triang) const {
    unsigned long nPents = triang->getNumberOfPentachora();
    unsigned long pent;
    int type;

    // Check that there are no prism pieces.
    for (pent = 0; pent < nPents; pent++) {
        for (type = 0; type < 10; type++)
            if (getPrismCoord(pent, type, triang) != 0)
                return 0;
    }

    // Now examine the tetrahedra to see if we link only a single vertex.
    Dim4Vertex* ans = 0;
    NLargeInteger ansMult;

    Dim4Pentachoron* p;
    NLargeInteger coord;

    for (pent = 0; pent < nPents; pent++) {
        p = triang->getPentachoron(pent);
        for (type = 0; type < 5; type++) {
            coord = getTetrahedronCoord(pent, type, triang);

            if (coord != 0) {
                // Some tetrahedron discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->getVertex(type);
                    ansMult = coord;
                } else if (ans != p->getVertex(type)) {
                    // We seem to be linking more than one vertex.
                    return 0;
                }
            }
        }
    }

    // Did we find any candidate vertices at all?
    if (! ans)
        return 0;

    // If the matching equations are satisfied, we are done at this
    // point.  Specifically, the only normal pieces we have are those
    // surrounding vertex ans.  However, although it is already implied
    // by the matching equations, let's just ensure the number
    // of pieces of each type is the same.
    for (unsigned long e = 0; e < ans->getNumberOfEmbeddings(); ++e)
        if (ansMult != getTetrahedronCoord(
                triang->pentachoronIndex(ans->getEmbedding(e).getPentachoron()),
                ans->getEmbedding(e).getVertex(), triang))
            return 0;

    // All good.
    return ans;
}

const Dim4Edge* NNormalHypersurfaceVector::isThinEdgeLink(
        Dim4Triangulation* triang) const {
    unsigned long nPents = triang->getNumberOfPentachora();
    unsigned long pent;
    int type;

    // Search through prism pieces for one and only one candidate edge.
    Dim4Edge* ans = 0;
    NLargeInteger ansMult;

    Dim4Pentachoron* p;
    NLargeInteger coord;

    for (pent = 0; pent < nPents; pent++) {
        p = triang->getPentachoron(pent);
        for (type = 0; type < 10; type++) {
            coord = getPrismCoord(pent, type, triang);

            if (coord != 0) {
                // Some prism discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->getEdge(type);
                    ansMult = coord;
                } else if (ans != p->getEdge(type)) {
                    // We seem to be linking more than one edge.
                    return 0;
                }
            }
        }
    }

    // Did we find any candidate edges at all?
    if (! ans)
        return 0;

    // There are no unwanted prism piece types.  However, we must still
    // run through the prism types that do appear to make sure that they
    // each appear with the same multiple.
    for (unsigned long e = 0; e < ans->getNumberOfEmbeddings(); ++e)
        if (ansMult != getPrismCoord(
                triang->pentachoronIndex(ans->getEmbedding(e).getPentachoron()),
                ans->getEmbedding(e).getEdge(), triang))
            return 0;

    // Finally, run through the tetrahedron piece types and make sure
    // that everything checks out.
    Dim4Vertex* v;
    bool crosses;
    int i;
    for (pent = 0; pent < nPents; pent++) {
        p = triang->getPentachoron(pent);
        for (type = 0; type < 5; type++) {
            v = p->getVertex(type);

            if (ans->getVertex(0) == v || ans->getVertex(1) == v) {
                // We should see tetrahedra here, but only if none of
                // the four pentachoron edges touching this vertex are
                // the same edge as ans.
                crosses = false;
                for (i = 0; i < 5; ++i) {
                    if (i == type)
                        continue;
                    if (p->getEdge(Dim4Edge::edgeNumber[type][i]) == ans) {
                        crosses = true;
                        break;
                    }
                }

                if (crosses) {
                    if (getTetrahedronCoord(pent, type, triang) != 0)
                        return 0;
                } else {
                    if (getTetrahedronCoord(pent, type, triang) != ansMult)
                        return 0;
                }
            } else {
                if (getTetrahedronCoord(pent, type, triang) != 0)
                    return 0;
            }
        }
    }

    // All good!
    return ans;
}

} // namespace regina

