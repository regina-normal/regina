
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#ifndef __BILINEARFORM_H
#ifndef __DOXYGEN
#define __BILINEARFORM_H
#endif

#include "regina-core.h"
#include "maths/sparsegrid.h"
#include "maths/matrixops.h"
#include "algebra/markedabeliangroup.h"
#include "algebra/svpolynomialring.h"
#include "maths/rational.h" 
#include "maths/integer.h"
#include "maths/primes.h"
#include <algorithm>
#include <map>
#include <list>
#include <cmath>

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This class holds general bilinear forms -- homomorphisms from a tensor
 * product of abelian groups, to an abelian group. It allows for the 
 * computation of things such as the signature and Kawauchi-Kojima invariants 
 * of the bilinear forms coming from Poincare duality on a 4 or
 * 3-manifold respectively. 
 *
 * @author Ryan Budney
 */
class REGINA_API NBilinearForm {
    private:
	/**
	 * Stores the pairing in SNF coordinates for all 3 groups. 
	 */
	NSparseGridRing< Integer > *reducedPairing; 
	/**
	 * The defining pairing.
	 */
    NSparseGridRing< Integer > *unreducedPairing;

	/**
	 * record of ldomain, rdomain and range
	 */
	MarkedAbelianGroup lDomain, rDomain, Range;

    /**
     *  This is internal storage for the precomputed Kawauchi-Kojima tests 
     *  for torsion linking forms.  
     */

    bool KKinvariantsComputed;
    bool torsionLinkingFormIsSplit;
    bool torsionLinkingFormIsHyperbolic;
    bool torsionLinkingFormKKTTC;
    std::string torsionRankString;    
    std::string torsionSigmaString;  
    std::string torsionLegendreString;

    public:
        /**
         * Creates the bilinear form as a map ldomain \otimes rdomain --> range,
         * where the map is described by pairing, a nxmxk-sparse grid where n 
         * is the rank of the chain complex for ldomain, m the rank of the 
         * chain complex for rdomain, and k the rank of the chain complex 
         * for range. 
         */
        NBilinearForm(const MarkedAbelianGroup &ldomain, 
              const MarkedAbelianGroup &rdomain, 
		      const MarkedAbelianGroup &range,   
              const NSparseGridRing< Integer > &pairing);

        /**
         * Creates a clone of the given bilinear form.
         *
         * @param cloneMe the form to clone.
         */
        NBilinearForm(const NBilinearForm& cloneMe);

    	/**
	     * Destructor
	     */
        ~NBilinearForm();

        /**
         * Evaluates the bilinear form on an element of ldomain and rdomain 
         * respectively, all in the chain complex coordinates.
         *
         * @param lcc chain complex representative in ldomain
         * @param rcc chain complex representative in rdomain
         * @returns chain complex representative in range. 
         */
        std::vector<Integer> evalCC(std::vector<Integer> &lcc, 
                                      std::vector<Integer> &rcc) const;

    	/**
	     * Access to the unreducedPairing map.
    	 */
        const std::map< NMultiIndex< unsigned long >, Integer* > & 
            unreducedMap() const;

        /**
         * Access to the reducedPairing map.  So if you wanted to iterate 
         * through the values of the bilinear form evaluated on the generators 
         * of the homology of ldomain and rdomain, you'd construct an iterator 
         * to a std::map< NMultiIndex< unsigned long >, Integer* > type 
         * and iterate through reducedMap(). 
         */
        const std::map< NMultiIndex< unsigned long >, Integer* > & 
            reducedMap() const;

        /**
         * reducedPairing access as an NSparseGridRing
        */
        const NSparseGridRing< Integer >* reducedSparseGrid() const;
 
        /**
         *  Given a bilinear form whose range is the integers, we can ask 
         * for the maximal dimension (rank) of a subspace on which it is 
         * non-degenerate. This is that number.  Returns zero if your target 
         * space is not the integers.
         */
        unsigned long rank() const;

        /**
         * If this is a symmetric bilinear form with range the integers, we
         * can ask for its  signature.  This routine computes said signature. 
         * See Freedman-Quinn section 10.2 for how rank, signature and type 
         * classify Z forms provided the form is indefinite. For definite forms
         * see the Milnor-Husemoller book for details on enumeration of types.
         * Generally zFormSignature and zFormType are not enough in this case.
         */
        long int zFormSignature() const;

        /**
         * Computes the form type.  This applies to symmetric Z-valued forms.
         * There are two primary invariants, whether the form is even or odd,
         * and the definiteness, i.e. the sign-values achieved by mu(x,x).
         * The bool return value will be false for even, and true for odd. 
         * The int return value will be 1 for positive definite, 
         * -1 for negative-definite, and 0 for indefinite.  Positive definite
         * means mu(x,x) >= 0 for all x in the domain. 
         */
        std::pair< bool, int > zFormType() const;

