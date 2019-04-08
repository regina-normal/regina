
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

#include "link/link.h"
#include "maths/laurent2.h"
#include "utilities/bitmanip.h"
#include "utilities/sequence.h"

// #define DUMP_STATES

namespace regina {

const char* Link::homflyAZVarX = "\u03B1"; // alpha
const char* Link::homflyAZVarY = "z";

const char* Link::homflyLMVarX = "\U0001D4C1"; // mathematical script small l
const char* Link::homflyLMVarY = "m";

const char* Link::homflyVarX = homflyAZVarX;
const char* Link::homflyVarY = homflyAZVarY;

namespace {
    /**
     * Possible states of crossings.  Used by Kauffman's algorithm.
     */
    enum CrossingState {
        /**
         * Not yet visited.  Moreover, this state indicates that - if there is
         * a decision to make - we should first attempt to switch this crossing.
         */
        CROSSING_UNSEEN = 0,

        /**
         * Not yet visited.  Moreover, this state indicates that - if there
         * is a decision to make - we have already attempted switching the
         * crossing, and we should now try to splice instead.
         */
        CROSSING_TRIED = 1,

        /**
         * First seen on the upper strand, and so the crossing was kept intact.
         * Visited only once so far.
         */
        CROSSING_KEEP_1 = 2,

        /**
         * First seen on the upper strand, and so the crossing was kept intact.
         * Visited twice (first upper, then lower).
         */
        CROSSING_KEEP_2 = 3,

        /**
         * First seen on the lower strand, and the decision was made to
         * switch the crossing.  Visited only once so far.
         */
        CROSSING_SWITCH_1 = 4,

        /**
         * First seen on the lower strand, and the decision was made to
         * switch the crossing.  Visited twice.
         */
        CROSSING_SWITCH_2 = 5,

        /**
         * First seen on the lower strand, and the decision was made to
         * splice.  Visited only once so far.
         */
        CROSSING_SPLICE_1 = 6,

        /**
         * First seen on the lower strand, and the decision was made to
         * splice.  Visited twice.
         */
        CROSSING_SPLICE_2 = 7
    };

    /**
     * Helper data used by the treewidth-based algorithm to test whether a
     * key is viable.  In other words, this tests whether the data from
     * a given key \e might survive all the way up to the root of the
     * tree decomposition.
     */
    struct ViabilityData {
        const Link* link;

        /**
         * An array that gives the bag index at which each crossing is
         * eventually forgotten.
         *
         * It is assumed that the underlying tree decomposition is nice.
         *
         * This array is a function of the link only, and is initialised
         * in the ViabilityData constructor.
         */
        int* forgetCrossing;

        /**
         * An array that, for each strand ID, records which of the two
         * crossings is forgotten last.  This array stores crossing IDs.
         *
         * This array is a function of the link only, and is initialised
         * in the ViabilityData constructor.
         */
        int* lastCrossing;

        /**
         * An array that, for each strand ID, records the bag index at
         * which the entire strand is forgotten.
         *
         * This array is a function of the link only, and is initialised
         * in the ViabilityData constructor.
         */
        int* forgetStrand;

        /**
         * For a crossing at index i that lives in the current bag,
         * mask[i] is a bitwise combination of:
         *
         * * 1 if the lower incoming strand comes from the forgotten zone;
         * * 2 if the upper incoming strand comes from the forgotten zone;
         * * 4 if the lower outgoing strand goes into the forgotten zone;
         * * 8 if the upper outgoing strand goes into the forgotten zone.
         *
         * This array is a function of the bag being processed, and is
         * initialised by initForgetBag() and initJoinBag().
         */
        char* mask;

        int *startLoop;

        ViabilityData(const Link* l, const TreeDecomposition& d) :
                link(l),
                forgetCrossing(new int[l->size()]),
                lastCrossing(new int[2 * l->size()]),
                forgetStrand(new int[2 * l->size()]),
                startLoop(new int[l->size()]),
                mask(new char[l->size()]) {
            for (const TreeBag* b = d.first(); b; b = b->next())
                if (b->type() == NICE_FORGET)
                    forgetCrossing[b->children()->element(b->subtype())] =
                        b->index();

            int from, to;
            for (int i = 0; i < 2 * l->size(); ++i) {
                from = i / 2;
                to = l->crossing(from)->next(i % 2).crossing()->index();
                if (forgetCrossing[from] >= forgetCrossing[to]) {
                    lastCrossing[i] = from;
                    forgetStrand[i] = forgetCrossing[from];
                } else {
                    lastCrossing[i] = to;
                    forgetStrand[i] = forgetCrossing[to];
                }
            }
        }

        ~ViabilityData() {
            delete[] mask;
            delete[] startLoop;
            delete[] forgetStrand;
            delete[] lastCrossing;
            delete[] forgetCrossing;
        }

        void initForgetBag(const TreeBag* bag,
                const LightweightSequence<int>* childKey,
                const Crossing* forget) {
            std::fill(mask, mask + link->size(), 0);

            // Identify all strands where one crossing is forgotten and
            // the other is not.
            int strandID;
            StrandRef from, to;
            for (int i = 0; i < childKey->size(); ++i) {
                strandID = (*childKey)[i];

                // In the child bag, this strand ran between the bag and the
                // forgotten zone.

                from = StrandRef(link->crossing(strandID / 2), strandID % 2);
                to = from.next();

                if (from.crossing() == forget || to.crossing() == forget) {
                    // The entire strand is lost in this (the parent bag).
                    continue;
                }

                // The strand survives through to this bag.
                if (lastCrossing[strandID] == from.crossing()->index()) {
                    // The strand runs from the bag into the forgotten zone.
                    if (from.strand() == 0)
                        mask[from.crossing()->index()] |= 4;
                    else
                        mask[from.crossing()->index()] |= 8;
                } else {
                    // The strand runs from the forgotten zone into the bag.
                    if (to.strand() == 0)
                        mask[to.crossing()->index()] |= 1;
                    else
                        mask[to.crossing()->index()] |= 2;
                }
            }

            // We also need to collect strands that run between the
            // newly-forgotten crossing and the bag.

            for (int i = 0; i < 2; ++i) {
                // From newly-forgotten crossing into the bag:
                to = forget->next(i);
                if (forgetCrossing[to.crossing()->index()] > bag->index()) {
                    if (to.strand() == 0)
                        mask[to.crossing()->index()] |= 1;
                    else
                        mask[to.crossing()->index()] |= 2;
                }

                // From the bag into the newly-forgotten crossing:
                from = forget->prev(i);
                if (forgetCrossing[from.crossing()->index()] > bag->index()) {
                    if (from.strand() == 0)
                        mask[from.crossing()->index()] |= 4;
                    else
                        mask[from.crossing()->index()] |= 8;
                }
            }
        }

        void initJoinBag(const LightweightSequence<int>* leftChildKey,
                const LightweightSequence<int>* rightChildKey) {
            std::fill(mask, mask + link->size(), 0);

            // Identify all strands where one crossing is forgotten and
            // the other is not.
            const LightweightSequence<int>* childKey;
            int strandID;
            StrandRef from, to;
            int side, i;
            for (side = 0; side < 2; ++side) {
                childKey = (side == 0 ? leftChildKey : rightChildKey);
                for (i = 0; i < childKey->size(); ++i) {
                    strandID = (*childKey)[i];

                    // This strand runs between the bag and the forgotten zone.

                    from = StrandRef(link->crossing(strandID / 2),
                        strandID % 2);
                    to = from.next();

                    if (lastCrossing[strandID] == from.crossing()->index()) {
                        // The strand runs from the bag into the forgotten zone.
                        if (from.strand() == 0)
                            mask[from.crossing()->index()] |= 4;
                        else
                            mask[from.crossing()->index()] |= 8;
                    } else {
                        // The strand runs from the forgotten zone into the bag.
                        if (to.strand() == 0)
                            mask[to.crossing()->index()] |= 1;
                        else
                            mask[to.crossing()->index()] |= 2;
                    }
                }
            }
        }

