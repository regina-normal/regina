
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

#include <algorithm>
#include "link/link.h"
#include "treewidth/treedecomposition.h"
#include "treewidth/treedecomposition-impl.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "utilities/memutils.h"

namespace regina {

// Instantiate templates for standard dimensions:
template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<2>&, TreeDecompositionAlg);
template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<3>&, TreeDecompositionAlg);
template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<4>&, TreeDecompositionAlg);

template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<2>&, TreeDecompositionAlg);
template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<3>&, TreeDecompositionAlg);
template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<4>&, TreeDecompositionAlg);

// Instantiate templates for common types:
template REGINA_API TreeDecomposition::TreeDecomposition(
    unsigned, bool const** const, TreeDecompositionAlg);
template REGINA_API TreeDecomposition::TreeDecomposition(
    unsigned, int const** const, TreeDecompositionAlg);

template REGINA_API void TreeDecomposition::reroot(
    const int*, const int*, const int*);

bool TreeBag::contains(int element) const {
    return std::binary_search(elements_, elements_ + size_, element);
}

BagComparison TreeBag::compare(const TreeBag& rhs) const {
    int p1 = 0;
    int p2 = 0;
    bool extraInLHS = false;
    bool extraInRHS = false;

    while (p1 < size_ && p2 < rhs.size_) {
        if (elements_[p1] == rhs.elements_[p2]) {
            ++p1;
            ++p2;
            continue;
        } else if (elements_[p1] < rhs.elements_[p2]) {
            ++p1;
            if (extraInRHS)
                return BAG_UNRELATED;
            extraInLHS = true;
        } else {
            ++p2;
            if (extraInLHS)
                return BAG_UNRELATED;
            extraInRHS = true;
        }
    }

    if (p1 < size_)
        return (extraInRHS ? BAG_UNRELATED : BAG_SUPERSET);
    if (p2 < rhs.size_)
        return (extraInLHS ? BAG_UNRELATED : BAG_SUBSET);
    return (extraInLHS ? BAG_SUPERSET : extraInRHS ? BAG_SUBSET : BAG_EQUAL);
}

const TreeBag* TreeBag::nextPrefix() const {
    if (children_)
        return children_;

    const TreeBag* b = this;
    while (b && ! b->sibling_)
        b = b->parent_;
    return (b ? b->sibling_ : nullptr);
}

const TreeBag* TreeBag::next() const {
    if (! sibling_)
        return parent_;

    const TreeBag* b = sibling_;
    while (b && b->children_)
        b = b->children_;
    return b;
}

void TreeBag::writeTextShort(std::ostream& out) const {
    if (size_ == 1)
        out << "Bag of 1 element:";
    else
        out << "Bag of " << size_ << " elements:";

    for (int i = 0; i < size_; ++i)
        out << ' ' << elements_[i];
}

void TreeDecomposition::Graph::dump(std::ostream& out) const {
    int i, j;
    for (i = 0; i < order_; ++i) {
        for (j = 0; j < order_; ++j)
            out << (adj_[i][j] ? '*' : '_');
        out << std::endl;
    }
}

TreeDecomposition::TreeDecomposition(const TreeDecomposition& cloneMe) :
        width_(cloneMe.width_), size_(cloneMe.size_) {
    TreeBag *me, *myPrev;
    const TreeBag *you, *yourPrev;

    // Clone the bags from root to leaves.
    you = cloneMe.root_;
    yourPrev = nullptr;
    while (true) {
        me = new TreeBag(*you);
        me->type_ = you->type_;
        me->subtype_ = you->subtype_;
        me->index_ = you->index_;

        // myPrev / yourPrev either points to the previous sibling or,
        // if there is none, the parent.
        if (yourPrev) {
            // This is not the root bag.  Hook it into the tree.
            if (yourPrev->children_ == you) {
                myPrev->children_ = me;
                me->parent_ = myPrev;
            } else {
                myPrev->sibling_ = me;
                me->parent_ = myPrev->parent_;
            }
        } else {
            // This is the root bag.
            root_ = me;
        }

        if (you->children_) {
            yourPrev = you;
            myPrev = me;
            you = you->children_;
        } else {
            while (you && ! you->sibling_) {
                you = you->parent_;
                me = me->parent_;
            }
            if (you) {
                yourPrev = you;
                myPrev = me;
                you = you->sibling_;
            } else
                break;
        }
    }
}

