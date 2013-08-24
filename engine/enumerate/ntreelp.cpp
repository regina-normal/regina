
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2013, Ben Burton                                   *
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

/* end stub */

#include "enumerate/ntreelp.tcc"

namespace regina {

// Instantiate templates:
template class LPInitialTableaux<LPConstraintNone>;
template class LPInitialTableaux<LPConstraintEuler>;
template class LPInitialTableaux<LPConstraintNonSpun>;
template class LPData<LPConstraintNone>;
template class LPData<LPConstraintEuler>;
template class LPData<LPConstraintNonSpun>;

void LPMatrix::combRow(const NInteger& destCoeff, unsigned dest,
        const NInteger& srcCoeff, unsigned src, const NInteger& div) {
    NInteger* ps = dat_ + src * cols_;
    NInteger* pd = dat_ + dest * cols_;
    if (div > 1)
        for (unsigned i = 0; i < cols_; ++i) {
            *pd *= destCoeff;
            *pd -= (srcCoeff * *ps++);
            (*pd++).divByExact(div);
        }
    else
        for (unsigned i = 0; i < cols_; ++i) {
            *pd *= destCoeff;
            *pd++ -= (srcCoeff * *ps++);
        }
}

NInteger LPMatrix::combRowAndNorm(const NInteger& destCoeff,
        unsigned dest, const NInteger& srcCoeff, unsigned src) {
    NInteger gcdRow; // Initialised to zero.
    NInteger* ps = dat_ + src * cols_;
    NInteger* pd = dat_ + dest * cols_;
    unsigned i;
    for (i = 0; i < cols_; ++i, pd++, ps++) {
        *pd *= destCoeff;
        *pd -= (srcCoeff * *ps);
        if (gcdRow != 1)
            gcdRow = gcdRow.gcd(*pd); // gcd() guarantees to be >= 0.
    }
    if (gcdRow > 1) {
        pd = dat_ + dest * cols_;
        for (i = 0; i < cols_; ++i)
            (*pd++).divByExact(gcdRow);
    }
    return gcdRow;
}

void LPMatrix::dump(std::ostream& out) const {
    out << "---------------------------------" << std::endl;
    unsigned r, c;
    for (r = 0; r < rows_; ++r) {
        for (c = 0; c < cols_; ++c)
            out << entry(r, c) << ' ';
        out << std::endl;
    }
    out << "---------------------------------" << std::endl;
}

} // namespace regina
