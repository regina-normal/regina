
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/abeliangroup.h"
#include "maths/matrixops.h"
#include "utilities/stringutils.h"

namespace regina {

const Integer& AbelianGroup::invariantFactor(size_t index) const {
    std::multiset<Integer>::const_iterator it = invariantFactors.begin();
    advance(it, index);
    return *it;
}

void AbelianGroup::addTorsionElement(const Integer& degree,
        unsigned mult) {
    // If there are no current torsion elements, just throw in the new
    // ones.
    if (invariantFactors.empty()) {
        for (unsigned j=0; j<mult; j++)
            invariantFactors.insert(invariantFactors.begin(), degree);
        return;
    }

    // Build a presentation matrix for the torsion.
    size_t len = invariantFactors.size() + mult;
    MatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i=0;
    std::multiset<Integer>::const_iterator it;
    for (it = invariantFactors.begin(); it != invariantFactors.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Put the passed torsion elements beneath.
    for (unsigned j=0; j<mult; j++) {
        a.entry(i,i) = degree;
        i++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addTorsionElements(const std::multiset<Integer>&
        torsion) {
    // Build a presentation matrix for the torsion.
    size_t len = invariantFactors.size() + torsion.size();
    MatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i=0;
    std::multiset<Integer>::const_iterator it;
    for (it = invariantFactors.begin(); it != invariantFactors.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Put the passed torsion elements beneath.
    for (it = torsion.begin(); it != torsion.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addGroup(const MatrixInt& presentation) {
    // Prepare to calculate invariant factors.
    size_t len = invariantFactors.size();
    MatrixInt a(len + presentation.rows(), len + presentation.columns());

    // Fill in the complete presentation matrix.
    // Fill the bottom half of the matrix with the presentation.
    unsigned i,j;
    for (i=0; i<presentation.rows(); i++)
        for (j=0; j<presentation.columns(); j++)
            a.entry(len + i, len + j) = presentation.entry(i, j);
    
    // Fill in the invariant factors in the top.
    i = 0;
    std::multiset<Integer>::const_iterator it;
    for (it = invariantFactors.begin(); it != invariantFactors.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void AbelianGroup::addGroup(const AbelianGroup& group) {
    rank_ += group.rank_;

    // Work out the torsion elements.
    if (invariantFactors.empty()) {
        // Copy the other group's factors!
        invariantFactors = group.invariantFactors;
        return;
    }
    if (group.invariantFactors.empty())
        return;

    // We will have to calculate the invariant factors ourselves.
    size_t len = invariantFactors.size() + group.invariantFactors.size();
    MatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i = 0;
    std::multiset<Integer>::const_iterator it;
    for (it = invariantFactors.begin(); it != invariantFactors.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Put the other group's invariant factors beneath.
    for (it = group.invariantFactors.begin();
            it != group.invariantFactors.end(); it++) {
        a.entry(i,i) = *it;
        i++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

unsigned AbelianGroup::torsionRank(const Integer& degree) const {
    std::multiset<Integer>::const_reverse_iterator it;
    unsigned ans = 0;
    // Because we have SNF, we can bail as soon as we reach a factor
    // that is not divisible by degree.
    for (it = invariantFactors.rbegin(); it != invariantFactors.rend(); it++)
        if (((*it) % degree) == 0)
            ans++;
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

    std::multiset<Integer>::const_iterator it =
        invariantFactors.begin();
    Integer currDegree;
    unsigned currMult = 0;
    while(true) {
        if (it != invariantFactors.end()) {
            if ((*it) == currDegree) {
                currMult++;
                it++;
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
        if (it == invariantFactors.end())
            break;
        currDegree = *it;
        currMult = 1;
        it++;
    }

    if (! writtenSomething)
        out << '0';
}

void AbelianGroup::replaceTorsion(const MatrixInt& matrix) {
    // Delete any preexisting torsion.
    invariantFactors.clear();

    // Run up the diagonal until we hit 1.
    // Hopefully this will be faster than running down the diagonal
    // looking for 0 because the SNF calculation should end up with
    // many 1s for a unnecessarily large presentation matrix such as
    // is produced for instance by homology calculations.
    unsigned long rows = matrix.rows();
    unsigned long i = matrix.columns();
    if (i > rows) {
        rank_ += (i - rows);
        i = rows;
    }
    while (i > 0) {
        if (matrix.entry(i-1, i-1) == 0)
            rank_++;
        else if (matrix.entry(i-1, i-1) == 1)
            return;
        else
            invariantFactors.insert(invariantFactors.begin(),
                matrix.entry(i-1, i-1));
        i--;
    }
}

void AbelianGroup::writeXMLData(std::ostream& out) const {
    out << "<abeliangroup rank=\"" << rank_ << "\"> ";
    for (std::multiset<Integer>::const_iterator it =
            invariantFactors.begin(); it != invariantFactors.end(); it++)
        out << (*it) << ' ';
    out << "</abeliangroup>";
}

// ---N--> CC --M-->  ie: M*N = 0.
AbelianGroup::AbelianGroup(const MatrixInt& M, const MatrixInt& N) {
    rank_ = N.rows();
    MatrixInt tempN(N);
    metricalSmithNormalForm(tempN);
    unsigned long lim =
        (tempN.rows() < tempN.columns() ? tempN.rows() : tempN.columns() );
    std::multiset<Integer> torsion;
    for (unsigned long i=0; i<lim; i++) {
        if (tempN.entry(i,i) != 0) {
            rank_--;
            if (tempN.entry(i,i) > 1)
                torsion.insert(tempN.entry(i,i));
        }
    }
    addTorsionElements(torsion);

    MatrixInt tempM(M);
    metricalSmithNormalForm(tempM);
    lim = (tempM.rows() < tempM.columns() ? tempM.rows() : tempM.columns());
    for (unsigned long i=0; i<lim; ++i) {
        if (tempM.entry(i,i) != 0)
            rank_--;
    }
}

AbelianGroup::AbelianGroup(const MatrixInt& M, const MatrixInt& N,
        const Integer &p) {
    Integer cof(p.abs());
    rank_ = N.rows();

    MatrixInt tempN(N);
    metricalSmithNormalForm(tempN);
    unsigned long lim =
        (tempN.rows() < tempN.columns() ? tempN.rows() : tempN.columns() );
    std::multiset<Integer> torsion;

    if (cof == 0) {
        for (unsigned long i=0; i<lim; i++)
            if (tempN.entry(i,i) != 0) {
                rank_--;
                if (tempN.entry(i,i) > 1)
                    torsion.insert(tempN.entry(i,i));
            }
    } else {
        for (unsigned long i=0; i<lim; i++)
            if (tempN.entry(i,i) !=0) {
                rank_--;
                Integer g( tempN.entry(i,i).gcd(cof) );
                if (g > 1)
                    torsion.insert(g);
            }
    }

    MatrixInt tempM(M);
    metricalSmithNormalForm(tempM);
    lim = (tempM.rows() < tempM.columns() ? tempM.rows() : tempM.columns() );
    for (unsigned long i=0; i<lim; i++) {
        if (tempM.entry(i,i) != 0) {
            rank_--;
            if (cof != 0) {
                Integer g( tempM.entry(i,i).gcd(cof) );
                if (g>1)
                    torsion.insert(g);
            }
        }
    }
    if (cof != 0) {
        for (unsigned long i=0; i<rank_; i++)
            torsion.insert(cof);
        rank_ = 0;
    }
    addTorsionElements(torsion);
}

} // namespace regina

