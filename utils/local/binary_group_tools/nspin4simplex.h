
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

/*! \file utils/localbinary_group_tools/NSpin4Simplex.h
 *  \brief Deals elements of the "binary" A5 -- this is the non-trivial
 *   Z_2-central extension of A5, the alternating group on 5 elements.
 *   Think of A5 as the symmetry group of a pentachoron.  This is the lift
 *   of that group to the spinor group.
 */

#ifndef __NSPIN4SIMPLEX_H
#ifndef __DOXYGEN
#define __NSPIN4SIMPLEX_H
#endif

#include <string>
#include <vector>
#include <iostream>
#include "maths/nrational.h"
#include "maths/nperm5.h"

#include "regina/algebra/nbinaryA5.h"
#include "nQuaternion25.h"
#include "nfield25.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This class is primarily designed to allow for easy manipulation of elements in
 * the binary group associated to the 4-simplex union its dual.  As a group, this is
 * Z_2 x binary group associated to A_5.  We want to be able to think of this group 
 * as both a subgroup of spin_4 = S^3 x S^3, and as Z_2 x a Z_2-central extension of
 * A_5.  To allow for table construction and perhaps search for an efficient cocycle
 * description of \tilde A_5, etc.  Basically, everything relevant for easy construction of
 * spin 3 and 4-manifolds.  This class does essentially all the same things as nbinaryA5
 * except this class is much slower.  If you want speed, use nbinaryA5.  If you need additional
 * flexibility, perhaps this class will be useful to you -- this class was used to create
 * nbinaryA5. 
 *
 * @author Ryan Budney
 */
class NSpin4Simplex {
private:
NQuaternion25 x,y; // components in S^3xS^3 which is where we'll work. 

public:
/** 
 * Constructor 1 left multiplication by X, right multiplication by Y
 */
NSpin4Simplex( const NQuaternion25 &X, const NQuaternion25 &Y );

/**
 * Constructor 2 left multiplication by X, right multiplication by X^{-1}
 */
NSpin4Simplex( const NQuaternion25 &X );

/**
  * Creates a clone of the given group.
  *
  * @param cloneMe the group to clone.
  */
NSpin4Simplex(const NSpin4Simplex& cloneMe);
 /**
  * Destroys the group.
  */
virtual ~NSpin4Simplex();

/**
 * 1st component
 */
NQuaternion25 comp1() const;
/**
 * 2nd component
 */
NQuaternion25 comp2() const;

/**
  * Determines whether this and the given quaternion are equal.
  */
bool operator == (const NSpin4Simplex& other) const;
/**
 * Assignment of quaternions.
 */
NSpin4Simplex operator = (const NSpin4Simplex& other);
/**
 * Addition of quaternions.
 */
NSpin4Simplex operator + (const NSpin4Simplex& other) const;
/**
 * Multiplication of quaternions.
 */
NSpin4Simplex operator * (const NSpin4Simplex& other) const;
/**
 * Vector space op
 */
NSpin4Simplex operator * (const NQuaternion25& other) const;
/**
 * Multiplicative inverse
 */
NSpin4Simplex inverse() const;
/**
 * determines if is equal to zero
 */
bool iszero() const;
/**
 * determines if is equal to an NRational
 */
bool operator == ( const NRational& other ) const;
/**
 * Writes the quaternionic integer to a stream in human-readable form.
 *
 * @param out the stream to write to.
 */
virtual void writeTextShort(std::ostream& out) const;
/**
 * Writes TeX output
 */
void writeTeX(std::ostream &out) const;
/**
 * Normsquared
 */
NField25 normsquared() const;	

/**
 * action
 */
NQuaternion25 act(const NQuaternion25 &on) const;

friend std::ostream& operator << (std::ostream& out, const NSpin4Simplex& dat);
friend NSpin4Simplex operator - (const NSpin4Simplex &in);
};

/** 
 * Constructor
 */
inline NSpin4Simplex::NSpin4Simplex( const NQuaternion25 &X, const NQuaternion25 &Y ) :
	x(X), y(Y) {}

/**
 * Constructor 2 left multiplication by X, right multiplication by X^{-1}
 */
inline NSpin4Simplex::NSpin4Simplex( const NQuaternion25 &X ) :
	x(X), y(X.inverse()) {}

/**
  * Creates a clone of the given group.
  *
  * @param cloneMe the group to clone.
  */
inline NSpin4Simplex::NSpin4Simplex(const NSpin4Simplex& cloneMe) :
  x(cloneMe.x), y(cloneMe.y) {}

 /**
  * Destroys the group.
  */
inline NSpin4Simplex::~NSpin4Simplex() {}

/**
 * 1st component
 */
NQuaternion25 NSpin4Simplex::comp1() const
{ return x; }

/**
 * 2nd component
 */
NQuaternion25 NSpin4Simplex::comp2() const
{ return y; }

/**
  * Determines whether this and the given quaternion are equal.
  */
inline bool NSpin4Simplex::operator == (const NSpin4Simplex& other) const
{ return ( (x==other.x) && (y==other.y) ); }

/**
 * Assignment of quaternions.
 */
inline NSpin4Simplex NSpin4Simplex::operator = (const NSpin4Simplex& other)
{ x=other.x; y=other.y; return (*this); }

/**
 * Addition of quaternions.
 */
inline NSpin4Simplex NSpin4Simplex::operator + (const NSpin4Simplex& other) const
{
	NSpin4Simplex retval( x+other.x, y+other.y );
	return retval;
}

/**
 * Multiplication of quaternions.
 */
inline NSpin4Simplex NSpin4Simplex::operator * (const NSpin4Simplex& other) const
{
	NSpin4Simplex retval( x*other.x, other.y*y );
	return retval;
}

/**
 * Vector space op
 */
inline NSpin4Simplex NSpin4Simplex::operator * (const NQuaternion25& other) const
{
	NSpin4Simplex retval( x*other, y*other );
	return retval;
}

/**
 * Multiplicative inverse
 */
inline NSpin4Simplex NSpin4Simplex::inverse() const
{
	NSpin4Simplex retval( x.inverse(), y.inverse() );
	return retval;
}

/**
 * determines if is equal to zero
 */
inline bool NSpin4Simplex::iszero() const
{
	return ( x.iszero() && y.iszero() );
}

/**
 * Writes the quaternionic integer to a stream in human-readable form.
 *
 * @param out the stream to write to.
 */
inline void NSpin4Simplex::writeTextShort(std::ostream& out) const
{
	out<<"["<<x<<", "<<y<<"]";
}

/**
 * Writes TeX output
 */
inline void NSpin4Simplex::writeTeX(std::ostream &out) const
{
	out<<"\\left["; x.writeTeX(out); 
	out<<", "; y.writeTeX(out); out<<"\\right]";
}

/**
 * Normsquared
 */
inline NField25 NSpin4Simplex::normsquared() const
{
	return (x.normsquared() + y.normsquared());
}

std::ostream& operator << (std::ostream& out, const NSpin4Simplex& dat)
{
	out<<"["<<dat.x<<", "<<dat.y<<"]";
	return out;
}

/**
 * action
 */
inline NQuaternion25 NSpin4Simplex::act(const NQuaternion25 &on) const
{
	return (x*on*y);
}

NSpin4Simplex operator - (const NSpin4Simplex &in)
{
	NSpin4Simplex retval(in);
	retval.x = -retval.x; retval.y = -retval.y;
	return retval;
}


} // namespace regina

#endif


