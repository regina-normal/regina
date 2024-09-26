
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
// This specialisation header will be re-included within perm.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM2_H
#ifndef __DOXYGEN
#define __REGINA_PERM2_H
#endif

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
 * (i.e., pair swap) constructor `Perm<n>(a,b).`  In addition, the
 * specialised classes Perm<3>, Perm<4> and Perm<5> provide "list of images"
 * constructors `Perm<3>(a,b,c)`, `Perm<4>(a,b,c,d)` and
 * `Perm<5>(a,b,c,d,e)`.  For Perm<2>, these two constructors would
 * be indistinguishable (since both would take two integer arguments).
 * Here Perm<2> takes an approach that is consistent with the generic Perm<n>
 * class: `Perm<2>(a,b)` is interpreted as the _transposition_ of
 * \a a and \a b.  In particular, `Perm(0,1)` is _not_ the
 * identity permutation.
 *
 * \python Since Python does not support templates, this class is
 * made available under the name Perm2.
 *
 * \ingroup maths
 */
template <>
class Perm<2> {
    public:
        /**
         * The degree of the underlying symmetric group; that is, the
         * template parameter \a n.
         *
         * This compile-time constant allows the programmer to extract \a n
         * from the type (e.g., when writing templated code).
         */
        static constexpr int degree = 2;

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
        static constexpr PermCodeType codeType = PermCodeType::Index;

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

