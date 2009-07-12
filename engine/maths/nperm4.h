
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

/*! \file maths/nperm4.h
 *  \brief Deals with permutations of {0,1,2,3}.
 */

#ifndef __NPERM4_H
#ifndef __DOXYGEN
#define __NPERM4_H
#endif

#include <string>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1,2,3}.
 * Amongst other things, such permutations are used in specifying how
 * simplices of a 3-manifold triangulation are glued together.
 * NPerm4 objects are small enough to pass about by value instead of by
 * reference.
 *
 * Each permutation has an internal code, and this code is sufficient to
 * reconstruct the permutation.
 * Thus the internal code may be a useful means for passing
 * permutation objects to and from the engine.
 *
 * The internal permutation codes have changed as of Regina 4.6.1:
 *
 * - \e First-generation codes were used internally in Regina 4.6 and earlier.
 *   These codes were characters whose lowest two bits represented the
 *   image of 0, whose next lowest two bits represented the image of 1,
 *   and so on.  The routines getPermCode(), setPermCode(), fromPermCode()
 *   and isPermCode() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe tetrahedron gluings.
 *
 * - \e Second-generation codes are used internally in Regina 4.6.1 and above.
 *   These codes are integers between 0 and 23 inclusive, representing the
 *   index of the permutation in the array NPerm4::S4.  The routines
 *   getPermCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by NPerm4).
 *
 * \testfull
 */
class NPerm4 {
    public:
        /**
         * Contains all possible permutations of four elements.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.
         */
        static const NPerm4 S4[24];

        /**
         * Contains the inverses of the permutations in the array \a S4.
         *
         * Specifically, the inverse of permutation <tt>S4[i]</tt> is
         * the permutation <tt>S4[ invS4[i] ]</tt>.
         */
        static const unsigned invS4[24];

        /**
         * Contains all possible permutations of four elements in
         * lexicographical order.
         */
        static const NPerm4 orderedS4[24];

        /**
         * Contains all possible permutations of three elements.
         * In each permutation, 3 maps to 3.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.
         */
        static const NPerm4 S3[6];

        /**
         * Contains the inverses of the permutations in the array \a S3.
         *
         * Specifically, the inverse of permutation <tt>S3[i]</tt> is
         * the permutation <tt>S3[ invS3[i] ]</tt>.
         */
        static const unsigned invS3[6];

        /**
         * Contains all possible permutations of three elements in
         * lexicographical order.  In each permutation, 3 maps to 3.
         */
        static const NPerm4 orderedS3[6];

        /**
         * Contains all possible permutations of two elements.
         * In each permutation, 2 maps to 2 and 3 maps to 3.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that the permutations are also in lexicographical order.
         */
        static const NPerm4 S2[2];

        /**
         * Contains the inverses of the permutations in the array \a S2.
         *
         * Specifically, the inverse of permutation <tt>S2[i]</tt> is
         * the permutation <tt>S2[ invS2[i] ]</tt>.
         */
        static const unsigned invS2[2];

