
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

/*! \file maths/spec/perm5.h
 *  \brief Internal header for permutations of {0,1,2,3,4}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included within perm.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM5_H
#ifndef __DOXYGEN
#define __REGINA_PERM5_H
#endif

namespace regina {

/**
 * Represents a permutation of {0,1,2,3,4}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 4-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<5>, the internal permutation
 * codes have changed as of Regina 7.0:
 *
 * - _First-generation_ codes were used internally in Regina 6.0.1 and earlier.
 *   These are _image packs_: integers whose lowest three bits represent
 *   the image of 0, whose next lowest three bits represent the image of 1,
 *   and so on.  The routines permCode1(), setPermCode1(), fromPermCode1()
 *   and isPermCode1() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe pentachoron gluings.
 *
 * - _Second-generation_ codes are used internally in Regina 7.0 and above.
 *   These codes are integers between 0 and 119 inclusive, representing the
 *   index of the permutation in the array Perm<5>::S5.  The routines
 *   permCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by Perm<5>).
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \python Since Python does not support templates, this class is
 * made available under the name Perm5.
 *
 * \ingroup maths
 */
template <>
class Perm<5> {
    public:
        /**
         * The degree of the underlying symmetric group; that is, the
         * template parameter \a n.
         *
         * This compile-time constant allows the programmer to extract \a n
         * from the type (e.g., when writing templated code).
         */
        static constexpr int degree = 5;

        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on five elements.  In other words, this is a
         * native signed integer type large enough to store (5!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PermCodeType::Index;

        /**
         * The total number of permutations on five elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 120;

        /**
         * The total number of permutations on four elements.
         * This is the size of the array Sn_1.
         */
        static constexpr Index nPerms_1 = 24;

        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines 5 such images together, and so uses
         * 5 * \a imageBits bits in total.
         */
        static constexpr int imageBits = 3;

        /**
         * Indicates the native unsigned integer type used to store a
         * single image pack.  See the class notes for more information
         * on image packs, and how they are used to build the old
         * first-generation permutation codes.
         */
        using ImagePack = uint16_t;

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
        using Code2 = uint8_t;

    private:
        /**
         * A lightweight array-like object used to implement Perm<5>::S5.
         */
        struct S5Lookup {
            /**
             * Returns the permutation at the given index in the array S5.
             * See Perm<5>::S5 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 119 inclusive.
             * \return the corresponding permutation in S5.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S5.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 120; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::orderedS5.
         */
        struct OrderedS5Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS5.  See Perm<5>::orderedS5 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 119 inclusive.
             * \return the corresponding permutation in orderedS5.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array orderedS5.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 120; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::S4.
         */
        struct S4Lookup {
            /**
             * Returns the permutation at the given index in the array S4.
             * See Perm<5>::S4 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 23 inclusive.
             * \return the corresponding permutation in S4.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S4.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 24; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::orderedS4.
         */
        struct OrderedS4Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS4.  See Perm<5>::orderedS4 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 23 inclusive.
             * \return the corresponding permutation in orderedS4.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array orderedS4.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 24; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::S3.
         */
        struct S3Lookup {
            /**
             * Returns the permutation at the given index in the array S3.
             * See Perm<5>::S3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 5 inclusive.
             * \return the corresponding permutation in S3.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S3.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 6; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::orderedS3.
         */
        struct OrderedS3Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS3.  See Perm<5>::orderedS3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 5 inclusive.
             * \return the corresponding permutation in orderedS3.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array orderedS3.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 6; }
        };

        /**
         * A lightweight array-like object used to implement Perm<5>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<5>::S2 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * \param index an index between 0 and 1 inclusive.
             * \return the corresponding permutation in S2.
             */
            constexpr Perm<5> operator[] (int index) const;

            /**
             * Returns the number of permutations in the array S2.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return 2; }
        };

    public:
        /**
         * A do-nothing routine that assists with writing generic code.
         *
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations, and so this precompute() function does nothing.
         * The only point of having precompute() in Perm<5> is to make it
         * easier to write generic code that works with Perm<n> for any \a n.
         *
         * - If you know you are only working with Perm<5>, you do not need to
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
         * Gives fast array-like access to all possible permutations of
         * five elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `Sn[i]`.  The index \a i must be
         * between 0 and 119 inclusive.
         * This element access is extremely fast (a fact that is not true for
         * the larger permutation classes Perm<n> with \a n ≥ 8).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This array is different from Perm<5>::orderedSn, since \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S5Lookup Sn {};

        /**
         * Gives fast array-like access to all possible permutations of
         * five elements.
         *
         * This is a dimension-specific alias for Perm<5>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<5>::S5 are only available for small \a n.
         */
        static constexpr S5Lookup S5 {};

        /**
         * Gives fast array-like access to all possible permutations of five
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `orderedSn[i]`.  The index \a i
         * must be between 0 and 119 inclusive.
         * This element access is extremely fast (a fact that is not true for
         * the larger permutation classes Perm<n> with \a n ≥ 8).
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[4]).
         *
         * This array is different from Perm<5>::Sn, since \a orderedSn stores
         * permutations in lexicographical order, whereas \a Sn alternates
         * between even and odd permutations.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr OrderedS5Lookup orderedSn {};

        /**
         * Gives fast array-like access to all possible permutations of five
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<5>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<5>::orderedS5 are only
         * available for small \a n.
         */
        static constexpr OrderedS5Lookup orderedS5 {};

        /**
         * Gives fast array-like access to all possible permutations of
         * four elements.  In each permutation, 4 maps to 4.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `Sn_1[i]`.  The index \a i must be
         * between 0 and 23 inclusive.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This array is different from Perm<5>::orderedS4, since \a Sn_1
         * (or equivalently, \a S4) alternates between even and odd
         * permutations, whereas \a orderedS4 stores permutations in
         * lexicographical order.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S4Lookup Sn_1 {};

        /**
         * Gives fast array-like access to all possible permutations of
         * four elements.
         *
         * This is a dimension-specific alias for Perm<5>::Sn_1; see that
         * member for further information.
         *
         * Note that both permutation classes Perm<4> and Perm<5> have
         * an \a S4 array; these both store the same 24 permutations in the
         * same order (but of course using different data types).
         */
        static constexpr S4Lookup S4 {};

        /**
         * Gives fast array-like access to all possible permutations of four
         * elements in lexicographical order.  In each permutation, 4 maps to 4.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `orderedS4[i]`.  The index \a i
         * must be between 0 and 23 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[3]).
         *
         * This array is different from Perm<5>::S4, since \a orderedS4 stores
         * permutations in lexicographical order, whereas \a S4 (or
         * equivalently, \a Sn_1) alternates between even and odd permutations.
         *
         * Note that both permutation classes Perm<4> and Perm<5> have an
         * \a orderedS4 array; these both store the same 24 permutations
         * in the same order (but of course using different data types).
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr OrderedS4Lookup orderedS4 {};

        /**
         * Gives fast array-like access to all possible permutations of three
         * elements.  In each permutation, 3 maps to 3 and 4 maps to 4.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `S3[i]`.  The index \a i must be
         * between 0 and 5 inclusive.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This array is different from Perm<5>::orderedS3, since \a S3
         * alternates between even and odd permutations, whereas \a orderedS3
         * stores permutations in lexicographical order.
         *
         * Note that the small permutation classes Perm<3>, Perm<4> and Perm<5>
         * all have an \a S3 array; these all store the same six permutations
         * in the same order (but of course using different data types).
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S3Lookup S3 {};

        /**
         * Gives fast array-like access to all possible permutations of three
         * elements in lexicographical order.  In each permutation,
         * 3 maps to 3 and 4 maps to 4.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `orderedS3[i]`.  The index \a i
         * must be between 0 and 5 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[2]).
         *
         * This array is different from Perm<5>::S3, since \a orderedS3 stores
         * permutations in lexicographical order, whereas \a S3 alternates
         * between even and odd permutations.
         *
         * Note that the small permutation classes Perm<3>, Perm<4> and Perm<5>
         * all have an \a orderedS3 array; these all store the same six
         * permutations in the same order (but of course using different data
         * types).
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr OrderedS3Lookup orderedS3 {};

        /**
         * Gives fast array-like access to all possible permutations of
         * two elements.  In each permutation, 2 maps to 2, 3 maps to 3,
         * and 4 maps to 4.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `S2[i]`.  The index \a i must be
         * between 0 and 1 inclusive.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that all small permutation classes (Perm<2>, ..., Perm<5>)
         * have an \a S2 array: these all store the same two permutations in
         * the same order (but of course using different data types).
         *
         * There is no corresponding \a orderedS2 array, since the
         * (trivial) arrays \a S2 and \a orderedS2 are identical.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  This is now a lightweight
         * object, and is defined in the headers only; in particular, you
         * cannot make a reference to it (but you can always make a copy).
         */
        static constexpr S2Lookup S2 {};

    protected:
        Code2 code2_;
            /**< The internal second-generation permutation code
                 representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        constexpr Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3,4}.
         *
         * \param a the element to switch with \a b.
         * \param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3,4) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>} = {0,1,2,3,4}.
         *
         * \param a the desired image of 0.
         * \param b the desired image of 1.
         * \param c the desired image of 2.
         * \param d the desired image of 3.
         * \param e the desired image of 4.
         */
        constexpr Perm(int a, int b, int c, int d, int e);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3,4.
         *
         * \pre The elements of \a image are 0, 1, 2, 3 and 4 in some order.
         *
         * \param image the array of images.
         */
        constexpr Perm(const std::array<int, 5>& image);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>) respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>,<i>e0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>,<i>e1</i>} =
         * {0,1,2,3,4}.
         *
         * \param a0 the desired preimage of <i>a1</i>.
         * \param b0 the desired preimage of <i>b1</i>.
         * \param c0 the desired preimage of <i>c1</i>.
         * \param d0 the desired preimage of <i>d1</i>.
         * \param e0 the desired preimage of <i>e1</i>.
         * \param a1 the desired image of <i>a0</i>.
         * \param b1 the desired image of <i>b0</i>.
         * \param c1 the desired image of <i>c0</i>.
         * \param d1 the desired image of <i>d0</i>.
         * \param e1 the desired image of <i>e0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1,
            int d0, int d1, int e0, int e1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * \param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<5>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine permCode2() for details.
         *
         * \return the first-generation permutation code.
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
         * used internally by the Perm<5> class.
         *
         * \return the second-generation permutation code.
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
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine setPermCode2() for details.
         *
         * \param code the first-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode1(Code1 code);

