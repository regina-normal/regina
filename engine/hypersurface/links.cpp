
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

bool NormalHypersurface::isVertexLinking() const {
    if (! enc_.couldBeVertexLink())
        return false;

    size_t nPents = triangulation_->size();
    for (size_t pent = 0; pent < nPents; pent++) {
        for (int type = 0; type < 10; type++)
            if (prisms(pent, type) != 0)
                return false;
    }
    return true;
}

const Vertex<4>* NormalHypersurface::isVertexLink() const {
    if (! enc_.couldBeVertexLink())
        return nullptr;

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<4>& tri(*triangulation_);
    size_t nPents = tri.size();

    // Check that there are no prism pieces.
    for (size_t pent = 0; pent < nPents; pent++) {
        for (int type = 0; type < 10; type++)
            if (prisms(pent, type) != 0)
                return nullptr;
    }

    // It follows from the matching equations that what we have is a
    // union of vertex links.  Make sure we are linking just the one vertex.

    Vertex<4>* ans = nullptr;

    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 5; type++) {
            LargeInteger coord = tetrahedra(pent, type);

            if (coord != 0) {
                // Some tetrahedron discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->vertex(type);
                } else if (ans != p->vertex(type)) {
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

const Edge<4>* NormalHypersurface::isThinEdgeLink() const {
    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<4>& tri(*triangulation_);
    size_t nPents = tri.size();

    // Search through prism pieces for one and only one candidate edge.
    Edge<4>* ans = nullptr;
    LargeInteger ansMult;

    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 10; type++) {
            LargeInteger coord = prisms(pent, type);

            if (coord != 0) {
                // Some prism discs of this type.
                if (! ans) {
                    // We've found our first and only possible candidate.
                    ans = p->edge(type);
                    ansMult = coord;
                } else if (ans != p->edge(type)) {
                    // We seem to be linking more than one edge.
                    return nullptr;
                }
            }
        }
    }

    // Did we find any candidate edges at all?
    if (! ans)
        return nullptr;

    // There are no unwanted prism piece types.  However, we must still
    // run through the prism types that do appear to make sure that they
    // each appear with the same multiple.
    for (size_t e = 0; e < ans->degree(); ++e)
        if (ansMult != prisms(
                ans->embedding(e).pentachoron()->index(),
                ans->embedding(e).edge()))
            return nullptr;

    // Finally, run through the tetrahedron piece types and make sure
    // that everything checks out.
    for (size_t pent = 0; pent < nPents; pent++) {
        const Pentachoron<4>* p = tri.pentachoron(pent);
        for (int type = 0; type < 5; type++) {
            Vertex<4>* v = p->vertex(type);

            if (ans->vertex(0) == v || ans->vertex(1) == v) {
                // We should see tetrahedra here, but only if none of
                // the four pentachoron edges touching this vertex are
                // the same edge as ans.
                bool crosses = false;
                for (int i = 0; i < 5; ++i) {
                    if (i == type)
                        continue;
                    if (p->edge(Edge<4>::edgeNumber[type][i]) == ans) {
                        crosses = true;
                        break;
                    }
                }

                if (crosses) {
                    if (tetrahedra(pent, type) != 0)
                        return nullptr;
                } else {
                    if (tetrahedra(pent, type) != ansMult)
                        return nullptr;
                }
            } else {
                if (tetrahedra(pent, type) != 0)
                    return nullptr;
            }
        }
    }

    // All good!
    return ans;
}

std::pair<const Triangle<4>*, const Triangle<4>*>
        NormalHypersurface::isThinTriangleLink() const {
    // TODO: Implement this.
    return { nullptr, nullptr };
}

} // namespace regina

