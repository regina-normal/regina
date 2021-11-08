
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
    // Loop from the largest invariant factor to the smallest.
    for (auto it = revInvFactors_.begin(); it != revInvFactors_.end(); ++it) {
        // INV: We still need to introduce a torsion element of degree,
        // and we know that degree divides all invariant factors beyond it
        // (i.e. all invariant factors that have already been seen in
        // this loop).

        // Replace (degree, *it) with (gcd, lcm).

        Integer g = degree.gcd(*it);
        degree.divByExact(g);
        (*it) *= degree;

        degree = g;
        if (degree == 1)
            return;
    }

    if (degree > 1)
        revInvFactors_.push_back(degree);
}

void AbelianGroup::addTorsionElements(const std::multiset<Integer>& torsion) {
    // Build a presentation matrix for the torsion.
    MatrixInt a(revInvFactors_.size() + torsion.size());

    // Put our own invariant factors in the top.
    unsigned i=0;
    for (auto it = revInvFactors_.rbegin(); it != revInvFactors_.rend(); ++it) {
        a.entry(i,i) = *it;
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
    size_t len = revInvFactors_.size();
    MatrixInt a(len + presentation.rows(), len + presentation.columns());

    // Fill in the complete presentation matrix.
    // Fill the bottom half of the matrix with the presentation.
    for (unsigned i=0; i<presentation.rows(); i++)
        for (unsigned j=0; j<presentation.columns(); j++)
            a.entry(len + i, len + j) = presentation.entry(i, j);

    // Fill in the invariant factors in the top.
    unsigned i = 0;
    for (auto it = revInvFactors_.rbegin(); it != revInvFactors_.rend(); ++it) {
        a.entry(i,i) = *it;
        ++i;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addGroup(const AbelianGroup& group) {
    rank_ += group.rank_;

    // Work out the torsion elements.
    if (revInvFactors_.empty()) {
        // Copy the other group's factors!
        revInvFactors_ = group.revInvFactors_;
        return;
    }
    if (group.revInvFactors_.empty())
        return;

    // We will have to calculate the invariant factors ourselves.
    size_t len = revInvFactors_.size() + group.revInvFactors_.size();
    MatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i = 0;
    for (auto it = revInvFactors_.rbegin(); it != revInvFactors_.rend(); ++it) {
        a.entry(i,i) = *it;
        ++i;
    }

    // Put the other group's invariant factors beneath.
    for (auto it = group.revInvFactors_.rbegin();
            it != group.revInvFactors_.rend(); ++it) {
        a.entry(i,i) = *it;
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
    for (const auto& factor : revInvFactors_)
        if (factor % degree == 0)
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

    auto it = revInvFactors_.rbegin();
    Integer currDegree;
    unsigned currMult = 0;
    while(true) {
        if (it != revInvFactors_.rend()) {
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
        if (it == revInvFactors_.rend())
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
    revInvFactors_.clear();

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

    // We inserted the invariant factors in reverse order, but this is
    // exactly what we need to do.
    while (i > 0) {
        --i;
        if (matrix.entry(i, i) == 0)
            ++rank_;
        else if (matrix.entry(i, i) == 1)
            break;
        else
            revInvFactors_.push_back(matrix.entry(i, i));
    }
}

void AbelianGroup::writeXMLData(std::ostream& out) const {
    out << "<abeliangroup rank=\"" << rank_ << "\"> ";
    for (auto it = revInvFactors_.rbegin(); it != revInvFactors_.rend(); ++it)
        out << (*it) << ' ';
    out << "</abeliangroup>";
}

// ---N--> CC --M-->  ie: M*N = 0.
AbelianGroup::AbelianGroup(MatrixInt M, MatrixInt N) : rank_(0) {
    smithNormalForm(N);
    replaceTorsion(N, false /* rank comes from the zero rows of N */);

    rank_ -= M.rowEchelonForm(); // subtracts the rank of M
}

AbelianGroup::AbelianGroup(MatrixInt M, MatrixInt N, const Integer &p) {
    Integer cof(p.abs());
    rank_ = N.rows();

    smithNormalForm(N);
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

    smithNormalForm(M);
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

