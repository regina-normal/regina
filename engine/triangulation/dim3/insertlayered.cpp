
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <numeric> // for std::gcd()
#include "manifold/lensspace.h"
#include "manifold/sfs.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

namespace regina {

Tetrahedron<3>* Triangulation<3>::layerOn(Edge<3>* edge) {
    if (! edge->isBoundary())
        throw InvalidArgument("layerOn() requires a boundary edge");

    // Locate the two boundary triangles.
    // Note that our preconditions ensure they exist and are distinct;
    // we won't test this again here.
    Tetrahedron<3>* tet1 = edge->front().tetrahedron();
    Tetrahedron<3>* tet2 = edge->back().tetrahedron();

    Perm<4> roles1 = edge->front().vertices();
    Perm<4> roles2 = edge->back().vertices();

    // At this stage, roles1 maps (0,1,2) to the tet1 tetrahedron vertices
    // for the first boundary triangle, and roles2 maps (0,1,3) to the tet2
    // tetrahedron vertices for the second boundary triangle.  In each case,
    // (0,1) maps to the endpoints of the given edge.
    //
    // The simplest thing to do is let (0,1,2,3) in the preimages for
    // roles1 and roles2 match up with vertices (0,1,2,3) of the new
    // tetrahedron.

    if (tet1->triangle(roles1[3]) == tet2->triangle(roles2[2]))
        throw InvalidArgument("layerOn() requires an edge between two "
            "distinct boundary triangles");
    if (tet1->isFacetLocked(roles1[3]) || tet2->isFacetLocked(roles2[2]))
        throw LockViolation("An attempt was made to layer on a locked facet");

    // Note: we use the "raw" routines (joinRaw, newSimplexRaw), mainly since
    // we did all our lock checking beforehand (not during the individual
    // joins).  This means that the ChangeAndClearSpan here is vital.
    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    Tetrahedron<3>* newTet = newSimplexRaw();

    newTet->joinRaw(3, tet1, roles1);
    newTet->joinRaw(2, tet2, roles2);

    return newTet;
}

bool Triangulation<3>::fillTorus(size_t cuts0, size_t cuts1, size_t cuts2,
        BoundaryComponent<3>* bc) {
    // Check that the cuts arguments are valid.
    int maxCuts;
    if (cuts2 == cuts0 + cuts1)
        maxCuts = 2;
    else if (cuts1 == cuts0 + cuts2)
        maxCuts = 1;
    else if (cuts0 == cuts1 + cuts2)
        maxCuts = 0;
    else
        return false;

    if (std::gcd(cuts0, cuts1) != 1)
        return false;

    // Deduce the boundary component if one was not given.
    if (! bc) {
        if (countBoundaryComponents() != 1)
            return false;
        bc = boundaryComponents_.front();
    }

    // Check that the boundary component is indeed a 2-triangle torus.
    if (bc->countTriangles() != 2)
        return false;
    if (bc->eulerChar() != 0 || ! bc->isOrientable())
        return false;

    // Identify the two boundary triangles and their relationships to the
    // three boundary edges.
    //
    // For each i = 0,1, we require that vertices (v[i][0], v[i][1], v[i][2])
    // of triangle t[i] form a boundary triangle, with v[i][k] opposite edge k
    // of the given boundary component.
    Tetrahedron<3>* t[2];
    Perm<4> v[2];

    Edge<3>* e = bc->edge(0);
    const EdgeEmbedding<3>& emb0 = e->front();
    const EdgeEmbedding<3>& emb1 = e->back();

    t[0] = emb0.simplex();
    t[1] = emb1.simplex();
    // emb0.vertices(): 0,1 -> bc->edge(0); 2 -> other bc vertex.
    // emb1.vertices(): 0,1 -> bc->edge(0); 3 -> other bc vertex.
    if (t[0]->edge(emb0.vertices()[0], emb0.vertices()[2]) == bc->edge(1)) {
        // emb0.vertices(): 0,2 -> bc->edge(1), 1,2 -> bc->edge(2).
        // emb1.vertices(): 1,3 -> bc->edge(1), 0,3 -> bc->edge(2).
        v[0] = emb0.vertices() * Perm<4>(2, 1, 0, 3);
        v[1] = emb1.vertices() * Perm<4>(3, 0, 1, 2);
    } else {
        // emb0.vertices(): 1,2 -> bc->edge(1), 0,2 -> bc->edge(2).
        // emb1.vertices(): 0,3 -> bc->edge(1), 1,3 -> bc->edge(2).
        v[0] = emb0.vertices() * Perm<4>(2, 0, 1, 3);
        v[1] = emb1.vertices() * Perm<4>(3, 1, 0, 2);
    }

    // Build and attach the solid torus.
    Tetrahedron<3>* filling;
    switch (maxCuts) {
        case 0:
            if (cuts1 <= cuts2) {
                filling = insertLayeredSolidTorus(cuts1, cuts2);
                if (cuts0 <= 2) {
                    // filling:12,03 -> bc->edge(2)
                    // filling:02,13 -> bc->edge(0)
                    // filling:01    -> bc->edge(1)
                    filling->join(3, t[0],
                        Perm<4>(v[0][2], v[0][0], v[0][1], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][0], v[1][2], v[1][3], v[1][1]));
                } else {
                    // filling:12,03 -> bc->edge(1)
                    // filling:02,13 -> bc->edge(2)
                    // filling:01    -> bc->edge(0)
                    filling->join(3, t[0],
                        Perm<4>(v[0][1], v[0][2], v[0][0], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][2], v[1][1], v[1][3], v[1][0]));
                }
            } else {
                filling = insertLayeredSolidTorus(cuts2, cuts1);
                if (cuts0 <= 2) {
                    // filling:12,03 -> bc->edge(1)
                    // filling:02,13 -> bc->edge(0)
                    // filling:01    -> bc->edge(2)
                    filling->join(3, t[0],
                        Perm<4>(v[0][1], v[0][0], v[0][2], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][0], v[1][1], v[1][3], v[1][2]));
                } else {
                    // filling:12,03 -> bc->edge(2)
                    // filling:02,13 -> bc->edge(1)
                    // filling:01    -> bc->edge(0)
                    filling->join(3, t[0],
                        Perm<4>(v[0][2], v[0][1], v[0][0], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][1], v[1][2], v[1][3], v[1][0]));
                }
            }
            break;
        case 1:
            if (cuts0 <= cuts2) {
                filling = insertLayeredSolidTorus(cuts0, cuts2);
                if (cuts1 <= 2) {
                    // filling:12,03 -> bc->edge(2)
                    // filling:02,13 -> bc->edge(1)
                    // filling:01    -> bc->edge(0)
                    filling->join(3, t[0],
                        Perm<4>(v[0][2], v[0][1], v[0][0], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][1], v[1][2], v[1][3], v[1][0]));
                } else {
                    // filling:12,03 -> bc->edge(0)
                    // filling:02,13 -> bc->edge(2)
                    // filling:01    -> bc->edge(1)
                    filling->join(3, t[0],
                        Perm<4>(v[0][0], v[0][2], v[0][1], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][2], v[1][0], v[1][3], v[1][1]));
                }
            } else {
                filling = insertLayeredSolidTorus(cuts2, cuts0);
                if (cuts1 <= 2) {
                    // filling:12,03 -> bc->edge(0)
                    // filling:02,13 -> bc->edge(1)
                    // filling:01    -> bc->edge(2)
                    filling->join(3, t[0],
                        Perm<4>(v[0][0], v[0][1], v[0][2], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][1], v[1][0], v[1][3], v[1][2]));
                } else {
                    // filling:12,03 -> bc->edge(2)
                    // filling:02,13 -> bc->edge(0)
                    // filling:01    -> bc->edge(1)
                    filling->join(3, t[0],
                        Perm<4>(v[0][2], v[0][0], v[0][1], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][0], v[1][2], v[1][3], v[1][1]));
                }
            }
            break;
        case 2:
            if (cuts0 <= cuts1) {
                filling = insertLayeredSolidTorus(cuts0, cuts1);
                if (cuts2 <= 2) {
                    // filling:12,03 -> bc->edge(1)
                    // filling:02,13 -> bc->edge(2)
                    // filling:01    -> bc->edge(0)
                    filling->join(3, t[0],
                        Perm<4>(v[0][1], v[0][2], v[0][0], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][2], v[1][1], v[1][3], v[1][0]));
                } else {
                    // filling:12,03 -> bc->edge(0)
                    // filling:02,13 -> bc->edge(1)
                    // filling:01    -> bc->edge(2)
                    filling->join(3, t[0],
                        Perm<4>(v[0][0], v[0][1], v[0][2], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][1], v[1][0], v[1][3], v[1][2]));
                }
            } else {
                filling = insertLayeredSolidTorus(cuts1, cuts0);
                if (cuts2 <= 2) {
                    // filling:12,03 -> bc->edge(0)
                    // filling:02,13 -> bc->edge(2)
                    // filling:01    -> bc->edge(1)
                    filling->join(3, t[0],
                        Perm<4>(v[0][0], v[0][2], v[0][1], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][2], v[1][0], v[1][3], v[1][1]));
                } else {
                    // filling:12,03 -> bc->edge(1)
                    // filling:02,13 -> bc->edge(0)
                    // filling:01    -> bc->edge(2)
                    filling->join(3, t[0],
                        Perm<4>(v[0][1], v[0][0], v[0][2], v[0][3]));
                    filling->join(2, t[1],
                        Perm<4>(v[1][0], v[1][1], v[1][3], v[1][2]));
                }
            }
            break;
    }

    intelligentSimplify();
    return true;
}

