
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

/*! \file algebra/nSimpleField.h
 *  \brief Deals with simple field/ring extensions K[x]/p(x)
 */

#ifndef __NSIMPLEFIELD_H
#ifndef __DOXYGEN
#define __NSIMPLEFIELD_H
#endif

#include <vector>
#include <iostream>
#include "maths/nrational.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents elements in an elementary number field -- a polynomial ring
 * K[x]/p(x) where p(x) is some monic polynomial in the polynomial ring K[x],
 * and K is from NRational. 
 *
 * WARNING: this code is not ready for usage.   
 *
 * @author Ryan Budney
 *
 */

class NSimpleField {
    private:
	/**
	 * NsimpleField class -- represents elements in a simple extension field: K[x]/p(x).
	 * 
	 * Restrictions: we'll use unsigned long integers for the exponents in the
	 *  polynomials, so the polynomial you initialize with can have degree at
	 *  most the largest unsigned long.  The field is templated so you get
	 *  whatever you give, there.  This class is designed to work in Regina
	 *  minimally with the NRational class.  It should also be able to accept
	 *  a previously-instantiated version of itself in the template field. 
	 *
	 * Todo: we will eventually have static members that keep track of 
	 * relevant data for efficient multiplication and inversion of elements
	 * in the number field.
	 *
	 * Initialize class with monic polynomial p(x) = x^n - a_{n-1}x^{n-1} - ... - a_0
	 *  the polynomial will be stored as a vector (a_0, a_1, ..., a_{n-1}) ie, in
	 *  K[x]/p(x) we have the defining relation x^n = a_0 + a_1 x + ... + a_{n-1}x^{n-1}
	 *
	 */ 


	//static NMatrixRing < T > redDat; // reduction data for multiplication
	std::vector<NRational> baseField; // temp representation of the base field
	std::vector<NRational> coeff; // rep of element in the field
	char var; // letter used for the variable when writeTextShort is called

    public:

	/**
	 * Generic constructor -- does not initialize.  In this state the only thing you can
	 *  do with the class is initialize it. (1/5) 
	 */
	NSimpleField();
	/**
	 * If you call the above constructor, you need to follow it with an initialization
	 * call, which is this. 
	 */
	void init(const std::vector<NRational> &bF, const std::vector<NRational> &cf, const char &let); 

        /**
	 * General constructor. (2/5)
         */
        NSimpleField(const std::vector<NRational> &bF, const std::vector<NRational> &cf, const char &let);
	/**
	 * Simplified general constructor
	 * base field specified by bF, polynomial is kt^n (3/5)
	 */
	NSimpleField(const std::vector<NRational> &bF, const NRational &k, const unsigned long &n, const char &let);
        /**
         * Creates a clone of the given field/element. (4/5)
         *
         * @param cloneMe the group to clone.
         */
        NSimpleField(const NSimpleField& cloneMe); 
        /**
         * Creates a clone of the given field, with the element initialized
	 *  to kt^n (5/5)
         *
         * @param cloneMe the group to clone.
         */
        NSimpleField(const NSimpleField& cloneMe, const NRational &k, const unsigned long &n);

        /**
         * Destroys the field/element.
         */
        ~NSimpleField();

        /**
         * Determines whether this and the other field elements are the same.
         */
        bool operator == (const NSimpleField& other) const;
        /**
         * Determines whether this is a constant polynomial other
         */
        bool operator == (const NRational& other) const;

	/**
	 * Assignment of field element.
	 */
	NSimpleField operator = (const NSimpleField& other);
	/**
	 * Assignment of coefficient.
	 */
	NSimpleField operator = (const NRational &other);

	/**
	 * Addition of field element.
	 */
	NSimpleField operator + (const NSimpleField& other) const;
	/**
	 * Negation of field elements.
	 */
	NSimpleField operator - (const NSimpleField &other) const;
	/**
	 * Addition of field element.
	 */
	NSimpleField operator += (const NSimpleField& other);
	/**
	 * Negation of field elements.
	 */
	NSimpleField operator -= (const NSimpleField &other);
	/**
	 * Negation of field elements.
	 */
	NSimpleField operator *= (const NSimpleField &other);

	/**
	 * Multiplication of field elements.
	 */
	NSimpleField operator * (const NSimpleField& other) const;
	/**
	 * Multiplicative inverse
	 */
	NSimpleField inverse() const;

	/**
	 * Vector space ops, right multiplication by field.
	 */
	NSimpleField operator * (const NRational& k) const;
	/**
	 * Vector space ops, left multiplication by field.
	 */
	friend NSimpleField operator*(const NRational& k, const NSimpleField& other);

       /**
	 * Writes the element of the field in human-readable form, base field not mentioned.
         *
         * @param out the stream to write to.
         */
        void writeTextShort(std::ostream& out) const;

};


/*@}*/

// blank constructor (1/5)
inline NSimpleField::NSimpleField()
{}

// constructor (2/5)
inline NSimpleField::NSimpleField(const std::vector<NRational> &bF, const std::vector<NRational> &cf, const char &let) :
 baseField(bF), coeff(cf), var(let) {}

/**
 * Simplified general constructor (3/5)
 */
inline NSimpleField::NSimpleField(const std::vector<NRational> &bF, const NRational &k, const unsigned long &n, const char &let) :
 baseField(bF), var(let) 
{
coeff = bF;
for (unsigned long i=0; i<coeff.size(); i++) coeff[i] = ( (i==n) ? k : k-k );
}

// copy constructor (4/5)
inline NSimpleField::NSimpleField(const NSimpleField& g) :
        baseField(g.baseField), coeff(g.coeff), var(g.var) {}

 /**
  * Creates a clone of the given field, with the element initialized
  *  to kt^n (/5)
  *
  * @param cloneMe the group to clone.
  */
NSimpleField::NSimpleField(const NSimpleField& cloneMe, const NRational &k, const unsigned long &n) :
 baseField(cloneMe.baseField), coeff(cloneMe.coeff), var(cloneMe.var)
{
 for (unsigned long i=0; i<coeff.size(); i++) coeff[i] = ( (i==n) ? k : k-k );
}


// destructor
inline NSimpleField::~NSimpleField() {}

// equality
inline bool NSimpleField::operator == ( const NSimpleField& other) const 
{
    return ((baseField==other.baseField) && (coeff==other.coeff) );
}
/**
  * Determines whether this is a constant polynomial other
  */
inline bool NSimpleField::operator == (const NRational& other) const
{
std::vector<NRational> temp(coeff.size(), NRational::zero);
temp[0]=other;
return ( temp == coeff );
}

// init for if not properly constructed the 1st time...
void NSimpleField::init(const std::vector<NRational> &bF, const std::vector<NRational> &cf, const char &let)
{
baseField = bF;
coeff = cf;
var = let;
}


} // namespace regina

#endif

