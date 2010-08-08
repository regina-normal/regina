
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

#include "algebra/NSVPolynomialRing.h"
#include "maths/nrational.h"
#include "maths/nmatrixint.h"
#include "maths/matrixops.h"
#include "maths/npartition.h"

#include <list>
#include <vector>

namespace regina {

void dumpIdeal( const std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
std::cout<<"< ";
std::list< NSVPolynomialRing< NLargeInteger > >::const_iterator I;
for (I=ideal.begin(); I!=ideal.end(); I++)
 {
  if (I!=ideal.begin()) std::cout<<", ";
  std::cout<<*I; 
 }
std::cout<<"> "; std::cout.flush();
}


/**
 *  Comparison function for sorting ideals in NSVPolynomialRing< NLargeInteger >
 */
bool ideal_comparison( const NSVPolynomialRing< NLargeInteger > &first, const NSVPolynomialRing< NLargeInteger > &second)
{
 if (first.lastTerm().first<second.lastTerm().first) return true; if (first.lastTerm().first>second.lastTerm().first) return false;
 if (first.PU_degree() < second.PU_degree()) return true; if (first.PU_degree() > second.PU_degree()) return false;
 if (first.lastTerm().second.abs()<second.lastTerm().second.abs()) return true; if (first.lastTerm().second.abs()>second.lastTerm().second.abs()) return true; 

 const std::map< signed long, NLargeInteger* > fTerms(first.allTerms()); const std::map< signed long, NLargeInteger* > sTerms(second.allTerms());
 std::map< signed long, NLargeInteger* >::const_iterator fI(fTerms.begin()); 
 std::map< signed long, NLargeInteger* >::const_iterator sI(sTerms.begin()); 
 while ( (fI != fTerms.end()) || (sI != sTerms.end()) )
  {
   if (fI->first < sI->first) return true;  if (fI->first > sI->first) return false;
   if ( *(fI->second) < *(sI->second) ) return true;  if ( *(fI->second) > *(sI->second) ) return false;
   fI++; sI++;
  }
 // iterate and check how they compare...
 return true;
}


/**
 *  Computes the GCD of elements in input, output is a vector such that the sum
 *  over i, input[i]*outputG[i] == GCD.  We assume output is initialized to the size
 *  of input.  Sum over i, input[i]*outputN[i] == 0 (for a non-trivial outputN)
 *  Assumes input.size()>=1. If input.size()==1, outputN will be zero. 
 */
NLargeInteger gcd(const std::vector< NLargeInteger > &input,
 std::vector< NLargeInteger > &outputG, 
 std::vector< NLargeInteger > &outputN )
{
 if (input.size()==1) { outputG[0]=NLargeInteger::one; return input[0]; }
 NMatrixInt X(1, input.size() );
 for (unsigned long i=0; i<X.columns(); i++) X.entry(0,i) = input[i];
 NMatrixInt R(input.size(), input.size());
 metricalSmithNormalForm( X, &R, NULL, NULL, NULL );
 for (unsigned long i=0; i<R.rows(); i++) outputG[i] = R.entry(i,0);
 // find the "smallest" between column 1 and the last column of R. 
 NLargeInteger colMetricS, colMetricC; unsigned long smallCol = 1;
 for (unsigned long j=1; j<R.columns(); j++)
  {
   colMetricC = NLargeInteger::zero; 
   for (unsigned long i=0; i<R.rows(); i++) colMetricC += R.entry(i,j)*R.entry(i,j); 
   if (j==1) colMetricS = colMetricC; 
   else if (colMetricC < colMetricS) { colMetricS = colMetricC; smallCol = j; } 
  }
 for (unsigned long i=0; i<R.rows(); i++) outputN[i] = R.entry(i, smallCol);
 return X.entry(0,0);
}

template <class T>
void dumpVector( const std::vector<T> &vec )
{
std::cout<<" <";
for (unsigned long j=0; j<vec.size(); j++)
 {  if (j != 0) std::cout<<", ";
    std::cout<<vec[j]; }
std::cout<<"> "; std::cout.flush();
}

bool reduceByIdeal( const std::list< NSVPolynomialRing< NLargeInteger > > &ideal, NSVPolynomialRing< NLargeInteger > &elt, 
                    bool laurentPoly )
{
 if (elt.isZero()) return true; 
 if (ideal.size()==0) return false;

 bool didSomething;
 do 
 { 
   didSomething=false;
   // build a vector consisting of elements of ideal with width <= width of Elt
   std::vector< NSVPolynomialRing< NLargeInteger > > indxId;
   std::list< NSVPolynomialRing< NLargeInteger > >::const_iterator i;
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
      elt -= NSVPolynomialRing< NLargeInteger >( q*gcdV[i], topD-indxId[i].lastTerm().first )*indxId[i];
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
        elt -= NSVPolynomialRing< NLargeInteger >( q*gcdV[i], topD-indxId[i].firstTerm().first )*indxId[i];
      }
    }
 }
 while ( (didSomething) && (!elt.isZero()) );

 return elt.isZero();
}

