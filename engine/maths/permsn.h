
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
 * Typically you would access this object through the static constant
 * `Perm<n>::Sn`.  There should be no need for end users to refer to this type
 * directly.
 *
 * The two main ways in which you can use this object are:
 *
 * - Array-style lookup, using `Perm<n>::Sn[i]` and `Perm<n>::Sn::size()`
 *   (or the usual `len()` function in Python);
 *
 * - Iteration, by iterating over `Perm<n>::Sn` in a range-based \c for loop
 *   (or by using the iterators `Perm<n>::Sn.begin()` and `Perm<n>::Sn.end()`
 *   directly).
 *
 * Regarding indices and iteration:
 *
 * - The permutations with even indices are the even permutations, and the
 *   permutations with odd indices are the odd permutations.  The identity
 *   permutation will have index 0.
 *
 * - The order of iteration is the same ordering used by index-based lookup.
 *   In particular, iteration will start with the identity and alternate
 *   between even and odd permutations.
 *
 * - Iterating is typically faster than calling `Sn[i]` for each index \a i in
 *   turn, particularly for larger \a n.  See the notes on time complexity
 *   below.
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
 * essentially identical.  As mentioned above, you would typically just use
 * `Perm<n>::Sn` instead of creating an object of this type yourself.
 *
 * \python Python does not support templates.  In Python, the various classes
 * `PermSn<n>` are available under the names PermSn2, PermSn3, ..., PermSn16.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 *
 * \ingroup maths
 */
template <int n, PermCodeType codeType = Perm<n>::codeType>
struct PermSn {
    // This is a generic implementation for n ≥ 8 (where codeType is Images).
    // There is a specialisation below for n ≤ 7 (where codeType is Index).
    static_assert(Perm<n>::codeType == PermCodeType::Images,
        "The generic implementation of PermSn<n> should only be used for "
        "larger n, where permutations are stored using image packs.");

    /**
     * An iterator over all permutations in \a Sn.
     *
     * See the PermSn class notes for further details on how iteration works
     * over \a Sn.  In particular:
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
     *   access iterator (\a n ≤ 7) or a forward iterator (\a n ≥ 8), and
     *   you would typically access iterators either via `Perm<n>::Sn.begin()`
     *   and `Perm<n>::Sn.end()`, or by using a range-based \c for loop over
     *   `Perm<n>::Sn`.
     *
     * - In Python, this class and PermSn together implement the expected
     *   iterface for Python iterators: you would typically create and use
     *   iterators by iterating over `Perm<n>.Sn`, which internally uses
     *   `PermSn.__iter__()` and `PermSn.iterator.__next__()`.
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
                     permutations in \a Sn are generated, not stored. */

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
             * This iterator will point to the identity permutation; that is,
             * it will be equal to `Perm<n>::Sn.begin()`.
             */
            constexpr iterator() : perm_(), valid_(true) {}
            /**
             * Creates either a begin or end iterator.
             *
             * If no arguments are given or if \a valid is \c true, then this
             * iterator will point to the identity permutation; that is, it
             * will be equal to `Perm<n>::Sn.begin()`.
             *
             * If \a valid is \c false, then this iterator will be past-the-end;
             * that is, it will be equal to `Perm<n>::Sn.end()`.
             *
             * \param valid \c true if this should be a begin iterator, or
             * \c false if this should be an end iterator.
             */
            constexpr iterator(bool valid) : perm_(), valid_(valid) {}
            /**
             * Creates a copy of the given iterator.
             *
             * \nopython The only way to create an iterator over \a Sn is to
             * iterate over `Perm<n>::Sn`.
             */
            constexpr iterator(const iterator&) = default;
            /**
             * Makes this a copy of the given iterator.
             *
             * \return a reference to this iterator.
             */
            iterator& operator = (const iterator&) = default;
            /**
             * Compares this with the given iterator for equality.
             *
             * To be considered equal, two iterators must be pointing
             * to the same permutation in \a Sn.
             *
             * Two past-the-end iterators will be considered equal.
             *
             * \param rhs the iterator to compare this with.
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
            iterator& operator ++ () {
                ++perm_;
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
            iterator operator ++ (int) {
                Perm<n> ans = perm_++;
                if (perm_.isIdentity())
                    valid_ = false;
                return ans;
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
     * An iterator over all permutations in \a Sn.
     *
     * Both \a iterator and \a const_iterator are the same type, since PermSn
     * only offers read-only access to its permutations.  See the
     * PermSn::iterator class for further details.
     */
    using const_iterator = iterator;

