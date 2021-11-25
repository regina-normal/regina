
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file maths/spec/perm3.h
 *  \brief Internal header for permutations of {0,1,2}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM3_H
#ifndef __DOXYGEN
#define __REGINA_PERM3_H
#endif

#include <cstdlib>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * Represents a permutation of {0,1,2}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 2-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.  Moreover, Perm<3> in particular is extremely
 * fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<3>, the internal code is an
 * integer between 0 and 5 inclusive that gives the index of the
 * permutation in the array Perm<3>::S3.  This is consistent with the
 * second-generation codes used in classes Perm<4>,...,Perm<7>.
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm3.
 *
 * \ingroup maths
 */
template <>
class Perm<3> {
    private:
        /**
         * An array-like object used to implement Perm<3>::S3.
         */
        struct S3Lookup {
            /**
             * Returns the permutation at the given index in the array S3.
             * See Perm<3>::S3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in S3.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<3>::orderedS3.
         */
        struct OrderedS3Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS3.  See Perm<3>::orderedS3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in orderedS3.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<3>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<3>::S2 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 1 inclusive.
             * @return the corresponding permutation in S2.
             */
            constexpr Perm<3> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on three elements.  In other words, this is a
         * native signed integer type large enough to store (3!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on three elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 6;

        /**
         * The total number of permutations on two elements.
         * This is the size of the array Sn_1.
         */
        static constexpr Index nPerms_1 = 2;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        using Code = uint8_t;

        /**
         * Gives array-like access to all possible permutations of
         * three elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 5 inclusive.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This is different from Perm<3>::orderedSn, since this array \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         */
        static constexpr S3Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * three elements.
         *
         * This is a dimension-specific alias for Perm<3>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<5>::S5 are only available for small \a n.
         *
         * Note that small permutation classes (Perm<3>, Perm<4>, Perm<5>)
         * have an \a S3 array: these all store the same six permutations in
         * the same order (but of course using different data types).
         */
        static constexpr S3Lookup S3 {};

        /**
         * Gives array-like access to all possible permutations of three
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 5 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], \a p[1], \a p[2]).
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * This is different from Perm<3>::Sn, since this array \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         */
        static constexpr OrderedS3Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of three
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<3>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<5>::orderedS5 are only
         * available for small \a n.
         */
        static constexpr OrderedS3Lookup orderedS3 {};

        /**
         * Gives array-like access to all possible permutations of
         * two elements.  In each permutation, 2 maps to 2.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn_1[i]</tt>.  The index \a i must be
         * between 0 and 1 inclusive.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         */
        static constexpr S2Lookup Sn_1 {};

        /**
         * Gives array-like access to all possible permutations of
         * two elements.
         *
         * This is a dimension-specific alias for Perm<3>::Sn_1; see that
         * member for further information.
         *
         * Note that all small permutation classes (Perm<2>, ..., Perm<5>)
         * have an \a S2 array: these all store the same two permutations in
         * the same order (but of course using different data types).
         *
         * There is no corresponding \a orderedS2 array, since the
         * (trivial) arrays \a S2 and \a orderedS2 are identical.
         */
        static constexpr S2Lookup S2 {};

        /**
         * The internal code for the permutation (0,1,2).
         */
        static constexpr Code code012 = 0;
        /**
         * The internal code for the permutation (0,2,1).
         */
        static constexpr Code code021 = 1;
        /**
         * The internal code for the permutation (1,2,0).
         */
        static constexpr Code code120 = 2;
        /**
         * The internal code for the permutation (1,0,2).
         */
        static constexpr Code code102 = 3;
        /**
         * The internal code for the permutation (2,0,1).
         */
        static constexpr Code code201 = 4;
        /**
         * The internal code for the permutation (2,1,0).
         */
        static constexpr Code code210 = 5;

    protected:
        Code code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        constexpr Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

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
        constexpr Perm(int a, int b, int c);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2.
         *
         * \pre The elements of \a image are 0, 1 and 2 in some order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 3>& image);

        /**
         * Deprecated constructor that creates a permutation mapping
         * \a i to \a image[i] for each \a i = 0,1,2.
         *
         * \deprecated Use the three-integer constructor or the
         * std::array constructor instead.
         *
         * \pre The array \a image contains three elements, which are
         * 0, 1 and 2 in some order.
         *
         * @param image the array of images.
         */
        [[deprecated]] constexpr Perm(const int* image);

