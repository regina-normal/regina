
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include <cctype>
#include <list>
#include <map>
#include <sstream>
#include <algorithm>

#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "algebra/abeliangroup.h"
#include "maths/numbertheory.h"
#include "maths/matrixops.h"
#include "utilities/exception.h"
#include "utilities/fixedarray.h"
#include "utilities/stringutils.h"

/**
 * Decide how to implement dehnAlgorithmSubMetric().  Options include:
 *
 * 1 - The old algorithm from Regina 7.x, which is worst-case cubic but
 *     typically quadratic in practice since most match positions require very
 *     little iteration before a mismatch occurs.
 *
 * 2,3 - Different variants of rewritten searches that organise the iteration
 *     differently to be worst-case quadratic, but which both appear to have
 *     larger constants and have been found to be slower in preliminary testing.
 *
 * Algorithm 1 picks up many non-maximal substitutions; algorithms 2,3 pick up
 * fewer (the only non-maximal matches they find should be those that begin at
 * the start of the target word, but which could have been extended by
 * wrapping back past the beginning of the target).  This does not matter
 * during group simplification (where only the best substitution matters),
 * but it does change the behaviour (in a harmless way) when calling
 * proliferateRelators().  For an example where these behaviours differ,
 * see GroupPresentation(3, ['abaabb', 'aabc']).
 *
 * TODO: Possibly the _real_ optimisations will come from caching calls to
 * dehnAlgorithmSubMetric() and thereby calling it fewer times.
 */
#define DEHN_SUB_ALGORITHM 1

namespace regina {

std::ostream& operator << (std::ostream& out,
        const GroupExpressionTerm& term) {
    if (term.exponent == 0)
        out << '1';
    else if (term.exponent == 1)
        out << 'g' << term.generator;
    else
        out << 'g' << term.generator << '^' << term.exponent;
    return out;
}

GroupExpressionTerm& GroupExpression::term(size_t index) {
    auto pos = terms_.begin();
    advance(pos, index);
    return *pos;
}

const GroupExpressionTerm& GroupExpression::term(size_t index) const {
    auto pos = terms_.begin();
    advance(pos, index);
    return *pos;
}

GroupExpression GroupExpression::inverse() const {
    GroupExpression ans;
    for (const GroupExpressionTerm& t : terms_)
        ans.terms_.push_front(t.inverse());
    return ans;
}

void GroupExpression::invert() {
    reverse(terms_.begin(), terms_.end());
    for (auto& t : terms_)
        t.exponent = -t.exponent;
}

GroupExpression GroupExpression::power(long exponent) const {
    GroupExpression ans;
    long i;
    if (exponent > 0) {
        for (i = 0; i < exponent; ++i)
            ans.terms_.insert(ans.terms_.end(), terms_.begin(), terms_.end());
    } else if (exponent < 0) {
        for (i = 0; i > exponent; --i)
            for (const GroupExpressionTerm& t : terms_)
                ans.terms_.push_front(t.inverse());
    }
    return ans;
}

bool GroupExpression::simplify(bool cyclic) {
    bool changed = false;
    for (auto next = terms_.begin(); next != terms_.end(); ) {
        // Take a look at merging next forwards.
        if (next->exponent == 0) {
            // Zero exponent.
            // Delete this term and step back to the previous term in
            // case we can now merge the previous and next terms.
            next = terms_.erase(next);
            if (next != terms_.begin())
                next--;
            changed = true;
            continue;
        }

        auto tmpIt = next;
        tmpIt++;

        // Now tmpIt points to the term after next.
        if (tmpIt == terms_.end()) {
            // No term to merge forwards with.
            next++;
        } else if ((*tmpIt) += (*next)) {
            // Successfully merged this with the following term.
            next = terms_.erase(next);
            changed = true;
            // Look at this term again to see if it can be merged further.
        } else {
            // Different generators; cannot merge.
            next++;
        }
    }

    if (! cyclic)
        return changed;

    // Now trying merging front and back terms.
    // We shall do this by popping terms off the back and merging them
    // with the front term.
    while (terms_.begin() != terms_.end() &&
            ++terms_.begin() != terms_.end()) {
        // Thus terms_.size() > 1.  The unusual test above is used to
        // avoid calling terms_.size() which takes linear time.
        if (terms_.front() += terms_.back()) {
            // Merged!
            terms_.pop_back();
            changed = true;

            // Did we create an empty term?
            if (terms_.front().exponent == 0)
                terms_.pop_front();
        } else
            break;
    }

    return changed;
}

bool GroupExpression::substitute(size_t generator,
        const GroupExpression& expansion, bool cyclic) {
    bool changed = false;
    std::optional<GroupExpression> inverse;
    for (auto current = terms_.begin(); current != terms_.end(); ) {
        if (current->generator != generator)
            current++;
        else {
            long exponent = current->exponent;
            if (exponent != 0) {
                const GroupExpression* use;
                if (exponent > 0)
                    use = std::addressof(expansion);
                else {
                    if (! inverse)
                        inverse = expansion.inverse();
                    use = std::addressof(*inverse);
                    exponent = -exponent;
                }

                // Fill in exponent copies of use.
                //
                // Note that the following insertion will invalidate
                // current if the wrong type of data structure is being used!
                for (long i = 0; i < exponent; i++)
                    terms_.insert(current, use->terms_.begin(), use->terms_.end());
            }

            current = terms_.erase(current);
            changed = true;
        }
    }
    if (changed)
        simplify(cyclic);
    return changed;
}

void GroupExpression::substitute(const std::vector<GroupExpression>& expansions,
        bool cyclic) {
    std::list<GroupExpressionTerm> expanded;

    long i;
    for (const auto& t : terms_) {
        if (t.exponent > 0) {
            const GroupExpression& use = expansions[t.generator];
            for (i = 0; i < t.exponent; ++i)
                expanded.insert(expanded.end(),
                    use.terms_.begin(), use.terms_.end());
        } else if (t.exponent < 0) {
            GroupExpression use = expansions[t.generator].inverse();
            for (i = 0; i > t.exponent; --i)
                expanded.insert(expanded.end(),
                    use.terms_.begin(), use.terms_.end());
        }
    }

    terms_.swap(expanded);
    simplify(cyclic);
}

// TODO: To add: platonic groups, octahedral/cubical, dihedral,
//       icosahedral/dodecahedral, tetrahedral and binary versions of them.
//       Also need to recognise circle bundles over surfaces.
//       Free products with amalgamation. Currently simplify()
//       isn't smart enough for this.
std::string GroupPresentation::recogniseGroup(bool moreUtf8) const {
    // Get the trivial case out of the way first.
    if (nGenerators_ == 0)
        return "0";

    // Run through cases.
    std::ostringstream out;

    // Let's record the abelianisation.
    AbelianGroup ab =  abelianisation();

    // abelian test
    if (identifyAbelian()) {
        ab.writeTextShort(out, moreUtf8);
        return std::move(out).str();
    }

    // not (clearly) abelian.  Check if free.
    if (relations_.empty()) {
        out << "Free(" << nGenerators_ << ")";
        return std::move(out).str();
    }

    // Check if its an extension over Z.
    // TODO: eventually look for extensions over at least fg abelian groups.
    //   ??maybe?? some other finite groups but it's not clear how to look
    //   for those.
    if (ab.rank()==1) {
        GroupPresentation presCopy( *this );
        auto AUT = presCopy.identifyExtensionOverZ();
        if (AUT) {
            // Let's try to identify the fibre.
            std::string domStr( AUT->domain().recogniseGroup(moreUtf8) );
            if (domStr.length()>0) {
                if (moreUtf8)
                    out << "\u2124~"; // unicode blackboard bold Z
                else
                    out << "Z~";
                out << domStr << " w/monodromy ";
                size_t numGen = AUT->domain().countGenerators();
                for (size_t i=0; i<numGen; i++) {
                    if (i!=0)
                        out<<", ";
                    if (numGen<27)
                        out<<( (char) (i+97) );
                    else
                        out<<"g"<<i;
                    out<<" \u21A6 "; // mapsto symbol in unicode
                    AUT->evaluate(i).writeTextShort(out, moreUtf8, numGen<27);
                }
                return std::move(out).str();
            } // domain not recognised, but it is an extension
            // TODO: put in something here for this case.
        }
    }

    std::list<GroupPresentation> fpDecomp(identifyFreeProduct());
    if (fpDecomp.size()>1) {
        out<<"FreeProduct( ";
        bool first = true;
        for (const GroupPresentation& factor : fpDecomp) {
            if (first)
                first = false;
            else
                out << ", ";
            std::string facStr(factor.recogniseGroup(moreUtf8));
            if (facStr.empty())
                out << "Unknown";
            else
                out << facStr;
        }
        out << " )";
        return std::move(out).str();
    }

    // TODO: let's put in the undergraduate test for finiteness, that every
    //  word can be written as a product of generators a^xb^yc^z with xyz
    //  in a finite interval. Look for the relators that would allow for this
    //  kind of conclusion.

    return std::string(); // returns empty string if not recognised.
}

AbelianGroup GroupPresentation::abelianisation() const {
    if (nGenerators_ == 0)
        return AbelianGroup();

    if (relations_.empty()) {
        // Free group becomes free abelian group.
        return AbelianGroup(nGenerators_);
    } else {
        MatrixInt m(relations_.size(), nGenerators_);

        size_t row = 0;
        for (const auto& r : relations_) {
            for (const auto& t : r.terms())
                m.entry(row, t.generator) += t.exponent;
            ++row;
        }

        return AbelianGroup(std::move(m));
    }
}

size_t GroupPresentation::abelianRank() const {
    if (nGenerators_ == 0)
        return 0;
    if (relations_.empty()) {
        // We have a free group, which becomes a free abelian group.
        return nGenerators_;
    }

    MatrixInt m(relations_.size(), nGenerators_);

    size_t row = 0;
    for (const auto& r : relations_) {
        for (const auto& t : r.terms())
            m.entry(row, t.generator) += t.exponent;
        ++row;
    }

    return nGenerators_ - std::move(m).rank();
}

MarkedAbelianGroup GroupPresentation::markedAbelianisation() const {
    // create presentation matrices to pass to MarkedAbelianGroup(M, N)
    MatrixInt N(countGenerators(), countRelations() );
    // run through rels, increment N entries appropriately
    for (size_t j=0; j<countRelations(); j++) {
        GroupExpression Rj ( relation(j) );
        for (size_t i=0; i<Rj.countTerms(); i++)
            N.entry( Rj.generator(i), j ) += Rj.exponent(i);
    }
    return MarkedAbelianGroup(
        MatrixInt(1, countGenerators()) /* zero matrix */,
        std::move(N));
}

GroupPresentation::SplayedWord GroupPresentation::splay(
        const GroupExpression& word, size_t length) {
    using SignedGenerator = SplayedWord::value_type;
    SplayedWord ans(length);

    auto it = ans.begin();
    for (const auto& t : word.terms())
        if (t.exponent >= 0) {
            SignedGenerator entry = t.generator + 1;
            for (long i = 0; i < t.exponent; ++i)
                *it++ = entry;
        } else {
            SignedGenerator entry =
                -static_cast<SignedGenerator>(t.generator + 1);
            for (long i = 0; i > t.exponent; --i)
                *it++ = entry;
        }

    return ans;
}

size_t GroupPresentation::extraCancellation(const SplayedWord& word,
        auto begin, auto end) {
    size_t ans = 0;
    while (true) {
        if (begin == end)
            return ans;

        // Move outwards to the next candidate pair of symbols to cancel.
        word.cycleBackward(begin);
        if (begin == end)
            return ans;

        if (*begin != -*end)
            return ans;
        ++ans;

        word.cycleForward(end);
    }
}

template <Aggregator<GroupPresentation::WordSubstitutionData> Agg>
typename Agg::Result GroupPresentation::dehnAlgorithmSubMetric(
        const GroupExpression &target, const GroupExpression &reducer,
        int step) {
    size_t target_len = target.wordLength();
    size_t reducer_len = reducer.wordLength();

    // generic early exit strategy
    if (target_len < 2 || reducer_len==0)
        return Agg().result();
    // early exit strategy based on step.
    if (2 * target_len + step <= reducer_len)
        return Agg().result();

    auto target_vec = splay(target, target_len);
    auto reducer_vec = splay(reducer, reducer_len);

    // search for cyclic subwords of reducer_vec in target_vec...
    Agg sub_list;

#if DEHN_SUB_ALGORITHM == 1
    WordSubstitutionData sub;
    auto start_sub = target_vec.begin();
    for (sub.target_pos = 0; sub.target_pos < target_len;
            ++sub.target_pos, ++start_sub) {
        ssize_t extra_score = -1; // -1 means not yet computed
        for (bool invert : { false, true }) {
            for (sub.reducer_pos = 0; sub.reducer_pos < reducer_len;
                    ++sub.reducer_pos) {
                sub.invert_reducer = invert;
                sub.length = 0;
                auto p = start_sub;
                auto q = (invert ? reducer_vec.end() - (sub.reducer_pos+1) :
                    reducer_vec.begin() + sub.reducer_pos);
                if (invert) {
                    while (*p == -*q && sub.length < reducer_len &&
                            sub.length < target_len) {
                        ++sub.length;
                        target_vec.cycleForward(p);
                        reducer_vec.cycleBackward(q);
                    }
                } else {
                    while (*p == *q && sub.length < reducer_len &&
                            sub.length < target_len) {
                        ++sub.length;
                        target_vec.cycleForward(p);
                        reducer_vec.cycleForward(q);
                    }
                }
                if (sub.length == reducer_len) {
                    // The entire copy of *reducer* will vanish.
                    // Will the remaining pieces of *target* cancel further?
                    if (extra_score < 0)
                        extra_score = extraCancellation(target_vec,
                            start_sub, p);
                    sub.score = reducer_len + extra_score;
                    sub_list += sub;
                } else if (sub.length > 0) {
                    sub.score = 2 * sub.length - reducer_len;
                    if ( sub.score > -step )
                        sub_list += sub;
                }
            }
        }
    }
#elif DEHN_SUB_ALGORITHM == 3
    // Cache results of extraCancellation(); -1 means not yet computed
    FixedArray<long> extraScore(target_len, -1);

    WordSubstitutionData sub;
    for (bool invert : { false, true }) {
        sub.invert_reducer = invert;

        // Consider laying out infinitely many copies of reducer_vec end-to-end.
        // There are *reducer_len* ways of doing this, according to which
        // element of *reducer* is aligned with the beginning of *target*.
        // The outermost loop here considers each of those alignments in turn.
        for (auto align = reducer_vec.begin(); align != reducer_vec.end();
                ++align) {
            // We will walk through *target* from start to end, looking for
            // matches that end at the current position *pos* and do not wrap
            // around the ends of *target*.  After this, we continue our
            // iteration further to locate matches that _do_ wrap around.

            auto this_pos = target_vec.begin();
            auto that_pos = align;
            sub.target_pos = 0;
            if (invert)
                sub.reducer_pos = (reducer_vec.end() - that_pos) - 1;
            else
                sub.reducer_pos = that_pos - reducer_vec.begin();
            sub.length = 0;
            bool wraparound = false;
            while (true) {
                if (sub.length == reducer_len || sub.length == target_len) {
                    // The current match is maximal, because we have matched
                    // either the entire reducer word or the entire target word.
                    if (sub.length == reducer_len) {
                        if (extraScore[sub.target_pos] < 0)
                            extraScore[sub.target_pos] = extraCancellation(
                                target_vec,
                                (sub.target_pos < sub.length ?
                                    this_pos + target_len - sub.length :
                                    this_pos - sub.length),
                                this_pos);
                        sub.score = reducer_len + extraScore[sub.target_pos];
                        sub_list += sub;
                    } else {
                        // The match covers all of *target*, but not
                        // all of *reducer_vec*.
                        sub.score = 2 * sub.length - reducer_len;
                        if (sub.score > -step)
                            sub_list += sub;
                    }

                    // We cannot use this starting point again, since no
                    // match can be longer than *reducer_len* or *target_len*.
                    ++sub.target_pos;
                    if (wraparound && sub.target_pos == target_len)
                        break; // no more wraparound matches possible
                    if (invert) {
                        if (sub.reducer_pos == 0)
                            sub.reducer_pos = reducer_len - 1;
                        else
                            --sub.reducer_pos;
                    } else {
                        if (++sub.reducer_pos == reducer_len)
                            sub.reducer_pos = 0;
                    }
                    --sub.length;
                }

                bool matchedSymbol = (invert ? *this_pos == -*that_pos :
                    *this_pos == *that_pos);
                if (! matchedSymbol) {
                    // If we are holding onto a prior match, it must now be
                    // maximal.
                    if (sub.length) {
                        // This symbol does not match, but we have a prior match
                        // which is therefore maximal.
                        sub.score = 2 * sub.length - reducer_len;
                        if (sub.score > -step)
                            sub_list += sub;
                    }
                    if (wraparound) {
                        // There can be no more wraparound matches from here.
                        break;
                    }
                }

                ++this_pos;
                if ((! wraparound) && this_pos == target_vec.end()) {
                    // TODO: Is this even possible if wraparound is true?
                    wraparound = true;
                    this_pos = target_vec.begin();
                }
                if (invert)
                    reducer_vec.cycleBackward(that_pos);
                else
                    reducer_vec.cycleForward(that_pos);

                if (matchedSymbol) {
                    ++sub.length;
                } else {
                    sub.length = 0;
                    sub.target_pos = this_pos - target_vec.begin();
                    if (invert)
                        sub.reducer_pos =
                            (reducer_vec.end() - that_pos) - 1;
                    else
                        sub.reducer_pos = that_pos - reducer_vec.begin();
                }
            }
        }
    }
#else
    #error Invalid value of DEHN_SUB_ALGORITHM
#endif

    return std::move(sub_list).result();
}

/**
 *  This applies a substitution generated by dehnAlgorithmSubMetric.
 */
void GroupPresentation::applySubstitution(GroupExpression& target,
        const GroupExpression &reducer, const WordSubstitutionData &sub)
{
    // okay, so let's do a quick cut-and-replace, reduce the word and
    // hand it back.
    size_t target_len = target.wordLength();
    size_t reducer_len = reducer.wordLength();

    #if 0
    std::cout << "Sub: (" << sub.target_pos << '/' << target_len
        << ")-(" << sub.reducer_pos << '/' << reducer_len
        << (sub.invert_reducer ? ")^-1: " : "): ")
        << sub.length << " -> " << sub.score << std::endl;
    #endif

    std::vector<GroupExpressionTerm> target_vec, reducer_vec;
    // we'll splay-out *target* and *reducer* so that it's easier to search
    // for commonalities.
    target_vec.reserve( target_len );
    reducer_vec.reserve( reducer_len );
    // start the splaying of target
    for (const auto& t : target.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            target_vec.emplace_back( t.generator, (t.exponent>0) ? 1 : -1 );
    }
    // and reducer
    for (const auto& t : reducer.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            reducer_vec.emplace_back( t.generator, (t.exponent>0) ? 1 : -1 );
    }
    // done splaying, produce inv_reducer
    std::vector< GroupExpressionTerm > inv_reducer( reducer_len );
    for (size_t i=0; i<reducer_len; i++)
        inv_reducer[reducer_len-(i+1)] = reducer_vec[i].inverse();
    // done with inv_reducer, erase terms
    target.terms().clear();

    // *this word is some conjugate of AB and the relator is some conjugate of AC.
    //  We are performing the substitution
    // A=C^{-1}, thus we need to produce the word C^{-1}B. Put in C^{-1} first..
    for (size_t i=0; i<(reducer_len - sub.length); i++)
        target.terms().push_back( sub.invert_reducer ?
            reducer_vec[(reducer_len - sub.reducer_pos + i) % reducer_len] :
            inv_reducer[(reducer_len - sub.reducer_pos + i) % reducer_len] );
    // iterate through remainder of target_vec, starting from
    //     sub.target_pos + sub.length, ie fill in B
    for (size_t i=0; i<(target_len - sub.length); i++)
        target.terms().push_back(
            target_vec[(sub.target_pos + sub.length + i) % target_len] );
    // done
    target.simplify();
}


namespace { // anonymous namespace
    bool compare_length( const GroupExpression& first,
             const GroupExpression& second )
     {  return ( first.wordLength() < second.wordLength() ); }

