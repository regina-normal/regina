
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file nperm5.h
 *  \brief Deals with permutations of {0,1,2,3,4}.
 */

#ifndef __NPERM5_H
#ifndef __DOXYGEN
#define __NPERM5_H
#endif

#include <string>
#include "triangulation/nperm.h"

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a permutation of {0,1,2,3,4}.
 * Amongst other things, such permutations are used in describing
 * simplex gluings in 4-manifold triangulations.  NPerm55 objects are small
 * enough to pass about by value instead of by reference.
 *
 * Each permutation has an internal code, and this code is sufficient to
 * reconstruct the permutation.
 * Thus the internal code may be a useful means for passing
 * permutation objects to and from the engine.
 *
 * The internal code is an unsigned integer.  The lowest three bits represent
 * the image of 0, the next lowest three bits represent the image of 1 and so
 * on.
 *
 * \testfull
 */
class NPerm5 {
    private:
        unsigned code;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        NPerm5();

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code from which the new
         * permutation will be created.
         */
        NPerm5(unsigned newCode);

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3,4}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm5(int a, int b);

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
         */
        NPerm5(int a, int b, int c, int d, int e);

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
         */
        NPerm5(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1,
            int e0, int e1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm5(const NPerm5& cloneMe);

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
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or NPerm5(unsigned) and are returned by getPermCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(unsigned newCode);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NPerm5& operator = (const NPerm5& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NPerm5 operator * (const NPerm5& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm5 inverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        int sign() const;

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
        bool operator == (const NPerm5& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3 or 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm5& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm5& other) const;

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
         * Expresses this permutation as a permutation of 0, 1, 2 and 3.
         * This only makes sense if the image of 4 is 4; otherwise the
         * return value is undefined.
         *
         * \pre This permutation maps 4 to 4.
         *
         * @return this permutation expressed as a permutation of four
         * elements, not five.
         */
        NPerm asPerm4() const;

        /**
         * Converts the given 4-element permutation to a 5-element permutation.
         * The resulting 5-element permutation will map 4 to 4, and will map
         * 0, 1, 2 and 3 to their respective images under \a base.
         *
         * @return the given permutation expressed as a permutation of five
         * elements, not four.
         */
        static NPerm5 fromPerm4(const NPerm& base);

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of five adjacent digits
         * representing the images of 0, 1, 2, 3 and 4 respectively.
         * An example of a string representation is <tt>30421</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string toString() const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0 and 1.  The resulting string will therefore
         * have length two.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc2() const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0, 1 and 2.  The resulting string will therefore
         * have length three.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc3() const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0, 1, 2 and 3.  The resulting string will therefore
         * have length four.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc4() const;

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

    friend std::ostream& operator << (std::ostream& out, const NPerm5& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NPerm5::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline std::ostream& operator << (std::ostream& out, const NPerm5& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for NPerm5

inline NPerm5::NPerm5() : code(18056) {
}

inline NPerm5::NPerm5(unsigned newCode) : code(newCode) {
}

inline NPerm5::NPerm5(int a, int b) {
    code = 18056;
    code += ((a << (3*b)) - (b << (3*b)));
    code += ((b << (3*a)) - (a << (3*a)));
}

inline NPerm5::NPerm5(int a, int b, int c, int d, int e) {
    code = (e << 12) + (d << 9) + (c << 6) + (b << 3) + a;
}

inline NPerm5::NPerm5(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1) {
    code =
        (a1 << (3*a0)) +
        (b1 << (3*b0)) +
        (c1 << (3*c0)) +
        (d1 << (3*d0)) +
        (e1 << (3*e0));
}

inline NPerm5::NPerm5(const NPerm5& cloneMe) : code(cloneMe.code) {
}

inline unsigned NPerm5::getPermCode() const {
    return code;
}

inline void NPerm5::setPermCode(unsigned newCode) {
    code = newCode;
}

inline NPerm5& NPerm5::operator = (const NPerm5& cloneMe) {
    code = cloneMe.code;
    return *this;
}

inline NPerm5 NPerm5::operator *(const NPerm5& q) const {
    return NPerm5(imageOf(q[0]), imageOf(q[1]), imageOf(q[2]),
        imageOf(q[3]), imageOf(q[4]));
}

inline NPerm5 NPerm5::inverse() const {
    // Specify the inverse by its internal code.
    return NPerm5(static_cast<unsigned>(
        (1 << (3*imageOf(1))) +
        (2 << (3*imageOf(2))) +
        (3 << (3*imageOf(3))) +
        (4 << (3*imageOf(4)))));
}

inline int NPerm5::operator[](int source) const {
    return (code >> (3*source)) & 7;
}

inline int NPerm5::preImageOf(int image) const {
    if (( code       & 7) == static_cast<unsigned>(image)) return 0;
    if (((code >> 3) & 7) == static_cast<unsigned>(image)) return 1;
    if (((code >> 6) & 7) == static_cast<unsigned>(image)) return 2;
    if (((code >> 9) & 7) == static_cast<unsigned>(image)) return 3;
    return 4;
}

inline bool NPerm5::operator == (const NPerm5& other) const {
    return (code == other.code);
}

inline bool NPerm5::operator != (const NPerm5& other) const {
    return (code != other.code);
}

inline bool NPerm5::isIdentity() const {
    return (code == 18056);
}

inline NPerm NPerm5::asPerm4() const {
    return NPerm(static_cast<unsigned char>(
        (code & 0x03) |
        ((code >> 1) & 0x0c) |
        ((code >> 2) & 0x30) |
        ((code >> 3) & 0xc0)));
}

inline NPerm5 NPerm5::fromPerm4(const NPerm& base) {
    // Cast the NPerm code (char) to an NPerm5 code (unsigned).
    unsigned code = base.getPermCode();

    return NPerm5(
        (code & 0x0003) |
        ((code & 0x000c) << 1) |
        ((code & 0x0030) << 2) |
        ((code & 0x00c0) << 3) |
        (4 << 12));
}

inline int NPerm5::imageOf(int source) const {
    return (code >> (3*source)) & 7;
}

} // namespace regina

#endif

