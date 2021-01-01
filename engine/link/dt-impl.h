
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

/*! \file link/dt-impl.h
 *  \brief Contains implementation details for parsing Dowker-Thistlethwaite
 *  notation for knots.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __DT_IMPL_H
#ifndef __DOXYGEN
#define __DT_IMPL_H
#endif

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace regina {

// -----------------------------------------------------------------------
//
// The following code contains significant portions of
// kernel/unix_kit/decode_new_DT.c from the SnapPy/SnapPea kernel.
//
// A full explanation of decoding Dowker-Thistlethwaite codes may be found in
//
//     Dowker and Thistlethwaite, Classification of knot projections,
//     Topology and its Applications 16 (1983) 19-31.
//
// -----------------------------------------------------------------------

template <typename Iterator>
Link* Link::fromDT(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    int aNumCrossings = end - begin;
    if (aNumCrossings == 0)
        return new Link(1);

    // Some basic sanity checking.
    // We ensure that the integers are in range, but we do not yet check
    // that their absolute values are distinct (that will come later).
    Iterator it;
    for (it = begin; it != end; ++it) {
        if (*it % 2 != 0) {
            std::cerr << "fromDT(): found odd integer " << *it << std::endl;
            return nullptr;
        }
        if (*it == 0 || *it > 2*aNumCrossings || *it < -2*aNumCrossings) {
            std::cerr << "fromDT(): integer " << *it << " out of range"
                << std::endl;
            return nullptr;
        }
    }

    Link* ans = new Link;

    int i;
    for (i = 0; i < aNumCrossings; ++i)
        ans->crossings_.push_back(new Crossing);

    ans->components_.push_back(StrandRef(ans->crossings_.front(), 0));

    // Here starts the SnapPea code!

    int             *theAlternatingDT,
                    *theInvolution;
    bool            *theRealization;

    /*
     *  Let theAlternatingDT contain the absolute values of the
     *  entries in the DT code.  It describes the alternating knot
     *  with the same projection as the given knot.  For the figure
     *  eight knot example, theAlternatingDT and the DT code are the same,
     *  because the figure eight knot is already alternating.
     */
    theAlternatingDT = new int[aNumCrossings];
    for (it = begin, i = 0; it != end; ++it, ++i)
        theAlternatingDT[i] = abs(*it);

    /*
     *  Switch from 1-based indexing to 0-based indexing.
     *  The involution for the figure eight knot becomes
     *
     *                      0  2  4  6
     *                      3  5  7  1
     *
     *  and theAlternatingDT becomes {3, 5, 7, 1}.
     */
    for (i = 0; i < aNumCrossings; i++)
        theAlternatingDT[i]--;

    /*
     *  Write out the full involution
     *
     *                      0  1  2  3  4  5  6  7
     *                      3  6  5  0  7  2  1  4
     *
     *  As an array, theInvolution = {3, 6, 5, 0, 7, 2, 1, 4}.
     */
    theInvolution = new int[2 * aNumCrossings];
    std::fill(theInvolution, theInvolution + 2 * aNumCrossings,
        1 /* a value that does not appear in theAlternatingDT[] */);
    for (i = 0; i < aNumCrossings; i++)
    {
        if (theInvolution[theAlternatingDT[i]] != 1 /* the initial value */) {
            std::cerr << "fromDT(): +/-" << (theAlternatingDT[i] + 1)
                << " appears more than once" << std::endl;
            delete[] theAlternatingDT;
            delete[] theInvolution;
            delete ans;
            return nullptr;
        }
        theInvolution[2*i]                  = theAlternatingDT[i];
        theInvolution[theAlternatingDT[i]]  = 2*i;
    }

    // After the sanity checks performed so far, we now know that the
    // input sequence contains even integers within range, and that each
    // required absolute value appears exactly once.

    /*
     *  To reconstruct the knot, we need an additional bit of
     *  information for each crossing, saying whether the odd-numbered
     *  strand passes left-to-right across the even-numbered strand,
     *  or vice versa.  Obtaining this "realization" of the DT code
     *  is nontrivial.  For details, see the Dowker-Thistlethwaite
     *  article cited at the top of this file.
     *
     *  Note:  theRealization is an array of booleans.  It doesn't
     *  matter whether you interpret true as meaning the odd-numbered
     *  strand passes left-to-right across the even-numbered strand,
     *  or vice versa, because DT codes don't record chirality
     *  to begin with.
     */
    theRealization = new bool[2 * aNumCrossings];
    if (! realizeDT(theInvolution, theRealization, aNumCrossings)) {
        // The sequence could not be realized.
        std::cerr << "fromDT(): sequence is not realisable" << std::endl;
        delete[] theAlternatingDT;
        delete[] theInvolution;
        delete[] theRealization;
        delete ans;
        return nullptr;
    }

    /*
     *  For each crossing, we now identify the two positions in the
     *  involution where it occurs.
     *
     *  For each position in the involution, we also identify which
     *  crossing it represents.
     */
    int* oddPos = new int[aNumCrossings];
    int* evenPos = new int[aNumCrossings];
    int* crossingForPos = new int[2 * aNumCrossings];

    int nextUnused = 0;
    for (i = 0; i < 2 * aNumCrossings; ++i) {
        // Examine position i from the involution.
        if (theInvolution[i] > i) {
            // First time we see this crossing.
            crossingForPos[i] = nextUnused++;
        } else {
            // Second time we see this crossing.
            crossingForPos[i] = crossingForPos[theInvolution[i]];
        }
        if (i % 2)
            oddPos[crossingForPos[i]] = i;
        else
            evenPos[crossingForPos[i]] = i;
    }

    /*
     *  Build the alternating knot.
     *
     *  Since we adjusted the input sequence to be 0-based, recall that
     *  (even, odd) positions mean (under, over)-crossings respectively.
     */
    Crossing* cr;
    Crossing* adj;
    for (i = 0; i < 2 * aNumCrossings; ++i) {
        cr = ans->crossings_[crossingForPos[i]];

        if (i % 2 == 0) {
            // Pass under.
            adj = ans->crossings_[crossingForPos[
                i < 2 * aNumCrossings - 1 ? i + 1 : 0]];
            cr->next_[0] = StrandRef(adj, 1);

            adj = ans->crossings_[crossingForPos[
                i > 0 ? i - 1 : 2 * aNumCrossings - 1]];
            cr->prev_[0] = StrandRef(adj, 1);

            // Set the sign of the crossing here also.
            // Note that the original SnapPea/SnapPy code only queries
            // theRealization[i] for even indices i.
            cr->sign_ = (theRealization[i] ? 1 : -1);
        } else {
            // Pass over.
            adj = ans->crossings_[crossingForPos[
                i < 2 * aNumCrossings - 1 ? i + 1 : 0]];
            cr->next_[1] = StrandRef(adj, 0);

            adj = ans->crossings_[crossingForPos[
                i > 0 ? i - 1 : 2 * aNumCrossings - 1]];
            cr->prev_[1] = StrandRef(adj, 0);
        }
    }

    /*
     *  Now switch crossings to reflect the signs in the input sequence.
     *
     *  TODO: Use a streamlined verison of change() that does not worry
     *  about multiple components, link properties and firing change events.
     */
    for (it = begin; it != end; ++it)
        if (*it < 0)
            ans->change(ans->crossings_[crossingForPos[-(*it) - 1]]);

    // All done!
    delete[] oddPos;
    delete[] evenPos;
    delete[] crossingForPos;
    delete[] theAlternatingDT;
    delete[] theInvolution;
    delete[] theRealization;

    return ans;
}

} // namespace regina

#endif

