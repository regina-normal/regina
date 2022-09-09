
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#ifndef __REGINA_DATA_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_DATA_IMPL_H
#endif

#include "utilities/exception.h"

namespace regina {

template <typename... Args>
Link Link::fromData(std::initializer_list<int> crossingSigns,
        std::initializer_list<Args>... components) {
    Link ans;
    for (auto sign : crossingSigns) {
        if (sign == 1 || sign == -1)
            ans.crossings_.push_back(new Crossing(sign));
        else
            throw InvalidArgument("fromData(): crossing sign not +/-1");
    }
    ans.addComponents(2 * crossingSigns.size(), components...);
    return ans;
}

inline void Link::addComponents(size_t strandsRemaining) {
    if (strandsRemaining != 0)
        throw InvalidArgument("fromData(): too few strands");
}

template <typename... Args>
void Link::addComponents(size_t strandsRemaining,
        std::initializer_list<int> component,
        std::initializer_list<Args>... otherComponents) {
    if (component.size() == 0) {
        // Support an empty component via { }, though I suspect this is
        // impossible to use because the C++ compiler cannot deduce the type.
        components_.emplace_back();
    } else if (component.size() == 1 && *component.begin() == 0) {
        // Support an empty component via { 0 }.
        // Here we increment strandsRemaining, to account for the extra
        // integer (0) in our list that does not belong to any crossing.
        components_.emplace_back();
        ++strandsRemaining;
    } else {
        size_t n = crossings_.size();

        if constexpr (sizeof(int) <= sizeof(size_t)) {
            if (n > static_cast<size_t>(INT_MAX))
                throw InvalidArgument("fromData(): too many crossings "
                    "to hold using native C++ ints");
        }
        const int maxCrossing = static_cast<int>(n);

        int tmpCross;
        int tmpStrand;

        auto it = component.begin();
        if (*it == 0 || *it > maxCrossing || *it < -maxCrossing)
            throw InvalidArgument("fromData(): crossing out of range");
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

            if (*it == 0 || *it > maxCrossing || *it < -maxCrossing)
                throw InvalidArgument("fromData(): crossing out of range");
            if (*it > 0) {
                tmpCross = *it;
                tmpStrand = 1;
            } else {
                tmpCross = - *it;
                tmpStrand = 0;
            }

            cr = crossings_[tmpCross - 1];
            curr = cr->strand(tmpStrand);

            if (prev.crossing()->next_[prev.strand()])
                throw InvalidArgument("fromData(): multiple passes "
                    "out of same strand of crossing");
            prev.crossing()->next_[prev.strand()] = curr;

            if (curr.crossing()->prev_[curr.strand()])
                throw InvalidArgument("fromData(): multiple passes "
                    "into same strand of crossing");
            curr.crossing()->prev_[curr.strand()] = prev;
        }

        prev = curr;
        curr = components_.back();

        if (prev.crossing()->next_[prev.strand()])
            throw InvalidArgument("fromData(): multiple passes "
                "out of same strand of crossing");
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()])
            throw InvalidArgument("fromData(): multiple passes "
                "into same strand of crossing");
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    addComponents(strandsRemaining - component.size(), otherComponents...);
}

template <typename SignIterator, typename ComponentIterator>
Link Link::fromData(SignIterator beginSigns, SignIterator endSigns,
        ComponentIterator beginComponents, ComponentIterator endComponents) {
    using InputInt = std::remove_cv_t<std::remove_reference_t<
        decltype(*beginComponents->begin())>>;
    static_assert(std::is_integral_v<InputInt> &&
        ! std::is_unsigned_v<InputInt>, "fromData(): the iterator type "
        "needs to dereference to give a native signed C++ integer type.");

    Link ans;

    for (auto sit = beginSigns; sit != endSigns; ++sit) {
        if (*sit == 1 || *sit == -1)
            ans.crossings_.push_back(new Crossing(*sit));
        else
            throw InvalidArgument("fromData(): crossing sign not +/-1");
    }

    size_t strandsFound = 0;
    size_t n = ans.crossings_.size();

    if constexpr (sizeof(InputInt) <= sizeof(size_t)) {
        if (n > static_cast<size_t>(std::numeric_limits<InputInt>::max()))
            throw InvalidArgument("fromData(): too many crossings for "
                "the given integer type");
    }
    const auto maxCrossing = static_cast<InputInt>(n);

    for (auto cit = beginComponents; cit != endComponents; ++cit) {
        if (cit->size() == 0) {
            // Support an empty component via { }.
            ans.components_.emplace_back();
        } else if (cit->size() == 1 && *cit->begin() == 0) {
            // Support an empty component via { 0 }.
            ans.components_.emplace_back();
        } else {
            bool first = true;
            StrandRef curr, prev;
            for (auto c : *cit) {
                if (c == 0 || c > maxCrossing || c < -maxCrossing)
                    throw InvalidArgument("fromData(): crossing out of range");
                if (c > 0)
                    curr = ans.crossings_[c - 1]->upper();
                else
                    curr = ans.crossings_[-(c + 1)]->lower();

                if (first) {
                    ans.components_.push_back(curr);
                    first = false;
                } else {
                    if (prev.crossing()->next_[prev.strand()])
                        throw InvalidArgument("fromData(): multiple passes "
                            "out of same strand of crossing");
                    prev.crossing()->next_[prev.strand()] = curr;

                    if (curr.crossing()->prev_[curr.strand()])
                        throw InvalidArgument("fromData(): multiple passes "
                            "into same strand of crossing");
                    curr.crossing()->prev_[curr.strand()] = prev;
                }

                prev = curr;
            }

            curr = ans.components_.back();

            if (prev.crossing()->next_[prev.strand()])
                throw InvalidArgument("fromData(): multiple passes "
                    "out of same strand of crossing");
            prev.crossing()->next_[prev.strand()] = curr;

            if (curr.crossing()->prev_[curr.strand()])
                throw InvalidArgument("fromData(): multiple passes "
                    "into same strand of crossing");
            curr.crossing()->prev_[curr.strand()] = prev;

            strandsFound += cit->size();
        }
    }

    if (strandsFound != 2 * ans.crossings_.size())
        throw InvalidArgument("fromData(): incorrect number of strands");

    return ans;
}

} // namespace regina

#endif

