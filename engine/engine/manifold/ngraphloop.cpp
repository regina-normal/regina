
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "algebra/nabeliangroup.h"
#include "manifold/ngraphloop.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"
#include <cstdlib> // For labs().

namespace regina {

NGraphLoop::~NGraphLoop() {
    delete sfs_;
}

NAbelianGroup* NGraphLoop::getHomologyH1() const {
    // Just for safety (this should always be true anyway):
    if (sfs_->punctures(false) != 2 || sfs_->punctures(true) != 0)
        return 0;

    // Construct a matrix.
    // Generators: fibre, base curves, two base boundaries, exceptional
    //             fibre boundaries, obstruction boundary,
    //             reflector boundaries, reflector half-fibres, plus one
    //             for the loop created by the joining of boundaries.
    // Relations: base curve relation, exception fibre relations,
    //            obstruction relation, reflector relations,
    //            fibre constraint, joining boundaries.
    unsigned long genus = sfs_->baseGenus();
    unsigned long fibres = sfs_->fibreCount();
    unsigned long ref = sfs_->reflectors();

    // If we have an orientable base space, we get two curves per genus.
    // The easiest thing to do is just to double the genus now.
    if (sfs_->baseOrientable())
        genus *= 2;

    NMatrixInt m(fibres + ref + 5, genus + fibres + 2 * ref + 5);

    unsigned long i, f;
    // The relation for the base orbifold:
    for (i = 1 + genus; i < 1 + genus + 2 + fibres + 1 + ref; i++)
        m.entry(0, i) = 1;
    if (! sfs_->baseOrientable())
        for (i = 1; i < 1 + genus; i++)
            m.entry(0, i) = 2;

    // A relation for each exceptional fibre:
    NSFSFibre fibre;
    for (f = 0; f < fibres; f++) {
        fibre = sfs_->fibre(f);
        m.entry(f + 1, 1 + genus + 2 + f) = fibre.alpha;
        m.entry(f + 1, 0) = fibre.beta;
    }

    // A relation for the obstruction constant:
    m.entry(1 + fibres, 1 + genus + 2 + fibres) = 1;
    m.entry(1 + fibres, 0) = sfs_->obstruction();

    // A relation for each reflector boundary:
    for (i = 0; i < ref; i++) {
        m.entry(2 + fibres + i, 0) = -1;
        m.entry(2 + fibres + i, 1 + genus + 2 + fibres + 1 + ref + i) = 2;
    }

    // A relation constraining the fibre.  This relationship only
    // appears in some cases; otherwise we will just have a (harmless)
    // zero row in the matrix.
    if (sfs_->reflectors(true))
        m.entry(2 + fibres + ref, 0) = 1;
    else if (sfs_->fibreReversing())
        m.entry(2 + fibres + ref, 0) = 2;

    // Two relations for the joining of boundaries:
    m.entry(3 + fibres + ref, 0) = -1;
    m.entry(3 + fibres + ref, 0) += matchingReln_[0][0];
    m.entry(3 + fibres + ref, 2 + genus) = matchingReln_[0][1];
    m.entry(4 + fibres + ref, 1 + genus) = -1;
    m.entry(4 + fibres + ref, 0) = matchingReln_[1][0];
    m.entry(4 + fibres + ref, 2 + genus) = matchingReln_[1][1];

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(m);
    return ans;
}

std::ostream& NGraphLoop::writeName(std::ostream& out) const {
    sfs_->writeName(out);
    return out << " / [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& NGraphLoop::writeTeXName(std::ostream& out) const {
    sfs_->writeTeXName(out);
    return out << "_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}}";
}

void NGraphLoop::reduce() {
    /**
     * Things to observe:
     *
     * 1. Inverting the matching matrix is harmless (it corresponds to
     * rotating the space a half-turn to switch the two boundary tori).
     *
     * 2. If we add a (1,1) twist to the SFS we can compensate by either:
     *    - setting row 2 -> row 2 + row 1, or
     *    - setting col 1 -> col 1 - col 2.
     */

    // Bring the SFS obstruction constant back to zero.
    long b = sfs_->obstruction();
    if (b != 0) {
        sfs_->insertFibre(1, -b);
        matchingReln_[0][0] += b * matchingReln_[0][1];
        matchingReln_[1][0] += b * matchingReln_[1][1];
    }

    reduce(matchingReln_);

    // See if we can do any better by reflecting the entire space
    // and adding (1,1) twists to bring the obstruction constant back
    // up to zero again.
    NMatrix2 compMatch =
        NMatrix2(1, 0, sfs_->fibreCount(), 1) *
        NMatrix2(1, 0, 0, -1) *
        matchingReln_ *
        NMatrix2(1, 0, 0, -1);
    reduce(compMatch);

    if (simpler(compMatch, matchingReln_)) {
        // Do it.
        matchingReln_ = compMatch;
        sfs_->complementAllFibres();
    }
}

void NGraphLoop::reduce(NMatrix2& reln) {
    // Reduce both the original and the inverse, and see who comes out
    // on top.
    reduceBasis(reln);

    NMatrix2 inv = reln.inverse();
    reduceBasis(inv);

    if (simpler(inv, reln))
        reln = inv;
}

void NGraphLoop::reduceBasis(NMatrix2& reln) {
    // Use (1,1) / (1,-1) pairs to make the top-left element of the
    // matrix as close to zero as possible.
    if (reln[0][1] != 0 && reln[0][0] != 0) {
        long nOps = (labs(reln[0][0]) + ((labs(reln[0][1]) - 1) / 2)) /
            labs(reln[0][1]);
        if ((reln[0][0] > 0 && reln[0][1] > 0) ||
                (reln[0][0] < 0 && reln[0][1] < 0)) {
            // Same signs.
            for (long i = 0; i < nOps; i++) {
                reln[0][0] -= reln[0][1];
                reln[1][0] -= reln[1][1];
                reln[1][0] -= reln[0][0];
                reln[1][1] -= reln[0][1];
            }
        } else {
            // Opposite signs.
            for (long i = 0; i < nOps; i++) {
                reln[0][0] += reln[0][1];
                reln[1][0] += reln[1][1];
                reln[1][0] += reln[0][0];
                reln[1][1] += reln[0][1];
            }
        }
    } else {
        // TODO: We can still do something here.
    }
}

bool NGraphLoop::simpler(const NMatrix2& m1, const NMatrix2& m2) {
    long maxAbs1 = 0, maxAbs2 = 0;
    unsigned nZeroes1 = 0, nZeroes2 = 0;

    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (m1[i][j] > maxAbs1)
                maxAbs1 = m1[i][j];
            if (m1[i][j] < -maxAbs1)
                maxAbs1 = -m1[i][j];
            if (m2[i][j] > maxAbs2)
                maxAbs2 = m2[i][j];
            if (m2[i][j] < -maxAbs2)
                maxAbs2 = -m2[i][j];

            if (m1[i][j] == 0)
                nZeroes1++;
            if (m2[i][j] == 0)
                nZeroes2++;
        }

    if (maxAbs1 < maxAbs2)
        return true;
    if (maxAbs1 > maxAbs2)
        return false;

    if (nZeroes1 > nZeroes2)
        return true;
    if (nZeroes1 < nZeroes2)
        return false;

    // Down to lexicographical order.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (m1[i][j] < m2[i][j])
                return true;
            else if (m1[i][j] > m2[i][j])
                return false;

    // They're identical.
    return false;
}

} // namespace regina

