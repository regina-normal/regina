
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "angle/anglestructures.h"
#include "enumerate/treeconstraint.h"
#include "enumerate/treelp.h"
#include "triangulation/dim3.h"
#include "snappea/snappeatriangulation.h"

namespace regina {

bool Triangulation<3>::knowsStrictAngleStructure(bool cachedOnly) const {
    if (std::holds_alternative<AngleStructure>(strictAngleStructure_))
        return true; // already known: a solution exists
    if (std::get<bool>(strictAngleStructure_))
        return true; // already known: no solution exists
    if (cachedOnly)
        return false;

    // There are some simple cases for which we can deduce the answer
    // automatically.
    if (simplices_.empty()) {
        strictAngleStructure_ = AngleStructure(*this, { 1 });
        return true;
    }

    if (! hasBoundaryTriangles()) {
        // It is easy to prove that, if an angle structure exists,
        // then we must have #edges = #tetrahedra.
        if (countEdges() != simplices_.size()) {
            strictAngleStructure_ = true; // confirmed: no solution
            return true;
        }
    }

    // Don't know.  This requres a real computation.
    return false;
}

bool Triangulation<3>::hasStrictAngleStructure() const {
    // The following test also catches any easy cases.
    if (knowsStrictAngleStructure())
        return std::holds_alternative<AngleStructure>(strictAngleStructure_);

    // Run the full computation and cache the resulting structure, if any.
    LPInitialTableaux<LPConstraintNone> eqns(*this, NormalCoords::Angle, false);

    LPData<LPConstraintNone, Integer> lp;
    lp.reserve(eqns);

    // Find an initial basis.
    lp.initStart();

    // Set all angles to be strictly positive.
    unsigned i;
    for (i = 0; i < eqns.columns(); ++i) {
        // std::cerr << "Constraining +ve: "
        //     << i << " / " << eqns.columns() << std::endl;
        lp.constrainPositive(i);
    }

    // Test for a solution!
    if (! lp.isFeasible()) {
        strictAngleStructure_ = true; // confirmed: no solution
        return false;
    }

    // We have a strict angle structure: reconstruct it.
    strictAngleStructure_ = AngleStructure(*this,
        lp.extractSolution<VectorInt>(nullptr /* type vector */));
    return true;
}

template <bool bdryNull>
bool Triangulation<3>::hasGeneralAngleStructureInternal() const {
    if constexpr (bdryNull) {
        if (std::holds_alternative<AngleStructure>(bdryNullAngleStructure_)) {
            return true; // known to have a solution
        } else if (std::get<bool>(bdryNullAngleStructure_)) {
            // At present, the preconditions are strong enough to ensure that
            // a boundary-null angle structure always exists.
            //TODO  Should probably use an exception or an assert.
            std::cerr << "ERROR: Failed to find a "
                "boundary-null angle structure." << std::endl;

            // At any rate, it must be the case that we successfully ran the
            // computation and failed to find a solution.
            return false;
        }
    } else {
        if (std::holds_alternative<AngleStructure>(generalAngleStructure_)) {
            return true; // known to have a solution
        } else if (std::get<bool>(generalAngleStructure_)) {
            return false; // known to have no solution.
        }
    }

    // As promised in the documentation, the empty triangulation gets the
    // empty angle structure.
    if (simplices_.empty()) {
        if constexpr (bdryNull) {
            bdryNullAngleStructure_ = AngleStructure(*this, { 1 });
        }
        generalAngleStructure_ = AngleStructure(*this, { 1 });
        return true;
    }

    // Handle the preconditions and/or other easy cases.
    if constexpr (bdryNull) {
        if (! isValid()) {
            throw regina::FailedPrecondition(
                    "boundaryNullAngleStructure() "
                    "requires a valid triangulation" );
        } else if (! isOriented()) {
            throw regina::FailedPrecondition(
                    "boundaryNullAngleStructure() "
                    "requires an oriented triangulation");
        }
        for ( Vertex<3>* v : vertices() ) {
            if ( (! v->isIdeal()) || (! v->isLinkOrientable()) ||
                    (v->linkEulerChar() != 0) ) {
                throw regina::FailedPrecondition(
                        "boundaryNullAngleStructure() "
                        "requires every vertex link to be a torus");
            }
        }
    } else if (! hasBoundaryTriangles()) {
        // It is easy to prove that, if an angle structure exists,
        // then we must have #edges = #tetrahedra.
        if (countEdges() != simplices_.size()) {
            generalAngleStructure_ = true; // confirmed: no solution
            return false;
        }

        // If the triangulation is valid, we also need every vertex link
        // to be a torus or Klein bottle.  The only way this can *not*
        // happen at this point in the code, given that we know that
        // #edges = #tetrahedra, is to have some combination of internal
        // vertices and higher-genus vertex links.  This seems sufficiently
        // exotic that we won't waste time testing it here; instead we
        // just run the full linear algebra code (which still does the right
        // thing if there is no solution).
    }

    // Run the full computation and cache the resulting structure, if any.
    //
    // We want a solution to the homogeneous angle structure equations where
    // the final coordinate (representing the scaling factor) is non-zero. In
    // the case where bdryNull is true, we also want the solution to have
    // vanishing peripheral rotational holonomy.
    //
    // The MatrixInt::rowEchelonForm() routine is enough for this: if there is
    // any solution where the final coordinate is non-zero, then the final
    // column will not appear as a leading coefficient in row echelon form.
    MatrixInt eqns;
    if constexpr (bdryNull) {
        SnapPeaTriangulation snapPea(*this);
        if (snapPea.isNull()) {
            throw regina::UnsolvedCase( "SnapPea produced a null "
                    "triangulation when attempting to make the "
                    "boundary-null angle structure equations" );
        }

        // Use a static_cast to ensure we are using the Triangulation<3>
        // equality test.
        if (static_cast<const Triangulation<3>&>(snapPea) != *this) {
            throw regina::UnsolvedCase( "SnapPea retriangulated "
                    "when attempting to make the boundary-null angle "
                    "structure equations" );
        }
        eqns = regina::makeBoundaryNullAngleEquations(snapPea);
    } else {
        eqns = regina::makeAngleEquations(*this);
    }
    size_t rank = eqns.rowEchelonForm();

    // Note: the rank is always positive, since the triangulation is
    // non-empty and so we always have tetrahedron equations present.

    // Go down through the matrix from top-left to bottom-right and work
    // out where the leading coefficients of each row appear.
    auto* leading = new size_t[rank];
    size_t row = 0;
    size_t col = 0;

    while (row < rank) {
        if (eqns.entry(row, col) != 0) {
            leading[row] = col;
            ++row;
        }
        ++col;
    }

    //TODO  When we compute an answer for bdryNullAngleStructure_, the same
    //      answer should also be suitable for generalAngleStructure_.
    //      But at least for now, we will keep the two computations completely
    //      independent (which keeps the behaviour of generalAngleStructure()
    //      consistent with Regina ≤ 7.x).

    if (leading[rank - 1] + 1 == eqns.columns()) {
        // The final column appears as a leading coefficient.
        delete[] leading;
        if constexpr (bdryNull) {
            bdryNullAngleStructure_ = true; // confirmed: no solution
        } else {
            generalAngleStructure_ = true; // confirmed: no solution
        }
        return false;
    }

    // Build up the final vector from back to front.
    VectorInt v(eqns.columns());
    v[eqns.columns() - 1] = 1;

    // We currently have row == rank.
    while (row > 0) {
        // INV:
        // - We have enforced equations row,...,(rank-1);
        // - The current solution has gcd=1.

        --row;
        // Enforce equation #row.

        col = leading[row];
        Integer den = eqns.entry(row, col);

        Integer num; // set to 0
        for (++col; col < v.size(); ++col)
            if (eqns.entry(row, col) != 0)
                num += (eqns.entry(row, col) * v[col]);

        // Our row echelon form guarantees that den > 0.
        // We need to set v[leading[row]] = -num/den.
        if (den == 1) {
            v[leading[row]] = -num;
        } else {
            Integer gcd = den.gcd(num); // guaranteed >= 0.
            if (gcd > 1) {
                den.divByExact(gcd);
                num.divByExact(gcd);
            }

            // Still we have den > 0.
            if (den > 1)
                v *= den;
            // Now the current solution has gcd == den.

            v[leading[row]] = -num;
            // Since gcd(num, den) == 1, there is no need to scale down again.
        }
    }

    delete[] leading;
    if constexpr (bdryNull) {
        bdryNullAngleStructure_ = AngleStructure(*this, std::move(v));
    } else {
        generalAngleStructure_ = AngleStructure(*this, std::move(v));
    }
    return true;
}

// Instantiate both variants of hasGeneralAngleStructureInternal().
template bool Triangulation<3>::hasGeneralAngleStructureInternal<true>() const;
template bool Triangulation<3>::hasGeneralAngleStructureInternal<false>() const;

} // namespace regina

