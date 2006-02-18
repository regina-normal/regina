
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

    // Note that twisting will never change the (0,1) entry of the
    // matrix.  It therefore seems useful to fix this first.  If the
    // matrix has determinant +1 then we have some control over this.
    long det = matchingReln_.determinant();
    if (det == 1 && matchingReln_[0][1] < 0)
        matchingReln_.invert();

    // Massage the obstruction constant for the SFS.
    long b = sfs_->getObstruction();
    if (b != 0) {
        sfs_->insertFibre(1, -b);
        matchingReln_[0][0] += b * matchingReln_[0][1];
        matchingReln_[1][0] += b * matchingReln_[1][1];
    }

    // Use (1,1) / (1,-1) pairs to make the top-left element of the
    // matrix zero, if we can.
    if (matchingReln_[0][1] != 0) {
        long nOps = matchingReln_[0][0] / matchingReln_[0][1];
        if (nOps > 0) {
            for (long i = 0; i < nOps; i++) {
                matchingReln_[0][0] -= matchingReln_[0][1];
                matchingReln_[1][0] -= matchingReln_[1][1];
                matchingReln_[1][0] -= matchingReln_[0][0];
                matchingReln_[1][1] -= matchingReln_[0][1];
            }
        } else if (nOps < 0) {
            for (long i = 0; i > nOps; i--) {
                matchingReln_[0][0] += matchingReln_[0][1];
                matchingReln_[1][0] += matchingReln_[1][1];
                matchingReln_[1][0] += matchingReln_[0][0];
                matchingReln_[1][1] += matchingReln_[0][1];
            }
        }
    }
}

} // namespace regina

