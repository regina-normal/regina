
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2021, Ben Burton                                   *
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

/*! \file enumerate/treeconstraint-impl.h
 *  \brief Contains implementation details for the various linear programming
 *  constraint classes.
 *
 *  This file is \e not included automatically by treeconstraint.h.
 *  However, most end users should not need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of these classes for common combinations of template arguments.
 */

#ifndef __REGINA_TREECONSTRAINT_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_TREECONSTRAINT_IMPL_H
#endif

#include "enumerate/treeconstraint.h"
#include "triangulation/dim3.h"

namespace regina {

template <class LPConstraint>
BanConstraintBase::BanConstraintBase(
        const LPInitialTableaux<LPConstraint>& init) :
        tri_(init.tri()), system_(init.system()) {
    const size_t nCols = system_.coords(tri_.size());
    banned_ = new bool[nCols];
    marked_ = new bool[nCols];
    std::fill(banned_, banned_ + nCols, false);
    std::fill(marked_, marked_ + nCols, false);
}

template <class LPConstraint>
BanBoundary::BanBoundary(const LPInitialTableaux<LPConstraint>& init) :
        BanConstraintBase(init) {
    unsigned n = tri_.size();
    unsigned tet, type, i, k;

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    const int* columnPerm = init.columnPerm();

    // Ban quadrilaterals in tetrahedra that meet the boundary
    // (every such quadrilateral meets a boundary face).
    for (i = 0; i < 3 * n; ++i) {
        if (system_.quad())
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (k = 0; k < 4; ++k)
            if (! tri_.tetrahedron(tet)->adjacentTetrahedron(k)) {
                banned_[i] = true;
                break;
            }
    }

    // Ban normal triangles in tetrahedra that meet the boundary (but
    // only those normal triangles that meet the boundary faces).
    if (system_.standard())
        for (i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            type = columnPerm[i] % 7;

            for (k = 0; k < 4; ++k)
                if (k != type &&
                        ! tri_.tetrahedron(tet)->adjacentTetrahedron(k)) {
                    banned_[i] = true;
                    break;
                }
        }
}

template <class LPConstraint>
BanTorusBoundary::BanTorusBoundary(
        const LPInitialTableaux<LPConstraint>& init) :
        BanConstraintBase(init) {
    unsigned n = tri_.size();
    unsigned tet, type, i, k;

    // Which boundary faces are we banning?
    unsigned nTriangles = tri_.countTriangles();
    bool* banTriangle = new bool[nTriangles];
    std::fill(banTriangle, banTriangle + nTriangles, false);

    // Which vertex links are we marking normal triangles around?
    unsigned nVertices = tri_.countVertices();
    bool* markVtx = new bool[nVertices];
    std::fill(markVtx, markVtx + nVertices, false);

    BoundaryComponent<3>* bc;
    for (i = 0; i < tri_.countBoundaryComponents(); ++i) {
        bc = tri_.boundaryComponent(i);
        if ((! bc->isIdeal()) && bc->isOrientable() &&
                bc->eulerChar() == 0) {
            // We've found a real torus boundary.
            for (k = 0; k < bc->countTriangles(); ++k)
                banTriangle[bc->triangle(k)->markedIndex()] = true;
            for (k = 0; k < bc->countVertices(); ++k)
                markVtx[bc->vertex(k)->markedIndex()] = true;
        }
    }

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    const int* columnPerm = init.columnPerm();

    // Ban quadrilaterals that touch torus boundaries.
    for (i = 0; i < 3 * n; ++i) {
        if (system_.quad())
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (k = 0; k < 4; ++k)
            if (banTriangle[tri_.tetrahedron(tet)->triangle(k)->
                    markedIndex()]) {
                banned_[i] = true;
                break;
            }
    }

    // Ban normal triangles that touch torus boundaries, and mark all
    // normal triangles that surround vertices on torus boundaries
    // (even if the normal triangles do not actually touch the boundary).
    if (system_.standard())
        for (i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            type = columnPerm[i] % 7;

            if (markVtx[tri_.tetrahedron(tet)->vertex(type)->
                    markedIndex()])
                marked_[i] = true;

            for (k = 0; k < 4; ++k)
                if (k != type &&
                        banTriangle[tri_.tetrahedron(tet)->triangle(k)->
                        markedIndex()]) {
                    banned_[i] = true;
                    break;
                }
        }

    delete[] banTriangle;
    delete[] markVtx;
}

} // namespace regina

#endif

