
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

/*! \file enumerate/treelp-impl.h
 *  \brief Contains implementation details for the various linear programming
 *  class templates.
 *
 *  This file is \e not included automatically by treelp.h.
 *  However, most end users should not need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of these classes for common combinations of template arguments.
 */

#ifndef __REGINA_TREELP_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_TREELP_IMPL_H
#endif

/**
 * Optimisation flags:
 * Define any combination of the following flags to switch \e off
 * various optimisations.
 * This is for diagnostic purposes only.
 */
// #define REGINA_NOOPT_REORDER_COLUMNS

/**
 * Diagnostic output flags:
 */
// #define REGINA_COUNT_PIVOTS

#include "angle/anglestructures.h"
#include "enumerate/treelp.h"
#include "maths/matrixops.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include <cstring>

namespace regina {

template <typename IntType>
void LPMatrix<IntType>::combRow(const IntType& destCoeff, size_t dest,
        const IntType& srcCoeff, size_t src, const IntType& div) {
    IntType* ps = dat_ + src * cols_;
    IntType* pd = dat_ + dest * cols_;
    IntType tmp; // Use this to avoid spurious temporary IntTypes.
    for (size_t i = 0; i < cols_; ++i) {
        *pd *= destCoeff;
        tmp = srcCoeff;
        tmp *= *ps++;
        *pd -= tmp;
        (*pd++).divByExact(div);
    }
}

template <typename IntType>
IntType LPMatrix<IntType>::combRowAndNorm(const IntType& destCoeff,
        size_t dest, const IntType& srcCoeff, size_t src) {
    IntType gcdRow; // Initialised to zero.
    IntType* ps = dat_ + src * cols_;
    IntType* pd = dat_ + dest * cols_;
    IntType tmp; // Use this to avoid spurious temporary IntTypes.
    size_t i;
    for (i = 0; i < cols_; ++i, ++pd, ++ps) {
        *pd *= destCoeff;
        tmp = srcCoeff;
        tmp *= *ps;
        *pd -= tmp;
        if (gcdRow != 1)
            gcdRow.gcdWith(*pd); // gcd() guarantees to be >= 0.
    }
    if (gcdRow > 1) {
        pd = dat_ + dest * cols_;
        for (i = 0; i < cols_; ++i)
            (*pd++).divByExact(gcdRow);
    }
    return gcdRow;
}

template <typename IntType>
void LPMatrix<IntType>::writeTextShort(std::ostream& out) const {
    size_t r, c;
    const IntType* entry = dat_;
    out << '[';
    for (r = 0; r < rows_; ++r) {
        if (r > 0)
            out << ' ';
        out << '[';
        for (c = 0; c < cols_; ++c)
            out << ' ' << *entry++;
        out << " ]";
    }
    out << ']';
}

template <typename IntType>
void LPMatrix<IntType>::writeTextLong(std::ostream& out) const {
    size_t r, c;
    const IntType* entry = dat_;
    for (r = 0; r < rows_; ++r) {
        for (c = 0; c < cols_; ++c) {
            if (c > 0) out << ' ';
            out << *entry++;
        }
        out << '\n';
    }
}

template <class LPConstraint>
LPInitialTableaux<LPConstraint>::LPInitialTableaux(
        const Triangulation<3>& tri, NormalEncoding enc, bool enumeration) :
        tri_(&tri), system_(enc) {
    size_t r, c;

    // Fetch the original (unadjusted) matrix of matching equations.
    if (system_.normal()) {
        // In both cases below we know that makeMatchingEquations() will
        // always succeed.
        if (system_.standard())
            eqns_ = regina::makeMatchingEquations(tri, NS_STANDARD);
        else
            eqns_ = regina::makeMatchingEquations(tri, NS_QUAD);
        scaling_ = 0;
    } else {
        eqns_ = regina::makeAngleEquations(tri);

        // Scale each row so that the rightmost entry (used for
        // projectivising the angle structure polytope) is always -2.
        // This is possible since the angle equation matrix
        // will have final entries of -1 and -2 only.
        scaling_ = -2;
        long rightmost;
        for (r = 0; r < eqns_.rows(); ++r) {
            rightmost = eqns_.entry(r, eqns_.columns() - 1).longValue();
            if (rightmost != scaling_)
                for (c = 0; c < eqns_.columns(); ++c)
                    eqns_.entry(r, c) *= (scaling_ / rightmost);
        }
    }

    // Compute the rank of the matrix, and reorder its rows so
    // the first \a rank_ rows are full rank.
    rank_ = regina::rowBasis(eqns_);

    // Reorder the columns using a good heuristic.
    cols_ = eqns_.columns() + LPConstraint::nConstraints;
    columnPerm_ = new size_t[cols_];
    reorder(enumeration);

    // Create and fill the sparse columns.
    col_ = new LPCol<LPConstraint>[cols_];
    for (c = 0; c < eqns_.columns() - (scaling_ ? 1 : 0); ++c)
        for (r = 0; r < rank_; ++r)
            if (eqns_.entry(r, c) != 0) {
                // Each entry should have absolute value. <= 4
                col_[c].push(r,
                    static_cast<int>(eqns_.entry(r, c).longValue()));
            }

    // Add in the final row(s) for any additional constraints.
    LPConstraint::addRows(col_, *this);
    rank_ += LPConstraint::nConstraints;
}

#ifdef REGINA_NOOPT_REORDER_COLUMNS
template <class LPConstraint>
void LPInitialTableaux<LPConstraint>::reorder(bool) {
    // This is a "do-nothing" version of reorder().
    if (! system_.standard()) {
        // Leave the columns exactly as they were.
        for (size_t i = 0; i < cols_; ++i)
            columnPerm_[i] = i;
        return;
    } else {
        // Keep the tetrahedra in the same order, but move
        // quadrilaterals to the front and triangles to the back
        // as required by columnPerm().
        size_t n = tri_->size();
        for (size_t i = 0; i < n; ++i) {
            columnPerm_[3 * i] = 7 * i + 4;
            columnPerm_[3 * i + 1] = 7 * i + 5;
            columnPerm_[3 * i + 2] = 7 * i + 6;
            columnPerm_[3 * n + 4 * i] = 7 * i;
            columnPerm_[3 * n + 4 * i + 1] = 7 * i + 1;
            columnPerm_[3 * n + 4 * i + 2] = 7 * i + 2;
            columnPerm_[3 * n + 4 * i + 3] = 7 * i + 3;
        }
    }

    // This fills the columnPerm_ array; now we need to move the
    // columns of eqns_ around accordingly, and then finish off
    // columnPerm_ with the columns for additional constraints
    // from LPConstraint (if we have any).
    //
    // From here on we copy code directly from the "real" reorder()
    // below.
    size_t* tmp = new size_t[eqns_.columns()];
    std::copy(columnPerm_, columnPerm_ + eqns_.columns(), tmp);
    for (size_t i = 0; i < eqns_.columns(); ++i) {
        // Column tmp[i] of the matrix should be moved to
        // column i.
        if (tmp[i] == i)
            continue;

        eqns_.swapCols(i, tmp[i]);

        // Adjust links to the old column i, which is now column tmp[i].
        size_t j;
        for (j = i + 1; j < eqns_.columns(); ++j)
            if (tmp[j] == i)
                break; // This is the link we need to change.
#ifdef REGINA_VERIFY_LPDATA
        if (j == eqns_.columns()) {
            std::cerr << "ERROR: Sorting error." << std::endl;
            ::exit(1);
        }
#endif
        tmp[j] = tmp[i];
        tmp[i] = i;
    }
    delete[] tmp;

    // If we have extra variables for additional constraints or
    // objectives, append the corresponding entries to the end of
    // the permutation for completeness.
    for (int i = 0; i < LPConstraint::nConstraints; ++i)
        columnPerm_[cols_ - i - 1] = cols_ - i - 1;
}
#else
template <class LPConstraint>
void LPInitialTableaux<LPConstraint>::reorder(bool enumeration) {
    size_t n = tri_->size();

    // Fill the columnPerm_ array according to what kind of
    // problem we're trying to solve.
    if (system_.standard() && enumeration) {
        // We're doing vertex enumeration in standard coordinates.
        //
        // Use exactly the same ordering of quadrilaterals that we
        // use in quadrilateral coordinates, and then just fill
        // in the triangles at the end.
        //
        // We remove our extra constraints here, since some constraints might
        // not be offered in quad coordinates.
        // Note: NS_QUAD is always safe; the constructor call below will
        // never throw.
        LPInitialTableaux<LPConstraintNone> quad(*tri_, NS_QUAD,
            true /* enumeration */);
        for (size_t i = 0; i < n; ++i) {
            size_t k = quad.columnPerm()[3 * i] / 3;
            columnPerm_[3 * i] = 7 * k + 4;
            columnPerm_[3 * i + 1] = 7 * k + 5;
            columnPerm_[3 * i + 2] = 7 * k + 6;
            columnPerm_[3 * n + 4 * i] = 7 * k;
            columnPerm_[3 * n + 4 * i + 1] = 7 * k + 1;
            columnPerm_[3 * n + 4 * i + 2] = 7 * k + 2;
            columnPerm_[3 * n + 4 * i + 3] = 7 * k + 3;
        }
    } else if (system_.angle()) {
        // TODO: Find a good heuristic to use for angle structure coordinates.
        // For now, we'll leave the columns exactly as they were.
        for (size_t i = 0; i < cols_; ++i)
            columnPerm_[i] = i;
        return;
    } else {
        // Either we're doing vertex enumeration in quad coordinates,
        // or we're in standard coordinates but just searching for a
        // single solution under some constraints.
        //
        // Process the rows in increasing order by number of tetrahedra
        // touched, and place the columns for each tetrahedron in the
        // order that we see them.  We place columns at the "back" of
        // the matrix, so that we fill the matrix in "reverse" order
        // from the last column to the first.

        // Track which rows have been processed so far.
        bool* used = new bool[rank_];
        std::fill(used, used + rank_, false);

        // Also track which tetrahedra have been used so far.
        bool* touched = new bool[n];
        std::fill(touched, touched + n, false);
        size_t nTouched = 0;

        // Off we go, one row at a time.
        for (size_t i = 0; i < rank_; ++i) {
            // Seek out the ith row to process.
            // Because the first rank_ rows of the matrix are full rank,
            // we are guaranteed that this row will be non-zero.

            size_t best = n + 1; // No row touches more than n tetrahedra.
            size_t bestRow;

            for (size_t j = 0; j < rank_; ++j) {
                if (used[j])
                    continue;
                size_t curr = 0;
                for (size_t k = 0; k < n; ++k) {
                    if (touched[k])
                        continue;
                    if (system_.quad()) {
                        // We're in quadrilateral coordinates.
                        if (eqns_.entry(j, 3 * k) != 0 ||
                                eqns_.entry(j, 3 * k + 1) != 0 ||
                                eqns_.entry(j, 3 * k + 2) != 0)
                            ++curr;
                    } else {
                        // We're in standard coordinates.
                        if (eqns_.entry(j, 7 * k + 4) != 0 ||
                                eqns_.entry(j, 7 * k + 5) != 0 ||
                                eqns_.entry(j, 7 * k + 6) != 0)
                            ++curr;
                    }
                    if (curr >= best)
                        break; // We cannot beat best from here.
                }
                if (curr < best) {
                    bestRow = j;
                    best = curr;
                }
            }

            // The next row to process is bestRow.
            // Find all the tetrahedra that it touches that we
            // haven't already used yet, and place the corresponding
            // columns at the end of the matrix.
            used[bestRow] = true;
            for (size_t k = 0; k < n; ++k) {
                if (touched[k])
                    continue;
                if (system_.quad()) {
                    // We're in quadrilateral coordinates.
                    if ((eqns_.entry(bestRow, 3 * k) != 0 ||
                            eqns_.entry(bestRow, 3 * k + 1) != 0 ||
                            eqns_.entry(bestRow, 3 * k + 2) != 0)) {
                        touched[k] = true;

                        columnPerm_[3 * (n - nTouched) - 3] = 3 * k;
                        columnPerm_[3 * (n - nTouched) - 2] = 3 * k + 1;
                        columnPerm_[3 * (n - nTouched) - 1] = 3 * k + 2;

                        ++nTouched;
                    }
                } else {
                    // We're in standard coordinates.
                    if ((eqns_.entry(bestRow, 7 * k + 4) != 0 ||
                            eqns_.entry(bestRow, 7 * k + 5) != 0 ||
                            eqns_.entry(bestRow, 7 * k + 6) != 0)) {
                        touched[k] = true;

                        // The quadrilateral columns...
                        columnPerm_[3 * (n - nTouched) - 3] = 7 * k + 4;
                        columnPerm_[3 * (n - nTouched) - 2] = 7 * k + 5;
                        columnPerm_[3 * (n - nTouched) - 1] = 7 * k + 6;
                        // ... and the triangle columns.
                        columnPerm_[3 * n + 4 * (n - nTouched) - 4] =
                            7 * k;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 3] =
                            7 * k + 1;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 2] =
                            7 * k + 2;
                        columnPerm_[3 * n + 4 * (n - nTouched) - 1] =
                            7 * k + 3;

                        ++nTouched;
                    }
                }
            }
        }

