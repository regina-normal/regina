
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <iterator>
#include <map>
#include <sstream>
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/nabeliangroup.h"
#include "maths/numbertheory.h"
#include "utilities/boostutils.h"
#include "utilities/stlutils.h"
#include "maths/matrixops.h"

namespace regina {

typedef std::list<NGroupExpressionTerm>::iterator TermIterator;
typedef std::list<NGroupExpressionTerm>::const_iterator TermIteratorConst;
typedef std::vector<NGroupExpression*>::iterator RelIterator;
typedef std::vector<NGroupExpression*>::const_iterator RelIteratorConst;
typedef std::list<NGroupExpression*>::iterator TmpRelIterator;

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
                    labs(rel->getExponent(0)) == 1 &&
                    labs(rel->getExponent(1)) == 1 &&
                    rel->getExponent(0) + rel->getExponent(2) == 0 &&
                    rel->getExponent(1) + rel->getExponent(3) == 0)
                out << "Z + Z (abelian)";
        }
// TODO: To add: platonic groups, octahedral/cubical, dihedral,
//       icosahedral/dodecahedral, tetrahedral and binary versions of them.  
//       Free products of torsion groups, free products with amalgamation.
//       currently intelligentSimplify() isn't smart enough for this. 
    } else if (nGenerators == 2 && nRels == 2) {
        // TODO: See if it's the quaternions.
    } else {
        // nGenerators >= 2 and nRels >= 2.
        // Don't have anything intelligent to say at this point.
    }
    return out.str();
}


std::auto_ptr<NAbelianGroup> NGroupPresentation::abelianisation() const
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

