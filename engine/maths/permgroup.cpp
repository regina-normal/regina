
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

#include "maths/permgroup.h"

namespace regina {

template <int n, bool cached>
typename PermGroup<n, cached>::iterator&
        PermGroup<n, cached>::iterator::operator ++() {
    int k = 1;
    // Work out which pos_[k] needs to be incremented.
    while (k < n && pos_[k] == group_->count_[k] - 1)
        ++k;
    if (k == n) {
        // Out of options.
        pos_[0] = 1; // past-the-end
        return *this;
    }

    // Conveniently, all the terms term_[i][j] that we _were_ using for i < k
    // were identities, since we insist that term_[i][i] == id.
    // Therefore the only term that we need to remove before the increment
    // is the term for k.

    if constexpr (cached)
        current_ = current_.cachedComp(
            group_->term_[group_->usable_[k][pos_[k]]][k]); /* inverse term */
    else
        current_ = current_ *
            group_->term_[group_->usable_[k][pos_[k]]][k]; /* inverse term */

    ++pos_[k];

    if constexpr (cached)
        current_ = current_.cachedComp(
            group_->term_[k][group_->usable_[k][pos_[k]]]);
    else
        current_ = current_ * group_->term_[k][group_->usable_[k][pos_[k]]];

    if (k > 1) {
        std::fill(pos_ + 1, pos_ + k, 0);
        if constexpr (cached)
            current_ = current_.cachedComp(group_->initSeq_[k - 1]);
        else
            current_ = current_ * group_->initSeq_[k - 1];
    }

    return *this;
}

template PermGroup<2, false>::iterator&
    PermGroup<2, false>::iterator::operator ++();
template PermGroup<3, false>::iterator&
    PermGroup<3, false>::iterator::operator ++();
template PermGroup<4, false>::iterator&
    PermGroup<4, false>::iterator::operator ++();
template PermGroup<5, false>::iterator&
    PermGroup<5, false>::iterator::operator ++();
template PermGroup<6, false>::iterator&
    PermGroup<6, false>::iterator::operator ++();
template PermGroup<7, false>::iterator&
    PermGroup<7, false>::iterator::operator ++();
template PermGroup<8, false>::iterator&
    PermGroup<8, false>::iterator::operator ++();
template PermGroup<9, false>::iterator&
    PermGroup<9, false>::iterator::operator ++();
template PermGroup<10, false>::iterator&
    PermGroup<10, false>::iterator::operator ++();
template PermGroup<11, false>::iterator&
    PermGroup<11, false>::iterator::operator ++();
template PermGroup<12, false>::iterator&
    PermGroup<12, false>::iterator::operator ++();
template PermGroup<13, false>::iterator&
    PermGroup<13, false>::iterator::operator ++();
template PermGroup<14, false>::iterator&
    PermGroup<14, false>::iterator::operator ++();
template PermGroup<15, false>::iterator&
    PermGroup<15, false>::iterator::operator ++();
template PermGroup<16, false>::iterator&
    PermGroup<16, false>::iterator::operator ++();
template PermGroup<2, true>::iterator&
    PermGroup<2, true>::iterator::operator ++();
template PermGroup<3, true>::iterator&
    PermGroup<3, true>::iterator::operator ++();
template PermGroup<4, true>::iterator&
    PermGroup<4, true>::iterator::operator ++();
template PermGroup<5, true>::iterator&
    PermGroup<5, true>::iterator::operator ++();
template PermGroup<6, true>::iterator&
    PermGroup<6, true>::iterator::operator ++();
template PermGroup<7, true>::iterator&
    PermGroup<7, true>::iterator::operator ++();
template PermGroup<8, true>::iterator&
    PermGroup<8, true>::iterator::operator ++();
template PermGroup<9, true>::iterator&
    PermGroup<9, true>::iterator::operator ++();
template PermGroup<10, true>::iterator&
    PermGroup<10, true>::iterator::operator ++();
template PermGroup<11, true>::iterator&
    PermGroup<11, true>::iterator::operator ++();
template PermGroup<12, true>::iterator&
    PermGroup<12, true>::iterator::operator ++();
template PermGroup<13, true>::iterator&
    PermGroup<13, true>::iterator::operator ++();
template PermGroup<14, true>::iterator&
    PermGroup<14, true>::iterator::operator ++();
template PermGroup<15, true>::iterator&
    PermGroup<15, true>::iterator::operator ++();
template PermGroup<16, true>::iterator&
    PermGroup<16, true>::iterator::operator ++();

template <int n, bool cached>
PermGroup<n, cached>::PermGroup(NamedPermGroup group) {
    // Remember: all permutations not explicitly set here will be
    // initialised to the identity.
    switch (group) {
        case NamedPermGroup::Symmetric:
            for (int k = 1; k < n; ++k)
                for (int j = 0; j < k; ++j) {
                    // These terms are all self-inverse.
                    term_[k][j] = term_[j][k] = Perm<n>(j, k);
                }
            for (int i = 0; i < n; ++i)
                count_[i] = i + 1;
            // Each usable_[i] should be the identity.
            break;
        case NamedPermGroup::Alternating:
            for (int k = 2; k < n; ++k) {
                // Each non-trivial term should be a 3-cycle.
                if constexpr (cached) {
                    term_[k][0] = Perm<n>(0, k).cachedComp(Perm<n>(0, 1));
                    term_[0][k] = term_[k][0].cachedInverse();
                } else {
                    term_[k][0] = Perm<n>(0, k) * Perm<n>(0, 1);
                    term_[0][k] = term_[k][0].inverse();
                }
                for (int j = 1; j < k; ++j) {
                    if constexpr (cached) {
                        term_[k][j] = Perm<n>(j, k).cachedComp(Perm<n>(0, j));
                        term_[j][k] = term_[k][j].cachedInverse();
                    } else {
                        term_[k][j] = Perm<n>(j, k) * Perm<n>(0, j);
                        term_[j][k] = term_[k][j].inverse();
                    }
                }
            }
            count_[0] = 1;
            count_[1] = 1; // this is where A_n differs from S_n
            for (int i = 2; i < n; ++i)
                count_[i] = i + 1;
            // All usable_[k] should be the identity for k != 1.
            usable_[1] = Perm<n>(0, 1);
            break;
        default:
            // Each term_[k][j] should be the identity.
            std::fill(count_, count_ + n, 1);
            for (int i = 1; i < n; ++i)
                usable_[i] = Perm<n>(0, i);
            break;
    }

    setup();
}

template PermGroup<2, false>::PermGroup(NamedPermGroup);
template PermGroup<3, false>::PermGroup(NamedPermGroup);
template PermGroup<4, false>::PermGroup(NamedPermGroup);
template PermGroup<5, false>::PermGroup(NamedPermGroup);
template PermGroup<6, false>::PermGroup(NamedPermGroup);
template PermGroup<7, false>::PermGroup(NamedPermGroup);
template PermGroup<8, false>::PermGroup(NamedPermGroup);
template PermGroup<9, false>::PermGroup(NamedPermGroup);
template PermGroup<10, false>::PermGroup(NamedPermGroup);
template PermGroup<11, false>::PermGroup(NamedPermGroup);
template PermGroup<12, false>::PermGroup(NamedPermGroup);
template PermGroup<13, false>::PermGroup(NamedPermGroup);
template PermGroup<14, false>::PermGroup(NamedPermGroup);
template PermGroup<15, false>::PermGroup(NamedPermGroup);
template PermGroup<16, false>::PermGroup(NamedPermGroup);
template PermGroup<2, true>::PermGroup(NamedPermGroup);
template PermGroup<3, true>::PermGroup(NamedPermGroup);
template PermGroup<4, true>::PermGroup(NamedPermGroup);
template PermGroup<5, true>::PermGroup(NamedPermGroup);
template PermGroup<6, true>::PermGroup(NamedPermGroup);
template PermGroup<7, true>::PermGroup(NamedPermGroup);
template PermGroup<8, true>::PermGroup(NamedPermGroup);
template PermGroup<9, true>::PermGroup(NamedPermGroup);
template PermGroup<10, true>::PermGroup(NamedPermGroup);
template PermGroup<11, true>::PermGroup(NamedPermGroup);
template PermGroup<12, true>::PermGroup(NamedPermGroup);
template PermGroup<13, true>::PermGroup(NamedPermGroup);
template PermGroup<14, true>::PermGroup(NamedPermGroup);
template PermGroup<15, true>::PermGroup(NamedPermGroup);
template PermGroup<16, true>::PermGroup(NamedPermGroup);

template <int n, bool cached>
PermGroup<n, cached>::PermGroup(int k) {
    // Remember: all permutations not explicitly set here will be
    // initialised to the identity.
    for (int upper = 1; upper < k; ++upper)
        for (int lower = 0; lower < upper; ++lower) {
            // These terms are all self-inverse.
            term_[upper][lower] = term_[lower][upper] = Perm<n>(lower, upper);
        }
    for (int i = 0; i < k; ++i)
        count_[i] = i + 1;
    std::fill(count_ + k, count_ + n, 1);
    // Each usable_[0..(k-1)] should be the identity.
    for (int i = k; i < n; ++i)
        usable_[i] = Perm<n>(0, i);

    setup();
}

template PermGroup<2, false>::PermGroup(int);
template PermGroup<3, false>::PermGroup(int);
template PermGroup<4, false>::PermGroup(int);
template PermGroup<5, false>::PermGroup(int);
template PermGroup<6, false>::PermGroup(int);
template PermGroup<7, false>::PermGroup(int);
template PermGroup<8, false>::PermGroup(int);
template PermGroup<9, false>::PermGroup(int);
template PermGroup<10, false>::PermGroup(int);
template PermGroup<11, false>::PermGroup(int);
template PermGroup<12, false>::PermGroup(int);
template PermGroup<13, false>::PermGroup(int);
template PermGroup<14, false>::PermGroup(int);
template PermGroup<15, false>::PermGroup(int);
template PermGroup<16, false>::PermGroup(int);
template PermGroup<2, true>::PermGroup(int);
template PermGroup<3, true>::PermGroup(int);
template PermGroup<4, true>::PermGroup(int);
template PermGroup<5, true>::PermGroup(int);
template PermGroup<6, true>::PermGroup(int);
template PermGroup<7, true>::PermGroup(int);
template PermGroup<8, true>::PermGroup(int);
template PermGroup<9, true>::PermGroup(int);
template PermGroup<10, true>::PermGroup(int);
template PermGroup<11, true>::PermGroup(int);
template PermGroup<12, true>::PermGroup(int);
template PermGroup<13, true>::PermGroup(int);
template PermGroup<14, true>::PermGroup(int);
template PermGroup<15, true>::PermGroup(int);
template PermGroup<16, true>::PermGroup(int);

template <int n, bool cached>
bool PermGroup<n, cached>::contains(Perm<n> p) const {
    for (int i = n - 1; i > 0; --i) {
        // INV: p fixes all elements > i, and if p is in the group then it has
        // a unique representation of the form:
        // term_[i][...] * term_[i-1][...] * ... * term_[1][...].

        int img = p[i];
        if (img == i) {
            // We are insisting that term_[i][i] is the identity.
            // Nothing more to do other than move down to the next i.
            continue;
        }

        // At this point we must have img < i.
        if (term_[i][img].isIdentity()) {
            // We cannot map i -> img.
            return false;
        }
        if (cached)
            p = term_[img][i].cachedComp(p); /* lhs is inverse */
        else
            p = term_[img][i] /* inverse term */ * p;
    }

    // Once we hit i == 0, p must be the identity.
    return true;
}

template bool PermGroup<2, false>::contains(Perm<2>) const;
template bool PermGroup<3, false>::contains(Perm<3>) const;
template bool PermGroup<4, false>::contains(Perm<4>) const;
template bool PermGroup<5, false>::contains(Perm<5>) const;
template bool PermGroup<6, false>::contains(Perm<6>) const;
template bool PermGroup<7, false>::contains(Perm<7>) const;
template bool PermGroup<8, false>::contains(Perm<8>) const;
template bool PermGroup<9, false>::contains(Perm<9>) const;
template bool PermGroup<10, false>::contains(Perm<10>) const;
template bool PermGroup<11, false>::contains(Perm<11>) const;
template bool PermGroup<12, false>::contains(Perm<12>) const;
template bool PermGroup<13, false>::contains(Perm<13>) const;
template bool PermGroup<14, false>::contains(Perm<14>) const;
template bool PermGroup<15, false>::contains(Perm<15>) const;
template bool PermGroup<16, false>::contains(Perm<16>) const;
template bool PermGroup<2, true>::contains(Perm<2>) const;
template bool PermGroup<3, true>::contains(Perm<3>) const;
template bool PermGroup<4, true>::contains(Perm<4>) const;
template bool PermGroup<5, true>::contains(Perm<5>) const;
template bool PermGroup<6, true>::contains(Perm<6>) const;
template bool PermGroup<7, true>::contains(Perm<7>) const;
template bool PermGroup<8, true>::contains(Perm<8>) const;
template bool PermGroup<9, true>::contains(Perm<9>) const;
template bool PermGroup<10, true>::contains(Perm<10>) const;
template bool PermGroup<11, true>::contains(Perm<11>) const;
template bool PermGroup<12, true>::contains(Perm<12>) const;
template bool PermGroup<13, true>::contains(Perm<13>) const;
template bool PermGroup<14, true>::contains(Perm<14>) const;
template bool PermGroup<15, true>::contains(Perm<15>) const;
template bool PermGroup<16, true>::contains(Perm<16>) const;

template <int n, bool cached>
bool PermGroup<n, cached>::operator == (const PermGroup& other) const {
    // A quick pre-check on count_[], which should be identical.
    if (! std::equal(count_, count_ + n, other.count_))
        return false;

    // Check that every generator of this group belongs to other.
    // If so, the groups are equal (since the sizes are the same, so we do
    // not need to do the same test in reverse).

    for (int k = 1; k < n; ++k) {
        // Do not test the last generator term_[k][k], since this is the
        // identity and so will pass for free.
        for (int i = 0; i < count_[k] - 1; ++i) {
            // Examine the following generator:
            Perm<n> p = term_[k][usable_[k][i]];

            // Our containment test is similar to contains(), but uses
            // the fact that we already know that our term fixes k+1,...,n.
            // See the contains() implementation for a full explanation.

            for (int j = k; j > 0; --j) {
                int img = p[j];
                if (img == j)
                    continue;

                if (other.term_[j][img].isIdentity())
                    return false;
                if (cached)
                    p = other.term_[img][j].cachedComp(p); /* lhs is inverse */
                else
                    p = other.term_[img][j] /* inverse term */ * p;
            }
        }
    }

    return true;
}

template bool PermGroup<2, false>::operator == (const PermGroup<2, false>&)
    const;
template bool PermGroup<3, false>::operator == (const PermGroup<3, false>&)
    const;
template bool PermGroup<4, false>::operator == (const PermGroup<4, false>&)
    const;
template bool PermGroup<5, false>::operator == (const PermGroup<5, false>&)
    const;
template bool PermGroup<6, false>::operator == (const PermGroup<6, false>&)
    const;
template bool PermGroup<7, false>::operator == (const PermGroup<7, false>&)
    const;
template bool PermGroup<8, false>::operator == (const PermGroup<8, false>&)
    const;
template bool PermGroup<9, false>::operator == (const PermGroup<9, false>&)
    const;
template bool PermGroup<10, false>::operator == (const PermGroup<10, false>&)
    const;
template bool PermGroup<11, false>::operator == (const PermGroup<11, false>&)
    const;
template bool PermGroup<12, false>::operator == (const PermGroup<12, false>&)
    const;
template bool PermGroup<13, false>::operator == (const PermGroup<13, false>&)
    const;
template bool PermGroup<14, false>::operator == (const PermGroup<14, false>&)
    const;
template bool PermGroup<15, false>::operator == (const PermGroup<15, false>&)
    const;
template bool PermGroup<16, false>::operator == (const PermGroup<16, false>&)
    const;
template bool PermGroup<2, true>::operator == (const PermGroup<2, true>&) const;
template bool PermGroup<3, true>::operator == (const PermGroup<3, true>&) const;
template bool PermGroup<4, true>::operator == (const PermGroup<4, true>&) const;
template bool PermGroup<5, true>::operator == (const PermGroup<5, true>&) const;
template bool PermGroup<6, true>::operator == (const PermGroup<6, true>&) const;
template bool PermGroup<7, true>::operator == (const PermGroup<7, true>&) const;
template bool PermGroup<8, true>::operator == (const PermGroup<8, true>&) const;
template bool PermGroup<9, true>::operator == (const PermGroup<9, true>&) const;
template bool PermGroup<10, true>::operator == (const PermGroup<10, true>&)
    const;
template bool PermGroup<11, true>::operator == (const PermGroup<11, true>&)
    const;
template bool PermGroup<12, true>::operator == (const PermGroup<12, true>&)
    const;
template bool PermGroup<13, true>::operator == (const PermGroup<13, true>&)
    const;
template bool PermGroup<14, true>::operator == (const PermGroup<14, true>&)
    const;
template bool PermGroup<15, true>::operator == (const PermGroup<15, true>&)
    const;
template bool PermGroup<16, true>::operator == (const PermGroup<16, true>&)
    const;

template <int n, bool cached>
void PermGroup<n, cached>::writeTextLong(std::ostream& out) const {
    // We repeat the code for writeTextShort() because we would like to
    // hang on to the computed group size for a bit longer.
    auto s = size();
    out << (s == 1 ? "Trivial" : s == Perm<n>::nPerms ? "Symmetric" :
        (s << 1) == Perm<n>::nPerms ? "Alternating" : "Permutation");
    out << " group of degree " << n << ", order " << s;
    out << std::endl;

    if (s == 1)
        out << "No generators" << std::endl;
    else {
        out << "Generators:" << std::endl;
        for (int k = 1; k < n; ++k)
            if (count_[k] > 1) {
                for (int i = 0; i < count_[k] - 1; ++i) {
                    if (i > 0)
                        out << ' ';
                    out << term_[k][usable_[k][i]];
                }
                out << std::endl;
            }
    }
}

template void PermGroup<2, false>::writeTextLong(std::ostream&) const;
template void PermGroup<3, false>::writeTextLong(std::ostream&) const;
template void PermGroup<4, false>::writeTextLong(std::ostream&) const;
template void PermGroup<5, false>::writeTextLong(std::ostream&) const;
template void PermGroup<6, false>::writeTextLong(std::ostream&) const;
template void PermGroup<7, false>::writeTextLong(std::ostream&) const;
template void PermGroup<8, false>::writeTextLong(std::ostream&) const;
template void PermGroup<9, false>::writeTextLong(std::ostream&) const;
template void PermGroup<10, false>::writeTextLong(std::ostream&) const;
template void PermGroup<11, false>::writeTextLong(std::ostream&) const;
template void PermGroup<12, false>::writeTextLong(std::ostream&) const;
template void PermGroup<13, false>::writeTextLong(std::ostream&) const;
template void PermGroup<14, false>::writeTextLong(std::ostream&) const;
template void PermGroup<15, false>::writeTextLong(std::ostream&) const;
template void PermGroup<16, false>::writeTextLong(std::ostream&) const;
template void PermGroup<2, true>::writeTextLong(std::ostream&) const;
template void PermGroup<3, true>::writeTextLong(std::ostream&) const;
template void PermGroup<4, true>::writeTextLong(std::ostream&) const;
template void PermGroup<5, true>::writeTextLong(std::ostream&) const;
template void PermGroup<6, true>::writeTextLong(std::ostream&) const;
template void PermGroup<7, true>::writeTextLong(std::ostream&) const;
template void PermGroup<8, true>::writeTextLong(std::ostream&) const;
template void PermGroup<9, true>::writeTextLong(std::ostream&) const;
template void PermGroup<10, true>::writeTextLong(std::ostream&) const;
template void PermGroup<11, true>::writeTextLong(std::ostream&) const;
template void PermGroup<12, true>::writeTextLong(std::ostream&) const;
template void PermGroup<13, true>::writeTextLong(std::ostream&) const;
template void PermGroup<14, true>::writeTextLong(std::ostream&) const;
template void PermGroup<15, true>::writeTextLong(std::ostream&) const;
template void PermGroup<16, true>::writeTextLong(std::ostream&) const;

template <int n, bool cached>
PermGroup<n, cached> PermGroup<n, cached>::centraliser(
        const PermClass<n>& conj) {
    // Begin with the trivial group.
    PermGroup ans;

    // The only k with non-trivial term_[k][j] (k > j) are those k that
    // occur at the end of a cycle.

    // Work through each group of cycles of the same size.

    int eltStart = 0; // first element in the first cycle of this group
    int eltEnd = 0;   // first element in the first cycle of the *next* group

    int cycleStart = 0; // first cycle of this group
    int cycleEnd = 1;   // first cycle of the *next* group

    while (true) {
        // INV: eltEnd == eltStart
        // INV: cycleEnd == cycleStart + 1

        int cycleLen = conj.cycle(cycleStart);
        eltEnd += cycleLen;

        int groupSize = 1;
        while (true) {
            // Process k as the last element of cycle #(cycleEnd - 1).
            // We can move k to any j in the range eltStart ≤ j < k.

            int k = eltEnd - 1;
            if (k != eltStart) {
                ans.count_[k] = eltEnd - eltStart;
                ans.usable_[k] = Perm<n>::rot(eltStart);

                if (cycleLen == 1) {
                    // This is a group of fixed points (length 1 cycles),
                    // so our terms can all just be pair swaps.
                    for (int j = eltStart; j < k; ++j)
                        ans.term_[k][j] = ans.term_[j][k] = Perm<n>(j, k);
                } else {
                    int kCycleStart = k + 1 - cycleLen;
                    std::array<int, n> img;

                    // Permutations moving k to an earlier cycle in this group:
                    int j = eltStart;
                    for (int c = 0; c < groupSize - 1; ++c) {
                        // Build the cycle ( j j+1 ... j+cycleLen-1 )
                        for (int i = 0; i < j; ++i)
                            img[i] = i;
                        for (int i = j; i < j + cycleLen - 1; ++i)
                            img[i] = i + 1;
                        img[j + cycleLen - 1] = j;
                        for (int i = j + cycleLen; i < n; ++i)
                            img[i] = i;
                        Perm<n> shift(img);

                        // Build a swap between cycles:
                        // ( j j+1 ... ) <-> ( k kCycleStart ... k-1 )
                        for (int i = 0; i < j; ++i)
                            img[i] = i;
                        img[j] = k;
                        for (int i = j + 1; i < j + cycleLen; ++i)
                            img[i] = i + k - j - cycleLen;
                        for (int i = j + cycleLen; i < kCycleStart; ++i)
                            img[i] = i;
                        for (int i = kCycleStart; i < k; ++i)
                            img[i] = i + j + cycleLen - k;
                        img[k] = j;
                        for (int i = k + 1; i < n; ++i)
                            img[i] = i;
                        Perm<n> term(img);

                        for (int i = 0; i < cycleLen; ++i) {
                            ans.term_[k][j] = term;
                            ans.term_[j][k] = term.inverse();

                            term = shift * term;
                            ++j;
                        }
                    }

                    // Permutations moving k within its own cycle:

                    // Build the cycle ( kCycleStart kCycleStart+1 ... k )
                    for (int i = 0; i < kCycleStart; ++i)
                        img[i] = i;
                    for (int i = kCycleStart; i < k; ++i)
                        img[i] = i + 1;
                    img[k] = kCycleStart;
                    for (int i = k + 1; i < n; ++i)
                        img[i] = i;
                    Perm<n> shift(img);

                    Perm<n> term = shift;

                    for (int i = 0; i < cycleLen - 1; ++i) {
                        ans.term_[k][j] = term;
                        ans.term_[j][k] = term.inverse();

                        term = shift * term;
                        ++j;
                    }

                    // At this point we should have j == k, and we are done.
                }
            }

            if (eltEnd == n)
                goto done;
            if (conj.cycle(cycleStart) != conj.cycle(cycleEnd))
                break;

            eltEnd += cycleLen;
            ++cycleEnd;
            ++groupSize;
        }

        // Move on to the next cycle group.
        eltStart = eltEnd;
        cycleStart = cycleEnd;
        ++cycleEnd;
    }

done:
    ans.setup();
    return ans;
}

template PermGroup<2, false> PermGroup<2, false>::centraliser(
    const PermClass<2>&);
template PermGroup<3, false> PermGroup<3, false>::centraliser(
    const PermClass<3>&);
template PermGroup<4, false> PermGroup<4, false>::centraliser(
    const PermClass<4>&);
template PermGroup<5, false> PermGroup<5, false>::centraliser(
    const PermClass<5>&);
template PermGroup<6, false> PermGroup<6, false>::centraliser(
    const PermClass<6>&);
template PermGroup<7, false> PermGroup<7, false>::centraliser(
    const PermClass<7>&);
template PermGroup<8, false> PermGroup<8, false>::centraliser(
    const PermClass<8>&);
template PermGroup<9, false> PermGroup<9, false>::centraliser(
    const PermClass<9>&);
template PermGroup<10, false> PermGroup<10, false>::centraliser(
    const PermClass<10>&);
template PermGroup<11, false> PermGroup<11, false>::centraliser(
    const PermClass<11>&);
template PermGroup<12, false> PermGroup<12, false>::centraliser(
    const PermClass<12>&);
template PermGroup<13, false> PermGroup<13, false>::centraliser(
    const PermClass<13>&);
template PermGroup<14, false> PermGroup<14, false>::centraliser(
    const PermClass<14>&);
template PermGroup<15, false> PermGroup<15, false>::centraliser(
    const PermClass<15>&);
template PermGroup<16, false> PermGroup<16, false>::centraliser(
    const PermClass<16>&);
template PermGroup<2, true> PermGroup<2, true>::centraliser(
    const PermClass<2>&);
template PermGroup<3, true> PermGroup<3, true>::centraliser(
    const PermClass<3>&);
template PermGroup<4, true> PermGroup<4, true>::centraliser(
    const PermClass<4>&);
template PermGroup<5, true> PermGroup<5, true>::centraliser(
    const PermClass<5>&);
template PermGroup<6, true> PermGroup<6, true>::centraliser(
    const PermClass<6>&);
template PermGroup<7, true> PermGroup<7, true>::centraliser(
    const PermClass<7>&);
template PermGroup<8, true> PermGroup<8, true>::centraliser(
    const PermClass<8>&);
template PermGroup<9, true> PermGroup<9, true>::centraliser(
    const PermClass<9>&);
template PermGroup<10, true> PermGroup<10, true>::centraliser(
    const PermClass<10>&);
template PermGroup<11, true> PermGroup<11, true>::centraliser(
    const PermClass<11>&);
template PermGroup<12, true> PermGroup<12, true>::centraliser(
    const PermClass<12>&);
template PermGroup<13, true> PermGroup<13, true>::centraliser(
    const PermClass<13>&);
template PermGroup<14, true> PermGroup<14, true>::centraliser(
    const PermClass<14>&);
template PermGroup<15, true> PermGroup<15, true>::centraliser(
    const PermClass<15>&);
template PermGroup<16, true> PermGroup<16, true>::centraliser(
    const PermClass<16>&);

} // namespace regina

