
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file maths/permsn.h
 *  \brief Provides lookup and iteration over all permutations in `S_n`.
 *
 *  This file is automatically included from perm.h; there is no need for
 *  end users to include this header explicitly.
 */

// We include perm.h before the header guard, to ensure that the
// various permutation headers are processed in exactly the right order.
// This header will be re-included within perm.h.
#include "maths/perm.h"

#ifndef __REGINA_PERMSN_H
#ifndef __DOXYGEN
#define __REGINA_PERMSN_H
#endif

namespace regina {

template <int n> class Perm;

/**
 * A lightweight array-like object that supports fast lookup and iteration
 * for permutations on \a n objects.
 *
 * Typically you would access this object through static constants such as
 * `Perm<n>::Sn` or `Perm<n>::orderedSn`.  There should be no need for end users
 * to refer to this type directly.
 *
 * There are two main ways in which you can use this object.  For the examples
 * below, we assume that we are accessing `Perm<n>::Sn`.
 *
 * - Array-style lookup.  Here you would use accessors such as
 *   `Perm<n>::Sn[i]` and `Perm<n>::Sn::size()`.  Instead of size(), you can
 *   also use the standard `len()` function in Python.
 *
 * - Iteration.  Here you would typically iterate over `Perm<n>::Sn` in a
 *   range-based \c for loop, or use begin/end pairs such as
 *   `Perm<n>::Sn.begin()` and `Perm<n>::Sn.end()`.
 *
 * Regarding indices and iteration:
 *
 * - Indices are between 0 and `(n!-1)` inclusive, and permutations are indexed
 *   according to the chosen ordering, i.e., the template parameter \a order.
 *   In particular: `Perm<n>::Sn` uses sign-based ordering, beginning with the
 *   identity permutation at index 0 and alternating between even and odd
 *   permutations, whereas `Perm<n>::orderedSn` uses lexicographical ordering
 *   according the images of `0,...,n-1` under each permutation.
 *
 * - The order of iteration is the same as the order used for indexing.
 *
 * - Iterating directly over this object (e.g., iterating over `Perm<n>::Sn`)
 *   is typically faster than using array-like access for each index in turn
 *   (e.g., accessing `Sn[0]`, `Sn[1]`, etc.).  This is particularly true when
 *   \a n is larger.  See the notes on time complexity below.
 *
 * Regarding time complexity:
 *
 * - For \a n ≤ 7, iteration steps and index-based lookup are both extremely
 *   fast constant time.  Iterators are random-access (and satisfy all of the
 *   expected time complexity constraints that come with this).
 *
 * - For \a n ≥ 8, the time for a single iteration step is in linear in \a n,
 *   and index-based lookup is currently _quadratic_ in \a n.  Iterators are
 *   merely forward iterators, not random access.
 *
 * Objects of this type contain no data at all, which means they are trivial to
 * pass by value or swap with std::swap(), and all objects of this type are
 * essentially identical.
 *
 * \python Python does not support templates.  Instead this class can be
 * accessed by appending \a n and \a order as suffixes (e.g., PermSn3_Sign,
 * or PermSn5_Lex).
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 * \tparam order the way in which this class orders permutations for the
 * purposes of indexing and iteration.
 * \tparam codeType the constant `Perm<n>::codeType`.  You should allow the
 * compiler to deduce this and not attempt to set it yourself.
 *
 * \ingroup maths
 */
template <int n, PermOrder order, PermCodeType codeType = Perm<n>::codeType>
struct PermSn {
    // This generic implementation is for n ≥ 8 (where codeType is Images).
    // There is a specialisation below for n ≤ 7.
    static_assert(Perm<n>::codeType == PermCodeType::Images,
        "The generic implementation of PermSn<n, order> should only be used "
        "for larger n, where permutations are stored using image packs.");

