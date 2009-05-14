
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

/*! \file NField25.h
 *  \brief Deals with the field extension Q[x,y]/(x^2-2, y^2-5)
 */

#ifndef __NFIELD25_H
#ifndef __DOXYGEN
#define __NFIELD25_H
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
 * Represents elements in the elementary number field given by the
 * rationals extended by the square root of 2 and 5. Ie: Q[x,y]/( x^2-2, y^2-5 )
 *
 * WARNING: this code is not ready for usage.   
 *
 * @author Ryan Budney
 *
 */

class NField25 {
    private:
	/**
	 * NField25 class -- represents elements in a simple extension field: K[x]/p(x).
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

	static const NRational c2;
	static const NRational c10;
	static const NRational c5;

	NRational a, b, c, d;

    public:

	/**
	 * Generic constructor -- does not initialize.  In this state the only thing you can
	 *  do with the class is initialize it. (1/3) 
	 */
	NField25();

        /**
         * Creates a clone of the given field/element. (2/3)
         *
         * @param cloneMe the group to clone.
         */
        NField25(const NField25& cloneMe); 
        /**
         * Initialize to a, b, c, d (3/3)
         */
        NField25(NRational &A, NRational &B, NRational &C, NRational &D);

        /**
         * Destroys the field/element.
         */
        ~NField25();


	/**
	 *
	 */
	NRational primary() const;

        /**
         * Determines whether this and the other field elements are the same.
         */
        bool operator == (const NField25& other) const;
        /**
         * Determines whether this is a constant polynomial other
         */
        bool operator == (const NRational& other) const;
        /**
         * Determines whether this and the other field elements are different.
         */
        bool operator != (const NField25& other) const;
        /**
         * Determines whether this is different from a constant polynomial
         */
        bool operator != (const NRational& other) const;
	/**
	 * Assignment of field element.
	 */
	NField25 operator = (const NField25& other);
	/**
	 * Assignment of coefficient.
	 */
	NField25 operator = (const NRational &other);

	/**
	 * Addition of field element.
	 */
	NField25 operator + (const NField25& other) const;
	/**
	 * Subtraction of field elements.
	 */
	NField25 operator - (const NField25 &other) const;
	/**
	 * Addition of field element.
	 */
	NField25 operator += (const NField25& other);
	/**
	 * Negation of field elements.
	 */
	NField25 operator -= (const NField25 &other);
	/**
	 * Negation of field elements.
	 */
	NField25 operator *= (const NField25 &other);

	/**
	 * Multiplication of field elements.
	 */
	NField25 operator * (const NField25& o) const;
	/**
	 * Multiplicative inverse
	 */
	NField25 inverse() const;
	/**
	 * negate this element
	 */
	void negate();
	/**
	 * the other negate
	 */
	friend NField25 operator - (const NField25 &o);

	/**
	 * Vector space ops, right multiplication by field.
	 */
	NField25 operator * (const NRational& k) const;
	/**
	 * Vector space ops, left multiplication by field.
	 */
	friend NField25 operator*(const NRational& k, const NField25& other);

       /**
	 * Writes the element of the field in human-readable form, base field not mentioned.
         *
         * @param out the stream to write to.
         */
        void writeTextShort(std::ostream& out) const;

	/**
	 * returns the number of non-zero terms
	 */
	int nnzt() const; 

	/**
	 * writes TeX output
	 */
	void writeTeX(std::ostream &out) const;

        friend std::ostream& operator << (std::ostream& out, const NField25& dat);

	/**
	 * lets you know if the leading term is negative, for output purposes.
	 */
	bool requires_padding() const;

};


/*@}*/

// blank constructor (1/3)
inline NField25::NField25()
{}

// copy constructor (2/3)
inline NField25::NField25(const NField25& g) :
        a(g.a), b(g.b), c(g.c), d(g.d) {}

/**
  * Constructor (3/3)
  */
