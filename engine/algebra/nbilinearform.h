
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

/*! \file algebra/nbilinearform.h
 *  \brief Deals with bilinear forms -- homomorphisms out of a tensor product
 *   of abelian groups, to an abelian group. 
 */

#ifndef __NBILINEARFORM_H
#ifndef __DOXYGEN
#define __NBILINEARFORM_H
#endif

//#include <string>
#include "maths/nsparsegrid.h"
#include "shareableobject.h"
#include "maths/nlargeinteger.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/NSVPolynomialRing.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This class holds general bilinear forms -- homomorphisms from a tensor product of abelian
 * groups, to an abelian group. It allows for the computation of things such as the signature
 * and Kawauchi-Kojima invariants of the bilinear forms coming from Poincare duality on a 4 or
 * 3-manifold respectively. 
 *
 * TODO : 1) other constructors allowing for conjugations w/maps between groups -- needing only the
 *        reduced pairing of a previously defined NBilinearForm. Thus we could define the intersection
 *        product first in Dual x Standard rel Bdry coords, then in other coords via conjugations. 
 *        
 *        2) Specify different methods of computation -- have an enum that allows for honest computations
 *        where for example the domain of the leftAdjoint would satisfy isEqualTo(A), etc, vs one where
 *        only isIsomorphicTo(A) would be satisfied -- one using unreducedPairing, the other reducedPairing, etc.
 *
 * @author Ryan Budney
 */
class NBilinearForm : public ShareableObject {
    private:
	/**
	 * Stores the pairing in SNF coordinates for all 3 groups. 
	 */
	NSparseGrid< NLargeInteger > *reducedPairing; 
	/**
	 * The defining pairing.
	 */
        NSparseGrid< NLargeInteger > *unreducedPairing;

	/**
	 * record of ldomain, rdomain and range
	 */
	NMarkedAbelianGroup lDomain, rDomain, Range;

    public:
        /**
         * Creates the bilinear form as a map ldomain \otimes rdomain --> range, where the map
         * is described by pairing, a nxmxk-sparse grid where n is the rank of
         * the chain complex for ldomain, m the rank of the chain complex for rdomain, and k the
         * rank of the chain complex for range. 
         */
        NBilinearForm(const NMarkedAbelianGroup &ldomain, const NMarkedAbelianGroup &rdomain, 
		      const NMarkedAbelianGroup &range,   const NSparseGrid< NLargeInteger > &pairing);

        /**
         * Creates a permutation that is a clone of the given
         * bilinear form.
         *
         * @param cloneMe the form to clone.
         */
        NBilinearForm(const NBilinearForm& cloneMe);

	/**
	 * Destructor
	 */
	~NBilinearForm();

        /**
	 * Assignment of bilinear forms.
         */
        NBilinearForm& operator = (const NBilinearForm& cloneMe);

	/**
	 * Access to the unreducedPairing map.
	 */
        const std::map< NMultiIndex, NLargeInteger* > & unreducedMap() const;

	/**
	 * Access to the reducedPairing map.
	 */
        const std::map< NMultiIndex, NLargeInteger* > & reducedMap() const;

	/**
	 * If this is a symmetric bilinear form, we can ask for its signature.  This
	 * routine computes said signature. 
         */
	unsigned long signature() const;

	/**
         * Kawauchi-Kojima sigma vector describing the odd prime torsion, provided the
         * form is symmetric. 
         */
	std::vector< unsigned long > oddKKvec() const;

	/**
         * Kawauchi-Kojima vector describing the 2-torsion, provided the
         * form is symmetric. 
         */
	std::vector< unsigned long > twoKKvec() const;

	/**
	 * Kawauchi-Kojima test for hyperbolicity, assuming form symmetric.
	 */
	bool isHyperbolic() const;

	/**
	 * Given a map A \otimes B --> C, there are two adjoints, 
	 *  this returns A --> Hom(B,C), the "left" adjoint. At present
         *  we use a cheap construction of this so the domain of this map
         *  will not satisfy isEqualTo(A)
	 */
	NHomMarkedAbelianGroup leftAdjoint() const;

	/**
	 * Given a map A \otimes B --> C, there are two adjoints, 
         *  this returns B --> Hom(A,C), the "right adjoint.   
	 */
	NHomMarkedAbelianGroup rightAdjoint() const;

        /**
         * todo! maybe talk about nullity, symmetry, l/rdomain, range, etc.
         * various invariants if they make sense, etc.
         */
        virtual void writeTextShort(std::ostream& out) const;

};

/*@}*/

} // namespace regina

#endif

