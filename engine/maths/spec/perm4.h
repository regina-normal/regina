
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __PERM4_H
#ifndef __DOXYGEN
#define __PERM4_H
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
 * Represents a permutation of {0,1,2,3}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, and also offers some additional functionality.
 * Amongst other things, this permutation class is used to specify how
 * simplices of a 3-manifold triangulation are glued together.
 *
 * As with all Perm template classes, these objects are small enough to
 * pass about by value instead of by reference.  Moreover, Perm<4>
 * is extremely fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<4>, the internal permutation
 * codes have changed as of Regina 4.6.1:
 *
 * - \e First-generation codes were used internally in Regina 4.6 and earlier.
 *   These codes were characters whose lowest two bits represented the
 *   image of 0, whose next lowest two bits represented the image of 1,
 *   and so on.  The routines permCode(), setPermCode(), fromPermCode()
 *   and isPermCode() continue to work with first-generation codes for
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
 */
template <>
class REGINA_API Perm<4> {
    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on four elements.  In other words, this is a
         * native signed integer type large enough to store (4!).
         */
        typedef int Index;

        /**
         * The total number of permutations on four elements.
         * This is the size of the array Sn.
         */
        static const Index nPerms = 24;

        /**
         * The total number of permutations on three elements.
         * This is the size of the array Sn_1.
         */
        static const Index nPerms_1 = 6;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        typedef uint8_t Code;

        /**
         * Contains all possible permutations of four elements.
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
        static const Perm<4> S4[24];

        /**
         * A dimension-agnostic alias for Perm<4>::S4.  In general, for
         * each \a K the class PermK will define an alias \a Sn
         * that references the list of all permutations PermK::SK.
         */
        static const Perm<4>* Sn;

        /**
         * Contains the inverses of the permutations in the array \a S4.
         *
         * Specifically, the inverse of permutation <tt>S4[i]</tt> is
         * the permutation <tt>S4[ invS4[i] ]</tt>.
         */
        static const unsigned invS4[24];

        /**
         * A dimension-agnostic alias for Perm<4>::invS4.  In general, for
         * each \a K the class PermK will define an alias \a invSn
         * that references the list of all permutations PermK::invSK.
         */
        static const unsigned* invSn;

        /**
         * Contains all possible permutations of four elements in
         * lexicographical order.
         */
        static const Perm<4> orderedS4[24];

        /**
         * A dimension-agnostic alias for Perm<4>::orderedS4.  In general, for
         * each \a K the class PermK will define an alias \a orderedSn
         * that references the list of all permutations PermK::orderedSK.
         */
        static const Perm<4>* orderedSn;

        /**
         * Contains all possible permutations of three elements.
         * In each permutation, 3 maps to 3.
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
         * lexicographical order.  For the corresponding inverse array,
         * see Perm<3>::invS3.
         */
        static const Perm<4> S3[6];

        /**
         * A dimension-agnostic alias for Perm<4>::S3.  In general, for
         * each \a K the class PermK will define an alias \a Sn_1
         * that references the list of all permutations PermK::S(K-1).
         */
        static const Perm<4>* Sn_1;

        /**
         * Contains all possible permutations of three elements in
         * lexicographical order.  In each permutation, 3 maps to 3.
         */
        static const Perm<4> orderedS3[6];

        /**
         * Contains all possible permutations of two elements.
         * In each permutation, 2 maps to 2 and 3 maps to 3.
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
        static const Perm<4> S2[2];

    private:
        Code code_;
            /**< The internal code representing this permutation. */

        /**
         * Contains the reverses of the permutations in the array \a S4.
         *
         * Specifically, the reverse of permutation <tt>S4[i]</tt> is
         * the permutation <tt>S4[ revS4[i] ]</tt>.
         *
         * See reverse() for details on what "reverse" means.
         */
        static const unsigned revS4[24];

