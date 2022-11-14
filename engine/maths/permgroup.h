
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

#ifndef __REGINA_PERMGROUP_H
#ifndef __DOXYGEN
#define __REGINA_PERMGROUP_H
#endif

/*! \file maths/permgroup.h
 *  \brief Implements groups of permutations on \a n objects.
 */

#include "core/output.h"
#include "maths/perm.h"

namespace regina {

/**
 * Constants that represent particular well-known classes of permutation groups.
 *
 * These constants are intended to be used with permutation groups on
 * \a n elements for arbitrary \a n.  (In particular, you can pass them
 * to the PermGroup<n> constructor.)
 */
enum NamedPermGroup {
    /**
     * Represents the trivial group on \a n elements, containing only the
     * identity permutation.
     */
    PERM_GROUP_TRIVIAL = 0,
    /**
     * Represents the symmetric group on \a n elements, containing all `n!`
     * possible permutations.
     */
    PERM_GROUP_SYMMETRIC = 1,
    /**
     * Represents the alternating group on \a n elements, containing all `n!/2`
     * even permutations.
     */
    PERM_GROUP_ALTERNATING = 2
};

/**
 * Represents a group of permutations on \a n elements.  This is a subgroup
 * of the symmetric group `S_n`.
 *
 * Groups are stored internally using Sims tables (see Knuth volume 4A for a
 * description of how these work); these are called _stabiliser chains_ in
 * many places.  This storage mechanism means that, even though a permutation
 * group could have size factorial in \a n, the storage space required is only
 * quadratic in \a n.
 *
 * PermGroup objects are, in their current implementation, entirely
 * stack-based.  This means they cannot support fast move or swap operations.
 * However, since their size is quadratic in \a n, copy operations involve
 * significantly more overhead than (for example) just copying a Perm object
 * (which just holds a single machine-native integer).  This decision is a
 * deliberate trade-off between speed versus space; the implication for
 * end users is that you should be economical about copying PermGroup objects,
 * and work with them in-place where possible.
 *
 * \python Python does not support templates.  In Python, the "vanilla"
 * non-cached variants `Perm<n>` are available under the names
 * PermGroup2, PermGroup3, ..., PermGroup16, and the cached variants
 * `Perm<n, true>` are available under the names
 * PermGroup2_Cached, PermGroup3_Cached, ..., PermGroup16_Cached.
 *
 * \tparam n the number of objects being permuted.
 * This must be between 2 and 16 inclusive.
 * \tparam cached \c true if we should use precomputation-assisted routines
 * such as Perm<n>::cachedComp() and Perm<n>::cachedInverse(), or \c false
 * (the default) if we should just use the composition operator, inverse(),
 * and so on.  If this argument is \c true, you _must_ have called
 * Perm<n>::precompute() at least once in the lifetime of the program
 * before using this class.
 *
 * \ingroup maths
 */
template <int n, bool cached = false>
class PermGroup : public Output<PermGroup<n, cached>> {
    private:
        /**
         * The permutation term_[k][j] for k >= j is:
         *
         * - any group element that maps k to j and fixes (k+1),...,(n-1),
         *   if the group has such an element;
         *
         * - the identity permutation if the group has no such element.
         *
         * In the special case k == j, we insist on using the identity.
         *
         * Every group element then has a unique representation of the form
         * `term_[n-1][...] * term_[n-2][...] * ... * term_[1][...]`,
         * where we only allow identity terms of the form term_[k][k]
         * (that is, if term_[k][j] is the identity for k > j, then we never
         * use that term at all).
         *
         * Note that term_[0][...] must always be term_[0][0] == identity,
         * which is why we exclude it from the representation above.
         *
         * Finally: since we have the space for it, we use term_[j][k] to hold
         * the _inverse_ of term_[k][j].  (The case j == k is not a problem,
         * since - as noted above - these are always identity permutations).
         */
        Perm<n> term_[n][n];

