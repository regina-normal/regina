
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2026, Ben Burton                                   *
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

/*! \file enumerate/typetrie.h
 *  \brief A supporting data structure for tree traversal enumeration methods.
 */

#ifndef __REGINA_TYPETRIE_H
#ifndef __DOXYGEN
#define __REGINA_TYPETRIE_H
#endif

#include "core/output.h"
#include "utilities/fixedarray.h"
#include "utilities/intutils.h"
#include <cstdint>
#include <cstring>
#include <stack>
#include <vector>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * A trie that stores a set of type vectors of a fixed length.
 *
 * This class forms part of the tree traversal algorithm for enumerating
 * vertex normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.
 *
 * A type vector is a sequence of integers, each between 0 and `nTypes-1`
 * inclusive.  The length of a type vector must be passed alongside it
 * (i.e., there is no special terminating value).
 *
 * A type vector \a v is said to _dominate_ \a u if, for each position
 * \a i, either `v[i] == u[i]` or else `u[i] == 0`.  So, for instance,
 * `(1,0,2,3)` dominates `(1,0,2,0)`, which in turn dominates `(1,0,0,0)`.
 * Domination is a partial order, not a total order: for instance,
 * neither of `(1,0,2,0)` or `(1,0,3,0)` dominates the other.
 *
 * We assume that all type vectors used in this trie have the same length.
 * This is important, since we optimise the implementation by ignoring
 * trailing zeroes, which means that this trie cannot distinguish between a
 * vector \a v and the same vector with additional zeroes appended to its end.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  However, be aware
 * that the cost of moving is linear in the template parameter \a nTypes
 * (which, as noted below, is usually very small).
 *
 * \tparam nTypes specifies the range of possible values for the elements of
 * the vectors that are stored.  For normal surface enumeration, typical values
 * for \a nTypes would be 4 or 7 (depending upon whether we are supporting
 * almost normal surfaces).
 *
 * \python This is available only for the template parameters
 * \a nTypes = 4 and 7, under the names TypeTrie4 and TypeTrie7 respectively.
 *
 * \ingroup enumerate
 */
template <int nTypes> requires (nTypes >= 1)
class TypeTrie : public Output<TypeTrie<nTypes>> {
    private:
        /**
         * An individual node in this trie.
         */
        struct Node {
            Node* child_[nTypes];
                /**< If this node is \a k levels deeper than the root of
                     the trie (that is, it corresponds to the \a kth position
                     in the type vector), then child_[i] stores the subtrie
                     of type vectors \a v for which v[k] == i.
                     Every non-null child node corresponds to a non-empty
                     subtrie (see \a elementHere_ for details). */
            bool elementHere_;
                /**< \c true if the path from the root of the trie to this
                     node precisely describes the elements of some type
                     vector in the set, ignoring any trailing zeroes.
                     (In particular, the zero vector is in the set if and
                     only if \a elementHere_ is \c true at the root node.)
                     If this is \c false at a non-root node, then the fact
                     that the node was ever constructed means that the path
                     from the root to this node describes some _prefix_ of
                     a longer type vector in the set that has additional
                     subsequent non-zero elements. */

            /**
             * Creates a new node with no children and holding no elements.
             */
            Node();
            /**
             * Destroys this node and all its descendants.
             */
            ~Node();

            // Make this class non-copyable.
            Node(const Node&) = delete;
            Node& operator = (const Node&) = delete;
        };

        Node root_;
            /**< Stores the root node in this trie. */

    public:
        /**
         * Creates an empty trie.
         */
        TypeTrie() = default;

        /**
         * Creates a new copy of the given trie.
         * This will induce a deep copy of \a src.
         *
         * \param src the trie to copy.
         */
        TypeTrie(const TypeTrie& src);

        /**
         * Moves the contents of the given trie into this new trie.
         * This is operation is constant time in the size of the trie,
         * but linear time in the template parameter \a nTypes.
         *
         * The trie that was passed (\a src) will no longer be usable.
         *
         * \param src the trie whose contents should be moved.
         */
        TypeTrie(TypeTrie&& src) noexcept;

