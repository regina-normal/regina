
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
#include "manifold/nlensspace.h"
#include "maths/numbertheory.h"
#include "triangulation/ntriangulation.h"

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

NTriangulation* NLensSpace::construct() const {
    NTriangulation* ans = new NTriangulation();
    ans->insertLayeredLensSpace(p, q);
    return ans;
}

NAbelianGroup* NLensSpace::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    if (p == 0)
        ans->addRank();
    else if (p > 1)
        ans->addTorsionElement(p);
    return ans;
}

std::ostream& NLensSpace::writeName(std::ostream& out) const {
    if (p == 0)
        out << "S2 x S1";
    else if (p == 1)
        out << "S3";
    else if (p == 2 && q == 1)
        out << "RP3";
    else
        out << "L(" << p << ',' << q << ')';
    return out;
}

std::ostream& NLensSpace::writeTeXName(std::ostream& out) const {
    if (p == 0)
        out << "S^2 \\times S^1";
    else if (p == 1)
        out << "S^3";
    else if (p == 2 && q == 1)
        out << "\\mathbb{R}P^3";
    else
        out << "L(" << p << ',' << q << ')';
    return out;
}

} // namespace regina

