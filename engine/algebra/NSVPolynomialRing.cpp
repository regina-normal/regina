
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
 * TODO: make more useful. We'll want to check to see if first | second, 
 *       or second|first, first.  Then I the order... hmm, dunno....
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
 *  Algorithm assumes all terms of m and n are of non-negative degree. Attempts to write m as q*n + r
 *  keeping the width of r as small as possible. 
 */
void partial_divisionAlg( const NSVPolynomialRing< NLargeInteger > &m, const NSVPolynomialRing < NLargeInteger > &n, 
                          NSVPolynomialRing< NLargeInteger > &q, NSVPolynomialRing< NLargeInteger > &r, 
                          bool fromLeft )
{
 // make sure we're not wasting our time
 if ( m.isZero() || n.isZero() ) return;  
 r = m; q=NSVPolynomialRing<NLargeInteger>::zero; 
 if ( m.width() < n.width() ) return; 
 if ( (!fromLeft) && (m.lastTerm().second.abs() < n.lastTerm().second.abs() ) ) return;
 if ( (fromLeft) && (m.firstTerm().second.abs() < n.firstTerm().second.abs() ) ) return;
 // look at the lead term of r, and the lead term of N, see if one divides the other, r := r - at^k N appropriately, 
 // q += at^k, repeat until lead terms do not divide exactly, or until otherwise run out of terms! So this will be a 
 // do () while () loop. 
 NLargeInteger D; NLargeInteger R; 
 do
  {
   D = ( fromLeft ? r.firstTerm().second.divisionAlg( n.firstTerm().second, R ) : 
                    r.lastTerm().second.divisionAlg( n.lastTerm().second, R ) );
   signed long expDiff = ( fromLeft ? r.firstTerm().first - n.firstTerm().first :
                                      r.lastTerm().first - n.lastTerm().first ); 
   r -= n*NSVPolynomialRing<NLargeInteger>( D, expDiff );
   q += NSVPolynomialRing<NLargeInteger>( D, expDiff );
  }
 while ( (R == 0) && (r.isZero() ? false : (r.width() >= n.width() ) ) );
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

bool reduceIdealLeftReductions( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator j;
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator i;
 bool didSomething = false;

 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   j=i; j++; 
   for ( ; j!=ideal.end(); j++)
    {
     NSVPolynomialRing< NLargeInteger > q, r; 
     partial_divisionAlg( *j, *i, q, r, true );
     if (!q.isZero()) didSomething=true;
     (*j) = r;
    }
   if (didSomething) return true; // keep things clean
  }

 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   j=i; j++; 
   for ( ; j!=ideal.end(); j++)
    {
     NLargeInteger g, u, v;
     g = i->firstTerm().second.gcdWithCoeffs( j->firstTerm().second, u, v );
     if ( (g >= i->firstTerm().second.abs()) || (g >= j->firstTerm().second.abs()) ) continue;
     // okay, we have a good linear combination. 
     ideal.push_back( NSVPolynomialRing<NLargeInteger>(u,0)*(*i) + 
                      NSVPolynomialRing<NLargeInteger>(v,0)*(*j) ); 
     return true; // keep things clean
    }
  }

 return didSomething;
}

bool reduceIdealRightReductions( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator j;
 std::list< NSVPolynomialRing< NLargeInteger > >::iterator i;
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
   if (didSomething) return true; // keep things clean
  }

 for (i=ideal.begin(); i!=ideal.end(); i++)
  {
   j=i; j++; 
   for ( ; j!=ideal.end(); j++)
    {
     NLargeInteger g, u, v;
     g = i->lastTerm().second.gcdWithCoeffs( j->lastTerm().second, u, v );
     if ( (g >= i->lastTerm().second.abs()) || (g >= j->lastTerm().second.abs()) ) continue;
     // okay, we have a good linear combination. 
     signed long expDiff ( (*j).lastTerm().first - (*i).lastTerm().first ); 
     ideal.push_back( NSVPolynomialRing<NLargeInteger>( u, expDiff )*(*i) + 
                      NSVPolynomialRing<NLargeInteger>( v, 0)*(*j) ); 
     return true; // keep things clean
    }
  }

 return didSomething;
}


/**
 *  Given a finitely-generated ideal in Z[t^\pm 1] this turns the ideal
 * into a Groebner basis for the ideal.  This is specifically for 
 * Laurent polynomial rings. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{ 
 bool left_done=false; bool right_done=false;  // keep track of what we consider "done"
 bool working_right=true;
  // ie: we work on the right hand side until it's done, then the left hand side until its
  // done, if the left hand side resulted in a change, work on the right hand side again, etc.
 reloop_loop: 

 //  Step 1: normalize list so that first non-zero term is t^0, and positive. Erase 0 entries.
 reduceIdealSortStep(ideal);
 if ((!right_done) && (working_right))
  {
   // Step 2a: elementary reductions from right -- div alg and gcd term production
   if (reduceIdealRightReductions(ideal)) { left_done=false; goto reloop_loop; } 
   right_done=true; if (!left_done) { working_right=false; goto reloop_loop; }
  }
 else if ((!left_done) && (!working_right))
  {
   // Step 2b: elementary reductions from left -- div alg and gcd term production
   if (reduceIdealLeftReductions(ideal)) { right_done=false; goto reloop_loop; }
   left_done=true; if (!right_done) { working_right=true; goto reloop_loop; }
  }
 // both sides maximally reduced. 
}

/**
 *  Determines whether or not idealA is a subideal of idealB. Assumes you've run idealA through reduceIdeal() first, 
 * i.e. idealA() is a Groebner basis. 
 */
bool isSubIdeal( const std::list< NSVPolynomialRing< NLargeInteger > > &idealA,  const std::list< NSVPolynomialRing< NLargeInteger > > &idealB )
{
 // find the GCDs of the leading terms of idealA and the linear combination that realizes the GCD. 
 // store this as a ?map? of <unsigned long, comb > where unsigned long is the max degree of elts from idealA, and
 // comb is the appropriate length vector<NLargeInteger> indicating the linear combination.

 // for every element of idealB, we reduce as much as possible using elements of idealA, see if we get to zero or not...
 std::list< NSVPolynomialRing< NLargeInteger > >::const_iterator j;
 for (j=idealB.begin(); j != idealB.end(); j++)
  {
   // TODO hmm... need to think over more examples first.
  }

 return false;
}


} // namespace regina