        /**
         * Sets this to be a copy of the given trie.
         * This will induce a deep copy of \a src.
         *
         * \param src the trie to copy.
         * \return a reference to this trie.
         */
        TypeTrie& operator = (const TypeTrie& src);

        /**
         * Moves the contents of the given trie into this trie.
         * This is operation is constant time in the size of the trie,
         * but linear time in the template parameter \a nTypes.
         *
         * The trie that was passed (\a src) will no longer be usable.
         *
         * \param src the trie whose contents should be moved.
         * \return a reference to this trie.
         */
        TypeTrie& operator = (TypeTrie&& src) noexcept;

        /**
         * Swaps the contents of this and the given trie.
         *
         * \param other the trie whose contents should be swapped with this.
         */
        void swap(TypeTrie& other) noexcept;

        /**
         * Determines whether this and the given trie store exactly the
         * same type vectors.
         *
         * \param other the trie to compare with this.
         * \return \c true if and only if both tries store the same type
         * vectors.
         */
        bool operator == (const TypeTrie& other) const;

        /**
         * Resets this to the empty trie.
         */
        void clear();

        /**
         * Inserts the given type vector into this trie.
         * The type vector is specified by an iterator range.
         *
         * \pre Each element of the given type vector is between 0 and
         * `nTypes-1` inclusive.
         *
         * \pre The length of the type vector (`endTypes - beginTypes`) is
         * fixed throughout the life of this trie; that is, it is the same
         * for every call to insert() or dominates().
         *
         * \python Instead of a pair of iterators, you should pass the type
         * vector as a single argument, which is a Python list of integers.
         *
         * \param beginTypes the beginning of the type vector to insert.
         * \param endTypes a past-the-end iterator indicating the end of the
         * type vector to insert.
         */
        template <std::bidirectional_iterator Iterator>
            requires CppInteger<std::iter_value_t<Iterator>>
        void insert(Iterator beginTypes, Iterator endTypes);

