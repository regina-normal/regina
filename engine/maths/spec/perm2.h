
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

#ifndef __PERM2_H
#ifndef __DOXYGEN
#define __PERM2_H
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
 * Represents a permutation of {0,1}.
 * This is a specialisation of the generic Perm template: it is highly
 * optimised, but also somewhat trivial (since there are only two
 * possible permutations).  It is provided simply to optimise the general
 * Perm<n> template for this trivial case.
 *
 * As with all Perm template classes, these objects are small enough to
 * pass about by value instead of by reference.  Moreover, Perm<2> in
 * particular is extremely fast to work with.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  For Perm<2>, the internal code is 0 for
 * the identity permutation, or 1 for the (unique) non-identity permutation.
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
 */
template <>
class REGINA_API Perm<2> {
    public:
        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on two elements.  In other words, this is a
         * native signed integer type large enough to store (2!).
         */
        typedef int Index;

        /**
         * The total number of permutations on two elements.
         * This is the size of the array Sn.
         */
        static const Index nPerms = 2;

        /**
         * The total number of permutations on one element.
         * This is the size of the array Sn_1.
         */
        static const Index nPerms_1 = 1;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         */
        typedef uint8_t Code;

        /**
         * Contains all possible permutations of two elements.
         *
         * The identity permutation has index 0, and the non-identity
         * permutation has index 1.  As a result, S2[\a i] is an even
         * permutation if and only if \a i is even.
         *
         * For all permutation classes (Perm<2>, Perm<3> and so on),
         * the S2 array stores the same permutations in the same order
         * (but of course using different data types).
         */
        static const Perm<2> S2[2];

        /**
         * A dimension-agnostic alias for Perm<2>::S2.  In general, for
         * each \a K the class PermK will define an alias \a Sn
         * that references the list of all permutations PermK::SK.
         */
        static const Perm<2>* Sn;

        /**
         * Contains the inverses of the permutations in the array \a S2.
         *
         * Specifically, the inverse of permutation <tt>S2[i]</tt> is
         * the permutation <tt>S2[ invS2[i] ]</tt>.
         *
         * This array is provided for consistency with larger permutation
         * classes; of course, for permutations of two elements, the
         * inverse of \a p is always \a p itself.
         */
        static const unsigned invS2[2];

        /**
         * A dimension-agnostic alias for Perm<2>::invS2.  In general, for
         * each \a K the class PermK will define an alias \a invSn
         * that references the list of all permutations PermK::invSK.
         */
        static const unsigned* invSn;

        /**
         * Contains all possible permutations of two elements in
         * lexicographical order.
         *
         * This is identical to the array Perm<2>::S2, and in fact
         * \a orderedS2 and \a S2 are pointers to the same array in memory.
         * Note however that for \a n &ge; 3, the arrays Perm<n>::Sn and
         * Perm<n>::orderedSn are different: \a Sn alternates between even
         * and odd permutations, and \a orderedSn stores permutations in
         * lexicograpical order.
         */
        static const Perm<2>* orderedS2;

        /**
         * A dimension-agnostic alias for Perm<2>::orderedS2.  In general, for
         * each \a K the class PermK will define an alias \a orderedSn
         * that references the list of all permutations PermK::orderedSK.
         */
        static const Perm<2>* orderedSn;

        /**
         * Contains all possible permutations of one element.
         * In each permutation, 1 maps to 1.
         *
         * Of course, this array is trivial: it contains just the
         * identity permutation.  This array is provided for consistency
         * with larger permutation classes Perm<n>.
         *
         * Note that, as an implementation detail, the arrays \a S1 and \a S2
         * point to the same location in memory (however, they are treated as
         * arrays of different lengths).
         */
        static const Perm<2>* S1;

        /**
         * A dimension-agnostic alias for Perm<2>::S1.  In general, for
         * each \a K the class PermK will define an alias \a Sn_1
         * that references the list of all permutations PermK::S(K-1).
         */
        static const Perm<2>* Sn_1;

    private:
        Code code_;
            /**< The internal code representing this permutation. */

    public:
        /**
         * Creates the identity permutation.
         */
        Perm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre \a a and \a b are in {0,1}.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        Perm(int a, int b);

