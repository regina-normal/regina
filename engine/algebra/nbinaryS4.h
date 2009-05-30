
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

/*! \file algebra/NBinaryS4.h
 *  \brief Deals elements of the "binary" S4 -- this is the non-trivial
 *   Z_2-central extension of S5, the alternating group on 5 elements
 *   coming from the spin cover of SO_3 restricted to the symmetry group
 *   of a tetrahedron union its geometric dual. 
 */

#ifndef __NBINARYS4_H
#ifndef __DOXYGEN
#define __NBINARYS4_H
#endif

#include <string>
#include "triangulation/nperm.h"
//#include "maths/nperm5.h"

#include <iostream>

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This is the fast class for dealing with elements in the binary symmetric group on
 * 4 elements.  The slow but versatile class is ??
 *
 * @author Ryan Budney
 */
class NBinaryS4 {
    private:
        /**
         * The internal code representing this permutation. At present
	 * 0, ..., 23 are reserved for the standard lifts and 32, ..., 55
	 * ie: 2^5 + n for n = 0, ..., 23 for the nonstandard lifts. 
	 *  thus 0 represents the identity, and 32 the kernel of the 
	 *  homomorphism NBinaryS4 --> NPerm.  
         */
        unsigned code;

	static const unsigned long mult_table[24];
	static const std::string names[48];
	
	static const std::string spinornames[48];
	static const unsigned long floormask;

    public:
        /**
         * Creates the identity element.
         */
        NBinaryS4();

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code from which the new
         * permutation will be created.
         */
        NBinaryS4(unsigned newCode);

        /**
         * Creates a permutation mapping (0,1,2,3,4) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>} = {0,1,2,3,4}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
	 * @param lift specifies which lift of this permutation to take.
         */
        NBinaryS4(int a, int b, int c, int d, bool lift);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>) respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>} =
         * {0,1,2,3,4}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param e0 the desired preimage of <i>e1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         * @param e1 the desired image of <i>e0</i>.
	 * @param lift
         */
        NBinaryS4(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1,
            bool lift);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NBinaryS4(const NBinaryS4& cloneMe);

        /**
         * Returns the internal code representing this permutation.
         * Note that the internal code is sufficient to reproduce the
         * entire permutation.
         *
         * The code returned will be a valid permutation code as
         * determined by isPermCode().
         *
         * @return the internal code.
         */
        unsigned getPermCode() const;

        /**
         * Sets this permutation to that represented by the given
         * internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code that will determine the
         * new value of this permutation.
         */
        void setPermCode(unsigned newCode);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NBinaryS4& operator = (const NBinaryS4& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NBinaryS4 operator * (const NBinaryS4& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NBinaryS4 inverse() const;

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 4 inclusive.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 4 inclusive.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2, 3 and 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const NBinaryS4& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3 or 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NBinaryS4& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2, 3 and 4 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        bool isIdentity() const;

        /**
         * Returns a string representation of this element 
         * It will be an enhanced cycle decomposition, ie:
	 * <tt>(ab)(cd) = -(cd)(ab)</tt>
         *
         * @return a string representation of this permutation.
         */
        std::string toString() const;

	/**
	 * Returns TeX string associated to spinor representation
	 * of this element.
	 */
	std::ostream& writeTeX(std::ostream &out) const;

	/**
	 * Returns TeX string associated to spinor representation
	 * of this element.
	 */
        std::string toTeX() const;

    private:
        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 4 inclusive.
         * @return the image of \a source.
         */
        int imageOf(int source) const;

    friend std::ostream& operator << (std::ostream& out, const NBinaryS4& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NBinaryS4::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline std::ostream& operator << (std::ostream& out, const NBinaryS4& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for NBinaryS4

inline NBinaryS4::NBinaryS4() : code(0) {
}

inline NBinaryS4::NBinaryS4(unsigned newCode) : code(newCode) {
}

inline NBinaryS4::NBinaryS4(int a, int b, int c, int d, bool lift) {
    code = NPerm(a,b,c,d).S4Index() + ( lift ? 32 : 0 );
}

inline NBinaryS4::NBinaryS4(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, bool lift) {
	code = NPerm(a0,a1,b0,b1,c0,c1,d0,d1).S4Index() + ( lift ? 32 : 0 );
}

inline NBinaryS4::NBinaryS4(const NBinaryS4& cloneMe) : code(cloneMe.code) {
}

inline unsigned NBinaryS4::getPermCode() const {
    return code;
}

inline void NBinaryS4::setPermCode(unsigned newCode) {
    code = newCode;
}

inline NBinaryS4& NBinaryS4::operator = (const NBinaryS4& cloneMe) {
    code = cloneMe.code;
    return *this;
}

inline NBinaryS4 NBinaryS4::operator *(const NBinaryS4& q) const {
std::cout<<" ["<<(code >> 5)<<" "<<(q.code >> 5)<<" "
	 <<(mult_table[code & floormask] >> (q.code & floormask)) % 2<<"] ";
std::cout.flush();
	return NBinaryS4( // something is wrong here...
          (NPerm::S4[code & floormask] * NPerm::S4[q.code & floormask]).S4Index() +
                       ((((code >> 5) + (q.code >> 5) + 
		        mult_table[code & floormask] >> (q.code & floormask)) % 2) << 5)
			);
}

inline NBinaryS4 NBinaryS4::inverse() const {
	return NBinaryS4( NPerm::invS4[code & floormask] + 
                          ((((mult_table[code & floormask] >> NPerm::invS4[code & floormask]) + 
			  (code >> 5)) % 2) << 5)
                        );
}

inline int NBinaryS4::operator[](int source) const {
    return NPerm::S4[code & floormask][source];
}

inline int NBinaryS4::preImageOf(int image) const {
    return NPerm::S4[code & floormask].preImageOf(image);
}

inline bool NBinaryS4::operator == (const NBinaryS4& other) const {
    return (code == other.code);
}

inline bool NBinaryS4::operator != (const NBinaryS4& other) const {
    return (code != other.code);
}

inline bool NBinaryS4::isIdentity() const {
    return (code == 0);
}

inline int NBinaryS4::imageOf(int source) const {
    return NPerm::S4[code & floormask][source];
}

inline std::string NBinaryS4::toString() const {
	return names[ (code & floormask) + ((code & 32) ? 24 : 0)];
}

inline std::ostream& NBinaryS4::writeTeX(std::ostream &out) const
{ out<<spinornames[(code & floormask) + ((code & 32) ? 24 : 0)]; return out; }

inline std::string NBinaryS4::toTeX() const
{ return spinornames[(code & floormask) + ((code & 32) ? 24 : 0)]; }


} // namespace regina

#endif

