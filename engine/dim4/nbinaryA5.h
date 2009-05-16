
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

/*! \file NBinaryA5.h
 *  \brief Deals elements of the "binary" A5 -- this is the non-trivial
 *   Z_2-central extension of A5, the alternating group on 5 elements.
 *   Think of A5 as the symmetry group of a pentachoron.  This is the lift
 *   of that group to the spinor group.
 */

#ifndef __NBINARYA5_H
#ifndef __DOXYGEN
#define __NBINARYA5_H
#endif

#include <string>
#include "triangulation/nperm.h"
#include "dim4/nperm5.h"

//#include <iostream>

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Say something...
 *
 * \testfull
 *
 * @author Ryan Budney
 */
class NBinaryA5 {
    private:
        /**
         * The internal code representing this permutation. At present this
	 * is implemented as even n corresponds the triv lift of NPerm5::S5[n]
	 * odd n corresponds to the non-trivial lift of NPerm5::S5[n-1]
         */
        unsigned code;

	static const unsigned long long mult_table[60];
	static const std::string names[120];
	
	static const unsigned long long floormask;

    public:
        /**
         * Creates the identity element.
         */
        NBinaryA5();

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code from which the new
         * permutation will be created.
         */
        NBinaryA5(unsigned newCode);

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
         * @param e the desired image of 4.
	 * @param lift specifies which lift of this permutation to take.
         */
        NBinaryA5(int a, int b, int c, int d, int e, bool lift);

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
        NBinaryA5(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1,
            int e0, int e1, bool lift);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NBinaryA5(const NBinaryA5& cloneMe);

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
        NBinaryA5& operator = (const NBinaryA5& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NBinaryA5 operator * (const NBinaryA5& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NBinaryA5 inverse() const;

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
        bool operator == (const NBinaryA5& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3 or 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NBinaryA5& other) const;

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
         * Returns a string representation of this permutation.
         * The representation will consist of five adjacent digits
         * representing the images of 0, 1, 2, 3 and 4 respectively.
         * An example of a string representation is <tt>30421</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string toString() const;

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

    friend std::ostream& operator << (std::ostream& out, const NBinaryA5& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NBinaryA5::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline std::ostream& operator << (std::ostream& out, const NBinaryA5& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for NBinaryA5

inline NBinaryA5::NBinaryA5() : code(0) {
}

inline NBinaryA5::NBinaryA5(unsigned newCode) : code(newCode) {
}

inline NBinaryA5::NBinaryA5(int a, int b, int c, int d, int e, bool lift) {
    code = NPerm5(a,b,c,d,e).S5Index() + ( lift ? 1 : 0 );
}

inline NBinaryA5::NBinaryA5(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1, bool lift) {
	code = NPerm5(a0,a1,b0,b1,c0,c1,d0,d1,e0,e1).S5Index() + ( lift ? 1 : 0 );
}

inline NBinaryA5::NBinaryA5(const NBinaryA5& cloneMe) : code(cloneMe.code) {
}

inline unsigned NBinaryA5::getPermCode() const {
    return code;
}

inline void NBinaryA5::setPermCode(unsigned newCode) {
    code = newCode;
}

inline NBinaryA5& NBinaryA5::operator = (const NBinaryA5& cloneMe) {
    code = cloneMe.code;
    return *this;
}

inline NBinaryA5 NBinaryA5::operator *(const NBinaryA5& q) const {
	return NBinaryA5( 
          (NPerm5::S5[code & floormask] * NPerm5::S5[q.code & floormask]).S5Index() + 
	  ((code + q.code + (mult_table[code >> 1] >> (q.code >> 1) )) % 2)
			);
// recover lift data from mult_table
}

inline NBinaryA5 NBinaryA5::inverse() const {
	// e_1 + f(a_1,a_1^-1) mod 2 is the lift code...
	// f(a_1,a_1^{-1}) is mult_table[code >> 1]

	// NPerm5::invS5[code & floormask] code for underlying A5 inverse elt.
	//  ((mult_table[code >> 1] >> (NPerm5::invS5[code & floormask] >> 1)) % 2)
	return NBinaryA5( NPerm5::invS5[code & floormask] + 
                         ( ((mult_table[code >> 1] >> (NPerm5::invS5[code & floormask] >> 1)) + code) % 2) 
                        );
}

inline int NBinaryA5::operator[](int source) const {
    return NPerm5::S5[code & floormask][source];
}

inline int NBinaryA5::preImageOf(int image) const {
    return NPerm5::S5[code & floormask].preImageOf(image);
}

inline bool NBinaryA5::operator == (const NBinaryA5& other) const {
    return (code == other.code);
}

inline bool NBinaryA5::operator != (const NBinaryA5& other) const {
    return (code != other.code);
}

inline bool NBinaryA5::isIdentity() const {
    return (code == 0);
}

inline int NBinaryA5::imageOf(int source) const {
    return NPerm5::S5[code & floormask][source];
}

inline std::string NBinaryA5::toString() const {
	return names[code];
}


} // namespace regina

#endif