    public:
        /**
         * Creates the identity permutation.
         */
        Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1,2,3}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        Perm(int a, int b);

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
        Perm(int a, int b, int c, int d);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1,2,3.
         *
         * \pre The array \a image contains four elements, which are
         * 0, 1, 2 and 3 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        Perm(const int* image);

        /**
         * Creates a permutation mapping (\a a[0], ..., \a a[3]) to
         * (\a b[0], ..., \a b[3]) respectively.
         *
         * \pre Both arrays \a a and \a b contain 4 elements, which
         * are 0,...,3 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length 4.
         * @param b the corresponding array of images; this must also have
         * length 4.
         */
        Perm(const int* a, const int* b);

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
        Perm(int a0, int a1, int b0, int b1, int c0, int c1, int d0, int d1);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        Perm(const Perm<4>& cloneMe);

        /**
         * Returns the first-generation code representing this permutation.
         * This code is sufficient to reproduce the entire permutation.
         *
         * The code returned will be a valid first-generation permutation
         * code as determined by isPermCode().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
         *
         * @return the second-generation permutation code.
         */
        Code permCode2() const;

        /**
         * Sets this permutation to that represented by the given
         * first-generation permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
         *
         * \pre the given code is a valid second-generation permutation code;
         * see isPermCode2() for details.
         *
         * @param code the second-generation code that will determine the
         * new value of this permutation.
         */
        void setPermCode2(Code code);

        /**
         * Creates a permutation from the given first-generation
         * permutation code.
         *
         * \pre the given code is a valid first-generation permutation code;
         * see isPermCode() for details.
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
         * See the class notes and the routine fromPermCode2() for details.
         *
         * @param code the first-generation code for the new permutation.
         * @return the permutation represented by the given code.
         */
        static Perm<4> fromPermCode(Code code);

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
        static Perm<4> fromPermCode2(Code code);

        /**
         * Determines whether the given character is a valid first-generation
         * permutation code.  Valid first-generation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * \warning This routine will incur additional overhead, since
         * Perm<4> now uses second-generation codes internally.
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
         * used internally by the Perm<4> class.
         *
         * @param code the permutation code to test.
         * @return \c true if and only if the given code is a valid
         * second-generation permutation code.
         */
        static bool isPermCode2(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<4>& operator = (const Perm<4>& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        Perm<4> operator *(const Perm<4>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        Perm<4> inverse() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0,...,3.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[3 - i]</tt> for all \a i.
         */
        Perm<4> reverse() const;

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
         * should be between 0 and 3 inclusive.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and 3 inclusive.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0, 1, 2 and 3.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const Perm<4>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0, 1, 2 or 3.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const Perm<4>& other) const;

        /**
         * Lexicographically compares the images of (0,1,2,3) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const Perm<4>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0, 1, 2 and 3 is
         * mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        bool isIdentity() const;

        /**
         * Returns the <i>i</i>th permutation on four elements, where
         * permutations are numbered lexicographically beginning at 0.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 4-tuple (\a p[0], \a p[1], \a p[2], \a p[3]).
         *
         * The return value will be identical to orderedS4[\a i].
         *
         * @param i the lexicographical index of the permutation; this
         * must be between 0 and 23 inclusive.
         * @return the <i>i</i>th permutation.
         */
        static Perm atIndex(Index i);

        /**
         * Returns the lexicographical index of this permutation.  This
         * indicates where this permutation sits within a full lexicographical
         * ordering of all 4! permutations on four elements.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * 4-tuple (\a p[0], \a p[1], \a p[2], \a p[3]).
         * In particular, the identity permutation has index 0, and the
         * "reverse" permutation (which maps each \a i to 3-<i>i</i>)
         * has index 23 = 4!-1.
         *
         * This routine is identical to orderedS4Index().
         *
         * @return the index of this permutation, which will be between
         * 0 and 23 inclusive.
         */
        Index index() const;

        /**
         * Returns a random permutation on four elements.
         * All permutations are returned with equal probability.
         *
         * The implementation uses the C standard ::rand() function for its
         * random number generation.
         *
         * @return a random permutation.
         */
        static Perm rand();

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
         * Returns the index of this permutation in the Perm<4>::S4 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::S4[i].  This will be between 0 and 23 inclusive.
         */
        int S4Index() const;

        /**
         * Returns the index of this permutation in the Perm<4>::S4 array.
         * This is a dimension-agnostic alias for S4Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::S4[i].  This will be between 0 and 23 inclusive.
         */
        int SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<4>::orderedS4 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::orderedS4[i].  This will be between 0 and 23 inclusive.
         */
        REGINA_INLINE_REQUIRED
        int orderedS4Index() const;

        /**
         * Returns the index of this permutation in the Perm<4>::orderedS4 array.
         * This is a dimension-agnostic alias for orderedS4Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<4>::orderedS4[i].  This will be between 0 and 23 inclusive.
         */
        int orderedSnIndex() const;

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
        static Perm<4> extend(Perm<k> p);

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
        static Perm<4> contract(Perm<k> p);

    private:
        /**
         * Contains the images of every element under every possible
         * permutation.
         *
         * Specifically, the image of \a x under the permutation <tt>S4[i]</tt>
         * is <tt>imageTable[i][x]</tt>.
         */
        static const Code imageTable[24][4];

        /**
         * Contains the full multiplication table for all possible
         * permutations.
         *
         * Specifically, the product <tt>S4[x] * S4[y]</tt> is the
         * permutation <tt>S4[product[x][y]]</tt>.
         */
        static const Code productTable[24][24];

        /**
         * Contains a full table of two-element swaps.
         *
         * Specifically, the permutation that swaps \a x and \a y is
         * <tt>S4[swapTable[x][y]]</tt>.  Here \a x and \a y may be equal.
         */
        static const Code swapTable[4][4];

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
        Perm<4>(Code code);

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
        REGINA_INLINE_REQUIRED
        static int S4Index(int a, int b, int c, int d);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPerm4 has now been renamed to Perm<4>.
 */
typedef Perm<4> NPerm4;

// Routines for constructing the permutations associated to
// triangles and edges of the triangulation

/*@}*/

// Inline functions for Perm<4>

inline Perm<4>::Perm() : code_(0) {
}

inline Perm<4>::Perm(Code code) : code_(code) {
}

inline Perm<4>::Perm(int a, int b) : code_(swapTable[a][b]) {
}

inline Perm<4>::Perm(int a, int b, int c, int d) :
        code_(static_cast<Code>(S4Index(a, b, c, d))) {
}

inline Perm<4>::Perm(const int* image) :
        code_(static_cast<Code>(S4Index(
        image[0], image[1], image[2], image[3]))) {
}

inline Perm<4>::Perm(const Perm<4>& cloneMe) : code_(cloneMe.code_) {
}

inline Perm<4>::Code Perm<4>::permCode() const {
    return static_cast<Code>(
        imageTable[code_][0] |
        (imageTable[code_][1] << 2) |
        (imageTable[code_][2] << 4) |
        (imageTable[code_][3] << 6));
}

inline Perm<4>::Code Perm<4>::permCode2() const {
    return code_;
}

inline void Perm<4>::setPermCode(Code code) {
    code_ = static_cast<Code>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03));
}

