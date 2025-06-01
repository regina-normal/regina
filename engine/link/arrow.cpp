
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "link/link.h"
#include "progress/progresstracker.h"
#include "utilities/bitmanip.h"
#include <thread>

namespace regina {

namespace {
    /**
     * Defines the granularity of how the naive algorithm allocates bitmasks
     * (resolutions of crossings) to the working threads.
     */
    constexpr int sliceBits = 10;

    /**
     * The polynomial -A^-2 - A^2.
     */
    const Laurent<Integer> loopPoly { -2, { -1, 0, 0, 0, -1 } };

    /**
     * Used as a return value when the arrow polynomial calculation has been
     * cancelled.
     */
    const regina::Arrow noResult;

    /**
     * Internal to arrowNaive().
     *
     * This function returns information about the loops in the given link that
     * are produced by resolving each crossing according to the given bitmask:
     *
     * - If the <i>i</i>th bit in \a mask is 0, crossing \a i should be
     *   resolved by turning _left_ when entering along the upper strand.
     *
     * - If the <i>i</i>th bit in \a mask is 1, crossing \a i should be
     *   resolved by turning _right_ when entering along the upper strand.
     *
     * The information returned consists of:
     *
     * - the number of loops obtained by the given resolution;
     *
     * - a sequence indicating how many loops there are with each possible
     *   number of cusp pairs.
     *
     * For details on what is meant by a cusp pair, see H.A. Dye and
     * L.H. Kauffman, "Virtual crossing number and the arrow polynomial",
     * J. Knot Theory Ramifications 18 (2009), no. 10, 1335-1357.
     *
     * If \a seq is the sequence that is returned, then `seq[i]` holds the
     * number of loops with `i+1` cusp pairs; moreover, if \a seq is non-empty
     * then its final entry will be strictly positive.
     *
     * \pre `link.size() < 64` (here 64 is the length of the bitmask type).
     */
    std::pair<size_t, Arrow::DiagramSequence> resolutionCuspedLoops(
            const Link& link, uint64_t mask) {
        size_t n = link.size();

        // Here we store whether we have seen the half of the upper strand
        // at each crossing...
        // found[0..n) : ... that exits the crossing
        // found[n..2n) : ... that enters the crossing
        FixedArray<bool> found(2 * n, false);

        size_t loops = 0;

        // We will count the number of cusp pairs (i.e., pairs of nodal arrows)
        // in each loop that we find.  Note: 2n strands yields ≤ n cusp pairs.
        //
        // - countForPairs[i] will holds the number of times we see a loop with
        //   (i+1) cusp pairs;
        // - maxPairs will hold the largest number of cusp pairs in any loop.

        FixedArray<size_t> countForPairs(n, 0);
        size_t maxPairs = 0;

        // The following two loops iterate through indices of found[] in
        // increasing order.
        for (int dirInit = 0; dirInit < 2; ++dirInit) {
            for (size_t pos = 0; pos < n; ++pos) {
                // dirInit: 1 = with arrows, 0 = against arrows.
                // This refers to the direction along the strand as you
                // approach the crossing (before you jump to the other strand).
                if (! found[pos + (dirInit ? n : 0)]) {
                    StrandRef s = link.crossing(pos)->upper();
                    int dir = dirInit;
                    size_t len = 0;

                    // Nodal arrows are represented by +/-1, indicating
                    // forward/backward along the current loop traversal.
                    size_t nodalArrowCount = 0;
                    int lastNodalArrow; // ignored if nodalArrowCount == 0

                    do {
                        const uint64_t bit =
                            uint64_t(1) << s.crossing()->index();

                        int nodalArrow;
                        if (s.crossing()->sign() > 0) {
                            // Positive crossing:
                            if ((mask & bit) == 0) {
                                // Turn in a way consistent with the arrows.
                                nodalArrow = 0;
                                if (dir == 1) {
                                    found[s.crossing()->index() +
                                        (s.strand() ? n : 0)] = true;
                                    s = s.crossing()->next(s.strand() ^ 1);
                                } else {
                                    found[s.crossing()->index() +
                                        (s.strand() ? 0 : n)] = true;
                                    s = s.crossing()->prev(s.strand() ^ 1);
                                }
                            } else {
                                // Turn in a way inconsistent with the arrows.
                                nodalArrow = (s.strand() ? 1 : -1);
                                if (dir == 1) {
                                    found[s.crossing()->index() + n] = true;
                                    s = s.crossing()->prev(s.strand() ^ 1);
                                } else {
                                    found[s.crossing()->index()] = true;
                                    s = s.crossing()->next(s.strand() ^ 1);
                                }
                                dir ^= 1;
                            }
                        } else {
                            // Negative crossing:
                            if (mask & bit) {
                                // Turn in a way consistent with the arrows.
                                nodalArrow = 0;
                                if (dir == 1) {
                                    found[s.crossing()->index() +
                                        (s.strand() ? n : 0)] = true;
                                    s = s.crossing()->next(s.strand() ^ 1);
                                } else {
                                    found[s.crossing()->index() +
                                        (s.strand() ? 0 : n)] = true;
                                    s = s.crossing()->prev(s.strand() ^ 1);
                                }
                            } else {
                                // Turn in a way inconsistent with the arrows.
                                nodalArrow = (s.strand() ? -1 : 1);
                                if (dir == 1) {
                                    found[s.crossing()->index() + n] = true;
                                    s = s.crossing()->prev(s.strand() ^ 1);
                                } else {
                                    found[s.crossing()->index()] = true;
                                    s = s.crossing()->next(s.strand() ^ 1);
                                }
                                dir ^= 1;
                            }
                        }

                        if (nodalArrow) {
                            if (nodalArrowCount) {
                                if (nodalArrow == lastNodalArrow) {
                                    // This nodal arrow cancels the last one.
                                    --nodalArrowCount;
                                } else {
                                    // The nodal arrows continue to alternate.
                                    ++nodalArrowCount;
                                }
                                // Either way, the last surviving nodal arrow
                                // changes direction.
                                lastNodalArrow = -lastNodalArrow;
                            } else {
                                // This is our first nodal arrow.
                                lastNodalArrow = nodalArrow;
                                nodalArrowCount = 1;
                            }
                        }

                        ++len;
                    } while (! (dir == dirInit &&
                        s.crossing()->index() == pos && s.strand() == 1));

                    if (nodalArrowCount) {
                        // It is a theorem that nodalArrowCount is always even.
                        if (nodalArrowCount & 1)
                            throw ImpossibleScenario("A resolution gives a "
                                "loop with an odd number of nodal arrows");

                        // Convert nodal arrows to cusp pairs:
                        nodalArrowCount >>= 1;
                        if (nodalArrowCount > maxPairs)
                            maxPairs = nodalArrowCount;
                        ++countForPairs[nodalArrowCount - 1];
                    }

                    ++loops;
                }
            }
        }

        std::pair<size_t, Arrow::DiagramSequence> ans { loops, maxPairs };
        std::copy(countForPairs.begin(), countForPairs.begin() + maxPairs,
            ans.second.begin());
        return ans;
    }