        /**
         * Indicates how many terms term_[k][j] are usable for each \a k.
         * For each \a k, we have 1 ≤ count_[k] ≤ (k+1).
         */
        int count_[n];

        /**
         * Indicates which terms term_[k][j] are usable for each \a k.
         * Specifically, if the usable terms for some \a k are
         * term_[k][a], term_[k][b], ..., term_[k][z], term_[k][k]
         * where `a < b < ... < z < k`, then usable_[k] maps
         * (0,1,...,count_[k]-1) to (a,b,...,z,k).
         */
        Perm<n> usable_[n];

        /**
         * Each initSeq_[i] is the precomputed product
         * term_[i][usable_[i][0]] * ... * term_[0][usable_[0][0]].
         * Note that initSeq_[0] will always be the identity, but it is
         * not doing too much harm to keep this around (and save other +/-1
         * operations on indices later on).
         */
        Perm<n> initSeq_[n];

    public:
        /**
         * The iterator type for this group.
         *
         * Unlike most iterator types, the dereference operator for this
         * iterator type returns by value (not by reference).  This is because
         * the individual permutations in a group are generated (not stored),
         * based upon an internal group representation that is typically
         * _much_ smaller than the group itself.
         *
         * Both \a iterator and \a const_iterator are the same type, since
         * a PermGroup only offers read-only access to its group members.
         */
        class iterator {
            public:
                using value_type = Perm<n>;
                    /**< Indicates what type the iterator points to. */
                using iterator_category = std::input_iterator_tag;
                    /**< Declares this to be an input iterator type. */
                using difference_type = typename Perm<n>::Index;
                    /**< The type obtained by subtracting iterators. */
                using pointer = const value_type*;
                    /**< A pointer to \a value_type. */
                using reference = Perm<n>;
                    /**< The type returned by the dereference operator,
                         which for this iterator type is by value.
                         See the iterator class notes for details. */

            private:
                const PermGroup* group_;
                    /**< The group over which we are iterating.  This is
                         a pointer (not a reference) so that we can
                         support the assignment operator. */
                int pos_[n];
                    /**< Indicates which of the terms PermGroup::term_[k][j]
                         are actually being used for the current permutation.
                         Specifically, for each \a k we use
                         term_[k][usable_[k][pos_[k]]].
                         We have 0 <= pos_[i] < count_[i] for each \a i.
                         For a past-the-end iterator, we set
                         pos_[0] = count_[0] (which is always 1), and we allow
                         pos_[i] to be undefined for \a i > 0. */
                Perm<n> current_;
                    /**< The curent permutation.
                         For a past-the-end iterator, this is undefined. */

            public:
                /**
                 * Creates a new uninitialised iterator.
                 *
                 * \nopython The only way to create an iterator over a
                 * PermGroup \a g is to iterate over \a g (i.e., to call
                 * `g.__iter__()`).
                 */
                iterator();
                /**
                 * Creates a copy of the given iterator.
                 *
                 * \nopython The only way to create an iterator over a
                 * PermGroup \a g is to iterate over \a g (i.e., to call
                 * `g.__iter__()`).
                 */
                iterator(const iterator&) = default;

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
                 * to the same permutation within the same group.
                 * (The second condition means that the underlying PermGroup
                 * pointers must be the same - it is not enough to have
                 * two distinct PermGorup objects with identical contents.)
                 *
                 * Two past-the-end iterators will always be considered equal.
                 *
                 * \param rhs the iterator to compare this with.
                 * \return \c true if the iterators point to the same
                 * permutation, or \c false if they do not.
                 */
                bool operator == (const iterator& rhs) const;
                /**
                 * Compares this with the given iterator for inequality.
                 *
                 * To be considered equal, two iterators must be pointing
                 * to the same permutation within the same group.
                 * (The second condition means that the underlying PermGroup
                 * pointers must be the same - it is not enough to have
                 * two distinct PermGorup objects with identical contents.)
                 *
                 * Two past-the-end iterators will always be considered equal.
                 *
                 * \param rhs the iterator to compare this with.
                 * \return \c false if the iterators point to the same
                 * permutation, or \c true if they do not.
                 */
                bool operator != (const iterator& rhs) const;