    private:
        unsigned char code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        NPerm4();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm4(int a, int b);

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
        NPerm4(int a, int b, int c, int d);

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
        NPerm4(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm4(const NPerm4& cloneMe);

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode().
         *
         * \warning This routine will incur additional overhead, since
         * NPerm4 now uses second-generation codes internally.
         * See the class notes and the routine getPermCode2() for details.
         *
         * @return the first-generation permutation code.
         */
        unsigned char getPermCode() const;

        /**
         * Returns the second-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid second-generation permutation
         * code as determined by isPermCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the NPerm4 class.
         *
         * @return the second-generation permutation code.
         */
        unsigned char getPermCode2() const;

        /**
         * Sets this permutation to that represented by the given
         * first-generation permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * NPerm4 now uses second-generation codes internally.
         * See the class notes and the routine setPermCode2() for details.
         *
         * @param code the first-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode(unsigned char code);

        /**
         * Sets this permutation to that represented by the given
         * second-generation permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the NPerm4 class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode2(unsigned char code);

        /**
         * Creates a permutation from the given first-generation
         * permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * NPerm4 now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static NPerm4 fromPermCode(unsigned char code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the NPerm4 class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static NPerm4 fromPermCode2(unsigned char code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by getPermCode().
         *
         * \warning This routine will incur additional overhead, since
         * NPerm4 now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * first-generation permutation code.
         */
        static bool isPermCode(unsigned char code);

        /**
         * Determines whether the given character is a valid second-generation
         * permutation code.  Valid second-generation codes can be passed
         * to setPermCode2() or fromPermCode2(), and are returned by
         * getPermCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the NPerm4 class.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static bool isPermCode2(unsigned char code);

        /**
         * Sets this permutation to the transposition of
         * \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3}.
         *
         * \deprecated This routine is largely unnecessary, since NPerm4
         * objects are tiny and cheap.  Just use the assignment operator
         * instead.  This routine will eventually be removed entirely in
         * a future version of Regina.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        void setPerm(int a, int b);

        /**
         * Sets this permutation to that mapping (0,1,2,3) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * \deprecated This routine is largely unnecessary, since NPerm4
         * objects are tiny and cheap.  Just use the assignment operator
         * instead.  This routine will eventually be removed entirely in
         * a future version of Regina.
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
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NPerm4& operator = (const NPerm4& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NPerm4 operator *(const NPerm4& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm4 inverse() const;

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
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const NPerm4& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2 or 3.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm4& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm4& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2 and 3 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        bool isIdentity() const;

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of four adjacent digits
         * representing the images of 0, 1, 2 and 3 respectively.  An
         * example of a string representation is <tt>1302</tt>.
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
         * the images of 0, 1 and 2 included.  The resulting string will
         * therefore have length three.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc3() const;

        /**
         * Returns the index of this permutation in the NPerm4::S4 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm4::S4[i].  This will be between 0 and 23 inclusive.
         */
        int S4Index() const;

        /**
         * Returns the index of this permutation in the NPerm4::orderedS4 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm4::orderedS4[i].  This will be between 0 and 23 inclusive.
         */
        int orderedS4Index() const;

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S4[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static const unsigned char imageTable[24][4];

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S4[x] * S4[y]</tt> is the
         * permutation <tt>S4[product[x][y]]</tt>.
         */
        static const unsigned char productTable[24][24];

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S4[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static const unsigned char swapTable[4][4];

    private:
        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code from which the new
         * permutation will be created.
         */
        NPerm4(unsigned char code);

        /**
         * Returns the index into the NPerm4::S4 array of the permutation that
         * maps (0,1,2,3) to (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @return the index \a i for which the given permutation is equal to
         * NPerm4::S4[i].  This will be between 0 and 23 inclusive.
         */
        static int S4Index(int a, int b, int c, int d);

    friend std::ostream& operator << (std::ostream& out, const NPerm4& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NPerm4::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline std::ostream& operator << (std::ostream& out, const NPerm4& p) {
    return (out << p.toString());
}

// Constants

/**
 * An array of size 24 containing all possible permutations of four elements.
 *
 * The permutations with even indices in the array are the even permutations,
 * and those with odd indices in the array are the odd permutations.
 *
 * Note that the permutations are not necessarily in lexicographical order.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::S4.  The deprecated name
 * regina::allPermsS4 now just points to NPerm4::S4, and will be removed
 * in some future version of Regina.
 */
extern const NPerm4* allPermsS4;

/**
 * An array of size 24 containing the inverses of the permutations in the
 * array \a allPermsS4.
 *
 * Specifically, the inverse of permutation <tt>allPermsS4[i]</tt> is
 * the permutation <tt>allPermsS4[ allPermsS4Inv[i] ]</tt>.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::invS4.  The deprecated name
 * regina::allPermsS4Inv now just points to NPerm4::invS4, and will
 * be removed in some future version of Regina.
 */
extern const unsigned* allPermsS4Inv;

/**
 * An array of size 24 containing all possible permutations of four elements
 * in lexicographical order.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::orderedS4.  The deprecated name
 * regina::orderedPermsS4 now just points to NPerm4::orderedS4, and will be
 * removed in some future version of Regina.
 */
extern const NPerm4* orderedPermsS4;

/**
 * An array of size 6 containing all possible permutations of three elements.
 * In each permutation, 3 maps to 3.
 *
 * The permutations with even indices in the array are the even permutations,
 * and those with odd indices in the array are the odd permutations.
 *
 * Note that the permutations are not necessarily in lexicographical order.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::S3.  The deprecated name
 * regina::allPermsS4 now just points to NPerm4::S3, and will be removed
 * in some future version of Regina.
 */
extern const NPerm4* allPermsS3;

/**
 * An array of size 6 containing the inverses of the permutations in the
 * array \a allPermsS3.
 *
 * Specifically, the inverse of permutation <tt>allPermsS3[i]</tt> is
 * the permutation <tt>allPermsS3[ allPermsS3Inv[i] ]</tt>.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::invS3.  The deprecated name
 * regina::allPermsS3Inv now just points to NPerm4::invS3, and will be
 * removed in some future version of Regina.
 */
extern const unsigned* allPermsS3Inv;

/**
 * An array of size 6 containing all possible permutations of three elements
 * in lexicographical order.  In each permutation, 3 maps to 3.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::orderedS3.  The deprecated name
 * regina::orderedPermsS3 now just points to NPerm4::orderedS3, and will be
 * removed in some future version of Regina.
 */
extern const NPerm4* orderedPermsS3;

/**
 * An array of size 2 containing all possible permutations of two elements.
 * In each permutation, 2 maps to 2 and 3 maps to 3.
 *
 * The permutations with even indices in the array are the even permutations,
 * and those with odd indices in the array are the odd permutations.
 *
 * Note that the permutations are also in lexicographical order.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::S2.  The deprecated name
 * regina::allPermsS2 now just points to NPerm4::S2, and will be removed in
 * some future version of Regina.
 */
extern const NPerm4* allPermsS2;

/**
 * An array of size 2 containing the inverses of the permutations in the
 * array \a allPermsS2.
 *
 * Specifically, the inverse of permutation <tt>allPermsS2[i]</tt> is
 * the permutation <tt>allPermsS2[ allPermsS2Inv[i] ]</tt>.
 *
 * \deprecated This array has been moved into the NPerm4 class, and can now
 * be accessed as the static array NPerm4::invS2.  The deprecated name
 * regina::allPermsS2Inv now just points to NPerm4::invS2, and will be
 * removed in some future version of Regina.
 */
extern const unsigned* allPermsS2Inv;

// Routines for constructing the permutations associated to
// faces and edges of the triangulation

/**
 * Returns a permutation mapping (0,1,2) to the vertices of the
 * given tetrahedron face in their canonical order.  The images of
 * (0,1,2) will be the vertex numbers of the vertices that make up the
 * given face of a generic tetrahedron.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use the lookup
 * table NFace::ordering instead (which gives identical results).
 *
 * @param face a face number in a tetrahedron.  This should be between 0
 * and 3 inclusive.  Note that face <i>i</i> is opposite vertex
 * <i>i</i>.
 * @return the permutation representing the canonical ordering of
 * vertices in the given face.
 */
NPerm4 faceOrdering(int face);

/**
 * Returns a permutation mapping (0,1) to the vertices of the
 * given tetrahedron edge in their canonical order.  The images of
 * (0,1) will be the vertex numbers of the vertices that make up the
 * given edge of a generic tetrahedron.
 *
 * The images of 2 and 3 in the returned permutation will be chosen so
 * that the permutation will be even.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use the lookup
 * table NEdge::ordering instead (which gives identical results).
 *
 * @param edge an edge number in a tetrahedron.  This should be between 0 and
 * 5 inclusive.  The constant arrays NEdge::edgeNumber and NEdge::edgeVertex
 * describe which vertex numbers are joined by which edge numbers.
 * @return the permutation representing the canonical ordering of
 * vertices in the given edge.
 */
NPerm4 edgeOrdering(int edge);

/**
 * Returns a string representation of the permutation mapping
 * (0,1,2) to the vertices of the given tetrahedron face in their
 * canonical order, as described in faceOrdering().
 * Only the images of 0, 1 and 2 will be put in the string.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use
 * <tt>NFace::ordering[face].trunc3()</tt> (which gives identical results).
 *
 * @param face a face number in a tetrahedron.  This should be between 0
 * and 3 inclusive.  Note that face <i>i</i> is opposite vertex
 * <i>i</i>.
 * @return a string representing the
 * canonical ordering of vertices in the given face.
 */
std::string faceDescription(int face);

/**
 * Returns a string representation of the given permutation with only
 * the images of 0, 1 and 2 included.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use NPerm4::trunc3()
 * instead (which gives identical results).
 *
 * @param facePerm the permutation to represent.
 * @return a restricted string representation of the given permutation.
 */
std::string faceDescription(const NPerm4& facePerm);

/**
 * Returns a string representation of the permutation mapping
 * (0,1) to the vertices of the given tetrahedron edge in their
 * canonical order, as described in edgeOrdering().
 * Only the images of 0 and 1 will be put in the string.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use
 * <tt>NEdge::ordering[edge].trunc2()</tt> (which gives identical results).
 *
 * @param edge an edge number in a tetrahedron.  This should be between 0 and
 * 5 inclusive.  The constant arrays NEdge::edgeNumber and NEdge::edgeVertex
 * describe which vertex numbers are joined by which edge numbers.
 * @return a string representing the canonical ordering of
 * vertices in the given edge.
 */
std::string edgeDescription(int edge);

/**
 * Returns a string representation of the given permutation with only
 * the images of 0 and 1 included.
 *
 * \deprecated This routine is no longer recommended, and will be
 * removed in some future version of Regina.  Please use NPerm4::trunc2()
 * instead (which gives identical results).
 *
 * @param edgePerm the permutation to represent.
 * @return a restricted string representation of the given permutation.
 */
std::string edgeDescription(const NPerm4& edgePerm);

/*@}*/

// Inline functions for NPerm4

inline NPerm4::NPerm4() : code_(0) {
}

inline NPerm4::NPerm4(unsigned char code) : code_(code) {
}

inline NPerm4::NPerm4(int a, int b) : code_(swapTable[a][b]) {
}

inline NPerm4::NPerm4(int a, int b, int c, int d) :
        code_(static_cast<unsigned char>(S4Index(a, b, c, d))) {
}

inline NPerm4::NPerm4(const NPerm4& cloneMe) : code_(cloneMe.code_) {
}

inline void NPerm4::setPerm(int a, int b) {
    code_ = swapTable[a][b];
}

inline void NPerm4::setPerm(int a, int b, int c, int d) {
    code_ = static_cast<unsigned char>(S4Index(a, b, c, d));
}

inline unsigned char NPerm4::getPermCode() const {
    return (static_cast<unsigned char>(imageTable[code_][0]) |
            (static_cast<unsigned char>(imageTable[code_][1]) << 2) |
            (static_cast<unsigned char>(imageTable[code_][2]) << 4) |
            (static_cast<unsigned char>(imageTable[code_][3]) << 6));
}

inline unsigned char NPerm4::getPermCode2() const {
    return code_;
}

inline void NPerm4::setPermCode(unsigned char code) {
    code_ = static_cast<unsigned char>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03));
}

inline void NPerm4::setPermCode2(unsigned char code) {
    code_ = code;
}

inline NPerm4 NPerm4::fromPermCode(unsigned char code) {
    return NPerm4(static_cast<unsigned char>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03)));
}

inline NPerm4 NPerm4::fromPermCode2(unsigned char code) {
    return NPerm4(code);
}

inline bool NPerm4::isPermCode2(unsigned char code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 24);
}