        // We have now processed all rows.  However, there may be some
        // tetrahedra that appear in no rows at all.
        // Make sure we catch these tetrahedra as well.
        for (size_t k = 0; k < n; ++k) {
            if (touched[k])
                continue;
            touched[k] = true;
            if (system_.quad()) {
                // We're in quadrilateral coordinates.
                columnPerm_[3 * (n - nTouched) - 3] = 3 * k;
                columnPerm_[3 * (n - nTouched) - 2] = 3 * k + 1;
                columnPerm_[3 * (n - nTouched) - 1] = 3 * k + 2;
            } else {
                // We're in standard coordinates.
                columnPerm_[3 * (n - nTouched) - 3] = 7 * k + 4;
                columnPerm_[3 * (n - nTouched) - 2] = 7 * k + 5;
                columnPerm_[3 * (n - nTouched) - 1] = 7 * k + 6;
                columnPerm_[3 * n + 4 * (n - nTouched) - 4] = 7 * k;
                columnPerm_[3 * n + 4 * (n - nTouched) - 3] = 7 * k + 1;
                columnPerm_[3 * n + 4 * (n - nTouched) - 2] = 7 * k + 2;
                columnPerm_[3 * n + 4 * (n - nTouched) - 1] = 7 * k + 3;
            }
            ++nTouched;
        }

