
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file maths/perm.h
 *  \brief Deals with permutations of {0,1,...,<i>n</i>-1}.
 *
 *  This is the only header that you need to include for Regina's permutation
 *  classes.  It will automatically bring in all implementation details and
 *  specialisations.
 */

#ifndef __REGINA_PERM_H
#ifndef __DOXYGEN
#define __REGINA_PERM_H
#endif

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include "regina-core.h"
#include "utilities/bitmanip.h"
#include "utilities/exception.h"
#include "utilities/intutils.h"
#include "utilities/randutils.h"

namespace regina {

/**
 * Returns the character used to express the integer \a i in a permutation.
 *
 * - For \a i = 0,...,9, this will be the usual digit representing \a i.
 *
 * - For \a i ≥ 10, this will be a lower-case letter.  In particular,
 *   for \a i = 10,...,15, this will be the usual hexadecimal digit
 *   representing \a i.
 *
 * - At present, this routine only supports integers \a i < 36.
 *
 * \param i the integer to represent; this must be between 0 and 35 inclusive.
 * \return the single character used to represent \a i.
 *
 * \ingroup maths
 */
inline constexpr char digit(int i) {
    return (i < 10 ? '0' + static_cast<char>(i) :
        'a' + static_cast<char>(i) - 10);
}

/**
 * Returns the factorial of \a n.
 *
 * \param n any non-negative integer; this must be at most 20
 * (since otherwise the factorial will overflow).
 * \return the factorial of \a n..
 *
 * \ingroup maths
 */
inline constexpr int64_t factorial(int n) {
    return (n <= 1 ? 1 : factorial(n - 1) * n);
}

/**
 * Represents the different kinds of internal permutation codes that are
 * used in Regina's various Perm<n> template classes.  See the Perm<n>
 * class notes for more information on exactly how these codes are constructed.
 * The class constant Perm<n>::codeType indicates which type of code is used
 * for which \a n.
 *
 * \ingroup maths
 */
enum PermCodeType {
    /**
     * This is a permutation code that packs the images of 0,...,<i>n</i>-1
     * into a single native integer using a handful of bits per image.
     * Such codes are easier to manipulate on an element-by-element basis.
     *
     * Codes of this type can always be queried using Perm<n>::permCode(), and
     * permutations can be recreated from them using Perm<n>::fromPermCode().
     */
    PERM_CODE_IMAGES = 1,
    /**
     * This is a permutation code that stores the index into the full
     * permutation group \a S_n.  Such codes typically require fewer bytes and
     * are packed together, making them ideal for working with lookup tables.
     *
     * Codes of this type can be queried using Perm<n>::SnIndex(), and
     * permutations can be recreated from them by indexing into Perm<n>::Sn.
     *
     * \warning The routines Perm<n>::permCode() and Perm<n>::fromPermCode()
     * will still be present, but in some classes (e.g., Perm<4> and Perm<5>),
     * these are legacy routines that refer to different types of codes.
     */
    PERM_CODE_INDEX = 2
};

namespace detail {
    /**
     * Stores the total number of conjugacy classes of permutations on
     * \a n elements, for all \a n ≤ 16.
     *
     * See OEIS, sequence A000041.
     *
     * This hard-coded array is an implementation detail; these numbers
     * should be accessed by end users as PermClass<n>::count.
     */
    constexpr int countPermClasses[17] = {
        1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77, 101, 135, 176, 231
    };

    /**
     * Stores the S_n indices of all minimal representatives of conjugacy
     * classes of permutations on \a n elements, for all \a n ≤ 16.
     *
     * Specifically, for each fixed \a n, the permutations obtained via
     * `Perm<n>::Sn[permClassRep[i]]` for 0 ≤ \a i < `countPermClasses[i]`
     * will be precisely the same permutations, and seen in the same order,
     * as you would obtain by beginning with a default-constructed
     * PermClass<n> \a c, repeatedly incrementing \a c until it is
     * past-the-end, and computing `c.rep()` at each stage.
     */
    constexpr int64_t permClassRep[countPermClasses[16]] = {
        0, 1, 2, 6, 9, 27, 32, 127, 128, 146, 153, 746, 753, 849, 872, 5166,
        5169, 5187, 5192, 5792, 5888, 5913, 41067, 41072, 41168, 41193, 45506,
        45513, 46113, 46232, 368047, 368048, 368066, 368073, 368673, 368769,
        368792, 404049, 404072, 408392, 408992, 409113, 3669866, 3669873,
        3669969, 3669992, 3674307, 3674312, 3674912, 3675033, 3997472, 3997568,
        3997593, 4032873, 4037193, 4037912, 40284846, 40284849, 40284867,
        40284872, 40285472, 40285568, 40285593, 40320848, 40320873, 40325193,
        40325793, 40325912, 43591106, 43591113, 43591713, 43591832, 43914369,
        43914392, 43949672, 43953992, 43954713, 482671467, 482671472, 482671568,
        482671593, 482675906, 482675913, 482676513, 482676632, 482999073,
        482999169, 482999192, 483034472, 483038792, 483039513, 519322449,
        519322472, 519326792, 519327392, 519327513, 522593312, 522593433,
        522915993, 522951273, 522956312, 6267305647, 6267305648, 6267305666,
        6267305673, 6267306273, 6267306369, 6267306392, 6267341649, 6267341672,
        6267345992, 6267346592, 6267346713, 6270611907, 6270611912, 6270612512,
        6270612633, 6270935168, 6270935193, 6270970473, 6270974793, 6270975512,
        6710019872, 6710019968, 6710019993, 6710055273, 6710059593, 6710060312,
        6746347593, 6746348193, 6746348312, 6749614232, 6749936792, 6749972072,
        6749977113, 87660962666, 87660962673, 87660962769, 87660962792,
        87660967107, 87660967112, 87660967712, 87660967833, 87661290272,
        87661290368, 87661290393, 87661325673, 87661329993, 87661330712,
        87697613648, 87697613673, 87697617993, 87697618593, 87697618712,
        87700884513, 87700884632, 87701207192, 87701242472, 87701247513,
        93448903106, 93448903113, 93448903713, 93448903832, 93449226369,
        93449226392, 93449261672, 93449265992, 93449266713, 93888346472,
        93888350792, 93888351513, 93924639392, 93924639513, 93927905433,
        93928227993, 93928268312, 1313941673646, 1313941673649, 1313941673667,
        1313941673672, 1313941674272, 1313941674368, 1313941674393,
        1313941709648, 1313941709673, 1313941713993, 1313941714593,
        1313941714712, 1313944979906, 1313944979913, 1313944980513,
        1313944980632, 1313945303169, 1313945303192, 1313945338472,
        1313945342792, 1313945343513, 1314384387873, 1314384387969,
        1314384387992, 1314384423272, 1314384427592, 1314384428313,
        1314420715592, 1314420716192, 1314420716313, 1314423982233,
        1314424304793, 1314424340073, 1314424345112, 1395371981649,
        1395371981672, 1395371985992, 1395371986592, 1395371986713,
        1395375252512, 1395375252633, 1395375575193, 1395375610473,
        1395375615512, 1401123594368, 1401123594393, 1401123629673,
        1401123633993, 1401123634712, 1401562718793, 1401562719512,
        1401599007512, 1401602273432, 1401602595992, 1401602636313
    };
}

/**
 * Represents a permutation of {0,1,...,<i>n</i>-1}.
 * Amongst other things, such permutations are used to describe
 * simplex gluings in (<i>n</i>-1)-manifold triangulations.
 *
 * Perm objects are small enough to pass by value and swap with std::swap(),
 * with no need to use references, specialised move operations or custom
 * swap functions.  The trade-off is that, for this to be possible, the
 * Perm template class can only work with \a n ≤ 16.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the entire permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  These codes are constructed as follows:
 *
 * - For 8 ≤ \a n ≤ 16, the code is an _image pack_: essentially a
 *   packed array that holds the images of 0,...,<i>n</i>-1 in a single native
 *   integer type.  More precisely, this is an unsigned integer of type
 *   \a ImagePack, whose lowest \a imageBits bits represent the image of 0,
 *   whose next lowest \a imageBits bits represent the image of 1, and so on.
 *   This scheme is consistent with the old first-generation codes for
 *   \a n = 4,...,7, which are still supported but no longer used internally.
 *
 * - For \a n ≤ 7, the code is an index into a hard-coded list of
 *   all possible permutations; more precisely, an index into the symmetric
 *   group Perm<n>::Sn.  The ordering of Perm<n>::Sn is "almost lexicographic",
 *   in that we swap some pairs of indices (2<i>k</i>, 2<i>k</i>+1) to ensure
 *   that the even permutations are precisely those with even indices.
 *
 * For \a n = 2,...,5 (which appear throughout 2-, 3- and 4-manifold
 * triangulations), this template is specialised: the code is highly optimised
 * and also offers some extra functionality.  For \a n = 6,7, this template
 * is again specialised and highly optimised, and it offers some extra
 * functionality but not as much as Perm<5> and below.  For \a n ≥ 8,
 * this template is generic and most operations require more time (in
 * particular, there are no harded-coded lookup tables).
 *
 * \python Python does not support templates.  For each
 * \a n = 2,...,16, this class is available in Python under the
 * corresponding name Perm2, Perm3, ..., Perm16.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n>
class Perm {
    static_assert(n >= 8 && n <= 16,
        "The generic Perm<n> template is only available for 8 <= n <= 16.");

    public:
        /**
         * Indicates the number of bits used in an image pack to store the
         * image of a single integer.
         *
         * A full image pack combines \a n such images together, and so uses
         * \a n * \a imageBits bits in total.
         */
        static constexpr int imageBits = regina::bitsRequired(n);

        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on \a n elements.  In other words, this is a
         * native signed integer type large enough to store (<i>n</i>!).
         *
         * \note This type is not hyper-optimised: for very small \a n where
         * Index is hard-coded this is just an \c int, and for larger \a n
         * where Index is derived it is actually large enough to hold an
         * entire image pack.  If at any stage there are plans to optimise this
         * type, be careful of the special case of \a n = 8, where (8!) can be
         * stored in an \e unsigned 16-bit type but not a signed 16-bit type.
         */
        using Index = typename IntOfMinSize<(imageBits * n + 7) / 8>::type;

