
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

/*! \file treewidth/treedecomposition-impl.h
 *  \brief Contains implementations of template member functions in the
 *  TreeDecomposition class.
 *
 *  This file is _not_ included automatically by treedecomposition.h.
 *  However, most end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of these routines for \ref stddim "standard dimensions" and for
 *  common types.
 */

#ifndef __REGINA_TREEDECOMPOSITION_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_TREEDECOMPOSITION_IMPL_H
#endif

#include "treewidth/treedecomposition.h"
#include "utilities/aggregator.h"
#include "utilities/fixedarray.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim> requires (supportedDim(dim))
TreeDecomposition::TreeDecomposition(
        const Triangulation<dim>& triangulation,
        TreeDecompositionAlg alg) :
        width_(0), root_(nullptr) {
    Graph g(triangulation.size());

    const Simplex<dim>* simp;
    for (size_t i = 0; i < g.order_; ++i) {
        simp = triangulation.simplex(i);
        for (int j = 0; j <= dim; ++j)
            if (simp->adjacentSimplex(j))
                g.adj_[i][simp->adjacentSimplex(j)->index()] = true;
    }

    construct(g, alg);
}

template <int dim> requires (supportedDim(dim))
TreeDecomposition::TreeDecomposition(
        const FacetPairing<dim>& pairing,
        TreeDecompositionAlg alg) :
        width_(0), root_(nullptr) {
    Graph g(pairing.size());

    for (size_t i = 0; i < g.order_; ++i)
        for (int j = 0; j <= dim; ++j)
            if (! pairing.isUnmatched(i, j))
                g.adj_[i][pairing.dest(i, j).simp] = true;

    construct(g, alg);
}

template <typename T>
requires std::regular<T> && std::three_way_comparable<T>
void TreeDecomposition::reroot(const T* costSame, const T* costReverse,
        const T* costRoot) {
    if (size_ <= 1)
        return;

    // For each bag, work out the maximum cost of all links *below* that
    // bag if the root is located at or *above* that bag.
    // We do this using a leaves-to-root iteration over the tree.
    FixedArray<MaxCountAggregator<T>> maxBelow(size_);
    for (const TreeBag* b = first(); b; b = b->next()) {
        for (const TreeBag* c = b->children(); c; c = c->sibling()) {
            maxBelow[b->index()].aggregate(maxBelow[c->index()]);
            maxBelow[b->index()].aggregate(costSame[c->index()]);
        }
    }

    // Now, for each bag, work out the maximum cost of all links *above*
    // that bag if the root is located at or *below* that bag.
    // We do this using a root-to-leaves iteration over the tree.
    FixedArray<MaxCountAggregator<T>> maxAbove(size_);
    for (const TreeBag* b = firstPrefix(); b; b = b->nextPrefix()) {
        if (const TreeBag* c = b->parent()) {
            maxAbove[b->index()].reset(costReverse[b->index()]);
            maxAbove[b->index()].aggregate(maxAbove[c->index()]);

            for (c = b->parent()->children(); c; c = c->sibling()) {
                if (c != b) {
                    maxAbove[b->index()].aggregate(maxBelow[c->index()]);
                    maxAbove[b->index()].aggregate(costSame[c->index()]);
                }
            }
        }
    }

    // For each node, the final cost of rooting the tree at that node is
    // found by combining maxBelow, maxAbove, and costRoot.
    const TreeBag* bestBag = nullptr;
    MaxCountAggregator<T> bestCost;

    for (const TreeBag* b = first(); b; b = b->next()) {
        // Combine all costs into maxBelow.
        maxBelow[b->index()].aggregate(maxAbove[b->index()]);
        if (costRoot)
            maxBelow[b->index()].aggregate(costRoot[b->index()]);

#if 0
        std::cerr << "Bag " << b->index() << ": "
            << maxBelow[b->index()].maxCost << ", "
            << maxBelow[b->index()].count << std::endl;
#endif

        // Is this the best so far?
        if ((! bestBag) || bestCost > maxBelow[b->index()]) {
            bestBag = b;
            bestCost = maxBelow[b->index()];
        }
    }

#if 0
    std::cerr << "INFO: Rerooting from bag " << bestBag->index()
        << " (cost " << bestCost.maxCost << ", " << bestCost.count << ")"
        << std::endl;
#endif
    reroot(const_cast<TreeBag*>(bestBag));
}

} // namespace regina

#endif
