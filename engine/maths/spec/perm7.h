
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

/*! \file maths/spec/perm5.h
 *  \brief Internal header for permutations of {0,1,2,3,4,5,6}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM7_H
#ifndef __DOXYGEN
#define __REGINA_PERM7_H
#endif

namespace regina {

/**
 * Represents a permutation of {0,1,2,3,4,5,6}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 6-dimensional triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<7>, the internal permutation
 * codes have changed as of Regina 7.0:
 *
 * - \e First-generation codes were used internally in Regina 6.0.1 and earlier.
 *   These are <i>image packs</i>: integers whose lowest three bits represent
 *   the image of 0, whose next lowest three bits represent the image of 1,
 *   and so on.  The routines permCode1(), setPermCode1(), fromPermCode1()
 *   and isPermCode1() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe gluings between
 *   6-simplices.
 *
 * - \e Second-generation codes are used internally in Regina 7.0 and above.
 *   These codes are integers between 0 and 5039 inclusive, representing the
 *   index of the permutation in the array Perm<7>::S7.  The routines
 *   permCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by Perm<7>).
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm7.
 *
 * \ingroup maths
 */
template <>
class Perm<7> {
    private:
        /**
         * An array-like object used to implement Perm<7>::S7.
         */
        struct S7Lookup {
            /**
             * Returns the permutation at the given index in the array S7.
             * See Perm<7>::S7 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5039 inclusive.
             * @return the corresponding permutation in S7.
             */
            constexpr Perm<7> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<7>::orderedS7.
         */
        struct OrderedS7Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS7.  See Perm<7>::orderedS7 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5039 inclusive.
             * @return the corresponding permutation in orderedS7.
             */
            constexpr Perm<7> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on seven elements.  In other words, this is a
         * native signed integer type large enough to store (7!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on seven elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 5040;

        /**
         * The total number of permutations on six elements.
         * This is the size of the symmetric group <i>S<sub>6</sub></i>.
         */
        static constexpr Index nPerms_1 = 720;

        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines 7 such images together, and so uses
         * 7 * \a imageBits bits in total.
         */
        static constexpr int imageBits = 3;

        /**
         * Indicates the native unsigned integer type used to store a
         * single image pack.  See the class notes for more information
         * on image packs, and how they are used to build the old
         * first-generation permutation codes.
         */
        using ImagePack = uint32_t;

        /**
         * A bitmask whose lowest \a imageBits bits are 1, and whose
         * remaining higher order bits are all 0.  This may be useful when
         * creating or analysing image packs.
         */
        static constexpr ImagePack imageMask =
                (static_cast<ImagePack>(1) << imageBits) - 1;

        /**
         * Indicates the native unsigned integer type used to store a
         * first-generation permutation code.
         */
        using Code1 = ImagePack;

        /**
         * Indicates the native unsigned integer type used to store a
         * second-generation permutation code.
         */
        using Code2 = uint16_t;

        /**
         * Gives array-like access to all possible permutations of
         * seven elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 5039 inclusive.
         *
         * Accessing elements through this object is extremely fast.
         * The object that is returned is lightweight and is defined in the
         * headers only; in particular, you cannot make a reference to it
         * (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This is different from Perm<7>::orderedSn, since this array \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         */
        static constexpr S7Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * seven elements.
         *
         * This is a dimension-specific alias for Perm<7>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<7>::S7 are only available for small \a n.
         */
        static constexpr S7Lookup S7 {};

        /**
         * Gives array-like access to all possible permutations of seven
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 5039 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[6]).
         *
         * Accessing elements through this object is extremely fast.
         * The object that is returned is lightweight and is defined in the
         * headers only; in particular, you cannot make a reference to it
         * (but you can always make a copy).
         *
         * This is different from Perm<7>::Sn, since this array \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         */
        static constexpr OrderedS7Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of seven
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<7>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<7>::orderedS7 are only
         * available for small \a n.
         */
        static constexpr OrderedS7Lookup orderedS7 {};

    protected:
        Code2 code2_;
            /**< The internal second-generation permutation code
                 representing this permutation. */

    private:
        static Code2 (*products_)[5040];
            /**< The precomputed table of products, or \c null if this
                 has not yet been generated.  This is a 5040-by-5040 array,
                 whose indices and values all represent indices into S7. */

        static int *orders_;
            /**< The precomputed table of orders, or \c null if this has
                 not yet been generated.  This is an array of size 5040,
                 whose indices represent indices into S7. */

        static Code2 (*powers_)[11];
            /**< The precomputed table of powers, or \c null if this has
                 not yet been generated.  This is a 5040-by-11 array,
                 where powers[i][e] is the S7 index of S7[i]^(e+1).
                 If e+1 >= orders_[i] then the corresponding value in
                 this array is left undefined (and is irrelevant). */

    public:
        /**
         * Performs the precomputation necessary for using the optimised
         * cachedComp(), cachedPow() and cachedOrder() routines.
         *
         * This \e must be called before calling any of cachedComp(),
         * cachedPow() or cachedOrder().
         *
         * This only needs to be done once in the lifetime of the program.
         * If you do try to call precompute() a second time then it will
         * do nothing and return immediately.
         *
         * This routine is thread-safe.
         */
        static void precompute();

        /**
         * Creates the identity permutation.
         */
        constexpr Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3,4,5,6}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3,4,5,6) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>)
         * respectively.
         *
         * \pre
         * {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>} =
         * {0,1,2,3,4,5,6}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         * @param g the desired image of 6.
         */
        constexpr Perm(int a, int b, int c, int d, int e, int f, int g);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3,4,5,6.
         *
         * \pre The elements of \a image are 0, 1, 2, 3, 4, 5 and 6 in some
         * order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 7>& image);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>,<i>g0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>,<i>g1</i>)
         * respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>,<i>g0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>,<i>g1</i>} =
         * {0,1,2,3,4,5,6}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param e0 the desired preimage of <i>e1</i>.
         * @param f0 the desired preimage of <i>f1</i>.
         * @param g0 the desired preimage of <i>g1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         * @param e1 the desired image of <i>e0</i>.
         * @param f1 the desired image of <i>f0</i>.
         * @param g1 the desired image of <i>g0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1,
            int d0, int d1, int e0, int e1, int f0, int f1, int g0, int g1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<7>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<7> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @return the first-generation permutation code.
         */
        constexpr Code1 permCode1() const;

        /**
         * Returns the second-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid second-generation permutation
         * code as determined by isPermCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<7> class.
         *
         * @return the second-generation permutation code.
         */
        constexpr Code2 permCode2() const;

        /**
         * Sets this permutation to that represented by the given
         * first-generation permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode1() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<7> now uses second-generation codes internally.
         * See the class notes and the routine setPermCode2() for details.
         *
         * @param code the first-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode1(Code1 code);

        /**
         * Sets this permutation to that represented by the given
         * second-generation permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<7> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode2(Code2 code);

        /**
         * Creates a permutation from the given first-generation
         * permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode1() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<7> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<7> fromPermCode1(Code1 code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<7> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<7> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode1() or fromPermCode1(), and are returned by permCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<7> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * first-generation permutation code.
         */
        static constexpr bool isPermCode1(Code1 code);

        /**
         * Determines whether the given character is a valid second-generation
         * permutation code.  Valid second-generation codes can be passed to
         * setPermCode2() or fromPermCode2(), and are returned by permCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<7> class.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static constexpr bool isPermCode2(Code2 code);

        /**
         * Returns the image pack that represents this permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<7>, this routine is identical to permCode1().
         *
         * @return the image pack for this permutation.
         */
        constexpr ImagePack imagePack() const;

        /**
         * Creates a permutation from the given image pack.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<7>, this routine is identical to fromPermCode1().
         *
         * \pre The argument \a pack is a valid image pack; see isImagePack()
         * for details.
         *
         * @param pack an image pack that describes a permutation.
         * @return the permutation represented by the given image pack.
         */
        static constexpr Perm fromImagePack(ImagePack pack);

        /**
         * Determines whether the given argument is the image pack of
         * some 7-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<7>, this routine is identical to isPermCode1().
         *
         * @param pack the candidate image pack to test.
         * @return \c true if and only if \a pack is a valid image pack.
         */
        static constexpr bool isImagePack(ImagePack pack);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<7>& operator = (const Perm<7>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * For permutations of five and fewer objects, composition is
         * extremely fast because it uses hard-coded lookup tables.
         * However, for Perm<7> these tables would grow too large, and so
         * instead this routine involves significant computational overhead.
         *
         * If you are going to make significant use of the Perm<7> class,
         * you should instead:
         *
         * - call precompute() to precompute a full 5040-by-5040 lookup table
         *   in advance (this will consume roughly 50MB of memory); and then
         *
         * - call cachedComp() instead of the * operator to compute your
         *   compositions.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        constexpr Perm<7> operator * (const Perm<7>& q) const;

        /**
         * Returns the composition of this and the given permutation,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp() is a
         * single table lookup, whereas the * operator requires significant
         * computational overhead.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup table
         * will consume roughly 50MB of memory for the lifetime of your program.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling <tt>(*this) * q</tt>.
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedComp().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        Perm<7> cachedComp(const Perm<7>& q) const;

        /**
         * Returns the composition of this and the given two permutations,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp()
         * with two arguments requires just two table lookups, whereas using
         * the * operator twice involves significant computational overhead.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly 50MB of memory for the lifetime of your program.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling <tt>(*this) * q * r</tt>.
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedComp().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param q the first permutation to compose this with.
         * @param r the second permutation to compose this with.
         * @return the composition of both permutations.
         */
        Perm<7> cachedComp(const Perm<7>& q, const Perm<7>& r) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<7> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * For Perm<7>, this routine makes use of the composition operator *,
         * which involves significant computational overhead.  If you need
         * your powers to be fast, you can instead:
         *
         * - call precompute() to precompute a full 5040-by-5040 product table
         *   in advance (this will consume roughly 50MB of memory); and then
         *
         * - call cachedPow() instead of pow() to make full use of this table,
         *   which will remove most of the overhead from this routine.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<7> pow(long exp) const;

        /**
         * Computes the given power of this permutation, using fast
         * precomputed lookup tables.
         *
         * This routine runs in constant time.
         *
         * The advantage of this routine is speed: calling cachedPow() removes
         * most of the significant computational overhead required by pow().
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly 50MB of memory for the lifetime of your program.
         * Note that these are the same lookup tables used by cachedComp() and
         * cachedOrder(), so if you are already using cachedComp() or
         * cachedOrder() then there is no extra cost for using this routine
         * also.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling pow(exp).
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedPow().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        Perm<7> cachedPow(long exp) const;

        /**
         * Returns the order of this permutation.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * Unlike the smaller permutation classes, Perm<7>::order() does
         * not use precomputed tables; instead it computes orders on the fly.
         * If you need your order computation to be faster, you can instead:
         *
         * - call precompute() to precompute a full table of orders in advance
         *   (though this will also compute the much larger 5040-by-5040 table
         *   of products, which consumes roughly 50MB of memory); and then
         *
         * - call cachedOrder() instead of order(), which will now become a
         *   fast table lookup.
         *
         * @return the order of this permutation.
         */
        constexpr int order() const;

        /**
         * Returns the order of this permutation, using fast precomputed
         * lookup tables.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * The advantage of this routine is speed: calling cachedOrder()
         * removes most of the computational overhead required by order().
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup tables
         * will consume roughly 50MB of memory for the lifetime of your program.
         * Note that these are the same lookup tables used by cachedComp()
         * and cachedPow(), so if you are already using cachedComp() or
         * cachedPow() then there is no extra cost for using this routine also.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling order().
         *
         * \pre You \e must have called the routine precompute() at least once
         * in the lifetime of this program before using cachedOrder().
         * Otherwise this routine will almost certainly crash your program.
         *
         * @return the order of this permutation.
         */
        int cachedOrder() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,...,6.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[6 - i]</tt> for all \a i.
         */
        constexpr Perm<7> reverse() const;

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
         * should be between 0 and 6 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 6 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2, 3, 4, 5 and 6.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<7>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3, 4, 5 or 6.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<7>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4,5,6) under this
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
        constexpr int compareWith(const Perm<7>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2, 3, 4, 5 and 6 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<7>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<7>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<7>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<7>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<7>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<7> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<7>::Sn.
         *
         * Note that this is \e not the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<7>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 7)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 6 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on seven elements.
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
         * Returns a random permutation on seven elements, using the
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
         * The representation will consist of seven adjacent digits
         * representing the images of 0, 1, 2, 3, 4, 5 and 6 respectively.
         * An example of a string representation is <tt>3045261</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 7 inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

        /**
         * Writes the tight encoding of this permutation to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n &le; 7), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (8 &le; \a n &le; 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * \ifacespython Not present; use tightEncoding() instead.
         *
         * @param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Returns the tight encoding of this permutation.
         * See the page on \ref tight "tight encodings" for details.
         *
         * For all permutation classes Perm<n>, the tight encoding is based on
         * the index into the full permutation group \a S_n.  For smaller
         * permutation classes (\a n &le; 7), such encodings are very fast to
         * work with since the \a S_n index is used as the internal permutation
         * code.  For larger permutation classes however (8 &le; \a n &le; 16),
         * the \a S_n index requires some non-trivial work to compute.
         *
         * @return the resulting encoded string.
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
         * (\a n &le; 7), but slower for larger permutation classes
         * (8 &le; \a n &le; 16).  See tightEncoding() for further details.
         *
         * \exception InvalidArgument the given string is not a tight encoding
         * of a 7-element permutation.
         *
         * @param enc the tight encoding for a 7-element permutation.
         * @return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(const std::string& enc);

        /**
         * Reconstructs a permutation from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input routine \e may contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n &le; 7), but slower for larger permutation classes
         * (8 &le; \a n &le; 16).  See tightEncoding() for further details.
         *
         * \exception InvalidInput the given input stream does not begin with
         * a tight encoding of a 7-element permutation.
         *
         * \ifacespython Not present, but the string version of this routine
         * is available.
         *
         * @param input an input stream that begins with the tight encoding
         * for a 7-element permutation.
         * @return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,6,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,6 are exactly
         * <i>from</i>,...,6, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 7 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<7>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<7>::Sn[i].  This will be between 0 and 5039 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<7>::S7 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<7>::S7Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<7>::S7[i].  This will be between 0 and 5039 inclusive.
         */
        constexpr Index S7Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<7>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 5039 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<7>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         * In general, for every \a n there will be a member function
         * Perm<n>::orderedSnIndex(); however, these numerical aliases
         * Perm<2>::orderedS2Index(), ..., Perm<7>::orderedS7Index() are
         * only available for small \a n.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 5039 inclusive.
         */
        constexpr Index orderedS7Index() const;

        /**
         * Extends a <i>k</i>-element permutation to a 7-element permutation,
         * where 2 &le; \a k &lt; 7.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,6 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2, 3, 4, 5 or 6.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation expressed as a permutation on
         * seven elements.
         */
        template <int k>
        static constexpr Perm<7> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to a 7-element
         * permutation, where \a k &gt; 7.
         *
         * The resulting permutation will map 0,...,6 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[7],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,6 to 0,...,6 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 7.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 7 elements.
         */
        template <int k>
        static constexpr Perm<7> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<7>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<7>, since it essentially
         * uses a hard-coded lookup table.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

    private:
        /**
         * Contains the inverses of the permutations in the array \a S7.
         *
         * Specifically, the inverse of permutation <tt>S7[i]</tt> is
         * the permutation <tt>S7[ invS7[i] ]</tt>.
         */
        static constexpr Code2 invS7[5040] = {
            // Generated using Regina 6.0.
            0, 1, 4, 3, 2, 5, 6, 7, 12, 19,
            18, 13, 8, 11, 20, 15, 16, 23, 10, 9,
            14, 21, 22, 17, 24, 25, 26, 29, 28, 27,
            48, 49, 96, 73, 72, 97, 52, 51, 74, 99,
            100, 77, 50, 53, 98, 75, 76, 101, 30, 31,
            42, 37, 36, 43, 54, 55, 78, 103, 102, 79,
            60, 67, 108, 85, 90, 115, 66, 61, 84, 109,
            114, 91, 34, 33, 38, 45, 46, 41, 56, 59,
            104, 81, 82, 107, 68, 63, 86, 111, 116, 93,
            64, 71, 112, 89, 94, 119, 32, 35, 44, 39,
            40, 47, 58, 57, 80, 105, 106, 83, 62, 69,
            110, 87, 92, 117, 70, 65, 88, 113, 118, 95,
            120, 121, 122, 125, 124, 123, 126, 127, 138, 133,
            132, 139, 130, 129, 134, 141, 142, 137, 128, 131,
            140, 135, 136, 143, 240, 241, 244, 243, 242, 245,
            360, 361, 480, 601, 600, 481, 362, 365, 602, 483,
            484, 605, 364, 363, 482, 603, 604, 485, 246, 247,
            252, 259, 258, 253, 366, 367, 606, 487, 486, 607,
            378, 373, 492, 613, 618, 499, 372, 379, 612, 493,
            498, 619, 248, 251, 260, 255, 256, 263, 370, 369,
            488, 609, 610, 491, 374, 381, 614, 495, 500, 621,
            382, 377, 496, 617, 622, 503, 250, 249, 254, 261,
            262, 257, 368, 371, 608, 489, 490, 611, 380, 375,
            494, 615, 620, 501, 376, 383, 616, 497, 502, 623,
            144, 145, 148, 147, 146, 149, 168, 169, 192, 217,
            216, 193, 170, 173, 218, 195, 196, 221, 172, 171,
            194, 219, 220, 197, 264, 265, 266, 269, 268, 267,
            384, 385, 624, 505, 504, 625, 388, 387, 506, 627,
            628, 509, 386, 389, 626, 507, 508, 629, 288, 289,
            336, 313, 312, 337, 408, 409, 528, 649, 648, 529,
            432, 457, 672, 553, 576, 697, 456, 433, 552, 673,
            696, 577, 292, 291, 314, 339, 340, 317, 410, 413,
            650, 531, 532, 653, 458, 435, 554, 675, 698, 579,
            436, 461, 676, 557, 580, 701, 290, 293, 338, 315,
            316, 341, 412, 411, 530, 651, 652, 533, 434, 459,
            674, 555, 578, 699, 460, 437, 556, 677, 700, 581,
            150, 151, 156, 163, 162, 157, 174, 175, 222, 199,
            198, 223, 186, 181, 204, 229, 234, 211, 180, 187,
            228, 205, 210, 235, 270, 271, 282, 277, 276, 283,
            390, 391, 510, 631, 630, 511, 396, 403, 636, 517,
            522, 643, 402, 397, 516, 637, 642, 523, 294, 295,
            318, 343, 342, 319, 414, 415, 654, 535, 534, 655,
            462, 439, 558, 679, 702, 583, 438, 463, 678, 559,
            582, 703, 300, 307, 348, 325, 330, 355, 426, 421,
            540, 661, 666, 547, 444, 469, 684, 565, 588, 709,
            474, 451, 570, 691, 714, 595, 306, 301, 324, 349,
            354, 331, 420, 427, 660, 541, 546, 667, 468, 445,
            564, 685, 708, 589, 450, 475, 690, 571, 594, 715,
            152, 155, 164, 159, 160, 167, 178, 177, 200, 225,
            226, 203, 182, 189, 230, 207, 212, 237, 190, 185,
            208, 233, 238, 215, 274, 273, 278, 285, 286, 281,
            392, 395, 632, 513, 514, 635, 404, 399, 518, 639,
            644, 525, 400, 407, 640, 521, 526, 647, 296, 299,
            344, 321, 322, 347, 418, 417, 536, 657, 658, 539,
            440, 465, 680, 561, 584, 705, 466, 443, 562, 683,
            706, 587, 308, 303, 326, 351, 356, 333, 422, 429,
            662, 543, 548, 669, 470, 447, 566, 687, 710, 591,
            452, 477, 692, 573, 596, 717, 304, 311, 352, 329,
            334, 359, 430, 425, 544, 665, 670, 551, 448, 473,
            688, 569, 592, 713, 478, 455, 574, 695, 718, 599,
            154, 153, 158, 165, 166, 161, 176, 179, 224, 201,
            202, 227, 188, 183, 206, 231, 236, 213, 184, 191,
            232, 209, 214, 239, 272, 275, 284, 279, 280, 287,
            394, 393, 512, 633, 634, 515, 398, 405, 638, 519,
            524, 645, 406, 401, 520, 641, 646, 527, 298, 297,
            320, 345, 346, 323, 416, 419, 656, 537, 538, 659,
            464, 441, 560, 681, 704, 585, 442, 467, 682, 563,
            586, 707, 302, 309, 350, 327, 332, 357, 428, 423,
            542, 663, 668, 549, 446, 471, 686, 567, 590, 711,
            476, 453, 572, 693, 716, 597, 310, 305, 328, 353,
            358, 335, 424, 431, 664, 545, 550, 671, 472, 449,
            568, 689, 712, 593, 454, 479, 694, 575, 598, 719,
            720, 721, 722, 725, 724, 723, 726, 727, 738, 733,
            732, 739, 730, 729, 734, 741, 742, 737, 728, 731,
            740, 735, 736, 743, 744, 745, 748, 747, 746, 749,
            768, 769, 792, 817, 816, 793, 770, 773, 818, 795,
            796, 821, 772, 771, 794, 819, 820, 797, 750, 751,
            756, 763, 762, 757, 774, 775, 822, 799, 798, 823,
            786, 781, 804, 829, 834, 811, 780, 787, 828, 805,
            810, 835, 752, 755, 764, 759, 760, 767, 778, 777,
            800, 825, 826, 803, 782, 789, 830, 807, 812, 837,
            790, 785, 808, 833, 838, 815, 754, 753, 758, 765,
            766, 761, 776, 779, 824, 801, 802, 827, 788, 783,
            806, 831, 836, 813, 784, 791, 832, 809, 814, 839,
            1440, 1441, 1444, 1443, 1442, 1445, 1446, 1447, 1452, 1459,
            1458, 1453, 1448, 1451, 1460, 1455, 1456, 1463, 1450, 1449,
            1454, 1461, 1462, 1457, 2160, 2161, 2162, 2165, 2164, 2163,
            2880, 2881, 4320, 3601, 3600, 4321, 2884, 2883, 3602, 4323,
            4324, 3605, 2882, 2885, 4322, 3603, 3604, 4325, 2166, 2167,
            2178, 2173, 2172, 2179, 2886, 2887, 3606, 4327, 4326, 3607,
            2892, 2899, 4332, 3613, 3618, 4339, 2898, 2893, 3612, 4333,
            4338, 3619, 2170, 2169, 2174, 2181, 2182, 2177, 2888, 2891,
            4328, 3609, 3610, 4331, 2900, 2895, 3614, 4335, 4340, 3621,
            2896, 2903, 4336, 3617, 3622, 4343, 2168, 2171, 2180, 2175,
            2176, 2183, 2890, 2889, 3608, 4329, 4330, 3611, 2894, 2901,
            4334, 3615, 3620, 4341, 2902, 2897, 3616, 4337, 4342, 3623,
            1464, 1465, 1466, 1469, 1468, 1467, 1488, 1489, 1536, 1513,
            1512, 1537, 1492, 1491, 1514, 1539, 1540, 1517, 1490, 1493,
            1538, 1515, 1516, 1541, 2184, 2185, 2188, 2187, 2186, 2189,
            2904, 2905, 3624, 4345, 4344, 3625, 2906, 2909, 4346, 3627,
            3628, 4349, 2908, 2907, 3626, 4347, 4348, 3629, 2208, 2209,
            2232, 2257, 2256, 2233, 2928, 2929, 4368, 3649, 3648, 4369,
            2976, 2953, 3672, 4393, 4416, 3697, 2952, 2977, 4392, 3673,
            3696, 4417, 2210, 2213, 2258, 2235, 2236, 2261, 2932, 2931,
            3650, 4371, 4372, 3653, 2954, 2979, 4394, 3675, 3698, 4419,
            2980, 2957, 3676, 4397, 4420, 3701, 2212, 2211, 2234, 2259,
            2260, 2237, 2930, 2933, 4370, 3651, 3652, 4373, 2978, 2955,
            3674, 4395, 4418, 3699, 2956, 2981, 4396, 3677, 3700, 4421,
            1470, 1471, 1482, 1477, 1476, 1483, 1494, 1495, 1518, 1543,
            1542, 1519, 1500, 1507, 1548, 1525, 1530, 1555, 1506, 1501,
            1524, 1549, 1554, 1531, 2190, 2191, 2196, 2203, 2202, 2197,
            2910, 2911, 4350, 3631, 3630, 4351, 2922, 2917, 3636, 4357,
            4362, 3643, 2916, 2923, 4356, 3637, 3642, 4363, 2214, 2215,
            2262, 2239, 2238, 2263, 2934, 2935, 3654, 4375, 4374, 3655,
            2958, 2983, 4398, 3679, 3702, 4423, 2982, 2959, 3678, 4399,
            4422, 3703, 2226, 2221, 2244, 2269, 2274, 2251, 2940, 2947,
            4380, 3661, 3666, 4387, 2988, 2965, 3684, 4405, 4428, 3709,
            2970, 2995, 4410, 3691, 3714, 4435, 2220, 2227, 2268, 2245,
            2250, 2275, 2946, 2941, 3660, 4381, 4386, 3667, 2964, 2989,
            4404, 3685, 3708, 4429, 2994, 2971, 3690, 4411, 4434, 3715,
            1474, 1473, 1478, 1485, 1486, 1481, 1496, 1499, 1544, 1521,
            1522, 1547, 1508, 1503, 1526, 1551, 1556, 1533, 1504, 1511,
            1552, 1529, 1534, 1559, 2192, 2195, 2204, 2199, 2200, 2207,
            2914, 2913, 3632, 4353, 4354, 3635, 2918, 2925, 4358, 3639,
            3644, 4365, 2926, 2921, 3640, 4361, 4366, 3647, 2218, 2217,
            2240, 2265, 2266, 2243, 2936, 2939, 4376, 3657, 3658, 4379,
            2984, 2961, 3680, 4401, 4424, 3705, 2962, 2987, 4402, 3683,
            3706, 4427, 2222, 2229, 2270, 2247, 2252, 2277, 2948, 2943,
            3662, 4383, 4388, 3669, 2966, 2991, 4406, 3687, 3710, 4431,
            2996, 2973, 3692, 4413, 4436, 3717, 2230, 2225, 2248, 2273,
            2278, 2255, 2944, 2951, 4384, 3665, 3670, 4391, 2992, 2969,
            3688, 4409, 4432, 3713, 2974, 2999, 4414, 3695, 3718, 4439,
            1472, 1475, 1484, 1479, 1480, 1487, 1498, 1497, 1520, 1545,
            1546, 1523, 1502, 1509, 1550, 1527, 1532, 1557, 1510, 1505,
            1528, 1553, 1558, 1535, 2194, 2193, 2198, 2205, 2206, 2201,
            2912, 2915, 4352, 3633, 3634, 4355, 2924, 2919, 3638, 4359,
            4364, 3645, 2920, 2927, 4360, 3641, 3646, 4367, 2216, 2219,
            2264, 2241, 2242, 2267, 2938, 2937, 3656, 4377, 4378, 3659,
            2960, 2985, 4400, 3681, 3704, 4425, 2986, 2963, 3682, 4403,
            4426, 3707, 2228, 2223, 2246, 2271, 2276, 2253, 2942, 2949,
            4382, 3663, 3668, 4389, 2990, 2967, 3686, 4407, 4430, 3711,
            2972, 2997, 4412, 3693, 3716, 4437, 2224, 2231, 2272, 2249,
            2254, 2279, 2950, 2945, 3664, 4385, 4390, 3671, 2968, 2993,
            4408, 3689, 3712, 4433, 2998, 2975, 3694, 4415, 4438, 3719,
            840, 841, 844, 843, 842, 845, 846, 847, 852, 859,
            858, 853, 848, 851, 860, 855, 856, 863, 850, 849,
            854, 861, 862, 857, 960, 961, 962, 965, 964, 963,
            1080, 1081, 1320, 1201, 1200, 1321, 1084, 1083, 1202, 1323,
            1324, 1205, 1082, 1085, 1322, 1203, 1204, 1325, 966, 967,
            978, 973, 972, 979, 1086, 1087, 1206, 1327, 1326, 1207,
            1092, 1099, 1332, 1213, 1218, 1339, 1098, 1093, 1212, 1333,
            1338, 1219, 970, 969, 974, 981, 982, 977, 1088, 1091,
            1328, 1209, 1210, 1331, 1100, 1095, 1214, 1335, 1340, 1221,
            1096, 1103, 1336, 1217, 1222, 1343, 968, 971, 980, 975,
            976, 983, 1090, 1089, 1208, 1329, 1330, 1211, 1094, 1101,
            1334, 1215, 1220, 1341, 1102, 1097, 1216, 1337, 1342, 1223,
            1560, 1561, 1562, 1565, 1564, 1563, 1566, 1567, 1578, 1573,
            1572, 1579, 1570, 1569, 1574, 1581, 1582, 1577, 1568, 1571,
            1580, 1575, 1576, 1583, 2280, 2281, 2284, 2283, 2282, 2285,
            3000, 3001, 3720, 4441, 4440, 3721, 3002, 3005, 4442, 3723,
            3724, 4445, 3004, 3003, 3722, 4443, 4444, 3725, 2286, 2287,
            2292, 2299, 2298, 2293, 3006, 3007, 4446, 3727, 3726, 4447,
            3018, 3013, 3732, 4453, 4458, 3739, 3012, 3019, 4452, 3733,
            3738, 4459, 2288, 2291, 2300, 2295, 2296, 2303, 3010, 3009,
            3728, 4449, 4450, 3731, 3014, 3021, 4454, 3735, 3740, 4461,
            3022, 3017, 3736, 4457, 4462, 3743, 2290, 2289, 2294, 2301,
            2302, 2297, 3008, 3011, 4448, 3729, 3730, 4451, 3020, 3015,
            3734, 4455, 4460, 3741, 3016, 3023, 4456, 3737, 3742, 4463,
            1680, 1681, 1684, 1683, 1682, 1685, 1800, 1801, 1920, 2041,
            2040, 1921, 1802, 1805, 2042, 1923, 1924, 2045, 1804, 1803,
            1922, 2043, 2044, 1925, 2400, 2401, 2402, 2405, 2404, 2403,
            3120, 3121, 4560, 3841, 3840, 4561, 3124, 3123, 3842, 4563,
            4564, 3845, 3122, 3125, 4562, 3843, 3844, 4565, 2520, 2521,
            2760, 2641, 2640, 2761, 3240, 3241, 3960, 4681, 4680, 3961,
            3360, 3481, 4800, 4081, 4200, 4921, 3480, 3361, 4080, 4801,
            4920, 4201, 2524, 2523, 2642, 2763, 2764, 2645, 3242, 3245,
            4682, 3963, 3964, 4685, 3482, 3363, 4082, 4803, 4922, 4203,
            3364, 3485, 4804, 4085, 4204, 4925, 2522, 2525, 2762, 2643,
            2644, 2765, 3244, 3243, 3962, 4683, 4684, 3965, 3362, 3483,
            4802, 4083, 4202, 4923, 3484, 3365, 4084, 4805, 4924, 4205,
            1686, 1687, 1692, 1699, 1698, 1693, 1806, 1807, 2046, 1927,
            1926, 2047, 1818, 1813, 1932, 2053, 2058, 1939, 1812, 1819,
            2052, 1933, 1938, 2059, 2406, 2407, 2418, 2413, 2412, 2419,
            3126, 3127, 3846, 4567, 4566, 3847, 3132, 3139, 4572, 3853,
            3858, 4579, 3138, 3133, 3852, 4573, 4578, 3859, 2526, 2527,
            2646, 2767, 2766, 2647, 3246, 3247, 4686, 3967, 3966, 4687,
            3486, 3367, 4086, 4807, 4926, 4207, 3366, 3487, 4806, 4087,
            4206, 4927, 2532, 2539, 2772, 2653, 2658, 2779, 3258, 3253,
            3972, 4693, 4698, 3979, 3372, 3493, 4812, 4093, 4212, 4933,
            3498, 3379, 4098, 4819, 4938, 4219, 2538, 2533, 2652, 2773,
            2778, 2659, 3252, 3259, 4692, 3973, 3978, 4699, 3492, 3373,
            4092, 4813, 4932, 4213, 3378, 3499, 4818, 4099, 4218, 4939,
            1688, 1691, 1700, 1695, 1696, 1703, 1810, 1809, 1928, 2049,
            2050, 1931, 1814, 1821, 2054, 1935, 1940, 2061, 1822, 1817,
            1936, 2057, 2062, 1943, 2410, 2409, 2414, 2421, 2422, 2417,
            3128, 3131, 4568, 3849, 3850, 4571, 3140, 3135, 3854, 4575,
            4580, 3861, 3136, 3143, 4576, 3857, 3862, 4583, 2528, 2531,
            2768, 2649, 2650, 2771, 3250, 3249, 3968, 4689, 4690, 3971,
            3368, 3489, 4808, 4089, 4208, 4929, 3490, 3371, 4090, 4811,
            4930, 4211, 2540, 2535, 2654, 2775, 2780, 2661, 3254, 3261,
            4694, 3975, 3980, 4701, 3494, 3375, 4094, 4815, 4934, 4215,
            3380, 3501, 4820, 4101, 4220, 4941, 2536, 2543, 2776, 2657,
            2662, 2783, 3262, 3257, 3976, 4697, 4702, 3983, 3376, 3497,
            4816, 4097, 4216, 4937, 3502, 3383, 4102, 4823, 4942, 4223,
            1690, 1689, 1694, 1701, 1702, 1697, 1808, 1811, 2048, 1929,
            1930, 2051, 1820, 1815, 1934, 2055, 2060, 1941, 1816, 1823,
            2056, 1937, 1942, 2063, 2408, 2411, 2420, 2415, 2416, 2423,
            3130, 3129, 3848, 4569, 4570, 3851, 3134, 3141, 4574, 3855,
            3860, 4581, 3142, 3137, 3856, 4577, 4582, 3863, 2530, 2529,
            2648, 2769, 2770, 2651, 3248, 3251, 4688, 3969, 3970, 4691,
            3488, 3369, 4088, 4809, 4928, 4209, 3370, 3491, 4810, 4091,
            4210, 4931, 2534, 2541, 2774, 2655, 2660, 2781, 3260, 3255,
            3974, 4695, 4700, 3981, 3374, 3495, 4814, 4095, 4214, 4935,
            3500, 3381, 4100, 4821, 4940, 4221, 2542, 2537, 2656, 2777,
            2782, 2663, 3256, 3263, 4696, 3977, 3982, 4703, 3496, 3377,
            4096, 4817, 4936, 4217, 3382, 3503, 4822, 4103, 4222, 4943,
            864, 865, 866, 869, 868, 867, 888, 889, 936, 913,
            912, 937, 892, 891, 914, 939, 940, 917, 890, 893,
            938, 915, 916, 941, 984, 985, 988, 987, 986, 989,
            1104, 1105, 1224, 1345, 1344, 1225, 1106, 1109, 1346, 1227,
            1228, 1349, 1108, 1107, 1226, 1347, 1348, 1229, 1008, 1009,
            1032, 1057, 1056, 1033, 1128, 1129, 1368, 1249, 1248, 1369,
            1176, 1153, 1272, 1393, 1416, 1297, 1152, 1177, 1392, 1273,
            1296, 1417, 1010, 1013, 1058, 1035, 1036, 1061, 1132, 1131,
            1250, 1371, 1372, 1253, 1154, 1179, 1394, 1275, 1298, 1419,
            1180, 1157, 1276, 1397, 1420, 1301, 1012, 1011, 1034, 1059,
            1060, 1037, 1130, 1133, 1370, 1251, 1252, 1373, 1178, 1155,
            1274, 1395, 1418, 1299, 1156, 1181, 1396, 1277, 1300, 1421,
            1584, 1585, 1588, 1587, 1586, 1589, 1608, 1609, 1632, 1657,
            1656, 1633, 1610, 1613, 1658, 1635, 1636, 1661, 1612, 1611,
            1634, 1659, 1660, 1637, 2304, 2305, 2306, 2309, 2308, 2307,
            3024, 3025, 4464, 3745, 3744, 4465, 3028, 3027, 3746, 4467,
            4468, 3749, 3026, 3029, 4466, 3747, 3748, 4469, 2328, 2329,
            2376, 2353, 2352, 2377, 3048, 3049, 3768, 4489, 4488, 3769,
            3072, 3097, 4512, 3793, 3816, 4537, 3096, 3073, 3792, 4513,
            4536, 3817, 2332, 2331, 2354, 2379, 2380, 2357, 3050, 3053,
            4490, 3771, 3772, 4493, 3098, 3075, 3794, 4515, 4538, 3819,
            3076, 3101, 4516, 3797, 3820, 4541, 2330, 2333, 2378, 2355,
            2356, 2381, 3052, 3051, 3770, 4491, 4492, 3773, 3074, 3099,
            4514, 3795, 3818, 4539, 3100, 3077, 3796, 4517, 4540, 3821,
            1704, 1705, 1706, 1709, 1708, 1707, 1824, 1825, 2064, 1945,
            1944, 2065, 1828, 1827, 1946, 2067, 2068, 1949, 1826, 1829,
            2066, 1947, 1948, 2069, 2424, 2425, 2428, 2427, 2426, 2429,
            3144, 3145, 3864, 4585, 4584, 3865, 3146, 3149, 4586, 3867,
            3868, 4589, 3148, 3147, 3866, 4587, 4588, 3869, 2544, 2545,
            2664, 2785, 2784, 2665, 3264, 3265, 4704, 3985, 3984, 4705,
            3504, 3385, 4104, 4825, 4944, 4225, 3384, 3505, 4824, 4105,
            4224, 4945, 2546, 2549, 2786, 2667, 2668, 2789, 3268, 3267,
            3986, 4707, 4708, 3989, 3386, 3507, 4826, 4107, 4226, 4947,
            3508, 3389, 4108, 4829, 4948, 4229, 2548, 2547, 2666, 2787,
            2788, 2669, 3266, 3269, 4706, 3987, 3988, 4709, 3506, 3387,
            4106, 4827, 4946, 4227, 3388, 3509, 4828, 4109, 4228, 4949,
            1728, 1729, 1776, 1753, 1752, 1777, 1848, 1849, 1968, 2089,
            2088, 1969, 1872, 1897, 2112, 1993, 2016, 2137, 1896, 1873,
            1992, 2113, 2136, 2017, 2448, 2449, 2472, 2497, 2496, 2473,
            3168, 3169, 4608, 3889, 3888, 4609, 3216, 3193, 3912, 4633,
            4656, 3937, 3192, 3217, 4632, 3913, 3936, 4657, 2568, 2569,
            2808, 2689, 2688, 2809, 3288, 3289, 4008, 4729, 4728, 4009,
            3408, 3529, 4848, 4129, 4248, 4969, 3528, 3409, 4128, 4849,
            4968, 4249, 2616, 2593, 2712, 2833, 2856, 2737, 3312, 3337,
            4752, 4033, 4056, 4777, 3552, 3433, 4152, 4873, 4992, 4273,
            3456, 3577, 4896, 4177, 4296, 5017, 2592, 2617, 2832, 2713,
            2736, 2857, 3336, 3313, 4032, 4753, 4776, 4057, 3432, 3553,
            4872, 4153, 4272, 4993, 3576, 3457, 4176, 4897, 5016, 4297,
            1732, 1731, 1754, 1779, 1780, 1757, 1850, 1853, 2090, 1971,
            1972, 2093, 1898, 1875, 1994, 2115, 2138, 2019, 1876, 1901,
            2116, 1997, 2020, 2141, 2450, 2453, 2498, 2475, 2476, 2501,
            3172, 3171, 3890, 4611, 4612, 3893, 3194, 3219, 4634, 3915,
            3938, 4659, 3220, 3197, 3916, 4637, 4660, 3941, 2572, 2571,
            2690, 2811, 2812, 2693, 3290, 3293, 4730, 4011, 4012, 4733,
            3530, 3411, 4130, 4851, 4970, 4251, 3412, 3533, 4852, 4133,
            4252, 4973, 2594, 2619, 2834, 2715, 2738, 2859, 3338, 3315,
            4034, 4755, 4778, 4059, 3434, 3555, 4874, 4155, 4274, 4995,
            3578, 3459, 4178, 4899, 5018, 4299, 2620, 2597, 2716, 2837,
            2860, 2741, 3316, 3341, 4756, 4037, 4060, 4781, 3556, 3437,
            4156, 4877, 4996, 4277, 3460, 3581, 4900, 4181, 4300, 5021,
            1730, 1733, 1778, 1755, 1756, 1781, 1852, 1851, 1970, 2091,
            2092, 1973, 1874, 1899, 2114, 1995, 2018, 2139, 1900, 1877,
            1996, 2117, 2140, 2021, 2452, 2451, 2474, 2499, 2500, 2477,
            3170, 3173, 4610, 3891, 3892, 4613, 3218, 3195, 3914, 4635,
            4658, 3939, 3196, 3221, 4636, 3917, 3940, 4661, 2570, 2573,
            2810, 2691, 2692, 2813, 3292, 3291, 4010, 4731, 4732, 4013,
            3410, 3531, 4850, 4131, 4250, 4971, 3532, 3413, 4132, 4853,
            4972, 4253, 2618, 2595, 2714, 2835, 2858, 2739, 3314, 3339,
            4754, 4035, 4058, 4779, 3554, 3435, 4154, 4875, 4994, 4275,
            3458, 3579, 4898, 4179, 4298, 5019, 2596, 2621, 2836, 2717,
            2740, 2861, 3340, 3317, 4036, 4757, 4780, 4061, 3436, 3557,
            4876, 4157, 4276, 4997, 3580, 3461, 4180, 4901, 5020, 4301,
            870, 871, 882, 877, 876, 883, 894, 895, 918, 943,
            942, 919, 900, 907, 948, 925, 930, 955, 906, 901,
            924, 949, 954, 931, 990, 991, 996, 1003, 1002, 997,
            1110, 1111, 1350, 1231, 1230, 1351, 1122, 1117, 1236, 1357,
            1362, 1243, 1116, 1123, 1356, 1237, 1242, 1363, 1014, 1015,
            1062, 1039, 1038, 1063, 1134, 1135, 1254, 1375, 1374, 1255,
            1158, 1183, 1398, 1279, 1302, 1423, 1182, 1159, 1278, 1399,
            1422, 1303, 1026, 1021, 1044, 1069, 1074, 1051, 1140, 1147,
            1380, 1261, 1266, 1387, 1188, 1165, 1284, 1405, 1428, 1309,
            1170, 1195, 1410, 1291, 1314, 1435, 1020, 1027, 1068, 1045,
            1050, 1075, 1146, 1141, 1260, 1381, 1386, 1267, 1164, 1189,
            1404, 1285, 1308, 1429, 1194, 1171, 1290, 1411, 1434, 1315,
            1590, 1591, 1596, 1603, 1602, 1597, 1614, 1615, 1662, 1639,
            1638, 1663, 1626, 1621, 1644, 1669, 1674, 1651, 1620, 1627,
            1668, 1645, 1650, 1675, 2310, 2311, 2322, 2317, 2316, 2323,
            3030, 3031, 3750, 4471, 4470, 3751, 3036, 3043, 4476, 3757,
            3762, 4483, 3042, 3037, 3756, 4477, 4482, 3763, 2334, 2335,
            2358, 2383, 2382, 2359, 3054, 3055, 4494, 3775, 3774, 4495,
            3102, 3079, 3798, 4519, 4542, 3823, 3078, 3103, 4518, 3799,
            3822, 4543, 2340, 2347, 2388, 2365, 2370, 2395, 3066, 3061,
            3780, 4501, 4506, 3787, 3084, 3109, 4524, 3805, 3828, 4549,
            3114, 3091, 3810, 4531, 4554, 3835, 2346, 2341, 2364, 2389,
            2394, 2371, 3060, 3067, 4500, 3781, 3786, 4507, 3108, 3085,
            3804, 4525, 4548, 3829, 3090, 3115, 4530, 3811, 3834, 4555,
            1710, 1711, 1722, 1717, 1716, 1723, 1830, 1831, 1950, 2071,
            2070, 1951, 1836, 1843, 2076, 1957, 1962, 2083, 1842, 1837,
            1956, 2077, 2082, 1963, 2430, 2431, 2436, 2443, 2442, 2437,
            3150, 3151, 4590, 3871, 3870, 4591, 3162, 3157, 3876, 4597,
            4602, 3883, 3156, 3163, 4596, 3877, 3882, 4603, 2550, 2551,
            2790, 2671, 2670, 2791, 3270, 3271, 3990, 4711, 4710, 3991,
            3390, 3511, 4830, 4111, 4230, 4951, 3510, 3391, 4110, 4831,
            4950, 4231, 2562, 2557, 2676, 2797, 2802, 2683, 3276, 3283,
            4716, 3997, 4002, 4723, 3516, 3397, 4116, 4837, 4956, 4237,
            3402, 3523, 4842, 4123, 4242, 4963, 2556, 2563, 2796, 2677,
            2682, 2803, 3282, 3277, 3996, 4717, 4722, 4003, 3396, 3517,
            4836, 4117, 4236, 4957, 3522, 3403, 4122, 4843, 4962, 4243,
            1734, 1735, 1758, 1783, 1782, 1759, 1854, 1855, 2094, 1975,
            1974, 2095, 1902, 1879, 1998, 2119, 2142, 2023, 1878, 1903,
            2118, 1999, 2022, 2143, 2454, 2455, 2502, 2479, 2478, 2503,
            3174, 3175, 3894, 4615, 4614, 3895, 3198, 3223, 4638, 3919,
            3942, 4663, 3222, 3199, 3918, 4639, 4662, 3943, 2574, 2575,
            2694, 2815, 2814, 2695, 3294, 3295, 4734, 4015, 4014, 4735,
            3534, 3415, 4134, 4855, 4974, 4255, 3414, 3535, 4854, 4135,
            4254, 4975, 2598, 2623, 2838, 2719, 2742, 2863, 3342, 3319,
            4038, 4759, 4782, 4063, 3438, 3559, 4878, 4159, 4278, 4999,
            3582, 3463, 4182, 4903, 5022, 4303, 2622, 2599, 2718, 2839,
            2862, 2743, 3318, 3343, 4758, 4039, 4062, 4783, 3558, 3439,
            4158, 4879, 4998, 4279, 3462, 3583, 4902, 4183, 4302, 5023,
            1740, 1747, 1788, 1765, 1770, 1795, 1866, 1861, 1980, 2101,
            2106, 1987, 1884, 1909, 2124, 2005, 2028, 2149, 1914, 1891,
            2010, 2131, 2154, 2035, 2466, 2461, 2484, 2509, 2514, 2491,
            3180, 3187, 4620, 3901, 3906, 4627, 3228, 3205, 3924, 4645,
            4668, 3949, 3210, 3235, 4650, 3931, 3954, 4675, 2580, 2587,
            2820, 2701, 2706, 2827, 3306, 3301, 4020, 4741, 4746, 4027,
            3420, 3541, 4860, 4141, 4260, 4981, 3546, 3427, 4146, 4867,
            4986, 4267, 2628, 2605, 2724, 2845, 2868, 2749, 3324, 3349,
            4764, 4045, 4068, 4789, 3564, 3445, 4164, 4885, 5004, 4285,
            3468, 3589, 4908, 4189, 4308, 5029, 2610, 2635, 2850, 2731,
            2754, 2875, 3354, 3331, 4050, 4771, 4794, 4075, 3450, 3571,
            4890, 4171, 4290, 5011, 3594, 3475, 4194, 4915, 5034, 4315,
            1746, 1741, 1764, 1789, 1794, 1771, 1860, 1867, 2100, 1981,
            1986, 2107, 1908, 1885, 2004, 2125, 2148, 2029, 1890, 1915,
            2130, 2011, 2034, 2155, 2460, 2467, 2508, 2485, 2490, 2515,
            3186, 3181, 3900, 4621, 4626, 3907, 3204, 3229, 4644, 3925,
            3948, 4669, 3234, 3211, 3930, 4651, 4674, 3955, 2586, 2581,
            2700, 2821, 2826, 2707, 3300, 3307, 4740, 4021, 4026, 4747,
            3540, 3421, 4140, 4861, 4980, 4261, 3426, 3547, 4866, 4147,
            4266, 4987, 2604, 2629, 2844, 2725, 2748, 2869, 3348, 3325,
            4044, 4765, 4788, 4069, 3444, 3565, 4884, 4165, 4284, 5005,
            3588, 3469, 4188, 4909, 5028, 4309, 2634, 2611, 2730, 2851,
            2874, 2755, 3330, 3355, 4770, 4051, 4074, 4795, 3570, 3451,
            4170, 4891, 5010, 4291, 3474, 3595, 4914, 4195, 4314, 5035,
            874, 873, 878, 885, 886, 881, 896, 899, 944, 921,
            922, 947, 908, 903, 926, 951, 956, 933, 904, 911,
            952, 929, 934, 959, 992, 995, 1004, 999, 1000, 1007,
            1114, 1113, 1232, 1353, 1354, 1235, 1118, 1125, 1358, 1239,
            1244, 1365, 1126, 1121, 1240, 1361, 1366, 1247, 1018, 1017,
            1040, 1065, 1066, 1043, 1136, 1139, 1376, 1257, 1258, 1379,
            1184, 1161, 1280, 1401, 1424, 1305, 1162, 1187, 1402, 1283,
            1306, 1427, 1022, 1029, 1070, 1047, 1052, 1077, 1148, 1143,
            1262, 1383, 1388, 1269, 1166, 1191, 1406, 1287, 1310, 1431,
            1196, 1173, 1292, 1413, 1436, 1317, 1030, 1025, 1048, 1073,
            1078, 1055, 1144, 1151, 1384, 1265, 1270, 1391, 1192, 1169,
            1288, 1409, 1432, 1313, 1174, 1199, 1414, 1295, 1318, 1439,
            1592, 1595, 1604, 1599, 1600, 1607, 1618, 1617, 1640, 1665,
            1666, 1643, 1622, 1629, 1670, 1647, 1652, 1677, 1630, 1625,
            1648, 1673, 1678, 1655, 2314, 2313, 2318, 2325, 2326, 2321,
            3032, 3035, 4472, 3753, 3754, 4475, 3044, 3039, 3758, 4479,
            4484, 3765, 3040, 3047, 4480, 3761, 3766, 4487, 2336, 2339,
            2384, 2361, 2362, 2387, 3058, 3057, 3776, 4497, 4498, 3779,
            3080, 3105, 4520, 3801, 3824, 4545, 3106, 3083, 3802, 4523,
            4546, 3827, 2348, 2343, 2366, 2391, 2396, 2373, 3062, 3069,
            4502, 3783, 3788, 4509, 3110, 3087, 3806, 4527, 4550, 3831,
            3092, 3117, 4532, 3813, 3836, 4557, 2344, 2351, 2392, 2369,
            2374, 2399, 3070, 3065, 3784, 4505, 4510, 3791, 3088, 3113,
            4528, 3809, 3832, 4553, 3118, 3095, 3814, 4535, 4558, 3839,
            1714, 1713, 1718, 1725, 1726, 1721, 1832, 1835, 2072, 1953,
            1954, 2075, 1844, 1839, 1958, 2079, 2084, 1965, 1840, 1847,
            2080, 1961, 1966, 2087, 2432, 2435, 2444, 2439, 2440, 2447,
            3154, 3153, 3872, 4593, 4594, 3875, 3158, 3165, 4598, 3879,
            3884, 4605, 3166, 3161, 3880, 4601, 4606, 3887, 2554, 2553,
            2672, 2793, 2794, 2675, 3272, 3275, 4712, 3993, 3994, 4715,
            3512, 3393, 4112, 4833, 4952, 4233, 3394, 3515, 4834, 4115,
            4234, 4955, 2558, 2565, 2798, 2679, 2684, 2805, 3284, 3279,
            3998, 4719, 4724, 4005, 3398, 3519, 4838, 4119, 4238, 4959,
            3524, 3405, 4124, 4845, 4964, 4245, 2566, 2561, 2680, 2801,
            2806, 2687, 3280, 3287, 4720, 4001, 4006, 4727, 3520, 3401,
            4120, 4841, 4960, 4241, 3406, 3527, 4846, 4127, 4246, 4967,
            1736, 1739, 1784, 1761, 1762, 1787, 1858, 1857, 1976, 2097,
            2098, 1979, 1880, 1905, 2120, 2001, 2024, 2145, 1906, 1883,
            2002, 2123, 2146, 2027, 2458, 2457, 2480, 2505, 2506, 2483,
            3176, 3179, 4616, 3897, 3898, 4619, 3224, 3201, 3920, 4641,
            4664, 3945, 3202, 3227, 4642, 3923, 3946, 4667, 2576, 2579,
            2816, 2697, 2698, 2819, 3298, 3297, 4016, 4737, 4738, 4019,
            3416, 3537, 4856, 4137, 4256, 4977, 3538, 3419, 4138, 4859,
            4978, 4259, 2624, 2601, 2720, 2841, 2864, 2745, 3320, 3345,
            4760, 4041, 4064, 4785, 3560, 3441, 4160, 4881, 5000, 4281,
            3464, 3585, 4904, 4185, 4304, 5025, 2602, 2627, 2842, 2723,
            2746, 2867, 3346, 3323, 4042, 4763, 4786, 4067, 3442, 3563,
            4882, 4163, 4282, 5003, 3586, 3467, 4186, 4907, 5026, 4307,
            1748, 1743, 1766, 1791, 1796, 1773, 1862, 1869, 2102, 1983,
            1988, 2109, 1910, 1887, 2006, 2127, 2150, 2031, 1892, 1917,
            2132, 2013, 2036, 2157, 2462, 2469, 2510, 2487, 2492, 2517,
            3188, 3183, 3902, 4623, 4628, 3909, 3206, 3231, 4646, 3927,
            3950, 4671, 3236, 3213, 3932, 4653, 4676, 3957, 2588, 2583,
            2702, 2823, 2828, 2709, 3302, 3309, 4742, 4023, 4028, 4749,
            3542, 3423, 4142, 4863, 4982, 4263, 3428, 3549, 4868, 4149,
            4268, 4989, 2606, 2631, 2846, 2727, 2750, 2871, 3350, 3327,
            4046, 4767, 4790, 4071, 3446, 3567, 4886, 4167, 4286, 5007,
            3590, 3471, 4190, 4911, 5030, 4311, 2636, 2613, 2732, 2853,
            2876, 2757, 3332, 3357, 4772, 4053, 4076, 4797, 3572, 3453,
            4172, 4893, 5012, 4293, 3476, 3597, 4916, 4197, 4316, 5037,
            1744, 1751, 1792, 1769, 1774, 1799, 1870, 1865, 1984, 2105,
            2110, 1991, 1888, 1913, 2128, 2009, 2032, 2153, 1918, 1895,
            2014, 2135, 2158, 2039, 2470, 2465, 2488, 2513, 2518, 2495,
            3184, 3191, 4624, 3905, 3910, 4631, 3232, 3209, 3928, 4649,
            4672, 3953, 3214, 3239, 4654, 3935, 3958, 4679, 2584, 2591,
            2824, 2705, 2710, 2831, 3310, 3305, 4024, 4745, 4750, 4031,
            3424, 3545, 4864, 4145, 4264, 4985, 3550, 3431, 4150, 4871,
            4990, 4271, 2632, 2609, 2728, 2849, 2872, 2753, 3328, 3353,
            4768, 4049, 4072, 4793, 3568, 3449, 4168, 4889, 5008, 4289,
            3472, 3593, 4912, 4193, 4312, 5033, 2614, 2639, 2854, 2735,
            2758, 2879, 3358, 3335, 4054, 4775, 4798, 4079, 3454, 3575,
            4894, 4175, 4294, 5015, 3598, 3479, 4198, 4919, 5038, 4319,
            872, 875, 884, 879, 880, 887, 898, 897, 920, 945,
            946, 923, 902, 909, 950, 927, 932, 957, 910, 905,
            928, 953, 958, 935, 994, 993, 998, 1005, 1006, 1001,
            1112, 1115, 1352, 1233, 1234, 1355, 1124, 1119, 1238, 1359,
            1364, 1245, 1120, 1127, 1360, 1241, 1246, 1367, 1016, 1019,
            1064, 1041, 1042, 1067, 1138, 1137, 1256, 1377, 1378, 1259,
            1160, 1185, 1400, 1281, 1304, 1425, 1186, 1163, 1282, 1403,
            1426, 1307, 1028, 1023, 1046, 1071, 1076, 1053, 1142, 1149,
            1382, 1263, 1268, 1389, 1190, 1167, 1286, 1407, 1430, 1311,
            1172, 1197, 1412, 1293, 1316, 1437, 1024, 1031, 1072, 1049,
            1054, 1079, 1150, 1145, 1264, 1385, 1390, 1271, 1168, 1193,
            1408, 1289, 1312, 1433, 1198, 1175, 1294, 1415, 1438, 1319,
            1594, 1593, 1598, 1605, 1606, 1601, 1616, 1619, 1664, 1641,
            1642, 1667, 1628, 1623, 1646, 1671, 1676, 1653, 1624, 1631,
            1672, 1649, 1654, 1679, 2312, 2315, 2324, 2319, 2320, 2327,
            3034, 3033, 3752, 4473, 4474, 3755, 3038, 3045, 4478, 3759,
            3764, 4485, 3046, 3041, 3760, 4481, 4486, 3767, 2338, 2337,
            2360, 2385, 2386, 2363, 3056, 3059, 4496, 3777, 3778, 4499,
            3104, 3081, 3800, 4521, 4544, 3825, 3082, 3107, 4522, 3803,
            3826, 4547, 2342, 2349, 2390, 2367, 2372, 2397, 3068, 3063,
            3782, 4503, 4508, 3789, 3086, 3111, 4526, 3807, 3830, 4551,
            3116, 3093, 3812, 4533, 4556, 3837, 2350, 2345, 2368, 2393,
            2398, 2375, 3064, 3071, 4504, 3785, 3790, 4511, 3112, 3089,
            3808, 4529, 4552, 3833, 3094, 3119, 4534, 3815, 3838, 4559,
            1712, 1715, 1724, 1719, 1720, 1727, 1834, 1833, 1952, 2073,
            2074, 1955, 1838, 1845, 2078, 1959, 1964, 2085, 1846, 1841,
            1960, 2081, 2086, 1967, 2434, 2433, 2438, 2445, 2446, 2441,
            3152, 3155, 4592, 3873, 3874, 4595, 3164, 3159, 3878, 4599,
            4604, 3885, 3160, 3167, 4600, 3881, 3886, 4607, 2552, 2555,
            2792, 2673, 2674, 2795, 3274, 3273, 3992, 4713, 4714, 3995,
            3392, 3513, 4832, 4113, 4232, 4953, 3514, 3395, 4114, 4835,
            4954, 4235, 2564, 2559, 2678, 2799, 2804, 2685, 3278, 3285,
            4718, 3999, 4004, 4725, 3518, 3399, 4118, 4839, 4958, 4239,
            3404, 3525, 4844, 4125, 4244, 4965, 2560, 2567, 2800, 2681,
            2686, 2807, 3286, 3281, 4000, 4721, 4726, 4007, 3400, 3521,
            4840, 4121, 4240, 4961, 3526, 3407, 4126, 4847, 4966, 4247,
            1738, 1737, 1760, 1785, 1786, 1763, 1856, 1859, 2096, 1977,
            1978, 2099, 1904, 1881, 2000, 2121, 2144, 2025, 1882, 1907,
            2122, 2003, 2026, 2147, 2456, 2459, 2504, 2481, 2482, 2507,
            3178, 3177, 3896, 4617, 4618, 3899, 3200, 3225, 4640, 3921,
            3944, 4665, 3226, 3203, 3922, 4643, 4666, 3947, 2578, 2577,
            2696, 2817, 2818, 2699, 3296, 3299, 4736, 4017, 4018, 4739,
            3536, 3417, 4136, 4857, 4976, 4257, 3418, 3539, 4858, 4139,
            4258, 4979, 2600, 2625, 2840, 2721, 2744, 2865, 3344, 3321,
            4040, 4761, 4784, 4065, 3440, 3561, 4880, 4161, 4280, 5001,
            3584, 3465, 4184, 4905, 5024, 4305, 2626, 2603, 2722, 2843,
            2866, 2747, 3322, 3347, 4762, 4043, 4066, 4787, 3562, 3443,
            4162, 4883, 5002, 4283, 3466, 3587, 4906, 4187, 4306, 5027,
            1742, 1749, 1790, 1767, 1772, 1797, 1868, 1863, 1982, 2103,
            2108, 1989, 1886, 1911, 2126, 2007, 2030, 2151, 1916, 1893,
            2012, 2133, 2156, 2037, 2468, 2463, 2486, 2511, 2516, 2493,
            3182, 3189, 4622, 3903, 3908, 4629, 3230, 3207, 3926, 4647,
            4670, 3951, 3212, 3237, 4652, 3933, 3956, 4677, 2582, 2589,
            2822, 2703, 2708, 2829, 3308, 3303, 4022, 4743, 4748, 4029,
            3422, 3543, 4862, 4143, 4262, 4983, 3548, 3429, 4148, 4869,
            4988, 4269, 2630, 2607, 2726, 2847, 2870, 2751, 3326, 3351,
            4766, 4047, 4070, 4791, 3566, 3447, 4166, 4887, 5006, 4287,
            3470, 3591, 4910, 4191, 4310, 5031, 2612, 2637, 2852, 2733,
            2756, 2877, 3356, 3333, 4052, 4773, 4796, 4077, 3452, 3573,
            4892, 4173, 4292, 5013, 3596, 3477, 4196, 4917, 5036, 4317,
            1750, 1745, 1768, 1793, 1798, 1775, 1864, 1871, 2104, 1985,
            1990, 2111, 1912, 1889, 2008, 2129, 2152, 2033, 1894, 1919,
            2134, 2015, 2038, 2159, 2464, 2471, 2512, 2489, 2494, 2519,
            3190, 3185, 3904, 4625, 4630, 3911, 3208, 3233, 4648, 3929,
            3952, 4673, 3238, 3215, 3934, 4655, 4678, 3959, 2590, 2585,
            2704, 2825, 2830, 2711, 3304, 3311, 4744, 4025, 4030, 4751,
            3544, 3425, 4144, 4865, 4984, 4265, 3430, 3551, 4870, 4151,
            4270, 4991, 2608, 2633, 2848, 2729, 2752, 2873, 3352, 3329,
            4048, 4769, 4792, 4073, 3448, 3569, 4888, 4169, 4288, 5009,
            3592, 3473, 4192, 4913, 5032, 4313, 2638, 2615, 2734, 2855,
            2878, 2759, 3334, 3359, 4774, 4055, 4078, 4799, 3574, 3455,
            4174, 4895, 5014, 4295, 3478, 3599, 4918, 4199, 4318, 5039
        };

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S7[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[7][7] = {
            // Generated using Regina 6.0.
            { 0, 721, 1561, 2305, 3031, 3753, 4473 },
            { 721, 0, 121, 265, 391, 513, 633 },
            { 1561, 121, 0, 25, 55, 81, 105 },
            { 2305, 265, 25, 0, 7, 15, 21 },
            { 3031, 391, 55, 7, 0, 3, 5 },
            { 3753, 513, 81, 15, 3, 0, 1 },
            { 4473, 633, 105, 21, 5, 1, 0 }
        };

    protected:
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
        constexpr Perm<7>(Code2 code);

    private:
        /**
         * Returns the index into the Perm<7>::S7 array of the permutation that
         * maps (0,...,6) to (<i>a</i>,...,<i>f</i>,<i>g</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>,<i>g</i>}
         * = {0,1,2,3,4,5,6}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         * @param g the desired image of 6.
         * @return the index \a i for which the given permutation is equal to
         * Perm<7>::S7[i].  This will be between 0 and 5039 inclusive.
         */
        static constexpr int S7Index(int a, int b, int c, int d, int e, int f,
            int g);

        /**
         * Converts between an index into Perm<7>::S7 and an index into
         * Perm<7>::orderedS7.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code2.
         */
        template <typename Int>
        static constexpr Int convOrderedUnordered(Int index);

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
         * \e finish at \a limit, or else the encoding will be considered
         * invalid also; if \a noTrailingData is \c false then there is no
         * constraint on the final state of the iterator.
         *
         * \exception InvalidInput the given iterator does not point to
         * a tight encoding of a 7-element permutation.
         *
         * \tparam iterator an input iterator type.
         *
         * @param start an iterator that points to the beginning of a
         * tight encoding.
         * @param limit an iterator that, if reached, indicates that no more
         * characters are available.
         * @param noTrailingData \c true if iteration should reach \a limit
         * immediately after the encoding is read, or \c false if there is
         * allowed to be additional unread data.
         * @return the permutation represented by the given tight encoding.
         */
        template <typename iterator>
        static Perm tightDecode(iterator start, iterator limit,
            bool noTrailingData);
};

