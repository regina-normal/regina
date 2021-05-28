
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

#ifndef __PD_IMPL_H
#ifndef __DOXYGEN
#define __PD_IMPL_H
#endif

#include <algorithm>

namespace regina {

template <typename Iterator>
Link* Link::fromPD(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n == 0) {
        // PD codes do not handle zero-crossing unknots.
        // Just return nothing at all.
        return new Link();
    }

    // Represents (crossing index, position in 4-tuple):
    typedef std::pair<size_t, int> PDPos;

    // The two occurrences of each strand in the PD code:
    typedef std::pair<PDPos, PDPos> PDOccurrence;

    // Identify the two crossings that each strand meets.
    // A position of -1 in the 4-tuple means "not yet seen".
    PDOccurrence* occ = new PDOccurrence[2 * n];
    for (size_t i = 0; i < 2 * n; ++i)
        occ[i].first.second = occ[i].second.second = -1;

    size_t index;
    Iterator it;
    for (it = begin, index = 0; it != end; ++it, ++index) {
        for (int i = 0; i < 4; ++i) {
            auto s = (*it)[i];
            if (s <= 0 || s > 2 * n) {
                std::cerr << "fromPD(): strand number " << s << " out of range"
                    << std::endl;
                delete[] occ;
                return nullptr;
            }
            PDOccurrence* o = occ + (s - 1);
            if (o->first.second < 0) {
                o->first.first = index;
                o->first.second = i;
            } else if (o->second.second < 0) {
                o->second.first = index;
                o->second.second = i;
            } else {
                std::cerr << "fromPD(): strand " << s << " appears "
                    "more than twice" << std::endl;
                delete[] occ;
                return nullptr;
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
        auto start = (*it)[0] - 1;
        if (dir[start]) {
            // We have already processed this strand (and the entire
            // component that contains it).
            continue;
        }

        // We know that start is the incoming lower strand.
        // Follow this component around and identify the directions of
        // all strands on the component.
        dir[start] = (occ[start].first.first == index &&
            occ[start].first.second == 0 ? 1 : -1);

        auto s = start;
        while (true) {
            // TODO: Move s forward to the next strand on this component.
            if (s == start)
                break;
            // Since we already know each strand appears exactly twice,
            // dir[s] should be unknown at this point.
            // Update it.
            // TODO: Update dir[s]
        }
    }

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

    delete[] dir;
    delete[] occ;
    return nullptr;
}

} // namespace regina

#endif