    /**
     * Returns the permutation at the given index in \a Sn.
     *
     * See the PermSn class notes for further details on how array-like
     * indexing works over \a Sn.
     *
     * For \a n ≤ 7, this operator is very fast constant time.
     * However, for \a n ≥ 8 the current implementation is quadratic in \a n.
     *
     * \param index an index between 0 and `n!-1` inclusive.
     * \return the corresponding permutation in Sn.
     */
    constexpr Perm<n> operator[] (Perm<n>::Index index) const {
        // This is the generic implementation for n ≥ 8.
        // The fast implementation for n ≤ 7 is in the specialisation of
        // PermSn<n> where codeType == PermCode::Index.
        using Code = typename Perm<n>::Code;
        Code code = 0;

        // We begin by constructing a code whose successive digits are "base"
        // n, n-1, ... 2, 1.
        // We can already see whether the resulting permutation will be even
        // or odd just from the parity of the sum of these "digits".
        bool parity = (index % 2 == 0);
        bool even = true;
        for (int p = 1; p <= n; ++p) {
            // Here p tells us how far back from the *end* of the code we are.
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
                        code |= (Code(p - 1) << ((n - p) * Perm<n>::imageBits));
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

        // Carry on as with do with orderedSn.
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
     * Returns the total number of permutations in \a Sn.
     * This is of course just `n!`.
     *
     * \python This is also used to implement the Python special
     * method __len__().
     *
     * \return the size of \a Sn.
     */
    constexpr Perm<n>::Index size() const {
        return Perm<n>::nPerms;
    }

    /**
     * Returns an iterator pointing to the first permutation in \a Sn.
     *
     * See the PermSn class notes for further details on how iteration works
     * over \a Sn.
     *
     * \nopython For Python users, PermSn implements the Python iterable
     * interface.  You can iterate over the elements of \a Sn in the same
     * way that you would iterate over any native Python container.
     *
     * \return a starting iterator for iterating over all of \a Sn.
     */
    constexpr iterator begin() const {
        return {};
    }

    /**
     * Returns an iterator pointing beyond the last permutation in \a Sn.
     *
     * See the PermSn class notes for further details on how iteration works
     * over \a Sn.
     *
     * \nopython For Python users, PermSn implements the Python iterable
     * interface.  You can iterate over the elements of \a Sn in the same
     * way that you would iterate over any native Python container.
     *
     * \return a past-the-end iterator for iterating over all of \a Sn.
     */
    constexpr iterator end() const {
        return { false };
    }

#ifdef __APIDOCS
    /**
     * Returns a Python iterator over all permutations in \a Sn.
     *
     * See the PermSn class notes for further details on how iteration works
     * over \a Sn.
     *
     * \nocpp For C++ users, PermSn provides the usual begin() and end()
     * functions instead.  In particular, you can iterate over \a Sn in the
     * usual way using a range-based \c for loop.
     *
     * \return an iterator over all permutations in \a Sn.
     */
    auto __iter__() const;
#endif
};

#ifndef __DOXYGEN
// Hide the details of template specialisations from doxygen.
template <int n>
struct PermSn<n, PermCodeType::Index> {
    static_assert(Perm<n>::codeType == PermCodeType::Index);

    class iterator;
    using const_iterator = iterator;

    constexpr Perm<n> operator[] (Perm<n>::Index index) const {
        if constexpr (n < 4)
            return { static_cast<Perm<n>::Code>(index) };
        else
            return { static_cast<Perm<n>::Code2>(index) };
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
                /**< The Sn index of the permutation that this iterator
                     currently points to, or Perm<n>::nPerms for a
                     past-the-end iterator. */

        public:
            constexpr iterator() : index_(0) {}
            constexpr iterator(bool valid) :
                index_(valid ? 0 : Perm<n>::nPerms) {}
            constexpr iterator(const iterator&) = default;
            iterator& operator = (const iterator&) = default;
            constexpr bool operator == (const iterator&) const = default;
            constexpr std::strong_ordering operator <=> (const iterator&)
                const = default;

            constexpr operator bool() const {
                return index_ >= 0 && index_ < Perm<n>::nPerms;
            }

            constexpr Perm<n> operator * () const {
                if constexpr (n < 4)
                    return { static_cast<Perm<n>::Code>(index_) };
                else
                    return { static_cast<Perm<n>::Code2>(index_) };
            }

            iterator& operator ++ () {
                ++index_;
                return *this;
            }

            iterator operator ++ (int) {
                return iterator(index_++);
            }

            iterator& operator -- () {
                --index_;
                return *this;
            }

            iterator operator -- (int) {
                return iterator(index_--);
            }

            constexpr iterator operator + (difference_type k) const {
                return iterator(index_ + k);
            }

            friend iterator operator + (difference_type k, iterator i) {
                return iterator(i.index_ + k);
            }

            iterator& operator += (difference_type k) {
                index_ += k;
                return *this;
            }

            constexpr iterator operator - (difference_type k) const {
                return iterator(index_ - k);
            }

            iterator& operator -= (difference_type k) {
                index_ -= k;
                return *this;
            }

            constexpr difference_type operator - (iterator rhs) const {
                return index_ - rhs.index_;
            }

            constexpr Perm<n> operator [] (difference_type k) const {
                if constexpr (n < 4)
                    return { static_cast<Perm<n>::Code>(index_ + k) };
                else
                    return { static_cast<Perm<n>::Code2>(index_ + k) };
            }

        private:
            /**
             * Creates a new iterator pointing to the permutation at the given
             * index in \a Sn.  An index of Perm<n>::nPerms indicates that the
             * iterator should be past-the-end.
             *
             * \param index the index of the permutation to point to; this
             * must be between 0 and Perm<n>::nPerms inclusive.
             */
            constexpr iterator(Perm<n>::Index index) : index_(index) {}
    };
};
#endif

} // namespace regina

#endif

