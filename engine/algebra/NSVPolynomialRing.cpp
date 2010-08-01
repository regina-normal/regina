
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
 if (first.lastTerm().first<second.lastTerm().first) return true; if (first.lastTerm().first>second.lastTerm().first) return false;
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
 *  Algorithm assumes all terms of m and n are of non-negative degree. Attempts to write m as q*n + r
 *  for r a polynomial whose terms are all non-negative degree, and keeping r as small as possible. 
 *  Assumes q and r passed as 0. 
 */
void partial_divisionAlg( const NSVPolynomialRing< NLargeInteger > &m, const NSVPolynomialRing < NLargeInteger > &n, 
                          NSVPolynomialRing< NLargeInteger > &q, NSVPolynomialRing< NLargeInteger > &r )
{
 // make sure we're not wasting our time
 if ( m.isZero() || n.isZero() ) return; 
 if ( (m.firstTerm().first!=0) || (n.firstTerm().first!=0) || (m.lastTerm().first < n.lastTerm().first) ) return; 
 r = m; 
 // look at the lead term of r, and the lead term of N, see if one divides the other, r := r - at^k N appropriately, 
 // q += at^k, repeat until lead terms do not divide exactly, or until otherwise run out of terms! So this will be a 
 // do () while () loop. 
 NLargeInteger D; NLargeInteger R; 
 do
  {
   D = r.lastTerm().second.divisionAlg( n.lastTerm().second, R );
   signed long expDiff = r.lastTerm().first - n.lastTerm().first; 
   r -= n*NSVPolynomialRing<NLargeInteger>( D, expDiff );
   q += NSVPolynomialRing<NLargeInteger>( D, expDiff );
  }
 while ( (R == 0) && (r.isZero() ? false : (r.lastTerm().first >= n.lastTerm().first ) ) );
}

/**
 *  Given a finitely-generated ideal in Z[t^\pm 1] this routine reduces
 * the ideal as much as possible.  We subtract multiples of one generator
 * from another in an attempt to make the width of the generators as 
 * short as possible. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
 reloop_loop: 

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

 // Step 3: any elementary reductions that are possible. 
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator j;
 bool didSomething = false; 
 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   j=i; j++; 
   for ( ; j!=ideal.end(); j++)
    {
     NSVPolynomialRing< NLargeInteger > q, r; 
     partial_divisionAlg( *j, *i, q, r );
     if (!q.isZero()) didSomething=true;
     (*j) = r;
    }
   if (didSomething) goto reloop_loop; // keep things clean
  }
}


} // namespace regina

