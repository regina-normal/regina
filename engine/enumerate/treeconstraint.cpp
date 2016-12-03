
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2016, Ben Burton                                   *
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

#include "enumerate/treeconstraint.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim3.h"

namespace regina {

bool LPConstraintEuler::addRows(
        LPCol<regina::LPConstraintEuler>* col,
        const int* columnPerm, const Triangulation<3>* tri) {
    int* obj = new int[7 * tri->size()];
    unsigned tet, i;
    Perm<4> p;
    for (i = 0; i < 7 * tri->size(); ++i)
        obj[i] = 1;
    for (i = 0; i < tri->countTriangles(); ++i) {
        tet = tri->triangle(i)->front().tetrahedron()->index();
        p = tri->triangle(i)->front().vertices();
        --obj[7 * tet + p[0]];
        --obj[7 * tet + p[1]];
        --obj[7 * tet + p[2]];
        --obj[7 * tet + 4];
        --obj[7 * tet + 5];
        --obj[7 * tet + 6];
    }
    for (i = 0; i < tri->countEdges(); ++i) {
        tet = tri->edge(i)->front().tetrahedron()->index();
        p = tri->edge(i)->front().vertices();
        ++obj[7 * tet + p[0]];
        ++obj[7 * tet + p[1]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][0]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][1]];
    }

    for (i = 0; i < 7 * tri->size(); ++i)
        col[i].euler = obj[columnPerm[i]];

    col[7 * tri->size()].euler = -1;

    delete[] obj;
    return true;
}

bool LPConstraintNonSpun::addRows(
        LPCol<regina::LPConstraintNonSpun>* col,
        const int* columnPerm, const Triangulation<3>* tri) {
    // Regardless of whether the constraints are broken,
    // we need to ensure that the matrix has full rank.
    // Therefore add the coefficients for the two new variables now.
    col[3 * tri->size()].meridian = -1;
    col[3 * tri->size() + 1].longitude = -1;

    // For the time being we insist on one vertex, which must be
    // ideal with torus link.
    if (tri->countVertices() != 1 ||
            (! tri->vertex(0)->isIdeal()) ||
            (! tri->vertex(0)->isLinkOrientable()) ||
            tri->vertex(0)->linkEulerChar() != 0)
        return false;

    // Compute the two slope equations for the torus cusp, if we can.
    SnapPeaTriangulation snapPea(*tri, false);
    MatrixInt* coeffs = snapPea.slopeEquations();
    if (! coeffs)
        return false;

    // Check that SnapPea hasn't changed the triangulation on us.
    if (! snapPea.isIdenticalTo(*tri)) {
        delete coeffs;
        return false;
    }

    // All good!  Add the two slope equations as extra rows to
    // our constraint matrix.
    //
    // The coefficients here are differences of terms from
    // SnapPy's get_cusp_equation(), which works in native
    // integers; therefore we will happily convert them back to
    // native integers now.
    for (int i = 0; i < 3 * tri->size(); ++i) {
        col[i].meridian = coeffs->entry(0, columnPerm[i]).longValue();
        col[i].longitude = coeffs->entry(1, columnPerm[i]).longValue();
    }

    delete coeffs;
    return true;
}

BanConstraintBase::BanConstraintBase(const Triangulation<3>* tri, int coords) :
        tri_(tri), coords_(coords) {
    unsigned nCols = (coords == NS_QUAD || coords == NS_AN_QUAD_OCT ?
            3 * tri->size() :
        coords == NS_ANGLE ? 3 * tri->size() + 1 :
        7 * tri->size());
    banned_ = new bool[nCols];
    marked_ = new bool[nCols];
    std::fill(banned_, banned_ + nCols, false);
    std::fill(marked_, marked_ + nCols, false);
}

void BanBoundary::init(const int* columnPerm) {
    unsigned n = tri_->size();
    unsigned tet, type, i, k;

    bool quadOnly = (coords_ == NS_QUAD || coords_ == NS_AN_QUAD_OCT);

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    // Ban quadrilaterals in tetrahedra that meet the boundary
    // (every such quadrilateral meets a boundary face).
    for (i = 0; i < 3 * n; ++i) {
        if (quadOnly)
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (k = 0; k < 4; ++k)
            if (! tri_->tetrahedron(tet)->adjacentTetrahedron(k)) {
                banned_[i] = true;
                break;
            }
    }

    // Ban normal triangles in tetrahedra that meet the boundary (but
    // only those normal triangles that meet the boundary faces).
    if (! quadOnly)
        for (i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            type = columnPerm[i] % 7;

            for (k = 0; k < 4; ++k)
                if (k != type &&
                        ! tri_->tetrahedron(tet)->
                        adjacentTetrahedron(k)) {
                    banned_[i] = true;
                    break;
                }
        }
}

void BanTorusBoundary::init(const int* columnPerm) {
    unsigned n = tri_->size();
    unsigned tet, type, i, k;

    // Which boundary faces are we banning?
    unsigned nTriangles = tri_->countTriangles();
    bool* banTriangle = new bool[nTriangles];
    std::fill(banTriangle, banTriangle + nTriangles, false);

    // Which vertex links are we marking normal triangles around?
    unsigned nVertices = tri_->countVertices();
    bool* markVtx = new bool[nVertices];
    std::fill(markVtx, markVtx + nVertices, false);

    BoundaryComponent<3>* bc;
    for (i = 0; i < tri_->countBoundaryComponents(); ++i) {
        bc = tri_->boundaryComponent(i);
        if ((! bc->isIdeal()) && bc->isOrientable() &&
                bc->eulerChar() == 0) {
            // We've found a real torus boundary.
            for (k = 0; k < bc->countTriangles(); ++k)
                banTriangle[bc->triangle(k)->markedIndex()] = true;
            for (k = 0; k < bc->countVertices(); ++k)
                markVtx[bc->vertex(k)->markedIndex()] = true;
        }
    }

    bool quadOnly = (coords_ == NS_QUAD || coords_ == NS_AN_QUAD_OCT);

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    // Ban quadrilaterals that touch torus boundaries.
    for (i = 0; i < 3 * n; ++i) {
        if (quadOnly)
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (k = 0; k < 4; ++k)
            if (banTriangle[tri_->tetrahedron(tet)->triangle(k)->
                    markedIndex()]) {
                banned_[i] = true;
                break;
            }
    }

    // Ban normal triangles that touch torus boundaries, and mark all
    // normal triangles that surround vertices on torus boundaries
    // (even if the normal triangles do not actually touch the boundary).
    if (! quadOnly)
        for (i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            type = columnPerm[i] % 7;

            if (markVtx[tri_->tetrahedron(tet)->vertex(type)->
                    markedIndex()])
                marked_[i] = true;

            for (k = 0; k < 4; ++k)
                if (k != type &&
                        banTriangle[tri_->tetrahedron(tet)->triangle(k)->
                        markedIndex()]) {
                    banned_[i] = true;
                    break;
                }
        }

    delete[] banTriangle;
    delete[] markVtx;
}

} // namespace regina

