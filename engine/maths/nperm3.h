
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file maths/nperm3.h
 *  \brief Deals with permutations of {0,1,2}.
 */

#ifndef __NPERM3_H
#ifndef __DOXYGEN
#define __NPERM3_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1,2}.
 *
 * These objects are small enough to pass about by value instead of by
 * reference.  Moreover, they are extremely fast to work with.
 *
 * Each permutation has an internal code, and this code is sufficient to
 * reconstruct the permutation.
 * Thus the internal code may be a useful means for passing
 * permutation objects to and from the engine.
 *
 * The internal code is an integer between 0 and 5 inclusive,
 * representing the index of the permutation in the array NPerm3::S3.
 *
 * This class is faster and sleeker than related classes such as NPerm4 and
 * NPerm5.  On the other hand, this class does not offer quite as rich an
 * interface as the others.
 *
 * \testfull
 */
class REGINA_API NPerm3 {
    public:
        /**
         * Contains all possible permutations of three elements.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (NPerm3, NPerm4 and so on),
         * the S3 array stores the same permutations in the same order
         * (but of course using different data types).
         *
         * Note that these permutations are not necessarily in
         * lexicographical order.
         */
        static const NPerm3 S3[6];

        /**
         * A dimension-agnostic alias for NPerm3::S3.  In general, for
         * each \a K the class NPerm<i>K</i> will define an alias \a Sn
         * that references the list of all permutations
         * NPerm<i>K</i>::S<i>K</i>.
         */
        static const NPerm3* Sn;

        /**
         * Contains the inverses of the permutations in the array \a S3.
         *
         * Specifically, the inverse of permutation <tt>S3[i]</tt> is
         * the permutation <tt>S3[ invS3[i] ]</tt>.
         */
        static const int invS3[6];

        /**
         * Contains all possible permutations of three elements in
         * lexicographical order.
         */
        static const NPerm3 orderedS3[6];

        /**
         * Contains all possible permutations of two elements.
         * In each permutation, 2 maps to 2 and 3 maps to 3.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (NPerm3, NPerm4 and so on),
         * the S2 array stores the same permutations in the same order
         * (but of course using different data types).
         *
         * Note that these permutations are already in lexicographical order.
         */
        static const NPerm3 S2[2];

        /**
         * A dimension-agnostic alias for NPerm3::S2.  In general, for
         * each \a K the class NPerm<i>K</i> will define an alias \a Sn_1
         * that references the list of all permutations
         * NPerm<i>K</i>::S<i>(K-1)</i>.
         */
        static const NPerm3* Sn_1;

        enum {
            /**
             * The total number of permutations on three elements.
             * This is the size of the array Sn.
             *
             * \ifacespython Not present.
             */
            nPerms = 6,

            /**
             * The total number of permutations on two elements.
             * This is the size of the array Sn_1.
             *
             * \ifacespython Not present.
             */
            nPerms_1 = 2
        };

        enum {
            /**
             * The internal code for the permutation (0,1,2).
             *
             * \ifacespython Not present.
             */
            code012 = 0,
            /**
             * The internal code for the permutation (0,2,1).
             *
             * \ifacespython Not present.
             */
            code021 = 1,
            /**
             * The internal code for the permutation (1,2,0).
             *
             * \ifacespython Not present.
             */
            code120 = 2,
            /**
             * The internal code for the permutation (1,0,2).
             *
             * \ifacespython Not present.
             */
            code102 = 3,
            /**
             * The internal code for the permutation (2,0,1).
             *
             * \ifacespython Not present.
             */
            code201 = 4,
            /**
             * The internal code for the permutation (2,1,0).
             *
             * \ifacespython Not present.
             */
            code210 = 5
        };

    private:
        unsigned char code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        NPerm3();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm3(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2) to
         * (<i>a</i>,<i>b</i>,<i>c</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>} = {0,1,2}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         */
        NPerm3(int a, int b, int c);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2.
         *
         * \pre The array \a image contains three elements, which are
         * 0, 1 and 2 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        NPerm3(const int* image);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm3(const NPerm3& cloneMe);

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
        unsigned char getPermCode() const;