        /**
         * Indicates the native unsigned integer type used to store a
         * single image pack.  See the class notes for more information
         * on image packs, and how they are used as permutation codes
         * for \a n ≥ 8.
         */
        using ImagePack = typename IntOfMinSize<(imageBits * n + 7) / 8>::utype;

        /**
         * A bitmask whose lowest \a imageBits bits are 1, and whose
         * remaining higher order bits are all 0.  This may be useful when
         * creating or analysing image packs.
         */
        static constexpr ImagePack imageMask =
            (static_cast<ImagePack>(1) << Perm<n>::imageBits) - 1;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         *
         * This type alias is present for most values of \a n, though its
         * precise size depends on how the permutation code is constructed.
         * However, for \a n = 4,...,7 it is replaced by two type aliases
         * \a Code1 and \a Code2, which describe the older first-generation
         * and newer second-generation permutation codes respectively.
         */
        using Code = ImagePack;

        /**
         * Indicates what type of internal permutation code is used by
         * this instance of the Perm class template.
         */
        static constexpr PermCodeType codeType = PERM_CODE_IMAGES;

        /**
         * The total number of permutations on \a n elements.
         * This is the size of the symmetric group <i>S<sub>n</sub></i>.
         */
        static constexpr Index nPerms = factorial(n);

        /**
         * The total number of permutations on <i>n</i>-1 elements.  This is
         * the size of the symmetric group <i>S</i><sub><i>n</i>-1</sub>.
         */
        static constexpr Index nPerms_1 = factorial(n-1);

    private:
        /**
         * A bitmask that can be applied to an image pack to isolate the
         * images of [0..n/2), where n/2 is rounded up for odd \a n.
         */
        static constexpr ImagePack lowerMask =
            (static_cast<ImagePack>(1) << (((n+1)/2) * Perm<n>::imageBits)) - 1;

        /**
         * `2^k`, where \a k is the number of bits set in \a lowerMask.
         */
        static constexpr ImagePack lowerCount =
            (static_cast<ImagePack>(1) << (((n+1)/2) * Perm<n>::imageBits));

        /**
         * The number of bits consumed by \a lowerMask.  Equivalently,
         * this is the number of zero bits at the end of \a upperMask.
         */
        static constexpr int upperShift = ((n+1)/2) * Perm<n>::imageBits;

        /**
         * A bitmask that can be applied to an image pack to isolate the
         * images of [n/2..n), where n/2 is rounded up for odd \a n.
         */
        static constexpr ImagePack upperMask =
            ((static_cast<ImagePack>(1) << ((n/2) * Perm<n>::imageBits)) - 1)
                << upperShift;

        /**
         * `2^k`, where \a k is the number of bits set in \a upperMask.
         */
        static constexpr ImagePack upperCount =
            (static_cast<ImagePack>(1) << ((n/2) * Perm<n>::imageBits));

        /**
         * The number of characters used in a tight encoding.
         * This is the smallest exponent k for which 94^k ≥ n!.
         *
         * This is only relevant for the generic Perm<n> template, where
         * tight encodings have a fixed length.
         */
        static constexpr int tightChars_ = (n == 8 || n == 9 ? 3 :
             n == 10 || n == 11 ? 4 : n == 12 || n == 13 ? 5 : n == 14 ? 6 : 7);

        /**
         * A lightweight array-like object used to implement Perm<n>::Sn.
         */
        struct SnLookup {
            /**
             * Returns the permutation at the given index in the array Sn.
             * See Perm<n>::Sn for details.
             *
             * For \a n ≤ 7, this operator is very fast (and constant time).
             * However, for \a n ≥ 8 it is not constant time; the current
             * implementation is quadratic in \a n.
             *
             * \param index an index between 0 and <i>n</i>!-1 inclusive.
             * \return the corresponding permutation in Sn.
             */
            constexpr Perm<n> operator[] (Index index) const;

            /**
             * Returns the number of permutations in the array Sn.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return nPerms; }
        };

        /**
         * A lightweight array-like object used to implement Perm<n>::orderedSn.
         */
        struct OrderedSnLookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedSn.  See Perm<n>::orderedSn for details.
             *
             * For \a n ≤ 7, this operator is very fast (and constant time).
             * However, for \a n ≥ 8 it is not constant time; the current
             * implementation is quadratic in \a n.
             *
             * \param index an index between 0 and <i>n</i>!-1 inclusive.
             * \return the corresponding permutation in orderedSn.
             */
            constexpr Perm<n> operator[] (Index index) const;

            /**
             * Returns the number of permutations in the array orderedSn.
             *
             * \return the size of this array.
             */
            static constexpr Index size() { return nPerms; }
        };

    public:
        /**
         * Gives array-like access to all possible permutations of
         * \a n elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `Sn[i]`.  The index \a i must be
         * between 0 and <i>n</i>!-1 inclusive.
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * This array is different from Perm<n>::orderedSn, since \a Sn
         * alternates between even and odd permutations, whereas \a orderedSn
         * stores permutations in lexicographical order.
         *
         * This is a lightweight object, and it is defined in the headers only.
         * In particular, you cannot make a reference to it (but it is cheap
         * to make a copy).
         *
         * \warning For \a n ≤ 7, the square bracket operator is a
         * very fast constant-time routine.  However, for \a n ≥ 8,
         * this is not constant time; the current implementation is
         * quadratic in \a n.
         */
        static constexpr SnLookup Sn {};

        /**
         * Gives array-like access to all possible permutations of
         * \a n elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: `orderedSn[i]`.  The index \a i
         * must be between 0 and <i>n</i>!-1 inclusive.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * <i>n</i>-tuple (\a p[0], \a p[1], ..., \a p[<i>n</i>-1]).
         *
         * This array is different from Perm<n>::Sn, since \a orderedSn
         * stores permutations in lexicographical order, whereas \a Sn
         * alternates between even and odd permutations.
         *
         * This is a lightweight object, and it is defined in the headers only.
         * In particular, you cannot make a reference to it (but it is cheap
         * to make a copy).
         *
         * \warning For \a n ≤ 7, the square bracket operator is a
         * very fast constant-time routine.  However, for \a n ≥ 8,
         * this is not constant time; the current implementation is
         * quadratic in \a n.
         */
        static constexpr OrderedSnLookup orderedSn {};

    protected:
        Code code_;
            /**< The internal code representing this permutation. */

    private:
        /**
         * Internal helper routine.  This routine recursively computes the
         * private constant idCode_ at compile time.
         */
        static constexpr typename Perm<n>::Code idCodePartial(int k);

        static constexpr Code idCode_ = idCodePartial(n - 1);
            /**< The internal code for the identity permutation. */

        static std::mutex precomputeMutex;
            /**< A mutex to make precomputation threadsafe. */

        static ImagePack *invLower_;
            /**< A precomputed table of "half-inverse" image packs.
                 The array index should be the lower half of an image pack
                 (i.e., should encode the images of [0..n/2).
                 Array elements whose indices do not correspond to a
                 "half image pack" will be left uninitialised. */
        static ImagePack *invUpper_;
            /**< A precomputed table of "half-inverse" image packs.
                 The array index should be the upper half of an image pack
                 (i.e., should encode the images of [n/2..n).
                 Array elements whose indices do not correspond to a
                 "half image pack" will be left uninitialised. */

    public:
        /**
         * Performs the precomputation necessary for using the optimised
         * cachedInverse() routine.
         *
         * This _must_ be called before calling cachedInverse().
         *
         * This only needs to be done once in the lifetime of the program.
         * If you do try to call precompute() a second time then it will
         * do nothing and return immediately.
         *
         * TODO: Add details on how much memory the precomputed tables
         * will consume.
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
         * \pre 0 ≤ \a a,\a b < \a n.
         *
         * \param a the element to switch with \a b.
         * \param b the element to switch with \a a.
         */
        constexpr Perm(int a, int b);

        /**
         * Creates a permutation mapping \a i to \a image[\a i] for each
         * 0 ≤ \a i < \a n.
         *
         * \pre The elements of \a image are 0,...,<i>n</i>-1 in some order.
         *
         * \param image the array of images.
         */
        constexpr Perm(const std::array<int, n>& image);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * \param cloneMe the permutation to clone.
         */
        constexpr Perm(const Perm<n>& cloneMe) = default;

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
         * \return the permutation reprsented by the given internal
         * code.
         */
        static constexpr Perm fromPermCode(Code code);

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
         * Returns the image pack that represents this permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build permutation codes.
         *
         * For \a n ≥ 8, this routine is identical to permCode().
         *
         * \return the image pack for this permutation.
         */
        constexpr ImagePack imagePack() const;

