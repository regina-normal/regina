
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2014, Ben Burton                                   *
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

/* end stub */

/*! \file enumerate/ntypetrie.h
 *  \brief A supporting data structure for tree traversal enumeration methods.
 */

#ifndef __NTYPETRIE_H
#ifndef __DOXYGEN
#define __NTYPETRIE_H
#endif

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
 * NTypeTrie object, each of which is responsible for managing the
 * lifespan of its descendant nodes.  Externally, a user only needs
 * to create and manage a single NTypeTrie object (which becomes
 * the root of the trie).
 *
 * \pre \a nTypes is at most 256.  The typical value for \a nTypes for
 * normal surface enumeration is \a nTypes = 4.
 *
 * \ifacespython Not present.
 */
template <int nTypes>
class NTypeTrie {
    private:
        NTypeTrie<nTypes>* child_[nTypes];
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
        inline NTypeTrie();

        /**
         * Destroys this trie.
         */
        inline ~NTypeTrie();

        /**
         * Resets this to the empty trie.
         */
        inline void clear();

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

/*@}*/

// Help the compiler by noting which explicit instantiations we offer.
extern template class REGINA_API NTypeTrie<7>;

// Inline functions for NTypeTrie

template <int nTypes>
inline NTypeTrie<nTypes>::NTypeTrie() : elementHere_(false) {
    ::memset(child_, 0, sizeof(NTypeTrie<nTypes>*) * nTypes);
}

/**
 * Destroys this trie.
 */
template <int nTypes>
inline NTypeTrie<nTypes>::~NTypeTrie() {
    for (int i = 0; i < nTypes; ++i)
        delete child_[i];
}

/**
 * Resets this to the empty trie.
 */
template <int nTypes>
inline void NTypeTrie<nTypes>::clear() {
    for (int i = 0; i < nTypes; ++i) {
        delete child_[i];
        child_[i] = 0;
    }
    elementHere_ = false;
}

} // namespace regina

#endif

