
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

// #define DUMP_STAGES

// When tracking progress, try to give much more weight to larger bags.
// (Of course, this should *really* be exponential, but it's nice to see
// some visual progress for smaller bags, so we try not to completely
// dwarf them in the weightings.)
#define HARD_BAG_WEIGHT(bag) (double(bag->size())*(bag->size())*(bag->size()))

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

    /**
     * Computes a partial sum in the naive algorithm for a subset of possible
     * resolutions.  This is used by arrowNaive(), and is designed to support
     * multithreading - each thread uses its own ArrowAccumulator, and works
     * over a different subset of resolutions.
     */
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

Arrow Link::arrowNaive(int threads, ProgressTracker* tracker) const {
    if (components_.empty())
        return {};

    size_t n = crossings_.size();
    if (n >= 64) {
        // We cannot use the naive algorithm, since our bitmask
        // type (uint64_t) does not contain enough bits.
        return arrowTreewidth(tracker);
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

Arrow Link::arrowTreewidth(ProgressTracker* tracker) const {
    if (crossings_.empty())
        return arrowNaive(1 /* single-threaded */, tracker);

    // We are guaranteed >= 1 crossing and >= 1 component.

    // Build a nice tree decomposition.
    if (tracker)
        tracker->newStage("Building tree decomposition", 0.05);

    const TreeDecomposition& d = niceTreeDecomposition();
    size_t nBags = d.size();

    const TreeBag *bag, *child, *sibling;

    size_t nEasyBags = 0;
    double hardBagWeightSum = 0;
    double increment, percent;
    if (tracker) {
        // Estimate processing stages.
        for (bag = d.first(); bag; bag = bag->next()) {
            switch (bag->niceType()) {
                case NiceType::Forget:
                case NiceType::Join:
                    hardBagWeightSum += HARD_BAG_WEIGHT(bag);
                    break;
                default:
                    ++nEasyBags;
                    break;
            }
        }
    }

    // Each partial solution is a key-value map.
    //
    // Each key pairs off strands that connect a crossing in the bag with a
    // crossing that has been forgotten. Strands are numbered 0..(2n-1),
    // where strand i of crossing c is numbered 2c+i.
    //
    // The key is stored as a sequence x[0 .. 2n-1], where each x[i] is a pair:
    // - if strand k is being paired off then x[k] = (s, a), where s is its
    //   partner strand and a is the number of nodal arrows on the path from k
    //   to s through the forgotten region, with the sign of a indicating
    //   whether the first arrow on this path is forwards or backwards;
    // - if strand k connects two forgotten crossings then x[k] = (-1, 0);
    // - otherwise x[k] = (-2, 0).
    //
    // Each value is essentially a partially computed arrow polynomial that
    // accounts for those crossings that have already been forgotten.
    //
    // We ignore any 0-crossing unknot components throughout this
    // calculation, and only factor them in at the very end when we
    // extract the final arrow polynomial.
    //
    // We will be using ints for strand IDs and nodal arrow counts, since we
    // will be storing exponentially many keys in our key-value map and so
    // space is at a premium.  Having strand IDs and arrow counts that fit
    // into an int is enforced through our preconditions (note that the number
    // of arrows is never more than the number of strands).

    size_t nStrands = 2 * size();
    size_t loops;

    using Dest = std::pair<int, int>;
    using Key = LightweightSequence<Dest>;
    using Value = Arrow;
    using SolnSet = std::map<Key, Value>;

    FixedArray<SolnSet*> partial(nBags, nullptr);

    for (bag = d.first(); bag; bag = bag->next()) {
        size_t index = bag->index();
#ifdef DUMP_STAGES
        if (! tracker)
            std::cerr << "Bag " << index << " [" << bag->size() << "] ";
#endif
        if (bag->isLeaf()) {
            // Leaf bag.

            if (tracker) {
                if (tracker->isCancelled())
                    break;
                tracker->newStage(
                    "Processing leaf bag (" + std::to_string(index) +
                        '/' + std::to_string(nBags) + ')',
                    0.05 / nEasyBags);
            }
#ifdef DUMP_STAGES
            else
                std::cerr << "LEAF" << std::endl;
#endif
            partial[index] = new SolnSet;

            Key k(nStrands);
            std::fill(k.begin(), k.end(), Dest(-2, 0));

            partial[index]->emplace(std::move(k),
                RingTraits<Laurent<Integer>>::one);
        } else if (bag->niceType() == NiceType::Introduce) {
            // Introduce bag.
            child = bag->children();

            if (tracker) {
                if (tracker->isCancelled())
                    break;
                tracker->newStage(
                    "Processing introduce bag (" + std::to_string(index) +
                        '/' + std::to_string(nBags) + ')',
                    0.05 / nEasyBags);
            }
#ifdef DUMP_STAGES
            else
                std::cerr << "INTRODUCE" << std::endl;
#endif
            // When introducing a new crossing, all of its arcs must
            // lead to unseen crossings or crossings already in the bag.
            // Therefore the keys and values remain unchanged.

            partial[index] = partial[child->index()];
            partial[child->index()] = nullptr;
        } else if (bag->niceType() == NiceType::Forget) {
            // Forget bag.
            child = bag->children();

            if (tracker) {
                if (tracker->isCancelled())
                    break;
                tracker->newStage(
                    "Processing forget bag (" + std::to_string(index) +
                        '/' + std::to_string(nBags) + ')',
                    0.9 * HARD_BAG_WEIGHT(bag) / hardBagWeightSum);

                percent = 0;
                if (partial[child->index()]->empty())
                    increment = 0;
                else
                    increment = 100.0 / partial[child->index()]->size();
            }
#ifdef DUMP_STAGES
            else
                std::cerr << "FORGET -> 2 x " <<
                    partial[child->index()]->size() << std::endl;
#endif
            Crossing* forget = crossings_[child->element(bag->niceIndex())];

            // The A resolution connects strands conn[0][0][0-1], and
            // connects strands conn[0][1][0-1].
            // The A^{-1} resolution connects strands conn[1][0][0-1], and
            // connects strands conn[1][1][0-1].
            // The resolution conn[nodal][...] has nodal arrows pointing in
            // the direction conn[nodal][i][0 -> 1].  The other resolution has
            // no nodal arrows.
            StrandRef conn[2][2][2];
            int nodal;
            if (forget->sign() > 0) {
                // No nodal arrows, A resolution:
                conn[0][0][0] = forget->upper().prev();
                conn[0][0][1] = forget->lower();
                conn[0][1][0] = forget->lower().prev();
                conn[0][1][1] = forget->upper();
                // Nodal arrows, A^{-1} resolution:
                conn[1][0][0] = forget->upper().prev();
                conn[1][0][1] = forget->lower().prev();
                conn[1][1][0] = forget->upper();
                conn[1][1][1] = forget->lower();
                nodal = 1;
            } else {
                // Nodal arrows, A resolution:
                conn[0][0][0] = forget->lower().prev();
                conn[0][0][1] = forget->upper().prev();
                conn[0][1][0] = forget->lower();
                conn[0][1][1] = forget->upper();
                // No nodal arrows, A^{-1} resolution:
                conn[1][0][0] = forget->upper().prev();
                conn[1][0][1] = forget->lower();
                conn[1][1][0] = forget->lower().prev();
                conn[1][1][1] = forget->upper();
                nodal = 0;
            }

            int connIdx[2][2][2];
            int i, j, k;
            for (i = 0; i < 2; ++i)
                for (j = 0; j < 2; ++j)
                    for (k = 0; k < 2; ++k)
                        connIdx[i][j][k] = 2 * static_cast<int>(
                            conn[i][j][k].crossing()->index()) +
                            conn[i][j][k].strand();

            partial[index] = new SolnSet;

            for (auto& soln : *(partial[child->index()])) {
                if (tracker) {
                    percent += increment;
                    if (! tracker->setPercent(percent))
                        break;
                }

                const Key& kChild = soln.first;
                const Value& vChild = soln.second;

                // Adjust the key and value to reflect the fact the newly
                // forgotten crossing, under both possible resolutions.
                for (i = 0; i < 2; ++i) {
                    // i = 0: A resolution
                    // i = 1: A^{-1} resolution

                    Key kNew = kChild;

                    // The number of _pairs_ of nodal arrows on each of the
                    // one or two loops that we create (if any).
                    int newLoopPairs[2] = { -1, -1 };
                    for (j = 0; j < 2; ++j) {
                        // Connect strands conn[i][j][0-1].
                        if (kNew[connIdx[i][j][0]].first == -2 &&
                                kNew[connIdx[i][j][1]].first == -2) {
                            // Both strands stay in or above the bag.
                            if (connIdx[i][j][0] == connIdx[i][j][1]) {
                                // The two strands form a loop with no nodal
                                // arrows.  Bury them in the forgotten region.
                                kNew[connIdx[i][j][0]] = Dest(-1, 0);
                                kNew[connIdx[i][j][1]] = Dest(-1, 0);
                                if (nodal == i)
                                    throw ImpossibleScenario("Nodal arrow "
                                        "found in a 1-crossing loop");
                                if (newLoopPairs[0] < 0)
                                    newLoopPairs[0] = 0;
                                else
                                    newLoopPairs[1] = 0;
                            } else {
                                // The two strands go separate ways.
                                // Make them the endponts of a new path that
                                // enters and exits the forgotten region.
                                kNew[connIdx[i][j][0]].first = connIdx[i][j][1];
                                kNew[connIdx[i][j][1]].first = connIdx[i][j][0];
                                if (nodal == i) {
                                    kNew[connIdx[i][j][0]].second = 1;
                                    kNew[connIdx[i][j][1]].second = -1;
                                } else {
                                    kNew[connIdx[i][j][0]].second = 0;
                                    kNew[connIdx[i][j][1]].second = 0;
                                }
                            }
                        } else if (kNew[connIdx[i][j][0]].first == -2) {
                            // We cannot have one strand as -2 and the
                            // other as -1, since -2 means neither end
                            // of the strand is forgotten and -1 means
                            // both ends are forgotten.

                            // In this case we lengthen a section of the link
                            // that passes through the forgotten region.
                            kNew[connIdx[i][j][0]] = kNew[connIdx[i][j][1]];
                            kNew[kNew[connIdx[i][j][1]].first].first =
                                connIdx[i][j][0];
                            if (nodal == i) {
                                int arr = 1 - kNew[connIdx[i][j][0]].second;
                                kNew[connIdx[i][j][0]].second = arr;
                                kNew[kNew[connIdx[i][j][1]].first].second =
                                    (arr % 2 == 0 ? arr : -arr);
                            }
                            kNew[connIdx[i][j][1]] = Dest(-1, 0);
                        } else if (kNew[connIdx[i][j][1]].first == -2) {
                            // As before, we lengthen a section of the link
                            // that passes through the forgotten region.
                            kNew[connIdx[i][j][1]] = kNew[connIdx[i][j][0]];
                            kNew[kNew[connIdx[i][j][0]].first].first =
                                connIdx[i][j][1];
                            if (nodal == i) {
                                int arr = -(1 + kNew[connIdx[i][j][1]].second);
                                kNew[connIdx[i][j][1]].second = arr;
                                kNew[kNew[connIdx[i][j][0]].first].second =
                                    (arr % 2 == 0 ? arr : -arr);
                            }
                            kNew[connIdx[i][j][0]] = Dest(-1, 0);
                        } else {
                            // Both strands head down into the forgotten region.
                            if (kNew[connIdx[i][j][0]].first ==
                                    connIdx[i][j][1]) {
                                // We have closed off a loop.
                                int arr = kNew[connIdx[i][j][0]].second;
                                if (nodal == i)
                                    ++arr;
                                if (arr < 0)
                                    arr = -arr;
                                if (arr % 2)
                                    throw ImpossibleScenario("Loop found with "
                                        "an odd number of nodal arrows");
                                if (newLoopPairs[0] < 0)
                                    newLoopPairs[0] = arr >> 1;
                                else
                                    newLoopPairs[1] = arr >> 1;
                            } else {
                                // We connect two sections of the link
                                // that pass through the forgotten region.
                                kNew[kNew[connIdx[i][j][0]].first].first =
                                    kNew[connIdx[i][j][1]].first;
                                kNew[kNew[connIdx[i][j][1]].first].first =
                                    kNew[connIdx[i][j][0]].first;
                                int arr1 = kNew[kNew[connIdx[i][j][0]].first].
                                    second;
                                int arr2 = kNew[connIdx[i][j][1]].second;
                                if (nodal == i)
                                    arr2 = 1 - arr2;
                                if (arr1 % 2 == 0)
                                    arr1 += arr2;
                                else
                                    arr1 -= arr2;
                                kNew[kNew[connIdx[i][j][0]].first].second =
                                    arr1;
                                kNew[kNew[connIdx[i][j][1]].first].second =
                                    (arr1 % 2 == 0 ? arr1 : -arr1);
                            }
                            kNew[connIdx[i][j][0]] = Dest(-1, 0);
                            kNew[connIdx[i][j][1]] = Dest(-1, 0);
                        }
                    }

                    // We start at each leaf with the polynomial 1, which
                    // effectively adds one closed loop that we didn't have.
                    // So in the very last iteration (which is guaranteed to
                    // close off at least one loop), skip one factor of
                    // loopPoly to compensate.
                    Value vNew = vChild;
                    vNew.shift(i == 0 ? 1 : -1);
                    if (newLoopPairs[0] >= 0) {
                        if (index != nBags - 1)
                            vNew *= loopPoly;
                        if (newLoopPairs[0] > 0)
                            vNew.multDiagram(newLoopPairs[0]);
                    }
                    if (newLoopPairs[1] >= 0) {
                        vNew *= loopPoly;
                        if (newLoopPairs[1] > 0)
                            vNew.multDiagram(newLoopPairs[1]);
                    }

                    // Insert the new key/value into our partial
                    // solution, aggregating if need be.
                    auto existingSoln = partial[index]->try_emplace(
                        std::move(kNew), std::move(vNew));
                    if (! existingSoln.second)
                        existingSoln.first->second += vNew;
                }
            }

            delete partial[child->index()];
            partial[child->index()] = nullptr;
        } else {
            // Join bag.
            child = bag->children();
            sibling = child->sibling();

            if (tracker) {
                if (tracker->isCancelled())
                    break;
                tracker->newStage(
                    "Processing join bag (" + std::to_string(index) +
                        '/' + std::to_string(nBags) + ')',
                    0.9 * HARD_BAG_WEIGHT(bag) / hardBagWeightSum);

                percent = 0;
                if (partial[child->index()]->empty())
                    increment = 0;
                else
                    increment = 100.0 / partial[child->index()]->size();
            }
#ifdef DUMP_STAGES
            else
                std::cerr << "JOIN -> " <<
                    partial[child->index()]->size() << " x " <<
                    partial[sibling->index()]->size() << std::endl;
#endif
            partial[index] = new SolnSet;

            for (auto& soln1 : *(partial[child->index()])) {
                if (tracker) {
                    percent += increment;
                    if (! tracker->setPercent(percent))
                        break;
                }

                for (auto& soln2 : *(partial[sibling->index()])) {
                    // Combine the two child keys and values.
                    Key kNew(nStrands);
                    for (size_t strand = 0; strand < nStrands; ++strand)
                        if (soln1.first[strand].first == -2)
                            kNew[strand] = soln2.first[strand];
                        else if (soln2.first[strand].first == -2)
                            kNew[strand] = soln1.first[strand];
                        else
                            throw ImpossibleScenario(
                                "Incompatible keys in join bag");

                    if (! partial[index]->emplace(std::move(kNew),
                            soln1.second * soln2.second).second)
                        throw ImpossibleScenario(
                            "Combined keys in join bag are not unique");
                }
            }

            delete partial[child->index()];
            delete partial[sibling->index()];
            partial[child->index()] = partial[sibling->index()] = nullptr;
        }
    }

    if (tracker && tracker->isCancelled()) {
        // We don't know which elements of partial[] have been
        // deallocated, so check them all.
        for (size_t i = 0; i < nBags; ++i)
            delete partial[i];
        return {};
    }

    // Collect the final answer from partial[nBags - 1].
#ifdef DUMP_STAGES
    if (! tracker)
        std::cerr << "FINISH" << std::endl;
#endif
    Value ans = std::move(partial[nBags - 1]->begin()->second);

    delete partial[nBags - 1];

    // Normalise the polynomial using the writhe of the diagram.
    long w = writhe();
    ans.shift(-3 * w);
    if (w % 2)
        ans.negate();

    // Finally, factor in any zero-crossing components.
    for (StrandRef s : components_)
        if (! s)
            ans *= loopPoly;

    return ans;
}

const Arrow& Link::arrow(Algorithm alg, int threads, ProgressTracker* tracker)
        const {
    if (arrow_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *arrow_;
    }

    if (size() > (INT_MAX >> 1))
        throw NotImplemented("This link has so many crossings that the total "
            "number of strands cannot fit into a native C++ signed int");

    Arrow ans;
    switch (alg) {
        case Algorithm::Naive:
            ans = arrowNaive(threads, tracker);
            break;
        default:
            ans = arrowTreewidth(tracker);
            break;
    }

    if (tracker && tracker->isCancelled()) {
        tracker->setFinished();
        return noResult;
    }

    arrow_ = std::move(ans);

    // The Kauffman bracket and Jones polynomial are easy to deduce from the
    // arrow polynomial; however, we won't do the (trivial) computation until
    // someone asks for it, since caching the result takes up space.

    if (tracker)
        tracker->setFinished();
    return *arrow_;
}

} // namespace regina