    class ArrowAccumulator {
        private:
            const Link& link_;

            // The number of trivial zero-crossing unknot components.
            const size_t trivialLoops_;

            // The polynomial count_[i-1] is a "partial" arrow polynomial:
            // it only accounts for resolutions with exactly i loops, and it has
            // not yet multiplied through by loopPoly^(i-1).  Our aim is to
            // save the expensive multiplication operations until the very end.
            //
            // Note: we will always have 1 <= i <= #components + #crossings.
            FixedArray<Arrow> count_;

            // The largest number of loops that this accumulator has seen.
            // It is guaranteed that count_[i] == 0 for all i >= maxLoops_.
            size_t maxLoops_;

        public:
            ArrowAccumulator(const Link& link, size_t trivialLoops) :
                    link_(link), trivialLoops_(trivialLoops),
                    count_(link.size() + link.countComponents()), maxLoops_(0) {
            }

            void accumulate(uint64_t maskBegin, uint64_t maskEnd) {
                for (uint64_t mask = maskBegin; mask != maskEnd; ++mask) {
                    auto [ loops, diagramSequence ] =
                        resolutionCuspedLoops(link_, mask);
                    loops += trivialLoops_;
                    if (loops > maxLoops_)
                        maxLoops_ = loops;

                    --loops;

                    // Set shift = #(0 bits) - #(1 bits) in mask.
                    long shift = link_.size() -
                        2 * BitManipulator<uint64_t>::bits(mask);

                    Arrow diagramTerm;
                    diagramTerm.initDiagram(std::move(diagramSequence));
                    diagramTerm.shift(shift);
                    count_[loops] += diagramTerm;
                }
            }

