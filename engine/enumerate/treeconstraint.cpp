
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2025, Ben Burton                                   *
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

template BanEdge::BanEdge(const LPInitialTableaux<LPConstraintNone>&, Edge<3>*);
template BanEdge::BanEdge(const LPInitialTableaux<LPConstraintEulerPositive>&,
    Edge<3>*);
template BanEdge::BanEdge(const LPInitialTableaux<LPConstraintEulerZero>&,
    Edge<3>*);
template BanEdge::BanEdge(const LPInitialTableaux<LPConstraintNonSpun>&,
    Edge<3>*);

template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintNone>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintEulerPositive>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintEulerZero>&);
template BanTorusBoundary::BanTorusBoundary(
    const LPInitialTableaux<LPConstraintNonSpun>&);

void LPConstraintEulerPositive::addRows(
        detail::LPCol<nConstraints, Coefficient>* col,
        const Triangulation<3>& tri,
        const size_t* columnPerm) {
    int* obj = new int[7 * tri.size()];
    size_t tet, i;
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
        col[i].extra[0] = obj[columnPerm[i]];

    col[7 * tri.size()].extra[0] = -1;

    delete[] obj;
}

void LPConstraintEulerZero::addRows(
        detail::LPCol<nConstraints, Coefficient>* col,
        const Triangulation<3>& tri,
        const size_t* columnPerm) {
    int* obj = new int[7 * tri.size()];
    size_t tet, i;
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
        col[i].extra[0] = obj[columnPerm[i]];

    col[7 * tri.size()].extra[0] = -1;

    delete[] obj;
}

void LPConstraintNonSpun::addRows(
        detail::LPCol<nConstraints, Coefficient>* col,
        const Triangulation<3>& tri,
        const size_t* columnPerm) {
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

    // Use a static_cast to ensure we are using the Triangulation<3>
    // equality test. Otherwise C++20 complains about ambiguity.
    if (static_cast<const Triangulation<3>&>(snapPea) != tri)
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
        for (size_t i = 0; i < 3 * tri.size(); ++i) {
            col[i].extra[0] =
                coeffs.entry(0, columnPerm[i]).safeLongValue();
            col[i].extra[1] =
                coeffs.entry(1, columnPerm[i]).safeLongValue();
        }
    } catch (const NoSolution&) {
        throw UnsolvedCase("The coefficients of the slope equations "
            "do not fit into a native long integer");
    }
}

void BanConstraintBase::writeTextShort(std::ostream& out) const {
    const size_t nCols = system_.coords(tri_.size());
    bool foundBanned = false, foundMarked = false;

    for (size_t i = 0; i < nCols; ++i)
        if (banned_[i]) {
            if (! foundBanned) {
                out << "Banned:";
                foundBanned = true;
            }
            out << ' ' << i;
        }
    if (! foundBanned)
        out << "Nothing banned";

    out << ", ";

    for (size_t i = 0; i < nCols; ++i)
        if (marked_[i]) {
            if (! foundMarked) {
                out << "marked:";
                foundMarked = true;
            }
            out << ' ' << i;
        }
    if (! foundMarked)
        out << "nothing marked";
}

} // namespace regina