        /**
         * Deprecated constructor that creates a permutation mapping
         * (\a a[0], ..., \a a[2]) to (\a b[0], ..., \a b[2]) respectively.
         *
         * \deprecated Use the six-integer constructor or the
         * std::array constructor instead.
         *
         * \pre Both arrays \a a and \a b contain 3 elements, which
         * are 0,...,2 in some order.
         *
         * \ifacespython Not present; use the single-array constructor instead.
         *
         * @param a the array of preimages; this must have length 3.
         * @param b the corresponding array of images; this must also have
         * length 3.
         */
        [[deprecated]] constexpr Perm(const int* a, const int* b);

        /**
         * Creates a permutation mapping (<i>a0</i>,<i>b0</i>,<i>c0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>) respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>} = {0,1,2}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<3>& cloneMe) = default;

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
        constexpr Code permCode() const;

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
        static constexpr Perm<3> fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static constexpr bool isPermCode(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<3>& operator = (const Perm<3>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        constexpr Perm<3> operator * (const Perm<3>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<3> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<3> pow(long exp) const;

        /**
         * Returns the order of this permutation.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * @return the order of this permutation.
         */
        constexpr int order() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,1,2.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[2 - i]</tt> for all \a i.
         */
        constexpr Perm<3> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        constexpr int sign() const;

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Deprecated routine that determines the preimage of the given
         * integer under this permutation.
         *
         * \deprecated This routine has been renamed to pre().
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 2 inclusive.
         * @return the preimage of \a image.
         */
        [[deprecated]] constexpr int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1 and 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<3>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1 or 2.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<3>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2) under this
         * and the given permutation.
         *
         * Note that this does \e not yield the same ordering of permutations
         * as used by the less-than and increment operators.  Moreover,
         * compareWith() is slower than the less-than operator to compute.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<3>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1 and 2 is mapped to itself.
         *
         * @return \c true if and only if this is the identity permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<3>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<3>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<3>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<3>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<3>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<3> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<3>::Sn.
         *
         * Note that this is \e not the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<3>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 3)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 2 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on three elements.
         * All permutations are returned with equal probability.
         *
         * This routine is thread-safe, and uses RandomEngine for its
         * random number generation.
         *
         * \warning This routine is expensive, since it locks and unlocks
         * the mutex protecting Regina's global uniform random bit generator.
         * If you are calling this many times in quick succession, consider
         * creating a single RandomEngine object yourself and then calling
         * <tt>rand(randomEngine.engine(), even)</tt>.
         *
         * @param even if \c true, then the resulting permutation is
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * @return a random permutation.
         */
        static Perm rand(bool even = false);

        /**
         * Returns a random permutation on three elements, using the
         * given uniform random bit generator.
         * All permutations are returned with equal probability.
         *
         * The thread safety of this routine is of course dependent on
         * the thread safety of your uniform random bit generator \a gen.
         *
         * \tparam URBG A type which, once any references are removed, must
         * adhere to the C++ \a UniformRandomBitGenerator concept.
         *
         * \ifacespython Not present, though the non-thread-safe variant
         * without the \a gen argument is available.
         *
         * @param gen the source of randomness to use (e.g., one of the
         * many options provided in the C++ standard \c random header).
         * @param even if \c true, then the resulting permutation is
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * @return a random permutation.
         */
        template <class URBG>
        static Perm rand(URBG&& gen, bool even = false);

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
         * Returns a string representation of this permutation with only
         * the images of 0 and 1.  The resulting string will therefore
         * have length two.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc2() const;

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,2,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,2 are exactly
         * <i>from</i>,...,2, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 3 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<3>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::Sn[i].  This will be between 0 and 5 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<3>::S3 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<5>::S5Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<3>::S3[i].  This will be between 0 and 5 inclusive.
         */
        constexpr Index S3Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<3>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 5 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<3>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         * In general, for every \a n there will be a member function
         * Perm<n>::orderedSnIndex(); however, these numerical aliases
         * Perm<2>::orderedS2Index(), ..., Perm<5>::orderedS5Index() are
         * only available for small \a n.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 5 inclusive.
         */
        constexpr Index orderedS3Index() const;

        /**
         * Deprecated routine that returns the lexicographical index of this
         * permutation.
         *
         * \deprecated Use the equivalent routine orderedSnIndex() instead.
         *
         * @return the lexicographical index of this permutation.
         */
        [[deprecated]] constexpr Index index() const;

        /**
         * Deprecated routine that returns the <i>i</i>th permutation on
         * three elements, where permutations are numbered lexicographically.
         *
         * \deprecated Use orderedSn[\a i] instead.
         *
         * @param i the lexicographical index of the permutation; this
         * must be between 0 and 5 inclusive.
         * @return the <i>i</i>th permutation.
         */
        [[deprecated]] static constexpr Perm atIndex(Index i);

        /**
         * Extends a <i>k</i>-element permutation to an 3-element permutation.
         * where 2 &le; \a k &lt; 3.  The only possible value of \a k is 2, but
         * this routine is kept as a template function for consistency
         * with the other classes' Perm<n>::extend() routines.
         *
         * The resulting permutation will map 0,1 to their respective images
         * under \a p, and will map the "unused" element 3 to itself.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be exactly 2.
         *
         * @param p a permutation on two elements.
         * @return the same permutation expressed as a permutation on
         * three elements.
         */
        template <int k>
        static constexpr Perm<3> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 3-element
         * permutation, where \a k &gt; 3.
         *
         * The resulting permutation will map 0,1,2 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[3],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,1,2 to 0,1,2 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 3.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 3 elements.
         */
        template <int k>
        static constexpr Perm<3> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<3>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<3>, since it essentially
         * uses a hard-coded lookup table.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S3[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static constexpr Code imageTable[6][3] = {
            { 0, 1, 2 },
            { 0, 2, 1 },
            { 1, 2, 0 },
            { 1, 0, 2 },
            { 2, 0, 1 },
            { 2, 1, 0 }
        };

        /**
         * Contains the inverses of the permutations in the array \a S3.
         *
         * Specifically, the inverse of permutation <tt>S3[i]</tt> is
         * the permutation <tt>S3[ invS3[i] ]</tt>.
         */
        static constexpr Code invS3[6] = {
            0, 1, 4, 3, 2, 5
        };

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S3[x] * S3[y]</tt> is
         * the permutation <tt>S3[product[x][y]]</tt>.
         */
        static constexpr Code productTable[6][6] = {
            { 0, 1, 2, 3, 4, 5 },
            { 1, 0, 5, 4, 3, 2 },
            { 2, 3, 4, 5, 0, 1 },
            { 3, 2, 1, 0, 5, 4 },
            { 4, 5, 0, 1, 2, 3 },
            { 5, 4, 3, 2, 1, 0 }
        };

        /**
         * Contains the orders of the permutations in the array \a S3.
         */
        static constexpr int orderTable[6] = {
            1, 2, 3, 2, 3, 2
        };

    protected:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code from which the new
         * permutation will be created.
         */
        constexpr Perm<3>(Code code);

    private:
        /**
         * Converts between an index into Perm<3>::S3 and an index into
         * Perm<3>::orderedS3.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code.
         */
        template <typename Int>
        static constexpr Int convOrderedUnordered(Int index);
};

