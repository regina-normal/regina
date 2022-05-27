
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
 *  \brief Internal header for permutations of {0,1,2,3,4,5}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM6_H
#ifndef __DOXYGEN
#define __REGINA_PERM6_H
#endif

namespace regina {

/**
 * Represents a permutation of {0,1,2,3,4,5}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 5-dimensional triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<6>, the internal permutation
 * codes have changed as of Regina 7.0:
 *
 * - \e First-generation codes were used internally in Regina 6.0.1 and earlier.
 *   These are <i>image packs</i>: integers whose lowest three bits represent
 *   the image of 0, whose next lowest three bits represent the image of 1,
 *   and so on.  The routines permCode1(), setPermCode1(), fromPermCode1()
 *   and isPermCode1() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe gluings between
 *   5-simplices.
 *
 * - \e Second-generation codes are used internally in Regina 7.0 and above.
 *   These codes are integers between 0 and 719 inclusive, representing the
 *   index of the permutation in the array Perm<6>::S6.  The routines
 *   permCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by Perm<6>).
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm6.
 *
 * \ingroup maths
 */
template <>
class Perm<6> {
    private:
        /**
         * An array-like object used to implement Perm<6>::S6.
         */
        struct S6Lookup {
            /**
             * Returns the permutation at the given index in the array S6.
             * See Perm<6>::S6 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 719 inclusive.
             * @return the corresponding permutation in S6.
             */
            constexpr Perm<6> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<6>::orderedS6.
         */
        struct OrderedS6Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS6.  See Perm<6>::orderedS6 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 719 inclusive.
             * @return the corresponding permutation in orderedS6.
             */
            constexpr Perm<6> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on six elements.  In other words, this is a
         * native signed integer type large enough to store (6!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on six elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 720;

        /**
         * The total number of permutations on five elements.
         * This is the size of the symmetric group <i>S<sub>5</sub></i>.
         */
        static constexpr Index nPerms_1 = 120;

        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines 6 such images together, and so uses
         * 6 * \a imageBits bits in total.
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
         * six elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 719 inclusive.
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
         * This is different from Perm<6>::orderedSn, since this array \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         */
        static constexpr S6Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * six elements.
         *
         * This is a dimension-specific alias for Perm<6>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<6>::S6 are only available for small \a n.
         */
        static constexpr S6Lookup S6 {};

        /**
         * Gives array-like access to all possible permutations of six
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 719 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[5]).
         *
         * Accessing elements through this object is extremely fast.
         * The object that is returned is lightweight and is defined in the
         * headers only; in particular, you cannot make a reference to it
         * (but you can always make a copy).
         *
         * This is different from Perm<6>::Sn, since this array \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         */
        static constexpr OrderedS6Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of six
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<6>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<6>::orderedS6 are only
         * available for small \a n.
         */
        static constexpr OrderedS6Lookup orderedS6 {};

    protected:
        Code2 code2_;
            /**< The internal second-generation permutation code
                 representing this permutation. */

    private:
        static Code2 (*products_)[720];
            /**< The precomputed table of products, or \c null if this
                 has not yet been generated.  This is a 720-by-720 array,
                 whose indices and values all represent indices into S6. */

    public:
        /**
         * Performs the precomputation necessary for using the optimised
         * cachedComp() and cachedPow() routines.
         *
         * This \e must be called before calling cachedComp() or cachedPow().
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
         * \pre \a a and \a b are in {0,1,2,3,4,5}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3,4,5) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>} =
         * {0,1,2,3,4,5}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         */
        constexpr Perm(int a, int b, int c, int d, int e, int f);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3,4,5.
         *
         * \pre The elements of \a image are 0, 1, 2, 3, 4 and 5 in some order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 6>& image);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>)
         * respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>,<i>f0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>,<i>f1</i>} =
         * {0,1,2,3,4,5}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param e0 the desired preimage of <i>e1</i>.
         * @param f0 the desired preimage of <i>f1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         * @param e1 the desired image of <i>e0</i>.
         * @param f1 the desired image of <i>f0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1,
            int d0, int d1, int e0, int e1, int f0, int f1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<6>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<6> now uses second-generation codes internally.
         * See the class notes and the routine permCode2() for details.
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
         * used internally by the Perm<6> class.
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
         * Perm<6> now uses second-generation codes internally.
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
         * used internally by the Perm<6> class.
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
         * Perm<6> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<6> fromPermCode1(Code1 code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<6> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<6> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode1() or fromPermCode1(), and are returned by permCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<6> now uses second-generation codes internally.
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
         * used internally by the Perm<6> class.
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
         * For Perm<6>, this routine is identical to permCode1().
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
         * For Perm<6>, this routine is identical to fromPermCode1().
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
         * some 6-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<6>, this routine is identical to isPermCode1().
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
        Perm<6>& operator = (const Perm<6>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * For permutations of five and fewer objects, composition is
         * extremely fast because it uses hard-coded lookup tables.
         * However, for Perm<6> these tables would grow too large and so
         * Regina adopts a hybrid approach: it uses "partial tables" which
         * are significantly smaller, combined with a small amount of
         * computation.
         *
         * If you do need your compositions to be as fast as possible,
         * with no computation required at all, then you can:
         *
         * - call precompute() to precompute a full 720-by-720 lookup table
         *   in advance (this will consume roughly 1MB of memory); and then
         *
         * - call cachedComp() instead of the * operator to compute your
         *   compositions.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        constexpr Perm<6> operator * (const Perm<6>& q) const;

        /**
         * Returns the composition of this and the given permutation,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp()
         * is a single lookup, whereas the * operator requires two
         * lookups and a few steps of mathematical computation.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup table
         * will consume roughly 1MB of memory for the lifetime of your program.
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
        Perm<6> cachedComp(const Perm<6>& q) const;

        /**
         * Returns the composition of this and the given two permutations,
         * using fast precomputed lookup tables.
         *
         * The advantage of this routine is speed: calling cachedComp()
         * with two arguments requires just two lookups, whereas using
         * the * operator twice would involve four lookups and a handful
         * of steps of mathematical computation.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup table
         * will consume roughly 1MB of memory for the lifetime of your program.
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
        Perm<6> cachedComp(const Perm<6>& q, const Perm<6>& r) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<6> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * For Perm<6>, this routine makes use of the "partial" product tables,
         * which (as seen with the composition operator) require some small
         * amount of extra computation to use.  If you need your powers to be
         * as fast as possible, you can instead:
         *
         * - call precompute() to precompute a full 720-by-720 product table
         *   in advance (this will consume roughly 1MB of memory); and then
         *
         * - call cachedPow() instead of pow() to make full use of this table,
         *   which will remove some (but not all) of the mathematical overhead
         *   from this routine.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<6> pow(long exp) const;

        /**
         * Computes the given power of this permutation, using fast
         * precomputed lookup tables.
         *
         * This routine runs in constant time.
         *
         * The advantage of this routine is speed: calling cachedPow() removes
         * some (but not all) of the mathematical overhead required by pow().
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the resulting lookup table
         * will consume roughly 1MB of memory for the lifetime of your program.
         * Note that this is the same lookup table used by cachedComp(), so if
         * you are already using cachedComp() then there is no extra cost for
         * using this routine also.
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
        Perm<6> cachedPow(long exp) const;

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
         * Here \e reverse means that we reverse the images of 0,...,5.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[5 - i]</tt> for all \a i.
         */
        constexpr Perm<6> reverse() const;

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
         * should be between 0 and 5 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 5 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2, 3, 4 and 5.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<6>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2, 3, 4 or 5.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<6>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4,5) under this
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
        constexpr int compareWith(const Perm<6>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2, 3, 4 and 5 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<6>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<6>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<6>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<6>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<6>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<6> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<6>::Sn.
         *
         * Note that this is \e not the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<6>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 6)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 5 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on six elements.
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
         * Returns a random permutation on six elements, using the
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
         * The representation will consist of six adjacent digits
         * representing the images of 0, 1, 2, 3, 4 and 5 respectively.
         * An example of a string representation is <tt>304521</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 6 inclusive.
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
         * of a 6-element permutation.
         *
         * @param enc the tight encoding for a 6-element permutation.
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
         * a tight encoding of a 6-element permutation.
         *
         * \ifacespython Not present, but the string version of this routine
         * is available.
         *
         * @param input an input stream that begins with the tight encoding
         * for a 6-element permutation.
         * @return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,5,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,5 are exactly
         * <i>from</i>,...,5, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 6 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<6>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<6>::Sn[i].  This will be between 0 and 719 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<6>::S6 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<6>::S6Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<6>::S6[i].  This will be between 0 and 719 inclusive.
         */
        constexpr Index S6Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<6>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 719 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<6>::orderedSn array.
         *
         * This is a dimension-specific alias for orderedSnIndex().
         * In general, for every \a n there will be a member function
         * Perm<n>::orderedSnIndex(); however, these numerical aliases
         * Perm<2>::orderedS2Index(), ..., Perm<6>::orderedS6Index() are
         * only available for small \a n.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 719 inclusive.
         */
        constexpr Index orderedS6Index() const;

        /**
         * Extends a <i>k</i>-element permutation to a 6-element permutation,
         * where 2 &le; \a k &lt; 6.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,5 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2, 3, 4 or 5.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation expressed as a permutation on
         * six elements.
         */
        template <int k>
        static constexpr Perm<6> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 6-element
         * permutation, where \a k &gt; 6.
         *
         * The resulting permutation will map 0,...,5 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[6],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,5 to 0,...,5 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 6.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 6 elements.
         */
        template <int k>
        static constexpr Perm<6> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<6>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<6>, since it essentially
         * uses a hard-coded lookup table.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

    private:
        /**
         * Contains the first-generation permutation codes for all
         * permutations in S6.
         *
         * Specifically, the permutation <tt>S6[i]</tt> has
         * first-generation code <tt>code1Table[i]</tt>.
         */
        static constexpr int code1Table[720] = {
            // Generated using Regina 6.0.
            181896, 153224, 120968, 178312, 146056, 117384, 152776, 181448,
            174280, 88264, 84680, 142024, 177416, 120072, 87816, 173832,
            109320, 80648, 116040, 144712, 141128, 83784, 80200, 108872,
            153168, 181840, 178256, 120912, 117328, 146000, 180944, 152272,
            55504, 170192, 137936, 51920, 119568, 176912, 169744, 55056,
            47888, 105232, 144208, 115536, 51024, 137040, 104784, 47440,
            181336, 152664, 88152, 174168, 141912, 84568, 152216, 180888,
            170136, 55448, 51864, 137880, 172824, 86808, 54552, 169240,
            72472, 43800, 82776, 140120, 136536, 50520, 43352, 72024,
            119904, 177248, 173664, 87648, 80480, 109152, 176800, 119456,
            54944, 169632, 105120, 47776, 86752, 172768, 169184, 54496,
            43744, 72416, 107360, 78688, 46432, 103776, 71520, 42848,
            144488, 115816, 83560, 140904, 108648, 79976, 115368, 144040,
            136872, 50856, 47272, 104616, 140008, 82664, 50408, 136424,
            71912, 43240, 78632, 107304, 103720, 46376, 42792, 71464,
            153217, 181889, 178305, 120961, 117377, 146049, 181441, 152769,
            88257, 174273, 142017, 84673, 120065, 177409, 173825, 87809,
            80641, 109313, 144705, 116033, 83777, 141121, 108865, 80193,
            181777, 153105, 120849, 178193, 145937, 117265, 151761, 180433,
            166097, 22737, 19153, 133841, 176401, 119057, 22289, 165649,
            101137, 15121, 115025, 143697, 132945, 18257, 14673, 100689,
            152601, 181273, 174105, 88089, 84505, 141849, 180377, 151705,
            22681, 166041, 133785, 19097, 86297, 172313, 165145, 21785,
            11033, 68377, 139609, 82265, 17753, 132441, 67929, 10585,
            177185, 119841, 87585, 173601, 109089, 80417, 118945, 176289,
            165537, 22177, 15009, 101025, 172257, 86241, 21729, 165089,
            68321, 10977, 78177, 106849, 99681, 13665, 10081, 67425,
            115753, 144425, 140841, 83497, 79913, 108585, 143529, 114857,
            18089, 132777, 100521, 14505, 82153, 139497, 132329, 17641,
            10473, 67817, 106793, 78121, 13609, 99625, 67369, 10025,
            181826, 153154, 120898, 178242, 145986, 117314, 152258, 180930,
            170178, 55490, 51906, 137922, 176898, 119554, 55042, 169730,
            105218, 47874, 115522, 144194, 137026, 51010, 47426, 104770,
            153098, 181770, 178186, 120842, 117258, 145930, 180426, 151754,
            22730, 166090, 133834, 19146, 119050, 176394, 165642, 22282,
            15114, 101130, 143690, 115018, 18250, 132938, 100682, 14666,
            180762, 152090, 55322, 170010, 137754, 51738, 151642, 180314,
            165978, 22618, 19034, 133722, 168218, 53530, 21274, 164634,
            35610, 6938, 49498, 135514, 131930, 17242, 6490, 35162,
            119330, 176674, 169506, 54818, 47650, 104994, 176226, 118882,
            22114, 165474, 100962, 14946, 53474, 168162, 164578, 21218,
            6882, 35554, 102754, 45410, 13154, 99170, 34658, 5986,
            143914, 115242, 50730, 136746, 104490, 47146, 114794, 143466,
            132714, 18026, 14442, 100458, 135402, 49386, 17130, 131818,
            35050, 6378, 45354, 102698, 99114, 13098, 5930, 34602,
            152643, 181315, 174147, 88131, 84547, 141891, 180867, 152195,
            55427, 170115, 137859, 51843, 86787, 172803, 169219, 54531,
            43779, 72451, 140099, 82755, 50499, 136515, 72003, 43331,
            181259, 152587, 88075, 174091, 141835, 84491, 151691, 180363,
            166027, 22667, 19083, 133771, 172299, 86283, 21771, 165131,
            68363, 11019, 82251, 139595, 132427, 17739, 10571, 67915,
            152083, 180755, 170003, 55315, 51731, 137747, 180307, 151635,
            22611, 165971, 133715, 19027, 53523, 168211, 164627, 21267,
            6931, 35603, 135507, 49491, 17235, 131923, 35155, 6483,
            172579, 86563, 54307, 168995, 72227, 43555, 86115, 172131,
            164963, 21603, 10851, 68195, 168099, 53411, 21155, 164515,
            35491, 6819, 41315, 69987, 66403, 9059, 5475, 34147,
            82475, 139819, 136235, 50219, 43051, 71723, 139371, 82027,
            17515, 132203, 67691, 10347, 49323, 135339, 131755, 17067,
            6315, 34987, 69931, 41259, 9003, 66347, 34091, 5419,
            177220, 119876, 87620, 173636, 109124, 80452, 119428, 176772,
            169604, 54916, 47748, 105092, 172740, 86724, 54468, 169156,
            72388, 43716, 78660, 107332, 103748, 46404, 42820, 71492,
            119820, 177164, 173580, 87564, 80396, 109068, 176268, 118924,
            22156, 165516, 101004, 14988, 86220, 172236, 165068, 21708,
            10956, 68300, 106828, 78156, 13644, 99660, 67404, 10060,
            176660, 119316, 54804, 169492, 104980, 47636, 118868, 176212,
            165460, 22100, 14932, 100948, 168148, 53460, 21204, 164564,
            35540, 6868, 45396, 102740, 99156, 13140, 5972, 34644,
            86556, 172572, 168988, 54300, 43548, 72220, 172124, 86108,
            21596, 164956, 68188, 10844, 53404, 168092, 164508, 21148,
            6812, 35484, 69980, 41308, 9052, 66396, 34140, 5468,
            107052, 78380, 46124, 103468, 71212, 42540, 77932, 106604,
            99436, 13420, 9836, 67180, 102572, 45228, 12972, 98988,
            34476, 5804, 41196, 69868, 66284, 8940, 5356, 34028,
            115781, 144453, 140869, 83525, 79941, 108613, 144005, 115333,
            50821, 136837, 104581, 47237, 82629, 139973, 136389, 50373,
            43205, 71877, 107269, 78597, 46341, 103685, 71429, 42757,
            144397, 115725, 83469, 140813, 108557, 79885, 114829, 143501,
            132749, 18061, 14477, 100493, 139469, 82125, 17613, 132301,
            67789, 10445, 78093, 106765, 99597, 13581, 9997, 67341,
            115221, 143893, 136725, 50709, 47125, 104469, 143445, 114773,
            18005, 132693, 100437, 14421, 49365, 135381, 131797, 17109,
            6357, 35029, 102677, 45333, 13077, 99093, 34581, 5909,
            139805, 82461, 50205, 136221, 71709, 43037, 82013, 139357,
            132189, 17501, 10333, 67677, 135325, 49309, 17053, 131741,
            34973, 6301, 41245, 69917, 66333, 8989, 5405, 34077,
            78373, 107045, 103461, 46117, 42533, 71205, 106597, 77925,
            13413, 99429, 67173, 9829, 45221, 102565, 98981, 12965,
            5797, 34469, 69861, 41189, 8933, 66277, 34021, 5349
        };

        /**
         * Contains the inverses of the permutations in the array \a S6.
         *
         * Specifically, the inverse of permutation <tt>S6[i]</tt> is
         * the permutation <tt>S6[ invS6[i] ]</tt>.
         */
        static constexpr Code2 invS6[720] = {
            // Generated using Regina 6.0.
            0,   1,   4,   3,   2,   5,   6,   7,   12,  19,  18,  13,
            8,   11,  20,  15,  16,  23,  10,  9,   14,  21,  22,  17,
            24,  25,  26,  29,  28,  27,  48,  49,  96,  73,  72,  97,
            52,  51,  74,  99,  100, 77,  50,  53,  98,  75,  76,  101,
            30,  31,  42,  37,  36,  43,  54,  55,  78,  103, 102, 79,
            60,  67,  108, 85,  90,  115, 66,  61,  84,  109, 114, 91,
            34,  33,  38,  45,  46,  41,  56,  59,  104, 81,  82,  107,
            68,  63,  86,  111, 116, 93,  64,  71,  112, 89,  94,  119,
            32,  35,  44,  39,  40,  47,  58,  57,  80,  105, 106, 83,
            62,  69,  110, 87,  92,  117, 70,  65,  88,  113, 118, 95,
            120, 121, 122, 125, 124, 123, 126, 127, 138, 133, 132, 139,
            130, 129, 134, 141, 142, 137, 128, 131, 140, 135, 136, 143,
            240, 241, 244, 243, 242, 245, 360, 361, 480, 601, 600, 481,
            362, 365, 602, 483, 484, 605, 364, 363, 482, 603, 604, 485,
            246, 247, 252, 259, 258, 253, 366, 367, 606, 487, 486, 607,
            378, 373, 492, 613, 618, 499, 372, 379, 612, 493, 498, 619,
            248, 251, 260, 255, 256, 263, 370, 369, 488, 609, 610, 491,
            374, 381, 614, 495, 500, 621, 382, 377, 496, 617, 622, 503,
            250, 249, 254, 261, 262, 257, 368, 371, 608, 489, 490, 611,
            380, 375, 494, 615, 620, 501, 376, 383, 616, 497, 502, 623,
            144, 145, 148, 147, 146, 149, 168, 169, 192, 217, 216, 193,
            170, 173, 218, 195, 196, 221, 172, 171, 194, 219, 220, 197,
            264, 265, 266, 269, 268, 267, 384, 385, 624, 505, 504, 625,
            388, 387, 506, 627, 628, 509, 386, 389, 626, 507, 508, 629,
            288, 289, 336, 313, 312, 337, 408, 409, 528, 649, 648, 529,
            432, 457, 672, 553, 576, 697, 456, 433, 552, 673, 696, 577,
            292, 291, 314, 339, 340, 317, 410, 413, 650, 531, 532, 653,
            458, 435, 554, 675, 698, 579, 436, 461, 676, 557, 580, 701,
            290, 293, 338, 315, 316, 341, 412, 411, 530, 651, 652, 533,
            434, 459, 674, 555, 578, 699, 460, 437, 556, 677, 700, 581,
            150, 151, 156, 163, 162, 157, 174, 175, 222, 199, 198, 223,
            186, 181, 204, 229, 234, 211, 180, 187, 228, 205, 210, 235,
            270, 271, 282, 277, 276, 283, 390, 391, 510, 631, 630, 511,
            396, 403, 636, 517, 522, 643, 402, 397, 516, 637, 642, 523,
            294, 295, 318, 343, 342, 319, 414, 415, 654, 535, 534, 655,
            462, 439, 558, 679, 702, 583, 438, 463, 678, 559, 582, 703,
            300, 307, 348, 325, 330, 355, 426, 421, 540, 661, 666, 547,
            444, 469, 684, 565, 588, 709, 474, 451, 570, 691, 714, 595,
            306, 301, 324, 349, 354, 331, 420, 427, 660, 541, 546, 667,
            468, 445, 564, 685, 708, 589, 450, 475, 690, 571, 594, 715,
            152, 155, 164, 159, 160, 167, 178, 177, 200, 225, 226, 203,
            182, 189, 230, 207, 212, 237, 190, 185, 208, 233, 238, 215,
            274, 273, 278, 285, 286, 281, 392, 395, 632, 513, 514, 635,
            404, 399, 518, 639, 644, 525, 400, 407, 640, 521, 526, 647,
            296, 299, 344, 321, 322, 347, 418, 417, 536, 657, 658, 539,
            440, 465, 680, 561, 584, 705, 466, 443, 562, 683, 706, 587,
            308, 303, 326, 351, 356, 333, 422, 429, 662, 543, 548, 669,
            470, 447, 566, 687, 710, 591, 452, 477, 692, 573, 596, 717,
            304, 311, 352, 329, 334, 359, 430, 425, 544, 665, 670, 551,
            448, 473, 688, 569, 592, 713, 478, 455, 574, 695, 718, 599,
            154, 153, 158, 165, 166, 161, 176, 179, 224, 201, 202, 227,
            188, 183, 206, 231, 236, 213, 184, 191, 232, 209, 214, 239,
            272, 275, 284, 279, 280, 287, 394, 393, 512, 633, 634, 515,
            398, 405, 638, 519, 524, 645, 406, 401, 520, 641, 646, 527,
            298, 297, 320, 345, 346, 323, 416, 419, 656, 537, 538, 659,
            464, 441, 560, 681, 704, 585, 442, 467, 682, 563, 586, 707,
            302, 309, 350, 327, 332, 357, 428, 423, 542, 663, 668, 549,
            446, 471, 686, 567, 590, 711, 476, 453, 572, 693, 716, 597,
            310, 305, 328, 353, 358, 335, 424, 431, 664, 545, 550, 671,
            472, 449, 568, 689, 712, 593, 454, 479, 694, 575, 598, 719
        };

        /**
         * For Perm<6>, we split the usual hard-coded product table into
         * two pieces: productTableLeft and productTableRight.  This requires
         * a little more work to compute products, but it also allows us to
         * reduce the table sizes to (30+24)*360 instead of 720*720 (a
         * compression factor of roughly 25).
         *
         * The idea is the following.  Suppose we wish to compute the
         * product p*q.  We consider the six images that form p, and separate
         * p[0,1] from p[2..5].  Write this as p = ab|cdef.
         *
         * Let x be the permutation in S6 that sends 0 -> a, 1 -> b,
         * and 2..5 to the remaining four digits in numerical order.
         * For instance, if a = 4 and b = 2, then x = 420135.
         *
         * Let y be the permutation in S6 that fixes {0,1}, and that reorders
         * {2,3,4,5} so that they appear in the same relative numerical
         * order as {c,d,e,f} appear in p.  For instance, if p = 42|3510,
         * then y maps 2345 -> 4532; that is, y = 014532.
         *
         * First observation: p = x * y.  This means that p * q = x * (y * q).
         *
         * In a weaker version of our scheme, we would then use two lookup
         * tables: productTableRight[24][720] to compute r = y * q, , and
         * then productTableLeft[30][720] to compute x * r.
         * The array indices are as follows:
         *
         * - The right-hand permutations q, r are indexed like S6.  This means
         *   we can just use q.code2_ and r.code2_ as the array indices.
         *
         * - The permutations y are indexed like S4 (where we treat y as a
         *   permutation of {2,3,4,5}).  This means the array index is
         *   either p.code2_ % 24 or (p.code2_ % 24 + 1).  Which of these we
         *   use depends on the parity of the index for x (see below).
         *
         * - The permutations x are indexed lexicographically.  This means
         *   that the array index is just p.code2_ / 24.
         *
         * The values that we store in each array are the S6 indices of
         * the resulting product permutations.
         *
         * Finally: we achieve an extra 2-fold compression by ignoring
         * the sign of the permutation q, and then reconstructing it later.
         * We pretend that q is even, by multiplying on the right by (4,5)
         * if necessary; this only flips the least significant bit of
         * the S6 index.  Now the right-hand permutations q, r are
         * indexed in the range [0,360) by dividing their S6 indices by 2.
         * The values stored in productTableRight are likewise given
         * in the range [0,360) by dividing the resulting S6 indices by 2,
         * and the values stored in productTableLeft are genuine S6 indices
         * in the range [0,720) but will always be even.  Once the entire
         * process is finished, we use the signs of p and q to decide if the
         * final result should be odd, and if so then we put back the least
         * significant bit in the final S6 index (i.e., we multiply on
         * the right again by (4,5)).
         */
        static constexpr Perm<6>::Code2 productTableLeft[30][360] = {
            { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718 },
            { 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              54, 56, 58, 48, 50, 52, 62, 60, 64, 68, 66, 70,
              78, 80, 82, 72, 74, 76, 86, 84, 88, 92, 90, 94,
              102, 104, 106, 96, 98, 100, 110, 108, 112, 116, 114, 118,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              366, 368, 370, 360, 362, 364, 374, 372, 376, 380, 378, 382,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              434, 432, 436, 444, 446, 448, 438, 440, 442, 450, 454, 452,
              458, 456, 460, 468, 470, 472, 462, 464, 466, 474, 478, 476,
              486, 488, 490, 480, 482, 484, 494, 492, 496, 500, 498, 502,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              554, 552, 556, 564, 566, 568, 558, 560, 562, 570, 574, 572,
              578, 576, 580, 588, 590, 592, 582, 584, 586, 594, 598, 596,
              606, 608, 610, 600, 602, 604, 614, 612, 616, 620, 618, 622,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              674, 672, 676, 684, 686, 688, 678, 680, 682, 690, 694, 692,
              698, 696, 700, 708, 710, 712, 702, 704, 706, 714, 718, 716 },
            { 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              6, 8, 10, 0, 2, 4, 14, 12, 16, 20, 18, 22,
              30, 32, 34, 24, 26, 28, 38, 36, 40, 44, 42, 46,
              84, 86, 88, 74, 72, 76, 80, 78, 82, 94, 90, 92,
              108, 110, 112, 98, 96, 100, 104, 102, 106, 118, 114, 116,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              126, 128, 130, 120, 122, 124, 134, 132, 136, 140, 138, 142,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              194, 192, 196, 204, 206, 208, 198, 200, 202, 210, 214, 212,
              218, 216, 220, 228, 230, 232, 222, 224, 226, 234, 238, 236,
              246, 248, 250, 240, 242, 244, 254, 252, 256, 260, 258, 262,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              314, 312, 316, 324, 326, 328, 318, 320, 322, 330, 334, 332,
              338, 336, 340, 348, 350, 352, 342, 344, 346, 354, 358, 356,
              492, 494, 496, 482, 480, 484, 488, 486, 490, 502, 498, 500,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              506, 504, 508, 516, 518, 520, 510, 512, 514, 522, 526, 524,
              530, 528, 532, 540, 542, 544, 534, 536, 538, 546, 550, 548,
              580, 576, 578, 594, 596, 598, 582, 586, 584, 588, 592, 590,
              612, 614, 616, 602, 600, 604, 608, 606, 610, 622, 618, 620,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              626, 624, 628, 636, 638, 640, 630, 632, 634, 642, 646, 644,
              650, 648, 652, 660, 662, 664, 654, 656, 658, 666, 670, 668,
              700, 696, 698, 714, 716, 718, 702, 706, 704, 708, 712, 710 },
            { 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              12, 14, 16, 2, 0, 4, 8, 6, 10, 22, 18, 20,
              36, 38, 40, 26, 24, 28, 32, 30, 34, 46, 42, 44,
              60, 62, 64, 50, 48, 52, 56, 54, 58, 70, 66, 68,
              114, 116, 118, 100, 96, 98, 106, 102, 104, 112, 108, 110,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              132, 134, 136, 122, 120, 124, 128, 126, 130, 142, 138, 140,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              146, 144, 148, 156, 158, 160, 150, 152, 154, 162, 166, 164,
              170, 168, 172, 180, 182, 184, 174, 176, 178, 186, 190, 188,
              220, 216, 218, 234, 236, 238, 222, 226, 224, 228, 232, 230,
              252, 254, 256, 242, 240, 244, 248, 246, 250, 262, 258, 260,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              266, 264, 268, 276, 278, 280, 270, 272, 274, 282, 286, 284,
              290, 288, 292, 300, 302, 304, 294, 296, 298, 306, 310, 308,
              340, 336, 338, 354, 356, 358, 342, 346, 344, 348, 352, 350,
              372, 374, 376, 362, 360, 364, 368, 366, 370, 382, 378, 380,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              386, 384, 388, 396, 398, 400, 390, 392, 394, 402, 406, 404,
              410, 408, 412, 420, 422, 424, 414, 416, 418, 426, 430, 428,
              460, 456, 458, 474, 476, 478, 462, 466, 464, 468, 472, 470,
              618, 620, 622, 604, 600, 602, 610, 606, 608, 616, 612, 614,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              628, 624, 626, 642, 644, 646, 630, 634, 632, 636, 640, 638,
              652, 648, 650, 666, 668, 670, 654, 658, 656, 660, 664, 662,
              676, 672, 674, 690, 692, 694, 678, 682, 680, 684, 688, 686 },
            { 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              18, 20, 22, 4, 0, 2, 10, 6, 8, 16, 12, 14,
              42, 44, 46, 28, 24, 26, 34, 30, 32, 40, 36, 38,
              66, 68, 70, 52, 48, 50, 58, 54, 56, 64, 60, 62,
              90, 92, 94, 76, 72, 74, 82, 78, 80, 88, 84, 86,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              138, 140, 142, 124, 120, 122, 130, 126, 128, 136, 132, 134,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              148, 144, 146, 162, 164, 166, 150, 154, 152, 156, 160, 158,
              172, 168, 170, 186, 188, 190, 174, 178, 176, 180, 184, 182,
              196, 192, 194, 210, 212, 214, 198, 202, 200, 204, 208, 206,
              258, 260, 262, 244, 240, 242, 250, 246, 248, 256, 252, 254,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              268, 264, 266, 282, 284, 286, 270, 274, 272, 276, 280, 278,
              292, 288, 290, 306, 308, 310, 294, 298, 296, 300, 304, 302,
              316, 312, 314, 330, 332, 334, 318, 322, 320, 324, 328, 326,
              378, 380, 382, 364, 360, 362, 370, 366, 368, 376, 372, 374,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              388, 384, 386, 402, 404, 406, 390, 394, 392, 396, 400, 398,
              412, 408, 410, 426, 428, 430, 414, 418, 416, 420, 424, 422,
              436, 432, 434, 450, 452, 454, 438, 442, 440, 444, 448, 446,
              498, 500, 502, 484, 480, 482, 490, 486, 488, 496, 492, 494,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              508, 504, 506, 522, 524, 526, 510, 514, 512, 516, 520, 518,
              532, 528, 530, 546, 548, 550, 534, 538, 536, 540, 544, 542,
              556, 552, 554, 570, 572, 574, 558, 562, 560, 564, 568, 566 },
            { 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              294, 296, 298, 288, 290, 292, 302, 300, 304, 308, 306, 310,
              318, 320, 322, 312, 314, 316, 326, 324, 328, 332, 330, 334,
              342, 344, 346, 336, 338, 340, 350, 348, 352, 356, 354, 358,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              414, 416, 418, 408, 410, 412, 422, 420, 424, 428, 426, 430,
              438, 440, 442, 432, 434, 436, 446, 444, 448, 452, 450, 454,
              462, 464, 466, 456, 458, 460, 470, 468, 472, 476, 474, 478,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              534, 536, 538, 528, 530, 532, 542, 540, 544, 548, 546, 550,
              558, 560, 562, 552, 554, 556, 566, 564, 568, 572, 570, 574,
              582, 584, 586, 576, 578, 580, 590, 588, 592, 596, 594, 598,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              654, 656, 658, 648, 650, 652, 662, 660, 664, 668, 666, 670,
              678, 680, 682, 672, 674, 676, 686, 684, 688, 692, 690, 694,
              702, 704, 706, 696, 698, 700, 710, 708, 712, 716, 714, 718 },
            { 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              174, 176, 178, 168, 170, 172, 182, 180, 184, 188, 186, 190,
              198, 200, 202, 192, 194, 196, 206, 204, 208, 212, 210, 214,
              222, 224, 226, 216, 218, 220, 230, 228, 232, 236, 234, 238,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              294, 296, 298, 288, 290, 292, 302, 300, 304, 308, 306, 310,
              318, 320, 322, 312, 314, 316, 326, 324, 328, 332, 330, 334,
              342, 344, 346, 336, 338, 340, 350, 348, 352, 356, 354, 358,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              390, 392, 394, 384, 386, 388, 398, 396, 400, 404, 402, 406,
              414, 416, 418, 408, 410, 412, 422, 420, 424, 428, 426, 430,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              440, 438, 442, 446, 444, 448, 432, 434, 436, 452, 454, 450,
              464, 462, 466, 470, 468, 472, 456, 458, 460, 476, 478, 474,
              510, 512, 514, 504, 506, 508, 518, 516, 520, 524, 522, 526,
              534, 536, 538, 528, 530, 532, 542, 540, 544, 548, 546, 550,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              560, 558, 562, 566, 564, 568, 552, 554, 556, 572, 574, 570,
              584, 582, 586, 590, 588, 592, 576, 578, 580, 596, 598, 594,
              630, 632, 634, 624, 626, 628, 638, 636, 640, 644, 642, 646,
              654, 656, 658, 648, 650, 652, 662, 660, 664, 668, 666, 670,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              680, 678, 682, 686, 684, 688, 672, 674, 676, 692, 694, 690,
              704, 702, 706, 710, 708, 712, 696, 698, 700, 716, 718, 714 },
            { 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              126, 128, 130, 120, 122, 124, 134, 132, 136, 140, 138, 142,
              150, 152, 154, 144, 146, 148, 158, 156, 160, 164, 162, 166,
              204, 206, 208, 194, 192, 196, 200, 198, 202, 214, 210, 212,
              228, 230, 232, 218, 216, 220, 224, 222, 226, 238, 234, 236,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              414, 416, 418, 408, 410, 412, 422, 420, 424, 428, 426, 430,
              438, 440, 442, 432, 434, 436, 446, 444, 448, 452, 450, 454,
              462, 464, 466, 456, 458, 460, 470, 468, 472, 476, 474, 478,
              6, 8, 10, 0, 2, 4, 14, 12, 16, 20, 18, 22,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              74, 72, 76, 84, 86, 88, 78, 80, 82, 90, 94, 92,
              98, 96, 100, 108, 110, 112, 102, 104, 106, 114, 118, 116,
              270, 272, 274, 264, 266, 268, 278, 276, 280, 284, 282, 286,
              294, 296, 298, 288, 290, 292, 302, 300, 304, 308, 306, 310,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              320, 318, 322, 326, 324, 328, 312, 314, 316, 332, 334, 330,
              344, 342, 346, 350, 348, 352, 336, 338, 340, 356, 358, 354,
              516, 518, 520, 506, 504, 508, 512, 510, 514, 526, 522, 524,
              558, 560, 562, 552, 554, 556, 566, 564, 568, 572, 570, 574,
              482, 480, 484, 492, 494, 496, 486, 488, 490, 498, 502, 500,
              536, 534, 538, 542, 540, 544, 528, 530, 532, 548, 550, 546,
              586, 582, 584, 596, 594, 598, 576, 580, 578, 590, 592, 588,
              636, 638, 640, 626, 624, 628, 632, 630, 634, 646, 642, 644,
              678, 680, 682, 672, 674, 676, 686, 684, 688, 692, 690, 694,
              602, 600, 604, 612, 614, 616, 606, 608, 610, 618, 622, 620,
              656, 654, 658, 662, 660, 664, 648, 650, 652, 668, 670, 666,
              706, 702, 704, 716, 714, 718, 696, 700, 698, 710, 712, 708 },
            { 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              132, 134, 136, 122, 120, 124, 128, 126, 130, 142, 138, 140,
              156, 158, 160, 146, 144, 148, 152, 150, 154, 166, 162, 164,
              180, 182, 184, 170, 168, 172, 176, 174, 178, 190, 186, 188,
              234, 236, 238, 220, 216, 218, 226, 222, 224, 232, 228, 230,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              534, 536, 538, 528, 530, 532, 542, 540, 544, 548, 546, 550,
              558, 560, 562, 552, 554, 556, 566, 564, 568, 572, 570, 574,
              582, 584, 586, 576, 578, 580, 590, 588, 592, 596, 594, 598,
              12, 14, 16, 2, 0, 4, 8, 6, 10, 22, 18, 20,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              26, 24, 28, 36, 38, 40, 30, 32, 34, 42, 46, 44,
              50, 48, 52, 60, 62, 64, 54, 56, 58, 66, 70, 68,
              100, 96, 98, 114, 116, 118, 102, 106, 104, 108, 112, 110,
              276, 278, 280, 266, 264, 268, 272, 270, 274, 286, 282, 284,
              318, 320, 322, 312, 314, 316, 326, 324, 328, 332, 330, 334,
              242, 240, 244, 252, 254, 256, 246, 248, 250, 258, 262, 260,
              296, 294, 298, 302, 300, 304, 288, 290, 292, 308, 310, 306,
              346, 342, 344, 356, 354, 358, 336, 340, 338, 350, 352, 348,
              396, 398, 400, 386, 384, 388, 392, 390, 394, 406, 402, 404,
              438, 440, 442, 432, 434, 436, 446, 444, 448, 452, 450, 454,
              362, 360, 364, 372, 374, 376, 366, 368, 370, 378, 382, 380,
              416, 414, 418, 422, 420, 424, 408, 410, 412, 428, 430, 426,
              466, 462, 464, 476, 474, 478, 456, 460, 458, 470, 472, 468,
              642, 644, 646, 628, 624, 626, 634, 630, 632, 640, 636, 638,
              702, 704, 706, 696, 698, 700, 710, 708, 712, 716, 714, 718,
              604, 600, 602, 618, 620, 622, 606, 610, 608, 612, 616, 614,
              658, 654, 656, 668, 666, 670, 648, 652, 650, 662, 664, 660,
              682, 678, 680, 692, 690, 694, 672, 676, 674, 686, 688, 684 },
            { 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              138, 140, 142, 124, 120, 122, 130, 126, 128, 136, 132, 134,
              162, 164, 166, 148, 144, 146, 154, 150, 152, 160, 156, 158,
              186, 188, 190, 172, 168, 170, 178, 174, 176, 184, 180, 182,
              210, 212, 214, 196, 192, 194, 202, 198, 200, 208, 204, 206,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              654, 656, 658, 648, 650, 652, 662, 660, 664, 668, 666, 670,
              678, 680, 682, 672, 674, 676, 686, 684, 688, 692, 690, 694,
              702, 704, 706, 696, 698, 700, 710, 708, 712, 716, 714, 718,
              18, 20, 22, 4, 0, 2, 10, 6, 8, 16, 12, 14,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              28, 24, 26, 42, 44, 46, 30, 34, 32, 36, 40, 38,
              52, 48, 50, 66, 68, 70, 54, 58, 56, 60, 64, 62,
              76, 72, 74, 90, 92, 94, 78, 82, 80, 84, 88, 86,
              282, 284, 286, 268, 264, 266, 274, 270, 272, 280, 276, 278,
              342, 344, 346, 336, 338, 340, 350, 348, 352, 356, 354, 358,
              244, 240, 242, 258, 260, 262, 246, 250, 248, 252, 256, 254,
              298, 294, 296, 308, 306, 310, 288, 292, 290, 302, 304, 300,
              322, 318, 320, 332, 330, 334, 312, 316, 314, 326, 328, 324,
              402, 404, 406, 388, 384, 386, 394, 390, 392, 400, 396, 398,
              462, 464, 466, 456, 458, 460, 470, 468, 472, 476, 474, 478,
              364, 360, 362, 378, 380, 382, 366, 370, 368, 372, 376, 374,
              418, 414, 416, 428, 426, 430, 408, 412, 410, 422, 424, 420,
              442, 438, 440, 452, 450, 454, 432, 436, 434, 446, 448, 444,
              522, 524, 526, 508, 504, 506, 514, 510, 512, 520, 516, 518,
              582, 584, 586, 576, 578, 580, 590, 588, 592, 596, 594, 598,
              484, 480, 482, 498, 500, 502, 486, 490, 488, 492, 496, 494,
              538, 534, 536, 548, 546, 550, 528, 532, 530, 542, 544, 540,
              562, 558, 560, 572, 570, 574, 552, 556, 554, 566, 568, 564 },
            { 240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              54, 56, 58, 48, 50, 52, 62, 60, 64, 68, 66, 70,
              78, 80, 82, 72, 74, 76, 86, 84, 88, 92, 90, 94,
              102, 104, 106, 96, 98, 100, 110, 108, 112, 116, 114, 118,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              174, 176, 178, 168, 170, 172, 182, 180, 184, 188, 186, 190,
              198, 200, 202, 192, 194, 196, 206, 204, 208, 212, 210, 214,
              222, 224, 226, 216, 218, 220, 230, 228, 232, 236, 234, 238,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              366, 368, 370, 360, 362, 364, 374, 372, 376, 380, 378, 382,
              390, 392, 394, 384, 386, 388, 398, 396, 400, 404, 402, 406,
              444, 446, 448, 434, 432, 436, 440, 438, 442, 454, 450, 452,
              468, 470, 472, 458, 456, 460, 464, 462, 466, 478, 474, 476,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              486, 488, 490, 480, 482, 484, 494, 492, 496, 500, 498, 502,
              510, 512, 514, 504, 506, 508, 518, 516, 520, 524, 522, 526,
              564, 566, 568, 554, 552, 556, 560, 558, 562, 574, 570, 572,
              588, 590, 592, 578, 576, 580, 584, 582, 586, 598, 594, 596,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              606, 608, 610, 600, 602, 604, 614, 612, 616, 620, 618, 622,
              630, 632, 634, 624, 626, 628, 638, 636, 640, 644, 642, 646,
              684, 686, 688, 674, 672, 676, 680, 678, 682, 694, 690, 692,
              708, 710, 712, 698, 696, 700, 704, 702, 706, 718, 714, 716 },
            { 264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286,
              240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262,
              294, 296, 298, 288, 290, 292, 302, 300, 304, 308, 306, 310,
              318, 320, 322, 312, 314, 316, 326, 324, 328, 332, 330, 334,
              342, 344, 346, 336, 338, 340, 350, 348, 352, 356, 354, 358,
              144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              174, 176, 178, 168, 170, 172, 182, 180, 184, 188, 186, 190,
              198, 200, 202, 192, 194, 196, 206, 204, 208, 212, 210, 214,
              222, 224, 226, 216, 218, 220, 230, 228, 232, 236, 234, 238,
              24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              54, 56, 58, 48, 50, 52, 62, 60, 64, 68, 66, 70,
              78, 80, 82, 72, 74, 76, 86, 84, 88, 92, 90, 94,
              102, 104, 106, 96, 98, 100, 110, 108, 112, 116, 114, 118,
              414, 416, 418, 408, 410, 412, 422, 420, 424, 428, 426, 430,
              390, 392, 394, 384, 386, 388, 398, 396, 400, 404, 402, 406,
              366, 368, 370, 360, 362, 364, 374, 372, 376, 380, 378, 382,
              446, 444, 448, 440, 438, 442, 434, 432, 436, 454, 452, 450,
              470, 468, 472, 464, 462, 466, 458, 456, 460, 478, 476, 474,
              534, 536, 538, 528, 530, 532, 542, 540, 544, 548, 546, 550,
              510, 512, 514, 504, 506, 508, 518, 516, 520, 524, 522, 526,
              486, 488, 490, 480, 482, 484, 494, 492, 496, 500, 498, 502,
              566, 564, 568, 560, 558, 562, 554, 552, 556, 574, 572, 570,
              590, 588, 592, 584, 582, 586, 578, 576, 580, 598, 596, 594,
              654, 656, 658, 648, 650, 652, 662, 660, 664, 668, 666, 670,
              630, 632, 634, 624, 626, 628, 638, 636, 640, 644, 642, 646,
              606, 608, 610, 600, 602, 604, 614, 612, 616, 620, 618, 622,
              686, 684, 688, 680, 678, 682, 674, 672, 676, 694, 692, 690,
              710, 708, 712, 704, 702, 706, 698, 696, 700, 718, 716, 714 },
            { 288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              246, 248, 250, 240, 242, 244, 254, 252, 256, 260, 258, 262,
              270, 272, 274, 264, 266, 268, 278, 276, 280, 284, 282, 286,
              324, 326, 328, 314, 312, 316, 320, 318, 322, 334, 330, 332,
              348, 350, 352, 338, 336, 340, 344, 342, 346, 358, 354, 356,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              366, 368, 370, 360, 362, 364, 374, 372, 376, 380, 378, 382,
              390, 392, 394, 384, 386, 388, 398, 396, 400, 404, 402, 406,
              444, 446, 448, 434, 432, 436, 440, 438, 442, 454, 450, 452,
              468, 470, 472, 458, 456, 460, 464, 462, 466, 478, 474, 476,
              30, 32, 34, 24, 26, 28, 38, 36, 40, 44, 42, 46,
              54, 56, 58, 48, 50, 52, 62, 60, 64, 68, 66, 70,
              0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
              80, 78, 82, 86, 84, 88, 72, 74, 76, 92, 94, 90,
              104, 102, 106, 110, 108, 112, 96, 98, 100, 116, 118, 114,
              150, 152, 154, 144, 146, 148, 158, 156, 160, 164, 162, 166,
              174, 176, 178, 168, 170, 172, 182, 180, 184, 188, 186, 190,
              120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142,
              200, 198, 202, 206, 204, 208, 192, 194, 196, 212, 214, 210,
              224, 222, 226, 230, 228, 232, 216, 218, 220, 236, 238, 234,
              540, 542, 544, 530, 528, 532, 536, 534, 538, 550, 546, 548,
              564, 566, 568, 554, 552, 556, 560, 558, 562, 574, 570, 572,
              488, 486, 490, 494, 492, 496, 480, 482, 484, 500, 502, 498,
              512, 510, 514, 518, 516, 520, 504, 506, 508, 524, 526, 522,
              592, 588, 590, 598, 594, 596, 578, 580, 576, 584, 586, 582,
              660, 662, 664, 650, 648, 652, 656, 654, 658, 670, 666, 668,
              684, 686, 688, 674, 672, 676, 680, 678, 682, 694, 690, 692,
              608, 606, 610, 614, 612, 616, 600, 602, 604, 620, 622, 618,
              632, 630, 634, 638, 636, 640, 624, 626, 628, 644, 646, 642,
              712, 708, 710, 718, 714, 716, 698, 700, 696, 704, 706, 702 },
            { 312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              252, 254, 256, 242, 240, 244, 248, 246, 250, 262, 258, 260,
              276, 278, 280, 266, 264, 268, 272, 270, 274, 286, 282, 284,
              300, 302, 304, 290, 288, 292, 296, 294, 298, 310, 306, 308,
              354, 356, 358, 340, 336, 338, 346, 342, 344, 352, 348, 350,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              486, 488, 490, 480, 482, 484, 494, 492, 496, 500, 498, 502,
              510, 512, 514, 504, 506, 508, 518, 516, 520, 524, 522, 526,
              564, 566, 568, 554, 552, 556, 560, 558, 562, 574, 570, 572,
              588, 590, 592, 578, 576, 580, 584, 582, 586, 598, 594, 596,
              36, 38, 40, 26, 24, 28, 32, 30, 34, 46, 42, 44,
              78, 80, 82, 72, 74, 76, 86, 84, 88, 92, 90, 94,
              2, 0, 4, 12, 14, 16, 6, 8, 10, 18, 22, 20,
              56, 54, 58, 62, 60, 64, 48, 50, 52, 68, 70, 66,
              106, 102, 104, 116, 114, 118, 96, 100, 98, 110, 112, 108,
              156, 158, 160, 146, 144, 148, 152, 150, 154, 166, 162, 164,
              198, 200, 202, 192, 194, 196, 206, 204, 208, 212, 210, 214,
              122, 120, 124, 132, 134, 136, 126, 128, 130, 138, 142, 140,
              176, 174, 178, 182, 180, 184, 168, 170, 172, 188, 190, 186,
              226, 222, 224, 236, 234, 238, 216, 220, 218, 230, 232, 228,
              420, 422, 424, 410, 408, 412, 416, 414, 418, 430, 426, 428,
              444, 446, 448, 434, 432, 436, 440, 438, 442, 454, 450, 452,
              368, 366, 370, 374, 372, 376, 360, 362, 364, 380, 382, 378,
              392, 390, 394, 398, 396, 400, 384, 386, 388, 404, 406, 402,
              472, 468, 470, 478, 474, 476, 458, 460, 456, 464, 466, 462,
              666, 668, 670, 652, 648, 650, 658, 654, 656, 664, 660, 662,
              708, 710, 712, 698, 696, 700, 704, 702, 706, 718, 714, 716,
              610, 606, 608, 620, 618, 622, 600, 604, 602, 614, 616, 612,
              634, 630, 632, 644, 642, 646, 624, 628, 626, 638, 640, 636,
              688, 684, 686, 694, 690, 692, 674, 676, 672, 680, 682, 678 },
            { 336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              258, 260, 262, 244, 240, 242, 250, 246, 248, 256, 252, 254,
              282, 284, 286, 268, 264, 266, 274, 270, 272, 280, 276, 278,
              306, 308, 310, 292, 288, 290, 298, 294, 296, 304, 300, 302,
              330, 332, 334, 316, 312, 314, 322, 318, 320, 328, 324, 326,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              606, 608, 610, 600, 602, 604, 614, 612, 616, 620, 618, 622,
              630, 632, 634, 624, 626, 628, 638, 636, 640, 644, 642, 646,
              684, 686, 688, 674, 672, 676, 680, 678, 682, 694, 690, 692,
              708, 710, 712, 698, 696, 700, 704, 702, 706, 718, 714, 716,
              42, 44, 46, 28, 24, 26, 34, 30, 32, 40, 36, 38,
              102, 104, 106, 96, 98, 100, 110, 108, 112, 116, 114, 118,
              4, 0, 2, 18, 20, 22, 6, 10, 8, 12, 16, 14,
              58, 54, 56, 68, 66, 70, 48, 52, 50, 62, 64, 60,
              82, 78, 80, 92, 90, 94, 72, 76, 74, 86, 88, 84,
              162, 164, 166, 148, 144, 146, 154, 150, 152, 160, 156, 158,
              222, 224, 226, 216, 218, 220, 230, 228, 232, 236, 234, 238,
              124, 120, 122, 138, 140, 142, 126, 130, 128, 132, 136, 134,
              178, 174, 176, 188, 186, 190, 168, 172, 170, 182, 184, 180,
              202, 198, 200, 212, 210, 214, 192, 196, 194, 206, 208, 204,
              426, 428, 430, 412, 408, 410, 418, 414, 416, 424, 420, 422,
              468, 470, 472, 458, 456, 460, 464, 462, 466, 478, 474, 476,
              370, 366, 368, 380, 378, 382, 360, 364, 362, 374, 376, 372,
              394, 390, 392, 404, 402, 406, 384, 388, 386, 398, 400, 396,
              448, 444, 446, 454, 450, 452, 434, 436, 432, 440, 442, 438,
              546, 548, 550, 532, 528, 530, 538, 534, 536, 544, 540, 542,
              588, 590, 592, 578, 576, 580, 584, 582, 586, 598, 594, 596,
              490, 486, 488, 500, 498, 502, 480, 484, 482, 494, 496, 492,
              514, 510, 512, 524, 522, 526, 504, 508, 506, 518, 520, 516,
              568, 564, 566, 574, 570, 572, 554, 556, 552, 560, 562, 558 },
            { 360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              6, 8, 10, 0, 2, 4, 14, 12, 16, 20, 18, 22,
              30, 32, 34, 24, 26, 28, 38, 36, 40, 44, 42, 46,
              84, 86, 88, 74, 72, 76, 80, 78, 82, 94, 90, 92,
              108, 110, 112, 98, 96, 100, 104, 102, 106, 118, 114, 116,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              126, 128, 130, 120, 122, 124, 134, 132, 136, 140, 138, 142,
              150, 152, 154, 144, 146, 148, 158, 156, 160, 164, 162, 166,
              204, 206, 208, 194, 192, 196, 200, 198, 202, 214, 210, 212,
              228, 230, 232, 218, 216, 220, 224, 222, 226, 238, 234, 236,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              246, 248, 250, 240, 242, 244, 254, 252, 256, 260, 258, 262,
              270, 272, 274, 264, 266, 268, 278, 276, 280, 284, 282, 286,
              324, 326, 328, 314, 312, 316, 320, 318, 322, 334, 330, 332,
              348, 350, 352, 338, 336, 340, 344, 342, 346, 358, 354, 356,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              492, 494, 496, 482, 480, 484, 488, 486, 490, 502, 498, 500,
              516, 518, 520, 506, 504, 508, 512, 510, 514, 526, 522, 524,
              540, 542, 544, 530, 528, 532, 536, 534, 538, 550, 546, 548,
              594, 596, 598, 580, 576, 578, 586, 582, 584, 592, 588, 590,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              612, 614, 616, 602, 600, 604, 608, 606, 610, 622, 618, 620,
              636, 638, 640, 626, 624, 628, 632, 630, 634, 646, 642, 644,
              660, 662, 664, 650, 648, 652, 656, 654, 658, 670, 666, 668,
              714, 716, 718, 700, 696, 698, 706, 702, 704, 712, 708, 710 },
            { 384, 386, 388, 390, 392, 394, 396, 398, 400, 402, 404, 406,
              360, 362, 364, 366, 368, 370, 372, 374, 376, 378, 380, 382,
              414, 416, 418, 408, 410, 412, 422, 420, 424, 428, 426, 430,
              438, 440, 442, 432, 434, 436, 446, 444, 448, 452, 450, 454,
              462, 464, 466, 456, 458, 460, 470, 468, 472, 476, 474, 478,
              168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
              126, 128, 130, 120, 122, 124, 134, 132, 136, 140, 138, 142,
              150, 152, 154, 144, 146, 148, 158, 156, 160, 164, 162, 166,
              204, 206, 208, 194, 192, 196, 200, 198, 202, 214, 210, 212,
              228, 230, 232, 218, 216, 220, 224, 222, 226, 238, 234, 236,
              48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70,
              6, 8, 10, 0, 2, 4, 14, 12, 16, 20, 18, 22,
              30, 32, 34, 24, 26, 28, 38, 36, 40, 44, 42, 46,
              84, 86, 88, 74, 72, 76, 80, 78, 82, 94, 90, 92,
              108, 110, 112, 98, 96, 100, 104, 102, 106, 118, 114, 116,
              294, 296, 298, 288, 290, 292, 302, 300, 304, 308, 306, 310,
              270, 272, 274, 264, 266, 268, 278, 276, 280, 284, 282, 286,
              246, 248, 250, 240, 242, 244, 254, 252, 256, 260, 258, 262,
              326, 324, 328, 320, 318, 322, 314, 312, 316, 334, 332, 330,
              350, 348, 352, 344, 342, 346, 338, 336, 340, 358, 356, 354,
              558, 560, 562, 552, 554, 556, 566, 564, 568, 572, 570, 574,
              516, 518, 520, 506, 504, 508, 512, 510, 514, 526, 522, 524,
              492, 494, 496, 482, 480, 484, 488, 486, 490, 502, 498, 500,
              542, 540, 544, 536, 534, 538, 530, 528, 532, 550, 548, 546,
              596, 594, 598, 586, 582, 584, 580, 576, 578, 592, 590, 588,
              678, 680, 682, 672, 674, 676, 686, 684, 688, 692, 690, 694,
              636, 638, 640, 626, 624, 628, 632, 630, 634, 646, 642, 644,
              612, 614, 616, 602, 600, 604, 608, 606, 610, 622, 618, 620,
              662, 660, 664, 656, 654, 658, 650, 648, 652, 670, 668, 666,
              716, 714, 718, 706, 702, 704, 700, 696, 698, 712, 710, 708 },
            { 408, 410, 412, 414, 416, 418, 420, 422, 424, 426, 428, 430,
              366, 368, 370, 360, 362, 364, 374, 372, 376, 380, 378, 382,
              390, 392, 394, 384, 386, 388, 398, 396, 400, 404, 402, 406,
              444, 446, 448, 434, 432, 436, 440, 438, 442, 454, 450, 452,
              468, 470, 472, 458, 456, 460, 464, 462, 466, 478, 474, 476,
              288, 290, 292, 294, 296, 298, 300, 302, 304, 306, 308, 310,
              246, 248, 250, 240, 242, 244, 254, 252, 256, 260, 258, 262,
              270, 272, 274, 264, 266, 268, 278, 276, 280, 284, 282, 286,
              324, 326, 328, 314, 312, 316, 320, 318, 322, 334, 330, 332,
              348, 350, 352, 338, 336, 340, 344, 342, 346, 358, 354, 356,
              54, 56, 58, 48, 50, 52, 62, 60, 64, 68, 66, 70,
              30, 32, 34, 24, 26, 28, 38, 36, 40, 44, 42, 46,
              6, 8, 10, 0, 2, 4, 14, 12, 16, 20, 18, 22,
              86, 84, 88, 80, 78, 82, 74, 72, 76, 94, 92, 90,
              110, 108, 112, 104, 102, 106, 98, 96, 100, 118, 116, 114,
              174, 176, 178, 168, 170, 172, 182, 180, 184, 188, 186, 190,
              150, 152, 154, 144, 146, 148, 158, 156, 160, 164, 162, 166,
              126, 128, 130, 120, 122, 124, 134, 132, 136, 140, 138, 142,
              206, 204, 208, 200, 198, 202, 194, 192, 196, 214, 212, 210,
              230, 228, 232, 224, 222, 226, 218, 216, 220, 238, 236, 234,
              564, 566, 568, 554, 552, 556, 560, 558, 562, 574, 570, 572,
              540, 542, 544, 530, 528, 532, 536, 534, 538, 550, 546, 548,
              494, 492, 496, 488, 486, 490, 482, 480, 484, 502, 500, 498,
              518, 516, 520, 512, 510, 514, 506, 504, 508, 526, 524, 522,
              598, 594, 596, 592, 588, 590, 580, 578, 576, 586, 584, 582,
              684, 686, 688, 674, 672, 676, 680, 678, 682, 694, 690, 692,
              660, 662, 664, 650, 648, 652, 656, 654, 658, 670, 666, 668,
              614, 612, 616, 608, 606, 610, 602, 600, 604, 622, 620, 618,
              638, 636, 640, 632, 630, 634, 626, 624, 628, 646, 644, 642,
              718, 714, 716, 712, 708, 710, 700, 698, 696, 706, 704, 702 },
            { 432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              372, 374, 376, 362, 360, 364, 368, 366, 370, 382, 378, 380,
              396, 398, 400, 386, 384, 388, 392, 390, 394, 406, 402, 404,
              420, 422, 424, 410, 408, 412, 416, 414, 418, 430, 426, 428,
              474, 476, 478, 460, 456, 458, 466, 462, 464, 472, 468, 470,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              492, 494, 496, 482, 480, 484, 488, 486, 490, 502, 498, 500,
              516, 518, 520, 506, 504, 508, 512, 510, 514, 526, 522, 524,
              540, 542, 544, 530, 528, 532, 536, 534, 538, 550, 546, 548,
              594, 596, 598, 580, 576, 578, 586, 582, 584, 592, 588, 590,
              60, 62, 64, 50, 48, 52, 56, 54, 58, 70, 66, 68,
              84, 86, 88, 74, 72, 76, 80, 78, 82, 94, 90, 92,
              8, 6, 10, 14, 12, 16, 0, 2, 4, 20, 22, 18,
              32, 30, 34, 38, 36, 40, 24, 26, 28, 44, 46, 42,
              112, 108, 110, 118, 114, 116, 98, 100, 96, 104, 106, 102,
              180, 182, 184, 170, 168, 172, 176, 174, 178, 190, 186, 188,
              204, 206, 208, 194, 192, 196, 200, 198, 202, 214, 210, 212,
              128, 126, 130, 134, 132, 136, 120, 122, 124, 140, 142, 138,
              152, 150, 154, 158, 156, 160, 144, 146, 148, 164, 166, 162,
              232, 228, 230, 238, 234, 236, 218, 220, 216, 224, 226, 222,
              300, 302, 304, 290, 288, 292, 296, 294, 298, 310, 306, 308,
              324, 326, 328, 314, 312, 316, 320, 318, 322, 334, 330, 332,
              248, 246, 250, 254, 252, 256, 240, 242, 244, 260, 262, 258,
              272, 270, 274, 278, 276, 280, 264, 266, 268, 284, 286, 282,
              352, 348, 350, 358, 354, 356, 338, 340, 336, 344, 346, 342,
              690, 692, 694, 676, 672, 674, 682, 678, 680, 688, 684, 686,
              714, 716, 718, 700, 696, 698, 706, 702, 704, 712, 708, 710,
              616, 612, 614, 622, 618, 620, 602, 604, 600, 608, 610, 606,
              640, 636, 638, 646, 642, 644, 626, 628, 624, 632, 634, 630,
              664, 660, 662, 670, 666, 668, 650, 652, 648, 656, 658, 654 },
            { 456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              378, 380, 382, 364, 360, 362, 370, 366, 368, 376, 372, 374,
              402, 404, 406, 388, 384, 386, 394, 390, 392, 400, 396, 398,
              426, 428, 430, 412, 408, 410, 418, 414, 416, 424, 420, 422,
              450, 452, 454, 436, 432, 434, 442, 438, 440, 448, 444, 446,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              612, 614, 616, 602, 600, 604, 608, 606, 610, 622, 618, 620,
              636, 638, 640, 626, 624, 628, 632, 630, 634, 646, 642, 644,
              660, 662, 664, 650, 648, 652, 656, 654, 658, 670, 666, 668,
              714, 716, 718, 700, 696, 698, 706, 702, 704, 712, 708, 710,
              66, 68, 70, 52, 48, 50, 58, 54, 56, 64, 60, 62,
              108, 110, 112, 98, 96, 100, 104, 102, 106, 118, 114, 116,
              10, 6, 8, 20, 18, 22, 0, 4, 2, 14, 16, 12,
              34, 30, 32, 44, 42, 46, 24, 28, 26, 38, 40, 36,
              88, 84, 86, 94, 90, 92, 74, 76, 72, 80, 82, 78,
              186, 188, 190, 172, 168, 170, 178, 174, 176, 184, 180, 182,
              228, 230, 232, 218, 216, 220, 224, 222, 226, 238, 234, 236,
              130, 126, 128, 140, 138, 142, 120, 124, 122, 134, 136, 132,
              154, 150, 152, 164, 162, 166, 144, 148, 146, 158, 160, 156,
              208, 204, 206, 214, 210, 212, 194, 196, 192, 200, 202, 198,
              306, 308, 310, 292, 288, 290, 298, 294, 296, 304, 300, 302,
              348, 350, 352, 338, 336, 340, 344, 342, 346, 358, 354, 356,
              250, 246, 248, 260, 258, 262, 240, 244, 242, 254, 256, 252,
              274, 270, 272, 284, 282, 286, 264, 268, 266, 278, 280, 276,
              328, 324, 326, 334, 330, 332, 314, 316, 312, 320, 322, 318,
              570, 572, 574, 556, 552, 554, 562, 558, 560, 568, 564, 566,
              594, 596, 598, 580, 576, 578, 586, 582, 584, 592, 588, 590,
              496, 492, 494, 502, 498, 500, 482, 484, 480, 488, 490, 486,
              520, 516, 518, 526, 522, 524, 506, 508, 504, 512, 514, 510,
              544, 540, 542, 550, 546, 548, 530, 532, 528, 536, 538, 534 },
            { 480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              12, 14, 16, 2, 0, 4, 8, 6, 10, 22, 18, 20,
              36, 38, 40, 26, 24, 28, 32, 30, 34, 46, 42, 44,
              60, 62, 64, 50, 48, 52, 56, 54, 58, 70, 66, 68,
              114, 116, 118, 100, 96, 98, 106, 102, 104, 112, 108, 110,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              132, 134, 136, 122, 120, 124, 128, 126, 130, 142, 138, 140,
              156, 158, 160, 146, 144, 148, 152, 150, 154, 166, 162, 164,
              180, 182, 184, 170, 168, 172, 176, 174, 178, 190, 186, 188,
              234, 236, 238, 220, 216, 218, 226, 222, 224, 232, 228, 230,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              252, 254, 256, 242, 240, 244, 248, 246, 250, 262, 258, 260,
              276, 278, 280, 266, 264, 268, 272, 270, 274, 286, 282, 284,
              300, 302, 304, 290, 288, 292, 296, 294, 298, 310, 306, 308,
              354, 356, 358, 340, 336, 338, 346, 342, 344, 352, 348, 350,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              372, 374, 376, 362, 360, 364, 368, 366, 370, 382, 378, 380,
              396, 398, 400, 386, 384, 388, 392, 390, 394, 406, 402, 404,
              420, 422, 424, 410, 408, 412, 416, 414, 418, 430, 426, 428,
              474, 476, 478, 460, 456, 458, 466, 462, 464, 472, 468, 470,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              618, 620, 622, 604, 600, 602, 610, 606, 608, 616, 612, 614,
              642, 644, 646, 628, 624, 626, 634, 630, 632, 640, 636, 638,
              666, 668, 670, 652, 648, 650, 658, 654, 656, 664, 660, 662,
              690, 692, 694, 676, 672, 674, 682, 678, 680, 688, 684, 686 },
            { 504, 506, 508, 510, 512, 514, 516, 518, 520, 522, 524, 526,
              480, 482, 484, 486, 488, 490, 492, 494, 496, 498, 500, 502,
              534, 536, 538, 528, 530, 532, 542, 540, 544, 548, 546, 550,
              558, 560, 562, 552, 554, 556, 566, 564, 568, 572, 570, 574,
              582, 584, 586, 576, 578, 580, 590, 588, 592, 596, 594, 598,
              192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214,
              132, 134, 136, 122, 120, 124, 128, 126, 130, 142, 138, 140,
              156, 158, 160, 146, 144, 148, 152, 150, 154, 166, 162, 164,
              180, 182, 184, 170, 168, 172, 176, 174, 178, 190, 186, 188,
              234, 236, 238, 220, 216, 218, 226, 222, 224, 232, 228, 230,
              72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
              12, 14, 16, 2, 0, 4, 8, 6, 10, 22, 18, 20,
              36, 38, 40, 26, 24, 28, 32, 30, 34, 46, 42, 44,
              60, 62, 64, 50, 48, 52, 56, 54, 58, 70, 66, 68,
              114, 116, 118, 100, 96, 98, 106, 102, 104, 112, 108, 110,
              318, 320, 322, 312, 314, 316, 326, 324, 328, 332, 330, 334,
              276, 278, 280, 266, 264, 268, 272, 270, 274, 286, 282, 284,
              252, 254, 256, 242, 240, 244, 248, 246, 250, 262, 258, 260,
              302, 300, 304, 296, 294, 298, 290, 288, 292, 310, 308, 306,
              356, 354, 358, 346, 342, 344, 340, 336, 338, 352, 350, 348,
              438, 440, 442, 432, 434, 436, 446, 444, 448, 452, 450, 454,
              396, 398, 400, 386, 384, 388, 392, 390, 394, 406, 402, 404,
              372, 374, 376, 362, 360, 364, 368, 366, 370, 382, 378, 380,
              422, 420, 424, 416, 414, 418, 410, 408, 412, 430, 428, 426,
              476, 474, 478, 466, 462, 464, 460, 456, 458, 472, 470, 468,
              702, 704, 706, 696, 698, 700, 710, 708, 712, 716, 714, 718,
              642, 644, 646, 628, 624, 626, 634, 630, 632, 640, 636, 638,
              618, 620, 622, 604, 600, 602, 610, 606, 608, 616, 612, 614,
              668, 666, 670, 658, 654, 656, 652, 648, 650, 664, 662, 660,
              692, 690, 694, 682, 678, 680, 676, 672, 674, 688, 686, 684 },
            { 528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, 550,
              486, 488, 490, 480, 482, 484, 494, 492, 496, 500, 498, 502,
              510, 512, 514, 504, 506, 508, 518, 516, 520, 524, 522, 526,
              564, 566, 568, 554, 552, 556, 560, 558, 562, 574, 570, 572,
              588, 590, 592, 578, 576, 580, 584, 582, 586, 598, 594, 596,
              312, 314, 316, 318, 320, 322, 324, 326, 328, 330, 332, 334,
              252, 254, 256, 242, 240, 244, 248, 246, 250, 262, 258, 260,
              276, 278, 280, 266, 264, 268, 272, 270, 274, 286, 282, 284,
              300, 302, 304, 290, 288, 292, 296, 294, 298, 310, 306, 308,
              354, 356, 358, 340, 336, 338, 346, 342, 344, 352, 348, 350,
              78, 80, 82, 72, 74, 76, 86, 84, 88, 92, 90, 94,
              36, 38, 40, 26, 24, 28, 32, 30, 34, 46, 42, 44,
              12, 14, 16, 2, 0, 4, 8, 6, 10, 22, 18, 20,
              62, 60, 64, 56, 54, 58, 50, 48, 52, 70, 68, 66,
              116, 114, 118, 106, 102, 104, 100, 96, 98, 112, 110, 108,
              198, 200, 202, 192, 194, 196, 206, 204, 208, 212, 210, 214,
              156, 158, 160, 146, 144, 148, 152, 150, 154, 166, 162, 164,
              132, 134, 136, 122, 120, 124, 128, 126, 130, 142, 138, 140,
              182, 180, 184, 176, 174, 178, 170, 168, 172, 190, 188, 186,
              236, 234, 238, 226, 222, 224, 220, 216, 218, 232, 230, 228,
              444, 446, 448, 434, 432, 436, 440, 438, 442, 454, 450, 452,
              420, 422, 424, 410, 408, 412, 416, 414, 418, 430, 426, 428,
              374, 372, 376, 368, 366, 370, 362, 360, 364, 382, 380, 378,
              398, 396, 400, 392, 390, 394, 386, 384, 388, 406, 404, 402,
              478, 474, 476, 472, 468, 470, 460, 458, 456, 466, 464, 462,
              708, 710, 712, 698, 696, 700, 704, 702, 706, 718, 714, 716,
              666, 668, 670, 652, 648, 650, 658, 654, 656, 664, 660, 662,
              620, 618, 622, 610, 606, 608, 604, 600, 602, 616, 614, 612,
              644, 642, 646, 634, 630, 632, 628, 624, 626, 640, 638, 636,
              694, 690, 692, 688, 684, 686, 676, 674, 672, 682, 680, 678 },
            { 552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574,
              492, 494, 496, 482, 480, 484, 488, 486, 490, 502, 498, 500,
              516, 518, 520, 506, 504, 508, 512, 510, 514, 526, 522, 524,
              540, 542, 544, 530, 528, 532, 536, 534, 538, 550, 546, 548,
              594, 596, 598, 580, 576, 578, 586, 582, 584, 592, 588, 590,
              432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454,
              372, 374, 376, 362, 360, 364, 368, 366, 370, 382, 378, 380,
              396, 398, 400, 386, 384, 388, 392, 390, 394, 406, 402, 404,
              420, 422, 424, 410, 408, 412, 416, 414, 418, 430, 426, 428,
              474, 476, 478, 460, 456, 458, 466, 462, 464, 472, 468, 470,
              84, 86, 88, 74, 72, 76, 80, 78, 82, 94, 90, 92,
              60, 62, 64, 50, 48, 52, 56, 54, 58, 70, 66, 68,
              14, 12, 16, 8, 6, 10, 2, 0, 4, 22, 20, 18,
              38, 36, 40, 32, 30, 34, 26, 24, 28, 46, 44, 42,
              118, 114, 116, 112, 108, 110, 100, 98, 96, 106, 104, 102,
              204, 206, 208, 194, 192, 196, 200, 198, 202, 214, 210, 212,
              180, 182, 184, 170, 168, 172, 176, 174, 178, 190, 186, 188,
              134, 132, 136, 128, 126, 130, 122, 120, 124, 142, 140, 138,
              158, 156, 160, 152, 150, 154, 146, 144, 148, 166, 164, 162,
              238, 234, 236, 232, 228, 230, 220, 218, 216, 226, 224, 222,
              324, 326, 328, 314, 312, 316, 320, 318, 322, 334, 330, 332,
              300, 302, 304, 290, 288, 292, 296, 294, 298, 310, 306, 308,
              254, 252, 256, 248, 246, 250, 242, 240, 244, 262, 260, 258,
              278, 276, 280, 272, 270, 274, 266, 264, 268, 286, 284, 282,
              358, 354, 356, 352, 348, 350, 340, 338, 336, 346, 344, 342,
              714, 716, 718, 700, 696, 698, 706, 702, 704, 712, 708, 710,
              690, 692, 694, 676, 672, 674, 682, 678, 680, 688, 684, 686,
              622, 618, 620, 616, 612, 614, 604, 602, 600, 610, 608, 606,
              646, 642, 644, 640, 636, 638, 628, 626, 624, 634, 632, 630,
              670, 666, 668, 664, 660, 662, 652, 650, 648, 658, 656, 654 },
            { 576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              498, 500, 502, 484, 480, 482, 490, 486, 488, 496, 492, 494,
              522, 524, 526, 508, 504, 506, 514, 510, 512, 520, 516, 518,
              546, 548, 550, 532, 528, 530, 538, 534, 536, 544, 540, 542,
              570, 572, 574, 556, 552, 554, 562, 558, 560, 568, 564, 566,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              618, 620, 622, 604, 600, 602, 610, 606, 608, 616, 612, 614,
              642, 644, 646, 628, 624, 626, 634, 630, 632, 640, 636, 638,
              666, 668, 670, 652, 648, 650, 658, 654, 656, 664, 660, 662,
              690, 692, 694, 676, 672, 674, 682, 678, 680, 688, 684, 686,
              90, 92, 94, 76, 72, 74, 82, 78, 80, 88, 84, 86,
              114, 116, 118, 100, 96, 98, 106, 102, 104, 112, 108, 110,
              16, 12, 14, 22, 18, 20, 2, 4, 0, 8, 10, 6,
              40, 36, 38, 46, 42, 44, 26, 28, 24, 32, 34, 30,
              64, 60, 62, 70, 66, 68, 50, 52, 48, 56, 58, 54,
              210, 212, 214, 196, 192, 194, 202, 198, 200, 208, 204, 206,
              234, 236, 238, 220, 216, 218, 226, 222, 224, 232, 228, 230,
              136, 132, 134, 142, 138, 140, 122, 124, 120, 128, 130, 126,
              160, 156, 158, 166, 162, 164, 146, 148, 144, 152, 154, 150,
              184, 180, 182, 190, 186, 188, 170, 172, 168, 176, 178, 174,
              330, 332, 334, 316, 312, 314, 322, 318, 320, 328, 324, 326,
              354, 356, 358, 340, 336, 338, 346, 342, 344, 352, 348, 350,
              256, 252, 254, 262, 258, 260, 242, 244, 240, 248, 250, 246,
              280, 276, 278, 286, 282, 284, 266, 268, 264, 272, 274, 270,
              304, 300, 302, 310, 306, 308, 290, 292, 288, 296, 298, 294,
              450, 452, 454, 436, 432, 434, 442, 438, 440, 448, 444, 446,
              474, 476, 478, 460, 456, 458, 466, 462, 464, 472, 468, 470,
              376, 372, 374, 382, 378, 380, 362, 364, 360, 368, 370, 366,
              400, 396, 398, 406, 402, 404, 386, 388, 384, 392, 394, 390,
              424, 420, 422, 430, 426, 428, 410, 412, 408, 416, 418, 414 },
            { 600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              18, 20, 22, 4, 0, 2, 10, 6, 8, 16, 12, 14,
              42, 44, 46, 28, 24, 26, 34, 30, 32, 40, 36, 38,
              66, 68, 70, 52, 48, 50, 58, 54, 56, 64, 60, 62,
              90, 92, 94, 76, 72, 74, 82, 78, 80, 88, 84, 86,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              138, 140, 142, 124, 120, 122, 130, 126, 128, 136, 132, 134,
              162, 164, 166, 148, 144, 146, 154, 150, 152, 160, 156, 158,
              186, 188, 190, 172, 168, 170, 178, 174, 176, 184, 180, 182,
              210, 212, 214, 196, 192, 194, 202, 198, 200, 208, 204, 206,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              258, 260, 262, 244, 240, 242, 250, 246, 248, 256, 252, 254,
              282, 284, 286, 268, 264, 266, 274, 270, 272, 280, 276, 278,
              306, 308, 310, 292, 288, 290, 298, 294, 296, 304, 300, 302,
              330, 332, 334, 316, 312, 314, 322, 318, 320, 328, 324, 326,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              378, 380, 382, 364, 360, 362, 370, 366, 368, 376, 372, 374,
              402, 404, 406, 388, 384, 386, 394, 390, 392, 400, 396, 398,
              426, 428, 430, 412, 408, 410, 418, 414, 416, 424, 420, 422,
              450, 452, 454, 436, 432, 434, 442, 438, 440, 448, 444, 446,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              498, 500, 502, 484, 480, 482, 490, 486, 488, 496, 492, 494,
              522, 524, 526, 508, 504, 506, 514, 510, 512, 520, 516, 518,
              546, 548, 550, 532, 528, 530, 538, 534, 536, 544, 540, 542,
              570, 572, 574, 556, 552, 554, 562, 558, 560, 568, 564, 566 },
            { 624, 626, 628, 630, 632, 634, 636, 638, 640, 642, 644, 646,
              600, 602, 604, 606, 608, 610, 612, 614, 616, 618, 620, 622,
              654, 656, 658, 648, 650, 652, 662, 660, 664, 668, 666, 670,
              678, 680, 682, 672, 674, 676, 686, 684, 688, 692, 690, 694,
              702, 704, 706, 696, 698, 700, 710, 708, 712, 716, 714, 718,
              216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238,
              138, 140, 142, 124, 120, 122, 130, 126, 128, 136, 132, 134,
              162, 164, 166, 148, 144, 146, 154, 150, 152, 160, 156, 158,
              186, 188, 190, 172, 168, 170, 178, 174, 176, 184, 180, 182,
              210, 212, 214, 196, 192, 194, 202, 198, 200, 208, 204, 206,
              96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118,
              18, 20, 22, 4, 0, 2, 10, 6, 8, 16, 12, 14,
              42, 44, 46, 28, 24, 26, 34, 30, 32, 40, 36, 38,
              66, 68, 70, 52, 48, 50, 58, 54, 56, 64, 60, 62,
              90, 92, 94, 76, 72, 74, 82, 78, 80, 88, 84, 86,
              342, 344, 346, 336, 338, 340, 350, 348, 352, 356, 354, 358,
              282, 284, 286, 268, 264, 266, 274, 270, 272, 280, 276, 278,
              258, 260, 262, 244, 240, 242, 250, 246, 248, 256, 252, 254,
              308, 306, 310, 298, 294, 296, 292, 288, 290, 304, 302, 300,
              332, 330, 334, 322, 318, 320, 316, 312, 314, 328, 326, 324,
              462, 464, 466, 456, 458, 460, 470, 468, 472, 476, 474, 478,
              402, 404, 406, 388, 384, 386, 394, 390, 392, 400, 396, 398,
              378, 380, 382, 364, 360, 362, 370, 366, 368, 376, 372, 374,
              428, 426, 430, 418, 414, 416, 412, 408, 410, 424, 422, 420,
              452, 450, 454, 442, 438, 440, 436, 432, 434, 448, 446, 444,
              582, 584, 586, 576, 578, 580, 590, 588, 592, 596, 594, 598,
              522, 524, 526, 508, 504, 506, 514, 510, 512, 520, 516, 518,
              498, 500, 502, 484, 480, 482, 490, 486, 488, 496, 492, 494,
              548, 546, 550, 538, 534, 536, 532, 528, 530, 544, 542, 540,
              572, 570, 574, 562, 558, 560, 556, 552, 554, 568, 566, 564 },
            { 648, 650, 652, 654, 656, 658, 660, 662, 664, 666, 668, 670,
              606, 608, 610, 600, 602, 604, 614, 612, 616, 620, 618, 622,
              630, 632, 634, 624, 626, 628, 638, 636, 640, 644, 642, 646,
              684, 686, 688, 674, 672, 676, 680, 678, 682, 694, 690, 692,
              708, 710, 712, 698, 696, 700, 704, 702, 706, 718, 714, 716,
              336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358,
              258, 260, 262, 244, 240, 242, 250, 246, 248, 256, 252, 254,
              282, 284, 286, 268, 264, 266, 274, 270, 272, 280, 276, 278,
              306, 308, 310, 292, 288, 290, 298, 294, 296, 304, 300, 302,
              330, 332, 334, 316, 312, 314, 322, 318, 320, 328, 324, 326,
              102, 104, 106, 96, 98, 100, 110, 108, 112, 116, 114, 118,
              42, 44, 46, 28, 24, 26, 34, 30, 32, 40, 36, 38,
              18, 20, 22, 4, 0, 2, 10, 6, 8, 16, 12, 14,
              68, 66, 70, 58, 54, 56, 52, 48, 50, 64, 62, 60,
              92, 90, 94, 82, 78, 80, 76, 72, 74, 88, 86, 84,
              222, 224, 226, 216, 218, 220, 230, 228, 232, 236, 234, 238,
              162, 164, 166, 148, 144, 146, 154, 150, 152, 160, 156, 158,
              138, 140, 142, 124, 120, 122, 130, 126, 128, 136, 132, 134,
              188, 186, 190, 178, 174, 176, 172, 168, 170, 184, 182, 180,
              212, 210, 214, 202, 198, 200, 196, 192, 194, 208, 206, 204,
              468, 470, 472, 458, 456, 460, 464, 462, 466, 478, 474, 476,
              426, 428, 430, 412, 408, 410, 418, 414, 416, 424, 420, 422,
              380, 378, 382, 370, 366, 368, 364, 360, 362, 376, 374, 372,
              404, 402, 406, 394, 390, 392, 388, 384, 386, 400, 398, 396,
              454, 450, 452, 448, 444, 446, 436, 434, 432, 442, 440, 438,
              588, 590, 592, 578, 576, 580, 584, 582, 586, 598, 594, 596,
              546, 548, 550, 532, 528, 530, 538, 534, 536, 544, 540, 542,
              500, 498, 502, 490, 486, 488, 484, 480, 482, 496, 494, 492,
              524, 522, 526, 514, 510, 512, 508, 504, 506, 520, 518, 516,
              574, 570, 572, 568, 564, 566, 556, 554, 552, 562, 560, 558 },
            { 672, 674, 676, 678, 680, 682, 684, 686, 688, 690, 692, 694,
              612, 614, 616, 602, 600, 604, 608, 606, 610, 622, 618, 620,
              636, 638, 640, 626, 624, 628, 632, 630, 634, 646, 642, 644,
              660, 662, 664, 650, 648, 652, 656, 654, 658, 670, 666, 668,
              714, 716, 718, 700, 696, 698, 706, 702, 704, 712, 708, 710,
              456, 458, 460, 462, 464, 466, 468, 470, 472, 474, 476, 478,
              378, 380, 382, 364, 360, 362, 370, 366, 368, 376, 372, 374,
              402, 404, 406, 388, 384, 386, 394, 390, 392, 400, 396, 398,
              426, 428, 430, 412, 408, 410, 418, 414, 416, 424, 420, 422,
              450, 452, 454, 436, 432, 434, 442, 438, 440, 448, 444, 446,
              108, 110, 112, 98, 96, 100, 104, 102, 106, 118, 114, 116,
              66, 68, 70, 52, 48, 50, 58, 54, 56, 64, 60, 62,
              20, 18, 22, 10, 6, 8, 4, 0, 2, 16, 14, 12,
              44, 42, 46, 34, 30, 32, 28, 24, 26, 40, 38, 36,
              94, 90, 92, 88, 84, 86, 76, 74, 72, 82, 80, 78,
              228, 230, 232, 218, 216, 220, 224, 222, 226, 238, 234, 236,
              186, 188, 190, 172, 168, 170, 178, 174, 176, 184, 180, 182,
              140, 138, 142, 130, 126, 128, 124, 120, 122, 136, 134, 132,
              164, 162, 166, 154, 150, 152, 148, 144, 146, 160, 158, 156,
              214, 210, 212, 208, 204, 206, 196, 194, 192, 202, 200, 198,
              348, 350, 352, 338, 336, 340, 344, 342, 346, 358, 354, 356,
              306, 308, 310, 292, 288, 290, 298, 294, 296, 304, 300, 302,
              260, 258, 262, 250, 246, 248, 244, 240, 242, 256, 254, 252,
              284, 282, 286, 274, 270, 272, 268, 264, 266, 280, 278, 276,
              334, 330, 332, 328, 324, 326, 316, 314, 312, 322, 320, 318,
              594, 596, 598, 580, 576, 578, 586, 582, 584, 592, 588, 590,
              570, 572, 574, 556, 552, 554, 562, 558, 560, 568, 564, 566,
              502, 498, 500, 496, 492, 494, 484, 482, 480, 490, 488, 486,
              526, 522, 524, 520, 516, 518, 508, 506, 504, 514, 512, 510,
              550, 546, 548, 544, 540, 542, 532, 530, 528, 538, 536, 534 },
            { 696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718,
              618, 620, 622, 604, 600, 602, 610, 606, 608, 616, 612, 614,
              642, 644, 646, 628, 624, 626, 634, 630, 632, 640, 636, 638,
              666, 668, 670, 652, 648, 650, 658, 654, 656, 664, 660, 662,
              690, 692, 694, 676, 672, 674, 682, 678, 680, 688, 684, 686,
              576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598,
              498, 500, 502, 484, 480, 482, 490, 486, 488, 496, 492, 494,
              522, 524, 526, 508, 504, 506, 514, 510, 512, 520, 516, 518,
              546, 548, 550, 532, 528, 530, 538, 534, 536, 544, 540, 542,
              570, 572, 574, 556, 552, 554, 562, 558, 560, 568, 564, 566,
              114, 116, 118, 100, 96, 98, 106, 102, 104, 112, 108, 110,
              90, 92, 94, 76, 72, 74, 82, 78, 80, 88, 84, 86,
              22, 18, 20, 16, 12, 14, 4, 2, 0, 10, 8, 6,
              46, 42, 44, 40, 36, 38, 28, 26, 24, 34, 32, 30,
              70, 66, 68, 64, 60, 62, 52, 50, 48, 58, 56, 54,
              234, 236, 238, 220, 216, 218, 226, 222, 224, 232, 228, 230,
              210, 212, 214, 196, 192, 194, 202, 198, 200, 208, 204, 206,
              142, 138, 140, 136, 132, 134, 124, 122, 120, 130, 128, 126,
              166, 162, 164, 160, 156, 158, 148, 146, 144, 154, 152, 150,
              190, 186, 188, 184, 180, 182, 172, 170, 168, 178, 176, 174,
              354, 356, 358, 340, 336, 338, 346, 342, 344, 352, 348, 350,
              330, 332, 334, 316, 312, 314, 322, 318, 320, 328, 324, 326,
              262, 258, 260, 256, 252, 254, 244, 242, 240, 250, 248, 246,
              286, 282, 284, 280, 276, 278, 268, 266, 264, 274, 272, 270,
              310, 306, 308, 304, 300, 302, 292, 290, 288, 298, 296, 294,
              474, 476, 478, 460, 456, 458, 466, 462, 464, 472, 468, 470,
              450, 452, 454, 436, 432, 434, 442, 438, 440, 448, 444, 446,
              382, 378, 380, 376, 372, 374, 364, 362, 360, 370, 368, 366,
              406, 402, 404, 400, 396, 398, 388, 386, 384, 394, 392, 390,
              430, 426, 428, 424, 420, 422, 412, 410, 408, 418, 416, 414 },
        };

        /**
         * For Perm<6>, we split the usual hard-coded product table into
         * two pieces (productTableLeft and productTableRight), which
         * together take up roughly 1/25 the space of a full product table.
         * See the productTableLeft notes for full details on how we split
         * these tables, and how we use them to compute products.
         */
        static constexpr Perm<6>::Code2 productTableRight[24][360] = {
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
              24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
              36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
              60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
              72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
              84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
              108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
              120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
              132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
              192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
              252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311,
              312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359 },
            { 0, 2, 1, 3, 5, 4, 9, 10, 11, 6, 7, 8,
              12, 14, 13, 15, 17, 16, 21, 22, 23, 18, 19, 20,
              24, 26, 25, 27, 29, 28, 33, 34, 35, 30, 31, 32,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
              36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              60, 62, 61, 63, 65, 64, 69, 70, 71, 66, 67, 68,
              72, 74, 73, 75, 77, 76, 81, 82, 83, 78, 79, 80,
              84, 86, 85, 87, 89, 88, 93, 94, 95, 90, 91, 92,
              108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
              120, 122, 121, 123, 125, 124, 129, 130, 131, 126, 127, 128,
              132, 134, 133, 135, 137, 136, 141, 142, 143, 138, 139, 140,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              180, 182, 181, 183, 185, 184, 189, 190, 191, 186, 187, 188,
              192, 194, 193, 195, 197, 196, 201, 202, 203, 198, 199, 200,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311,
              312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
              252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299 },
            { 1, 2, 0, 6, 8, 7, 9, 11, 10, 3, 4, 5,
              13, 14, 12, 18, 20, 19, 21, 23, 22, 15, 16, 17,
              36, 38, 37, 39, 41, 40, 45, 46, 47, 42, 43, 44,
              48, 50, 49, 51, 53, 52, 57, 58, 59, 54, 55, 56,
              24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
              61, 62, 60, 66, 68, 67, 69, 71, 70, 63, 64, 65,
              73, 74, 72, 78, 80, 79, 81, 83, 82, 75, 76, 77,
              96, 98, 97, 99, 101, 100, 105, 106, 107, 102, 103, 104,
              108, 110, 109, 111, 113, 112, 117, 118, 119, 114, 115, 116,
              84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              121, 122, 120, 126, 128, 127, 129, 131, 130, 123, 124, 125,
              133, 134, 132, 138, 140, 139, 141, 143, 142, 135, 136, 137,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              240, 242, 241, 243, 245, 244, 249, 250, 251, 246, 247, 248,
              252, 254, 253, 255, 257, 256, 261, 262, 263, 258, 259, 260,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              300, 302, 301, 303, 305, 304, 309, 310, 311, 306, 307, 308,
              312, 314, 313, 315, 317, 316, 321, 322, 323, 318, 319, 320,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
              192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239 },
            { 1, 0, 2, 6, 7, 8, 3, 4, 5, 9, 11, 10,
              13, 12, 14, 18, 19, 20, 15, 16, 17, 21, 23, 22,
              36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
              48, 50, 49, 51, 53, 52, 57, 58, 59, 54, 55, 56,
              61, 60, 62, 66, 67, 68, 63, 64, 65, 69, 71, 70,
              73, 72, 74, 78, 79, 80, 75, 76, 77, 81, 83, 82,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
              84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              108, 110, 109, 111, 113, 112, 117, 118, 119, 114, 115, 116,
              121, 120, 122, 126, 127, 128, 123, 124, 125, 129, 131, 130,
              133, 132, 134, 138, 139, 140, 135, 136, 137, 141, 143, 142,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
              252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
              192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              300, 302, 301, 303, 305, 304, 309, 310, 311, 306, 307, 308,
              312, 314, 313, 315, 317, 316, 321, 322, 323, 318, 319, 320,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347 },
            { 2, 0, 1, 9, 10, 11, 3, 5, 4, 6, 8, 7,
              14, 12, 13, 21, 22, 23, 15, 17, 16, 18, 20, 19,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
              24, 26, 25, 27, 29, 28, 33, 34, 35, 30, 31, 32,
              36, 38, 37, 39, 41, 40, 45, 46, 47, 42, 43, 44,
              62, 60, 61, 69, 70, 71, 63, 65, 64, 66, 68, 67,
              74, 72, 73, 81, 82, 83, 75, 77, 76, 78, 80, 79,
              108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
              84, 86, 85, 87, 89, 88, 93, 94, 95, 90, 91, 92,
              96, 98, 97, 99, 101, 100, 105, 106, 107, 102, 103, 104,
              122, 120, 121, 129, 130, 131, 123, 125, 124, 126, 128, 127,
              134, 132, 133, 141, 142, 143, 135, 137, 136, 138, 140, 139,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311,
              312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              180, 182, 181, 183, 185, 184, 189, 190, 191, 186, 187, 188,
              192, 194, 193, 195, 197, 196, 201, 202, 203, 198, 199, 200,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              240, 242, 241, 243, 245, 244, 249, 250, 251, 246, 247, 248,
              252, 254, 253, 255, 257, 256, 261, 262, 263, 258, 259, 260,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287 },
            { 2, 1, 0, 9, 11, 10, 6, 8, 7, 3, 5, 4,
              14, 13, 12, 21, 23, 22, 18, 20, 19, 15, 17, 16,
              48, 50, 49, 51, 53, 52, 57, 58, 59, 54, 55, 56,
              36, 38, 37, 39, 41, 40, 45, 46, 47, 42, 43, 44,
              24, 26, 25, 27, 29, 28, 33, 34, 35, 30, 31, 32,
              62, 61, 60, 69, 71, 70, 66, 68, 67, 63, 65, 64,
              74, 73, 72, 81, 83, 82, 78, 80, 79, 75, 77, 76,
              108, 110, 109, 111, 113, 112, 117, 118, 119, 114, 115, 116,
              96, 98, 97, 99, 101, 100, 105, 106, 107, 102, 103, 104,
              84, 86, 85, 87, 89, 88, 93, 94, 95, 90, 91, 92,
              122, 121, 120, 129, 131, 130, 126, 128, 127, 123, 125, 124,
              134, 133, 132, 141, 143, 142, 138, 140, 139, 135, 137, 136,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              300, 302, 301, 303, 305, 304, 309, 310, 311, 306, 307, 308,
              312, 314, 313, 315, 317, 316, 321, 322, 323, 318, 319, 320,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              240, 242, 241, 243, 245, 244, 249, 250, 251, 246, 247, 248,
              252, 254, 253, 255, 257, 256, 261, 262, 263, 258, 259, 260,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              180, 182, 181, 183, 185, 184, 189, 190, 191, 186, 187, 188,
              192, 194, 193, 195, 197, 196, 201, 202, 203, 198, 199, 200,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227 },
            { 3, 5, 4, 0, 2, 1, 10, 9, 11, 7, 6, 8,
              24, 26, 25, 27, 29, 28, 33, 34, 35, 30, 31, 32,
              12, 14, 13, 15, 17, 16, 21, 22, 23, 18, 19, 20,
              49, 48, 50, 54, 55, 56, 51, 52, 53, 57, 59, 58,
              37, 36, 38, 42, 43, 44, 39, 40, 41, 45, 47, 46,
              63, 65, 64, 60, 62, 61, 70, 69, 71, 67, 66, 68,
              84, 86, 85, 87, 89, 88, 93, 94, 95, 90, 91, 92,
              72, 74, 73, 75, 77, 76, 81, 82, 83, 78, 79, 80,
              109, 108, 110, 114, 115, 116, 111, 112, 113, 117, 119, 118,
              97, 96, 98, 102, 103, 104, 99, 100, 101, 105, 107, 106,
              180, 182, 181, 183, 185, 184, 189, 190, 191, 186, 187, 188,
              192, 194, 193, 195, 197, 196, 201, 202, 203, 198, 199, 200,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              120, 122, 121, 123, 125, 124, 129, 130, 131, 126, 127, 128,
              132, 134, 133, 135, 137, 136, 141, 142, 143, 138, 139, 140,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              301, 300, 302, 306, 307, 308, 303, 304, 305, 309, 311, 310,
              313, 312, 314, 318, 319, 320, 315, 316, 317, 321, 323, 322,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              241, 240, 242, 246, 247, 248, 243, 244, 245, 249, 251, 250,
              253, 252, 254, 258, 259, 260, 255, 256, 257, 261, 263, 262,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296 },
            { 3, 4, 5, 0, 1, 2, 7, 6, 8, 10, 9, 11,
              24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
              37, 36, 38, 42, 43, 44, 39, 40, 41, 45, 47, 46,
              49, 48, 50, 54, 55, 56, 51, 52, 53, 57, 59, 58,
              63, 64, 65, 60, 61, 62, 67, 66, 68, 70, 69, 71,
              84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
              97, 96, 98, 102, 103, 104, 99, 100, 101, 105, 107, 106,
              109, 108, 110, 114, 115, 116, 111, 112, 113, 117, 119, 118,
              180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
              192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
              132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              241, 240, 242, 246, 247, 248, 243, 244, 245, 249, 251, 250,
              253, 252, 254, 258, 259, 260, 255, 256, 257, 261, 263, 262,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              301, 300, 302, 306, 307, 308, 303, 304, 305, 309, 311, 310,
              313, 312, 314, 318, 319, 320, 315, 316, 317, 321, 323, 322,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356 },
            { 4, 3, 5, 7, 6, 8, 0, 1, 2, 10, 11, 9,
              25, 24, 26, 30, 31, 32, 27, 28, 29, 33, 35, 34,
              37, 36, 38, 42, 43, 44, 39, 40, 41, 45, 47, 46,
              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
              49, 50, 48, 54, 56, 55, 57, 59, 58, 51, 52, 53,
              64, 63, 65, 67, 66, 68, 60, 61, 62, 70, 71, 69,
              85, 84, 86, 90, 91, 92, 87, 88, 89, 93, 95, 94,
              97, 96, 98, 102, 103, 104, 99, 100, 101, 105, 107, 106,
              72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
              109, 110, 108, 114, 116, 115, 117, 119, 118, 111, 112, 113,
              181, 180, 182, 186, 187, 188, 183, 184, 185, 189, 191, 190,
              193, 192, 194, 198, 199, 200, 195, 196, 197, 201, 203, 202,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              241, 240, 242, 246, 247, 248, 243, 244, 245, 249, 251, 250,
              253, 252, 254, 258, 259, 260, 255, 256, 257, 261, 263, 262,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
              132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              301, 302, 300, 306, 308, 307, 309, 311, 310, 303, 304, 305,
              313, 314, 312, 318, 320, 319, 321, 323, 322, 315, 316, 317,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335 },
            { 4, 5, 3, 7, 8, 6, 10, 11, 9, 0, 1, 2,
              25, 26, 24, 30, 32, 31, 33, 35, 34, 27, 28, 29,
              37, 38, 36, 42, 44, 43, 45, 47, 46, 39, 40, 41,
              49, 50, 48, 54, 56, 55, 57, 59, 58, 51, 52, 53,
              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
              64, 65, 63, 67, 68, 66, 70, 71, 69, 60, 61, 62,
              85, 86, 84, 90, 92, 91, 93, 95, 94, 87, 88, 89,
              97, 98, 96, 102, 104, 103, 105, 107, 106, 99, 100, 101,
              109, 110, 108, 114, 116, 115, 117, 119, 118, 111, 112, 113,
              72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
              181, 182, 180, 186, 188, 187, 189, 191, 190, 183, 184, 185,
              193, 194, 192, 198, 200, 199, 201, 203, 202, 195, 196, 197,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              241, 242, 240, 246, 248, 247, 249, 251, 250, 243, 244, 245,
              253, 254, 252, 258, 260, 259, 261, 263, 262, 255, 256, 257,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              301, 302, 300, 306, 308, 307, 309, 311, 310, 303, 304, 305,
              313, 314, 312, 318, 320, 319, 321, 323, 322, 315, 316, 317,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
              132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179 },
            { 5, 4, 3, 10, 11, 9, 7, 8, 6, 0, 2, 1,
              26, 25, 24, 33, 35, 34, 30, 32, 31, 27, 29, 28,
              49, 50, 48, 54, 56, 55, 57, 59, 58, 51, 52, 53,
              37, 38, 36, 42, 44, 43, 45, 47, 46, 39, 40, 41,
              12, 14, 13, 15, 17, 16, 21, 22, 23, 18, 19, 20,
              65, 64, 63, 70, 71, 69, 67, 68, 66, 60, 62, 61,
              86, 85, 84, 93, 95, 94, 90, 92, 91, 87, 89, 88,
              109, 110, 108, 114, 116, 115, 117, 119, 118, 111, 112, 113,
              97, 98, 96, 102, 104, 103, 105, 107, 106, 99, 100, 101,
              72, 74, 73, 75, 77, 76, 81, 82, 83, 78, 79, 80,
              182, 181, 180, 189, 191, 190, 186, 188, 187, 183, 185, 184,
              194, 193, 192, 201, 203, 202, 198, 200, 199, 195, 197, 196,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              301, 302, 300, 306, 308, 307, 309, 311, 310, 303, 304, 305,
              313, 314, 312, 318, 320, 319, 321, 323, 322, 315, 316, 317,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              241, 242, 240, 246, 248, 247, 249, 251, 250, 243, 244, 245,
              253, 254, 252, 258, 260, 259, 261, 263, 262, 255, 256, 257,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              120, 122, 121, 123, 125, 124, 129, 130, 131, 126, 127, 128,
              132, 134, 133, 135, 137, 136, 141, 142, 143, 138, 139, 140,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167 },
            { 5, 3, 4, 10, 9, 11, 0, 2, 1, 7, 8, 6,
              26, 24, 25, 33, 34, 35, 27, 29, 28, 30, 32, 31,
              49, 48, 50, 54, 55, 56, 51, 52, 53, 57, 59, 58,
              12, 14, 13, 15, 17, 16, 21, 22, 23, 18, 19, 20,
              37, 38, 36, 42, 44, 43, 45, 47, 46, 39, 40, 41,
              65, 63, 64, 70, 69, 71, 60, 62, 61, 67, 68, 66,
              86, 84, 85, 93, 94, 95, 87, 89, 88, 90, 92, 91,
              109, 108, 110, 114, 115, 116, 111, 112, 113, 117, 119, 118,
              72, 74, 73, 75, 77, 76, 81, 82, 83, 78, 79, 80,
              97, 98, 96, 102, 104, 103, 105, 107, 106, 99, 100, 101,
              182, 180, 181, 189, 190, 191, 183, 185, 184, 186, 188, 187,
              194, 192, 193, 201, 202, 203, 195, 197, 196, 198, 200, 199,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              301, 300, 302, 306, 307, 308, 303, 304, 305, 309, 311, 310,
              313, 312, 314, 318, 319, 320, 315, 316, 317, 321, 323, 322,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              120, 122, 121, 123, 125, 124, 129, 130, 131, 126, 127, 128,
              132, 134, 133, 135, 137, 136, 141, 142, 143, 138, 139, 140,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              241, 242, 240, 246, 248, 247, 249, 251, 250, 243, 244, 245,
              253, 254, 252, 258, 260, 259, 261, 263, 262, 255, 256, 257,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275 },
            { 6, 7, 8, 1, 0, 2, 4, 3, 5, 11, 9, 10,
              36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              13, 12, 14, 18, 19, 20, 15, 16, 17, 21, 23, 22,
              25, 24, 26, 30, 31, 32, 27, 28, 29, 33, 35, 34,
              50, 48, 49, 57, 58, 59, 51, 53, 52, 54, 56, 55,
              66, 67, 68, 61, 60, 62, 64, 63, 65, 71, 69, 70,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
              73, 72, 74, 78, 79, 80, 75, 76, 77, 81, 83, 82,
              85, 84, 86, 90, 91, 92, 87, 88, 89, 93, 95, 94,
              110, 108, 109, 117, 118, 119, 111, 113, 112, 114, 116, 115,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
              252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              121, 120, 122, 126, 127, 128, 123, 124, 125, 129, 131, 130,
              133, 132, 134, 138, 139, 140, 135, 136, 137, 141, 143, 142,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              181, 180, 182, 186, 187, 188, 183, 184, 185, 189, 191, 190,
              193, 192, 194, 198, 199, 200, 195, 196, 197, 201, 203, 202,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              302, 300, 301, 309, 310, 311, 303, 305, 304, 306, 308, 307,
              314, 312, 313, 321, 322, 323, 315, 317, 316, 318, 320, 319,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344 },
            { 6, 8, 7, 1, 2, 0, 11, 9, 10, 4, 3, 5,
              36, 38, 37, 39, 41, 40, 45, 46, 47, 42, 43, 44,
              13, 14, 12, 18, 20, 19, 21, 23, 22, 15, 16, 17,
              50, 48, 49, 57, 58, 59, 51, 53, 52, 54, 56, 55,
              25, 24, 26, 30, 31, 32, 27, 28, 29, 33, 35, 34,
              66, 68, 67, 61, 62, 60, 71, 69, 70, 64, 63, 65,
              96, 98, 97, 99, 101, 100, 105, 106, 107, 102, 103, 104,
              73, 74, 72, 78, 80, 79, 81, 83, 82, 75, 76, 77,
              110, 108, 109, 117, 118, 119, 111, 113, 112, 114, 116, 115,
              85, 84, 86, 90, 91, 92, 87, 88, 89, 93, 95, 94,
              240, 242, 241, 243, 245, 244, 249, 250, 251, 246, 247, 248,
              252, 254, 253, 255, 257, 256, 261, 262, 263, 258, 259, 260,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              121, 122, 120, 126, 128, 127, 129, 131, 130, 123, 124, 125,
              133, 134, 132, 138, 140, 139, 141, 143, 142, 135, 136, 137,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              302, 300, 301, 309, 310, 311, 303, 305, 304, 306, 308, 307,
              314, 312, 313, 321, 322, 323, 315, 317, 316, 318, 320, 319,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              181, 180, 182, 186, 187, 188, 183, 184, 185, 189, 191, 190,
              193, 192, 194, 198, 199, 200, 195, 196, 197, 201, 203, 202,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236 },
            { 7, 8, 6, 4, 5, 3, 11, 10, 9, 1, 0, 2,
              37, 38, 36, 42, 44, 43, 45, 47, 46, 39, 40, 41,
              25, 26, 24, 30, 32, 31, 33, 35, 34, 27, 28, 29,
              50, 49, 48, 57, 59, 58, 54, 56, 55, 51, 53, 52,
              13, 12, 14, 18, 19, 20, 15, 16, 17, 21, 23, 22,
              67, 68, 66, 64, 65, 63, 71, 70, 69, 61, 60, 62,
              97, 98, 96, 102, 104, 103, 105, 107, 106, 99, 100, 101,
              85, 86, 84, 90, 92, 91, 93, 95, 94, 87, 88, 89,
              110, 109, 108, 117, 119, 118, 114, 116, 115, 111, 113, 112,
              73, 72, 74, 78, 79, 80, 75, 76, 77, 81, 83, 82,
              241, 242, 240, 246, 248, 247, 249, 251, 250, 243, 244, 245,
              253, 254, 252, 258, 260, 259, 261, 263, 262, 255, 256, 257,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              181, 182, 180, 186, 188, 187, 189, 191, 190, 183, 184, 185,
              193, 194, 192, 198, 200, 199, 201, 203, 202, 195, 196, 197,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              302, 301, 300, 309, 311, 310, 306, 308, 307, 303, 305, 304,
              314, 313, 312, 321, 323, 322, 318, 320, 319, 315, 317, 316,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              121, 120, 122, 126, 127, 128, 123, 124, 125, 129, 131, 130,
              133, 132, 134, 138, 139, 140, 135, 136, 137, 141, 143, 142,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176 },
            { 7, 6, 8, 4, 3, 5, 1, 0, 2, 11, 10, 9,
              37, 36, 38, 42, 43, 44, 39, 40, 41, 45, 47, 46,
              25, 24, 26, 30, 31, 32, 27, 28, 29, 33, 35, 34,
              13, 12, 14, 18, 19, 20, 15, 16, 17, 21, 23, 22,
              50, 49, 48, 57, 59, 58, 54, 56, 55, 51, 53, 52,
              67, 66, 68, 64, 63, 65, 61, 60, 62, 71, 70, 69,
              97, 96, 98, 102, 103, 104, 99, 100, 101, 105, 107, 106,
              85, 84, 86, 90, 91, 92, 87, 88, 89, 93, 95, 94,
              73, 72, 74, 78, 79, 80, 75, 76, 77, 81, 83, 82,
              110, 109, 108, 117, 119, 118, 114, 116, 115, 111, 113, 112,
              241, 240, 242, 246, 247, 248, 243, 244, 245, 249, 251, 250,
              253, 252, 254, 258, 259, 260, 255, 256, 257, 261, 263, 262,
              276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
              264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              181, 180, 182, 186, 187, 188, 183, 184, 185, 189, 191, 190,
              193, 192, 194, 198, 199, 200, 195, 196, 197, 201, 203, 202,
              216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              121, 120, 122, 126, 127, 128, 123, 124, 125, 129, 131, 130,
              133, 132, 134, 138, 139, 140, 135, 136, 137, 141, 143, 142,
              156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              302, 301, 300, 309, 311, 310, 306, 308, 307, 303, 305, 304,
              314, 313, 312, 321, 323, 322, 318, 320, 319, 315, 317, 316,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332 },
            { 8, 6, 7, 11, 9, 10, 1, 2, 0, 4, 5, 3,
              38, 36, 37, 45, 46, 47, 39, 41, 40, 42, 44, 43,
              50, 48, 49, 57, 58, 59, 51, 53, 52, 54, 56, 55,
              13, 14, 12, 18, 20, 19, 21, 23, 22, 15, 16, 17,
              25, 26, 24, 30, 32, 31, 33, 35, 34, 27, 28, 29,
              68, 66, 67, 71, 69, 70, 61, 62, 60, 64, 65, 63,
              98, 96, 97, 105, 106, 107, 99, 101, 100, 102, 104, 103,
              110, 108, 109, 117, 118, 119, 111, 113, 112, 114, 116, 115,
              73, 74, 72, 78, 80, 79, 81, 83, 82, 75, 76, 77,
              85, 86, 84, 90, 92, 91, 93, 95, 94, 87, 88, 89,
              242, 240, 241, 249, 250, 251, 243, 245, 244, 246, 248, 247,
              254, 252, 253, 261, 262, 263, 255, 257, 256, 258, 260, 259,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              302, 300, 301, 309, 310, 311, 303, 305, 304, 306, 308, 307,
              314, 312, 313, 321, 322, 323, 315, 317, 316, 318, 320, 319,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              121, 122, 120, 126, 128, 127, 129, 131, 130, 123, 124, 125,
              133, 134, 132, 138, 140, 139, 141, 143, 142, 135, 136, 137,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
              181, 182, 180, 186, 188, 187, 189, 191, 190, 183, 184, 185,
              193, 194, 192, 198, 200, 199, 201, 203, 202, 195, 196, 197,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215 },
            { 8, 7, 6, 11, 10, 9, 4, 5, 3, 1, 2, 0,
              38, 37, 36, 45, 47, 46, 42, 44, 43, 39, 41, 40,
              50, 49, 48, 57, 59, 58, 54, 56, 55, 51, 53, 52,
              25, 26, 24, 30, 32, 31, 33, 35, 34, 27, 28, 29,
              13, 14, 12, 18, 20, 19, 21, 23, 22, 15, 16, 17,
              68, 67, 66, 71, 70, 69, 64, 65, 63, 61, 62, 60,
              98, 97, 96, 105, 107, 106, 102, 104, 103, 99, 101, 100,
              110, 109, 108, 117, 119, 118, 114, 116, 115, 111, 113, 112,
              85, 86, 84, 90, 92, 91, 93, 95, 94, 87, 88, 89,
              73, 74, 72, 78, 80, 79, 81, 83, 82, 75, 76, 77,
              242, 241, 240, 249, 251, 250, 246, 248, 247, 243, 245, 244,
              254, 253, 252, 261, 263, 262, 258, 260, 259, 255, 257, 256,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              302, 301, 300, 309, 311, 310, 306, 308, 307, 303, 305, 304,
              314, 313, 312, 321, 323, 322, 318, 320, 319, 315, 317, 316,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              181, 182, 180, 186, 188, 187, 189, 191, 190, 183, 184, 185,
              193, 194, 192, 198, 200, 199, 201, 203, 202, 195, 196, 197,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
              121, 122, 120, 126, 128, 127, 129, 131, 130, 123, 124, 125,
              133, 134, 132, 138, 140, 139, 141, 143, 142, 135, 136, 137,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155 },
            { 9, 11, 10, 2, 1, 0, 8, 6, 7, 5, 3, 4,
              48, 50, 49, 51, 53, 52, 57, 58, 59, 54, 55, 56,
              14, 13, 12, 21, 23, 22, 18, 20, 19, 15, 17, 16,
              38, 36, 37, 45, 46, 47, 39, 41, 40, 42, 44, 43,
              26, 24, 25, 33, 34, 35, 27, 29, 28, 30, 32, 31,
              69, 71, 70, 62, 61, 60, 68, 66, 67, 65, 63, 64,
              108, 110, 109, 111, 113, 112, 117, 118, 119, 114, 115, 116,
              74, 73, 72, 81, 83, 82, 78, 80, 79, 75, 77, 76,
              98, 96, 97, 105, 106, 107, 99, 101, 100, 102, 104, 103,
              86, 84, 85, 93, 94, 95, 87, 89, 88, 90, 92, 91,
              300, 302, 301, 303, 305, 304, 309, 310, 311, 306, 307, 308,
              312, 314, 313, 315, 317, 316, 321, 322, 323, 318, 319, 320,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              122, 121, 120, 129, 131, 130, 126, 128, 127, 123, 125, 124,
              134, 133, 132, 141, 143, 142, 138, 140, 139, 135, 137, 136,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              242, 240, 241, 249, 250, 251, 243, 245, 244, 246, 248, 247,
              254, 252, 253, 261, 262, 263, 255, 257, 256, 258, 260, 259,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              182, 180, 181, 189, 190, 191, 183, 185, 184, 186, 188, 187,
              194, 192, 193, 201, 202, 203, 195, 197, 196, 198, 200, 199,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224 },
            { 9, 10, 11, 2, 0, 1, 5, 3, 4, 8, 6, 7,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
              14, 12, 13, 21, 22, 23, 15, 17, 16, 18, 20, 19,
              26, 24, 25, 33, 34, 35, 27, 29, 28, 30, 32, 31,
              38, 36, 37, 45, 46, 47, 39, 41, 40, 42, 44, 43,
              69, 70, 71, 62, 60, 61, 65, 63, 64, 68, 66, 67,
              108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
              74, 72, 73, 81, 82, 83, 75, 77, 76, 78, 80, 79,
              86, 84, 85, 93, 94, 95, 87, 89, 88, 90, 92, 91,
              98, 96, 97, 105, 106, 107, 99, 101, 100, 102, 104, 103,
              300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311,
              312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              122, 120, 121, 129, 130, 131, 123, 125, 124, 126, 128, 127,
              134, 132, 133, 141, 142, 143, 135, 137, 136, 138, 140, 139,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              182, 180, 181, 189, 190, 191, 183, 185, 184, 186, 188, 187,
              194, 192, 193, 201, 202, 203, 195, 197, 196, 198, 200, 199,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              242, 240, 241, 249, 250, 251, 243, 245, 244, 246, 248, 247,
              254, 252, 253, 261, 262, 263, 255, 257, 256, 258, 260, 259,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284 },
            { 10, 9, 11, 5, 3, 4, 2, 0, 1, 8, 7, 6,
              49, 48, 50, 54, 55, 56, 51, 52, 53, 57, 59, 58,
              26, 24, 25, 33, 34, 35, 27, 29, 28, 30, 32, 31,
              14, 12, 13, 21, 22, 23, 15, 17, 16, 18, 20, 19,
              38, 37, 36, 45, 47, 46, 42, 44, 43, 39, 41, 40,
              70, 69, 71, 65, 63, 64, 62, 60, 61, 68, 67, 66,
              109, 108, 110, 114, 115, 116, 111, 112, 113, 117, 119, 118,
              86, 84, 85, 93, 94, 95, 87, 89, 88, 90, 92, 91,
              74, 72, 73, 81, 82, 83, 75, 77, 76, 78, 80, 79,
              98, 97, 96, 105, 107, 106, 102, 104, 103, 99, 101, 100,
              301, 300, 302, 306, 307, 308, 303, 304, 305, 309, 311, 310,
              313, 312, 314, 318, 319, 320, 315, 316, 317, 321, 323, 322,
              336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              182, 180, 181, 189, 190, 191, 183, 185, 184, 186, 188, 187,
              194, 192, 193, 201, 202, 203, 195, 197, 196, 198, 200, 199,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              122, 120, 121, 129, 130, 131, 123, 125, 124, 126, 128, 127,
              134, 132, 133, 141, 142, 143, 135, 137, 136, 138, 140, 139,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              242, 241, 240, 249, 251, 250, 246, 248, 247, 243, 245, 244,
              254, 253, 252, 261, 263, 262, 258, 260, 259, 255, 257, 256,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272 },
            { 10, 11, 9, 5, 4, 3, 8, 7, 6, 2, 0, 1,
              49, 50, 48, 54, 56, 55, 57, 59, 58, 51, 52, 53,
              26, 25, 24, 33, 35, 34, 30, 32, 31, 27, 29, 28,
              38, 37, 36, 45, 47, 46, 42, 44, 43, 39, 41, 40,
              14, 12, 13, 21, 22, 23, 15, 17, 16, 18, 20, 19,
              70, 71, 69, 65, 64, 63, 68, 67, 66, 62, 60, 61,
              109, 110, 108, 114, 116, 115, 117, 119, 118, 111, 112, 113,
              86, 85, 84, 93, 95, 94, 90, 92, 91, 87, 89, 88,
              98, 97, 96, 105, 107, 106, 102, 104, 103, 99, 101, 100,
              74, 72, 73, 81, 82, 83, 75, 77, 76, 78, 80, 79,
              301, 302, 300, 306, 308, 307, 309, 311, 310, 303, 304, 305,
              313, 314, 312, 318, 320, 319, 321, 323, 322, 315, 316, 317,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335,
              182, 181, 180, 189, 191, 190, 186, 188, 187, 183, 185, 184,
              194, 193, 192, 201, 203, 202, 198, 200, 199, 195, 197, 196,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              242, 241, 240, 249, 251, 250, 246, 248, 247, 243, 245, 244,
              254, 253, 252, 261, 263, 262, 258, 260, 259, 255, 257, 256,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              122, 120, 121, 129, 130, 131, 123, 125, 124, 126, 128, 127,
              134, 132, 133, 141, 142, 143, 135, 137, 136, 138, 140, 139,
              168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164 },
            { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
              50, 49, 48, 57, 59, 58, 54, 56, 55, 51, 53, 52,
              38, 37, 36, 45, 47, 46, 42, 44, 43, 39, 41, 40,
              26, 25, 24, 33, 35, 34, 30, 32, 31, 27, 29, 28,
              14, 13, 12, 21, 23, 22, 18, 20, 19, 15, 17, 16,
              71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60,
              110, 109, 108, 117, 119, 118, 114, 116, 115, 111, 113, 112,
              98, 97, 96, 105, 107, 106, 102, 104, 103, 99, 101, 100,
              86, 85, 84, 93, 95, 94, 90, 92, 91, 87, 89, 88,
              74, 73, 72, 81, 83, 82, 78, 80, 79, 75, 77, 76,
              302, 301, 300, 309, 311, 310, 306, 308, 307, 303, 305, 304,
              314, 313, 312, 321, 323, 322, 318, 320, 319, 315, 317, 316,
              348, 350, 349, 351, 353, 352, 357, 358, 359, 354, 355, 356,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              242, 241, 240, 249, 251, 250, 246, 248, 247, 243, 245, 244,
              254, 253, 252, 261, 263, 262, 258, 260, 259, 255, 257, 256,
              288, 290, 289, 291, 293, 292, 297, 298, 299, 294, 295, 296,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              182, 181, 180, 189, 191, 190, 186, 188, 187, 183, 185, 184,
              194, 193, 192, 201, 203, 202, 198, 200, 199, 195, 197, 196,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212,
              122, 121, 120, 129, 131, 130, 126, 128, 127, 123, 125, 124,
              134, 133, 132, 141, 143, 142, 138, 140, 139, 135, 137, 136,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152 },
            { 11, 9, 10, 8, 6, 7, 2, 1, 0, 5, 4, 3,
              50, 48, 49, 57, 58, 59, 51, 53, 52, 54, 56, 55,
              38, 36, 37, 45, 46, 47, 39, 41, 40, 42, 44, 43,
              14, 13, 12, 21, 23, 22, 18, 20, 19, 15, 17, 16,
              26, 25, 24, 33, 35, 34, 30, 32, 31, 27, 29, 28,
              71, 69, 70, 68, 66, 67, 62, 61, 60, 65, 64, 63,
              110, 108, 109, 117, 118, 119, 111, 113, 112, 114, 116, 115,
              98, 96, 97, 105, 106, 107, 99, 101, 100, 102, 104, 103,
              74, 73, 72, 81, 83, 82, 78, 80, 79, 75, 77, 76,
              86, 85, 84, 93, 95, 94, 90, 92, 91, 87, 89, 88,
              302, 300, 301, 309, 310, 311, 303, 305, 304, 306, 308, 307,
              314, 312, 313, 321, 322, 323, 315, 317, 316, 318, 320, 319,
              348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
              324, 326, 325, 327, 329, 328, 333, 334, 335, 330, 331, 332,
              336, 338, 337, 339, 341, 340, 345, 346, 347, 342, 343, 344,
              242, 240, 241, 249, 250, 251, 243, 245, 244, 246, 248, 247,
              254, 252, 253, 261, 262, 263, 255, 257, 256, 258, 260, 259,
              288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
              264, 266, 265, 267, 269, 268, 273, 274, 275, 270, 271, 272,
              276, 278, 277, 279, 281, 280, 285, 286, 287, 282, 283, 284,
              122, 121, 120, 129, 131, 130, 126, 128, 127, 123, 125, 124,
              134, 133, 132, 141, 143, 142, 138, 140, 139, 135, 137, 136,
              168, 170, 169, 171, 173, 172, 177, 178, 179, 174, 175, 176,
              156, 158, 157, 159, 161, 160, 165, 166, 167, 162, 163, 164,
              144, 146, 145, 147, 149, 148, 153, 154, 155, 150, 151, 152,
              182, 181, 180, 189, 191, 190, 186, 188, 187, 183, 185, 184,
              194, 193, 192, 201, 203, 202, 198, 200, 199, 195, 197, 196,
              228, 230, 229, 231, 233, 232, 237, 238, 239, 234, 235, 236,
              216, 218, 217, 219, 221, 220, 225, 226, 227, 222, 223, 224,
              204, 206, 205, 207, 209, 208, 213, 214, 215, 210, 211, 212 }
        };

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S6[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[6][6] = {
            // Generated using Regina 6.0.
            { 0, 121, 265, 391, 513, 633 },
            { 121, 0, 25, 55, 81, 105 },
            { 265, 25, 0, 7, 15, 21 },
            { 391, 55, 7, 0, 3, 5 },
            { 513, 81, 15, 3, 0, 1 },
            { 633, 105, 21, 5, 1, 0 }
        };

        /**
         * Contains the orders of the permutations in the array \a S6.
         */
        static constexpr int orderTable[720] = {
            1, 2, 3, 2, 3, 2, 2, 2, 3, 4, 3, 4, 3, 4, 3, 2, 2, 4, 3, 4,
            3, 2, 2, 4, 2, 2, 2, 6, 2, 6, 3, 6, 5, 4, 5, 4, 5, 4, 3, 4,
            5, 6, 5, 4, 3, 4, 5, 6, 3, 6, 5, 4, 5, 4, 2, 2, 3, 4, 3, 4,
            2, 6, 5, 4, 5, 6, 2, 6, 5, 4, 5, 6, 5, 4, 3, 4, 5, 6, 3, 4,
            3, 2, 2, 4, 5, 4, 2, 6, 5, 6, 5, 6, 5, 6, 2, 4, 5, 4, 3, 4,
            5, 6, 3, 4, 3, 2, 2, 4, 5, 4, 2, 6, 5, 6, 5, 6, 5, 6, 2, 4,
            2, 2, 2, 6, 2, 6, 2, 2, 4, 6, 4, 6, 4, 6, 2, 6, 4, 2, 4, 6,
            2, 6, 4, 2, 3, 6, 3, 6, 3, 6, 4, 4, 5, 6, 5, 6, 5, 6, 5, 4,
            4, 6, 5, 6, 5, 4, 4, 6, 4, 4, 5, 6, 5, 6, 3, 6, 5, 4, 5, 4,
            3, 6, 5, 6, 4, 6, 3, 6, 5, 6, 4, 6, 5, 6, 5, 4, 4, 6, 5, 4,
            3, 4, 5, 6, 5, 6, 4, 6, 3, 6, 4, 6, 3, 6, 5, 6, 5, 6, 5, 4,
            4, 6, 5, 4, 3, 4, 5, 6, 5, 6, 4, 6, 3, 6, 4, 6, 3, 6, 5, 6,
            3, 6, 3, 6, 3, 6, 4, 4, 5, 6, 5, 6, 5, 6, 5, 4, 4, 6, 5, 6,
            5, 4, 4, 6, 2, 2, 2, 6, 2, 6, 3, 6, 5, 4, 5, 4, 5, 4, 3, 4,
            5, 6, 5, 4, 3, 4, 5, 6, 2, 2, 4, 6, 4, 6, 4, 4, 5, 6, 5, 6,
            5, 6, 4, 6, 3, 6, 5, 6, 4, 6, 3, 6, 4, 6, 2, 6, 4, 2, 5, 6,
            5, 4, 4, 6, 3, 6, 5, 6, 4, 6, 4, 6, 3, 6, 5, 6, 4, 6, 2, 6,
            4, 2, 5, 6, 5, 4, 4, 6, 3, 6, 5, 6, 4, 6, 4, 6, 3, 6, 5, 6,
            4, 4, 5, 6, 5, 6, 3, 6, 5, 4, 5, 4, 3, 6, 5, 6, 4, 6, 3, 6,
            5, 6, 4, 6, 3, 6, 5, 4, 5, 4, 2, 2, 3, 4, 3, 4, 2, 6, 5, 4,
            5, 6, 2, 6, 5, 4, 5, 6, 4, 4, 5, 6, 5, 6, 2, 2, 4, 6, 4, 6,
            4, 6, 5, 6, 3, 6, 4, 6, 5, 6, 3, 6, 5, 6, 3, 6, 4, 6, 4, 6,
            5, 6, 3, 6, 2, 6, 5, 4, 5, 6, 4, 2, 4, 6, 4, 6, 5, 6, 3, 6,
            4, 6, 4, 6, 5, 6, 3, 6, 2, 6, 5, 4, 5, 6, 4, 2, 4, 6, 4, 6,
            5, 6, 5, 4, 4, 6, 5, 4, 3, 4, 5, 6, 5, 6, 4, 6, 3, 6, 4, 6,
            3, 6, 5, 6, 5, 4, 3, 4, 5, 6, 3, 4, 3, 2, 2, 4, 5, 4, 2, 6,
            5, 6, 5, 6, 5, 6, 2, 4, 5, 6, 5, 4, 4, 6, 4, 6, 2, 6, 4, 2,
            5, 6, 3, 6, 4, 6, 3, 6, 4, 6, 5, 6, 4, 6, 5, 6, 3, 6, 5, 6,
            3, 6, 4, 6, 5, 4, 2, 6, 5, 6, 4, 6, 4, 2, 4, 6, 3, 6, 4, 6,
            5, 6, 3, 6, 4, 6, 5, 6, 5, 6, 5, 6, 2, 4, 4, 6, 4, 6, 4, 2,
            5, 6, 5, 4, 4, 6, 5, 4, 3, 4, 5, 6, 5, 6, 4, 6, 3, 6, 4, 6,
            3, 6, 5, 6, 5, 4, 3, 4, 5, 6, 3, 4, 3, 2, 2, 4, 5, 4, 2, 6,
            5, 6, 5, 6, 5, 6, 2, 4, 5, 6, 5, 4, 4, 6, 4, 6, 2, 6, 4, 2,
            5, 6, 3, 6, 4, 6, 3, 6, 4, 6, 5, 6, 4, 6, 5, 6, 3, 6, 5, 6,
            3, 6, 4, 6, 5, 4, 2, 6, 5, 6, 4, 6, 4, 2, 4, 6, 3, 6, 4, 6,
            5, 6, 3, 6, 4, 6, 5, 6, 5, 6, 5, 6, 2, 4, 4, 6, 4, 6, 4, 2
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
        constexpr Perm<6>(Code2 code);

    private:
        /**
         * Returns the index into the Perm<6>::S6 array of the permutation that
         * maps (0,...,5) to (<i>a</i>,...,<i>f</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>,<i>f</i>} =
         * {0,1,2,3,4,5}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @param f the desired image of 5.
         * @return the index \a i for which the given permutation is equal to
         * Perm<6>::S6[i].  This will be between 0 and 719 inclusive.
         */
        static constexpr int S6Index(int a, int b, int c, int d, int e, int f);

        /**
         * Converts between an index into Perm<6>::S6 and an index into
         * Perm<6>::orderedS6.  This conversion works in either direction.
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
         * a tight encoding of a 6-element permutation.
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

// Inline functions for Perm<6>

template <typename Int>
inline constexpr Int Perm<6>::convOrderedUnordered(Int index) {
    // S6 is almost the same as orderedS6, except that some pairs
    // S6[2i] <--> S6[2i+1] have been swapped to ensure that all
    // permutations S6[2i] are even and all permutations S6[2i+1] are odd.
    //
    // Specifically, we must flip between 2i <--> 2i+1 if and only if
    // one but not both of (S6Index / 2) and (S6Index / 24) is even.
    // Here we use (index >> 1), which is equivalent to (index / 2).
    //
    // A note for myself for if/when we extend this beyond 6:
    // what matters is the mod 2 sum of the parities of
    // (index / 2!), (index / 4!), (index / 6!), etc., up to (index / (n-1)!).
    //
    return ((((index >> 1) ^ (index / 24)) & 1) ? (index ^ 1) : index);
}

inline constexpr Perm<6> Perm<6>::S6Lookup::operator[] (int index) const {
    return Perm<6>(static_cast<Code2>(index));
}

inline constexpr Perm<6> Perm<6>::OrderedS6Lookup::operator[] (int index)
        const {
    return Perm<6>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline constexpr Perm<6>::Perm() : code2_(0) {
}

inline constexpr Perm<6>::Perm(Code2 code) : code2_(code) {
}

inline constexpr Perm<6>::Perm(int a, int b) : code2_(swapTable[a][b]) {
}

inline constexpr Perm<6>::Perm(int a, int b, int c, int d, int e, int f) :
        code2_(static_cast<Code2>(S6Index(a, b, c, d, e, f))) {
}

inline constexpr Perm<6>::Perm(const std::array<int, 6>& image) :
        code2_(static_cast<Code2>(S6Index(
            image[0], image[1], image[2], image[3], image[4], image[5]))) {
}

inline constexpr Perm<6>::Perm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1, int f0, int f1) :
        code2_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[6] = { 0, 0, 0, 0, 0, 0 };
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    image[e0] = e1;
    image[f0] = f1;
    code2_ = static_cast<Code2>(S6Index(
        image[0], image[1], image[2], image[3], image[4], image[5]));
}

inline constexpr Perm<6>::Code1 Perm<6>::permCode1() const {
    return code1Table[code2_];
}

inline constexpr Perm<6>::Code2 Perm<6>::permCode2() const {
    return code2_;
}

inline void Perm<6>::setPermCode1(Code1 code) {
    code2_ = static_cast<Code2>(S6Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07));
}

inline void Perm<6>::setPermCode2(Code2 code) {
    code2_ = code;
}

inline constexpr Perm<6> Perm<6>::fromPermCode1(Code1 code) {
    return Perm<6>(static_cast<Code2>(S6Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07,
        (code >> 15) & 0x07)));
}

inline constexpr Perm<6> Perm<6>::fromPermCode2(Code2 code) {
    return Perm<6>(code);
}

inline constexpr bool Perm<6>::isPermCode1(Code1 code) {
    unsigned mask = 0;
    for (int i = 0; i < 6; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 63 && (code >> 18) == 0);
}

inline constexpr bool Perm<6>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 720);
}

inline constexpr Perm<6>::ImagePack Perm<6>::imagePack() const {
    return permCode1();
}

inline constexpr Perm<6> Perm<6>::fromImagePack(ImagePack pack) {
    return fromPermCode1(pack);
}

inline constexpr bool Perm<6>::isImagePack(ImagePack pack) {
    return isPermCode1(pack);
}

inline constexpr Perm<6> Perm<6>::operator *(const Perm<6>& q) const {
    // For Perm<6>, we split the usual hard-coded product table into
    // two pieces (productTableLeft and productTableRight), which
    // together take up roughly 1/25 the space of a full product table.
    // See the productTableLeft notes for full details on how we split
    // these tables, and how we use them to compute products.
    //
    // Here we basically follow the reconstuction process described in
    // the productTableLeft documentation.

    Code2 left = code2_ / 24;
    Code2 right = ((left & 1) ? (code2_ % 24 ^ 1) : (code2_ % 24));
    Code2 ans = productTableLeft[left][productTableRight[right][q.code2_ >> 1]];
    return Perm<6>(((code2_ ^ q.code2_) & 1) ? (ans ^ 1) : ans);
}

inline Perm<6> Perm<6>::cachedComp(const Perm<6>& q) const {
    return Perm<6>(products_[code2_][q.code2_]);
}

inline Perm<6> Perm<6>::cachedComp(const Perm<6>& q, const Perm<6>& r) const {
    return Perm<6>(products_[code2_][products_[q.code2_][r.code2_]]);
}

inline constexpr Perm<6> Perm<6>::inverse() const {
    return Perm<6>(invS6[code2_]);
}

inline constexpr Perm<6> Perm<6>::pow(long exp) const {
    // Maximum order is 6, from cycles: (.)(..)(...) or (......)
    // Normalise exp to be in the range ( -order/2, +order/2 ].
    int ord = order();
    exp %= ord;
    if (exp < 0)
        exp += ord;
    if ((exp << 1) > ord)
        exp -= ord;

    switch (exp) {
        case 0: return Perm<6>();
        case 1: return *this;
        case -1: return inverse();
        case 2: return (*this) * (*this);
        case -2: {
            Perm<6> inv = inverse();
            return inv * inv;
        }
        default /* 3 */: return (*this) * (*this) * (*this);
    }
}

inline Perm<6> Perm<6>::cachedPow(long exp) const {
    // Maximum order is 6, from cycles: (.)(..)(...) or (......)
    // Normalise exp to be in the range ( -order/2, +order/2 ].
    int ord = order();
    exp %= ord;
    if (exp < 0)
        exp += ord;
    if ((exp << 1) > ord)
        exp -= ord;

    switch (exp) {
        case 0: return Perm<6>();
        case 1: return *this;
        case -1: return inverse();
        case 2: return Perm<6>(products_[code2_][code2_]);
        case -2: {
            Code2 inv = invS6[code2_];
            return Perm<6>(products_[inv][inv]);
        }
        default /* 3 */:
            return Perm<6>(products_[products_[code2_][code2_]][code2_]);
    }
}

inline constexpr int Perm<6>::order() const {
    return orderTable[code2_];
}

inline constexpr Perm<6> Perm<6>::reverse() const {
    // p becomes p * 543210 (which has second-generation code 719).
    return (*this) * Perm<6>(static_cast<Code2>(719));
}

inline constexpr int Perm<6>::sign() const {
    return (code2_ % 2 ? -1 : 1);
}

inline constexpr int Perm<6>::operator[](int source) const {
    return (code1Table[code2_] >> (3 * source)) & 0x07;
}

inline constexpr int Perm<6>::pre(int image) const {
    return (code1Table[invS6[code2_]] >> (3 * image)) & 0x07;
}

inline constexpr bool Perm<6>::operator == (const Perm<6>& other) const {
    return (code2_ == other.code2_);
}

inline constexpr bool Perm<6>::operator != (const Perm<6>& other) const {
    return (code2_ != other.code2_);
}

inline constexpr int Perm<6>::compareWith(const Perm<6>& other) const {
    // Computing orderedS6Index() is very fast, now that we use S6 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS6Index();
    int o2 = other.orderedS6Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<6>::isIdentity() const {
    return (code2_ == 0);
}

inline Perm<6>& Perm<6>::operator ++() {
    if (++code2_ == 720)
        code2_ = 0;
    return *this;
}

inline constexpr Perm<6> Perm<6>::operator ++(int) {
    Perm<6> ans(code2_);
    if (++code2_ == 720)
        code2_ = 0;
    return ans;
}

inline constexpr bool Perm<6>::operator < (const Perm<6>& rhs) const {
    return code2_ < rhs.code2_;
}

inline constexpr Perm<6> Perm<6>::rot(int i) {
    switch (i) {
        case 1: return Perm<6>(static_cast<Code2>(153));
        case 2: return Perm<6>(static_cast<Code2>(304));
        case 3: return Perm<6>(static_cast<Code2>(451));
        case 4: return Perm<6>(static_cast<Code2>(576));
        case 5: return Perm<6>(static_cast<Code2>(601));
        default: return Perm<6>(); // Identity
    }
}

inline Perm<6> Perm<6>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<6> Perm<6>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 359);
        return S6[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 719);
        return S6[d(gen)];
    }
}

