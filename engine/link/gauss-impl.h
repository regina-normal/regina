
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

/*! \file link/gauss-impl.h
 *  \brief Contains implementation details for parsing Gauss codes of knots.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __GAUSS_IMPL_H
#ifndef __DOXYGEN
#define __GAUSS_IMPL_H
#endif

#include <algorithm>

namespace regina {

template <typename Iterator>
Link* Link::fromGauss(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n % 2) {
        std::cerr << "fromGauss(): odd number of terms" << std::endl;
        return nullptr;
    }
    n = n / 2;

    if (n == 0)
        return new Link(1);

    // Run Adam's code to determine the handedness of each crossing.

    // Copy the sequence of crossing numbers, since we will need to modify it.
    int* S = new int[2 * n];
    std::copy(begin, end, S);

    int pos1, pos2;
    for (int i=1; i <= n; ++i) {
        // Find the two instances of crossing i,
        // and reverse the subsequence between them.
        //
        // Also, make each element of S[..] positive; we will not need
        // the under/over markers again until the end of this routine.

        for (pos1 = 0; pos1 < 2*n; ++pos1) {
            if (S[pos1] == i || S[pos1] == -i)
                break;
        }
        if (pos1 == 2*n) {
            // Crossing i was not found at all.
            std::cerr << "fromGauss(): crossing " << i << " not found"
                << std::endl;
            delete[] S;
            return nullptr;
        }

        for (pos2 = pos1+1; pos2 < 2*n; ++pos2) {
            if (S[pos2] == i || S[pos2] == -i)
                break;
        }
        if (pos2 == 2*n) {
            // Crossing i was not found a second time.
            std::cerr << "fromGauss(): crossing " << i << " seen only once"
                << std::endl;
            delete[] S;
            return nullptr;
        }

        // Make both S[pos1] and S[pos2] positive.
        if (S[pos1] == S[pos2]) {
            // The two instances of crossing i both have the same sign.
            std::cerr << "fromGauss(): crossing " << i <<
                " uses same sign twice" << std::endl;
            delete[] S;
            return nullptr;
        }
        if (S[pos1] < 0)
            S[pos1] = - S[pos1];
        else
            S[pos2] = - S[pos2];

        std::reverse(S+pos1+1, S+pos2);
    }

    // At this point, we know that the input sequence contained each of
    // the integers +/-1, +/-2, ..., +/-n exactly once each.

    // The sequence J encodes which elements of S[..] are first vs second
    // occurrences of a crossing.
    //
    // If S[i] == S[j] and i < j, then we set J[i] = false and J[j] = true.
    bool* J = new bool[2*n];

    bool* seen = new bool[n];
    std::fill(seen, seen + n, false);

    for (int i=0; i < 2*n; ++i) {
        J[i] = seen[S[i]-1];
        seen[S[i]-1] = true;
    }

    delete[] seen;

    int* Q0 = new int[2*n];
    bool* Q1 = new bool[2*n];

    Q0[0]=S[0];
    Q1[0]=false;
    for (int i=1; i < 2*n; ++i) {
        // Find the *other* occurrence of the crossing used at Q[i-1].
        for (int j=0; j < 2*n; ++j) {
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
        if (Q0[i] != *(begin + i) && -Q0[i] != *(begin + i)) {
            std::cerr << "fromGauss(): Q0 != abs(input sequence)" << std::endl;
            delete[] J;
            delete[] Q0;
            delete[] Q1;
            delete[] S;
            return nullptr;
        }
    }

    delete[] J;
    delete[] S;

    // At this point we can work out the sign of each crossing.
    // The integer crossHand[i] relates to the crossing at S[i] and Q0[i],
    // and takes the value (-1 vs 1) if the other strand runs
    // (left-to-right vs right-to-left).
    int* crossHand = new int[2*n];

    for (int i = 1; i <= n; ++i) {
        int index1 = -1;
        int index2 = -1;
        int temp1, temp2;
        // Find the two occurrences of crossing i in sequence Q0.
        for (int j=0; j < 2*n; ++j) {
            if (Q0[j]==i) {
                if (j % 2 == 0) {
                    index1 = j;
                    temp1 = (Q1[j] ? -1 : 1);
                } else {
                    index2 = j;
                    temp2 = (Q1[j] ? -1 : 1);
                }
            }
        }
        if (index1 < 0 || index2 < 0) {
            std::cerr << "fromGauss(): crossing " << i
                << " does not appear with alternate parities in Q0"
                << std::endl;
            delete[] Q0;
            delete[] Q1;
            delete[] crossHand;
            return nullptr;
        }
        crossHand[index1] = temp1 * temp2;
        crossHand[index2] = - crossHand[index1];
    }

    delete[] Q0;
    delete[] Q1;

    // Output

    for (int i=0; i < 2*n; i++){
        if (*(begin + i) < 0) {
            std::cerr<<"-";
        }
        if (crossHand[i] == 1) {
            std::cerr<<"<";
        } else {
            std::cerr<<">";
        }
        std::cerr << *(begin + i) << ' ';
    }
    std::cerr << std::endl;
    return nullptr;

/*
    Link* ans = new Link;

    size_t i;
    for (i = 0; i < n; ++i)
        ans->crossings_.push_back(new Crossing);

    StrandRef prev, curr;
    Iterator it = begin;

    size_t tmpCross;
    int tmpStrand, tmpSign;
    if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
        std::cerr << "fromOrientedGauss(): could not parse " << *it
            << std::endl;
        delete ans;
        return nullptr;
    }

    Crossing* cr = ans->crossings_[tmpCross - 1];
    cr->sign_ = tmpSign;
    curr = cr->strand(tmpStrand);
    ans->components_.push_back(curr);

    for (++it; it != end; ++it) {
        prev = curr;

        if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
            std::cerr << "fromOrientedGauss(): could not parse " << *it
                << std::endl;
            delete ans;
            return nullptr;
        }

        cr = ans->crossings_[tmpCross - 1];
        if (cr->sign_ == 0)
            cr->sign_ = tmpSign;
        else if (cr->sign_ != tmpSign) {
            std::cerr << "fromOrientedGauss(): inconsistent signs "
                "for crossing " << tmpCross << std::endl;
            delete ans;
            return nullptr;
        }

        curr = cr->strand(tmpStrand);

        if (prev.crossing()->next_[prev.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes out of "
                << (prev.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (prev.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return nullptr;
        }
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes into "
                << (curr.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (curr.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return nullptr;
        }
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    prev = curr;
    curr = ans->components_.back();

    if (prev.crossing()->next_[prev.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes out of "
            << (prev.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (prev.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return nullptr;
    }
    prev.crossing()->next_[prev.strand()] = curr;

    if (curr.crossing()->prev_[curr.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes into "
            << (curr.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (curr.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return nullptr;
    }
    curr.crossing()->prev_[curr.strand()] = prev;

    // All done!
    return ans;
*/
}