// Inline functions for Perm<3>

template <typename Int>
inline constexpr Int Perm<3>::convOrderedUnordered(Int index) {
    // S5 is almost the same as orderedS5, except that we
    // swap indices 2 <--> 3.
    return ((index == 2 || index == 3) ? (index ^ 1) : index);
}

inline constexpr Perm<3> Perm<3>::S3Lookup::operator[] (int index) const {
    return Perm<3>(static_cast<Code>(index));
}

inline constexpr Perm<3> Perm<3>::OrderedS3Lookup::operator[] (int index)
        const {
    return Perm<3>(static_cast<Code>(convOrderedUnordered(index)));
}

inline constexpr Perm<3> Perm<3>::S2Lookup::operator[] (int index) const {
    return Perm<3>(index == 0 ? code012 : code102);
}

inline constexpr Perm<3>::Perm() : code_(0) {
}

inline constexpr Perm<3>::Perm(Code code) : code_(code) {
}

inline constexpr Perm<3>::Perm(int a, int b) :
        code_(a == b ? code012 :
              a == 0 ? (b == 1 ? code102 : code210) :
              a == 1 ? (b == 0 ? code102 : code021) :
                       (b == 0 ? code210 : code021)) {
}

inline constexpr Perm<3>::Perm(int a, int b, int) :
        code_(a == 0 ? (b == 1 ? 0 : 1) :
              a == 1 ? (b == 2 ? 2 : 3) :
                       (b == 0 ? 4 : 5)) {
}

inline constexpr Perm<3>::Perm(const std::array<int, 3>& image) :
        code_(image[0] == 0 ? (image[1] == 1 ? 0 : 1) :
              image[0] == 1 ? (image[1] == 2 ? 2 : 3) :
                              (image[1] == 0 ? 4 : 5)) {
}

inline constexpr Perm<3>::Perm(const int* image) :
        code_(image[0] == 0 ? (image[1] == 1 ? 0 : 1) :
              image[0] == 1 ? (image[1] == 2 ? 2 : 3) :
                              (image[1] == 0 ? 4 : 5)) {
}

inline constexpr Perm<3>::Perm(const int* a, const int* b) : code_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[3] = { 0, 0, 0 };
    image[a[0]] = b[0];
    image[a[1]] = b[1];
    image[a[2]] = b[2];

    switch (image[0]) {
        case 0:
            code_ = static_cast<Code>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(image[1] == 0 ? 4 : 5); break;
    }
}

