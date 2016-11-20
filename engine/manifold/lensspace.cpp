
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/abeliangroup.h"
#include "manifold/lensspace.h"
#include "maths/numbertheory.h"
#include "triangulation/dim3.h"

namespace regina {

void LensSpace::reduce() {
    if (p_ == 0) {
        q_ = 1;
        return;
    } else if (p_ == 1) {
        q_ = 0;
        return;
    }

    // p > 1 and gcd(p,q) = 1.
    
    // Reduce q to +/-q.
    q_ = q_ % p_;
    if (2 * q_ > p_)
        q_ = p_ - q_;

    unsigned long inv = modularInverse(p_, q_);
    if (2 * inv > p_)
        inv = p_ - inv;
    if (inv < q_)
        q_ = inv;
}

Triangulation<3>* LensSpace::construct() const {
    Triangulation<3>* ans = new Triangulation<3>();
    ans->insertLayeredLensSpace(p_, q_);
    return ans;
}

AbelianGroup* LensSpace::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    if (p_ == 0)
        ans->addRank();
    else if (p_ > 1)
        ans->addTorsionElement(p_);
    return ans;
}

std::ostream& LensSpace::writeName(std::ostream& out) const {
    if (p_ == 0)
        out << "S2 x S1";
    else if (p_ == 1)
        out << "S3";
    else if (p_ == 2 && q_ == 1)
        out << "RP3";
    else
        out << "L(" << p_ << ',' << q_ << ')';
    return out;
}

std::ostream& LensSpace::writeTeXName(std::ostream& out) const {
    if (p_ == 0)
        out << "S^2 \\times S^1";
    else if (p_ == 1)
        out << "S^3";
    else if (p_ == 2 && q_ == 1)
        out << "\\mathbb{R}P^3";
    else
        out << "L(" << p_ << ',' << q_ << ')';
    return out;
}

} // namespace regina