    	/**
          * Kawauchi-Kojima vector describing the torsion ranks, as prime
          *  powers.
          *
          * Returns the torsion form rank vector. This is one of the three 
          * Kawauchi-Kojima invariants of a torsion linking form.  It is a 
          * notation for the prime power decomposition of the
          * torsion subgroup of H1.  Z_8 would be 2(0 0 1), Z_8 + Z_8 
          * would be 2(0 0 2).  Z_12 would be 2(0 1) 3(1), etc.  It is a 
          * list consisting of p(a b c ... ) elements, meaning the prime p
          * appears in several ways: there are a summands of Z_p, b of Z_{p^2},
          * c of Z_{p^3}, etc.  It can also be seen as the dimension over 
          * Z_(p^k) of G_(p,k) described above.   This is 
          * orientation-independent, as it only depends on H1 of the manifold. 
          */
    	const std::string& kkTorRank() const;

         /**
          * Kawauchi-Kojima sigma vector.
          *
          * Returns the 2-torsion sigma vector.  This is an object that takes
          * values in the set {0,1,2,3,4,5,6,7,inf}, i.e. Z_8 \cup {inf}.  It 
          * is only relevant if H1 has 2-torsion.  Torsion linking forms with 
          * only odd torsion are classified by the power decomposition
          * and the Legendre symbol vector.  This is the part of the 
          * classification of torsion linking forms due to Kawauchi and Kojima. 
          *
          * A characteristic element of f_(2,k) is an element q so that 
          * f_(2,k)(q,x)=f_(2,k)(x,x) for all x in G_(2,k).  It's a fact that 
          * this element exists and it is unique, so we denote it by c_(f,k).  
          *  If c_(f,k) is zero, we make the k-th element of the sigma
          * string infinity.  Otherwise, consider the function 
          * q_k : G_2 / G_2,k--> Q/Z defined
          * by q_k([x]) = 2^(k-1)f(x,x).  It's a fact that the sum 
          *
          *       GS_k := \Sum_{x in G_2/G_2,k} exp(2\pi iq_k(x))
          *
          * satisfies GS_k = |GS_k| e^{2 \pi i \sigma / 8} for some integer j. 
          * This the \sigma_k. 
          *
          * Notice that under an orientation reversal, inf is fixed, but GS_k 
          * is modified by conjugation, which on sigma is the 
          * involution 0<->0, 1<->7, 2<->6, 3<->5, 4<->4. 
          */
	    const std::string& kkTorSigma() const;

    	/**
          * Kawauchi-Kojima Legendre symbol vector
          *
          * Returns the odd p-torsion Legendre symbol vector. This is the part 
          * of the Kawauchi-Kojima classification that's due to Seifert.  If
          * G_(p,k) is a trivial group this is defined as zero, if G_(p,k) is 
          * non-trivial the idea is to think of f_(p,k) as a symmetric bilinear
          * form on G_(p,k).  Plugging in a basis for G_(p,k) you get a square 
          * matrix whose entries are elements of Z_p.  We take the Legendre 
          * symbol of the determinant of this matrix.  This is a number 
          * -1, 0 or 1.   You get 1 if the determinant is a quadratic residue 
          * mod p (i.e. if its congruent mod p to the square of a non-zero 
          * integer), 0 if its congruent to 0 mod p, and -1 otherwise, sometimes
          *  called "a quadratic non-residue mod p". We do this for all k so 
          * that Z_p^k is a subgroup of G. 
          *
          * Under an orientation reversal, at most the sign can change.  And 
          * it does so by a factor of (-1)^{(p-1)rank(G_(p,k))/2}.            
          */
	    const std::string& kkTorLegendre() const;

      	 /**
          * Kawauchi-Kojima hyperbolicity test. 
          */    	
        bool kkIsHyperbolic() const;

    	 /**
          * Kawauchi-Kojima split test.
          */
        bool kkIsSplit() const;

         /**
          * Kawauchi-Kojima 2-torsion test.
          */
        bool kkTwoTor() const;

	/**
	 * The subgroup of the range generated by the image.
	 */
	MarkedAbelianGroup image() const;

	/**
	 * Determines whether or not the form is symmetric.
	 */
	bool isSymmetric() const;

	/**
	 * Determines whether or not the form is anti-symmetric.
	 */
	bool isAntiSymmetric() const;

	/**
	 *  Given a bilinear form A x B --> C and a map A' --> A, there
     *  is a natural composite A' x B --> C
	 */
	NBilinearForm lCompose(const HomMarkedAbelianGroup &f) const; 
	/**
	 *  Given a bilinear form A x B --> C and a map B' --> B, there
     *  is a natural composite A x B' --> C
	 */
	NBilinearForm rCompose(const HomMarkedAbelianGroup &f) const; 

	/**
	 *  Given a bilinear form A x B --> C and a map C --> C', there
     *  is a natural composite A x B --> C'
	 */
	NBilinearForm postCompose(const HomMarkedAbelianGroup &f) const; 

	/**
	 * Given a map A x B --> C, there are two adjoints, 
	 *  this returns A --> Hom(B,C), the "left" adjoint. At present
     *  we use a cheap construction of this so the domain of this map
     *  will not satisfy isEqualTo(A)
	 */
	HomMarkedAbelianGroup leftAdjoint() const;

	/**
	 * Given a map A x B --> C, there are two adjoints, 
     *  this returns B --> Hom(A,C), the "right" adjoint.   
	 */
	HomMarkedAbelianGroup rightAdjoint() const;

