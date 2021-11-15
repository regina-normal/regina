
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

/*! \file maths/spec/perm2.h
 *  \brief Internal header for permutations of {0,1}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM2_H
#ifndef __DOXYGEN
#define __REGINA_PERM2_H
#endif

#include <cstdlib>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * Represents a permutation of {0,1}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, but also somewhat trivial (since there are only two
 * possible permutations).  It is provided simply to optimise the general
 * Perm<n> template for this trivial case.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.  Moreover, Perm<2> in particular is extremely
 * fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<2>, the internal code is 0 for
 * the identity permutation, or 1 for the (unique) non-identity permutation.
 * This is consistent with the second-generation codes used in classes
 * Perm<4>,...,Perm<7>.
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \warning Every permutation class Perm<n> provides a transposition
 * (i.e., pair swap) constructor <tt>Perm<n>(a,b).</tt>  In addition, the
 * specialised classes Perm<3>, Perm<4> and Perm<5> provide "list of images"
 * constructors <tt>Perm<3>(a,b,c)</tt>, <tt>Perm<4>(a,b,c,d)</tt> and
 * <tt>Perm<5>(a,b,c,d,e)</tt>.  For Perm<2>, these two constructors would
 * be indistinguishable (since both would take two integer arguments).
 * Here Perm<2> takes an approach that is consistent with the generic Perm<n>
 * class: <tt>Perm<2>(a,b)</tt> is interpreted as the \e transposition of
 * \a a and \a b.  In particular, <tt>Perm(0,1)</tt> is \e not the
 * identity permutation.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm2.
 *
 * \ingroup maths
 */
template <>
class Perm<2> {
    private:
        /**
         * An array-like object used to implement Perm<2>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<2>::S2 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 1 inclusive.
             * @return the corresponding permutation in S2.
             */
            constexpr Perm<2> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on two elements.  In other words, this is a
         * native signed integer type large enough to store (2!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on two elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 2;

        /**
         * The total number of permutations on one element.
         * This is the size of the array Sn_1.
         */
        static constexpr Index nPerms_1 = 1;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        using Code = uint8_t;

        /**
         * Gives array-like access to all possible permutations of
         * two elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 1 inclusive.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The identity permutation has index 0, and the non-identity
         * permutation has index 1.  As a result, Sn[\a i] is an even
         * permutation if and only if \a i is even.
         *
         * This ordered array is identical to Perm<2>::orderedSn.
         * Note however that for \a n &ge; 3, the arrays Perm<n>::Sn and
         * Perm<n>::orderedSn are different: \a Sn alternates between even
         * and odd permutations, whereas \a orderedSn stores permutations in
         * lexicographical order.
         */
        static constexpr S2Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * two elements.
         *
         * This is a dimension-specific alias for Perm<2>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<5>::S5 are only available for small \a n.
         *
         * Note that all small permutation classes (Perm<2>, ..., Perm<5>)
         * have an \a S2 array: these all store the same two permutations in
         * the same order (but of course using different data types).
         */
        static constexpr S2Lookup S2 {};

        /**
         * Gives array-like access to all possible permutations of two
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 1 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], \a p[1]).
         * Therefore the identity permutation has index 0, and the
         * (unique) non-identity permutation has index 1.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * This ordered array is identical to Perm<2>::Sn.
         * Note however that for \a n &ge; 3, the arrays Perm<n>::Sn and
         * Perm<n>::orderedSn are different: \a Sn alternates between even
         * and odd permutations, whereas \a orderedSn stores permutations in
         * lexicographical order.
         */
        static constexpr S2Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of two
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<2>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<5>::orderedS5 are only
         * available for small \a n.
         */
        static constexpr S2Lookup orderedS2 {};

        /**
         * Gives array-like access to all possible permutations of one element.
         *
         * Of course, this array is trivial: it contains just the identity
         * permutation.  This array is provided for consistency with
         * larger permutation classes Perm<n>.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn_1[i]</tt>.  The index \a i must be 0.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S2Lookup Sn_1 {};

        /**
         * Gives array-like access to all possible permutations of one element.
         *
         * This is a dimension-specific alias for Perm<2>::Sn_1; see that
         * member for further information.
         */
        static constexpr S2Lookup S1 {};

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
         * \pre \a a and \a b are in {0,1}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1.
         *
         * \pre The elements of \a image are 0 and 1 in some order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 2>& image);

