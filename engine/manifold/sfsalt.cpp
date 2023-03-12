
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "manifold/sfs.h"
#include "manifold/sfsalt.h"

namespace regina {

// NOLINTNEXTLINE(modernize-pass-by-value)
SFSAlt::SFSAlt(const SFSpace& original) :
        alt_(original), conversion_(1, 0, 0, 1), reflected_(false) {
    /**
     * Note that, whenever we add a (1,1) twist, we compensate by setting
     * row 2 -> row 2 + row 1 in our conversion matrix.
     */

    // Reduce the original to give obstruction constant zero.
    alt_.reduce(false);

    long b = alt_.obstruction();
    if (b) {
        alt_.insertFibre(1, -b);
        conversion_[1][0] = -b;
    }

    /**
     * If the space is M/n2, we can replace it with D:(2,1)(2,-1)
     * with fibre and orbifold curves switched.  To preserve the
     * determinant of the matching matrix we will actually use a
     * [0,1,-1,0] switch instead of a [0,1,1,0] switch.
     *
     * In fact we will use D:(2,1)(2,1) instead, which means:
     *
     * M_basis = [  0 1 ] [  1 0 ] D_basis = [ -1 1 ] D_basis;
     *           [ -1 0 ] [ -1 1 ]           [ -1 0 ]
     *
     * D_basis = [ 1 0 ] [  0 -1 ] M_basis = [ 0 -1 ] M_basis.
     *           [ 1 1 ] [  1  0 ]           [ 1 -1 ]
     */
    if (alt_.baseClass() == SFSpace::bn2 &&
            alt_.baseGenus() == 1 &&
            (! alt_.baseOrientable()) &&
            alt_.punctures(false) == 1 &&
            alt_.punctures(true) == 0 &&
            alt_.reflectors() == 0 &&
            alt_.fibreCount() == 0 &&
            alt_.obstruction() == 0) {
        alt_ = SFSpace(SFSpace::bo1, 0 /* genus */,
            1 /* punctures */, 0 /* twisted */,
            0 /* reflectors */, 0 /* twisted */);
        alt_.insertFibre(2, 1);
        alt_.insertFibre(2, 1);

        conversion_ = Matrix2(0, -1, 1, -1) * conversion_;
    }
}

SFSAlt::SFSAlt(const SFSAlt& base, bool reflect, bool negate) :
        alt_(base.alt_),
        conversion_(base.conversion_),
        reflected_(base.reflected_) {
    long extraTwist = 0;

    if (negate) {
        // Since the number of exceptional fibres is odd, and since
        // (1,2) == (1,0) due to the fact that fibres can be negated,
        // it following that adding (1,1) for every exceptional fibre is
        // the same as adding just a single (1,1).
        alt_.insertFibre(1, 1);
        extraTwist = 1;
    }
    if (reflect) {
        alt_.reflect();
        reflected_ = ! reflected_;
        extraTwist = -extraTwist;
    }

    alt_.reduce(false);

    long b = alt_.obstruction();
    alt_.insertFibre(1, -b);
    conversion_ = Matrix2(1, 0, -b + extraTwist, reflect ? -1 : 1) *
        conversion_;
}

} // namespace regina

