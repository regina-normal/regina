
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

/*! \file nSimpleField.h
 *  \brief Deals with simple field/ring extensions K[x]/p(x)
 */

#ifndef __NSIMPLEFIELD_H
#ifndef __DOXYGEN
#define __NSIMPLEFIELD_H
#endif

#include <vector>
#include <iostream>

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents elements in an elementary number field -- a polynomial ring
 * K[x]/p(x) where p(x) is some monic polynomial in the polynomial ring K[x],
 * and K is an arbitrary templated field. We assume that K
 *
 * WARNING: this code is not remotely ready for usage.  It compiles but does not
 *  instantiate! and it's a great big mess. 
 *
 * @author Ryan Budney
 *
 */

template <class T>
class NSimpleField {
    private:
	/**
	 * NsimpleField class -- represents elements in a simple extension field: K[x]/p(x).
	 * 
	 * Requirements: K must be a field, so it needs basic arithmatic plus
	 *  an element called zero, and all non-zero elements need a multiplicative
	 *  inverse.  K[x]/p(x) is always a ring, but if you want inverses to be
	 *  defined, you need p(x) to be prime in K[x].  K needs to cast 0.
	 *
	 * Restrictions: we'll use unsigned long integers for the exponents in the
	 *  polynomials, so the polynomial you initialize with can have degree at
	 *  most the largest unsigned long.  The field is templated so you get
	 *  whatever you give, there.  This class is designed to work in Regina
	 *  minimally with the NRational class.
	 *
	 * we will have static members that keep track of 
	 * relevant data for efficient multiplication and inversion of elements
	 * in the number field.
	 *
	 * Initialize class with monic polynomial p(x) = x^n - a_{n-1}x^{n-1} - ... - a_0
	 *  the polynomial will be stored as a vector (a_0, a_1, ..., a_{n-1}) ie, in
	 *  K[x]/p(x) we have the rule that x^n = a_0 + a_1 x + ... + a_{n-1}x^{n-1}
	 *
	 * So on initialization there is some static data we need to compute, in order
	 *  to make multiplication efficient.  x^n, x^{n+1}, ..., x^{2n-2}
	 *  to be written in terms of 1, x, x^2, ..., x^{n-1}.
	 */ 


	//static NMatrixRing < T > redDat; // reduction data for multiplication
	std::vector<T> baseField; // temp representation of the base field
	std::vector<T> coeff; // rep of element in the field

    public:

        /**
	 * General constructor.
         */
        NSimpleField(std::vector<T> bF, std::vector<T> cf);

        /**
         * Creates a clone of the given field/element.
         *
         * @param cloneMe the group to clone.
         */
        NSimpleField(const NSimpleField<T>& cloneMe);

        /**
         * Creates a clone of the given field with all 
	 *  elements initialized to zero if zero = true, otherwise initialized
	 *  to one.
         *
         * @param cloneMe the group to clone.
         */
        NSimpleField(const NSimpleField<T>& cloneMe, bool zero);

        /**
         * Destroys the field/element.
         */
        ~NSimpleField();

	/**
	 * The zero element of the given field.
	 */
	NSimpleField<T> zero();

	/**
	 * The identity element of the given field.
	 */
	NSimpleField<T> one();

        /**
         * Determines whether this and the other field elements are the same.
         */
        bool operator == (const NSimpleField<T>& other) const;

	/**
	 * Assignment of field element.
	 */
	NSimpleField<T> operator = (const NSimpleField<T>& other);
	/**
	 * Addition of field element.
	 */
	NSimpleField<T> operator + (const NSimpleField<T>& other) const;
	/**
	 * Multiplication of field elements.
	 */
	NSimpleField<T> operator * (const NSimpleField<T>& other) const;
	/**
	 * Multiplicative inverse
	 */
	NSimpleField<T> inverse() const;
	/**
	 * Vector space ops
	 */
	NSimpleField<T> operator * (const T& k) const;

       /**
	 * Writes the element of the field in human-readable form, base field not mentioned.
         *
         * @param out the stream to write to.
         */
        void writeTextShort(std::ostream& out) const;

	/**
	 * Vector space ops
	 */
	template <class TT>
	friend NSimpleField<TT> operator*(const TT& k, const NSimpleField<TT>& other);

};


/*@}*/

// constructor
template <class T>
inline NSimpleField<T>::NSimpleField(std::vector<T> bF, std::vector<T> cf) :
 baseField(bF), coeff(cf) {}

// copy constructor
template <class T>
inline NSimpleField<T>::NSimpleField(const NSimpleField<T>& g) :
        baseField(g.baseField), coeff(g.coeff) {}

// destructor
template <class T>
inline NSimpleField<T>::~NSimpleField() {}

// equality
template <class T>
inline bool NSimpleField<T>::operator == (
        const NSimpleField<T>& other) const {
    return ((baseField==other.baseField) && (coeff==other.coeff) );
}



} // namespace regina

#endif