    /**
     * An iterator over all permutations of \a n objects.
     *
     * See the PermSn class notes for further details on how iteration works.
     * In particular:
     *
     * - For smaller permutations (\a n ≤ 7), these iterators are random-access,
     *   and all of the expected operations are fast constant time.
     *
     * - For larger permutations (\a n ≥ 8), these are forward iterators only,
     *   and a single forward step takes time _linear_ in \a n.
     *
     * Unlike most iterator types, the dereference operator for this
     * iterator type returns by value, not by reference.  This is because
     * the individual permutations are generated, not stored.
     *
     * For most iterator classes, Regina now uses specialisations of
     * std::iterator_traits to provide access to their associated types
     * (e.g., value_type).  However, this is not possible for
     * PermSn::iterator since PermSn is templated.  Therefore,
     * for PermSn::iterator, we continue to provide these associated
     * types directly as class members.
     *
     * Both \a iterator and \a const_iterator are the same type, since
     * PermSn only offers read-only access to its permutations.
     *
     * The full interface for the iterator type is not documented here, since
     * it changes according to both \a n and your programming language:
     *
     * - In C++, this class implements the full interface for either a random
     *   access iterator (\a n ≤ 7) or a forward iterator (\a n ≥ 8).  Using
     *   `Perm<n>::Sn` as an example, you would typically access iterators
     *   either via `Perm<n>::Sn.begin()` and `Perm<n>::Sn.end()`, or by using
     *   a range-based \c for loop over `Perm<n>::Sn`.
     *
     * - In Python, this class and PermSn together implement the expected
     *   interface for Python iterators.  Using `Perm4.Sn` as an example, you
     *   would typically create and use iterators by iterating over `Perm4.Sn`,
     *   which internally uses `PermSn4_Sign.__iter__()` and
     *   `PermSn4_Sign.iterator.__next__()`.
     */
    class iterator {
        public:
            using value_type = Perm<n>;
                /**< Indicates what type the iterator points to. */
            using iterator_category = std::forward_iterator_tag;
                /**< Declares the type of this iterator.  For `n ≤ 7`
                     this will be `std::random_access_iterator_tag`, and for
                     `n ≥ 8` this will be `std::forward_iterator_tag`. */
            using difference_type = typename Perm<n>::Index;
                /**< The type obtained by subtracting iterators. */
            using pointer = const value_type*;
                /**< A pointer to \a value_type. */
            using reference = Perm<n>;
                /**< The type returned by the dereference operator, which for
                     this iterator type is by value.  This is because
                     permutations are generated, not stored. */

        private:
            Perm<n> perm_;
                /**< The permutation that this iterator points to, or the
                     identity permutation if this iterator is past-the-end. */
            bool valid_;
                /**< \c true if this iterator is dereferencable, or \c false
                     if this iterator is past-the-end. */

        public:
            /**
             * Creates a begin iterator.
             *
             * This iterator will point to the identity permutation.
             * So, for example, if you are iterating over `Perm<n>::Sn`, then
             * this iterator will be equal to `Perm<n>::Sn.begin()`.
             */
            constexpr iterator() : perm_(), valid_(true) {}
            /**
             * Creates either a begin or end iterator.
             *
             * If no arguments are given or if \a valid is \c true, then this
             * iterator will point to the identity permutation.  For example,
             * if you are iterating over `Perm<n>::Sn`, then this iterator will
             * be equal to `Perm<n>::Sn.begin()`.
             *
             * If \a valid is \c false, then this iterator will be past-the-end.
             * For example, if you are iterating over `Perm<n>::Sn`, then this
             * iterator will be equal to `Perm<n>::Sn.end()`.
             *
             * \param valid \c true if this should be a begin iterator, or
             * \c false if this should be an end iterator.
             */
            constexpr iterator(bool valid) : perm_(), valid_(valid) {}
            /**
             * Creates a copy of the given iterator.
             *
             * \nopython The only way to create an iterator is to iterate over
             * an object such as `Perm<n>::Sn` or `Perm<n>::orderedSn`.
             */
            constexpr iterator(const iterator&) = default;
            /**
             * Makes this a copy of the given iterator.
             *
             * \return a reference to this iterator.
             */
            constexpr iterator& operator = (const iterator&) = default;
            /**
             * Compares this with the given iterator for equality.
             *
             * To be considered equal, two iterators must both be
             * dereferencable and pointing to the same permutation, or must
             * both be past-the-end.
             *
             * \return \c true if and only if the two iterators are equal.
             */
            constexpr bool operator == (const iterator&) const = default;

