
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

#include "hypersurface/normalhypersurface.h"
#include "triangulation/dim4.h"

namespace regina {

bool NormalHypersurfaceVector::isVertexLinking(const Triangulation<4>* triang)
        const {
    size_t nPents = triang->size();
    size_t pent;
    int type;
    for (pent = 0; pent < nPents; pent++) {
        for (type = 0; type < 10; type++)
            if (prisms(pent, type, triang) != 0)
                return false;
    }
    return true;
}

const Vertex<4>* NormalHypersurfaceVector::isVertexLink(
        const Triangulation<4>* triang) const {
    size_t nPents = triang->size();
    size_t pent;
    int type;

    // Check that there are no prism pieces.
    for (pent = 0; pent < nPents; pent++) {
        for (type = 0; type < 10; type++)
            if (prisms(pent, type, triang) != 0)
                return 0;
    }

    // Now examine the tetrahedra to see if we link only a single vertex.
    Vertex<4>* ans = 0;
    LargeInteger ansMult;

    const Pentachoron<4>* p;
    LargeInteger coord;

    for (pent = 0; pent < nPents; pent++) {
        p = triang->pentachoron(pent);
        for (type = 0; type < 5; type++) {
            coord = tetrahedra(pent, type, triang);

            if (coord != 0) {
                // Some tetrahedron discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->vertex(type);
                    ansMult = coord;
                } else if (ans != p->vertex(type)) {
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
    for (size_t e = 0; e < ans->degree(); ++e)
        if (ansMult != tetrahedra(
                ans->embedding(e).pentachoron()->index(),
                ans->embedding(e).vertex(), triang))
            return 0;

    // All good.
    return ans;
}

const Edge<4>* NormalHypersurfaceVector::isThinEdgeLink(
        const Triangulation<4>* triang) const {
    size_t nPents = triang->size();
    size_t pent;
    int type;

    // Search through prism pieces for one and only one candidate edge.
    Edge<4>* ans = 0;
    LargeInteger ansMult;

    const Pentachoron<4>* p;
    LargeInteger coord;

    for (pent = 0; pent < nPents; pent++) {
        p = triang->pentachoron(pent);
        for (type = 0; type < 10; type++) {
            coord = prisms(pent, type, triang);

            if (coord != 0) {
                // Some prism discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->edge(type);
                    ansMult = coord;
                } else if (ans != p->edge(type)) {
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
    for (size_t e = 0; e < ans->degree(); ++e)
        if (ansMult != prisms(
                ans->embedding(e).pentachoron()->index(),
                ans->embedding(e).edge(), triang))
            return 0;

    // Finally, run through the tetrahedron piece types and make sure
    // that everything checks out.
    Vertex<4>* v;
    bool crosses;
    int i;
    for (pent = 0; pent < nPents; pent++) {
        p = triang->pentachoron(pent);
        for (type = 0; type < 5; type++) {
            v = p->vertex(type);

            if (ans->vertex(0) == v || ans->vertex(1) == v) {
                // We should see tetrahedra here, but only if none of
                // the four pentachoron edges touching this vertex are
                // the same edge as ans.
                crosses = false;
                for (i = 0; i < 5; ++i) {
                    if (i == type)
                        continue;
                    if (p->edge(Edge<4>::edgeNumber[type][i]) == ans) {
                        crosses = true;
                        break;
                    }
                }

                if (crosses) {
                    if (tetrahedra(pent, type, triang) != 0)
                        return 0;
                } else {
                    if (tetrahedra(pent, type, triang) != ansMult)
                        return 0;
                }
            } else {
                if (tetrahedra(pent, type, triang) != 0)
                    return 0;
            }
        }
    }

    // All good!
    return ans;
}

} // namespace regina