    /**
     *  This routine takes a list of words, together with expVec. It's assumed
     * expVec is initialized to be zero and as long as the number of generators
     * in the group.  What this routine does is, for each generator of the
     * group, it records the sum of the absolute value of the exponents of that
     * generator in word.  For the i-th generator this number is recorded in
     * expVec[i].
     */
    void build_exponent_vec( const std::list< GroupExpressionTerm > & word,
                              std::vector<unsigned long> &expVec )
    {
     for ( const auto& t : word)
         expVec[ t.generator ] += std::abs( t.exponent );
    }
}

// gives a string that describes the substitution
std::string GroupPresentation::WordSubstitutionData::substitutionString(
        const GroupExpression &word) const {
    std::string retval;
    // cut the substitution data into bits, assemble what we're cutting
    //  out and what we're pasting in.
    size_t word_length = word.wordLength();
    std::vector<GroupExpressionTerm> reducer_vec;
    reducer_vec.reserve( word_length );
    // splay word
    for (auto it = word.terms().begin(); it!=word.terms().end(); it++) {
        for (long i=0; i<std::abs(it->exponent); i++)
            reducer_vec.emplace_back(it->generator, (it->exponent>0) ? 1 : -1);
    }
    // done splaying, produce inv_reducer
    std::vector< GroupExpressionTerm > inv_reducer( word_length );
    for (size_t i=0; i<word_length; i++)
        inv_reducer[word_length-(i+1)] = reducer_vec[i].inverse();
    GroupExpression del_word, rep_word;
        // produce word to delete, and word to replace with.

    for (size_t i=0; i<(word_length - length); i++)
        rep_word.addTermLast( invert_reducer ?
            reducer_vec[(word_length - reducer_pos + i) % word_length] :
            inv_reducer[(word_length - reducer_pos + i) % word_length] );
    for (size_t i=0; i<length; i++)
        del_word.addTermLast( invert_reducer ?
            inv_reducer[(reducer_pos + i) % word_length] :
            reducer_vec[(reducer_pos + i) % word_length] );
    rep_word.simplify();
    del_word.simplify();
    retval = del_word.str()+" -> "+rep_word.str();
    return retval;
}

/**
 * Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_i2^j2 ... g_in^jn g_i1^j1
 */
void GroupExpression::cycleRight() {
    if (terms_.size() > 1) {
        GroupExpressionTerm temp(terms_.front());
        terms_.pop_front();
        terms_.push_back(temp);
    }
}

/**
 * Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1
 */
void GroupExpression::cycleLeft() {
    if (terms_.size() > 1) {
        GroupExpressionTerm temp(terms_.back());
        terms_.pop_back();
        terms_.push_front(temp);
    }
}

GroupExpression::GroupExpression(const char* word, size_t nGens) {
    // interpret word as GroupExpression as one of forms a^7b^-2,
    // a^7B^2, aaaaaaaBB, g0^7g1^-2.

    enum class WordStatus {
        Null,   /**< at start of word, nothing has been input */
        VarLet, /**< read a letter but unknown if it's like a^5 or g2^-2 */
        VarNum, /**< `gk` situation read */
        Exp,    /**< `^` read */
        ExpSig, /**< `^-` read */
        ExpNum  /**< reading numbers after `^` or `^-` */
    };

    // a loop that goes through the entries of word.
    WordStatus ws = WordStatus::Null;
    GroupExpressionTerm buildTerm;
    for (const char* i = word; *i; ++i) {
        // read *i, see what to do next.
        // case 1: it is a letter a..z or A..Z
        if ( ( *i >= 'a' && *i <= 'z' ) || ( *i >= 'A' && *i <= 'Z' ) ) {
            switch (ws) {
                case WordStatus::Null:
                    // fresh letter
                    // build buildTerm.
                    if ( *i >= 'a' && *i <= 'z' ) {
                        buildTerm.generator = *i - 'a';
                        buildTerm.exponent = 1;
                    } else {
                        buildTerm.generator = *i - 'A';
                        buildTerm.exponent = -1;
                    }
                    ws = WordStatus::VarLet;
                    continue;
                case WordStatus::VarLet:
                case WordStatus::VarNum:
                case WordStatus::ExpNum:
                    // new letter but previous letter to finish
                    if (nGens && buildTerm.generator >= nGens)
                        throw InvalidArgument(
                            "Generator out of range in group expression");
                    terms_.push_back(buildTerm);
                    if ( *i >= 'a' && *i <= 'z' ) {
                        buildTerm.generator = *i - 'a';
                        buildTerm.exponent = 1;
                    } else {
                        buildTerm.generator = *i - 'A';
                        buildTerm.exponent = -1;
                    }
                    ws = WordStatus::VarLet;
                    continue;
                default:
                    // anything else is a mistake.
                    throw InvalidArgument(
                        "Unexpected letter found in group expression");
            }
        }

        // case 2: it is a ^, can only occur after a generator
        if ( *i == '^' ) {
            if ( !(ws == WordStatus::VarLet || ws == WordStatus::VarNum) ) {
                throw InvalidArgument(
                    "Unexpected exponent found in group expression");
            }
            ws = WordStatus::Exp;
            continue;
        } // end case 2

        // case 3: it is a -, only valid after ^
        if ( *i == '-' ) {
            if ( ws != WordStatus::Exp ) {
                throw InvalidArgument(
                    "Unexpected minus sign found in group expression");
            }
            buildTerm.exponent = -buildTerm.exponent; // ok with A^-1.
            ws = WordStatus::ExpSig;
            continue;
        } // end case 3

        // case 4: it is a number
        if ( ::isdigit(*i) ) {
            //  subcase (a) this is to build a variable
            // buildTerm.generator == 'g'
            if ( ws == WordStatus::VarLet &&
                    buildTerm.generator == ('g' - 'a') ) {
                buildTerm.generator=(*i - '0');
                ws = WordStatus::VarNum;
                continue;
            } else // we've already started building the variable number
                if ( ws == WordStatus::VarNum ) {
                    buildTerm.generator=10*buildTerm.generator + (*i - '0');
                    continue;
                } else //  subcase (b) this is to build an exponent.
                    if ( ws == WordStatus::Exp || ws == WordStatus::ExpSig ) {
                        // ^num or ^-num
                        if (buildTerm.exponent<0)
                            buildTerm.exponent = - static_cast<long>(*i - '0');
                        else
                            buildTerm.exponent = (*i - '0');
                        ws = WordStatus::ExpNum;
                        continue;
                    } else if ( ws == WordStatus::ExpNum ) {
                        // blah[num] previously dealt with numbers
                        if (buildTerm.exponent<0)
                            buildTerm.exponent = 10*buildTerm.exponent -
                                                 (*i - '0');
                        else
                            buildTerm.exponent = 10*buildTerm.exponent +
                                                 (*i - '0');
                        continue;
                    } else {
                        throw InvalidArgument(
                            "Unexpected number found in group expression");
                    }
        } // end case 4
        // now we've dealt will all important input.  Let's deal with spaces
        // next, and any other input will fail.
        if ( ::isspace(*i) )
            continue;

        throw InvalidArgument("Invalid character in group expression");
    } // end i loop

    // end of input
    if (ws == WordStatus::Null) {
        // Term is empty
        return;
    }
    if (ws == WordStatus::Exp || ws == WordStatus::ExpSig) {
        // Term ends with ^ or ^-
        throw InvalidArgument("Unexpected end of string in group expression");
    }

    // we reached the end of input without any errors
    if (nGens && buildTerm.generator >= nGens)
        throw InvalidArgument("Generator out of range in group expression");
    terms_.push_back(buildTerm);

    // Since some people use the form "aaaaBBB", combine adjacent terms
    // where we can.
    simplify();
}

//             **********  GroupPresentation below **************

GroupPresentation::GroupPresentation(size_t nGens,
        const std::vector<std::string> &rels) {
    nGenerators_ = nGens;

    relations_.reserve(rels.size());
    for (const std::string& r : rels)
        relations_.emplace_back(r, nGens);

    if (nGens == 0) {
        // In this case the GroupExpression constructor will not verify
        // that all generators are in range.  Check this now (which for
        // nGens == 0 simply means ensuring each relation is empty).
        for (const auto& r : relations_)
            if (! r.isTrivial())
                throw InvalidArgument(
                    "Generator out of range in group presentation");
    }
}

bool GroupPresentation::simplifyAndConjugate(GroupExpression &word) const {
    bool retval = word.simplify(false);
    if (word.isTrivial())
        return retval;

    // now recursively apply relators until no reduction is possible.
    bool continueSimplify = true;
    while (continueSimplify) {
        continueSimplify = false;
        for (const auto& r : relations_) {
            auto sub =
                dehnAlgorithmSubMetric<MinAggregator<WordSubstitutionData>>(
                word, r );
            if (sub && sub->score > 0) {
                applySubstitution( word, r, *sub );
                if (word.isTrivial())
                    return true;
                continueSimplify = true;
                retval = true;
            }
        }
    }
    return retval;
}

// for now we iterate:
// 1) Small Cancellation theory
// 2) Nielsen moves until no reducing ones available
// 3) Loop back to (1) until nothing happens in either (1) or (2).
// TODO: consider a homological alignment call if the abelianization
//       has rank 1 or any other situation where we know it can be useful.
std::optional<HomGroupPresentation> GroupPresentation::simplify() {
    // IMPORTANT NOTE: This routine *must* be deterministic.
    // This matters (for example) when we simplify the domain and codomain
    // of a monodromy, and we need both ends to simplify identically.

    bool doRep(true);
    std::optional<HomGroupPresentation> redHom;
    while (doRep) {
        doRep = false;
        if (auto h = smallCancellation()) {
            doRep = true;
            if (!redHom)
                redHom = std::move(*h);
            else
                *redHom = (*h) * (*redHom);
        }

        if (auto h = nielsen()) {
            doRep = true;
            if (!redHom)
                redHom = std::move(*h);
            else
                *redHom = (*h) * (*redHom);
        }
    }

    if (auto h = prettyRewriting()) {
        if (!redHom)
            redHom = std::move(*h);
        else
            *redHom = (*h) * (*redHom);
    }

    return redHom;
}

std::optional<HomGroupPresentation> GroupPresentation::smallCancellation() {
    // IMPORTANT NOTE: This routine *must* be deterministic.
    // This matters (for example) when we simplify the domain and codomain
    // of a monodromy, and we need both ends to simplify identically.

    bool didSomething(false);
    // start by taking a copy of *this group, for construction of reductionMap
    GroupPresentation oldGroup( *this );

    // substitutionTable[i] == 1 means kill gi.
    //                      != gi means replace gi by this.
    std::vector< GroupExpression > substitutionTable( nGenerators_ );
    for (size_t i=0; i<nGenerators_; i++)
        substitutionTable[i].addTermFirst( i, 1 );

    bool we_value_iteration(true);
    while (we_value_iteration) {
        we_value_iteration = false;
        // cyclically reduce relators
        for (GroupExpression& r : relations_)
            r.simplify(true);
        std::sort(relations_.begin(), relations_.end(), compare_length);// (1)

        // start (2) deletion of 0-length relators
        while (! relations_.empty()) {
            if (relations_.front().wordLength() == 0)
                relations_.erase(relations_.begin());
            else
                break;
        }

        // start (3) - apply shorter relators to longer.
        for (auto it = relations_.begin(); it != relations_.end(); it++)
            if ( it->wordLength() > 0 ) { // don't bother if this is a trivial word.
                auto tit = it; // target of it manips.
                tit++;
                while (tit != relations_.end()) {
                    // attempt to apply *it to *tit
                    auto sub = dehnAlgorithmSubMetric<
                        MinAggregator<WordSubstitutionData>>( *tit, *it );
                    if (sub && sub->score > 0) {
                        applySubstitution( *tit, *it, *sub );
                        we_value_iteration = true;
                        didSomething = true;
                    }
                    tit++;
                }
            } // end (3) - application of shorter to longer relators.

        // (4) Build and sort a list (by length) of generator-killing relations.
        std::sort(relations_.begin(), relations_.end(), compare_length);
        for (const GroupExpression& r : relations_) {
            bool word_length_3_trigger(false);
            size_t WL = r.wordLength();
            // build a table expressing # times each generator is used in r.
            std::vector<unsigned long> genUsage( nGenerators_ );
            build_exponent_vec( r.terms(), genUsage );

            for (size_t i=0; i<genUsage.size(); i++)
                if (genUsage[i] == 1) {
                    // have we found a substitution for generator i ?
                    if ( ( substitutionTable[i].countTerms() == 1 ) &&
                            ( substitutionTable[i].generator(0) == i ) ) {
                        // we have a valid substitution.  Replace all occurances
                        // of generator genUsage[i] with the inverse of the
                        // remaining word
                        bool inv(true);
                        bool before_flag(true); // true if we have not yet
                                                // encountered gen
                        GroupExpression prefix, complement;
                        for (const GroupExpressionTerm& t : r.terms()) {
                            if ( t.generator == i ) {
                                inv = (t.exponent != 1);
                                before_flag=false;
                            } else {
                                if (before_flag)
                                    prefix.addTermLast(t);
                                else
                                    complement.addTermLast(t);
                            }
                        }
                        complement.addTermsLast(std::move(prefix));
                        // WARNING: Cannot use prefix beyond here.
                        if (!inv)
                            complement.invert();
                        // sub gi --> complement, in both substitutionTable
                        // and relations_
                        for (GroupExpression& e : substitutionTable)
                            e.substitute( i, complement );
                        for (GroupExpression& e : relations_)
                            e.substitute( i, complement );
                        we_value_iteration = true;
                        didSomething = true;
                        if (WL>3)
                            word_length_3_trigger=true;
                        goto found_a_generator_killer;
                    }
                } // the look through genUsage loop

found_a_generator_killer:
            if (word_length_3_trigger)
                break;
        } // end (4)
    } // end of main_while_loop (6)

    nGenerators_ = 0;
    for (size_t i=0; i<substitutionTable.size(); i++)
        if ( substitutionTable[i].countTerms() == 1 )
            if ( substitutionTable[i].generator(0) == i )
                nGenerators_++;

    // now we can build a mapping of where the current generators get sent to.
    // make it a std::vector.

    std::vector<size_t> genReductionMapping( nGenerators_ );
    size_t indx = 0;
    for (size_t i=0; i<substitutionTable.size(); i++) {
        if ( substitutionTable[i].countTerms() == 1 )
            if ( substitutionTable[i].generator(0) == i ) {
                genReductionMapping[ indx ] = i;
                indx++;
            }
    }

    // now run through relations_ and substitute genReductionMapping[i] -> i
    for (auto& r : relations_)
        for (size_t i=0; i<nGenerators_; i++) {
            GroupExpression gi;
            gi.addTermFirst( i, 1 );
            r.substitute( genReductionMapping[i], gi );
        }
    // and might as well do substitutionTable, too.
    for (auto& sub : substitutionTable)
        for (size_t i=0; i<nGenerators_; i++) {
            GroupExpression gi;
            gi.addTermFirst( i, 1 );
            sub.substitute( genReductionMapping[i], gi );
        }
    // and build the reverse isomorphism from the new group to the old
    std::vector< GroupExpression > revMap(nGenerators_);
    for (size_t i=0; i<revMap.size(); i++)
        revMap[i].addTermFirst( genReductionMapping[i], 1 );

    if (didSomething) {
        // now we can initialize reductionMap
        return HomGroupPresentation(std::move(oldGroup), *this,
            substitutionTable, revMap);
    } else
        return {};
}// end smallCancellation()

std::optional<HomGroupPresentation> GroupPresentation::nielsen() {
    // IMPORTANT NOTE: This routine *must* be deterministic.
    // This matters (for example) when we simplify the domain and codomain
    // of a monodromy, and we need both ends to simplify identically.

    if (nGenerators_ < 2) return {};
    // let's keep a record of the best possible substitution,
    bool didSomething(true);
    std::optional<HomGroupPresentation> retval;
    while (didSomething) {
        didSomething = false;
        size_t bSubi = 0, bSubj = 0;
        int bSubType = 0; // IJ IJi JI or JIi 0,1,2,3
        signed long bSubScore(0);

        for (size_t i=0; i<nGenerators_; i++)
            for (size_t j=0; j<nGenerators_; j++) {
                if (i==j) continue;
                signed long scrIJ(0), scrIJi(0), scrJI(0), scrJIi(0); // ongoing score count.
                // run through all the relators.
                for (GroupExpression& rel : relations_) {
                    std::list<GroupExpressionTerm>& terms( rel.terms() );
                    // now we run from front to back, cyclically keeping track of the
                    //  previous and next terms respectively.
                    GroupExpressionTerm prevTerm(terms.back()),
                                         thisTerm(terms.front()), nextTerm;
                    for (auto T=terms.begin(); T!=terms.end(); T++) {
                        auto Tn = T;
                        Tn++;
                        if (T!=terms.begin()) {
                            prevTerm = thisTerm;
                            thisTerm=nextTerm;
                        }
                        nextTerm = ( (Tn == terms.end()) ? terms.front() : *Tn );

                        // check IJ  gigj    -> gi  i.e. gi -> gigj^-1 all other gens fixed
                        if ( (thisTerm.generator == i) && (thisTerm.exponent>0) )
                            ( (nextTerm.generator == j) && (nextTerm.exponent>0) ) ?
                            scrIJ += (2-thisTerm.exponent) : scrIJ -= thisTerm.exponent;
                        if ( (thisTerm.generator == i) && (thisTerm.exponent<0) )
                            ( (prevTerm.generator == j) && (prevTerm.exponent<0) ) ?
                            scrIJ += (2+thisTerm.exponent) : scrIJ += thisTerm.exponent;
                        // check IJi gigj^-1 -> gi i.e. gi -> gigj
                        if ( (thisTerm.generator == i) && (thisTerm.exponent>0) )
                            ( (nextTerm.generator == j) && (nextTerm.exponent<0) ) ?
                            scrIJi += (2-thisTerm.exponent) : scrIJi -= thisTerm.exponent;
                        if ( (thisTerm.generator == i) && (thisTerm.exponent<0) )
                            ( (prevTerm.generator == j) && (prevTerm.exponent>0) ) ?
                            scrIJi += (2+thisTerm.exponent) : scrIJi += thisTerm.exponent;
                        // check JI  gjgi    -> gi
                        if ( (thisTerm.generator == i) && (thisTerm.exponent<0) )
                            ( (nextTerm.generator == j) && (nextTerm.exponent<0) ) ?
                            scrJI += (2+thisTerm.exponent) : scrJI += thisTerm.exponent;
                        if ( (thisTerm.generator == i) && (thisTerm.exponent>0) )
                            ( (prevTerm.generator == j) && (prevTerm.exponent>0) ) ?
                            scrJI += (2-thisTerm.exponent) : scrJI -= thisTerm.exponent;
                        // check JIi gj^-1gi -> gi
                        if ( (thisTerm.generator == i) && (thisTerm.exponent<0) )
                            ( (nextTerm.generator == j) && (nextTerm.exponent>0) ) ?
                            scrJIi += (2+thisTerm.exponent) : scrJIi += thisTerm.exponent;
                        if ( (thisTerm.generator == i) && (thisTerm.exponent>0) )
                            ( (prevTerm.generator == j) && (prevTerm.exponent<0) ) ?
                            scrJIi += (2-thisTerm.exponent) : scrJIi -= thisTerm.exponent;
                    }
                } // end of relator loop
                // compare the IJ, IJi, JI, JIi subs to the best known one, if it exists
                if (scrIJ > bSubScore)  {
                    bSubScore = scrIJ;
                    bSubi = i;
                    bSubj = j;
                    bSubType = 0;
                }
                if (scrIJi > bSubScore) {
                    bSubScore = scrIJi;
                    bSubi = i;
                    bSubj = j;
                    bSubType = 1;
                }
                if (scrJI > bSubScore)  {
                    bSubScore = scrJI;
                    bSubi = i;
                    bSubj = j;
                    bSubType = 2;
                }
                if (scrJIi > bSubScore) {
                    bSubScore = scrJIi;
                    bSubi = i;
                    bSubj = j;
                    bSubType = 3;
                }
            }

        if (bSubScore > 0) {
            // perform sub!
            didSomething=true;
            GroupPresentation oldPres(*this);
            std::vector<GroupExpression> DomToRan( oldPres.countGenerators() );
            std::vector<GroupExpression> RanToDom( oldPres.countGenerators() );
            for (size_t i=0; i<DomToRan.size(); i++) {
                DomToRan[i].addTermFirst(i, 1);
                RanToDom[i].addTermFirst(i, 1);
            }
            if (bSubType == 0) {
                nielsenCombine( bSubi, bSubj, 1, true );
                DomToRan[ bSubi ].addTermLast(bSubj, -1);
                RanToDom[ bSubi ].addTermLast(bSubj, 1);
            } else if (bSubType == 1) {
                nielsenCombine( bSubi, bSubj, -1, true );
                DomToRan[ bSubi ].addTermLast(bSubj, 1);
                RanToDom[ bSubi ].addTermLast(bSubj, -1);
            } else if (bSubType == 2) {
                nielsenCombine( bSubi, bSubj, 1, false );
                DomToRan[ bSubi ].addTermFirst(bSubj, -1);
                RanToDom[ bSubi ].addTermFirst(bSubj, 1);
            } else if (bSubType == 3) {
                nielsenCombine( bSubi, bSubj, -1, false );
                DomToRan[ bSubi ].addTermFirst(bSubj, 1);
                RanToDom[ bSubi ].addTermFirst(bSubj, -1);
            }
            HomGroupPresentation tempHom(std::move(oldPres), *this,
                    DomToRan,RanToDom);
            if (!retval)
                retval = std::move(tempHom);
            else
                *retval = tempHom * (*retval);
            // make the dom->ran and ran->dom vectors.
        }
    } // the while loop

    return retval;
}

std::optional<HomGroupPresentation> GroupPresentation::homologicalAlignment() {
    std::optional<HomGroupPresentation> retval; // only allocate if appropriate.
    // step 1: compute abelianization and how generators map to abelianization.
    MarkedAbelianGroup abelianized = markedAbelianisation();
    MatrixInt abMat( abelianized.snfRank(), countGenerators() );

    for (size_t j=0; j<countGenerators(); j++) {
        Vector<Integer> temp = abelianized.snfRep(
            Vector<Integer>::unit(countGenerators(), j));
        for (size_t i=0; i<abelianized.snfRank(); i++)
            abMat.entry(i,j) = temp[i]; // cols are snfreps of abelianized gens.
    }

    size_t abNF = abelianized.countInvariantFactors();
    size_t abNG = abelianized.snfRank();

    // step 2: we will mimic the simple smith normal form algorithm algorithm
    //         using corresponding moves on the group presentation.
    //         first the free generators.
    try { // Integer::safeValue<long>() could throw exceptions.
        for (size_t i=abNF; i<abNG; i++) {
            // in row i we will eliminate all but one entry using column
            // operations.  Now we need to do a while loop -- find any two
            // non-zero entries in the row, and reduce.  If there's only one
            // non-zero entry, we're done.
            size_t j0 = 0, j1 = abMat.columns()-1;
            while (j0 < j1) {
                // if at j0 its zero, inc, if at j1 zero, dec
                if (abMat.entry(i,j0).isZero()) {
                    j0++;
                    continue;
                }
                if (abMat.entry(i,j1).isZero()) {
                    j1--;
                    continue;
                }
                // column op!
                bool colFlag(
                    abMat.entry(i,j0).abs() < abMat.entry(i,j1).abs() );
                Integer q = abMat.entry(i,colFlag ? j1 : j0) /
                            abMat.entry(i,colFlag ? j0 : j1);
                // subtract q times column j0 from column j1
                for (size_t r=0; r<abMat.rows(); r++)
                    abMat.entry(r,colFlag ? j1 : j0) -=
                        abMat.entry(r,colFlag ? j0 : j1) * q;
                GroupPresentation oldPres(*this);
                std::vector<GroupExpression> fVec( nGenerators_ );
                std::vector<GroupExpression> bVec( nGenerators_ );
                for (size_t l=0; l<nGenerators_; l++) {
                    fVec[l].addTermLast(l, 1);
                    bVec[l].addTermLast(l, 1);
                    if (l==j1) {
                        fVec[l].addTermLast(colFlag ? j0 : j1,
                            q.safeValue<long>());
                        bVec[l].addTermLast(colFlag ? j0 : j1,
                            -q.safeValue<long>());
                    }
                }
                // manufacture the Nielsen automorphism...
                nielsenCombine(colFlag ? j1 : j0, colFlag ? j0 : j1,
                    -q.safeValue<long>() );
                HomGroupPresentation tempHom(std::move(oldPres), *this,
                    fVec, bVec);
                if (!retval)
                    retval = std::move(tempHom);
                else
                    *retval = tempHom * (*retval);
            } // j0==j1 is the column such that entry (i,j1) is +-1.
            nielsenTransposition(i, j1);
            abMat.swapCols(i, j1);
            // NOTE: the matrix will have the form:
            //       [ * * * ]
            //       [ 0 D 0 ]  At this point, with D a diagonal +-1 matrix.
        }

        for (size_t i=0; i<abNF; i++)
            for (size_t j=abNF; j<abNG; j++)
                abMat.entry(i,j) = 0;
        // now we're at [ * 0 * ]
        //              [ 0 D 0 ]

        // step 3: reduce inv fac terms, kill the rest.
        for (size_t i=0; i<abNF; i++) {
            // let's start working on entry(i,j0) and (i,j1)
            // with j0<j1 in 0...invFacNum
            size_t j0 = 0, j1 = abMat.columns()-1;
            while (j0 < j1) {
                // if at j0 its zero, inc, if at j1 zero, dec
                if ((abMat.entry(i,j0) %
                        abelianized.invariantFactor(i)).isZero()) {
                    j0++;
                    continue;
                }
                if ((abMat.entry(i,j1) %
                        abelianized.invariantFactor(i)).isZero()) {
                    j1--;
                    continue;
                }
                // column op!
                bool colFlag(
                    (abMat.entry(i,j0) % abelianized.invariantFactor(i)).abs() <
                    (abMat.entry(i,j1) % abelianized.invariantFactor(i)).abs());
                Integer q = abMat.entry(i,colFlag ? j1 : j0) /
                            abMat.entry(i,colFlag ? j0 : j1);

                // subtract q times column j0 from column j1
                for (size_t r=0; r<abMat.rows(); r++)
                    abMat.entry(r,colFlag ? j1 : j0) -=
                        abMat.entry(r,colFlag ? j0 : j1)*q;
                GroupPresentation oldPres(*this);
                std::vector<GroupExpression> fVec( nGenerators_ );
                std::vector<GroupExpression> bVec( nGenerators_ );
                for (size_t l=0; l<nGenerators_; l++) {
                    fVec[l].addTermLast(l, 1);
                    bVec[l].addTermLast(l, 1);
                    if (l==j1) {
                        fVec[l].addTermLast(colFlag ? j0 : j1,
                            q.safeValue<long>());
                        bVec[l].addTermLast(colFlag ? j0 : j1,
                            -q.safeValue<long>());
                    }
                }
                // manufacture the Nielsen automorphism...
                nielsenCombine(colFlag ? j1 : j0, colFlag ? j0 : j1,
                    -q.safeValue<long>() );
                HomGroupPresentation tempHom(std::move(oldPres), *this,
                    fVec, bVec);
                if (!retval)
                    retval = std::move(tempHom);
                else
                    *retval = tempHom * (*retval);
            } // j0==j1 is the column such that entry (i,j1) is +-1.
            if (i!=j1) {
                nielsenTransposition(i, j1);
                abMat.swapCols(i, j1);
            }
        }
        // now we're at [ P 0 0 ]
        //              [ 0 D 0 ] so we're essentially done.
    } catch (const IntegerOverflow&) {
        throw UnsolvedCase("An integer overflow occurred while attempting "
            "to build a group expression");
    }

    // call prettify
    if (auto h = prettyRewriting()) {
        if (!retval)
            retval = std::move(*h);
        else
            *retval = (*h) * (*retval);
    }

    return retval;
}

// This algorithm has to be at least moderately sophisticated to ensure it
// recognises that < a, b, a^2, abaB > is abelian.
bool GroupPresentation::identifyAbelian() const
{
    // The idea will be to take all commutators of the generators, and see if
    //  the relators can kill them.
    for (size_t i=0; i<nGenerators_; i++)
        for (size_t j=i+1; j<nGenerators_; j++) {
            // let's see if we can recursively apply the relations to
            // [gi,gj] in order to kill it.
            GroupExpression COM; // commutator [gi,gj]
            COM.addTermLast( i, 1 );
            COM.addTermLast( j, 1 );
            COM.addTermLast( i, -1 );
            COM.addTermLast( j, -1 );
            simplifyAndConjugate( COM );
            if (!COM.isTrivial()) return false;
        }
    return true;
}

/**
 * This routine takes two words as input, *this and other, and
 * determines whether or not one can re-label the generators in
 * this word to get the other word. If so, it returns a non-empty
 * list of such re-labellings. Re-labellings are partially-defined
 * permutations (with possible inversions if cyclic=true) on the
 * generator set.
 *
 * \param other is what the return permutation turn
 * \param cyclic, if false we get a list of exact relabellings from
 *  *this to other.  If true, it can be up to cyclic permutation and
 *  inversion. If cyclic is true, the routine demands both words
 *  are cyclically-reduced.
 */
std::list<std::map<size_t, GroupExpressionTerm>>
GroupExpression::relabellingsThisToOther( const GroupExpression &other,
        bool cyclic ) const
{
    // we'll handle the cyclic==true case as a repeated cyclic==false call.
    if (cyclic) {
        std::list<std::map<size_t, GroupExpressionTerm>> retval;
        if (countTerms() != other.countTerms())
            return retval;
        GroupExpression tempW( *this );
        for (size_t i=0; i<tempW.countTerms(); i++) {
            retval.splice( retval.end(),
                tempW.relabellingsThisToOther( other, false ) );
            tempW.cycleRight();
        }
        tempW.invert();
        for (size_t i=0; i<tempW.countTerms(); i++) {
            retval.splice( retval.end(),
                tempW.relabellingsThisToOther( other, false ) );
            tempW.cycleRight();
        }
        return retval;
        // TODO: consider removing duplicates at this stage.  Or make the return
        //  value a set.
    }

    // cyclic==false
    std::map<size_t, GroupExpressionTerm> tempMap;
    auto j=other.terms().begin();
    auto i=terms().begin();
    for ( ; ( (i!=terms().end()) && (j!=other.terms().end()) ); i++, j++)
        if (std::abs(i->exponent) == std::abs(j->exponent)) {
            // matching exponents, so check if generators have been used yet.
            auto k=tempMap.find( i->generator );
            GroupExpressionTerm MAPTO( j->generator,
                                        (i->exponent == j->exponent) ? 1 : -1 );
            if (k!=tempMap.end()) { // previously defined, check consistency
                if (!(k->second == MAPTO)) // contradicting definition
                    return std::list<std::map<size_t, GroupExpressionTerm>>();
            } else tempMap[i->generator] = MAPTO;
        }
    // check if words had different number of terms
    if ( (i!=terms().end()) || (j!=other.terms().end()) )
        return std::list<std::map<size_t, GroupExpressionTerm>>();

    // okay, we have something.
    std::list<std::map<size_t, GroupExpressionTerm>> retval;
    retval.push_back( tempMap );
    return retval;
}

std::list<GroupPresentation> GroupPresentation::identifyFreeProduct() const
{
    // let's create a list of generators not used in the relators, then
    // generators that appear in a common generator, or recursively related
    std::set<size_t> unRelated;
    for (size_t i=0; i<countGenerators(); i++)
        unRelated.insert(i);
    std::list< std::set<size_t> > equivRel;
    // determine which generators are used in the relators, to initialize
    // equivRel.
    {
        // forced scope
        std::set<size_t> usedRels;
        for (const auto& r : relations_)
            for (const auto& t : r.terms())
                usedRels.insert( t.generator );
        for (size_t u : usedRels) {
            std::set<size_t> singleton;
            singleton.insert(u);
            unRelated.erase(u);
            equivRel.push_back(singleton);
        }
    } // end forced scope
    // now we grow the equivalence relation.

    for (const auto& r : relations_) {
        std::set<size_t> relSet; // related by r
        if ( (unRelated.size()==0) && (equivRel.size()==1) )
            break;
        for (const auto& t : r.terms())
            relSet.insert( t.generator );
        if (relSet.size() < 2)
            continue; // in case of empty word
        for (auto I=relSet.begin(); I!=relSet.end(); I++)
            for (auto J=I; J!=relSet.end(); J++) {
                if (I==J) continue;
                std::list<std::set<size_t>>::iterator SI, SJ;
                for (SI=equivRel.begin(); SI!=equivRel.end(); SI++)
                    if (SI->find( *I )!=SI->end()) break;
                for (SJ=equivRel.begin(); SJ!=equivRel.end(); SJ++)
                    if (SJ->find( *J )!=SJ->end()) break;
                if (SI==SJ) continue; // so we merge the sets SI and SJ are pointing to.
                SI->insert( SJ->begin(), SJ->end() );
                equivRel.erase( SJ );
            } // I J loop
    } // i loop

    if ( (equivRel.size()+unRelated.size()) < 2)
        return std::list< GroupPresentation >();
    // build return value. We'll have subgroup free products, and a free group
    // provided unRelated is non-empty.

    std::list<GroupPresentation> retval;
    if (unRelated.size()>0) {
        retval.emplace_back(unRelated.size());
    }
    for (const auto& I : equivRel) {
        GroupPresentation newGrp( I.size() );
        std::map<size_t, size_t> downMap; // old to new map
        size_t count = 0;
        for (size_t J : I) {
            downMap.insert( std::pair<size_t, size_t>(J, count) );
            count++;
        }
        // Build map from this groups generators to corresponding generators of *this
        // decide which relators from *this are relevant.
        for (const auto& r : relations_) {
            // check if r generator is in I
            if (I.find( r.term(0).generator)!=I.end()) { // yes!
                GroupExpression newRel;
                for (const auto& et : r.terms())
                    newRel.addTermLast( downMap[ et.generator ], et.exponent );
                newGrp.addRelation( std::move(newRel) );
            }
        } // end relations i loop
        retval.push_back( std::move(newGrp) );
    }
    return retval;
} // end identifyFreeProduct()


// Routine (at present) looks for isomorphism between *this group and other,
// at present only maps of the form that send generators
//  g_i --> g_{sigma i}^{delta_i} where sigma is some permutation of the
// generators, and delta is some function {0,1,...,ngens-1} --> {+1, -1}
//
// We do this by creating a routine that runs through the relators of this
// group and checks if there are any partial permutations sigma that allow
// that relator to be respected by a map.  It builds up a big list of all
// these partial subs, one list for every relator in *this group.  As we
// iterate through the relators we iteratively check compatibility of these
// subsititions lists, winnowing-down the list of substitions as we go.
// Once done, if non-empty that would define the map
// on all generators other than free factors, so then we have to similarly
// check for free factors in other.  Then we check the inverse (in the free
// group) descends to a map, if so we're done.
//
// To enable this we should probably carefully index the relations. And we
// should handle 1-gen relations differently than multiple-gen relations,
// otherwise there's a potential memory explosion.
//
// TODO: we can modify this to be a findHom routine. And if the target is
// a finite group, find *all* homs up to conjugacy, etc.
//
bool GroupPresentation::identifySimplyIsomorphicTo(
        const GroupPresentation& other) const
{
    // Check if presentations have the same number of generators.
    if (nGenerators_ != other.nGenerators_) return false;
    // Check if relations empty
    if ( (relations_.empty()) && (other.relations_.empty()) ) return true;
    if ( (relations_.empty()) || (other.relations_.empty()) ) return false;
    // Both relations_.size()>0, and have the same number of generators.

    // list of related by number of generators appearing
    std::map<size_t, std::list<const GroupExpression*>> domRelIdx, ranRelIdx;

    for (const GroupExpression& r : relations_) {
        std::set<size_t> gensUsed;
        for (const auto& t : r.terms())
            gensUsed.insert( t.generator );
        domRelIdx[ gensUsed.size() ].push_back(&r);
    }
    for (const GroupExpression& r : other.relations_) {
        std::set<size_t> gensUsed;
        for (const auto& t : r.terms())
            gensUsed.insert( t.generator );
        ranRelIdx[ gensUsed.size() ].push_back(&r);
    }

    // for each relator of this we have lists of potential substitutions
    std::list<std::map<size_t, GroupExpressionTerm>> allPartialSubs;
    allPartialSubs.emplace_back();

    for (auto i=domRelIdx.rbegin(); i!=domRelIdx.rend(); i++) {
        // currently we'll do the most simplistic thing possible -- look for relabellings
        // of these relators in the target presentation.
        size_t nGens = i->first;
        if (ranRelIdx.find(nGens)==ranRelIdx.end()) return false;

        const std::list< const GroupExpression* > DOMR( i->second );
        const std::list< const GroupExpression* > RANR( ranRelIdx[nGens] );
        // build list of subs for all DOMR -> RANR possibilities
        for (const GroupExpression* DI : DOMR) {
            std::list<std::map<size_t, GroupExpressionTerm>> newPartialSubs;
            // for each DI, every extension or consistent hom with allPArtialSubs
            // we find using DI will be put in newPartialSubs, at the end, we replace
            // allPartialSubs with newPartialSubs.
            for (const GroupExpression* RI : RANR) {
                // built tempList
                // TODO: let's put the special case nGens==1 here, where instead of
                //  looking making all possible maps, we just choose one.  This is
                //  because if we get here and its not defined on a torsion element,
                //  is must have been a free factor Z_k * other stuff.  So we only
                //  need to choose a complementary map.

                std::list<std::map<size_t, GroupExpressionTerm>>
                    tempList = DI->relabellingsThisToOther(*RI, true);
                for (const auto& X : tempList)
                    for (const auto& Y : allPartialSubs) {
                        // newMap will be the potential extension of X and Y
                        std::map<size_t, GroupExpressionTerm> newMap;
                        auto Xi=X.begin();
                        auto Yi=Y.begin();
                        while ( (Xi!=X.end()) || (Yi!=Y.end()) ) {
                            if ( (Xi!=X.end()) && (Yi!=Y.end()) ) {
                                if (Xi->first < Yi->first) {
                                    newMap.insert( *Xi );
                                    Xi++;
                                } else if (Xi->first > Yi->first) {
                                    newMap.insert( *Yi );
                                    Yi++;
                                } else if (Xi->second == Yi->second) {
                                    newMap.insert( *Xi );
                                    Xi++;
                                    Yi++;
                                } else // this does not extend
                                    goto get_out_of_while_loop_goto_tag;
                            } else if (Xi!=X.end()) {
                                newMap.insert( *Xi );
                                Xi++;
                            } else if (Yi!=Y.end()) {
                                newMap.insert( *Yi );
                                Yi++;
                            }
                        }
                        newPartialSubs.push_back( newMap );
get_out_of_while_loop_goto_tag: { // this skips insertion
                        } // goto tags apparently can't be at the end of while loops!
                    }
            } // end RI loop
            if (newPartialSubs.empty()) return false;
            allPartialSubs.clear();
            allPartialSubs.swap( newPartialSubs );
            // TODO: Remove duplicates if they exist. This would help reduce time
            //  wasted
        } // end DI and newPartialSubs loop
    }

    // TODO: if still undefined, there are some free factors.  Count them on
    //  both sides then define.

    for (const auto& X : allPartialSubs) {
        size_t gi=0;
        std::set<size_t> rGen;
        for (const auto& GI : X) {
            rGen.insert(GI.second.generator);
            if (GI.first!=gi) break;
            else gi++;
        }
        if ( (rGen.size()==nGenerators_) && (gi==nGenerators_) ) {
            std::vector< GroupExpression > map(nGenerators_);
            for (const auto& GI : X) {
                GroupExpression let;
                let.addTermFirst( GI.first, GI.second.exponent );
                map[GI.second.generator] = let;
            }
            HomGroupPresentation invMap( other, *this, map );
            if (invMap.verify()) { //std::cout<<invMap.detail()<<"\n";
                return true;
            }
        }
    }

    return false;
}


bool GroupPresentation::nielsenTransposition(size_t i, size_t j)
{
    if (i==j) return false;
    bool retval=false;
    for (GroupExpression& r : relations_) {
        for (GroupExpressionTerm& t : r.terms()) {
            if (t.generator == i) {
                t.generator = j;
                retval = true;
            } else if (t.generator == j) {
                t.generator = i;
                retval = true;
            }
        }
    }
    return retval;
}

bool GroupPresentation::nielsenInvert(size_t i)
{
    bool retval=false;
    for (GroupExpression& r : relations_) {
        for (GroupExpressionTerm& t : r.terms()) {
            if (t.generator == i) {
                t.exponent=(-t.exponent);
                retval = true;
            }
        }
    }
    return retval;
}

bool GroupPresentation::nielsenCombine(size_t i, size_t j,
                                       long k, bool rightMult)
{
    if (k == 0) return false;
    // replace ri with (ri)(rj)^(-k)
    bool retval(false);
    GroupExpression let;
    if (rightMult) {
        let.addTermFirst(i, 1);
        let.addTermLast(j, -k);
    } else {
        let.addTermLast(i, 1);
        let.addTermFirst(j, -k);
    }
    for (GroupExpression& r : relations_)
        if (r.substitute(i, let, true))
            retval = true;
    return retval;
}

// this macro is used only in the identifyExtensionOverZ routine below.
#define idx(gen, cov) static_cast<size_t>(((gen)-1)+nGm1*(cov))

// if presentation is of a group that can bet written as an extension
//  0 --> A --> G --> Z --> 0
// this routine is to change the presentation to appear to be such a split
//  extension.
//
// TODO: at present it will not declare presentations of the form:
//  < a, b | a^5, bab^-1=a^2 > extensions over Z, because of the
// a^2 term.  Should fix this.  But how to do it in any generality?
// Perhaps multiply conjugating automorphisms, to deduce
//  < a, b | a^5, bab^-1=a^2, ba^2b^-1=a^4=a^-1 > etc...
// Short-term we can recognise the fibre as being abelian and check
// invertibility using HomMarkedAbelianGroup routines.
//
std::optional<HomGroupPresentation> GroupPresentation::identifyExtensionOverZ()
{
    // step 1: let's build the abelianization homomorphism.
    try {
        homologicalAlignment();
    } catch (const UnsolvedCase&) {
        // An integer overflow occurred during homologicalAlignment().
        // Just declare that the algorithm did not succeed.
        return {};
    }
    MarkedAbelianGroup abelianized = markedAbelianisation();
    if (abelianized.rank() != 1)
        return {};
    if (abelianized.countInvariantFactors()>0)  // put Z generator at 0-th
        nielsenTransposition(0, abelianized.countInvariantFactors() );

    // We have the presentation of this group in the form
    // < a, g1, g2, ..., gn | r1, ..., rm > with a->1, gi->0 under abelianization
    //
    // step 2: An infinite presentation of the kernel of the map to Z is given
    //  by < g1i, g2i, ..., gni | r1i, ..., rmi > for all lifts i of the generators
    //  and relators above, after collapsing "a". We can collapse this to a finite
    //  presentation if and only if unique max and minima (in the Z cover) exist
    //  among the lifted relators.  So we check for that.

    // lifts stores the lifts of the ri's, after crushing the lifts of the a's.
    std::vector< std::list< std::pair< GroupExpressionTerm, signed long > > >
        lifts( relations_.size() );

    // the following max/minKiller give a list of the found pairs
    //  (generator index, relator index) to keep track of which relators we can
    //  use to kill generators (in the covering space).
    std::map<size_t, size_t> maxKiller;
    std::map<size_t, size_t> minKiller;
    std::map<size_t, unsigned long> cellWidth; // 2-cell width in cover

    for (size_t l=0; l<relations_.size(); l++) {
        // for each relator determine highest and lowest lifts, and if they
        // are unique or not.

        signed long lift = 0;
        signed long maxLift = 0, minLift = 0;   // sheet index
        size_t maxCell = 0, minCell = 0; // generator's index in presentation
        bool dupMax(false), dupMin(false);    // duplicate lift height?
        std::list<GroupExpressionTerm>& terms(relations_[l].terms());
        for (auto k=terms.rbegin(); k!=terms.rend(); k++) {
            // right to left through the relator
            if (k->generator > 0) {
                lifts[l].emplace_back( *k, lift );
                // special case if maxCell and minCell not yet initialized.
                if (maxCell==0) {
                    maxLift = lift;
                    minLift = lift;
                    maxCell = k->generator;
                    minCell = k->generator;
                    dupMax = (labs(k->exponent)==1) ? false : true;
                    dupMin = (labs(k->exponent)==1) ? false : true;
                } else { // back to regular case
                    if (lift > maxLift) {
                        maxLift = lift;
                        dupMax = (labs(k->exponent)==1) ? false : true;
                        maxCell = k->generator;
                    } else if (lift==maxLift) {
                        dupMax = true;
                    }
                    if (lift < minLift) {
                        minLift = lift;
                        dupMin = (labs(k->exponent)==1) ? false : true;
                        minCell = k->generator;
                    } else if (lift==minLift) {
                        dupMin = true;
                    }
                }
            } else lift += k->exponent;
        }
        // maxCell and minCell have to be non-zero at this point.
        cellWidth[l] = static_cast<unsigned long>(maxLift - minLift);

        if ( (maxCell!=0) && (!dupMax) ) {
            auto I=maxKiller.find(maxCell);
            if (I!=maxKiller.end()) { // compare the current maxKiller[maxCell] to l.
                if (cellWidth[l] > cellWidth[ I->second ]) maxKiller[maxCell]=l;
            } else maxKiller[maxCell]=l;
        }
        if ( (minCell!=0) && (!dupMin) ) {
            auto I=minKiller.find(minCell);
            if (I!=minKiller.end()) { // compare the current maxKiller[minCell] to l.
                if (cellWidth[l] > cellWidth[ I->second ]) minKiller[minCell]=l;
            }
            minKiller[minCell]=l;
        }
        // now let's readjust the relator so that its minLift is at level 0.
        if (minLift != 0) {
            relations_[l].addTermFirst(0, minLift);
            relations_[l].addTermLast(0, -minLift);
            relations_[l].simplify();
            for (auto& I : lifts[l])
                I.second -= minLift; // adjust the lifts to have min lift 0
        }
        // cyclically permute lifts so that the max-weight rep appears first
        while (lifts[l].front().second != static_cast<long>(cellWidth[l])) {
            auto temp = lifts[l].front();
            lifts[l].pop_front();
            lifts[l].push_back( temp );
        }
        // ensure word starts with highest-weight element as inverted.
        if (lifts[l].front().first.exponent == 1) {
            auto temp = lifts[l].front();
            lifts[l].pop_front();
            lifts[l].reverse();
            lifts[l].push_front(temp); // now run and change the exponents
            for (auto& I : lifts[l])
                I.first.exponent = -I.first.exponent;
        }
    }
    // this is the test for whether or not we can find a finite collection of
    // generators
    size_t nGm1 = nGenerators_ - 1;
    if ( (maxKiller.size() != nGm1) || (minKiller.size() != nGm1) )
        return {};

    unsigned long maxWidth = 0;
    unsigned long liftCount = 0; // how many lifts of our generators do we need?
    for (size_t i=0; i<maxKiller.size(); i++) {
        if (cellWidth[maxKiller[i]]>liftCount)
            liftCount = cellWidth[maxKiller[i]];
        if (cellWidth[minKiller[i]]>liftCount)
            liftCount = cellWidth[minKiller[i]];
    }
    for (const auto& I : cellWidth)
        if (I.second > maxWidth)
            maxWidth = I.second;
    // we need liftCount lifts of our generators and relators.  Perhaps we should
    // either cite something in Magnus-Karass-Solitar for this or put in a proof.
    // let's build a vector that describes the relationa a(gi)a^-1 = ...

    // build table of reductions of the liftCount==M lift of generators.
    // the indexing of the generators of the kernel of G --> Z will be handled
    // by the above idx macro.
    std::map<size_t, GroupExpression> genKiller;
    // start with the liftCount lift, i.e. the first order reducers a^-Mg_ia^M =...
    for (size_t i=1; i<countGenerators(); i++) {
        GroupExpression temp;
        // maxKiller[i] is the index in lifts of the relator that kills generator gi
        // i is a liftIdx
        unsigned long delta = 0;
        for (auto I=lifts[maxKiller[i]].begin(); I!=lifts[maxKiller[i]].end(); I++) {
            if (I==lifts[maxKiller[i]].begin()) {
                // push up delta sheets so that it kills appropriately
                delta = static_cast<unsigned long>(liftCount - I->second);
                continue;
            }
            temp.addTermFirst(idx(I->first.generator, I->second + delta), I->first.exponent);
        }
        genKiller.emplace(idx(i,liftCount), std::move(temp));
    }

    // extra genKillers -- sometimes there are wider words than the killing words.
    //  like with presentations such as:
    //   < a b | b a^-1 b a^-1 b^-1 a^2, a^-3 b^2 a^3 b^2 >
    // We could alternatively use the genKiller to reduce the width of the other
    // relators.  But for now we use this less-sophisticated work-around.
    for (unsigned long j=liftCount; j<maxWidth; j++) {
        for (size_t i=1; i<countGenerators(); i++) {
            // bump-up lift of each genKiller then apply previous genKillers to them
            // to create word in the fibre group.
            GroupExpression tempW( genKiller[idx(i, j)] );
            for (auto& I : tempW.terms())
                I.generator += nGm1;
            for (const auto& J : genKiller)
                tempW.substitute( J.first, J.second, false );
            genKiller.emplace(idx(i,j+1), std::move(tempW));
        }
    }

    //  initialize tempTable with the 0-th lifts of the relators.
    std::list< GroupExpression > tempTable;
    GroupPresentation kerPres;
    kerPres.addGenerator( liftCount * nGm1 );

    for (const auto& lift : lifts) {
        GroupExpression temp;
        for (const auto& I : lift)
            temp.addTermFirst(idx(I.first.generator, I.second), I.first.exponent);
        for (const auto& J : genKiller)
            temp.substitute( J.first, J.second, false );
        temp.simplify();
        if (temp.wordLength()>0) {
            tempTable.push_back(temp);
            kerPres.addRelation( std::move(temp) );
        }
    }
    if (!kerPres.isValid()) {
        std::cout<<"identifyExtensionOverZ() error:"<<
                 " out of bounds relator in kerPres.\n";
        std::cout.flush();
        exit(1);
    }
    // build the reductions of the {0,1,...,liftCount-1} translates of **all**
    // the relators from the group, and assemble them into the relators of the
    // kernel.
    for (unsigned long M=0; M<liftCount; M++) {
        // increment the words in tempTable
        for (auto& I : tempTable) {
            for (auto& J : I.terms())
                J.generator += nGm1; // this depends on choice of idx function
            for (const auto& J : genKiller)
                I.substitute( J.first, J.second, false );
            // apply genKiller to reduce the words, and push to presentation
            kerPres.addRelation(I);
        }
    }
    // replace this presentation by the semi-direct product presentation.
    std::vector<GroupExpression> autVec;

    autVec.resize( nGm1*liftCount );
    for (size_t i=0; i<autVec.size(); i++) // this part depends on idx
        if ( i >= nGm1*(liftCount-1) )
            autVec[i] = genKiller[i+nGm1];
        else {
            GroupExpression temp;
            temp.addTermFirst( i+nGm1, 1 );
            autVec[i] = temp;
        }

    // TODO: Possibly we could use std::move() on one of the copies of
    // kerPres in the line below, to save one of the two deep copies
    // that we are currently making.
    HomGroupPresentation retval( kerPres, kerPres, autVec );
    retval.simplify();

    // Modify this presentation to reflect the semi-direct product
    //        structure we've discovered!
    // deallocate relations, and resize and repopulate with copies of kerPres's
    //  relations.

    nGenerators_ = retval.domain().nGenerators_ + 1;
    relations_.resize( retval.domain().nGenerators_ +
                       retval.domain().relations_.size() );

    for (size_t i=0; i<retval.domain().relations_.size(); i++)
        relations_[i] = retval.domain().relations_[i];

    // And now all the b^-1g_ib = genKiller(i) and b^-1g_ib = g_{i+1} relations.
    for (size_t i=0; i<retval.domain().nGenerators_; i++) {
        GroupExpression temp = retval.evaluate(i);
        temp.addTermFirst( retval.domain().nGenerators_, 1);
        temp.addTermFirst( i, -1);
        temp.addTermFirst( retval.domain().nGenerators_, -1);
        relations_[ i+retval.domain().relations_.size() ] = std::move(temp);
    }

    return retval;
}


bool GroupPresentation::isValid() const {
    for (const auto& r : relations_)
        for (const auto& t : r.terms())
            if (t.generator >= nGenerators_)
                return false;
    return true;
}


/*
 * This is an entirely cosmetic re-writing of the presentation, is
 * fast and superficial.
 *  1) It inverts relators if net sign of the generators is negative.
 *  2) It cyclically permutes relators to start with smallest gen.
 *  3) It sorts the relators by number of generator indices that
 *     appear, followed by relator numbers (lexico) followed by
 *     relator length.
 *  4) Makes elementary simplifications to aid in seeing standard
 *     relators like commutators.
 */
namespace { // anonymous namespace
    bool compare_words(const GroupExpression& first,
                       const GroupExpression& second)
    {
    // compute number of letters used
        std::set<size_t> usedTermsF, usedTermsS;
        for (const auto& j : first.terms())
            usedTermsF.insert( j.generator );
        for (const auto& j : second.terms())
            usedTermsS.insert( j.generator );
        if (usedTermsF.size() < usedTermsS.size()) return true;
        if (usedTermsF.size() > usedTermsS.size()) return false;
    // have all the same terms, sort lexicographically on which terms used
        auto i=usedTermsF.begin();
        auto j=usedTermsS.begin();
        while (i != usedTermsF.end()) {
            if (*i < *j) return true;
            if (*i > *j) return false;
            i++;
            j++;
        }
    // the two words first and second use exactly the same letters.
        if (first.wordLength() < second.wordLength()) return true;
        if (first.wordLength() > second.wordLength()) return false;
        if (first.countTerms() < second.countTerms()) return true;
        if (first.countTerms() > second.countTerms()) return false;
    // now we can compare them lexicographically, letter by letter.
    // first we lay them out one letter at a time.
        std::vector<GroupExpressionTerm> first_word_vec, second_word_vec;
        first_word_vec.reserve( first.wordLength() );
        second_word_vec.reserve( second.wordLength() );
        for (const auto& t : first.terms()) {
            for (long I=0; I<std::abs(t.exponent); I++)
                first_word_vec.emplace_back( t.generator,
                                             (t.exponent>0) ? 1 : -1 );
        }
        for (const auto& t : second.terms()) {
            for (long I=0; I<std::abs(t.exponent); I++)
                second_word_vec.emplace_back( t.generator,
                                              (t.exponent>0) ? 1 : -1 );
        }
    // now we compare termwise
        for (size_t I=0; I<first_word_vec.size(); I++) {
            if (first_word_vec[I].generator < second_word_vec[I].generator) return true;
            if (first_word_vec[I].generator > second_word_vec[I].generator) return false;
            if (first_word_vec[I].exponent  < second_word_vec[I].exponent) return true;
            if (first_word_vec[I].exponent  > second_word_vec[I].exponent) return false;
        }

    // exactly the same words.
        return false;
    }
} // end anonymous namespace

// this routine iteratively finds length 1 relators, and uses them to simplify
// other relators.  In the end it deletes all length 0 relators and re-indexes.
std::optional<HomGroupPresentation> GroupPresentation::prettyRewriting() {
    // IMPORTANT NOTE: This routine *must* be deterministic.
    // This matters (for example) when we simplify the domain and codomain
    // of a monodromy, and we need both ends to simplify identically.

    GroupPresentation oldPres(*this);

    // move the relators into a separate list for now.
    std::vector<GroupExpression> relatorPile;
    relatorPile.swap(relations_);
    // now relations_ is empty.

    // step 1: cyclic reduce relators. Delete length 0 relators.
    //         delete generators corresponding to length 1 relators
    for (auto& r : relatorPile)
        r.simplify(true);

    std::set<size_t> genToDel; // generators we've eliminated
    bool reloopFlag(true);
    while (reloopFlag) {
        reloopFlag=false;
        std::set<size_t> newGenDel;
        for (const auto& r : relatorPile) {
            if (r.countTerms() == 1)
                if ( std::abs( r.terms().front().exponent ) == 1 ) {
                    // a killer!
                    newGenDel.insert( r.terms().front().generator );
                }
        }
        genToDel.insert( newGenDel.begin(), newGenDel.end() );

        for (size_t i : newGenDel)
            for (auto& r : relatorPile)
                if (r.substitute( i, GroupExpression(), true ))
                    reloopFlag=true;
    }

    relations_.reserve( relatorPile.size() - genToDel.size() );
    for (auto& r : relatorPile)
        if ( r.countTerms()>0 )
            relations_.push_back( std::move(r) );
    relatorPile.clear();

    // We are not planning to delete any more relations, and so we work
    // directly with the relations_ vector from here on.

    std::optional<HomGroupPresentation> redMap;
    if (genToDel.size()>0) {
        std::set<size_t> interval, compDelete; // complement
        for (size_t i=0; i<nGenerators_; i++)
            interval.insert(interval.end(), i);
        std::set_difference( interval.begin(), interval.end(),
                             genToDel.begin(), genToDel.end(),
                             std::inserter(compDelete, compDelete.end() ) );

        // then reduce the group, run through genToDel and do gi->1 subs on all
        //  relators, and gk --> gk-1 for larger generators.
        std::vector< GroupExpression > downSub(nGenerators_);
        std::vector< GroupExpression > upSub(nGenerators_ - genToDel.size());
        size_t i=0;
        for (size_t I : compDelete) {
            upSub[i].addTermFirst(I, 1);
            downSub[I].addTermFirst(i, 1);
            // might as well perform downSub now on all relators.
            for (auto& r : relations_)
                r.substitute( I, downSub[I], true);
            i++;
        }
        nGenerators_ -= genToDel.size();
        // assemble the reduction map
        redMap = HomGroupPresentation(std::move(oldPres), *this, downSub, upSub);
    }

    // WARNING: Do not use oldPres past this point, since we may have
    // just moved its contents out.

    // step 2: sort by number of letters present, followed by word length
    //         to do this, we build a list of relators and a sorting criterion.
    std::sort(relations_.begin(), relations_.end(), compare_words);

    // step 3: if there is a relator with net sign < 0, invert it
    for (auto& r : relations_) {
        // add up signs
        int sig = 0;
        for (const auto& t : r.terms())
            sig += t.exponent;
        if (sig < 0)
            r.invert();
    }

    // step 4: for each generator find first relator where it appears with
    // non-zero weight, if negative, invert the generator.
    // TODO maybe...

    // step 5: cyclically permute to start with a lexicographically smallest
    //         term
    for (auto& r : relations_)
        if (r.countTerms()>0) {
            // form list of all terms involved, find smallest, cyclically permute to
            //  start with that one.
            std::set<size_t> usedTerms;
            for (const auto& t : r.terms())
                usedTerms.insert( t.generator );
            size_t smallestGen = *usedTerms.begin();
            while ( r.term(0).generator != smallestGen )
                r.cycleRight();
        }

    return redMap;
}


////////////////// ALL INPUT / OUTPUT routines below //////////////////////

// XML output

void GroupPresentation::writeXMLData(std::ostream& out) const {
    out << "<group generators=\"" << nGenerators_ << "\">\n";
    for (const auto& r : relations_) {
        out << "  ";
        r.writeXMLData(out);
        out << '\n';
    }
    out << "</group>\n";
}

void GroupExpression::writeXMLData(std::ostream& out) const {
    out << "<reln> ";
    for (const GroupExpressionTerm& t : terms_)
        out << t.generator << '^' << t.exponent << ' ';
    out << "</reln>";
}

// group expression output routines

std::string GroupExpression::tex() const {
    std::ostringstream out;
    writeTeX(out);
    return std::move(out).str();
}

void GroupExpression::writeTeX(std::ostream& out) const {
    if (terms_.empty())
        out << 'e';
    else {
        for (const auto& t : terms_) {
            out << "g_{" << t.generator << '}';
            if ( t.exponent != 1 )
                out << "^{" << t.exponent << '}';
        }
    }
}

void GroupExpression::writeTextShort(std::ostream& out, bool utf8,
        bool alphaGen) const {
    if (terms_.empty())
        out << '1';
    else {
        for (auto i = terms_.begin(); i!=terms_.end(); i++) {
            if (i != terms_.begin()) {
                if (utf8 && ! alphaGen) {
                    // Spaces get lost between g012 g456 ...
                    out << " \u00b7 "; // \cdot
                } else
                    out << ' ';
            }
            if (alphaGen)
                out << char('a' + i->generator);
            else
                out << 'g' << i->generator;
            if (i->exponent != 1) {
                if (utf8)
                    out << regina::superscript(i->exponent);
                else
                    out << '^' << i->exponent;
            }
        }
    }
}

// presentation output routines below

std::string GroupPresentation::tex() const {
    std::ostringstream out;
    writeTeX(out);
    return std::move(out).str();
}

void GroupPresentation::writeTeX(std::ostream& out) const {
    out << "\\langle ";
    if (nGenerators_ == 0) out << "\\cdot";
    else if (nGenerators_ == 1) out << "g_0";
    else if (nGenerators_ == 2) out << "g_0, g_1";
    else {
        out << "g0, \\cdots, g" <<(nGenerators_ - 1);
    }
    out << " | ";
    if (relations_.empty())
        out << "\\cdot";
    else {
        bool first = true;
        for (const auto& r : relations_) {
            if (first)
                first = false;
            else
                out << ", ";
            r.writeTeX(out);
        }
    }
    out << " \\rangle";
}

void GroupPresentation::writeTextLong(std::ostream& out) const {
    out << "Generators: ";
    if (nGenerators_ == 0)
        out << "(none)";
    else if (nGenerators_ == 1)
        out << "a";
    else if (nGenerators_ == 2)
        out << "a, b";
    else if (nGenerators_ <= 26)
        out << "a .. " << char('a' + nGenerators_ - 1);
    else
        out << "g0 .. g" << (nGenerators_ - 1);
    out << std::endl;

    out << "Relations:\n";
    if (relations_.empty())
        out << "    (none)\n";
    else
        for (const auto& r : relations_) {
            out << "    ";
            r.writeTextShort(out, false /* utf8 */, nGenerators_ <= 26);
            out << std::endl;
        }
}

std::string GroupPresentation::compact() const {
    std::ostringstream out;
    writeTextCompact(out);
    return std::move(out).str();
}

void GroupPresentation::writeTextCompact(std::ostream& out) const {
    if (nGenerators_ == 0) {
        out << "< >";
        return;
    }

    out << "<";
    if (nGenerators_ <= 26) {
        for (size_t i=0; i<nGenerators_; i++)
            out << ' ' << char('a' + i);
    } else {
        out << " g0 .. g" << (nGenerators_ - 1);
    }

    if (relations_.empty()) {
        out << " >";
        return;
    }

    out << " | ";
    bool first = true;
    for (const auto& r : relations_) {
        if (first)
            first = false;
        else
            out << ", ";
        r.writeTextShort(out, false /* utf8 */, nGenerators_ <= 26);
    }
    out << " >";
}


// We will go through, apply dehnAlgSubMetric look for substitutions,
//  then apply all of them within a reasonable length.
// if user requests we will go further and do a 2nd iteration with more care...
// depth==1 by default.

void GroupPresentation::proliferateRelators(int depth) {
    std::list< GroupExpression > newRels;
    for (size_t i=0; i<relations_.size(); i++)
        for (size_t j=0; j<relations_.size(); j++) {
            if (i==j)
                continue; // TODO: maybe accept novel self-substitutions?
            auto sub_list =
                dehnAlgorithmSubMetric<SetAggregator<WordSubstitutionData>>(
                relations_[i], relations_[j], depth);
            while (!sub_list.empty()) {
                GroupExpression newRel( relations_[i] );
                applySubstitution( newRel, relations_[j], *sub_list.begin() );
                sub_list.erase( sub_list.begin() );
                newRels.push_back(std::move(newRel));
            }
        }
    --depth;
    while (depth>0) {
        std::list< GroupExpression > tempRels;
        for (const auto& r : relations_)
            for (const auto& j : newRels) {
                // attempt to tack r to j. To do this, we should perhaps keep
                // a record of how j was created, as in where the two junction
                // points are so as to ensure what we're adding spans at least
                // one of the junctions.
                auto sub_list = dehnAlgorithmSubMetric<
                    SetAggregator<WordSubstitutionData>>( j, r, depth );
                while (!sub_list.empty()) {
                    // TODO: we might want to avoid some obviously repetitive
                    //       subs as noted above?
                    GroupExpression newRel( j );
                    applySubstitution( newRel, r, *sub_list.begin() );
                    sub_list.erase( sub_list.begin() );
                    tempRels.push_back(std::move(newRel));
                }
            }
        --depth;

        // Move our newly generated tempRels onto the end of newRels.
        newRels.splice(newRels.end(), std::move(tempRels));
    }

    // Finally, move *all* of our new relations onto the end of relations_.
    for (auto& r : newRels)
        relations_.push_back(std::move(r));
}

Matrix<bool> GroupPresentation::incidence() const {
    Matrix<bool> inc(relations_.size(), nGenerators_);
    inc.fill(false);

    size_t row = 0;
    for (const auto& r : relations_) {
        for (const auto& t : r.terms())
            inc.entry(row, t.generator) = true;
        ++row;
    }

    return inc;
}

std::string GroupPresentation::gap(const std::string& groupVariable) const {
    std::ostringstream out;
    size_t g;

    out << groupVariable << " := CallFuncList(function() local F";
    for (g = 0; g < nGenerators_; ++g)
        out << ", x" << g;
    out << "; F := FreeGroup(";
    for (g = 0; g < nGenerators_; ++g) {
        if (g > 0)
            out << ", ";
        out << "\"x" << g << '\"';
    }
    out << "); ";
    for (g = 0; g < nGenerators_; ++g)
        out << 'x' << g << " := F." << (g+1) << "; ";
    out << "return F/[";
    bool firstReln = true;
    for (const auto& r : relations_) {
        if (r.terms().empty())
            continue;

        if (firstReln)
            firstReln = false;
        else
            out << ", ";

        bool firstGen = true;
        for (const auto& t : r.terms()) {
            if (firstGen)
                firstGen = false;
            else
                out << '*';
            out << 'x' << t.generator;
            if (t.exponent != 1)
                out << '^' << t.exponent;
        }
    }
    out << "]; end,[]);";
    return std::move(out).str();
}

} // namespace regina

