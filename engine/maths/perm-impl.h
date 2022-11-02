
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
 *  \brief Contains some of the implementation details for the various
 *  Perm class templates.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this implementation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
#include "maths/perm.h"

#ifndef __DOXYGEN
// Hide the entire implementation from doxygen, since it cannot handle
// specialisations.
#ifndef __REGINA_PERM_IMPL_H
#define __REGINA_PERM_IMPL_H

// Import the specialised template classes.
#include "maths/spec/perm6.h" // Required by Perm<4>::pairs()
#include "maths/spec/perm2.h"
#include "maths/spec/perm3.h"
#include "maths/spec/perm4.h"
#include "maths/spec/perm5.h"
#include "maths/spec/perm7.h"

namespace regina {

template <int k>
inline constexpr Perm<2> Perm<2>::contract(Perm<k> p) {
    static_assert(k >= 8, "The generic implementation of Perm<2>::contract<k> "
        "requires k >= 8.");

    return Perm<2>(static_cast<Code>(p.permCode() % 2 ? 1 : 0));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<3> p) {
    return Perm<2>(static_cast<Code>(p.permCode() == 0 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<4> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 6 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<5> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 24 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<6> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 120 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<7> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 720 ? 0 : 1));
}

inline void Perm<2>::clear(unsigned from) {
    if (from == 0)
        code_ = 0;
}

template <>
inline constexpr Perm<3> Perm<3>::extend(Perm<2> p) {
    return Perm<3>(static_cast<Code>(
        p.permCode() == 0 ? code012 : code102));
}

template <int k>
inline constexpr Perm<3> Perm<3>::contract(Perm<k> p) {
    static_assert(k >= 5, "The generic implementation of Perm<3>::contract<k> "
        "requires k >= 5.");

    return Perm<3>(p[0], p[1], p[2]);
}

template <>
inline constexpr Perm<3> Perm<3>::contract(Perm<4> p) {
    // Code map: 0,3,8,7,12,15 -> 0,1,2,3,4,5.
    Perm<4>::Code2 c = p.permCode2();
    return Perm<3>::fromPermCode(c == 8 ? 2 : c == 7 ? 3 : c / 3);
}

inline void Perm<3>::clear(unsigned from) {
    if (from <= 1)
        code_ = code012;
}

template <>
inline constexpr Perm<4> Perm<4>::extend(Perm<2> p) {
    return Perm<4>(static_cast<Code2>(p.permCode() == 0 ? 0 : 7));
}

template <>
inline constexpr Perm<4> Perm<4>::extend(Perm<3> p) {
    // This is implemented as an array lookup.
    return Perm<4>::S3[p.S3Index()];
}

template <int k>
inline constexpr Perm<4> Perm<4>::contract(Perm<k> p) {
    static_assert(k >= 5, "The generic implementation of Perm<4>::contract<k> "
        "requires k >= 5.");

    return Perm<4>(p[0], p[1], p[2], p[3]);
}

inline void Perm<4>::clear(unsigned from) {
    if (from <= 1)
        code_ = 0;
    else if (from == 2)
        code_ = (imageTable[code_][0] == 0 ? 0 /* 0123 */ : 7 /* 1023 */);
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<2> p) {
    return Perm<5>(static_cast<Code2>(p.permCode() == 0 ? 0 : 25));
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<3> p) {
    // This is implemented as an array lookup.
    return Perm<5>::S3[p.S3Index()];
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<4> p) {
    // This is implemented as an array lookup.
    return Perm<5>::S4[p.S4Index()];
}

template <int k>
constexpr Perm<5> Perm<5>::contract(Perm<k> p) {
    static_assert(k > 5, "Perm<5>::contract<k> requires k > 5.");

    return Perm<5>(p[0], p[1], p[2], p[3], p[4]);
}

inline void Perm<5>::clear(unsigned from) {
    if (from <= 1)
        code2_ = 0;
    else if (from == 2)
        code2_ = (imageTable[code2_][0] == 0 ? 0 /* 01234 */ : 25 /* 10234 */);
    else if (from == 3) {
        if (imageTable[code2_][3] == 4)
            code2_ = code2_ ^ 1;
    }
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<2> p) {
    return Perm<6>(static_cast<Code2>(p.permCode() == 0 ? 0 : 121));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<3> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode()));
    // Now p6 acts on {3,4,5} in the way that p acts on {0,1,2}.

    return Perm<6>(static_cast<Code2>(451 /* 345012 */)) *
        p6 * Perm<6>(static_cast<Code2>(451 /* 345012 */));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<4> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode2()));
    // Now p6 acts on {2,3,4,5} in the way that p acts on {0,1,2,3}.

    return Perm<6>(static_cast<Code2>(576 /* 450123 */)) *
        p6 * Perm<6>(static_cast<Code2>(304 /* 234501 */));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<5> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode2()));
    // Now p6 acts on {1,2,3,4,5} in the way that p acts on {0,1,2,3,4}.

    return Perm<6>(static_cast<Code2>(601 /* 501234 */)) *
        p6 * Perm<6>(static_cast<Code2>(153 /* 123450 */));
}

template <int k>
constexpr Perm<6> Perm<6>::contract(Perm<k> p) {
    static_assert(k > 6, "Perm<6>::contract<k> requires k > 6.");

    return Perm<6>(p[0], p[1], p[2], p[3], p[4], p[5]);
}

