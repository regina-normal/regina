
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

#include "algebra/intersectionform.h"

namespace regina {

IntersectionForm::IntersectionForm(MatrixInt form) :
        matrix_(form), rank_(0), signature_(0), even_(true) {
    if (form.rows() != form.columns())
        throw InvalidArgument("IntersectionForm requires a square matrix.");

    unsigned long n = form.rows();
    unsigned long i;

    for (i = 0; i < n; ++i)
        if (form.entry(i, i) % 2 != 0) {
            even_ = false;
            break;
        }

    // Determine the rank and signature by diagonalising the matrix.
    //
    // Here we will allow operations that scale individual rows/columns,
    // since this lets us stay with in exact integer arithmetic, and it
    // will not change either the rank or the signature.

    while (rank_ < n) {
        // INV: The first rank_ rows and columns contain non-zero entries on
        // the diagonal and zero entries everywhere else.  These non-zero
        // diagonal entries have been added into both rank_ and signature_.

        Integer d = form.entry(rank_, rank_);
        if (d != 0) {
            for (i = rank_ + 1; i < n; ++i) {
                Integer e = form.entry(i, rank_);
                if (e != 0) {
                    if (form.entry(rank_, i) != e)
                        throw InvalidArgument(
                            "IntersectionForm requires a symmetric matrix.");
                    Integer gcd = d.gcd(e);

                    d.divByExact(gcd);
                    e.divByExact(gcd);

                    form.multRow(i, d, rank_);
                    form.addRow(rank_, i, -e, rank_);
                    form.multCol(i, d, rank_);
                    form.addCol(rank_, i, -e, rank_);
                }
            }

            if (d > 0)
                ++signature_;
            else
                --signature_;
            ++rank_;
            continue;
        }

        // The next diagonal entry is zero.
        // See if we can find a non-zero diagonal entry further down.
        for (i = rank_ + 1; i < n; ++i)
            if (form.entry(i, i) != 0)
                break;
        if (i < n) {
            form.swapRows(rank_, i);
            form.swapCols(rank_, i);
            continue;
        }

        // All remaining diagonal entries are zero.
        // See if we can find a non-zero entry elsewhere to use.
        for (unsigned long r = rank_; r < n; ++r)
            for (i = r + 1; i < n; ++i)
                if (form.entry(r, i) != 0) {
                    // Got one.
                    form.addRow(i, r);
                    form.addCol(i, r);
                    if (r != rank_) {
                        form.swapRows(rank_, r);
                        form.swapCols(rank_, r);
                    }
                    goto loopAgain;
                }

        // All entries above the main diagonal are zero.
        // This should be the end of it, but we will check the below-diagonal
        // entries also to finish verifying that the matrix is symmetric.
        for (unsigned long r = rank_; r < n; ++r)
            for (i = r + 1; i < n; ++i)
                if (form.entry(i, r) != 0)
                    throw InvalidArgument(
                        "IntersectionForm requires a symmetric matrix.");
        break;

        loopAgain:
            ;
    }
}

void IntersectionForm::writeTextShort(std::ostream& out) const {
    if (even_)
        out << "Even";
    else
        out << "Odd";
    out << ", rank = " << rank_ << ", sig = " << signature_ << ": ";
    matrix_.writeTextShort(out);
}

void IntersectionForm::writeTextLong(std::ostream& out) const {
    if (even_)
        out << "Even";
    else
        out << "Odd";
    out << ", rank = " << rank_ << ", signature = " << signature_ << "\n\n";
    matrix_.writeTextLong(out);
}

} // namespace regina