// Inline functions for Perm<7>

template <typename Int>
inline constexpr Int Perm<7>::convOrderedUnordered(Int index) {
    // S7 is almost the same as orderedS7, except that some pairs
    // S7[2i] <--> S7[2i+1] have been swapped to ensure that all
    // permutations S7[2i] are even and all permutations S7[2i+1] are odd.
    //
    // Specifically, we must flip between 2i <--> 2i+1 if and only if the
    // parities of (S7Index / 2), (S7Index / 24) and (S7Index / 720) XOR to 1.
    // Here we use (index >> 1), which is equivalent to (index / 2).
    //
    // A note for myself for if/when we extend this beyond 7:
    // what matters is the mod 2 sum of the parities of
    // (index / 2!), (index / 4!), (index / 6!), etc., up to (index / (n-1)!).
    //
    return ((((index >> 1) ^ (index / 24) ^ (index / 720)) & 1) ?
        (index ^ 1) : index);
}

inline constexpr Perm<7> Perm<7>::S7Lookup::operator[] (int index) const {
    return Perm<7>(static_cast<Code2>(index));
}

inline constexpr Perm<7> Perm<7>::OrderedS7Lookup::operator[] (int index)
        const {
    return Perm<7>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline constexpr Perm<7>::Perm() : code2_(0) {
}

inline constexpr Perm<7>::Perm(Code2 code) : code2_(code) {
}

inline constexpr Perm<7>::Perm(int a, int b) : code2_(swapTable[a][b]) {
}

inline constexpr Perm<7>::Perm(int a, int b, int c, int d, int e, int f,
        int g) : code2_(static_cast<Code2>(S7Index(a, b, c, d, e, f, g))) {
}

inline constexpr Perm<7>::Perm(const std::array<int, 7>& image) :
        code2_(static_cast<Code2>(S7Index(
            image[0], image[1], image[2], image[3], image[4], image[5],
            image[6]))) {
}

inline constexpr Perm<7>::Perm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1, int f0, int f1,
        int g0, int g1) :
        code2_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[7] = { 0, 0, 0, 0, 0, 0, 0 };
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    image[e0] = e1;
    image[f0] = f1;
    image[g0] = g1;
    code2_ = static_cast<Code2>(S7Index(
        image[0], image[1], image[2], image[3], image[4], image[5], image[6]));
}