        /**
         * Creates a permutation from the given image pack.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build permutation codes.
         *
         * For \a n ≥ 8, this routine is identical to fromPermCode().
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
         * some <i>n</i>-element permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build permutation codes.
         *
         * For \a n ≥ 8, this routine is identical to isPermCode().
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
        Perm& operator = (const Perm& cloneMe) = default;

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is \a p, the
         * resulting permutation will be <i>p</i>∘<i>q</i>, and will satisfy
         * `(p*q)[x] == p[q[x]]`.
         *
         * \param q the permutation to compose this with.
         * \return the composition of both permutations.
         */
        constexpr Perm operator * (const Perm& q) const;

        /**
         * An alias for the composition operator, provided to assist with
         * writing generic code.
         *
         * This generic Perm<n> class does not use precomputation to compute
         * compositions.  The only point of having cachedComp() in this generic
         * Perm<n> class is to make it easier to write generic code that works
         * with Perm<n> for any \a n.
         *
         * - If you know you are only working with the generic Perm<n>, you
         *   should just use the composition operator instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedComp().
         *
         * The permutation that is returned is the same as you would
         * obtain by calling `(*this) * q`.
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedComp().  For generic
         * Perm<n>, precompute() does not affect compositions; however, for
         * other Perm<n> classes a failure to do this will almost certainly
         * crash your program.
         *
         * \param q the permutation to compose this with.
         * \return the composition of both permutations.
         */
        constexpr Perm cachedComp(const Perm& q) const;

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
         * lifetime of this program before calling cachedComp().  For generic
         * Perm<n>, precompute() does not affect compositions; however, for
         * other Perm<n> classes a failure to do this will almost certainly
         * crash your program.
         *
         * \param q the first permutation to compose this with.
         * \param r the second permutation to compose this with.
         * \return the composition of both permutations.
         */
        [[deprecated]] constexpr Perm cachedComp(const Perm& q, const Perm& r)
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
        constexpr Perm conjugate(const Perm& q) const;

