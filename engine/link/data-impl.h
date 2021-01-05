
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file link/data-impl.h
 *  \brief Contains implementation details for constructing knots and
 *  links from C++ initialiser lists.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __DATA_IMPL_H
#ifndef __DOXYGEN
#define __DATA_IMPL_H
#endif

namespace regina {

template <typename... Args>
Link* Link::fromData(std::initializer_list<int> crossingSigns,
        std::initializer_list<Args>... components) {
    Link* ans = new Link();

    for (auto sign : crossingSigns) {
        if (sign == 1 || sign == -1)
            ans->crossings_.push_back(new Crossing(sign));
        else {
            std::cerr << "fromData(): crossing sign not +/-1" << std::endl;
            delete ans;
            return 0;
        }
    }

    if (! ans->addComponents(2 * crossingSigns.size(), components...)) {
        delete ans;
        return 0;
    }

    return ans;
}

inline bool Link::addComponents(size_t strandsRemaining) {
    if (strandsRemaining != 0) {
        std::cerr << "fromData(): too few strands" << std::endl;
        return false;
    } else
        return true;
}

template <typename... Args>
bool Link::addComponents(size_t strandsRemaining,
        std::initializer_list<int> component,
        std::initializer_list<Args>... otherComponents) {
    if (component.size() == 0) {
        // Support an empty component via { }, though I suspect this is
        // impossible to use because the C++ compiler cannot deduce the type.
        components_.push_back(StrandRef());
    } else if (component.size() == 1 && *component.begin() == 0) {
        // Support an empty component via { 0 }.
        // Here we increment strandsRemaining, to account for the extra
        // integer (0) in our list that does not belong to any crossing.
        components_.push_back(StrandRef());
        ++strandsRemaining;
    } else {
        long n = crossings_.size();

        int tmpCross;
        int tmpStrand;

        auto it = component.begin();
        if (*it == 0 || *it > n || *it < -n) {
            std::cerr << "fromData(): crossing " << *it
                << " out of range" << std::endl;
            return false;
        }
        if (*it > 0) {
            tmpCross = *it;
            tmpStrand = 1;
        } else {
            tmpCross = - *it;
            tmpStrand = 0;
        }

        StrandRef curr, prev;
        Crossing* cr = crossings_[tmpCross - 1];
        curr = cr->strand(tmpStrand);
        components_.push_back(curr);

        for (++it; it != component.end(); ++it) {
            prev = curr;

            if (*it == 0 || *it > n || *it < -n) {
                std::cerr << "fromData(): crossing " << *it
                    << " out of range" << std::endl;
                return false;
            }
            if (*it > 0) {
                tmpCross = *it;
                tmpStrand = 1;
            } else {
                tmpCross = - *it;
                tmpStrand = 0;
            }

            cr = crossings_[tmpCross - 1];
            curr = cr->strand(tmpStrand);

            if (prev.crossing()->next_[prev.strand()]) {
                std::cerr << "fromData(): multiple passes out of "
                    << (prev.strand() == 0 ? "lower" : "upper")
                    << " strand of crossing " << (prev.crossing()->index() + 1)
                    << std::endl;
                return false;
            }
            prev.crossing()->next_[prev.strand()] = curr;

            if (curr.crossing()->prev_[curr.strand()]) {
                std::cerr << "fromData(): multiple passes into "
                    << (curr.strand() == 0 ? "lower" : "upper")
                    << " strand of crossing " << (curr.crossing()->index() + 1)
                    << std::endl;
                return false;
            }
            curr.crossing()->prev_[curr.strand()] = prev;
        }

        prev = curr;
        curr = components_.back();

        if (prev.crossing()->next_[prev.strand()]) {
            std::cerr << "fromData(): multiple passes out of "
                << (prev.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (prev.crossing()->index() + 1)
                << std::endl;
            return false;
        }
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()]) {
            std::cerr << "fromData(): multiple passes into "
                << (curr.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (curr.crossing()->index() + 1)
                << std::endl;
            return false;
        }
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    return addComponents(strandsRemaining - component.size(),
        otherComponents...);
}

} // namespace regina

#endif

