
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

// #define DUMP_STATES

namespace regina {

const char* Link::homflyAZVarX = "\u03B1"; // alpha
const char* Link::homflyAZVarY = "z";

const char* Link::homflyLMVarX = "\U0001D4C1"; // mathematical script small l
const char* Link::homflyLMVarY = "m";

const char* Link::homflyVarX = homflyAZVarX;
const char* Link::homflyVarY = homflyAZVarY;

// Possible states of crossings:

enum CrossingState {
    /**
     * Not yet visited.  Moreover, this state indicates that - if there
     * is a decision to make - we should first attempt to switch this crossing.
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
    // TODO: Implement this!
    return homflyKauffman();
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

