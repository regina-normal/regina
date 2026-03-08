
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file link/dt-impl.h
 *  \brief Contains implementation details for parsing Dowker-Thistlethwaite
 *  notation for knots.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __REGINA_DT_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_DT_IMPL_H
#endif

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace regina {

template <typename Iterator>
Link Link::fromDT(Iterator begin, Iterator end) {
    using InputInt = typename std::iterator_traits<Iterator>::value_type;
    static_assert(SignedCppInteger<InputInt>, "fromDT(): the iterator type "
        "needs to dereference to give a native signed C++ integer type.");

    // Extract the number of crossings.
    size_t n = end - begin;
    if (n == 0)
        return { 1 }; // zero-crossing unknot

    // Some basic sanity checking.
    // We ensure that the integers are in range, but we do not yet check
    // that their absolute values are distinct (that will come later).

    if constexpr (sizeof(InputInt) <= sizeof(size_t)) {
        if (2 * n > static_cast<size_t>(std::numeric_limits<InputInt>::max()))
            throw InvalidArgument("fromDT(): too many crossings for "
                "the given integer type");
    }
    const auto maxEntry = static_cast<InputInt>(2 * n);

    Iterator it;
    for (it = begin; it != end; ++it) {
        if (*it % 2 != 0)
            throw InvalidArgument("fromDT(): code contains odd integer");
        if (*it == 0 || *it > maxEntry || *it < -maxEntry)
            throw InvalidArgument("fromDT(): integer out of range in code");
    }

    // First we follow the way that the SnapPea kernel converts the D-T code
    // into a sequence of crossings.  We will then use this sequence of
    // crossings to build a classical Gauss code, and finally we reconstruct
    // the knot diagram from that using Regina's separate fromGauss() routine.

    // 1) Cache the absolute values of the entries in the D-T code, and
    // switch from 1-based indexing to 0-based indexing.
    size_t i;
    FixedArray<size_t> abs(n);
    for (it = begin, i = 0; it != end; ++it, ++i)
        abs[i] = std::abs(*it) - 1;

    // 2) Build the involution that relates the two passes through each
    // crossing (one at an even index, and one at an odd index).
    FixedArray<size_t> inv(2 * n, 1 /* does not appear in abs[] */);
    for (i = 0; i < n; i++) {
        if (inv[abs[i]] != 1 /* the initial value */)
            throw InvalidArgument("fromDT(): repeated |entry| in code");
        inv[2 * i] = abs[i];
        inv[abs[i]] = 2 * i;
    }

    // 3) For each crossing, identify the two positions in inv[] where it
    // occurs.  We will use 1-based indexing for the crossings, since that is
    // what the Gauss code will need.
    FixedArray<InputInt> crossingForPos(2 * n);
    InputInt nextUnused = 1;
    for (i = 0; i < 2 * n; ++i) {
        // Examine position i from the involution.
        if (inv[i] > i) {
            // First time we see this crossing.
            crossingForPos[i] = nextUnused++;
        } else {
            // Second time we see this crossing.
            crossingForPos[i] = crossingForPos[inv[i]];
        }
    }

    // At this point we are done following the SnapPea kernel.  Use the data
    // we have just extracted to build the classical Gauss code.
    FixedArray<InputInt> gauss(2 * n);
    for (i = 0; i < 2 * n; ++i) {
        if (i % 2 == 0) {
            // This is an odd index in the original 1-based indexing.
            if (*(begin + (i / 2)) > 0)
                gauss[i] = -crossingForPos[i];
            else
                gauss[i] = crossingForPos[i];
        } else {
            // This is an even index in the original 1-based indexing.
            if (*(begin + (inv[i] / 2)) > 0)
                gauss[i] = crossingForPos[i];
            else
                gauss[i] = -crossingForPos[i];
        }
    }

    // Now that we have a classical Gauss code, we can reconstruct the precise
    // planar embedding of the knot diagram using Regina's Gauss code
    // reconstruction routine.
    return fromGauss(gauss.begin(), gauss.end());
}

} // namespace regina

#endif