            /**
             * Identifies whether this iterator is dereferenceable.
             *
             * \nopython For Python users, this class implements the Python
             * iterator interface instead.  See __next__() for details.
             *
             * \return \c true if and only if this is dereferenceable
             * (i.e., not past-the-end).
             */
            constexpr operator bool() const {
                return valid_;
            }

            /**
             * Returns the permutation that this iterator is currently
             * pointing to.
             *
             * \pre This iterator is dereferenceable (in particular, it is not
             * past-the-end).
             *
             * \nopython For Python users, this class implements the Python
             * iterator interface instead.  See __next__() for details.
             *
             * \return the corresponding permutation.
             */
            constexpr Perm<n> operator * () const {
                return perm_;
            }

            /**
             * The preincrement operator.
             *
             * \nopython For Python users, this class implements the Python
             * iterator interface instead.  See __next__() for details.
             *
             * \return a reference to this iterator after the increment.
             */
            constexpr iterator& operator ++ () {
                if constexpr (order == PermOrder::Sign)
                    ++perm_;
                else
                    perm_.lexInc();
                if (perm_.isIdentity())
                    valid_ = false;
                return *this;
            }

            /**
             * The postincrement operator.
             *
             * \nopython For Python users, this class implements the Python
             * iterator interface instead.  See __next__() for details.
             *
             * \return a copy of this iterator before the increment took place.
             */
            constexpr iterator operator ++ (int) {
                if constexpr (order == PermOrder::Sign) {
                    Perm<n> ans = perm_++;
                    if (perm_.isIdentity())
                        valid_ = false;
                    return ans;
                } else {
                    Perm<n> ans = perm_;
                    perm_.lexInc();
                    if (perm_.isIdentity())
                        valid_ = false;
                    return ans;
                }
            }

#ifdef __APIDOCS
            /**
             * Returns the current permutation and increments this iterator.
             *
             * \nocpp For C++ users, this iterator class provides the usual
             * preincrement, postincrement and dereferencing operators
             * (++ and *) instead.
             *
             * \exception StopIteration The iterator is already past-the-end
             * when this function is called.
             *
             * \return the permutation that this iterator is pointing to,
             * before the increment takes place.
             */
            Perm<n> __next__();
#endif

        private:
            /**
             * Creates a new iterator pointing to the given permutation.
             * This iterator will not be past-the-end.
             */
            constexpr iterator(Perm<n> perm) : perm_(perm), valid_(true) {}
    };

    /**
     * An iterator over all permutations of \a n objects.
     *
     * Both \a iterator and \a const_iterator are the same type, since PermSn
     * only offers read-only access to its permutations.  See the
     * PermSn::iterator class for further details.
     */
    using const_iterator = iterator;