            /**
             * Precondition: this and \a other use the same link, which in
             * particular means that their internal \a count_ arrays have the
             * same size.
             */
            void accumulate(ArrowAccumulator&& other) {
                if (maxLoops_ >= other.maxLoops_) {
                    for (size_t i = 0; i < other.maxLoops_; ++i)
                        count_[i] += std::move(other.count_[i]);
                } else {
                    count_.swap(other.count_);
                    for (size_t i = 0; i < maxLoops_; ++i)
                        count_[i] += std::move(other.count_[i]);
                    maxLoops_ = other.maxLoops_;
                }
            }

            Arrow finalise() {
                Arrow ans;

                Laurent<Integer> loopPow = RingTraits<Laurent<Integer>>::one;
                for (size_t loops = 0; loops < maxLoops_; ++loops) {
                    if (! count_[loops].isZero()) {
                        count_[loops] *= loopPow;
                        ans += count_[loops];
                    }

                    loopPow *= loopPoly;
                }

                // Normalise the polynomial using the writhe of the diagram.
                long w = link_.writhe();
                ans.shift(-3 * w);
                if (w % 2)
                    ans.negate();

                return ans;
            }
    };
}

Arrow Link::arrowNaive(ProgressTracker* tracker, int threads) const {
    if (components_.empty())
        return {};

    size_t n = crossings_.size();
    if (n >= 64) {
        // We cannot use this naive algorithm, since our bitmask type
        // (uint64_t) does not contain enough bits.
        throw NotImplemented("At present, Regina can only compute arrow "
            "polynomials for diagrams with < 64 crossings");
    }

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    static_assert(BitManipulator<uint64_t>::specialised,
        "BitManipulator is not specialised for the mask type.");

    if (tracker)
        tracker->newStage("Enumerating resolutions");

    size_t nTrivial = countTrivialComponents();
    ArrowAccumulator acc(*this, nTrivial);
    if (threads <= 1 || n <= sliceBits) {
        acc.accumulate(0, uint64_t(1) << n);
    } else {
        uint64_t nextSlice = 0;
        uint64_t endSlice = (uint64_t(1) << (n - sliceBits));
        std::mutex mutex;
        FixedArray<std::thread> thread(threads);
        for (int i = 0; i < threads; ++i) {
            thread[i] = std::thread([=, this, &mutex, &nextSlice, &acc]() {
                ArrowAccumulator sub(*this, nTrivial);
                uint64_t currSlice;
                while (true) {
                    {
                        std::scoped_lock lock(mutex);
                        if (tracker) {
                            // Check for cancellation.
                            if (! tracker->setPercent(
                                    double(nextSlice) * 100.0 /
                                    double(endSlice)))
                                break;
                        }
                        if (nextSlice == endSlice) {
                            acc.accumulate(std::move(sub));
                            return;
                        }
                        currSlice = nextSlice++;
                    }
                    sub.accumulate(currSlice << sliceBits,
                        (currSlice + 1) << sliceBits);
                }
            });
        }
        for (int i = 0; i < threads; ++i) {
            thread[i].join();
        }
    }

    if (tracker && tracker->isCancelled())
        return {};

    return acc.finalise();
}

const Arrow& Link::arrow(Algorithm, ProgressTracker* tracker, int threads)
        const {
    if (arrow_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *arrow_;
    }

    // For now we only support the naive algorithm.
    Arrow ans = arrowNaive(tracker, threads);

    if (tracker && tracker->isCancelled()) {
        tracker->setFinished();
        return noResult;
    }

    arrow_ = std::move(ans);

    if (tracker)
        tracker->setFinished();
    return *arrow_;
}

} // namespace regina

