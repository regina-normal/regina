
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

/**
 * Bracket skein relation:
 *
 * \ /         \ /            \_/
 *  /   ->   A | |   +   A^-1  _
 * / \         / \            / \
 *
 * O^k  ->  (-A^2 - A^-2)^(k-1)
 */

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
     * Used as a return value when the Jones/bracket calculation has been
     * cancelled.
     */
    const regina::Laurent<regina::Integer> noResult;

    /**
     * Internal to bracketNaive().
     *
     * This function returns the number of loops in the given link that are
     * produced by resolving each crossing according to the given bitmask:
     *
     * - If the <i>i</i>th bit in \a mask is 0, crossing \a i should be
     *   resolved by turning _left_ when entering along the upper strand.
     *
     * - If the <i>i</i>th bit in \a mask is 1, crossing \a i should be
     *   resolved by turning _right_ when entering along the upper strand.
     *
     * If the array \a loopIDs is non-null, then it will be filled with an
     * identifier for each loop.  Each identifier will be the minimum of the
     * following values that are computed as you follow the loop: when passing
     * through crossing \a i, if we encounter the half of the upper strand that
     * _exits_ the crossing then we take the value `i`, and if we encounter the
     * half of the upper strand that _enters_ the crossing then we take the
     * value `i + n`.  These identifiers will be returned in the array
     * \a loopIDs in sorted order.
     *
     * If the array \a loopLengths is non-null, then it will be filled with the
     * number of strands in each loop (so these should sum to twice the number
     * of crossings).  These loop lengths will be placed in the array in the
     * same order as the loop IDs as described above.
     *
     * \pre `link.size() < 64` (here 64 is the length of the bitmask type).
     *
     * \pre If either or both arrays \a loopIDs and \a loopLengths are not null,
     * then they are arrays whose size is at least the return value (i.e., the
     * number of loops).  This typically means that the caller must put an upper
     * bound on the number of loops in advance, before calling this routine.
     */
    size_t resolutionLoops(const Link& link, uint64_t mask,
            size_t* loopIDs = nullptr, size_t* loopLengths = nullptr) {
        size_t n = link.size();

        // Here we store whether we have seen the half of the upper strand
        // at each crossing...
        // found[0..n) : ... that exits the crossing
        // found[n..2n) : ... that enters the crossing
        FixedArray<bool> found(2 * n, false);

        size_t loops = 0;

        // The following two loops iterate through indices of found[] in
        // increasing order.
        for (int dirInit = 0; dirInit < 2; ++dirInit) {
            for (size_t pos = 0; pos < n; ++pos) {
                // dirInit: 1 = with arrows, 0 = against arrows.
                // This refers to the direction along the strand as you
                // approach the crossing (before you jump to the other strand).
                if (! found[pos + (dirInit ? n : 0)]) {
                    //std::cerr << "LOOP\n";
                    if (loopIDs)
                        loopIDs[loops] = pos + (dirInit ? n : 0);

                    StrandRef s = link.crossing(pos)->upper();
                    int dir = dirInit;
                    size_t len = 0;

                    do {
                        //std::cerr << "At: " << s <<
                        //    (dir == 1 ? " ->" : " <-") << std::endl;
                        const uint64_t bit =
                            uint64_t(1) << s.crossing()->index();

                        if (    ((mask & bit) && s.crossing()->sign() < 0) ||
                                ((mask & bit) == 0 && s.crossing()->sign() > 0)) {
                            // Turn in a way consistent with the arrows.
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
                            if (dir == 1) {
                                found[s.crossing()->index() + n] = true;
                                s = s.crossing()->prev(s.strand() ^ 1);
                            } else {
                                found[s.crossing()->index()] = true;
                                s = s.crossing()->next(s.strand() ^ 1);
                            }
                            dir ^= 1;
                        }

                        ++len;
                    } while (! (dir == dirInit &&
                        s.crossing()->index() == pos && s.strand() == 1));

                    if (loopLengths)
                        loopLengths[loops] = len;
                    ++loops;
                }
            }
        }

        return loops;
    }

    /**
     * Computes a partial sum in the naive algorithm for a subset of possible
     * resolutions.  This is used by bracketNaive(), and is designed to support
     * multithreading - each thread uses its own BracketAccumulator, and works
     * over a different subset of resolutions.
     */
    class BracketAccumulator {
        private:
            const Link& link_;

            // The number of trivial zero-crossing unknot components.
            const size_t trivialLoops_;

            // In count[i-1], the coefficient of A^k reflects the number of
            // resolutions with i loops and multiplier A^k.
            //
            // Note: we will always have 1 <= i <= #components + #crossings.
            FixedArray<Laurent<Integer>> count_;

            // The largest number of loops that this accumulator has seen.
            // It is guaranteed that count_[i] == 0 for all i >= maxLoops_.
            size_t maxLoops_;

        public:
            BracketAccumulator(const Link& link, size_t trivialLoops) :
                    link_(link), trivialLoops_(trivialLoops),
                    count_(link.size() + link.countComponents()), maxLoops_(0) {
            }

            void accumulate(uint64_t maskBegin, uint64_t maskEnd) {
                for (uint64_t mask = maskBegin; mask != maskEnd; ++mask) {
                    size_t loops = trivialLoops_ + resolutionLoops(link_, mask);
                    if (loops > maxLoops_)
                        maxLoops_ = loops;

                    --loops;

                    // Set shift = #(0 bits) - #(1 bits) in mask.
                    long shift = link_.size() -
                        2 * BitManipulator<uint64_t>::bits(mask);

                    if (shift > count_[loops].maxExp() ||
                            shift < count_[loops].minExp())
                        count_[loops].set(shift, 1);
                    else
                        count_[loops].set(shift, count_[loops][shift] + 1);
                }
            }

            /**
             * Precondition: this and \a other use the same link, which in
             * particular means that their internal \a count_ arrays have the
             * same size.
             */
            void accumulate(BracketAccumulator&& other) {
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

            Laurent<Integer> finalise() {
                Laurent<Integer> ans;

                Laurent<Integer> loopPow = RingTraits<Laurent<Integer>>::one;
                for (size_t loops = 0; loops < maxLoops_; ++loops) {
                    // std::cerr << "count[" << loops << "] = "
                    //     << count[loops] << std::endl;
                    if (! count_[loops].isZero()) {
                        count_[loops] *= loopPow;
                        ans += count_[loops];
                    }

                    loopPow *= loopPoly;
                }

                return ans;
            }
    };
}

