
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/nlensspace.h"
#include "maths/numbertheory.h"

namespace regina {

void NLensSpace::reduce() {
    if (p == 0) {
        q = 1;
        return;
    } else if (p == 1) {
        q = 0;
        return;
    }

    // p > 1 and gcd(p,q) = 1.
    
    // Reduce q to +/-q.
    q = q % p;
    if (2 * q > p)
        q = p - q;

    unsigned long inv = modularInverse(p, q);
    if (2 * inv > p)
        inv = p - inv;
    if (inv < q)
        q = inv;
}

std::string NLensSpace::getCommonName() const {
    if (p == 0)
        return "S2xS1";
    else if (p == 1)
        return "S3";
    else if (p == 2 && q == 1)
        return "RP3";
    else
        return toString();
}

} // namespace regina