    /**
     * Returns the permutation at the given index in \a Sn, according to the
     * chosen ordering.
     *
     * See the PermSn class notes for further details on how array-like
     * indexing works for permutations of \a n objects.  In particular, note
     * that which permutation corresponds to which index will depend upon the
     * template parameter \a order.
     *
     * For \a n ≤ 7, this operator is very fast constant time.
     * However, for \a n ≥ 8 the current implementation is quadratic in \a n.
     *
     * \param index an index between 0 and `n!-1` inclusive.
     * \return the corresponding permutation.
     */
    constexpr Perm<n> operator[] (Perm<n>::Index index) const {
        // This is the generic implementation for n ≥ 8.
        // The fast implementation for n ≤ 7 is in the specialisation of
        // PermSn<n> where codeType == PermCode::Index.
        using Code = typename Perm<n>::Code;
        Code code = 0;

        if constexpr (order == PermOrder::Lex) {
            // We begin by constructing a code whose successive digits are
            // "base" n, n-1, ... 2, 1.
            for (int p = 1; p <= n; ++p) {
                // p tells us how far back from the *end* of the code we are.
                // n - p -> index % p;
                code |= (static_cast<Code>(index % p) <<
                    ((n - p) * Perm<n>::imageBits));
                index /= p;
            }
        } else {
            // As above, construct a code whose successive digits are
            // "base" n, n-1, ... 2, 1.
            // We can already see whether the resulting permutation will be even
            // or odd just from the parity of the sum of these "digits".
            bool parity = (index % 2 == 0);
            bool even = true;
            for (int p = 1; p <= n; ++p) {
                // p tells us how far back from the *end* of the code we are.
                int digit = index % p;
                // n - p -> digit
                code |= (static_cast<Code>(digit) <<
                    ((n - p) * Perm<n>::imageBits));
                if (digit % 2)
                    even = ! even;
                index /= p;
            }

            if (even != parity) {
                // Our algorithm below computes orderedSn, not Sn, and these
                // differ at index.  We adjust the code now to compensate.
                if (even) {
                    // index is odd: move to the previous permutation.
                    for (int p = 1; p <= n; ++p) {
                        int digit = ((code >> ((n - p) * Perm<n>::imageBits)) &
                            Perm<n>::imageMask);
                        // This digit is treated mod p.
                        if (digit > 0) {
                            // Decrement digit and stop.
                            code -= (Code(1) << ((n - p) * Perm<n>::imageBits));
                            break;
                        } else {
                            // Set digit to (p-1) and carry.
                            code |= (Code(p - 1) <<
                                ((n - p) * Perm<n>::imageBits));
                        }
                    }
                } else {
                    // i is even: move to the next permutation.
                    for (int p = 1; p <= n; ++p) {
                        int digit = ((code >> ((n - p) * Perm<n>::imageBits)) &
                            Perm<n>::imageMask);
                        // This digit is treated mod p.
                        if (digit < p - 1) {
                            // Increment digit and stop.
                            code += (Code(1) << ((n - p) * Perm<n>::imageBits));
                            break;
                        } else {
                            // Set digit to zero and carry.
                            code ^= (static_cast<Code>(digit) <<
                                ((n - p) * Perm<n>::imageBits));
                        }
                    }
                }
            }
        }

        for (int pos1 = Perm<n>::imageBits * (n - 1); pos1 >= 0;
                pos1 -= Perm<n>::imageBits) {
            for (int pos2 = pos1 + Perm<n>::imageBits;
                    pos2 < n * Perm<n>::imageBits; pos2 += Perm<n>::imageBits) {
                if (((code >> pos2) & Perm<n>::imageMask) >= ((code >> pos1) &
                        Perm<n>::imageMask))
                    code += (Code(1) << pos2); // increment image at pos2
            }
        }

        return Perm<n>(code);
    }

    /**
     * Returns the total number of permutations of \a n objects.
     * This is of course just `n!`.
     *
     * \python This is also used to implement the Python special
     * method __len__().
     *
     * \return the total number of permutations.
     */
    constexpr Perm<n>::Index size() const {
        return Perm<n>::nPerms;
    }

    /**
     * Returns an iterator pointing to the first permutation according to
     * the chosen ordering.
     *
     * For all supported orderings, this first permutation is the identity
     * permutation; however, as the iterator steps through from begin() to
     * end(), the order in which subsequent permutations appear will depend
     * upon the template parameter \a order.
     *
     * See the PermSn class notes for further details on how iteration works
     * over all permutations of \a n objects.
     *
     * \nopython For Python users, PermSn implements the Python iterable
     * interface.  You can iterate over all permutations in the same way
     * that you would iterate over any native Python container.
     *
     * \return a starting iterator for iterating over all permutations of
     * \a n objects.
     */
    constexpr iterator begin() const {
        return {};
    }

    /**
     * Returns an iterator pointing beyond the last permutation.
     *
     * See the PermSn class notes for further details on how iteration works
     * over all permutations of \a n objects.
     *
     * \nopython For Python users, PermSn implements the Python iterable
     * interface.  You can iterate over all permutations in the same way
     * that you would iterate over any native Python container.
     *
     * \return a past-the-end iterator for iterating over all permutations
     * of \a n objects.
     */
    constexpr iterator end() const {
        return { false };
    }

#ifdef __APIDOCS
    /**
     * Returns a Python iterator over all permutations of \a n objects.
     *
     * See the PermSn class notes for further details on how iteration works.
     *
     * \nocpp For C++ users, PermSn provides the usual begin() and end()
     * functions instead.  In particular, you can iterate over all permutations
     * in the usual way using a range-based \c for loop.
     *
     * \return an iterator over all permutations of \a n objects.
     */
    auto __iter__() const;
#endif