/**
 * Removes zeros, normalizes so 0th term is first non-zero term and 
 * positive. Sorts, removes duplicates.
 */
void reduceIdealSortStep( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator i;
 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   while (i->isZero()) i=ideal.erase(i);  if (i==ideal.end()) break;
   std::pair< signed long, NLargeInteger > LT( i->firstTerm() );
   NSVPolynomialRing< NLargeInteger > opTerm( (LT.second>0) ? NLargeInteger::one : -NLargeInteger::one, -LT.first );
   (*i) = (*i)*opTerm;
  }

 // sort and remove duplicates
 ideal.sort(ideal_comparison);
 ideal.unique();
}


// run through elements of idea, see if they can be killed by the others, if so, do so!
void elementaryReductions( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator i;
 i = ideal.begin(); 
 while (i!=ideal.end())
  { // check to see if *i can be reduced by testId = ideal \ *i
    // TODO use j to iterate backwards from top to bottom of list
   std::list< NSVPolynomialRing< NLargeInteger > > testId; 
   std::list< NSVPolynomialRing< NLargeInteger > >::iterator j;
   for (j=ideal.begin(); j!=ideal.end(); j++) if (j!=i)
    testId.push_back( *j );
   NSVPolynomialRing< NLargeInteger > testP( *i );
   if (reduceByIdeal( testId, testP ) ) ideal.erase(i++); 
    else i++;
  }
}

/**
 *  Given a finitely-generated ideal in Z[t^\pm 1] this turns the ideal
 * into a Groebner basis for the ideal.  This is specifically for 
 * Laurent polynomial rings. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< NLargeInteger > > &ideal, bool laurentPoly )
{ 
 // Step 1: normalize list so that first non-zero term is t^0, and positive. Erase 0 entries.
 reduceIdealSortStep(ideal); // TODO: correct if laurentPoly false
 // Step 2: remove redundant elements -- walk through list and see if expressable in terms of others.
 elementaryReductions(ideal); 
 reloop_loop: // will have some goto calls that return here.
 bool didSomething(false); 

 // Step 2: cast the ideal to a vector
 std::vector< NSVPolynomialRing< NLargeInteger > > vecIdeal; 
 std::list< NSVPolynomialRing< NLargeInteger > >::const_iterator i; 
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
   NSVPolynomialRing< NLargeInteger > combo;
   for (unsigned long j=0; j<gcdV.size(); j++)
    combo += NSVPolynomialRing< NLargeInteger >(gcdV[j],maxExp - vecIdeal[subsetV[j]].lastTerm().first)*vecIdeal[subsetV[j]];
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
   NSVPolynomialRing< NLargeInteger > combo;
   for (unsigned long j=0; j<gcdV.size(); j++)
    combo += NSVPolynomialRing< NLargeInteger >(gcdV[j],minExp - vecIdeal[subsetV[j]].firstTerm().first)*vecIdeal[subsetV[j]];
   if (!reduceByIdeal( ideal, combo ) ) { ideal.push_back(combo); vecIdeal.push_back(combo); didSomething = true; }
   ++subSet2;
  }
 if (didSomething) goto reloop_loop;
 reduceIdealSortStep(ideal);
 elementaryReductions(ideal);
}


bool isSubIdeal( const std::list< NSVPolynomialRing< NLargeInteger > > &idealA,  
                 const std::list< NSVPolynomialRing< NLargeInteger > > &idealB )
{
 // for every element of idealB, we reduce as much as possible using elements of idealA, see if we get to zero or not...
 std::list< NSVPolynomialRing< NLargeInteger > >::const_iterator j;
 for (j=idealA.begin(); j != idealA.end(); j++) 
   { NSVPolynomialRing< NLargeInteger > temp(*j); if (!reduceByIdeal( idealB, temp ) ) return false; }
 return true;
}


} // namespace regina