inline constexpr Perm<7>::Code1 Perm<7>::permCode1() const {
    // This basically follows the implementation of operator [], but reuses
    // the parts of the computation that are common to each image calculation.
    int slice = code2_ / 720;
    Perm<6> suffix = Perm<6>::Sn[(code2_ % 720) ^ (slice & 1)];

    Code1 ans = slice;
    for (int i = 0; i < 6; ++i) {
        int img = suffix[i];
        if (img >= slice)
            ++img;
        ans |= (static_cast<Code1>(img) << (3 * (i + 1)));
    }

    return ans;
}

inline constexpr Perm<7>::Code2 Perm<7>::permCode2() const {
    return code2_;
}

inline void Perm<7>::setPermCode1(Code1 code) {
    code2_ = static_cast<Code2>(S7Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07,
        (code >> 18) & 0x07));
}

inline void Perm<7>::setPermCode2(Code2 code) {
    code2_ = code;
}

inline constexpr Perm<7> Perm<7>::fromPermCode1(Code1 code) {
    return Perm<7>(static_cast<Code2>(S7Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07,
        (code >> 18) & 0x07)));
}

inline constexpr Perm<7> Perm<7>::fromPermCode2(Code2 code) {
    return Perm<7>(code);
}

inline constexpr bool Perm<7>::isPermCode1(Code1 code) {
    unsigned mask = 0;
    for (int i = 0; i < 7; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 127 && (code >> 21) == 0);
}