                /**
                 * The preincrement operator.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return a reference to this iterator after the increment.
                 */
                iterator& operator ++ ();
                /**
                 * The postincrement operator.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return a copy of this iterator before the increment took
                 * place.
                 */
                iterator operator ++ (int);
#ifdef __APIDOCS
                /**
                 * Returns the current permutation and increments this iterator.
                 *
                 * \nocpp For C++ users, ChildIterator provides the usual
                 * iterator preincrement, postincrement and dereferencing
                 * operators (++ and *) instead.
                 *
                 * \exception StopIteration The iterator is already past-the-end
                 * when this function is called.
                 *
                 * \return the permutation that this iterator is pointing to,
                 * before the increment takes place.
                 */
                Perm<n> __next__();
#endif

                /**
                 * Returns the permutation that this iterator is currently
                 * pointing to.
                 *
                 * \pre This iterator is dereferenceable (in particular,
                 * it is not past-the-end).
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return the corresponding permutation.
                 */
                Perm<n> operator * () const;

                /**
                 * Identifies whether this iterator is dereferenceable.
                 *
                 * \nopython For Python users, this class implements the Python
                 * iterator interface instead.  See __next__() for details.
                 *
                 * \return \c true if and only if this is dereferenceable
                 * (i.e., not past-the-end).
                 */
                operator bool() const;

            private:
                /**
                 * Creates a new iterator with group_ set to the given
                 * group, but with all other data members left uninitialised.
                 */
                iterator(const PermGroup* group);

                friend class PermGroup<n, cached>;
        };

        /**
         * The iterator type for this group.
         *
         * Both \a iterator and \a const_iterator are the same type, since
         * a PermGroup only offers read-only access to its group members.
         * See the PermGroup::iterator class for further details.
         */
        using const_iterator = iterator;

    public:
        /**
         * Constructs the trivial group, containing only the identity
         * permutation.
         */
        PermGroup();
        /**
         * Construct the given well-known permutation group.
         * This constructor can (for example) be used to easily construct
         * the symmetric or alternating group on \a n elements.
         *
         * \param group indicates which well-known permutation group to
         * construct.
         */
        PermGroup(NamedPermGroup group);
        /**
         * Constructs the symmetric group `S_k`, formed from all
         * permutations of 1,...,\a k.  The elements (\a k + 1),...,\a n
         * will remain fixed under all permutations in this group.
         *
         * The size of this group will be `k!`.
         *
         * \param k indicates how many elements should be permuted; this
         * must be between 0 and \a n inclusive.
         */
        PermGroup(int k);
        /**
         * Creates a new copy of the given group.
         *
         * \param src the group to copy.
         */
        PermGroup(const PermGroup& src) = default;
        /**
         * Generates the subgroup of all elements in the given group
         * that pass the given membership test.
         *
         * Specifically, this generates the subgroup of all permutations \a p
         * in \a parent for which `test(p, args...)` returns \c true.
         *
         * The argument \a test should be a function or some other callable
         * object.  It must return a boolean, and its first argument should
         * be a permutation (either by value as type `Perm<n>`, or by
         * const reference as type `const Perm<n>&`).  If there are
         * any additional arguments supplied in the list \a args, these
         * will be forwarded through as additional arguments to \a test.
         *
         * Note that \a test will not necessarily be called for _all_
         * permutations in \a parent, since this routine will deduce
         * some subgroup members using the standard subgroup properties
         * (e.g., closure and inverse).  It is, however, guaranteed that
         * the _only_ permutations passed to \a test will be permutations
         * that are already known to belong to \a parent.
         *
         * \pre The given membership test does actually define a subgroup
         * (that is, it behaves appropriately with respect to identity,
         * inverse and closure).
         *
         * \python This constructor is available in Python, and the \a test
         * argument may be a pure Python function.  However, its form is more
         * restricted: \a test must take exactly one argument (the permutation),
         * and the \a args argument to this constructor is not present.
         *
         * \param parent the "starting" group of all permutations under
         * consideration.
         * \param test a function (or other callable object) that determines
         * which permutations in \a parent become members of this subgroup.
         * \param any additional arguments that should be passed to \a test,
         * following the initial permutation argument.
         */
        template <typename Test, typename... Args>
        PermGroup(const PermGroup& parent, Test&& test, Args&&... args);

