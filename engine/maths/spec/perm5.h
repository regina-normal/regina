
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
 * objects to and from the engine.  For Perm<5>, the internal permutation
 * codes have changed as of Regina 6.1:
 *
 * - \e First-generation codes were used internally in Regina 6.0 and earlier.
 *   These codes were characters whose lowest three bits represented the
 *   image of 0, whose next lowest three bits represented the image of 1,
 *   and so on.  The routines permCode(), setPermCode(), fromPermCode()
 *   and isPermCode() continue to work with first-generation codes for
 *   backward compatibility.  Likewise, the XML data file format
 *   continues to use first-generation codes to describe pentachoron gluings.
 *
 * - \e Second-generation codes are used internally in Regina 6.1 and above.
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
 * \ifacespython Since Python does not support templates, this class is
 * made available under the name Perm5.
 */
template <>
class REGINA_API Perm<5> {
    private:
        /**
         * An array-like object used to implement Perm<5>::S5.
         */
        struct S5Lookup {
            /**
             * Returns the permutation at the given index in the array S5.
             * See Perm<5>::S5 for details.
             *
             * @param index an index between 0 and 119 inclusive.
             * @return the corresponding permutation in S5.
             */
            Perm<5> operator[] (int index) const;
        };

        /**
         * An array-like object used to implement Perm<5>::orderedS5.
         */
        struct OrderedS5Lookup {
            /**
             * Returns the permutation at the given index in the array
             * orderedS5.  See Perm<5>::orderedS5 for details.
             *
             * @param index an index between 0 and 119 inclusive.
             * @return the corresponding permutation in orderedS5.
             */
            Perm<5> operator[] (int index) const;
        };

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
         * Indicates the number of bits used by a first-generation
         * permutation code to store the image of a single integer.
         *
         * The full first-generation code packs 5 such images together, and
         * so uses 5 * \a imageBits bits in total.
         */
        static const int imageBits = 3;

        /**
         * Indicates the native unsigned integer type used to store a
         * first-generation permutation code.
         */
        typedef uint16_t Code;

        /**
         * Indicates the native unsigned integer type used to store a
         * second-generation permutation code.
         */
        typedef uint8_t Code2;

        /**
         * Gives array-like access to all possible permutations of
         * five elements.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>S5[i]</tt>.  The index \a i must be
         * between 0 and 119 inclusive.
         *
         * In Regina 6.0 and earlier, this was a hard-coded C-style array;
         * since Regina 6.1 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         *
         * The permutations with even indices in the array are the even
         * permutations, and those with odd indices in the array are the
         * odd permutations.
         *
         * Note that the permutations are not necessarily in
         * lexicographical order.
         */
        static constexpr S5Lookup S5 {};

        /**
         * A dimension-agnostic alias for Perm<5>::S5.  In general, for
         * each \a K the class PermK will define an alias \a Sn
         * that references the list of all permutations PermK::SK.
         */
        static constexpr S5Lookup Sn {};

        /**
         * Gives array-like access to all possible permutations of five
         * elements in lexicographical order.
         *
         * To access the permutation at index \a i, you simply use the
         * square bracket operator: <tt>orderedS5[i]</tt>.  The index \a i
         * must be between 0 and 119 inclusive.
         *
         * In Regina 6.0 and earlier, this was a hard-coded C-style array;
         * since Regina 6.1 it has changed type, but accessing elements as
         * described above remains extremely fast.  The object that is returned
         * is lightweight and is defined in the headers only; in particular,
         * you cannot make a reference to it (but you can always make a copy).
         */
        static constexpr OrderedS5Lookup orderedS5 {};

        /**
         * A dimension-agnostic alias for Perm<5>::orderedS5.  In general, for
         * each \a K the class PermK will define an alias \a orderedSn
         * that references the list of all permutations PermK::orderedSK.
         */
        static constexpr OrderedS5Lookup orderedSn {};

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
         * lexicographical order.
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
         * lexicographical order.
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
        Code2 code2_;
            /**< The internal second-generation permutation code
                 representing this permutation. */

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
        Perm(const Perm<5>& cloneMe) = default;

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine permCode2() for details.
         *
         * @return the first-generation permutation code.
         */
        Code permCode() const;

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
         * @return the second-generation permutation code.
         */
        Code2 permCode2() const;