        /**
         * Sets this permutation to that represented by the given
         * second-generation permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<5> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * \param code the second-generation code that will determine the
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
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * \param code the first-generation code for the new permutation.
         * \return the permutation represented by the given code.
         */
        static constexpr Perm<5> fromPermCode1(Code1 code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<5> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * \param code the second-generation code for the new permutation.
         * \return the permutation represented by the given code.
         */
        static constexpr Perm<5> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode1() or fromPermCode1(), and are returned by permCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * \param code the permutation code to test.
         * \return \c true if and only if the given code is a valid
         * first-generation permutation code.
         */
        static constexpr bool isPermCode1(Code1 code);

        /**
         * Determines whether the given character is a valid second-generation
         * permutation code.  Valid second-generation codes can be passed to
         * setPermCode2() or fromPermCode2(), and are returned by permCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<5> class.
         *
         * \param code the permutation code to test.
         * \return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static constexpr bool isPermCode2(Code2 code);

        /**
         * Returns the image pack that represents this permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<5>, this routine is identical to permCode1().
         *
         * \return the image pack for this permutation.
         */
        constexpr ImagePack imagePack() const;

        /**
         * Creates a permutation from the given image pack.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<5>, this routine is identical to fromPermCode1().
         *
         * \pre The argument \a pack is a valid image pack; see isImagePack()
         * for details.
         *
         * \param pack an image pack that describes a permutation.
         * \return the permutation represented by the given image pack.
         */
        static constexpr Perm fromImagePack(ImagePack pack);

        /**
         * Determines whether the given argument is the image pack of
         * some 5-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<5>, this routine is identical to isPermCode1().
         *
         * \param pack the candidate image pack to test.
         * \return \c true if and only if \a pack is a valid image pack.
         */
        static constexpr bool isImagePack(ImagePack pack);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * \param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * \return a reference to this permutation.
         */
        Perm<5>& operator = (const Perm<5>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is \a p, the
         * resulting permutation will be <i>p</i>∘<i>q</i>, and will satisfy
         * `(p*q)[x] == p[q[x]]`.
         *
         * \param q the permutation with which to compose this.
         * \return the composition of both permutations.
         */
        constexpr Perm<5> operator * (const Perm<5>& q) const;

        /**
         * An alias for the composition operator, provided to assist with
         * writing generic code.
         *
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations.  The only point of having cachedComp() in Perm<5>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<5>, you should just
         *   use the composition operator instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedComp().  (For Perm<5>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * The permutation that is returned is the same as you would
         * obtain by calling `(*this) * q`.
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedComp().  For Perm<5>,
         * precompute() does nothing; however, for other Perm<n> classes
         * a failure to do this will almost certainly crash your program.
         *
         * \param q the permutation to compose this with.
         * \return the composition of both permutations.
         */
        Perm<5> cachedComp(const Perm<5>& q) const;

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
         * lifetime of this program before calling cachedComp().  For Perm<5>,
         * precompute() does nothing; however, for other Perm<n> classes
         * a failure to do this will almost certainly crash your program.
         *
         * \param q the first permutation to compose this with.
         * \param r the second permutation to compose this with.
         * \return the composition of both permutations.
         */
        [[deprecated]] Perm<5> cachedComp(const Perm<5>& q, const Perm<5>& r)
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
        constexpr Perm<5> conjugate(const Perm<5>& q) const;

        /**
         * An alias for conjugate(), provided to assist with writing generic
         * code.
         *
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations.  The only point of having cachedConjugate() in
         * Perm<5> is to make it easier to write generic code that works with
         * Perm<n> for any \a n.
         *
         * - If you know you are only working with Perm<5>, you should just
         *   call conjugate() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedConjugate().  (For Perm<5>, which does not use
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
        Perm<5> cachedConjugate(const Perm<5>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * \return the inverse of this permutation.
         */
        constexpr Perm<5> inverse() const;

        /**
         * An alias for inverse(), provided to assist with writing
         * generic code.
         *
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations.  The only point of having cachedInverse() in Perm<5>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<5>, you should just
         *   call inverse() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedInverse().  (For Perm<5>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedInverse().  For
         * Perm<5>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \return the inverse of this permutation.
         */
        Perm<5> cachedInverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        constexpr Perm<5> pow(long exp) const;

        /**
         * An alias for pow(), provided to assist with writing generic code.
         *
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations.  The only point of having cachedPow() in Perm<5>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<5>, you should just
         *   call pow() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedPow().  (For Perm<5>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedPow().  For
         * Perm<5>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        Perm<5> cachedPow(long exp) const;

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
         * This specialised Perm<5> class does not use precomputation for its
         * optimisations.  The only point of having cachedOrder() in Perm<5>
         * is to make it easier to write generic code that works with Perm<n>
         * for any \a n.
         *
         * - If you know you are only working with Perm<5>, you should just
         *   call order() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedOrder().  (For Perm<5>, which does not use
         *   precomputation for its optimisations, precompute() does nothing.)
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedOrder().  For
         * Perm<5>, precompute() does nothing; however, for other Perm<n>
         * classes a failure to do this will almost certainly crash your
         * program.
         *
         * \return the order of this permutation.
         */
        int cachedOrder() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here _reverse_ means that we reverse the images of 0,...,4.
         * In other words, if permutation \a q is the
         * reverse of \a p, then `p[i] == q[4 - i]` for all \a i.
         */
        constexpr Perm<5> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * \return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        constexpr int sign() const;

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * \param source the integer whose image we wish to find.  This
         * should be between 0 and 4 inclusive.
         * \return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * \param image the integer whose preimage we wish to find.  This
         * should be between 0 and 4 inclusive.
         * \return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2, 3 and 4.
         *
         * \param other the permutation with which to compare this.
         * \return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<5>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3,4) under this
         * and the given permutation.
         *
         * Note that this does _not_ yield the same ordering of permutations
         * as used by the less-than and increment operators.  Moreover,
         * compareWith() is slower than the less-than operator to compute.
         *
         * \param other the permutation with which to compare this.
         * \return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm<5>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2, 3 and 4 is
         * mapped to itself.
         *
         * \return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<5>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<5>::Sn, which is the identity.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this permutation after the increment.
         */
        Perm<5>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<5>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<5>::Sn, which is the identity.
         *
         * \python This routine is named inc() since python does
         * not support the increment operator.
         *
         * \return a copy of this permutation before the increment took place.
         */
        constexpr Perm<5> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<5>::Sn.
         *
         * Note that this is _not_ the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * \param rhs the permutation to compare this against.
         * \return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<5>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps \a k to \a k + \a i (mod 5) for all \a k.
         *
         * \param i the image of 0; this must be between 0 and 4 inclusive.
         * \return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on five elements.
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
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * \return a random permutation.
         */
        static Perm rand(bool even = false);

        /**
         * Returns a random permutation on five elements, using the
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
         * guaranteed to be even (and again all even permutations are
         * returned with equal probability).
         * \return a random permutation.
         */
        template <class URBG>
        static Perm rand(URBG&& gen, bool even = false);

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of five adjacent digits
         * representing the images of 0, 1, 2, 3 and 4 respectively.
         * An example of a string representation is `30421`.
         *
         * \return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * \param len the length of the prefix required; this must be
         * between 0 and 5 inclusive.
         * \return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(int len) const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0 and 1.  The resulting string will therefore
         * have length two.
         *
         * \return a truncated string representation of this permutation.
         */
        std::string trunc2() const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0, 1 and 2.  The resulting string will therefore
         * have length three.
         *
         * \return a truncated string representation of this permutation.
         */
        std::string trunc3() const;

        /**
         * Returns a string representation of this permutation with only
         * the images of 0, 1, 2 and 3.  The resulting string will therefore
         * have length four.
         *
         * \return a truncated string representation of this permutation.
         */
        std::string trunc4() const;

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
         * of a 5-element permutation.
         *
         * \param enc the tight encoding for a 5-element permutation.
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
         * a tight encoding of a 5-element permutation.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a 5-element permutation.
         * \return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Hashes this permutation to a non-negative integer, allowing it
         * to be used for keys in hash tables.
         *
         * The implementation currently returns the internal permutation code
         * (which for Perm<5> will always fit within a \c size_t).  This
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
         * Specifically, for each \a i in the range <i>from</i>,...,4,
         * this routine will ensure that `image[i] == i`.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,4 are exactly
         * <i>from</i>,...,4, but possibly in a different order.
         *
         * \param from the first integer whose image should be reset.
         * This must be between 0 and 5 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<5>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * \return the index \a i for which this permutation is equal to
         * Perm<5>::Sn[i].  This will be between 0 and 119 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<5>::S5 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<7>::S7Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * \return the index \a i for which this permutation is equal to
         * Perm<5>::S5[i].  This will be between 0 and 119 inclusive.
         */
        constexpr Index S5Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<5>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * \return the lexicographical index of this permutation.
         * This will be between 0 and 119 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<5>::orderedSn array.
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
         * This will be between 0 and 119 inclusive.
         */
        constexpr Index orderedS5Index() const;

        /**
         * Extends a <i>k</i>-element permutation to a 5-element permutation,
         * where 2 ≤ \a k \< 5.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,4 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2, 3 or 4.
         *
         * \param p a permutation on \a k elements.
         * \return the same permutation expressed as a permutation on
         * five elements.
         */
        template <int k>
        static constexpr Perm<5> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 5-element
         * permutation, where \a k > 5.
         *
         * The resulting permutation will map 0,...,4 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[5],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,4 to 0,...,4 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 5.
         *
         * \param p a permutation on \a k elements.
         * \return the same permutation restricted to a permutation on
         * 5 elements.
         */
        template <int k>
        static constexpr Perm<5> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<5>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<5>, since it essentially
         * uses a hard-coded lookup table.
         *
         * \return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation `S5[i]`
         * is `imageTable[i][x]`.
         */
        static constexpr int imageTable[120][5] = {
            // Generated using Regina 6.0.
            { 0,1,2,3,4 }, { 0,1,2,4,3 }, { 0,1,3,4,2 }, { 0,1,3,2,4 },
            { 0,1,4,2,3 }, { 0,1,4,3,2 }, { 0,2,1,4,3 }, { 0,2,1,3,4 },
            { 0,2,3,1,4 }, { 0,2,3,4,1 }, { 0,2,4,3,1 }, { 0,2,4,1,3 },
            { 0,3,1,2,4 }, { 0,3,1,4,2 }, { 0,3,2,4,1 }, { 0,3,2,1,4 },
            { 0,3,4,1,2 }, { 0,3,4,2,1 }, { 0,4,1,3,2 }, { 0,4,1,2,3 },
            { 0,4,2,1,3 }, { 0,4,2,3,1 }, { 0,4,3,2,1 }, { 0,4,3,1,2 },
            { 1,0,2,4,3 }, { 1,0,2,3,4 }, { 1,0,3,2,4 }, { 1,0,3,4,2 },
            { 1,0,4,3,2 }, { 1,0,4,2,3 }, { 1,2,0,3,4 }, { 1,2,0,4,3 },
            { 1,2,3,4,0 }, { 1,2,3,0,4 }, { 1,2,4,0,3 }, { 1,2,4,3,0 },
            { 1,3,0,4,2 }, { 1,3,0,2,4 }, { 1,3,2,0,4 }, { 1,3,2,4,0 },
            { 1,3,4,2,0 }, { 1,3,4,0,2 }, { 1,4,0,2,3 }, { 1,4,0,3,2 },
            { 1,4,2,3,0 }, { 1,4,2,0,3 }, { 1,4,3,0,2 }, { 1,4,3,2,0 },
            { 2,0,1,3,4 }, { 2,0,1,4,3 }, { 2,0,3,4,1 }, { 2,0,3,1,4 },
            { 2,0,4,1,3 }, { 2,0,4,3,1 }, { 2,1,0,4,3 }, { 2,1,0,3,4 },
            { 2,1,3,0,4 }, { 2,1,3,4,0 }, { 2,1,4,3,0 }, { 2,1,4,0,3 },
            { 2,3,0,1,4 }, { 2,3,0,4,1 }, { 2,3,1,4,0 }, { 2,3,1,0,4 },
            { 2,3,4,0,1 }, { 2,3,4,1,0 }, { 2,4,0,3,1 }, { 2,4,0,1,3 },
            { 2,4,1,0,3 }, { 2,4,1,3,0 }, { 2,4,3,1,0 }, { 2,4,3,0,1 },
            { 3,0,1,4,2 }, { 3,0,1,2,4 }, { 3,0,2,1,4 }, { 3,0,2,4,1 },
            { 3,0,4,2,1 }, { 3,0,4,1,2 }, { 3,1,0,2,4 }, { 3,1,0,4,2 },
            { 3,1,2,4,0 }, { 3,1,2,0,4 }, { 3,1,4,0,2 }, { 3,1,4,2,0 },
            { 3,2,0,4,1 }, { 3,2,0,1,4 }, { 3,2,1,0,4 }, { 3,2,1,4,0 },
            { 3,2,4,1,0 }, { 3,2,4,0,1 }, { 3,4,0,1,2 }, { 3,4,0,2,1 },
            { 3,4,1,2,0 }, { 3,4,1,0,2 }, { 3,4,2,0,1 }, { 3,4,2,1,0 },
            { 4,0,1,2,3 }, { 4,0,1,3,2 }, { 4,0,2,3,1 }, { 4,0,2,1,3 },
            { 4,0,3,1,2 }, { 4,0,3,2,1 }, { 4,1,0,3,2 }, { 4,1,0,2,3 },
            { 4,1,2,0,3 }, { 4,1,2,3,0 }, { 4,1,3,2,0 }, { 4,1,3,0,2 },
            { 4,2,0,1,3 }, { 4,2,0,3,1 }, { 4,2,1,3,0 }, { 4,2,1,0,3 },
            { 4,2,3,0,1 }, { 4,2,3,1,0 }, { 4,3,0,2,1 }, { 4,3,0,1,2 },
            { 4,3,1,0,2 }, { 4,3,1,2,0 }, { 4,3,2,1,0 }, { 4,3,2,0,1 }
        };

        /**
         * Contains the inverses of the permutations in the array \a S5.
         *
         * Specifically, the inverse of permutation `S5[i]` is
         * the permutation `S5[ invS5[i] ]`.
         */
        static constexpr Code2 invS5[120] = {
             0,  1,  4,  3,  2,  5,  6,  7, 12, 19, 18, 13,
             8, 11, 20, 15, 16, 23, 10,  9, 14, 21, 22, 17,
            24, 25, 26, 29, 28, 27, 48, 49, 96, 73, 72, 97,
            52, 51, 74, 99,100, 77, 50, 53, 98, 75, 76,101,
            30, 31, 42, 37, 36, 43, 54, 55, 78,103,102, 79,
            60, 67,108, 85, 90,115, 66, 61, 84,109,114, 91,
            34, 33, 38, 45, 46, 41, 56, 59,104, 81, 82,107,
            68, 63, 86,111,116, 93, 64, 71,112, 89, 94,119,
            32, 35, 44, 39, 40, 47, 58, 57, 80,105,106, 83,
            62, 69,110, 87, 92,117, 70, 65, 88,113,118, 95
        };

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product `S5[x] * S5[y]` is the
         * permutation `S5[product[x][y]]`.
         *
         * This table contains 14.4 kilobytes of data, and so as of the year
         * 2021 we declare this is a perfectly reasonably memory cost for the
         * speed-ups that a hard-coded multiplication table gives us.
         */
        static constexpr Code2 productTable[120][120] = {
            // Generated using Regina 6.0.
            { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119 },
            { 1,0,5,4,3,2,7,6,11,10,9,8,19,18,21,20,23,22,13,12,15,14,17,16,25,24,29,28,27,26,31,30,35,34,33,32,43,42,45,44,47,46,37,36,39,38,41,40,49,48,53,52,51,50,55,54,59,58,57,56,67,66,69,68,71,70,61,60,63,62,65,64,97,96,99,98,101,100,103,102,105,104,107,106,109,108,111,110,113,112,115,114,117,116,119,118,73,72,75,74,77,76,79,78,81,80,83,82,85,84,87,86,89,88,91,90,93,92,95,94 },
            { 2,3,4,5,0,1,12,13,16,17,14,15,18,19,22,23,20,21,6,7,8,9,10,11,26,27,28,29,24,25,36,37,40,41,38,39,42,43,46,47,44,45,30,31,32,33,34,35,72,73,76,77,74,75,78,79,82,83,80,81,90,91,92,93,94,95,84,85,86,87,88,89,96,97,100,101,98,99,102,103,106,107,104,105,114,115,116,117,118,119,108,109,110,111,112,113,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71 },
            { 3,2,1,0,5,4,13,12,15,14,17,16,7,6,9,8,11,10,19,18,23,22,21,20,27,26,25,24,29,28,37,36,39,38,41,40,31,30,33,32,35,34,43,42,47,46,45,44,73,72,75,74,77,76,79,78,81,80,83,82,85,84,87,86,89,88,91,90,93,92,95,94,49,48,51,50,53,52,55,54,57,56,59,58,61,60,63,62,65,64,67,66,69,68,71,70,97,96,101,100,99,98,103,102,107,106,105,104,115,114,117,116,119,118,109,108,111,110,113,112 },
            { 4,5,0,1,2,3,18,19,20,21,22,23,6,7,10,11,8,9,12,13,16,17,14,15,28,29,24,25,26,27,42,43,44,45,46,47,30,31,34,35,32,33,36,37,40,41,38,39,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,48,49,52,53,50,51,54,55,58,59,56,57,66,67,68,69,70,71,60,61,62,63,64,65,72,73,76,77,74,75,78,79,82,83,80,81,90,91,92,93,94,95,84,85,86,87,88,89 },
            { 5,4,3,2,1,0,19,18,23,22,21,20,13,12,17,16,15,14,7,6,11,10,9,8,29,28,27,26,25,24,43,42,47,46,45,44,37,36,41,40,39,38,31,30,35,34,33,32,97,96,101,100,99,98,103,102,107,106,105,104,115,114,117,116,119,118,109,108,111,110,113,112,73,72,77,76,75,74,79,78,83,82,81,80,91,90,93,92,95,94,85,84,87,86,89,88,49,48,53,52,51,50,55,54,59,58,57,56,67,66,69,68,71,70,61,60,63,62,65,64 },
            { 6,7,10,11,8,9,0,1,4,5,2,3,20,21,18,19,22,23,14,15,12,13,16,17,48,49,52,53,50,51,54,55,58,59,56,57,66,67,68,69,70,71,60,61,62,63,64,65,24,25,28,29,26,27,30,31,34,35,32,33,42,43,44,45,46,47,36,37,38,39,40,41,98,99,96,97,100,101,108,109,110,111,112,113,102,103,104,105,106,107,114,115,118,119,116,117,74,75,72,73,76,77,84,85,86,87,88,89,78,79,80,81,82,83,90,91,94,95,92,93 },
            { 7,6,9,8,11,10,1,0,3,2,5,4,15,14,13,12,17,16,21,20,19,18,23,22,49,48,51,50,53,52,55,54,57,56,59,58,61,60,63,62,65,64,67,66,69,68,71,70,25,24,27,26,29,28,31,30,33,32,35,34,37,36,39,38,41,40,43,42,45,44,47,46,75,74,73,72,77,76,85,84,87,86,89,88,79,78,81,80,83,82,91,90,95,94,93,92,99,98,97,96,101,100,109,108,111,110,113,112,103,102,105,104,107,106,115,114,119,118,117,116 },
            { 8,9,6,7,10,11,14,15,12,13,16,17,0,1,2,3,4,5,20,21,22,23,18,19,50,51,48,49,52,53,60,61,62,63,64,65,54,55,56,57,58,59,66,67,70,71,68,69,74,75,72,73,76,77,84,85,86,87,88,89,78,79,80,81,82,83,90,91,94,95,92,93,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,98,99,100,101,96,97,108,109,112,113,110,111,114,115,118,119,116,117,102,103,104,105,106,107 },
            { 9,8,11,10,7,6,15,14,17,16,13,12,21,20,23,22,19,18,1,0,3,2,5,4,51,50,53,52,49,48,61,60,65,64,63,62,67,66,71,70,69,68,55,54,57,56,59,58,75,74,77,76,73,72,85,84,89,88,87,86,91,90,95,94,93,92,79,78,81,80,83,82,99,98,101,100,97,96,109,108,113,112,111,110,115,114,119,118,117,116,103,102,105,104,107,106,25,24,27,26,29,28,31,30,33,32,35,34,37,36,39,38,41,40,43,42,45,44,47,46 },
            { 10,11,8,9,6,7,20,21,22,23,18,19,14,15,16,17,12,13,0,1,4,5,2,3,52,53,50,51,48,49,66,67,70,71,68,69,60,61,64,65,62,63,54,55,58,59,56,57,98,99,100,101,96,97,108,109,112,113,110,111,114,115,118,119,116,117,102,103,104,105,106,107,74,75,76,77,72,73,84,85,88,89,86,87,90,91,94,95,92,93,78,79,80,81,82,83,24,25,28,29,26,27,30,31,34,35,32,33,42,43,44,45,46,47,36,37,38,39,40,41 },
            { 11,10,7,6,9,8,21,20,19,18,23,22,1,0,5,4,3,2,15,14,17,16,13,12,53,52,49,48,51,50,67,66,69,68,71,70,55,54,59,58,57,56,61,60,65,64,63,62,99,98,97,96,101,100,109,108,111,110,113,112,103,102,105,104,107,106,115,114,119,118,117,116,25,24,29,28,27,26,31,30,35,34,33,32,43,42,45,44,47,46,37,36,39,38,41,40,75,74,77,76,73,72,85,84,89,88,87,86,91,90,95,94,93,92,79,78,81,80,83,82 },
            { 12,13,14,15,16,17,2,3,0,1,4,5,8,9,6,7,10,11,22,23,18,19,20,21,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,26,27,24,25,28,29,36,37,38,39,40,41,30,31,32,33,34,35,42,43,46,47,44,45,50,51,48,49,52,53,60,61,62,63,64,65,54,55,56,57,58,59,66,67,70,71,68,69,100,101,96,97,98,99,114,115,116,117,118,119,102,103,106,107,104,105,108,109,112,113,110,111 },
            { 13,12,17,16,15,14,3,2,5,4,1,0,23,22,19,18,21,20,9,8,7,6,11,10,73,72,77,76,75,74,79,78,83,82,81,80,91,90,93,92,95,94,85,84,87,86,89,88,27,26,29,28,25,24,37,36,41,40,39,38,43,42,47,46,45,44,31,30,33,32,35,34,101,100,97,96,99,98,115,114,117,116,119,118,103,102,107,106,105,104,109,108,113,112,111,110,51,50,49,48,53,52,61,60,63,62,65,64,55,54,57,56,59,58,67,66,71,70,69,68 },
            { 14,15,16,17,12,13,8,9,10,11,6,7,22,23,20,21,18,19,2,3,0,1,4,5,74,75,76,77,72,73,84,85,88,89,86,87,90,91,94,95,92,93,78,79,80,81,82,83,50,51,52,53,48,49,60,61,64,65,62,63,66,67,70,71,68,69,54,55,56,57,58,59,100,101,98,99,96,97,114,115,118,119,116,117,108,109,112,113,110,111,102,103,106,107,104,105,26,27,24,25,28,29,36,37,38,39,40,41,30,31,32,33,34,35,42,43,46,47,44,45 },
            { 15,14,13,12,17,16,9,8,7,6,11,10,3,2,1,0,5,4,23,22,21,20,19,18,75,74,73,72,77,76,85,84,87,86,89,88,79,78,81,80,83,82,91,90,95,94,93,92,51,50,49,48,53,52,61,60,63,62,65,64,55,54,57,56,59,58,67,66,71,70,69,68,27,26,25,24,29,28,37,36,39,38,41,40,31,30,33,32,35,34,43,42,47,46,45,44,101,100,99,98,97,96,115,114,119,118,117,116,109,108,113,112,111,110,103,102,107,106,105,104 },
            { 16,17,12,13,14,15,22,23,18,19,20,21,2,3,4,5,0,1,8,9,10,11,6,7,76,77,72,73,74,75,90,91,92,93,94,95,78,79,82,83,80,81,84,85,88,89,86,87,100,101,96,97,98,99,114,115,116,117,118,119,102,103,106,107,104,105,108,109,112,113,110,111,26,27,28,29,24,25,36,37,40,41,38,39,42,43,46,47,44,45,30,31,32,33,34,35,50,51,52,53,48,49,60,61,64,65,62,63,66,67,70,71,68,69,54,55,56,57,58,59 },
            { 17,16,15,14,13,12,23,22,21,20,19,18,9,8,11,10,7,6,3,2,5,4,1,0,77,76,75,74,73,72,91,90,95,94,93,92,85,84,89,88,87,86,79,78,83,82,81,80,101,100,99,98,97,96,115,114,119,118,117,116,109,108,113,112,111,110,103,102,107,106,105,104,51,50,53,52,49,48,61,60,65,64,63,62,67,66,71,70,69,68,55,54,57,56,59,58,27,26,29,28,25,24,37,36,41,40,39,38,43,42,47,46,45,44,31,30,33,32,35,34 },
            { 18,19,22,23,20,21,4,5,2,3,0,1,16,17,12,13,14,15,10,11,6,7,8,9,96,97,100,101,98,99,102,103,106,107,104,105,114,115,116,117,118,119,108,109,110,111,112,113,28,29,26,27,24,25,42,43,46,47,44,45,36,37,40,41,38,39,30,31,34,35,32,33,76,77,72,73,74,75,90,91,92,93,94,95,78,79,82,83,80,81,84,85,88,89,86,87,52,53,48,49,50,51,66,67,68,69,70,71,54,55,58,59,56,57,60,61,64,65,62,63 },
            { 19,18,21,20,23,22,5,4,1,0,3,2,11,10,7,6,9,8,17,16,13,12,15,14,97,96,99,98,101,100,103,102,105,104,107,106,109,108,111,110,113,112,115,114,117,116,119,118,29,28,25,24,27,26,43,42,45,44,47,46,31,30,35,34,33,32,37,36,41,40,39,38,53,52,49,48,51,50,67,66,69,68,71,70,55,54,59,58,57,56,61,60,65,64,63,62,77,76,73,72,75,74,91,90,93,92,95,94,79,78,83,82,81,80,85,84,89,88,87,86 },
            { 20,21,18,19,22,23,10,11,6,7,8,9,4,5,0,1,2,3,16,17,14,15,12,13,98,99,96,97,100,101,108,109,110,111,112,113,102,103,104,105,106,107,114,115,118,119,116,117,52,53,48,49,50,51,66,67,68,69,70,71,54,55,58,59,56,57,60,61,64,65,62,63,28,29,24,25,26,27,42,43,44,45,46,47,30,31,34,35,32,33,36,37,40,41,38,39,76,77,74,75,72,73,90,91,94,95,92,93,84,85,88,89,86,87,78,79,82,83,80,81 },
            { 21,20,23,22,19,18,11,10,9,8,7,6,17,16,15,14,13,12,5,4,1,0,3,2,99,98,101,100,97,96,109,108,113,112,111,110,115,114,119,118,117,116,103,102,105,104,107,106,53,52,51,50,49,48,67,66,71,70,69,68,61,60,65,64,63,62,55,54,59,58,57,56,77,76,75,74,73,72,91,90,95,94,93,92,85,84,89,88,87,86,79,78,83,82,81,80,29,28,25,24,27,26,43,42,45,44,47,46,31,30,35,34,33,32,37,36,41,40,39,38 },
            { 22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1,100,101,98,99,96,97,114,115,118,119,116,117,108,109,112,113,110,111,102,103,106,107,104,105,76,77,74,75,72,73,90,91,94,95,92,93,84,85,88,89,86,87,78,79,82,83,80,81,52,53,50,51,48,49,66,67,70,71,68,69,60,61,64,65,62,63,54,55,58,59,56,57,28,29,26,27,24,25,42,43,46,47,44,45,36,37,40,41,38,39,30,31,34,35,32,33 },
            { 23,22,19,18,21,20,17,16,13,12,15,14,5,4,3,2,1,0,11,10,9,8,7,6,101,100,97,96,99,98,115,114,117,116,119,118,103,102,107,106,105,104,109,108,113,112,111,110,77,76,73,72,75,74,91,90,93,92,95,94,79,78,83,82,81,80,85,84,89,88,87,86,29,28,27,26,25,24,43,42,47,46,45,44,37,36,41,40,39,38,31,30,35,34,33,32,53,52,51,50,49,48,67,66,71,70,69,68,61,60,65,64,63,62,55,54,59,58,57,56 },
            { 24,25,28,29,26,27,30,31,34,35,32,33,42,43,44,45,46,47,36,37,38,39,40,41,0,1,4,5,2,3,6,7,10,11,8,9,18,19,20,21,22,23,12,13,14,15,16,17,54,55,58,59,56,57,48,49,52,53,50,51,68,69,66,67,70,71,62,63,60,61,64,65,102,103,104,105,106,107,96,97,98,99,100,101,110,111,108,109,112,113,116,117,114,115,118,119,78,79,80,81,82,83,72,73,74,75,76,77,86,87,84,85,88,89,92,93,90,91,94,95 },
            { 25,24,27,26,29,28,31,30,33,32,35,34,37,36,39,38,41,40,43,42,45,44,47,46,1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14,17,16,19,18,21,20,23,22,55,54,57,56,59,58,49,48,51,50,53,52,63,62,61,60,65,64,69,68,67,66,71,70,79,78,81,80,83,82,73,72,75,74,77,76,87,86,85,84,89,88,93,92,91,90,95,94,103,102,105,104,107,106,97,96,99,98,101,100,111,110,109,108,113,112,117,116,115,114,119,118 },
            { 26,27,24,25,28,29,36,37,38,39,40,41,30,31,32,33,34,35,42,43,46,47,44,45,2,3,0,1,4,5,12,13,14,15,16,17,6,7,8,9,10,11,18,19,22,23,20,21,78,79,80,81,82,83,72,73,74,75,76,77,86,87,84,85,88,89,92,93,90,91,94,95,54,55,56,57,58,59,48,49,50,51,52,53,62,63,60,61,64,65,68,69,66,67,70,71,102,103,106,107,104,105,96,97,100,101,98,99,116,117,114,115,118,119,110,111,108,109,112,113 },
            { 27,26,29,28,25,24,37,36,41,40,39,38,43,42,47,46,45,44,31,30,33,32,35,34,3,2,5,4,1,0,13,12,17,16,15,14,19,18,23,22,21,20,7,6,9,8,11,10,79,78,83,82,81,80,73,72,77,76,75,74,93,92,91,90,95,94,87,86,85,84,89,88,103,102,107,106,105,104,97,96,101,100,99,98,117,116,115,114,119,118,111,110,109,108,113,112,55,54,57,56,59,58,49,48,51,50,53,52,63,62,61,60,65,64,69,68,67,66,71,70 },
            { 28,29,26,27,24,25,42,43,46,47,44,45,36,37,40,41,38,39,30,31,34,35,32,33,4,5,2,3,0,1,18,19,22,23,20,21,12,13,16,17,14,15,6,7,10,11,8,9,102,103,106,107,104,105,96,97,100,101,98,99,116,117,114,115,118,119,110,111,108,109,112,113,78,79,82,83,80,81,72,73,76,77,74,75,92,93,90,91,94,95,86,87,84,85,88,89,54,55,58,59,56,57,48,49,52,53,50,51,68,69,66,67,70,71,62,63,60,61,64,65 },
            { 29,28,25,24,27,26,43,42,45,44,47,46,31,30,35,34,33,32,37,36,41,40,39,38,5,4,1,0,3,2,19,18,21,20,23,22,7,6,11,10,9,8,13,12,17,16,15,14,103,102,105,104,107,106,97,96,99,98,101,100,111,110,109,108,113,112,117,116,115,114,119,118,55,54,59,58,57,56,49,48,53,52,51,50,69,68,67,66,71,70,63,62,61,60,65,64,79,78,83,82,81,80,73,72,77,76,75,74,93,92,91,90,95,94,87,86,85,84,89,88 },
            { 30,31,32,33,34,35,24,25,26,27,28,29,38,39,36,37,40,41,44,45,42,43,46,47,54,55,56,57,58,59,48,49,50,51,52,53,62,63,60,61,64,65,68,69,66,67,70,71,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,80,81,78,79,82,83,86,87,84,85,88,89,72,73,74,75,76,77,92,93,94,95,90,91,104,105,102,103,106,107,110,111,108,109,112,113,96,97,98,99,100,101,116,117,118,119,114,115 },
            { 31,30,35,34,33,32,25,24,29,28,27,26,45,44,43,42,47,46,39,38,37,36,41,40,55,54,59,58,57,56,49,48,53,52,51,50,69,68,67,66,71,70,63,62,61,60,65,64,1,0,5,4,3,2,7,6,11,10,9,8,19,18,21,20,23,22,13,12,15,14,17,16,105,104,103,102,107,106,111,110,109,108,113,112,97,96,99,98,101,100,117,116,119,118,115,114,81,80,79,78,83,82,87,86,85,84,89,88,73,72,75,74,77,76,93,92,95,94,91,90 },
            { 32,33,34,35,30,31,38,39,40,41,36,37,44,45,46,47,42,43,24,25,26,27,28,29,56,57,58,59,54,55,62,63,64,65,60,61,68,69,70,71,66,67,48,49,50,51,52,53,80,81,82,83,78,79,86,87,88,89,84,85,92,93,94,95,90,91,72,73,74,75,76,77,104,105,106,107,102,103,110,111,112,113,108,109,116,117,118,119,114,115,96,97,98,99,100,101,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 },
            { 33,32,31,30,35,34,39,38,37,36,41,40,25,24,27,26,29,28,45,44,47,46,43,42,57,56,55,54,59,58,63,62,61,60,65,64,49,48,51,50,53,52,69,68,71,70,67,66,81,80,79,78,83,82,87,86,85,84,89,88,73,72,75,74,77,76,93,92,95,94,91,90,1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14,17,16,19,18,21,20,23,22,105,104,107,106,103,102,111,110,113,112,109,108,117,116,119,118,115,114,97,96,99,98,101,100 },
            { 34,35,30,31,32,33,44,45,42,43,46,47,24,25,28,29,26,27,38,39,40,41,36,37,58,59,54,55,56,57,68,69,66,67,70,71,48,49,52,53,50,51,62,63,64,65,60,61,104,105,102,103,106,107,110,111,108,109,112,113,96,97,98,99,100,101,116,117,118,119,114,115,0,1,4,5,2,3,6,7,10,11,8,9,18,19,20,21,22,23,12,13,14,15,16,17,80,81,82,83,78,79,86,87,88,89,84,85,92,93,94,95,90,91,72,73,74,75,76,77 },
            { 35,34,33,32,31,30,45,44,47,46,43,42,39,38,41,40,37,36,25,24,29,28,27,26,59,58,57,56,55,54,69,68,71,70,67,66,63,62,65,64,61,60,49,48,53,52,51,50,105,104,107,106,103,102,111,110,113,112,109,108,117,116,119,118,115,114,97,96,99,98,101,100,81,80,83,82,79,78,87,86,89,88,85,84,93,92,95,94,91,90,73,72,75,74,77,76,1,0,5,4,3,2,7,6,11,10,9,8,19,18,21,20,23,22,13,12,15,14,17,16 },
            { 36,37,40,41,38,39,26,27,28,29,24,25,46,47,42,43,44,45,32,33,30,31,34,35,78,79,82,83,80,81,72,73,76,77,74,75,92,93,90,91,94,95,86,87,84,85,88,89,2,3,4,5,0,1,12,13,16,17,14,15,18,19,22,23,20,21,6,7,8,9,10,11,106,107,102,103,104,105,116,117,114,115,118,119,96,97,100,101,98,99,110,111,112,113,108,109,56,57,54,55,58,59,62,63,60,61,64,65,48,49,50,51,52,53,68,69,70,71,66,67 },
            { 37,36,39,38,41,40,27,26,25,24,29,28,33,32,31,30,35,34,47,46,43,42,45,44,79,78,81,80,83,82,73,72,75,74,77,76,87,86,85,84,89,88,93,92,91,90,95,94,3,2,1,0,5,4,13,12,15,14,17,16,7,6,9,8,11,10,19,18,23,22,21,20,57,56,55,54,59,58,63,62,61,60,65,64,49,48,51,50,53,52,69,68,71,70,67,66,107,106,103,102,105,104,117,116,115,114,119,118,97,96,101,100,99,98,111,110,113,112,109,108 },
            { 38,39,36,37,40,41,32,33,30,31,34,35,26,27,24,25,28,29,46,47,44,45,42,43,80,81,78,79,82,83,86,87,84,85,88,89,72,73,74,75,76,77,92,93,94,95,90,91,56,57,54,55,58,59,62,63,60,61,64,65,48,49,50,51,52,53,68,69,70,71,66,67,2,3,0,1,4,5,12,13,14,15,16,17,6,7,8,9,10,11,18,19,22,23,20,21,106,107,104,105,102,103,116,117,118,119,114,115,110,111,112,113,108,109,96,97,100,101,98,99 },
            { 39,38,41,40,37,36,33,32,35,34,31,30,47,46,45,44,43,42,27,26,25,24,29,28,81,80,83,82,79,78,87,86,89,88,85,84,93,92,95,94,91,90,73,72,75,74,77,76,57,56,59,58,55,54,63,62,65,64,61,60,69,68,71,70,67,66,49,48,51,50,53,52,107,106,105,104,103,102,117,116,119,118,115,114,111,110,113,112,109,108,97,96,101,100,99,98,3,2,1,0,5,4,13,12,15,14,17,16,7,6,9,8,11,10,19,18,23,22,21,20 },
            { 40,41,38,39,36,37,46,47,44,45,42,43,32,33,34,35,30,31,26,27,28,29,24,25,82,83,80,81,78,79,92,93,94,95,90,91,86,87,88,89,84,85,72,73,76,77,74,75,106,107,104,105,102,103,116,117,118,119,114,115,110,111,112,113,108,109,96,97,100,101,98,99,56,57,58,59,54,55,62,63,64,65,60,61,68,69,70,71,66,67,48,49,50,51,52,53,2,3,4,5,0,1,12,13,16,17,14,15,18,19,22,23,20,21,6,7,8,9,10,11 },
            { 41,40,37,36,39,38,47,46,43,42,45,44,27,26,29,28,25,24,33,32,35,34,31,30,83,82,79,78,81,80,93,92,91,90,95,94,73,72,77,76,75,74,87,86,89,88,85,84,107,106,103,102,105,104,117,116,115,114,119,118,97,96,101,100,99,98,111,110,113,112,109,108,3,2,5,4,1,0,13,12,17,16,15,14,19,18,23,22,21,20,7,6,9,8,11,10,57,56,59,58,55,54,63,62,65,64,61,60,69,68,71,70,67,66,49,48,51,50,53,52 },
            { 42,43,44,45,46,47,28,29,24,25,26,27,34,35,30,31,32,33,40,41,36,37,38,39,102,103,104,105,106,107,96,97,98,99,100,101,110,111,108,109,112,113,116,117,114,115,118,119,4,5,0,1,2,3,18,19,20,21,22,23,6,7,10,11,8,9,12,13,16,17,14,15,58,59,54,55,56,57,68,69,66,67,70,71,48,49,52,53,50,51,62,63,64,65,60,61,82,83,78,79,80,81,92,93,90,91,94,95,72,73,76,77,74,75,86,87,88,89,84,85 },
            { 43,42,47,46,45,44,29,28,27,26,25,24,41,40,37,36,39,38,35,34,31,30,33,32,103,102,107,106,105,104,97,96,101,100,99,98,117,116,115,114,119,118,111,110,109,108,113,112,5,4,3,2,1,0,19,18,23,22,21,20,13,12,17,16,15,14,7,6,11,10,9,8,83,82,79,78,81,80,93,92,91,90,95,94,73,72,77,76,75,74,87,86,89,88,85,84,59,58,55,54,57,56,69,68,67,66,71,70,49,48,53,52,51,50,63,62,65,64,61,60 },
            { 44,45,46,47,42,43,34,35,32,33,30,31,40,41,38,39,36,37,28,29,24,25,26,27,104,105,106,107,102,103,110,111,112,113,108,109,116,117,118,119,114,115,96,97,98,99,100,101,58,59,56,57,54,55,68,69,70,71,66,67,62,63,64,65,60,61,48,49,52,53,50,51,82,83,80,81,78,79,92,93,94,95,90,91,86,87,88,89,84,85,72,73,76,77,74,75,4,5,0,1,2,3,18,19,20,21,22,23,6,7,10,11,8,9,12,13,16,17,14,15 },
            { 45,44,43,42,47,46,35,34,31,30,33,32,29,28,25,24,27,26,41,40,39,38,37,36,105,104,103,102,107,106,111,110,109,108,113,112,97,96,99,98,101,100,117,116,119,118,115,114,59,58,55,54,57,56,69,68,67,66,71,70,49,48,53,52,51,50,63,62,65,64,61,60,5,4,1,0,3,2,19,18,21,20,23,22,7,6,11,10,9,8,13,12,17,16,15,14,83,82,81,80,79,78,93,92,95,94,91,90,87,86,89,88,85,84,73,72,77,76,75,74 },
            { 46,47,42,43,44,45,40,41,36,37,38,39,28,29,26,27,24,25,34,35,32,33,30,31,106,107,102,103,104,105,116,117,114,115,118,119,96,97,100,101,98,99,110,111,112,113,108,109,82,83,78,79,80,81,92,93,90,91,94,95,72,73,76,77,74,75,86,87,88,89,84,85,4,5,2,3,0,1,18,19,22,23,20,21,12,13,16,17,14,15,6,7,10,11,8,9,58,59,56,57,54,55,68,69,70,71,66,67,62,63,64,65,60,61,48,49,52,53,50,51 },
            { 47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,107,106,105,104,103,102,117,116,119,118,115,114,111,110,113,112,109,108,97,96,101,100,99,98,83,82,81,80,79,78,93,92,95,94,91,90,87,86,89,88,85,84,73,72,77,76,75,74,59,58,57,56,55,54,69,68,71,70,67,66,63,62,65,64,61,60,49,48,53,52,51,50,5,4,3,2,1,0,19,18,23,22,21,20,13,12,17,16,15,14,7,6,11,10,9,8 },
            { 48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,6,7,8,9,10,11,0,1,2,3,4,5,14,15,12,13,16,17,20,21,18,19,22,23,30,31,32,33,34,35,24,25,26,27,28,29,38,39,36,37,40,41,44,45,42,43,46,47,84,85,86,87,88,89,74,75,72,73,76,77,80,81,78,79,82,83,94,95,90,91,92,93,108,109,110,111,112,113,98,99,96,97,100,101,104,105,102,103,106,107,118,119,114,115,116,117 },
            { 49,48,53,52,51,50,55,54,59,58,57,56,67,66,69,68,71,70,61,60,63,62,65,64,7,6,11,10,9,8,1,0,5,4,3,2,21,20,19,18,23,22,15,14,13,12,17,16,31,30,35,34,33,32,25,24,29,28,27,26,45,44,43,42,47,46,39,38,37,36,41,40,109,108,111,110,113,112,99,98,97,96,101,100,105,104,103,102,107,106,119,118,115,114,117,116,85,84,87,86,89,88,75,74,73,72,77,76,81,80,79,78,83,82,95,94,91,90,93,92 },
            { 50,51,52,53,48,49,60,61,64,65,62,63,66,67,70,71,68,69,54,55,56,57,58,59,8,9,10,11,6,7,14,15,16,17,12,13,20,21,22,23,18,19,0,1,2,3,4,5,84,85,88,89,86,87,74,75,76,77,72,73,94,95,90,91,92,93,80,81,78,79,82,83,108,109,112,113,110,111,98,99,100,101,96,97,118,119,114,115,116,117,104,105,102,103,106,107,30,31,32,33,34,35,24,25,26,27,28,29,38,39,36,37,40,41,44,45,42,43,46,47 },
            { 51,50,49,48,53,52,61,60,63,62,65,64,55,54,57,56,59,58,67,66,71,70,69,68,9,8,7,6,11,10,15,14,13,12,17,16,1,0,3,2,5,4,21,20,23,22,19,18,85,84,87,86,89,88,75,74,73,72,77,76,81,80,79,78,83,82,95,94,91,90,93,92,31,30,33,32,35,34,25,24,27,26,29,28,39,38,37,36,41,40,45,44,43,42,47,46,109,108,113,112,111,110,99,98,101,100,97,96,119,118,115,114,117,116,105,104,103,102,107,106 },
            { 52,53,48,49,50,51,66,67,68,69,70,71,54,55,58,59,56,57,60,61,64,65,62,63,10,11,6,7,8,9,20,21,18,19,22,23,0,1,4,5,2,3,14,15,16,17,12,13,108,109,110,111,112,113,98,99,96,97,100,101,104,105,102,103,106,107,118,119,114,115,116,117,30,31,34,35,32,33,24,25,28,29,26,27,44,45,42,43,46,47,38,39,36,37,40,41,84,85,88,89,86,87,74,75,76,77,72,73,94,95,90,91,92,93,80,81,78,79,82,83 },
            { 53,52,51,50,49,48,67,66,71,70,69,68,61,60,65,64,63,62,55,54,59,58,57,56,11,10,9,8,7,6,21,20,23,22,19,18,15,14,17,16,13,12,1,0,5,4,3,2,109,108,113,112,111,110,99,98,101,100,97,96,119,118,115,114,117,116,105,104,103,102,107,106,85,84,89,88,87,86,75,74,77,76,73,72,95,94,91,90,93,92,81,80,79,78,83,82,31,30,35,34,33,32,25,24,29,28,27,26,45,44,43,42,47,46,39,38,37,36,41,40 },
            { 54,55,58,59,56,57,48,49,52,53,50,51,68,69,66,67,70,71,62,63,60,61,64,65,30,31,34,35,32,33,24,25,28,29,26,27,44,45,42,43,46,47,38,39,36,37,40,41,6,7,10,11,8,9,0,1,4,5,2,3,20,21,18,19,22,23,14,15,12,13,16,17,110,111,108,109,112,113,104,105,102,103,106,107,98,99,96,97,100,101,118,119,116,117,114,115,86,87,84,85,88,89,80,81,78,79,82,83,74,75,72,73,76,77,94,95,92,93,90,91 },
            { 55,54,57,56,59,58,49,48,51,50,53,52,63,62,61,60,65,64,69,68,67,66,71,70,31,30,33,32,35,34,25,24,27,26,29,28,39,38,37,36,41,40,45,44,43,42,47,46,7,6,9,8,11,10,1,0,3,2,5,4,15,14,13,12,17,16,21,20,19,18,23,22,87,86,85,84,89,88,81,80,79,78,83,82,75,74,73,72,77,76,95,94,93,92,91,90,111,110,109,108,113,112,105,104,103,102,107,106,99,98,97,96,101,100,119,118,117,116,115,114 },
            { 56,57,54,55,58,59,62,63,60,61,64,65,48,49,50,51,52,53,68,69,70,71,66,67,32,33,30,31,34,35,38,39,36,37,40,41,24,25,26,27,28,29,44,45,46,47,42,43,86,87,84,85,88,89,80,81,78,79,82,83,74,75,72,73,76,77,94,95,92,93,90,91,6,7,8,9,10,11,0,1,2,3,4,5,14,15,12,13,16,17,20,21,18,19,22,23,110,111,112,113,108,109,104,105,106,107,102,103,118,119,116,117,114,115,98,99,96,97,100,101 },
            { 57,56,59,58,55,54,63,62,65,64,61,60,69,68,71,70,67,66,49,48,51,50,53,52,33,32,35,34,31,30,39,38,41,40,37,36,45,44,47,46,43,42,25,24,27,26,29,28,87,86,89,88,85,84,81,80,83,82,79,78,95,94,93,92,91,90,75,74,73,72,77,76,111,110,113,112,109,108,105,104,107,106,103,102,119,118,117,116,115,114,99,98,97,96,101,100,7,6,9,8,11,10,1,0,3,2,5,4,15,14,13,12,17,16,21,20,19,18,23,22 },
            { 58,59,56,57,54,55,68,69,70,71,66,67,62,63,64,65,60,61,48,49,52,53,50,51,34,35,32,33,30,31,44,45,46,47,42,43,38,39,40,41,36,37,24,25,28,29,26,27,110,111,112,113,108,109,104,105,106,107,102,103,118,119,116,117,114,115,98,99,96,97,100,101,86,87,88,89,84,85,80,81,82,83,78,79,94,95,92,93,90,91,74,75,72,73,76,77,6,7,10,11,8,9,0,1,4,5,2,3,20,21,18,19,22,23,14,15,12,13,16,17 },
            { 59,58,55,54,57,56,69,68,67,66,71,70,49,48,53,52,51,50,63,62,65,64,61,60,35,34,31,30,33,32,45,44,43,42,47,46,25,24,29,28,27,26,39,38,41,40,37,36,111,110,109,108,113,112,105,104,103,102,107,106,99,98,97,96,101,100,119,118,117,116,115,114,7,6,11,10,9,8,1,0,5,4,3,2,21,20,19,18,23,22,15,14,13,12,17,16,87,86,89,88,85,84,81,80,83,82,79,78,95,94,93,92,91,90,75,74,73,72,77,76 },
            { 60,61,62,63,64,65,50,51,48,49,52,53,56,57,54,55,58,59,70,71,66,67,68,69,84,85,86,87,88,89,74,75,72,73,76,77,80,81,78,79,82,83,94,95,90,91,92,93,8,9,6,7,10,11,14,15,12,13,16,17,0,1,2,3,4,5,20,21,22,23,18,19,32,33,30,31,34,35,38,39,36,37,40,41,24,25,26,27,28,29,44,45,46,47,42,43,112,113,108,109,110,111,118,119,114,115,116,117,98,99,100,101,96,97,104,105,106,107,102,103 },
            { 61,60,65,64,63,62,51,50,53,52,49,48,71,70,67,66,69,68,57,56,55,54,59,58,85,84,89,88,87,86,75,74,77,76,73,72,95,94,91,90,93,92,81,80,79,78,83,82,9,8,11,10,7,6,15,14,17,16,13,12,21,20,23,22,19,18,1,0,3,2,5,4,113,112,109,108,111,110,119,118,115,114,117,116,99,98,101,100,97,96,105,104,107,106,103,102,33,32,31,30,35,34,39,38,37,36,41,40,25,24,27,26,29,28,45,44,47,46,43,42 },
            { 62,63,64,65,60,61,56,57,58,59,54,55,70,71,68,69,66,67,50,51,48,49,52,53,86,87,88,89,84,85,80,81,82,83,78,79,94,95,92,93,90,91,74,75,72,73,76,77,32,33,34,35,30,31,38,39,40,41,36,37,44,45,46,47,42,43,24,25,26,27,28,29,112,113,110,111,108,109,118,119,116,117,114,115,104,105,106,107,102,103,98,99,100,101,96,97,8,9,6,7,10,11,14,15,12,13,16,17,0,1,2,3,4,5,20,21,22,23,18,19 },
            { 63,62,61,60,65,64,57,56,55,54,59,58,51,50,49,48,53,52,71,70,69,68,67,66,87,86,85,84,89,88,81,80,79,78,83,82,75,74,73,72,77,76,95,94,93,92,91,90,33,32,31,30,35,34,39,38,37,36,41,40,25,24,27,26,29,28,45,44,47,46,43,42,9,8,7,6,11,10,15,14,13,12,17,16,1,0,3,2,5,4,21,20,23,22,19,18,113,112,111,110,109,108,119,118,117,116,115,114,105,104,107,106,103,102,99,98,101,100,97,96 },
            { 64,65,60,61,62,63,70,71,66,67,68,69,50,51,52,53,48,49,56,57,58,59,54,55,88,89,84,85,86,87,94,95,90,91,92,93,74,75,76,77,72,73,80,81,82,83,78,79,112,113,108,109,110,111,118,119,114,115,116,117,98,99,100,101,96,97,104,105,106,107,102,103,8,9,10,11,6,7,14,15,16,17,12,13,20,21,22,23,18,19,0,1,2,3,4,5,32,33,34,35,30,31,38,39,40,41,36,37,44,45,46,47,42,43,24,25,26,27,28,29 },
            { 65,64,63,62,61,60,71,70,69,68,67,66,57,56,59,58,55,54,51,50,53,52,49,48,89,88,87,86,85,84,95,94,93,92,91,90,81,80,83,82,79,78,75,74,77,76,73,72,113,112,111,110,109,108,119,118,117,116,115,114,105,104,107,106,103,102,99,98,101,100,97,96,33,32,35,34,31,30,39,38,41,40,37,36,45,44,47,46,43,42,25,24,27,26,29,28,9,8,11,10,7,6,15,14,17,16,13,12,21,20,23,22,19,18,1,0,3,2,5,4 },
            { 66,67,70,71,68,69,52,53,50,51,48,49,64,65,60,61,62,63,58,59,54,55,56,57,108,109,112,113,110,111,98,99,100,101,96,97,118,119,114,115,116,117,104,105,102,103,106,107,10,11,8,9,6,7,20,21,22,23,18,19,14,15,16,17,12,13,0,1,4,5,2,3,88,89,84,85,86,87,94,95,90,91,92,93,74,75,76,77,72,73,80,81,82,83,78,79,34,35,30,31,32,33,44,45,42,43,46,47,24,25,28,29,26,27,38,39,40,41,36,37 },
            { 67,66,69,68,71,70,53,52,49,48,51,50,59,58,55,54,57,56,65,64,61,60,63,62,109,108,111,110,113,112,99,98,97,96,101,100,105,104,103,102,107,106,119,118,115,114,117,116,11,10,7,6,9,8,21,20,19,18,23,22,1,0,5,4,3,2,15,14,17,16,13,12,35,34,31,30,33,32,45,44,43,42,47,46,25,24,29,28,27,26,39,38,41,40,37,36,89,88,85,84,87,86,95,94,91,90,93,92,75,74,77,76,73,72,81,80,83,82,79,78 },
            { 68,69,66,67,70,71,58,59,54,55,56,57,52,53,48,49,50,51,64,65,62,63,60,61,110,111,108,109,112,113,104,105,102,103,106,107,98,99,96,97,100,101,118,119,116,117,114,115,34,35,30,31,32,33,44,45,42,43,46,47,24,25,28,29,26,27,38,39,40,41,36,37,10,11,6,7,8,9,20,21,18,19,22,23,0,1,4,5,2,3,14,15,16,17,12,13,88,89,86,87,84,85,94,95,92,93,90,91,80,81,82,83,78,79,74,75,76,77,72,73 },
            { 69,68,71,70,67,66,59,58,57,56,55,54,65,64,63,62,61,60,53,52,49,48,51,50,111,110,113,112,109,108,105,104,107,106,103,102,119,118,117,116,115,114,99,98,97,96,101,100,35,34,33,32,31,30,45,44,47,46,43,42,39,38,41,40,37,36,25,24,29,28,27,26,89,88,87,86,85,84,95,94,93,92,91,90,81,80,83,82,79,78,75,74,77,76,73,72,11,10,7,6,9,8,21,20,19,18,23,22,1,0,5,4,3,2,15,14,17,16,13,12 },
            { 70,71,68,69,66,67,64,65,62,63,60,61,58,59,56,57,54,55,52,53,50,51,48,49,112,113,110,111,108,109,118,119,116,117,114,115,104,105,106,107,102,103,98,99,100,101,96,97,88,89,86,87,84,85,94,95,92,93,90,91,80,81,82,83,78,79,74,75,76,77,72,73,34,35,32,33,30,31,44,45,46,47,42,43,38,39,40,41,36,37,24,25,28,29,26,27,10,11,8,9,6,7,20,21,22,23,18,19,14,15,16,17,12,13,0,1,4,5,2,3 },
            { 71,70,67,66,69,68,65,64,61,60,63,62,53,52,51,50,49,48,59,58,57,56,55,54,113,112,109,108,111,110,119,118,115,114,117,116,99,98,101,100,97,96,105,104,107,106,103,102,89,88,85,84,87,86,95,94,91,90,93,92,75,74,77,76,73,72,81,80,83,82,79,78,11,10,9,8,7,6,21,20,23,22,19,18,15,14,17,16,13,12,1,0,5,4,3,2,35,34,33,32,31,30,45,44,47,46,43,42,39,38,41,40,37,36,25,24,29,28,27,26 },
            { 72,73,76,77,74,75,78,79,82,83,80,81,90,91,92,93,94,95,84,85,86,87,88,89,12,13,16,17,14,15,2,3,4,5,0,1,22,23,18,19,20,21,8,9,6,7,10,11,36,37,40,41,38,39,26,27,28,29,24,25,46,47,42,43,44,45,32,33,30,31,34,35,114,115,116,117,118,119,100,101,96,97,98,99,106,107,102,103,104,105,112,113,108,109,110,111,60,61,62,63,64,65,50,51,48,49,52,53,56,57,54,55,58,59,70,71,66,67,68,69 },
            { 73,72,75,74,77,76,79,78,81,80,83,82,85,84,87,86,89,88,91,90,93,92,95,94,13,12,15,14,17,16,3,2,1,0,5,4,9,8,7,6,11,10,23,22,19,18,21,20,37,36,39,38,41,40,27,26,25,24,29,28,33,32,31,30,35,34,47,46,43,42,45,44,61,60,63,62,65,64,51,50,49,48,53,52,57,56,55,54,59,58,71,70,67,66,69,68,115,114,117,116,119,118,101,100,97,96,99,98,107,106,103,102,105,104,113,112,109,108,111,110 },
            { 74,75,72,73,76,77,84,85,86,87,88,89,78,79,80,81,82,83,90,91,94,95,92,93,14,15,12,13,16,17,8,9,6,7,10,11,2,3,0,1,4,5,22,23,20,21,18,19,60,61,62,63,64,65,50,51,48,49,52,53,56,57,54,55,58,59,70,71,66,67,68,69,36,37,38,39,40,41,26,27,24,25,28,29,32,33,30,31,34,35,46,47,42,43,44,45,114,115,118,119,116,117,100,101,98,99,96,97,112,113,108,109,110,111,106,107,102,103,104,105 },
            { 75,74,77,76,73,72,85,84,89,88,87,86,91,90,95,94,93,92,79,78,81,80,83,82,15,14,17,16,13,12,9,8,11,10,7,6,23,22,21,20,19,18,3,2,1,0,5,4,61,60,65,64,63,62,51,50,53,52,49,48,71,70,67,66,69,68,57,56,55,54,59,58,115,114,119,118,117,116,101,100,99,98,97,96,113,112,109,108,111,110,107,106,103,102,105,104,37,36,39,38,41,40,27,26,25,24,29,28,33,32,31,30,35,34,47,46,43,42,45,44 },
            { 76,77,74,75,72,73,90,91,94,95,92,93,84,85,88,89,86,87,78,79,82,83,80,81,16,17,14,15,12,13,22,23,20,21,18,19,8,9,10,11,6,7,2,3,4,5,0,1,114,115,118,119,116,117,100,101,98,99,96,97,112,113,108,109,110,111,106,107,102,103,104,105,60,61,64,65,62,63,50,51,52,53,48,49,70,71,66,67,68,69,56,57,54,55,58,59,36,37,40,41,38,39,26,27,28,29,24,25,46,47,42,43,44,45,32,33,30,31,34,35 },
            { 77,76,73,72,75,74,91,90,93,92,95,94,79,78,83,82,81,80,85,84,89,88,87,86,17,16,13,12,15,14,23,22,19,18,21,20,3,2,5,4,1,0,9,8,11,10,7,6,115,114,117,116,119,118,101,100,97,96,99,98,107,106,103,102,105,104,113,112,109,108,111,110,37,36,41,40,39,38,27,26,29,28,25,24,47,46,43,42,45,44,33,32,31,30,35,34,61,60,65,64,63,62,51,50,53,52,49,48,71,70,67,66,69,68,57,56,55,54,59,58 },
            { 78,79,80,81,82,83,72,73,74,75,76,77,86,87,84,85,88,89,92,93,90,91,94,95,36,37,38,39,40,41,26,27,24,25,28,29,32,33,30,31,34,35,46,47,42,43,44,45,12,13,14,15,16,17,2,3,0,1,4,5,8,9,6,7,10,11,22,23,18,19,20,21,62,63,60,61,64,65,56,57,54,55,58,59,50,51,48,49,52,53,70,71,68,69,66,67,116,117,114,115,118,119,106,107,102,103,104,105,100,101,96,97,98,99,112,113,110,111,108,109 },
            { 79,78,83,82,81,80,73,72,77,76,75,74,93,92,91,90,95,94,87,86,85,84,89,88,37,36,41,40,39,38,27,26,29,28,25,24,47,46,43,42,45,44,33,32,31,30,35,34,13,12,17,16,15,14,3,2,5,4,1,0,23,22,19,18,21,20,9,8,7,6,11,10,117,116,115,114,119,118,107,106,103,102,105,104,101,100,97,96,99,98,113,112,111,110,109,108,63,62,61,60,65,64,57,56,55,54,59,58,51,50,49,48,53,52,71,70,69,68,67,66 },
            { 80,81,82,83,78,79,86,87,88,89,84,85,92,93,94,95,90,91,72,73,74,75,76,77,38,39,40,41,36,37,32,33,34,35,30,31,46,47,44,45,42,43,26,27,24,25,28,29,62,63,64,65,60,61,56,57,58,59,54,55,70,71,68,69,66,67,50,51,48,49,52,53,116,117,118,119,114,115,106,107,104,105,102,103,112,113,110,111,108,109,100,101,96,97,98,99,12,13,14,15,16,17,2,3,0,1,4,5,8,9,6,7,10,11,22,23,18,19,20,21 },
            { 81,80,79,78,83,82,87,86,85,84,89,88,73,72,75,74,77,76,93,92,95,94,91,90,39,38,37,36,41,40,33,32,31,30,35,34,27,26,25,24,29,28,47,46,45,44,43,42,63,62,61,60,65,64,57,56,55,54,59,58,51,50,49,48,53,52,71,70,69,68,67,66,13,12,15,14,17,16,3,2,1,0,5,4,9,8,7,6,11,10,23,22,19,18,21,20,117,116,119,118,115,114,107,106,105,104,103,102,113,112,111,110,109,108,101,100,97,96,99,98 },
            { 82,83,78,79,80,81,92,93,90,91,94,95,72,73,76,77,74,75,86,87,88,89,84,85,40,41,36,37,38,39,46,47,42,43,44,45,26,27,28,29,24,25,32,33,34,35,30,31,116,117,114,115,118,119,106,107,102,103,104,105,100,101,96,97,98,99,112,113,110,111,108,109,12,13,16,17,14,15,2,3,4,5,0,1,22,23,18,19,20,21,8,9,6,7,10,11,62,63,64,65,60,61,56,57,58,59,54,55,70,71,68,69,66,67,50,51,48,49,52,53 },
            { 83,82,81,80,79,78,93,92,95,94,91,90,87,86,89,88,85,84,73,72,77,76,75,74,41,40,39,38,37,36,47,46,45,44,43,42,33,32,35,34,31,30,27,26,29,28,25,24,117,116,119,118,115,114,107,106,105,104,103,102,113,112,111,110,109,108,101,100,97,96,99,98,63,62,65,64,61,60,57,56,59,58,55,54,71,70,69,68,67,66,51,50,49,48,53,52,13,12,17,16,15,14,3,2,5,4,1,0,23,22,19,18,21,20,9,8,7,6,11,10 },
            { 84,85,88,89,86,87,74,75,76,77,72,73,94,95,90,91,92,93,80,81,78,79,82,83,60,61,64,65,62,63,50,51,52,53,48,49,70,71,66,67,68,69,56,57,54,55,58,59,14,15,16,17,12,13,8,9,10,11,6,7,22,23,20,21,18,19,2,3,0,1,4,5,118,119,114,115,116,117,112,113,108,109,110,111,100,101,98,99,96,97,106,107,104,105,102,103,38,39,36,37,40,41,32,33,30,31,34,35,26,27,24,25,28,29,46,47,44,45,42,43 },
            { 85,84,87,86,89,88,75,74,73,72,77,76,81,80,79,78,83,82,95,94,91,90,93,92,61,60,63,62,65,64,51,50,49,48,53,52,57,56,55,54,59,58,71,70,67,66,69,68,15,14,13,12,17,16,9,8,7,6,11,10,3,2,1,0,5,4,23,22,21,20,19,18,39,38,37,36,41,40,33,32,31,30,35,34,27,26,25,24,29,28,47,46,45,44,43,42,119,118,115,114,117,116,113,112,109,108,111,110,101,100,99,98,97,96,107,106,105,104,103,102 },
            { 86,87,84,85,88,89,80,81,78,79,82,83,74,75,72,73,76,77,94,95,92,93,90,91,62,63,60,61,64,65,56,57,54,55,58,59,50,51,48,49,52,53,70,71,68,69,66,67,38,39,36,37,40,41,32,33,30,31,34,35,26,27,24,25,28,29,46,47,44,45,42,43,14,15,12,13,16,17,8,9,6,7,10,11,2,3,0,1,4,5,22,23,20,21,18,19,118,119,116,117,114,115,112,113,110,111,108,109,106,107,104,105,102,103,100,101,98,99,96,97 },
            { 87,86,89,88,85,84,81,80,83,82,79,78,95,94,93,92,91,90,75,74,73,72,77,76,63,62,65,64,61,60,57,56,59,58,55,54,71,70,69,68,67,66,51,50,49,48,53,52,39,38,41,40,37,36,33,32,35,34,31,30,47,46,45,44,43,42,27,26,25,24,29,28,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,15,14,13,12,17,16,9,8,7,6,11,10,3,2,1,0,5,4,23,22,21,20,19,18 },
            { 88,89,86,87,84,85,94,95,92,93,90,91,80,81,82,83,78,79,74,75,76,77,72,73,64,65,62,63,60,61,70,71,68,69,66,67,56,57,58,59,54,55,50,51,52,53,48,49,118,119,116,117,114,115,112,113,110,111,108,109,106,107,104,105,102,103,100,101,98,99,96,97,38,39,40,41,36,37,32,33,34,35,30,31,46,47,44,45,42,43,26,27,24,25,28,29,14,15,16,17,12,13,8,9,10,11,6,7,22,23,20,21,18,19,2,3,0,1,4,5 },
            { 89,88,85,84,87,86,95,94,91,90,93,92,75,74,77,76,73,72,81,80,83,82,79,78,65,64,61,60,63,62,71,70,67,66,69,68,51,50,53,52,49,48,57,56,59,58,55,54,119,118,115,114,117,116,113,112,109,108,111,110,101,100,99,98,97,96,107,106,105,104,103,102,15,14,17,16,13,12,9,8,11,10,7,6,23,22,21,20,19,18,3,2,1,0,5,4,39,38,41,40,37,36,33,32,35,34,31,30,47,46,45,44,43,42,27,26,25,24,29,28 },
            { 90,91,92,93,94,95,76,77,72,73,74,75,82,83,78,79,80,81,88,89,84,85,86,87,114,115,116,117,118,119,100,101,96,97,98,99,106,107,102,103,104,105,112,113,108,109,110,111,16,17,12,13,14,15,22,23,18,19,20,21,2,3,4,5,0,1,8,9,10,11,6,7,40,41,36,37,38,39,46,47,42,43,44,45,26,27,28,29,24,25,32,33,34,35,30,31,64,65,60,61,62,63,70,71,66,67,68,69,50,51,52,53,48,49,56,57,58,59,54,55 },
            { 91,90,95,94,93,92,77,76,75,74,73,72,89,88,85,84,87,86,83,82,79,78,81,80,115,114,119,118,117,116,101,100,99,98,97,96,113,112,109,108,111,110,107,106,103,102,105,104,17,16,15,14,13,12,23,22,21,20,19,18,9,8,11,10,7,6,3,2,5,4,1,0,65,64,61,60,63,62,71,70,67,66,69,68,51,50,53,52,49,48,57,56,59,58,55,54,41,40,37,36,39,38,47,46,43,42,45,44,27,26,29,28,25,24,33,32,35,34,31,30 },
            { 92,93,94,95,90,91,82,83,80,81,78,79,88,89,86,87,84,85,76,77,72,73,74,75,116,117,118,119,114,115,106,107,104,105,102,103,112,113,110,111,108,109,100,101,96,97,98,99,40,41,38,39,36,37,46,47,44,45,42,43,32,33,34,35,30,31,26,27,28,29,24,25,64,65,62,63,60,61,70,71,68,69,66,67,56,57,58,59,54,55,50,51,52,53,48,49,16,17,12,13,14,15,22,23,18,19,20,21,2,3,4,5,0,1,8,9,10,11,6,7 },
            { 93,92,91,90,95,94,83,82,79,78,81,80,77,76,73,72,75,74,89,88,87,86,85,84,117,116,115,114,119,118,107,106,103,102,105,104,101,100,97,96,99,98,113,112,111,110,109,108,41,40,37,36,39,38,47,46,43,42,45,44,27,26,29,28,25,24,33,32,35,34,31,30,17,16,13,12,15,14,23,22,19,18,21,20,3,2,5,4,1,0,9,8,11,10,7,6,65,64,63,62,61,60,71,70,69,68,67,66,57,56,59,58,55,54,51,50,53,52,49,48 },
            { 94,95,90,91,92,93,88,89,84,85,86,87,76,77,74,75,72,73,82,83,80,81,78,79,118,119,114,115,116,117,112,113,108,109,110,111,100,101,98,99,96,97,106,107,104,105,102,103,64,65,60,61,62,63,70,71,66,67,68,69,50,51,52,53,48,49,56,57,58,59,54,55,16,17,14,15,12,13,22,23,20,21,18,19,8,9,10,11,6,7,2,3,4,5,0,1,40,41,38,39,36,37,46,47,44,45,42,43,32,33,34,35,30,31,26,27,28,29,24,25 },
            { 95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,65,64,63,62,61,60,71,70,69,68,67,66,57,56,59,58,55,54,51,50,53,52,49,48,41,40,39,38,37,36,47,46,45,44,43,42,33,32,35,34,31,30,27,26,29,28,25,24,17,16,15,14,13,12,23,22,21,20,19,18,9,8,11,10,7,6,3,2,5,4,1,0 },
            { 96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,18,19,20,21,22,23,4,5,0,1,2,3,10,11,6,7,8,9,16,17,12,13,14,15,42,43,44,45,46,47,28,29,24,25,26,27,34,35,30,31,32,33,40,41,36,37,38,39,66,67,68,69,70,71,52,53,48,49,50,51,58,59,54,55,56,57,64,65,60,61,62,63,90,91,92,93,94,95,76,77,72,73,74,75,82,83,78,79,80,81,88,89,84,85,86,87 },
            { 97,96,101,100,99,98,103,102,107,106,105,104,115,114,117,116,119,118,109,108,111,110,113,112,19,18,23,22,21,20,5,4,3,2,1,0,17,16,13,12,15,14,11,10,7,6,9,8,43,42,47,46,45,44,29,28,27,26,25,24,41,40,37,36,39,38,35,34,31,30,33,32,91,90,93,92,95,94,77,76,73,72,75,74,83,82,79,78,81,80,89,88,85,84,87,86,67,66,69,68,71,70,53,52,49,48,51,50,59,58,55,54,57,56,65,64,61,60,63,62 },
            { 98,99,100,101,96,97,108,109,112,113,110,111,114,115,118,119,116,117,102,103,104,105,106,107,20,21,22,23,18,19,10,11,8,9,6,7,16,17,14,15,12,13,4,5,0,1,2,3,66,67,70,71,68,69,52,53,50,51,48,49,64,65,60,61,62,63,58,59,54,55,56,57,90,91,94,95,92,93,76,77,74,75,72,73,88,89,84,85,86,87,82,83,78,79,80,81,42,43,44,45,46,47,28,29,24,25,26,27,34,35,30,31,32,33,40,41,36,37,38,39 },
            { 99,98,97,96,101,100,109,108,111,110,113,112,103,102,105,104,107,106,115,114,119,118,117,116,21,20,19,18,23,22,11,10,7,6,9,8,5,4,1,0,3,2,17,16,15,14,13,12,67,66,69,68,71,70,53,52,49,48,51,50,59,58,55,54,57,56,65,64,61,60,63,62,43,42,45,44,47,46,29,28,25,24,27,26,35,34,31,30,33,32,41,40,37,36,39,38,91,90,95,94,93,92,77,76,75,74,73,72,89,88,85,84,87,86,83,82,79,78,81,80 },
            { 100,101,96,97,98,99,114,115,116,117,118,119,102,103,106,107,104,105,108,109,112,113,110,111,22,23,18,19,20,21,16,17,12,13,14,15,4,5,2,3,0,1,10,11,8,9,6,7,90,91,92,93,94,95,76,77,72,73,74,75,82,83,78,79,80,81,88,89,84,85,86,87,42,43,46,47,44,45,28,29,26,27,24,25,40,41,36,37,38,39,34,35,30,31,32,33,66,67,70,71,68,69,52,53,50,51,48,49,64,65,60,61,62,63,58,59,54,55,56,57 },
            { 101,100,99,98,97,96,115,114,119,118,117,116,109,108,113,112,111,110,103,102,107,106,105,104,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,91,90,95,94,93,92,77,76,75,74,73,72,89,88,85,84,87,86,83,82,79,78,81,80,67,66,71,70,69,68,53,52,51,50,49,48,65,64,61,60,63,62,59,58,55,54,57,56,43,42,47,46,45,44,29,28,27,26,25,24,41,40,37,36,39,38,35,34,31,30,33,32 },
            { 102,103,106,107,104,105,96,97,100,101,98,99,116,117,114,115,118,119,110,111,108,109,112,113,42,43,46,47,44,45,28,29,26,27,24,25,40,41,36,37,38,39,34,35,30,31,32,33,18,19,22,23,20,21,4,5,2,3,0,1,16,17,12,13,14,15,10,11,6,7,8,9,92,93,90,91,94,95,82,83,78,79,80,81,76,77,72,73,74,75,88,89,86,87,84,85,68,69,66,67,70,71,58,59,54,55,56,57,52,53,48,49,50,51,64,65,62,63,60,61 },
            { 103,102,105,104,107,106,97,96,99,98,101,100,111,110,109,108,113,112,117,116,115,114,119,118,43,42,45,44,47,46,29,28,25,24,27,26,35,34,31,30,33,32,41,40,37,36,39,38,19,18,21,20,23,22,5,4,1,0,3,2,11,10,7,6,9,8,17,16,13,12,15,14,69,68,67,66,71,70,59,58,55,54,57,56,53,52,49,48,51,50,65,64,63,62,61,60,93,92,91,90,95,94,83,82,79,78,81,80,77,76,73,72,75,74,89,88,87,86,85,84 },
            { 104,105,102,103,106,107,110,111,108,109,112,113,96,97,98,99,100,101,116,117,118,119,114,115,44,45,42,43,46,47,34,35,30,31,32,33,28,29,24,25,26,27,40,41,38,39,36,37,68,69,66,67,70,71,58,59,54,55,56,57,52,53,48,49,50,51,64,65,62,63,60,61,18,19,20,21,22,23,4,5,0,1,2,3,10,11,6,7,8,9,16,17,12,13,14,15,92,93,94,95,90,91,82,83,80,81,78,79,88,89,86,87,84,85,76,77,72,73,74,75 },
            { 105,104,107,106,103,102,111,110,113,112,109,108,117,116,119,118,115,114,97,96,99,98,101,100,45,44,47,46,43,42,35,34,33,32,31,30,41,40,39,38,37,36,29,28,25,24,27,26,69,68,71,70,67,66,59,58,57,56,55,54,65,64,63,62,61,60,53,52,49,48,51,50,93,92,95,94,91,90,83,82,81,80,79,78,89,88,87,86,85,84,77,76,73,72,75,74,19,18,21,20,23,22,5,4,1,0,3,2,11,10,7,6,9,8,17,16,13,12,15,14 },
            { 106,107,104,105,102,103,116,117,118,119,114,115,110,111,112,113,108,109,96,97,100,101,98,99,46,47,44,45,42,43,40,41,38,39,36,37,34,35,32,33,30,31,28,29,26,27,24,25,92,93,94,95,90,91,82,83,80,81,78,79,88,89,86,87,84,85,76,77,72,73,74,75,68,69,70,71,66,67,58,59,56,57,54,55,64,65,62,63,60,61,52,53,48,49,50,51,18,19,22,23,20,21,4,5,2,3,0,1,16,17,12,13,14,15,10,11,6,7,8,9 },
            { 107,106,103,102,105,104,117,116,115,114,119,118,97,96,101,100,99,98,111,110,113,112,109,108,47,46,43,42,45,44,41,40,37,36,39,38,29,28,27,26,25,24,35,34,33,32,31,30,93,92,91,90,95,94,83,82,79,78,81,80,77,76,73,72,75,74,89,88,87,86,85,84,19,18,23,22,21,20,5,4,3,2,1,0,17,16,13,12,15,14,11,10,7,6,9,8,69,68,71,70,67,66,59,58,57,56,55,54,65,64,63,62,61,60,53,52,49,48,51,50 },
            { 108,109,110,111,112,113,98,99,96,97,100,101,104,105,102,103,106,107,118,119,114,115,116,117,66,67,68,69,70,71,52,53,48,49,50,51,58,59,54,55,56,57,64,65,60,61,62,63,20,21,18,19,22,23,10,11,6,7,8,9,4,5,0,1,2,3,16,17,14,15,12,13,44,45,42,43,46,47,34,35,30,31,32,33,28,29,24,25,26,27,40,41,38,39,36,37,94,95,90,91,92,93,88,89,84,85,86,87,76,77,74,75,72,73,82,83,80,81,78,79 },
            { 109,108,113,112,111,110,99,98,101,100,97,96,119,118,115,114,117,116,105,104,103,102,107,106,67,66,71,70,69,68,53,52,51,50,49,48,65,64,61,60,63,62,59,58,55,54,57,56,21,20,23,22,19,18,11,10,9,8,7,6,17,16,15,14,13,12,5,4,1,0,3,2,95,94,91,90,93,92,89,88,85,84,87,86,77,76,75,74,73,72,83,82,81,80,79,78,45,44,43,42,47,46,35,34,31,30,33,32,29,28,25,24,27,26,41,40,39,38,37,36 },
            { 110,111,112,113,108,109,104,105,106,107,102,103,118,119,116,117,114,115,98,99,96,97,100,101,68,69,70,71,66,67,58,59,56,57,54,55,64,65,62,63,60,61,52,53,48,49,50,51,44,45,46,47,42,43,34,35,32,33,30,31,40,41,38,39,36,37,28,29,24,25,26,27,94,95,92,93,90,91,88,89,86,87,84,85,82,83,80,81,78,79,76,77,74,75,72,73,20,21,18,19,22,23,10,11,6,7,8,9,4,5,0,1,2,3,16,17,14,15,12,13 },
            { 111,110,109,108,113,112,105,104,103,102,107,106,99,98,97,96,101,100,119,118,117,116,115,114,69,68,67,66,71,70,59,58,55,54,57,56,53,52,49,48,51,50,65,64,63,62,61,60,45,44,43,42,47,46,35,34,31,30,33,32,29,28,25,24,27,26,41,40,39,38,37,36,21,20,19,18,23,22,11,10,7,6,9,8,5,4,1,0,3,2,17,16,15,14,13,12,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72 },
            { 112,113,108,109,110,111,118,119,114,115,116,117,98,99,100,101,96,97,104,105,106,107,102,103,70,71,66,67,68,69,64,65,60,61,62,63,52,53,50,51,48,49,58,59,56,57,54,55,94,95,90,91,92,93,88,89,84,85,86,87,76,77,74,75,72,73,82,83,80,81,78,79,20,21,22,23,18,19,10,11,8,9,6,7,16,17,14,15,12,13,4,5,0,1,2,3,44,45,46,47,42,43,34,35,32,33,30,31,40,41,38,39,36,37,28,29,24,25,26,27 },
            { 113,112,111,110,109,108,119,118,117,116,115,114,105,104,107,106,103,102,99,98,101,100,97,96,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,45,44,47,46,43,42,35,34,33,32,31,30,41,40,39,38,37,36,29,28,25,24,27,26,21,20,23,22,19,18,11,10,9,8,7,6,17,16,15,14,13,12,5,4,1,0,3,2 },
            { 114,115,118,119,116,117,100,101,98,99,96,97,112,113,108,109,110,111,106,107,102,103,104,105,90,91,94,95,92,93,76,77,74,75,72,73,88,89,84,85,86,87,82,83,78,79,80,81,22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1,70,71,66,67,68,69,64,65,60,61,62,63,52,53,50,51,48,49,58,59,56,57,54,55,46,47,42,43,44,45,40,41,36,37,38,39,28,29,26,27,24,25,34,35,32,33,30,31 },
            { 115,114,117,116,119,118,101,100,97,96,99,98,107,106,103,102,105,104,113,112,109,108,111,110,91,90,93,92,95,94,77,76,73,72,75,74,83,82,79,78,81,80,89,88,85,84,87,86,23,22,19,18,21,20,17,16,13,12,15,14,5,4,3,2,1,0,11,10,9,8,7,6,47,46,43,42,45,44,41,40,37,36,39,38,29,28,27,26,25,24,35,34,33,32,31,30,71,70,67,66,69,68,65,64,61,60,63,62,53,52,51,50,49,48,59,58,57,56,55,54 },
            { 116,117,114,115,118,119,106,107,102,103,104,105,100,101,96,97,98,99,112,113,110,111,108,109,92,93,90,91,94,95,82,83,78,79,80,81,76,77,72,73,74,75,88,89,86,87,84,85,46,47,42,43,44,45,40,41,36,37,38,39,28,29,26,27,24,25,34,35,32,33,30,31,22,23,18,19,20,21,16,17,12,13,14,15,4,5,2,3,0,1,10,11,8,9,6,7,70,71,68,69,66,67,64,65,62,63,60,61,58,59,56,57,54,55,52,53,50,51,48,49 },
            { 117,116,119,118,115,114,107,106,105,104,103,102,113,112,111,110,109,108,101,100,97,96,99,98,93,92,95,94,91,90,83,82,81,80,79,78,89,88,87,86,85,84,77,76,73,72,75,74,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,23,22,19,18,21,20,17,16,13,12,15,14,5,4,3,2,1,0,11,10,9,8,7,6 },
            { 118,119,116,117,114,115,112,113,110,111,108,109,106,107,104,105,102,103,100,101,98,99,96,97,94,95,92,93,90,91,88,89,86,87,84,85,82,83,80,81,78,79,76,77,74,75,72,73,70,71,68,69,66,67,64,65,62,63,60,61,58,59,56,57,54,55,52,53,50,51,48,49,46,47,44,45,42,43,40,41,38,39,36,37,34,35,32,33,30,31,28,29,26,27,24,25,22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1 },
            { 119,118,115,114,117,116,113,112,109,108,111,110,101,100,99,98,97,96,107,106,105,104,103,102,95,94,91,90,93,92,89,88,85,84,87,86,77,76,75,74,73,72,83,82,81,80,79,78,71,70,67,66,69,68,65,64,61,60,63,62,53,52,51,50,49,48,59,58,57,56,55,54,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24 }
        };

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * `S5[swapTable[x][y]]`.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[5][5] = {
            // Generated using Regina 6.0.
            { 0, 25, 55, 81, 105 },
            { 25, 0, 7, 15, 21 },
            { 55, 7, 0, 3, 5 },
            { 81, 15, 3, 0, 1 },
            { 105, 21, 5, 1, 0 }
        };

        /**
         * Contains the orders of the permutations in the array \a S5.
         */
        static constexpr int orderTable[120] = {
            1, 2, 3, 2, 3, 2, 2, 2, 3, 4, 3, 4, 3, 4, 3, 2, 2, 4, 3, 4,
            3, 2, 2, 4, 2, 2, 2, 6, 2, 6, 3, 6, 5, 4, 5, 4, 5, 4, 3, 4,
            5, 6, 5, 4, 3, 4, 5, 6, 3, 6, 5, 4, 5, 4, 2, 2, 3, 4, 3, 4,
            2, 6, 5, 4, 5, 6, 2, 6, 5, 4, 5, 6, 5, 4, 3, 4, 5, 6, 3, 4,
            3, 2, 2, 4, 5, 4, 2, 6, 5, 6, 5, 6, 5, 6, 2, 4, 5, 4, 3, 4,
            5, 6, 3, 4, 3, 2, 2, 4, 5, 4, 2, 6, 5, 6, 5, 6, 5, 6, 2, 4
        };

        /**
         * Contains the S5 indices of the elements of S4, where the
         * element 4 maps to itself.
         */
        static constexpr Code2 S4Table[24] = {
            0, 3, 8, 7, 12, 15, 26, 25, 30, 33, 38, 37,
            48, 51, 56, 55, 60, 63, 74, 73, 78, 81, 86, 85
        };

        /**
         * Contains the S5 indices of the elements of S3, where the
         * elements 3 and 4 map to themselves.
         */
        static constexpr Code2 S3Table[6] = { 0, 7, 30, 25, 48, 55 };

    protected:
        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * \param code the second-generation code from which the new
         * permutation will be created.
         */
        constexpr Perm(Code2 code);

    private:
        /**
         * Returns the index into the Perm<5>::S5 array of the permutation that
         * maps (0,...,5) to (<i>a</i>,...,<i>e</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>} = {0,1,2,3,4}.
         *
         * \param a the desired image of 0.
         * \param b the desired image of 1.
         * \param c the desired image of 2.
         * \param d the desired image of 3.
         * \param e the desired image of 4.
         * \return the index \a i for which the given permutation is equal to
         * Perm<5>::S5[i].  This will be between 0 and 119 inclusive.
         */
        static constexpr int S5Index(int a, int b, int c, int d, int e);

        /**
         * Converts between an index into Perm<5>::S5 and an index into
         * Perm<5>::orderedS5.  This conversion works in either direction.
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
         * _finish_ at \a limit, or else the encoding will be considered
         * invalid also; if \a noTrailingData is \c false then there is no
         * constraint on the final state of the iterator.
         *
         * \exception InvalidInput The given iterator does not point to
         * a tight encoding of a 5-element permutation.
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

// Inline functions for Perm<5>

template <typename Int>
inline constexpr Int Perm<5>::convOrderedUnordered(Int index) {
    // S5 is almost the same as orderedS5, except that some pairs
    // S5[2i] <--> S5[2i+1] have been swapped to ensure that all
    // permutations S5[2i] are even and all permutations S5[2i+1] are odd.
    //
    // Specifically, we must flip between 2i <--> 2i+1 if and only if
    // one but not both of (S5Index / 2) and (S5Index / 24) is even.
    // Here we use (index >> 1), which is equivalent to (index / 2).
    //
    return ((((index >> 1) ^ (index / 24)) & 1) ? (index ^ 1) : index);
}

inline constexpr Perm<5> Perm<5>::S5Lookup::operator[] (int index) const {
    return Perm<5>(static_cast<Code2>(index));
}

inline constexpr Perm<5> Perm<5>::OrderedS5Lookup::operator[] (int index)
        const {
    return Perm<5>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline constexpr Perm<5> Perm<5>::S4Lookup::operator[] (int index) const {
    return Perm<5>(S4Table[index]);
}

inline constexpr Perm<5> Perm<5>::OrderedS4Lookup::operator[] (int index)
        const {
    return Perm<5>(S4Table[Perm<4>::orderedS4[index].S4Index()]);
}

inline constexpr Perm<5> Perm<5>::S3Lookup::operator[] (int index) const {
    return Perm<5>(S3Table[index]);
}

inline constexpr Perm<5> Perm<5>::OrderedS3Lookup::operator[] (int index)
        const {
    return Perm<5>(S3Table[Perm<3>::orderedS3[index].S3Index()]);
}

inline constexpr Perm<5> Perm<5>::S2Lookup::operator[] (int index) const {
    return Perm<5>(index == 0 ? 0 : 25);
}

inline constexpr void Perm<5>::precompute() {
}

inline constexpr Perm<5>::Perm() : code2_(0) {
}

inline constexpr Perm<5>::Perm(Code2 code) : code2_(code) {
}

inline constexpr Perm<5>::Perm(int a, int b) : code2_(swapTable[a][b]) {
}

inline constexpr Perm<5>::Perm(int a, int b, int c, int d, int e) :
        code2_(static_cast<Code2>(S5Index(a, b, c, d, e))) {
}

inline constexpr Perm<5>::Perm(const std::array<int, 5>& image) :
        code2_(static_cast<Code2>(S5Index(
            image[0], image[1], image[2], image[3], image[4]))) {
}

inline constexpr Perm<5>::Perm(int a0, int a1, int b0, int b1,
        int c0, int c1, int d0, int d1, int e0, int e1) : code2_(0) {
    int image[5];
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    image[e0] = e1;
    code2_ = static_cast<Code2>(S5Index(
        image[0], image[1], image[2], image[3], image[4]));
}

inline constexpr Perm<5>::Code1 Perm<5>::permCode1() const {
    return static_cast<Code1>(
        imageTable[code2_][0] |
        (imageTable[code2_][1] << 3) |
        (imageTable[code2_][2] << 6) |
        (imageTable[code2_][3] << 9) |
        (imageTable[code2_][4] << 12));
}

inline constexpr Perm<5>::Code2 Perm<5>::permCode2() const {
    return code2_;
}

inline void Perm<5>::setPermCode1(Code1 code) {
    code2_ = static_cast<Code2>(S5Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07));
}

inline void Perm<5>::setPermCode2(Code2 code) {
    code2_ = code;
}

inline constexpr Perm<5> Perm<5>::fromPermCode1(Code1 code) {
    return Perm<5>(static_cast<Code2>(S5Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07)));
}

inline constexpr Perm<5> Perm<5>::fromPermCode2(Code2 code) {
    return Perm<5>(code);
}

inline constexpr bool Perm<5>::isPermCode1(Code1 code) {
    unsigned mask = 0;
    for (int i = 0; i < 5; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 31 && (code >> 15) == 0);
}

inline constexpr bool Perm<5>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 120);
}

inline constexpr Perm<5>::ImagePack Perm<5>::imagePack() const {
    return permCode1();
}

inline constexpr Perm<5> Perm<5>::fromImagePack(ImagePack pack) {
    return fromPermCode1(pack);
}

inline constexpr bool Perm<5>::isImagePack(ImagePack pack) {
    return isPermCode1(pack);
}

inline constexpr Perm<5> Perm<5>::operator *(const Perm<5>& q) const {
    return Perm<5>(productTable[code2_][q.code2_]);
}

inline Perm<5> Perm<5>::cachedComp(const Perm<5>& q) const {
    return Perm<5>(productTable[code2_][q.code2_]);
}

inline Perm<5> Perm<5>::cachedComp(const Perm<5>& q, const Perm<5>& r) const {
    return Perm<5>(productTable[code2_][productTable[q.code2_][r.code2_]]);
}

inline constexpr Perm<5> Perm<5>::conjugate(const Perm<5>& q) const {
    return Perm<5>(productTable[q.code2_][productTable[code2_][
        invS5[q.code2_]]]);
}

inline Perm<5> Perm<5>::cachedConjugate(const Perm<5>& q) const {
    return Perm<5>(productTable[q.code2_][productTable[code2_][
        invS5[q.code2_]]]);
}

inline constexpr Perm<5> Perm<5>::inverse() const {
    return Perm<5>(invS5[code2_]);
}

inline Perm<5> Perm<5>::cachedInverse() const {
    return Perm<5>(invS5[code2_]);
}

inline constexpr Perm<5> Perm<5>::pow(long exp) const {
    // Maximum order is 6, from cycles: (..)(...)
    // Normalise exp to be in the range ( -order/2, +order/2 ].
    int ord = order();
    exp %= ord;
    if (exp < 0)
        exp += ord;
    if ((exp << 1) > ord)
        exp -= ord;

    switch (exp) {
        case 0: return Perm<5>();
        case 1: return *this;
        case -1: return inverse();
        case 2: return Perm<5>(productTable[code2_][code2_]);
        case -2: {
            Code2 inv = invS5[code2_];
            return Perm<5>(productTable[inv][inv]);
        }
        default /* 3 */:
            return Perm<5>(productTable[productTable[code2_][code2_]][code2_]);
    }
}

