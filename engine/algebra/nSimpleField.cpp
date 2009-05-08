
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

#include "algebra/nSimpleField.h"
#include <iostream>
#include "maths/nmatrix.h"
#include "utilities/nrational.h"

namespace regina {

/**
 * Assignment 
 */
NSimpleField NSimpleField::operator = (const NSimpleField& other)
 {
 baseField = other.baseField; coeff = other.coeff; 
 return (*this);
 }

/**
 * Assignment of coefficient.
 *  this turns other into a polynomial of degree 0.
 */
NSimpleField NSimpleField::operator = (const NRational &other)
 {
 return NSimpleField( *this, other, 0 ); 
 }



/**
 * Addition of field elts
 */
NSimpleField NSimpleField::operator + (const NSimpleField& other) const
{
 NSimpleField temp( other );
 for (unsigned long i=0; i<coeff.size(); i++) temp.coeff[i] += coeff[i];
 return temp;
}
/**
 * Negation of field elements.
 */
NSimpleField NSimpleField::operator - (const NSimpleField &other) const
{
 NSimpleField temp( *this );
 for (unsigned long i=0; i<coeff.size(); i++) temp.coeff[i] -= other.coeff[i];
 return temp;
}

/**
 * Addition of field element.
 */
NSimpleField NSimpleField::operator += (const NSimpleField& other)
{
for (unsigned long i=0; i<coeff.size(); i++) coeff[i] += other.coeff[i];
return (*this);
}
/**
 * Negation of field elements.
 */
NSimpleField NSimpleField::operator -= (const NSimpleField &other)
{
for (unsigned long i=0; i<coeff.size(); i++) coeff[i] -= other.coeff[i];
return (*this);
}

/**
 * Negation of field elements.
 */
NSimpleField NSimpleField::operator *= (const NSimpleField &other)
{
(*this) = (*this) * other;
return (*this);
}



/**
 * Multiplication of field elts.
 */
NSimpleField NSimpleField::operator * (const NSimpleField& other) const
{
 NSimpleField temp(other); 

 NMatrix<NRational> redMat(baseField.size(),baseField.size()-1); // computed in step 1
 redMat.initialise(NRational::zero);

   // the column vectors will be representations of x^{n+j} wrt 1, x, ..., x^{n-1}
   // n=baseField.size()

  // step 1: compute reduction matrix

  for (unsigned long i=0; i<redMat.rows(); i++) redMat.entry(i,0) = baseField[i];

  for (unsigned long i=1; i<redMat.columns(); i++)
   { // shift entry(j,i) <-- (j-1,i-1)...
    for (unsigned long j=1; j<redMat.rows(); j++)
	redMat.entry(j,i) = redMat.entry(j-1,i-1); 
    for (unsigned long j=0; j<redMat.rows(); j++)
	redMat.entry(j,i) += redMat.entry(baseField.size()-1, i-1)*redMat.entry(j,0);
   }

  // step 2: compute sum_k sum_{i+j=k} a_ib_j x^k
  std::vector<NRational> polySum(redMat.columns()*2+1,NRational::zero);

  for (unsigned long k=0; k<polySum.size(); k++)
	for (unsigned long i=((k<coeff.size())?(0):(k-coeff.size()+1)); (i<k) && (i<coeff.size()); i++) polySum[k] += coeff[i]*other.coeff[k-i];

  // step 3: reduce 
  for (unsigned long j=0; j<redMat.columns(); j++) // coeffs of x^{n+j} in the product
    for (unsigned long i=0; i<baseField.size(); i++) // component iterator
		polySum[i] += polySum[baseField.size()+j]*redMat.entry(i,j);

  for (unsigned long i=0; i<temp.coeff.size(); i++) temp.coeff[i] = polySum[i];

  return temp;
}

/**
 * Multiplicative inverse
 */
NSimpleField NSimpleField::inverse() const
{
 NSimpleField temp(*this);

 // not implemented yet

 return temp;
}

/**
 * Vector space ops
 */
NSimpleField NSimpleField::operator * (const NRational& k) const
{
NSimpleField temp(*this);
for (unsigned long i=0; i<coeff.size(); i++) temp.coeff[i]*=k;
return temp;
}

/**
 * Vector space ops
 */
NSimpleField operator * (const NRational& k, const NSimpleField& other)
{
NSimpleField temp( other );
for (unsigned long i=0; i<temp.coeff.size(); i++) temp.coeff[i]*=k;
return temp;
}


void NSimpleField::writeTextShort(std::ostream& out) const 
{
// output polynomials in a sensible fashion, eg:  1+x^2,  -1,  1-x+20x^6, etc...
//  +1, -1 special rules, and ^1 special rule, and seperator special rule...
bool ws=false;

 for (unsigned long i=0; i<coeff.size(); i++)
	if (coeff[i]!=0)
	{
	if (coeff[i]==1)
		{
		if (ws) 
		 {
		  if (i==1) out<<"+"<<var; else out<<"+"<<var<<"^"<<i;
		 } 
		else if (i==0) out<<"1";
	        else if (i==1) out<<var;
		else out<<var<<"^"<<i;
		ws=true;		
		} 
	else if (coeff[i]==-1)
		{
	 	if (ws) 
		 {
		  if (i==1) out<<"-"<<var; else out<<"-"<<var<<"^"<<i;
		 }
		else if (i==0) out<<"-1";
		else if (i==1) out<<var;
		else out<<"-"<<var<<"^"<<i;
		ws=true;
		} 
	else
		{
		if (coeff[i]>0) 
			{
			if (ws) out<<"+";
		        out<<coeff[i];
			if (i==1) out<<var;
			else if (i!=0) out<<var<<"^"<<i;
			}
		else    {
			out<<coeff[i];
			if (i==1) out<<var;
			else if (i!=0) out<<var<<"^"<<i;
			}
		ws=true;
		}
	}	
}

} // namespace regina

