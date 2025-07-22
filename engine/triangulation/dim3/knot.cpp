
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

#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "maths/numbertheory.h"
#include "snappea/snappeatriangulation.h"
#include <cstdlib>

namespace regina {

namespace {
    // A variant of 3-sphere recognition whose priority is to be fast,
    // at the expense of possibly returning a false negative.
    //
    // If this routine returns true, then t is definitely a 3-sphere.
    //
    // If this routine returns false, then we have good reason to
    // believe that t is not a 3-sphere, though we cannot be certain.
    //
    // It is assumed that t is a homology sphere (and so, in particular,
    // we do not bother computing homology).
    //
    // It is possible that this routine will retriangulate t.
    //
    // PRE: t does not have any locked simplices or facets.
    //
    bool fastSphere(Triangulation<3>& t) {
        // Try simplifying the triangulation - see if we can make it
        // small enough to get a precise answer.
        while (t.simplify())
            ;

        if (t.size() <= 10) {
            // This is small enough to just find the answer exactly.
            return t.isSphere();
        }

        // It seems quite hard to find a 3-sphere triangulation that
        // Regina *cannot* simplify to one tetrahedron, so at this point
        // we already suspect that t is not a 3-sphere.  However, we
        // will try a few more things before giving up.

        // Note that group() already tries to simplify the group presentation.
        if (t.group().countGenerators() == 0) {
            // Poincare conjecture says yes!
            return true;
        }

        // At this point we suspect very strongly that t is not a 3-sphere.

        // Still: try kicking it, just in case.
        t.subdivide();
        while (t.simplify())
            ;
        if (t.size() <= 10)
            return t.isSphere();

        // Okay: both the triangulation and the fundamental group have
        // resisted our simplification attempts - let's call this a
        // non-sphere and return without spending more time.
        return false;

#if 0
        // If SnapPea says the manifold is hyperbolic then this is not a
        // proof that is not a sphere (since SnapPea does not perform
        // exact computations), but it is reason enough to give up.
        t.orient();
        SnapPeaTriangulation s(t);
        if (! s.isNull())
            if (s.solutionType() == SnapPeaTriangulation::geometric_solution)
                return false;

        // Try once again to simplify the triangulation, but try harder
        // this time.
        t.simplifyExhaustive();
        if (t.size() <= 10)
            return t.isSphere();

        // Okay: the triangulation has resisted even exhaustive
        // simplification (at least at a basic level), so let's call
        // this a non-sphere and return without spending more time.
        return false;
#endif
    }
}

std::array<long, 3> Triangulation<3>::longitudeCuts() const {
    // Basic sanity checks.  Does this look like a 1-vertex knot
    // complement with real boundary?
    if (! (isValid() && isOrientable() && countVertices() == 1))
        throw FailedPrecondition("longitudeCuts() requires a "
            "valid orientable one-vertex triangulation");

    if (countBoundaryComponents() != 1)
        throw FailedPrecondition("longitudeCuts() requires a "
            "triangulation with precisely one boundary component");

    BoundaryComponent<3>* bc = boundaryComponents_.front();
    if (bc->countTriangles() != 2 || bc->countEdges() != 3)
        throw FailedPrecondition("longitudeCuts() requires a "
            "triangulation whose boundary is a two-triangle torus");

    // Locate the longitude algebraically.
    MatrixInt m(1, countEdges()); // Leave as (0,0,...,0)
    MatrixInt n(countEdges(), countTriangles());

    for (auto t : triangles()) {
        for (int j = 0; j < 3; ++j) {
            size_t e = t->edge(j)->index();
            if (t->edgeMapping(j).sign() > 0)
                ++n.entry(e, t->index());
            else
                --n.entry(e, t->index());
        }
    }

    MarkedAbelianGroup a(m, n);
    if (! a.isZ())
        throw FailedPrecondition("longitudeCuts() requires a triangulation "
            "with homology Z, as expected for a knot complement in S^3");

    std::array<long, 3> longCuts;
    Vector<Integer> v(countEdges()); // zero vector
    for (int j = 0; j < 3; ++j) {
        v[bc->edge(j)->index()] = 1;

        // Fetch the number of times the longitude cuts this boundary
        // edge, but be careful to detect overflow.
        Integer tmp = a.snfRep(v)[0].abs();
        tmp.tryReduce();
        if (! tmp.isNative()) {
            // The result does not fit into a C/C++ long.
            throw UnsolvedCase("longitude() detected an integer overflow");
        }
        longCuts[j] = tmp.longValue();

        v[bc->edge(j)->index()] = 0;
    }

    return longCuts;
}

Edge<3>* Triangulation<3>::longitude() {
    // The call to longitudeCuts() handles the necessary sanity checks.
    std::array<long, 3> longCuts = longitudeCuts();

    // std::cerr << longCuts[0] << ',' << longCuts[1] << ',' << longCuts[2]
    //     << std::endl;

    BoundaryComponent<3>* bc = boundaryComponents_.front();

    // Layer until the longitude is a boundary edge.
    // Note: if there is a lock violation, this will happen on the first
    // layering (i.e., no changes will be made).
    //
    // Since we are modifying the triangulation now, we must stop
    // referencing edges and start referencing tetrahedra instead.
    Tetrahedron<3>* bdryTet[3];
    int bdryEdge[3];
    for (int j = 0; j < 3; ++j) {
        bdryTet[j] = bc->edge(j)->front().simplex();
        bdryEdge[j] = bc->edge(j)->front().edge();
    }

    while (true) {
        if (longCuts[0] == 0)
            return bdryTet[0]->edge(bdryEdge[0]);
        if (longCuts[1] == 0)
            return bdryTet[1]->edge(bdryEdge[1]);
        if (longCuts[2] == 0)
            return bdryTet[2]->edge(bdryEdge[2]);

        if (longCuts[0] == longCuts[1] + longCuts[2]) {
            // Layer over boundary edge 0.
            bdryTet[0] = layerOn(bdryTet[0]->edge(bdryEdge[0]));
            bdryEdge[0] = 5;
            longCuts[0] = labs(longCuts[1] - longCuts[2]);
        } else if (longCuts[1] == longCuts[0] + longCuts[2]) {
            // Layer over boundary edge 1.
            bdryTet[1] = layerOn(bdryTet[1]->edge(bdryEdge[1]));
            bdryEdge[1] = 5;
            longCuts[1] = labs(longCuts[0] - longCuts[2]);
        } else {
            // Layer over boundary edge 2.
            bdryTet[2] = layerOn(bdryTet[2]->edge(bdryEdge[2]));
            bdryEdge[2] = 5;
            longCuts[2] = labs(longCuts[0] - longCuts[1]);
        }
    }
}

Edge<3>* Triangulation<3>::meridian() {
    // First work out the longitude as a triple of edge weights.
    // This call to longitudeCuts() handles the necessary sanity checks.
    std::array<long, 3> longCuts = longitudeCuts();

    // Fetch the three boundary edges, in the same order that
    // corresponds to the triple longCuts.
    BoundaryComponent<3>* bc = boundaryComponents_.front();
    Edge<3>* e[3];
    for (int i = 0; i < 3; ++i)
        e[i] = bc->edge(i);

    // Reorder the boundary edges so that longCuts is in ascending order.
    // We do this using a trivial three-element bubblesort.
    if (longCuts[0] > longCuts[1]) {
        std::swap(longCuts[0], longCuts[1]);
        std::swap(e[0], e[1]);
    }
    if (longCuts[1] > longCuts[2]) {
        std::swap(longCuts[1], longCuts[2]);
        std::swap(e[1], e[2]);
    }
    if (longCuts[0] > longCuts[1]) {
        std::swap(longCuts[0], longCuts[1]);
        std::swap(e[0], e[1]);
    }

    // At this point, longCuts[0] <= longCuts[1] <= longCuts[2].

    // Now fetch the boundary edges in (tetrahedron, edge number) form,
    // since this survives modifications to the triangulation.

    Tetrahedron<3>* bdryTet[3];
    int bdryEdge[3];
    for (int i = 0; i < 3; ++i) {
        bdryTet[i] = e[i]->front().simplex();
        bdryEdge[i] = e[i]->front().edge();
    }

    // Next work out the meridian as triple of edge weights.
    // It can be shown that:
    // - If the longitude is (0,1,1), then the meridian must be of the form
    //   (1,k,k+1) or (1,k+1,k).
    // - If the longitude is (a,b,a+b) for a,b > 0, then the meridian must be
    //   (x,y,x+y) where ay-bx = ±1.
    //
    // We will treat these two cases separately.
    // In each case, we identify the meridian by repeatedly filling along
    // candidate curves until we obtain a 3-sphere.

    long merCuts[3];

    if (longCuts[0] == 0) {
        // - The meridian is of the form (1,k,k+1) or (1,k+1,k).

        merCuts[0] = 1;
        for (long k = 0; ; ++k) {
            {
                Triangulation<3> t(*this, false, false);
                t.fillTorus(
                    t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                    t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                    t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                    1, k, k + 1);
                if (fastSphere(t)) {
                    merCuts[1] = k;
                    merCuts[2] = k + 1;
                    break;
                }
            }
            {
                Triangulation<3> t(*this, false, false);
                t.fillTorus(
                    t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                    t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                    t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                    1, k + 1, k);
                if (fastSphere(t)) {
                    merCuts[1] = k + 1;
                    merCuts[2] = k;
                    break;
                }
            }
        }
    } else {
        // The meridian is of the form (x,y,x+y) where ay-bx = ±1.

        // First find *some* integer solution to give each of +1 or -1.
        auto [d, u, v] = gcdWithCoeffs(longCuts[0], longCuts[1]);
        // We should have a * u + b * v = 1.

        // The pairs (x,y) that give +1 and -1 respectively:
        long pos[2] = { -v, u };
        long neg[2] = { v, -u };

        // For each equation (+1 and -1), all solutions can be obtained
        // from our initial solution by adding or subtracting multiples
        // of (a,b) to (x,y).  We will find the smallest non-negative solution
        // for each equation, and then iterate by repeatedly adding (a,b).

        long k0, k1, k;

        k0 = (longCuts[0] == 0 ? 0 :
                pos[0] >= 0 ? -(pos[0] / longCuts[0]) :
                -((pos[0] + 1) / longCuts[0]) + 1);
        k1 = (longCuts[1] == 0 ? 0 :
                pos[1] >= 0 ? -(pos[1] / longCuts[1]) :
                -((pos[1] + 1) / longCuts[1]) + 1);
        k = (longCuts[0] == 0 ? k1 : longCuts[1] == 0 ? k0 : std::max(k0, k1));
        pos[0] += (k * longCuts[0]);
        pos[1] += (k * longCuts[1]);

        k0 = (longCuts[0] == 0 ? 0 :
                neg[0] >= 0 ? -(neg[0] / longCuts[0]) :
                -((neg[0] + 1) / longCuts[0]) + 1);
        k1 = (longCuts[1] == 0 ? 0 :
                neg[1] >= 0 ? -(neg[1] / longCuts[1]) :
                -((neg[1] + 1) / longCuts[1]) + 1);
        k = (longCuts[0] == 0 ? k1 : longCuts[1] == 0 ? k0 : std::max(k0, k1));
        neg[0] += (k * longCuts[0]);
        neg[1] += (k * longCuts[1]);

        while (true) {
            {
                Triangulation<3> t(*this, false, false);
                t.fillTorus(
                    t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                    t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                    t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                    pos[0], pos[1], pos[0] + pos[1]);
                if (fastSphere(t)) {
                    merCuts[0] = pos[0];
                    merCuts[1] = pos[1];
                    merCuts[2] = pos[0] + pos[1];
                    break;
                }
            }
            {
                Triangulation<3> t(*this, false, false);
                t.fillTorus(
                    t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                    t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                    t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                    neg[0], neg[1], neg[0] + neg[1]);
                if (fastSphere(t)) {
                    merCuts[0] = neg[0];
                    merCuts[1] = neg[1];
                    merCuts[2] = neg[0] + neg[1];
                    break;
                }
            }
            pos[0] += longCuts[0];
            pos[1] += longCuts[1];
            neg[0] += longCuts[0];
            neg[1] += longCuts[1];
        }
    }

    // Now layer so that the meridian is a boundary edge.
    // Note: if there is a lock violation, this will happen on the first
    // layering (i.e., no changes will be made).
    while (true) {
        if (merCuts[0] == 0)
            return bdryTet[0]->edge(bdryEdge[0]);
        if (merCuts[1] == 0)
            return bdryTet[1]->edge(bdryEdge[1]);
        if (merCuts[2] == 0)
            return bdryTet[2]->edge(bdryEdge[2]);

        if (merCuts[0] == merCuts[1] + merCuts[2]) {
            // Layer over boundary edge 0.
            bdryTet[0] = layerOn(bdryTet[0]->edge(bdryEdge[0]));
            bdryEdge[0] = 5;
            merCuts[0] = labs(merCuts[1] - merCuts[2]);
        } else if (merCuts[1] == merCuts[0] + merCuts[2]) {
            // Layer over boundary edge 1.
            bdryTet[1] = layerOn(bdryTet[1]->edge(bdryEdge[1]));
            bdryEdge[1] = 5;
            merCuts[1] = labs(merCuts[0] - merCuts[2]);
        } else {
            // Layer over boundary edge 2.
            bdryTet[2] = layerOn(bdryTet[2]->edge(bdryEdge[2]));
            bdryEdge[2] = 5;
            merCuts[2] = labs(merCuts[0] - merCuts[1]);
        }
    }
}

std::pair<Edge<3>*, Edge<3>*> Triangulation<3>::meridianLongitude() {
    // As with longitude() and meridian(), if there is a lock violation,
    // this will happen on the first layering (i.e., no changes will be made).

    // The easy part: find the algebraic longitude.
    // This routine also handles all our basic sanity checks.
    Edge<3>* l = longitude();

    // Fetch the three boundary edges.
    // The longitude will be e[0].
    // Since we are modifying the triangulation, we must not
    // reference edges, but rather tetrahedra.
    BoundaryComponent<3>* bc = boundaryComponents_.front();
    Edge<3>* e[3];

    int i;
    for (i = 0; i < 3; ++i)
        e[i] = bc->edge(i);

    if (l == e[1])
        std::swap(e[0], e[1]);
    else if (l == e[2])
        std::swap(e[0], e[2]);

    Tetrahedron<3>* bdryTet[3];
    int bdryEdge[3];
    for (i = 0; i < 3; ++i) {
        bdryTet[i] = e[i]->front().simplex();
        bdryEdge[i] = e[i]->front().edge();
    }

    // Our next task is to find the meridian.
    // We do this by repeatedly filling along candidate curves until we
    // obtain a 3-sphere.
    // A candidate curve must cut the longitude exactly once.
    for (long merCut = 0; ; ++merCut) {
        {
            // std::cerr << "Trying filling: " << 1 << ',' << merCut << ','
            //     << (merCut + 1) << std::endl;
            Triangulation<3> t(*this, false, false);
            t.fillTorus(
                t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                1, merCut, merCut + 1);
            // std::cerr << "Filled: " << t.isoSig() << std::endl;
            if (fastSphere(t)) {
                while (merCut > 0) {
                    // Layer over boundary edge 2.
                    bdryTet[2] = layerOn(bdryTet[2]->edge(bdryEdge[2]));
                    bdryEdge[2] = 5;
                    std::swap(bdryTet[1], bdryTet[2]);
                    std::swap(bdryEdge[1], bdryEdge[2]);
                    --merCut;
                }
                return std::make_pair(
                    simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                    simplex(bdryTet[0]->index())->edge(bdryEdge[0]));
            }
        }
        {
            // std::cerr << "Trying filling: " << 1 << ','
            //     << (merCut + 1) << ',' << merCut << std::endl;
            Triangulation<3> t(*this, false, false);
            t.fillTorus(
                t.simplex(bdryTet[0]->index())->edge(bdryEdge[0]),
                t.simplex(bdryTet[1]->index())->edge(bdryEdge[1]),
                t.simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                1, merCut + 1, merCut);
            // std::cerr << "Filled: " << t.isoSig() << std::endl;
            if (fastSphere(t)) {
                while (merCut > 0) {
                    // Layer over boundary edge 1.
                    bdryTet[1] = layerOn(bdryTet[1]->edge(bdryEdge[1]));
                    bdryEdge[1] = 5;
                    std::swap(bdryTet[1], bdryTet[2]);
                    std::swap(bdryEdge[1], bdryEdge[2]);
                    --merCut;
                }
                return std::make_pair(
                    simplex(bdryTet[2]->index())->edge(bdryEdge[2]),
                    simplex(bdryTet[0]->index())->edge(bdryEdge[0]));
            }
        }
    }
}

} // namespace regina