    private:
        /**
         * A lightweight array-like object used to implement Perm<2>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<2>::S2 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 1 inclusive.
             * \return the corresponding permutation in S2.
             */
            constexpr Perm<2> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S2.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 2; }
        };

        /**
         * A lightweight array-like object used to implement Perm<2>::S1.
         */
        struct S1Lookup {
            /**
             * Returns the permutation at the given index in the array S1.
             * See Perm<2>::S1 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index; the only allowed value is 0.
             * \return the corresponding permutation in S1.
             */
            constexpr Perm<2> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S1.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 1; }
        };

    public:
        /**
         * Gives fast array-like access to all possible permutations of
         * two elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `Sn[i]`.  The index \a i must be
         * between 0 and 1 inclusive.
         * This element access is extremely fast (a fact that is not true for
         * the larger permutation classes Perm<n> with \a n ≥ 8).
         *
         * The identity permutation has index 0, and the non-identity
         * permutation has index 1.  As a result, Sn[\a i] is an even
         * permutation if and only if \a i is even.
         *
         * This ordered array is identical to Perm<2>::orderedSn.
         * Note however that for \a n ≥ 3, the arrays Perm<n>::Sn and
         * Perm<n>::orderedSn are different: \a Sn alternates between even
         * and odd permutations, whereas \a orderedSn stores permutations in
         * lexicographical order.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S2Lookup Sn {};

        /**
         * Gives fast array-like access to all possible permutations of
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
         * Gives fast array-like access to all possible permutations of two
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `orderedSn[i]`.  The index \a i
         * must be between 0 and 1 inclusive.
         * This element access is extremely fast (a fact that is not true for
         * the larger permutation classes Perm<n> with \a n ≥ 8).
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], \a p[1]).
         * Therefore the identity permutation has index 0, and the
         * (unique) non-identity permutation has index 1.
         *
         * This ordered array is identical to Perm<2>::Sn.
         * Note however that for \a n ≥ 3, the arrays Perm<n>::Sn and
         * Perm<n>::orderedSn are different: \a Sn alternates between even
         * and odd permutations, whereas \a orderedSn stores permutations in
         * lexicographical order.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S2Lookup orderedSn {};

        /**
         * Gives fast array-like access to all possible permutations of two
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
         * Gives fast array-like access to all possible permutations of
         * one element.
         *
         * Of course, this array is trivial: it contains just the identity
         * permutation.  This array is provided for consistency with
         * larger permutation classes Perm<n>.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `Sn_1[i]`.  The index \a i must be 0.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S1Lookup Sn_1 {};

        /**
         * Gives fast array-like access to all possible permutations of
         * one element.
         *
         * This is a dimension-specific alias for Perm<2>::Sn_1; see that
         * member for further information.
         */
        static constexpr S1Lookup S1 {};

    protected:
        Code code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * A do-nothing routine that assists with writing generic code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations, and so this precompute() function does nothing.
         * The only point of having precompute() in Perm<2> is to make it
         * easier to write generic code that works with Perm<n> for any \a n.
         *
         * - If you know you are only working with Perm<2>, you do not need to
         *   call this function at all.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using any of the optimised `cachedXXX()` functions,
         *   such as cachedComp(), cachedInverse(), and so on.
         *
         * All Perm<n>::precompute() routines are thread-safe, and are
         * harmless if called multiple times (since any call after the
         * first will do nothing).
         */
        static constexpr void precompute();

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
         * \param a the element to switch with \a b.
         * \param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1.
         *
         * \pre The elements of \a image are 0 and 1 in some order.
         *
         * \param image the array of images.
         */
        constexpr Perm(const std::array<int, 2>& image);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * \param cloneMe the permutation to clone.
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
         * \return the internal code.
         */
        constexpr Code permCode() const;

        /**
         * Sets this permutation to that represented by the given
         * internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * \param code the internal code that will determine the
         * new value of this permutation.
         */
        void setPermCode(Code code);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * \param code the internal code for the new permutation.
         * \return the permutation represented by the given internal code.
         */
        static constexpr Perm<2> fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * \param code the permutation code to test.
         * \return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static constexpr bool isPermCode(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * \param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * \return a reference to this permutation.
         */
        Perm<2>& operator = (const Perm<2>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is \a p, the
         * resulting permutation will be <i>p</i>∘<i>q</i>, and will satisfy
         * `(p*q)[x] == p[q[x]]`.
         *
         * \param q the permutation with which to compose this.
         * \return the composition of both permutations.
         */
        constexpr Perm<2> operator * (const Perm<2>& q) const;

        /**
         * An alias for the composition operator, provided to assist with
         * writing generic code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations.  The only point of having cachedComp() in Perm<2>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<2>, you should just
         *   use the composition operator instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedComp().  (For Perm<2>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * The permutation that is returned is the same as you would
         * obtain by calling `(*this) * q`.
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedComp().  For Perm<2>,
         * precompute() does nothing; however, for other Perm<n> classes
         * a failure to do this will almost certainly crash your program.
         *
         * \param q the permutation to compose this with.
         * \return the composition of both permutations.
         */
        Perm<2> cachedComp(const Perm<2>& q) const;

        /**
         * Deprecated alias for using the composition operator twice, provided
         * to assist with writing generic code.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling `(*this) * q * r`.
         *
         * \deprecated The three-way cachedComp() was originally written to
         * support conjugation.  If you are indeed conjugating, then call
         * cachedConjugate() instead; otherwise just call the two-way
         * cachedComp() twice.
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedComp().  For Perm<2>,
         * precompute() does nothing; however, for other Perm<n> classes
         * a failure to do this will almost certainly crash your program.
         *
         * \param q the first permutation to compose this with.
         * \param r the second permutation to compose this with.
         * \return the composition of both permutations.
         */
        [[deprecated]] Perm<2> cachedComp(const Perm<2>& q, const Perm<2>& r)
            const;

        /**
         * Computes the conjugate of this permutation by \a q.
         *
         * Specifically, calling `p.conjugate(q)` is equivalent to computing
         * `q * p * q.inverse()`.  The resulting permutation will have the
         * same cycle structure as \a p, but with the cycle elements
         * translated according to \a q.
         *
         * \param q the permutation to conjugate this by.
         * \return the conjugate of this permutation by \a q.
         */
        constexpr Perm<2> conjugate(const Perm<2>& q) const;

        /**
         * An alias for conjugate(), provided to assist with writing generic
         * code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations.  The only point of having cachedConjugate() in
         * Perm<2> is to make it easier to write generic code that works with
         * Perm<n> for any \a n.
         *
         * - If you know you are only working with Perm<2>, you should just
         *   call conjugate() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedConjugate().  (For Perm<2>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedConjugate().  For
         * Perm<6>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \param q the permutation to conjugate this by.
         * \return the conjugate of this permutation by \a q.
         */
        Perm<2> cachedConjugate(const Perm<2>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * \return the inverse of this permutation.
         */
        constexpr Perm<2> inverse() const;

        /**
         * An alias for inverse(), provided to assist with writing
         * generic code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations.  The only point of having cachedInverse() in Perm<2>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<2>, you should just
         *   call inverse() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedInverse().  (For Perm<2>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedInverse().  For
         * Perm<2>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \return the inverse of this permutation.
         */
        Perm<2> cachedInverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        constexpr Perm<2> pow(long exp) const;

        /**
         * An alias for pow(), provided to assist with writing generic code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations.  The only point of having cachedPow() in Perm<2>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<2>, you should just
         *   call pow() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedPow().  (For Perm<2>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedPow().  For
         * Perm<2>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        Perm<2> cachedPow(long exp) const;

        /**
         * Returns the order of this permutation.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * \return the order of this permutation.
         */
        constexpr int order() const;

        /**
         * An alias for order(), provided to assist with writing generic code.
         *
         * This specialised Perm<2> class does not use precomputation for its
         * optimisations.  The only point of having cachedOrder() in Perm<2>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<2>, you should just
         *   call order() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedOrder().  (For Perm<2>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedOrder().  For
         * Perm<2>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \return the order of this permutation.
         */
        int cachedOrder() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here _reverse_ means that we reverse the images of 0 and 1.
         * In other words, if permutation \a q is the
         * reverse of \a p, then `p[i] == q[1 - i]` for all \a i.
         */
        constexpr Perm<2> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * \return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        constexpr int sign() const;

        /**
         * Determines the image of the given integer under this permutation.
         *
         * \param source the integer whose image we wish to find.  This
         * should be 0 or 1.
         * \return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this permutation.
         *
         * \param image the integer whose preimage we wish to find.  This
         * should be 0 or 1.
         * \return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0 and 1.
         *
         * \param other the permutation with which to compare this.
         * \return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<2>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0 or 1.
         *
         * \param other the permutation with which to compare this.
         * \return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<2>& other) const;

        /**
         * Lexicographically compares the images of (0,1) under this
         * and the given permutation.
         *
         * \param other the permutation with which to compare this.
         * \return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<2>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0 and 1 is mapped to itself.
         *
         * \return \c true if and only if this is the identity permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<2>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<2>::Sn, which is the identity.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this permutation after the increment.
         */
        Perm<2>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<2>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<2>::Sn, which is the identity.
         *
         * \python This routine is named inc() since python does
         * not support the increment operator.
         *
         * \return a copy of this permutation before the increment took place.
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
         * \param rhs the permutation to compare this against.
         * \return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<2>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps \a k to \a k + \a i (mod 2) for all \a k.
         *
         * \param i the image of 0; this must be 0 or 1.
         * \return the <i>i</i>th rotation.
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
         * `rand(randomEngine.engine(), even)`.
         *
         * \param even if \c true, then the resulting permutation is
         * guaranteed to be even (which means, for a permutation on two
         * elements, the resulting permutation must be the identity).
         * \return a random permutation.
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
         * \nopython Python users are still able to use the non-thread-safe
         * variant without the \a gen argument.
         *
         * \param gen the source of randomness to use (e.g., one of the
         * many options provided in the C++ standard \c random header).
         * \param even if \c true, then the resulting permutation is
         * guaranteed to be even (which means, for a permutation on two
         * elements, the resulting permutation must be the identity).
         * \return a random permutation.
         */
        template <class URBG>
        static Perm rand(URBG&& gen, bool even = false);

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of two adjacent digits
         * representing the images of 0 and 1 respectively.  An
         * example of a string representation is `10`.
         *
         * \return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * \param len the length of the prefix required; this must be
         * between 0 and 2 inclusive.
         * \return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(int len) const;

        /**
         * Writes the tight encoding of this permutation to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n ≤ 7), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (8 ≤ \a n ≤ 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Returns the tight encoding of this permutation.
         * See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n ≤ 7), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (8 ≤ \a n ≤ 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * \return the resulting encoded string.
         */
        std::string tightEncoding() const;

        /**
         * Reconstructs a permutation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be given as a string.  If this string
         * contains leading whitespace or any trailing characters at all
         * (including trailing whitespace), then it will be treated as
         * an invalid encoding (i.e., this routine will throw an exception).
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n ≤ 7), but slower for larger permutation classes
         * (8 ≤ \a n ≤ 16).  See tightEncoding() for further details.
         *
         * \exception InvalidArgument The given string is not a tight encoding
         * of a 2-element permutation.
         *
         * \param enc the tight encoding for a 2-element permutation.
         * \return the permutation represented by the given tight encoding.
         */
        static Perm tightDecoding(const std::string& enc);

        /**
         * Reconstructs a permutation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n ≤ 7), but slower for larger permutation classes
         * (8 ≤ \a n ≤ 16).  See tightEncoding() for further details.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of a 2-element permutation.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a 2-element permutation.
         * \return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Hashes this permutation to a non-negative integer, allowing it
         * to be used for keys in hash tables.
         *
         * The implementation currently returns the internal permutation code
         * (which for Perm<2> will always fit within a \c size_t).  This
         * implementation (and therefore the specific hash values obtained)
         * is subject to change in future versions of Regina.
         *
         * \python For Python users, this function uses the standard Python
         * name __hash__().  This allows permutations to be used as keys in
         * Python dictionaries and sets.
         *
         * \return The integer hash of this permutation.
         */
        constexpr size_t hash() const;

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,1,
         * this routine will ensure that `image[i] == i`.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,1 are exactly
         * <i>from</i>,...,1, but possibly in a different order.
         *
         * \param from the first integer whose image should be reset.
         * This must be between 0 and 2 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<2>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * \return the index \a i for which this permutation is equal to
         * Perm<2>::Sn[i].  This will be 0 or 1.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<2>::S2 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<7>::S7Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * \return the index \a i for which this permutation is equal to
         * Perm<2>::S2[i].  This will be 0 or 1.
         */
        constexpr Index S2Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<2>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * \return the lexicographical index of this permutation.
         * This will be 0 or 1.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<2>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         * In general, for every \a n there will be a member function
         * Perm<n>::orderedSnIndex(); however, these numerical aliases
         * Perm<2>::orderedS2Index(), ..., Perm<7>::orderedS7Index() are
         * only available for small \a n.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * \return the lexicographical index of this permutation.
         * This will be 0 or 1.
         */
        constexpr Index orderedS2Index() const;

        /**
         * Restricts a <i>k</i>-element permutation to an 2-element
         * permutation, where \a k > 2.
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
         * \param p a permutation on \a k elements.
         * \return the same permutation restricted to a permutation on
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
         * \return \c true if and only if this permutation is minimal in its
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
         * \param code the internal code from which the new
         * permutation will be created.
         */
        constexpr Perm(Code code);

    private:
        /**
         * Reconstructs a permutation from its given tight encoding.
         *
         * The tight encoding will be extracted one character at a time
         * beginning with the iterator \a start, in a single pass, without
         * skipping any leading whitespace.  If the iterator ever reaches
         * \a limit before the encoding is complete then the encoding is
         * treated as invalid (i.e., this routine will throw an exception).
         *
         * If \a noTrailingData is \c true then the iterator is required to
         * _finish_ at \a limit, or else the encoding will be considered
         * invalid also; if \a noTrailingData is \c false then there is no
         * constraint on the final state of the iterator.
         *
         * \exception InvalidInput The given iterator does not point to
         * a tight encoding of a 2-element permutation.
         *
         * \tparam iterator an input iterator type.
         *
         * \param start an iterator that points to the beginning of a
         * tight encoding.
         * \param limit an iterator that, if reached, indicates that no more
         * characters are available.
         * \param noTrailingData \c true if iteration should reach \a limit
         * immediately after the encoding is read, or \c false if there is
         * allowed to be additional unread data.
         * \return the permutation represented by the given tight encoding.
         */
        template <typename iterator>
        static Perm tightDecode(iterator start, iterator limit,
            bool noTrailingData);
};

// Inline functions for Perm<2>

inline constexpr Perm<2> Perm<2>::S2Lookup::operator[] (int index) const {
    return Perm<2>(static_cast<Code>(index));
}

inline constexpr Perm<2> Perm<2>::S1Lookup::operator[] (int) const {
    return Perm<2>();
}

inline constexpr void Perm<2>::precompute() {
}

inline constexpr Perm<2>::Perm() : code_(0) {
}

inline constexpr Perm<2>::Perm(Code code) : code_(code) {
}

inline constexpr Perm<2>::Perm(int a, int b) : code_(a == b ? 0 : 1) {
}

inline constexpr Perm<2>::Perm(const std::array<int, 2>& image) :
        code_(static_cast<Code>(image[0])) {
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

inline Perm<2> Perm<2>::cachedComp(const Perm<2>& q) const {
    return Perm<2>(code_ ^ q.code_);
}

inline Perm<2> Perm<2>::cachedComp(const Perm<2>& q, const Perm<2>& r) const {
    return Perm<2>(code_ ^ q.code_ ^ r.code_);
}

inline constexpr Perm<2> Perm<2>::conjugate(const Perm<2>& q) const {
    // In S2, conjugation does nothing.
    return *this;
}

inline Perm<2> Perm<2>::cachedConjugate(const Perm<2>& q) const {
    // In S2, conjugation does nothing.
    return *this;
}

inline constexpr Perm<2> Perm<2>::inverse() const {
    return *this;
}

inline Perm<2> Perm<2>::cachedInverse() const {
    return *this;
}

inline constexpr Perm<2> Perm<2>::pow(long exp) const {
    return (exp & 1 ? *this : Perm<2>());
}

inline Perm<2> Perm<2>::cachedPow(long exp) const {
    return (exp & 1 ? *this : Perm<2>());
}

inline constexpr int Perm<2>::order() const {
    return (code_ + 1);
}

inline int Perm<2>::cachedOrder() const {
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

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <class URBG>
inline Perm<2> Perm<2>::rand(URBG&& gen, bool even) {
    if (even)
        return Perm<2>();
    else {
        std::uniform_int_distribution<short> d(0, 1);
        return Perm<2>(static_cast<Code>(d(gen)));
    }
}
#endif // __DOXYGEN

inline std::string Perm<2>::str() const {
    return (code_ == 0 ? "01" : "10");
}

inline std::string Perm<2>::trunc(int len) const {
    switch (len) {
        case 2 : return (code_ == 0 ? "01" : "10");
        case 1 : return (code_ == 0 ? "0" : "1");
        default : return std::string();
    }
}

inline void Perm<2>::tightEncode(std::ostream& out) const {
    out << static_cast<char>(code_ + 33);
}

inline std::string Perm<2>::tightEncoding() const {
    char ans[2] { static_cast<char>(code_ + 33), 0 };
    return ans;
}

inline Perm<2> Perm<2>::tightDecoding(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

inline Perm<2> Perm<2>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <typename iterator>
Perm<2> Perm<2>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");

    Code code = (*start++) - 33;
    // code >= 0 because we are using an unsigned data type.
    if (code > 1)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Perm<2>(code);
}
#endif // __DOXYGEN

inline constexpr size_t Perm<2>::hash() const {
    return code_;
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

inline constexpr bool Perm<2>::isConjugacyMinimal() const {
    // All (i.e., both) permutations are minimal.
    return true;
}

} // namespace regina

#endif