inline NPerm4& NPerm4::operator = (const NPerm4& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

inline NPerm4 NPerm4::operator *(const NPerm4& q) const {
    return NPerm4(productTable[code_][q.code_]);
}

inline NPerm4 NPerm4::inverse() const {
    return NPerm4(static_cast<unsigned char>(invS4[code_]));
}

inline int NPerm4::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline int NPerm4::operator[](int source) const {
    return imageTable[code_][source];
}

inline int NPerm4::preImageOf(int image) const {
    return imageTable[invS4[code_]][image];
}

inline bool NPerm4::isIdentity() const {
    return (code_ == 0);
}

inline bool NPerm4::operator == (const NPerm4& other) const {
    return (code_ == other.code_);
}

inline bool NPerm4::operator != (const NPerm4& other) const {
    return (code_ != other.code_);
}

inline int NPerm4::S4Index() const {
    return code_;
}

inline int NPerm4::orderedS4Index() const {
    // S4 is almost the same as orderedS4, except that some pairs
    // S4[2i] <--> S4[2i+1] have been swapped to ensure that all
    // permutations S4[2i] are even and all permutations S4[2i+1] are odd.
    //
    // Specifically, we must interchange all pairs 4i+2 <--> 4i+3.
    return ((code_ & 2) ? (code_ ^ 1) : code_);
}

inline int NPerm4::S4Index(int a, int b, int c, int d) {
    int orderedS4Index = 6 * a +
                         2 * (b - (b > a ? 1 : 0)) +
                             (c > d ? 1 : 0);

    // As above, to obtain an S4 index, interchange all pairs 4i+2 <--> 4i+3.
    return ((orderedS4Index & 2) ? (orderedS4Index ^ 1) : orderedS4Index);
}

inline std::string faceDescription(const NPerm4& facePerm) {
    return facePerm.trunc3();
}

inline std::string edgeDescription(const NPerm4& edgePerm) {
    return edgePerm.trunc2();
}

} // namespace regina

#endif

