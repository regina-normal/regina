
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "maths/laurent.h"

namespace regina {

const char* Link::jonesVar = "\u221At"; // \u221A = square root

Laurent<Integer>* Link::bracketNaive() const {
    /**
     * \ /         \ /            \_/
     *  /   ->   A | |   +   A^-1  _
     * / \         / \            / \
     *
     * O^k  ->  (-A^2 - A^-2)^(k-1)
     */

    if (components_.size() == 0)
        return new Laurent<Integer>();

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    // How many zero-crossing components do we start with?
    size_t initLoops = 0;
    for (StrandRef s : components_)
        if (! s)
            ++initLoops;

    size_t n = crossings_.size();
    if (n >= sizeof(long) * 8)
        return nullptr;

    // In count[i-1], the coefficient of A^k reflects the number of
    // resolutions with i loops and multiplier A^k.
    // We will always have 1 <= i <= #components + #crossings.
    Laurent<Integer>* count = new Laurent<Integer>[n + components_.size()];

    size_t maxLoops = 0;

    unsigned long mask;
    size_t loops;
    size_t pos;
    int end;
    StrandRef s;
    int dirInit, dir;
    long shift;
    bool* found = new bool[2 * n];
    for (mask = 0; mask != (1 << n); ++mask) {
        loops = initLoops;

        // found[0..n-1] : seen exiting the crossing on the upper strand
        // found[n..2n-1] : seen entering the crossing on the upper strand
        std::fill(found, found + 2 * n, false);

        //std::cerr << "Mask: " << mask << std::endl;
        for (pos = 0; pos < n; ++pos)
            for (dirInit = 0; dirInit < 2; ++dirInit) {
                // dirInit: 1 = with arrows, 0 = against arrows.
                // This refers to the direction along the strand as you
                // approach the crossing (before you jump to the other strand).
                if (! found[pos + (dirInit ? n : 0)]) {
                    //std::cerr << "LOOP\n";
                    ++loops;

                    s = crossings_[pos]->upper();
                    dir = dirInit;

                    do {
                        //std::cerr << "At: " << s <<
                        //    (dir == 1 ? " ->" : " <-") << std::endl;

                        if (    ((mask & (1 << s.crossing()->index())) &&
                                    s.crossing()->sign() > 0) ||
                                ((mask & (1 << s.crossing()->index())) == 0 &&
                                    s.crossing()->sign() < 0)) {
                            if (dir == 1) {
                                found[s.crossing()->index() +
                                    (s.strand() ? n : 0)] = true;
                                s = s.crossing()->next(s.strand() ^ 1);
                            } else {
                                found[s.crossing()->index() +
                                    (s.strand() ? 0 : n)] = true;
                                s = s.crossing()->prev(s.strand() ^ 1);
                            }
                        } else {
                            if (dir == 1) {
                                found[s.crossing()->index() + n] = true;
                                s = s.crossing()->prev(s.strand() ^ 1);
                            } else {
                                found[s.crossing()->index()] = true;
                                s = s.crossing()->next(s.strand() ^ 1);
                            }
                            dir ^= 1;
                        }
                    } while (! (dir == dirInit &&
                        s.crossing()->index() == pos && s.strand() == 1));
                }
            }

        shift = 0;
        for (pos = 0; pos < n; ++pos)
            if (mask & (1 << pos))
                ++shift;
            else
                --shift;

        if (loops > maxLoops)
            maxLoops = loops;

        --loops;
        if (shift > count[loops].maxExp() || shift < count[loops].minExp())
            count[loops].set(shift, 1);
        else
            count[loops].set(shift, count[loops][shift] + 1);
    }
    delete[] found;

    Laurent<Integer>* ans = new Laurent<Integer>;

    Laurent<Integer> loopPoly;
    loopPoly.set(0, -1);
    loopPoly.set(4, -1);
    loopPoly.shift(-2);

    Laurent<Integer> loopPow(0); // Initialises to x^0 == 1.
    for (loops = 0; loops < maxLoops; ++loops) {
        // std::cerr << "count[" << loops << "] = " << count[loops] << std::endl;
        if (! count[loops].isZero()) {
            count[loops] *= loopPow;
            (*ans) += count[loops];
        }

        loopPow *= loopPoly;
    }

    delete[] count;
    return ans;
}

Laurent<Integer>* Link::bracketTreewidth() const {
    // TODO: implement
    return bracketNaive();
}

const Laurent<Integer>& Link::bracket(Algorithm alg) const {
    if (bracket_.known())
        return *bracket_.value();

    switch (alg) {
        case ALG_TREEWIDTH:
            bracket_ = bracketTreewidth();
            break;
        default:
            bracket_ = bracketNaive();
            break;
    }
    return *bracket_.value();
}

const Laurent<Integer>& Link::jones(Algorithm alg) const {
    if (jones_.known())
        return *jones_.value();

    // (-A^3)^(-w) * bracket, then multiply all exponents by -1/4.
    Laurent<Integer>* ans = new Laurent<Integer>(bracket(alg));
    if (ans == 0)
        return *(jones_ = ans);

    long w = writhe();
    ans->shift(-3 * w);
    if (w % 2)
        ans->negate();

    // We only scale exponents by -1/2, since we are returning a Laurent
    // polynomial in sqrt(t).
    ans->scaleDown(-2);
    return *(jones_ = ans);
}

} // namespace regina

