
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

/*! \file utilities/trieset.h
 *  \brief Provides a trie-like structure for storing sets.
 */

#ifndef __REGINA_TRIESET_H
#ifndef __DOXYGEN
#define __REGINA_TRIESET_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "utilities/bitmask.h"

namespace regina {

/**
 * A trie-like data structure for storing and retriving sets.
 * This class is useful when the _elements_ of these sets are taken from a
 * fairly small universe, but where the _number_ of sets being stored
 * can be extremely large.
 *
 * For simplicity, let the universe consist of the integers 0,...,(<i>n</i>-1).
 * Sets are represented as bitmasks of length \a n (using one of Regina's
 * bitmask types, such as Bitmask, Bitmask1 or Bitmask2).  The <i>i</i>th bit
 * of a bitmask indicates whether the integer \a i belongs to the corresponding
 * set.
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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup utilities
 */
class TrieSet : public Output<TrieSet> {
    private:
        /**
         * An individual node in this trie.
         */
        struct Node {
            Node* child_[2];
                /**< Stores the two child nodes that appear beneath this
                     node in the tree.  If \c P is the prefix corresponding
                     to this node, then the two child nodes will correspond to
                     prefixes \c P0 and \c P1 respectively.  If there are no
                     sets stored at or beneath a child node, then the
                     corresponding child pointer will be \c null. */
            size_t descendants_;
                /**< The number of sets stored at or beneath this node in
                     the tree.  The number of sets stored _precisely_ at
                     this node can be computed by subtracting the descendant
                     counts for each child node. */

            /**
             * Constructs an empty node.
             */
            Node();
            /**
             * Constructs a node filled with the given data.
             */
            Node(Node* child0, Node* child1, size_t descendants);
            /**
             * Destroys this node and all its descendants.
             */
            ~Node();

            // Make this class non-copyable.
            Node(const Node&) = delete;
            Node& operator = (const Node&) = delete;
        };

        Node root_;
            /**< Stores the root node in this tree. */

    public:
        /**
         * Constructs an empty collection of sets.
         */
        TrieSet() = default;

        /**
         * Creates a new copy of the given collection.
         * This will induce a deep copy of \a src.
         *
         * \param src the collection of sets to copy.
         */
        TrieSet(const TrieSet& src);

        /**
         * Moves the contents of the given collection into this new collection.
         * This is a fast (constant time) operation.
         *
         * The collection that was passed (\a src) will no longer be usable.
         *
         * \param src the collection of sets whose contents should be moved.
         */
        TrieSet(TrieSet&& src) noexcept;

        /**
         * Sets this to be a copy of the given collection.
         * This will induce a deep copy of \a src.
         *
         * \param src the collection of sets to copy.
         * \return a reference to this collection.
         */
        TrieSet& operator = (const TrieSet& src);

        /**
         * Moves the contents of the given collection into this collection.
         * This is a fast (constant time) operation.
         *
         * The collection that was passed (\a src) will no longer be usable.
         *
         * \param src the collection of sets whose contents should be moved.
         * \return a reference to this collection.
         */
        TrieSet& operator = (TrieSet&& src) noexcept;

        /**
         * Swaps the contents of this and the given collection.
         *
         * \param other the collection whose contents should be swapped
         * with this.
         */
        void swap(TrieSet& other) noexcept;

        /**
         * Determines whether this and the given collection store
         * exactly the same sets.
         *
         * \param other the collection to compare with this.
         * \return \c true if and only if both collections store the same sets.
         */
        bool operator == (const TrieSet& other) const;

        /**
         * Insert the given set into this collection.  The same set may
         * be insert into this collection multiple times (and this
         * multiplicity will be recorded correctly).
         *
         * Running time for insertion is O(\a n), where \a n is the
         * bitmask length.
         *
         * \tparam T One of Regina's bitmask types, such as Bitmask, Bitmask1
         * or Bitmask2.
         *
         * \param entry the new set to insert.
         */
        template <typename T>
        void insert(const T& entry);

        /**
         * Determines whether this collection of sets contains any subset
         * of the argument \a superset.
         * Subsets need not be _proper_ subsets (so if an exact copy of
         * \a superset is found in the tree then this will suffice).
         *
         * This routine has a slow running time, which in
         * pathological cases can grow to either `2^n`
         * (where \a n is the bitmask length) or the number of sets
         * stored in this collection, whichever is smaller.  However,
         * for "typical" searches in the context of normal surface
         * enumeration, the running time is often significantly faster.
         *
         * \tparam T One of Regina's bitmask types, such as Bitmask, Bitmask1
         * or Bitmask2.
         *
         * \param superset the object of the query: we are searching this
         * collection for a (non-strict) subset of this argument.
         * \param universeSize the number of elements in the underlying
         * universe (and therefore the lowest possible level in the
         * search tree).  This must be less than or equal to the number of
         * bits that the underlying bitmask type \a T can support.
         * \return \c true if a subset was found, or \c false otherwise.
         */
        template <typename T>
        bool hasSubset(const T& superset, size_t universeSize) const;

