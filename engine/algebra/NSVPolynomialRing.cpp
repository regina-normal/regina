
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
 *  Given a finitely-generated ideal in Z[t^\pm 1] this routine reduces
 * the ideal as much as possible.  We subtract multiples of one generator
 * from another in an attempt to make the width of the generators as 
 * short as possible. 
 */
void reduceIdeal( std::list< NSVPolynomialRing< NLargeInteger > > &ideal )
{
// TODO
// Simple reduction move:
//   Or maybe the priority should be to not increase any word's length
//    beyond the current max word length, otherwise try to decrease 
//    magnitude of leading terms.

//  Step 1: remove zeros. 
//  Step 2: normalize list so that first non-zero term is t^0, and positive.
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