bool Triangulation<3>::fillTorus(Edge<3>* e0, Edge<3>* e1, Edge<3>* e2,
        size_t cuts0, size_t cuts1, size_t cuts2) {
    if (e0 == e1 || e0 == e2 || e1 == e2)
        return false;

    BoundaryComponent<3>* bc = e0->boundaryComponent();
    if ((! bc) || bc != e1->boundaryComponent() ||
            bc != e2->boundaryComponent())
        return false;

    if (bc->countEdges() != 3)
        return false;

    // e0, e1 and e2 are now known to be the three distinct edges of bc.
    if (e0 == bc->edge(0)) {
        if (e1 == bc->edge(1))
            return fillTorus(cuts0, cuts1, cuts2, bc);
        else
            return fillTorus(cuts0, cuts2, cuts1, bc);
    } else if (e0 == bc->edge(1)) {
        if (e1 == bc->edge(0))
            return fillTorus(cuts1, cuts0, cuts2, bc);
        else
            return fillTorus(cuts2, cuts0, cuts1, bc);
    } else if (e0 == bc->edge(2)) {
        if (e1 == bc->edge(0))
            return fillTorus(cuts1, cuts2, cuts0, bc);
        else
            return fillTorus(cuts2, cuts1, cuts0, bc);
    }

    return false;
}

