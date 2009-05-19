
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

/*! \file algebra/nquaternionicinteger.h
 *  \brief Deals with the types of quaternions the come up representing spin structures
 */

#ifndef __NQUATERNIONICINTEGER_H
#ifndef __DOXYGEN
#define __NQUATERNIONICINTEGER_H
#endif

#include "shareableobject.h"
#include <vector>
#include "maths/nrational.h"
#include "algebra/nfield25.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * Represents elements in the group of projective quaternionic integers, at least
 * that's what I'm calling it.  This is the subgroup of the group of unit quaternions,
 * thought of as being the unit sphere in R^4, such that every element has a non-zero
 * multiple that is in the quaternionic integer lattice ZxZxZxZ.  Can also be used
 * to represent plain old quaternionic integers, if you're into that. 
 *
 * At present this class is only used to represent spin structures on triangulated
 * 3 and 4-manifolds. 
 *
 * @author Ryan Budney
 *
 */
class NQuaternionicInteger : public ShareableObject {
    private:
        /** Keep track of the components. */
	NField25 Rc;
	NField25 Ic;
	NField25 Jc;
	NField25 Kc;

    public:

        /**
	 Initialize with real part R, i-component I, j-component J, k-component K.
         */
        NQuaternionicInteger(const NField25& R, 
	                     const NField25& I, 
			     const NField25& J,
		             const NField25& K);

        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        NQuaternionicInteger(const NQuaternionicInteger& cloneMe);

        /**
         * Destroys the group.
         */
        virtual ~NQuaternionicInteger();

	/**
	 * r,i,j,k components
	 */
	NField25 rcomp() const;
	NField25 icomp() const;
	NField25 jcomp() const;
	NField25 kcomp() const;

        /**
         * Determines whether this and the given quaternion are equal.
         */
        bool operator == (const NQuaternionicInteger& other) const;

	/**
	 * Assignment of quaternions.
	 */
	NQuaternionicInteger operator = (const NQuaternionicInteger& other);
	/**
	 * Addition of quaternions.
	 */
	NQuaternionicInteger operator + (const NQuaternionicInteger& other) const;
	/**
	 * Multiplication of quaternions.
	 */
	NQuaternionicInteger operator * (const NQuaternionicInteger& other) const;
	/**
	 * Vector space op
	 */
	NQuaternionicInteger operator * (const NField25& other) const;
	/**
	 * Negate
	 */
	friend NQuaternionicInteger operator - (const NQuaternionicInteger &o);

	/**
	 * Multiplicative inverse
	 */
	NQuaternionicInteger inverse() const;
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
	void writeTeX(std::ostream& out) const;

	/**
	 * Normsquared
	 */
	NField25 normsquared() const;	

        friend std::ostream& operator << (std::ostream& out, const NQuaternionicInteger& dat);

};


/*@}*/

// Inline functions for NQuaternionicInteger

inline NQuaternionicInteger::NQuaternionicInteger(const NField25& R, 
				        const NField25& I, 
				        const NField25& J,
					const NField25& K) : ShareableObject(),
	Rc(R), Ic(I), Jc(J), Kc(K) {}

// copy constructor
inline NQuaternionicInteger::NQuaternionicInteger(const NQuaternionicInteger& g) :
        ShareableObject(), Rc(g.Rc), Ic(g.Ic), Jc(g.Jc), Kc(g.Kc) {}

// destructor
inline NQuaternionicInteger::~NQuaternionicInteger() {}

inline bool NQuaternionicInteger::operator == (
        const NQuaternionicInteger& other) const {
    return ((Rc==other.Rc) && (Ic==other.Ic) && (Jc==other.Jc) && (Kc==other.Kc));
}

/**
 * Normsquared
 */
inline NField25 NQuaternionicInteger::normsquared() const
{
	NField25 retval;
	retval = Rc*Rc + Ic*Ic + Jc*Jc + Kc*Kc;
	return retval;
}

inline NField25 NQuaternionicInteger::rcomp() const
{ return Rc; }

inline NField25 NQuaternionicInteger::icomp() const
{ return Ic; }

inline NField25 NQuaternionicInteger::jcomp() const
{ return Jc; }

inline NField25 NQuaternionicInteger::kcomp() const
{ return Kc; }


/**
 * Negate
 */
NQuaternionicInteger operator - (const NQuaternionicInteger &o)
{
	NQuaternionicInteger retval(o);
	retval.Rc = (-retval.Rc);
	retval.Ic = (-retval.Ic);
	retval.Jc = (-retval.Jc);
	retval.Kc = (-retval.Kc);
	return retval;
}

} // namespace regina

#endif

