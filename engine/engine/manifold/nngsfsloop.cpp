
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

#include "manifold/nngsfsloop.h"
#include "manifold/nsfs.h"

namespace regina {

NNGSFSLoop::~NNGSFSLoop() {
    delete sfs_;
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

