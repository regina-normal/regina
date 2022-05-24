
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

/*! \file maths/spec/perm4.h
 *  \brief Internal header for permutations of {0,1,2,3}.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this specialisation header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This specialisation header will be re-included at the beginning of
// perm-impl.h.
#include "maths/perm.h"

#ifndef __REGINA_PERM4_H
#ifndef __DOXYGEN
#define __REGINA_PERM4_H
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * Represents a permutation of {0,1,2,3}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 3-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to pass
 * by value and swap with std::swap(), with no need for any specialised move
 * operations or swap functions.  Moreover, Perm<4> in particular is extremely
 * fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<4>, the internal permutation
 * codes have changed as of Regina 4.6.1:
 *
 * - \e First-generation codes were used internally in Regina 4.6 and earlier.
 *   These are <i>image packs</i>: characters whose lowest two bits represent
 *   the image of 0, whose next lowest two bits represent the image of 1,
 *   and so on.  The routines permCode1(), setPermCode1(), fromPermCode1()
 *   and isPermCode1() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe tetrahedron gluings.
 *
 * - \e Second-generation codes are used internally in Regina 4.6.1 and above.
 *   These codes are integers between 0 and 23 inclusive, representing the
 *   index of the permutation in the array Perm<4>::S4.  The routines
 *   permCode2(), setPermCode2(), fromPermCode2() and isPermCode2()
 *   work with second-generation codes.
 *
 * It is highly recommended that, if you need to work with permutation
 * codes at all, you use second-generation codes where possible.  This
 * is because the first-generation routines incur additional overhead
 * in converting back and forth between the second-generation codes
 * (which are used internally by Perm<4>).
 *
 * To use this class, simply include the main permutation header maths/perm.h.
 *
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm4.
 *
 * \ingroup maths
 */
template <>
class Perm<4> {
    private:
        /**
         * An array-like object used to implement Perm<4>::S4.
         */
        struct S4Lookup {
            /**
             * Returns the permutation at the given index in the array S4.
             * See Perm<4>::S4 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 23 inclusive.
             * @return the corresponding permutation in S4.
             */
            constexpr Perm<4> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<4>::orderedS4.
         */
        struct OrderedS4Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS4.  See Perm<4>::orderedS4 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 23 inclusive.
             * @return the corresponding permutation in orderedS4.
             */
            constexpr Perm<4> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<4>::S3.
         */
        struct S3Lookup {
            /**
             * Returns the permutation at the given index in the array S3.
             * See Perm<4>::S3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in S3.
             */
            constexpr Perm<4> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<4>::orderedS3.
         */
        struct OrderedS3Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS3.  See Perm<4>::orderedS3 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 5 inclusive.
             * @return the corresponding permutation in orderedS3.
             */
            constexpr Perm<4> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<4>::S2.
         */
        struct S2Lookup {
            /**
             * Returns the permutation at the given index in the array S2.
             * See Perm<4>::S2 for details.
             *
             * This operation is extremely fast (and constant time).
             *
             * @param index an index between 0 and 1 inclusive.
             * @return the corresponding permutation in S2.
             */
            constexpr Perm<4> operator[] (int index) const;
        };

    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on four elements.  In other words, this is a
         * native signed integer type large enough to store (4!).
         */
        using Index = int;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_INDEX;

        /**
         * The total number of permutations on four elements.
         * This is the size of the array Sn.
         */
        static constexpr Index nPerms = 24;

        /**
         * The total number of permutations on three elements.
         * This is the size of the array Sn_1.
         */
        static constexpr Index nPerms_1 = 6;

        /**
         * Indicates the native unsigned integer type used to store a
         * single image pack.  See the class notes for more information
         * on image packs, and how they are used to build the old
         * first-generation permutation codes.
         */
        using ImagePack = uint8_t;

        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines 4 such images together, and so uses
         * 4 * \a imageBits bits in total.
         */
        static constexpr int imageBits = 2;

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
         *
         * Although types \a Code1 and \a Code2 are identical, they are
         * provided as separate type aliases to help communicate in your
         * source code which type of code is being used.
         */
        using Code1 = ImagePack;

        /**
         * Indicates the native unsigned integer type used to store a
         * second-generation permutation code.
         *
         * Although types \a Code1 and \a Code2 are identical, they are
         * provided as separate type aliases to help communicate in your
         * source code which type of code is being used.
         */
        using Code2 = uint8_t;

