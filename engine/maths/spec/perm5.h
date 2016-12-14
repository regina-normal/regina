
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file maths/spec/perm5.h
 *  \brief Internal header for permutations of {0,1,2,3,4}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __PERM5_H
#ifndef __DOXYGEN
#define __PERM5_H
#endif

#include <cstdlib>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1,2,3,4}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 4-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to
 * pass about by value instead of by reference.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<5>, the internal code follows
 * the general scheme used for Perm<n>: the lowest three bits represent
 * the image of 0, the next lowest three bits represent the image of 1 and so
 * on.  See the generic Perm template for further details.
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm5.
 */
template <>
class REGINA_API Perm<5> {
    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on five elements.  In other words, this is a
         * native signed integer type large enough to store (5!).
         */
        typedef int Index;

        /**
         * The total number of permutations on five elements.
         * This is the size of the array Sn.
         */
        static const Index nPerms = 120;

        /**
         * The total number of permutations on four elements.
         * This is the size of the array Sn_1.
         */
        static const Index nPerms_1 = 24;

        /**
         * Indicates the number of bits used by the permutation code to
         * store the image of a single integer.
         *
         * The full permutation code packs 5 such images together, and
         * so uses 5 * \a imageBits bits in total.
         */
        static const int imageBits = 3;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        typedef uint16_t Code;

        /**
         * Contains all possible permutations of five elements.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.
         */
        static const Perm<5> S5[120];

        /**
         * A dimension-agnostic alias for Perm<5>::S5.  In general, for
         * each \a K the class PermK will define an alias \a Sn
         * that references the list of all permutations PermK::SK.
         */
        static const Perm<5>* Sn;

        /**
         * Contains all possible permutations of five elements in
         * lexicographical order.
         */
        static const Perm<5> orderedS5[120];

        /**
         * A dimension-agnostic alias for Perm<5>::orderedS5.  In general, for
         * each \a K the class PermK will define an alias \a orderedSn
         * that references the list of all permutations PermK::orderedSK.
         */
        static const Perm<5>* orderedSn;

        /**
         * Contains the inverses of the permutations in the array \a S5.
         *
         * Specifically, the inverse of permutation <tt>S5[i]</tt> is
         * the permutation <tt>S5[ invS5[i] ]</tt>.
         */
        static const unsigned invS5[120];

        /**
         * A dimension-agnostic alias for Perm<5>::invS5.  In general, for
         * each \a K the class PermK will define an alias \a invSn
         * that references the list of all permutations PermK::invSK.
         */
        static const unsigned* invSn;

        /**
         * Contains all possible permutations of four elements.
         * In each permutation, 4 maps to 4.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (Perm<4>, Perm<5> and so on), the
         * S4 array stores the same permutations in the same order (but
         * of course using different data types).
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.  For the corresponding inverse array,
         * see Perm<4>::invS4.
         */
        static const Perm<5> S4[24];

        /**
         * A dimension-agnostic alias for Perm<5>::S4.  In general, for
         * each \a K the class PermK will define an alias \a Sn_1
         * that references the list of all permutations PermK::S(K-1).
         */
        static const Perm<5>* Sn_1;

        /**
         * Contains all possible permutations of four elements in
         * lexicographical order.  In each permutation, 4 maps to 4.
         */
        static const Perm<5> orderedS4[24];

        /**
         * Contains all possible permutations of three elements.
         * In each permutation, 3 maps to 3 and 4 maps to 4.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (Perm<4>, Perm<5> and so on), the
         * S3 array stores the same permutations in the same order (but
         * of course using different data types).
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.  For the corresponding inverse array,
         * see Perm<3>::invS3.
         */
        static const Perm<5> S3[6];

        /**
         * Contains all possible permutations of three elements in
         * lexicographical order.  In each permutation, 3 maps to 3 and
         * 4 maps to 4.
         */
        static const Perm<5> orderedS3[6];

        /**
         * Contains all possible permutations of two elements.
         * In each permutation, 2 maps to 2, 3 maps to 3, and 4 maps to 4.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * For all permutation classes (Perm<4>, Perm<5> and so on), the
         * S2 array stores the same permutations in the same order (but
         * of course using different data types).
         *
         * Note that these permutations are already in lexicographical order.
         */
        static const Perm<5> S2[2];

