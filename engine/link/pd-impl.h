
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

/*! \file link/pd-impl.h
 *  \brief Contains implementation details for parsing planar diagram codes
 *  of links.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __REGINA_PD_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_PD_IMPL_H
#endif

#include <algorithm>

namespace regina {

template <typename Iterator>
Link Link::fromPD(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n == 0) {
        // PD codes do not handle zero-crossing unknots.
        // Just return nothing at all.
        return Link();
    }

    // Represents (crossing index, position in 4-tuple):
    using PDPos = std::pair<size_t, int>;

    // The two occurrences of each strand in the PD code:
    using PDOccurrence = std::pair<PDPos, PDPos>;

    // The zero-based strand numbers that will begin each component:
    std::vector<size_t> components;

    // Identify the two crossings that each strand meets.
    // A position of -1 in the 4-tuple means "not yet seen".
    auto* occ = new PDOccurrence[2 * n];
    for (size_t i = 0; i < 2 * n; ++i)
        occ[i].first.second = occ[i].second.second = -1;

    size_t index;
    Iterator it;
    for (it = begin, index = 0; it != end; ++it, ++index) {
        for (int i = 0; i < 4; ++i) {
            auto s = (*it)[i];
            if (s <= 0 || s > 2 * n) {
                delete[] occ;
                throw InvalidArgument("fromPD(): strand out of range");
            }
            PDOccurrence* o = occ + (s - 1);
            if (o->first.second < 0) {
                o->first.first = index;
                o->first.second = i;
            } else if (o->second.second < 0) {
                o->second.first = index;
                o->second.second = i;
            } else {
                delete[] occ;
                throw InvalidArgument(
                    "fromPD(): strand appears more than twice");
            }
        }
    }

    // Identify the direction of each strand.
    // 0 means unknown;
    // 1 means first occurrence -> second occurrence;
    // -1 means second occurrence -> first occurrence.

    int* dir = new int[2 * n];
    std::fill(dir, dir + 2 * n, 0);

    // First walk through the crossings and work out what we can.
    for (it = begin, index = 0; it != end; ++it, ++index) {
        // Examine the incoming lower strand (which is the only one
        // whose direction is predetermined):
        auto start = (*it)[0] - 1;
        if (dir[start]) {
            // We have already processed this strand (and the entire
            // component that contains it).
            continue;
        }

        // We know that start is the incoming lower strand.
        // Follow this component around and identify the directions of
        // all strands on the component.
        // As we do this, we will also collect the minimum strand label on the
        // component (which will become its starting point).
        PDPos pos { index, 0 };
        dir[start] = (occ[start].first == pos ? -1 : 1);
        size_t min = start;

        auto s = start;
        while (true) {
            // Move s forward to the next strand on this component.
            if (dir[s] > 0)
                pos = occ[s].second;
            else
                pos = occ[s].first;
            pos.second ^= 2;

            s = (*(begin + pos.first))[pos.second] - 1;
            if (s == start)
                break;

            // Since we already know each strand appears exactly twice,
            // dir[s] should be unknown at this point.  Update it.
            dir[s] = (occ[s].first == pos ? 1 : -1);

            if (s < min)
                min = s;
        }

        // This finishes the current component.
        // Collect its starting point.
        components.push_back(min);
    }

    // Look for any components that haven't been processed (because they
    // consist entirely of overcrossings, and so the PD code does not
    // define their orientation).
    for (it = begin, index = 0; it != end; ++it, ++index) {
        // This time we look at one of the two (connected) upper strands.
        auto start = (*it)[1] - 1;
        if (dir[start])
            continue;

        // We found a component that has not been processed.
        // Follow the component as before, but this time we choose an
        // arbitrary direction for the starting strand (since we cannot
        // deduce this from the PD code).
        PDPos pos { index, 1 };
        dir[start] = 1;
        size_t min = start;

        auto s = start;
        while (true) {
            if (dir[s] > 0)
                pos = occ[s].second;
            else
                pos = occ[s].first;
            pos.second ^= 2;

            s = (*(begin + pos.first))[pos.second] - 1;
            if (s == start)
                break;

            dir[s] = (occ[s].first == pos ? 1 : -1);

            if (s < min)
                min = s;
        }
        components.push_back(min);
    }

    /*
    for (size_t i = 0; i < 2 * n; ++i) {
        std::cerr << "Strand " << (i + 1) << ": ";
        if (dir[i] > 0) {
            std::cerr << "(" << occ[i].first.first << ","
                << occ[i].first.second << ") --> (" << occ[i].second.first
                << "," << occ[i].second.second << ")" << std::endl;
        } else {
            std::cerr << "(" << occ[i].first.first << ","
                << occ[i].first.second << ") <-- (" << occ[i].second.first
                << "," << occ[i].second.second << ")" << std::endl;
        }
    }
    */

    // Build and hook together the final list of crossings.
    Link ans;
    for (size_t i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing);
    for (size_t i = 0; i < 2 * n; ++i) {
        PDPos from, to;
        if (dir[i] > 0) {
            from = occ[i].first;
            to = occ[i].second;
        } else {
            from = occ[i].second;
            to = occ[i].first;
        }
        ans.join(
            StrandRef(ans.crossings_[from.first], (from.second % 2 ? 1 : 0)),
            StrandRef(ans.crossings_[to.first], (to.second % 2 ? 1 : 0)));

        // If this strand exits from the upper side of its source crossing,
        // use this to determine the crossing's sign.
        if (from.second == 1)
            ans.crossings_[from.first]->sign_ = 1;
        else if (from.second == 3)
            ans.crossings_[from.first]->sign_ = -1;
    }

    // Finally, mark the starting point of each component.
    std::sort(components.begin(), components.end());
    for (auto start : components) {
        const PDPos& from = (dir[start] > 0 ? occ[start].first :
            occ[start].second);
        ans.components_.emplace_back(ans.crossings_[from.first],
            (from.second % 2 ? 1 : 0));
    }

    delete[] dir;
    delete[] occ;
    return ans;
}

} // namespace regina

#endif