inline Perm<5> Perm<5>::cachedPow(long exp) const {
    return pow(exp);
}

inline constexpr int Perm<5>::order() const {
    return orderTable[code2_];
}

inline int Perm<5>::cachedOrder() const {
    return orderTable[code2_];
}

inline constexpr Perm<5> Perm<5>::reverse() const {
    // p becomes p * 43210 (which has second-generation code 118).
    return Perm<5>(productTable[code2_][118]);
}

inline constexpr int Perm<5>::sign() const {
    return (code2_ % 2 ? -1 : 1);
}

inline constexpr int Perm<5>::operator[](int source) const {
    return imageTable[code2_][source];
}

inline constexpr int Perm<5>::pre(int image) const {
    return imageTable[invS5[code2_]][image];
}

inline constexpr bool Perm<5>::operator == (const Perm<5>& other) const {
    return (code2_ == other.code2_);
}

inline constexpr int Perm<5>::compareWith(const Perm<5>& other) const {
    // Computing orderedS5Index() is very fast, now that we use S5 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS5Index();
    int o2 = other.orderedS5Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<5>::isIdentity() const {
    return (code2_ == 0);
}

inline Perm<5>& Perm<5>::operator ++() {
    if (++code2_ == 120)
        code2_ = 0;
    return *this;
}

inline constexpr Perm<5> Perm<5>::operator ++(int) {
    Perm<5> ans(code2_);
    if (++code2_ == 120)
        code2_ = 0;
    return ans;
}

inline constexpr bool Perm<5>::operator < (const Perm<5>& rhs) const {
    return code2_ < rhs.code2_;
}

inline constexpr Perm<5> Perm<5>::rot(int i) {
    switch (i) {
        case 1: return Perm<5>(static_cast<Code2>(32));
        case 2: return Perm<5>(static_cast<Code2>(64));
        case 3: return Perm<5>(static_cast<Code2>(90));
        case 4: return Perm<5>(static_cast<Code2>(96));
        default: return Perm<5>(); // Identity
    }
}

inline Perm<5> Perm<5>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <class URBG>
inline Perm<5> Perm<5>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 59);
        return S5[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 119);
        return S5[d(gen)];
    }
}
#endif // __DOXYGEN

