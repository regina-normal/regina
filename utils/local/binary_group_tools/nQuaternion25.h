
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

/*! \file utils/local/binary_group_tools/nQuaternion25.h
 *  \brief Deals with the types of quaternions the come up representing spin structures
 */

#ifndef __NQUATERNION25_H
#ifndef __DOXYGEN
#define __NQUATERNION25_H
#endif

#include "shareableobject.h"
#include <vector>
#include "maths/nrational.h"
#include "nfield25.h"

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
class NQuaternion25 : public ShareableObject {
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
        NQuaternion25(const NField25& R, 
	                     const NField25& I, 
			     const NField25& J,
		             const NField25& K);

        /**
         * Creates a clone of the given group.
         *
         * @param cloneMe the group to clone.
         */
        NQuaternion25(const NQuaternion25& cloneMe);

        /**
         * Destroys the group.
         */
        virtual ~NQuaternion25();

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
        bool operator == (const NQuaternion25& other) const;

	/**
	 * Assignment of quaternions.
	 */
	NQuaternion25 operator = (const NQuaternion25& other);
	/**
	 * Addition of quaternions.
	 */
	NQuaternion25 operator + (const NQuaternion25& other) const;
	/**
	 * Multiplication of quaternions.
	 */
	NQuaternion25 operator * (const NQuaternion25& other) const;
	/**
	 * Vector space op
	 */
	NQuaternion25 operator * (const NField25& other) const;
	/**
	 * Negate
	 */
	friend NQuaternion25 operator - (const NQuaternion25 &o);

	/**
	 * Multiplicative inverse
	 */
	NQuaternion25 inverse() const;
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

        friend std::ostream& operator << (std::ostream& out, const NQuaternion25& dat);

};


/*@}*/

// Inline functions for NQuaternion25

inline NQuaternion25::NQuaternion25(const NField25& R, 
				        const NField25& I, 
				        const NField25& J,
					const NField25& K) : ShareableObject(),
	Rc(R), Ic(I), Jc(J), Kc(K) {}

// copy constructor
inline NQuaternion25::NQuaternion25(const NQuaternion25& g) :
        ShareableObject(), Rc(g.Rc), Ic(g.Ic), Jc(g.Jc), Kc(g.Kc) {}

// destructor
inline NQuaternion25::~NQuaternion25() {}

inline bool NQuaternion25::operator == (
        const NQuaternion25& other) const {
    return ((Rc==other.Rc) && (Ic==other.Ic) && (Jc==other.Jc) && (Kc==other.Kc));
}

/**
 * Normsquared
 */
inline NField25 NQuaternion25::normsquared() const
{
	NField25 retval;
	retval = Rc*Rc + Ic*Ic + Jc*Jc + Kc*Kc;
	return retval;
}

inline NField25 NQuaternion25::rcomp() const
{ return Rc; }

inline NField25 NQuaternion25::icomp() const
{ return Ic; }

inline NField25 NQuaternion25::jcomp() const
{ return Jc; }

inline NField25 NQuaternion25::kcomp() const
{ return Kc; }


/**
 * Negate
 */
inline NQuaternion25 operator - (const NQuaternion25 &o)
{
	NQuaternion25 retval(o);
	retval.Rc = (-retval.Rc);
	retval.Ic = (-retval.Ic);
	retval.Jc = (-retval.Jc);
	retval.Kc = (-retval.Kc);
	return retval;
}

} // namespace regina

#endif