        /**
         * Gives array-like access to all possible permutations of
         * four elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn[i]</tt>.  The index \a i must be
         * between 0 and 23 inclusive.
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
         * This is different from Perm<4>::orderedSn, since this array \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         */
        static constexpr S4Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * four elements.
         *
         * This is a dimension-specific alias for Perm<4>::Sn; see that member
         * for further information.  In general, for every \a n there will be
         * a static member Perm<n>::Sn; however, these numerical aliases
         * Perm<2>::S2, ..., Perm<5>::S5 are only available for small \a n.
         *
         * Note that both permutation classes Perm<4> and Perm<5> have
         * an \a S4 array; these both store the same 24 permutations in the
         * same order (but of course using different data types).
         */
        static constexpr S4Lookup S4 {};

        /**
         * Gives array-like access to all possible permutations of four
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedSn[i]</tt>.  The index \a i
         * must be between 0 and 23 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * ordered pair (\a p[0], ..., \a p[3]).
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * This is different from Perm<4>::Sn, since this array \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         */
        static constexpr OrderedS4Lookup orderedSn {};

        /**
         * Gives array-like access to all possible permutations of four
         * elements in lexicographical order.
         *
         * This is a dimension-specific alias for Perm<4>::orderedSn; see that
         * member for further information.  In general, for every \a n there
         * will be a static member Perm<n>::orderedSn; however, these numerical
         * aliases Perm<2>::orderedS2, ..., Perm<5>::orderedS5 are only
         * available for small \a n.
         */
        static constexpr OrderedS4Lookup orderedS4 {};

        /**
         * Gives array-like access to all possible permutations of
         * three elements.  In each permutation, 3 maps to 3.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>Sn_1[i]</tt>.  The index \a i must be
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
         * This is different from Perm<4>::orderedS3, since this array
         * \a Sn_1 (or equivalently, \a S3) alternates between even and odd
         * permutations, whereas \a orderedS3 stores permutations in
         * lexicographical order.
         */
        static constexpr S3Lookup Sn_1 {};

        /**
         * Gives array-like access to all possible permutations of
         * three elements.
         *
         * This is a dimension-specific alias for Perm<4>::Sn_1; see that
         * member for further information.
         *
         * Note that the small permutation classes Perm<3>, Perm<4> and Perm<5>
         * all have an \a S3 array; these all store the same six permutations
         * in the same order (but of course using different data types).
         */
        static constexpr S3Lookup S3 {};

        /**
         * Gives array-like access to all possible permutations of three
         * elements in lexicographical order.  In each permutation, 3 maps to 3.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedS3[i]</tt>.  The index \a i
         * must be between 0 and 5 inclusive.
         *
         * In Regina 6.0.1 and earlier, this was a hard-coded C-style array;
         * since Regina 7.0 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * This is different from Perm<4>::S3, since this array \a orderedS3
         * stores permutations in lexicographical order, whereas \a S3 (or
         * equivalently, \a Sn_1) alternates between even and odd permutations.
         *
         * Note that the small permutation classes Perm<3>, Perm<4> and Perm<5>
         * all have an \a orderedS3 array; these all store the same six
         * permutations in the same order (but of course using different data
         * types).
         */
        static constexpr OrderedS3Lookup orderedS3 {};

        /**
         * Gives array-like access to all possible permutations of
         * two elements.  In each permutation, 2 maps to 2 and 3 maps to 3.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>S2[i]</tt>.  The index \a i must be
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
         *
         * Note that all small permutation classes (Perm<2>, ..., Perm<5>)
         * have an \a S2 array: these all store the same two permutations in
         * the same order (but of course using different data types).
         *
         * There is no corresponding \a orderedS2 array, since the
         * (trivial) arrays \a S2 and \a orderedS2 are identical.
         */
        static constexpr S2Lookup S2 {};

