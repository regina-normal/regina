
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "surfaces/nconeray.h"
#include "maths/nvectorunit.h"
#include "maths/nvectormatrix.h"
#include "maths/nmatrixint.h"

namespace regina {

void NConeRay::scaleDown() {
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

NConeRay* intersectLine(const NConeRay& pos, const NConeRay& neg,
        const NConeRay& hyperplane) {
    NConeRay* ans = (NConeRay*)neg.clone();

    (*ans) *= (hyperplane * pos);
    (*ans).subtractCopies(pos, hyperplane * neg);
    (*ans).scaleDown();

    return ans;
}

} // namespace regina

