
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

#include "algebra/nmvpolynomialring.h"
#include "maths/nrational.h"
#include "maths/nmatrixint.h"
#include "maths/matrixops.h"
#include "maths/npartition.h"

#include <list>
#include <vector>
#include <algorithm>

namespace regina {

/**
 *  Comparison function for polynomials in NMVPolynomialRing< NLargeInteger >
 */
bool MV_polynomial_comparison( const NMVPolynomialRing< NLargeInteger > &first, const NMVPolynomialRing< NLargeInteger > &second)
{
 // first, number of non-zero terms.
 if (first.degree() < second.degree()) return true; if (first.degree() > second.degree()) return false;

 // perhaps add a sort on the size of the bounding diamond. Diameter than lexico on side lengths in decreasing
 // order??  TODO

 // other things to sort on: ?vertices on the boundary? (expensive?), volume? 
 const std::map< NPolynomialIndex< signed long >, NLargeInteger* > fTerms(first.allTerms()); 
 const std::map< NPolynomialIndex< signed long >, NLargeInteger* > sTerms(second.allTerms());
 std::map< NPolynomialIndex< signed long >, NLargeInteger* >::const_iterator fI(fTerms.begin()); 
 std::map< NPolynomialIndex< signed long >, NLargeInteger* >::const_iterator sI(sTerms.begin()); 
 while ( (fI != fTerms.end()) || (sI != sTerms.end()) )
  {
   if (fI->first < sI->first) return true;  if (fI->first != sI->first) return false;
   if ( *(fI->second) < *(sI->second) ) return true;  if ( *(fI->second) > *(sI->second) ) return false;
   fI++; sI++;
  }
 // iterate and check how they compare...
 return true;
}

/**
 * Given a polynomial in n variables, compute the maximum of \pm x1 + ... + \pm xn for all possible \pm
 * signs, indexed by NPartitions of n-element sets.   
 */ // TODO this is presently antiquated.  Delete unless there's a purpose for it.
void buildBoundingDiamond( const NMVPolynomialRing< NLargeInteger > &poly, 
                           std::map< NPartition, signed long > &boundDiamond )
{
 if (poly.degree() == 0) return; boundDiamond.clear();
 std::map< NPolynomialIndex< signed long >, NLargeInteger* >::const_iterator I(poly.allTerms().begin());
 unsigned long DIM(I->first.dim());
 NPartition P( DIM, 0, false );
 // initialize using first term of poly
 while (!P.atEnd())
  {
   signed long sum(0); 
   for (unsigned long i=0; i<DIM; i++)
    { sum += ( P.partition().get(i) ? -I->first.entry(i) : I->first.entry(i)); }
   boundDiamond.insert( std::pair< NPartition, signed long >(P, sum) );
   ++P;
  }
 // now go through the rest of poly.
 I++;
 while (I!=poly.allTerms().end())
  {
   P.reset( DIM, 0, false );
   while (!P.atEnd())
    {
     signed long sum(0); 
     for (unsigned long i=0; i<DIM; i++) // error, should be taking ? max ? min ? something, not sum
      { sum += ( P.partition().get(i) ? -I->first.entry(i) : I->first.entry(i)); }
     boundDiamond[P] = std::max( sum, boundDiamond[P] );
     ++P;
    }
   I++; // advance
  }
}  

/**
 * Given a multi-variable polynomial, multiply it appropriately by \pm 1 t^I so that its terms
 * are as small as possible in the taxicab metric (i1,...,in) --> |i1| + ... + |in|
 */
void recentreNormalize( NMVPolynomialRing< NLargeInteger > &poly, bool &plusBias )
{
 // let's get rid of the silly cases
 if (poly.degree() == 0) return; 
 if (poly.degree() == 1) { poly = poly*NMVPolynomialRing< NLargeInteger >(NLargeInteger::one, -poly.allTerms().begin()->first);
                          return; }
 std::map< NPolynomialIndex< signed long >, NLargeInteger* >::const_iterator ci;
 //okay, we're past the trivial cases. let's figure out the radius of this polynomial. 
 unsigned long dim( poly.allTerms().begin()->first.dim() );
 dim = poly.allTerms().begin()->first.dim();
 // Step 1: let's find R  add up abs of last term of polynomial
 unsigned long R(0); 
 for (unsigned long i=0; i<dim; i++) R += abs(poly.allTerms().rbegin()->first.entry(i));

 // Step 2: support hits which faces? Possible loop-back point.
 std::vector< signed long > delta( dim, 0 );
 find_bdry_faces: // loop to here if we need to re-determine which boundary faces the support touches
 std::vector< bool > touchBdry( 2*dim, false ); // entry [2*i + j] rep whether or not there is element on (top/bot (j)) of i-th bdry face 
 std::vector< bool > besideBdry( 2*dim, false ); 
 for (ci=poly.allTerms().begin(); ci!=poly.allTerms().end(); ci++)
  {
   unsigned long tR(0); 
   for (unsigned long i=0; i<dim; i++) tR+=abs(ci->first.entry(i)-delta[i]);
   if (tR==R) for (unsigned long i=0; i<dim; i++) 
    {
     if (ci->first.entry(i)-delta[i]>=0) touchBdry[2*i]=true; 
     if (ci->first.entry(i)-delta[i]<=0) touchBdry[2*i+1]=true;
    } else
   if (tR+1==R) for (unsigned long i=0; i<dim; i++) 
    {
     if (ci->first.entry(i)-delta[i]>=0) besideBdry[2*i]=true; 
     if (ci->first.entry(i)-delta[i]<=0) besideBdry[2*i+1]=true;
    }
  }

 // Step 3: Find pair of opp faces where you're touching outside of one and not the inside of the other.
 //         if so, shift, reduce R and loop back to find_bdry_faces, if not continue.
 for (unsigned long i=0; i<dim; i++)
  {
   if (touchBdry[2*i]  && !touchBdry[2*i+1] && !besideBdry[2*i+1]) // touch + side, not beside - side
    {// touches the + side not the - side
        delta[i]++; R--;
        goto find_bdry_faces;
    } else
   if (!touchBdry[2*i] && touchBdry[2*i+1] && !besideBdry[2*i]) // touch - side, not beside + side
    {// touches the - side, not the + side
        delta[i]--;  R--;
        goto find_bdry_faces;
    } else
   if ( plusBias && !touchBdry[2*i]) // does not touch + side and we don't like that
    {
        delta[i]++; // normalize to the + side if possible, i.e. 
        goto find_bdry_faces; // favour 1 + x over x^{-1} + 1
    } else
   if ( !plusBias && !touchBdry[2*i+1]) // does not touch the - side and we don't like that
    {
        delta[i]--; // normalize to the + side if possible, i.e. 
        goto find_bdry_faces; // favour 1 + x over x^{-1} + 1
    }
  } 

 // Step 4:  modify polynomial appropriately. 
 NPolynomialIndex< signed long > Delta(dim); 
 for (unsigned long i=0; i<dim; i++) Delta[i] = -delta[i];
 NMVPolynomialRing< NLargeInteger > trans( NLargeInteger::one, Delta );
 poly = poly * trans;

 // Step 5: ensure the smallest coefficient is positive
 if (*poly.allTerms().begin()->second < 0) poly*=(-NLargeInteger::one);
}

// TODO this will be the remainder / division algorithm. 
// To implement this we need to know which elements from the ideal can be translated to be inside the bounding
// diamong for elt. 
bool reduceByIdeal( const std::list< NMVPolynomialRing< NLargeInteger > > &ideal, NMVPolynomialRing< NLargeInteger > &elt )
{
 if (elt.isZero()) return true; 
 if (ideal.size()==0) return false;
 recentreNormalize( elt, false ); // normalize, shift to left-side

 bool didSomething;
 do 
 { 
   didSomething=false;
/*
   // build a vector consisting of elements of ideal with ?? in dimension 1
   // it's a simple width constraint. Here...more subtle... TODO
   std::vector< NMVPolynomialRing< NLargeInteger > > indxId;
   std::list< NMVPolynomialRing< NLargeInteger > >::const_iterator i;
   for (i=ideal.begin(); i!=ideal.end(); i++)
    if (i->width() <= elt.width()) indxId.push_back(*i);
   // if indxId empty, we're done.. 
   if (indxId.size()==0) continue;

   // step 1: build vector of right-leading coeffs from indxId
   std::vector< NLargeInteger > leadV( indxId.size() );
   for (unsigned long i=0; i<leadV.size(); i++)
    leadV[i] = indxId[i].lastTerm().second;
   // step 2: check divisibility
   std::vector< NLargeInteger > gcdV( indxId.size() ), killV( indxId.size() ); 
   NLargeInteger G(gcd(leadV, gcdV, killV)); 

   if (elt.lastTerm().second % G == NLargeInteger::zero) 
    { 
     didSomething = true;
     NLargeInteger q( elt.lastTerm().second.divExact(G) );
     signed long topD( elt.lastTerm().first );
    for (unsigned long i=0; i<gcdV.size(); i++)
      elt -= NMVPolynomialRing< NLargeInteger >( q*gcdV[i], topD-indxId[i].lastTerm().first )*indxId[i];
    }

   if ((!didSomething) && laurentPoly) // if no right-reductions worked ,try left-reductions
    {
     // step 1: build vector of right-leading coeffs from indxId
     for (unsigned long i=0; i<leadV.size(); i++)
      leadV[i] = indxId[i].firstTerm().second;
     // step 2: check divisibility
     NLargeInteger G(gcd(leadV, gcdV, killV)); 
     if (elt.lastTerm().second % G == NLargeInteger::zero) 
      { 
       didSomething = true;
       NLargeInteger q( elt.firstTerm().second.divExact(G) );
       signed long topD( elt.firstTerm().first );
       for (unsigned long i=0; i<gcdV.size(); i++)
        elt -= NMVPolynomialRing< NLargeInteger >( q*gcdV[i], topD-indxId[i].firstTerm().first )*indxId[i];
      }
    }
*/
 }
 while ( (didSomething) && (!elt.isZero()) );
 recentreNormalize( elt, true ); // normalize, shift to right side for presentation purposes

 return elt.isZero();
}

/**
 * Removes zeros, normalizes so 0th term is first non-zero term and 
 * positive. Sorts, removes duplicates.
 */
/*
void reduceIdealSortStep( std::list< NMVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NMVPolynomialRing< NLargeInteger > >::iterator i;
 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   while (i->isZero()) i=ideal.erase(i);  if (i==ideal.end()) break;
   std::pair< signed long, NLargeInteger > LT( i->firstTerm() );
   NMVPolynomialRing< NLargeInteger > opTerm( (LT.second>0) ? NLargeInteger::one : -NLargeInteger::one, -LT.first );
   (*i) = (*i)*opTerm;
  }

 // sort and remove duplicates
 ideal.sort(MV_ideal_comparison);
 ideal.unique();
}
*/

// run through elements of idea, see if they can be killed by the others, if so, do so!
/*
void elementaryReductions( std::list< NMVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NMVPolynomialRing< NLargeInteger > >::iterator i;
 ideal.reverse();
 i = ideal.begin(); 
 while (i!=ideal.end())
  { // check to see if *i can be reduced by testId = ideal \ *i
   std::list< NMVPolynomialRing< NLargeInteger > > testId; 
   std::list< NMVPolynomialRing< NLargeInteger > >::iterator j;
   for (j=ideal.begin(); j!=ideal.end(); j++) if (j!=i)
    testId.push_back( *j );
   NMVPolynomialRing< NLargeInteger > testP( *i );
   if (reduceByIdeal( testId, testP ) ) ideal.erase(i++); 
    else i++;
  }
 ideal.reverse();
}
*/

/**
 *  Given a finitely-generated ideal in Z[t^\pm 1] this turns the ideal
 * into a Groebner basis for the ideal.  This is specifically for 
 * Laurent polynomial rings. 
 *
 * Implements Groebner basis algorithms adapted for Laurent polynomial rings from Pauer and Unterkircher
 * Groebner Basis for Ideals in Laurent Polynomial Rings and their Application to Systems of Difference
 * Equations.  AAECC 9, 271--291 (1999). 
 */
/*
void reduceIdeal( std::list< NMVPolynomialRing< NLargeInteger > > &ideal, bool laurentPoly )
{ 
 // Step 1: normalize list so that first non-zero term is t^0, and positive. Erase 0 entries.
 reduceIdealSortStep(ideal); // TODO: correct if laurentPoly false
 // Step 2: remove redundant elements -- walk through list and see if expressable in terms of others.

 elementaryReductions(ideal); 
 reloop_loop: // will have some goto calls that return here.
 bool didSomething(false); 

 // Step 2: cast the ideal to a vector
 std::vector< NMVPolynomialRing< NLargeInteger > > vecIdeal; 
 std::list< NMVPolynomialRing< NLargeInteger > >::const_iterator i; 
 for (i=ideal.begin(); i!=ideal.end(); i++) vecIdeal.push_back(*i);

 // Step 3: right GCDs
 NPartition subSet( ideal.size(), 2, false );
 while (!subSet.atEnd())
  {
   std::vector< unsigned long > subsetV(subSet.vectorDesc());
   // make vector of right-leading coeffs, compute GCD
   std::vector< NLargeInteger > leadV(subsetV.size());
   signed long maxExp( vecIdeal[0].lastTerm().first );
   for (unsigned long j=0; j<leadV.size(); j++) 
     { leadV[j] = vecIdeal[subsetV[j]].lastTerm().second;
       if (vecIdeal[subsetV[j]].lastTerm().first > maxExp) maxExp = vecIdeal[subsetV[j]].lastTerm().first; }
   std::vector< NLargeInteger > gcdV(subsetV.size()), killV(subsetV.size());
   gcd( leadV, gcdV, killV );
   NMVPolynomialRing< NLargeInteger > combo;
   for (unsigned long j=0; j<gcdV.size(); j++)
    combo += NMVPolynomialRing< NLargeInteger >(gcdV[j],maxExp - vecIdeal[subsetV[j]].lastTerm().first)*vecIdeal[subsetV[j]];
   if (!reduceByIdeal( ideal, combo ) ) { ideal.push_back(combo); vecIdeal.push_back(combo); didSomething = true; }
   ++subSet;
  }

 if ( (!laurentPoly) && didSomething ) goto reloop_loop; 
 if ( (!laurentPoly) && (!didSomething) ) { reduceIdealSortStep(ideal); elementaryReductions(ideal); return; }
 // Step 4: left GCDs
 NPartition subSet2( ideal.size(), 2, false );
 while (!subSet2.atEnd())
  {
   std::vector< unsigned long > subsetV(subSet2.vectorDesc());
   // make vector of right-leading coeffs, compute GCD
   std::vector< NLargeInteger > leadV(subsetV.size());
   signed long minExp( vecIdeal[0].firstTerm().first );
   for (unsigned long j=0; j<leadV.size(); j++) 
     { leadV[j] = vecIdeal[subsetV[j]].firstTerm().second;
       if (vecIdeal[subsetV[j]].firstTerm().first < minExp) minExp = vecIdeal[subsetV[j]].firstTerm().first; }
   std::vector< NLargeInteger > gcdV(subsetV.size()), killV(subsetV.size());
   gcd( leadV, gcdV, killV );
   NMVPolynomialRing< NLargeInteger > combo;
   for (unsigned long j=0; j<gcdV.size(); j++)
    combo += NMVPolynomialRing< NLargeInteger >(gcdV[j],minExp - vecIdeal[subsetV[j]].firstTerm().first)*vecIdeal[subsetV[j]];
   if (!reduceByIdeal( ideal, combo ) ) { ideal.push_back(combo); vecIdeal.push_back(combo); didSomething = true; }
   ++subSet2;
  }
 if (didSomething) goto reloop_loop;

 reduceIdealSortStep(ideal);
 elementaryReductions(ideal);
}
*/

/*
bool isSubIdeal( const std::list< NMVPolynomialRing< NLargeInteger > > &idealA,  
                 const std::list< NMVPolynomialRing< NLargeInteger > > &idealB )
{
 // for every element of idealB, we reduce as much as possible using elements of idealA, see if we get to zero or not...
 std::list< NMVPolynomialRing< NLargeInteger > >::const_iterator j;
 for (j=idealA.begin(); j != idealA.end(); j++) 
   { NMVPolynomialRing< NLargeInteger > temp(*j); if (!reduceByIdeal( idealB, temp ) ) return false; }
 return true;
}
*/

} // namespace regina