        void analyseLoops(LightweightSequence<int>* key,
                int fromPos, int maxForget) {
            std::fill(startLoop, startLoop + link->size(), -1);

            for ( ; fromPos >= 0; fromPos -= 2) {
                // Examine the pair starting at position i in the key.

                // We are entering and then exiting the forgotten zone.
                // Identify the crossings in the bag at which these events
                // happen.
                if (maxForget < forgetStrand[(*key)[fromPos + 1]])
                    maxForget = forgetStrand[(*key)[fromPos + 1]];

                if (maxForget <= forgetStrand[(*key)[fromPos]]) {
                    maxForget = forgetStrand[(*key)[fromPos]];

                    // We can start a loop from position i in the key.
                    startLoop[(*key)[fromPos] / 2] = fromPos;
                }
            }
        }

        /**
         * Tests whether the data from the given key might survive all the way
         * up to the root of the tree decomposition.
         *
         * Side-effect: if the answer is no, then this routine deletes the key.
         */
        bool keyViable(LightweightSequence<int>* key) {
            bool analysedLoops = false;

            int n = key->size();
            int i;

            // Of all the strands passed so far that leave a crossing c to enter
            // the forgotten zone, what is the highest bag at which we forget
            // such a crossing c?
            int maxForgetEnter = -1;

            // Of all the strands passed so far that exit the forgotten zone to
            // return to a crossing c, what is the highest bag at which we
            // forget such a crossing c?
            int maxForgetExit = -1;

            int exit, enter;
            int strandID;
            for (i = n - 2; i >= 0; i -= 2) {
                // Examine the pair starting at position i in the key.

                // We are entering and then exiting the forgotten zone.
                // Identify the crossings in the bag at which these events
                // happen.
                enter = lastCrossing[(*key)[i]];
                exit = lastCrossing[(*key)[i + 1]];

                if (maxForgetEnter < forgetStrand[(*key)[i]])
                    maxForgetEnter = forgetStrand[(*key)[i]];
                if (maxForgetExit < forgetStrand[(*key)[i + 1]])
                    maxForgetExit = forgetStrand[(*key)[i + 1]];

                if ((mask[enter] & 3) == 3) {
                    // We enter the forgotten zone from crossing #enter, and
                    // both incoming strands at #enter come *from* the
                    // forgotten zone.  Therefore either one of them must
                    // appear immediately prior to this, or else it must be
                    // possible for this to start a closed-off loop.

                    if (i == 0 || lastCrossing[(*key)[i-1]] != enter) {
                        // We need to be starting a loop.
                        if (maxForgetExit != forgetStrand[(*key)[i]] ||
                                maxForgetEnter > forgetStrand[(*key)[i]]) {
                            // This cannot start a loop.
                            delete key;
                            return false;
                        }
                    }
                }

                if ((mask[exit] & 12) == 12) {
                    // We exit the forgotten zone back into crossing #exit,
                    // and both outgoing strands at #exit lead back into the
                    // forgotten zone.  Therefore either one of them must
                    // appear immediately after this, or else it must be
                    // possible for this to be the end of a closed-off loop.

                    if (i == n - 2 || lastCrossing[(*key)[i+2]] != exit) {
                        // We need to be closing off a loop.
                        if (! analysedLoops) {
                            analyseLoops(key, i,
                                (maxForgetExit > maxForgetEnter) ?
                                maxForgetExit : maxForgetEnter);
                            analysedLoops = true;
                        }
                        if (startLoop[exit] < 0 || startLoop[exit] > i) {
                            delete key;
                            return false;
                        }
                    }
                }
            }

            return true;
        }
    };

    // Convenience functions for the treewidth HOMFLY algorithm:

    inline void aggregate(
            std::map<LightweightSequence<int>*, Laurent2<Integer>*,
                LightweightSequence<int>::Less>* solns,
            LightweightSequence<int>* key, Laurent2<Integer>* value) {
        auto existingSoln = solns->emplace(key, value);
        if (! existingSoln.second) {
            *(existingSoln.first->second) += *value;
            delete key;
            delete value;
        }
    }

    inline Laurent2<Integer>* passValue(const Laurent2<Integer>* from) {
        return new Laurent2<Integer>(*from);
    }

    inline Laurent2<Integer>* switchValue(const Laurent2<Integer>* from,
            Crossing* c) {
        return new Laurent2<Integer>(*from, (c->sign() > 0 ? -2 : 2), 0);
    }