inline void Perm<6>::tightEncode(std::ostream& out) const {
    // Write the Sn index in base 94, least significant digit first.
    // Note: 94^2 = 8836 > 6! = 720
    out << static_cast<char>((code2_ % 94) + 33)
        << static_cast<char>((code2_ / 94) + 33);
}

inline std::string Perm<6>::tightEncoding() const {
    // Write the Sn index in base 94, least significant digit first.
    // Note: 94^2 = 8836 > 6! = 720
    char ans[3] {
        static_cast<char>((code2_ % 94) + 33),
        static_cast<char>((code2_ / 94) + 33),
        0 };
    return ans;
}

inline Perm<6> Perm<6>::tightDecode(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

inline Perm<6> Perm<6>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

template <typename iterator>
Perm<6> Perm<6>::tightDecode(iterator start, iterator limit,
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
    if (code1 > 7 /* (6! / 94) */)
        throw InvalidInput("The tight encoding is invalid");

    code0 += (94 * code1);
    if (code0 >= 720)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Perm<6>(code0);
}

inline constexpr Perm<6>::Index Perm<6>::S6Index() const {
    return code2_;
}

inline constexpr Perm<6>::Index Perm<6>::SnIndex() const {
    return code2_;
}

inline constexpr Perm<6>::Index Perm<6>::orderedS6Index() const {
    return convOrderedUnordered(code2_);
}

inline constexpr Perm<6>::Index Perm<6>::orderedSnIndex() const {
    return convOrderedUnordered(code2_);
}

inline constexpr int Perm<6>::S6Index(int a, int b, int c, int d, int e,
        int f) {
    // First compute the ordered S6 index.
    int ans = 120 * a +
              24 * (b - (b > a ? 1 : 0)) +
              6 * (c - ((c > b ? 1 : 0) + (c > a ? 1 : 0))) +
              2 * ((d > e ? 1 : 0) + (d > f ? 1 : 0)) +
                  (e > f ? 1 : 0);

    // Then switch to the plain (unordered) S6 index.
    return convOrderedUnordered(ans);
}

inline constexpr bool Perm<6>::isConjugacyMinimal() const {
    switch (code2_) {
        // Put the 11 minimal cases in a switch statement, which may help
        // the compiler to optimise this better than we can.
        case 0: case 1: case 2: case 6: case 9: case 27:
        case 32: case 127: case 128: case 146: case 153:
            return true;
        default:
            return false;
    }
}

} // namespace regina

#endif