template <typename Iterator>
Link* Link::fromOrientedGauss(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n % 2) {
        std::cerr << "fromOrientedGauss(): odd number of terms" << std::endl;
        return nullptr;
    }
    n = n / 2;

    if (n == 0)
        return new Link(1);

    Link* ans = new Link;

    size_t i;
    for (i = 0; i < n; ++i)
        ans->crossings_.push_back(new Crossing);

    StrandRef prev, curr;
    Iterator it = begin;

    size_t tmpCross;
    int tmpStrand, tmpSign;
    if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
        std::cerr << "fromOrientedGauss(): could not parse " << *it
            << std::endl;
        delete ans;
        return nullptr;
    }

    Crossing* cr = ans->crossings_[tmpCross - 1];
    cr->sign_ = tmpSign;
    curr = cr->strand(tmpStrand);
    ans->components_.push_back(curr);

    for (++it; it != end; ++it) {
        prev = curr;

        if (! parseOrientedGaussTerm(*it, n, tmpCross, tmpStrand, tmpSign)) {
            std::cerr << "fromOrientedGauss(): could not parse " << *it
                << std::endl;
            delete ans;
            return nullptr;
        }

        cr = ans->crossings_[tmpCross - 1];
        if (cr->sign_ == 0)
            cr->sign_ = tmpSign;
        else if (cr->sign_ != tmpSign) {
            std::cerr << "fromOrientedGauss(): inconsistent signs "
                "for crossing " << tmpCross << std::endl;
            delete ans;
            return nullptr;
        }

        curr = cr->strand(tmpStrand);

        if (prev.crossing()->next_[prev.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes out of "
                << (prev.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (prev.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return nullptr;
        }
        prev.crossing()->next_[prev.strand()] = curr;

        if (curr.crossing()->prev_[curr.strand()]) {
            std::cerr << "fromOrientedGauss(): multiple passes into "
                << (curr.strand() == 0 ? "lower" : "upper")
                << " strand of crossing " << (curr.crossing()->index() + 1)
                << std::endl;
            delete ans;
            return nullptr;
        }
        curr.crossing()->prev_[curr.strand()] = prev;
    }

    prev = curr;
    curr = ans->components_.back();

    if (prev.crossing()->next_[prev.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes out of "
            << (prev.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (prev.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return nullptr;
    }
    prev.crossing()->next_[prev.strand()] = curr;

    if (curr.crossing()->prev_[curr.strand()]) {
        std::cerr << "fromOrientedGauss(): multiple passes into "
            << (curr.strand() == 0 ? "lower" : "upper")
            << " strand of crossing " << (curr.crossing()->index() + 1)
            << std::endl;
        delete ans;
        return nullptr;
    }
    curr.crossing()->prev_[curr.strand()] = prev;

    // All done!
    return ans;
}

} // namespace regina

#endif

