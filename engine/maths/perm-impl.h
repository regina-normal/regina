
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

/*! \file maths/perm-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Perm class template.
 *
 *  This file is _not_ included from perm.h, and it is not shipped with
 *  Regina's development headers.  The routines it contains are explicitly
 *  instantiated in Regina's calculation engine for all \a n.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
#include "maths/perm.h"

#ifndef __REGINA_PERM_IMPL_H
#define __REGINA_PERM_IMPL_H

namespace regina {

template <int n>
void Perm<n>::precompute() {
    std::scoped_lock lock(precomputeMutex);
    if (invLower_)
        return;

    invLower_ = new ImagePack[lowerCount];
    invUpper_ = new ImagePack[upperCount];

    LowerSlice lower;
    do {
        ImagePack d = 0;
        for (int i = 0; i < LowerSlice::length; ++i)
            d |= (static_cast<ImagePack>(i) << (imageBits * lower.image[i]));
        invLower_[lower.pack()] = d;
    } while (lower.inc());

    UpperSlice upper;
    do {
        ImagePack d = 0;
        for (int i = 0; i < UpperSlice::length; ++i)
            d |= (static_cast<ImagePack>(i + LowerSlice::length)
                << (imageBits * upper.image[i]));
        invUpper_[upper.pack()] = d;
    } while (upper.inc());
}

template <int n>
Perm<n>& Perm<n>::operator ++() {
    // We implement a lexicographic "next permutation" algorithm; however,
    // we want to increment according to Sn index, not orderedSn index.
    // Thus some mod 2 fiddling around signs is required.

    if (sign() > 0) {
        // Going from even to odd, we always just swap the last two elements.
        swapImages(n - 2, n - 1);
        return *this;
    }

    bool needSignChange = true;

    // The algorithm for lexicographical "next permutation": find the *last*
    // point in the sequence of images where successive images increase:
    // ..... p < q > r > s > ... > z
    // We then change p to whichever of z,...,s,r,q first exceeds it,
    // and then place the remaining images (along with p) in increasing order.
    // The sign changes iff the length of the sequence q > r > ... > z
    // is 0 or 1 (mod 4).

    int q = (*this)[n - 1];
    int p = (*this)[n - 2];
    int pIdx = n - 2;
    if (p < q) {
        // The next permutation just swaps the last two images.
        // However, since we started with an odd permutation, this will be
        // going *backwards* in the Sn ordering.
        // Swap them now, and carry on to get the *following* permutation.
        swapImages(n - 2, n - 1);
        std::swap(p, q);
        needSignChange = false;
    }
    while (p > q && pIdx > 0) {
        q = p;
        --pIdx;
        p = (*this)[pIdx];
    }
    if (p > q) {
        // The sequence was entirely decreasing.
        // We have reached the end of our iteration.
        code_ = idCode_;
        return *this;
    }

    // Reverse the sequence from pIdx onwards.
    for (int i = 1; pIdx + i < n - i; ++i)
        swapImages(pIdx + i, n - i);

    // Now identify which element needs to be swapped with p.
    for (int i = pIdx + 1; i < n; ++i)
        if ((*this)[i] > p) {
            swapImages(pIdx, i);
            break;
        }

    // Did the sign change?
    int seqRem = (n - 1 - pIdx) % 4;
    if (seqRem == 0 || seqRem == 1) {
        // The sign changed.
        if (! needSignChange)
            swapImages(n - 2, n - 1);
    } else {
        // The sign did not change.
        if (needSignChange)
            swapImages(n - 2, n - 1);
    }

    // Woof.
    return *this;
}

template <int n>
std::vector<Perm<n>> PermClass<n>::centraliser() const {
    // The centraliser could in the worst case have size (n!).
    // Throw an exception on systems where size_t is not large enough for this.
    //
    // Note: the C++ standard guarantees that sizeof(size_t) ≥ 2.
    // The only values we really expect to see in the wild are 2, 4 or ≥ 8,
    // but this is a compile-time test and so we will be pedantic and check
    // for unusual sizes (3, 5) also.
    //
    // Also, note: we exclude n == 8 on 16-bit systems because 8! is too
    // large for a _signed_ 16-bit integer (even though it fits into uint8_t).
    //
    if constexpr (sizeof(size_t) == 2 && n >= 8)
        throw FailedPrecondition("This system only supports 16-bit array "
            "sizes, which is not large enough to hold all of S_n for n ≥ 8");
    else if constexpr (sizeof(size_t) == 3 && n >= 11)
        throw FailedPrecondition("This system only supports 24-bit array "
            "sizes, which is not large enough to hold all of S_n for n ≥ 11");
    else if constexpr (sizeof(size_t) == 4 && n >= 13)
        throw FailedPrecondition("This system only supports 32-bit array "
            "sizes, which is not large enough to hold all of S_n for n ≥ 13");
    else if constexpr (sizeof(size_t) == 5 && n >= 15)
        throw FailedPrecondition("This system only supports 40-bit array "
            "sizes, which is not large enough to hold all of S_n for n ≥ 15");

    size_t count = 1;

    // Identify groups of cycles of the same size.
    int nGroups = 0;
    int groupCycle[n]; // cycle length for this group
    int groupSize[n];  // number of cycles in this group
    int groupStart[n]; // first cycle in this group

    int start = 0;
    int end = 1;
    while (true) {
        groupStart[nGroups] = start;
        groupCycle[nGroups] = cycle_[start];
        count *= groupCycle[nGroups];
        while (end < nCycles_ && cycle_[start] == cycle_[end]) {
            ++end;
            count *= groupCycle[nGroups];
        }
        groupSize[nGroups] = end - start;
        count *= regina::factorial(groupSize[nGroups]);

        ++nGroups;

        if (end == nCycles_)
            break;
        start = end;
        ++end;
    }

    std::vector<Perm<n>> ans;
    ans.reserve(count);

    int cycleStart[n + 1];
    cycleStart[0] = 0;
    for (int i = 0; i < nCycles_; ++i)
        cycleStart[i + 1] = cycleStart[i] + cycle_[i];

    // Prepare to iterate through permutations.
    int cycleMap[n];   // mapping between cycles
    int cycleShift[n]; // shift to apply to each permutation
    std::array<int, n> img;

    for (int i = 0; i < nCycles_; ++i)
        cycleMap[i] = i;
    std::fill(cycleShift, cycleShift + nCycles_, 0);

    int mapDepth, shiftDepth;
    while (true) {
        // Process this mapping between cycles.
        while (true) {
            // Process this set of shifts.
            for (int i = 0; i < nCycles_; ++i)
                for (int j = 0; j < cycle_[i]; ++j)
                    img[cycleStart[i] + j] = cycleStart[cycleMap[i]] +
                        ((j + cycleShift[i]) % cycle_[i]);
            ans.emplace_back(img);

            // Move to the next set of shifts.
            for (shiftDepth = nCycles_ - 1; shiftDepth >= 0; --shiftDepth) {
                if (++cycleShift[shiftDepth] < cycle_[shiftDepth])
                    break;
                cycleShift[shiftDepth] = 0;
            }
            if (shiftDepth < 0)
                break;
        }

        // Move to the next mapping.
        for (mapDepth = nGroups - 1; mapDepth >= 0; --mapDepth) {
            if (groupSize[mapDepth] == 1)
                continue;
            if (std::next_permutation(cycleMap + groupStart[mapDepth],
                    cycleMap + groupStart[mapDepth] + groupSize[mapDepth]))
                break;
        }
        if (mapDepth < 0)
            break;
    }

    return ans;
}

} // namespace regina

#endif // __PERM_IMPL_H