        /**
         * Determines whether the given type vector dominates any vector in
         * this trie.  The given type vector is specified by an iterator range.
         *
         * \pre Each element of the given type vector is between 0 and
         * `nTypes-1` inclusive.
         *
         * \pre The length of the type vector (`endTypes - beginTypes`) is
         * fixed throughout the life of this trie; that is, it is the same
         * for every call to insert() or dominates().
         *
         * \python Instead of a pair of iterators, you should pass the type
         * vector as a single argument, which is a Python list of integers.
         *
         * \param beginTypes the beginning of the type vector to test.
         * \param endTypes a past-the-end iterator indicating the end of the
         * type vector to test.
         * \return \c true if and only if the given type vector dominates some
         * type vector stored in this trie.
         */
        template <std::random_access_iterator Iterator>
            requires CppInteger<std::iter_value_t<Iterator>>
        bool dominates(Iterator beginTypes, Iterator endTypes) const;

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
 * Swaps the contents of the two given tries.
 *
 * \param a the first trie whose contents should be swapped.
 * \param b the second trie whose contents should be swapped.
 *
 * \ingroup enumerate
 */
template <int nTypes>
void swap(TypeTrie<nTypes>& a, TypeTrie<nTypes>& b) noexcept;

// Inline functions for TypeTrie

template <int nTypes> requires (nTypes >= 1)
inline TypeTrie<nTypes>::Node::Node() : elementHere_(false) {
    // NOLINTNEXTLINE(bugprone-sizeof-expression)
    ::memset(child_, 0, sizeof(Node*) * nTypes);
}

template <int nTypes> requires (nTypes >= 1)
inline TypeTrie<nTypes>::Node::~Node() {
    for (int i = 0; i < nTypes; ++i)
        delete child_[i];
}

template <int nTypes> requires (nTypes >= 1)
TypeTrie<nTypes>::TypeTrie(const TypeTrie& src) {
    // We don't know how deep the tree could get, so to avoid recursion
    // we use our own stack.
    std::stack<std::pair<Node*, const Node*>> toProcess;
    toProcess.push({&root_, &src.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        next.first->elementHere_ = next.second->elementHere_;
        for (int i = 0; i < nTypes; ++i)
            if (next.second->child_[i]) {
                next.first->child_[i] = new Node;
                toProcess.push({next.first->child_[i], next.second->child_[i]});
            }
    }
}

template <int nTypes> requires (nTypes >= 1)
inline TypeTrie<nTypes>::TypeTrie(TypeTrie&& src) noexcept {
    std::copy(src.root_.child_, src.root_.child_ + nTypes, root_.child_);
    std::fill(src.root_.child_, src.root_.child_ + nTypes, nullptr);
    root_.elementHere_ = src.root_.elementHere_;
}

template <int nTypes> requires (nTypes >= 1)
TypeTrie<nTypes>& TypeTrie<nTypes>::operator = (const TypeTrie& src) {
    for (int i = 0; i < nTypes; ++i) {
        delete root_.child_[i];
        root_.child_[i] = nullptr;
    }

    // Follow what we did with the copy constructor.
    std::stack<std::pair<Node*, const Node*>> toProcess;
    toProcess.push({&root_, &src.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        next.first->elementHere_ = next.second->elementHere_;
        for (int i = 0; i < nTypes; ++i)
            if (next.second->child_[i]) {
                next.first->child_[i] = new Node;
                toProcess.push({next.first->child_[i], next.second->child_[i]});
            }
    }

    return *this;
}

template <int nTypes> requires (nTypes >= 1)
inline TypeTrie<nTypes>& TypeTrie<nTypes>::operator = (TypeTrie&& src)
        noexcept {
    std::swap_ranges(root_.child_, root_.child_ + nTypes, src.root_.child_);
    root_.elementHere_ = src.root_.elementHere_;
    // Let src dispose of the original children in its own destructor.
    return *this;
}

template <int nTypes> requires (nTypes >= 1)
inline void TypeTrie<nTypes>::swap(TypeTrie& other) noexcept {
    std::swap_ranges(root_.child_, root_.child_ + nTypes, other.root_.child_);
    std::swap(root_.elementHere_, other.root_.elementHere_);
}

template <int nTypes> requires (nTypes >= 1)
bool TypeTrie<nTypes>::operator == (const TypeTrie& other) const {
    std::stack<std::pair<const Node*, const Node*>> toProcess;
    toProcess.push({&root_, &other.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        if (next.first->elementHere_ != next.second->elementHere_)
            return false;
        for (int i = 0; i < nTypes; ++i) {
            if (! next.first->child_[i]) {
                if (next.second->child_[i])
                    return false;
            } else {
                if (! next.second->child_[i])
                    return false;
                toProcess.push({next.first->child_[i], next.second->child_[i]});
            }
        }
    }
    return true;
}

template <int nTypes> requires (nTypes >= 1)
inline void TypeTrie<nTypes>::clear() {
    for (int i = 0; i < nTypes; ++i) {
        delete root_.child_[i];
        root_.child_[i] = nullptr;
    }
    root_.elementHere_ = false;
}

template <int nTypes> requires (nTypes >= 1)
template <std::bidirectional_iterator Iterator>
    requires CppInteger<std::iter_value_t<Iterator>>
void TypeTrie<nTypes>::insert(Iterator beginTypes, Iterator endTypes) {
    // Strip off trailing zeroes.
    while (endTypes != beginTypes && ! *std::prev(endTypes))
        --endTypes;

    // Insert this type vector, creating new nodes only when required.
    Node* node = &root_;
    while (beginTypes != endTypes) {
        if (! node->child_[*beginTypes])
            node->child_[*beginTypes] = new Node();
        node = node->child_[*beginTypes++];
    }
    node->elementHere_ = true;
}

template <int nTypes> requires (nTypes >= 1)
template <std::random_access_iterator Iterator>
    requires CppInteger<std::iter_value_t<Iterator>>
bool TypeTrie<nTypes>::dominates(Iterator beginTypes, Iterator endTypes) const {
    // Note: we need Iterator to be random access because further down we
    // compute the type vector length as (endTypes - beginTypes).

    // Strip off trailing zeroes.
    while (endTypes != beginTypes && ! *std::prev(endTypes))
        --endTypes;

    // At worst we have a recursive O(2^len) search on our hands.
    // Create a stack of options that describe which branch of the
    // trie we follow at each stage of the search.
    //
    // Here nodes[i] will store the next candidate node to try at
    // depth i in the tree (where the root is at depth 0), or null
    // if we have exhausted our options at that level of the search.
    FixedArray<const Node*> nodes(endTypes - beginTypes + 1);

    auto level = nodes.begin();
    auto type = beginTypes;

    *level = std::addressof(root_);
    while (true) {
        // INV: level and type are at the same index in their respective
        // sequences, with 0 ≤ index ≤ length of given type vector.
        // This means that level must be dereferenceable, but type could be
        // equal to endTypes.

        if (! *level) {
            // We ran out of options at this level - we need to backtrack.
            if (level == nodes.begin())
                return false;

            // Move back up one level...
            --level;
            --type;
            // ... and then across to the next sibling at this (higher) level.
            if (level != nodes.begin() &&
                    *level == (*std::prev(level))->child_[0] &&
                    *std::prev(type))
                *level = (*std::prev(level))->child_[*std::prev(type)];
            else
                *level = nullptr;
            continue;
        }

        // Is this node (padded with trailing zeroes) dominated by the given
        // type vector?
        if ((*level)->elementHere_)
            return true;

        if (type == endTypes) {
            // Any node deeper in this subtree must have non-zero elements
            // where the given vector only has zeros.  Therefore we cannot
            // continue this path through the tree: instead move on to the
            // next sibling at this level.
            if (level != nodes.begin() &&
                    *level == (*std::prev(level))->child_[0] &&
                    *std::prev(type))
                *level = (*std::prev(level))->child_[*std::prev(type)];
            else
                *level = nullptr;
            continue;
        }

        // Descend deeper into the tree.
        //
        // Our first option is always to descend to child_[0].
        // If the given vector has a non-zero type at the corresponding
        // position, we can also try descending to child_[type].
        //
        // The following code will descend to the first of these options that
        // is non-null (if there are any).  A null value here means there are
        // no usable options at all.
        auto next = (*level)->child_[0];
        if (! next)
            next = (*level)->child_[*type];
        ++level;
        ++type;
        *level = next;
    }

    return false;
}

template <int nTypes> requires (nTypes >= 1)
inline void TypeTrie<nTypes>::writeTextShort(std::ostream& out) const {
    if (nTypes == 1)
        out << "Trie for 1 type";
    else
        out << "Trie for " << nTypes << " types";
}

template <int nTypes> requires (nTypes >= 1)
void TypeTrie<nTypes>::writeTextLong(std::ostream& out) const {
    if (nTypes == 1)
        out << "Trie for 1 type:";
    else
        out << "Trie for " << nTypes << " types:";
    out << std::endl;

    // We don't know how deep the tree goes, so use our own stack.
    std::stack<const Node*> nodes;
    std::vector<int> types;
    int didChild = -1;
    nodes.push(&root_);
    while (true) {
        const Node* curr = nodes.top();
        if (didChild < 0 && curr->elementHere_) {
            // Output the current type vector.
            if (types.empty())
                out << "    0" << std::endl;
            else {
                out << "   ";
                for (auto i : types)
                    out << ' ' << i;
                out << std::endl;
            }
        }
        int i;
        for (i = didChild + 1; i < nTypes; ++i)
            if (curr->child_[i]) {
                types.push_back(i);
                nodes.push(curr->child_[i]);
                didChild = -1;
                break;
            }
        if (i == nTypes) {
            // We have exhausted this node.
            nodes.pop();
            if (nodes.empty())
                break;
            else {
                // This was not the root node, and so types is non-empty.
                didChild = types.back();
                types.pop_back();
            }
        }
    }
}

template <int nTypes>
inline void swap(TypeTrie<nTypes>& a, TypeTrie<nTypes>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

