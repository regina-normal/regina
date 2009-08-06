
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

#include "algebra/polynomialringsv.h"

namespace regina {

void NSVPolynomialRing::setCoefficient (const unsigned long & i, const NLargeInteger& c) const
{
std::map< unsigned long, NLargeInteger* >::const_iterator p = cof.find(i);
if ( p == cof.end() ) cof.insert(std::pair< unsigned long, NLargeInteger* >( i, clonePtr(&c) ) );
else p->second = c;
}

void NSVPolynomialRing::setCoefficients (const std::map< unsigned long, NLargeInteger* >& c) const
{
// bulk setCoefficient... is there a smart way to insert in mass?  I think so...
}

NSVPolynomialRing NSVPolynomialRing::operator * (const NSVPolynomialRing& q) const
{}

friend NSVPolynomialRing NSVPolynomialRing::operator * (const NLargeInteger &k, const NSVPolynomialRing& q)
{
NSVPolynomialRing retval;
// todo...
if (k != 0)
 {
 std::map< unsigned long, NLargeInteger* >::iterator ci;
 for (ci = q.cof.begin(); ci != q.cof.end(); ci++)
        {
	cof.insert(std::pair< unsigned long, NLargeInteger* >( ci->first, clonePtr(ci->second) ) );
        } // multiply by k how? 
 }
return retval;
}

NSVPolynomialRing NSVPolynomialRing::operator + (const NSVPolynomialRing& q) const
{}

NSVPolynomialRing NSVPolynomialRing::operator - (const NSVPolynomialRing& q) const
{}

} // namespace regina

