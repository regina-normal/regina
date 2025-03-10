
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <stack>
#include <vector>
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
    root_.child_[0] = root_.child_[1] = nullptr;

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

bool TrieSet::operator == (const TrieSet& other) const {
    std::stack<std::pair<const Node*, const Node*>> toProcess;
    toProcess.push({&root_, &other.root_});
    while (! toProcess.empty()) {
        auto next = toProcess.top();
        toProcess.pop();

        if (next.first->descendants_ != next.second->descendants_)
            return false;
        for (int i = 0; i < 2; ++i) {
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

void TrieSet::writeTextLong(std::ostream& out) const {
    out << "Trie containing ";
    if (root_.descendants_ == 0) {
        out << "0 sets." << std::endl;
        return;
    }

    if (root_.descendants_ == 1)
        out << "1 set:" << std::endl;
    else
        out << root_.descendants_ << " sets:" << std::endl;

    // We don't know how deep the tree goes, so use our own stack.
    std::stack<const Node*> nodes;
    std::vector<size_t> elts;
    const Node* didChild = nullptr;

    // We will process the right branch of each child before the left, since
    // this will result in the sets being output in lexicographical order.
    nodes.push(&root_);
    while (! nodes.empty()) {
        const Node* curr = nodes.top();
        if (! didChild) {
            // We are visiting this node for the first time.
            size_t beneath =
                (curr->child_[0] ? curr->child_[0]->descendants_ : 0) +
                (curr->child_[1] ? curr->child_[1]->descendants_ : 0);
            if (curr->descendants_ > beneath) {
                for (size_t i = beneath; i < curr->descendants_; ++i) {
                    out << "    {";
                    bool first = true;
                    for (auto elt : elts) {
                        if (first)
                            first = false;
                        else
                            out << ',';
                        out << ' ' << elt;
                    }
                    out << " }" << std::endl;
                }
            }
            if (curr->child_[1]) {
                elts.push_back(nodes.size() - 1);
                nodes.push(curr->child_[1]);
                didChild = nullptr;
            } else if (curr->child_[0]) {
                nodes.push(curr->child_[0]);
                didChild = nullptr;
            } else {
                didChild = curr;
                nodes.pop();
            }
        } else if (didChild == curr->child_[1]) {
            // We have just finished processing the right child.
            if (elts.back() == nodes.size() - 1)
                elts.pop_back();

            // Now do the left branch, if that branch exists.
            if (curr->child_[0]) {
                nodes.push(curr->child_[0]);
                didChild = nullptr;
            } else {
                didChild = curr;
                nodes.pop();
            }
        } else {
            // We have just finished processing the left child.
            // We are done with this part of the tree.
            didChild = curr;
            nodes.pop();
        }
    }
}

} // namespace regina
