
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

#include <algorithm>
#include "algebra/abeliangroup.h"
#include "maths/matrixops.h"
#include "utilities/stringutils.h"

namespace regina {

void AbelianGroup::addTorsion(Integer degree) {
    for (auto it = invariantFactors_.rbegin();
            it != invariantFactors_.rend(); ++it) {
        // INV: We still need to introduce an torsion element of degree,
        // and we know that degree divides all invariant factors beyond it
        // (i.e. all invariant factors that have already been seen in
        // this reverse loop).

        // Replace (degree, *it) with (gcd, lcm).

        Integer g = degree.gcd(*it);
        degree.divByExact(g);
        (*it) *= degree;

        degree = g;
        if (degree == 1)
            return;
    }

    if (degree > 1)
        invariantFactors_.push_front(degree);
}

void AbelianGroup::addTorsionElements(const std::multiset<Integer>& torsion) {
    // Build a presentation matrix for the torsion.
    MatrixInt a(invariantFactors_.size() + torsion.size());

    // Put our own invariant factors in the top.
    unsigned i=0;
    for (const auto& f : invariantFactors_) {
        a.entry(i,i) = f;
        ++i;
    }

    // Put the passed torsion elements beneath.
    for (const auto& f : torsion) {
        a.entry(i,i) = f;
        ++i;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addGroup(const MatrixInt& presentation) {
    // Prepare to calculate invariant factors.
    size_t len = invariantFactors_.size();
    MatrixInt a(len + presentation.rows(), len + presentation.columns());

    // Fill in the complete presentation matrix.
    // Fill the bottom half of the matrix with the presentation.
    for (unsigned i=0; i<presentation.rows(); i++)
        for (unsigned j=0; j<presentation.columns(); j++)
            a.entry(len + i, len + j) = presentation.entry(i, j);

    // Fill in the invariant factors in the top.
    unsigned i = 0;
    for (const auto& f : invariantFactors_) {
        a.entry(i,i) = f;
        ++i;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addGroup(const AbelianGroup& group) {
    rank_ += group.rank_;

    // Work out the torsion elements.
    if (invariantFactors_.empty()) {
        // Copy the other group's factors!
        invariantFactors_ = group.invariantFactors_;
        return;
    }
    if (group.invariantFactors_.empty())
        return;

    // We will have to calculate the invariant factors ourselves.
    size_t len = invariantFactors_.size() + group.invariantFactors_.size();
    MatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i = 0;
    for (const auto& f : invariantFactors_) {
        a.entry(i,i) = f;
        ++i;
    }

    // Put the other group's invariant factors beneath.
    for (const auto& f : group.invariantFactors_) {
        a.entry(i,i) = f;
        ++i;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

unsigned AbelianGroup::torsionRank(const Integer& degree) const {
    unsigned ans = 0;
    // Because we have SNF, we can bail as soon as we reach a factor
    // that is not divisible by degree.
    for (auto it = invariantFactors_.rbegin();
            it != invariantFactors_.rend(); ++it)
        if (((*it) % degree) == 0)
            ++ans;
        else
            return ans;
    return ans;
}

void AbelianGroup::writeTextShort(std::ostream& out, bool utf8) const {
    bool writtenSomething = false;

    if (rank_ > 0) {
        if (rank_ > 1)
            out << rank_ << ' ';
        if (utf8)
            out << "\u2124";
        else
            out << 'Z';
        writtenSomething = true;
    }

    auto it = invariantFactors_.begin();
    Integer currDegree;
    unsigned currMult = 0;
    while(true) {
        if (it != invariantFactors_.end()) {
            if ((*it) == currDegree) {
                ++currMult;
                ++it;
                continue;
            }
        }
        if (currMult > 0) {
            if (writtenSomething)
                out << " + ";
            if (currMult > 1)
                out << currMult << ' ';
            if (utf8)
                out << "\u2124" << regina::subscript(currDegree);
            else
                out << "Z_" << currDegree.stringValue();
            writtenSomething = true;
        }
        if (it == invariantFactors_.end())
            break;
        currDegree = *it;
        currMult = 1;
        ++it;
    }

    if (! writtenSomething)
        out << '0';
}

void AbelianGroup::replaceTorsion(const MatrixInt& matrix, bool rankFromCols) {
    // Delete any preexisting torsion.
    invariantFactors_.clear();

    // Run up the diagonal until we hit 1.
    // Hopefully this will be faster than running down the diagonal
    // looking for 0 because the SNF calculation should end up with
    // many 1s for a unnecessarily large presentation matrix such as
    // is produced for instance by homology calculations.
    unsigned long i;
    if (rankFromCols) {
        i = matrix.columns();

        unsigned long rows = matrix.rows();
        if (rows < i) {
            // There are more columns than rows; these extras cols must be zero.
            rank_ += (i - rows);
            i = rows;
        }
    } else {
        i = matrix.rows();

        unsigned long cols = matrix.columns();
        if (cols < i) {
            // There are more rows than columns; these extras rows must be zero.
            rank_ += (i - cols);
            i = cols;
        }
    }

    // Now i is precisely the length of the diagonal.

    while (i > 0) {
        --i;
        if (matrix.entry(i, i) == 0)
            ++rank_;
        else if (matrix.entry(i, i) == 1)
            break;
        else
            invariantFactors_.push_back(matrix.entry(i, i));
    }

    // We inserted the invariant factors in reverse order; fix this now.
    std::reverse(invariantFactors_.begin(), invariantFactors_.end());
}

void AbelianGroup::writeXMLData(std::ostream& out) const {
    out << "<abeliangroup rank=\"" << rank_ << "\"> ";
    for (const auto& f : invariantFactors_)
        out << f << ' ';
    out << "</abeliangroup>";
}

// ---N--> CC --M-->  ie: M*N = 0.
AbelianGroup::AbelianGroup(MatrixInt M, MatrixInt N) : rank_(0) {
    metricalSmithNormalForm(N);
    replaceTorsion(N, false /* rank comes from the zero rows of N */);

    rank_ -= M.rowEchelonForm(); // subtracts the rank of M
}

AbelianGroup::AbelianGroup(MatrixInt M, MatrixInt N, const Integer &p) {
    Integer cof(p.abs());
    rank_ = N.rows();

    metricalSmithNormalForm(N);
    unsigned long lim = (N.rows() < N.columns() ? N.rows() : N.columns() );

    if (cof == 0) {
        for (unsigned long i=0; i<lim; i++)
            if (N.entry(i,i) != 0) {
                rank_--;
                if (N.entry(i,i) > 1)
                    addTorsion(N.entry(i,i));
            }
    } else {
        for (unsigned long i=0; i<lim; i++)
            if (N.entry(i,i) !=0) {
                rank_--;
                Integer g( N.entry(i,i).gcd(cof) );
                if (g > 1)
                    addTorsion(g);
            }
    }

    metricalSmithNormalForm(M);
    lim = (M.rows() < M.columns() ? M.rows() : M.columns() );
    for (unsigned long i=0; i<lim; i++) {
        if (M.entry(i,i) != 0) {
            rank_--;
            if (cof != 0) {
                Integer g( M.entry(i,i).gcd(cof) );
                if (g>1)
                    addTorsion(g);
            }
        }
    }
    for ( ; rank_ > 0; --rank_)
        addTorsion(cof);
}

} // namespace regina