inline void Perm<6>::clear(unsigned from) {
    switch (from) {
        case 0:
        case 1:
            code2_ = 0;
            break;
        case 2:
            // Test if 0 -> 0.
            code2_ = (code2_ < 120 ? 0 /* 012345 */ : 121 /* 102345 */);
            break;
        case 3:
            // Calculate the target ordered S6 index.
            // When rounded down to the nearest multiple of 6,
            // the code should be: 0, 24, 120, 144, 240 or 264.
            // What we want is:    0, 25, 121, 144, 240 or 265.
            code2_ -= (code2_ % 6);
            if (code2_ == 24 || code2_ == 120 || code2_ == 264)
                code2_ ^= 1;
            break;
        case 4:
            if ((*this)[4] == 5)
                code2_ = code2_ ^ 1;
            break;
        default:
            break;
    }
}

template <>
inline constexpr Perm<7> Perm<7>::extend(Perm<2> p) {
    return Perm<7>(static_cast<Code2>(p.permCode() == 0 ? 0 : 721));
}

template <int k>
inline constexpr Perm<7> Perm<7>::extend(Perm<k> p) {
    static_assert(2 < k && k < 7,
        "The generic implementation of Perm<7>::extend<k> requires 2 < k < 7.");

    Perm<7> p7(static_cast<Code2>(p.SnIndex()));
    // Now p7 acts on {(7-k),...,6} in the way that p acts on {0,...,(k-1)}.

    // Since rot(k) can be evaluated at compile-time, we hope that the
    // compiler actually does this.
    return rot(k) * p7 * rot(7 - k);
}

template <int k>
constexpr Perm<7> Perm<7>::contract(Perm<k> p) {
    static_assert(k > 7, "Perm<7>::contract<k> requires k > 7.");

    return Perm<7>(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
}

inline void Perm<7>::clear(unsigned from) {
    switch (from) {
        case 0:
        case 1:
            code2_ = 0;
            break;
        case 2:
            // Test if 0 -> 0.
            code2_ = (code2_ < 720 ? 0 /* 0123456 */ : 721 /* 1023456 */);
            break;
        case 3:
            // When rounded down to the nearest multiple of 24,
            // the code is the correct *ordered* S7 index.
            code2_ = convOrderedUnordered<Code2>(code2_ - (code2_ % 24));
            break;
        case 4:
            // When rounded down to the nearest multiple of 6,
            // the code is the correct *ordered* S7 index.
            code2_ = convOrderedUnordered<Code2>(code2_ - (code2_ % 6));
            break;
        case 5:
            if ((*this)[5] == 6)
                code2_ = code2_ ^ 1;
            break;
        default:
            break;
    }
}

template <int n>
template <int k>
constexpr Perm<n> Perm<n>::extend(Perm<k> p) {
    static_assert(k < n, "Perm<n>::extend<k> requires k < n.");

    // TODO: Reimplement this to replace the first loop with a direct
    // copy of p's code, in the case where Perm<k> and Perm<n> use the
    // same style of code with the same value of imageBits.

    Code c = 0;
    int i = 0;
    for ( ; i < k; ++i)
        c |= (static_cast<Code>(p[i]) << (imageBits * i));
    for ( ; i < n; ++i)
        c |= (static_cast<Code>(i) << (imageBits * i));

    return Perm<n>(c);
}

template <int n>
template <int k>
constexpr Perm<n> Perm<n>::contract(Perm<k> p) {
    static_assert(n < k, "Perm<n>::contract<k> requires n < k.");

    // TODO: Reimplement this to directly truncate p's code, in the case
    // where Perm<k> and Perm<n> use the same style of code with the
    // same value of imageBits.

    Code c = 0;
    int i = 0;
    for ( ; i < n; ++i)
        c |= (static_cast<Code>(p[i]) << (imageBits * i));

    return Perm<n>(c);
}

template <int n>
void Perm<n>::tightEncode(std::ostream& out) const {
    // Write the Sn index in base 94, least significant digit first.
    Index idx = SnIndex();
    for (int i = 0; i < tightChars_; ++i) {
        out << static_cast<char>((idx % 94) + 33);
        idx /= 94;
    }
}

template <int n>
std::string Perm<n>::tightEncoding() const {
    // Write the Sn index in base 94, least significant digit first.
    char ans[tightChars_ + 1];
    Index idx = SnIndex();
    for (int i = 0; i < tightChars_; ++i) {
        ans[i] = static_cast<char>((idx % 94) + 33);
        idx /= 94;
    }
    ans[tightChars_] = 0;
    return ans;
}

template <int n>
template <typename iterator>
Perm<n> Perm<n>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    // Ensure that our calculations will not overflow, even when reading
    // an *invalid* encoding.  Here we note that:
    //   - 2^31 > 94^4;
    //   - 2^63 > 94^7.
    // We enforce this now so that we will notice that we could technically
    // break things if we optimise the Index type at a later date.  (Though
    // the only possible "breakage" is that an invalid encoding might be
    // incorrectly recognised as valid due to an overflow wrap-around).
    static_assert(sizeof(Index) >= (tightChars_ <= 4 ? 4 : 8),
        "The Index type could potentially overflow when reading an "
        "invalid tight encoding.");

    Index idx = 0;
    Index power = 1;
    for (int i = 0; i < tightChars_; ++i) {
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        Index piece = (*start++) - 33;
        // code >= 0 because we are using an unsigned data type.
        if (piece > 94)
            throw InvalidInput("The tight encoding is invalid");
        idx += (piece * power);
        power *= 94;
    }

    if (idx < 0 || idx >= nPerms)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Sn[idx];
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
inline void Perm<n>::clear(unsigned from) {
    for (int i = from; i < n; ++i) {
        code_ &= ~(imageMask << (imageBits * i));
        code_ |= (static_cast<Code>(i) << (imageBits * i));
    }
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
#endif // __DOXYGEN

