
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
#include "manifold/nngsfsloop.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"

namespace regina {

NNGSFSLoop::~NNGSFSLoop() {
    delete sfs_;
}

NAbelianGroup* NNGSFSLoop::getHomologyH1() const {
    // Is it a case we can deal with?
    if (sfs_->getBaseClass() != NSFSpace::o1 &&
            sfs_->getBaseClass() != NSFSpace::n1)
        return 0;
    if (sfs_->getBaseReflectors() > 0)
        return 0;

    // Just for safety (this should always be true anyway):
    if (sfs_->getBasePunctures() != 2)
        return 0;

    // Construct a matrix.
    // Generators: fibre, base curves, base boundaries, exceptional
    //             fibre boundaries, obstruction boundary, plus one for
    //             the loop created by the joining of boundaries.
    // Relations: base curve relation, exception fibre relations,
    //            obstruction relation, joining boundaries.
    unsigned long genus = sfs_->getBaseGenus();
    unsigned long punc = sfs_->getBasePunctures();
    unsigned long fibres = sfs_->getFibreCount();

    NMatrixInt m(fibres + 4, genus + punc + fibres + 3);

    unsigned long i, f;
    // The relation for the base orbifold:
    for (i = 1 + genus; i < 1 + genus + punc + fibres + 1; i++)
        m.entry(0, i) = 1;
    if (! sfs_->isBaseOrientable())
        for (i = 1; i < 1 + genus; i++)
            m.entry(0, i) = 2;

    // A relation for each exceptional fibre:
    NSFSFibre fibre;
    for (f = 0; f < fibres; f++) {
        fibre = sfs_->getFibre(f);
        m.entry(f + 1, 1 + genus + punc + f) = fibre.alpha;
        m.entry(f + 1, 0) = fibre.beta;
    }

    // A relation for the obstruction constant:
    m.entry(1 + fibres, 1 + genus + punc + fibres) = 1;
    m.entry(1 + fibres, 0) = sfs_->getObstruction();

    // Two relations for the joining of boundaries:
    m.entry(2 + fibres, 0) = -1;
    m.entry(2 + fibres, 0) += matchingReln_[0][0];
    m.entry(2 + fibres, 2 + genus) = matchingReln_[0][1];
    m.entry(3 + fibres, 1 + genus) = -1;
    m.entry(3 + fibres, 0) = matchingReln_[1][0];
    m.entry(3 + fibres, 2 + genus) = matchingReln_[1][1];

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(m);
    return ans;
}

std::ostream& NNGSFSLoop::writeName(std::ostream& out) const {
    sfs_->writeName(out);
    return out << " / [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& NNGSFSLoop::writeTeXName(std::ostream& out) const {
    sfs_->writeTeXName(out);
    return out << "_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}}";
}

void NNGSFSLoop::reduce() {
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
    long b = sfs_->getObstruction();
    if (b != 0) {
        sfs_->insertFibre(1, -b);
        matchingReln_[0][0] += b * matchingReln_[0][1];
        matchingReln_[1][0] += b * matchingReln_[1][1];
    }

    reduceBasis(matchingReln_);

    // See if we can do any better by reflecting the entire space
    // and adding (1,1) twists to bring the obstruction constant back
    // up to zero again.
    NMatrix2 compMatch =
        NMatrix2(1, 0, sfs_->getFibreCount(), 1) *
        NMatrix2(1, 0, 0, -1) *
        matchingReln_ *
        NMatrix2(1, 0, 0, -1);
    reduceBasis(compMatch);

    if (simpler(compMatch, matchingReln_)) {
        // Do it.
        matchingReln_ = compMatch;
        sfs_->complementAllFibres();
    }
}

void NNGSFSLoop::reduceBasis(NMatrix2& reln) {
    // Note that twisting will never change the (0,1) entry of the
    // matrix.  It therefore seems useful to fix this first.  If the
    // matrix has determinant +1 then we have some control over this.
    if (reln.determinant() == 1 && reln[0][1] < 0)
        reln.invert();

    // Use (1,1) / (1,-1) pairs to make the top-left element of the
    // matrix zero, if we can.
    if (reln[0][1] != 0) {
        long nOps = reln[0][0] / reln[0][1];
        if (nOps > 0) {
            for (long i = 0; i < nOps; i++) {
                reln[0][0] -= reln[0][1];
                reln[1][0] -= reln[1][1];
                reln[1][0] -= reln[0][0];
                reln[1][1] -= reln[0][1];
            }
        } else if (nOps < 0) {
            for (long i = 0; i > nOps; i--) {
                reln[0][0] += reln[0][1];
                reln[1][0] += reln[1][1];
                reln[1][0] += reln[0][0];
                reln[1][1] += reln[0][1];
            }
        }
    }
}

bool NNGSFSLoop::simpler(const NMatrix2& m1, const NMatrix2& m2) {
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