        /**
         * Deprecated constructor that creates a permutation mapping
         * \a i to \a image[i] for each \a i = 0,1.
         *
         * \deprecated Use the two-integer constructor or the
         * std::array constructor instead.
         *
         * \pre The array \a image contains two elements, which are
         * 0 and 1 in some order.
         *
         * @param image the array of images.
         */
        [[deprecated]] constexpr Perm(const int* image);

        /**
         * Deprecated constructor that creates a permutation mapping
         * (\a a[0], \a a[1]) to (\a b[0], \a b[1]) respectively.
         *
         * \deprecated Use the four-integer constructor or the
         * std::array constructor instead.
         *
         * \pre Both arrays \a a and \a b contain two elements, which
         * are 0 and 1 in some order.
         *
         * \ifacespython Not present; use the single-array constructor instead.
         *
         * @param a the array of preimages; this must have length 2.
         * @param b the corresponding array of images; this must also have
         * length 2.
         */
        [[deprecated]] constexpr Perm(const int* a, const int* b);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<2>& cloneMe) = default;

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
        static constexpr Perm<2> fromPermCode(Code code);

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
        Perm<2>& operator = (const Perm<2>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        constexpr Perm<2> operator * (const Perm<2>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<2> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<2> pow(long exp) const;

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
         * Here \e reverse means that we reverse the images of 0 and 1.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[1 - i]</tt> for all \a i.
         */
        constexpr Perm<2> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        constexpr int sign() const;

        /**
         * Determines the image of the given integer under this permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be 0 or 1.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be 0 or 1.
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
         * should be 0 or 1.
         * @return the preimage of \a image.
         */
        [[deprecated]] constexpr int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0 and 1.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<2>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0 or 1.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<2>& other) const;

        /**
         * Lexicographically compares the images of (0,1) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<2>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0 and 1 is mapped to itself.
         *
         * @return \c true if and only if this is the identity permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<2>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<2>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<2>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<2>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<2>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<2> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<2>::Sn.
         *
         * For the special case of permutations on two elements, this
         * ordering is consistent with the ordering implied by compareWith()
         * (but beware: for other permutation classes this is not true).
         * Also, like all permutation classes, this ordering is consistent
         * with the ordering implied by the ++ operators.
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<2>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 2)
         * for all \a k.
         *
         * @param i the image of 0; this must be 0 or 1.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on two elements.
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
         * guaranteed to be even (which means, for a permutation on two
         * elements, the resulting permutation must be the identity).
         * @return a random permutation.
         */
        static Perm rand(bool even = false);

        /**
         * Returns a random permutation on two elements, using the
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
         * guaranteed to be even (which means, for a permutation on two
         * elements, the resulting permutation must be the identity).
         * @return a random permutation.
         */
        template <class URBG>
        static Perm rand(URBG&& gen, bool even = false);

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of two adjacent digits
         * representing the images of 0 and 1 respectively.  An
         * example of a string representation is <tt>10</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 2 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,1,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,1 are exactly
         * <i>from</i>,...,1, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 2 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<2>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::Sn[i].  This will be 0 or 1.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<2>::S2 array.
         *
         * This is a dimension-specific alias for SnIndex().
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::S2[i].  This will be 0 or 1.
         */
        constexpr Index S2Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<2>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be 0 or 1.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<2>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be 0 or 1.
         */
        constexpr Index orderedS2Index() const;

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
         * two elements, where permutations are numbered lexicographically.
         *
         * \deprecated Use orderedSn[\a i] instead.
         *
         * @param i the lexicographical index of the permutation; this
         * must be 0 or 1.
         * @return the <i>i</i>th permutation.
         */
        [[deprecated]] static constexpr Perm atIndex(Index i);

        /**
         * Restricts a <i>k</i>-element permutation to an 2-element
         * permutation, where \a k &gt; 2.
         *
         * The resulting permutation will map 0,1 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[2],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,1 to 0,1 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 2.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 2 elements.
         */
        template <int k>
        static constexpr Perm<2> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<2>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<2>, since the answer
         * is always \c true.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

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
        constexpr Perm<2>(Code code);
};

// Inline functions for Perm<2>

inline constexpr Perm<2> Perm<2>::S2Lookup::operator[] (int index) const {
    return Perm<2>(static_cast<Code>(index));
}

inline constexpr Perm<2>::Perm() : code_(0) {
}

