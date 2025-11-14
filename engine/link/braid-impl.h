
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2025, Alex He                                           *
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

/*! \file link/braid-impl.h
 *  \brief Contains implementation details for parsing braid words.
 *
 *  This file is automatically included from link.h; there is no need for end
 *  users to include it explicitly.
 */

#ifndef __REGINA_BRAID_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_BRAID_IMPL_H
#endif

#include <cstdlib>  // For std::abs
#include <unordered_set>

namespace regina {

template <typename Iterator>
Link Link::fromBraid(Iterator begin, Iterator end) {
    using InputInt = std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>;
    static_assert(std::is_integral_v<InputInt> &&
        ! std::is_unsigned_v<InputInt>, "fromBraid(): the iterator type "
        "needs to dereference to give a native signed C++ integer type.");

    size_t numCross = end - begin;
    if (numCross == 0) {
        return { 1 };   // Zero-crossing unknot.
    }

    // TODO It should be possible to do everything in just a single pass
    //      through the braid. But is this worth the effort?

    // Make a first pass through the braid in order to:
    //  (a) Work out the number of "rows" in the braid (we use "rows" to avoid
    //      a class of terminology with "strands" of the link diagram).
    //  (b) Work out the signs of all the crossings of the link diagram.
    size_t numRows = 0;
    size_t upperRow;
    size_t iCross;
    InputInt s;
    Link ans;
    for (iCross = 0; iCross < numCross; ++iCross) {
        s = begin[iCross];
        if (s == 0) {
            throw InvalidArgument("fromBraid(): braid word contains 0");
        } else {
            // Have we found a new uppermost row in the braid?
            upperRow = static_cast<size_t>( std::abs(s) );
            if (upperRow > numRows) {
                numRows = upperRow;
            }

            // What's the sign of crossing number iCross?
            if (s > 0) {
                ans.crossings_.push_back( new Crossing(1) );
            } else {
                ans.crossings_.push_back( new Crossing(-1) );
            }
        }
    }
    numRows += 1;

    // In a moment, we will make a second pass through the braid in order to
    // join all the crossings together. As part of the book-keeping, we will:
    //  --> search for the leftmost strand in each row, and
    //  --> keep track of the latest strand that we have encountered in each
    //      row.
    std::vector<StrandRef> leftmostStrand;
    std::vector<StrandRef> latestStrand;
    size_t row;
    for (row = 0; row < numRows; ++row) {
        leftmostStrand.emplace_back( StrandRef() );
        latestStrand.emplace_back( StrandRef() );
    }

    // As promised, go through the braid again to work out how to join all the
    // crossings together.
    for (iCross = 0; iCross < numCross; ++iCross) {
        s = begin[iCross];
        //TODO
    }

    // Close up the braid.
    //TODO

    // Find all the components.
    std::unordered_set<InputInt> unvisitedRows;
    for (InputInt i = 0; i < numCross; ++i) {
        unvisitedRows.insert(i);
    }
    //TODO
}

}   // namespace regina

#endif