        delete[] touched;
        delete[] used;
    }

    // At this point we have filled the columnPerm_ array
    // (except for the final columns for additional constraints
    // from LPConstraint, which we will deal with later).
    //
    // Now go ahead and actually move the columns around accordingly.
    auto* tmp = new size_t[eqns_.columns()];
    std::copy(columnPerm_, columnPerm_ + eqns_.columns(), tmp);
    for (size_t i = 0; i < eqns_.columns(); ++i) {
        // Column tmp[i] of the matrix should be moved to
        // column i.
        if (tmp[i] == i)
            continue;

        eqns_.swapCols(i, tmp[i]);

        // Adjust links to the old column i, which is now column tmp[i].
        size_t j;
        for (j = i + 1; j < eqns_.columns(); ++j)
            if (tmp[j] == i)
                break; // This is the link we need to change.
#ifdef REGINA_VERIFY_LPDATA
        if (j == eqns_.columns()) {
            std::cerr << "ERROR: Sorting error." << std::endl;
            ::exit(1);
        }
#endif
        tmp[j] = tmp[i];
        tmp[i] = i;
    }
    delete[] tmp;

    // If we have extra variables for additional constraints or
    // objectives, append the corresponding entries to the end of
    // the permutation for completeness.
    for (int i = 0; i < LPConstraint::nConstraints; ++i)
        columnPerm_[cols_ - i - 1] = cols_ - i - 1;
}
#endif

template <class LPConstraint>
void LPInitialTableaux<LPConstraint>::writeTextShort(std::ostream& out) const {
    out << "Columns:";
    for (size_t c = 0; c < eqns_.columns(); ++c) {
        if (system_.angle() && c + 1 == eqns_.columns())
            out << " {scaling " << scaling_ << '}';
        else {
            out << " {";
            if (col_[c].nPlus) {
                out << '+';
                for (int i = 0; i < col_[c].nPlus; ++i) {
                    if (i > 0)
                        out << ',';
                    out << col_[c].plus[i];
                }
            }
            if (col_[c].nMinus) {
                if (col_[c].nPlus)
                    out << ' ';
                out << '-';
                for (int i = 0; i < col_[c].nMinus; ++i) {
                    if (i > 0)
                        out << ',';
                    out << col_[c].minus[i];
                }
            }
            out << '}';
        }
    }
    if constexpr (LPConstraint::nConstraints > 0) {
        out << ", constraints:";
        for (int i = 0; i < LPConstraint::nConstraints; ++i) {
            out << " [";
            for (size_t c = 0; c < cols_; ++c)
                out << ' ' << col_[c].extra[i];
            out << " ]";
        }
    }
}

