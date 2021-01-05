
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

/*! \file treewidth/treedecomposition-impl.h
 *  \brief Contains implementations of template member functions in the
 *  TreeDecomposition class.
 *
 *  This file is \e not included automatically by treedecomposition.h.
 *  However, most end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of these routines for \ref stddim "standard dimensions" and for
 *  common types.
 */

#ifndef __TREEDECOMPOSITION_IMPL_H
#ifndef __DOXYGEN
#define __TREEDECOMPOSITION_IMPL_H
#endif

#include "treewidth/treedecomposition.h"

namespace regina {

template <int dim>
TreeDecomposition::TreeDecomposition(
        const Triangulation<dim>& triangulation,
        TreeDecompositionAlg alg) :
        width_(0), root_(0) {
    Graph g(triangulation.size());

    int i, j;
    const Simplex<dim>* simp;
    for (i = 0; i < g.order_; ++i) {
        simp = triangulation.simplex(i);
        for (j = 0; j <= dim; ++j)
            if (simp->adjacentSimplex(j))
                g.adj_[i][simp->adjacentSimplex(j)->index()] = true;
    }

    construct(g, alg);
}

template <int dim>
TreeDecomposition::TreeDecomposition(
        const FacetPairing<dim>& pairing,
        TreeDecompositionAlg alg) :
        width_(0), root_(0) {
    Graph g(pairing.size());

    int i, j;
    for (i = 0; i < g.order_; ++i)
        for (j = 0; j <= dim; ++j)
            if (! pairing.isUnmatched(i, j))
                g.adj_[i][pairing.dest(i, j).simp] = true;

    construct(g, alg);
}

template <typename T>
TreeDecomposition::TreeDecomposition(unsigned order, T const** graph,
        TreeDecompositionAlg alg) :
        width_(0), root_(0) {
    Graph g(order);

    int i, j;
    for (i = 0; i < order; ++i)
        for (j = 0; j < order; ++j)
            g.adj_[i][j] = graph[i][j] || graph[j][i];

    construct(g, alg);
}

template <typename T>
void TreeDecomposition::reroot(const T* costSame, const T* costReverse,
        const T* costRoot) {
    if (size_ <= 1)
        return;

    const TreeBag *b, *c;

    // Cost = (max cost of any tree edge, number of times that cost appears)
    typedef std::pair<T, int> Cost;

    // For each bag, work out the maximum cost of all links *below* that
    // bag if the root is located at or *above* that bag.
    // We do this using a leaves-to-root iteration over the tree.
    Cost* maxBelow = new Cost[size_];
    std::fill(maxBelow, maxBelow + size_, Cost(0, 0));
    for (b = first(); b; b = b->next()) {
        for (c = b->children(); c; c = c->sibling()) {
            if (maxBelow[b->index()].first < maxBelow[c->index()].first)
                maxBelow[b->index()] = maxBelow[c->index()];
            else if (maxBelow[b->index()].first == maxBelow[c->index()].first)
                maxBelow[b->index()].second += maxBelow[c->index()].second;

            if (maxBelow[b->index()].first < costSame[c->index()]) {
                maxBelow[b->index()].first = costSame[c->index()];
                maxBelow[b->index()].second = 1;
            } else if (maxBelow[b->index()].first == costSame[c->index()])
                ++maxBelow[b->index()].second;
        }
    }

    // Now, for each bag, work out the maximum cost of all links *above*
    // that bag if the root is located at or *below* that bag.
    // We do this using a root-to-leaves iteration over the tree.
    Cost* maxAbove = new Cost[size_];
    std::fill(maxAbove, maxAbove + size_, Cost(0, 0));
    for (b = firstPrefix(); b; b = b->nextPrefix()) {
        if (! b->parent()) {
            maxAbove[b->index()].first = 0;
            maxAbove[b->index()].second = 0;
        } else {
            maxAbove[b->index()].first = costReverse[b->index()];
            maxAbove[b->index()].second = 1;

            c = b->parent();
            if (maxAbove[b->index()].first < maxAbove[c->index()].first)
                maxAbove[b->index()] = maxAbove[c->index()];
            else if (maxAbove[b->index()].first == maxAbove[c->index()].first)
                maxAbove[b->index()].second += maxAbove[c->index()].second;

            for (c = b->parent()->children(); c; c = c->sibling()) {
                if (c == b)
                    continue;

                if (maxAbove[b->index()].first < maxBelow[c->index()].first)
                    maxAbove[b->index()] = maxBelow[c->index()];
                else if (maxAbove[b->index()].first ==
                        maxBelow[c->index()].first)
                    maxAbove[b->index()].second += maxBelow[c->index()].second;

                if (maxAbove[b->index()].first < costSame[c->index()]) {
                    maxAbove[b->index()].first = costSame[c->index()];
                    maxAbove[b->index()].second = 1;
                } else if (maxAbove[b->index()].first == costSame[c->index()])
                    ++maxAbove[b->index()].second;
            }
        }
    }

    // For each node, the final cost of rooting the tree at that node is
    // found by combining maxBelow, maxAbove, and costRoot.
    const TreeBag* bestBag = nullptr;
    Cost bestCost;

    for (b = first(); b; b = b->next()) {
        // Combine all costs into maxBelow.
        if (maxAbove[b->index()].first > maxBelow[b->index()].first)
            maxBelow[b->index()] = maxAbove[b->index()];
        else if (maxAbove[b->index()].first == maxBelow[b->index()].first)
            maxBelow[b->index()].second += maxAbove[b->index()].second;

        if (costRoot && costRoot[b->index()] > maxBelow[b->index()].first) {
            maxBelow[b->index()].first = costRoot[b->index()];
            maxBelow[b->index()].second = 1;
        } else if (costRoot[b->index()] == maxBelow[b->index()].first)
            ++maxBelow[b->index()].second;

        /*
        std::cerr << "Bag " << b->index() << ": "
            << maxBelow[b->index()].first << ", "
            << maxBelow[b->index()].second << std::endl;
        */

        // Is this the best so far?
        if ((! bestBag) || bestCost.first > maxBelow[b->index()].first ||
                (bestCost.first == maxBelow[b->index()].first &&
                 bestCost.second > maxBelow[b->index()].second)) {
            bestBag = b;
            bestCost = maxBelow[b->index()];
        }
    }

    delete[] maxAbove;
    delete[] maxBelow;

    /*
    std::cerr << "INFO: Rerooting from bag " << bestBag->index()
        << " (cost " << bestCost.first << ", " << bestCost.second
        << ")" << std::endl;
    */
    reroot(const_cast<TreeBag*>(bestBag));
}

} // namespace regina

#endif