Laurent<Integer> Link::bracketNaive(int threads, ProgressTracker* tracker)
        const {
    if (components_.empty())
        return {};

    size_t n = crossings_.size();
    if (n >= 64) {
        // We cannot use the naive algorithm, since our bitmask
        // type (uint64_t) does not contain enough bits.
        return bracketTreewidth(tracker);
    }

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    if (tracker)
        tracker->newStage("Enumerating resolutions");

    size_t nTrivial = countTrivialComponents();
    BracketAccumulator acc(*this, nTrivial);
    if (threads <= 1 || n <= sliceBits) {
        acc.accumulate(0, uint64_t(1) << n);
    } else {
        uint64_t nextSlice = 0;
        uint64_t endSlice = (uint64_t(1) << (n - sliceBits));
        std::mutex mutex;
        FixedArray<std::thread> thread(threads);
        for (int i = 0; i < threads; ++i) {
            thread[i] = std::thread([=, this, &mutex, &nextSlice, &acc]() {
                BracketAccumulator sub(*this, nTrivial);
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

    if (tracker && tracker->isCancelled()) {
        return {};
    }

    return acc.finalise();
}

Laurent<Integer> Link::bracketTreewidth(ProgressTracker* tracker) const {
    if (crossings_.empty())
        return bracketNaive(1 /* single-threaded */, tracker);

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
    // The key is stored as a sequence x[0 .. 2n-1], where
    // - if strand k is being paired off then x[k] is its partner strand;
    // - if strand k connects two forgotten crossings then x[k] = -1;
    // - otherwise x[k] = -2.
    //
    // Each value is a Laurent polynomial, which is essentially a
    // partially computed bracket polynomial that accounts for those
    // crossings that have already been forgotten.
    //
    // We ignore any 0-crossing unknot components throughout this
    // calculation, and only factor them in at the very end when we
    // extract the final bracket polynomial.
    //
    // We will be using ints for strand IDs, since we will be storing
    // exponentially many keys in our key-value map and so space is at a
    // premium.  Having strand IDs that fit into an int is enforced through
    // our preconditions.

    size_t nStrands = 2 * size();
    size_t loops;

    using Key = LightweightSequence<int>;
    using Value = Laurent<Integer>;
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
            std::fill(k.begin(), k.end(), -2);

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
            StrandRef conn[2][2][2];
            conn[0][0][0] = conn[1][0][0] = forget->upper().prev();
            if (forget->sign() > 0) {
                conn[0][0][1] = conn[1][1][0] = forget->lower();
                conn[0][1][0] = conn[1][0][1] = forget->lower().prev();
            } else {
                conn[0][0][1] = conn[1][1][0] = forget->lower().prev();
                conn[0][1][0] = conn[1][0][1] = forget->lower();
            }
            conn[0][1][1] = conn[1][1][1] = forget->upper();

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

                    size_t newLoops = 0;
                    for (j = 0; j < 2; ++j) {
                        // Connect strands conn[i][j][0-1].
                        if (kNew[connIdx[i][j][0]] == -2 &&
                                kNew[connIdx[i][j][1]] == -2) {
                            // Both strands stay in or above the bag.
                            if (connIdx[i][j][0] == connIdx[i][j][1]) {
                                // The two strands form a loop.
                                // Bury them in the forgotten region.
                                kNew[connIdx[i][j][0]] = -1;
                                kNew[connIdx[i][j][1]] = -1;
                                ++newLoops;
                            } else {
                                // The two strands go separate ways.
                                // Make them the endponts of a new path that
                                // enters and exits the forgotten region.
                                kNew[connIdx[i][j][0]] = connIdx[i][j][1];
                                kNew[connIdx[i][j][1]] = connIdx[i][j][0];
                            }
                        } else if (kNew[connIdx[i][j][0]] == -2) {
                            // We cannot have one strand as -2 and the
                            // other as -1, since -2 means neither end
                            // of the strand is forgotten and -1 means
                            // both ends are forgotten.

                            // In this case we lengthen a section of the link
                            // that passes through the forgotten region.
                            kNew[connIdx[i][j][0]] = kNew[connIdx[i][j][1]];
                            kNew[kNew[connIdx[i][j][1]]] = connIdx[i][j][0];
                            kNew[connIdx[i][j][1]] = -1;
                        } else if (kNew[connIdx[i][j][1]] == -2) {
                            // As before, we lengthen a section of the link
                            // that passes through the forgotten region.
                            kNew[connIdx[i][j][1]] = kNew[connIdx[i][j][0]];
                            kNew[kNew[connIdx[i][j][0]]] = connIdx[i][j][1];
                            kNew[connIdx[i][j][0]] = -1;
                        } else {
                            // Both strands head down into the forgotten region.
                            if (kNew[connIdx[i][j][0]] == connIdx[i][j][1]) {
                                // We have closed off a loop.
                                ++newLoops;
                            } else {
                                // We connect two sections of the link
                                // that pass through the forgotten region.
                                kNew[kNew[connIdx[i][j][0]]] =
                                    kNew[connIdx[i][j][1]];
                                kNew[kNew[connIdx[i][j][1]]] =
                                    kNew[connIdx[i][j][0]];
                            }
                            kNew[connIdx[i][j][0]] = -1;
                            kNew[connIdx[i][j][1]] = -1;
                        }
                    }

                    // We start at each leaf with the polynomial 1,
                    // which effectively adds one closed loop that we
                    // didn't have.  So in the very last iteration (which
                    // is guaranteed to close off at least one loop),
                    // subtract one closed loop to compensate.
                    if (index == nBags - 1)
                        --newLoops;

                    Value vNew = vChild;
                    vNew.shift(i == 0 ? 1 : -1);
                    for (loops = 0; loops < newLoops; ++loops)
                        vNew *= loopPoly;

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
                        if (soln1.first[strand] == -2)
                            kNew[strand] = soln2.first[strand];
                        else if (soln2.first[strand] == -2)
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

    // Finally, factor in any zero-crossing components.
    for (StrandRef s : components_)
        if (! s)
            ans *= loopPoly;

    return ans;
}

const Laurent<Integer>& Link::bracket(Algorithm alg, int threads,
        ProgressTracker* tracker) const {
    if (bracket_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *bracket_;
    }

    if (arrow_.has_value()) {
        // It is trivial to deduce the Kauffman bracket and Jones polynomial
        // from the arrow polynomial.
        bracket_ = arrow_->sumLaurent();

        // Set jones_ while the Kauffman bracket is still normalised.
        jones_ = bracket_;
        jones_->scaleDown(-2);

        // Now de-normalise the Kauffman bracket.
        long w = writhe();
        bracket_->shift(3 * w);
        if (w % 2)
            bracket_->negate();

        if (tracker)
            tracker->setFinished();
        return *bracket_;
    }

    if (size() > (INT_MAX >> 1))
        throw NotImplemented("This link has so many crossings that the total "
            "number of strands cannot fit into a native C++ signed int");

    Laurent<Integer> ans;
    switch (alg) {
        case Algorithm::Naive:
            ans = bracketNaive(threads, tracker);
            break;
        default:
            ans = bracketTreewidth(tracker);
            break;
    }

    if (tracker && tracker->isCancelled()) {
        tracker->setFinished();
        return noResult;
    }

    setPropertiesFromBracket(std::move(ans));

    if (tracker)
        tracker->setFinished();
    return *bracket_;
}

const Laurent<Integer>& Link::jones(Algorithm alg, int threads,
        ProgressTracker* tracker) const {
    if (jones_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *jones_;
    }

    // Computing bracket_ will also set jones_.
    bracket(alg, threads, tracker); // this marks tracker as finished
    if (tracker && tracker->isCancelled())
        return noResult;
    return *jones_;
}

void Link::setPropertiesFromBracket(Laurent<Integer>&& bracket) const {
    bracket_ = std::move(bracket);

    // Normalise the bracket using the writhe: multiply by (-A^3)^(-w).
    Laurent<Integer> normalised(*bracket_);
    long w = writhe();
    normalised.shift(-3 * w);
    if (w % 2)
        normalised.negate();

    if ((! arrow_.has_value()) && isClassical()) {
        // The arrow polynomial for a _classical_ link is just the normalised
        // bracket.
        arrow_ = normalised;
    }

    // The Jones polynomial is obtained from the normalised bracket by
    // multiplying all exponents by -1/4.
    //
    // We only scale exponents by -1/2, since we are returning a Laurent
    // polynomial in sqrt(t).
    normalised.scaleDown(-2);
    jones_ = std::move(normalised);
}

void Link::optimiseForJones(TreeDecomposition& td) const {
    td.compress();
    if (td.size() <= 1)
        return;

    // In order to estimate processing costs, we need to be able to
    // query whether a given node appears in a given subtree.
    // Do some preprocessing to make these queries constant time.
    //
    // For crossing i, crossingSubtree[i] will contain the highest index
    // bag that contains that crossing.
    //
    // For bag j, subtreeStart[j] will contain the lowest index bag
    // within the subtree rooted at bag j (including j itself).
    // Due to our leaf-to-root indexing, it follows that the subtree
    // rooted at j contains precisely those bags with indices k for
    // which subtreeStart[j] <= k <= j.

    auto* crossingSubtree = new size_t[size()];
    auto* subtreeStart = new size_t[td.size()];

    const TreeBag* b;
    for (b = td.first(); b; b = b->next())
        if (b->children())
            subtreeStart[b->index()] = subtreeStart[b->children()->index()];
        else
            subtreeStart[b->index()] = b->index();

    for (b = td.first(); b; b = b->next())
        for (size_t i = 0; i < b->size(); ++i)
            crossingSubtree[b->element(i)] = b->index();

    // Now we can build our cost estimates.

    auto* costSame = new size_t[td.size()];
    auto* costReverse = new size_t[td.size()];
    auto* costRoot = new size_t[td.size()];

    // For a bag b:
    //
    // costRoot: Count strands from crossings in b to crossings not in b.
    //
    // costSame: Count strands from crossings in b to crossings not in b,
    // but in one of b's descendants.
    //
    // costReverse: Count strands from crossings in b->parent to crossings
    // not in b->parent and not in b or any of b's descendants.

    std::fill(costSame, costSame + td.size(), 0);
    std::fill(costReverse, costReverse + td.size(), 0);
    std::fill(costRoot, costRoot + td.size(), 0);

    Crossing *c;
    size_t adj, adjRoot;
    for (b = td.first(); b; b = b->next()) {
        for (size_t i = 0; i < b->size(); ++i) {
            c = crossings_[b->element(i)];
            for (int p = 0; p < 2; ++p)
                for (int q = 0; q < 2; ++q) {
                    adj = (p == 0 ? c->prev(q).crossing() :
                        c->next(q).crossing())->index();
                    if (! b->contains(adj)) {
                        // We have a strand from a crossing in b that
                        // leads to a crossing not in b.
                        ++costRoot[b->index()];

                        // Is adj buried within b's descendants?
                        adjRoot = crossingSubtree[adj];
                        if (adjRoot >= subtreeStart[b->index()] &&
                                adjRoot < b->index())
                            ++costSame[b->index()];
                    }
                }
        }

        if (b->parent()) {
            for (size_t i = 0; i < b->parent()->size(); ++i) {
                c = crossings_[b->parent()->element(i)];
                for (int p = 0; p < 2; ++p)
                    for (int q = 0; q < 2; ++q) {
                        adj = (p == 0 ? c->prev(q).crossing() :
                            c->next(q).crossing())->index();
                        if (! b->parent()->contains(adj)) {
                            // We have a strand from a crossing in b's parent
                            // that leads to a crossing not in b's parent.

                            // Is adj *not* buried within b or its descendants?
                            adjRoot = crossingSubtree[adj];
                            if (! (adjRoot >= subtreeStart[b->index()] &&
                                    adjRoot <= b->index()))
                                ++costReverse[b->index()];
                        }
                    }
            }
        }
    }

    delete[] subtreeStart;
    delete[] crossingSubtree;

    td.reroot(costSame, costReverse, costRoot);

    delete[] costSame;
    delete[] costReverse;
    delete[] costRoot;
}

} // namespace regina