    /**
     * A trivial equality test that always returns \c true.
     *
     * Since PermSn contains no data of its own, any two PermSn objects of the
     * same type (i.e., using the same template parameters) will always
     * describe the same sequence of permutations in the same order.
     *
     * \return \c true, always.
     */
    constexpr bool operator == (const PermSn&) const {
        return true;
    }
};

#ifndef __DOXYGEN
// Hide the details of template specialisations from doxygen.
template <int n, PermOrder order>
struct PermSn<n, order, PermCodeType::Index> {
    static_assert(Perm<n>::codeType == PermCodeType::Index);

    class iterator;
    using const_iterator = iterator;

    constexpr Perm<n> operator[] (Perm<n>::Index index) const {
        if constexpr (order == PermOrder::Sign || n == 2) {
            return { static_cast<Perm<n>::Code2>(index) };
        } else {
            return Perm<n>(static_cast<Perm<n>::Code2>(
                Perm<n>::convOrderedUnordered(index)));
        }
    }

    constexpr Perm<n>::Index size() const {
        return Perm<n>::nPerms;
    }

    constexpr iterator begin() const {
        return {};
    }

    constexpr iterator end() const {
        return { false };
    }

    constexpr bool operator == (const PermSn&) const {
        return true;
    }

    // Any additions to the generic PermSn<n>::iterator API (documented above)
    // and the standard C++ random access iterator API are documented below as
    // they occur.
    class iterator {
        public:
            using value_type = Perm<n>;
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = typename Perm<n>::Index;
            using pointer = const value_type*;
            using reference = Perm<n>;

        private:
            Perm<n>::Index index_;
                /**< The index of the permutation that this iterator currently
                     points to, using the chosen ordering, or Perm<n>::nPerms
                     for a past-the-end iterator. */

        public:
            constexpr iterator() : index_(0) {}
            constexpr iterator(bool valid) :
                index_(valid ? 0 : Perm<n>::nPerms) {}
            constexpr iterator(const iterator&) = default;
            constexpr iterator& operator = (const iterator&) = default;
            constexpr bool operator == (const iterator&) const = default;
            constexpr std::strong_ordering operator <=> (const iterator&)
                const = default;

            constexpr operator bool() const {
                return index_ >= 0 && index_ < Perm<n>::nPerms;
            }

            constexpr Perm<n> operator * () const {
                if constexpr (order == PermOrder::Sign || n == 2) {
                    return { static_cast<Perm<n>::Code2>(index_) };
                } else {
                    return { static_cast<Perm<n>::Code2>(
                        Perm<n>::convOrderedUnordered(index_)) };
                }
            }

            constexpr iterator& operator ++ () {
                ++index_;
                return *this;
            }

            constexpr iterator operator ++ (int) {
                return iterator(index_++);
            }

            constexpr iterator& operator -- () {
                --index_;
                return *this;
            }

            constexpr iterator operator -- (int) {
                return iterator(index_--);
            }

            constexpr iterator operator + (difference_type k) const {
                return iterator(index_ + k);
            }

            friend constexpr iterator operator + (difference_type k, iterator i) {
                return iterator(i.index_ + k);
            }

            constexpr iterator& operator += (difference_type k) {
                index_ += k;
                return *this;
            }

            constexpr iterator operator - (difference_type k) const {
                return iterator(index_ - k);
            }

            constexpr iterator& operator -= (difference_type k) {
                index_ -= k;
                return *this;
            }

            constexpr difference_type operator - (iterator rhs) const {
                return index_ - rhs.index_;
            }

            constexpr Perm<n> operator [] (difference_type k) const {
                if constexpr (order == PermOrder::Sign || n == 2) {
                    return { static_cast<Perm<n>::Code2>(index_ + k) };
                } else {
                    return { static_cast<Perm<n>::Code2>(
                        Perm<n>::convOrderedUnordered(index_ + k)) };
                }
            }