inline constexpr bool Perm<7>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 5040);
}

inline constexpr Perm<7>::ImagePack Perm<7>::imagePack() const {
    return permCode1();
}

inline constexpr Perm<7> Perm<7>::fromImagePack(ImagePack pack) {
    return fromPermCode1(pack);
}

inline constexpr bool Perm<7>::isImagePack(ImagePack pack) {
    return isPermCode1(pack);
}

inline constexpr Perm<7> Perm<7>::operator *(const Perm<7>& q) const {
    // TODO: Speed this up.
    // We are not fussing too hard about this right now, since if you
    // really care about speed then you should be using cachedComp() instead.
    // Still: at the very least, there is code within operator [] that can be
    // reused instead of being recomputed for all seven images below.
    return Perm<7>((*this)[q[0]], (*this)[q[1]], (*this)[q[2]], (*this)[q[3]],
        (*this)[q[4]], (*this)[q[5]], (*this)[q[6]]);
}

inline Perm<7> Perm<7>::cachedComp(const Perm<7>& q) const {
    return Perm<7>(products_[code2_][q.code2_]);
}

inline Perm<7> Perm<7>::cachedComp(const Perm<7>& q, const Perm<7>& r) const {
    return Perm<7>(products_[code2_][products_[q.code2_][r.code2_]]);
}