    private:
        unsigned code;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3,4}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        Perm(int a, int b);

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
        Perm(int a, int b, int c, int d, int e);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3,4.
         *
         * \pre The array \a image contains five elements, which are
         * 0, 1, 2, 3 and 4 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        Perm(const int* image);

        /**
         * Creates a permutation mapping (\a a[0], ..., \a a[4]) to
         * (\a b[0], ..., \a b[4]) respectively.
         *
         * \pre Both arrays \a a and \a b contain 5 elements, which
         * are 0,...,4 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length 5.
         * @param b the corresponding array of images; this must also have
         * length 5.
         */
        Perm(const int* a, const int* b);

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
        Perm(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1,
            int e0, int e1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        Perm(const Perm<5>& cloneMe);

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
        Code permCode() const;

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
        void setPermCode(Code newCode);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param newCode the internal code for the new permutation.
         * @return the permutation reprsented by the given internal code.
         */
        static Perm<5> fromPermCode(Code newCode);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(Code newCode);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<5>& operator = (const Perm<5>& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        Perm<5> operator * (const Perm<5>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        Perm<5> inverse() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,...,4.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[4 - i]</tt> for all \a i.
         */
        Perm<5> reverse() const;

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
        REGINA_INLINE_REQUIRED
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
        bool operator == (const Perm<5>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3 or 4.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const Perm<5>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const Perm<5>& other) const;

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
         * Returns the <i>i</i>th permutation on five elements, where
         * permutations are numbered lexicographically beginning at 0.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 5-tuple (\a p[0], \a p[1], \a p[2], \a p[3], \a p[4]).
         *
         * The return value will be identical to orderedS5[\a i].
         *
         * @param i the lexicographical index of the permutation; this
         * must be between 0 and 119 inclusive.
         * @return the <i>i</i>th permutation.
         */
        static Perm atIndex(Index i);

        /**
         * Returns the lexicographical index of this permutation.  This
         * indicates where this permutation sits within a full lexicographical
         * ordering of all 5! permutations on five elements.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 5-tuple (\a p[0], \a p[1], \a p[2], \a p[3], \a p[4]).
         * In particular, the identity permutation has index 0, and the
         * "reverse" permutation (which maps each \a i to 4-<i>i</i>)
         * has index 119 = 5!-1.
         *
         * This routine is identical to orderedS5Index().
         *
         * @return the index of this permutation, which will be between
         * 0 and 119 inclusive.
         */
        Index index() const;

        /**
         * Returns a random permutation on five elements.
         * All permutations are returned with equal probability.
         *
         * The implementation uses the C standard ::rand() function for its
         * random number generation.
         *
         * @return a random permutation.
         */
        static Perm rand();

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of five adjacent digits
         * representing the images of 0, 1, 2, 3 and 4 respectively.
         * An example of a string representation is <tt>30421</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 5 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

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

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,4,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,4 are exactly
         * <i>from</i>,...,4, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 5 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<5>::S5 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<5>::S5[i].  This will be between 0 and 119 inclusive.
         *
         * @author Ryan Budney
         */
        int S5Index() const;

        /**
         * Returns the index of this permutation in the Perm<5>::S5 array.
         * This is a dimension-agnostic alias for S5Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<5>::S5[i].  This will be between 0 and 119 inclusive.
         */
        int SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<5>::orderedS5 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<5>::orderedS5[i].  This will be between 0 and 119 inclusive.
         *
         * @author Ryan Budney
         */
        int orderedS5Index() const;

        /**
         * Returns the index of this permutation in the Perm<5>::orderedS5 array.
         * This is a dimension-agnostic alias for orderedS5Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<5>::orderedS5[i].  This will be between 0 and 119 inclusive.
         */
        int orderedSnIndex() const;

        /**
         * Extends a <i>k</i>-element permutation to a 5-element permutation,
         * where 2 &le; \a k &lt; 5.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,4 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2, 3 or 4.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation expressed as a permutation on
         * five elements.
         */
        template <int k>
        static Perm<5> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 5-element
         * permutation, where \a k &gt; 5.
         *
         * The resulting permutation will map 0,...,3 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[5],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,4 to 0,...,4 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 5.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 5 elements.
         */
        template <int k>
        static Perm<5> contract(Perm<k> p);

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
        Perm<5>(Code newCode);

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 4 inclusive.
         * @return the image of \a source.
         */
        REGINA_INLINE_REQUIRED
        int imageOf(int source) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPerm5 has now been renamed to Perm<5>.
 */
typedef Perm<5> NPerm5;

/*@}*/

// Inline functions for Perm<5>

inline Perm<5>::Perm() : code(18056) {
}

inline Perm<5>::Perm(Code newCode) : code(newCode) {
}

inline Perm<5>::Perm(int a, int b) {
    code = 18056;
    code += ((a << (3*b)) - (b << (3*b)));
    code += ((b << (3*a)) - (a << (3*a)));
}

inline Perm<5>::Perm(int a, int b, int c, int d, int e) {
    code = (e << 12) | (d << 9) | (c << 6) | (b << 3) | a;
}

inline Perm<5>::Perm(const int* image) {
    code = (image[4] << 12) | (image[3] << 9) | (image[2] << 6) |
        (image[1] << 3) | image[0];
}

inline Perm<5>::Perm(const int* a, const int* b) {
    code =
        (b[0] << (3*a[0])) |
        (b[1] << (3*a[1])) |
        (b[2] << (3*a[2])) |
        (b[3] << (3*a[3])) |
        (b[4] << (3*a[4]));
}

inline Perm<5>::Perm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1) {
    code =
        (a1 << (3*a0)) |
        (b1 << (3*b0)) |
        (c1 << (3*c0)) |
        (d1 << (3*d0)) |
        (e1 << (3*e0));
}

inline Perm<5>::Perm(const Perm<5>& cloneMe) : code(cloneMe.code) {
}

inline Perm<5>::Code Perm<5>::permCode() const {
    return code;
}

inline void Perm<5>::setPermCode(Code newCode) {
    code = newCode;
}

inline Perm<5> Perm<5>::fromPermCode(Code newCode) {
    return Perm<5>(newCode);
}

inline Perm<5>& Perm<5>::operator = (const Perm<5>& cloneMe) {
    code = cloneMe.code;
    return *this;
}

inline Perm<5> Perm<5>::operator *(const Perm<5>& q) const {
    return Perm<5>(imageOf(q[0]), imageOf(q[1]), imageOf(q[2]),
        imageOf(q[3]), imageOf(q[4]));
}

inline Perm<5> Perm<5>::inverse() const {
    // Specify the inverse by its internal code.
    return Perm<5>(static_cast<Code>(
        (1 << (3*imageOf(1))) |
        (2 << (3*imageOf(2))) |
        (3 << (3*imageOf(3))) |
        (4 << (3*imageOf(4)))));
}

inline Perm<5> Perm<5>::reverse() const {
    // Specify the inverse by its internal code.
    return Perm<5>(static_cast<Code>(
        (imageOf(4)) |
        (imageOf(3) << 3) |
        (imageOf(2) << 6) |
        (imageOf(1) << 9) |
        (imageOf(0) << 12)));
}

inline int Perm<5>::operator[](int source) const {
    return (code >> (3*source)) & 7;
}

inline int Perm<5>::preImageOf(int image) const {
    if (( code       & 7) == static_cast<Code>(image)) return 0;
    if (((code >> 3) & 7) == static_cast<Code>(image)) return 1;
    if (((code >> 6) & 7) == static_cast<Code>(image)) return 2;
    if (((code >> 9) & 7) == static_cast<Code>(image)) return 3;
    return 4;
}

inline bool Perm<5>::operator == (const Perm<5>& other) const {
    return (code == other.code);
}

inline bool Perm<5>::operator != (const Perm<5>& other) const {
    return (code != other.code);
}

inline bool Perm<5>::isIdentity() const {
    return (code == 18056);
}

inline Perm<5> Perm<5>::atIndex(Index i) {
    return orderedS5[i];
}

inline Perm<5>::Index Perm<5>::index() const {
    return orderedS5Index();
}

inline Perm<5> Perm<5>::rand() {
    return S5[::rand() % 120];
}

inline int Perm<5>::imageOf(int source) const {
    return (code >> (3*source)) & 7;
}

inline int Perm<5>::SnIndex() const {
    return S5Index();
}

inline int Perm<5>::orderedSnIndex() const {
    return orderedS5Index();
}

} // namespace regina

#endif