TreeDecomposition::TreeDecomposition(const Link& link,
        TreeDecompositionAlg alg) :
        width_(0), root_(nullptr) {
    Graph g(link.size());

    int i, j;
    const Crossing* c;
    for (i = 0; i < g.order_; ++i) {
        c = link.crossing(i);
        for (j = 0; j < 2; ++j) {
            g.adj_[i][c->next(j).crossing()->index()] = true;
            g.adj_[i][c->prev(j).crossing()->index()] = true;
        }
    }

    construct(g, alg);
}

TreeDecomposition* TreeDecomposition::fromPACE(const std::string& str) {
    std::istringstream s(str);
    return fromPACE(s);
}

TreeDecomposition* TreeDecomposition::fromPACE(std::istream& in) {
    TreeBag** bags = nullptr; // non-null after reading the header line
    size_t nVert, nBags, maxBagSize; // set after reading the header line
    size_t readBags = 0, readJoins = 0, readMaxBagSize = 0;

    std::string line;
    char c;
    int idx, i, j;
    std::string tmp;
    while (std::getline(in, line)) {
        if (line.empty())
            continue;
        if (line[0] == 'c')
            continue;

        std::istringstream s(line);
        if (! bags) {
            // We are expecting the header line.
            if (! ((s >> c >> tmp >> nBags >> maxBagSize >> nVert) &&
                    (c == 's') && (tmp == "td") && (nBags > 0) &&
                    ! (s >> tmp))) {
                std::cerr << "ERROR: Invalid header line" << std::endl;
                return nullptr;
            }

            bags = new TreeBag*[nBags];
            std::fill(bags, bags + nBags, nullptr);
        } else if (readBags < nBags) {
            // We are expecting a bag.
            if (! ((s >> c >> idx) && (c == 'b') && (idx > 0) &&
                    (idx <= nBags) && (! bags[idx - 1]))) {
                std::cerr << "ERROR: Invalid bag line" << std::endl;
                std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
                delete[] bags;
                return nullptr;
            }
            --idx;

            bags[idx] = new TreeBag(maxBagSize + 1);
            bags[idx]->size_ = 0;
            while (bags[idx]->size_ <= maxBagSize) {
                if (s >> bags[idx]->elements_[bags[idx]->size_]) {
                    if (bags[idx]->size_ == maxBagSize ||
                            bags[idx]->elements_[bags[idx]->size_] <= 0 ||
                            bags[idx]->elements_[bags[idx]->size_] > nVert) {
                        std::cerr << "ERROR: Invalid bag contents" << std::endl;
                        std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
                        delete[] bags;
                        return nullptr;
                    } else {
                        --bags[idx]->elements_[bags[idx]->size_];
                        ++bags[idx]->size_;
                    }
                } else
                    break;
            }
            // We don't set bags[idx]->index_, since we will reindex later.
            bags[idx]->parent_ = bags[idx]->sibling_ = bags[idx]->children_ =
                nullptr;
            bags[idx]->type_ = 0;

            std::sort(bags[idx]->elements_,
                bags[idx]->elements_ + bags[idx]->size_);

            // Make sure there are no duplicate vertices in the bag.
            for (i = 0; i + 1 < bags[idx]->size_; ++i)
                if (bags[idx]->elements_[i] == bags[idx]->elements_[i + 1]) {
                    std::cerr << "ERROR: Duplicate bag element" << std::endl;
                    std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
                    delete[] bags;
                    return nullptr;
                }

            // TODO: Reallocate bags[idx]->elements_ to be just the right size.

            if (bags[idx]->size_ > readMaxBagSize)
                readMaxBagSize = bags[idx]->size_;

            ++readBags;
        } else if (readJoins + 1 < nBags) {
            // We are expecting a connection between two bags.
            if (! ((s >> i >> j) && (i != j) && (i > 0) && (j > 0) &&
                    (i <= nBags) && (j <= nBags) && ! (s >> tmp))) {
                std::cerr << "ERROR: Invalid connection line" << std::endl;
                std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
                delete[] bags;
                return nullptr;
            }
            --i;
            --j;

            // Connect bags i and j.
            if (! bags[i]->parent_) {
                // We can hook bag i beneath bag j.
                bags[i]->parent_ = bags[j];
                bags[i]->sibling_ = bags[j]->children_;
                bags[j]->children_ = bags[i];
            } else if (! bags[j]->parent_) {
                // We can hook bag j beneath bag i.
                bags[j]->parent_ = bags[i];
                bags[j]->sibling_ = bags[i]->children_;
                bags[i]->children_ = bags[j];
            } else {
                // Both bags already have parents.
                // We need to do some more serious rearrangement of the tree.
                bags[i]->makeRoot();

                bags[i]->parent_ = bags[j];
                bags[i]->sibling_ = bags[j]->children_;
                bags[j]->children_ = bags[i];
            }

            ++readJoins;
        } else {
            // We are not expecting any more data.
            std::cerr << "ERROR: Unexpected additional data" << std::endl;
            std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
            delete[] bags;
            return nullptr;
        }
    }

    if (! (bags && readBags == nBags && readJoins + 1 == nBags &&
            readMaxBagSize == maxBagSize)) {
        std::cerr << "ERROR: Mismatched max bag size" << std::endl;
        std::for_each(bags, bags + nBags, FuncDelete<TreeBag>());
        delete[] bags;
        return nullptr;
    }

    TreeDecomposition* ans = new TreeDecomposition();
    ans->width_ = maxBagSize - 1;

    for (ans->root_ = bags[nBags - 1]; ans->root_->parent_;
            ans->root_ = ans->root_->parent_)
        ;

    delete[] bags;

    ans->reindex();
    return ans;
}

