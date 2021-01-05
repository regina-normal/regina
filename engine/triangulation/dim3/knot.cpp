
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

#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
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
    bool fastSphere(Triangulation<3>& t) {
        // Try simplifying the triangulation - see if we can make it
        // small enough to get a precise answer.
        while (t.intelligentSimplify())
            ;

        if (t.size() <= 10) {
            // This is small enough to just find the answer exactly.
            return t.isThreeSphere();
        }

        // It seems quite hard to find a 3-sphere triangulation that
        // Regina *cannot* simplify to one tetrahedron, so at this point
        // we already suspect that t is not a 3-sphere.  However, we
        // will try a few more things before giving up.

        // Note that fundamentalGroup() already tries to simplify the
        // group presentation.
        if (t.fundamentalGroup().countGenerators() == 0) {
            // Poincare conjecture says yes!
            return true;
        }

        // At this point we suspect very strongly that t is not a 3-sphere.

        // Still: try kicking it, just in case.
        t.barycentricSubdivision();
        while (t.intelligentSimplify())
            ;
        if (t.size() <= 10)
            return t.isThreeSphere();

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
            return t.isThreeSphere();

        // Okay: the triangulation has resisted even exhaustive
        // simplification (at least at a basic level), so let's call
        // this a non-sphere and return without spending more time.
        return false;
#endif
    }
}

Edge<3>* Triangulation<3>::longitude() {
    // Basic sanity checks.  Does this look like a 1-vertex knot
    // complement with real boundary?
    if (! (isValid() && isOrientable()))
        return nullptr;
    if (countVertices() != 1)
        return nullptr;

    if (countBoundaryComponents() != 1)
        return nullptr;

    BoundaryComponent<3>* bc = boundaryComponents_.front();
    if (bc->countTriangles() != 2)
        return nullptr;
    if (bc->countEdges() != 3)
        return nullptr;

    // Locate the longitude algebraically.
    MatrixInt m(1, countEdges()); // Leave as (0,0,...,0)
    MatrixInt n(countEdges(), countTriangles());

    int j;
    size_t e;
    for (auto t : triangles()) {
        for (j = 0; j < 3; ++j) {
            e = t->edge(j)->index();
            if (t->edgeMapping(j).sign() > 0)
                ++n.entry(e, t->index());
            else
                --n.entry(e, t->index());
        }
    }

    MarkedAbelianGroup a(m, n);
    if (! a.isZ())
        return nullptr;

    long longCuts[3];
    std::vector<Integer> v(countEdges()); // zero vector
    for (j = 0; j < 3; ++j) {
        v[bc->edge(j)->index()] = 1;

        // Fetch the number of times the longitude cuts this boundary
        // edge, but be careful to detect overflow.
        Integer tmp = a.snfRep(v)[0].abs();
        tmp.tryReduce();
        if (! tmp.isNative()) {
            // The result does not fit into a C/C++ long.
            std::cerr << "Overflow detected in longitude()." << std::endl;
            return nullptr;
        }
        longCuts[j] = tmp.longValue();

        v[bc->edge(j)->index()] = 0;
    }

    // std::cerr << longCuts[0] << ',' << longCuts[1] << ',' << longCuts[2]
    //     << std::endl;

    // Layer until the longitude is a boundary edge.
    // Since we are modifying the triangulation now, we must stop
    // referencing edges and start referencing tetrahedra instead.
    Tetrahedron<3>* bdryTet[3];
    int bdryEdge[3];
    for (j = 0; j < 3; ++j) {
        bdryTet[j] = bc->edge(j)->front().simplex();
        bdryEdge[j] = bc->edge(j)->front().edge();
    }

    while (longCuts[0] > 0 && longCuts[1] > 0 && longCuts[2] > 0) {
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

    // std::cerr << longCuts[0] << ',' << longCuts[1] << ',' << longCuts[2]
    //     << std::endl;

    if (longCuts[1] == 0) {
        std::swap(bdryTet[0], bdryTet[1]);
        std::swap(bdryEdge[0], bdryEdge[1]);
        std::swap(longCuts[0], longCuts[1]);
    } else if (longCuts[2] == 0) {
        std::swap(bdryTet[0], bdryTet[2]);
        std::swap(bdryEdge[0], bdryEdge[2]);
        std::swap(longCuts[0], longCuts[2]);
    }

    // std::cerr << longCuts[0] << ',' << longCuts[1] << ',' << longCuts[2]
    //     << std::endl;

    // The longitude is edge bdryEdge[0] of tetrahedron bdryTet[0].
    return bdryTet[0]->edge(bdryEdge[0]);
}

std::pair<Edge<3>*, Edge<3>*> Triangulation<3>::meridianLongitude() {
    // The easy part: find the algebraic longitude.
    // This routine also handles all our basic sanity checks.
    Edge<3>* l = longitude();
    if (! l)
        return std::make_pair(nullptr, nullptr);

    // Fetch the three boundary edges.
    // The longitude will be e[0].
    // Since we are modifying the triangulation, we must not
    // reference edge, but rather tetrahedra.
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
            Triangulation<3> t(*this);
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
            Triangulation<3> t(*this);
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