        /**
         * Creates a permutation mapping \a i to \a image[i] for each
         * \a i = 0,1.
         *
         * \pre The array \a image contains two elements, which are
         * 0 and 1 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        Perm(const int* image);

        /**
         * Creates a permutation mapping (\a a[0], \a a[1]) to
         * (\a b[0], \a b[1]) respectively.
         *
         * \pre Both arrays \a a and \a b contain two elements, which
         * are 0 and 1 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length 2.
         * @param b the corresponding array of images; this must also have
         * length 2.
         */
        Perm(const int* a, const int* b);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        Perm(const Perm<2>& cloneMe);

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
        Code permCode() const;

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
        static Perm<2> fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by permCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(Code code);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        Perm<2>& operator = (const Perm<2>& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will be <i>p o q</i>, satisfying
         * <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation with which to compose this.
         * @return the composition of both permutations.
         */
        Perm<2> operator * (const Perm<2>& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        Perm<2> inverse() const;

        /**
         * Finds the reverse of this permutation.
         *
         * Here \e reverse means that we reverse the images of 0 and 1.
         * In other words, if permutation \a q is the
         * reverse of \a p, then <tt>p[i] == q[1 - i]</tt> for all \a i.
         */
        Perm<2> reverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        int sign() const;

        /**
         * Determines the image of the given integer under this permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be 0 or 1.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be 0 or 1.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for 0 and 1.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const Perm<2>& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for at least one of 0 or 1.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const Perm<2>& other) const;

        /**
         * Lexicographically compares the images of (0,1) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const Perm<2>& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if each of 0 and 1 is mapped to itself.
         *
         * @return \c true if and only if this is the identity permutation.
         */
        bool isIdentity() const;

        /**
         * Returns the <i>i</i>th permutation on two elements, where
         * permutations are numbered lexicographically beginning at 0.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * pair (\a p[0], \a p[1]).
         *
         * The return value will be identical to orderedS2[\a i].
         *
         * @param i the lexicographical index of the permutation; this
         * must be 0 or 1.
         * @return the <i>i</i>th permutation.
         */
        static Perm atIndex(Index i);

        /**
         * Returns a random permutation on two elements.
         * All permutations are returned with equal probability.
         *
         * The implementation uses the C standard ::rand() function for its
         * random number generation.
         *
         * @return a random permutation.
         */
        static Perm rand();

        /**
         * Returns the lexicographical index of this permutation.  This
         * indicates where this permutation sits within a full lexicographical
         * ordering of all 2! permutations on two elements.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * pair (\a p[0], \a p[1]).
         * That is, the identity permutation has index 0, and the
         * (unique) non-identity permutation has index 1.
         *
         * This routine is identical to orderedS2Index().
         *
         * @return the index of this permutation, which will be 0 or 1.
         */
        Index index() const;

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
         * Returns the index of this permutation in the Perm<2>::S2 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::S2[i].  This will be 0 or 1.
         */
        int S2Index() const;

        /**
         * Returns the index of this permutation in the Perm<2>::S2 array.
         * This is a dimension-agnostic alias for S2Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::S2[i].  This will be 0 or 1.
         */
        int SnIndex() const;

        /**
         * Returns the index of this permutation in the Perm<2>::orderedS2 array.
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::orderedS2[i].  This will be 0 or 1.
         */
        int orderedS2Index() const;

        /**
         * Returns the index of this permutation in the Perm<2>::orderedS2 array.
         * This is a dimension-agnostic alias for orderedS2Index().
         *
         * @return the index \a i for which this permutation is equal to
         * Perm<2>::orderedS2[i].  This will be 0 or 1.
         */
        int orderedSnIndex() const;

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
        static Perm<2> contract(Perm<k> p);

    private:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code from which the new
         * permutation will be created.
         */
        Perm<2>(Code code);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPerm2 has now been renamed to Perm<2>.
 */
typedef Perm<2> NPerm2;

/*@}*/

// Inline functions for Perm<2>

inline Perm<2>::Perm() : code_(0) {
}

inline Perm<2>::Perm(Code code) : code_(code) {
}

inline Perm<2>::Perm(int a, int b) {
    // Transposition.
    code_ = (a == b ? 0 : 1);
}

inline Perm<2>::Perm(const int* image) {
    code_ = image[0];
}

inline Perm<2>::Perm(const int* a, const int* b) {
    code_ = (a[0] == b[0] ? 0 : 1);
}

inline Perm<2>::Perm(const Perm<2>& cloneMe) : code_(cloneMe.code_) {
}

inline Perm<2>::Code Perm<2>::permCode() const {
    return code_;
}

inline void Perm<2>::setPermCode(Code code) {
    code_ = code;
}

inline Perm<2> Perm<2>::fromPermCode(Code code) {
    return Perm<2>(code);
}

inline bool Perm<2>::isPermCode(Code code) {
    // code >= 0 is a no-op because we are using an unsigned data type.
    return (code < 2);
}

inline Perm<2>& Perm<2>::operator = (const Perm<2>& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

inline Perm<2> Perm<2>::operator * (const Perm<2>& q) const {
    return Perm<2>(code_ ^ q.code_);
}

inline Perm<2> Perm<2>::inverse() const {
    return *this;
}

inline Perm<2> Perm<2>::reverse() const {
    return Perm<2>(static_cast<Code>(1 - code_));
}

inline int Perm<2>::sign() const {
    return (code_ ? -1 : 1);
}

inline int Perm<2>::operator[](int source) const {
    return source ^ code_;
}

inline int Perm<2>::preImageOf(int image) const {
    return image ^ code_;
}

inline bool Perm<2>::operator == (const Perm<2>& other) const {
    return (code_ == other.code_);
}

inline bool Perm<2>::operator != (const Perm<2>& other) const {
    return (code_ != other.code_);
}

inline int Perm<2>::compareWith(const Perm<2>& other) const {
    // For n=2, permutation codes respect lexicographical order.
    return (code_ == other.code_ ? 0 : code_ < other.code_ ? -1 : 1);
}

inline bool Perm<2>::isIdentity() const {
    return (code_ == 0);
}

inline Perm<2> Perm<2>::atIndex(Index i) {
    return Perm<2>(static_cast<Code>(i));
}

inline Perm<2>::Index Perm<2>::index() const {
    return code_;
}

inline Perm<2> Perm<2>::rand() {
    return Perm<2>(static_cast<Code>(::rand() % 2));
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

inline int Perm<2>::S2Index() const {
    return code_;
}

inline int Perm<2>::SnIndex() const {
    return code_;
}

inline int Perm<2>::orderedS2Index() const {
    return code_;
}

inline int Perm<2>::orderedSnIndex() const {
    return code_;
}

} // namespace regina

#endif

