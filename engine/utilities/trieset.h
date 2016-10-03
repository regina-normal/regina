
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

/*! \file utilities/trieset.h
 *  \brief Provides a trie-like structure for storing sets.
 */

#ifndef __TRIESET_H
#ifndef __DOXYGEN
#define __TRIESET_H
#endif

#include "regina-core.h"
#include "utilities/bitmask.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A trie-like data structure for storing and retriving sets.
 * This class is useful when the \e elements of these sets are taken from a
 * fairly small universe, but where the \e number of sets being stored
 * can be extremely large.
 *
 * For simplicity, let the universe consist of the integers 0,...,\a n.
 * Sets are represented as bitmasks of type \a T (which must be capable
 * of holding bitmasks of length \a n).  The <i>i</i>th bit of a bitmask
 * indicates whether the integer \a i belongs to the corresponding set.
 *
 * To construct an empty trie, simply call the default constructor.
 * To insert a new set into the trie, call insert() (whose running time is
 * linear in \a n).  You can insert the same set into the trie multiple times
 * and the trie will record the number of times that it occurs.
 *
 * Currently the only searching operations are hasSubset() and
 * hasExtraSuperset().  These operations are slow, but still
 * much faster than searching through a linear list; see the
 * hasSubset() and hasExtraSuperset() documentation for details.
 *
 * The implementation of this data structure uses a binary tree with
 * depth levels 0,...,\a n, where each node at level \a i represents a
 * potential length-\a i prefix for a bitmask.  So, for instance,
 * the root node represents the empty prefix, its children represent
 * prefixes 0 and 1, their children represent prefixes 00, 01, 10 and 11,
 * and so on.
 *
 * Internally, a set is "stored" at the first node whose prefix
 * in fact describes the entire set.  For instance, the bitmask
 * 101100 is stored at the node corresponding to the prefix 1011,
 * which occurs at level 3 of the tree.  Regions of the tree that do
 * not store any sets are never explicitly constructed in memory.
 *
 * \pre The template argument T is one of Regina's bitmask types, such
 * as Bitmask, Bitmask1 or Bitmask2.
 *
 * \ifacespython Not present.
 */
template <typename T>
class TrieSet {
    private:
        TrieSet* child_[2];
            /**< Stores the two child nodes that appear beneath this
                 node in the tree.  If \c P is the prefix corresponding
                 to this node, then the two child nodes will correspond to
                 prefixes \c P0 and \c P1 respectively.  If there are no
                 sets stored at or beneath a child node, then the
                 corresponding child pointer will be \c null. */
        unsigned long descendants_;
            /**< The number of sets stored at or beneath this node in
                 the tree.  The number of sets stored \e precisely at
                 this node can be computed by subtracting the descendant
                 counts for each child node. */

    public:
        /**
         * Constructs an empty collection of sets.
         */
        TrieSet();
        /**
         * Destroys this collection of sets.
         */
        ~TrieSet();

        /**
         * Insert the given set into this collection.  The same set may
         * be insert into this collection multiple times (and this
         * multiplicity will be recorded correctly).
         *
         * Running time for insertion is O(\a n), where \a n is the
         * bitmask length.
         *
         * \param entry the new set to insert.
         */
        void insert(const T& entry);

        /**
         * Determines whether this collection of sets contains any subset
         * of the argument \a superset.
         * Subsets need not be \e proper subsets (so if an exact copy of
         * \a superset is found in the tree then this will suffice).
         *
         * This routine has a slow running time, which in
         * pathological cases can grow to either <tt>2^n</tt>
         * (where \a n is the bitmask length) or the number of sets
         * stored in this collection, whichever is smaller.  However,
         * for "typical" searches in the context of normal surface
         * enumeration, the running time is often significantly faster.
         *
         * \param superset the object of the query: we are searching this
         * collection for a (non-strict) subset of this argument.
         * \param universeSize the number of elements in the underlying
         * universe (and therefore the lowest possible level in the
         * search tree).  Note that this is always less than or equal to
         * the number of bits that the underlying bitmask type \a T
         * can support.
         * \return \c true if a subset was found, or \c false otherwise.
         */
        bool hasSubset(const T& superset, unsigned long universeSize) const;

        /**
         * Performs the particular superset search required by the double
         * description method.
         *
         * This routine asks the following question:  In this collection
         * of sets, is there any superset of the argument \a subset
         * \e other than \a exc1 or \a exc2?  Here the sets \a exc1 and
         * \a exc2 are explicitly excluded from our search.  Supersets
         * need not be \e proper supersets (so if an exact copy of
         * \a subset is found in the tree then this will suffice).
         *
         * This routine has a slow running time, which in
         * pathological cases can grow to either <tt>2^n</tt>
         * (where \a n is the bitmask length) or the number of sets
         * stored in this collection, whichever is smaller.  However,
         * for "typical" searches in the context of normal surface
         * enumeration, the running time is often significantly faster.
         *
         * \pre The sets \a exc1 and \a exc2 are distinct, and each is
         * contained in this collection precisely once.
         *
         * \param subset the object of the query: we are searching this
         * collection for a (non-strict) superset of this argument.
         * \param exc1 the first set in the collection to be excluded
         * from this search.
         * \param exc2 the second set in the collection to be excluded
         * from this search.
         * \param universeSize the number of elements in the underlying
         * universe (and therefore the lowest possible level in the
         * search tree).  Note that this is always less than or equal to
         * the number of bits that the underlying bitmask type \a T
         * can support.
         * \return \c true if a superset with the required properties
         * was found, or \c false otherwise.
         */
        bool hasExtraSuperset(const T& subset, const T& exc1, const T& exc2,
            unsigned long universeSize) const;
};