std::auto_ptr<NMarkedAbelianGroup> NGroupPresentation::markedAbelianisation() 
const {
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

void NGroupPresentation::dehnAlgorithmSubMetric( 
    const NGroupExpression &this_word, 
    const NGroupExpression &that_word, 
    std::set< NWordSubstitutionData > &sub_list, unsigned long step ) {
 unsigned long this_length ( this_word.wordLength() );
 unsigned long that_length ( that_word.wordLength() );
 // generic early exit strategy
 if ( (this_length < 2) || (that_length==0) ) return;
 // early exit strategy based on step. 
 if ( (step==1) && ((step+1)*this_length < that_length) ) return; 
 // TODO: should check to whatever extent the above is of much use...

 // this -> splayed to this_word, that_word -> reducer
 std::vector< NGroupExpressionTerm > this_word_vec( 0 );
 std::vector< NGroupExpressionTerm > reducer( 0 ); 
 this_word_vec.reserve( this_length ); reducer.reserve( that_length );
 std::list<NGroupExpressionTerm>::const_iterator it; 
 for (it = this_word.getTerms().begin(); it!=this_word.getTerms().end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     this_word_vec.push_back( NGroupExpressionTerm( (*it).generator, 
        ((*it).exponent>0) ? 1 : -1 ) );  }
 for (it = that_word.getTerms().begin();
        it!=that_word.getTerms().end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     reducer.push_back( NGroupExpressionTerm( (*it).generator, 
        ((*it).exponent>0) ? 1 : -1 ) );    }
 std::vector< NGroupExpressionTerm > inv_reducer( that_length );
 for (unsigned long i=0; i<reducer.size(); i++) 
        inv_reducer[that_length-(i+1)] = reducer[i].inverse(); 

 // search for cyclic subwords of reducer in this_word_vec...
 for (unsigned long i=0; i<this_length; i++) 
        for (unsigned long j=0; j<that_length; j++)
  { 
    unsigned long comp_length = 0; 
    while ( (this_word_vec[(i+comp_length) % this_length] == 
                reducer[(j+comp_length) % that_length]) && 
            (comp_length < that_length) && (comp_length < this_length) ) 
                comp_length++;  
    NWordSubstitutionData subData;
    subData.invertB=false; subData.sub_length=comp_length;
         subData.start_sub_at=i; subData.start_from=j;
    if (comp_length == that_length)	
	{ 
	  subData.score = that_length;
      unsigned long a=1; 
	  while ( (this_word_vec[( (i+this_length)-a )%this_length].inverse()==
                  this_word_vec[( (i+comp_length)+(a-1) )%this_length]) &&
		  (2*a+that_length <= this_length ) ) { a++; subData.score++; }
	  sub_list.insert(subData);
	}
    else if ( comp_length > 0 ) 
        { 
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
    subData.invertB=true; subData.sub_length=comp_length; 
    if (comp_length == that_length)	
	{ 
	  subData.score = that_length;
      unsigned long a=1; 
	  while ( (this_word_vec[( (i+this_length)-a )%this_length].inverse()==
                this_word_vec[( (i+comp_length)+(a-1) )%this_length]) &&
		  (2*a+that_length <= this_length ) ) { a++; subData.score++; }
	  sub_list.insert(subData);
	}
    else if ( comp_length > 0 ) 
    { 
     subData.score = 2*comp_length - that_length;
     if ( subData.score > -((long int)step) )
      sub_list.insert(subData);
    }
  }
} 

/**
 *  This applies a substitution generated by dehnAlgorithmSubMetric. 
 */
void NGroupPresentation::applySubstitution( NGroupExpression& this_word,
                        const NGroupExpression &that_word, 
                        const NWordSubstitutionData &sub_data )
{
 // okay, so let's do a quick cut-and-replace, reduce the word and hand it back. 
 unsigned long this_length ( this_word.wordLength() );
 unsigned long that_length ( that_word.wordLength() );
 std::vector< NGroupExpressionTerm > this_word_vec( 0 );
 std::vector< NGroupExpressionTerm > reducer( 0 ); // we'll splay-out *this and 
  // that_word so that it's easier to search for commonalities.
 this_word_vec.reserve( this_length ); reducer.reserve( that_length );
 std::list<NGroupExpressionTerm>::const_iterator it; 
 // start the splaying of terms
 for (it = this_word.getTerms().begin(); it!=this_word.getTerms().end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     this_word_vec.push_back( NGroupExpressionTerm( (*it).generator, 
                ((*it).exponent>0) ? 1 : -1 ) );  }
  // and that_word
 for (it = that_word.getTerms().begin(); it!=that_word.getTerms().end(); it++)
  { for (unsigned long i=0; i<abs((*it).exponent); i++)
     reducer.push_back( NGroupExpressionTerm( (*it).generator, 
         ((*it).exponent>0) ? 1 : -1 ) );    }
 // done splaying, produce inv_reducer
 std::vector< NGroupExpressionTerm > inv_reducer( that_length );
 for (unsigned long i=0; i<that_length; i++) inv_reducer[that_length-(i+1)] = 
         reducer[i].inverse(); 
 // done with inv_reducer, erase terms
 this_word.getTerms().clear();

 // *this word is some conjugate of AB and the relator is some conjugate of AC.
  //  We are performing the substitution
 // A=C^{-1}, thus we need to produce the word C^{-1}B. Put in C^{-1} first..
 for (unsigned long i=0; i<(that_length - sub_data.sub_length); i++)
  this_word.getTerms().push_back( sub_data.invertB ?     
        reducer[(that_length - sub_data.start_from + i) % that_length] : 
    inv_reducer[(that_length - sub_data.start_from + i) % that_length] );
 // iterate through remainder of this_word_vec, starting from 
 //     sub_data.start_sub_at + sub_length, ie fill in B 
 for (unsigned long i=0; i<(this_length - sub_data.sub_length); i++)
  this_word.getTerms().push_back( this_word_vec[(sub_data.start_sub_at + sub_data.sub_length + i) %
         this_length] );
 // done
 this_word.simplify();
}

namespace {
    // An anonymous namespace for some helper routines.

    bool compare_length( const NGroupExpression* first, 
             const NGroupExpression* second ) 
     {  return ( first->wordLength() < second->wordLength() ); }

    /**
     *  This routine takes a list of words, together with expVec. It's assumed
     * expVec is initialized to be zero and as long as the number of generators 
     * in the group.  What this routine does is, for each generator of the 
     * group, it records the sum of the absolute value of the exponents of that
     * generator in word.  For the i-th generator this number is recorded in
     * expVec[i]. 
     */
    void build_exponent_vec( const std::list< NGroupExpressionTerm > & word, 
                              std::vector<unsigned long> &expVec ) 
    {
     std::list<NGroupExpressionTerm>::const_iterator tit;
     for ( tit = word.begin(); tit != word.end(); tit++) 
         expVec[ (*tit).generator ] += abs( (*tit).exponent );
    }

    /*
     * Commenting out since this is not used, and requires access to a
     * private struct.  If you want to put it back, it should probably
     * be a member function of NWordSubstitutionData. - bab
     *
    // gives a string that describes the substitution
    std::string substitutionString( const NGroupExpression &word, 
                    const NGroupPresentation::NWordSubstitutionData &subData )
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
     for (unsigned long i=0; i<word_length; i++)
        inv_reducer[word_length-(i+1)] = reducer[i].inverse(); 
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
     retval = del_word.toString()+" -> "+rep_word.toString();
     return retval;
    }
    */
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

/**
 *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_i2^j2 ... g_in^jn g_i1^j1  
 */
void NGroupExpression::cycleRight()
{
 if (terms.size() > 1)
  {
   NGroupExpressionTerm temp(terms.front());
   terms.pop_front();
   terms.push_back(temp);
  }
}

/**
 *  Given a word of the form g_i1^j1 g_i2^j2 ... g_in^jn
 * converts the word into g_in^jn g_i1^j1 g_i1^j1 ... g_in-1^jn-1  
 */
void NGroupExpression::cycleLeft()
{
 if (terms.size() > 1)
  {
   NGroupExpressionTerm temp(terms.back());
   terms.pop_back();
   terms.push_front(temp);
  }
}

bool NGroupPresentation::simplifyWord( NGroupExpression &input ) const
 {
  bool retval(false);
  for (unsigned long i=0; i<relations.size(); i++)
   { // apply relations[i] to input word.
     std::set< NWordSubstitutionData > sub_list;
     dehnAlgorithmSubMetric( input, *relations[i], sub_list );
     if (sub_list.size() != 0) if ( (*sub_list.begin()).score > 0 )
        {
        applySubstitution( input, *relations[i], *sub_list.begin() );
        retval = true; 
        }
   }
  return retval;
 }

bool NGroupPresentation::intelligentSimplify() {
    // This is not inline because it needs the NHomGroupPresentation
    // class definition, so that the auto_ptr destructor works correctly.
    return intelligentSimplifyDetail().get();
}

// TODO: we should make this routine more modular, calling simpler more "canned" 
//  group modification routines in a more transparent way. 
std::auto_ptr<NHomGroupPresentation>
        NGroupPresentation::intelligentSimplifyDetail() {
 bool didSomething(false);
 // start by taking a copy of *this group, for construction of reductionMap
 NGroupPresentation oldGroup( *this );

 // let's start with the shortest relators first, and see to what extent we can
 // use them to simplify the other relators.  To do this we'll need a temporary 
 // relator table, sorted on the length of the relators. 
 std::list< NGroupExpression* >::iterator it;  
 std::list< NGroupExpression* > relatorList; 
 for (unsigned long i=0; i<relations.size(); i++) 
   relatorList.push_back( relations[i] );
 relations.resize(0); // we'll rebuild the relations later. 

 // the substitutionTable keeps track of how generators are killed by relators. 
 // substitutionTable[i] != generator gi if and only the algorithm discovers 
 //  gi to be trivial in the presentation or expressible in terms of other 
 // generators
 std::vector< NGroupExpression > substitutionTable( nGenerators ); 
 for (unsigned long i=0; i<nGenerators; i++) 
   substitutionTable[i].addTermFirst( i, 1 ); 

 bool we_value_iteration(true);

 while (we_value_iteration)
  { // 1) Sort. 
    // 2) deallocate and remove any trivial relators from relatorList 
    // 3) apply shorter relators to longer ones to reduce length of relators, 
    //    possibly triggering we_value_iteration
    // 4) use relators to kill generators
    // 5) Look for convienient Tietze transforms. 
    // 6) return to (1) if we_value_iteration

   we_value_iteration = false; // set to true if we ever make a substitution. 
   // cyclically reduce relators
   for ( it = relatorList.begin(); it != relatorList.end(); it++)
        (*it)->simplify(true); 

   relatorList.sort( compare_length ); // (1)

   // start (2) deletion of 0-length relators
   it = relatorList.begin();  
   while ( it != relatorList.end() )  { if ( (*it)->wordLength() == 0 )
       { delete (*it); it = relatorList.erase(it); }  else break; }
   // end (2) deletion of 0-length relators

   // start (3) - apply shorter relators to longer.
   for (it = relatorList.begin(); it != relatorList.end(); it++)
     if ( (*it)->wordLength() > 0 ) // don't bother if this is a trivial word.
      {
       std::list< NGroupExpression* >::iterator tit; // target of it manips. 
       tit = it; tit++;
       while (tit != relatorList.end())
 	   {// attempt to apply *it to *tit
          std::set< NWordSubstitutionData > sub_list; 
          dehnAlgorithmSubMetric( **tit, **it, sub_list );
          // for now let's just choose the first suggested substitution, 
          // provided it exists and has a score of at least one. 
          if (sub_list.size() != 0) if ( (*sub_list.begin()).score > 0 )
	    {
             applySubstitution( **tit, **it, *sub_list.begin() );
             we_value_iteration = true;
             didSomething = true;
	    }
	   tit++;
	   }
    } // end (3) - application of shorter to longer relators.

  // (4) Build and sort a list (by length) of generator-killing relations. 
  //     So we need to:
  //     (a) Find generator-killing relators
  //     (b) Apply all possible length 1 and length 2 relators
  //     (c) If any length 3 relators, apply a single one then loop back to (1)
  //     (d) Keep a running list of generators we've killed and their 
  //         substitutions in terms of the new generators. 

  relatorList.sort( compare_length ); 

  // okay, lets start walking through relatorList. Terminate either when we hit 
  // the end or a length >=3 relator.  If we can use the relator *it, do. And 
  // record in substitutionTable. begin (4)
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
       if ( ( substitutionTable[i].getNumberOfTerms() == 1 ) && 
            ( substitutionTable[i].getGenerator(0) == i ) )
        { // we have a valid substitution.  So we have to replace all occurances 
          // of generator genUsage[i] with the inverse of the remaining word in
          // *both* substitutionTable and relatorList find where genUsage[i] 
          // occurs. 
          bool inv(true); 
          bool before_flag(true); // true if we have not yet encountered gen 
          NGroupExpression prefix, complement; 
          for ( tit = (*it)->getTerms().begin(); 
                ( tit != (*it)->getTerms().end() ); tit++)
           { // skip the term whose generator is the i-th, record whether 
             // or not it appears as an inverse...
	     if ( (*tit).generator == i ) { 
             inv = ((*tit).exponent != 1); before_flag=false; } 
             else 
              { if (before_flag) prefix.addTermLast( (*tit) );
                else complement.addTermLast( (*tit) ); }
           } 
          complement.addTermsLast(prefix);
          if (!inv) complement.invert();
	      // sub gi --> complement, in both substitutionTable and relatorList
         for (unsigned long j=0; j<substitutionTable.size(); j++)
	    substitutionTable[j].substitute( i, complement );
	    for (std::list< NGroupExpression* >::iterator pit = relatorList.begin(); 
               pit != relatorList.end(); pit++)
	     (*pit)->substitute( i, complement );
	    we_value_iteration = true;
        didSomething = true;
	    if (WL>3) word_length_3_trigger=true;
  	    goto found_a_generator_killer;
        }
     } // the look through genUsage loop

    found_a_generator_killer:
    if (word_length_3_trigger) break; 
   } // end (4)
  } // end of main_while_loop (6)
 
 // We need to remove the generators that have been killed or expressed
 // in terms of other generators.  We have to similarly re-index the 
 // remaining generators. 

 nGenerators = 0;
 for (unsigned long i=0; i<substitutionTable.size(); i++)
  if ( substitutionTable[i].getNumberOfTerms() == 1 ) 
  if ( substitutionTable[i].getGenerator(0) == i ) nGenerators++;

 // now we can build up a mapping of where the current generators get sent to.  
 // make it a std::vector.

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

 // and build the reverse isomorphism from the new group to the old
 std::vector< NGroupExpression > revMap(nGenerators);
 for (unsigned long i=0; i<revMap.size(); i++)
  revMap[i].addTermFirst( genReductionMapping[i], 1 );

 if (didSomething) {
   // now we can initialize reductionMap 
   return std::auto_ptr<NHomGroupPresentation>(new NHomGroupPresentation(
    oldGroup, *this, substitutionTable, revMap));
 } else
   return std::auto_ptr<NHomGroupPresentation>();
}// end dehnAlgorithm()

NGroupPresentation& NGroupPresentation::operator=(
        const NGroupPresentation& copyMe) {
    nGenerators = copyMe.nGenerators; 
    for (unsigned long i=0; i<relations.size(); i++)
        delete relations[i];
    relations.resize(copyMe.relations.size());
    for (unsigned long i=0; i<relations.size(); i++)
        relations[i] = new NGroupExpression( *copyMe.relations[i] );
    return *this;
}

/**
 *  This routine attempts to rewrite the presentation so that generators
 * of the group map to generators of the abelianization, with any
 * left-over generators mapping to zero. 
 */
bool NGroupPresentation::linearRewriting()
{
 // step 1: compute abelianization and how generators map to abelianization.
 std::auto_ptr< NMarkedAbelianGroup > abelianized( markedAbelianisation() );
 NMatrixInt abMat( abelianized->minNumberOfGenerators(), getNumberOfGenerators() );
 for (unsigned long j=0; j<getNumberOfGenerators(); j++)
  {
  std::vector<NLargeInteger> epsilon( getNumberOfGenerators() );
  epsilon[j] = 1;
  std::vector<NLargeInteger> temp( abelianized->snfRep(epsilon) );
  for (unsigned long i=0; i<abelianized->minNumberOfGenerators(); i++)
    abMat.entry(i,j) = temp[i];   
  }

 unsigned long SR( abelianized->getNumberOfInvariantFactors() );

 // step 2: we will mimic the simple smith normal form algorithm algorithm 
 //         here, using corresponding moves on the group presentation. 
 for (unsigned long i=abelianized->getNumberOfInvariantFactors(); 
      i<abelianized->minNumberOfGenerators(); i++)
  { // in row i we will eliminate all but one entry using column
    // operations.  Now we need to do a while loop -- find any two non-zero
    // entries in the row, and reduce.  If there's only one non-zero entry, 
    // we're done. 
    unsigned long j0=0, j1=abMat.columns()-1;
    while (j0 < j1)
    { // if at j0 its zero, inc, if at j1 zero, dec
      if (abMat.entry(i,j0).isZero()) { j0++; continue; } 
      if (abMat.entry(i,j1).isZero()) { j1--; continue; }
      // column op! 
      if (abMat.entry(i,j0).abs() < abMat.entry(i,j1).abs())
       { NLargeInteger q = abMat.entry(i,j1) / abMat.entry(i,j0); 
         // subtract q times column j0 from column j1 
         for (unsigned long r=0; r<abMat.rows(); r++)
          { abMat.entry(r,j1) -= abMat.entry(r,j0)*q; }
            nielsenCombine(j1, j0, -q.longValue() );
       }
      else // (i,j0).abs >= (i,j1).abs()
       { NLargeInteger q = abMat.entry(i,j0) / abMat.entry(i,j1); 
         // substract q times column j1 from column j0 
         for (unsigned long r=0; r<abMat.rows(); r++)
          { abMat.entry(r,j0) -= abMat.entry(r,j1)*q; }
          nielsenCombine(j0, j1, -q.longValue() );
       }
    }
   // okay j0 and j1 should be equal.  Let's permute and change sign if appropriate. 
        { nielsenTransposition(i, j1); abMat.swapColumns(i, j1); }
   // TODO signs
  }

 // step 3: kill the free parts of the abelianizations of the other generators.
 //         these are row ops. 
 // TODO the torsion part...

 // step 4: reduce the torsion part to generators modulo invariant factors.
}

// TODO: we should probably make this more sophisticated at some point.  For
//  example < a, b : a^2, abaB > it would not detect as abelian. 
bool NGroupPresentation::isAbelian() const
 {
  // The idea will be to take all commutators of the generators, and see if
  //  the relators can kill them. 
  for (unsigned long i=0; i<nGenerators; i++)
   for (unsigned long j=i+1; j<nGenerators; j++)
    {
     NGroupExpression COM; // commutator [gi,gj]
     COM.addTermLast( i, 1 );   COM.addTermLast( j, 1 );
     COM.addTermLast( i, -1 );  COM.addTermLast( j, -1 );
     // reduce! We use dehnAlgorithmSubMetric
     std::set< NWordSubstitutionData > sub_list;
     // loop through all relators in the group, X. 
     bool commute(false);
     for (unsigned long k=0; (k<relations.size()) && (!commute); k++)
      {
       dehnAlgorithmSubMetric( COM, *relations[k], sub_list, 1 ); 
       // check to see if sub_list has a score=4 move.
       for (std::set< NWordSubstitutionData >::iterator I=sub_list.begin();
            I!=sub_list.end(); I++)
        if (I->score == 4) { commute = true; break; }
      }
     if (!commute) return false; 
    }
  return true;
 }

bool NGroupPresentation::nielsenTransposition(const unsigned long &i, 
                                              const unsigned long &j)
{
    if (i==j) return false;
    bool retval=false;
    for (unsigned long l=0; l<relations.size(); l++)
     {
      std::list<NGroupExpressionTerm>& terms( relations[l]->getTerms() );
      for (std::list<NGroupExpressionTerm>::iterator k=terms.begin(); 
           k!=terms.end(); k++)
        { 
          if (k->generator == i) { k->generator = j; retval = true; }
          else if (k->generator == j) { k->generator = i; retval = true; } 
        }
     }
    return retval;
}

bool NGroupPresentation::nielsenInvert(const unsigned long &i)
{
    bool retval=false;
    for (unsigned long l=0; l<relations.size(); l++)
     {
      std::list<NGroupExpressionTerm>& terms(relations[l]->getTerms());
      for (std::list<NGroupExpressionTerm>::iterator k=terms.begin(); 
           k!=terms.end(); k++)
        {
        if (k->generator == i) { k->exponent=(-k->exponent); retval = true; }
        }
     }
    return retval;
}

bool NGroupPresentation::nielsenCombine(const unsigned long &i, 
    const unsigned long &j, const signed long &k,  const bool &flag)
{ // replace ri with (ri)(rj)^(-k)
    bool retval(false);
    NGroupExpression let;
    if (flag) { let.addTermFirst(i, 1); let.addTermLast(j, -k); }
    else { let.addTermLast(i, 1); let.addTermFirst(j, -k); }
    for (unsigned long k=0; k<relations.size(); k++)
          if (relations[k]->substitute(i, let, true)) retval = true;
    return retval;
}


// these macros are used only in the identify_extension_over_Z routine below.
#define idx(gen, cov) ((unsigned long)(gen-1)+nGm1*cov)
#define unidx(dat) std::pair<unsigned long, unsigned long>((dat % nGm1)+1, dat/nGm1)

// if presentation is of a group that can bet written as an extension
//  0 --> A --> G --> Z --> 0
// this routine is to change the presentation to appear to be such a split
//  extension. 
std::auto_ptr< NHomGroupPresentation > NGroupPresentation::identify_extension_over_Z()
{
 // step 1: let's build the abelianization homomorphism. 
 linearRewriting();
 std::auto_ptr< NMarkedAbelianGroup > abelianized( markedAbelianisation() );
 if (abelianized->getRank() != 1) return std::auto_ptr< NHomGroupPresentation >(NULL);
 if (abelianized->getNumberOfInvariantFactors()>0)  // put Z generator at 0-th
    nielsenTransposition(0, abelianized->getNumberOfInvariantFactors() );

 // We have the presentation of this group in the form
 // < a, g1, g2, ..., gn | r1, ..., rm > with a->1, gi->0 under abelianization
 // 
 // step 2: An infinite presentation of the kernel of the map to Z is given
 //  by < g1i, g2i, ..., gni | r1i, ..., rmi > for all lifts i of the generators
 //  and relators above, after collapsing "a". We can collapse this to a finite
 //  presentation if and only if unique max and minima (in the Z cover) exist
 //  among the lifted relators.  So we check for that. 
 
 // lifts stores the lifts of the ri's, after crushing the lifts of the a's. 
 std::vector< std::list< std::pair< NGroupExpressionTerm, signed long > > > 
    lifts( relations.size() );

 // the following max/minKiller give a list of the found pairs
 //  (generator index, relator index) to keep track of which relators we can 
 //  use to kill generators (in the covering space).
 std::map< unsigned long, unsigned long > maxKiller;
 std::map< unsigned long, unsigned long > minKiller;
 std::map< unsigned long, unsigned long > cellWidth; // 2-cell width in cover

 for (unsigned long l=0; l<relations.size(); l++)
     { // for each relator determine highest and lowest lifts, and if they
       // are unique or not.

      signed long lift=0;
      signed long maxLift(0), minLift(0);   // sheet index
      unsigned long maxCell(0), minCell(0); // generator's index in presentation
      bool dupMax(false), dupMin(false);    // have we found duplicate lift height?
      std::list<NGroupExpressionTerm>& terms(relations[l]->getTerms());
      for (std::list<NGroupExpressionTerm>::reverse_iterator k=terms.rbegin(); 
           k!=terms.rend(); k++)
        { // right to left through the relator
          if (k->generator > 0) {
            lifts[l].push_back( std::pair< NGroupExpressionTerm, signed long >
              ( *k, lift ) ); 
            // special case if maxCell and minCell not yet initialized.
            if (maxCell==0) { maxLift = lift; minLift = lift; maxCell = k->generator;
                              minCell = k->generator; 
              dupMax = (labs(k->exponent)==1) ? false : true;
              dupMin = (labs(k->exponent)==1) ? false : true; }
            else { // back to regular case
              if (lift > maxLift) { maxLift = lift; dupMax = (labs(k->exponent)==1) ?
               false : true; maxCell = k->generator; } 
              else if (lift==maxLift) { dupMax = true; }
               if (lift < minLift) { minLift = lift; dupMin = (labs(k->exponent)==1) ?
                false : true; minCell = k->generator; }
              else if (lift==minLift) { dupMin = true; }
                 }
             }
          else lift += k->exponent;
        }
      // maxCell and minCell have to be non-zero at this point.
      cellWidth[l] = (unsigned long)(maxLift - minLift); 

      if ( (maxCell!=0) && (!dupMax) ) 
       { 
         std::map< unsigned long, unsigned long>::iterator I=maxKiller.find(maxCell);
         if (I!=maxKiller.end()) // compare the current maxKiller[maxCell] to l. 
           { if (cellWidth[l] > cellWidth[ I->second ]) maxKiller[maxCell]=l; }
         else maxKiller[maxCell]=l; 
       }
      if ( (minCell!=0) && (!dupMin) ) 
       { 
         std::map< unsigned long, unsigned long>::iterator I=minKiller.find(minCell);
         if (I!=minKiller.end()) // compare the current maxKiller[minCell] to l. 
           { if (cellWidth[l] > cellWidth[ I->second ]) minKiller[minCell]=l; }
         minKiller[minCell]=l; 
       }      
      // now let's readjust the relator so that its minLift is at level 0.
      if (minLift != 0)
       {
         relations[l]->addTermFirst(0, minLift);
         relations[l]->addTermLast(0, -minLift);
         relations[l]->simplify();
         for (std::list< std::pair< NGroupExpressionTerm, signed long > >::iterator I=
          lifts[l].begin(); I!=lifts[l].end(); I++)
           I->second -= minLift; // adjust the lifts to have min lift 0
       }
      // cyclically permute lifts so that the max-weight rep appears first
      while (lifts[l].front().second != cellWidth[l])
       { std::pair< NGroupExpressionTerm, signed long > temp(lifts[l].front());
         lifts[l].pop_front();
         lifts[l].push_back( temp ); 
       }
      // ensure word starts with highest-weight element as inverted.
      if (lifts[l].front().first.exponent == 1)
         {
           std::pair< NGroupExpressionTerm, signed long > temp(lifts[l].front());
           lifts[l].pop_front();
           lifts[l].reverse();
           lifts[l].push_front(temp); // now run and change the exponents
           for (std::list< std::pair< NGroupExpressionTerm, signed long > 
            >::iterator I=lifts[l].begin(); I!=lifts[l].end(); I++)
            I->first.exponent = -I->first.exponent;
         }
     }
 // this is the test for whether or not we can find a finite collection of
 // generators
 unsigned long nGm1( nGenerators - 1 );
 if ( (maxKiller.size() != nGm1) || (minKiller.size() != nGm1) )
    return std::auto_ptr< NHomGroupPresentation >(NULL); 

 unsigned long maxWidth(0);
 unsigned long liftCount(0); // how many lifts of our generators do we need?
 for (unsigned long i=0; i<maxKiller.size(); i++)
  {
  if (cellWidth[maxKiller[i]]>liftCount) liftCount = cellWidth[maxKiller[i]];
  if (cellWidth[minKiller[i]]>liftCount) liftCount = cellWidth[minKiller[i]];
  }
 for (std::map< unsigned long, unsigned long>::iterator I=cellWidth.begin(); 
      I!=cellWidth.end(); I++)
  if (I->second > maxWidth) maxWidth = I->second;
 // we need liftCount lifts of our generators and relators.  Perhaps we should
 // either cite something in Magnus-Karass-Solitar for this or put in a proof.
 // let's build a vector that describes the relationa a(gi)a^-1 = ...

// NOTE: presentations like < a | > are fine with liftCount==0. 
// if (liftCount==0) { std::cout<<"identify_extension_over_Z() error: liftcount is zero.\n";
//   std::cout<<"Group: "<<this->toStringCompact()<<"\n";
//   std::cout.flush(); exit(1); }

 // build table of reductions of the liftCount==M lift of generators.  
 // the indexing of the generators of the kernel of G --> Z will be handled
 // by the above idx and unidx macros.
 std::map<unsigned long, NGroupExpression> genKiller;
 // start with the liftCount lift, i.e. the first order reducers a^-Mg_ia^M =...
 for (unsigned long i=1; i<getNumberOfGenerators(); i++)
  {
    NGroupExpression temp;
    // maxKiller[i] is the index in lifts of the relator that kills generator gi
    // i is a liftIdx
    unsigned long delta(0);
    for (std::list< std::pair< NGroupExpressionTerm, signed long > 
            >::iterator I=lifts[maxKiller[i]].begin(); I!=lifts[maxKiller[i]].end();
             I++)
    { 
     if (I==lifts[maxKiller[i]].begin()) 
       { // push up delta sheets so that it kills appropriately
        delta = (unsigned long)(liftCount - I->second);
        continue;
       }  
     temp.addTermFirst( NGroupExpressionTerm( 
        idx( I->first.generator, I->second + delta ), I->first.exponent ) );
    }
   genKiller.insert( std::pair< unsigned long, NGroupExpression >
                     (idx(i,liftCount),temp) );
  }

 // extra genKillers -- sometimes there are wider words than the killing words. 
 //  like with presentations such as:
 //   < a b | b a^-1 b a^-1 b^-1 a^2, a^-3 b^2 a^3 b^2 >
 // We could alternatively use the genKiller to reduce the width of the other
 // relators.  But for now we use this less-sophisticated work-around. 
 for (unsigned long j=liftCount; j<maxWidth; j++)
  { 
    for (unsigned long i=1; i<getNumberOfGenerators(); i++)
     { // bump-up lift of each genKiller then apply previous genKillers to them 
       // to create word in the fibre group.
       NGroupExpression tempW( genKiller[idx(i, j)] );
       for (std::list<NGroupExpressionTerm>::iterator I=tempW.getTerms().begin();
            I!=tempW.getTerms().end(); I++) I->generator += nGm1;
       for (std::map<unsigned long, NGroupExpression>::iterator J=genKiller.begin();
         J!=genKiller.end(); J++)
            tempW.substitute( J->first, J->second, false );
       genKiller.insert( std::pair< unsigned long, NGroupExpression >
                (idx(i,j+1), tempW) );
     }
  }

 //  initialize tempTable with the 0-th lifts of the relators. 
 std::list< NGroupExpression > tempTable;
 NGroupPresentation kerPres;
 kerPres.addGenerator( liftCount * nGm1 );

 for (unsigned long i=0; i<lifts.size(); i++)
 {
  NGroupExpression temp;
  for (std::list< std::pair< NGroupExpressionTerm, signed long > 
            >::iterator I=lifts[i].begin(); I!=lifts[i].end(); I++)
     temp.addTermFirst( NGroupExpressionTerm( 
        idx( I->first.generator, I->second ), I->first.exponent ) );
  for (std::map<unsigned long, NGroupExpression>::iterator J=genKiller.begin();
         J!=genKiller.end(); J++)
      temp.substitute( J->first, J->second, false );
  temp.simplify(); 
  if (temp.wordLength()>0)
    {
    tempTable.push_back(temp);
    kerPres.addRelation( new NGroupExpression(temp) );
    }
 }
 if (!kerPres.isValid()) { std::cout<<"identify_extension_over_Z() error: out of bounds relator in kerPres.\n";
  std::cout.flush();   exit(1); }
 // now we build the reductions of the {0,1,...,liftCount-1} translates of **all**
 // the relators from the group, and assemble them into the relators of the
 // kernel.
 for (unsigned long M=0; M<liftCount; M++)
  { // increment the words in tempTable
   for ( std::list< NGroupExpression >::iterator I=tempTable.begin(); 
         I != tempTable.end(); I++)
   { 
    std::list< NGroupExpressionTerm >& It(I->getTerms() );
    for (std::list<NGroupExpressionTerm>::iterator J=It.begin();
         J!=It.end(); J++)
      J->generator += nGm1; // this depends on choice of idx function
    for (std::map<unsigned long, NGroupExpression>::iterator J=genKiller.begin();
         J!=genKiller.end(); J++)
      I->substitute( J->first, J->second, false );
    // apply genKiller to reduce the words, and push to presentation
    kerPres.addRelation( new NGroupExpression( *I ) );   
   }
  }
  // replace this presentation by the semi-direct product presentation.
  std::vector<NGroupExpression> autVec;

  autVec.resize( nGm1*liftCount );
  for (unsigned long i=0; i<autVec.size(); i++) // this part depends on idx
   if ( i >= nGm1*(liftCount-1) ) 
     autVec[i] = genKiller[i+nGm1]; 
   else 
     { 
     NGroupExpression temp;
     temp.addTermFirst( i+nGm1, 1 );
     autVec[i] = temp;
     }

  std::auto_ptr< NHomGroupPresentation > retval(new NHomGroupPresentation( kerPres, kerPres, autVec ) );
  retval->intelligentSimplify();

  // Modify this presentation to reflect the semi-direct product
  //        structure we've discovered! 
  // now deallocate relations, and resize and repopulate with copies of kerPres's
  //  relations. 

  nGenerators = retval->getDomain().nGenerators + 1;
  for (unsigned long i=0; i<relations.size(); i++)
   delete relations[i];
  relations.resize( retval->getDomain().nGenerators + retval->getDomain().relations.size() );

  for (unsigned long i=0; i<retval->getDomain().relations.size(); i++)
   relations[i] = new NGroupExpression( *retval->getDomain().relations[i] );

  // And now all the b^-1g_ib = genKiller(i) and b^-1g_ib = g_{i+1} relations.
  for (unsigned long i=0; i<retval->getDomain().nGenerators; i++)
   {
    NGroupExpression* temp;
    temp = new NGroupExpression( retval->evaluate(i) );
    temp->addTermFirst( retval->getDomain().nGenerators, 1);
    temp->addTermFirst( i, -1);
    temp->addTermFirst( retval->getDomain().nGenerators, -1); 
    relations[ i+retval->getDomain().relations.size() ] = temp;
   }

 return std::auto_ptr< NHomGroupPresentation >( retval );
}

bool NGroupPresentation::isValid() const
 {
    for (unsigned long i=0; i<relations.size(); i++)
     for (std::list<NGroupExpressionTerm>::const_iterator 
            j=relations[i]->getTerms().begin(); 
            j!=relations[i]->getTerms().end(); j++)
      if (j->generator >= nGenerators) return false;
    return true;
 }


////////////////// ALL INPUT / OUTPUT routines below //////////////////////

// XML output

void NGroupPresentation::writeXMLData(std::ostream& out) const {
    out << "<group generators=\"" << nGenerators << "\">\n";
    for (RelIteratorConst it = relations.begin(); it != relations.end(); it++) {
        out << "  ";
        (*it)->writeXMLData(out);
        out << '\n';
    }
    out << "</group>\n";
}

void NGroupExpression::writeXMLData(std::ostream& out) const {
    out << "<reln> ";
    for (TermIteratorConst it = terms.begin(); it != terms.end(); it++)
        out << (*it).generator << '^' << (*it).exponent << ' ';
    out << "</reln>";
}

// group expression output routines

void NGroupExpression::writeText(std::ostream& out, bool shortword) const {
    if (terms.empty())
        out << '1';
    else {
        std::list<NGroupExpressionTerm>::const_iterator i;
        for (i = terms.begin(); i!=terms.end(); i++) {
            if (i != terms.begin())
                out << ' ';
            if (shortword)
                out << char('a' + i->generator);
            else
                out << "g_" << i->generator;
            if ( i->exponent != 1 )
                out << '^' << i->exponent;
        }
    }
}

std::string NGroupExpression::toTeX() const {
    std::ostringstream out;
    writeTeX(out);
    return out.str();
}

void NGroupExpression::writeTeX(std::ostream& out) const {
    if (terms.empty())
        out << 'e';
    else {
        std::list<NGroupExpressionTerm>::const_iterator i;
        for (i = terms.begin(); i!=terms.end(); i++) {
            out << "g_{" << i->generator << '}';
            if ( i->exponent != 1 )
                out << "^{" << i->exponent << '}';
        }
    }
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

// presentation output routines below

std::string NGroupPresentation::toTeX() const {
    std::ostringstream out;
    writeTeX(out);
    return out.str();
}

void NGroupPresentation::writeTeX(std::ostream& out) const {
    out << "\\langle ";
    if (nGenerators == 0) out << "\\cdot";
    else if (nGenerators == 1) out << "g_0";
    else if (nGenerators == 2) out << "g_0, g_1";
    else {
        out << "g0, \\cdots, g" <<(nGenerators - 1);
    }
    out << " | ";
    if (relations.empty())
        out << "\\cdot";
    else
        for (RelIteratorConst it = relations.begin();
                it != relations.end(); it++) {
            if (it != relations.begin())
                out << ", ";
            (*it)->writeTeX(out);
        }
    out << " \\rangle";
}

void NGroupPresentation::writeTextLong(std::ostream& out) const {
    out << "Generators: ";
    if (nGenerators == 0)
        out << "(none)";
    else if (nGenerators == 1)
        out << "a";
    else if (nGenerators == 2)
        out << "a, b";
    else if (nGenerators <= 26)
        out << "a .. " << char('a' + nGenerators - 1);
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
            (*it)->writeText(out, nGenerators <= 26);
            out << std::endl;
        }
}

std::string NGroupPresentation::toStringCompact() const {
    std::ostringstream out;
    writeTextCompact(out);
    return out.str();
}

void NGroupPresentation::writeTextCompact(std::ostream& out) const {
    if (nGenerators == 0) {
        out << "< >";
        return;
    }

    out << "<";
    if (nGenerators <= 26) {
        for (unsigned long i=0; i<nGenerators; i++)
            out << ' ' << char('a' + i);
    } else {
        out << " g0 .. g" << (nGenerators - 1);
    }

    if (relations.empty()) {
        out << " >";
        return;
    }

    out << " | ";
    for (RelIteratorConst it = relations.begin();
            it != relations.end(); it++) {
        if (it != relations.begin())
            out << ", ";
        (*it)->writeText(out, nGenerators <= 26);
    }
    out << " >";
}


// We will go through, apply dehnAlgSubMetric look for substitutions, 
//  then apply all of them within a reasonable length. 
// if user requests we will go further and do a 2nd iteration with more care...
// depth==1 by default. 

void NGroupPresentation::proliferateRelators(unsigned long depth) {
 std::list< NGroupExpression* > newRels;
 for (unsigned long i=0; i<relations.size(); i++)
 for (unsigned long j=0; j<relations.size(); j++)
  {
   if (i==j) continue; // TODO: maybe accept novel self-substitutions? 
   std::set< NWordSubstitutionData > sub_list; 
   dehnAlgorithmSubMetric( *(relations[i]), *(relations[j]), sub_list, depth );
   while (!sub_list.empty())
    {
     NGroupExpression* newRel( new NGroupExpression( *(relations[i]) ) );
     applySubstitution( *newRel, *(relations[j]), *sub_list.begin() );
     sub_list.erase( sub_list.begin() );
     newRels.push_back(newRel);
    }
  }
 depth--;
 while (depth>0) 
  {
   std::list< NGroupExpression* > tempRels;
   for (unsigned long i=0; i<relations.size(); i++)
   for (std::list< NGroupExpression* >::iterator j=newRels.begin(); 
        j!=newRels.end(); j++)
    { // attempt to tack relation[i] to *j. To do this, we should perhaps 
      // keep a record of how *j was created, as in where the two junction 
      // points are so as to ensure what we're adding spans at least one 
      // of the junctions. 
      std::set< NWordSubstitutionData > sub_list; 
      dehnAlgorithmSubMetric( **j, *(relations[i]), sub_list, depth ); 
      while (!sub_list.empty())
       { // TODO: we might want to avoid some obviously repetitive subs 
         //       as noted above? 
        NGroupExpression* newRel( new NGroupExpression( *(*j) ) );
        applySubstitution( *newRel, *(relations[i]), *sub_list.begin() );
        sub_list.erase( sub_list.begin() );
        tempRels.push_back(newRel);
       }
    }
   depth--;
   while (!tempRels.empty()) { newRels.push_back( *tempRels.begin() ); 
                               tempRels.erase( tempRels.begin() ); }
  }
 while (!newRels.empty()) { relations.push_back( newRels.front() ); 
                            newRels.pop_front(); }
}

} // namespace regina