template <class LPConstraint>
void LPInitialTableaux<LPConstraint>::writeTextLong(std::ostream& out) const {
    out << "System: ";
    system_.writeTextShort(out);
    out << "\nRank: " << rank_
        << "\nColumn permutation:";
    for (size_t c = 0; c < cols_; ++c)
        out << ' ' << columnPerm_[c];
    out << '\n';
    for (size_t c = 0; c < eqns_.columns(); ++c) {
        out << "Column " << c << ':';
        if (system_.angle() && c + 1 == eqns_.columns())
            out << " scaling -> " << scaling_;
        else {
            if (col_[c].nPlus) {
                out << " + {";
                for (int i = 0; i < col_[c].nPlus; ++i) {
                    if (i > 0)
                        out << ',';
                    out << col_[c].plus[i];
                }
                out << '}';
            }
            if (col_[c].nMinus) {
                out << " - {";
                for (int i = 0; i < col_[c].nMinus; ++i) {
                    if (i > 0)
                        out << ',';
                    out << col_[c].minus[i];
                }
                out << '}';
            }
        }
        out << '\n';
    }
    if constexpr (LPConstraint::nConstraints > 0) {
        for (int i = 0; i < LPConstraint::nConstraints; ++i) {
            out << "Constraint " << i << ':';
            for (size_t c = 0; c < cols_; ++c)
                out << ' ' << col_[c].extra[i];
            out << '\n';
        }
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::initStart() {
    // In this routine we rely on the fact that the
    // LPInitialTableaux constructor ensures that the original
    // tableaux has full rank.

    // Begin at the original tableaux, with no row operations performed
    // and with all equations having a right-hand side of zero.
    rowOps_.initIdentity(origTableaux_->rank());
    std::fill(rhs_, rhs_ + origTableaux_->rank(), 0);
    rank_ = origTableaux_->rank();
    octPrimary_ = -1;

    // From here, find any feasible basis.
    findInitialBasis();

    // Since RHS = 0, this basis is already feasible.
    feasible_ = true;

    // Finally, enforce our additional linear constraints.
    // This might break feasibility.
    LPConstraint::constrain(*this, origTableaux_->columns());
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::initClone(const LPData& parent) {
    // If the parent tableaux is infeasible, mark this tableaux as
    // infeasible also and abort.
    feasible_ = parent.feasible_;
    if (! feasible_)
        return;

    // The parent tableaux is feasible: clone all of its data.
    std::copy(parent.rhs_, parent.rhs_ + parent.rank_, rhs_);
    rowOps_.initClone(parent.rowOps_);
    rank_ = parent.rank_;
    memcpy(basis_, parent.basis_, parent.rank_ * sizeof(size_t));
    memcpy(basisRow_, parent.basisRow_,
        origTableaux_->columns() * sizeof(ssize_t));
    octPrimary_ = parent.octPrimary_;
    octSecondary_ = parent.octSecondary_;
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::constrainZero(size_t pos) {
    // If the variable has already been deactivated, there is
    // nothing to do.
    if (! isActive(pos))
        return;

    // If the system is infeasible beforehand, it will certainly
    // be infeasible afterwards.  In this case, abort.
    if (! feasible_)
        return;

    // If we ever do something that *might* make the basis
    // infeasible, we will set perhapsInfeasible to true as a
    // reminder to fix things later.
    bool perhapsInfeasible = false;

    // Is the variable currently in the basis?  If so, get it out.
    if (basisRow_[pos] >= 0) {
        auto row = static_cast<size_t>(basisRow_[pos]);
        ssize_t c;
        if (rhs_[row].isZero()) {
            // We can pivot in any other variable that appears in
            // this basis row.  Choose the one with largest index.
            for (c = origTableaux_->columns() - 1; c >= 0; --c)
                if (basisRow_[c] < 0 /* c is active and non-basic */ &&
                        entrySign(row, c))
                    break;
            if (c >= 0) {
                pivot(pos, c);
                // Because rhs_[r] == 0, this pivot can never create
                // infeasibility.
            } else {
                // There are no other variables in this basis row!
                // Our equation just looks like x_pos = 0.
                //
                // This means that, if we deactivate pos, we lose rank
                // and we need to delete the corresponding row entirely.
                --rank_;

                // "Delete" the row by moving it to index rank_, which
                // is now outside our scope of interest (since we are
                // now only interested in rows 0,...,rank_-1).
                if (row != rank_) {
                    std::swap(rhs_[row], rhs_[rank_]);
                    rowOps_.swapRows(row, rank_);
                    basis_[row] = basis_[rank_];
                    basisRow_[basis_[row]] = row;
                }

                // This column is already filled with zeroes
                // from row 0 to rank_-1, because pos was in the basis.
                // If we're in paranoid mode, check this.
#ifdef REGINA_VERIFY_LPDATA
                for (size_t r = 0; r < rank_; ++r)
                    if (! entry(r, pos).isZero()) {
                        std::cerr << "VERIFY: Drop error." << std::endl;
                        ::exit(1);
                    }
#endif
            }
        } else {
            // Because the system is feasible, we have rhs_[r] > 0.
            // This means we can only pivot in a variable with positive
            // coefficient in this basis row.  If there is one,
            // choose the one with largest index.  If there is
            // no such variable, the entire system becomes infeasible.
            for (c = origTableaux_->columns() - 1; c >= 0; --c)
                if (basisRow_[c] < 0 /* c is active and non-basic */ &&
                        entrySign(row, c) > 0)
                    break;
            if (c < 0) {
                // There is no possible variable to pivot in.
                // The system must be infeasible.
                feasible_ = false;
                return;
            }
            pivot(pos, c);

            // The pivot *might* have made the new basis infeasible.
            // Remember this so we can fix things afterwards.
            perhapsInfeasible = true;
        }
    }

    // The variable is out of the basis.  Deactivate the column
    // (which simply means setting basisRow to some non-negative
    // integer).
    basisRow_[pos] = 0;

#ifdef REGINA_VERIFY_LPDATA
    verify();
#endif

    // The variable is gone, but we might have pivoted to an
    // infeasible basis.  If this is a possibility, then move to a
    // feasible basis if we can.  If we cannot, then makeFeasible()
    // will detect this and set \a feasible_ to \c false.
    if (perhapsInfeasible)
        makeFeasible();

#ifdef REGINA_VERIFY_LPDATA
    verify();
#endif
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::constrainPositive(size_t pos) {
    // If the variable has already been deactivated, it cannot
    // be positive.
    if (! isActive(pos)) {
        feasible_ = false;
        return;
    }

    // If the system is infeasible beforehand, it will certainly
    // be infeasible afterwards.  In this case, abort.
    if (! feasible_)
        return;

    // Just replace x with (1+x'), where now x' must be non-negative.
    // This corresponds to subtracting column pos of this tableaux
    // from the right-hand side.
    // If there is any possibility that some entry on the
    // right-hand side could become negative, we must remember to
    // pivot back to feasibility.
    ssize_t row = basisRow_[pos];
    IntType tmp;
    if (row >= 0) {
        // This variable is in the basis, and so there is only
        // one non-zero entry in column pos.
        // This makes subtracting column pos from rhs_ very easy
        // (just a single operation):
        entry(row, pos, tmp);
        if ((rhs_[row] -= tmp) < 0)
            makeFeasible();
    } else {
        // This variable is not in the basis.
        // We know nothing about the column, so just do a full
        // element-by-element column subtraction.
        for (size_t r = 0; r < rank_; ++r) {
            entry(r, pos, tmp);
            rhs_[r] -= tmp;
        }
        makeFeasible();
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::constrainOct(size_t quad1, size_t quad2) {
    // If either variable has already been deactivated, it cannot
    // be positive.
    if (! (isActive(quad1) && isActive(quad2))) {
        feasible_ = false;
        return;
    }

    // If the system is infeasible beforehand, it will certainly
    // be infeasible afterwards.  In this case, abort.
    if (! feasible_)
        return;

    // Suppose we choose to count octagons using column i, and
    // to zero out and deactivate column j (where i and j are
    // quad1 and quad2 in some order).  Then our tasks are to:
    //
    // (i) Set x_i = x_j, by replacing the variable x_j with
    //     x_j' = x_j - x_i;
    // (ii) If we have any additional linear constraints through
    //      the template parameter LPConstraints, adjust the
    //      coefficients in columns i and/or j if necessary to
    //      reflect the presence of octagons (recalling that the
    //      coefficients for an octagon type need not be the sum
    //      of coefficients for the corresponding two
    //      quadrilateral types);
    // (iii) Set x_i >= 1 and x_j' = 0.
    //
    // We do this as follows:
    //
    // (i) Add column j to column i;
    // (ii) Add or subtract further multiples of the final column(s)
    //      to/from column i to reflect any change in coefficients;
    // (iii) Call constrainZero(j) and constrainPositive(i).
    //
    // We perform steps (i) and (ii) just by setting octPrimary_ and
    // octSecondary_ (this works because the implementation of entry()
    // adjusts its results according to the current values of
    // octPrimary_ and octSecondary_).  However, if we change a
    // column corresponding to a basic variable then we must
    // remember to reorganise the tableaux so that the column once
    // again contains all zeroes except for a single positive entry.

    ssize_t row1 = basisRow_[quad1];
    ssize_t row2 = basisRow_[quad2];

    if (row1 < 0) {
        if (row2 < 0) {
            // Both variables are non-basic.
            // We will use quad1 to count octagons.

            // First adjust the columns in the tableaux...
            octPrimary_ = quad1;
            octSecondary_ = quad2;

            // ... and then constrain variables as required.
            // Since quad2 is non-basic, it is already zero so
            // we can simply deactivate it.
            basisRow_[quad2] = 0;
            constrainPositive(quad1);
        } else {
            // quad1 is non-basic, but quad2 is basic.
            // Once again we will use quad1 to count octagons.

            // First adjust the columns in the tableaux...
            octPrimary_ = quad1;
            octSecondary_ = quad2;

            // ... and then constrain variables as required.
            // This time quad2 might be non-zero, so we need to
            // call the more expensive constrainZero(quad2).
            constrainZero(quad2);
            constrainPositive(quad1);
        }
    } else if (row2 < 0) {
        // quad2 is non-basic, but quad1 is basic.
        // This time we will use quad2 to count octagons.
        // Do what we did in the previous case, but the other way
        // around.

        // First adjust the columns in the tableaux...
        octPrimary_ = quad2;
        octSecondary_ = quad1;

        // ... and then constrain variables as required.
        constrainZero(quad1);
        constrainPositive(quad2);
    } else {
        // Both quad1 and quad2 are basic.
        //
        // Because we might need to adjust columns to reflect changes
        // in our additional linear constraints, whichever column we
        // keep could change in any crazy way.  We will need to adjust
        // things to make sure it looks like a basis column once again.

        // For no particular reason, let's choose to count
        // octagons using quad1, and eventually drop quad2.
        // It's going to be messy whichever we choose.

        // Adjust column quad1 now.
        octPrimary_ = quad1;
        octSecondary_ = quad2;

        // Although quad1 is in the basis, its column could now
        // look like anything.  We need to repair it so it
        // contains all zeroes except for cell (row1, quad1),
        // which must be strictly positive.
        IntType e1;
        entry(row1, quad1, e1);
        if (! e1.isZero()) {
            // The (row1, quad1) entry is non-zero.
            // It's clear what to do from here: make sure
            // this entry is positive, perform row operations to
            // clear out the rest of column quad1, and then restore
            // feasibility.
            if (e1 < 0) {
                e1.negate();
                rhs_[row1].negate();
                rowOps_.negateRow(row1);
            }

            IntType coeff, gcdRow;
            for (size_t r = 0; r < rank_; ++r) {
                if (r == static_cast<size_t>(row1))
                    continue;

                // We will reuse coeff, to avoid too many temporary IntTypes.
                // We first set coeff here, and then we reuse and alter it
                // within the IF block below.
                entry(r, quad1, coeff);
                if (! coeff.isZero()) {
                    gcdRow = rowOps_.combRowAndNorm(e1, r, coeff, row1);

                    // As usual, we already know in advance that
                    // gcdRow must divide into rhs_[r].
                    rhs_[r] *= e1;
                    coeff *= rhs_[row1];
                    rhs_[r] -= coeff;
                    rhs_[r].divByExact(gcdRow);
                }
            }

            makeFeasible();

            // Right: that takes care of the column adjustments.
            // Now constrain the variables as required.
            constrainZero(quad2);
            constrainPositive(quad1);
        } else {
            // The (row1, quad1) entry is now zero.
            // Our solution here is to get quad1 out of the basis.
            // Try to find some other non-zero coefficient in row1;
            // note that the only possible locations for another
            // non-zero coefficient are in non-basic columns.

            // Choose the column with largest index.
            ssize_t c;
            for (c = origTableaux_->columns() - 1; c >= 0; --c)
                if (basisRow_[c] < 0 /* active and non-basic */ &&
                        entrySign(row1, c))
                    break;
            if (c >= 0) {
                // We've found an alternative.
                // Pivot quad1 out of the basis, and put column c
                // in its place.
                pivot(quad1, c);

                // We now have a basis again with a corresponding
                // tableaux, but the pivot may have broken feasibility.
                makeFeasible();

                // This takes care of the column adjustments.
                // Now constrain the variables as required.
                constrainZero(quad2);
                constrainPositive(quad1);
            } else {
                // Every single entry in this row is zero!
                //
                // If rhs_[row1] == 0, this is a tautology.
                // If rhs_[row1] != 0, this is an impossibility.
                if (rhs_[row1] != 0) {
                    feasible_ = false;
                } else {
                    // Just pull quad1 out of the basis.  Since
                    // the rank drops, we don't need another
                    // variable to replace it.
                    basisRow_[quad1] = -1;

                    // Move the empty row out of the active area
                    // of the matrix.
                    --rank_;
                    if (row1 != static_cast<ssize_t>(rank_)) {
                        std::swap(rhs_[row1], rhs_[rank_]);
                        rowOps_.swapRows(row1, rank_);
                        basis_[row1] = basis_[rank_];
                        basisRow_[basis_[row1]] = row1;
                    }

                    // Since the RHS did not change, the system
                    // is still feasible.
                    // Constrain the variables as required.
                    constrainZero(quad2);
                    constrainPositive(quad1);
                }
            }
        }
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::writeTextShort(std::ostream& out) const {
    if (! basis_) {
        out << "Uninitialised";
        return;
    }

    if (feasible_)
        out << "Feasible basis:";
    else
        out << "Infeasible basis:";
    if (rank_ > 0)
        for (size_t r = 0; r < rank_; ++r)
            out << ' ' << basis_[r];
    else
        out << " (empty)";
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::writeTextLong(std::ostream& out) const {
    if (! basis_) {
        out << "Uninitialised";
        return;
    }

    if (feasible_)
        out << "Feasible basis:";
    else
        out << "Infeasible basis:";
    if (rank_ > 0)
        for (size_t r = 0; r < rank_; ++r)
            out << ' ' << basis_[r];
    else
        out << " (empty)";

    out << "\nTableaux:\n";
    for (size_t r = 0; r < rank_; ++r) {
        for (size_t c = 0; c < origTableaux_->columns(); ++c)
            out << entry(r, c) << ' ';
        out << '\n';
    }
}

template <class LPConstraint, typename IntType>
template <class RayClass>
RayClass LPData<LPConstraint, IntType>::extractSolution(const char* type)
        const {
    static_assert(
        FaithfulAssignment<IntType, typename RayClass::value_type>::value,
        "LPData::extractSolution() requires a RayClass template parameter "
        "whose elements can faithfully store integers of the template "
        "parameter IntType.");

    // This next test is to ensure that RayClass zero-initialises its elements.
    static_assert(IsReginaInteger<typename RayClass::value_type>::value,
        "LPData::extractSolution() requires a RayClass template parameter "
        "that stores one of Regina's own integer types.");

    // Fetch details on how to undo the column permutation.
    const size_t* columnPerm = origTableaux_->columnPerm();

    // We will multiply the solution vector by
    // lcm(basis coefficients in the tableaux), which will
    // ensure that the variables will all be integers.
    // This multiple might be too large, but we will shrink the
    // vector down again at the end of this routine.
    //
    // First compute this lcm.
    size_t i;
    typename RayClass::value_type lcm(1);
    for (i = 0; i < rank_; ++i)
        lcm = lcm.lcm(entry(i, basis_[i]));

    RayClass v(origTableaux_->coordinateColumns());

    // Now compute (lcm * the solution vector).  We do not yet
    // take into account the change of variables x_i -> x_i - 1
    // that occurred each time we called constrainPositive(),
    // or the more complex changes of variables that occurred
    // each time we called constrainOct().
    //
    // All non-basic variables will be zero (and so we do nothing,
    // since they will already have been initialised to zero in \a v).
    //
    // For basic variables, compute the values from the tableaux.
    // Because we are multiplying everything by lcm, the
    // divisions in the following code are all perfectly safe
    // (and give precise integer results).
    typename RayClass::value_type coord;
    for (i = 0; i < rank_; ++i) {
        if (basis_[i] >= v.size())
            continue;
        coord = lcm;
        coord *= rhs_[i];
        coord /= entry(i, basis_[i]);
        v[columnPerm[basis_[i]]] = coord;
    }

    // Now we take into account the changes of variable due
    // to past calls to constrainPositive(), as described above.
    // Since we have multiplied everything by lcm, instead of
    // adding +1 to each relevant variable we must add +lcm.
    size_t pos;
    if (origTableaux_->system().angle()) {
        if (type) {
            // For taut angle structures, the only coordinate that is explicitly
            // constrained to be positive is the final scaling coordinate.
            // Even better, this coordinate is never moved by the column
            // permutation.
            pos = 3 * origTableaux_->tri().size();
            v[pos] = v[pos] + lcm;
        } else {
            // For strict angle structures, we pass type == nullptr, and we
            // constrain *all* coordinates as positive.
            for (pos = 0; pos <= 3 * origTableaux_->tri().size(); ++pos)
                v[pos] = v[pos] + lcm;
        }
    } else {
        // For normal and almost normal surfaces, we need to work through
        // each past call to constrainPositive() and/or constrainOct().
        const size_t nTets = origTableaux_->tri().size();

        // First take into account the quadrilateral types...
        for (i = 0; i < nTets; ++i)
            if (type[i] && type[i] < 4) {
                pos = columnPerm[3 * i + type[i] - 1];
                v[pos] = v[pos] + lcm;
            }
        // ... and then the triangle types.
        for (i = 3 * nTets; i < v.size(); ++i)
            if (type[i - 2 * nTets]) {
                pos = columnPerm[i];
                v[pos] = v[pos] + lcm;
            }

        // Next take into account the changes of variable due to
        // past calls to constrainOct().
        if (octPrimary_ >= 0) {
            pos = columnPerm[octPrimary_];
            v[pos] = v[pos] + lcm;
            v[columnPerm[octSecondary_]] = v[pos];
        }
    }

    // To finish, divide through by the gcd so we have the
    // smallest multiple that is an integer vector.
    v.scaleDown();
    return v;
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::pivot(size_t outCol, size_t inCol) {
    size_t defRow = basisRow_[outCol];
    basisRow_[outCol] = -1;
    basisRow_[inCol] = defRow;
    basis_[defRow] = inCol;

    // Make sure that inCol has a positive coefficient in row defRow.
    IntType base;
    entry(defRow, inCol, base);
    if (base < 0) {
        base.negate();
        rhs_[defRow].negate();
        rowOps_.negateRow(defRow);
    }

    // Walk through the entire tableaux and perform row operations
    // to ensure that the only non-zero entry in column \a inCol
    // is the entry base in row defRow (as extracted above).
    IntType coeff, gcdRow;
    for (size_t r = 0; r < rank_; ++r) {
        if (r == defRow)
            continue;

        // We will reuse coeff, to avoid too many temporary IntTypes.
        // We first set coeff here, and then we reuse and alter it within the
        // IF block below.
        entry(r, inCol, coeff);
        if (! coeff.isZero()) {
            // Perform the row operation on the matrix...
            gcdRow = rowOps_.combRowAndNorm(base, r, coeff, defRow);

            // ... and on the right-hand side also.
            // We already know that gcdRow must divide into rhs_[r],
            // since rhs_ is obtained by multiplying the integer
            // matrix rowOps_ with an integer vector.
            rhs_[r] *= base;
            coeff *= rhs_[defRow];
            rhs_[r] -= coeff;
            rhs_[r].divByExact(gcdRow);
        }
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::findInitialBasis() {
    // Start with all variables active but non-basic.
    std::fill(basisRow_, basisRow_ + origTableaux_->columns(), -1);

    // We find our initial basis using Gauss-Jordan elimination.
    // Until we sit down and prove some results about the magnitude of
    // the intermediate integers that appear, we will need to do this
    // entire process using the arbitrary-precision Integer class.

    // We do not touch rhs_ at all, since our preconditions ensure that
    // rhs_ is the zero vector.

    // Temporary matrices:
    // tab = begins as starting tableaux, becomes identity in the basis columns.
    // ops = begins as identity matrix, becomes the final row operation matrix.

    // Build a dense copy of the starting tableaux, which we
    // will work with as we perform our Gauss-Jordan elimination.
    LPMatrix<Integer> tab(rank_, origTableaux_->columns());
    origTableaux_->fillInitialTableaux(tab);

    LPMatrix<Integer> ops(rank_, rank_);
    ops.initIdentity(rank_);

    // Off we go with our Gauss-Jordan elimination.
    // Since the original tableaux is full rank, we know in
    // advance that every row will define some basic variable.
    size_t row;
    size_t r, c;
    Integer base, coeff;
    Integer gcdRow;
    for (row = 0; row < rank_; ++row) {
        // Find the first non-zero entry in this row.
        // The corresponding column will become our next basic variable.
        for (c = 0; c < origTableaux_->columns(); ++c)
            if (basisRow_[c] < 0 /* non-basic variable */ &&
                    ! tab.entry(row, c).isZero())
                break;

        // Since the original tableaux has full rank, we must
        // have found a non-zero entry.  However, for sanity,
        // add some code to deal with the situation where we did not.
        if (c == origTableaux_->columns()) {
            // Impossible, assuming the matrix had the correct rank...
#ifdef REGINA_VERIFY_LPDATA
            std::cerr << "ERROR: No initial basis, bad rank."
                << std::endl;
            ::exit(1);
#endif
            // ... but deal with it anyway by just dropping rank.
            --rank_;
            if (row != rank_) {
                tab.swapRows(row, rank_);
                ops.swapRows(row, rank_);
            }
            --row; // We will ++row again for the next loop iteration.
            continue;
        }

        // Here is our non-zero entry.
        // Make this a basis variable.
        basis_[row] = c;
        basisRow_[c] = row;

        // Make the corresponding non-zero entry positive.
        base = tab.entry(row, c);
        if (base < 0) {
            base.negate();
            tab.negateRow(row);
            ops.negateRow(row);
        }

        // Make sure this basis variable has zero coefficients
        // in all other rows.
        for (r = 0; r < rank_; ++r) {
            if (r == row)
                continue;
            coeff = tab.entry(r, c);
            if (! coeff.isZero()) {
                gcdRow = ops.combRowAndNorm(base, r, coeff, row);
                tab.combRow(base, r, coeff, row, gcdRow);
            }
        }
    }

    // Copy the final tableaux into our own rowOps_ matrix.
    for (r = 0; r < rank_; ++r)
        for (c = 0; c < rank_; ++c)
            rowOps_.entry(r, c) = IntType(ops.entry(r, c));
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::makeFeasible() {
    ssize_t outCol;
    size_t outRow;
    IntType outEntry, tmp, v1, v2;

    // Variables for detecting cycling.
    //
    // The bits in oldBasis are a snapshot of which variables were in
    // the basis at some point in the past, and the bits in currBasis
    // indicate which variables are in the basis right now.
    // We store these using std::vector<bool>, which is typically a
    // specialised bit-packed vector (just like the old regina::Bitmask).
    //
    // We use Brent's method for detecting cycles:
    // We store a snapshot in oldBasis after 2^k pivots, for all k.
    // This means that, regardless of the length of the cycle or
    // the number of pivots that precede the cycle, we will
    // detect oldBasis == currBasis shortly after cycling occurs
    // (in particular, the total number of pivots that we take
    // overall is at most three times the total number of pivots
    // before the first repeated basis).
    size_t nCols = origTableaux_->columns();
    std::vector<bool> currBasis(nCols, false);
    for (size_t r = 0; r < rank_; ++r)
        currBasis[basis_[r]] = true;
    std::vector<bool> oldBasis = currBasis;
    unsigned long pow2 = 1;
    unsigned long nPivots = 0;

    while (true) {
#ifdef REGINA_COUNT_PIVOTS
        if (nPivots % 1000 == 0)
            std::cerr << "Pivots: " << nPivots << std::endl;
#endif
        // Locate a variable in the basis with negative value.
        // If there are many, choose the variable with largest 
        // magnitude negative value.
        outCol = -1;
        for (size_t r = 0; r < rank_; ++r)
            if (rhs_[r] < 0) {
                if (outCol < 0) {
                    // First candidate we've seen.
                    // Use it until we find something better.
                    outRow = r;
                    outCol = basis_[r];
                    entry(r, outCol, outEntry);
                    continue;
                }
                // Compare which variable is most negative.
                entry(r, basis_[r], tmp);
                v1 = rhs_[r]; v1 *= outEntry; // Avoid spurious temporaries.
                v2 = rhs_[outRow]; v2 *= tmp; // Avoid spurious temporaries.
                if (v1 < v2) {
                    outRow = r;
                    outCol = basis_[r];
                    outEntry = tmp;
                }
            }
        if (outCol < 0) {
            // All basis variables are non-negative!
            // This is a feasible basis; we're done.
            return;
        }

        // Fix this bad variable by pivoting it out.
        // The pivot-in variable must be the largest-index
        // column with negative coefficient in this row.
        ssize_t c;
        for (c = nCols - 1; c >= 0; --c)
            if (basisRow_[c] < 0 /* active, non-basic variable */ &&
                    entrySign(outRow, c) < 0)
                break;
        if (c < 0) {
            // There is no possible variable to pivot in.
            // The system is infeasible.
            feasible_ = false;
            return;
        }
        pivot(outCol, c);

        // Run our cycle-detection machinery.
        currBasis[outCol] = false;
        currBasis[c] = true;

        if (currBasis == oldBasis) {
            // We've cycled!
            // Switch to a slower but cycle-free pivot rule.
            makeFeasibleAntiCycling();
            return;
        }

        if (++nPivots == pow2) {
            oldBasis = currBasis;
            pow2 <<= 1;
            // On a modern (64-bit) system, pow2 will only overflow
            // after something like 10^19 pivots, and the human
            // will have given up in frustration long before this.
            // Nevertheless, make sure things work even in this case:
            if (! pow2) {
                makeFeasibleAntiCycling();
                return;
            }
        }
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::makeFeasibleAntiCycling() {
    ssize_t outCol;
#ifdef REGINA_COUNT_PIVOTS
    unsigned long nPivots = 0;
#endif
    while (true) {
#ifdef REGINA_COUNT_PIVOTS
        if (nPivots % 1000 == 0)
            std::cerr << "Pivots: " << nPivots << std::endl;
        ++nPivots;
#endif
        // Locate a variable in the basis with negative value.
        // If there are many, choose the one with largest index.
        outCol = -1;
        for (size_t r = 0; r < rank_; ++r)
            if (rhs_[r] < 0) {
                if (static_cast<ssize_t>(basis_[r]) > outCol)
                    outCol = basis_[r];
            }
        if (outCol < 0) {
            // All basis variables are non-negative!
            // This is a feasible basis; we're done.
            return;
        }

        // Fix this bad variable by pivoting it out.
        // The pivot-in variable must be the largest-index
        // column with negative coefficient in this row.
        ssize_t c;
        for (c = origTableaux_->columns() - 1; c >= 0; --c)
            if (basisRow_[c] < 0 /* active, non-basic variable */ &&
                    entrySign(basisRow_[outCol], c) < 0)
                break;
        if (c < 0) {
            // There is no possible variable to pivot in.
            // The system is infeasible.
            feasible_ = false;
            return;
        }
        pivot(outCol, c);
    }
}

template <class LPConstraint, typename IntType>
void LPData<LPConstraint, IntType>::verify() const {
    size_t r, c;
    for (r = 0; r < rank_; ++r) {
        // Check that rowOps_ is an inverse matrix.
        for (c = 0; c < rank_; ++c)
            if (r != c && entrySign(r, basis_[c])) {
                std::cerr << "VERIFY: Inverse error" << std::endl;
                ::exit(1);
            }

        // Check that each row has gcd = 1.
        IntType g; // Initialised to zero.
        for (c = 0; c < rowOps_.columns(); ++c)
            g.gcdWith(rowOps_.entry(r, c));
        if (g != 1) {
            std::cerr << "VERIFY: GCD error" << std::endl;
            ::exit(1);
        }
    }
}

} // namespace regina

#endif