	/**
	 * Form is a bilinear function A x B --> C, this returns A.
	 */
	const MarkedAbelianGroup& ldomain() const;
	/**
	 * Form is a bilinear function A x B --> C, this returns B.
	 */
	const MarkedAbelianGroup& rdomain() const;
	/**
	 * Form is a bilinear function A x B --> C, this returns C.
	 */
	const MarkedAbelianGroup& range() const;

    /**
     * TODO: maybe talk about nullity, symmetry, l/rdomain, range, etc.
     * various invariants if they make sense, etc.
     */
    void writeTextShort(std::ostream& out) const;

    /**
     * TODO: maybe talk about nullity, symmetry, l/rdomain, range, etc.
     * various invariants if they make sense, etc.
     */
    void writeTextLong(std::ostream& out) const;
};


/**
 *  This procedure takes as input the NBilinearForm intP which is required to 
 * be a torsion linking form on an abelian group.  Specifically, it's assumed 
 * the map A x A --> Q/Z is symmetric, and the range is a trivially 
 * presented Z/nZ. 
 *
 * The procedure outputs the complete Kawauchi-Kojima invariants of the form, 
 * which are:
 *
 * 1) The prime power decomposition of A, as ppVec.
 *
 * 2) The classification of the 2-torsion linking pairing.  This invariant 
 *    takes the form of a vector with n elements, where 2^n is the order of 
 *    the largest subgroup of A whose order is a power of two.  The entries
 *    of the vector represent which the angle of a certain vector V in the 
 *    complex plane -- this vector can either be at the origin (8), or it is 
 *    a non-zero vector along the line of angle 2(pi)i/8*k, k=0,1,2,3,4,5,6,7. 
 *    Kawauchi and Kojima use the symbol infinity to represent zero-sums, but 
 *    we use 8. The vector V is the sum of e^{ 2^{i+1} pi i form(x,x) } 
 *    where x ranges over ?? read KK paper again...  So this invariant is 
 *    orientation-sensitive.  In particular if you reverse the orientation, the 
 *    elements of the vector negate mod 8 (except for 8, which remains 
 *    fixed).  So a vector (1 3 8 7 8) is turned to (7 5 8 1 8) on reversal of
 *    orientation.  TODO: supply more details
 *
 * 3) The classification of the odd p-torsion linking pairing.  Given a prime 
 *    power p^k, you consider the induced linking form on the quotient of 
 *    elements of order p^k modulo p^{k-1}.  The determinant you can consider 
 *    as an element of the field Z_p, and then you take the legendre symbol
 *    (whether or not the determinant is a square mod p).  This is the 
 *    invariant, as a list for all p^k dividing the order of A, with p odd. 
 *    This invariant is also orientation-sensitive, since when you change the 
 *    orientation you negate the relevant matrices, so the legendre symbol 
 *    changes if and only if the rank n of the p^k-torsion mod p^(k-1) is 
 *    odd and p | n^2 + 1. 
 *
 *   The routine also outputs linkingFormPD, which is the restriction of the 
 *   torsion linking form to the subgroups of A divisible by all the prime 
 *   factors of the order of A. 
 */
REGINA_API void computeTorsionLinkingFormInvariants(const NBilinearForm &intP, 
std::vector< std::pair< Integer, std::vector< unsigned long > > > &ppVec, 
std::vector< std::pair< Integer, std::vector< unsigned long > > > &ppList,
std::vector<unsigned long> &ttVec, 
std::vector< std::pair< unsigned long, std::vector< int > > > &ptVec, 
std::vector< MatrixRing<Rational>* > &linkingFormPD );

/**
 * Function takes as input the output of computeTorsionLinkingFormInvariants, 
 * and returns various text human-readable strings that interpret the result.
 */
REGINA_API void readTeaLeavesTLF(const std::vector< std::pair< Integer, 
                                     std::vector< unsigned long > > > &ppVec,
        const std::vector< std::pair< Integer, 
                           std::vector< unsigned long > > > &ppList,
        const std::vector<unsigned long> &ttVec, 
        const std::vector< std::pair< unsigned long, 
                           std::vector< int > > > &ptVec, 
        const std::vector< MatrixRing<Rational>* > &linkingFormPD, 
        bool orientable, 
        bool* torsionLinkingFormIsSplit, bool* torsionLinkingFormIsHyperbolic, 
        bool* torsionLinkingFormKKTTC, 
        std::string* torsionRankString,     std::string* torsionSigmaString,     
        std::string* torsionLegendreString );


/*@}*/

inline REGINA_API const NSparseGridRing< Integer >* 
 NBilinearForm::reducedSparseGrid() const
{ return reducedPairing; }

inline REGINA_API const MarkedAbelianGroup& NBilinearForm::ldomain() const
{ return lDomain; }
inline REGINA_API const MarkedAbelianGroup& NBilinearForm::rdomain() const
{ return rDomain; }
inline REGINA_API const MarkedAbelianGroup& NBilinearForm::range() const
{ return Range; }


} // namespace regina

#endif