inline constexpr Perm<7> Perm<7>::inverse() const {
    return Perm<7>(invS7[code2_]);
}

inline constexpr Perm<7> Perm<7>::pow(long exp) const {
    // We will just iterate the exponentiation here.
    // The maximum order is 12, so this will involve at most 6 compositions.
    // If you really need this to be fast, use cachedPow() instead.

    int ord = order();

    exp %= ord;
    if (exp == 0)
        return Perm<7>();

    // Normalise exp to be in the range ( -order/2, +order/2 ].
    if (exp < 0)
        exp += ord;
    if ((exp << 1) > ord)
        exp -= ord;

    if (exp > 0) {
        Perm<7> p = *this;
        --exp;
        while (exp) {
            p = p * (*this);
            --exp;
        }
        return p;
    } else {
        Perm<7> inv = inverse();
        Perm<7> p = inv;
        ++exp;
        while (exp) {
            p = p * inv;
            ++exp;
        }
        return p;
    }
}

inline Perm<7> Perm<7>::cachedPow(long exp) const {
    // Normalise exp in the range [0, order), and look up the result.
    int ord = cachedOrder();
    exp %= ord;
    if (exp == 0)
        return Perm<7>();
    if (exp < 0)
        exp += ord;
    return Perm<7>(powers_[code2_][exp]);
}

