
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

/*! \file link/gauss-impl.h
 *  \brief Contains implementation details for parsing Gauss codes of knots.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __REGINA_GAUSS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_GAUSS_IMPL_H
#endif

#include <algorithm>
#include <cstdlib>

namespace regina {

template <typename Iterator>
Link Link::fromGauss(Iterator begin, Iterator end) {
    using InputInt = std::remove_cv_t<std::remove_reference_t<decltype(*begin)>>;
    static_assert(std::is_integral_v<InputInt> &&
        ! std::is_unsigned_v<InputInt>, "fromGauss(): the iterator type "
        "needs to refer to a native signed C++ integer type.");

    // Extract the number of crossings.
    size_t n = end - begin;
    if (n % 2)
        throw InvalidArgument("fromGauss(): code has odd number of terms");
    n = n / 2;

    if (n == 0)
        return { 1 }; // Zero-crossing unknot

    // Run Adam's code to determine the handedness of each crossing.

    // Copy the sequence of crossing numbers, since we will need to modify it.
    FixedArray<InputInt> S(2 * n);
    std::copy(begin, end, S.begin());

    for (size_t i=1; i <= n; ++i) {
        // Find the two instances of crossing i,
        // and reverse the subsequence between them.
        //
        // Also, make each element of S[..] positive; we will not need
        // the under/over markers again until the end of this routine.

        size_t pos1;
        for (pos1 = 0; pos1 < 2*n; ++pos1) {
            if (S[pos1] == static_cast<InputInt>(i) ||
                    S[pos1] == -static_cast<InputInt>(i))
                break;
        }
        if (pos1 == 2*n)
            throw InvalidArgument("fromGauss(): crossing not found");

        size_t pos2;
        for (pos2 = pos1+1; pos2 < 2*n; ++pos2) {
            if (S[pos2] == static_cast<InputInt>(i) ||
                    S[pos2] == -static_cast<InputInt>(i))
                break;
        }
        if (pos2 == 2*n)
            throw InvalidArgument("fromGauss(): crossing seen only once");

        // Make both S[pos1] and S[pos2] positive.
        if (S[pos1] == S[pos2])
            throw InvalidArgument(
                "fromGauss(): crossing uses same strand twice");
        if (S[pos1] < 0)
            S[pos1] = - S[pos1];
        else
            S[pos2] = - S[pos2];

        std::reverse(S.begin() + pos1 + 1, S.begin() + pos2);
    }

    // Find the first and second position of each crossing number in the
    // permuted array S[].
    FixedArray<ssize_t> indx0(n, -1);
    FixedArray<ssize_t> indx1(n, -1);

    for (size_t pos1 = 0; pos1 < 2*n; ++pos1) {
        InputInt i = S[pos1] - 1;
        if (indx0[i] < 0)
            indx0[i] = pos1;
        else if (indx1[i] < 0)
            indx1[i] = pos1;
        else
            throw InvalidArgument(
                "fromGauss(): crossing occurs more than twice");
    }

    // Identify interleaved pairs of crossings.
    FixedArray<bool> graph(n*n, false);

    for (size_t i = 1; i < n + 1; i++){
        for (size_t j = 1; j < n + 1; j++){
            if ( ((indx0[i-1] < indx0[j-1]) && (indx1[i-1] > indx0[j-1]) && (indx1[i-1] < indx1[j-1])) ||
                 ((indx0[i-1] < indx1[j-1]) && (indx0[i-1] > indx0[j-1]) && (indx1[i-1] > indx1[j-1])) ) {
                graph[n * (i-1) + (j-1)] = true;
            }
        }
    }

    // Pull apart the nodes of the graph into opposite sides of a
    // bipartite graph.
    FixedArray<int> side(n, 0);
    FixedArray<size_t> stack(n, 0);
    ssize_t top = 0;
    size_t pop = 0;

    for (size_t i = 0; i < n; i++){
        if (side[i] == 0){
            // Make an arbitrary decision for this node, and propagate
            // it through its connected component.
            side[i] = 1;
            top = 0;
            stack[top] = i;

            while (top >= 0) {
                pop = stack[top];
                top--;

                for (size_t j = 0; j < n; j++){
                    if (graph[n * pop + j]) {
                        if (side[j] == 0) {
                            side[j] = - side[pop];
                            top++;
                            stack[top] = j;
                        } else if (side[j] != - side[pop]) {
                            throw InvalidArgument(
                                "fromGauss(): non-bipartite graph");
                        }
                    }
                }
            }
        }
    }

    // From the work above, we know that the input sequence contained each of
    // the integers ±1, ±2, ..., ±n exactly once each.

    // The sequence J encodes which elements of S[..] are first vs second
    // occurrences of a crossing.
    //
    // If S[i] == S[j] and i < j, then we set J[i] = false and J[j] = true.
    FixedArray<bool> J(2*n);
    FixedArray<bool> seen(n, false);
    for (size_t i=0; i < 2*n; ++i) {
        J[i] = seen[S[i]-1];
        seen[S[i]-1] = true;
    }

    FixedArray<InputInt> Q0(2*n);
    FixedArray<bool> Q1(2*n);
    Q0[0]=S[0];
    Q1[0]=false;
    for (size_t i=1; i < 2*n; ++i) {
        // Find the *other* occurrence of the crossing used at Q[i-1].
        for (size_t j=0; j < 2*n; ++j) {
            if (Q0[i-1] == S[j] && Q1[i-1] == ! J[j]) {
                if (i % 2 == 1) {
                    Q0[i] = S[(j > 0 ? j-1 : 2*n-1)];
                    Q1[i] = J[(j > 0 ? j-1 : 2*n-1)];
                } else {
                    Q0[i] = S[(j < 2*n-1 ? j+1 : 0)];
                    Q1[i] = J[(j < 2*n-1 ? j+1 : 0)];
                }
            }
        }

        // Q0 should match the original input sequence.
        if (Q0[i] != *(begin + i) && -Q0[i] != *(begin + i))
            throw InvalidArgument("fromGauss(): Q0 != abs(input sequence)");
    }

    // At this point we can work out the sign of each crossing.
    // The integer crossHand takes the value (-1 vs 1) if, when examining the
    // strand at index1, the other strand runs (left-to-right vs right-to-left).

    Link ans;

    for (size_t i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing);

    int crossHand;
    for (size_t i = 1; i <= n; ++i) {
        ssize_t index1 = -1;
        ssize_t index2 = -1;
        int temp1 = 0, temp2 = 0; // Zero unnecessary but quietens warnings
        // Find the two occurrences of crossing i in sequence Q0.
        for (size_t j=0; j < 2*n; ++j) {
            if (Q0[j] == static_cast<InputInt>(i)) {
                if (j % 2 == 0) {
                    index1 = j;
                    temp1 = (Q1[j] ? -1 : 1);
                } else {
                    index2 = j;
                    temp2 = (Q1[j] ? -1 : 1);
                }
            }
        }
        if (index1 < 0 || index2 < 0)
            throw InvalidArgument("fromGauss(): crossing does not "
                "appear with alternate parities in Q0");

        crossHand = temp1 * temp2 * side[i-1];

        if (*(begin + index1) > 0) {
            // The occurrence at index1 is an over-crossing.
            // A positive crossing corresponds to crossHand == 1.
            ans.crossings_[i - 1]->sign_ = crossHand;
        } else {
            // The occurrence at index1 is an under-crossing.
            // A positive crossing corresponds to crossHand == -1.
            ans.crossings_[i - 1]->sign_ = - crossHand;
        }
    }

    // Now that we have the crossing signs, just hook the crossings together
    // following the input sequence.

    StrandRef prev, curr;
    Iterator it = begin;

    Crossing* cr = ans.crossings_[std::abs(*it) - 1];
    curr = cr->strand(*it > 0 ? 1 : 0);
    ans.components_.push_back(curr);

    for (++it; it != end; ++it) {
        prev = curr;

        cr = ans.crossings_[std::abs(*it) - 1];
        curr = cr->strand(*it > 0 ? 1 : 0);

        prev.crossing()->next_[prev.strand()] = curr;
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    prev = curr;
    curr = ans.components_.back();

    prev.crossing()->next_[prev.strand()] = curr;
    curr.crossing()->prev_[curr.strand()] = prev;

    // I am not sure if a non-planar Gauss code could ever reach this point.
    // I suspect it cannot (and exhaustive testing has failed to find a
    // counterexample).  However, until someone sits down to prove this,
    // we should add a final planarity check since we do promise to throw an
    // exception in such a case.
    if (! ans.isClassical())
        throw InvalidArgument("fromGauss(): diagram is non-planar");

    // All done!
    return ans;
}

template <Link::GaussEnhancement type_, typename Iterator>
Link Link::fromEnhancedGauss(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n % 2)
        throw InvalidArgument("Enhanced Gauss code has odd number of terms");
    n = n / 2;

    if (n == 0)
        return { 1 }; // Zero-crossing unknot

    Link ans;

    size_t i;
    for (i = 0; i < n; ++i)
        ans.crossings_.push_back(new Crossing);

    StrandRef prev, curr;
    Iterator it = begin;

    size_t tmpCross;
    int tmpStrand, tmpSign;
    if constexpr (type_ == GaussEnhancement::Oriented) {
        if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign))
            throw InvalidArgument(
                "Could not parse term in oriented Gauss code");
    } else {
        if (! parseSignedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign))
            throw InvalidArgument(
                "Could not parse term in signed Gauss code");
    }

    Crossing* cr = ans.crossings_[tmpCross - 1];
    cr->sign_ = tmpSign;
    curr = cr->strand(tmpStrand);
    ans.components_.push_back(curr);

    for (++it; it != end; ++it) {
        prev = curr;

        if constexpr (type_ == GaussEnhancement::Oriented) {
            if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign))
                throw InvalidArgument(
                    "Could not parse term in oriented Gauss code");
        } else {
            if (! parseSignedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign))
                throw InvalidArgument(
                    "Could not parse term in signed Gauss code");
        }

        cr = ans.crossings_[tmpCross - 1];
        if (cr->sign_ == 0)
            cr->sign_ = tmpSign;
        else if (cr->sign_ != tmpSign)
            throw InvalidArgument(
                "Crossing has inconsistent signs in enhanced Gauss code");

        curr = cr->strand(tmpStrand);

        if (prev.crossing()->next_[prev.strand()])
            throw InvalidArgument(
                "Multiple passes out of the same strand in enhanced Gauss code");
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()])
            throw InvalidArgument(
                "Multiple passes into the same strand in enhanced Gauss code");
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    prev = curr;
    curr = ans.components_.back();

    if (prev.crossing()->next_[prev.strand()])
        throw InvalidArgument(
            "Multiple passes out of the same strand in enhanced Gauss code");
    prev.crossing()->next_[prev.strand()] = curr;

    if (curr.crossing()->prev_[curr.strand()])
        throw InvalidArgument(
            "Multiple passes into the same strand in enhanced Gauss code");
    curr.crossing()->prev_[curr.strand()] = prev;

    // All done!
    return ans;
}

} // namespace regina

#endif