void TreeDecomposition::construct(Graph& graph, TreeDecompositionAlg alg) {
    if (graph.order_ == 0) {
        // No tree, no bags.
        width_ = -1;
        size_ = 0;
        return;
    }

    switch (alg) {
        case TD_UPPER_GREEDY_FILL_IN:
        default:
            greedyFillIn(graph);
    }

    reindex();
}

void TreeDecomposition::greedyFillIn(Graph& graph) {
    width_ = 0;

    // Find a good elimination order.
    //
    // We add edges to graph as we do this, so that the graph becomes chordal.
    // We also construct the bags as we go.
    //
    // Note: This step currently requires O(n^4) time; surely with a
    // little tweaking we can improve this.

    bool* used = new bool[graph.order_];
    int* elimOrder = new int[graph.order_]; // Elimination stage -> vertex
    int* elimStage = new int[graph.order_]; // Vertex -> elimination stage
    TreeBag** bags = new TreeBag*[graph.order_];

    std::fill(used, used + graph.order_, false);

    int elim, elimEdges, elimBagSize;
    int bestElim, bestElimEdges, bestElimBagSize;
    int stage, j, k, which;
    for (stage = 0; stage < graph.order_; ++stage) {
        bestElim = -1;

        for (elim = 0; elim < graph.order_; ++elim) {
            if (used[elim])
                continue;

            // See how many edges we need to add if we eliminate this vertex.
            elimEdges = 0;
            elimBagSize = 1;
            for (j = 0; j < graph.order_; ++j) {
                if (used[j] || j == elim || ! graph.adj_[elim][j])
                    continue;

                // j is an unused neighbour of elim.
                ++elimBagSize;
                for (k = j + 1; k < graph.order_; ++k) {
                    if (used[k] || k == elim || ! graph.adj_[elim][k])
                        continue;

                    // k is also an unused neighbour of elim.
                    if (! graph.adj_[j][k])
                        ++elimEdges;
                }
            }

            if (bestElim < 0 || elimEdges < bestElimEdges) {
                bestElim = elim;
                bestElimEdges = elimEdges;
                bestElimBagSize = elimBagSize;
            }
        }

        used[bestElim] = true;
        elimOrder[stage] = bestElim;
        elimStage[bestElim] = stage;

        if (bestElimBagSize > width_ + 1)
            width_ = bestElimBagSize - 1;

        // Build the corresponding bag.
        // This contains the eliminated vertex and all of its unused neighbours.
        // Ensure the elements are stored in sorted order.
        bags[stage] = new TreeBag(bestElimBagSize);
        which = 0;
        for (j = 0; j < graph.order_; ++j) {
            if (j == bestElim) {
                bags[stage]->elements_[which++] = j;
            } else if ((! used[j]) && graph.adj_[bestElim][j]) {
                bags[stage]->elements_[which++] = j;

                // Add links between neighbours of bestElim so that this bag
                // becomes a clique.
                for (k = j + 1; k < graph.order_; ++k) {
                    if (used[k] || ! graph.adj_[bestElim][k])
                        continue;
                    if (! graph.adj_[j][k])
                        graph.adj_[j][k] = graph.adj_[k][j] = true;
                }
            }
        }
    }

    // Now hook the bags together into a tree.
    // Step 2: Set the parent relationships in the tree.
    root_ = bags[graph.order_ - 1];

    int parent;
    for (stage = 0; stage < graph.order_ - 1; ++stage) {
        if (bags[stage]->size_ == 1) {
            // The graph must have been disconnected, and the resulting
            // tree decomposition becomes a forest.
            // Just hook this bag directly beneath the root.
            root_->insertChild(bags[stage]);
            continue;
        }

        parent = graph.order_ - 1;
        for (j = 0; j < bags[stage]->size_; ++j) {
            k = elimStage[bags[stage]->elements_[j]];
            if (k > stage && k < parent)
                parent = k;
        }
        bags[parent]->insertChild(bags[stage]);
    }

    // Clean up.

    delete[] used;
    delete[] elimOrder;
    delete[] elimStage;
    delete[] bags;
}

