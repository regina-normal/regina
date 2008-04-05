
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "maths/nray.h"

namespace regina {

void NRay::scaleDown() {
    NLargeInteger gcd; // Initialised to 0.
    unsigned i;
    for (i = 0; i < vectorSize; i++) {
        if (elements[i].isInfinite() || elements[i] == zero)
            continue;
        gcd = gcd.gcd(elements[i]);
        if (gcd < 0)
            gcd.negate();
        if (gcd == one)
            return;
    }
    if (gcd == zero)
        return;
    for (i = 0; i < vectorSize; i++)
        if ((! elements[i].isInfinite()) && elements[i] != zero)
            elements[i].divByExact(gcd);
}

NRay* intersect(const NRay& first, const NRay& second,
        const NVector<NLargeInteger>& hyperplane) {
    // Return (hyperplane * first) second - (hyperplane * second) first.
    // Negate this if (hyperplane * first) is negative.

    NLargeInteger firstCoeff(hyperplane * first);
    NRay* ans = dynamic_cast<NRay*>(second.clone());
    (*ans) *= firstCoeff;
    (*ans).subtractCopies(first, hyperplane * second);
    (*ans).scaleDown();

    if (firstCoeff < NLargeInteger::zero)
        (*ans).negate();

    return ans;
}

} // namespace regina

