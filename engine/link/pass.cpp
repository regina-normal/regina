
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

#include "link.h"
#include <algorithm>

namespace regina {

bool Link::hasReducingPass() const {
    // Get rid of 0-crossing links.
    if (crossings_.empty())
        return false;

    // The planarity test is expensive (linear time), but finding a reducing
    // pass is more expensive (cubic time), and it's best to tell users if
    // they're doing things that will give mysteriously wrong answers.
    if (! isClassical())
        throw FailedPrecondition("Regina can only work with pass moves "
            "in classical link diagrams, not virtual link diagrams");

    // We consider sides of arcs: for crossing i, we denote:
    // 4i   = left side of upper outgoing arc
    // 4i+1 = right side of upper outgoing arc
    // 4i+2 = left side of lower outgoing arc
    // 4i+3 = right side of lower outgoing arc
    //
    // Thus the index is:
    // 4 * crossing index + (2 if lower) + (1 if right side)

    // We will build an all-pairs shortest paths matrix between sides of
    // arcs, where the distance represents the number of strands we must
    // cross to get from one (side of arc) to the other.  Here we only
    // allow crossing a single strand (i.e., we cannot move between
    // regions by passing through a crossing).
    //
    // For links diagrams with multiple disjoint components, our shortest paths
    // algorithm will fail to realise that you can jump between components
    // (since it essentially works by crawling around the boundary curves of
    // cells in the link diagram).  As a result, the "shortest paths" between
    // disjoint components will be infinity.  This does not matter, since a
    // pass move always begins in a single component of the diagram, and (as
    // we want a _reducing_ pass) it is never beneficial for such a move to
    // interact with other components.

    size_t nSides = 4 * crossings_.size();

    // A distance that is larger than anything we should legitimately see:
    size_t infinity = nSides + 1;

    // dist[nSides*i + j] denotes the distance from (side of arc) i to j.
    FixedArray<size_t> dist(nSides * nSides, infinity);

    size_t upperOutgoing, lowerOutgoing, upperIncoming, lowerIncoming;
    StrandRef prev;
    for (Crossing* c : crossings_) {
        upperOutgoing = 4 * c->index();
        lowerOutgoing = upperOutgoing + 2;

        prev = c->upper().prev();
        upperIncoming = 4 * prev.crossing()->index() +
            (prev.strand() == 0 ? 2 : 0);

        prev = c->lower().prev();
        lowerIncoming = 4 * prev.crossing()->index() +
            (prev.strand() == 0 ? 2 : 0);

        // Identify the initial distance 0 pairs:
        if (c->sign() > 0) {
            // Clockwise order:
            // upper outgoing; lower incoming; upper incoming; lower outgoing
            dist[nSides * (upperOutgoing + 1) + (lowerIncoming + 1)] =
            dist[nSides * (lowerIncoming + 1) + (upperOutgoing + 1)] =
            dist[nSides * (lowerIncoming) + (upperIncoming + 1)] =
            dist[nSides * (upperIncoming + 1) + (lowerIncoming)] =
            dist[nSides * (upperIncoming) + (lowerOutgoing)] =
            dist[nSides * (lowerOutgoing) + (upperIncoming)] =
            dist[nSides * (lowerOutgoing + 1) + (upperOutgoing)] =
            dist[nSides * (upperOutgoing) + (lowerOutgoing + 1)] = 0;
        } else {
            // Clockwise order:
            // upper outgoing; lower outgoing; upper incoming; lower incoming
            dist[nSides * (upperOutgoing + 1) + (lowerOutgoing)] =
            dist[nSides * (lowerOutgoing) + (upperOutgoing + 1)] =
            dist[nSides * (lowerOutgoing + 1) + (upperIncoming + 1)] =
            dist[nSides * (upperIncoming + 1) + (lowerOutgoing + 1)] =
            dist[nSides * (upperIncoming) + (lowerIncoming + 1)] =
            dist[nSides * (lowerIncoming + 1) + (upperIncoming)] =
            dist[nSides * (lowerIncoming) + (upperOutgoing)] =
            dist[nSides * (upperOutgoing) + (lowerIncoming)] = 0;
        }
    }

    // Identify the initial distance 1 pairs, which are just the
    // matching left/right pairs:
    size_t i, j, k;
    for (i = 0; i < nSides; i += 2)
        dist[nSides * i + (i + 1)] =
        dist[nSides * (i + 1) + i] = 1;

    // Run Floyd-Warshall to compute all-pairs shortest paths:
    for (k = 0; k < nSides; ++k)
        for (i = 0; i < nSides; ++i)
            for (j = 0; j < nSides; ++j) {
                // To get from i to j, go via k if this helps.
                if (dist[nSides * i + j] >
                        dist[nSides * i + k] + dist[nSides * k + j])
                    dist[nSides * i + j] =
                        dist[nSides * i + k] + dist[nSides * k + j];
            }

#if 0
    for (i = 0; i < nSides; ++i)
        for (j = i + 1; j < nSides; ++j) {
            std::cout << ((i & 2) ? '_' : '^') << (i / 4)
                << ((i & 1) ? '>' : '<') << "  <-->  ";
            std::cout << ((j & 2) ? '_' : '^') << (j / 4)
                << ((j & 1) ? '>' : '<') << "  :  ";
            std::cout << dist[nSides * i + j] << "  ==  "
                << dist[nSides * j + i] << std::endl;
        }
#endif

    // Now we need to identify maximal sequences of strands that either
    // (i) only involve over-crossings, or (ii) only involve under-crossings;
    // and then see compare the number of crossings in these sequences
    // against the corresponding shortest-paths entries in dist[].

    // We make two passes through each link component: once looking for
    // sequences of over-crossings, and then once looking for sequences of
    // under-crossings.  This is wasteful, but I've been flying for 28 hours
    // now and this increases my chances of getting it right the first time. :/

    for (auto comp : components_) {
        // Zero-crossing components do not have reducing pass moves.
        if (! comp)
            continue;

        // Start with over-crossings.  This requires us to begin our
        // traversal from an under-crossing.
        StrandRef start = underForComponent(comp);
        if (! start) {
            // This component is a zero-crossing knot placed above the rest of
            // the diagram, which means the entire knot can be slid away.
            // This can be viewed as a reducing pass.
            return true;
        }

        StrandRef s = start;
        StrandRef beginSeq; // Arc from under-crossing to over-crossing
        StrandRef endSeq; // Arc from over-crossing to under-crossing
        size_t seqLen;

        do {
            if (s.strand() == 0) {
                // This is always run in the first iteration of the loop,
                // thereby initialising beginSeq and seqLen.
                beginSeq = s;
                seqLen = 0;
            } else
                ++seqLen;

            ++s;

            if (s.strand() == 0) {
                endSeq = s.prev();

                if (seqLen > 0) {
                    // Compare this to the shortest path in dist[].
                    // Note: we know that beginSeq is a (lower outgoing) arc,
                    // and that endSeq is an (upper outgoing) arc.
                    i = 4 * beginSeq.crossing()->index() + 2;
                    j = 4 * endSeq.crossing()->index();
                    if (dist[nSides * i + j] < seqLen ||
                            dist[nSides * i + j + 1] < seqLen ||
                            dist[nSides * (i + 1) + j] < seqLen ||
                            dist[nSides * (i + 1) + j + 1] < seqLen) {
                        return true;
                    }
                }
            }
        } while (s != start);

        // Now look for sequences of under-crossings.
        // This time we must begin our traversal from an over-crossing.
        start = overForComponent(comp);
        if (! start) {
            // This component is a zero-crossing knot placed below the rest of
            // the diagram.  Again this can be viewed as a reducing pass.
            return true;
        }

        s = start;
        do {
            if (s.strand() == 1) {
                // This is always run in the first iteration of the loop,
                // thereby initialising beginSeq and seqLen.
                beginSeq = s;
                seqLen = 0;
            } else
                ++seqLen;

            ++s;

            if (s.strand() == 1) {
                endSeq = s.prev();

                if (seqLen > 0) {
                    // Compare this to the shortest path in dist[].
                    // Note: we know that beginSeq is an (upper outgoing) arc,
                    // and that endSeq is a (lower outgoing) arc.
                    i = 4 * beginSeq.crossing()->index();
                    j = 4 * endSeq.crossing()->index() + 2;
                    if (dist[nSides * i + j] < seqLen ||
                            dist[nSides * i + j + 1] < seqLen ||
                            dist[nSides * (i + 1) + j] < seqLen ||
                            dist[nSides * (i + 1) + j + 1] < seqLen) {
                        return true;
                    }
                }
            }
        } while (s != start);
    }

    // Nothing found!
    return false;
}

} // namespace regina

