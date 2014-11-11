
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include <algorithm>
#include "dim2/dim2triangulation.h"
#include "treewidth/ntreedecomposition.h"
#include "treewidth/ntreedecomposition-impl.h"
#include "triangulation/ntriangulation.h"

namespace regina {

// Instantiate templates:

template NTreeDecomposition::NTreeDecomposition(
    const NGenericTriangulation<2>&, TreeDecompositionAlg);
template NTreeDecomposition::NTreeDecomposition(
    const NGenericTriangulation<3>&, TreeDecompositionAlg);

template NTreeDecomposition::NTreeDecomposition(
    const NGenericFacetPairing<2>&, TreeDecompositionAlg);
template NTreeDecomposition::NTreeDecomposition(
    const NGenericFacetPairing<3>&, TreeDecompositionAlg);

bool NTreeBag::contains(int element) const {
    return std::binary_search(elements_, elements_ + size_, element);
}

void NTreeBag::writeTextShort(std::ostream& out) const {
    if (size_ == 1)
        out << "Bag of 1 element:";
    else
        out << "Bag of " << size_ << " elements:";

    for (int i = 0; i < size_; ++i)
        out << ' ' << elements_[i];
}

void NTreeDecomposition::Graph::dump(std::ostream& out) const {
    int i, j;
    for (i = 0; i < order_; ++i) {
        for (j = 0; j < order_; ++j)
            std::cout << (adj_[i][j] ? '*' : '_');
        std::cout << std::endl;
    }
}

void NTreeDecomposition::construct(Graph& graph, TreeDecompositionAlg alg) {
    if (graph.order_ == 0) {
        // No tree, no bags.
        width = -1;
        return;
    }

    switch (alg) {
        case TD_EXACT:
            // TODO: Implement an exact algorithm.

        case TD_UPPER_GREEDY_FILL_IN:
        default:
            greedyFillIn(graph);
    }
}

void NTreeDecomposition::greedyFillIn(Graph& graph) {
    width_ = 0;

    size_ = graph.order_;
    bags_ = new NTreeBag*[size_];
    parent_ = new int[size_];

    // Step 1: Find a good elimination order.
    //
    // We add edges to graph as we do this, so that the graph becomes chordal.
    // We also construct the bags as we go.
    //
    // Note: This step currently requires O(n^4) time; surely with a
    // little tweaking we can improve this.

    bool* used = new bool[size_];
    int* elimOrder = new int[size_]; // Elimination stage -> vertex
    int* elimStage = new int[size_]; // Vertex -> elimination stage

    std::fill(used, used + size_, false);

    int elim, elimEdges, elimBagSize;
    int bestElim, bestElimEdges, bestElimBagSize;
    int stage, j, k, which;
    for (stage = 0; stage < size_; ++stage) {
        bestElim = -1;

        for (elim = 0; elim < size_; ++elim) {
            if (used[elim])
                continue;

            // See how many edges we need to add if we eliminate this vertex.
            elimEdges = 0;
            elimBagSize = 1;
            for (j = 0; j < size_; ++j) {
                if (used[j] || j == elim || ! graph.adj_[elim][j])
                    continue;

                // j is an unused neighbour of elim.
                ++elimBagSize;
                for (k = j + 1; k < size_; ++k) {
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
        bags_[stage] = new NTreeBag(bestElimBagSize);
        which = 0;
        for (j = 0; j < size_; ++j) {
            if (j == bestElim) {
                bags_[stage]->elements_[which++] = j;
            } else if ((! used[j]) && graph.adj_[bestElim][j]) {
                bags_[stage]->elements_[which++] = j;

                // Add links between neighbours of bestElim so that this bag
                // becomes a clique.
                for (k = j + 1; k < size_; ++k) {
                    if (used[k] || ! graph.adj_[bestElim][k])
                        continue;
                    if (! graph.adj_[j][k])
                        graph.adj_[j][k] = graph.adj_[k][j] = true;
                }
            }
        }
    }

    // Step 2: Set the parent relationships in the tree.

    for (stage = 0; stage < size_ - 1; ++stage) {
        parent_[stage] = size_ - 1;

        if (bags_[stage]->size_ == 1) {
            // The graph must have been disconnected, and the resulting
            // tree decomposition becomes a forest.
            // Just hook this bag directly beneath the root.
            continue;
        }

        for (j = 0; j < bags_[stage]->size_; ++j) {
            k = elimStage[bags_[stage]->elements_[j]];
            if (k > stage && k < parent_[stage])
                parent_[stage] = k;
        }
    }

    parent_[size_ - 1] = - 1;

    // Clean up.

    delete[] used;
    delete[] elimOrder;
    delete[] elimStage;
}

void NTreeDecomposition::writeTextShort(std::ostream& out) const {
    out << "Tree decomposition: ";
    if (size_ == 1)
        out << "1 bag";
    else
        out << size_ << " bags";
    out << ", width " << width_;
}

void NTreeDecomposition::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    int i, j;
    for (i = 0; i < size_; ++i) {
        out << "Bag " << i;
        if (parent_[i] < 0)
            out << " (root)";
        else
            out << " (-> " << parent_[i] << ')';
        out << " - ";

        if (bags_[i]->size_ == 1)
            out << "1 element:";
        else
            out << bags_[i]->size_ << " elements:";

        for (j = 0; j < bags_[i]->size_; ++j)
            out << ' ' << bags_[i]->elements_[j];
        out << std::endl;
    }
}

} // namespace regina
