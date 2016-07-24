
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

/*! \file link/gauss-impl.h
 *  \brief Contains implementation details for parsing Gauss codes of knots.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __GAUSS_IMPL_H
#ifndef __DOXYGEN
#define __GAUSS_IMPL_H
#endif

namespace regina {

template <typename Iterator>
Link* Link::fromOrientedGauss(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n % 2) {
        std::cerr << "fromOrientedGauss(): odd number of terms" << std::endl;
        return 0;
    }
    n = n / 2;

    if (n == 0)
        return new Link(1);

    Link* ans = new Link;

    size_t i;
    for (i = 0; i < n; ++i)
        ans->crossings_.push_back(new Crossing);

    StrandRef prev, curr;
    Iterator it = begin;

    size_t tmpCross;
    int tmpStrand, tmpSign;
    if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
        std::cerr << "fromOrientedGauss(): could not parse " << *it
            << std::endl;
        delete ans;
        return 0;
    }

    Crossing* cr = ans->crossings_[tmpCross - 1];
    cr->sign_ = tmpSign;
    curr = cr->strand(tmpStrand);
    ans->components_.push_back(curr);

    for (++it; it != end; ++it) {
        prev = curr;

        if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
            std::cerr << "fromOrientedGauss(): could not parse " << *it
                << std::endl;
            delete ans;
            return 0;
        }

        cr = ans->crossings_[tmpCross - 1];
        if (cr->sign_ == 0)
            cr->sign_ = tmpSign;
        else if (cr->sign_ != tmpSign) {
            std::cerr << "fromOrientedGauss(): inconsistent signs "
                "for crossing " << tmpCross << std::endl;
            delete ans;
            return 0;
        }

        curr = cr->strand(tmpStrand);

        if (prev.crossing()->next_[prev.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes out of "
                << (prev.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (prev.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return 0;
        }
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes into "
                << (curr.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (curr.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return 0;
        }
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    prev = curr;
    curr = ans->components_.back();

    if (prev.crossing()->next_[prev.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes out of "
            << (prev.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (prev.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return 0;
    }
    prev.crossing()->next_[prev.strand()] = curr;

    if (curr.crossing()->prev_[curr.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes into "
            << (curr.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (curr.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return 0;
    }
    curr.crossing()->prev_[curr.strand()] = prev;

    // All done!
    return ans;
}

} // namespace regina

#endif