const TreeBag* TreeDecomposition::first() const {
    if (! root_)
        return nullptr;

    TreeBag* b = root_;
    while (b->children_)
        b = b->children_;
    return b;
}

void TreeBag::makeRoot() {
    TreeBag* child = this;
    TreeBag* newParent = nullptr;
    TreeBag* oldParent;
    TreeBag* b;

    while (child) {
        oldParent = child->parent_;

        // We need to convert child into the first child of newParent.
        // INV: There is currently no link between child and newParent.

        // We remove the link between child and oldParent,
        // hook child beneath newParent instead, and remember that
        // we need to hook oldParent *beneath* child.
        if (oldParent) {
            if (child == oldParent->children_)
                oldParent->children_ = child->sibling_;
            else {
                b = oldParent->children_;
                while (b->sibling_ != child)
                    b = b->sibling_;
                b->sibling_ = child->sibling_;
            }
        }

        child->parent_ = newParent;
        if (newParent) {
            child->sibling_ = newParent->children_;
            newParent->children_ = child;
        } else
            child->sibling_ = nullptr;

        newParent = child;
        child = oldParent;
    }
}

const TreeBag* TreeDecomposition::bag(int index) const {
    const TreeBag* b = root_;
    while (b->index() != index) {
        b = b->children();
        while (b->index() < index)
            b = b->sibling();
    }
    return b;
}

