
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

#include "maths/perm.h"

namespace regina {

template <int n>
void Perm<n>::precompute() {
    std::scoped_lock lock(precomputeMutex);
    if (invLower_)
        return;

    if constexpr (sizeof(size_t) <= 2 /* we always have n >= 8 here */) {
        // We are on a 16-bit machine.
        throw FailedPrecondition("This appears to be a 16-bit machine, and so "
            "cannot build tables for Perm<n>::precompute() for any n ≥ 8.");
    } else if constexpr (sizeof(size_t) == 3 && n >= 9) {
        // We are on a 24-bit machine.
        throw FailedPrecondition("This appears to be a 24-bit machine, and so "
            "cannot build tables for Perm<n>::precompute() for any n ≥ 9.");
    } else if constexpr (sizeof(size_t) < 8 && n >= 13) {
        // This is smaller than 64 bits; make the conservative (but very
        // reasonable) assumption that we are on a 32-bit machine.
        throw FailedPrecondition("This appears to be a 32-bit machine, and so "
            "cannot build tables for Perm<n>::precompute() for any n ≥ 13.");
    } else {
        try {
            invLower_ = new ImagePack[lowerCount];
            invUpper_ = new ImagePack[upperCount];
        } catch (const std::bad_alloc&) {
            throw FailedPrecondition("Not enough memory available to "
                "dynamically allocate tables for Perm<n>::precompute().");
        }

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

// Explicitly instantiate all of the higher-order permutation classes,
// so that their static data members are defined in one and only one place.
//
// See the notes alongside the matching extern declarations in the header
// for why we are doing this.  (Short answer: Windows is a terrible platform
// to try to port software to.)
//
template class Perm<8>;
template class Perm<9>;
template class Perm<10>;
template class Perm<11>;
template class Perm<12>;
template class Perm<13>;
template class Perm<14>;
template class Perm<15>;
template class Perm<16>;

} // namespace regina