inline NField25::NField25(NRational &A, NRational &B, NRational &C, NRational &D) :
	a(A), b(B), c(C), d(D) {}

// destructor
inline NField25::~NField25() {}

inline NRational NField25::primary() const
{ return a; }


// equality
inline bool NField25::operator == ( const NField25& other) const 
{
    return ( (a==other.a) && (b==other.b) && (c==other.c) && (d==other.d) );
}
/**
  * Determines whether this is a constant polynomial other
  */
inline bool NField25::operator == (const NRational& other) const
{
return ( (a == other) && (b == NRational::zero) && (c == NRational::zero) & (d == NRational::zero) );
}
/**
  * Determines whether this and the other field elements are different.
  */
inline bool NField25::operator != (const NField25& other) const
{
return (!((*this)==other));
}

/**
  * Determines whether this is different from a constant polynomial
  */
inline bool NField25::operator != (const NRational& other) const
{
return (!((*this)==other));
}

/**
 * Assignment of field element.
 */
inline NField25 NField25::operator = (const NField25& other)
{ a=other.a; b=other.b; c=other.c; d=other.d; return (*this); }

/**
 * Assignment of coefficient.
 */
inline NField25 NField25::operator = (const NRational &other)
{	a = other;  b=NRational::zero; c = NRational::zero; d=NRational::zero;
 return (*this);
}

/**
 * Addition of field element.
 */
inline NField25 NField25::operator + (const NField25& other) const 
{
	NField25 retval;
	retval.a = a + other.a; retval.b= b + other.b;
	retval.c = c + other.c; retval.d= d + other.d;
	return retval;
}

/**
 * Negation of field elements.
 */
inline NField25 NField25::operator - (const NField25 &other) const
{
	NField25 retval;
	retval.a = a - other.a; retval.b= b - other.b;
	retval.c = c - other.c; retval.d= d - other.d;
	return retval;
}

/**
 * Addition of field element.
 */
inline NField25 NField25::operator += (const NField25& other) 
 { a+=other.a; b+=other.b; c+=other.c; d+=other.d; return (*this); }

/**
 * Negation of field elements.
 */
inline NField25 NField25::operator -= (const NField25 &other)
 { a-=other.a; b-=other.b; c-=other.c; d-=other.d; return (*this); }

/**
 * Multiplication of field elements.
 */
inline NField25 NField25::operator *= (const NField25 &other)
 {
  NField25 temp( (*this)*other );
  (*this)=temp;
  return temp;
 }

/**
 * Multiplication of field elements.
 */
inline NField25 NField25::operator * (const NField25& o) const
 {
   NField25 temp;
   temp.a = a*o.a + c2*b*o.b + c5*c*o.c + c10*d*o.d;
   temp.b = b*o.a + a*o.b + c5*(c*o.d + d*o.c);
   temp.c = c*o.a + a*o.c + c2*(d*o.b + b*o.d);
   temp.d = a*o.d + b*o.c + c*o.b + d*o.a;
   return temp;
 }

/**
 * Negate
 */
inline void NField25::negate()
{
 a.negate(); b.negate(); c.negate(); d.negate();
}

/**
 * Vector space ops, right multiplication by field.
 */
inline NField25 NField25::operator * (const NRational& k) const
{
 NField25 temp( *this );
  temp.a *= k;
  temp.b *= k;
  temp.c *= k;
  temp.d *= k;
 return temp;
}

/**
 * Vector space ops, left multiplication by field.
 */
inline NField25 operator * (const NRational& k, const NField25& other)
{
 NField25 temp( other );
  temp.a *= k;
  temp.b *= k;
  temp.c *= k;
  temp.d *= k;
 return temp;
}

/**
 * returns the number of non-zero terms
 */
inline int NField25::nnzt() const
{
 int retval=0;
 if (a!=NRational::zero) retval++;
 if (b!=NRational::zero) retval++;
 if (c!=NRational::zero) retval++;
 if (d!=NRational::zero) retval++;
 return retval;
} 



} // namespace regina

#endif