Tetrahedron<3>* Triangulation<3>::insertLayeredSolidTorus(
        size_t cuts0, size_t cuts1) {
    if (cuts0 > cuts1)
        throw InvalidArgument("insertLayeredSolidTorus() requires "
            "cuts0 ≤ cuts1");

    // Note: we use the "raw" routines (joinRaw, newSimplexRaw, etc.),
    // mainly so that deleting tetrahedra is easy in the case where
    // the arguments were not coprime and we have to unwind the operation.
    // This means that the ChangeAndClearSpan here is vital.
    ChangeAndClearSpan<> span(*this);

    size_t cuts2 = cuts0 + cuts1;

    if (cuts2 < 3) {
        // These are the degenerate cases.
        // Valid options: 0-1-1, 1-1-2
        // Invalid options: 0-0-0, 0-2-2

        if (cuts1 != 1)
            throw InvalidArgument("insertLayeredSolidTorus() requires "
                "cuts0 and cuts1 to be coprime");

        if (cuts2 == 2) {
            auto [top, base] = newSimplicesRaw<2>();
            base->joinRaw(0, base, {1,2,3,0});
            base->joinRaw(2, top, {2,3,0,1});
            base->joinRaw(3, top, {2,3,0,1});
            return top;
        } else {
            auto [top, middle, base] = newSimplicesRaw<3>();
            base->joinRaw(0, base, {1,2,3,0});
            base->joinRaw(2, middle, {2,3,0,1});
            base->joinRaw(3, middle, {2,3,0,1});
            middle->joinRaw(2, top, {0,2,1,3});
            middle->joinRaw(3, top, {3,1,2,0});
            return top;
        }
    }

    // This is a standard case that begins with a 1-2-3 LST and works up.

    Tetrahedron<3>* top = newSimplexRaw();
    Tetrahedron<3>* curr = top;
    while (cuts0 > 0 && cuts2 > 3) {
        // Work our way down to the 1-2-3 case.
        Tetrahedron<3>* next = newSimplexRaw();
        if (cuts1 - cuts0 > cuts0) {
            next->joinRaw(2, curr, Perm<4>(0,2,1,3));
            next->joinRaw(3, curr, Perm<4>(3,1,2,0));

            // Remaining to build: (cuts0, cuts1 - cuts0, cuts1)
            cuts2 = cuts1;
            cuts1 = cuts1 - cuts0;
        } else {
            next->joinRaw(2, curr, Perm<4>(3,1,0,2));
            next->joinRaw(3, curr, Perm<4>(0,2,3,1));

            // Remaining to build: (cuts1 - cuts0, cuts0, cuts1)
            cuts2 = cuts1;
            cuts1 = cuts0;
            cuts0 = cuts2 - cuts1;
        }
        curr = next;
    }

    // One of two things happens at this point:
    // - we successfully worked our way down to 1-2-3; or
    // - we worked our way down to 0-k-k for some k > 1, which means that
    //   the arguments were not coprime.
    // Note that the (valid) 0-1-1 case was already handled earlier, in the
    // section for degenerate cases.

    if (cuts0 == 0) {
        // The arguments were not coprime.
        // Unwind the operation that we performed thus far, and throw.
        // We don't bother with isolating the tetrahedra before removal,
        // since we are deleting an entire connected component.
        auto deleteFrom = simplices_.begin() + top->markedIndex();
        for (auto it = deleteFrom; it != simplices_.end(); ++it)
            delete *it;
        simplices_.erase(deleteFrom, simplices_.end());

        throw InvalidArgument("insertLayeredSolidTorus() requires "
            "cuts0 and cuts1 to be coprime");
    }

    // Finalise the 1-2-3 LST at the base, and return.
    curr->joinRaw(0, curr, {1,2,3,0});
    return top;
}

} // namespace regina

