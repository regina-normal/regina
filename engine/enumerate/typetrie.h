
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2016, Ben Burton                                   *
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

/*! \file enumerate/typetrie.h
 *  \brief A supporting data structure for tree traversal enumeration methods.
 */

#ifndef __TYPETRIE_H
#ifndef __DOXYGEN
#define __TYPETRIE_H
#endif

#include "regina-core.h"
#include <cstring>

namespace regina {

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * A trie that stores a set of type vectors of a fixed length.
 *
 * This class forms part of the tree traversal algorithm for enumerating
 * vertex normal surfaces, as described in "A tree traversal algorithm
 * for decision problems in knot theory and 3-manifold topology",
 * Burton and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.
 *
 * A type vector is a sequence of digits, each between 0 and \a nTypes-1
 * inclusive.  Type vectors are represented as arrays of characters:
 * these are not strings, but simply sequences of one-byte integers.
 * In particular, you cannot print them (since they use raw integer
 * values, not ASCII digits).  The length of a type vector must be
 * passed alongside it (i.e., there is no special terminating character).
 *
 * A type vector \a v is said to \e dominate \a u if, for each position
 * \a i, either v[i] == u[i] or else u[i] == 0.  So, for instance,
 * (1,0,2,3) dominates (1,0,2,0), which in turn dominates (1,0,0,0).
 * Domination is a partial order, not a total order: for instance,
 * neither of (1,0,2,0) or (1,0,3,0) dominates the other.
 *
 * We assume that all type vectors used in this trie have the same
 * length.  This is important, since we optimise the implementation by
 * ignoring trailing zeroes, which means that this trie cannot distinguish
 * between a vector \a v and the same vector with additional zeroes
 * appended to its end.
 *
 * Internally, each node of the trie is represented by a separate
 * TypeTrie object, each of which is responsible for managing the
 * lifespan of its descendant nodes.  Externally, a user only needs
 * to create and manage a single TypeTrie object (which becomes
 * the root of the trie).
 *
 * \pre \a nTypes is at most 256.  The typical value for \a nTypes for
 * normal surface enumeration is \a nTypes = 4.
 *
 * \ifacespython Not present.
 */
template <int nTypes>
class TypeTrie {
    private:
        TypeTrie<nTypes>* child_[nTypes];
            /**< If this node is \a k levels deeper than the root of
                 the trie (that is, it corresponds to the \a kth position
                 in the type vector), then child_[i] stores the subtrie
                 of type vectors \a v for which v[k] == i. */
        bool elementHere_;
            /**< \c true if the path from the root of the trie to this
                 node precisely describes the elements of some type
                 vector in the set, ignoring any trailing zeroes.
                 (In particular, the zero vector is in the set if and
                 only if \a elementHere_ is \c true at the root node.)
                 If this is \c false at a non-root node, then the fact
                 that the node was ever constructed means that the path
                 from the root to this node describes some \e prefix of
                 a longer type vector in the set that has additional
                 subsequent non-zero elements. */

    public:
        /**
         * Initialises an empty trie.
         */
        TypeTrie();

        /**
         * Destroys this trie.
         */
        ~TypeTrie();

        /**
         * Resets this to the empty trie.
         */
        void clear();

        /**
         * Inserts the given type vector into this trie.
         *
         * \pre The given length \a len is non-zero, and is fixed throughout
         * the life of this trie; that is, it is the same every time
         * insert() or dominates() is called.
         *
         * @param entry the type vector to insert.
         * @param len the number of elements in the given type vector.
         */
        void insert(const char* entry, unsigned len);

        /**
         * Determines whether the given type vector dominates any vector
         * in this trie.
         *
         * \pre The given length \a len is non-zero, and is fixed throughout
         * the life of this trie; that is, it is the same every time
         * insert() or dominates() is called.
         *
         * @param vec the type vector to test.
         * @param len the number of elements in the given type vector.
         * @return \c true if and only if \a vec dominates some type
         * vector stored in this trie.
         */
        bool dominates(const char* vec, unsigned len) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTypeTrie has now been renamed to TypeTrie.
 */
template <int nTypes>
using NTypeTrie REGINA_DEPRECATED = TypeTrie<nTypes>;

/*@}*/

// Help the compiler by noting which explicit instantiations we offer.
extern template class REGINA_API TypeTrie<7>;

// Inline functions for TypeTrie

template <int nTypes>
inline TypeTrie<nTypes>::TypeTrie() : elementHere_(false) {
    ::memset(child_, 0, sizeof(TypeTrie<nTypes>*) * nTypes);
}

template <int nTypes>
inline TypeTrie<nTypes>::~TypeTrie() {
    for (int i = 0; i < nTypes; ++i)
        delete child_[i];
}

template <int nTypes>
inline void TypeTrie<nTypes>::clear() {
    for (int i = 0; i < nTypes; ++i) {
        delete child_[i];
        child_[i] = 0;
    }
    elementHere_ = false;
}

template <int nTypes>
void TypeTrie<nTypes>::insert(const char* entry, unsigned len) {
    // Strip off trailing zeroes.
    while (len > 0 && ! entry[len - 1])
        --len;

    // Insert this type vector, creating new nodes only when required.
    TypeTrie<nTypes>* node = this;
    const char* next = entry;
    for (int pos = 0; pos < len; ++pos, ++next) {
        if (! node->child_[*next])
            node->child_[*next] = new TypeTrie<nTypes>();
        node = node->child_[*next];
    }
    node->elementHere_ = true;
}

template <int nTypes>
bool TypeTrie<nTypes>::dominates(const char* vec, unsigned len) const {
    // Strip off trailing zeroes.
    while (len > 0 && ! vec[len - 1])
        --len;

    // At worst we have a recursive O(2^len) search on our hands.
    // Create a stack of options that describe which branch of the
    // trie we follow at each stage of the search.
    //
    // Here node[i] will store the next candidate node to try at
    // depth i in the tree (where the root is at depth 0), or 0
    // if we have exhausted our options at that level of the search.
    const TypeTrie<nTypes>** node =
        new const TypeTrie<nTypes>*[len + 2];

    int level = 0;
    node[0] = this;
    while (level >= 0) {
        if ((! node[level]) || level > len) {
            // If node[level] is 0, then we ran out of siblings
            // at this level.
            // If level > len, then any vector in this subtree
            // must have non-zero elements where vec only has zeros.
            // Either way, we need to backtrack.

            // Move back up one level...
            --level;
            // ... and then move to the next sibling at this (higher)
            // level.
            if (level > 0 &&
                    node[level] == node[level - 1]->child_[0] &&
                    vec[level - 1])
                node[level] = node[level - 1]->child_[vec[level - 1]];
            else if (level >= 0)
                node[level] = 0;
            continue;
        }

        // Process the node at the current level.
        if (node[level]->elementHere_) {
            // This node (padded with trailing zeroes) is
            // dominated by the given type vector.
            delete[] node;
            return true;
        }

        // Descend further into the tree.
        //
        // If vec[level] == 0, we must descend to child_[0].
        // Otherwise we try child_[0] and then child_[type].
        //
        // The following code sets node[level + 1] to the first non-zero
        // child in this selection, or to 0 if all such children are 0.
        if (node[level]->child_[0])
            node[level + 1] = node[level]->child_[0];
        else
            node[level + 1] = node[level]->child_[vec[level]];
        ++level;
    }

    delete[] node;
    return false;
}

} // namespace regina

#endif