    protected:
        Code2 code_;
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
         * \pre \a a and \a b are in {0,1,2,3}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping (0,1,2,3) to
         * (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         */
        constexpr Perm(int a, int b, int c, int d);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3.
         *
         * \pre The elements of \a image are 0, 1, 2 and 3 in some order.
         *
         * @param image the array of images.
         */
        constexpr Perm(const std::array<int, 4>& image);

        /**
         * Creates a permutation mapping
         * (<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>) to
         * (<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>) respectively.
         *
         * \pre {<i>a0</i>,<i>b0</i>,<i>c0</i>,<i>d0</i>} =
         * {<i>a1</i>,<i>b1</i>,<i>c1</i>,<i>d1</i>} =
         * {0,1,2,3}.
         *
         * @param a0 the desired preimage of <i>a1</i>.
         * @param b0 the desired preimage of <i>b1</i>.
         * @param c0 the desired preimage of <i>c1</i>.
         * @param d0 the desired preimage of <i>d1</i>.
         * @param a1 the desired image of <i>a0</i>.
         * @param b1 the desired image of <i>b0</i>.
         * @param c1 the desired image of <i>c0</i>.
         * @param d1 the desired image of <i>d0</i>.
         */
        constexpr Perm(int a0, int a1, int b0, int b1, int c0, int c1,
            int d0, int d1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<4>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
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
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
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
         * Perm<4> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<4> fromPermCode1(Code1 code);

        /**
         * Creates a permutation from the given second-generation
         * permutation code.
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<4> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static constexpr Perm<4> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode1() or fromPermCode1(), and are returned by permCode1().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
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
         * For Perm<4>, this routine is identical to permCode1().
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
         * For Perm<4>, this routine is identical to fromPermCode1().
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
         * some 4-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build the old first-generation permutation codes.
         *
         * For Perm<4>, this routine is identical to isPermCode1().
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
        Perm<4>& operator = (const Perm<4>& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        constexpr Perm<4> operator *(const Perm<4>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        constexpr Perm<4> inverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in constant time.
         *
         * @param exp the exponent; this may be positive, zero or negative.
         * @return this permutation raised to the power of \a exp.
         */
        constexpr Perm<4> pow(long exp) const;

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
         * Here \e reverse means that we reverse the images of 0,...,3.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[3 - i]</tt> for all \a i.
         */
        constexpr Perm<4> reverse() const;

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
         * should be between 0 and 3 inclusive.
         * @return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 3 inclusive.
         * @return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2 and 3.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm<4>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2 or 3.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm<4>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3) under this
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
        constexpr int compareWith(const Perm<4>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2 and 3 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<4>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<4>::Sn, which is the identity.
         *
         * \ifacespython Not present, although the postincrement operator is
         * present in python as the member function inc().
         *
         * @return a reference to this permutation after the increment.
         */
        Perm<4>& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<4>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<4>::Sn, which is the identity.
         *
         * \ifacespython This routine is named inc() since python does
         * not support the increment operator.
         *
         * @return a copy of this permutation before the increment took place.
         */
        constexpr Perm<4> operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<4>::Sn.
         *
         * Note that this is \e not the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators,
         * and this order is also faster to compute than compareWith().
         *
         * @param rhs the permutation to compare this against.
         * @return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm<4>& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps <i>k</i> to <i>k</i>&nbsp;+&nbsp;<i>i</i> (mod 4)
         * for all \a k.
         *
         * @param i the image of 0; this must be between 0 and 3 inclusive.
         * @return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns a random permutation on four elements.
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
         * Returns a random permutation on four elements, using the
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
         * The representation will consist of four adjacent digits
         * representing the images of 0, 1, 2 and 3 respectively.  An
         * example of a string representation is <tt>1302</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and 4 inclusive.
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
         * the images of 0, 1 and 2 included.  The resulting string will
         * therefore have length three.
         *
         * @return a truncated string representation of this permutation.
         */
        std::string trunc3() const;

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
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,3,
         * this routine will ensure that <tt>image[i] == i</tt>.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,3 are exactly
         * <i>from</i>,...,3, but possibly in a different order.
         *
         * @param from the first integer whose image should be reset.
         * This must be between 0 and 4 inclusive.
         */
        void clear(unsigned from);

        /**
         * Returns the index of this permutation in the Perm<4>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::Sn[i].  This will be between 0 and 23 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<4>::S4 array.
         *
         * This is a dimension-specific alias for SnIndex().  In general,
         * for every \a n there will be a member function Perm<n>::SnIndex();
         * however, these numerical aliases Perm<2>::S2Index(), ...,
         * Perm<5>::S5Index() are only available for small \a n.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::S4[i].  This will be between 0 and 23 inclusive.
         */
        constexpr Index S4Index() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<4>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * @return the lexicographical index of this permutation.
         * This will be between 0 and 23 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<4>::orderedSn array.
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
         * This will be between 0 and 23 inclusive.
         */
        constexpr Index orderedS4Index() const;

        /**
         * Extends a <i>k</i>-element permutation to a 4-element permutation,
         * where 2 &le; \a k &lt; 4.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,3 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be 2 or 3.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation expressed as a permutation on
         * four elements.
         */
        template <int k>
        static constexpr Perm<4> extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an 4-element
         * permutation, where \a k &gt; 4.
         *
         * The resulting permutation will map 0,...,3 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[4],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,3 to 0,...,3 in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than 4.
         *
         * @param p a permutation on \a k elements.
         * @return the same permutation restricted to a permutation on
         * 4 elements.
         */
        template <int k>
        static constexpr Perm<4> contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<4>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
         *
         * This routine is extremely fast for Perm<4>, since it essentially
         * uses a hard-coded lookup table.
         *
         * @return \c true if and only if this permutation is minimal in its
         * conjugacy class.
         */
        constexpr bool isConjugacyMinimal() const;

        /**
         * Returns the induced permutation on all six \e pairs of
         * elements from 0,1,2,3.
         *
         * Specifically: suppose we number the six pairs in
         * lexicographical order, so that the pairs 01, 02, 03, 12, 13, 23
         * are indexed as 0, 1, 2, 3, 4, 5 respectively.  Then this
         * permutation induces a corresponding permutation of 0,...,5, where
         * the pair (\a a, \a b) maps to the pair (<i>p[a]</i>, <i>p[b]</i>).
         * This routine returns that induced permutation.
         *
         * Note that, if \a p permutes the four vertices (or equivalently,
         * the four faces) of a tetrahedron, then <tt>p.pairs()</tt> is
         * the induced permutation on the six \e edges of the tetrahedron.
         *
         * @return the induced permutation on six pairs of elements.
         */
        constexpr Perm<6> pairs() const;

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S4[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static constexpr int imageTable[24][4] = {
            { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 2, 3, 1 }, { 0, 2, 1, 3 },
            { 0, 3, 1, 2 }, { 0, 3, 2, 1 }, { 1, 0, 3, 2 }, { 1, 0, 2, 3 },
            { 1, 2, 0, 3 }, { 1, 2, 3, 0 }, { 1, 3, 2, 0 }, { 1, 3, 0, 2 },
            { 2, 0, 1, 3 }, { 2, 0, 3, 1 }, { 2, 1, 3, 0 }, { 2, 1, 0, 3 },
            { 2, 3, 0, 1 }, { 2, 3, 1, 0 }, { 3, 0, 2, 1 }, { 3, 0, 1, 2 },
            { 3, 1, 0, 2 }, { 3, 1, 2, 0 }, { 3, 2, 1, 0 }, { 3, 2, 0, 1 }
        };

        /**
         * Contains the inverses of the permutations in the array \a S4.
         *
         * Specifically, the inverse of permutation <tt>S4[i]</tt> is
         * the permutation <tt>S4[ invS4[i] ]</tt>.
         */
        static constexpr Code2 invS4[24] = {
            0, 1, 4, 3, 2, 5, 6, 7, 12, 19, 18, 13,
            8, 11, 20, 15, 16, 23, 10, 9, 14, 21, 22, 17
        };

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S4[x] * S4[y]</tt> is the
         * permutation <tt>S4[product[x][y]]</tt>.
         */
        static constexpr Code2 productTable[24][24] = {
            // Generated using an older version of Regina in which products
            // were computed (not simply looked up from a dictionary like here).
            { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 },
            { 1,0,5,4,3,2,7,6,11,10,9,8,19,18,21,20,23,22,13,12,15,14,17,16 },
            { 2,3,4,5,0,1,12,13,16,17,14,15,18,19,22,23,20,21,6,7,8,9,10,11 },
            { 3,2,1,0,5,4,13,12,15,14,17,16,7,6,9,8,11,10,19,18,23,22,21,20 },
            { 4,5,0,1,2,3,18,19,20,21,22,23,6,7,10,11,8,9,12,13,16,17,14,15 },
            { 5,4,3,2,1,0,19,18,23,22,21,20,13,12,17,16,15,14,7,6,11,10,9,8 },
            { 6,7,10,11,8,9,0,1,4,5,2,3,20,21,18,19,22,23,14,15,12,13,16,17 },
            { 7,6,9,8,11,10,1,0,3,2,5,4,15,14,13,12,17,16,21,20,19,18,23,22 },
            { 8,9,6,7,10,11,14,15,12,13,16,17,0,1,2,3,4,5,20,21,22,23,18,19 },
            { 9,8,11,10,7,6,15,14,17,16,13,12,21,20,23,22,19,18,1,0,3,2,5,4 },
            { 10,11,8,9,6,7,20,21,22,23,18,19,14,15,16,17,12,13,0,1,4,5,2,3 },
            { 11,10,7,6,9,8,21,20,19,18,23,22,1,0,5,4,3,2,15,14,17,16,13,12 },
            { 12,13,14,15,16,17,2,3,0,1,4,5,8,9,6,7,10,11,22,23,18,19,20,21 },
            { 13,12,17,16,15,14,3,2,5,4,1,0,23,22,19,18,21,20,9,8,7,6,11,10 },
            { 14,15,16,17,12,13,8,9,10,11,6,7,22,23,20,21,18,19,2,3,0,1,4,5 },
            { 15,14,13,12,17,16,9,8,7,6,11,10,3,2,1,0,5,4,23,22,21,20,19,18 },
            { 16,17,12,13,14,15,22,23,18,19,20,21,2,3,4,5,0,1,8,9,10,11,6,7 },
            { 17,16,15,14,13,12,23,22,21,20,19,18,9,8,11,10,7,6,3,2,5,4,1,0 },
            { 18,19,22,23,20,21,4,5,2,3,0,1,16,17,12,13,14,15,10,11,6,7,8,9 },
            { 19,18,21,20,23,22,5,4,1,0,3,2,11,10,7,6,9,8,17,16,13,12,15,14 },
            { 20,21,18,19,22,23,10,11,6,7,8,9,4,5,0,1,2,3,16,17,14,15,12,13 },
            { 21,20,23,22,19,18,11,10,9,8,7,6,17,16,15,14,13,12,5,4,1,0,3,2 },
            { 22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1 },
            { 23,22,19,18,21,20,17,16,13,12,15,14,5,4,3,2,1,0,11,10,9,8,7,6 }
        };

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S4[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[4][4] = {
            {  0, 7, 15, 21 },
            {  7, 0,  3,  5 },
            { 15, 3,  0,  1 },
            { 21, 5,  1,  0 }
        };

        /**
         * Contains the orders of the permutations in the array \a S4.
         */
        static constexpr int orderTable[24] = {
            1, 2, 3, 2, 3, 2, 2, 2, 3, 4, 3, 4,
            3, 4, 3, 2, 2, 4, 3, 4, 3, 2, 2, 4
        };

        /**
         * Contains the S4 indices of the elements of S3, where the
         * element 3 maps to itself.
         */
        static constexpr Code2 S3Table[6] = { 0, 3, 8, 7, 12, 15 };

        /**
         * Contains a full table of induced permutations on six
         * elements, as returned by pairs().  This array is indexed
         * according to S4, and its values are indices in S6.
         */
        static constexpr uint16_t pairsTable[24] = {
            0,   26,  148, 120, 242, 268,  86,  60, 372, 436, 556, 494,
            186, 230, 466, 402, 638, 682, 356, 330, 546, 592, 712, 668
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
        constexpr Perm<4>(Code2 code);

    private:
        /**
         * Returns the index into the Perm<4>::S4 array of the permutation that
         * maps (0,1,2,3) to (<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>} = {0,1,2,3}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @return the index \a i for which the given permutation is equal to
         * Perm<4>::S4[i].  This will be between 0 and 23 inclusive.
         */
        static constexpr int S4Index(int a, int b, int c, int d);

        /**
         * Converts between an index into Perm<4>::S4 and an index into
         * Perm<4>::orderedS4.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code2.
         */
        template <typename Int>
        static constexpr Int convOrderedUnordered(Int index);
};

// Inline functions for Perm<4>

template <typename Int>
inline constexpr Int Perm<4>::convOrderedUnordered(Int index) {
    // S4 is almost the same as orderedS4, except that some pairs
    // S4[2i] <--> S4[2i+1] have been swapped to ensure that all
    // permutations S4[2i] are even and all permutations S4[2i+1] are odd.
    //
    // Specifically, we must interchange all pairs 4i+2 <--> 4i+3.
    //
    return ((index & 2) ? (index ^ 1) : index);
}

inline constexpr Perm<4> Perm<4>::S4Lookup::operator[] (int index) const {
    return Perm<4>(static_cast<Code2>(index));
}

inline constexpr Perm<4> Perm<4>::OrderedS4Lookup::operator[] (int index)
        const {
    return Perm<4>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline constexpr Perm<4> Perm<4>::S3Lookup::operator[] (int index) const {
    return Perm<4>(S3Table[index]);
}

inline constexpr Perm<4> Perm<4>::OrderedS3Lookup::operator[] (int index)
        const {
    return Perm<4>(S3Table[Perm<3>::orderedS3[index].S3Index()]);
}

inline constexpr Perm<4> Perm<4>::S2Lookup::operator[] (int index) const {
    return Perm<4>(index == 0 ? 0 : 7);
}

inline constexpr Perm<4>::Perm() : code_(0) {
}

inline constexpr Perm<4>::Perm(Code2 code) : code_(code) {
}

inline constexpr Perm<4>::Perm(int a, int b) : code_(swapTable[a][b]) {
}

inline constexpr Perm<4>::Perm(int a, int b, int c, int d) :
        code_(static_cast<Code2>(S4Index(a, b, c, d))) {
}

inline constexpr Perm<4>::Perm(const std::array<int, 4>& image) :
        code_(static_cast<Code2>(S4Index(
        image[0], image[1], image[2], image[3]))) {
}

inline constexpr Perm<4>::Perm(int a0, int a1, int b0, int b1, int c0, int c1,
        int d0, int d1) : code_(0) {
    // TODO: When we move to C++20, we can get rid of the zero initialisers.
    int image[4] = { 0, 0, 0, 0 };
    image[a0] = a1;
    image[b0] = b1;
    image[c0] = c1;
    image[d0] = d1;
    code_ = static_cast<Code2>(S4Index(image[0], image[1], image[2], image[3]));
}

inline constexpr Perm<4>::Code1 Perm<4>::permCode1() const {
    return static_cast<Code1>(
        imageTable[code_][0] |
        (imageTable[code_][1] << 2) |
        (imageTable[code_][2] << 4) |
        (imageTable[code_][3] << 6));
}

inline constexpr Perm<4>::Code2 Perm<4>::permCode2() const {
    return code_;
}

inline void Perm<4>::setPermCode1(Code1 code) {
    code_ = static_cast<Code2>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03));
}

inline void Perm<4>::setPermCode2(Code2 code) {
    code_ = code;
}

inline constexpr Perm<4> Perm<4>::fromPermCode1(Code1 code) {
    return Perm<4>(static_cast<Code2>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03)));
}

inline constexpr Perm<4> Perm<4>::fromPermCode2(Code2 code) {
    return Perm<4>(code);
}

inline constexpr bool Perm<4>::isPermCode1(Code1 code) {
    // There are no "spare" bits since Code1 is uint8_t, which means there is
    // no need to check that only the first 8 bits are in use.
    unsigned mask = 0;
    for (int i = 0; i < 4; i++)
        mask |= (1 << ((code >> (2 * i)) & 3));
            // mask |= (1 << imageOf(i));
    return (mask == 15);
}

inline constexpr bool Perm<4>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 24);
}

inline constexpr Perm<4>::ImagePack Perm<4>::imagePack() const {
    return permCode1();
}

inline constexpr Perm<4> Perm<4>::fromImagePack(ImagePack pack) {
    return fromPermCode1(pack);
}

inline constexpr bool Perm<4>::isImagePack(ImagePack pack) {
    return isPermCode1(pack);
}

inline constexpr Perm<4> Perm<4>::operator *(const Perm<4>& q) const {
    return Perm<4>(productTable[code_][q.code_]);
}

inline constexpr Perm<4> Perm<4>::inverse() const {
    return Perm<4>(invS4[code_]);
}

inline constexpr Perm<4> Perm<4>::pow(long exp) const {
    // Maximum order is 4.
    switch (orderTable[code_]) {
        case 1:
            return Perm<4>();
        case 2:
            return (exp & 1 ? *this : Perm<4>());
        case 3:
            switch (exp % 3) {
                case 0:          return Perm<4>();
                case 1: case -2: return *this;
                default:         return inverse();
            }
        default:
            switch (exp % 4) {
                case 0:          return Perm<4>();
                case 1: case -3: return *this;
                case 3: case -1: return inverse();
                default:         return Perm<4>(productTable[code_][code_]);
            }
    }
}

inline constexpr int Perm<4>::order() const {
    return orderTable[code_];
}

inline constexpr Perm<4> Perm<4>::reverse() const {
    // p becomes p * 3210 (which has second-generation code 22).
    return Perm<4>(productTable[code_][22]);
}

inline constexpr int Perm<4>::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline constexpr int Perm<4>::operator[](int source) const {
    return imageTable[code_][source];
}

inline constexpr int Perm<4>::pre(int image) const {
    return imageTable[invS4[code_]][image];
}

inline constexpr int Perm<4>::compareWith(const Perm<4>& other) const {
    // Computing orderedS4Index() is very fast, now that we use S4 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS4Index();
    int o2 = other.orderedS4Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline constexpr bool Perm<4>::isIdentity() const {
    return (code_ == 0);
}

inline Perm<4>& Perm<4>::operator ++() {
    if (++code_ == 24)
        code_ = 0;
    return *this;
}

inline constexpr Perm<4> Perm<4>::operator ++(int) {
    Perm<4> ans(code_);
    if (++code_ == 24)
        code_ = 0;
    return ans;
}

inline constexpr bool Perm<4>::operator < (const Perm<4>& rhs) const {
    return code_ < rhs.code_;
}

inline constexpr Perm<4> Perm<4>::rot(int i) {
    switch (i) {
        case 1: return Perm<4>(static_cast<Code2>(9));
        case 2: return Perm<4>(static_cast<Code2>(16));
        case 3: return Perm<4>(static_cast<Code2>(19));
        default: return Perm<4>(); // Identity
    }
}

inline Perm<4> Perm<4>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

template <class URBG>
inline Perm<4> Perm<4>::rand(URBG&& gen, bool even) {
    if (even) {
        std::uniform_int_distribution<short> d(0, 11);
        return S4[2 * d(gen)];
    } else {
        std::uniform_int_distribution<short> d(0, 23);
        return S4[d(gen)];
    }
}

inline constexpr bool Perm<4>::operator == (const Perm<4>& other) const {
    return (code_ == other.code_);
}

inline constexpr bool Perm<4>::operator != (const Perm<4>& other) const {
    return (code_ != other.code_);
}

inline void Perm<4>::tightEncode(std::ostream& out) const {
    out << static_cast<char>(code_ + 33);
}

inline std::string Perm<4>::tightEncoding() const {
    char ans[2] { static_cast<char>(code_ + 33), 0 };
    return ans;
}

inline constexpr int Perm<4>::S4Index(int a, int b, int c, int d) {
    // First compute the ordered S4 index.
    int ans = 6 * a +
              2 * (b - (b > a ? 1 : 0)) +
                  (c > d ? 1 : 0);

    // Then switch to the plain (unordered) S4 index.
    return convOrderedUnordered(ans);
}

inline constexpr Perm<4>::Index Perm<4>::S4Index() const {
    return code_;
}

inline constexpr Perm<4>::Index Perm<4>::SnIndex() const {
    return code_;
}

inline constexpr Perm<4>::Index Perm<4>::orderedS4Index() const {
    return convOrderedUnordered(code_);
}

inline constexpr Perm<4>::Index Perm<4>::orderedSnIndex() const {
    return convOrderedUnordered(code_);
}

inline constexpr bool Perm<4>::isConjugacyMinimal() const {
    return (code_ < 3 || code_ == 6 || code_ == 9);
}

inline constexpr Perm<6> Perm<4>::pairs() const {
    return Perm<6>::fromPermCode2(pairsTable[code_]);
}

} // namespace regina

#endif