bool TreeDecomposition::compress() {
    // Do a prefix enumeration (root first), and compress edges up to
    // parents when one bag is a subset of the other.
    // The path condition ensures that no such subset relationships
    // should remain.
    if (! (root_ && root_->children_))
        return false;

    bool changed = false;
    TreeBag* b = root_->children_;
    TreeBag* siblingOf = nullptr;
    TreeBag* next;
    TreeBag* nextIsSiblingOf;
    TreeBag* child;
    while (b) {
        // We are ready to process bag b.
        // Invariants:
        // - Bag b has a parent (i.e., is not the root).
        // - We have already processed all ancestors of b, but we have
        //   not processed any children of b.
        // - If siblingOf is non-null, then b = siblingOf->sibling_.
        // - If siblingOf is null, then b = b->parent_->children_.

        // First work out which bag will be processed next, so the tree
        // traversal runs as expected even if we merge b into its parent.
        if (b->children_) {
            next = b->children_;
            nextIsSiblingOf = nullptr;
        } else {
            next = b;
            while (next && ! next->sibling_)
                next = next->parent_;
            if (next) {
                nextIsSiblingOf = next;
                next = next->sibling_;
            }
        }

        // Now see if we need to merge b with b->parent_.
        BagComparison compare = b->compare(*b->parent_);
        if (compare != BAG_UNRELATED) {
            // We will merge b with b->parent_, and then remove b.
            if (compare == BAG_SUPERSET)
                b->swapContents(*b->parent_);

            if (b->children_) {
                // Bag b has children.
                // Replace bag b with its list of children.

                // 1) Make all children of b point to the correct parent,
                // and also make note of the last child of b.
                child = b->children_;
                while (true) {
                    child->parent_ = b->parent_;
                    if (child->sibling_)
                        child = child->sibling_;
                    else
                        break;
                }

                // 2) Splice the children of b into the higher list of
                // children to which b belongs.
                child->sibling_ = b->sibling_;
                if (siblingOf)
                    siblingOf->sibling_ = b->children_;
                else
                    b->parent_->children_ = b->children_;

                // Note: in this case we have next == b->children_.
                // Adjust this for the new tree structure.
                nextIsSiblingOf = siblingOf;
            } else {
                // Bag b is a leaf.  Just remove it.
                if (siblingOf)
                    siblingOf->sibling_ = b->sibling_;
                else 
                    b->parent_->children_ = b->sibling_;

                // In this case, next could either be the sibling of b, or
                // (if b has no sibling) something further along in the tree.
                // Adjust nextIsSiblingOf if we need to.
                if (nextIsSiblingOf == b)
                    nextIsSiblingOf = siblingOf;
            }

            // Ensure that deleting b does not cascade to its children.
            b->children_ = nullptr;

            delete b;

            changed = true;
        }

        // Move to the next node for processing.
        b = next;
        siblingOf = nextIsSiblingOf;
    }

    if (changed)
        reindex();
    return changed;
}

