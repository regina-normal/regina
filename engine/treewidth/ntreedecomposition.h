
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

/*! \file treewidth/ntreedecomposition.h
 *  \brief Deals with tree decompositions of facet pairing graphs.
 */

#ifndef __NTREEDECOMPOSITION_H
#ifndef __DOXYGEN
#define __NTREEDECOMPOSITION_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "census/ngenericfacetpairing.h"
#include "generic/ngenerictriangulation.h"

namespace regina {

/**
 * \addtogroup treewidth Treewidth
 * Treewidth and tree decompositions.
 * @{
 */

class NTreeBag;

/**
 * TODO: Document this file.
 */

enum TreeDecompositionAlg {
    TD_UPPER = 0x0001,
    TD_UPPER_GREEDY_FILL_IN = 0x0001,
    TD_EXACT = 0x0010
};

class REGINA_API NTreeBag : public ShareableObject {
    private:
        int size_;
            /**< The number of elements in this bag. */
        int* elements_;
            /**< The elements of this bag, sorted in ascending order. */

    public:
        ~NTreeBag();

        int size() const;
        int element(int which) const;
        bool contains(int element) const;

        void writeTextShort(std::ostream& out) const;

    private:
        NTreeBag(int size);

    friend class NTreeDecomposition;
};

/**
 * TODO: Document that a bag's parent always has a higher number,
 * and so processing bags in order 0..(size-1) will give a leaf-to-root
 * ordering.
 */
class REGINA_API NTreeDecomposition : public ShareableObject {
    protected:
        struct Graph {
            int order_;
            bool** adj_;

            /**
             * Fills with false.
             */
            Graph(int order);
            ~Graph();

            void dump(std::ostream& out) const;
        };

    private:
        int size_;
        int width_;
        NTreeBag** bags_;
        int* parent_;

    public:
        template <int dim>
        NTreeDecomposition(
            const NGenericTriangulation<dim>& triangulation,
            TreeDecompositionAlg alg = TD_UPPER);

        template <int dim>
        NTreeDecomposition(
            const NGenericFacetPairing<dim>& pairing,
            TreeDecompositionAlg alg = TD_UPPER);

        ~NTreeDecomposition();

        int width() const;
        int size() const;
        const NTreeBag& bag(int which) const;
        int parent(int which) const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Note: graph may be modified during this routine.
         */
        void construct(Graph& graph, TreeDecompositionAlg alg);
        void greedyFillIn(Graph& graph);
};

/*@}*/

// Inline functions for NTreeBag

inline NTreeBag::NTreeBag(int size) :
        size_(size), elements_(new int[size_]) {
}

inline NTreeBag::~NTreeBag() {
    delete[] elements_;
}

inline int NTreeBag::size() const {
    return size_;
}

inline int NTreeBag::element(int which) const {
    return elements_[which];
}

// Inline functions for NTreeDecomposition

inline NTreeDecomposition::~NTreeDecomposition() {
    if (bags_) {
        for (int i = 0; i < size_; ++i)
            delete bags_[i];
        delete[] bags_;
    }
    delete[] parent_;
}

inline int NTreeDecomposition::width() const {
    return width_;
}

inline int NTreeDecomposition::size() const {
    return size_;
}

inline const NTreeBag& NTreeDecomposition::bag(int which) const {
    return *bags_[which];
}

inline int NTreeDecomposition::parent(int which) const {
    return parent_[which];
}

// Inline functions for NTreeDecomposition::Graph

inline NTreeDecomposition::Graph::Graph(int order) :
        order_(order), adj_(new bool*[order]) {
    int i, j;
    for (i = 0; i < order_; ++i) {
        adj_[i] = new bool[order];
        for (j = 0; j < order; ++j)
            adj_[i][j] = false;
    }
}

inline NTreeDecomposition::Graph::~Graph() {
    for (int i = 0; i < order_; ++i)
        delete[] adj_[i];
    delete[] adj_;
}

} // namespace regina

#endif