inline constexpr int Perm<7>::order() const {
    // TODO: Speed this up.
    // We are not fussing too hard about optimisation here, because if you
    // really care about speed then you would be using cachedOrder() instead.
    //
    // Still: there should be optimisations possible that look at the orders
    // of individual elements - in particular, how many elements have orders
    // 1, 2 and 3.  Using these counts, we should be able to work out the
    // order completely without powering any element beyond its cube.
    //
    // The possible overall orders, and the corresponding element orders, are:
    //
    // Order 1: orders 1 1 1 1 1 1 1
    // Order 2: orders 1 1 1 1 1 2 2, or 1 1 1 2 2 2 2, or 1 2 2 2 2 2 2
    // Order 3: orders 1 1 1 1 3 3 3, or 1 3 3 3 3 3 3
    // Order 4: orders 1 1 1 4 4 4 4, or 1 2 2 4 4 4 4
    // Order 5: orders 1 1 5 5 5 5 5
    // Order 6: orders 1 1 2 2 3 3 3, or 2 2 2 2 3 3 3, or 1 6 6 6 6 6 6
    // Order 7: orders 7 7 7 7 7 7 7
    // Order 10: orders 2 2 5 5 5 5 5
    // Order 12: orders 3 3 3 4 4 4 4

    // But anyway... for now, here is the brute force implementation, which
    // in the worse case performs a (costly) composition operation 12 times.

    Perm<7> p = *this;
    int ans = 1;
    while (! p.isIdentity()) {
        p = p * (*this);
        ++ans;
    }
    return ans;
}