void TreeDecomposition::makeNice(int* heightHint) {
    if (! root_)
        return;

    compress();

    if (root_ && (! root_->children()) && root_->size_ == 0) {
        delete root_;
        root_ = nullptr;
        size_ = 0;
        // width_ is unchanged (it must have been -1 already).
        return;
    }

    // First add a chain of forget nodes above the root, right up to a
    // new empty bag.
    TreeBag* b = root_;
    TreeBag *tmp, *tmp2, *tmp3;
    int forget;
    while (root_->size_ > 0) {
        // Work out which node of root_ we wish to forget.
        if (heightHint) {
            // This makes building the forget chain quadratic time.
            // We could always sort the elements at the beginning of the
            // chain and then make this faster, but the copy operation
            // still gives us quadratic time overall so we don't stress
            // too hard about this.
            forget = std::min_element(root_->elements_,
                root_->elements_ + root_->size_,
                [heightHint](int a, int b) {
                    return heightHint[a] < heightHint[b];
                }) - root_->elements_;
        } else
            forget = root_->size_ - 1;

        tmp = new TreeBag(root_->size_ - 1);
        std::copy(root_->elements_,
            root_->elements_ + forget,
            tmp->elements_);
        std::copy(root_->elements_ + forget + 1,
            root_->elements_ + root_->size_,
            tmp->elements_ + forget);
        tmp->children_ = root_;
        root_->parent_ = tmp;
        root_ = tmp;

        tmp->type_ = NICE_FORGET;
        tmp->subtype_ = forget;
    }

    TreeBag* next;
    int p1, p2;
    while (b) {
        // Invariants:
        // - b is not the root;
        // - everything before b in a prefix ordering has been made nice.
        if (b->children_ && b->children_->sibling_) {
            // b is a branching node.
            b->type_ = NICE_JOIN;
            b->subtype_ = 0;

            tmp = new TreeBag(*b);
            tmp2 = new TreeBag(*b);

            tmp2->children_ = b->children_->sibling_;
            tmp2->children_->parent_ = tmp2;

            tmp->children_ = b->children_;
            tmp->children_->parent_ = tmp;
            tmp->children_->sibling_ = nullptr;

            b->children_ = tmp;
            tmp->sibling_ = tmp2;
            tmp->parent_ = b;
            tmp2->parent_ = b;

            b = tmp;
        } else if (b->children_) {
            // b has only one child.
            // Insert the necessary sequence of forgets and introduces.
            // Because we called compress() above, we know that we will
            // need at least one forget and at least one introduce.
            next = b->children_;

            p1 = p2 = 0;
            tmp = b;
            tmp2 = next;
            while (p1 < tmp->size_ || p2 < tmp2->size_) {
                // [b, ..., tmp) and [tmp2, ..., next) are nice.
                // p1, p2 are element indices into tmp, tmp2 respectively.
                if (p2 == tmp2->size_ ||
                        (p1 < tmp->size_ &&
                         tmp->elements_[p1] < tmp2->elements_[p2])) {
                    // Introduce tmp->elements_[p1].
                    tmp->type_ = NICE_INTRODUCE;
                    tmp->subtype_ = p1;

                    tmp3 = new TreeBag(tmp->size_ - 1);
                    std::copy(tmp->elements_,
                        tmp->elements_ + p1, tmp3->elements_);
                    std::copy(tmp->elements_ + p1 + 1,
                        tmp->elements_ + tmp->size_, tmp3->elements_ + p1);

                    tmp3->parent_ = tmp;
                    tmp3->children_ = tmp2;
                    tmp->children_ = tmp3;
                    tmp2->parent_ = tmp3;

                    tmp = tmp3;
                } else if (p1 == tmp->size_ ||
                        (tmp->elements_[p1] > tmp2->elements_[p2])) {
                    // Forget tmp2->elements_[p2].
                    tmp3 = new TreeBag(tmp2->size_ - 1);
                    std::copy(tmp2->elements_,
                        tmp2->elements_ + p2, tmp3->elements_);
                    std::copy(tmp2->elements_ + p2 + 1,
                        tmp2->elements_ + tmp2->size_, tmp3->elements_ + p2);

                    tmp3->type_ = NICE_FORGET;
                    tmp3->subtype_ = p2;

                    tmp3->parent_ = tmp;
                    tmp3->children_ = tmp2;
                    tmp->children_ = tmp3;
                    tmp2->parent_ = tmp3;

                    tmp2 = tmp3;
                } else {
                    // tmp->elements_[p1] == tmp2->elements_[p2].
                    ++p1;
                    ++p2;
                }
            }

            // Now tmp and tmp2 contain the same elements.
            // Remove tmp.
            // Since there is at least one forget and at least one
            // introduce in this sequence, we know that tmp lies strictly
            // between b and next.
            tmp->parent_->children_ = tmp->children_;
            tmp->children_->parent_ = tmp->parent_;
            tmp->children_ = nullptr;
            delete tmp;

            // Done!  Jump to the bottom of the sequence and continue.
            b = next;
        } else {
            // b is a leaf node.
            // Build a series of introduce nodes.
            next = const_cast<TreeBag*>(b->nextPrefix());

            b->type_ = NICE_INTRODUCE;
            b->subtype_ = b->size_ - 1;

            tmp = b;
            for (int i = b->size_ - 1; i > 0; --i) {
                tmp2 = new TreeBag(i);
                std::copy(b->elements_, b->elements_ + i, tmp2->elements_);
                tmp->children_ = tmp2;
                tmp2->parent_ = tmp;
                tmp = tmp2;

                tmp->type_ = NICE_INTRODUCE;
                tmp->subtype_ = i - 1;
            }

            b = next;
        }
    }

    reindex();
}

