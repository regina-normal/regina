
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "link/link.h"

namespace regina {

Laurent<Integer> Link::affineIndex() const {
    if (components_.size() != 1)
        throw FailedPrecondition("Regina can only compute affine index "
            "polynomials for links with exactly one component");

    if (size() == 0)
        return {}; // all classical knot have affine index polynomial zero.

    // Build a Cheng colouring.  This assigns an integer to each strand.
    // The starting value does not affect the affine index polynomial, and so
    // we choose a starting value of zero.

    FixedArray<long> label(2 * size());

    StrandRef s = components_.front();
    long colour = 0;
    do {
        if (s.crossing()->sign() > 0) {
            if (s.strand() == 0)
                ++colour;
            else
                --colour;
        } else {
            if (s.strand() == 0)
                --colour;
            else
                ++colour;
        }
        label[s.id()] = colour;
        ++s;
    } while (s != components_.front());

    Laurent<Integer> ans;
    long writhe = 0;
    for (auto c : crossings_) {
        long exp = label[c->upper().id()] - label[c->lower().id()];
        if (c->sign() > 0) {
            ++exp;
            ++writhe;
            ans.set(exp, ans[exp] + 1);
        } else {
            --exp;
            --writhe;
            ans.set(exp, ans[exp] - 1);
        }
    }

    ans.set(0, ans[0] - writhe);

    return ans;
}

} // namespace regina