inline constexpr Perm<3>::Perm(int a0, int a1, int b0, int b1, int c0, int c1) :
        code_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[3] = { 0, 0, 0 };
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;

    switch (image[0]) {
        case 0:
            code_ = static_cast<Code>(image[1] == 1 ? 0 : 1); break;
        case 1:
            code_ = static_cast<Code>(image[1] == 2 ? 2 : 3); break;
        case 2:
            code_ = static_cast<Code>(image[1] == 0 ? 4 : 5); break;
    }
}

inline constexpr Perm<3>::Code Perm<3>::permCode() const {
    return code_;
}

inline void Perm<3>::setPermCode(Code code) {
    code_ = code;
}

inline constexpr Perm<3> Perm<3>::fromPermCode(Code code) {
    return Perm<3>(code);
}

inline constexpr bool Perm<3>::isPermCode(Code code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 6);
}

inline constexpr Perm<3> Perm<3>::operator * (const Perm<3>& q) const {
    return Perm<3>(productTable[code_][q.code_]);
}

inline constexpr Perm<3> Perm<3>::inverse() const {
    return Perm<3>(invS3[code_]);
}

inline constexpr Perm<3> Perm<3>::pow(long exp) const {
    if (code_ & 1) {
        // This is a pair swap.
        return (exp & 1 ? *this : Perm<3>());
    } else if (code_ == 0) {
        // This is the identity.
        return Perm<3>();
    } else {
        // This is a 3-cycle.
        switch (exp % 3) {
            case 0:
                return Perm<3>();
            case 1: case -2:
                return *this;
            default:
                return Perm<3>(code_ ^ 6 /* swaps 2 <-> 4 */);
        }
    }
}

inline constexpr int Perm<3>::order() const {
    return orderTable[code_];
}

inline constexpr Perm<3> Perm<3>::reverse() const {
    // p becomes p * 210.
    return Perm<3>(productTable[code_][code210]);
}

inline constexpr int Perm<3>::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline constexpr int Perm<3>::operator[](int source) const {
    return imageTable[code_][source];
}

inline constexpr int Perm<3>::pre(int image) const {
    return imageTable[invS3[code_]][image];
}

inline constexpr int Perm<3>::preImageOf(int image) const {
    return imageTable[invS3[code_]][image];
}

inline constexpr bool Perm<3>::operator == (const Perm<3>& other) const {
    return (code_ == other.code_);
}

inline constexpr bool Perm<3>::operator != (const Perm<3>& other) const {
    return (code_ != other.code_);
}

inline constexpr int Perm<3>::compareWith(const Perm<3>& other) const {
    // Computing orderedS3Index() is very fast.
    // Use this instead of comparing images one at a time.
    int o1 = orderedS3Index();
    int o2 = other.orderedS3Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<3>::isIdentity() const {
    return (code_ == 0);
}

inline Perm<3>& Perm<3>::operator ++() {
    if (++code_ == 6)
        code_ = 0;
    return *this;
}

inline constexpr Perm<3> Perm<3>::operator ++(int) {
    Perm<3> ans(code_);
    if (++code_ == 6)
        code_ = 0;
    return ans;
}

inline constexpr bool Perm<3>::operator < (const Perm<3>& rhs) const {
    return code_ < rhs.code_;
}

inline constexpr Perm<3> Perm<3>::rot(int i) {
    switch (i) {
        case 1: return Perm<3>(static_cast<Code>(code120));
        case 2: return Perm<3>(static_cast<Code>(code201));
        default: return Perm<3>(); // Identity
    }
}

inline Perm<3> Perm<3>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<3> Perm<3>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 2);
        return S3[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 5);
        return S3[d(gen)];
    }
}

inline constexpr Perm<3>::Index Perm<3>::S3Index() const {
    return code_;
}

inline constexpr Perm<3>::Index Perm<3>::SnIndex() const {
    return code_;
}

inline constexpr Perm<3>::Index Perm<3>::orderedS3Index() const {
    return convOrderedUnordered(code_);
}

inline constexpr Perm<3>::Index Perm<3>::orderedSnIndex() const {
    return convOrderedUnordered(code_);
}

inline constexpr Perm<3>::Index Perm<3>::index() const {
    return convOrderedUnordered(code_);
}

inline constexpr Perm<3> Perm<3>::atIndex(Index i) {
    return orderedS3[i];
}

inline constexpr bool Perm<3>::isConjugacyMinimal() const {
    return (code_ < 3);
}

} // namespace regina

#endif