inline void Perm<4>::setPermCode2(Code code) {
    code_ = code;
}

inline Perm<4> Perm<4>::fromPermCode(Code code) {
    return Perm<4>(static_cast<Code>(S4Index(
        code & 0x03,
        (code >> 2) & 0x03,
        (code >> 4) & 0x03,
        (code >> 6) & 0x03)));
}

inline Perm<4> Perm<4>::fromPermCode2(Code code) {
    return Perm<4>(code);
}

inline bool Perm<4>::isPermCode2(Code code) {
    // code >= 0 is automatic because we are using an unsigned data type.
    return (code < 24);
}

inline Perm<4>& Perm<4>::operator = (const Perm<4>& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

inline Perm<4> Perm<4>::operator *(const Perm<4>& q) const {
    return Perm<4>(productTable[code_][q.code_]);
}

inline Perm<4> Perm<4>::inverse() const {
    return Perm<4>(static_cast<Code>(invS4[code_]));
}

inline Perm<4> Perm<4>::reverse() const {
    return Perm<4>(static_cast<Code>(revS4[code_]));
}

inline int Perm<4>::sign() const {
    return (code_ % 2 ? -1 : 1);
}

inline int Perm<4>::operator[](int source) const {
    return imageTable[code_][source];
}

inline int Perm<4>::preImageOf(int image) const {
    return imageTable[invS4[code_]][image];
}

inline int Perm<4>::compareWith(const Perm<4>& other) const {
    // Computing orderedS4Index() is very fast, now that we use S4 indices
    // for internal permutation codes.  Use this instead of comparing images
    // one at a time.
    int o1 = orderedS4Index();
    int o2 = other.orderedS4Index();
    return (o1 == o2 ? 0 : o1 < o2 ? -1 : 1);
}

inline bool Perm<4>::isIdentity() const {
    return (code_ == 0);
}

inline Perm<4> Perm<4>::atIndex(Index i) {
    return orderedS4[i];
}

inline Perm<4>::Index Perm<4>::index() const {
    return orderedS4Index();
}

inline Perm<4> Perm<4>::rand() {
    return S4[::rand() % 24];
}

inline bool Perm<4>::operator == (const Perm<4>& other) const {
    return (code_ == other.code_);
}

inline bool Perm<4>::operator != (const Perm<4>& other) const {
    return (code_ != other.code_);
}

inline int Perm<4>::S4Index() const {
    return code_;
}

inline int Perm<4>::orderedS4Index() const {
    // S4 is almost the same as orderedS4, except that some pairs
    // S4[2i] <--> S4[2i+1] have been swapped to ensure that all
    // permutations S4[2i] are even and all permutations S4[2i+1] are odd.
    //
    // Specifically, we must interchange all pairs 4i+2 <--> 4i+3.
    return ((code_ & 2) ? (code_ ^ 1) : code_);
}

inline int Perm<4>::orderedSnIndex() const {
    return orderedS4Index();
}

inline int Perm<4>::S4Index(int a, int b, int c, int d) {
    int orderedS4Index = 6 * a +
                         2 * (b - (b > a ? 1 : 0)) +
                             (c > d ? 1 : 0);

    // As above, to obtain an S4 index, interchange all pairs 4i+2 <--> 4i+3.
    return ((orderedS4Index & 2) ? (orderedS4Index ^ 1) : orderedS4Index);
}

inline int Perm<4>::SnIndex() const {
    return S4Index();
}

} // namespace regina

#endif