inline int Perm<7>::cachedOrder() const {
    return orders_[code2_];
}

inline constexpr Perm<7> Perm<7>::reverse() const {
    // p becomes p * 6543210 (which has second-generation code 5039).
    return (*this) * Perm<7>(static_cast<Code2>(5039));
}

inline constexpr int Perm<7>::sign() const {
    return (code2_ % 2 ? -1 : 1);
}

inline constexpr int Perm<7>::operator[](int src) const {
    // We are trying to fly without a hard-coded table as best we can.
    // Our strategy: use the hard-coded tables from Perm6, which only
    // require a small amount of work to adapt to Perm7.
    int slice = code2_ / 720;
    if (src == 0) {
        return slice;
    } else {
        int img = Perm<6>::Sn[(code2_ % 720) ^ (slice & 1)][src - 1];
        return (img < slice ? img : img + 1);
    }
}

inline constexpr int Perm<7>::pre(int image) const {
    return inverse()[image];
}

inline constexpr bool Perm<7>::operator == (const Perm<7>& other) const {
    return (code2_ == other.code2_);
}

inline constexpr bool Perm<7>::operator != (const Perm<7>& other) const {
    return (code2_ != other.code2_);
}

inline constexpr int Perm<7>::compareWith(const Perm<7>& other) const {
    // Computing orderedS7Index() is very fast, now that we use S7 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS7Index();
    int o2 = other.orderedS7Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<7>::isIdentity() const {
    return (code2_ == 0);
}

inline Perm<7>& Perm<7>::operator ++() {
    if (++code2_ == 5040)
        code2_ = 0;
    return *this;
}

inline constexpr Perm<7> Perm<7>::operator ++(int) {
    Perm<7> ans(code2_);
    if (++code2_ == 5040)
        code2_ = 0;
    return ans;
}

inline constexpr bool Perm<7>::operator < (const Perm<7>& rhs) const {
    return code2_ < rhs.code2_;
}

inline constexpr Perm<7> Perm<7>::rot(int i) {
    switch (i) {
        case 1: return Perm<7>(static_cast<Code2>(872));
        case 2: return Perm<7>(static_cast<Code2>(1744));
        case 3: return Perm<7>(static_cast<Code2>(2610));
        case 4: return Perm<7>(static_cast<Code2>(3456));
        case 5: return Perm<7>(static_cast<Code2>(4200));
        case 6: return Perm<7>(static_cast<Code2>(4320));
        default: return Perm<7>(); // Identity
    }
}

inline Perm<7> Perm<7>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<7> Perm<7>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 2519);
        return S7[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 5039);
        return S7[d(gen)];
    }
}