        /**
         * Sets this permutation to that represented by the given
         * internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code that will determine the
         * new value of this permutation.
         */
        void setPermCode(unsigned char code);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code for the new permutation.
         * @return the permutation represented by the given internal code.
         */
        static NPerm3 fromPermCode(unsigned char code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by getPermCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(unsigned char code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NPerm3& operator = (const NPerm3& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NPerm3 operator * (const NPerm3& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm3 inverse() const;

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
         * should be between 0 and 2 inclusive.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1 and 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const NPerm3& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1 or 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm3& other) const;

        /**
         * Lexicographically compares the images of (0,1,2) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm3& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1 and 2 is mapped to itself.
         *
         * @return \c true if and only if this is the identity permutation.
         */
        bool isIdentity() const;

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of three adjacent digits
         * representing the images of 0, 1 and 2 respectively.  An
         * example of a string representation is <tt>120</tt>.
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
         * Returns the index of this permutation in the NPerm3::S3 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm3::S3[i].  This will be between 0 and 5 inclusive.
         */
        int S3Index() const;

        /**
         * Returns the index of this permutation in the NPerm3::orderedS3 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm3::orderedS3[i].  This will be between 0 and 5 inclusive.
         */
        int orderedS3Index() const;

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S3[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static const unsigned char imageTable[6][3];

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S3[x] * S3[y]</tt> is
         * the permutation <tt>S3[product[x][y]]</tt>.
         */
        static const unsigned char productTable[6][6];

    private:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code from which the new
         * permutation will be created.
         */
        NPerm3(unsigned char code);

    friend std::ostream& operator << (std::ostream& out, const NPerm3& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NPerm3::toString().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline REGINA_API std::ostream& operator << (std::ostream& out,
        const NPerm3& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for NPerm3

inline NPerm3::NPerm3() : code_(0) {
}

inline NPerm3::NPerm3(unsigned char code) : code_(code) {
}

inline NPerm3::NPerm3(int a, int b) {
    // Transposition.
    if (a == b)
        code_ = code012;
    else switch (a) {
        case 0:
            code_ = (b == 1 ? code102 : code210); break;
        case 1:
            code_ = (b == 0 ? code102 : code021); break;
        case 2:
            code_ = (b == 0 ? code210 : code021); break;
    }
}

inline NPerm3::NPerm3(int a, int b, int) {
    // Images of (0, 1, 2).
    switch (a) {
        case 0:
            code_ = static_cast<unsigned char>(b == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<unsigned char>(b == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<unsigned char>(b == 0 ? 4 : 5); break;
    }
}

inline NPerm3::NPerm3(const int* image) {
    switch (image[0]) {
        case 0:
            code_ = static_cast<unsigned char>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<unsigned char>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<unsigned char>(image[1] == 0 ? 4 : 5); break;
    }
}

inline NPerm3::NPerm3(const NPerm3& cloneMe) : code_(cloneMe.code_) {
}

inline unsigned char NPerm3::getPermCode() const {
    return code_;
}

inline void NPerm3::setPermCode(unsigned char code) {
    code_ = code;
}

inline NPerm3 NPerm3::fromPermCode(unsigned char code) {
    return NPerm3(code);
}

inline bool NPerm3::isPermCode(unsigned char code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 6);
}

inline NPerm3& NPerm3::operator = (const NPerm3& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

inline NPerm3 NPerm3::operator * (const NPerm3& q) const {
    return NPerm3(productTable[code_][q.code_]);
}

inline NPerm3 NPerm3::inverse() const {
    return NPerm3(static_cast<unsigned char>(invS3[code_]));
}

inline int NPerm3::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline int NPerm3::operator[](int source) const {
    return imageTable[code_][source];
}

inline int NPerm3::preImageOf(int image) const {
    return imageTable[invS3[code_]][image];
}

inline bool NPerm3::operator == (const NPerm3& other) const {
    return (code_ == other.code_);
}

inline bool NPerm3::operator != (const NPerm3& other) const {
    return (code_ != other.code_);
}

inline int NPerm3::compareWith(const NPerm3& other) const {
    // Computing orderedS3Index() is very fast.
    // Use this instead of comparing images one at a time.
    int o1 = orderedS3Index();
    int o2 = other.orderedS3Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline bool NPerm3::isIdentity() const {
    return (code_ == 0);
}

inline int NPerm3::S3Index() const {
    return code_;
}

inline int NPerm3::orderedS3Index() const {
    if (code_ == 2 || code_ == 3)
        return code_ ^ 1;
    else
        return code_;
}

} // namespace regina

#endif

