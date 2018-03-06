
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

#include "algebra/svpolynomialring.h"
#include "maths/rational.h"
#include "maths/matrixint.h"
#include "maths/matrixops.h"
#include "maths/partition.h"

#include <list>
#include <vector>

namespace regina {

/**
 *  Comparison function for sorting ideals in NSVPolynomialRing< Integer >
 */
bool ideal_comparison( const NSVPolynomialRing< Integer > &first, 
                      const NSVPolynomialRing< Integer > &second)
{
 if (first.PU_degree() < second.PU_degree()) return true; 
  if (first.PU_degree() > second.PU_degree()) return false;
 if (first.width() < second.width()) return true; 
  if (first.width() > second.width()) return false; 

 if (first.lastTerm().first<second.lastTerm().first) return true; 
  if (first.lastTerm().first>second.lastTerm().first) return false;
 if (first.lastTerm().second.abs()<second.lastTerm().second.abs()) return true; 
  if (first.lastTerm().second.abs()>second.lastTerm().second.abs()) 
   return false; 

 const std::map< signed long, Integer* > fTerms(first.allTerms()); 
 const std::map< signed long, Integer* > sTerms(second.allTerms());
 std::map< signed long, Integer* >::const_iterator fI(fTerms.begin()); 
 std::map< signed long, Integer* >::const_iterator sI(sTerms.begin()); 
 while ( (fI != fTerms.end()) || (sI != sTerms.end()) )
  {
   if (fI->first < sI->first) return true;  
    if (fI->first > sI->first) return false;
   if ( *(fI->second) < *(sI->second) ) return true;  
    if ( *(fI->second) > *(sI->second) ) return false;
   fI++; sI++;
  }
 // iterate and check how they compare...
 return true;
}

/**
 *  Computes the GCD of elements in input, output is a vector such that the sum
 *  over i, input[i]*outputG[i] == GCD.  We assume output is initialized to the 
 *  size of input.  Sum over i, 
 *    input[i]*outputN[i] == 0 (for a non-trivial outputN)
 *  Assumes input.size()>=1. If input.size()==1, outputN will be zero. 
 */
Integer gcd(const std::vector< Integer > &input,
                  std::vector< Integer > &outputG,  
                  std::vector< Integer > &outputN )
{
 if (input.size()==1) { outputG[0]=Integer::one; return input[0]; }
 NMatrixInt X(1, input.size() );
 for (unsigned long i=0; i<X.columns(); i++) X.entry(0,i) = input[i];
 NMatrixInt R(input.size(), input.size());
 metricalSmithNormalForm( X, &R, NULL, NULL, NULL );
 for (unsigned long i=0; i<R.rows(); i++) outputG[i] = R.entry(i,0);
 // find the "smallest" between column 1 and the last column of R. 
 Integer colMetricS, colMetricC; unsigned long smallCol = 1;
 for (unsigned long j=1; j<R.columns(); j++)
  {
   colMetricC = Integer::zero; 
   for (unsigned long i=0; i<R.rows(); i++) 
    colMetricC += R.entry(i,j)*R.entry(i,j); 
   if (j==1) colMetricS = colMetricC; 
   else if (colMetricC < colMetricS) { colMetricS = colMetricC; smallCol = j; } 
  }
 for (unsigned long i=0; i<R.rows(); i++) outputN[i] = R.entry(i, smallCol);
 return X.entry(0,0);
}

// takes as input an element elt, and attempts to reduce it by the ideal. 
//  returns true only if the element is reduced to zero.
bool reduceByIdeal( const std::list< NSVPolynomialRing< Integer > > 
                    &ideal, NSVPolynomialRing< Integer > &elt, 
                    bool laurentPoly )
{
 if (elt.isZero()) return true;  
 if (ideal.size()==0) return false; // trivial cases

 bool didSomething;
 do 
 { 
   didSomething=false;
   // build a vector consisting of elements of ideal with width <= width of Elt
   std::vector< NSVPolynomialRing< Integer > > indxId;
   std::list< NSVPolynomialRing< Integer > >::const_iterator i;
   for (i=ideal.begin(); i!=ideal.end(); i++) if (i->width() <= elt.width()) 
    { indxId.push_back(*i); }
   if (indxId.size()==0) continue;   // if indxId empty, we're done.. 

   // step 1: build vector of right-leading coeffs from indxId
   std::vector< Integer > leadV( indxId.size() );
   for (unsigned long i=0; i<leadV.size(); i++)
    leadV[i] = indxId[i].lastTerm().second;
   // step 2: check divisibility
   std::vector< Integer > gcdV( indxId.size() ), killV( indxId.size() ); 
   Integer G(gcd(leadV, gcdV, killV)); 

   if ( (elt.lastTerm().second % G) == Integer::zero) 
    {
     didSomething = true;
     Integer q( elt.lastTerm().second.divExact(G) );
     signed long topD( elt.lastTerm().first );
    for (unsigned long i=0; i<gcdV.size(); i++)
      elt -= NSVPolynomialRing< Integer >
        ( q*gcdV[i], topD-indxId[i].lastTerm().first )*indxId[i];
    }

   // if no right-reductions worked ,try left-reductions
   if ((!didSomething) && laurentPoly) 
    {
     // step 1: build vector of right-leading coeffs from indxId
     for (unsigned long i=0; i<leadV.size(); i++)
      leadV[i] = indxId[i].firstTerm().second;
     // step 2: check divisibility
     Integer G(gcd(leadV, gcdV, killV)); 
     if (elt.firstTerm().second % G == Integer::zero) 
      { 
       didSomething = true;
       Integer q( elt.firstTerm().second.divExact(G) );
       signed long topD( elt.firstTerm().first );
       for (unsigned long i=0; i<gcdV.size(); i++)
        elt -= NSVPolynomialRing< Integer >
            ( q*gcdV[i], topD-indxId[i].firstTerm().first )*indxId[i];
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
void reduceIdealSortStep( 
        std::list< NSVPolynomialRing< Integer > > &ideal )
{
 std::list< NSVPolynomialRing< Integer > >::iterator i;
 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   while ( (i!=ideal.end()) ? i->isZero() : 0) i=ideal.erase(i);  
   if (i==ideal.end()) break;
   std::pair< signed long, Integer > LT( i->firstTerm() );
   NSVPolynomialRing< Integer > opTerm( (LT.second>0) ? 
        Integer::one : -Integer::one, -LT.first );
   (*i) = (*i)*opTerm;
  }

 // sort and remove duplicates
 ideal.sort(ideal_comparison);
 ideal.unique();
}


// run through elements of idea, see if they can be killed by the others, 
//  if so, do so! also if it finds more than one degree zero polynomial, 
//  replaces them by their GCD
void elementaryReductions( 
        std::list< NSVPolynomialRing< Integer > > &ideal )
{
 std::list< NSVPolynomialRing< Integer > >::iterator i;

 // amalgamate width zero terms
 std::vector< Integer > deg0s;
 for (i=ideal.begin(); i!=ideal.end(); i++)
  if (i->width()==0) { deg0s.push_back( i->eval(1) ); }
 if ( deg0s.size() > 1 ) {
  std::vector< Integer > outG( deg0s.size() ), outN( deg0s.size() );
  ideal.push_front( NSVPolynomialRing< Integer >
        (gcd( deg0s, outG, outN ), 0 ) ); }

 ideal.reverse(); 
 i = ideal.begin();  
 while (i!=ideal.end())
  { // check to see if *i can be reduced by testId = ideal \ *i
   std::list< NSVPolynomialRing< Integer > > testId; 
   std::list< NSVPolynomialRing< Integer > >::iterator j;
   for (j=ideal.begin(); j!=ideal.end(); j++) if (j!=i)
     testId.push_back( *j );
   NSVPolynomialRing< Integer > testP( *i );
   if ( reduceByIdeal( testId, testP ) ) ideal.erase(i++); 
    else i++;
  }
 ideal.reverse();
}

/**
 * This is the Pauer-Unterkircher algorithm.  Be careful not to pass it very 
 * large ideals or you may be waiting many years for it to complete or for your
 * hard-drive to melt down. reduceIdeal attempts to mitigate the main problems 
 * in the PU algorithm. 
 */ 
void pauer_unterkircher( std::list< NSVPolynomialRing< Integer > > &ideal, 
                         bool laurentPoly )
{
 if (ideal.size() < 2) return;

 reloop_loop:
 bool didSomething=false;
 std::list< NSVPolynomialRing< Integer > >::iterator i; 
 std::list< NSVPolynomialRing< Integer > >::const_iterator ci; 

 // Step 1: cast the ideal to a vector
 std::vector< NSVPolynomialRing< Integer > > vecIdeal; 
 for (i=ideal.begin(); i!=ideal.end(); i++) vecIdeal.push_back(*i);
 
 // Step 2: right GCDs
 NPartition subSet( ideal.size(), 2, false );
 while (!subSet.atEnd())
  {
   std::vector< unsigned long > subsetV(subSet.vectorDesc());
   // make vector of right-leading coeffs, compute GCD
   std::vector< Integer > leadV(subsetV.size());
   signed long maxExp( vecIdeal[0].lastTerm().first );
   for (unsigned long j=0; j<leadV.size(); j++) 
     { leadV[j] = vecIdeal[subsetV[j]].lastTerm().second;
       if (vecIdeal[subsetV[j]].lastTerm().first > maxExp) maxExp = 
          vecIdeal[subsetV[j]].lastTerm().first; }
   std::vector< Integer > gcdV(subsetV.size()), killV(subsetV.size());
   gcd( leadV, gcdV, killV );
   NSVPolynomialRing< Integer > combo;
   for (unsigned long j=0; j<killV.size(); j++)
    combo += NSVPolynomialRing< Integer >(killV[j],maxExp - 
             vecIdeal[subsetV[j]].lastTerm().first)*vecIdeal[subsetV[j]];
   if (!reduceByIdeal( ideal, combo ) ) { ideal.push_back(combo); 
      vecIdeal.push_back(combo); didSomething = true; }
   ++subSet;
  }

 if (didSomething) { reduceIdealSortStep(ideal); elementaryReductions(ideal); }
 if (!laurentPoly) { if (didSomething) goto reloop_loop; else return; } 
 // Step 3: left GCDs

 subSet.reset(ideal.size(), 2, false );
 while (!subSet.atEnd())
  {
   std::vector< unsigned long > subsetV(subSet.vectorDesc());
   // make vector of right-leading coeffs, compute GCD
   std::vector< Integer > leadV(subsetV.size());
   signed long minExp( vecIdeal[0].firstTerm().first );
   for (unsigned long j=0; j<leadV.size(); j++) 
     { leadV[j] = vecIdeal[subsetV[j]].firstTerm().second;
       if (vecIdeal[subsetV[j]].firstTerm().first < minExp) minExp = 
           vecIdeal[subsetV[j]].firstTerm().first; }
   std::vector< Integer > gcdV(subsetV.size()), killV(subsetV.size());
   gcd( leadV, gcdV, killV );
   NSVPolynomialRing< Integer > combo;
   for (unsigned long j=0; j<killV.size(); j++)
    combo += NSVPolynomialRing< Integer >(killV[j],minExp - 
             vecIdeal[subsetV[j]].firstTerm().first)*vecIdeal[subsetV[j]];
   if (!reduceByIdeal( ideal, combo ) ) { ideal.push_back(combo); 
        vecIdeal.push_back(combo); didSomething = true; }
   ++subSet;
  }
 if (didSomething) { reduceIdealSortStep(ideal); elementaryReductions(ideal); 
                      goto reloop_loop; }
}

/**
 *  Given a finitely-generated ideal in Z[t^\pm 1] this turns the ideal
 * into a Groebner basis for the ideal.  This is specifically for 
 * Laurent polynomial rings. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< Integer > > &ideal, 
                  bool laurentPoly )
{ 
 if (ideal.size()<2) return; // lets only consider interesting cases
 static const unsigned long blockSize(5); // blocks of managable sizes
 std::list< std::list< NSVPolynomialRing< Integer > > >::iterator I1,I2;
 std::list< NSVPolynomialRing< Integer > >::iterator i; 
 std::list< NSVPolynomialRing< Integer > >::const_iterator ci; 
 bool subIdealSizeOne = false;

 // Step 1: basic reductions of ideal and check non-trivial
 reduceIdealSortStep(ideal); // TODO: correct if laurentPoly false
 elementaryReductions(ideal); 
 if (ideal.size() < 2) return; // not interesting

 // Step 2: if the ideal size large, break into a list of ideals of size 
 //         blockSize, run pauerunterkircher on these, then use results to 
 //         reduce other ideals amalgamate ideals when possible, and repeat 
 //         until ideal is either of managable size where we run final 
 //         pauer-unterkircher, or increase our blocksize by one and repeat.
  
 unsigned long cbs(blockSize); // current block size...

 std::list< std::list< NSVPolynomialRing< Integer > > > subIdeals; 
 std::list< NSVPolynomialRing< Integer > > subIdeal;               

 while (!ideal.empty()) // group ideal into various subideals of size cbs or 
   // less, store in subIdeals. This helps curb the exponential growth
  { subIdeal.push_back( ideal.front() ); ideal.pop_front();
    if (subIdeal.size() >= cbs) 
        { subIdeals.push_back(subIdeal); subIdeal.clear(); } }
 if (!subIdeal.empty()) subIdeals.push_back(subIdeal); subIdeal.clear(); 
  // ensure the last subideal makes it.

 reloop_loop:
 // we'll apply pauer-unterkircher to *I1, to reduce *I2 for all I1!=I2
 for (I1=subIdeals.begin(); I1!=subIdeals.end(); I1++)
  { 
   pauer_unterkircher( *I1, laurentPoly ); 
   I2=subIdeals.begin(); 
   while (I2 != subIdeals.end())
    {
     if (I2==I1) { I2++; if (I2 == subIdeals.end()) break; }
     // okay, I1 != I2 and neither is at end. Let's reduce I2 by I1
     i = I2->begin();
     while (i != I2->end()) { if (reduceByIdeal( *I1, *i ) ) 
        I2->erase(i++); else i++; }
     // if I2 size zero, erase it.
     if (I2->empty()) subIdeals.erase(I2++); else I2++; 
     if (I2==I1) I2++;
    }
  } // end iteration I1 

  // step 3 - check for the final PU application.
  for (I1=subIdeals.begin(); I1!=subIdeals.end(); I1++) 
   for (i = I1->begin(); i!= I1->end(); i++) ideal.push_back(*i); 
  // re-assemble the big ideal.
  reduceIdealSortStep(ideal); // TODO: correct if laurentPoly false
  elementaryReductions(ideal); 
  // we're done if subIdeals.size() <= 1 and we haven't iterated this on the 
  //  ideal at least once.
  if (ideal.size() <= 1) return;

  if ( (subIdeals.size() > 1) || 
       ( (!subIdealSizeOne) && (subIdeals.size()==1) ) )
   {
    if (subIdeals.size()==1) subIdealSizeOne = true;
    // check to see if any of the subideals have any room for amalgamation, 
    // if so great, amalgamate and reloop.
    // increment partition size if we aren't making progress...
    if (ideal.size()/cbs + ( (ideal.size() % cbs) == 0 ? 0 : 1 ) >= 
        subIdeals.size() )  cbs++; 
    subIdeals.clear(); 
    while (!ideal.empty())
     { subIdeal.push_back( ideal.front() ); ideal.pop_front();
       if (subIdeal.size() >= cbs) 
            { subIdeals.push_back(subIdeal); subIdeal.clear(); } }
     if (!subIdeal.empty()) subIdeals.push_back(subIdeal); 
     subIdeal.clear(); // ensure the last subideal makes it.
    goto reloop_loop; 
   }
 elementaryReductions(ideal); 
 reduceIdealSortStep(ideal); // TODO: correct if laurentPoly false
}


bool isSubIdeal( const std::list< NSVPolynomialRing< Integer > > &idealA,  
                 const std::list< NSVPolynomialRing< Integer > > &idealB )
{
 // for every element of idealB, we reduce as much as possible using 
 //  elements of idealA, see if we get to zero or not...
 std::list< NSVPolynomialRing< Integer > >::const_iterator j;
 for (j=idealA.begin(); j != idealA.end(); j++) 
   { NSVPolynomialRing< Integer > temp(*j); 
     if (!reduceByIdeal( idealB, temp ) ) return false; }
 return true;
}


} // namespace regina