        /**
         * Performs the particular superset search required by the double
         * description method.
         *
         * This routine asks the following question:  In this collection
         * of sets, is there any superset of the argument \a subset
         * _other_ than \a exc1 or \a exc2?  Here the sets \a exc1 and
         * \a exc2 are explicitly excluded from our search.  Supersets
         * need not be _proper_ supersets (so if an exact copy of
         * \a subset is found in the tree then this will suffice).
         *
         * This routine has a slow running time, which in
         * pathological cases can grow to either `2^n`
         * (where \a n is the bitmask length) or the number of sets
         * stored in this collection, whichever is smaller.  However,
         * for "typical" searches in the context of normal surface
         * enumeration, the running time is often significantly faster.
         *
         * \pre The sets \a exc1 and \a exc2 are distinct, and each is
         * contained in this collection precisely once.
         *
         * \tparam T One of Regina's bitmask types, such as Bitmask, Bitmask1
         * or Bitmask2.
         *
         * \param subset the object of the query: we are searching this
         * collection for a (non-strict) superset of this argument.
         * \param exc1 the first set in the collection to be excluded
         * from this search.
         * \param exc2 the second set in the collection to be excluded
         * from this search.
         * \param universeSize the number of elements in the underlying
         * universe (and therefore the lowest possible level in the
         * search tree).  This must be less than or equal to the number of
         * bits that the underlying bitmask type \a T can support.
         * \return \c true if a superset with the required properties
         * was found, or \c false otherwise.
         */
        template <typename T>
        bool hasExtraSuperset(const T& subset, const T& exc1, const T& exc2,
            size_t universeSize) const;

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
};

/**
 * Swaps the contents of the two given collections.
 *
 * \param a the first collection of sets whose contents should be swapped.
 * \param b the second collection of sets whose contents should be swapped.
 *
 * \ingroup utilities
 */
void swap(TrieSet& a, TrieSet& b) noexcept;

// Inline functions and template implementations for TrieSet

inline TrieSet::Node::Node() : child_ { nullptr, nullptr }, descendants_(0) {
}

inline TrieSet::Node::Node(Node* child0, Node* child1, size_t descendants) :
        child_ { child0, child1 }, descendants_(descendants) {
}

inline TrieSet::Node::~Node() {
    delete child_[0];
    delete child_[1];
}

inline TrieSet::TrieSet(TrieSet&& src) noexcept :
        root_(src.root_.child_[0], src.root_.child_[1],
            src.root_.descendants_) {
    src.root_.child_[0] = src.root_.child_[1] = nullptr;
}

inline TrieSet& TrieSet::operator = (TrieSet&& src) noexcept {
    std::swap(root_.child_[0], src.root_.child_[0]);
    std::swap(root_.child_[1], src.root_.child_[1]);
    root_.descendants_ = src.root_.descendants_;
    // Let src.root_ dispose of the original children in its own destructor.
    return *this;
}

inline void TrieSet::swap(TrieSet& other) noexcept {
    std::swap(root_.child_[0], other.root_.child_[0]);
    std::swap(root_.child_[1], other.root_.child_[1]);
    std::swap(root_.descendants_, other.root_.descendants_);
}

template <typename T>
void TrieSet::insert(const T& entry) {
    ++root_.descendants_;

    ssize_t last = entry.lastBit();
    if (last < 0)
        return;

    Node* node = &root_;
    for (ssize_t pos = 0; pos <= last; ++pos) {
        if (entry.get(pos)) {
            // Follow right branch.
            if (! node->child_[1])
                node->child_[1] = new Node;
            node = node->child_[1];
        } else {
            // Follow left branch.
            if (! node->child_[0])
                node->child_[0] = new Node;
            node = node->child_[0];
        }
        ++node->descendants_;
    }
}

template <typename T>
bool TrieSet::hasSubset(const T& superset, size_t universeSize) const {
    const Node** node = new const Node*[universeSize + 2];

    ssize_t level = 0;
    node[0] = &root_;
    while (level >= 0) {
        if (! node[level]) {
            // We ran out of siblings at this level.  Move up.
            --level;
            // Move to the next sibling at this (higher) level.
            if (level > 0 && node[level] == node[level - 1]->child_[1])
                node[level] = node[level - 1]->child_[0];
            else if (level >= 0)
                node[level] = nullptr;
            continue;
        }

        // Process the node at the current level.
        if (level >= static_cast<ssize_t>(universeSize)) {
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
bool TrieSet::hasExtraSuperset(const T& subset,
        const T& exc1, const T& exc2, size_t universeSize) const {
    const Node** node = new const Node*[universeSize + 2];

    ssize_t last = subset.lastBit();

    ssize_t level = 0;
    ssize_t prefixOfExc1 = 0; // Last layer for which this is true.
    ssize_t prefixOfExc2 = 0; // Last layer for which this is true.

    node[0] = &root_;

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
                node[level] = nullptr;
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
            node[level] = nullptr;
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

inline void swap(TrieSet& a, TrieSet& b) noexcept {
    a.swap(b);
}

inline void TrieSet::writeTextShort(std::ostream& out) const {
    if (root_.descendants_ == 1) {
        out << "Trie containing 1 set";
    } else {
        out << "Trie containing " << root_.descendants_ << " sets";
    }
}

} // namespace regina

#endif