        /**
         * An alias for conjugate(), provided to assist with writing generic
         * code.
         *
         * This generic Perm<n> class does not use precomputation to compute
         * conjugates.  The only point of having cachedConjugate() in this
         * generic Perm<n> class is to make it easier to write generic code
         * that works with Perm<n> for any \a n.
         *
         * - If you know you are only working with the generic Perm<n>, you
         *   should just call conjugate() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedConjugate().
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedConjugate().  For
         * generic Perm<n>, precompute() does not affect conjugate
         * computations; however, for other Perm<n> classes a failure to do
         * this will almost certainly crash your program.
         *
         * \param q the permutation to conjugate this by.
         * \return the conjugate of this permutation by \a q.
         */
        constexpr Perm cachedConjugate(const Perm& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * For permutations of seven and fewer objects, inversion is extremely
         * fast because it uses hard-coded lookup tables.  However, for this
         * generic Perm<n> class, inversion cannot uses these lookup tables
         * (for multiple reasons), and so inverse() takes time linear in \a n.
         *
         * If you are going to make significant use of the generic Perm<n>
         * class for some particular value of \a n, you should instead:
         *
         * - call precompute() to precompute some "partial lookup tables"
         *   in advance (see precompute() for details on how much memory
         *   they will consume); and then
         *
         * - call cachedInverse() instead of inverse() to compute your
         *   inverses.
         *
         * \return the inverse of this permutation.
         */
        constexpr Perm inverse() const;

        /**
         * Finds the inverse of this permutation, optimised using precomputed
         * "partial lookup tables".
         *
         * The advantage of this routine is speed: calling cachedInverse()
         * involves two table lookups and some simple arithmetic to combine
         * the results, whereas inverse() requires time linear in \a n.
         *
         * The disadvantages of this routine are that (1) you must remember
         * to call precompute() in advance, and (2) the precomputed lookup
         * tables will consume additional memory for the lifetime of your
         * program.  See precompute() for details on just how much memory
         * these tables will consume for each \a n.
         *
         * The permutation that is returned is the same as you would
         * obtain by calling inverse().
         *
         * \pre You _must_ have called the routine precompute() at least once
         * in the lifetime of the program before using cachedInverse().
         * Otherwise this routine will almost certainly crash your program.
         *
         * \return the inverse of this permutation.
         */
        constexpr Perm cachedInverse() const;

        /**
         * Computes the given power of this permutation.
         *
         * This routine runs in time linear in \a n (in particular, the
         * running time does not depend upon the given exponent).
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        constexpr Perm pow(long exp) const;

        /**
         * An alias for pow(), provided to assist with writing generic code.
         *
         * This specialised Perm<n> class does not use precomputation to
         * compute powers.  The only point of having cachedPow() in this
         * generic Perm<n> class is to make it easier to write generic code
         * that works with Perm<n> for any \a n.
         *
         * - If you know you are only working with the generic Perm<n>, you
         *   should just call pow() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedPow().
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedPow().  For generic
         * Perm<n>, precompute() does not affect powers; however, for other
         * Perm<n> classes a failure to do this will almost certainly crash
         * your program.
         *
         * \param exp the exponent; this may be positive, zero or negative.
         * \return this permutation raised to the power of \a exp.
         */
        constexpr Perm<n> cachedPow(long exp) const;

        /**
         * Returns the order of this permutation.
         *
         * In other words; this routine returns the smallest positive
         * integer \a k for which the <i>k</i>th power of this
         * permutation is the identity.
         *
         * Note that the largest possible order for the largest supported \a n
         * (\a n = 16) is 140.  See OEIS sequence A000793 for details.
         *
         * \return the order of this permutation.
         */
        constexpr int order() const;

        /**
         * An alias for order(), provided to assist with writing generic code.
         *
         * This specialised Perm<n> class does not use precomputation to
         * compute orders.  The only point of having cachedOrder() in this
         * generic Perm<n> class is to make it easier to write generic code
         * that works with Perm<n> for any \a n.
         *
         * - If you know you are only working with the generic Perm<n>, you
         *   should just call order() instead.
         *
         * - If you are writing generic code, you _must_ remember to call
         *   precompute() at least once in the lifetime of this program
         *   before using cachedOrder().
         *
         * \pre You _must_ have called precompute() at least once in the
         * lifetime of this program before calling cachedOrder().  For generic
         * Perm<n>, precompute() does not affect order computations; however,
         * for other Perm<n> classes a failure to do this will almost certainly
         * crash your program.
         *
         * \return the order of this permutation.
         */
        constexpr int cachedOrder() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here _reverse_ means that we reverse the images of
         * 0,...,<i>n</i>-1.  In other words, if permutation \a q is the
         * reverse of \a p, then `p[i] == q[n - 1 - i]` for all \a i.
         */
        constexpr Perm reverse() const;

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
         * should be between 0 and <i>n</i>-1 inclusive.
         * \return the image of \a source.
         */
        constexpr int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * \param image the integer whose preimage we wish to find.  This
         * should be between 0 and <i>n</i>-1 inclusive.
         * \return the preimage of \a image.
         */
        constexpr int pre(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for all 0 ≤ \a i < \a n.
         *
         * \param other the permutation with which to compare this.
         * \return \c true if and only if this and the given permutation
         * are equal.
         */
        constexpr bool operator == (const Perm& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for some 0 ≤ \a i < \a n.
         *
         * \param other the permutation with which to compare this.
         * \return \c true if and only if this and the given permutation
         * differ.
         */
        constexpr bool operator != (const Perm& other) const;

        /**
         * Lexicographically compares the images of (0,1,...,\a n-1) under this
         * and the given permutation.
         *
         * \param other the permutation with which to compare this.
         * \return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal, and 1 if this permutation produces
         * a greater image.
         */
        constexpr int compareWith(const Perm& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if every integer
         * 0 ≤ \a i < \a n is mapped to itself.
         *
         * \return \c true if and only if this is the identity
         * permutation.
         */
        constexpr bool isIdentity() const;

        /**
         * A preincrement operator that changes this to be the next permutation
         * in the array Perm<n>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<n>::Sn, which is the identity.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this permutation after the increment.
         */
        Perm& operator ++();

        /**
         * A postincrement operator that changes this to be the next permutation
         * in the array Perm<n>::Sn.  If this is the last such permutation
         * then this will wrap around to become the first permutation in
         * Perm<n>::Sn, which is the identity.
         *
         * \python This routine is named inc() since python does
         * not support the increment operator.
         *
         * \return a copy of this permutation before the increment took place.
         */
        Perm operator ++(int);

        /**
         * Determines if this appears earlier than the given permutation
         * in the array Perm<n>::Sn.
         *
         * Note that this is _not_ the same ordering of permutations as
         * the ordering implied by compareWith().  This is, however,
         * consistent with the ordering implied by the ++ operators.
         *
         * Unlike the smaller permutation classes that use \a Sn indices
         * as internal permutation codes, for this generic Perm class the
         * ordering defined here is _slower_ to compute than compareWith().
         * It is recommended that, unless you specifically need to align
         * your ordering with \a Sn indices, you either (i) use compareWith()
         * for lexicographical ordering (which is a little faster), or else
         * (ii) just compare permutation codes if you are happy with an
         * arbitrary ordering (which will be _much_ faster).
         *
         * \param rhs the permutation to compare this against.
         * \return \c true if and only if this appears before \a rhs in \a Sn.
         */
        constexpr bool operator < (const Perm& rhs) const;

        /**
         * Returns the <i>i</i>th rotation.
         * This maps \a k to \a k + \a i (mod \a n) for all \a k.
         *
         * \param i the image of 0; this must be between 0 and <i>n</i>-1
         * inclusive.
         * \return the <i>i</i>th rotation.
         */
        static constexpr Perm rot(int i);

        /**
         * Returns the index of this permutation in the Perm<n>::Sn array.
         *
         * See Sn for further information on how these permutations are indexed.
         *
         * \return the index \a i for which this permutation is equal to
         * Perm<n>::Sn[i].  This will be between 0 and <i>n</i>!-1 inclusive.
         */
        constexpr Index SnIndex() const;

        /**
         * Returns the lexicographical index of this permutation.  This will
         * be the index of this permutation in the Perm<n>::orderedSn array.
         *
         * See orderedSn for further information on lexicographical ordering.
         *
         * \return the lexicographical index of this permutation.
         * This will be between 0 and <i>n</i>!-1 inclusive.
         */
        constexpr Index orderedSnIndex() const;

        /**
         * Returns a random permutation on \a n elements.
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
         * Returns a random permutation on \a n elements, using the
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
         * The representation will consist of \a n adjacent digits
         * representing the images of 0,...,<i>n</i>-1 respectively.
         * If \a n > 10, then lower-case hexadecimal digits will be used.
         *
         * An example of a string representation for \a n = 5 is `30421`.
         *
         * \return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * \param len the length of the prefix required; this must be
         * between 0 and \a n inclusive.
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
         * of an <i>n</i>-element permutation.
         *
         * \param enc the tight encoding for an <i>n</i>-element permutation.
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
         * exception).  The input routine _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * Tight encodings are fast to work with for small permutation classes
         * (\a n ≤ 7), but slower for larger permutation classes
         * (8 ≤ \a n ≤ 16).  See tightEncoding() for further details.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of an <i>n</i>-element permutation.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for an <i>n</i>-element permutation.
         * \return the permutation represented by the given tight encoding.
         */
        static Perm tightDecode(std::istream& input);

        /**
         * Resets the images of all integers from \a from onwards to the
         * identity map.
         *
         * Specifically, for each \a i in the range <i>from</i>,...,<i>n</i>-1,
         * this routine will ensure that `image[i] == i`.  The images of
         * 0,1,...,<i>from</i>-1 will not be altered.
         *
         * \pre The images of <i>from</i>,...,<i>n</i>-1 are exactly
         * <i>from</i>,...,<i>n</i>-1, but possibly in a different order.
         *
         * \param from the first integer whose image should be reset.
         * This must be between 0 and \a n inclusive.
         */
        void clear(unsigned from);

        /**
         * Extends a <i>k</i>-element permutation to an <i>n</i>-element
         * permutation, where 2 ≤ \a k \< \a n.
         *
         * The resulting permutation will map 0,...,<i>k</i>-1 to their
         * respective images under \a p, and will map the "unused" elements
         * <i>k</i>,...,<i>n</i>-1 to themselves.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be at least 2, and strictly less than \a n.
         *
         * \param p a permutation on \a k elements.
         * \return the same permutation expressed as a permutation on
         * \a n elements.
         */
        template <int k>
        static constexpr Perm extend(Perm<k> p);

        /**
         * Restricts a <i>k</i>-element permutation to an <i>n</i>-element
         * permutation, where \a k > \a n.
         *
         * The resulting permutation will map 0,...,<i>n</i>-1 to their
         * respective images under \a p, and will ignore the "unused" images
         * \a p[\a n],...,\a p[<i>k</i>-1].
         *
         * \pre The given permutation maps 0,...,<i>n</i>-1 to 0,...,<i>n</i>-1
         * in some order.
         *
         * \tparam k the number of elements for the input permutation;
         * this must be strictly greater than \a n.
         *
         * \param p a permutation on \a k elements.
         * \return the same permutation restricted to a permutation on
         * \a n elements.
         */
        template <int k>
        static constexpr Perm contract(Perm<k> p);

        /**
         * Is this permutation minimal in its conjugacy class?
         *
         * Here "minimal" means that, amongst all its conjugates, this
         * permutation has the smallest index in the array Perm<n>::Sn.
         *
         * See Sn for further information on how permutations are indexed.
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
         * \param code the internal code from which the new permutation
         * will be created.
         */
        constexpr Perm(Code code);

    private:
        /**
         * Encodes a "partial" permutation, as a one-to-one map from [0..len)
         * into [0..n).  The main point of this class is to iterate through
         * all such partial permutations, for use in building our precomputed
         * tables.
         */
        template <int len>
        struct Slice {
            int image[len];
                /**< The image of \a i, for each \a i. */
            bool used[n];
                /**< Indicates which values are seen in the range of this
                     map. */

            static constexpr int length = len;
                /**< The number of images encoded in this partial
                     permutation. */

            /**
             * Initialises this to the first partial permutation, which
             * maps \a i to \a i.
             */
            Slice();

            /**
             * Increments this to become the next partial permutation in
             * a lexicographical ordering.  If there is no next permutation
             * then this does nothing and returns \c false.
             */
            bool inc();

            /**
             * Constructs a "partial image pack" corresponding to this
             * partial permutation.
             */
            ImagePack pack() const;
        };

        using LowerSlice = Slice<(n + 1) / 2>;
        using UpperSlice = Slice<n / 2>;

        /**
         * Swaps the images of a and b in this permutation.
         *
         * \pre a < b.
         */
        constexpr void swapImages(int a, int b);

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
         * a tight encoding of an <i>n</i>-element permutation.
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

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * Perm::str().
 *
 * \param out the output stream to which to write.
 * \param p the permutation to write.
 * \return a reference to \a out.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n>
inline std::ostream& operator << (std::ostream& out, const Perm<n>& p) {
    return (out << p.str());
}

// Note that some of our permutation classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers just yet.
template <> class Perm<2>;
template <> class Perm<3>;
template <> class Perm<4>;
template <> class Perm<5>;
template <> class Perm<6>;
template <> class Perm<7>;

/**
 * Represents a conjugacy class of permutations on \a n elements.
 *
 * Each such conjugacy class correpsonds to an unordered partition of \a n,
 * describing the cycle structure of the permutations in the conjugacy class.
 *
 * There is a special "past-the-end" conjugacy class, which effectively
 * holds no value; the only way you can obtain such a class is to iterate
 * through \e all classes using the increment operators.  You can test
 * for a past-the-end value by casting to \c bool.
 *
 * PermClass objects are, in their current implementation, entirely
 * stack-based.  This means they cannot support fast move or swap operations.
 * However, their size is linear in \a n, so while copy operations are
 * not enormously expensive, they are not as cheap as copying a Perm object
 * (which just holds a single machine-native integer).  This decision is a
 * deliberate trade-off between speed versus space; the implication for
 * end users is that you should be economical about copying PermClass objects,
 * and work with them in-place where possible.
 *
 * \python Python does not support templates.  For each
 * \a n = 2,...,16, this class is available in Python under the
 * corresponding name PermClass2, PermClass3, ..., PermClass16.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n>
class PermClass {
    public:
        /**
         * The total number of distinct conjugacy classes of permutations
         * on \a n elements.
         */
        static constexpr int count = detail::countPermClasses[n];

    private:
        int nCycles_;
            /**< The number of cycles in a permutation in this conjugacy
                 class. */
        int cycle_[n];
            /**< The individual cycle lengths for a permutation in this
                 conjugacy class; these are stored in increasing order. */

    public:
        /**
         * Creates the conjugacy class for the identity permutation.
         *
         * The corresponding cycle structure (i.e., partition of \a n)
         * is \a n cycles, each of length 1.
         */
        PermClass();
        /**
         * Creates a copy of the given conjugacy class.
         *
         * \param src the conjugacy class to clone.
         */
        PermClass(const PermClass& src);

        /**
         * Sets this to be a copy of the given conjugacy class.
         *
         * \param src the conjugacy class to clone.
         * \return a reference to this conjugacy class.
         */
        PermClass& operator = (const PermClass& src);

        /**
         * Determines whether this and the given object describe the
         * same conjugacy class.
         *
         * Two past-the-end conjugacy classes will be treated as equal.
         *
         * \param other the conjugacy class to compare with this.
         * \return \c true if and only if this and the given conjugacy class
         * are identical.
         */
        bool operator == (const PermClass& other) const;
        /**
         * Determines whether this and the given object describe
         * different conjugacy classes.
         *
         * Two past-the-end conjugacy classes will be treated as equal.
         *
         * \param other the conjugacy class to compare with this.
         * \return \c true if and only if this and the given conjugacy class
         * are different.
         */
        bool operator != (const PermClass& other) const;

        /**
         * Determines whether this is the conjugacy class for the identity
         * permutation.
         *
         * \return \c true if and only if this conjugacy class represents
         * the identity permutation.
         */
        bool isIdentity() const;

        /**
         * Returns the minimal representative of this conjugacy class.
         *
         * The _minimal representative_ is the permutation belonging to this
         * class with the smallest index in Perm<n>::Sn.
         *
         * Because all permutations in the same class must have the same sign,
         * it follows that this is also the permutation in this class with the
         * smallest index in Perm<n>::orderedSn.
         *
         * \pre This is not the past-the-end conjugacy class.
         *
         * \return the minimal representative of this conjugacy class.
         */
        Perm<n> rep() const;

        /**
         * Returns a string representation of this conjugacy class.
         *
         * At present, the string representation consists of a sequence
         * of digits indicating the cycle lengths (where cycle lengths
         * 10,11,...,16 use `a,b,...,g` for their respective digits).
         *
         * If this is the past-the-end conjugacy class, a suitable
         * string will be returned.
         *
         * \return a string representation of this conjugacy class.
         */
        std::string str() const;

        /**
         * A preincrement operator that changes this to become the next
         * conjugacy class in a lexicographical enumeration.
         *
         * Specifically, the increment operators work through conjugacy
         * classes in lexicographical order, where each class is represented
         * by its sequence of cycle lengths (given in increasing order).
         * Thus the conjugacy class `11...1` (representing the identity
         * permutation) will come first, and the conjugacy class `n`
         * (representing a single long cycle) will come last.
         *
         * If there are no more conjugacy classes (i.e., this represents
         * a single cycle of length \a n), then this object will be set
         * to the special past-the-end value (which can be tested by
         * casting to a \c bool).
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this conjugacy class after the increment.
         */
        PermClass& operator ++();
        /**
         * A postincrement operator that changes this to become the next
         * conjugacy class in a lexicographical enumeration.
         *
         * Specifically, the increment operators work through conjugacy
         * classes in lexicographical order, where each class is represented
         * by its sequence of cycle lengths (given in increasing order).
         * Thus the conjugacy class `11...1` (representing the identity
         * permutation) will come first, and the conjugacy class `n`
         * (representing a single long cycle) will come last.
         *
         * If there are no more conjugacy classes (i.e., this represents
         * a single cycle of length \a n), then this object will be set
         * to the special past-the-end value (which can be tested by
         * casting to a \c bool).
         *
         * \python This routine is named inc() since python does
         * not support the increment operator.
         *
         * \return a copy of this conjugacy class before the increment took
         * place.
         */
        PermClass operator ++(int);

        /**
         * Determines whether this represents a genuine conjugacy class,
         * or whether it represents the special past-the-end value.
         *
         * See the class notes and the increment operators for further
         * discussion on what past-the-end represents.
         *
         * \return \c false if this is the past-the-end conjugacy class,
         * or \c true otherwise.
         */
        operator bool() const;

        /**
         * Returns the set of all permutations that fix the minimal
         * representative of this conjugacy class under conjugation.
         *
         * Specifically, if \a r is the minimal representative of this class
         * as returned by rep(), then this routine constructs the subgroup of
         * all permutations \a p for which `p.inverse() * r * p == r`.
         *
         * The permutations will be returned in an arbitrary order
         * (and in particular, this order may be subject to change in
         * future releases of Regina).
         *
         * \warning This group could get \e very large.  If this conjugacy
         * class represents the identity permutation, then the centraliser
         * will be all of S_n.  For \a n ≥ 5, it can be show that the
         * next-worst case is where this conjugacy class represents a single
         * pair swap, in which case the centraliser has size `2⋅(n-2)!`.
         *
         * \pre This is not the past-the-end conjugacy class.
         *
         * \pre Arrays on this system can be large enough to store n! objects.
         * This is a technical condition on the bit-size of \c size_t that will
         * be explicitly checked (with an exception thrown if it fails).
         * On a 64-bit system this condition should be true for all supported
         * \a n (that is, \a n ≤ 16), but on a 32-bit or 16-bit system it will
         * mean that centraliser() cannot be used for larger values of \a n.
         *
         * \exception FailedPrecondition A signed integer of the same bit-size
         * as \c size_t cannot hold (n!).  See the precondition above for
         * further discussion on this constraint.
         *
         * \return all permutations that leave rep() fixed under conjugation.
         */
        std::vector<Perm<n>> centraliser() const;
};

/**
 * Writes a string representation of the given conjugacy class of permutations
 * to the given output stream.  The format will be the same as is used by
 * PermClass<n>::str().
 *
 * \param out the output stream to which to write.
 * \param p the conjugacy class to write.
 * \return a reference to \a out.
 *
 * \tparam n the number of objects being permuted.  This must be between
 * 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n>
inline std::ostream& operator << (std::ostream& out, const PermClass<n>& c) {
    return (out << c.str());
}

// Static variables for Perm

template <int n> std::mutex Perm<n>::precomputeMutex;
template <int n> typename Perm<n>::ImagePack* Perm<n>::invLower_ = nullptr;
template <int n> typename Perm<n>::ImagePack* Perm<n>::invUpper_ = nullptr;

// Inline functions for Perm

template <int n>
constexpr Perm<n> Perm<n>::OrderedSnLookup::operator[] (
        Perm<n>::Index i) const {
    Code code = 0;
    for (int p = 1; p <= n; ++p) {
        // n - p -> i % p;
        code |= (static_cast<Code>(i % p) << ((n - p) * imageBits));
        i /= p;
    }
    for (int pos1 = imageBits * (n - 1); pos1 >= 0; pos1 -= imageBits) {
        for (int pos2 = pos1 + imageBits; pos2 < n * imageBits;
                pos2 += imageBits) {
            if (((code >> pos2) & imageMask) >= ((code >> pos1) & imageMask))
                code += (Code(1) << pos2); // increment image at pos2
        }
    }
    return Perm<n>(code);
}

template <int n>
constexpr Perm<n> Perm<n>::SnLookup::operator[] (Perm<n>::Index i) const {
    Code code = 0;

    // We begin by constructing a code whose successive digits are "base"
    // n, n-1, ... 2, 1.
    // We can already see whether the resulting permutation will be even
    // or odd just from the parity of the sum of these "digits".
    bool parity = (i % 2 == 0);
    bool even = true;
    for (int p = 1; p <= n; ++p) {
        // Here p tells us how far back from the *end* of the code we are.
        int digit = i % p;
        // n - p -> digit
        code |= (static_cast<Code>(digit) << ((n - p) * imageBits));
        if (digit % 2)
            even = ! even;
        i /= p;
    }

    if (even != parity) {
        // Our algorithm below computes orderedSn, not Sn, and these
        // differ at index i.  We adjust the code now to compensate.
        if (even) {
            // i is odd: move to the previous permutation.
            for (int p = 1; p <= n; ++p) {
                int digit = ((code >> ((n - p) * imageBits)) & imageMask);
                // This digit is treated mod p.
                if (digit > 0) {
                    // Decrement digit and stop.
                    code -= (Code(1) << ((n - p) * imageBits));
                    break;
                } else {
                    // Set digit to (p-1) and carry.
                    code |= (Code(p - 1) << ((n - p) * imageBits));
                }
            }
        } else {
            // i is even: move to the next permutation.
            for (int p = 1; p <= n; ++p) {
                int digit = ((code >> ((n - p) * imageBits)) & imageMask);
                // This digit is treated mod p.
                if (digit < p - 1) {
                    // Increment digit and stop.
                    code += (Code(1) << ((n - p) * imageBits));
                    break;
                } else {
                    // Set digit to zero and carry.
                    code ^= (static_cast<Code>(digit) << ((n - p) * imageBits));
                }
            }
        }
    }

    // Carry on as with do with orderedSn.
    for (int pos1 = imageBits * (n - 1); pos1 >= 0; pos1 -= imageBits) {
        for (int pos2 = pos1 + imageBits; pos2 < n * imageBits;
                pos2 += imageBits) {
            if (((code >> pos2) & imageMask) >= ((code >> pos1) & imageMask))
                code += (Code(1) << pos2); // increment image at pos2
        }
    }
    return Perm<n>(code);
}

template <int n>
inline constexpr typename Perm<n>::Code Perm<n>::idCodePartial(int k) {
    return (k == 0 ? 0 :
        (static_cast<Code>(k) << (Perm<n>::imageBits * k)) |
            idCodePartial(k - 1));
}

template <int n>
inline constexpr Perm<n>::Perm() : code_(idCode_) {
}

template <int n>
inline constexpr Perm<n>::Perm(int a, int b) : code_(idCode_) {
    code_ &= ~(imageMask << (imageBits * a));
    code_ &= ~(imageMask << (imageBits * b));
    code_ |= (static_cast<Code>(a) << (imageBits * b));
    code_ |= (static_cast<Code>(b) << (imageBits * a));
}

template <int n>
inline constexpr Perm<n>::Perm(const std::array<int, n>& image) : code_(0) {
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        code_ |= (static_cast<Code>(image[i]) << bits);
}

template <int n>
inline constexpr Perm<n>::Perm(Code code) : code_(code) {
}

template <int n>
inline constexpr typename Perm<n>::Code Perm<n>::permCode() const {
    return code_;
}

template <int n>
inline void Perm<n>::setPermCode(Code code) {
    code_ = code;
}

template <int n>
inline constexpr Perm<n> Perm<n>::fromPermCode(Code code) {
    return Perm<n>(code);
}

template <int n>
constexpr bool Perm<n>::isPermCode(Code code) {
    unsigned mask = 0;
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        mask |= ((unsigned)1 << ((code >> bits) & imageMask));
    if constexpr (n < 16) {
        return (mask + 1 == ((unsigned)1 << n) && (code >> bits) == 0);
    } else {
        // We should not increment mask, since this could overflow on
        // some platforms (since unsigned might be only 16 bits long).
        // Also: code has no "spare bits" beyond the 16 * 4 bits that we use,
        // and so we do not need to check if any unwanted extra bits are set.
        return mask == 0xffff;
    }
}

template <int n>
inline constexpr typename Perm<n>::ImagePack Perm<n>::imagePack() const {
    return permCode();
}

template <int n>
inline constexpr Perm<n> Perm<n>::fromImagePack(ImagePack pack) {
    return fromPermCode(pack);
}

template <int n>
inline constexpr bool Perm<n>::isImagePack(ImagePack pack) {
    return isPermCode(pack);
}

template <int n>
inline constexpr Perm<n> Perm<n>::operator * (const Perm& q) const {
    Code c = 0;
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        c |= (static_cast<Code>((*this)[(q.code_ >> bits) & imageMask])
            << bits);
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::cachedComp(const Perm& q) const {
    Code c = 0;
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        c |= (static_cast<Code>((*this)[(q.code_ >> bits) & imageMask])
            << bits);
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::cachedComp(const Perm& q, const Perm& r)
        const {
    Code c = 0;
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        c |= (static_cast<Code>((*this)[q[(r.code_ >> bits) & imageMask]])
            << bits);
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::conjugate(const Perm<n>& q) const {
    Code c = 0;
    for (int bits = 0; bits < imageBits * n; bits += imageBits) {
        // q[i] -> q[this[i]]
        c |= (static_cast<Code>(q[(code_ >> bits) & imageMask])
            << (imageBits * ((q.code_ >> bits) & imageMask)));
    }
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::cachedConjugate(const Perm<n>& q) const {
    Code c = 0;
    for (int bits = 0; bits < imageBits * n; bits += imageBits) {
        // q[i] -> q[this[i]]
        c |= (static_cast<Code>(q[(code_ >> bits) & imageMask])
            << (imageBits * ((q.code_ >> bits) & imageMask)));
    }
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::inverse() const {
    Code c = 0;
    for (int i = 0; i < n; ++i)
        c |= (static_cast<Code>(i) << (imageBits * (*this)[i]));
    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::cachedInverse() const {
    return invLower_[code_ & lowerMask] |
        invUpper_[(code_ & upperMask) >> upperShift];
}

template <int n>
constexpr Perm<n> Perm<n>::pow(long exp) const {
    // Get the trivial cases out of the way first.
    if (exp == 0)
        return Perm<n>();
    else if (exp == 1)
        return *this;
    else if (exp == -1)
        return inverse();

    // Work out the power by using the cycle structure.

    // Use a bitmask to track which elements we've visited, since we want to
    // be constexpr (which rules out anything that requires std::fill).
    static_assert(sizeof(int) * 8 >= n);
    int seen = 0;

    Code c = 0;

    for (int i = 0; i < n; ++i) {
        if (seen & (1 << i))
            continue;

        int len = 0;
        Code cycle[n];

        int j = i;
        do {
            cycle[len] = j;
            seen |= (1 << j);
            j = (*this)[j];
            ++len;
        } while (j != i);

        int shift = exp % len;
        if (shift < 0)
            shift += len;

        for (j = 0; j < len; ++j) {
            // cycle[j] maps to cycle[(j + shift) % len].
            c |= (cycle[(j + shift) % len] << (imageBits * cycle[j]));
        }
    }

    return Perm<n>(c);
}

template <int n>
inline constexpr Perm<n> Perm<n>::cachedPow(long exp) const {
    return pow(exp);
}

template <int n>
constexpr int Perm<n>::order() const {
    // Work out the order by using the cycle structure.
    int ans = 1;

    // Use a bitmask to track which elements we've visited, since we want to
    // be constexpr (which rules out anything that requires std::fill).
    static_assert(sizeof(int) * 8 >= n);
    int seen = 0;

    for (int i = 0; i < n; ++i) {
        if (seen & (1 << i))
            continue;

        int len = 0;

        int j = i;
        do {
            seen |= (1 << j);
            j = (*this)[j];
            ++len;
        } while (j != i);

        ans = std::lcm(ans, len);
    }

    return ans;
}

template <int n>
inline constexpr int Perm<n>::cachedOrder() const {
    return order();
}

template <int n>
inline constexpr Perm<n> Perm<n>::reverse() const {
    Code c = 0;
    int bits = imageBits * (n - 1);
    for (int i = 0; i < n; ++i, bits -= imageBits)
        c |= (static_cast<Code>((*this)[i]) << bits);
    return Perm<n>(c);
}

template <int n>
constexpr int Perm<n>::sign() const {
#if 0
    // This algorithm is quadratic in n.  Surely we can do better?
    bool even = true;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if ((*this)[i] > (*this)[j])
                even = ! even;
    return (even ? 1 : -1);
#endif
    // Yes, we can.
    // The algorithm commented out above is quadratic with low overhead;
    // this one is linear with a little more overhead.  Testing on my
    // build machine suggests this linear algorithm is indeed faster for
    // the cases we care about (8 <= n <= 16).
    bool evenPerm = true;

    // Use a bitmask to track which indices we've seen, since we want to
    // be constexpr (which rules out anything that requires std::fill).
    static_assert(sizeof(int) * 8 >= n);
    int seen = 0;

    for (int i = 0; i < n; ++i) {
        if (seen & (1 << i))
            continue;

        int j = i;
        bool oddCycle = true;
        do {
            j = (*this)[j];
            seen |= (1 << j);
            oddCycle = ! oddCycle;
        } while (j != i);

        if (oddCycle)
            evenPerm = ! evenPerm;
    }

    return (evenPerm ? 1 : -1);
}

template <int n>
inline constexpr int Perm<n>::operator[](int source) const {
    return (code_ >> (imageBits * source)) & imageMask;
}

template <int n>
inline constexpr int Perm<n>::pre(int image) const {
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        if (((code_ >> bits) & imageMask) == image)
            return i;
    // We should never reach this point.
    return -1;
}

template <int n>
inline constexpr bool Perm<n>::operator == (const Perm& other) const {
    return (code_ == other.code_);
}

template <int n>
inline constexpr bool Perm<n>::operator != (const Perm& other) const {
    return (code_ != other.code_);
}

template <int n>
constexpr int Perm<n>::compareWith(const Perm& other) const {
    Code mask = imageMask;
    for (int i = 0; i < n; ++i) {
        if ((code_ & mask) < (other.code_ & mask))
            return -1;
        if ((code_ & mask) > (other.code_ & mask))
            return 1;
        mask <<= imageBits;
    }
    return 0;
}

template <int n>
inline constexpr bool Perm<n>::isIdentity() const {
    return (code_ == idCode_);
}

template <int n>
inline Perm<n> Perm<n>::operator ++(int) {
    // Note: this cannot be constexpr, since it calls the (non-constexpr)
    // preincrement operator.
    Perm<n> ans(code_);
    ++(*this);
    return ans;
}

template <int n>
constexpr bool Perm<n>::operator < (const Perm<n>& rhs) const {
    if (code_ == rhs.code_)
        return false;

    // The following mask blots out the images of n-2 and n-1.
    Code mask = ~((imageMask | (imageMask << imageBits))
        << (imageBits * (n-2)));
    if ((code_ & mask) == (rhs.code_ & mask)) {
        // The two permutations differ precisely in the last two images.
        // In this case, the even permutation will have smaller Sn index.
        return sign() > 0;
    } else {
        // The two permutations do not just differ in the last two images.
        // This means that comparison by Sn indices will give the same
        // result as comparison by orderedSn indices.
        return compareWith(rhs) < 0;
    }
}

template <int n>
constexpr Perm<n> Perm<n>::rot(int i) {
    Code code = 0;
    Code src = 0;
    Code dest = i;
    int bits = 0;
    while (src < n) {
        code |= (dest << bits);
        ++src;
        if (++dest == n)
            dest = 0;
        bits += imageBits;
    }
    return Perm<n>(code);
}

template <int n>
constexpr typename Perm<n>::Index Perm<n>::orderedSnIndex() const {
    Index ans = 0;
    uint16_t seen = 0; // bitmask
    int p = 0, pos1 = 0;
    for ( ; p < n - 1; ++p, pos1 += imageBits) {
        // position pos1 holds the (p)th image
        Code pImg = (code_ >> pos1) & imageMask; // image at pos1
        seen |= (1 << pImg);

        // Identify how many images we have already seen that are less
        // than pImg: this tells us how to "rescale" pImg.
        pImg -= BitManipulator<uint16_t>::bits(
            seen & ((uint16_t(1) << pImg) - 1));

        ans *= (n - p);
        ans += pImg;
    }
    return ans;
}

template <int n>
constexpr typename Perm<n>::Index Perm<n>::SnIndex() const {
    Index ans = 0;
    bool even = true;
    uint16_t seen = 0; // bitmask
    int p = 0, pos1 = 0;
    // Unlike orderedSnIndex(), we need to run this loop all the way to
    // position n-1 since we need it for the sign test.
    for ( ; p < n; ++p, pos1 += imageBits) {
        // position pos1 holds the (p)th image
        Code pImg = (code_ >> pos1) & imageMask; // image at pos1
        seen |= (1 << pImg);

        // Identify how many images we have already seen that are less
        // than pImg: this tells us how to "rescale" pImg.
        int lower = BitManipulator<uint16_t>::bits(
            seen & ((uint16_t(1) << pImg) - 1));
        pImg -= lower;
        if ((p - lower) & 1)
            even = ! even; // odd # of inversions for the sign computation

        ans *= (n - p);
        ans += pImg;
    }

    return (even == (ans % 2 == 0) ? ans : (ans ^ 1));
}

template <int n>
Perm<n> Perm<n>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <int n>
template <class URBG>
Perm<n> Perm<n>::rand(URBG&& gen, bool even) {
    // Note: This generic implementation of Perm covers 8 <= n <= 16.
    // The corresponding index types require 16, 32 or 64 bits.
    //
    // A slight messiness here is that std::uniform_int_distribution
    // requires the type argument to be one of short, int, long or long long.
    static_assert(sizeof(Index) <= sizeof(long long),
        "Permutation index cannot fit inside a long long");
    using Arg = std::conditional_t<sizeof(Index) <= sizeof(short), short,
        std::conditional_t<sizeof(Index) <= sizeof(int), int,
        std::conditional_t<sizeof(Index) <= sizeof(long), long,
        long long>>>;

    if (even) {
        std::uniform_int_distribution<Arg> d(0, (nPerms / 2) - 1);
        return Sn[2 * d(gen)];
    } else {
        // Computing orderedSn is faster than computing Sn.
        std::uniform_int_distribution<Arg> d(0, nPerms - 1);
        return orderedSn[d(gen)];
    }
}
#endif // __DOXYGEN

template <int n>
std::string Perm<n>::str() const {
    char ans[n + 1];
    int bits = 0;
    for (int i = 0; i < n; ++i, bits += imageBits)
        ans[i] = regina::digit((code_ >> bits) & imageMask);
    ans[n] = 0;

    return ans;
}

template <int n>
std::string Perm<n>::trunc(int len) const {
    char ans[n + 1];
    int bits = 0;
    for (int i = 0; i < len; ++i, bits += imageBits)
        ans[i] = regina::digit((code_ >> bits) & imageMask);
    ans[len] = 0;

    return ans;
}

template <int n>
template <int k>
constexpr Perm<n> Perm<n>::extend(Perm<k> p) {
    static_assert(k < n, "Perm<n>::extend<k> requires k < n.");
    static_assert(n > 7, "The generic implementation of Perm<n>::extend() "
        "should not be used for n <= 7.");

    // TODO: Reimplement this to replace the first loop with a direct
    // copy of p's code, in the case where Perm<k> and Perm<n> use the
    // same style of code with the same value of imageBits.

    Code c = 0;
    int i = 0;
    int bits = 0;
    for ( ; i < k; ++i, bits += imageBits)
        c |= (static_cast<Code>(p[i]) << bits);
    for ( ; i < n; ++i, bits += imageBits)
        c |= (static_cast<Code>(i) << bits);

    return Perm<n>(c);
}

template <int n>
template <int k>
constexpr Perm<n> Perm<n>::contract(Perm<k> p) {
    static_assert(n < k, "Perm<n>::contract<k> requires n < k.");
    static_assert(n > 7, "The generic implementation of Perm<n>::contract() "
        "should not be used for n <= 7.");

    // TODO: Reimplement this to directly truncate p's code, in the case
    // where Perm<k> and Perm<n> use the same style of code with the
    // same value of imageBits.

    Code c = 0;
    int i = 0;
    int bits = 0;
    for ( ; i < n; ++i, bits += imageBits)
        c |= (static_cast<Code>(p[i]) << bits);

    return Perm<n>(c);
}

template <int n>
inline void Perm<n>::clear(unsigned from) {
    static_assert(n > 7, "The generic implementation of Perm<n>::clear() "
        "should not be used for n <= 7.");

    if (from < n) {
        Code mask = (~static_cast<Code>(0)) << (from * imageBits);
        code_ &= ~mask;
        code_ |= (idCode_ & mask);
    }
}

template <int n>
void Perm<n>::tightEncode(std::ostream& out) const {
    // Write the Sn index in base 94, least significant digit first.
    Index idx = SnIndex();
    for (int i = 0; i < tightChars_; ++i) {
        out << static_cast<char>((idx % 94) + 33);
        idx /= 94;
    }
}

template <int n>
std::string Perm<n>::tightEncoding() const {
    // Write the Sn index in base 94, least significant digit first.
    char ans[tightChars_ + 1];
    Index idx = SnIndex();
    for (int i = 0; i < tightChars_; ++i) {
        ans[i] = static_cast<char>((idx % 94) + 33);
        idx /= 94;
    }
    ans[tightChars_] = 0;
    return ans;
}

template <int n>
template <typename iterator>
Perm<n> Perm<n>::tightDecode(iterator start, iterator limit,
        bool noTrailingData) {
    // Ensure that our calculations will not overflow, even when reading
    // an *invalid* encoding.  Here we note that:
    //   - 2^31 > 94^4;
    //   - 2^63 > 94^7.
    // We enforce this now so that we will notice that we could technically
    // break things if we optimise the Index type at a later date.  (Though
    // the only possible "breakage" is that an invalid encoding might be
    // incorrectly recognised as valid due to an overflow wrap-around).
    static_assert(sizeof(Index) >= (tightChars_ <= 4 ? 4 : 8),
        "The Index type could potentially overflow when reading an "
        "invalid tight encoding.");

    Index idx = 0;
    Index power = 1;
    for (int i = 0; i < tightChars_; ++i) {
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        Index piece = (*start++) - 33;
        // code >= 0 because we are using an unsigned data type.
        if (piece > 94)
            throw InvalidInput("The tight encoding is invalid");
        idx += (piece * power);
        power *= 94;
    }

    if (idx < 0 || idx >= nPerms)
        throw InvalidInput("The tight encoding is invalid");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");

    return Sn[idx];
}

template <int n>
inline Perm<n> Perm<n>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

template <int n>
inline Perm<n> Perm<n>::tightDecoding(const std::string& enc) {
    try {
        return tightDecode(enc.begin(), enc.end(), true);
    } catch (const InvalidInput& exc) {
        // For strings we use a different exception type.
        throw InvalidArgument(exc.what());
    }
}

template <int n>
constexpr bool Perm<n>::isConjugacyMinimal() const {
    int prevCycle = 0;
    int currCycle = 0;
    for (int j = 0; j < n; ++j) {
        int img = (*this)[j];
        if (img > j + 1)
            return false;
        else if (img == j + 1)
            ++currCycle;
        else {
            // We have closed off a cycle.
            ++currCycle;
            if (currCycle < prevCycle)
                return false;
            prevCycle = currCycle;
            currCycle = 0;
        }
    }
    return true;
}

template <int n>
inline constexpr void Perm<n>::swapImages(int a, int b) {
    Code diff = ((code_ >> (a * imageBits)) ^ (code_ >> (b * imageBits)))
        & imageMask;
    code_ = code_ ^ (diff << (a * imageBits)) ^ (diff << (b * imageBits));
}

template <int n>
template <int len>
inline Perm<n>::Slice<len>::Slice() {
    for (int i = 0; i < len; ++i)
        image[i] = i;
    std::fill(used, used + len, true);
    std::fill(used + len, used + n, false);
}

template <int n>
template <int len>
bool Perm<n>::Slice<len>::inc() {
    int pos = len - 1;
    while (pos >= 0) {
        used[image[pos]] = false;
        ++image[pos];
        while (image[pos] < n && used[image[pos]])
            ++image[pos];
        if (image[pos] < n) {
            used[image[pos]] = true;
            break;
        }
        --pos;
    }

    if (pos < 0)
        return false;

    ++pos;
    int next = 0;
    while (pos < len) {
        while (used[next])
            ++next;
        used[next] = true;
        image[pos++] = next++;
    }

    return true;
}

template <int n>
template <int len>
inline typename Perm<n>::ImagePack Perm<n>::Slice<len>::pack() const {
    ImagePack ans = 0;
    int bits = 0;
    for (int i = 0; i < len; ++i, bits += imageBits)
        ans |= (static_cast<ImagePack>(image[i]) << bits);
    return ans;
}

// Inline functions for PermClass

template <int n>
inline PermClass<n>::PermClass() : nCycles_(n) {
    std::fill(cycle_, cycle_ + n, 1);
}

template <int n>
inline PermClass<n>::PermClass(const PermClass& src) : nCycles_(src.nCycles_) {
    std::copy(src.cycle_, src.cycle_ + n, cycle_);
}

template <int n>
inline PermClass<n>& PermClass<n>::operator = (const PermClass& src) {
    nCycles_ = src.nCycles_;
    std::copy(src.cycle_, src.cycle_ + n, cycle_);
    return *this;
}

template <int n>
inline bool PermClass<n>::operator == (const PermClass& other) const {
    return nCycles_ == other.nCycles_ &&
        std::equal(cycle_, cycle_ + n, other.cycle_);
}

template <int n>
inline bool PermClass<n>::operator != (const PermClass& other) const {
    return nCycles_ != other.nCycles_ ||
        ! std::equal(cycle_, cycle_ + n, other.cycle_);
}

template <int n>
inline bool PermClass<n>::isIdentity() const {
    return nCycles_ == n;
}

template <int n>
inline PermClass<n>::operator bool() const {
    return nCycles_;
}

template <int n>
inline Perm<n> PermClass<n>::rep() const {
    std::array<int, n> img;
    int pos = 0;
    for (int c = 0; c < nCycles_; ++c) {
        for (int j = 0; j < cycle_[c] - 1; ++j)
            img[pos + j] = pos + j + 1;
        img[pos + cycle_[c] - 1] = pos;
        pos += cycle_[c];
    }
    return Perm<n>(img);
}

template <int n>
inline std::string PermClass<n>::str() const {
    if (nCycles_) {
        char ans[n + 1];
        for (int i = 0; i < nCycles_; ++i)
            ans[i] = regina::digit(cycle_[i]);
        ans[nCycles_] = 0;

        return ans;
    } else {
        return "(past-the-end)";
    }
}

template <int n>
inline PermClass<n>& PermClass<n>::operator ++() {
    if (nCycles_ <= 1) {
        // End of the line.
        nCycles_ = 0;
    } else {
        if (cycle_[nCycles_ - 2] <= cycle_[nCycles_ - 1] - 2) {
            // Replace ... x y -> ... (x+1) (x+1) ... (x+1) (leftover)
            int leftover = cycle_[nCycles_ - 1] - 1;
            int base = ++cycle_[nCycles_ - 2];
            --nCycles_;
            while (leftover >= 2 * base) {
                cycle_[nCycles_++] = base;
                leftover -= base;
            }
            cycle_[nCycles_++] = leftover;
        } else {
            // Replace ... x y -> ... (x+y)
            cycle_[nCycles_ - 2] += cycle_[nCycles_ - 1];
            --nCycles_;
        }
    }

    return *this;
}

template <int n>
inline PermClass<n> PermClass<n>::operator ++(int) {
    PermClass<n> ans = *this;
    ++(*this);
    return ans;
}

} // namespace regina

// Import the specialised template classes.
#include "maths/spec/perm6.h" // Required by Perm<4>::pairs()
#include "maths/spec/perm2.h"
#include "maths/spec/perm3.h"
#include "maths/spec/perm4.h"
#include "maths/spec/perm5.h"
#include "maths/spec/perm7.h"

namespace regina {

// What follows are implementations that use these specialised classes.
// We hide them from doxygen since specialisations can confuse it.
#ifndef __DOXYGEN

template <int k>
inline constexpr Perm<2> Perm<2>::contract(Perm<k> p) {
    static_assert(k >= 8, "The generic implementation of Perm<2>::contract<k> "
        "requires k >= 8.");

    return Perm<2>(static_cast<Code>(p.permCode() % 2 ? 1 : 0));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<3> p) {
    return Perm<2>(static_cast<Code>(p.permCode() == 0 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<4> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 6 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<5> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 24 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<6> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 120 ? 0 : 1));
}

template <>
inline constexpr Perm<2> Perm<2>::contract(Perm<7> p) {
    return Perm<2>(static_cast<Code>(p.permCode2() < 720 ? 0 : 1));
}

inline void Perm<2>::clear(unsigned from) {
    if (from == 0)
        code_ = 0;
}

template <>
inline constexpr Perm<3> Perm<3>::extend(Perm<2> p) {
    return Perm<3>(static_cast<Code>(
        p.permCode() == 0 ? code012 : code102));
}

template <int k>
inline constexpr Perm<3> Perm<3>::contract(Perm<k> p) {
    static_assert(k >= 5, "The generic implementation of Perm<3>::contract<k> "
        "requires k >= 5.");

    return Perm<3>(p[0], p[1], p[2]);
}

template <>
inline constexpr Perm<3> Perm<3>::contract(Perm<4> p) {
    // Code map: 0,3,8,7,12,15 -> 0,1,2,3,4,5.
    Perm<4>::Code2 c = p.permCode2();
    return Perm<3>::fromPermCode(c == 8 ? 2 : c == 7 ? 3 : c / 3);
}

inline void Perm<3>::clear(unsigned from) {
    if (from <= 1)
        code_ = code012;
}

template <>
inline constexpr Perm<4> Perm<4>::extend(Perm<2> p) {
    return Perm<4>(static_cast<Code2>(p.permCode() == 0 ? 0 : 7));
}

template <>
inline constexpr Perm<4> Perm<4>::extend(Perm<3> p) {
    // This is implemented as an array lookup.
    return Perm<4>::S3[p.S3Index()];
}

template <int k>
inline constexpr Perm<4> Perm<4>::contract(Perm<k> p) {
    static_assert(k >= 5, "The generic implementation of Perm<4>::contract<k> "
        "requires k >= 5.");

    return Perm<4>(p[0], p[1], p[2], p[3]);
}

inline void Perm<4>::clear(unsigned from) {
    if (from <= 1)
        code_ = 0;
    else if (from == 2)
        code_ = (imageTable[code_][0] == 0 ? 0 /* 0123 */ : 7 /* 1023 */);
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<2> p) {
    return Perm<5>(static_cast<Code2>(p.permCode() == 0 ? 0 : 25));
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<3> p) {
    // This is implemented as an array lookup.
    return Perm<5>::S3[p.S3Index()];
}

template <>
inline constexpr Perm<5> Perm<5>::extend(Perm<4> p) {
    // This is implemented as an array lookup.
    return Perm<5>::S4[p.S4Index()];
}

template <int k>
constexpr Perm<5> Perm<5>::contract(Perm<k> p) {
    static_assert(k > 5, "Perm<5>::contract<k> requires k > 5.");

    return Perm<5>(p[0], p[1], p[2], p[3], p[4]);
}

inline void Perm<5>::clear(unsigned from) {
    if (from <= 1)
        code2_ = 0;
    else if (from == 2)
        code2_ = (imageTable[code2_][0] == 0 ? 0 /* 01234 */ : 25 /* 10234 */);
    else if (from == 3) {
        if (imageTable[code2_][3] == 4)
            code2_ = code2_ ^ 1;
    }
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<2> p) {
    return Perm<6>(static_cast<Code2>(p.permCode() == 0 ? 0 : 121));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<3> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode()));
    // Now p6 acts on {3,4,5} in the way that p acts on {0,1,2}.