    inline Laurent2<Integer>* spliceValue(const Laurent2<Integer>* from,
            Crossing* c) {
        Laurent2<Integer>* ans = new Laurent2<Integer>(*from,
            (c->sign() > 0 ? -1 : 1), 1);
        if (c->sign() < 0)
            ans->negate();
        return ans;
    }
}

Laurent2<Integer>* Link::homflyKauffman() const {
    // Throughout this code, delta = (alpha - alpha^-1) / z.

    // We know from the preconditions that there is at least one crossing.
    size_t n = crossings_.size();

    // We order the arcs as follows:
    // - crossing 0, entering lower strand
    // - crossing 0, entering upper strand
    // - crossing 1, entering lower strand
    // - crossing 1, entering upper strand
    // - ...
    // followed by all zero-crossing unknot components (which we never
    // need to process explicitly).

    long comp = 0;
    long splices = 0;
    long splicesNeg = 0;
    long writheAdj = 0;

    // Count the number of 0-crossing unknot components separately.
    size_t unknots = 0;
    for (StrandRef s : components_)
        if (! s)
            ++unknots;

    // The final polynomial will be sum_i (coeff[i] * delta^(i + unknots)).
    //
    // Here i represents one less than the number of link components in
    // a state, not counting any 0-crossing unknot components.
    // Since we are assured at least one crossing at this point,
    // we have 0 <= i <= #components + #crossings - 1.
    size_t maxComp = 0;
    Laurent2<Integer>* coeff = new Laurent2<Integer>[
        n + components_.size()];

    // Iterate through a tree of states:
    CrossingState* state = new CrossingState[n];
    std::fill(state, state + n, CROSSING_UNSEEN);

    StrandRef* first = new StrandRef[n + components_.size()];
    std::fill(first, first + n + components_.size(), StrandRef());

    bool* seen = new bool[2 * n]; // index = 2 * crossing_index + strand
    std::fill(seen, seen + 2 * n, false);

    Laurent2<Integer> term;
    StrandRef s = StrandRef(crossings_.front(), 0);
    long pos = 0;
    bool backtrack;
    while (pos >= 0) {
        // We prepare to follow the (pos)th arc.

#ifdef DUMP_STATES
        std::cerr << "=> " << pos << ", s" << s << ", c" << comp
            << ", w" << writheAdj << " : ";
        for (size_t i = 0; i < n; ++i)
            std::cerr << (int)state[i];
        std::cerr << ' ';
        for (size_t i = 0; i < comp; ++i)
            std::cerr << first[i];
        std::cerr << ' ';
        for (size_t i = 0; i < 2*n; ++i)
            std::cerr << (seen[i] ? 'X' : '_');
        if (pos == 2 * n)
            std::cerr << "  ***";
        std::cerr << std::endl;
#endif

        if (seen[2 * s.crossing()->index() + s.strand()]) {
            // We have closed off a component of the (possibly spliced) link.
            first[comp] = s;
            ++comp;

            if (pos == 2 * n) {
                // We have completely determined a state.
                // The contribution to the HOMFLY polynomial is:
                //     (-1)^splicesNeg * z^splices * alpha^writheAdj *
                //     delta^(#components-1).
                // Note that delta^(#components-1) will be computed later;
                // here we just store the rest of the term in coeff[comp-1].
                term.init(writheAdj, splices);
                if (splicesNeg % 2)
                    term.negate();

                coeff[comp - 1] += term;
                if (comp > maxComp)
                    maxComp = comp;

                // Backtrack!
                backtrack = true;
                --comp;
                while (backtrack) {
#ifdef DUMP_STATES
                    std::cerr << "<- " << pos << ", s" << s << ", c" << comp
                        << ", w" << writheAdj << " : ";
                    for (size_t i = 0; i < n; ++i)
                        std::cerr << (int)state[i];
                    std::cerr << ' ';
                    for (size_t i = 0; i < comp; ++i)
                        std::cerr << first[i];
                    std::cerr << ' ';
                    for (size_t i = 0; i < 2*n; ++i)
                        std::cerr << (seen[i] ? 'X' : '_');
                    std::cerr << std::endl;
#endif

                    --pos;
                    if (pos < 0)
                        break;

                    --s;
                    if (state[s.crossing()->index()] == CROSSING_SPLICE_1 ||
                            state[s.crossing()->index()] == CROSSING_SPLICE_2)
                        s.jump();

                    if (! seen[2 * s.crossing()->index() + s.strand()]) {
                        --comp;
                        s = first[comp];

                        // We have to step backwards again from first[comp].
                        ++pos;
                        continue;
                    } else
                        seen[2 * s.crossing()->index() + s.strand()] = false;

                    switch (state[s.crossing()->index()]) {
                        case CROSSING_KEEP_1:
                            state[s.crossing()->index()] = CROSSING_UNSEEN;
                            break;
                        case CROSSING_SWITCH_1:
                            // We switched this crossing the first time around.
                            // Set things up so that we splice this time.
                            writheAdj += 2 * s.crossing()->sign();
                            state[s.crossing()->index()] = CROSSING_TRIED;

                            // Resume iteration from here.
                            backtrack = false;
                            break;
                        case CROSSING_SPLICE_1:
                            --splices;
                            if (s.crossing()->sign() < 0)
                                --splicesNeg;
                            writheAdj += s.crossing()->sign();

                            state[s.crossing()->index()] = CROSSING_UNSEEN;
                            break;
                        case CROSSING_KEEP_2:
                            state[s.crossing()->index()] = CROSSING_KEEP_1;
                            break;
                        case CROSSING_SWITCH_2:
                            state[s.crossing()->index()] = CROSSING_SWITCH_1;
                            break;
                        case CROSSING_SPLICE_2:
                            state[s.crossing()->index()] = CROSSING_SPLICE_1;
                            break;
                        case CROSSING_UNSEEN:
                        case CROSSING_TRIED:
                            // Should never happen.
                            std::cerr << "ERROR: homfly() is backtracking "
                                "through a crossing that does not seem to have "
                                "been visited." << std::endl;
                            break;
                    }
                }

                continue;
            } else {
                // Move to the next component.
                // Note that s should at this point be equal to the starting
                // strand of the component we just closed off.
                for (size_t i = 2 * s.crossing()->index() + s.strand() + 1;
                        i < 2 * n; ++i)
                    if (! seen[i]) {
                        s = StrandRef(crossings_[i / 2], i % 2);
                        break;
                    }
            }
        }

        seen[2 * s.crossing()->index() + s.strand()] = true;

        switch (state[s.crossing()->index()]) {
            case CROSSING_UNSEEN:
                if (s.strand() == 1) {
                    // We first visit this crossing on the upper strand.
                    // There is nothing to do.
                    // Just pass through the crossing.
                    state[s.crossing()->index()] = CROSSING_KEEP_1;
                } else {
                    // We first visit this crossing on the lower strand.
                    // Our first option is to switch.
                    // Following this, we pass through the crossing.
                    state[s.crossing()->index()] = CROSSING_SWITCH_1;

                    writheAdj -= 2 * s.crossing()->sign();
                }
                break;
            case CROSSING_TRIED:
                // We previously switched this crossing.
                // Splice, and then jump to the other strand and
                // continue through the crossing.
                state[s.crossing()->index()] = CROSSING_SPLICE_1;

                ++splices;
                if (s.crossing()->sign() < 0)
                    ++splicesNeg;
                writheAdj -= s.crossing()->sign();

                s.jump();
                break;
            case CROSSING_KEEP_1:
                // Pass through the crossing.
                state[s.crossing()->index()] = CROSSING_KEEP_2;
                break;
            case CROSSING_SWITCH_1:
                // Pass through the crossing.
                state[s.crossing()->index()] = CROSSING_SWITCH_2;
                break;
            case CROSSING_SPLICE_1:
                // Jump to the other strand and continue through the
                // crossing.
                state[s.crossing()->index()] = CROSSING_SPLICE_2;
                s.jump();
                break;

            case CROSSING_KEEP_2:
            case CROSSING_SWITCH_2:
            case CROSSING_SPLICE_2:
                // Should never happen.
                std::cerr << "ERROR: homfly() is visiting a "
                    "crossing for the third time." << std::endl;
                break;
        }
        ++s;
        ++pos;
    }
    delete[] seen;
    delete[] first;
    delete[] state;

    // Piece together the final polynomial.

    Laurent2<Integer>* ans = new Laurent2<Integer>;

    Laurent2<Integer> delta(1, -1);
    delta.set(-1, -1, -1);

    Laurent2<Integer> deltaPow(0, 0); // Initialises to delta^0 == 1.
    for (size_t i = 0; i < unknots; ++i)
        deltaPow *= delta;
    for (size_t i = 0; i < maxComp; ++i) {
        if (! coeff[i].isZero()) {
            coeff[i] *= deltaPow;
            (*ans) += coeff[i];
        }
        deltaPow *= delta;
    }

    delete[] coeff;
    return ans;
}

Laurent2<Integer>* Link::homflyTreewidth() const {
    // We know from the precondition that there is at least one crossing.

    Laurent2<Integer> delta(1, -1);
    delta.set(-1, -1, -1);

    // Build a nice tree decomposition.
    const TreeDecomposition& d = niceTreeDecomposition();
    size_t nBags = d.size();

    const TreeBag *bag, *child, *sibling;
    int index;

    // Each partial solution is a key-value map.
    //
    // Each key TODO.
    //
    // An important fact: each bag is guaranteed to have at least one solution,
    // since there is always some way to traverse the link.

    typedef LightweightSequence<int> Key;
    typedef Laurent2<Integer> Value;

    typedef std::map<Key*, Value*, Key::Less> SolnSet;

    SolnSet** partial = new SolnSet*[nBags];
    std::fill(partial, partial + nBags, nullptr);

    ViabilityData vData(this, d);

    for (bag = d.first(); bag; bag = bag->next()) {
        index = bag->index();
        std::cerr << "Bag " << index << " [" << bag->size() << "] ";

        if (bag->isLeaf()) {
            // Leaf bag.
            std::cerr << "LEAF" << std::endl;

            partial[index] = new SolnSet;
            partial[index]->emplace(new Key(), new Laurent2<Integer>(0, 0));
        } else if (bag->type() == NICE_INTRODUCE) {
            // Introduce bag.
            child = bag->children();
            std::cerr << "INTRODUCE" << std::endl;

            // When introducing a new crossing, all of its arcs must
            // lead to unseen crossings or crossings already in the bag.
            // Therefore the keys and values remain unchanged.

            partial[index] = partial[child->index()];
            partial[child->index()] = nullptr;
        } else if (bag->type() == NICE_FORGET) {
            // Forget bag.
            child = bag->children();
            std::cerr << "FORGET -> " <<
                partial[child->index()]->size() << std::endl;

            Crossing* c = crossings_[child->element(bag->subtype())];

            vData.initForgetBag(bag,
                partial[child->index()]->begin()->first, c);

            if (c->next(0).crossing() == c && c->next(1).crossing() == c) {
                // The crossing is part of two loops.
                // This means that we are forgetting a complete
                // 1-crossing unknot component.
                //
                // Steal the list of solutions directly from the child
                // bag, and just factor the extra unknot into each value.
                partial[index] = partial[child->index()];
                partial[child->index()] = nullptr;

                // We do *not* factor in the extra unknot if this is the
                // last crossing to ever be forgotten.  This is because
                // the HOMFLY formula requires us to subtract 1 from the
                // total number of loops.
                if (index != nBags - 1)
                    for (auto& soln : *(partial[index]))
                        (*soln.second) *= delta;

                continue;
            }

            partial[index] = new SolnSet;

            const Key *kChild;
            Key *kNew;
            const Value *vChild;
            Value *vNew;

            // Identify if/where the four strands touching this
            // crossing appear in the key:
            //   - id[0:lower, 1:upper][0:in, 1:out] is the unique
            //     strand ID (2 * crossing + strand);
            //   - pos[0:lower, 1:upper][0:in, 1:out] is index,
            //     or -1 if not present.
            // We also make a bitmask indicating which of these
            // four strands head into the forgotten zone.
            //
            // Both id and mask are independent of which partial solution we're
            // looking at, so we just extract them from the first key in
            // the child bag.
            // However, pos depends on the key, and so we compute that
            // every time.
            int id[2][2];
            int pos[2][2];
            int i, j;
            char mask;

            id[0][0] = c->prev(0).id();
            id[0][1] = c->lower().id();
            id[1][0] = c->prev(1).id();
            id[1][1] = c->upper().id();

            kChild = partial[child->index()]->begin()->first;

            pos[0][0] = pos[0][1] = pos[1][0] = pos[1][1] = -1;
            mask = 0;
            for (i = 0; i < kChild->size(); ++i) {
                if ((*kChild)[i] == id[0][0]) {
                    pos[0][0] = i;
                    mask |= 1;
                } else if ((*kChild)[i] == id[0][1]) {
                    pos[0][1] = i;
                    mask |= 2;
                } else if ((*kChild)[i] == id[1][0]) {
                    pos[1][0] = i;
                    mask |= 4;
                } else if ((*kChild)[i] == id[1][1]) {
                    pos[1][1] = i;
                    mask |= 8;
                }
            }

            for (auto& soln : *(partial[child->index()])) {
                kChild = soln.first;
                vChild = soln.second;

                // Recompute the pos array.
                // We don't need to reset it, since the same strands
                // will be found each time.
                for (i = 0; i < kChild->size(); ++i) {
                    if ((*kChild)[i] == id[0][0])
                        pos[0][0] = i;
                    else if ((*kChild)[i] == id[0][1])
                        pos[0][1] = i;
                    else if ((*kChild)[i] == id[1][0])
                        pos[1][0] = i;
                    else if ((*kChild)[i] == id[1][1])
                        pos[1][1] = i;
                }

                // There are *many* different cases that we need to deal
                // with here.

                if (c->next(0).crossing() == c) {
                    // TODO: Case verified.
                    // Case: the crossing is part of one loop (lower -> upper)
                    // Work out which strands to/from the crossing run
                    // into the forgotten zone.
                    // In all of our analysis, we silently untwist the loop at
                    // crossing c, and pretend there is no crossing at all.
                    switch (mask) {
                        case 0:
                            // Neither strand is from the forgotten zone.
                            for (i = 0; i <= kChild->size(); i += 2) {
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            }
                            break;
                        case 1:
                            // One strand is from a forgotten crossing.
                            // Merge it with the other.
                            std::cerr << "loop1a 1 merge" << std::endl;
                            kNew = new Key(*kChild);
                            (*kNew)[pos[0][0]] = id[1][1];

                            if (vData.keyViable(kNew))
                                aggregate(partial[index], kNew,
                                    passValue(vChild));
                            break;
                        case 8:
                            // One strand is from a forgotten crossing.
                            // Merge it with the other.
                            std::cerr << "loop1a 8 merge" << std::endl;
                            kNew = new Key(*kChild);
                            (*kNew)[pos[1][1]] = id[0][0];

                            if (vData.keyViable(kNew))
                                aggregate(partial[index], kNew,
                                    passValue(vChild));
                            break;
                        case 9:
                            // Both strands are from the forgotten zone.
                            if (pos[1][1] + 1 == pos[0][0]) {
                                // We are closing off a loop.
                                if (pos[1][1] == kChild->size() - 2) {
                                    std::cerr << "loop1a 9 pass" << std::endl;
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());

                                    // This is one of the few cases that
                                    // could describe the last forget bag,
                                    // where we must remember to subtract 1
                                    // from the total number of loops.
                                    if (vData.keyViable(kNew)) {
                                        vNew = passValue(vChild);
                                        if (index != nBags - 1)
                                            (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else {
                                // Just merge the two free ends.
                                if (pos[0][0] + 1 == pos[1][1]) {
                                    std::cerr << "loop1a 9 merge" << std::endl;
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[0][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[0][0]);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));
                                }
                            }
                            break;
                    }
                } else if (c->next(1).crossing() == c) {
                    // TODO: Case verified.
                    // Case: the crossing is part of one loop (upper -> lower)
                    // Work out which strands to/from the crossing run
                    // into the forgotten zone.
                    // In all of our analysis, we silently untwist the loop at
                    // crossing c, and pretend there is no crossing at all.
                    switch (mask) {
                        case 0:
                            // Neither strand is from the forgotten zone.
                            for (i = 0; i <= kChild->size(); i += 2) {
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            }
                            break;
                        case 2:
                            // One strand is from a forgotten crossing.
                            // Merge it with the other.
                            std::cerr << "loop1b 2 merge" << std::endl;
                            kNew = new Key(*kChild);
                            (*kNew)[pos[0][1]] = id[1][0];

                            if (vData.keyViable(kNew))
                                aggregate(partial[index], kNew,
                                    passValue(vChild));
                            break;
                        case 4:
                            // One strand is from a forgotten crossing.
                            // Merge it with the other.
                            std::cerr << "loop1b 4 merge" << std::endl;
                            kNew = new Key(*kChild);
                            (*kNew)[pos[1][0]] = id[0][1];

                            if (vData.keyViable(kNew))
                                aggregate(partial[index], kNew,
                                    passValue(vChild));
                            break;
                        case 6:
                            // Both strands are from the forgotten zone.
                            if (pos[0][1] + 1 == pos[1][0]) {
                                // We are closing off a loop.
                                if (pos[0][1] == kChild->size() - 2) {
                                    std::cerr << "loop1b 6 pass" << std::endl;
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());

                                    // This is one of the few cases that
                                    // could describe the last forget bag,
                                    // where we must remember to subtract 1
                                    // from the total number of loops.
                                    if (vData.keyViable(kNew)) {
                                        vNew = passValue(vChild);
                                        if (index != nBags - 1)
                                            (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else {
                                // Just merge the two free ends.
                                if (pos[1][0] + 1 == pos[0][1]) {
                                    std::cerr << "loop1b 6 merge" << std::endl;
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[1][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[1][0]);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));
                                }
                            }
                            break;
                    }
                } else {
                    // Case: the crossing is part of no loops.
                    // Work out which strands to/from the crossing run
                    // into the forgotten zone.
                    switch (mask) {
                        case 0:
                            // Case verified.
                            // No strands are from forgotten crossings.
                            for (i = 0; i <= kChild->size(); i += 2)
                                for (j = i; j <= kChild->size(); j += 2) {
                                    // Pass:
                                    kNew = new Key(kChild->size() + 4);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + i,
                                        kNew->begin());
                                    (*kNew)[i] = id[1][0];
                                    (*kNew)[i + 1] = id[1][1];
                                    std::copy(kChild->begin() + i,
                                        kChild->begin() + j,
                                        kNew->begin() + i + 2);
                                    (*kNew)[j + 2] = id[0][0];
                                    (*kNew)[j + 3] = id[0][1];
                                    std::copy(kChild->begin() + j,
                                        kChild->end(),
                                        kNew->begin() + j + 4);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));

                                    // Switch:
                                    kNew = new Key(kChild->size() + 4);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + i,
                                        kNew->begin());
                                    (*kNew)[i] = id[0][0];
                                    (*kNew)[i + 1] = id[0][1];
                                    std::copy(kChild->begin() + i,
                                        kChild->begin() + j,
                                        kNew->begin() + i + 2);
                                    (*kNew)[j + 2] = id[1][0];
                                    (*kNew)[j + 3] = id[1][1];
                                    std::copy(kChild->begin() + j,
                                        kChild->end(),
                                        kNew->begin() + j + 4);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            switchValue(vChild, c));

                                    // Splice:
                                    kNew = new Key(kChild->size() + 4);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + i,
                                        kNew->begin());
                                    (*kNew)[i] = id[0][0];
                                    (*kNew)[i + 1] = id[1][1];
                                    std::copy(kChild->begin() + i,
                                        kChild->begin() + j,
                                        kNew->begin() + i + 2);
                                    (*kNew)[j + 2] = id[1][0];
                                    (*kNew)[j + 3] = id[0][1];
                                    std::copy(kChild->begin() + j,
                                        kChild->end(),
                                        kNew->begin() + j + 4);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            break;
                        case 1:
                            // Case verified.
                            for (i = 0; i < pos[0][0]; i += 2) {
                                // Pass:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[0][0],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[0][0] + 2] = id[0][1];
                                std::copy(kChild->begin() + pos[0][0] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[0][0] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            }
                            for (i = pos[0][0] + 1; i <= kChild->size();
                                    i += 2) {
                                // Switch:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[0][0],
                                    kNew->begin());
                                (*kNew)[pos[0][0]] = id[0][1];
                                std::copy(kChild->begin() + pos[0][0] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[0][0] + 1);
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));

                                // Splice:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[0][0],
                                    kNew->begin());
                                (*kNew)[pos[0][0]] = id[1][1];
                                std::copy(kChild->begin() + pos[0][0] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[0][0] + 1);
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            }
                            break;
                        case 2:
                            // Case verified.
                            for (i = 0; i <= pos[0][1]; i += 2) {
                                // Pass:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[0][1],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[0][1] + 2] = id[0][0];
                                std::copy(kChild->begin() + pos[0][1] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[0][1] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));

