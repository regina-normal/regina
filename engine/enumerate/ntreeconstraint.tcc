
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2013, Ben Burton                                   *
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

/* end stub */

/* Definitions of template member function from ntreeconstraint.h.
   If you only use common template arguments, such as NInteger or
   NNativeLong, then you will not need to include this file (since all
   templates will already be instantiated for you).
   You will, however, need to include this file if you provide your own
   custom integer classes as template arguments. */

#ifndef __NTREECONSTRAINT_TCC
#ifndef __DOXYGEN
#define __NTREECONSTRAINT_TCC
#endif

#include "enumerate/ntreeconstraint.h"
#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"

namespace regina {

template <typename Integer>
bool LPConstraintEuler<Integer>::addRows(typename
        LPInitialTableaux<regina::LPConstraintEuler, Integer>::Col* col,
        const int* columnPerm, NTriangulation* tri) {
    int* obj = new int[7 * tri->getNumberOfTetrahedra()];
    unsigned tet, i;
    NPerm4 p;
    for (i = 0; i < 7 * tri->getNumberOfTetrahedra(); ++i)
        obj[i] = 1;
    for (i = 0; i < tri->getNumberOfFaces(); ++i) {
        tet = tri->tetrahedronIndex(
            tri->getFace(i)->getEmbedding(0).getTetrahedron());
        p = tri->getFace(i)->getEmbedding(0).getVertices();
        --obj[7 * tet + p[0]];
        --obj[7 * tet + p[1]];
        --obj[7 * tet + p[2]];
        --obj[7 * tet + 4];
        --obj[7 * tet + 5];
        --obj[7 * tet + 6];
    }
    for (i = 0; i < tri->getNumberOfEdges(); ++i) {
        tet = tri->tetrahedronIndex(
            tri->getEdge(i)->getEmbedding(0).getTetrahedron());
        p = tri->getEdge(i)->getEmbedding(0).getVertices();
        ++obj[7 * tet + p[0]];
        ++obj[7 * tet + p[1]];
        ++obj[7 * tet + 4 + vertexSplitMeeting[p[0]][p[1]][0]];
        ++obj[7 * tet + 4 + vertexSplitMeeting[p[0]][p[1]][1]];
    }

    for (i = 0; i < 7 * tri->getNumberOfTetrahedra(); ++i)
        col[i].euler = obj[columnPerm[i]];

    col[7 * tri->getNumberOfTetrahedra()].euler = -1;

    delete[] obj;
    return true;
}

#ifndef EXCLUDE_SNAPPEA
template <typename Integer>
bool LPConstraintNonSpun<Integer>::addRows(typename
        LPInitialTableaux<regina::LPConstraintNonSpun, Integer>::Col* col,
        const int* columnPerm, NTriangulation* tri) {
    // Regardless of whether the constraints are broken,
    // we need to ensure that the matrix has full rank.
    // Therefore add the coefficients for the two new variables now.
    col[3 * tri->getNumberOfTetrahedra()].meridian = -1;
    col[3 * tri->getNumberOfTetrahedra() + 1].longitude = -1;

    // For the time being we insist on one vertex, which must be
    // ideal with torus link.
    if (tri->getNumberOfVertices() != 1 ||
            (! tri->getVertex(0)->isIdeal()) ||
            (! tri->getVertex(0)->isLinkOrientable()) ||
            tri->getVertex(0)->getLinkEulerCharacteristic() != 0)
        return false;

    // Compute the two slope equations for the torus cusp, if we can.
    NSnapPeaTriangulation snapPea(*tri, false);
    NMatrixInt* coeffs = snapPea.slopeEquations();
    if (! coeffs)
        return false;

    // Check that SnapPea hasn't changed the triangulation on us.
    if (! snapPea.verifyTriangulation(*tri)) {
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
    for (int i = 0; i < 3 * tri->getNumberOfTetrahedra(); ++i) {
        col[i].meridian = coeffs->entry(0, columnPerm[i]).longValue();
        col[i].longitude = coeffs->entry(1, columnPerm[i]).longValue();
    }

    delete coeffs;
    return true;
}
#endif // EXCLUDE_SNAPPEA

} // namespace regina

#endif