    return Perm<6>(static_cast<Code2>(451 /* 345012 */)) *
        p6 * Perm<6>(static_cast<Code2>(451 /* 345012 */));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<4> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode2()));
    // Now p6 acts on {2,3,4,5} in the way that p acts on {0,1,2,3}.

    return Perm<6>(static_cast<Code2>(576 /* 450123 */)) *
        p6 * Perm<6>(static_cast<Code2>(304 /* 234501 */));
}

template <>
inline constexpr Perm<6> Perm<6>::extend(Perm<5> p) {
    Perm<6> p6(static_cast<Code2>(p.permCode2()));
    // Now p6 acts on {1,2,3,4,5} in the way that p acts on {0,1,2,3,4}.

    return Perm<6>(static_cast<Code2>(601 /* 501234 */)) *
        p6 * Perm<6>(static_cast<Code2>(153 /* 123450 */));
}

template <int k>
constexpr Perm<6> Perm<6>::contract(Perm<k> p) {
    static_assert(k > 6, "Perm<6>::contract<k> requires k > 6.");

    return Perm<6>(p[0], p[1], p[2], p[3], p[4], p[5]);
}

inline void Perm<6>::clear(unsigned from) {
    switch (from) {
        case 0:
        case 1:
            code2_ = 0;
            break;
        case 2:
            // Test if 0 -> 0.
            code2_ = (code2_ < 120 ? 0 /* 012345 */ : 121 /* 102345 */);
            break;
        case 3:
            // Calculate the target ordered S6 index.
            // When rounded down to the nearest multiple of 6,
            // the code should be: 0, 24, 120, 144, 240 or 264.
            // What we want is:    0, 25, 121, 144, 240 or 265.
            code2_ -= (code2_ % 6);
            if (code2_ == 24 || code2_ == 120 || code2_ == 264)
                code2_ ^= 1;
            break;
        case 4:
            if ((*this)[4] == 5)
                code2_ = code2_ ^ 1;
            break;
        default:
            break;
    }
}