        private:
            /**
             * Creates a new iterator pointing to the permutation at the given
             * index according to the chosen ordering.  An index of
             * Perm<n>::nPerms indicates that the iterator should be
             * past-the-end.
             *
             * \param index the index of the permutation to point to; this
             * must be between 0 and Perm<n>::nPerms inclusive.
             */
            constexpr iterator(Perm<n>::Index index) : index_(index) {}
    };
};
#endif

namespace detail {

/**
 * A lightweight array-like object that indexes smaller permutations within
 * larger permutation groups; that is, it embeds the group \a S_m inside
 * \a S_n for some `n > m`.
 *
 * This class is not intended for end users.  Its main purpose is to support
 * other parts of Regina's API, such as `Perm<n>::extend()`, and some old (and
 * now deprecated) permutation class constants such as `Perm<4>::S3`.  This
 * class is hard-coded only for some specific small values of \a n and \a m
 * (where the operations are trivial or the compiler can use small lookup
 * tables).  If you need to express a smaller permutation using a larger
 * permutation class, you should use `Perm<n>::extend()` instead.
 *
 * This class only offers index-based lookup: you can either use the static
 * function `at(index)`, or you can treat an object \c of this class like a
 * container and use `c[index]` and `c.size()` (and in Python, `len(c)`).
 * This class does not support iteration in C++ (although Python still allows
 * it because Python detects the array-like structure).
 *
 * Permutations are indexed according to the template parameter \a order.
 * In particular, `PermSubSn<n, m, order>` indexes permutations in the same
 * order as `PermSn<m, order>`.
 *
 * All operations in this class are fast constant time.
 *
 * Objects of this type contain no data at all, which means they are trivial to
 * pass by value or swap with std::swap(), and all objects of this type are
 * essentially identical.
 *
 * \warning This class may be altered or removed without notice from a future
 * version of Regina, since this is essentially an internal class designed to
 * support deprecated constants such as `Perm<4>::S3`.  The "officially
 * supported" way of accessing the <i>i</i>th permutation of \a m objects
 * using the type `Perm<n>` is `Perm<n>::extend(Perm<m>::Sn[i])`.
 *
 * \python This class does not live inside an inner `detail` namespace, though
 * as an internal class it is subject to change or removal without notice
 * (see the warning above).  Moreover, Python does not support templates,
 * and so the name of this class is constructed by appending \a n, \a m and
 * \a order as suffixes (e.g., PermSubSn4_3_Sign, or PermSubSn5_3_Lex).
 * The only template parameters that are bound in Python are those that are
 * used in Regina's public-facing API (specifically, those that are used by
 * deprecated constants such as `Perm4.S3`).
 *
 * \tparam n indicates the return type: permutations of \a m objects will be
 * returned as the larger type `Perm<n>`.  It is required that `2 ≤ n ≤ 5`.
 * \tparam m the number of objects being permuted in the group \a S_m that we
 * are enumerating.  It is required that `1 ≤ m < n`.
 * \tparam order the way in which this class orders permutations for the
 * purposes of indexing.
 *
 * \ingroup maths
 */
template <int n, int m, PermOrder order = PermOrder::Sign>
#ifdef __APIDOCS
struct PermSubSn {
    /**
     * Returns the permutation at the given index.
     *
     * This is a permutation on \a m objects being returned as the larger type
     * `Perm<n>`, and so the unused elements `m,m+1,...,n-1` will all be
     * mapped to themselves.
     *
     * \param index an index between 0 and `m!-1` inclusive.
     * \return the corresponding permutation of \a m objects.
     */
    static constexpr Perm<n> at(int index);

    /**
     * Returns the permutation at the given index.
     *
     * This is a permutation on \a m objects being returned as the larger type
     * `Perm<n>`, and so the unused elements `m,m+1,...,n-1` will all be
     * mapped to themselves.
     *
     * This operator is identical to calling the static member function at().
     * It is provided for convenience so that permutations can be accessed
     * using array-like syntax.
     *
     * \param index an index between 0 and `m!-1` inclusive.
     * \return the corresponding permutation of \a m objects.
     */
    constexpr Perm<n> operator[] (int index) const;

    /**
     * Returns the total number of permutations on \a m objects.
     * This of course is just `m!`.
     *
     * \python This is also used to implement the Python special
     * method __len__().
     *
     * \return the total number of permutations.
     */
    constexpr int size();