/*@}*/

// Inline functions and template implementations for TrieSet

template <typename T>
inline TrieSet<T>::TrieSet() : descendants_(0) {
    child_[0] = child_[1] = 0;
}

template <typename T>
inline TrieSet<T>::~TrieSet() {
    delete child_[0];
    delete child_[1];
}

template <typename T>
void TrieSet<T>::insert(const T& entry) {
    ++descendants_;

    long last = entry.lastBit();
    if (last < 0)
        return;

    TrieSet<T>* node = this;
    for (long pos = 0; pos <= last; ++pos) {
        if (entry.get(pos)) {
            // Follow right branch.
            if (! node->child_[1])
                node->child_[1] = new TrieSet<T>();
            node = node->child_[1];
        } else {
            // Follow left branch.
            if (! node->child_[0])
                node->child_[0] = new TrieSet<T>();
            node = node->child_[0];
        }
        ++node->descendants_;
    }
}

template <typename T>
bool TrieSet<T>::hasSubset(const T& superset, unsigned long universeSize)
        const {
    const TrieSet<T>** node = new const TrieSet<T>*[universeSize + 2];

    long level = 0;
    node[0] = this;
    while (level >= 0) {
        if (! node[level]) {
            // We ran out of siblings at this level.  Move up.
            --level;
            // Move to the next sibling at this (higher) level.
            if (level > 0 && node[level] == node[level - 1]->child_[1])
                node[level] = node[level - 1]->child_[0];
            else if (level >= 0)
                node[level] = 0;
            continue;
        }

        // Process the node at the current level.
        if (level >= universeSize) {
            // Our subtree is now a subset of the given superset.
            delete[] node;
            return true;
        }

        // Descend further into the tree.
        if ((! superset.get(level)) || (! node[level]->child_[1]))
            node[level + 1] = node[level]->child_[0];
        else
            node[level + 1] = node[level]->child_[1];
        ++level;
    }

    delete[] node;
    return false;
}

template <typename T>
bool TrieSet<T>::hasExtraSuperset(const T& subset,
        const T& exc1, const T& exc2, unsigned long universeSize) const {
    const TrieSet<T>** node = new const TrieSet<T>*[universeSize + 2];

    long last = subset.lastBit();

    long level = 0;
    long prefixOfExc1 = 0; // Last layer for which this is true.
    long prefixOfExc2 = 0; // Last layer for which this is true.

    node[0] = this;

    while (level >= 0) {
        if (! node[level]) {
            // We ran out of siblings at this level.  Move up.
            if (prefixOfExc1 == level)
                --prefixOfExc1;
            if (prefixOfExc2 == level)
                --prefixOfExc2;
            --level;
            // Move to the next sibling at this (higher) level.
            if (level > 0 && node[level] == node[level - 1]->child_[0]) {
                node[level] = node[level - 1]->child_[1];
                if (prefixOfExc1 == level)
                    --prefixOfExc1;
                else if (prefixOfExc1 == level - 1 && exc1.get(level - 1))
                    ++prefixOfExc1;
                if (prefixOfExc2 == level)
                    --prefixOfExc2;
                else if (prefixOfExc2 == level - 1 && exc2.get(level - 1))
                    ++prefixOfExc2;
            } else if (level >= 0)
                node[level] = 0;
            continue;
        }

        // Process the node at the current level.
        if (level > last) {
            // Our subtree is now a superset of the given subset.
            unsigned junk = (prefixOfExc1 == level ? 1 : 0) +
                (prefixOfExc2 == level ? 1 : 0);
            if (node[level]->descendants_ > junk) {
                delete[] node;
                return true;
            }

            // Back up.
            node[level] = 0;
            continue;
        }

        // Descend further into the tree.
        if (subset.get(level) || ! node[level]->child_[0]) {
            node[level + 1] = node[level]->child_[1];
            if (prefixOfExc1 == level && exc1.get(level))
                ++prefixOfExc1;
            if (prefixOfExc2 == level && exc2.get(level))
                ++prefixOfExc2;
        } else {
            node[level + 1] = node[level]->child_[0];
            if (prefixOfExc1 == level && ! exc1.get(level))
                ++prefixOfExc1;
            if (prefixOfExc2 == level && ! exc2.get(level))
                ++prefixOfExc2;
        }
        ++level;
    }

    delete[] node;
    return false;
}

} // namespace regina

#endif

