
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
#include "utilities/stringutils.h"

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
        if ((*next).exponent == 0) {
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

bool GroupExpression::substitute(unsigned long generator,
        const GroupExpression& expansion, bool cyclic) {
    bool changed = false;
    GroupExpression* inverse = nullptr;
    const GroupExpression* use;
    long exponent, i;
    for (auto current = terms_.begin(); current != terms_.end(); ) {
        if (current->generator != generator)
            current++;
        else {
            exponent = current->exponent;
            if (exponent != 0) {
                if (exponent > 0)
                    use = &expansion;
                else {
                    if (! inverse)
                        inverse = new GroupExpression(expansion.inverse());
                    use = inverse;
                    exponent = -exponent;
                }

                // Fill in exponent copies of use.
                //
                // Note that the following insertion will invalidate
                // current if the wrong type of data structure is being used!
                for (i = 0; i < exponent; i++)
                    terms_.insert(current, use->terms_.begin(), use->terms_.end());
            }

            current = terms_.erase(current);
            changed = true;
        }
    }
    delete inverse;
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
//       Free products with amalgamation. Currently intelligentSimplify()
//       isn't smart enough for this.
std::string GroupPresentation::recogniseGroup(bool moreUtf8) const {
    std::ostringstream out;

    // Run through cases.
    if (nGenerators_ == 0)
        { out << 0; return out.str(); }

    // Let's record the abelianisation.
    AbelianGroup ab =  abelianisation();

    // abelian test
    if (identifyAbelian()) {
        ab.writeTextShort(out, moreUtf8);
        return out.str();
    }

    // not (clearly) abelian.  Check if free.
    if (relations_.empty()) {
        out << "Free(" << nGenerators_ << ")";
        return out.str();
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
                unsigned long numGen = AUT->domain().countGenerators();
                for (unsigned long i=0; i<numGen; i++) {
                    if (i!=0)
                        out<<", ";
                    if (numGen<27)
                        out<<( (char) (i+97) );
                    else
                        out<<"g"<<i;
                    out<<" \u21A6 "; // mapsto symbol in unicode
                    AUT->evaluate(i).writeTextShort(out, moreUtf8, numGen<27);
                }
                return out.str();
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
        return out.str();
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

unsigned long GroupPresentation::abelianRank() const {
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

    // Matrix::rowEchelonForm() returns the rank of the relations matrix.
    return nGenerators_ - m.rowEchelonForm();
}

MarkedAbelianGroup GroupPresentation::markedAbelianisation() const {
    // create presentation matrices to pass to MarkedAbelianGroup(M, N)
    MatrixInt N(countGenerators(), countRelations() );
    // run through rels, increment N entries appropriately
    for (unsigned long j=0; j<countRelations(); j++) {
        GroupExpression Rj ( relation(j) );
        for (unsigned long i=0; i<Rj.countTerms(); i++)
            N.entry( Rj.generator(i), j ) += Rj.exponent(i);
    }
    return MarkedAbelianGroup(
        MatrixInt(1, countGenerators()) /* zero matrix */,
        std::move(N));
}

void GroupPresentation::dehnAlgorithmSubMetric(
    const GroupExpression &this_word,
    const GroupExpression &that_word,
    std::set<WordSubstitutionData> &sub_list, unsigned long step )
{
    size_t this_length = this_word.wordLength();
    size_t that_length = that_word.wordLength();
    // generic early exit strategy
    if ( (this_length < 2) || (that_length==0) )
        return;
    // early exit strategy based on step.
    if ( (step==1) && ((step+1)*this_length < that_length) )
        return;
    // TODO: should check to whatever extent the above is of much use...

    // this -> splayed to this_word, that_word -> reducer
    std::vector<GroupExpressionTerm> this_word_vec, reducer;
    this_word_vec.reserve( this_length );
    reducer.reserve( that_length );
    for (const auto& t : this_word.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            this_word_vec.emplace_back( t.generator,
                                        (t.exponent>0) ? 1 : -1 );
    }
    for (const auto& t : that_word.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            reducer.emplace_back( t.generator,
                                  (t.exponent>0) ? 1 : -1 );
    }
    std::vector< GroupExpressionTerm > inv_reducer( that_length );
    for (size_t i=0; i<reducer.size(); i++)
        inv_reducer[that_length-(i+1)] = reducer[i].inverse();

    // search for cyclic subwords of reducer in this_word_vec...
    for (size_t i=0; i<this_length; i++)
        for (size_t j=0; j<that_length; j++) {
            size_t comp_length = 0;
            while ( (this_word_vec[(i+comp_length) % this_length] ==
                     reducer[(j+comp_length) % that_length]) &&
                    (comp_length < that_length) && (comp_length < this_length) )
                comp_length++;
            WordSubstitutionData subData;
            subData.invertB=false;
            subData.sub_length=comp_length;
            subData.start_sub_at=i;
            subData.start_from=j;
            if (comp_length == that_length) {
                subData.score = that_length;
                size_t a=1;
                while ( (this_word_vec[( (i+this_length)-a )%this_length].inverse()==
                         this_word_vec[( (i+comp_length)+(a-1) )%this_length]) &&
                        (2*a+that_length <= this_length ) ) {
                    a++;
                    subData.score++;
                }
                sub_list.insert(subData);
            } else if ( comp_length > 0 ) {
                subData.score = 2*comp_length - that_length;
                if ( subData.score > -((long int)step) )
                    sub_list.insert(subData);
            }
            // and the corresponding search with the inverse of reducer.
            comp_length = 0;
            while ( (this_word_vec[(i+comp_length) % this_length] ==
                     inv_reducer[(j+comp_length) % that_length]) &&
                    (comp_length < that_length) && (comp_length < this_length) )
                comp_length++;
            subData.invertB=true;
            subData.sub_length=comp_length;
            if (comp_length == that_length) {
                subData.score = that_length;
                size_t a=1;
                while ( (this_word_vec[( (i+this_length)-a )%this_length].inverse()==
                         this_word_vec[( (i+comp_length)+(a-1) )%this_length]) &&
                        (2*a+that_length <= this_length ) ) {
                    a++;
                    subData.score++;
                }
                sub_list.insert(subData);
            } else if ( comp_length > 0 ) {
                subData.score = 2*comp_length - that_length;
                if ( subData.score > -((long int)step) )
                    sub_list.insert(subData);
            }
        }
}

/**
 *  This applies a substitution generated by dehnAlgorithmSubMetric.
 */
void GroupPresentation::applySubstitution( GroupExpression& this_word,
        const GroupExpression &that_word,
        const WordSubstitutionData &sub_data )
{
    // okay, so let's do a quick cut-and-replace, reduce the word and
    // hand it back.
    size_t this_length = this_word.wordLength();
    size_t that_length = that_word.wordLength();
    std::vector<GroupExpressionTerm> this_word_vec, reducer;
    // we'll splay-out *this and that_word so that it's easier to search
    // for commonalities.
    this_word_vec.reserve( this_length );
    reducer.reserve( that_length );
    // start the splaying of terms
    for (const auto& t : this_word.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            this_word_vec.emplace_back( t.generator,
                                        (t.exponent>0) ? 1 : -1 );
    }
    // and that_word
    for (const auto& t : that_word.terms()) {
        for (long i=0; i<std::abs(t.exponent); i++)
            reducer.emplace_back( t.generator,
                                  (t.exponent>0) ? 1 : -1 );
    }
    // done splaying, produce inv_reducer
    std::vector< GroupExpressionTerm > inv_reducer( that_length );
    for (size_t i=0; i<that_length; i++)
        inv_reducer[that_length-(i+1)] = reducer[i].inverse();
    // done with inv_reducer, erase terms
    this_word.terms().clear();

    // *this word is some conjugate of AB and the relator is some conjugate of AC.
    //  We are performing the substitution
    // A=C^{-1}, thus we need to produce the word C^{-1}B. Put in C^{-1} first..
    for (size_t i=0; i<(that_length - sub_data.sub_length); i++)
        this_word.terms().push_back( sub_data.invertB ?
                                        reducer[(that_length - sub_data.start_from + i) % that_length] :
                                        inv_reducer[(that_length - sub_data.start_from + i) % that_length] );
    // iterate through remainder of this_word_vec, starting from
    //     sub_data.start_sub_at + sub_length, ie fill in B
    for (size_t i=0; i<(this_length - sub_data.sub_length); i++)
        this_word.terms().push_back(
            this_word_vec[(sub_data.start_sub_at + sub_data.sub_length + i) %
                          this_length] );
    // done
    this_word.simplify();
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

    /*
     * Commenting out since this is not used, and requires access to a
     * private struct.  If you want to put it back, it should probably
     * be a member function of WordSubstitutionData. - bab
     *
    // gives a string that describes the substitution
    std::string substitutionString( const GroupExpression &word,
                    const GroupPresentation::WordSubstitutionData &subData )
    {
     std::string retval;
     // cut subData into bits, assemble what we're cutting
     //  out and what we're pasting in.
     unsigned long word_length ( word.wordLength() );
     std::vector< GroupExpressionTerm > reducer;
     reducer.reserve( word_length );
      // splay word
     for (auto it = word.terms().begin(); it!=word.terms().end(); it++)
      { for (unsigned long i=0; i<std::abs((*it).exponent); i++)
         reducer.push_back( GroupExpressionTerm( (*it).generator,
                          ((*it).exponent>0) ? 1 : -1 ) );    }
     // done splaying, produce inv_reducer
     std::vector< GroupExpressionTerm > inv_reducer( word_length );
     for (unsigned long i=0; i<word_length; i++)
        inv_reducer[word_length-(i+1)] = reducer[i].inverse();
     GroupExpression del_word, rep_word;
            // produce word to delete, and word to replace with.

     for (unsigned long i=0; i<(word_length - subData.sub_length); i++)
      rep_word.addTermLast( subData.invertB ?
            reducer[(word_length - subData.start_from + i) % word_length] :
        inv_reducer[(word_length - subData.start_from + i) % word_length] );
     for (unsigned long i=0; i<subData.sub_length; i++)
      del_word.addTermLast( subData.invertB ?
            inv_reducer[(subData.start_from + i) % word_length] :
                reducer[(subData.start_from + i) % word_length] );
     rep_word.simplify(); del_word.simplify();
     retval = del_word.str()+" -> "+rep_word.str();
     return retval;
    }
    */
}

/**
 *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_i2^j2 ... g_in^jn g_i1^j1
 */
void GroupExpression::cycleRight()
{
    if (terms_.size() > 1) {
        GroupExpressionTerm temp(terms_.front());
        terms_.pop_front();
        terms_.push_back(temp);
    }
}

/**
 *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1
 */
void GroupExpression::cycleLeft()
{
    if (terms_.size() > 1) {
        GroupExpressionTerm temp(terms_.back());
        terms_.pop_back();
        terms_.push_front(temp);
    }
}

GroupExpression::GroupExpression( const char* input )
{
    // interpret input as GroupExpression as one of forms a^7b^-2,
    // a^7B^2, aaaaaaaBB, g0^7g1^-2.

    // WSNULL - at start of word, nothing has been input.
    // WSVARLET - read a letter, but don't know if we're in an a^5 or g2^-2 sit.
    // WSVARNUM - gk situation read.
    // WSEXP - ^ read
    // WSEXPSIG ^- read
    // WSEXPNUM reading numbers after ^ or ^-
    enum WORD_STATUS { WSNULL, WSVARLET, WSVARNUM, WSEXP, WSEXPSIG, WSEXPNUM };

    // a loop that goes through the entries of input.
    WORD_STATUS WS(WSNULL);
    GroupExpressionTerm buildTerm;
    for (const char* i = input; *i; ++i) {
        // read *i, see what to do next.
        // case 1: it is a letter a..z or A..Z
        if ( ( *i >= 'a' && *i <= 'z' ) || ( *i >= 'A' && *i <= 'Z' ) ) {
            if (WS==WSNULL) { // fresh letter
                // build buildTerm.
                if ( *i >= 'a' && *i <= 'z' ) {
                    buildTerm.generator = *i - 'a';
                    buildTerm.exponent = 1;
                } else {
                    buildTerm.generator = *i - 'A';
                    buildTerm.exponent = -1;
                }
                WS=WSVARLET;
                continue;
            } else if ( (WS==WSVARLET) || (WS==WSVARNUM) || (WS==WSEXPNUM) ) {
                // new letter but previous letter to finish
                terms_.push_back(buildTerm);
                if ( *i >= 'a' && *i <= 'z' ) {
                    buildTerm.generator = *i - 'a';
                    buildTerm.exponent = 1;
                } else {
                    buildTerm.generator = *i - 'A';
                    buildTerm.exponent = -1;
                }
                WS=WSVARLET;
                continue;
            } else {
                // anything else is a mistake.
                throw InvalidArgument(
                    "Unexpected letter found in group expression");
            }
        }

        // case 2: it is a ^, can only occur after a generator
        if ( *i == '^' ) {
            if (!( (WS==WSVARLET) || (WS==WSVARNUM) ) ) {
                throw InvalidArgument(
                    "Unexpected exponent found in group expression");
            }
            WS=WSEXP;
            continue;
        } // end case 2

        // case 3: it is a -, only valid after ^
        if ( *i == '-' ) {
            if (!(WS==WSEXP)) {
                throw InvalidArgument(
                    "Unexpected minus sign found in group expression");
            }
            buildTerm.exponent = -buildTerm.exponent; // ok with A^-1.
            WS=WSEXPSIG;
            continue;
        } // end case 3

        // case 4: it is a number
        if ( ::isdigit(*i) ) {
            //  subcase (a) this is to build a variable
            // buildTerm.generator == 'g'
            if ( (WS==WSVARLET) && (buildTerm.generator == ('g' - 'a') ) ) {
                buildTerm.generator=(*i - '0');
                WS=WSVARNUM;
                continue;
            } else // we've already started building the variable number
                if ( WS==WSVARNUM ) {
                    buildTerm.generator=10*buildTerm.generator + (*i - '0');
                    continue;
                } else //  subcase (b) this is to build an exponent.
                    if ( (WS==WSEXP) || (WS==WSEXPSIG) ) { // ^num or ^-num
                        if (buildTerm.exponent<0)
                            buildTerm.exponent = - static_cast<long>(*i - '0');
                        else
                            buildTerm.exponent = (*i - '0');
                        WS=WSEXPNUM;
                        continue;
                    } else if (WS==WSEXPNUM) {
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

    // we reached the end of input without any errors
    terms_.push_back(buildTerm);
}

//             **********  GroupPresentation below **************

GroupPresentation::GroupPresentation(unsigned long nGens,
            const std::vector<std::string> &rels)
{
    nGenerators_ = nGens;

    relations_.reserve(rels.size());
    for (const std::string& r : rels)
        relations_.emplace_back(r);
}

bool GroupPresentation::simplifyWord( GroupExpression &input ) const
{
    bool retval( input.simplify(false) );
    if (input.isTrivial())
        return retval;

    // now recursively apply relators until no reduction is possible.
    bool continueSimplify(true);
    while (continueSimplify) {
        continueSimplify = false;
        for (const auto& r : relations_) {
            std::set<WordSubstitutionData> sub_list; // highest score is *first*
            dehnAlgorithmSubMetric( input, r, sub_list );
            if (! sub_list.empty())
                if ( (*sub_list.begin()).score > 0 ) {
                    applySubstitution( input, r, *sub_list.begin() );
                    if (input.isTrivial())
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
std::optional<HomGroupPresentation> GroupPresentation::intelligentSimplify() {
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

        if (auto h = intelligentNielsen()) {
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
    bool didSomething(false);
    // start by taking a copy of *this group, for construction of reductionMap
    GroupPresentation oldGroup( *this );

    // substitutionTable[i] == 1 means kill gi.
    //                      != gi means replace gi by this.
    std::vector< GroupExpression > substitutionTable( nGenerators_ );
    for (unsigned long i=0; i<nGenerators_; i++)
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
                    std::set<WordSubstitutionData> sub_list;
                    dehnAlgorithmSubMetric( *tit, *it, sub_list ); // take first valid sub
                    if (sub_list.size() != 0)
                        if ( (*sub_list.begin()).score > 0 ) {
                            applySubstitution( *tit, *it, *sub_list.begin() );
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
            unsigned long WL ( r.wordLength() );
            // build a table expressing # times each generator is used in r.
            std::vector< unsigned long > genUsage( nGenerators_ );
            build_exponent_vec( r.terms(), genUsage );

            for (unsigned long i=0; i<genUsage.size(); i++)
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
    for (unsigned long i=0; i<substitutionTable.size(); i++)
        if ( substitutionTable[i].countTerms() == 1 )
            if ( substitutionTable[i].generator(0) == i )
                nGenerators_++;

    // now we can build a mapping of where the current generators get sent to.
    // make it a std::vector.

    std::vector< unsigned long > genReductionMapping( nGenerators_ );
    unsigned long indx(0);
    for (unsigned long i=0; i<substitutionTable.size(); i++) {
        if ( substitutionTable[i].countTerms() == 1 )
            if ( substitutionTable[i].generator(0) == i ) {
                genReductionMapping[ indx ] = i;
                indx++;
            }
    }

    // now run through relations_ and substitute genReductionMapping[i] -> i
    for (auto& r : relations_)
        for (unsigned long i=0; i<nGenerators_; i++) {
            GroupExpression gi;
            gi.addTermFirst( i, 1 );
            r.substitute( genReductionMapping[i], gi );
        }
    // and might as well do substitutionTable, too.
    for (auto& sub : substitutionTable)
        for (unsigned long i=0; i<nGenerators_; i++) {
            GroupExpression gi;
            gi.addTermFirst( i, 1 );
            sub.substitute( genReductionMapping[i], gi );
        }
    // and build the reverse isomorphism from the new group to the old
    std::vector< GroupExpression > revMap(nGenerators_);
    for (unsigned long i=0; i<revMap.size(); i++)
        revMap[i].addTermFirst( genReductionMapping[i], 1 );

    if (didSomething) {
        // now we can initialize reductionMap
        return HomGroupPresentation(std::move(oldGroup), *this,
            substitutionTable, revMap);
    } else
        return std::nullopt;
}// end smallCancellation()

std::optional<HomGroupPresentation> GroupPresentation::intelligentNielsen() {
    if (nGenerators_ < 2) return std::nullopt;
    // let's keep a record of the best possible substitution,
    bool didSomething(true);
    std::optional<HomGroupPresentation> retval;
    while (didSomething) {
        didSomething = false;
        unsigned long bSubi(0), bSubj(0), bSubType(0); // IJ IJi JI or JIi 0,1,2,3
        signed long bSubScore(0);

        for (unsigned long i=0; i<nGenerators_; i++)
            for (unsigned long j=0; j<nGenerators_; j++) {
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
            for (unsigned long i=0; i<DomToRan.size(); i++) {
                DomToRan[i].addTermFirst( GroupExpressionTerm( i, 1 ) );
                RanToDom[i].addTermFirst( GroupExpressionTerm( i, 1 ) );
            }
            if (bSubType == 0) {
                nielsenCombine( bSubi, bSubj, 1, true );
                DomToRan[ bSubi ].addTermLast( GroupExpressionTerm( bSubj, -1 ) );
                RanToDom[ bSubi ].addTermLast( GroupExpressionTerm( bSubj, 1 ) );
            } else if (bSubType == 1) {
                nielsenCombine( bSubi, bSubj, -1, true );
                DomToRan[ bSubi ].addTermLast( GroupExpressionTerm( bSubj, 1 ) );
                RanToDom[ bSubi ].addTermLast( GroupExpressionTerm( bSubj, -1 ) );
            } else if (bSubType == 2) {
                nielsenCombine( bSubi, bSubj, 1, false );
                DomToRan[ bSubi ].addTermFirst( GroupExpressionTerm( bSubj, -1 ) );
                RanToDom[ bSubi ].addTermFirst( GroupExpressionTerm( bSubj, 1 ) );
            } else if (bSubType == 3) {
                nielsenCombine( bSubi, bSubj, -1, false );
                DomToRan[ bSubi ].addTermFirst( GroupExpressionTerm( bSubj, 1 ) );
                RanToDom[ bSubi ].addTermFirst( GroupExpressionTerm( bSubj, -1 ) );
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

    for (unsigned long j=0; j<countGenerators(); j++) {
        Vector<Integer> temp = abelianized.snfRep(
            Vector<Integer>::unit(countGenerators(), j));
        for (unsigned long i=0; i<abelianized.snfRank(); i++)
            abMat.entry(i,j) = temp[i]; // columns are snfreps of abelianized gens.
    }

    unsigned long abNF( abelianized.countInvariantFactors() );
    unsigned long abNG( abelianized.snfRank() );
    // step 2: we will mimic the simple smith normal form algorithm algorithm
    //         using corresponding moves on the group presentation.
    //         first the free generators.
    for (unsigned long i=abNF; i<abNG; i++) {
        // in row i we will eliminate all but one entry using column
        // operations.  Now we need to do a while loop -- find any two non-zero
        // entries in the row, and reduce.  If there's only one non-zero entry,
        // we're done.
        unsigned long j0=0, j1=abMat.columns()-1;
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
            bool colFlag( abMat.entry(i,j0).abs() < abMat.entry(i,j1).abs() );
            Integer q = abMat.entry(i,colFlag ? j1 : j0) /
                        abMat.entry(i,colFlag ? j0 : j1);
            // subtract q times column j0 from column j1
            for (unsigned long r=0; r<abMat.rows(); r++)
                abMat.entry(r,colFlag ? j1 : j0) -= abMat.entry(r,colFlag ? j0 : j1)*q;
            GroupPresentation oldPres(*this);
            std::vector<GroupExpression> fVec( nGenerators_ ), bVec( nGenerators_ );
            for (unsigned long l=0; l<nGenerators_; l++) {
                fVec[l].addTermLast( GroupExpressionTerm( l, 1 ) );
                bVec[l].addTermLast( GroupExpressionTerm( l, 1 ) );
                if (l==j1) {
                    fVec[l].addTermLast( GroupExpressionTerm(
                                             colFlag ? j0 : j1, q.longValue() ) );
                    bVec[l].addTermLast( GroupExpressionTerm(
                                             colFlag ? j0 : j1, -q.longValue() ) );
                }
            }
            // manufacture the Nielsen automorphism...
            nielsenCombine(colFlag ? j1 : j0, colFlag ? j0 : j1, -q.longValue() );
            HomGroupPresentation tempHom(std::move(oldPres), *this, fVec, bVec);
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

    for (unsigned long i=0; i<abNF; i++) for (unsigned long j=abNF; j<abNG; j++)
            abMat.entry(i,j) = 0;
    // now we're at [ * 0 * ]
    //              [ 0 D 0 ]

    // step 3: reduce inv fac terms, kill the rest.
    for (unsigned long i=0; i<abNF; i++) {
        // let's start working on entry(i,j0) and (i,j1) with j0<j1 in 0...invFacNum
        unsigned long j0=0, j1=abMat.columns()-1;
        while (j0 < j1) {
            // if at j0 its zero, inc, if at j1 zero, dec
            if ((abMat.entry(i,j0) % abelianized.invariantFactor(i)).isZero()) {
                j0++;
                continue;
            }
            if ((abMat.entry(i,j1) % abelianized.invariantFactor(i)).isZero()) {
                j1--;
                continue;
            }
            // column op!
            bool colFlag( (abMat.entry(i,j0) % abelianized.invariantFactor(i)).abs() <
                          (abMat.entry(i,j1) % abelianized.invariantFactor(i)).abs() );
            Integer q = abMat.entry(i,colFlag ? j1 : j0) /
                        abMat.entry(i,colFlag ? j0 : j1);

            // subtract q times column j0 from column j1
            for (unsigned long r=0; r<abMat.rows(); r++)
                abMat.entry(r,colFlag ? j1 : j0) -= abMat.entry(r,colFlag ? j0 : j1)*q;
            GroupPresentation oldPres(*this);
            std::vector<GroupExpression> fVec( nGenerators_ ), bVec( nGenerators_ );
            for (unsigned long l=0; l<nGenerators_; l++) {
                fVec[l].addTermLast( GroupExpressionTerm( l, 1 ) );
                bVec[l].addTermLast( GroupExpressionTerm( l, 1 ) );
                if (l==j1) {
                    fVec[l].addTermLast( GroupExpressionTerm(
                                             colFlag ? j0 : j1, q.longValue() ) );
                    bVec[l].addTermLast( GroupExpressionTerm(
                                             colFlag ? j0 : j1, -q.longValue() ) );
                }
            }
            // manufacture the Nielsen automorphism...
            nielsenCombine(colFlag ? j1 : j0, colFlag ? j0 : j1, -q.longValue() );
            HomGroupPresentation tempHom(std::move(oldPres), *this, fVec, bVec);
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
    for (unsigned long i=0; i<nGenerators_; i++)
        for (unsigned long j=i+1; j<nGenerators_; j++) {
            // let's see if we can recursively apply the relations to
            // [gi,gj] in order to kill it.
            GroupExpression COM; // commutator [gi,gj]
            COM.addTermLast( i, 1 );
            COM.addTermLast( j, 1 );
            COM.addTermLast( i, -1 );
            COM.addTermLast( j, -1 );
            simplifyWord( COM );
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
 * @param other is what the return permutation turn
 * @param cyclic, if false we get a list of exact relabellings from
 *  *this to other.  If true, it can be up to cyclic permutation and
 *  inversion. If cyclic is true, the routine demands both words
 *  are cyclically-reduced.
 */
std::list< std::map< unsigned long, GroupExpressionTerm > >
GroupExpression::relabellingsThisToOther( const GroupExpression &other,
        bool cyclic ) const
{
    // we'll handle the cyclic==true case as a repeated cyclic==false call.
    if (cyclic) {
        std::list< std::map< unsigned long, GroupExpressionTerm > > retval;
        if (countTerms() != other.countTerms())
            return retval;
        GroupExpression tempW( *this );
        for (unsigned long i=0; i<tempW.countTerms(); i++) {
            retval.splice( retval.end(),
                tempW.relabellingsThisToOther( other, false ) );
            tempW.cycleRight();
        }
        tempW.invert();
        for (unsigned long i=0; i<tempW.countTerms(); i++) {
            retval.splice( retval.end(),
                tempW.relabellingsThisToOther( other, false ) );
            tempW.cycleRight();
        }
        return retval;
        // TODO: consider removing duplicates at this stage.  Or make the return
        //  value a set.
    }

    // cyclic==false
    std::map< unsigned long, GroupExpressionTerm > tempMap;
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
                    return std::list< std::map< unsigned long, GroupExpressionTerm > >();
            } else tempMap[i->generator] = MAPTO;
        }
    // check if words had different number of terms
    if ( (i!=terms().end()) || (j!=other.terms().end()) )
        return std::list< std::map< unsigned long, GroupExpressionTerm > >();

    // okay, we have something.
    std::list< std::map< unsigned long, GroupExpressionTerm > > retval;
    retval.push_back( tempMap );
    return retval;
}

std::list<GroupPresentation> GroupPresentation::identifyFreeProduct() const
{
    // let's create a list of generators not used in the relators, then
    // generators that appear in a common generator, or recursively related
    std::set< unsigned long > unRelated;
    for (unsigned long i=0; i<countGenerators(); i++)
        unRelated.insert(i);
    std::list< std::set< unsigned long > > equivRel;
    // determine which generators are used in the relators, to initialize
    // equivRel.
    {
        // forced scope
        std::set< unsigned long > usedRels;
        for (const auto& r : relations_)
            for (const auto& t : r.terms())
                usedRels.insert( t.generator );
        for (unsigned long u : usedRels) {
            std::set< unsigned long > singleton;
            singleton.insert(u);
            unRelated.erase(u);
            equivRel.push_back(singleton);
        }
    } // end forced scope
    // now we grow the equivalence relation.

    for (const auto& r : relations_) {
        std::set< unsigned long > relSet; // related by r
        if ( (unRelated.size()==0) && (equivRel.size()==1) )
            break;
        for (const auto& t : r.terms())
            relSet.insert( t.generator );
        if (relSet.size() < 2)
            continue; // in case of empty word
        for (auto I=relSet.begin(); I!=relSet.end(); I++)
            for (auto J=I; J!=relSet.end(); J++) {
                if (I==J) continue;
                std::list< std::set< unsigned long > >::iterator SI, SJ;
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
        std::map< unsigned long, unsigned long > downMap; // old to new map
        unsigned long count(0);
        for (unsigned long J : I) {
            downMap.insert( std::pair< unsigned long, unsigned long >(J, count) );
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
    std::map< unsigned long, std::list< const GroupExpression* > >
        domRelIdx, ranRelIdx;

    for (const GroupExpression& r : relations_) {
        std::set< unsigned long > gensUsed;
        for (const auto& t : r.terms())
            gensUsed.insert( t.generator );
        domRelIdx[ gensUsed.size() ].push_back(&r);
    }
    for (const GroupExpression& r : other.relations_) {
        std::set< unsigned long > gensUsed;
        for (const auto& t : r.terms())
            gensUsed.insert( t.generator );
        ranRelIdx[ gensUsed.size() ].push_back(&r);
    }

    // for each relator of this we have lists of potential substitutions
    std::list< std::map<unsigned long, GroupExpressionTerm> > allPartialSubs;
    allPartialSubs.emplace_back();

    for (auto i=domRelIdx.rbegin(); i!=domRelIdx.rend(); i++) {
        // currently we'll do the most simplistic thing possible -- look for relabellings
        // of these relators in the target presentation.
        unsigned long nGens = i->first;
        if (ranRelIdx.find(nGens)==ranRelIdx.end()) return false;

        const std::list< const GroupExpression* > DOMR( i->second );
        const std::list< const GroupExpression* > RANR( ranRelIdx[nGens] );
        // build list of subs for all DOMR -> RANR possibilities
        for (const GroupExpression* DI : DOMR) {
            std::list< std::map<unsigned long, GroupExpressionTerm> > newPartialSubs;
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

                std::list< std::map< unsigned long, GroupExpressionTerm > >
                    tempList( DI->relabellingsThisToOther( *RI, true ) );
                for (const auto& X : tempList)
                    for (const auto& Y : allPartialSubs) {
                        // newMap will be the potential extension of X and Y
                        std::map< unsigned long, GroupExpressionTerm > newMap;
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
        unsigned long gi=0;
        std::set< unsigned long > rGen;
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


bool GroupPresentation::nielsenTransposition(unsigned long i, unsigned long j)
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

bool GroupPresentation::nielsenInvert(unsigned long i)
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

bool GroupPresentation::nielsenCombine(unsigned long i, unsigned long j,
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

// these macros are used only in the identifyExtensionOverZ routine below.
#define idx(gen, cov) ((unsigned long)((gen)-1)+nGm1*(cov))
#define unidx(dat) std::pair<unsigned long, unsigned long>(((dat) % nGm1)+1, (dat)/nGm1)

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
    homologicalAlignment();
    MarkedAbelianGroup abelianized = markedAbelianisation();
    if (abelianized.rank() != 1)
        return std::nullopt;
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
    std::map< unsigned long, unsigned long > maxKiller;
    std::map< unsigned long, unsigned long > minKiller;
    std::map< unsigned long, unsigned long > cellWidth; // 2-cell width in cover

    for (unsigned long l=0; l<relations_.size(); l++) {
        // for each relator determine highest and lowest lifts, and if they
        // are unique or not.

        signed long lift=0;
        signed long maxLift(0), minLift(0);   // sheet index
        unsigned long maxCell(0), minCell(0); // generator's index in presentation
        bool dupMax(false), dupMin(false);    // duplicate lift height?
        std::list<GroupExpressionTerm>& terms(relations_[l].terms());
        for (auto k=terms.rbegin(); k!=terms.rend(); k++) {
            // right to left through the relator
            if (k->generator > 0) {
                lifts[l].push_back( std::pair< GroupExpressionTerm, signed long >
                                    ( *k, lift ) );
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
        cellWidth[l] = (unsigned long)(maxLift - minLift);

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
        while (lifts[l].front().second != cellWidth[l]) {
            std::pair< GroupExpressionTerm, signed long > temp(lifts[l].front());
            lifts[l].pop_front();
            lifts[l].push_back( temp );
        }
        // ensure word starts with highest-weight element as inverted.
        if (lifts[l].front().first.exponent == 1) {
            std::pair< GroupExpressionTerm, signed long > temp(lifts[l].front());
            lifts[l].pop_front();
            lifts[l].reverse();
            lifts[l].push_front(temp); // now run and change the exponents
            for (auto& I : lifts[l])
                I.first.exponent = -I.first.exponent;
        }
    }
    // this is the test for whether or not we can find a finite collection of
    // generators
    unsigned long nGm1( nGenerators_ - 1 );
    if ( (maxKiller.size() != nGm1) || (minKiller.size() != nGm1) )
        return std::nullopt;

    unsigned long maxWidth(0);
    unsigned long liftCount(0); // how many lifts of our generators do we need?
    for (unsigned long i=0; i<maxKiller.size(); i++) {
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
    // by the above idx and unidx macros.
    std::map<unsigned long, GroupExpression> genKiller;
    // start with the liftCount lift, i.e. the first order reducers a^-Mg_ia^M =...
    for (unsigned long i=1; i<countGenerators(); i++) {
        GroupExpression temp;
        // maxKiller[i] is the index in lifts of the relator that kills generator gi
        // i is a liftIdx
        unsigned long delta(0);
        for (auto I=lifts[maxKiller[i]].begin(); I!=lifts[maxKiller[i]].end(); I++) {
            if (I==lifts[maxKiller[i]].begin()) {
                // push up delta sheets so that it kills appropriately
                delta = (unsigned long)(liftCount - I->second);
                continue;
            }
            temp.addTermFirst( GroupExpressionTerm(
                                   idx( I->first.generator, I->second + delta ), I->first.exponent ) );
        }
        genKiller.insert( std::pair< unsigned long, GroupExpression >
                          (idx(i,liftCount),temp) );
    }

    // extra genKillers -- sometimes there are wider words than the killing words.
    //  like with presentations such as:
    //   < a b | b a^-1 b a^-1 b^-1 a^2, a^-3 b^2 a^3 b^2 >
    // We could alternatively use the genKiller to reduce the width of the other
    // relators.  But for now we use this less-sophisticated work-around.
    for (unsigned long j=liftCount; j<maxWidth; j++) {
        for (unsigned long i=1; i<countGenerators(); i++) {
            // bump-up lift of each genKiller then apply previous genKillers to them
            // to create word in the fibre group.
            GroupExpression tempW( genKiller[idx(i, j)] );
            for (auto& I : tempW.terms())
                I.generator += nGm1;
            for (const auto& J : genKiller)
                tempW.substitute( J.first, J.second, false );
            genKiller.insert( std::pair< unsigned long, GroupExpression >
                              (idx(i,j+1), tempW) );
        }
    }

    //  initialize tempTable with the 0-th lifts of the relators.
    std::list< GroupExpression > tempTable;
    GroupPresentation kerPres;
    kerPres.addGenerator( liftCount * nGm1 );

    for (const auto& lift : lifts) {
        GroupExpression temp;
        for (const auto& I : lift)
            temp.addTermFirst( GroupExpressionTerm(
                                   idx( I.first.generator, I.second ), I.first.exponent ) );
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
    for (unsigned long i=0; i<autVec.size(); i++) // this part depends on idx
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
    retval.intelligentSimplify();

    // Modify this presentation to reflect the semi-direct product
    //        structure we've discovered!
    // deallocate relations, and resize and repopulate with copies of kerPres's
    //  relations.

    nGenerators_ = retval.domain().nGenerators_ + 1;
    relations_.resize( retval.domain().nGenerators_ +
                       retval.domain().relations_.size() );

    for (unsigned long i=0; i<retval.domain().relations_.size(); i++)
        relations_[i] = retval.domain().relations_[i];

    // And now all the b^-1g_ib = genKiller(i) and b^-1g_ib = g_{i+1} relations.
    for (unsigned long i=0; i<retval.domain().nGenerators_; i++) {
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
        std::set<unsigned long> usedTermsF, usedTermsS;
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
    GroupPresentation oldPres(*this);

    // move the relators into a separate list for now.
    std::vector<GroupExpression> relatorPile;
    relatorPile.swap(relations_);
    // now relations_ is empty.

    // step 1: cyclic reduce relators. Delete length 0 relators.
    //         delete generators corresponding to length 1 relators
    for (auto& r : relatorPile)
        r.simplify(true);

    std::set<unsigned long> genToDel; // generators we've eliminated
    bool reloopFlag(true);
    while (reloopFlag) {
        reloopFlag=false;
        std::set<unsigned long> newGenDel;
        for (const auto& r : relatorPile) {
            if (r.countTerms() == 1)
                if ( std::abs( r.terms().front().exponent ) == 1 ) {
                    // a killer!
                    newGenDel.insert( r.terms().front().generator );
                }
        }
        genToDel.insert( newGenDel.begin(), newGenDel.end() );

        for (unsigned long i : newGenDel)
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
        std::set< unsigned long > interval, compDelete; // complement
        for (unsigned long i=0; i<nGenerators_; i++)
            interval.insert(interval.end(), i);
        std::set_difference( interval.begin(), interval.end(),
                             genToDel.begin(), genToDel.end(),
                             std::inserter(compDelete, compDelete.end() ) );

        // then reduce the group, run through genToDel and do gi->1 subs on all
        //  relators, and gk --> gk-1 for larger generators.
        std::vector< GroupExpression > downSub(nGenerators_);
        std::vector< GroupExpression > upSub(nGenerators_ - genToDel.size());
        unsigned long i=0;
        for (unsigned long I : compDelete) {
            upSub[i].addTermFirst( GroupExpressionTerm( I, 1 ) );
            downSub[I].addTermFirst( GroupExpressionTerm( i, 1 ) );
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
            std::set<unsigned long> usedTerms;
            for (const auto& t : r.terms())
                usedTerms.insert( t.generator );
            unsigned long smallestGen = *usedTerms.begin();
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
    return out.str();
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
    return out.str();
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
    return out.str();
}

void GroupPresentation::writeTextCompact(std::ostream& out) const {
    if (nGenerators_ == 0) {
        out << "< >";
        return;
    }

    out << "<";
    if (nGenerators_ <= 26) {
        for (unsigned long i=0; i<nGenerators_; i++)
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

void GroupPresentation::proliferateRelators(unsigned long depth) {
    std::list< GroupExpression > newRels;
    for (unsigned long i=0; i<relations_.size(); i++)
        for (unsigned long j=0; j<relations_.size(); j++) {
            if (i==j) continue; // TODO: maybe accept novel self-substitutions?
            std::set<WordSubstitutionData> sub_list;
            dehnAlgorithmSubMetric( relations_[i], relations_[j], sub_list, depth );
            while (!sub_list.empty()) {
                GroupExpression newRel( relations_[i] );
                applySubstitution( newRel, relations_[j], *sub_list.begin() );
                sub_list.erase( sub_list.begin() );
                newRels.push_back(std::move(newRel));
            }
        }
    depth--;
    while (depth>0) {
        std::list< GroupExpression > tempRels;
        for (const auto& r : relations_)
            for (const auto& j : newRels) {
                // attempt to tack r to j. To do this, we should perhaps
                // keep a record of how j was created, as in where the two junction
                // points are so as to ensure what we're adding spans at least one
                // of the junctions.
                std::set<WordSubstitutionData> sub_list;
                dehnAlgorithmSubMetric( j, r, sub_list, depth );
                while (!sub_list.empty()) {
                    // TODO: we might want to avoid some obviously repetitive
                    //       subs as noted above?
                    GroupExpression newRel( j );
                    applySubstitution( newRel, r, *sub_list.begin() );
                    sub_list.erase( sub_list.begin() );
                    tempRels.push_back(std::move(newRel));
                }
            }
        depth--;

        // Move our newly generated tempRels onto the end of newRels.
        newRels.splice(newRels.end(), std::move(tempRels));
    }

    // Finally, move *all* of our new relations onto the end of relations_.
    for (auto& r : newRels)
        relations_.push_back(std::move(r));
}

Matrix<bool> GroupPresentation::incidence() const {
    Matrix<bool> inc(relations_.size(), nGenerators_);
    inc.initialise(false);

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
    return out.str();
}

} // namespace regina