    /**
     * A trivial equality test that always returns \c true.
     *
     * Since PermSubSn contains no data of its own, any two PermSubSn objects
     * of the same type (i.e., using the same template parameters) will always
     * describe the same sequence of permutations in the same order.
     *
     * \return \c true, always.
     */
    constexpr bool operator == (const PermSubSn&) const;
};
#else
struct PermSubSn;

template <int n, PermOrder order>
struct PermSubSn<n, 1, order> {
    static_assert(n > 1);

    static constexpr Perm<n> at(int) {
        return {};
    }

    constexpr Perm<n> operator[] (int) const {
        return {};
    }

    constexpr int size() {
        return 1;
    }

    constexpr bool operator == (const PermSubSn&) const {
        return true;
    }
};

template <int n, PermOrder order>
struct PermSubSn<n, 2, order> {
    static_assert(n > 2);

    static constexpr Perm<n> at(int index) {
        return (index == 0 ? Perm<n>() : Perm<n>(0, 1) /* pair swap */);
    }

    constexpr Perm<n> operator[] (int index) const {
        return at(index);
    }

    constexpr int size() {
        return 2;
    }

    constexpr bool operator == (const PermSubSn&) const {
        return true;
    }
};

template <int n, PermOrder order>
struct PermSubSn<n, 3, order> {
    static_assert(n > 3);
    static_assert(Perm<n>::codeType == PermCodeType::Index);

    private:
        using Code = typename Perm<n>::Code2;
        static constexpr Code fact1 = Perm<n-1>::nPerms;
        static constexpr Code fact2 = Perm<n-2>::nPerms;
        static constexpr Code table[6] {
            0, fact2+1, fact1+fact2, fact1+1, 2*fact1, 2*fact1+fact2+1
        }; /**< Maps sign-based indices from S3 into Sn. */

    public:
        static constexpr Perm<n> at(int index) {
            if constexpr (order == PermOrder::Sign)
                return Perm<n>::fromPermCode2(table[index]);
            else {
                // Sign vs lex orderings differ only at indices 2,3.
                return Perm<n>::fromPermCode2(table[
                    (index & 2) ? (index ^ 1) : index]);
            }
        }

        constexpr Perm<n> operator[] (int index) const {
            return at(index);
        }

        constexpr int size() {
            return 6;
        }

        constexpr bool operator == (const PermSubSn&) const {
            return true;
        }
};

template <int n, PermOrder order>
struct PermSubSn<n, 4, order> {
    static_assert(n > 4);
    static_assert(Perm<n>::codeType == PermCodeType::Index);

    private:
        using Code = typename Perm<n>::Code2;
        static constexpr Code fact1 = Perm<n-1>::nPerms;
        static constexpr Code fact2 = Perm<n-2>::nPerms;
        static constexpr Code fact3 = Perm<n-3>::nPerms;
        static constexpr Code table[24] {
            0, fact3+1, fact2+fact3, fact2+1, 2*fact2, 2*fact2+fact3+1,
            fact1+fact3, fact1+1, fact1+fact2, fact1+fact2+fact3+1,
                fact1+2*fact2+fact3, fact1+2*fact2+1,
            2*fact1, 2*fact1+fact3+1, 2*fact1+fact2+fact3, 2*fact1+fact2+1,
                2*fact1+2*fact2, 2*fact1+2*fact2+fact3+1,
            3*fact1+fact3, 3*fact1+1, 3*fact1+fact2, 3*fact1+fact2+fact3+1,
                3*fact1+2*fact2+fact3, 3*fact1+2*fact2+1
        }; /**< Maps sign-based indices from S4 into Sn. */

    public:
        static constexpr Perm<n> at(int index) {
            if constexpr (order == PermOrder::Sign)
                return Perm<n>::fromPermCode2(table[index]);
            else {
                // Sign vs lex orderings differ at indices {2,3} mod 4.
                return Perm<n>::fromPermCode2(table[
                    (index & 2) ? (index ^ 1) : index]);
            }
        }

        constexpr Perm<n> operator[] (int index) const {
            return at(index);
        }

        constexpr int size() {
            return 24;
        }

        constexpr bool operator == (const PermSubSn&) const {
            return true;
        }
};
#endif

} // namespace detail
} // namespace regina

#endif

