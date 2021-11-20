
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

#include "enumerate/treeconstraint-impl.h"
#include "snappea/snappeatriangulation.h"

namespace regina {

// Instantiate all templates that we might need:
template BanBoundary::BanBoundary(const LPInitialTableaux<LPConstraintNone>&);
template BanBoundary::BanBoundary(
    const LPInitialTableaux<LPConstraintEulerPositive>&);
template BanBoundary::BanBoundary(
    const LPInitialTableaux<LPConstraintEulerZero>&);
template BanBoundary::BanBoundary(
    const LPInitialTableaux<LPConstraintNonSpun>&);

template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintNone>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintEulerPositive>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintEulerZero>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintNonSpun>&);

void LPConstraintEulerPositive::addRows(
        LPCol<regina::LPConstraintEulerPositive>* col,
        const LPInitialTableaux<LPConstraintEulerPositive>& init) {
    const Triangulation<3>& tri = init.tri();

    int* obj = new int[7 * tri.size()];
    unsigned tet, i;
    Perm<4> p;
    for (i = 0; i < 7 * tri.size(); ++i)
        obj[i] = 1;
    for (i = 0; i < tri.countTriangles(); ++i) {
        tet = tri.triangle(i)->front().tetrahedron()->index();
        p = tri.triangle(i)->front().vertices();
        --obj[7 * tet + p[0]];
        --obj[7 * tet + p[1]];
        --obj[7 * tet + p[2]];
        --obj[7 * tet + 4];
        --obj[7 * tet + 5];
        --obj[7 * tet + 6];
    }
    for (i = 0; i < tri.countEdges(); ++i) {
        tet = tri.edge(i)->front().tetrahedron()->index();
        p = tri.edge(i)->front().vertices();
        ++obj[7 * tet + p[0]];
        ++obj[7 * tet + p[1]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][0]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][1]];
    }

    for (i = 0; i < 7 * tri.size(); ++i)
        col[i].extra[0] = obj[init.columnPerm()[i]];

    col[7 * tri.size()].extra[0] = -1;

    delete[] obj;
}

void LPConstraintEulerZero::addRows(
        LPCol<regina::LPConstraintEulerZero>* col,
        const LPInitialTableaux<LPConstraintEulerZero>& init) {
    const Triangulation<3>& tri = init.tri();

    int* obj = new int[7 * tri.size()];
    unsigned tet, i;
    Perm<4> p;
    for (i = 0; i < 7 * tri.size(); ++i)
        obj[i] = 1;
    for (i = 0; i < tri.countTriangles(); ++i) {
        tet = tri.triangle(i)->front().tetrahedron()->index();
        p = tri.triangle(i)->front().vertices();
        --obj[7 * tet + p[0]];
        --obj[7 * tet + p[1]];
        --obj[7 * tet + p[2]];
        --obj[7 * tet + 4];
        --obj[7 * tet + 5];
        --obj[7 * tet + 6];
    }
    for (i = 0; i < tri.countEdges(); ++i) {
        tet = tri.edge(i)->front().tetrahedron()->index();
        p = tri.edge(i)->front().vertices();
        ++obj[7 * tet + p[0]];
        ++obj[7 * tet + p[1]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][0]];
        ++obj[7 * tet + 4 + quadMeeting[p[0]][p[1]][1]];
    }

    for (i = 0; i < 7 * tri.size(); ++i)
        col[i].extra[0] = obj[init.columnPerm()[i]];

    col[7 * tri.size()].extra[0] = -1;

    delete[] obj;
}

void LPConstraintNonSpun::addRows(
        LPCol<regina::LPConstraintNonSpun>* col,
        const LPInitialTableaux<LPConstraintNonSpun>& init) {
    const Triangulation<3>& tri = init.tri();

    // Add the coefficients for the two new variables now.
    col[3 * tri.size()].extra[0] = -1;
    col[3 * tri.size() + 1].extra[1] = -1;

    // For the time being we insist on one vertex, which must be
    // ideal with torus link.
    if (tri.countVertices() != 1 ||
            (! tri.vertex(0)->isIdeal()) ||
            (! tri.vertex(0)->isLinkOrientable()) ||
            tri.vertex(0)->linkEulerChar() != 0)
        throw InvalidArgument(
            "LPConstraintNonSpun requires an oriented ideal triangulation "
            "with precisely one torus cusp and no other vertices");

    // Compute the two slope equations for the torus cusp, if we can.
    SnapPeaTriangulation snapPea(tri, false);
    if (snapPea.isNull())
        throw UnsolvedCase("SnapPea produced a null triangulation "
            "when attempting to use LPConstraintNonSpun");

    MatrixInt coeffs = snapPea.slopeEquations();

    if (! snapPea.isIdenticalTo(tri))
        throw UnsolvedCase("SnapPea retriangulated "
            "when attempting to use LPConstraintNonSpun");

    // All good!  Add the two slope equations as extra rows to
    // our constraint matrix.
    //
    // The coefficients here are differences of terms from
    // SnapPy's get_cusp_equation(), which works in native
    // integers; therefore we *should* be able to happily convert them
    // back to native integers now.  However, just in case:
    try {
        for (int i = 0; i < 3 * tri.size(); ++i) {
            col[i].extra[0] =
                coeffs.entry(0, init.columnPerm()[i]).safeLongValue();
            col[i].extra[1] =
                coeffs.entry(1, init.columnPerm()[i]).safeLongValue();
        }
    } catch (const NoSolution&) {
        throw UnsolvedCase("The coefficients of the slope equations "
            "do not fit into a native long integer");
    }
}

} // namespace regina

