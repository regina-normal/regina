
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

#include "manifold/nngsfspair.h"
#include "manifold/nsfs.h"

namespace regina {

NNGSFSPair::~NNGSFSPair() {
    delete sfs_[0];
    delete sfs_[1];
}

std::ostream& NNGSFSPair::writeName(std::ostream& out) const {
    sfs_[0]->writeName(out);
    out << " U/m ";
    sfs_[1]->writeName(out);
    return out << ", m = [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& NNGSFSPair::writeTeXName(std::ostream& out) const {
    sfs_[0]->writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}} ";
    return sfs_[1]->writeTeXName(out);
}

void NNGSFSPair::reduce() {
    /**
     * Things to observe:
     *
     * 1. If we add a (1,1) twist to sfs_[0] we can compensate by setting
     *    col 1 -> col 1 - col 2.
     *
     * 2. If we add a (1,1) twist to sfs_[1] we can compensate by setting
     *    row 2 -> row 2 + row 1.
     *
     * 3. We can negate the entire matrix without problems (this
     *    corresponds to rotating one space by 180 degrees).
     *
     * 4. If we negate all fibres in sfs_[0] we can compensate by
     *    negating col 1, though note that this negates the determinant
     *    of the matrix.
     *
     * 5. If we negate all fibres in sfs_[1] we can compensate by
     *    negating row 1, though again note that this negates the
     *    determinant of the matrix.
     *
     * 6. If we wish to swap the two spaces, we invert M.
     */

    // Bring the obstruction constant for each SFS down to zero.
    long b;

    b = sfs_[0]->getObstruction();
    if (b != 0) {
        sfs_[0]->insertFibre(1, -b);
        matchingReln_[0][0] += b * matchingReln_[0][1];
        matchingReln_[1][0] += b * matchingReln_[1][1];
    }

    b = sfs_[1]->getObstruction();
    if (b != 0) {
        sfs_[1]->insertFibre(1, -b);
        matchingReln_[1][0] -= b * matchingReln_[0][0];
        matchingReln_[1][1] -= b * matchingReln_[0][1];
    }

    /**
     * If one of the spaces is M/n2, we can replace it with D:(2,1)(2,-1)
     * with fibre and orbifold curves switched.  To preserve the
     * determinant of the matching matrix we will actually use a [0,1,-1,0]
     * switch instead of a [0,1,1,0] switch.
     *
     * In fact we will use D:(2,1)(2,1) instead, which means:
     *
     * M_basis = [  0 1 ] [  1 0 ] D_basis = [ -1 1 ] D_basis;
     *           [ -1 0 ] [ -1 1 ]           [ -1 0 ]
     *
     * D_basis = [ 1 0 ] [  0 -1 ] M_basis = [ 0 -1 ] M_basis.
     *           [ 1 1 ] [  1  0 ]           [ 1 -1 ]
     */

    for (int i = 0; i < 2; i++)
        if (sfs_[i]->getBaseClass() == NSFSpace::n2 &&
                sfs_[i]->getBaseGenus() == 1 &&
                (! sfs_[i]->isBaseOrientable()) &&
                sfs_[i]->getBasePunctures() == 1 &&
                sfs_[i]->getBaseReflectors() == 0 &&
                sfs_[i]->getFibreCount() == 0 &&
                sfs_[i]->getObstruction() == 0) {
            delete sfs_[i];

            sfs_[i] = new NSFSpace(NSFSpace::o1, 0, 1, 0);
            sfs_[i]->insertFibre(2, 1);
            sfs_[i]->insertFibre(2, 1);

            if (i == 0)
                matchingReln_ = matchingReln_ * NMatrix2(-1, 1, -1, 0);
            else
                matchingReln_ = NMatrix2(0, -1, 1, -1) * matchingReln_;

            // If we reordered the SFSs in a displeasing way, switch
            // them and change the matrix accordingly.
            if (*sfs_[1] < *sfs_[0]) {
                NSFSpace* tmp = sfs_[0];
                sfs_[0] = sfs_[1];
                sfs_[1] = tmp;

                matchingReln_.invert();
            }
        }

    // Consider replacing each space with its reflection.
    bool ref0, ref1;
    reduceReflect(matchingReln_, sfs_[0]->getFibreCount(),
        sfs_[1]->getFibreCount(), ref0, ref1);

    if (ref0)
        sfs_[0]->complementAllFibres();
    if (ref1)
        sfs_[1]->complementAllFibres();

    // TODO: More reductions!
    // We can probably exploit twist identities such as (1,2) = (1,0) in
    // certain non-orientable cases.
    // TODO: Also consider swapping spaces.
}

void NNGSFSPair::reduceReflect(NMatrix2& reln, unsigned long fibres0,
        unsigned long fibres1, bool& ref0, bool& ref1) {
    // Consider replacing each space with its reflection.
    // Note that we have b=0 for both SFSs at this stage.
    NMatrix2 r0 = reln * NMatrix2(1, 0, fibres0, -1);
    NMatrix2 r1 = NMatrix2(1, 0, fibres1, -1) * reln;
    NMatrix2 r01 = NMatrix2(1, 0, fibres1, -1) * reln *
        NMatrix2(1, 0, fibres0, -1);

    reduceSign(reln);
    reduceSign(r0);
    reduceSign(r1);
    reduceSign(r01);

    if (simpler(r0, reln) && simpler(r0, r1) && simpler(r0, r01)) {
        reln = r0;
        ref0 = true;
        ref1 = false;
    } else if (simpler(r1, reln) && simpler(r1, r01)) {
        reln = r1;
        ref0 = false;
        ref1 = true;
    } else if (simpler(r01, reln)) {
        reln = r01;
        ref0 = true;
        ref1 = true;
    } else {
        ref0 = false;
        ref1 = false;
    }
}

void NNGSFSPair::reduceSign(NMatrix2& reln) {
    // Make the first non-zero entry positive.
    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (reln[i][j] > 0)
                return;

            if (reln[i][j] < 0) {
                // Negate everything (180 degree rotation along the join)
                // and return.

                for (i = 0; i < 2; i++)
                    for (j = 0; j < 2; j++)
                        reln[i][j] = - reln[i][j];
                return;
            }
        }

    // The matrix is entirely zero (which, incidentally, should never
    // happen).  Do nothing.
}

bool NNGSFSPair::simpler(const NMatrix2& m1, const NMatrix2& m2) {
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

    // Go lexicograhpic.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (m1[i][j] < m2[i][j])
                return true;
            else if (m1[i][j] > m2[i][j])
                return false;

    // They're the same.
    return false;
}

} // namespace regina

