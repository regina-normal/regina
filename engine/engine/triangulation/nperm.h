
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nperm.h
 *  \brief Deals with permutations of {0,1,2,3}.
 */

#ifndef __NPERM_H
#ifndef __DOXYGEN
#define __NPERM_H
#endif

#include <string>

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a permutation of {0,1,2,3}.
 * Such permutations are used in specifying how simplices are glued together.
 * NPerm objects are small enough to pass about by value instead of by
 * reference.
 *
 * Each permutation has an internal code, and this code is sufficient to
 * reconstruct the permutation.
 * Thus the internal code may be a useful means for passing
 * permutation objects to and from the engine.
 *
 * The internal code is a character.  The lowest two bits represent the
 * image of 0, the next lowest two bits represent the image of 1 and so
 * on.
 */
class NPerm {
    private:
        char code;
            /**< The internal code representing this permutation. */
    
    public:
        /**
         * Creates the identity permutation.
         */
        NPerm();

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code from which the new
         * permutation will be created.
         */
        NPerm(char newCode);

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         */
        NPerm(int a, int b, int c, int d);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>) respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>} =
         * {0,1,2,3}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         */
        NPerm(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm(const NPerm& cloneMe);

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
        char getPermCode() const;

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
        void setPermCode(char newCode);

        /**
         * Determines whether the given character is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or NPerm(char) and are returned by getPermCode().
         *
         * \ifaces Not present.
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(char newCode);

        /**
         * Sets this permutation to that mapping (0,1,2,3) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * \ifaces Not present.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         */
        void setPerm(int a, int b, int c, int d);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * \ifaces Not present.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         */
        void operator = (const NPerm& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * \ifaces This routine is called \a composeWith.
         * \ifacesjython Provided as an operator overload.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NPerm operator *(const NPerm& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm inverse() const;

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
         * \ifaces This routine is called \a imageOf.
         * \ifacesjython Provided as an operator overload.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 3 inclusive.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 3 inclusive.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2 and 3.
         *
         * \ifaces Not present.
         * \ifacesjython Provided as an operator overload.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const NPerm& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2 or 3.
         *
         * \ifaces Not present.
         * \ifacesjython Provided as an operator overload.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2 and 3 is
         * mapped to itself.
         *
         * \ifacescpp Not present.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        #ifdef __DOXYGEN
        bool isIdentity() const;
        #endif

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of four adjacent digits
         * representing the images of 0, 1, 2 and 3 respectively.  An
         * example of a string representation is <tt>1302</tt>.
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
         * should be between 0 and 3 inclusive.
         * @return the image of \a source.
         */
        int imageOf(int source) const;
};

// Constants

/**
 * Contains all possible permutations of four elements.
 * The permutations with even indices in the array are the even
 * permutations, and those with odd indices in the array are the odd
 * permutations.
 *
 * Note that the permutations are not necessarily in lexicographical order.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const NPerm allPermsS4[24];

/**
 * Contains the inverses of the permutations in array \a allPermsS4.
 *
 * Specifically, the inverse of permutation <tt>allPermsS4[i]</tt> is
 * permutation <tt>allPermsS4[ allPermsS4Inv[i] ]</tt>.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const unsigned allPermsS4Inv[24];

/**
 * Contains all possible permutations of four elements in
 * lexicographical order.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const NPerm orderedPermsS4[24];

/**
 * Contains all possible permutations of three elements.
 * In each permutation, 3 maps to 3.
 * The permutations with even indices in the array are the even
 * permutations, and those with odd indices in the array are the odd
 * permutations.
 *
 * Note that the permutations are not necessarily in lexicographical order.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const NPerm allPermsS3[6];

/**
 * Contains the inverses of the permutations in array \a allPermsS3.
 *
 * Specifically, the inverse of permutation <tt>allPermsS3[i]</tt> is
 * permutation <tt>allPermsS3[ allPermsS3Inv[i] ]</tt>.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const unsigned allPermsS3Inv[6];

/**
 * Contains all possible permutations of three elements in
 * lexicographical order.
 * In each permutation, 3 maps to 3.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const NPerm orderedPermsS3[6];

/**
 * Contains all possible permutations of two elements.  
 * In each permutation, 2 maps to 2 and 3 maps to 3.
 * The permutations with even indices in the array are the even
 * permutations, and those with odd indices in the array are the odd
 * permutations.
 *
 * Note that the permutations are also in lexicographical order.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const NPerm allPermsS2[2];

/**
 * Contains the inverses of the permutations in array \a allPermsS2.
 *
 * Specifically, the inverse of permutation <tt>allPermsS2[i]</tt> is
 * permutation <tt>allPermsS2[ allPermsS2Inv[i] ]</tt>.
 *
 * \ifacesjava This array is a static member of class \a NPerm.
 */
extern const unsigned allPermsS2Inv[2];

// Routines for constructing the permutations associated to
// faces and edges of the triangulation

/**
 * Returns a permutation mapping (0,1,2) to the vertices of the
 * given tetrahedron face in their canonical order.  The images of
 * (0,1,2) will be the vertex numbers of the vertices that make up the
 * given face of a generic tetrahedron.
 *
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param face a face number in a tetrahedron.  This should be between 0
 * and 3 inclusive.  Note that face <i>i</i> is opposite vertex
 * <i>i</i>.
 * @return the permutation representing the canonical ordering of
 * vertices in the given face.
 */
NPerm faceOrdering(int face);
    
/**
 * Returns a permutation mapping (0,1) to the vertices of the
 * given tetrahedron edge in their canonical order.  The images of
 * (0,1) will be the vertex numbers of the vertices that make up the
 * given edge of a generic tetrahedron.
 *
 * The images of 2 and 3 in the returned permutation will be chosen so
 * that the permutation will be even.
 *
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param edge an edge number in a tetrahedron.  This should be between 0
 * and 5 inclusive.  The constant arrays \c edgeNumber, \c edgeStart and
 * \c edgeEnd describe which vertex numbers are joined by which edge
 * numbers.
 * @return the permutation representing the canonical ordering of
 * vertices in the given edge.
 */
NPerm edgeOrdering(int edge);

/**
 * Returns a string representation of the permutation mapping
 * (0,1,2) to the vertices of the given tetrahedron face in their
 * canonical order, as described in faceOrdering().
 * Only the images of 0, 1 and 2 will be put in the string.
 *
 * \ifacescpp Not present.
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param face a face number in a tetrahedron.  This should be between 0
 * and 3 inclusive.  Note that face <i>i</i> is opposite vertex
 * <i>i</i>.
 * @return a string representing the
 * canonical ordering of vertices in the given face.
 */
#ifdef __DOXYGEN
std::string faceDescription(int face);
#endif

/**
 * Returns a string representation of the given permutation with only
 * the images of 0, 1 and 2 included.
 *
 * \ifacescpp Not present.
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param facePerm the permutation to represent.
 * @return a restricted string representation of the given permutation.
 */
#ifdef __DOXYGEN
std::string faceDescription(const NPerm& facePerm);
#endif

/**
 * Returns a string representation of the permutation mapping
 * (0,1) to the vertices of the given tetrahedron edge in their
 * canonical order, as described in edgeOrdering().
 * Only the images of 0 and 1 will be put in the string.
 *
 * \ifacescpp Not present.
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param edge an edge number in a tetrahedron.  This should be between 0
 * and 5 inclusive.  The constant arrays \c edgeNumber, \c edgeStart and
 * \c edgeEnd describe which vertex numbers are joined by which edge
 * numbers.
 * @return a string representing the canonical ordering of
 * vertices in the given edge.
 */
#ifdef __DOXYGEN
std::string edgeDescription(int edge);
#endif

/**
 * Returns a string representation of the given permutation with only
 * the images of 0 and 1 included.
 *
 * \ifacescpp Not present.
 * \ifacesjava This routine is a static member of class \a NPerm.
 *
 * @param edgePerm the permutation to represent.
 * @return a restricted string representation of the given permutation.
 */
#ifdef __DOXYGEN
std::string edgeDescription(const NPerm& edgePerm);
#endif

/*@}*/

// Inline functions for NPerm

inline NPerm::NPerm() : code(228) {
}

inline NPerm::NPerm(char newCode) : code(newCode) {
}

inline NPerm::NPerm(int a, int b) {
    code = 228;
    code += (short) ( (a << (2*b)) - (b << (2*b)) );
    code += (short) ( (b << (2*a)) - (a << (2*a)) );
}

inline NPerm::NPerm(int a, int b, int c, int d) {
    code = (short) ( (d << 6) + (c << 4) + (b << 2) + a);
}

inline NPerm::NPerm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1) {
    code = char(
        (a1 << (2*a0)) +
        (b1 << (2*b0)) +
        (c1 << (2*c0)) +
        (d1 << (2*d0)));
}

inline NPerm::NPerm(const NPerm& cloneMe) : code(cloneMe.code) {
}

inline void NPerm::setPerm(int a, int b, int c, int d) {
    code = (char) ( (d << 6) + (c << 4) + (b << 2) + a);
}

inline void NPerm::setPermCode(char newCode) {
    code = newCode;
}

inline void NPerm::operator = (const NPerm& cloneMe) {
    code = cloneMe.code;
}

inline NPerm NPerm::operator *(const NPerm& q) const {
    return NPerm(imageOf(q[0]), imageOf(q[1]), imageOf(q[2]), imageOf(q[3]));
}

inline NPerm NPerm::inverse() const {
    // Specify the inverse by its internal code.
    return NPerm(char(
        (1 << (2*imageOf(1))) +
        (2 << (2*imageOf(2))) +
        (3 << (2*imageOf(3)))));
}

inline char NPerm::getPermCode() const {
    return code;
}

inline int NPerm::operator[](int source) const {
    return imageOf(source);
}

inline int NPerm::preImageOf(int image) const {
    return inverse().imageOf(image);
}

inline int NPerm::imageOf(int source) const {
    return (code >> (2*source)) & 3;
}

inline bool NPerm::operator == (const NPerm& other) const {
    return (code == other.code);
}

inline bool NPerm::operator != (const NPerm& other) const {
    return (code != other.code);
}

} // namespace regina

#endif

