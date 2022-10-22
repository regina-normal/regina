
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

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include "regina-core.h"
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
 * \ifacespython Python does not support templates.  For each
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
         * for \a n ≥ 7.
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

    public:
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
        static constexpr bool isPermCode(Code newCode);

        /**
         * Returns the image pack that represents this permutation.
         *
         * See the class notes for more information on image packs, and how
         * they are used to build permutation codes.
         *
         * For \a n ≥ 7, this routine is identical to permCode().
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
         * For \a n ≥ 7, this routine is identical to fromPermCode().
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
         * For \a n ≥ 7, this routine is identical to isPermCode().
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
         * Finds the inverse of this permutation.
         *
         * \return the inverse of this permutation.
         */
        constexpr Perm inverse() const;

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
 * This must be between 3 and 16 inclusive.
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
    for (int i = 0; i < n; ++i)
        code_ |= (static_cast<Code>(image[i]) << (imageBits * i));
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
    for (int i = 0; i < n; ++i)
        mask |= ((unsigned)1 << ((code >> (imageBits * i)) & imageMask));
    if constexpr (n < 16) {
        return (mask + 1 == ((unsigned)1 << n) &&
            (code >> (imageBits * n)) == 0);
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
    for (int i = 0; i < n; ++i)
        c |= (static_cast<Code>((*this)[q[i]]) << (imageBits * i));
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
inline constexpr Perm<n> Perm<n>::reverse() const {
    Code c = 0;
    for (int i = 0; i < n; ++i)
        c |= (static_cast<Code>((*this)[i]) << (imageBits * (n - 1 - i)));
    return Perm<n>(c);
}

template <int n>
constexpr int Perm<n>::sign() const {
    // This algorithm is quadratic in n.  Surely we can do better?
    bool even = true;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if ((*this)[i] > (*this)[j])
                even = ! even;
    return (even ? 1 : -1);
}

template <int n>
inline constexpr int Perm<n>::operator[](int source) const {
    return (code_ >> (imageBits * source)) & imageMask;
}

template <int n>
inline constexpr int Perm<n>::pre(int image) const {
    for (int i = 0; i < n; ++i)
        if (((code_ >> (imageBits * i)) & imageMask) == image)
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
    for (int i = 0; i < n; ++i) {
        if ((*this)[i] < other[i])
            return -1;
        if ((*this)[i] > other[i])
            return 1;
    }
    return 0;
}

template <int n>
inline constexpr bool Perm<n>::isIdentity() const {
    return (code_ == idCode_);
}

template <int n>
constexpr Perm<n> Perm<n>::rot(int i) {
    Code code = 0;
    Code src = 0;
    Code dest = i;
    while (src < n) {
        code |= (dest << (imageBits * src));
        ++src;
        if (++dest == n)
            dest = 0;
    }
    return Perm<n>(code);
}

template <int n>
constexpr typename Perm<n>::Index Perm<n>::orderedSnIndex() const {
    Index ans = 0;
    Code c = code_;
    int p = 0, pos1 = 0;
    for ( ; p < n - 1; ++p, pos1 += imageBits) {
        // position pos1 holds the (p)th image
        Code pImg = (c >> pos1) & imageMask; // image at pos1
        for (int pos2 = pos1 + imageBits; pos2 < n * imageBits;
                pos2 += imageBits)
            if (((c >> pos2) & imageMask) > pImg)
                c -= (Code(1) << pos2); // decrement image at pos2
        ans *= (n - p);
        ans += pImg;
    }
    return ans;
}

template <int n>
constexpr typename Perm<n>::Index Perm<n>::SnIndex() const {
    Index ans = 0;
    bool even = true;
    Code c = code_;
    int p = 0, pos1 = 0;
    for ( ; p < n - 1; ++p, pos1 += imageBits) {
        // position pos1 holds the (p)th image
        Code pImg = (c >> pos1) & imageMask; // image at pos1
        // The following loop preserves the relative order of the images
        // at positions *after* pos1.
        for (int pos2 = pos1 + imageBits; pos2 < n * imageBits;
                pos2 += imageBits) {
            // Compare image at pos2 with image at pos1.
            if (((c >> pos2) & imageMask) > pImg)
                c -= (Code(1) << pos2); // decrement image at pos2
            else
                even = ! even; // an inversion for the sign computation
        }
        ans *= (n - p);
        ans += pImg;
        // From this point we never look at positions 0..pos1 again.
    }
    return (even == (ans % 2 == 0) ? ans : (ans ^ 1));
}

template <int n>
Perm<n> Perm<n>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

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

template <int n>
std::string Perm<n>::str() const {
    char ans[n + 1];
    for (int i = 0; i < n; ++i)
        ans[i] = regina::digit((code_ >> (imageBits * i)) & imageMask);
    ans[n] = 0;

    return ans;
}

template <int n>
std::string Perm<n>::trunc(int len) const {
    char ans[n + 1];
    for (int i = 0; i < len; ++i)
        ans[i] = regina::digit((code_ >> (imageBits * i)) & imageMask);
    ans[len] = 0;

    return ans;
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
inline Perm<n> Perm<n>::tightDecode(std::istream& input) {
    return tightDecode(std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(), false);
}

} // namespace regina

#include "maths/perm-impl.h"

#endif

