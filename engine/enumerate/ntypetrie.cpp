
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

#include "enumerate/ntypetrie.h"

namespace regina {

template <int nTypes>
void NTypeTrie<nTypes>::insert(const char* entry, unsigned len) {
    // Strip off trailing zeroes.
    while (len > 0 && ! entry[len - 1])
        --len;

    // Insert this type vector, creating new nodes only when required.
    NTypeTrie<nTypes>* node = this;
    const char* next = entry;
    for (int pos = 0; pos < len; ++pos, ++next) {
        if (! node->child_[*next])
            node->child_[*next] = new NTypeTrie<nTypes>();
        node = node->child_[*next];
    }
    node->elementHere_ = true;
}

template <int nTypes>
bool NTypeTrie<nTypes>::dominates(const char* vec, unsigned len) const {
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
    const NTypeTrie<nTypes>** node =
        new const NTypeTrie<nTypes>*[len + 2];

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

// Instantiate templates for standard cases.
template class NTypeTrie<7>;

} // namespace regina