template <>
inline constexpr Perm<7> Perm<7>::extend(Perm<2> p) {
    return Perm<7>(static_cast<Code2>(p.permCode() == 0 ? 0 : 721));
}

template <int k>
inline constexpr Perm<7> Perm<7>::extend(Perm<k> p) {
    static_assert(2 < k && k < 7,
        "The generic implementation of Perm<7>::extend<k> requires 2 < k < 7.");

    Perm<7> p7(static_cast<Code2>(p.SnIndex()));
    // Now p7 acts on {(7-k),...,6} in the way that p acts on {0,...,(k-1)}.

    // Since rot(k) can be evaluated at compile-time, we hope that the
    // compiler actually does this.
    return rot(k) * p7 * rot(7 - k);
}

template <int k>
constexpr Perm<7> Perm<7>::contract(Perm<k> p) {
    static_assert(k > 7, "Perm<7>::contract<k> requires k > 7.");

    return Perm<7>(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
}

inline void Perm<7>::clear(unsigned from) {
    switch (from) {
        case 0:
        case 1:
            code2_ = 0;
            break;
        case 2:
            // Test if 0 -> 0.
            code2_ = (code2_ < 720 ? 0 /* 0123456 */ : 721 /* 1023456 */);
            break;
        case 3:
            // When rounded down to the nearest multiple of 24,
            // the code is the correct *ordered* S7 index.
            code2_ = convOrderedUnordered<Code2>(code2_ - (code2_ % 24));
            break;
        case 4:
            // When rounded down to the nearest multiple of 6,
            // the code is the correct *ordered* S7 index.
            code2_ = convOrderedUnordered<Code2>(code2_ - (code2_ % 6));
            break;
        case 5:
            if ((*this)[5] == 6)
                code2_ = code2_ ^ 1;
            break;
        default:
            break;
    }
}

#endif // __DOXYGEN

} // namespace regina

#endif

