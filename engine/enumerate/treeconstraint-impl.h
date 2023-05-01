
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2023, Ben Burton                                   *
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
 *  This file is _not_ included automatically by treeconstraint.h.
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
BanBoundary::BanBoundary(const LPInitialTableaux<LPConstraint>& init) :
        BanConstraintBase(init) {
    size_t n = tri_.size();
    size_t tet;

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    const size_t* columnPerm = init.columnPerm();

    // Ban quadrilaterals in tetrahedra that meet the boundary
    // (every such quadrilateral meets a boundary face).
    for (size_t i = 0; i < 3 * n; ++i) {
        if (system_.quad())
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (int k = 0; k < 4; ++k)
            if (! tri_.tetrahedron(tet)->adjacentTetrahedron(k)) {
                banned_[i] = true;
                break;
            }
    }

    // Ban normal triangles in tetrahedra that meet the boundary (but
    // only those normal triangles that meet the boundary faces).
    if (system_.standard())
        for (size_t i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            int type = columnPerm[i] % 7;

            for (int k = 0; k < 4; ++k)
                if (k != type &&
                        ! tri_.tetrahedron(tet)->adjacentTetrahedron(k)) {
                    banned_[i] = true;
                    break;
                }
        }
}

template <class LPConstraint>
BanEdge::BanEdge(const LPInitialTableaux<LPConstraint>& init, Edge<3>* edge) :
        BanConstraintBase(init) {
    size_t n = tri_.size();
    size_t tet;
    int type;

    // The implementation here is a little inefficient (if we had the
    // inverse to columnPerm then we could just iterate through the
    // embeddings of the given edge).  However, this routine is only called
    // at the beginning of the enumeration process so no need to worry.

    const size_t* columnPerm = init.columnPerm();

    // Ban quadrilaterals that meet the given edge.
    for (size_t i = 0; i < 3 * n; ++i) {
        if (system_.quad()) {
            tet = columnPerm[i] / 3;
            type = columnPerm[i] % 3;
        } else {
            tet = columnPerm[i] / 7;
            type = (columnPerm[i] % 7) - 4;
        }

        for (int k = 0; k < 6; ++k)
            if (k != type && k != 5 - type)
                if (tri_.tetrahedron(tet)->edge(k) == edge) {
                    banned_[i] = true;
                    break;
                }
    }

    // Ban triangles that meet the given edge.
    if (system_.standard())
        for (size_t i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            type = columnPerm[i] % 7;

            for (int k = 0; k < 4; ++k)
                if (k != type)
                    if (tri_.tetrahedron(tet)->edge(
                            Edge<3>::edgeNumber[k][type]) == edge) {
                        banned_[i] = true;
                        break;
                    }
        }
}

template <class LPConstraint>
BanTorusBoundary::BanTorusBoundary(
        const LPInitialTableaux<LPConstraint>& init) :
        BanConstraintBase(init) {
    size_t n = tri_.size();
    size_t tet;

    // Which boundary faces are we banning?
    size_t nTriangles = tri_.countTriangles();
    bool* banTriangle = new bool[nTriangles];
    std::fill(banTriangle, banTriangle + nTriangles, false);

    // Which vertex links are we marking normal triangles around?
    size_t nVertices = tri_.countVertices();
    bool* markVtx = new bool[nVertices];
    std::fill(markVtx, markVtx + nVertices, false);

    BoundaryComponent<3>* bc;
    for (size_t i = 0; i < tri_.countBoundaryComponents(); ++i) {
        bc = tri_.boundaryComponent(i);
        if ((! bc->isIdeal()) && bc->isOrientable() &&
                bc->eulerChar() == 0) {
            // We've found a real torus boundary.
            for (size_t k = 0; k < bc->countTriangles(); ++k)
                banTriangle[bc->triangle(k)->markedIndex()] = true;
            for (size_t k = 0; k < bc->countVertices(); ++k)
                markVtx[bc->vertex(k)->markedIndex()] = true;
        }
    }

    // The implementation here is a little inefficient (we repeat tests
    // three or four times over), but this routine is only called at
    // the beginning of the enumeration process so no need to worry.

    const size_t* columnPerm = init.columnPerm();

    // Ban quadrilaterals that touch torus boundaries.
    for (size_t i = 0; i < 3 * n; ++i) {
        if (system_.quad())
            tet = columnPerm[i] / 3;
        else
            tet = columnPerm[i] / 7;

        for (int k = 0; k < 4; ++k)
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
        for (size_t i = 3 * n; i < 7 * n; ++i) {
            tet = columnPerm[i] / 7;
            int type = columnPerm[i] % 7;

            if (markVtx[tri_.tetrahedron(tet)->vertex(type)->
                    markedIndex()])
                marked_[i] = true;

            for (int k = 0; k < 4; ++k)
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

