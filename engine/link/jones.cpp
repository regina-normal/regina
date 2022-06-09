
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

#include "link/link.h"
#include "maths/laurent.h"
#include "progress/progresstracker.h"
#include "utilities/bitmanip.h"
#include "utilities/sequence.h"
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
     * Used as a return value when the Jones/bracket calculation is running in
     * a new thread and we need to return immediately without a result.
     */
    const regina::Laurent<regina::Integer> noResult;
}

size_t Link::resolutionLoops(uint64_t mask, size_t* loopIDs,
        size_t* loopLengths) const {
    size_t n = crossings_.size();

    size_t pos;
    int dirInit, dir;
    StrandRef s;

    // found[0..n-1] : seen the half of the upper strand that exits the crossing
    // found[n..2n-1] : seen the half of the upper strand that enters the crossing
    bool* found = new bool[2 * n];
    std::fill(found, found + 2 * n, false);

    size_t loops = 0;
    size_t len;

    // The following two loops iterate through indices of found[] in
    // increasing order.
    for (dirInit = 0; dirInit < 2; ++dirInit) {
        for (pos = 0; pos < n; ++pos) {
            // dirInit: 1 = with arrows, 0 = against arrows.
            // This refers to the direction along the strand as you
            // approach the crossing (before you jump to the other strand).
            if (! found[pos + (dirInit ? n : 0)]) {
                //std::cerr << "LOOP\n";
                if (loopIDs)
                    loopIDs[loops] = pos + (dirInit ? n : 0);

                s = crossings_[pos]->upper();
                dir = dirInit;
                len = 0;

                do {
                    //std::cerr << "At: " << s <<
                    //    (dir == 1 ? " ->" : " <-") << std::endl;
                    const uint64_t bit = (uint64_t)1 << s.crossing()->index();

                    if (    ((mask & bit) && s.crossing()->sign() < 0) ||
                            ((mask & bit) == 0 && s.crossing()->sign() > 0)) {
                        // Turn in a way that is consistent with the arrows.
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
                        // Turn in a way that is inconsistent with the arrows.
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

    delete[] found;
    return loops;
}

Laurent<Integer> Link::bracketNaive(ProgressTracker* tracker) const {
    /**
     * \ /         \ /            \_/
     *  /   ->   A | |   +   A^-1  _
     * / \         / \            / \
     *
     * O^k  ->  (-A^2 - A^-2)^(k-1)
     */

    if (components_.size() == 0)
        return Laurent<Integer>();

    size_t n = crossings_.size();
    if (n >= 64) {
        // We cannot use the backtracking algorithm, since our bitmask
        // type (uint64_t) does not contain enough bits.
        return bracketTreewidth(tracker);
    }

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    // How many zero-crossing components do we start with?
    size_t initLoops = 0;
    for (StrandRef s : components_)
        if (! s)
            ++initLoops;

    // In count[i-1], the coefficient of A^k reflects the number of
    // resolutions with i loops and multiplier A^k.
    // We will always have 1 <= i <= #components + #crossings.
    auto* count = new Laurent<Integer>[n + components_.size()];

    size_t maxLoops = 0;

    static_assert(BitManipulator<uint64_t>::specialised,
        "BitManipulator is not specialised for the mask type.");

    if (tracker)
        tracker->newStage("Enumerating resolutions");

    size_t loops;
    long shift;
    const uint64_t maskEnd = ((uint64_t)1 << n);
    for (uint64_t mask = 0; mask != maskEnd; ++mask) {
        // std::cerr << "Mask: " << mask << std::endl;

        // Check for cancellation every 1024 steps.
        if (tracker && ((mask & 1023) == 0)) {
            if (! tracker->setPercent(double(mask) * 100.0 / double(maskEnd)))
                break;
        }

        loops = initLoops + resolutionLoops(mask);
        if (loops > maxLoops)
            maxLoops = loops;

        --loops;

        // Set shift = #(0 bits) - #(1 bits) in mask.
        shift = n - 2 * BitManipulator<uint64_t>::bits(mask);
        if (shift > count[loops].maxExp() || shift < count[loops].minExp())
            count[loops].set(shift, 1);
        else
            count[loops].set(shift, count[loops][shift] + 1);
    }

    if (tracker && tracker->isCancelled()) {
        delete[] count;
        return Laurent<Integer>();
    }

    Laurent<Integer> ans;

    Laurent<Integer> loopPoly;
    loopPoly.set(0, -1);
    loopPoly.set(4, -1);
    loopPoly.shift(-2);

    Laurent<Integer> loopPow(0); // Initialises to x^0 == 1.
    for (loops = 0; loops < maxLoops; ++loops) {
        // std::cerr << "count[" << loops << "] = " << count[loops] << std::endl;
        if (! count[loops].isZero()) {
            count[loops] *= loopPow;
            ans += count[loops];
        }

        loopPow *= loopPoly;
    }

    delete[] count;
    return ans;
}

Laurent<Integer> Link::bracketTreewidth(ProgressTracker* tracker) const {
    if (crossings_.empty())
        return bracketNaive(tracker);

    // We are guaranteed >= 1 crossing and >= 1 component.

    Laurent<Integer> loopPoly;
    loopPoly.set(0, -1);
    loopPoly.set(4, -1);
    loopPoly.shift(-2);

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
            switch (bag->type()) {
                case NICE_FORGET:
                case NICE_JOIN:
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

    auto* partial = new SolnSet*[nBags];
    std::fill(partial, partial + nBags, nullptr);

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
                Laurent<Integer>(0) /* x^0 = 1 */);
        } else if (bag->type() == NICE_INTRODUCE) {
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
        } else if (bag->type() == NICE_FORGET) {
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
            Crossing* forget = crossings_[child->element(bag->subtype())];

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
                    if (! tracker->setPercent(percent)) {
                        // In normal processing, the loop through solutions
                        // deletes the child keys and values as it goes.
                        // Therefore we need to finish the loop to ensure that
                        // all remaining child keys and values are deleted,
                        // even if we do not want to process them.
                        //
                        // TODO: Now that keys and values are stored by
                        // value, not by pointer, do we still need this?
                        continue;
                    }
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
                            std::cerr <<
                                "ERROR: Incompatible keys in join bag"
                                << std::endl;

                    if (! partial[index]->emplace(std::move(kNew),
                            soln1.second * soln2.second).second)
                        std::cerr << "ERROR: Combined keys in join bag "
                            "are not unique" << std::endl;
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
        delete[] partial;
        return Value();
    }

    // Collect the final answer from partial[nBags - 1].
#ifdef DUMP_STAGES
    if (! tracker)
        std::cerr << "FINISH" << std::endl;
#endif
    Value ans = std::move(partial[nBags - 1]->begin()->second);

    delete partial[nBags - 1];
    delete[] partial;

    // Finally, factor in any zero-crossing components.
    for (StrandRef s : components_)
        if (! s)
            ans *= loopPoly;

    return ans;
}

const Laurent<Integer>& Link::bracket(Algorithm alg, ProgressTracker* tracker)
        const {
    if (bracket_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *bracket_;
    }

    if (size() > (INT_MAX >> 1))
        throw NotImplemented("This link has so many crossings that "
            "the largest strand ID cannot fit into a native C++ int");

    Laurent<Integer> ans;
    switch (alg) {
        case ALG_NAIVE:
            ans = bracketNaive(tracker);
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

const Laurent<Integer>& Link::jones(Algorithm alg, ProgressTracker* tracker)
        const {
    if (jones_.has_value()) {
        if (tracker)
            tracker->setFinished();
        return *jones_;
    }

    // Computing bracket_ will also set jones_.
    bracket(alg, tracker); // this marks tracker as finished
    if (tracker && tracker->isCancelled())
        return noResult;
    return *jones_;
}

void Link::setPropertiesFromBracket(Laurent<Integer>&& bracket) const {
    bracket_ = std::move(bracket);

    // Convert bracket into jones:
    // (-A^3)^(-w) * bracket, then multiply all exponents by -1/4.
    Laurent<Integer> jones(*bracket_);
    long w = writhe();
    jones.shift(-3 * w);
    if (w % 2)
        jones.negate();

    // We only scale exponents by -1/2, since we are returning a Laurent
    // polynomial in sqrt(t).
    jones.scaleDown(-2);
    jones_ = std::move(jones);
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