void TreeDecomposition::reroot(TreeBag* newRoot) {
    if (root_ == newRoot)
        return;

    newRoot->makeRoot();
    root_ = newRoot;

    for (const TreeBag* b = first(); b; b = b->next())
        const_cast<TreeBag*>(b)->type_ = 0;

    reindex();
}

void TreeDecomposition::writeDot(std::ostream& out) const {
    out << "digraph tree {\n"
        "edge [color=black];\n"
        "node [style=filled,fontsize=9,fontcolor=\"#751010\"];\n";

    TreeBag* b = root_;
    int i;
    while (b) {
        out << "b_" << b->index_ << " [label=\"";
        if (b->size_) {
            out << b->elements_[0];
            for (i = 1; i < b->size_; ++i)
                out << ", " << b->elements_[i];
        } else
            out << "empty";
        out << "\"]\n";

        if (b->parent_)
            out << "b_" << b->parent_->index_ << " -> b_" << b->index_ << '\n';

        if (b->children_)
            b = b->children_;
        else {
            while (b && ! b->sibling_)
                b = b->parent_;
            if (b)
                b = b->sibling_;
        }
    }

    out << "}" << std::endl;
}

std::string TreeDecomposition::dot() const {
    std::ostringstream out;
    writeDot(out);
    return out.str();
}

void TreeDecomposition::writePACE(std::ostream& out) const {
    out << "c Output from Regina using TreeDecomposition::writePACE()"
        << std::endl;

    const TreeBag* b;
    int i;

    int nVert = 0;
    for (b = first(); b; b = b->next()) {
        if (b->size() && nVert <= b->elements_[b->size_ - 1])
            nVert = b->elements_[b->size_ - 1] + 1;
    }

    out << "s td " << size_ << ' ' << (width_ + 1) << ' ' << nVert << std::endl;

    for (b = first(); b; b = b->next()) {
        out << "b " << (b->index() + 1);
        for (i = 0; i < b->size_; ++i)
            out << ' ' << (b->elements_[i] + 1);
        out << std::endl;
    }
    for (b = first(); b; b = b->next()) {
        if (b->parent())
            out << (b->index() + 1) << ' '
                << (b->parent()->index() + 1) << std::endl;
    }
}

std::string TreeDecomposition::pace() const {
    std::ostringstream out;
    writePACE(out);
    return out.str();
}

void TreeDecomposition::writeTextShort(std::ostream& out) const {
    out << "Tree decomposition: width " << width_
        << ", size " << size_;
}

void TreeDecomposition::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    int indent = 0;
    TreeBag* b = root_;
    int i;

    while (b) {
        for (i = 0; i < indent; ++i)
            out << "  ";
        out << "Bag " << b->index_ << " [" << b->size_ << "]:";
        for (i = 0; i < b->size_; ++i)
            out << ' ' << b->elements_[i];
        out << std::endl;

        if (b->children_) {
            ++indent;
            b = b->children_;
        } else {
            while (b && ! b->sibling_) {
                --indent;
                b = b->parent_;
            }
            if (b)
                b = b->sibling_;
        }
    }
}

} // namespace regina
