
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

/*! \file maths/nperm3.h
 *  \brief Deals with permutations of {0,1,2}.
 */

#ifndef __NPERM3_H
#ifndef __DOXYGEN
#define __NPERM3_H
#endif

#include <string>

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
 * This class is faster and sleeker than related classes such as NPerm4.
 * On the other hand, this class does not offer quite as rich an interface.
 *
 * \testfull
 */
class NPerm3 {
    public:
        /**
         * Contains all possible permutations of three elements.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This array contains the same permutations in the same order
         * as the corresponding array NPerm4::S3 (though of course this
         * array stores NPerm3 objects instead of NPerm4 objects).
         *
         * Note that these permutations are not necessarily in
         * lexicographical order.
         */
        static const NPerm3 S3[6];

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

        enum {
            /**
             * The internal code for the permutation (0,1,2).
             */
            code012 = 0,
            /**
             * The internal code for the permutation (0,2,1).
             */
            code021 = 1,
            /**
             * The internal code for the permutation (1,2,0).
             */
            code120 = 2,
            /**
             * The internal code for the permutation (1,0,2).
             */
            code102 = 3,
            /**
             * The internal code for the permutation (2,0,1).
             */
            code201 = 4,
            /**
             * The internal code for the permutation (2,1,0).
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
inline std::ostream& operator << (std::ostream& out, const NPerm3& p) {
    return (out << p.toString());
}

/*@}*/

// Inline functions for NPerm3

inline NPerm3::NPerm3() : code_(0) {
}

inline NPerm3::NPerm3(unsigned char code) : code_(code) {
}

inline NPerm3::NPerm3(int a, int b, int) {
    switch (a) {
        case 0:
            code_ = (b == 1 ? 0 : 1); break;
        case 1:
            code_ = (b == 2 ? 2 : 3); break;
        case 2:
            code_ = (b == 0 ? 4 : 5); break;
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
    return NPerm3(invS3[code_]);
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

