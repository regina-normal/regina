
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

/*! \file link/gauss-tangle-impl.h
 *  \brief Contains implementation details for parsing oriented Gauss codes
 *  of tangles.
 *
 *  This file is automatically included from tangle.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __REGINA_GAUSS_TANGLE_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_GAUSS_TANGLE_IMPL_H
#endif

#include <algorithm>

namespace regina {

template <typename Iterator>
Tangle Tangle::fromOrientedGauss(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n < 2)
        throw InvalidArgument("fromOrientedGauss(): too few terms");
    if (n % 2)
        throw InvalidArgument("fromOrientedGauss(): odd number of terms");
    n = (n / 2) - 1;

    char type = extractChar(*begin++);
    if (! (type == '|' || type == '-' || type == 'x'))
        throw InvalidArgument("fromOrientedGauss(): invalid tangle type");

    Tangle ans;
    ans.type_ = type;

    size_t i;
    for (i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing);

    StrandRef prev, curr;
    Iterator it = begin;

    int string = 0;

    size_t tmpCross;
    int tmpStrand, tmpSign;
    Crossing* cr;

    for ( ; it != end; ++it) {
        if (! Link::parseOrientedGaussTerm(
                *it, n, tmpCross, tmpStrand, tmpSign)) {
            if (extractChar(*it) == '_') {
                if (string == 1)
                    throw InvalidArgument("fromOrientedGauss(): more than one "
                        "underscore present");
                if (curr) {
                    if (curr.crossing()->next_[curr.strand()])
                        throw InvalidArgument("fromOrientedGauss(): "
                            "multiple passes out of the same strand");
                    ans.end_[0][1] = curr;
                    curr = StrandRef();
                }
                string = 1;
                continue;
            } else {
                throw InvalidArgument("fromOrientedGauss(): "
                    "could not parse term");
            }
        }

        prev = curr;

        cr = ans.crossings_[tmpCross - 1];

        if (cr->sign_ == 0)
            cr->sign_ = tmpSign;
        else if (cr->sign_ != tmpSign)
            throw InvalidArgument("fromOrientedGauss(): inconsistent signs "
                "for crossing");

        curr = cr->strand(tmpStrand);

        if (curr.crossing()->prev_[curr.strand()] ||
                (string == 1 && curr == ans.end_[0][0]))
            throw InvalidArgument("fromOrientedGauss(): multiple passes into "
                "the same strand");

        if (! prev) {
            ans.end_[string][0] = curr;
        } else {
            if (prev.crossing()->next_[prev.strand()])
                throw InvalidArgument("fromOrientedGauss(): multiple "
                    "passes out of the same strand");
            prev.crossing()->next_[prev.strand()] = curr;
            curr.crossing()->prev_[curr.strand()] = prev;
        }
    }

    if (string != 1)
        throw InvalidArgument("fromOrientedGauss(): missing underscore");

    if (curr) {
        if (curr.crossing()->next_[curr.strand()] || curr == ans.end_[0][1])
            throw InvalidArgument("fromOrientedGauss(): multiple "
                "passes out of the same strand");
        ans.end_[1][1] = curr;
    }

    // All done!
    return ans;
}

} // namespace regina

#endif