                                // Splice:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[0][1],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[0][1] + 2] = id[1][0];
                                std::copy(kChild->begin() + pos[0][1] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[0][1] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            }
                            for (i = pos[0][1] + 2; i <= kChild->size();
                                    i += 2) {
                                // Switch:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[0][1],
                                    kNew->begin());
                                (*kNew)[pos[0][1]] = id[0][0];
                                std::copy(kChild->begin() + pos[0][1] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[0][1] + 1);
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));
                            }
                            break;
                        case 3:
                            // TODO: Case verified.
                            if (pos[0][1] + 1 == pos[0][0]) {
                                // d=a
                                // Pass:
                                if (pos[0][1] == kChild->size() - 2) {
                                    std::cerr << "3a pass" << std::endl;
                                    for (i = 0; i < kChild->size(); i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + i,
                                            kNew->begin());
                                        (*kNew)[i] = id[1][0];
                                        (*kNew)[i + 1] = id[1][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->end() - 2,
                                            kNew->begin() + i + 2);

                                        if (vData.keyViable(kNew)) {
                                            vNew = passValue(vChild);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                }
                            } else if (pos[0][0] < pos[0][1]) {
                                // Splice:
                                std::cerr << "3b splice" << std::endl;
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][0]] = id[1][1];
                                (*kNew)[pos[0][1]] = id[1][0];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));

                                if (pos[0][0] + 1 == pos[0][1]) {
                                    // Pass and switch:
                                    std::cerr << "3b pass/switch" << std::endl;
                                    for (i = 0; i < pos[0][0]; i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + i,
                                            kNew->begin());
                                        (*kNew)[i] = id[1][0];
                                        (*kNew)[i + 1] = id[1][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->begin() + pos[0][0],
                                            kNew->begin() + i + 2);
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0] + 2);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    }
                                    for (i = pos[0][1] + 2;
                                            i <= kChild->size(); i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->begin() + i,
                                            kNew->begin() + pos[0][0]);
                                        (*kNew)[i - 2] = id[1][0];
                                        (*kNew)[i - 1] = id[1][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->end(),
                                            kNew->begin() + i);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                }
                            }
                            break;
                        case 4:
                            // Case verified.
                            for (i = 0; i < pos[1][0]; i += 2) {
                                // Switch:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[1][0],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[1][0] + 2] = id[1][1];
                                std::copy(kChild->begin() + pos[1][0] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[1][0] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));

                                // Splice:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[1][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[1][0],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[1][0] + 2] = id[0][1];
                                std::copy(kChild->begin() + pos[1][0] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[1][0] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            }
                            for (i = pos[1][0] + 1; i <= kChild->size();
                                    i += 2) {
                                // Pass:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[1][0],
                                    kNew->begin());
                                (*kNew)[pos[1][0]] = id[1][1];
                                std::copy(kChild->begin() + pos[1][0] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[1][0] + 1);
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            }
                            break;
                        case 5:
                            // Case verified.
                            // Both incoming strands are from
                            // forgotten crossings.
                            if (pos[0][0] < pos[1][0]) {
                                // Switch:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][0]] = id[0][1];
                                (*kNew)[pos[1][0]] = id[1][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));

                                // Splice:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][0]] = id[1][1];
                                (*kNew)[pos[1][0]] = id[0][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            } else {
                                // Pass:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][0]] = id[0][1];
                                (*kNew)[pos[1][0]] = id[1][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            }
                            break;
                        case 6:
                            // Case verified.
                            if (pos[0][1] + 1 == pos[1][0]) {
                                // d=b
                                // Switch:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][1]] = id[0][0];
                                (*kNew)[pos[1][0]] = id[1][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));

                                if (pos[0][1] == kChild->size() - 2) {
                                    // Splice:
                                    for (i = 0; i < kChild->size(); i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + i,
                                            kNew->begin());
                                        (*kNew)[i] = id[0][0];
                                        (*kNew)[i + 1] = id[1][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->end() - 2,
                                            kNew->begin() + i + 2);

                                        if (vData.keyViable(kNew)) {
                                            vNew = spliceValue(vChild, c);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                }
                            } else {
                                if (pos[1][0] < pos[0][1]) {
                                    // Pass:
                                    kNew = new Key(*kChild);
                                    (*kNew)[pos[0][1]] = id[0][0];
                                    (*kNew)[pos[1][0]] = id[1][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));

                                    if (pos[1][0] + 1 == pos[0][1]) {
                                        // Splice:
                                        for (i = 0; i < pos[1][0]; i += 2) {
                                            kNew = new Key(kChild->size());
                                            std::copy(kChild->begin(),
                                                kChild->begin() + i,
                                                kNew->begin());
                                            (*kNew)[i] = id[0][0];
                                            (*kNew)[i + 1] = id[1][1];
                                            std::copy(kChild->begin() + i,
                                                kChild->begin() + pos[1][0],
                                                kNew->begin() + i + 2);
                                            std::copy(
                                                kChild->begin() + pos[1][0] + 2,
                                                kChild->end(),
                                                kNew->begin() + pos[1][0] + 2);

                                            if (vData.keyViable(kNew))
                                                aggregate(partial[index], kNew,
                                                    spliceValue(vChild, c));
                                        }
                                    }
                                } else {
                                    // Switch:
                                    kNew = new Key(*kChild);
                                    (*kNew)[pos[0][1]] = id[0][0];
                                    (*kNew)[pos[1][0]] = id[1][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            switchValue(vChild, c));
                                }
                            }
                            break;
                        case 7:
                            // Case verified.
                            if (pos[0][1] + 1 == pos[1][0]) {
                                // d=b
                                // Switch and splice:
                                if (pos[0][0] + 1 == pos[0][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][0],
                                        kNew->begin());
                                    (*kNew)[pos[0][0]] = id[1][1];
                                    std::copy(kChild->begin() + pos[0][0] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[0][0] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            switchValue(vChild, c));
                                }
                                if (pos[0][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[0][0]] = id[1][1];

                                    if (vData.keyViable(kNew)) {
                                        vNew = spliceValue(vChild, c);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else if (pos[0][1] + 1 == pos[0][0]) {
                                // d=a
                                // Pass:
                                if (pos[0][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[1][0]] = id[1][1];

                                    if (vData.keyViable(kNew)) {
                                        vNew = passValue(vChild);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else {
                                if (pos[0][0] + 1 == pos[0][1]) {
                                    // Pass and switch:
                                    kNew = new Key(kChild->size() - 2);
                                    if (pos[1][0] < pos[0][0]) {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);
                                        (*kNew)[pos[1][0]] = id[1][1];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    } else {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);
                                        (*kNew)[pos[1][0] - 2] = id[1][1];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                } else if (pos[1][0] + 1 == pos[0][1] &&
                                        pos[0][0] < pos[1][0]) {
                                    // Splice:
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[1][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[1][0]);
                                    (*kNew)[pos[0][0]] = id[1][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            }
                            break;
                        case 8:
                            // Case verified.
                            for (i = 0; i <= pos[1][1]; i += 2) {
                                // Switch:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + i,
                                    kNew->begin());
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->begin() + pos[1][1],
                                    kNew->begin() + i + 2);
                                (*kNew)[pos[1][1] + 2] = id[1][0];
                                std::copy(kChild->begin() + pos[1][1] + 1,
                                    kChild->end(),
                                    kNew->begin() + pos[1][1] + 3);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));
                            }
                            for (i = pos[1][1] + 2; i <= kChild->size();
                                    i += 2) {
                                // Pass:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[1][1],
                                    kNew->begin());
                                (*kNew)[pos[1][1]] = id[1][0];
                                std::copy(kChild->begin() + pos[1][1] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[1][1] + 1);
                                (*kNew)[i] = id[0][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));

                                // Splice:
                                kNew = new Key(kChild->size() + 2);
                                std::copy(kChild->begin(),
                                    kChild->begin() + pos[1][1],
                                    kNew->begin());
                                (*kNew)[pos[1][1]] = id[0][0];
                                std::copy(kChild->begin() + pos[1][1] + 1,
                                    kChild->begin() + i,
                                    kNew->begin() + pos[1][1] + 1);
                                (*kNew)[i] = id[1][0];
                                (*kNew)[i + 1] = id[0][1];
                                std::copy(kChild->begin() + i,
                                    kChild->end(),
                                    kNew->begin() + i + 2);

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            }
                            break;
                        case 9:
                            // Case verified.
                            if (pos[1][1] + 1 == pos[0][0]) {
                                // c=a
                                // Pass:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[1][1]] = id[1][0];
                                (*kNew)[pos[0][0]] = id[0][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));
                            } else {
                                if (pos[1][1] < pos[0][0]) {
                                    // Pass:
                                    kNew = new Key(*kChild);
                                    (*kNew)[pos[1][1]] = id[1][0];
                                    (*kNew)[pos[0][0]] = id[0][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));
                                } else {
                                    // Switch:
                                    kNew = new Key(*kChild);
                                    (*kNew)[pos[1][1]] = id[1][0];
                                    (*kNew)[pos[0][0]] = id[0][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            switchValue(vChild, c));

                                    if (pos[0][0] + 1 == pos[1][1]) {
                                        // Splice:
                                        for (i = pos[1][1] + 2;
                                                i <= kChild->size(); i += 2) {
                                            kNew = new Key(kChild->size());
                                            std::copy(kChild->begin(),
                                                kChild->begin() + pos[0][0],
                                                kNew->begin());
                                            std::copy(
                                                kChild->begin() + pos[0][0] + 2,
                                                kChild->begin() + i,
                                                kNew->begin() + pos[0][0]);
                                            (*kNew)[i - 2] = id[1][0];
                                            (*kNew)[i - 1] = id[0][1];
                                            std::copy(kChild->begin() + i,
                                                kChild->end(),
                                                kNew->begin() + i);

                                            if (vData.keyViable(kNew))
                                                aggregate(partial[index], kNew,
                                                    spliceValue(vChild, c));
                                        }
                                    }
                                }
                            }
                            break;
                        case 10:
                            // Case verified.
                            // Both outgoing strands are to
                            // forgotten crossings.
                            if (pos[0][1] < pos[1][1]) {
                                // Switch:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][1]] = id[0][0];
                                (*kNew)[pos[1][1]] = id[1][0];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        switchValue(vChild, c));
                            } else {
                                // Pass:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][1]] = id[0][0];
                                (*kNew)[pos[1][1]] = id[1][0];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        passValue(vChild));

                                // Splice:
                                kNew = new Key(*kChild);
                                (*kNew)[pos[0][1]] = id[1][0];
                                (*kNew)[pos[1][1]] = id[0][0];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));
                            }
                            break;
                        case 11:
                            // Case verified.
                            if (pos[0][1] + 1 == pos[0][0]) {
                                // d=a
                                // Pass:
                                if (pos[0][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[1][1]] = id[1][0];

                                    if (vData.keyViable(kNew)) {
                                        vNew = passValue(vChild);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else if (pos[1][1] + 1 == pos[0][0]) {
                                // c=a
                                // Pass:
                                if (pos[0][0] + 1 == pos[0][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][1],
                                        kNew->begin());
                                    (*kNew)[pos[1][1]] = id[1][0];
                                    std::copy(kChild->begin() + pos[1][1] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[1][1] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));
                                }
                            } else {
                                if (pos[0][0] + 1 == pos[0][1]) {
                                    // Pass and switch:
                                    kNew = new Key(kChild->size() - 2);
                                    if (pos[1][1] < pos[0][1]) {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);
                                        (*kNew)[pos[1][1]] = id[1][0];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    } else {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);
                                        (*kNew)[pos[1][1] - 2] = id[1][0];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                } else if (pos[0][0] + 1 == pos[1][1] &&
                                        pos[1][1] < pos[0][1]) {
                                    // Splice:
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[0][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[0][0]);
                                    (*kNew)[pos[0][1] - 2] = id[1][0];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            }
                            break;
                        case 12:
                            // TODO: Case verified.
                            if (pos[1][1] + 1 == pos[1][0]) {
                                // c=b
                                // Splice:
                                std::cerr << "12a splice" << std::endl;
                                kNew = new Key(*kChild);
                                (*kNew)[pos[1][1]] = id[0][0];
                                (*kNew)[pos[1][0]] = id[0][1];

                                if (vData.keyViable(kNew))
                                    aggregate(partial[index], kNew,
                                        spliceValue(vChild, c));

                                if (pos[1][1] == kChild->size() - 2) {
                                    // Switch:
                                    std::cerr << "12a switch" << std::endl;
                                    for (i = 0; i < kChild->size(); i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + i,
                                            kNew->begin());
                                        (*kNew)[i] = id[0][0];
                                        (*kNew)[i + 1] = id[0][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->end() - 2,
                                            kNew->begin() + i + 2);

                                        if (vData.keyViable(kNew)) {
                                            vNew = switchValue(vChild, c);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                }
                            } else {
                                if (pos[1][1] < pos[1][0]) {
                                    // Splice:
                                    std::cerr << "12b splice" << std::endl;
                                    kNew = new Key(*kChild);
                                    (*kNew)[pos[1][1]] = id[0][0];
                                    (*kNew)[pos[1][0]] = id[0][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                } else if (pos[1][0] + 1 == pos[1][1]) {
                                    // Pass and switch:
                                    std::cerr << "12b pass/switch" << std::endl;
                                    for (i = 0; i < pos[1][0]; i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + i,
                                            kNew->begin());
                                        (*kNew)[i] = id[0][0];
                                        (*kNew)[i + 1] = id[0][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->begin() + pos[1][0],
                                            kNew->begin() + i + 2);
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0] + 2);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                    for (i = pos[1][1] + 2;
                                            i <= kChild->size(); i += 2) {
                                        kNew = new Key(kChild->size());
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->begin() + i,
                                            kNew->begin() + pos[1][0]);
                                        (*kNew)[i - 2] = id[0][0];
                                        (*kNew)[i - 1] = id[0][1];
                                        std::copy(kChild->begin() + i,
                                            kChild->end(),
                                            kNew->begin() + i);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    }
                                }
                            }
                            break;
                        case 13:
                            // Case verified.
                            if (pos[1][1] + 1 == pos[0][0]) {
                                // c=a
                                // Pass:
                                if (pos[1][0] + 1 == pos[1][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][0],
                                        kNew->begin());
                                    (*kNew)[pos[1][0]] = id[0][1];
                                    std::copy(kChild->begin() + pos[1][0] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[1][0] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            passValue(vChild));
                                }
                            } else if (pos[1][1] + 1 == pos[1][0]) {
                                // c=b
                                // Switch and splice:
                                if (pos[1][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[0][0]] = id[0][1];

                                    if (vData.keyViable(kNew)) {
                                        vNew = switchValue(vChild, c);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                                if (pos[0][0] + 1 == pos[1][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][0],
                                        kNew->begin());
                                    (*kNew)[pos[0][0]] = id[0][1];
                                    std::copy(kChild->begin() + pos[0][0] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[0][0] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            } else {
                                if (pos[1][0] + 1 == pos[1][1]) {
                                    // Pass and switch:
                                    kNew = new Key(kChild->size() - 2);
                                    if (pos[1][0] < pos[0][0]) {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0]);
                                        (*kNew)[pos[0][0] - 2] = id[0][1];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    } else {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0]);
                                        (*kNew)[pos[0][0]] = id[0][1];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                } else if (pos[0][0] + 1 == pos[1][1] &&
                                        pos[0][0] < pos[1][0]) {
                                    // Splice:
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[0][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[0][0]);
                                    (*kNew)[pos[1][0] - 2] = id[0][1];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            }
                            break;
                        case 14:
                            // Case verified.
                            if (pos[0][1] + 1 == pos[1][0]) {
                                // d=b
                                // Switch and splice:
                                if (pos[1][0] + 1 == pos[1][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[0][1],
                                        kNew->begin());
                                    (*kNew)[pos[0][1]] = id[0][0];
                                    std::copy(kChild->begin() + pos[0][1] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[0][1] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            switchValue(vChild, c));
                                } else if (pos[0][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[1][1]] = id[0][0];

                                    if (vData.keyViable(kNew)) {
                                        vNew = spliceValue(vChild, c);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else if (pos[1][1] + 1 == pos[1][0]) {
                                // c=b
                                // Switch and splice:
                                if (pos[1][0] + 1 == pos[0][1]) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][1],
                                        kNew->begin());
                                    (*kNew)[pos[1][1]] = id[0][0];
                                    std::copy(kChild->begin() + pos[1][1] + 3,
                                        kChild->end(),
                                        kNew->begin() + pos[1][1] + 1);

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                } else if (pos[1][1] == kChild->size() - 2) {
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->end() - 2, kNew->begin());
                                    (*kNew)[pos[0][1]] = id[0][0];

                                    if (vData.keyViable(kNew)) {
                                        vNew = switchValue(vChild, c);
                                        (*vNew) *= delta;
                                        aggregate(partial[index], kNew, vNew);
                                    }
                                }
                            } else {
                                if (pos[1][0] + 1 == pos[1][1]) {
                                    // Pass and switch:
                                    kNew = new Key(kChild->size() - 2);
                                    if (pos[1][1] < pos[0][1]) {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0]);
                                        (*kNew)[pos[0][1] - 2] = id[0][0];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    } else {
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0]);
                                        (*kNew)[pos[0][1]] = id[0][0];

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    }
                                } else if (pos[1][0] + 1 == pos[0][1] &&
                                        pos[1][1] < pos[0][1]) {
                                    // Splice:
                                    kNew = new Key(kChild->size() - 2);
                                    std::copy(kChild->begin(),
                                        kChild->begin() + pos[1][0],
                                        kNew->begin());
                                    std::copy(
                                        kChild->begin() + pos[1][0] + 2,
                                        kChild->end(),
                                        kNew->begin() + pos[1][0]);
                                    (*kNew)[pos[1][1]] = id[0][0];

                                    if (vData.keyViable(kNew))
                                        aggregate(partial[index], kNew,
                                            spliceValue(vChild, c));
                                }
                            }
                            break;
                        case 15:
                            // Case verified.
                            // All strands are from forgotten crossings.
                            if (pos[0][1] + 1 == pos[0][0]) {
                                if (pos[1][1] + 1 == pos[1][0]) {
                                    // d=a, c=b
                                    // Pass:
                                    if (pos[1][1] == kChild->size() - 4 &&
                                            pos[0][1] == kChild->size() - 2) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->end() - 4, kNew->begin());

                                        // This is one of the few cases that
                                        // could describe the last forget bag,
                                        // where we must remember to subtract 1
                                        // from the total number of loops.
                                        if (vData.keyViable(kNew)) {
                                            vNew = passValue(vChild);
                                            (*vNew) *= delta;
                                            if (index != nBags - 1)
                                                (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                } else {
                                    // d=a
                                    // Pass:
                                    if (pos[0][1] == kChild->size() - 2 &&
                                            pos[1][0] + 1 == pos[1][1]) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end() - 2,
                                            kNew->begin() + pos[1][0]);

                                        if (vData.keyViable(kNew)) {
                                            vNew = passValue(vChild);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                }
                            } else if (pos[0][1] + 1 == pos[1][0]) {
                                if (pos[1][1] + 1 == pos[0][0]) {
                                    // d=b, c=a
                                    // Pass:
                                    if (pos[1][1] == kChild->size() - 4 &&
                                            pos[0][1] == kChild->size() - 2) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->end() - 4, kNew->begin());

                                        // This is one of the few cases that
                                        // could describe the last forget bag,
                                        // where we must remember to subtract 1
                                        // from the total number of loops.
                                        if (vData.keyViable(kNew)) {
                                            vNew = passValue(vChild);
                                            if (index != nBags - 1)
                                                (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                } else {
                                    // d=b
                                    // Switch and splice:
                                    if (pos[1][0] + 1 == pos[1][1] &&
                                            pos[0][0] + 1 == pos[0][1]) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 4,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                switchValue(vChild, c));
                                    } else if (pos[0][0] + 1 == pos[1][1] &&
                                            pos[0][1] == kChild->size() - 2) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end() - 2,
                                            kNew->begin() + pos[0][0]);

                                        if (vData.keyViable(kNew)) {
                                            vNew = spliceValue(vChild, c);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    }
                                }
                            } else {
                                if (pos[1][1] + 1 == pos[1][0]) {
                                    // c=b
                                    // Switch and splice:
                                    if (pos[0][0] + 1 == pos[0][1] &&
                                            pos[1][1] == kChild->size() - 2) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->end() - 2,
                                            kNew->begin() + pos[0][0]);

                                        if (vData.keyViable(kNew)) {
                                            vNew = switchValue(vChild, c);
                                            (*vNew) *= delta;
                                            aggregate(partial[index], kNew, vNew);
                                        }
                                    } else if (pos[0][0] + 1 == pos[1][1] &&
                                            pos[1][0] + 1 == pos[0][1]) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 4,
                                            kChild->end(),
                                            kNew->begin() + pos[0][0]);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                spliceValue(vChild, c));
                                    }
                                } else if (pos[1][1] + 1 == pos[0][0]) {
                                    // c=a
                                    // Pass:
                                    if (pos[1][0] + 1 == pos[1][1] &&
                                            pos[0][0] + 1 == pos[0][1]) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[1][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 4,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0]);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                passValue(vChild));
                                    }
                                } else {
                                    // Pass, switch and splice:
                                    if (pos[0][0] + 1 == pos[0][1] &&
                                            pos[1][0] + 1 == pos[1][1]) {
                                        kNew = new Key(kChild->size() - 4);
                                        if (pos[1][0] < pos[0][0]) {
                                            std::copy(kChild->begin(),
                                                kChild->begin() + pos[1][0],
                                                kNew->begin());
                                            std::copy(
                                                kChild->begin() + pos[1][0] + 2,
                                                kChild->begin() + pos[0][0],
                                                kNew->begin() + pos[1][0]);
                                            std::copy(
                                                kChild->begin() + pos[0][0] + 2,
                                                kChild->end(),
                                                kNew->begin() + pos[0][0] - 2);

                                            if (vData.keyViable(kNew))
                                                aggregate(partial[index], kNew,
                                                    passValue(vChild));
                                        } else {
                                            std::copy(kChild->begin(),
                                                kChild->begin() + pos[0][0],
                                                kNew->begin());
                                            std::copy(
                                                kChild->begin() + pos[0][0] + 2,
                                                kChild->begin() + pos[1][0],
                                                kNew->begin() + pos[0][0]);
                                            std::copy(
                                                kChild->begin() + pos[1][0] + 2,
                                                kChild->end(),
                                                kNew->begin() + pos[1][0] - 2);

                                            if (vData.keyViable(kNew))
                                                aggregate(partial[index], kNew,
                                                    switchValue(vChild, c));
                                        }
                                    } else if (pos[0][0] + 1 == pos[1][1] &&
                                            pos[1][0] + 1 == pos[0][1] &&
                                            pos[0][0] < pos[1][0]) {
                                        kNew = new Key(kChild->size() - 4);
                                        std::copy(kChild->begin(),
                                            kChild->begin() + pos[0][0],
                                            kNew->begin());
                                        std::copy(
                                            kChild->begin() + pos[0][0] + 2,
                                            kChild->begin() + pos[1][0],
                                            kNew->begin() + pos[0][0]);
                                        std::copy(
                                            kChild->begin() + pos[1][0] + 2,
                                            kChild->end(),
                                            kNew->begin() + pos[1][0] - 2);

                                        if (vData.keyViable(kNew))
                                            aggregate(partial[index], kNew,
                                                spliceValue(vChild, c));
                                    }
                                }
                            }
                            break;
                    }
                }

                delete kChild;
                delete vChild;
            }

            delete partial[child->index()];
            partial[child->index()] = nullptr;
        } else {
            // Join bag.
            child = bag->children();
            sibling = child->sibling();

            // Extract the sizes of each bag's keys.
            // The key size depends only on the bag, not the particular
            // key-value solution at that bag, and so we get this data
            // by looking at the first solution in each bag.
            size_t pairs1 = partial[child->index()]->begin()->first->size()/2;
            size_t pairs2 = partial[sibling->index()]->begin()->first->size()/2;
            size_t pairs = pairs1 + pairs2;

            std::cerr << "JOIN -> " <<
                partial[child->index()]->size() << " x " <<
                partial[sibling->index()]->size() << " : #pairs = " <<
                pairs1 << " / " << pairs2 << std::endl;

            if (pairs1 == 0) {
                // The keys are exactly the keys from the second child,
                // so we steal the second child solution set entirely
                // without copying solutions individually.
                //
                // The first child should have exactly one key (which is
                // empty), and we just need to multiply all values by
                // the corresponding value.

                auto emptySoln = partial[child->index()]->begin();

                partial[index] = partial[sibling->index()];
                for (auto& soln : *(partial[index]))
                    (*soln.second) *= *(emptySoln->second);

                delete emptySoln->first;
                delete emptySoln->second;
                delete partial[child->index()];

                partial[child->index()] = partial[sibling->index()] = nullptr;
                continue;
            } else if (pairs2 == 0) {
                // As before, but with the two children the other way around.
                auto emptySoln = partial[sibling->index()]->begin();

                partial[index] = partial[child->index()];
                for (auto& soln : *(partial[index]))
                    (*soln.second) *= *(emptySoln->second);

                delete emptySoln->first;
                delete emptySoln->second;
                delete partial[sibling->index()];

                partial[child->index()] = partial[sibling->index()] = nullptr;
                continue;
            }

            // Both child bags have positive length keys.

            vData.initJoinBag(partial[child->index()]->begin()->first,
                partial[sibling->index()]->begin()->first);

            partial[index] = new SolnSet;
            const Key *k1, *k2;
            const Value *v1, *v2;
            Key *kNew;
            Value *vNew;

            // TODO: Make this work for arbitrary sized bags.
            // Currently we can only handle sizeof(long) * 8 pairs,
            // which means at most sizeof(long) * 4 crossings per bag.
            typedef unsigned long Mask;
            Mask mask;
            int pos1, pos2, pos;
            for (auto& soln1 : *(partial[child->index()])) {
                k1 = soln1.first;
                v1 = soln1.second;

                for (auto& soln2 : *(partial[sibling->index()])) {
                    k2 = soln2.first;
                    v2 = soln2.second;

                    // Combine the two child keys and values in all
                    // possible ways.
                    Value val(*v1);
                    val *= *v2;

                    for (mask = ((Mask)(1) << pairs2) - 1;
                            mask && ! (mask & ((Mask)(1) << pairs));
                            mask = BitManipulator<Mask>::nextPermutation(mask)) {
                        // The bits of mask correspond to the
                        // positions of pairs in the final key.
                        kNew = new Key(k1->size() + k2->size());

                        pos1 = pos2 = 0;
                        for (pos = 0; pos < pairs; ++pos) {
                            if (mask & ((Mask)(1) << pos)) {
                                // Use the next pair from k2.
                                (*kNew)[2 * pos] = (*k2)[2 * pos2];
                                (*kNew)[2 * pos + 1] = (*k2)[2 * pos2 + 1];
                                ++pos2;
                            } else {
                                // Use the next pair from k1.
                                (*kNew)[2 * pos] = (*k1)[2 * pos1];
                                (*kNew)[2 * pos + 1] = (*k1)[2 * pos1 + 1];
                                ++pos1;
                            }
                        }

                        if (vData.keyViable(kNew)) {
                            if (! partial[index]->emplace(
                                    kNew, new Value(val)).second)
                                std::cerr << "ERROR: Combined keys in join "
                                    "bag are not unique" << std::endl;
                        }
                    }
                }
            }

            for (auto& soln : *(partial[child->index()])) {
                delete soln.first;
                delete soln.second;
            }
            for (auto& soln : *(partial[sibling->index()])) {
                delete soln.first;
                delete soln.second;
            }

            delete partial[child->index()];
            delete partial[sibling->index()];
            partial[child->index()] = partial[sibling->index()] = nullptr;
        }

        /*
        for (const auto& soln : *(partial[index])) {
            for (int i = 0; i < soln.first->size(); ++i)
                std::cerr << (*soln.first)[i] << ' ';
            std::cerr << "-> " << (*soln.second) << std::endl;
        }
        */
    }

    // Collect the final answer from partial[nBags - 1].
    // std::cerr << "FINISH" << std::endl;
    Value* ans = partial[nBags - 1]->begin()->second;

    for (auto& soln : *(partial[nBags - 1])) {
        delete soln.first;
        if (soln.second != ans)
            delete soln.second;
    }
    delete partial[nBags - 1];

    delete[] partial;

    // Finally, factor in any zero-crossing components.
    for (StrandRef s : components_)
        if (! s)
            (*ans) *= delta;

    return ans;
}

