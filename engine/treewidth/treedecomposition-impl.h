
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

} // namespace regina

#endif
