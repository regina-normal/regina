
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file maths/nperm2.h
 *  \brief Deals with permutations of {0,1}.
 */

#ifndef __NPERM2_H
#ifndef __DOXYGEN
#define __NPERM2_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1}.
 *
 * These objects are small enough to pass about by value instead of by
 * reference.  Moreover, they are extremely fast to work with.
 *
 * Each permutation has an internal code, and this code is sufficient to
 * reconstruct the permutation.
 * Thus the internal code may be a useful means for passing
 * permutation objects to and from the engine.
 *
 * The internal code is 0 for the identity permutation, or 1 for the
 * permutation that swaps (0,1).
 */
class REGINA_API NPerm2 {
    public:
        /**
         * The total number of permutations on two elements.
         * This is the size of the array Sn.
         */
        static const int nPerms = 2;

        /**
         * The total number of permutations on one element.
         * This is the size of the array Sn_1.
         */
        static const int nPerms_1 = 1;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        typedef uint8_t Code;

        /**
         * Contains all possible permutations of two elements.
         *
         * The identity is S2[0], and the swap is S2[1].
         *
         * For all permutation classes (NPerm3, NPerm4 and so on),
         * the S2 array stores the same permutations in the same order
         * (but of course using different data types).
         */
        static const NPerm2 S2[6];

        /**
         * A dimension-agnostic alias for NPerm2::S2.  In general, for
         * each \a K the class NPermK will define an alias \a Sn
         * that references the list of all permutations NPermK::SK.
         */
        static const NPerm2* Sn;

        /**
         * Contains the inverses of the permutations in the array \a S3.
         *
         * Specifically, the inverse of permutation <tt>S3[i]</tt> is
         * the permutation <tt>S3[ invS3[i] ]</tt>.
         */
        static const unsigned invS3[6];

        /**
         * A dimension-agnostic alias for NPerm2::invS3.  In general, for
         * each \a K the class NPermK will define an alias \a invSn
         * that references the list of all permutations NPermK::invSK.
         */
        static const unsigned* invSn;

        /**
         * Contains all possible permutations of three elements in
         * lexicographical order.
         */
        static const NPerm2 orderedS3[6];

        /**
         * A dimension-agnostic alias for NPerm2::orderedS3.  In general, for
         * each \a K the class NPermK will define an alias \a orderedSn
         * that references the list of all permutations NPermK::orderedSK.
         */
        static const NPerm2* orderedSn;

        /**
         * Contains all possible permutations of two elements.
         * In each permutation, 2 maps to 2 and 3 maps to 3.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (NPerm2, NPerm4 and so on),
         * the S2 array stores the same permutations in the same order
         * (but of course using different data types).
         *
         * Note that these permutations are already in lexicographical order.
         */
        static const NPerm2 S2[2];

        /**
         * A dimension-agnostic alias for NPerm2::S2.  In general, for
         * each \a K the class NPermK will define an alias \a Sn_1
         * that references the list of all permutations NPermK::S(K-1).
         */
        static const NPerm2* Sn_1;

    private:
        Code code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        NPerm2();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm2(int a, int b);

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
        NPerm2(const int* image);

