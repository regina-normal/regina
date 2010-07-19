
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <iterator>
#include <map>
#include <sstream>
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "file/nfile.h"
#include "maths/numbertheory.h"
#include "utilities/boostutils.h"
#include "utilities/stlutils.h"

namespace regina {

typedef std::list<NGroupExpressionTerm>::iterator TermIterator;
typedef std::list<NGroupExpressionTerm>::const_iterator TermIteratorConst;
typedef std::vector<NGroupExpression*>::iterator RelIterator;
typedef std::vector<NGroupExpression*>::const_iterator RelIteratorConst;
typedef std::list<NGroupExpression*>::iterator TmpRelIterator;

NGroupExpressionTerm NGroupExpressionTerm::readFromFile(NFile& in) {
    return NGroupExpressionTerm(in.readULong(), in.readLong());
}

void NGroupExpressionTerm::writeToFile(NFile& out) const {
    out.writeULong(generator);
    out.writeLong(exponent);
}

std::ostream& operator << (std::ostream& out,
        const NGroupExpressionTerm& term) {
    if (term.exponent == 0)
        out << '1';
    else if (term.exponent == 1)
        out << 'g' << term.generator;
    else
        out << 'g' << term.generator << '^' << term.exponent;
    return out;
}

NGroupExpressionTerm& NGroupExpression::getTerm(unsigned long index) {
    TermIterator pos = terms.begin();
    advance(pos, index);
    return *pos;
}

const NGroupExpressionTerm& NGroupExpression::getTerm(
        unsigned long index) const {
    TermIteratorConst pos = terms.begin();
    advance(pos, index);
    return *pos;
}

NGroupExpression* NGroupExpression::inverse() const {
    NGroupExpression* ans = new NGroupExpression();
    transform(terms.begin(), terms.end(), front_inserter(ans->terms),
        std::mem_fun_ref(&NGroupExpressionTerm::inverse));
    return ans;
}

void NGroupExpression::invert() {
	reverse(terms.begin(), terms.end());
	std::list< NGroupExpressionTerm >::iterator it; 
	for (it = terms.begin(); it != terms.end(); it++)
		(*it).exponent = -(*it).exponent;
}


NGroupExpression* NGroupExpression::power(long exponent) const {
    NGroupExpression* ans = new NGroupExpression();
    if (exponent == 0)
        return ans;

    long i;
    if (exponent > 0)
        for (i = 0; i < exponent; i++)
            ans->terms.insert(ans->terms.end(), terms.begin(), terms.end());
    else
        for (i = 0; i > exponent; i--)
            transform(terms.begin(), terms.end(), front_inserter(ans->terms),
                std::mem_fun_ref(&NGroupExpressionTerm::inverse));
    return ans;
}

bool NGroupExpression::simplify(bool cyclic) {
    bool changed = false;
    TermIterator next, tmpIt;
    for (next = terms.begin(); next != terms.end(); ) {
        // Take a look at merging next forwards.
        if ((*next).exponent == 0) {
            // Zero exponent.
            // Delete this term and step back to the previous term in
            // case we can now merge the previous and next terms.
            next = terms.erase(next);
            if (next != terms.begin())
                next--;
            changed = true;
            continue;
        }

        tmpIt = next;
        tmpIt++;

        // Now tmpIt points to the term after next.
        if (tmpIt == terms.end()) {
            // No term to merge forwards with.
            next++;
        } else if ((*tmpIt) += (*next)) {
            // Successfully merged this with the following term.
            next = terms.erase(next);
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
    while (terms.begin() != terms.end() &&
            ++terms.begin() != terms.end()) {
        // Thus terms.size() > 1.  The unusual test above is used to
        // avoid calling terms.size() which takes linear time.
        if (terms.front() += terms.back()) {
            // Merged!
            terms.pop_back();
            changed = true;

            // Did we create an empty term?
            if (terms.front().exponent == 0)
                terms.pop_front();
        } else
            break;
    }

    return changed;
}

bool NGroupExpression::substitute(unsigned long generator,
        const NGroupExpression& expansion, bool cyclic) {
    bool changed = false;
    NGroupExpression* inverse = 0;
    const NGroupExpression* use;
    long exponent, i;
    for (TermIterator current = terms.begin(); current != terms.end(); ) {
        if ((*current).generator != generator)
            current++;
        else {
            exponent = (*current).exponent;
            if (exponent != 0) {
                if (exponent > 0)
                    use = &expansion;
                else {
                    if (inverse == 0)
                        inverse = expansion.inverse();
                    use = inverse;
                    exponent = -exponent;
                }

                // Fill in exponent copies of use.
                //
                // Note that the following insertion will invalidate
                // current if the wrong type of data structure is being used!
                for (i = 0; i < exponent; i++)
                    terms.insert(current, use->terms.begin(), use->terms.end());
            }

            current = terms.erase(current);
            changed = true;
        }
    }
    if (inverse)
        delete inverse;
    if (changed)
        simplify(cyclic);
    return changed;
}

void NGroupExpression::writeXMLData(std::ostream& out) const {
    out << "<reln> ";
    for (TermIteratorConst it = terms.begin(); it != terms.end(); it++)
        out << (*it).generator << '^' << (*it).exponent << ' ';
    out << "</reln>";
}

void NGroupExpression::writeToFile(NFile& out) const {
    out.writeULong(terms.size());
    for (TermIteratorConst it = terms.begin(); it != terms.end(); it++)
        (*it).writeToFile(out);
}

NGroupExpression* NGroupExpression::readFromFile(NFile& in) {
    NGroupExpression* ans = new NGroupExpression();
    unsigned long nTerms = in.readULong();
    for (unsigned long i = 0; i < nTerms; i++)
        ans->terms.push_back(NGroupExpressionTerm::readFromFile(in));
    return ans;
}

void NGroupExpression::writeTextShort(std::ostream& out) const {
    if (terms.empty())
        out << '1';
    else {
        TermIteratorConst last = --terms.end();
        copy(terms.begin(), last,
            std::ostream_iterator<NGroupExpressionTerm>(out, " "));
        out << *last;
    }
}

NGroupPresentation::NGroupPresentation(const NGroupPresentation& cloneMe) :
        ShareableObject(), nGenerators(cloneMe.nGenerators) {
    transform(cloneMe.relations.begin(), cloneMe.relations.end(),
        back_inserter(relations), FuncNewCopyPtr<NGroupExpression>());
}

std::string NGroupPresentation::recogniseGroup() const {
    std::ostringstream out;
    unsigned long nRels = relations.size();
    NGroupExpression* rel;
    long exp;

    // Run through cases.
    if (nGenerators == 0)
        out << 0;
    else if (nGenerators == 1) {
        // Each term is of the form g^k=1.  This is Z_d where d is the
        // gcd of the various values of k.
        unsigned long d = 0;
        for (unsigned long i = 0; i < nRels; i++) {
            rel = relations[i];
            if (rel->getNumberOfTerms() > 1)
                rel->simplify();
            // The relation should have at most one term now.
            if (rel->getNumberOfTerms() == 1) {
                exp = rel->getExponent(0);
                if (exp > 0)
                    d = gcd(d, exp);
                else if (exp < 0)
                    d = gcd(d, -exp);
            }
        }
        if (d == 0)
            out << 'Z';
        else if (d == 1)
            out << 0;
        else
            out << "Z_" << d;
    } else if (nRels == 0) {
        out << "Free (" << nGenerators << " generators)";
    } else if (nGenerators == 2 && nRels == 1) {
        // See if it's the abelian Z + Z.
        rel = relations[0];
        rel->simplify(true);

        // Look for a relation (x y x^-1 y^-1).
        if (rel->getNumberOfTerms() == 4) {
            if (rel->getGenerator(0) == rel->getGenerator(2) &&
                    rel->getGenerator(1) == rel->getGenerator(3) &&
                    rel->getGenerator(0) != rel->getGenerator(1) &&
                    abs(rel->getExponent(0)) == 1 &&
                    abs(rel->getExponent(1)) == 1 &&
                    rel->getExponent(0) + rel->getExponent(2) == 0 &&
                    rel->getExponent(1) + rel->getExponent(3) == 0)
                out << "Z + Z (abelian)";
        }
    } else if (nGenerators == 2 && nRels == 2) {
        // TODO: See if it's the quaternions.
    } else {
        // nGenerators >= 2 and nRels >= 2.
        // Don't have anything intelligent to say at this point.
    }
    return out.str();
}

void NGroupPresentation::writeXMLData(std::ostream& out) const {
    out << "<group generators=\"" << nGenerators << "\">\n";
    for (RelIteratorConst it = relations.begin(); it != relations.end(); it++) {
        out << "  ";
        (*it)->writeXMLData(out);
        out << '\n';
    }
    out << "</group>\n";
}

void NGroupPresentation::writeToFile(NFile& out) const {
    out.writeULong(nGenerators);
    out.writeULong(relations.size());
    for (RelIteratorConst it = relations.begin(); it != relations.end(); it++)
        (*it)->writeToFile(out);

    // Write properties.
    out.writeAllPropertiesFooter();
}

NGroupPresentation* NGroupPresentation::readFromFile(NFile& in) {
    NGroupPresentation* ans = new NGroupPresentation();
    ans->nGenerators = in.readULong();
    unsigned long nRels = in.readULong();
    for (unsigned long i = 0; i < nRels; i++)
        ans->relations.push_back(NGroupExpression::readFromFile(in));

    // Read properties.
    in.readProperties(0);

    return ans;
}

void NGroupPresentation::writeTextLong(std::ostream& out) const {
    out << "Generators: ";
    if (nGenerators == 0)
        out << "(none)";
    else if (nGenerators == 1)
        out << "g0";
    else if (nGenerators == 2)
        out << "g0, g1";
    else
        out << "g0 .. g" << (nGenerators - 1);
    out << std::endl;

    out << "Relations:\n";
    if (relations.empty())
        out << "    (none)\n";
    else
        for (RelIteratorConst it = relations.begin();
                it != relations.end(); it++) {
            out << "    ";
            (*it)->writeTextShort(out);
            out << std::endl;
        }
}

std::string NGroupPresentation::stringOutput() const {
	std::string retval;
	retval.append("< ");
        if (nGenerators == 0) retval.append("");
        else if (nGenerators == 1) retval.append("g0");
        else if (nGenerators == 2) retval.append("g0, g1");
        else { retval.append("g0 .. g"); std::stringstream num;
                 num<<(nGenerators - 1); retval.append(num.str()); }
        retval.append(" | ");
        std::stringstream temp;
        if (relations.empty()) retval.append(""); 
	else for (RelIteratorConst it = relations.begin(); 
                it != relations.end(); it++) {
            if (it != relations.begin()) temp<<", ";
            (*it)->writeTextShort(temp); }
        retval.append(temp.str());
        retval.append(" >");
	return retval;
}

std::string NGroupExpression::TeXOutput() const {
    std::string retval;
    if (terms.empty())
        retval.append("e");
    else {
       std::list<NGroupExpressionTerm>::const_iterator i;
        for (i = terms.begin(); i!=terms.end(); i++)
         {
          std::stringstream genss; genss<<(*i).generator;
          std::stringstream expss; expss<<(*i).exponent; 
          retval.append("g_{"); retval.append(genss.str());
          retval.append("}"); 
          if ( (*i).exponent != 1 ) {
           retval.append("^{"); retval.append(expss.str());
           retval.append("}"); }
         }
    }
    return retval;
}

std::string NGroupPresentation::TeXOutput() const {
	std::string retval;
	retval.append("\\langle ");
        if (nGenerators == 0) retval.append("\\cdot");
        else if (nGenerators == 1) retval.append("g_0");
        else if (nGenerators == 2) retval.append("g_0, g_1");
        else { retval.append("g0, \\cdots, g"); 
        std::stringstream num; num<<(nGenerators - 1); 
        retval.append(num.str()); }
        retval.append(" | ");
        // std::stringstream temp;
        if (relations.empty()) retval.append("\\cdot"); 
	else for (RelIteratorConst it = relations.begin(); 
                it != relations.end(); it++) {
            if (it != relations.begin()) retval.append(", ");
            retval.append( (*it)->TeXOutput() ); }
        retval.append(" \\rangle");
	return retval;
}


std::auto_ptr<NAbelianGroup> NGroupPresentation::unMarkedAbelianization() const
{
 // create presentation matrices to pass to NAbelianGroup(M, N)
 NMatrixInt M(1, getNumberOfGenerators() ); // zero matrix
 NMatrixInt N(getNumberOfGenerators(), getNumberOfRelations() ); 
 // run through rels, increment N entries appropriately
 for (unsigned long j=0; j<getNumberOfRelations(); j++)
  {
   NGroupExpression Rj ( getRelation(j) );
   for (unsigned long i=0; i<Rj.getNumberOfTerms(); i++)
     N.entry( Rj.getGenerator(i), j ) += Rj.getExponent(i);
  }
 return std::auto_ptr<NAbelianGroup>(new NAbelianGroup(M,N));
}

std::auto_ptr<NMarkedAbelianGroup> NGroupPresentation::markedAbelianization() const
{
 // create presentation matrices to pass to NMarkedAbelianGroup(M, N)
 NMatrixInt M(1, getNumberOfGenerators() ); // zero matrix
 NMatrixInt N(getNumberOfGenerators(), getNumberOfRelations() ); 
 // run through rels, increment N entries appropriately
 for (unsigned long j=0; j<getNumberOfRelations(); j++)
  {
   NGroupExpression Rj ( getRelation(j) );
   for (unsigned long i=0; i<Rj.getNumberOfTerms(); i++)
     N.entry( Rj.getGenerator(i), j ) += Rj.getExponent(i);
  }
 return std::auto_ptr<NMarkedAbelianGroup>(new NMarkedAbelianGroup(M,N));
}

//  okay, so we have *this word, and that_word.  We want to walk through
//  both and see to what extend we can use that_word to reduce
//  *this word.  Everything with a score >= 0, i.e. not increasing 
// the length, we will record in the sub_list.  The score will 
//  simply be the difference between the length of the common chain
//  between *this word and that_word, minus the length of the remaining
//  word in that_word, *unless* all of that_word is used, in which case
//  we have to do a computation to see if additional simplifications
//  happen.
//
// so if that_word is longer than this_word, the new word can have at 
// best length: length(that_word) - length(this_word).  So if
// length(that_word) > 2*length(this_word) there's no point in 
// attempting a substitution as any substitution would increase length. 
void NGroupExpression::dehnAlgorithmSubMetric( const NGroupExpression 
        &that_word, std::set< NWordSubstitutionData > &sub_list ) const
{
 // first thing first, let's rewrite *this and that_word as a sequence of 
 // monomials, ie gi^+-1.  Might as well store as vectors. 
 unsigned long this_length ( wordLength() );
 unsigned long that_length ( that_word.wordLength() );
 if ( 2*this_length < that_length ) return; // if that_length is more 
   // than twice the size of this_length, don't bother 
 if ( that_length == 0 ) return; // empty relator, don't bother!
 std::vector< NGroupExpressionTerm > this_word( 0 );
 std::vector< NGroupExpressionTerm > reducer( 0 ); // we'll splay-out 
  //*this and that_word so that it's easier to search for commonalities.
 this_word.reserve( this_length ); reducer.reserve( that_length );
 std::list<NGroupExpressionTerm>::const_iterator it; 

 for (it = terms.begin(); it!=terms.end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     this_word.push_back( NGroupExpressionTerm( (*it).generator, 
        ((*it).exponent>0) ? 1 : -1 ) );  }
 for (it = that_word.terms.begin(); it!=that_word.terms.end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     reducer.push_back( NGroupExpressionTerm( (*it).generator, 
        ((*it).exponent>0) ? 1 : -1 ) );    }
 std::vector< NGroupExpressionTerm > inv_reducer( that_length );
 for (unsigned long i=0; i<reducer.size(); i++) 
        inv_reducer[that_length-(i+1)] = reducer[i].inverse(); 

 // okay, let's look for cyclic subwords of reducer in this_word.  
 // The loop will go from the beginning to the end of this_word, 
 //  for each position in this_word we look for commonality with 
 //  some part of reducer, and we follow it along (cyclically) to 
 //  see how far it goes.  Once we find the end, we record it. 
 for (unsigned long i=0; i<this_length; i++) 
        for (unsigned long j=0; j<that_length; j++)
  { // start point for comparison is this_word[i] and reducer[j]
    unsigned long comp_length = 0; 
    // check to seeif this_word[(i+comp_length) % this_word.size()] == 
    //  reducer[(j+comp_length) % comp_length], if so, increment...
    while ( (this_word[(i+comp_length) % this_length] == 
                reducer[(j+comp_length) % that_length]) && 
            (comp_length < that_length) && (comp_length < this_length) ) 
                comp_length++;  
    // okay, we've found a subword of reducer of length comp_length that
    //  agrees with a subword of this_word of the same length. 
    // now we need to measure how successful a substitution it might be.
    NWordSubstitutionData subData;
    subData.invertB=false; subData.sub_length=comp_length;
         subData.start_sub_at=i; subData.start_from=j;
    if (comp_length == that_length)	
	{ // special case, we record this regardless but its score is special.
          // assemble the word with reducer removed, *reduce* the word any 
          // further if possible, check the length. 
	  subData.score = that_length;
	  // increment subData.score for every consecutive mutually-inverse
          //  pair this_word[i-a] this_word[i+comp_length+a]
          //  a=1,2,... up until (this_length-that_length)/2
          unsigned long a=1; 
	  while ( (this_word[( (i+this_length)-a )%this_length].inverse()==
                  this_word[( (i+comp_length)+(a-1) )%this_length]) &&
		  (2*a+that_length <= this_length ) ) { a++; subData.score++; }
	  sub_list.insert(subData);
	}
    else if (2*comp_length >= that_length) // only bother if we're using 
        //  at least half of the relator
        { // so we'll record this but its score is relatively easy to compute
          subData.score = 2*comp_length - that_length;
	  sub_list.insert(subData);
        }
    // now lets look for inv_reducer substitutions. 
    comp_length = 0; 
    // check to seeif this_word[(i+comp_length) % this_word.size()] == 
    //  inv_reducer[(j+comp_length) % comp_length], if so, increment...
    while ( (this_word[(i+comp_length) % this_length] == 
                 inv_reducer[(j+comp_length) % that_length]) && 
            (comp_length < that_length) && (comp_length < this_length) ) 
                comp_length++;  
    // okay, we've found a subword of reducer of length comp_length 
    // that agrees with a subword of this_word of the same length. 
    // now we need to measure how successful a substitution it might be.
    subData.invertB=true; subData.sub_length=comp_length; 
    if (comp_length == that_length)	
	{ // special case, we record this regardless but its score is special.
          // assemble the word with reducer removed, *reduce* the word any 
          //  further if possible, check the length. 
	  subData.score = that_length;
	  // increment subData.score for every consecutive mutually-inverse 
          //  pair this_word[i-a] this_word[i+comp_length+a]
          //  a=1,2,... up until (this_length-that_length)/2
          unsigned long a=1; 
	  while ( (this_word[( (i+this_length)-a )%this_length].inverse()==
                this_word[( (i+comp_length)+(a-1) )%this_length]) &&
		  (2*a+that_length <= this_length ) ) { a++; subData.score++; }
	  sub_list.insert(subData);
	}
    else if (2*comp_length >= that_length) // only bother if we're using at 
        //  least half of the relator
        { // so we'll record this but its score is relatively easy to compute
          subData.score = 2*comp_length - that_length;
	  sub_list.insert(subData);
        }
  }
}

void NGroupExpression::applySubstitution( const NGroupExpression &that_word, 
                        const NWordSubstitutionData &sub_data )
{
 // okay, so let's do a quick cut-and-replace, reduce the word and hand it back. 
 unsigned long this_length ( wordLength() );
 unsigned long that_length ( that_word.wordLength() );
 std::vector< NGroupExpressionTerm > this_word( 0 );
 std::vector< NGroupExpressionTerm > reducer( 0 ); // we'll splay-out *this and 
  // that_word so that it's easier to search for commonalities.
 this_word.reserve( this_length ); reducer.reserve( that_length );
 std::list<NGroupExpressionTerm>::const_iterator it; 
 // start the splaying of terms
 for (it = terms.begin(); it!=terms.end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     this_word.push_back( NGroupExpressionTerm( (*it).generator, 
                ((*it).exponent>0) ? 1 : -1 ) );  }
  // and that_word
 for (it = that_word.terms.begin(); it!=that_word.terms.end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     reducer.push_back( NGroupExpressionTerm( (*it).generator, 
         ((*it).exponent>0) ? 1 : -1 ) );    }
 // done splaying, produce inv_reducer
 std::vector< NGroupExpressionTerm > inv_reducer( that_length );
 for (unsigned long i=0; i<that_length; i++) inv_reducer[that_length-(i+1)] = 
         reducer[i].inverse(); 
 // done with inv_reducer, erase terms
 terms.clear();

 // *this word is some conjugate of AB and the relator is some conjugate of AC.
  //  We are performing the substitution
 // A=C^{-1}, thus we need to produce the word C^{-1}B. Put in C^{-1} first..
 for (unsigned long i=0; i<(that_length - sub_data.sub_length); i++)
  terms.push_back( sub_data.invertB ?     
        reducer[(that_length - sub_data.start_from + i) % that_length] : 
    inv_reducer[(that_length - sub_data.start_from + i) % that_length] );
 // iterate through remainder of this_word, starting from 
 //     sub_data.start_sub_at + sub_length, ie fill in B 
 for (unsigned long i=0; i<(this_length - sub_data.sub_length); i++)
  terms.push_back( this_word[(sub_data.start_sub_at + sub_data.sub_length + i) %
         this_length] );
 // done
 simplify();
}

bool compare_length( const NGroupExpression* first, 
         const NGroupExpression* second ) 
 {  return ( first->wordLength() < second->wordLength() ); }

// assumes expVec initialized to the number of generators in the group, 
//  and entries zero.
void build_exponent_vec( const std::list< NGroupExpressionTerm > & word, 
                          std::vector<unsigned long> &expVec ) 
{
 std::list<NGroupExpressionTerm>::const_iterator tit;
 for ( tit = word.begin(); tit != word.end(); tit++) 
     expVec[ (*tit).generator ] += abs( (*tit).exponent );
}

// gives a string that describes the substitution
std::string substitutionString( const NGroupExpression &word, 
                const NGroupExpression::NWordSubstitutionData &subData )
{
 std::string retval;
 // cut subData into bits, assemble what we're cutting 
 //  out and what we're pasting in. 
 unsigned long word_length ( word.wordLength() );
 std::vector< NGroupExpressionTerm > reducer( 0 ); 
 reducer.reserve( word_length );
 std::list<NGroupExpressionTerm>::const_iterator it; 
  // splay word
 for (it = word.getTerms().begin(); it!=word.getTerms().end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     reducer.push_back( NGroupExpressionTerm( (*it).generator, 
                      ((*it).exponent>0) ? 1 : -1 ) );    }
 // done splaying, produce inv_reducer
 std::vector< NGroupExpressionTerm > inv_reducer( word_length );
 for (unsigned long i=0; i<word_length; i++) inv_reducer[word_length-(i+1)] = 
                reducer[i].inverse(); 
 NGroupExpression del_word, rep_word; 
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
 retval = del_word.stringOutput()+" -> "+rep_word.stringOutput();
 return retval;
}

std::string NGroupExpression::stringOutput() const
{
 std::stringstream out; 
 writeTextShort(out); 
 return out.str();
}

void NGroupExpression::addTermsLast( const NGroupExpression& word)
{
std::list< NGroupExpressionTerm >::const_iterator it; 
for (it = word.terms.begin(); it != word.terms.end(); it++)
 addTermLast( *it );
}

void NGroupExpression::addTermsFirst( const NGroupExpression& word)
{ // traverse word's terms in reverse order.
std::list< NGroupExpressionTerm >::const_reverse_iterator it; 
for (it = word.terms.rbegin(); it != word.terms.rend(); it++)
 addTermFirst( *it );
}

// this is a wrapper for the "real" intelligentSimplify() routine, 
//  one that throws out the map that it produces. 
bool NGroupPresentation::intelligentSimplify() {
 NHomGroupPresentation* reductionMap(NULL);
 bool changed( intelligentSimplify(reductionMap) );
 delete reductionMap;
 return changed;
}


// TODO: 1) recognise commutators and use that to our advantage. 
//       recognise other advantageous small words ??
//       random walks using score==0 substitutions??
//       recognise utility of some Nielsen transforms, ie
//       2) basic automorphisms of the free group, x_1 --> x_1, ...
//             x_i --> x_ix_j, ..., x_n --> x_n, etc. 
bool NGroupPresentation::intelligentSimplify(NHomGroupPresentation*& reductionMap)
{
 bool didSomething(false);
 // start by taking a copy of *this group, for the eventual construction of reductionMap
 NGroupPresentation oldGroup( *this );

 // let's start with the shortest relators first, and see to what extent we can use them to 
 // simplify the other relators.  To do this we'll need a temporary relator table, sorted 
 // on the length of the relators. 
 std::list< NGroupExpression* >::iterator it;
 std::list< NGroupExpression* > relatorList; 
 for (unsigned long i=0; i<relations.size(); i++) relatorList.push_back( relations[i] );
 relations.resize(0); // we'll rebuild the relations later. 

 // the substitutionTable keeps track of how generators are killed by relators. 
 // substitutionTable[i] != generator gi if and only the algorithm discovers 
 //  gi to be trivial in the presentation or expressible in terms of other generators
 std::vector< NGroupExpression > substitutionTable( nGenerators ); 
 for (unsigned long i=0; i<nGenerators; i++) substitutionTable[i].addTermFirst( i, 1 ); 

 bool we_value_iteration(true);

 while (we_value_iteration)
  { // 1) Sort. 
    // 2) deallocate and remove any trivial relators 
    // 3) apply shorter relators to longer ones to reduce length of relators, possibly triggering we_value_iteration
    // 4) use relators to kill generators
    // 5) Look for convienient Nielsen transforms. TODO
    // 6) return to (1) if we_value_iteration

   we_value_iteration = false; // set this back to true if we ever make a successful substitution. 
   relatorList.sort( compare_length ); // (1)

   // start (2) deletion of 0-length relators
   it = relatorList.begin();  
   while ( it != relatorList.end() )  { if ( (*it)->wordLength() == 0 )
       { delete (*it); it = relatorList.erase(it); }  else break; }
   // end (2) deletion of 0-length relators

   // start (3) - apply shorter relators to longer.
   for (it = relatorList.begin(); it != relatorList.end(); it++)
    {
     if ( (*it)->wordLength() > 0 ) // don't bother if this is a trivial word.
      {
       std::list< NGroupExpression* >::iterator tit; // target of it manipulations. 
       tit = it; tit++;
       while (tit != relatorList.end())
 	{// attempt to apply *it to *tit
          std::set< NGroupExpression::NWordSubstitutionData > sub_list; 
          (*tit)->dehnAlgorithmSubMetric( *(*it), sub_list );
          // for now let's just choose the first suggested substitution, provided it exists
          // and has a score of at least one. 
          if (sub_list.size() != 0) if ( (*sub_list.begin()).score > 0 )
	    {
             (*tit)->applySubstitution( *(*it), *sub_list.begin() );
             we_value_iteration = true;
             didSomething = true;
	    }
	  tit++;
	}
      }
    } // end (3) - application of shorter to longer relators.

  // (4) Build and sort a list (by length) of generator-killing relations. So we need to:
  //     (a) Find generator-killing relators
  //     (b) Apply all possible length 1 and length 2 relators
  //     (c) If any length 3 relators, apply a single one then loop back to (1)
  //     (d) Keep a running list of generators we've killed and their substitutions in terms of the new generators. 

  relatorList.sort( compare_length ); 

  // okay, lets start walking through relatorList. Terminate either when we hit the end or a length >=3 relator. 
  //  If we can use the relator *it, do. And record in substitutionTable.
  // begin (4)
  for (it = relatorList.begin(); it!=relatorList.end(); it++)
   {  
    bool word_length_3_trigger(false);
    unsigned long WL ( (*it)->wordLength() );
    // build a table expressing number of times each generator is used in *it. 
    std::vector< unsigned long > genUsage( nGenerators );   
    build_exponent_vec( (*it)->getTerms(), genUsage );

    std::list<NGroupExpressionTerm>::iterator tit;
    for (unsigned long i=0; i<genUsage.size(); i++) if (genUsage[i] == 1)
     { // have we found a substitution for generator i ?
       if ( ( substitutionTable[i].getNumberOfTerms() == 1 ) && ( substitutionTable[i].getGenerator(0) == i ) )
        { // we have a valid substitution.  So we have to replace all occurances of generator
	  // genUsage[i] with the inverse of the remaining word in *both* substitutionTable and relatorList
	  // find where genUsage[i] occurs. 
          bool inv(true); 
          bool before_flag(true); // true if we have not yet encountered generator 
          NGroupExpression prefix, complement; 
          for ( tit = (*it)->getTerms().begin(); ( tit != (*it)->getTerms().end() ); tit++)
           { // skip the term whose generator is the i-th, record whether or not it appears as an inverse...
	     if ( (*tit).generator == i ) { inv = ((*tit).exponent != 1); before_flag=false; } 
             else 
              { if (before_flag) prefix.addTermLast( (*tit) );
                else complement.addTermLast( (*tit) ); }
           } 
	  complement.addTermsLast(prefix);

          if (!inv) complement.invert();
	  // so we sub gi --> complement, in both substitutionTable and relatorList
          for (unsigned long j=0; j<substitutionTable.size(); j++)
	    substitutionTable[j].substitute( i, complement );
	  for (std::list< NGroupExpression* >::iterator pit = relatorList.begin(); pit != relatorList.end(); pit++)
	     { // aha! using it in a nested way!!
	     (*pit)->substitute( i, complement ); // except this
	     }
	  we_value_iteration = true;
          didSomething = true;
	  if (WL>3) word_length_3_trigger=true;

  	  goto found_a_generator_killer;
        }
     } // end (4)

    found_a_generator_killer:
    if (word_length_3_trigger) break; 
   } // end (4)
  // (5) look for convienient Nielsen moves. These are automorphisms of the
  //     the free group that fix all but one generator, which maps as
  //     g_i --> g_i g_j^k for some j \neq i, and any k, similarly
  //     g_i --> g_j^k g_i.  To pull this off I think I'll have to separate
  //     a killTable from the substitutionTable....

  } // end of main_while_loop (6)
 
 // We need to remove the generators that have been killed or expressed
 // in terms of other generators.  We have to similarly re-index the 
 // remaining generators. 
 
 nGenerators = 0;
 for (unsigned long i=0; i<substitutionTable.size(); i++)
  if ( substitutionTable[i].getNumberOfTerms() == 1 ) 
  if ( substitutionTable[i].getGenerator(0) == i ) nGenerators++;

 // now we can build up a mapping of where the current generators get sent to.  
 // make it a std::vector, I suppose.  This will suffice for a looped substitute call. 

 std::vector< unsigned long > genReductionMapping( nGenerators );
 unsigned long indx(0);
 for (unsigned long i=0; i<substitutionTable.size(); i++) 
  {
   if ( substitutionTable[i].getNumberOfTerms() == 1 ) 
   if ( substitutionTable[i].getGenerator(0) == i )
    { genReductionMapping[ indx ] = i;
      indx++; }
  }

 // now let's run through relatorList and substitute genReductionMapping[i] -> i
 for (it = relatorList.begin(); it != relatorList.end(); it++)
   for (unsigned long i=0; i<nGenerators; i++)
    { NGroupExpression gi; gi.addTermFirst( i, 1 );
      (*it)->substitute( genReductionMapping[i], gi ); } 
 // and might as well do substitutionTable, too. 
 for (unsigned long j=0; j<substitutionTable.size(); j++)
   for (unsigned long i=0; i<nGenerators; i++)
    { NGroupExpression gi; gi.addTermFirst( i, 1 );
      substitutionTable[j].substitute( genReductionMapping[i], gi ); } 

 // okay, now let's replace relations with relatorList
 relations.reserve( relatorList.size() );
 for (it = relatorList.begin(); it != relatorList.end(); it++) 
  { relations.push_back( (*it) ); }
  
 NGroupPresentation newGroup( *this );
 reductionMap = new NHomGroupPresentation( oldGroup, newGroup, substitutionTable );
 // now we can initialize reductionMap 
 return didSomething;
}// end dehnAlgorithm()

void NGroupPresentation::operator=(const NGroupPresentation& copyMe)
{
 nGenerators = copyMe.nGenerators; 
 for (unsigned long i=0; i<relations.size(); i++) delete relations[i];
 relations.resize(copyMe.relations.size());
 for (unsigned long i=0; i<relations.size(); i++)
  relations[i] = new NGroupExpression( *copyMe.relations[i] );
}


} // namespace regina

