
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

namespace regina {

const NSVPolynomialRing NSVPolynomialRing::zero;
const NSVPolynomialRing NSVPolynomialRing::one(  NLargeInteger::one, 0 );
const NSVPolynomialRing NSVPolynomialRing::pvar( NLargeInteger::one, 1 );

void NSVPolynomialRing::setCoefficient (const unsigned long & i, const NLargeInteger & c) 
{ // redo using insert
NLargeInteger* P(new NLargeInteger(c));
std::pair< std::map< unsigned long, NLargeInteger* >::iterator, bool > res = 
 cof.insert(std::pair< unsigned long, NLargeInteger* >( i, P ) );
if (res.second == false) { (*res.first->second) = (*P); delete P; }
}

NSVPolynomialRing NSVPolynomialRing::operator * (const NSVPolynomialRing& q) const
{
// There's a faster way to do polynomial multiplication using the FFT. See
// http://www.cs.iastate.edu/~cs577/handouts/polymultiply.pdf
// Fateman 2005 indicates nobody has implemented such algorithms in any major package, and that
//  the asymptotic advantage only appears for extremely large polynomials, a problem being that
//  roots of unity are required so they seem to think arbitrary precision complex numbers are
//  required, which is slow.  Can we avoid this using exact arithmatic? 
std::map< unsigned long, NLargeInteger* >::const_iterator I, J;
NSVPolynomialRing retval;

for (I = cof.begin(); I!=cof.end(); I++) for (J=q.cof.begin(); J!=q.cof.end(); J++)
 {
   // lets multiply *I->second and *J->second
   NLargeInteger* P(new NLargeInteger( (*I->second)*(*J->second) ) );
   std::pair< std::map< unsigned long, NLargeInteger* >::iterator, bool > res = 
    retval.cof.insert( std::pair< unsigned long, NLargeInteger* > (
    I->first+J->first, P ) );
   if (res.second == false) { (*res.first->second) += (*P); delete P; }
 }

//  now run through find zero coefficients and deallocate.
std::map< unsigned long, NLargeInteger* >::iterator K;
for (K = retval.cof.begin(); K!=retval.cof.end(); K++)
 if ( (*K->second) == NLargeInteger::zero )
  {
   delete K->second;
   retval.cof.erase(K);
  }

return retval;
}

NSVPolynomialRing operator * (const NLargeInteger &k, const NSVPolynomialRing& q)
{
NSVPolynomialRing retval;

if (k != 0)
 {
 retval = q;
 std::map< unsigned long, NLargeInteger* >::iterator ci;
 for (ci = retval.cof.begin(); ci != retval.cof.end(); ci++)
        {
	(*ci->second) *= k;
        } 
 }

return retval;
}

NSVPolynomialRing NSVPolynomialRing::operator + (const NSVPolynomialRing& q) const
{
NSVPolynomialRing retval;
// two iterators, one for this->cof, one for q.cof, start at beginning for both, 
// smallest we add, if only one then that's okay. We increment smallest, repeat...
std::map< unsigned long, NLargeInteger* >::const_iterator i,j;
i = cof.begin(); j = q.cof.begin();
while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(*j->second) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, new NLargeInteger(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, new NLargeInteger(*i->second) ) );
       i++; }
    else if ( i->first > j->first )
     { retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(*j->second) ) );
       j++; }
    else
     { NLargeInteger* temp(new NLargeInteger( (*i->second) + (*j->second) ) );
       if ( (*temp) != NLargeInteger::zero )
       retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }

return retval;
}

NSVPolynomialRing NSVPolynomialRing::operator - (const NSVPolynomialRing& q) const
{
NSVPolynomialRing retval;
// two iterators, one for this->cof, one for q.cof, start at beginning for both, 
// smallest we add, if only one then that's okay. We increment smallest, repeat...
std::map< unsigned long, NLargeInteger* >::const_iterator i,j;
i = cof.begin(); j = q.cof.begin();
while ( (i != cof.end()) || (j != q.cof.end()) )
 {
  if (i == cof.end())
   { // only j relevant
     retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(-(*j->second) ) ) );
     j++; }
  else if (j == q.cof.end())
   { // only i relevant
     retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, new NLargeInteger(*i->second) ) );
     i++; }
  else
   {if ( i->first < j->first )
     { retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, new NLargeInteger(*i->second) ) );
       i++; }
    else if ( i->first > j->first )
     { retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(-(*j->second) ) ) );
       j++; }
    else
     { NLargeInteger* temp(new NLargeInteger( (*i->second) - (*j->second) ) );
       if ( (*temp) != NLargeInteger::zero )
       retval.cof.insert( std::pair< unsigned long, NLargeInteger* >(i->first, temp ) );
	else delete temp;
       i++; j++; } 
   }
 }
return retval;
}

NSVPolynomialRing& NSVPolynomialRing::operator -=(const NSVPolynomialRing& q)
{ // todo: redo with insert w/iterator
std::map< unsigned long, NLargeInteger* >::iterator i;
std::map< unsigned long, NLargeInteger* >::const_iterator j;
i = cof.begin(); j = q.cof.begin();
while ( (i != cof.end()) || (j != q.cof.end()) )
 { if (i == cof.end())
   { // only j relevant
     cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(-(*j->second) ) ) );
     j++; }
  else if (j == q.cof.end())
   { i++; }
  else
   {if ( i->first < j->first )
     { i++; }
    else if ( i->first > j->first )
     { cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger(-(*j->second) ) ) );
       j++; }
    else
     { (*i->second) -= (*j->second); i++; j++; } 
   }
 }
return (*this);
}

NSVPolynomialRing& NSVPolynomialRing::operator +=(const NSVPolynomialRing& q)
{ // todo: redo with insert w/iterator
std::map< unsigned long, NLargeInteger* >::iterator i;
std::map< unsigned long, NLargeInteger* >::const_iterator j;
i = cof.begin(); j = q.cof.begin();
while ( (i != cof.end()) || (j != q.cof.end()) )
 { if (i == cof.end())
   { // only j relevant
     cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger( (*j->second) ) ) );
     j++; }
  else if (j == q.cof.end())
   { i++; }
  else
   {if ( i->first < j->first )
     { i++; }
    else if ( i->first > j->first )
     { cof.insert( std::pair< unsigned long, NLargeInteger* >(j->first, new NLargeInteger( (*j->second) ) ) );
       j++; }
    else
     { (*i->second) += (*j->second); i++; j++; } 
   }
 }
return (*this);
}

NSVPolynomialRing NSVPolynomialRing::operator -() const
{
NSVPolynomialRing retval(*this);
std::map< unsigned long, NLargeInteger* >::iterator i;
for (i = retval.cof.begin(); i!=retval.cof.end(); i++)
 i->second->negate();
return retval;
}



long int NSVPolynomialRing::descartesNo() const
{
long int retval = 0;
bool signP(false), signN(false); // keeps track of signs L to R for P(t) and P(-t) respectively.
std::map< unsigned long, NLargeInteger* >::const_iterator i;

for (i = cof.begin(); i!=cof.end(); i++)
 {
  bool NsignP = ( (*i->second) > 0 ? true : false );
  bool NsignN = NsignP; if ( i->first % 2 == 1 ) NsignN = !NsignN; 

  if (i != cof.begin()) // compare
   {
    if (signP != NsignP) retval++;
    if (signN != NsignN) retval--;
   }
  signP = NsignP; signN = NsignN;
 }

return retval;
}


} // namespace regina