inline void Perm<7>::tightEncode(std::ostream& out) const {
    // Write the Sn index in base 94, least significant digit first.
    // Note: 94^2 = 8836 > 7! = 5040
    out << static_cast<char>((code2_ % 94) + 33)
        << static_cast<char>((code2_ / 94) + 33);
}

inline std::string Perm<7>::tightEncoding() const {
    // Write the Sn index in base 94, least significant digit first.
    // Note: 94^2 = 8836 > 7! = 5040
    char ans[3] {
        static_cast<char>((code2_ % 94) + 33),
        static_cast<char>((code2_ / 94) + 33),
        0 };
    return ans;
}

inline Perm<7> Perm<7>::tightDecode(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

inline Perm<7> Perm<7>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

template <typename iterator>
Perm<7> Perm<7>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    // All codes are >= 0 because we are using an unsigned data type.
    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    Code2 code0 = (*start++) - 33;
    if (code0 >= 94)
        throw InvalidInput("The tight encoding is invalid");

    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    Code2 code1 = (*start++) - 33;
    if (code1 > 53 /* (7! / 94) */)
        throw InvalidInput("The tight encoding is invalid");

    code0 += (94 * code1);
    if (code0 >= 5040 /* 7! */)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Perm<7>(code0);
}

inline constexpr Perm<7>::Index Perm<7>::S7Index() const {
    return code2_;
}

inline constexpr Perm<7>::Index Perm<7>::SnIndex() const {
    return code2_;
}

inline constexpr Perm<7>::Index Perm<7>::orderedS7Index() const {
    return convOrderedUnordered(code2_);
}

inline constexpr Perm<7>::Index Perm<7>::orderedSnIndex() const {
    return convOrderedUnordered(code2_);
}

inline constexpr int Perm<7>::S7Index(int a, int b, int c, int d, int e,
        int f, int g) {
    // First compute the ordered S7 index.
    int ans = 720 * a +
              120 * (b - (b > a ? 1 : 0)) +
              24 * (c - ((c > b ? 1 : 0) + (c > a ? 1 : 0))) +
              6 * ((d > e ? 1 : 0) + (d > f ? 1 : 0) + (d > g ? 1 : 0)) +
              2 * ((e > f ? 1 : 0) + (e > g ? 1 : 0)) +
                  (f > g ? 1 : 0);

    // Then switch to the plain (unordered) S7 index.
    return convOrderedUnordered(ans);
}

inline constexpr bool Perm<7>::isConjugacyMinimal() const {
    switch (code2_) {
        // Put the 15 minimal cases in a switch statement, which may help
        // the compiler to optimise this better than we can.
        case 0: case 1: case 2: case 6: case 9: case 27:
        case 32: case 127: case 128: case 146: case 153:
        case 746: case 753: case 849: case 872:
            return true;
        default:
            return false;
    }
}

} // namespace regina

#endif