        /**
         * Creates a permutation mapping (\a a[0], ..., \a a[2]) to
         * (\a b[0], ..., \a b[2]) respectively.
         *
         * \pre Both arrays \a a and \a b contain 3 elements, which
         * are 0,...,2 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length 3.
         * @param b the corresponding array of images; this must also have
         * length 3.
         */
        NPerm2(const int* a, const int* b);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm2(const NPerm2& cloneMe);

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
        Code getPermCode() const;

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
        void setPermCode(Code code);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code for the new permutation.
         * @return the permutation represented by the given internal code.
         */
        static NPerm2 fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by getPermCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NPerm2& operator = (const NPerm2& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        NPerm2 operator * (const NPerm2& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm2 inverse() const;

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
        bool operator == (const NPerm2& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1 or 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm2& other) const;

        /**
         * Lexicographically compares the images of (0,1,2) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm2& other) const;

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
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 3 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

        /**
         * Returns the index of this permutation in the NPerm2::S3 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm2::S3[i].  This will be between 0 and 5 inclusive.
         */
        int S2Index() const;

        /**
         * Returns the index of this permutation in the NPerm2::S3 array.
         * This is a dimension-agnostic alias for S3Index().
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm2::S2[i].  This will be between 0 and 5 inclusive.
         */
        int SnIndex() const;

        /**
         * Returns the index of this permutation in the NPerm2::orderedS2 array.
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm2::orderedS3[i].  This will be between 0 and 5 inclusive.
         */
        int orderedS2Index() const;

        /**
         * Returns the index of this permutation in the NPerm2::orderedS2 array.
         * This is a dimension-agnostic alias for orderedS3Index().
         *
         * @return the index \a i for which this permutation is equal to
         * NPerm2::orderedS3[i].  This will be between 0 and 5 inclusive.
         */
        int orderedSnIndex() const;

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
        NPerm2(Code code);

    friend std::ostream& operator << (std::ostream& out, const NPerm2& p);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NPerm2::str().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 */
inline REGINA_API std::ostream& operator << (std::ostream& out,
        const NPerm2& p) {
    return (out << p.str());
}

/*@}*/

// Inline functions for NPerm2

inline NPerm2::NPerm2() : code_(0) {
}

inline NPerm2::NPerm2(Code code) : code_(code) {
}

inline NPerm2::NPerm2(int a, int b) {
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

inline NPerm2::NPerm2(int a, int b, int) {
    // Images of (0, 1, 2).
    switch (a) {
        case 0:
            code_ = static_cast<Code>(b == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(b == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(b == 0 ? 4 : 5); break;
    }
}

inline NPerm2::NPerm2(const int* image) {
    switch (image[0]) {
        case 0:
            code_ = static_cast<Code>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(image[1] == 0 ? 4 : 5); break;
    }
}

inline NPerm2::NPerm2(const NPerm2& cloneMe) : code_(cloneMe.code_) {
}

inline NPerm2::Code NPerm2::getPermCode() const {
    return code_;
}

inline void NPerm2::setPermCode(Code code) {
    code_ = code;
}

inline NPerm2 NPerm2::fromPermCode(Code code) {
    return NPerm2(code);
}

inline bool NPerm2::isPermCode(Code code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 6);
}

inline NPerm2& NPerm2::operator = (const NPerm2& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

inline NPerm2 NPerm2::operator * (const NPerm2& q) const {
    return NPerm2(productTable[code_][q.code_]);
}

inline NPerm2 NPerm2::inverse() const {
    return NPerm2(static_cast<Code>(invS3[code_]));
}

inline int NPerm2::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline int NPerm2::operator[](int source) const {
    return imageTable[code_][source];
}

inline int NPerm2::preImageOf(int image) const {
    return imageTable[invS3[code_]][image];
}

inline bool NPerm2::operator == (const NPerm2& other) const {
    return (code_ == other.code_);
}

inline bool NPerm2::operator != (const NPerm2& other) const {
    return (code_ != other.code_);
}

inline int NPerm2::compareWith(const NPerm2& other) const {
    // Computing orderedS3Index() is very fast.
    // Use this instead of comparing images one at a time.
    int o1 = orderedS3Index();
    int o2 = other.orderedS3Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline bool NPerm2::isIdentity() const {
    return (code_ == 0);
}

inline std::string NPerm2::toString() const {
    return str();
}

inline int NPerm2::S3Index() const {
    return code_;
}

inline int NPerm2::SnIndex() const {
    return code_;
}

inline int NPerm2::orderedS3Index() const {
    if (code_ == 2 || code_ == 3)
        return code_ ^ 1;
    else
        return code_;
}

inline int NPerm2::orderedSnIndex() const {
    return orderedS3Index();
}

} // namespace regina

#endif

