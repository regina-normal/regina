
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

/*! \file algebra/nbinaryS4.h
 *  \brief Deals elements of the "binary" S4 -- this is the non-trivial
 *   Z_2-central extension of S5, the alternating group on 5 elements
 *   coming from the spin cover of SO_3 restricted to the symmetry group
 *   of a tetrahedron union its geometric dual. 
 * TODO: add action os S4
 */

#ifndef __BINARYS4_H
#ifndef __DOXYGEN
#define __BINARYS4_H
#endif

#include <string>
#include "regina-core.h"
#include "maths/perm.h"

#include <iostream>

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * This is the fast class for dealing with elements in the binary symmetric 
 * group on 4 elements.  The slow but versatile class is ??
 *
 * @author Ryan Budney
 */
class REGINA_API BinaryS4 {
    private:
        /**
         * The internal code representing this permutation. At present the first
	 * bit indicates the lift, and the remaining bits are the << 1 shift of
	 * a number 0, ..., 23 representing Perm<4>::S4
         */
        unsigned code;

	static const unsigned long mult_table[24];
	static const std::string names[48];
	static const std::string spinornames[48];

    public:
        /**
         * Creates the identity element.
         */
        BinaryS4();

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
        BinaryS4(int a, int b, int c, int d, bool lift);

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
        BinaryS4(int a0, int a1, int b0, int b1, int c0, int c1, 
            int d0, int d1, bool lift);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        BinaryS4(const BinaryS4& cloneMe);

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
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code for the new permutation.
         * @return the permutation represented by the given internal
         * code.
         */
        static BinaryS4 fromPermCode(unsigned newCode);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        BinaryS4& operator = (const BinaryS4& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        BinaryS4 operator * (const BinaryS4& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        BinaryS4 inverse() const;

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
        bool operator == (const BinaryS4& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3 or 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const BinaryS4& other) const;

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

	/**
	 * get underlying Perm4
	 */
	Perm<4> getPerm4() const;

    private:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code from which the new
         * permutation will be created.
         */
        BinaryS4(unsigned newCode);

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 4 inclusive.
         * @return the image of \a source.
         */
        int imageOf(int source) const;

    friend std::ostream& operator << (std::ostream& out, const BinaryS4& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * BinaryS4::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline REGINA_API std::ostream& operator << (std::ostream& out,
        const BinaryS4& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for BinaryS4

inline BinaryS4::BinaryS4() : code(0) {
}

inline BinaryS4::BinaryS4(unsigned newCode) : code(newCode) {
}

inline BinaryS4::BinaryS4(int a, int b, int c, int d, bool lift) {
    code = (Perm<4>(a,b,c,d).S4Index() << 1) + ( lift ? 1 : 0 );
}

inline BinaryS4::BinaryS4(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, bool lift) {
	code = (Perm<4>(a0,a1,b0,b1,c0,c1,d0,d1).S4Index() << 1) + ( lift ? 1 : 0 );
}

inline BinaryS4::BinaryS4(const BinaryS4& cloneMe) : code(cloneMe.code) {
}

inline unsigned BinaryS4::getPermCode() const {
    return code;
}

inline void BinaryS4::setPermCode(unsigned newCode) {
    code = newCode;
}

inline BinaryS4 BinaryS4::fromPermCode(unsigned newCode) {
    return BinaryS4(newCode);
}

inline BinaryS4& BinaryS4::operator = (const BinaryS4& cloneMe) {
    code = cloneMe.code;
    return *this;
}

inline BinaryS4 BinaryS4::operator * (const BinaryS4& q) const {
	return BinaryS4( 
          ((Perm<4>::S4[code >> 1] * Perm<4>::S4[q.code >> 1]).S4Index()<<1) + 
          (( code + q.code + (mult_table[code >> 1] >> (q.code >> 1) ) ) % 2)
			);
}

inline BinaryS4 BinaryS4::inverse() const {
 return BinaryS4( (Perm<4>::invS4[code >> 1] << 1) + 
 ( (code + (mult_table[code >> 1] >> Perm<4>::invS4[code >> 1])) % 2 ) );
}

inline int BinaryS4::operator[](int source) const {
    return Perm<4>::S4[code >> 1][source];
}

inline int BinaryS4::preImageOf(int image) const {
    return Perm<4>::S4[code >> 1].preImageOf(image);
}

inline bool BinaryS4::operator == (const BinaryS4& other) const {
    return (code == other.code);
}

inline bool BinaryS4::operator != (const BinaryS4& other) const {
    return (code != other.code);
}

inline bool BinaryS4::isIdentity() const {
    return (code == 0);
}

inline int BinaryS4::imageOf(int source) const {
    return Perm<4>::S4[code >> 1][source];
}

inline std::string BinaryS4::toString() const {
	return names[code];
}

inline Perm<4> BinaryS4::getPerm4() const
{
	return Perm<4>::S4[code >> 1];
}

inline std::ostream& BinaryS4::writeTeX(std::ostream &out) const
{ out<<spinornames[code]; return out; }

inline std::string BinaryS4::toTeX() const
{ return spinornames[code]; }


} // namespace regina

#endif