        /**
         * Sets this to be a copy of the given group.
         *
         * \param src the group to copy.
         * \return a reference to this group.
         */
        PermGroup& operator = (const PermGroup& src) = default;

        /**
         * Returns the total number of elements in this group.
         *
         * \return the size of this group.
         */
        typename Perm<n>::Index size() const;

        /**
         * Determines whether the given permutation belongs to this group.
         *
         * Regardless of the size of this group, the running time for
         * this routine is small polynomial in \a n.
         *
         * \param p the permutation whose membership we wish to test.
         * \return \c true if and only if \a p belongs to this group.
         */
        bool contains(Perm<n> p) const;

        /**
         * Indicates whether this and the given group are identical.
         *
         * This does _not_ test group isomorphism, and it does _not_
         * test whether the two groups use the same internal representation.
         * Instead it tests _membership_; that is, whether or not the two
         * groups contain precisely the same set of permutations.
         *
         * As a result, this test is not trivial.  It _is_ small polynomial
         * time in \a n, but it is not as fast as (for example) directly
         * comparing the internal representations.
         *
         * \other the group to compare this with.
         * \return \c true if and only if this and the given group contain
         * the same permutations.
         */
        bool operator == (const PermGroup& other) const;
        /**
         * Indicates whether this and the given group are different.
         *
         * This does _not_ test group isomorphism, and it does _not_
         * test whether the two groups use the same internal representation.
         * Instead it tests _membership_; that is, whether or not the two
         * groups contain precisely the same set of permutations.
         *
         * As a result, this test is not trivial.  It _is_ small polynomial
         * time in \a n, but it is not as fast as (for example) directly
         * comparing the internal representations.
         *
         * \other the group to compare this with.
         * \return \c true if and only if there is some permutation that
         * belongs to one group but not the other.
         */
        bool operator != (const PermGroup& other) const;