inline void Perm<5>::tightEncode(std::ostream& out) const {
    // Since 5! is only slightly larger than 94, we use a variable-length
    // encoding.  Perm<5> is the *only* Perm<n> class that does this.
    // I apologise to anyone who needs to implement a specialised decoding
    // routine just for n=5; however, one of the aims of a tight encoding
    // is to optimise for space.  So we will go ahead and do this here.

    // Our marker for length 2 encodings is character 126 == '~'.
    if (code2_ < 93)
        out << static_cast<char>(code2_ + 33);
    else
        out << '~' << static_cast<char>(code2_ - 60);
}

inline std::string Perm<5>::tightEncoding() const {
    // Use a variable-length encoding (see above).
    if (code2_ < 93) {
        char ans[2] { static_cast<char>(code2_ + 33), 0 };
        return ans;
    } else {
        char ans[3] { '~', static_cast<char>(code2_ - 60), 0 };
        return ans;
    }
}

inline Perm<5> Perm<5>::tightDecoding(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

inline Perm<5> Perm<5>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <typename iterator>
Perm<5> Perm<5>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");

    Code2 code = (*start++) - 33;
    // code >= 0 because we are using an unsigned data type.
    if (code > 93)
        throw InvalidInput("The tight encoding is invalid");
    else if (code == 93) {
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        code = (*start++) + 60;
        if (code < 93 || code >= 120)
            throw InvalidInput("The tight encoding is invalid");
    }

    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Perm<5>(code);
}
#endif // __DOXYGEN

inline constexpr size_t Perm<5>::hash() const {
    return code2_;
}

inline constexpr Perm<5>::Index Perm<5>::S5Index() const {
    return code2_;
}

inline constexpr Perm<5>::Index Perm<5>::SnIndex() const {
    return code2_;
}

inline constexpr Perm<5>::Index Perm<5>::orderedS5Index() const {
    return convOrderedUnordered(code2_);
}

inline constexpr Perm<5>::Index Perm<5>::orderedSnIndex() const {
    return convOrderedUnordered(code2_);
}

inline constexpr int Perm<5>::S5Index(int a, int b, int c, int d, int e) {
    // First compute the ordered S5 index.
    int ans = 24 * a +
              6 * (b - (b > a ? 1 : 0)) +
              2 * ((c > d ? 1 : 0) + (c > e ? 1 : 0)) +
                  (d > e ? 1 : 0);

    // Then switch to the plain (unordered) S5 index.
    return convOrderedUnordered(ans);
}

inline constexpr bool Perm<5>::isConjugacyMinimal() const {
    return (code2_ < 3 || code2_ == 6 || code2_ == 9 ||
        code2_ == 27 || code2_ == 32);
}

} // namespace regina

#endif