const Laurent2<Integer>& Link::homflyAZ(Algorithm alg) const {
    if (homflyAZ_.known())
        return *homflyAZ_.value();

    if (crossings_.empty()) {
        if (components_.empty())
            return *(homflyAZ_ = new Laurent2<Integer>());

        // We have an unlink with no crossings.
        // The HOMFLY polynomial is delta^(#components - 1).
        Laurent2<Integer> delta(1, -1);
        delta.set(-1, -1, -1);

        // The following constructor initialises ans to 1.
        Laurent2<Integer>* ans = new Laurent2<Integer>(0, 0);
        for (size_t i = 1; i < components_.size(); ++i)
            (*ans) *= delta;

        return *(homflyAZ_ = ans);
    }

    switch (alg) {
        case ALG_TREEWIDTH:
            homflyAZ_ = homflyTreewidth();
            break;
        default:
            homflyAZ_ = homflyKauffman();
            break;
    }
    return *homflyAZ_.value();
}

const Laurent2<Integer>& Link::homflyLM(Algorithm alg) const {
    if (homflyLM_.known())
        return *homflyLM_.value();

    Laurent2<Integer>* ans = new Laurent2<Integer>(homflyAZ(alg));

    // Negate all coefficients for a^i z^j where i-j == 2 (mod 4).
    // Note that i-j should always be 0 or 2 (mod 4), never odd.
    for (auto& term : ans->coeff_) {
        if ((term.first.first - term.first.second) % 4 != 0)
            term.second.negate();
    }

    return *(homflyLM_ = ans);
}

} // namespace regina

