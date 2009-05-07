
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

namespace regina {

/**
 * Assignment 
 */
template <class T>
NSimpleField<T> NSimpleField<T>::operator = (const NSimpleField<T>& other)
 {
 baseField = other.baseField; coeff = other.coeff; 
 return (*this);
 }

/**
  * Creates a clone of the given field with all 
  *  elements initialized to zero if zero = true, otherwise initialized
  *  to one.
  *
  * @param cloneMe the group to clone.
  */
template <class T>
NSimpleField<T>::NSimpleField(const NSimpleField<T>& cloneMe, bool zero) :
        baseField(cloneMe.baseField), coeff(cloneMe.coeff)
{
 for (int i=0; i<coeff.size(); i++) coeff[i]=0;
}


/**
 * The zero element of the given field.
 */
template <class T>
NSimpleField<T> NSimpleField<T>::zero()
{
}

/**
 * The identity element of the given field.
 */
template <class T>
NSimpleField<T> NSimpleField<T>::one()
{
}

/**
 * Addition of field elts
 */
template <class T>
NSimpleField<T> NSimpleField<T>::operator + (const NSimpleField<T>& other) const
{
 NSimpleField<T> temp( other );
 for (int i=0; i<coeff.size(); i++) temp.coeff[i] += coeff[i];
 return temp;
}

/**
 * Multiplication of field elts.
 */
template <class T>
NSimpleField<T> NSimpleField<T>::operator * (const NSimpleField<T>& other) const
{
 NSimpleField<T> temp(other, true); // zero element

 NMatrix<T> redMat(baseField.size(),baseField.size()-1); // computed in step 1

   // the column vectors will be representations of x^{n+j} wrt 1, x, ..., x^{n-1}
   // n=baseField.size()

  // step 1: compute reduction matrix

  for (int i=0; i<redMat.rows(); i++) redMat.entry(i,0) = baseField[i];

  for (int i=1; i<redMat.columns(); i++)
   { // shift entry(j,i) <-- (j-1,i-1)...
    for (int j=1; j<redMat.rows(); j++)
	redMat.entry(j,i) = redMat.entry(j-1,i-1); 
    for (int j=0; j<redMat.rows(); j++)
	redMat.entry(j,i) += redMat.entry(baseField.size()-1, i-1)*redMat.entry(j,0);
   }

  // step 2: compute sum_k sum_{i+j=k} a_ib_j x^k
  std::vector<T> polySum(redMat.columns()*2, 0);
  for (int k=0; k<polySum.size(); k++)
	for (int i=0; i<k+1; i++) polySum[k] += coeff[i]*other.coeff[k-i];

  // step 3: reduce 
  for (int k=baseField.size(); k<polySum.size(); k++)
	{
	for (int j=0; j<redMat.columns(); j++) // coeffs of x^{n+j} in the product
	  for (int i=0; i<baseField.size(); i++) // component iterator
		polySum[i] += polySum[baseField.size()+j]*redMat.entry(i,j);
	}

  return temp;
}

/**
 * Multiplicative inverse
 */
template <class T>
NSimpleField<T> NSimpleField<T>::inverse() const
{
 NSimpleField<T> temp(*this);

 // not implemented yet

 return temp;
}

/**
 * Vector space ops
 */
template <class T>
NSimpleField<T> NSimpleField<T>::operator * (const T& k) const
{
// not implemented yet
}

/**
 * Vector space ops
 */
template <class TT>
NSimpleField<TT> operator * (const TT& k, const NSimpleField<TT>& other)
{
// not implemented yet
}


template <class T>
void NSimpleField<T>::writeTextShort(std::ostream& out) const 
{
 for (int i=0; i<coeff.size(); i++)
	{
	out<<coeff[i]<<"x^"<<i;
	if (i<coeff.size()-1) out<<"+"; else out<<" ";
	}
}

template class NSimpleField<int>;

} // namespace regina

