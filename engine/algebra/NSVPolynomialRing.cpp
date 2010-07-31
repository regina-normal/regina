
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
#include <list>

namespace regina {

/**
 *  Comparison function for sorting ideals in NSVPolynomialRing< NLargeInteger >
 */
bool ideal_comparison( const NSVPolynomialRing< NLargeInteger > &first, const NSVPolynomialRing< NLargeInteger > &second)
{
if (first.PU_degree() < second.PU_degree()) return true; if (first.PU_degree() > second.PU_degree()) return false;

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
 *  Given a finitely-generated ideal in Z[t^\pm 1] this routine reduces
 * the ideal as much as possible.  We subtract multiples of one generator
 * from another in an attempt to make the width of the generators as 
 * short as possible. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
//  Step 1: normalize list so that first non-zero term is t^0, and positive. Erase 0 entries.
std::list< NSVPolynomialRing< NLargeInteger > >::iterator i;
for (i=ideal.begin(); i!=ideal.end(); i++)
 {
  while (i->isZero()) i=ideal.erase(i);  if (i==ideal.end()) break;
  std::pair< signed long, NLargeInteger > LT( i->firstTerm() );
  NSVPolynomialRing< NLargeInteger > opTerm( (LT.second>0) ? NLargeInteger::one : -NLargeInteger::one, -LT.first );
  (*i) = (*i)*opTerm;
 }

//  Step 2: sort and remove duplicates
ideal.sort(ideal_comparison);
ideal.unique();

// TODO at present everything below is hypothetical
//  Step 3: sort list by increasing width, and among common width by size 
//          of leading term. (which one?) maybe which one could reduce the
//          other the most.
//  Step 4: use shortest width terms to diminish leading-terms of longer
//          width terms. 
//  If nothing eventful in step 4, end cycle. 

}

/**
 *  Given two polynomials with rational coefficients p and q, find d and r such that
 *  p = dq + r with 0 <= degree(r) < degree(q)
 */
void euclideanAlgorithm( const NSVPolynomialRing< NRational > &p, const NSVPolynomialRing< NRational > &q, 
                         NSVPolynomialRing< NRational > &d, NSVPolynomialRing< NRational > &r )
{
// TODO
}


} // namespace regina

