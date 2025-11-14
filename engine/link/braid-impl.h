
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

    // The braid must have at least 2 "rows" (we use "rows" to avoid a clash
    // of terminology with "strands" of the link diagram), so we can at least
    // make a start on the book-keeping for the first 2 rows (ie, the rows
    // numbered either 0 or 1).
    std::vector<StrandRef> leftmostStrand;
    std::vector<StrandRef> previousStrand;
    std::vector<size_t> rowPerm;
    size_t row;
    for (row : {0,1}) {
        leftmostStrand.emplace_back();
        previousStrand.emplace_back();
        rowPerm.push_back(row);
    }

    // Iterate through the braid word and build the link.
    Link ans;
    size_t uppermostRow = 1;
    size_t upperRow;
    size_t iCross;
    InputInt s;
    for (iCross = 0; iCross < numCross; ++iCross) {
        s = begin[iCross];
        if (s == 0) {
            throw InvalidArgument("fromBraid(): braid word contains 0");
        }

        // Have we found a new uppermost row in the braid?
        upperRow = static_cast<size_t>( std::abs(s) );
        for (++uppermostRow; uppermostRow <= upperRow; ++uppermostRow) {
            leftmostStrand.emplace_back();
            previousStrand.emplace_back();
            rowPerm.push_back(uppermostRow);
        }

        // We have a new crossing that exchanges upperRow and upperRow - 1.
        std::swap( rowPerm[upperRow], rowPerm[upperRow - 1] );
        Crossing* crossing = new Crossing;
        ans.crossings_.push_back(crossing);
        if (s > 0) {
            // Positive crossing.
            //  ___   ___
            //     \ /
            //      \
            //  ___/ \___
            //
            crossing->sign_ = 1;

            // The overstrand either:
            //  --> joins up with the previous strand in upperRow; or
            //  --> there is no previous strand, which means that the
            //      overstrand is the leftmost strand in upperRow.
            if (previousStrand[upperRow]) {
                ans.join( previousStrand[upperRow], crossing->over() );
            } else {
                leftmostStrand[upperRow] = crossing->over();
            }

            // The understrand either:
            //  --> joins up with the previous strand in upperRow - 1; or
            //  --> there is no previous strand, which means that the
            //      understrand is the leftmost strand in upperRow - 1.
            if (previousStrand[upperRow - 1]) {
                ans.join( previousStrand[upperRow - 1], crossing->under() );
            } else {
                leftmostStrand[upperRow - 1] = crossing->under();
            }

            // Update the previous strands.
            previousStrand[upperRow - 1] = crossing->over();
            previousStrand[upperRow] = crossing->under();
        } else {
            // Negative crossing.
            //  ___   ___
            //     \ /
            //      /
            //  ___/ \___
            //
            crossing->sign_ = -1;

            // The understrand either:
            //  --> joins up with the previous strand in upperRow; or
            //  --> there is no previous strand, which means that the
            //      understrand is the leftmost strand in upperRow.
            if (previousStrand[upperRow]) {
                ans.join( previousStrand[upperRow], crossing->under() );
            } else {
                leftmostStrand[upperRow] = crossing->under();
            }

            // The overstrand either:
            //  --> joins up with the previous strand in upperRow - 1; or
            //  --> there is no previous strand, which means that the
            //      overstrand is the leftmost strand in upperRow - 1.
            if (previousStrand[upperRow - 1]) {
                ans.join( previousStrand[upperRow - 1], crossing->over() );
            } else {
                leftmostStrand[upperRow - 1] = crossing->over();
            }

            // Update the previous strands.
            previousStrand[upperRow - 1] = crossing->under();
            previousStrand[upperRow] = crossing->over();
        }
    }

    // At this point, we have effectively built the braid, but haven't done
    // the closure yet.
    std::unordered_set<size_t> untraversedRows;
    for (row = 0; row <= uppermostRow; ++row) {
        if (previousStrand[row]) {
            // Close up this row.
            ans.join( previousStrand[row], leftmostStrand[row] );

            // In a moment, we will need to traverse the leftmost strand of
            // this row to find all the components of the link with at least
            // one crossing.
            untraversedRows.insert(row);
        } else {
            // This row isn't involved in any crossings at all, so it simply
            // forms a zero-crossing unknotted component of the link.
            ans.components_.emplace_back();
        }
    }

    // All that remains is to find all the components (with at least one
    // crossing).
    size_t firstRow;
    size_t currentRow;
    while (not untraversedRows.empty()) {
        firstRow = *untraversedRows.begin();
        untraversedRows.erase( untraversedRows.begin() );
        ans.components_.push_back( leftmostStrand[firstRow] );

        // Traverse and erase all the other leftmost strands that belong to
        // this component.
        currentRow = rowPerm[firstRow];
        while (currentRow != firstRow) {
            untraversedRows.erase(currentRow);
            currentRow = rowPerm[firstRow];
        }
    }
    return ans;
}

}   // namespace regina

#endif