        /**
         * Sets this permutation to that represented by the given
         * first-generation permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine setPermCode2() for details.
         *
         * @param code the first-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode(Code code);

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
         * @param code the second-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode2(Code2 code);

        /**
         * Creates a permutation from the given first-generation
         * permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static Perm<5> fromPermCode(Code code);

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
         * @param code the second-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static Perm<5> fromPermCode2(Code2 code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<5> now uses second-generation codes internally.
         * See the class notes and the routine isPermCode2() for details.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * first-generation permutation code.
         */
        static bool isPermCode(Code code);

        /**
         * Determines whether the given character is a valid second-generation
         * permutation code.  Valid second-generation codes can be passed to
         * setPermCode2() or fromPermCode2(), and are returned by permCode2().
         *
         * Second-generation codes are fast to work with, since they are
         * used internally by the Perm<5> class.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static bool isPermCode2(Code2 code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<5>& operator = (const Perm<5>& cloneMe) = default;

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
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S5[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
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
         * Specifically, the inverse of permutation <tt>S5[i]</tt> is
         * the permutation <tt>S5[ invS5[i] ]</tt>.
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
         * Specifically, the product <tt>S5[x] * S5[y]</tt> is the
         * permutation <tt>S5[product[x][y]]</tt>.
         *
         * This table contains 14.4 kilobytes of data, and so as of the year
         * 2021 we declare this is a perfectly reasonably memory cost for the
         * speed-ups that a hard-coded multiplication table gives us.
         */
        static const Code2 productTable[120][120];

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S5[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static constexpr Code2 swapTable[5][5] = {
            // Generated using Regina 6.0.
            { 0, 25, 55, 81, 105 },
            { 25, 0, 7, 15, 21 },
            { 55, 7, 0, 3, 5 },
            { 81, 15, 3, 0, 1 },
            { 105, 21, 5, 1, 0 }
        };

    private:
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
        Perm<5>(Code2 code);

        /**
         * Returns the index into the Perm<5>::S5 array of the permutation that
         * maps (0,...,5) to (<i>a</i>,...,<i>e</i>) respectively.
         *
         * \pre {<i>a</i>,<i>b</i>,<i>c</i>,<i>d</i>,<i>e</i>} = {0,1,2,3,4}.
         *
         * @param a the desired image of 0.
         * @param b the desired image of 1.
         * @param c the desired image of 2.
         * @param d the desired image of 3.
         * @param e the desired image of 4.
         * @return the index \a i for which the given permutation is equal to
         * Perm<5>::S5[i].  This will be between 0 and 119 inclusive.
         */
        REGINA_INLINE_REQUIRED
        static int S5Index(int a, int b, int c, int d, int e);

        /**
         * Converts between an index into Perm<5>::S5 and an index into
         * Perm<5>::orderedS5.  This conversion works in either direction.
         *
         * \tparam Int a native integer type; this would typically be
         * either \c int or \a Code2.
         */
        template <typename Int>
        static Int convOrderedUnordered(Int index);
};

/*@}*/

// Inline functions for Perm<5>

template <typename Int>
inline Int Perm<5>::convOrderedUnordered(Int index) {
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

inline Perm<5> Perm<5>::S5Lookup::operator[] (int index) const {
    return Perm<5>(static_cast<Code2>(index));
}

inline Perm<5> Perm<5>::OrderedS5Lookup::operator[] (int index) const {
    return Perm<5>(static_cast<Code2>(convOrderedUnordered(index)));
}

inline Perm<5>::Perm() : code2_(0) {
}

inline Perm<5>::Perm(Code2 code) : code2_(code) {
}

inline Perm<5>::Perm(int a, int b) : code2_(swapTable[a][b]) {
}

inline Perm<5>::Perm(int a, int b, int c, int d, int e) :
        code2_(static_cast<Code2>(S5Index(a, b, c, d, e))) {
}

inline Perm<5>::Perm(const int* image) :
        code2_(static_cast<Code2>(S5Index(
            image[0], image[1], image[2], image[3], image[4]))) {
}

inline Perm<5>::Code Perm<5>::permCode() const {
    return static_cast<Code>(
        imageTable[code2_][0] |
        (imageTable[code2_][1] << 3) |
        (imageTable[code2_][2] << 6) |
        (imageTable[code2_][3] << 9) |
        (imageTable[code2_][4] << 12));
}

inline Perm<5>::Code2 Perm<5>::permCode2() const {
    return code2_;
}

inline void Perm<5>::setPermCode(Code code) {
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

inline Perm<5> Perm<5>::fromPermCode(Code code) {
    return Perm<5>(static_cast<Code2>(S5Index(
        code & 0x07,
        (code >> 3) & 0x07,
        (code >> 6) & 0x07,
        (code >> 9) & 0x07,
        (code >> 12) & 0x07)));
}

inline Perm<5> Perm<5>::fromPermCode2(Code2 code) {
    return Perm<5>(code);
}

inline bool Perm<5>::isPermCode(Code code) {
    unsigned mask = 0;
    for (int i = 0; i < 5; i++)
        mask |= (1 << ((code >> (3 * i)) & 7));
            // mask |= (1 << imageOf(i));
    return (mask == 31);
}

inline bool Perm<5>::isPermCode2(Code2 code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 120);
}

inline Perm<5> Perm<5>::operator *(const Perm<5>& q) const {
    return Perm<5>(productTable[code2_][q.code2_]);
}

inline Perm<5> Perm<5>::inverse() const {
    return Perm<5>(invS5[code2_]);
}

inline Perm<5> Perm<5>::reverse() const {
    // p becomes p * 43210 (which has second-generation code 118).
    return Perm<5>(productTable[code2_][118]);
}

inline int Perm<5>::sign() const {
    return (code2_ % 2 ? -1 : 1);
}

inline int Perm<5>::operator[](int source) const {
    return imageTable[code2_][source];
}

inline int Perm<5>::preImageOf(int image) const {
    return imageTable[invS5[code2_]][image];
}

inline bool Perm<5>::operator == (const Perm<5>& other) const {
    return (code2_ == other.code2_);
}

inline bool Perm<5>::operator != (const Perm<5>& other) const {
    return (code2_ != other.code2_);
}

inline int Perm<5>::compareWith(const Perm<5>& other) const {
    // Computing orderedS5Index() is very fast, now that we use S5 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS5Index();
    int o2 = other.orderedS5Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline bool Perm<5>::isIdentity() const {
    return (code2_ == 0);
}

inline Perm<5> Perm<5>::atIndex(Index i) {
    return orderedS5[i];
}

inline Perm<5>::Index Perm<5>::index() const {
    return orderedS5Index();
}

inline Perm<5> Perm<5>::rand(bool even) {
    RandomEngine engine;
    return rand(engine.engine(), even);
}

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

inline int Perm<5>::S5Index() const {
    return code2_;
}

inline int Perm<5>::SnIndex() const {
    return code2_;
}

inline int Perm<5>::orderedS5Index() const {
    return convOrderedUnordered(code2_);
}

inline int Perm<5>::S5Index(int a, int b, int c, int d, int e) {
    // First compute the ordered S5 index.
    int ans = 24 * a +
              6 * (b - (b > a ? 1 : 0)) +
              2 * (c - ((c > b ? 1 : 0) + (c > a ? 1 : 0))) +
                  (d > e ? 1 : 0);

    // Then switch to the plain (unordered) S5 index.
    return convOrderedUnordered(ans);
}

inline int Perm<5>::orderedSnIndex() const {
    return orderedS5Index();
}

} // namespace regina

#endif

