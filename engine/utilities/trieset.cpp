
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <stack>
#include "utilities/trieset.h"

namespace regina {

TrieSet::TrieSet(const TrieSet& src) {
    // Meh.  We don't know how deep the tree could get, so it's either
    // recursion or we use our own stack.
    std::stack<std::pair<Node*, const Node*>> toProcess;
    toProcess.push({&root_, &src.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        next.first->descendants_ = next.second->descendants_;
        for (int i = 0; i < 2; ++i)
            if (next.second->child_[i]) {
                next.first->child_[i] = new Node;
                toProcess.push({next.first->child_[i], next.second->child_[i]});
            }
    }
}

TrieSet& TrieSet::operator = (const TrieSet& src) {
    delete root_.child_[0];
    delete root_.child_[1];

    // Follow what we did with the copy constructor.
    std::stack<std::pair<Node*, const Node*>> toProcess;
    toProcess.push({&root_, &src.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        next.first->descendants_ = next.second->descendants_;
        for (int i = 0; i < 2; ++i)
            if (next.second->child_[i]) {
                next.first->child_[i] = new Node;
                toProcess.push({next.first->child_[i], next.second->child_[i]});
            }
    }

    return *this;
}

} // namespace regina