inline constexpr Perm<2>::Perm(Code code) : code_(code) {
}

inline constexpr Perm<2>::Perm(int a, int b) : code_(a == b ? 0 : 1) {
}

inline constexpr Perm<2>::Perm(const std::array<int, 2>& image) :
        code_(image[0]) {
}

inline constexpr Perm<2>::Perm(const int* image) : code_(image[0]) {
}

inline constexpr Perm<2>::Perm(const int* a, const int* b) :
        code_(a[0] == b[0] ? 0 : 1) {
}

inline constexpr Perm<2>::Code Perm<2>::permCode() const {
    return code_;
}

inline void Perm<2>::setPermCode(Code code) {
    code_ = code;
}

inline constexpr Perm<2> Perm<2>::fromPermCode(Code code) {
    return Perm<2>(code);
}

inline constexpr bool Perm<2>::isPermCode(Code code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 2);
}

inline constexpr Perm<2> Perm<2>::operator * (const Perm<2>& q) const {
    return Perm<2>(code_ ^ q.code_);
}

inline constexpr Perm<2> Perm<2>::inverse() const {
    return *this;
}

inline constexpr Perm<2> Perm<2>::pow(long exp) const {
    return (exp & 1 ? *this : Perm<2>());
}

inline constexpr int Perm<2>::order() const {
    return (code_ + 1);
}

inline constexpr Perm<2> Perm<2>::reverse() const {
    return Perm<2>(static_cast<Code>(1 - code_));
}

inline constexpr int Perm<2>::sign() const {
    return (code_ ? -1 : 1);
}

inline constexpr int Perm<2>::operator[](int source) const {
    return source ^ code_;
}

inline constexpr int Perm<2>::pre(int image) const {
    return image ^ code_;
}

inline constexpr int Perm<2>::preImageOf(int image) const {
    return image ^ code_;
}

inline constexpr bool Perm<2>::operator == (const Perm<2>& other) const {
    return (code_ == other.code_);
}

inline constexpr bool Perm<2>::operator != (const Perm<2>& other) const {
    return (code_ != other.code_);
}

inline constexpr int Perm<2>::compareWith(const Perm<2>& other) const {
    // For n=2, permutation codes respect lexicographical order.
    return (code_ == other.code_ ? 0 : code_ < other.code_ ? -1 : 1);
}

inline constexpr bool Perm<2>::isIdentity() const {
    return (code_ == 0);
}

inline Perm<2>& Perm<2>::operator ++() {
    code_ ^= 1;
    return *this;
}

inline constexpr Perm<2> Perm<2>::operator ++(int) {
    Perm<2> ans(code_);
    code_ ^= 1;
    return ans;
}

inline constexpr bool Perm<2>::operator < (const Perm<2>& rhs) const {
    return code_ < rhs.code_;
}

inline constexpr Perm<2> Perm<2>::rot(int i) {
    return Perm<2>(static_cast<Code>(i));
}

inline Perm<2> Perm<2>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<2> Perm<2>::rand(URBG&& gen, bool even) {
    if (even)
        return Perm<2>();
    else {
        std::uniform_int_distribution<short> d(0, 1);
        return Perm<2>(static_cast<Code>(d(gen)));
    }
}

inline std::string Perm<2>::str() const {
    return (code_ == 0 ? "01" : "10");
}

inline std::string Perm<2>::trunc(unsigned len) const {
    switch (len) {
        case 2 : return (code_ == 0 ? "01" : "10");
        case 1 : return (code_ == 0 ? "0" : "1");
        default : return std::string();
    }
}

inline constexpr Perm<2>::Index Perm<2>::S2Index() const {
    return code_;
}

inline constexpr Perm<2>::Index Perm<2>::SnIndex() const {
    return code_;
}

inline constexpr Perm<2>::Index Perm<2>::orderedS2Index() const {
    return code_;
}

inline constexpr Perm<2>::Index Perm<2>::orderedSnIndex() const {
    return code_;
}

inline constexpr Perm<2>::Index Perm<2>::index() const {
    return code_;
}

inline constexpr Perm<2> Perm<2>::atIndex(Index i) {
    return Perm<2>(static_cast<Code>(i));
}

inline constexpr bool Perm<2>::isConjugacyMinimal() const {
    // All (i.e., both) permutations are minimal.
    return true;
}

} // namespace regina

#endif