        /**
         * Returns a C++ iterator pointing to the first element of this group.
         *
         * The iterator range from begin() to end() runs through all
         * permutations in this group.  The order of iteration is arbitrary,
         * and may change in future releases of Regina.
         *
         * \nopython For Python users, PermGroup implements the Python iterable
         * interface.  You can iterate over the elements of this group in the
         * same way that you would iterate over any native Python container.
         *
         * \return an iterator pointing to the first element of this group.
         */
        iterator begin() const;
        /**
         * Returns a C++ iterator beyond the last element of this group.
         *
         * The iterator range from begin() to end() runs through all
         * permutations in this group.  The order of iteration is arbitrary,
         * and may change in future releases of Regina.
         *
         * \nopython For Python users, PermGroup implements the Python iterable
         * interface.  You can iterate over the elements of this group in the
         * same way that you would iterate over any native Python container.
         *
         * \return an iterator beyond the last element of this group.
         */
        iterator end() const;
#ifdef __APIDOCS
        /**
         * Returns a Python iterator over the elements of this group.
         *
         * The order of iteration is arbitrary, and may change in future
         * releases of Regina.
         *
         * \nocpp For C++ users, PermGroup provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over the elements
         * of this group in the usual way using a range-based \c for loop.
         *
         * \return an iterator over the elements of this group.
         */
        auto __iter__() const;
#endif

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Additional initialisation tasks that are common to all
         * constructors.
         *
         * Currently this just fills the initSeq_[] array.
         */
        void setup();
};

// Inline functions for PermGroup::iterator

template <int n, bool cached>
inline PermGroup<n, cached>::iterator::iterator() : group_(nullptr) {
}

template <int n, bool cached>
inline PermGroup<n, cached>::iterator::iterator(
        const PermGroup<n, cached>* group) : group_(group) {
}

template <int n, bool cached>
inline bool PermGroup<n, cached>::iterator::operator == (
        const PermGroup<n, cached>::iterator& rhs) const {
    if (*this) {
        // This is dereferenceable.
        // Compare the permutations before the groups, since the groups will
        // always be equal in "normal" scenarios.
        // Note that, for dereferenceable iterators, the permutations are
        // enough to define all of pos_[...].
        return rhs && (current_ == rhs.current_) && (group_ == rhs.group_);
    } else {
        // This is past-the-end.
        return (! rhs);
    }
}

template <int n, bool cached>
inline bool PermGroup<n, cached>::iterator::operator != (
        const PermGroup<n, cached>::iterator& rhs) const {
    // See the == operator for an explanation.
    if (*this) {
        // This is dereferenceable.
        return (! rhs) || (current_ != rhs.current_) || (group_ != rhs.group_);
    } else {
        // This is past-the-end.
        return rhs;
    }
}

template <int n, bool cached>
inline typename PermGroup<n, cached>::iterator
        PermGroup<n, cached>::iterator::operator ++(int) {
    iterator prev = *this;
    ++(*this);
    return prev;
}

template <int n, bool cached>
inline Perm<n> PermGroup<n, cached>::iterator::operator * () const {
    return current_;
}

template <int n, bool cached>
inline PermGroup<n, cached>::iterator::operator bool() const {
    return pos_[0] == 0;
}

// Inline functions for PermGroup

template <int n, bool cached>
inline PermGroup<n, cached>::PermGroup() {
    // All permutations term_[k][j] are already initialised to the identity.
    std::fill(count_, count_ + n, 1);
    for (int i = 1; i < n; ++i)
        usable_[i] = Perm<n>(0, i);

    setup();
}

template <int n, bool cached>
template <typename Test, typename... Args>
inline PermGroup<n, cached>::PermGroup(const PermGroup& parent, Test&& test,
        Args&&... args) {
    // Go through and fix term_[k][j] (k >= j), in order of increasing k.

    count_[0] = 1;
    // usable_[0] and term_[0][0] are already (correctly) identities.

    for (int k = 1; k < n; ++k) {
        std::array<int, n> usable;
        int count = 0;
        int unusedSlot = n - 1;

        for (int j = 0; j < k; ++j) {
            if (parent.term_[k][j].isIdentity()) {
                // The parent group cannot map k -> j.
                usable[unusedSlot--] = j;
                continue;
            }
            if (! term_[k][j].isIdentity()) {
                // We already have a candidate ready, which we opportunistically
                // pre-filled when we saw its inverse, back when j was smaller.
                usable[count++] = j;
                continue;
            }

            // Every member of the parent group that maps k -> j is of the form
            // parent.term_[k][j] * parent.term_[k-1][...] * ... .
            // Iterate through the subgroup
            // { parent.term_[k-1][...] * ...  * parent.term_[0][...] }
            // until we find a permutation that passes our membership test.

            // The iteration code below basically follows what the iterator
            // class does; see that class for further explanation.

            int pos[n];
            std::fill(pos, pos + k, 0);
            Perm<n> current;
            if constexpr (cached)
                current = parent.term_[k][j].cachedComp(parent.initSeq_[k - 1]);
            else
                current = parent.term_[k][j] * parent.initSeq_[k - 1];

            while (true) {
                if (test(current, std::forward<Args>(args)...)) {
                    // Found one!
                    term_[k][j] = current;
                    if (cached)
                        term_[j][k] = current.cachedInverse();
                    else
                        term_[j][k] = current.inverse();
                    usable[count++] = j;

                    // See if the inverse lets us fill in a later term
                    // that we won't have to compute.
                    int jInv = term_[j][k][k];
                    if (jInv > j) {
                        // Yes!  We will collect this when j reaches jInv.
                        term_[k][jInv] = term_[j][k];
                        term_[jInv][k] = term_[k][j];
                    }
                    break;
                }

                // Work out which pos_[i] needs to be incremented.
                int inc = 1;
                while (inc < k && pos[inc] == parent.count_[inc] - 1)
                    ++inc;
                if (inc == k) {
                    // Out of options.
                    usable[unusedSlot--] = j;
                    break;
                }

                if constexpr (cached)
                    current = current.cachedComp( /* rhs is inverse */
                        parent.term_[parent.usable_[inc][pos[inc]]][inc]);
                else
                    current = current * /* rhs is inverse */
                        parent.term_[parent.usable_[inc][pos[inc]]][inc];

                ++pos[inc];
                if constexpr (cached)
                    current = current.cachedComp(
                        parent.term_[inc][parent.usable_[inc][pos[inc]]]);
                else
                    current = current *
                        parent.term_[inc][parent.usable_[inc][pos[inc]]];

                if (inc > 1) {
                    std::fill(pos + 1, pos + inc, 0);
                    if constexpr (cached)
                        current = current.cachedComp(parent.initSeq_[inc - 1]);
                    else
                        current = current * parent.initSeq_[inc - 1];
                }
            }
        }

        // term_[k][k] is already (correctly) the identity.
        usable[count++] = k;

        count_[k] = count;
        usable_[k] = Perm<n>(usable);
    }

    setup();
}

template <int n, bool cached>
inline typename Perm<n>::Index PermGroup<n, cached>::size() const {
    using Index = typename Perm<n>::Index;
    Index ans = 1;
    for (int i = 1; i < n; ++i)
        ans *= static_cast<Index>(count_[i]);
    return ans;
}

template <int n, bool cached>
inline bool PermGroup<n, cached>::operator != (const PermGroup& other) const {
    return ! ((*this) == other);
}

template <int n, bool cached>
inline typename PermGroup<n, cached>::iterator PermGroup<n, cached>::begin()
        const {
    iterator ans(this);
    std::fill(ans.pos_, ans.pos_ + n, 0);
    ans.current_ = initSeq_[n - 1];
    return ans;
}

template <int n, bool cached>
inline typename PermGroup<n, cached>::iterator PermGroup<n, cached>::end()
        const {
    iterator ans(this);
    ans.pos_[0] = 1;
    // pos_[1..] and current_ may be left undefined.
    return ans;
}

template <int n, bool cached>
inline void PermGroup<n, cached>::writeTextShort(std::ostream& out) const {
    auto s = size();
    out << (s == 1 ? "Trivial" : s == Perm<n>::nPerms ? "Symmetric" :
        (s << 1) == Perm<n>::nPerms ? "Alternating" : "Permutation");
    out << " group of degree " << n << ", order " << s;
}

template <int n, bool cached>
inline void PermGroup<n, cached>::setup() {
    // initSeq_[0] is already (correctly) the identity.
    for (int k = 1; k < n; ++k)
        if (count_[k] == 1) {
            // The next term to multiply by is the identity.
            // Save the multiplication and just copy instead.
            initSeq_[k] = initSeq_[k - 1];
        } else {
            if (cached)
                initSeq_[k] =
                    term_[k][usable_[k][0]].cachedComp(initSeq_[k - 1]);
            else
                initSeq_[k] = term_[k][usable_[k][0]] * initSeq_[k - 1];
        }
}

} // namespace regina

#endif
