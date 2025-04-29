
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

/*! \file link/modellinkgraph-impl.h
 *  \brief Contains implementation details for generating links from
 *  model graphs.
 *
 *  This file is automatically included from modellinkgraph.h; there is no
 *  need for end users to include it explicitly.
 */

#ifndef __REGINA_MODELLINKGRAPH_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_MODELLINKGRAPH_IMPL_H
#endif

#include "link/link.h"
#include "triangulation/facetpairing3.h"
#include "utilities/fixedarray.h"
#include <set>

namespace regina {

template <typename Action, typename... Args>
void ModelLinkGraph::generateMinimalLinks(Action&& action, Args&&... args)
        const {
    if (size() == 0) {
        // Generate a single empty link.
        action(Link(), std::forward<Args>(args)...);
        return;
    }

    // First work out the orientation of the link components as they pass
    // through each node.
    FixedArray<char> dir(size(), 0); // Bits 0,1,2,3 are 1/0 for forward/back.

    std::vector<ModelLinkGraphArc> componentArcs;

    size_t steps = 0;
    for (size_t i = 0; i < size(); ++i) {
        auto node = nodes_[i];

        // Look at the strand passing through arcs 0 and 2:
        if ((dir[node->index()] & 5 /* 0101 */) == 0) {
            ModelLinkGraphArc a(node, 0);
            componentArcs.push_back(a);
            do {
                dir[a.node()->index()] |= (1 << a.arc());
                a = a.next();
                ++steps;
            } while (a.node() != node || a.arc() != 0);
        }

        // Look at the strand passing through arcs 1 and 3:
        if ((dir[node->index()] & 10 /* 1010 */) == 0) {
            ModelLinkGraphArc a(node, 1);
            componentArcs.push_back(a);
            do {
                dir[a.node()->index()] |= (1 << a.arc());
                a = a.next();
                ++steps;
            } while (a.node() != node || a.arc() != 1);
        }
    }
    if (steps != 2 * size()) {
        // This should never happen.
        std::cerr << "ERROR: generateMinimalLinks() did not identify "
            "components correctly" << std::endl;
        return;
    }

    // Force computation of the cell structure (which we will need below),
    // and enforce our precondition (since otherwise this may corrupt the
    // structure of our union-find-like tree).

    cells();
    for (size_t i = 0; i < cells_->nCells_; ++i)
        if (cells_->size(i) == 1)
            throw FailedPrecondition("generateMinimalLinks() requires the "
                 "induced cell decomposition to have no 1-gons");

    // Now work out which relationships we may assume between different
    // crossing signs.  This will be a quadratic-time "poor man's union-find" -
    // the criterion for how to join subtrees is not depth (to keep the
    // worst-case depth logarithmic), but rather the insistence that
    // parent[i] < i (so we can easily choose the parent sign before the child).

    // If parent[i] >= 0, then the sign of crossing i is tied to the
    // sign of crossing parent[i].  The signs are the same if flip[i] is false,
    // and the signs are different if flip[i] is true.
    // We guarantee for all nodes that parent[i] < i.
    FixedArray<ssize_t> parent(size(), -1);
    FixedArray<bool> flip(size());

    // The relationships that we deduce all happen around bigons and/or
    // triangles.  Because of the precondition that the cell decomposition
    // has no 1-gons, we can safely assume that any bigon or triangle is
    // embedded (i.e., does not contain the same node more than once).

    for (size_t i = 0; i < cells_->nCells_; ++i)
        if (cells_->size(i) == 2) {
            // Both crossings on the bigon should have the same sign.
            ModelLinkGraphArc a1 = cells_->arc(i, 0);
            ModelLinkGraphArc a2 = cells_->arc(i, 1);
            size_t n1 = a1.node()->index();
            size_t n2 = a2.node()->index();
            bool flip1 = false;
            bool flip2 = false;
            while (parent[n1] >= 0) {
                if (flip[n1])
                    flip1 = ! flip1;
                n1 = parent[n1];
            }
            while (parent[n2] >= 0) {
                if (flip[n2])
                    flip2 = ! flip2;
                n2 = parent[n2];
            }
            if (n1 < n2) {
                parent[n2] = n1;
                flip[n2] = (flip1 != flip2);
            } else if (n2 < n1) {
                parent[n1] = n2;
                flip[n1] = (flip1 != flip2);
            } else if (flip1 != flip2) {
                // A simplifying R2 is unavoidable.
                // I _think_ one can argue that this never occurs, even for
                // non-planar graphs, but until I write the argument down
                // properly let's keep this test here.
                return;
            }

            // At this point we will modify arcs a1 and a2, but not their nodes.
            ++a1;
            ++a2;
            if (cells_->size(cells_->cell(a1)) == 3) {
                // We have a triangle beside the original arc a1.
                // The third crossing of the triangle has its sign forced also.
                size_t n3 = a1.traverse().node()->index();
                bool flip3 = (
                    ((dir[a1.node()->index()] >> a1.arc()) & 1) ==
                    ((dir[a2.node()->index()] >> a2.arc()) & 1));
                while (parent[n3] >= 0) {
                    if (flip[n3])
                        flip3 = ! flip3;
                    n3 = parent[n3];
                }
                if (n1 < n3) {
                    parent[n3] = n1;
                    flip[n3] = (flip1 != flip3);
                } else if (n3 < n1) {
                    parent[n1] = n3;
                    flip[n1] = (flip1 != flip3);
                } else if (flip1 != flip3) {
                    // A simplifying R2 is unavoidable.
                    // Again, I _think_ this can never occur, even for
                    // non-planar graphs, but we keep the test here for now.
                    return;
                }
            }
            if (cells_->size(cells_->cell(a2)) == 3) {
                // We have a triangle beside the original arc a2.
                // As above.
                size_t n3 = a2.traverse().node()->index();
                bool flip3 = (
                    ((dir[a1.node()->index()] >> a1.arc()) & 1) ==
                    ((dir[a2.node()->index()] >> a2.arc()) & 1));
                while (parent[n3] >= 0) {
                    if (flip[n3])
                        flip3 = ! flip3;
                    n3 = parent[n3];
                }
                if (n2 < n3) {
                    parent[n3] = n2;
                    flip[n3] = (flip2 != flip3);
                } else if (n3 < n2) {
                    parent[n2] = n3;
                    flip[n2] = (flip2 != flip3);
                } else if (flip2 != flip3) {
                    // A simplifying R2 is unavoidable.
                    // Again, I _think_ this can never occur, even for
                    // non-planar graphs, but we keep the test here for now.
                    return;
                }
            }
        }

    // Now choose the signs of the crossings!
    FixedArray<int> sign(size(), 0);

    ssize_t curr = 0;
    while (curr >= 0) {
        // We have selected the signs for all crossings < curr, and we
        // need to move to the next available sign at crossing curr.
        if (curr == static_cast<ssize_t>(size())) {
            // We have a complete selection of crossings.
            Link l;
            for (size_t i = 0; i < size(); ++i)
                l.crossings_.push_back(new Crossing(sign[i]));
            for (size_t i = 0; i < size(); ++i) {
                // Upper outgoing arc:
                ModelLinkGraphArc a =
                    nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 1 : 0][dir[i]]];
                size_t adj = a.node_->index();
                int adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l.crossings_[i]->next_[1].crossing_ = l.crossings_[adj];
                l.crossings_[i]->next_[1].strand_ = adjStrand;

                l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
                l.crossings_[adj]->prev_[adjStrand].strand_ = 1;

                // Lower outgoing arc:
                a = nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 0 : 1][dir[i]]];
                adj = a.node_->index();
                adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l.crossings_[i]->next_[0].crossing_ = l.crossings_[adj];
                l.crossings_[i]->next_[0].strand_ = adjStrand;

                l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
                l.crossings_[adj]->prev_[adjStrand].strand_ = 0;
            }

            for (const auto& a : componentArcs) {
                size_t i = a.node_->index();
                // We know from above that a.arc_ is either 0 or 1,
                // and that dir[i] sets the bit for a.arc_.
                if (sign[i] > 0) {
                    // If the outgoing arcs are j, j+1 then j is lower.
                    if (dir[i] == (3 << a.arc_)) {
                        // The outgoing arcs are a.arc_, a.arc_+1.
                        l.components_.emplace_back(l.crossings_[i], 0);
                    } else {
                        // The outgoing arcs are a.arc_, a.arc_-1.
                        l.components_.emplace_back(l.crossings_[i], 1);
                    }
                } else {
                    // If the outgoing arcs are j,j+1 then j is upper.
                    if (dir[i] == (3 << a.arc_)) {
                        // The outgoing arcs are a.arc_, a.arc_+1.
                        l.components_.emplace_back(l.crossings_[i], 1);
                    } else {
                        // The outgoing arcs are a.arc_, a.arc_-1.
                        l.components_.emplace_back(l.crossings_[i], 0);
                    }
                }
            }

            action(std::move(l), std::forward<Args>(args)...);

            // Backtrack!
            --curr;
            // Here: 0 <= curr < size (since the model graph is non-empty).
            while (parent[curr] >= 0)
                --curr;
            // Here also: 0 <= curr < size (since parent[0] == -1 always).
        }

        // Here: 0 <= curr < size.
        if (parent[curr] >= 0)
            sign[curr] =
                (flip[curr] ? -sign[parent[curr]] : sign[parent[curr]]);
        else if (sign[curr] == 0)
            sign[curr] = 1;
        else if (curr > 0 /* WLOG, sign[0] = 1 */ && sign[curr] == 1)
            sign[curr] = -1;
        else {
            // We have exhausted our options here.
            sign[curr] = 0;
            --curr;
            if (curr >= 0)
                while (parent[curr] >= 0)
                    --curr;
            continue;
        }

        // Move on to the next crossing.
        ++curr;
    }

    // All done!
}

template <typename Action, typename... Args>
void ModelLinkGraph::generateAllLinks(Action&& action, Args&&... args)
        const {
    if (size() == 0) {
        // Generate a single empty link.
        action(Link(), std::forward<Args>(args)...);
        return;
    }

    // First work out the orientation of the link components as they pass
    // through each node.
    FixedArray<char> dir(size(), 0); // Bits 0,1,2,3 are 1/0 for forward/back.

    std::vector<ModelLinkGraphArc> componentArcs;

    size_t steps = 0;
    for (size_t i = 0; i < size(); ++i) {
        auto node = nodes_[i];

        // Look at the strand passing through arcs 0 and 2:
        if ((dir[node->index()] & 5 /* 0101 */) == 0) {
            ModelLinkGraphArc a(node, 0);
            componentArcs.push_back(a);
            do {
                dir[a.node()->index()] |= (1 << a.arc());
                a = a.next();
                ++steps;
            } while (a.node() != node || a.arc() != 0);
        }

        // Look at the strand passing through arcs 1 and 3:
        if ((dir[node->index()] & 10 /* 1010 */) == 0) {
            ModelLinkGraphArc a(node, 1);
            componentArcs.push_back(a);
            do {
                dir[a.node()->index()] |= (1 << a.arc());
                a = a.next();
                ++steps;
            } while (a.node() != node || a.arc() != 1);
        }
    }
    if (steps != 2 * size()) {
        // This should never happen.
        std::cerr << "ERROR: generateAllLinks() did not identify "
            "components correctly" << std::endl;
        return;
    }

    // Now choose the signs of the crossings!
    FixedArray<int> sign(size(), 0);

    ssize_t curr = 0;
    while (curr >= 0) {
        // We have selected the signs for all crossings < curr, and we
        // need to move to the next available sign at crossing curr.
        if (curr == static_cast<ssize_t>(size())) {
            // We have a complete selection of crossings.
            Link l;
            for (size_t i = 0; i < size(); ++i)
                l.crossings_.push_back(new Crossing(sign[i]));
            for (size_t i = 0; i < size(); ++i) {
                // Upper outgoing arc:
                ModelLinkGraphArc a =
                    nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 1 : 0][dir[i]]];
                size_t adj = a.node_->index();
                int adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l.crossings_[i]->next_[1].crossing_ = l.crossings_[adj];
                l.crossings_[i]->next_[1].strand_ = adjStrand;

                l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
                l.crossings_[adj]->prev_[adjStrand].strand_ = 1;

                // Lower outgoing arc:
                a = nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 0 : 1][dir[i]]];
                adj = a.node_->index();
                adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l.crossings_[i]->next_[0].crossing_ = l.crossings_[adj];
                l.crossings_[i]->next_[0].strand_ = adjStrand;

                l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
                l.crossings_[adj]->prev_[adjStrand].strand_ = 0;
            }

            for (const auto& a : componentArcs) {
                size_t i = a.node_->index();
                // We know from above that a.arc_ is either 0 or 1,
                // and that dir[i] sets the bit for a.arc_.
                if (sign[i] > 0) {
                    // If the outgoing arcs are j, j+1 then j is lower.
                    if (dir[i] == (3 << a.arc_)) {
                        // The outgoing arcs are a.arc_, a.arc_+1.
                        l.components_.emplace_back(l.crossings_[i], 0);
                    } else {
                        // The outgoing arcs are a.arc_, a.arc_-1.
                        l.components_.emplace_back(l.crossings_[i], 1);
                    }
                } else {
                    // If the outgoing arcs are j,j+1 then j is upper.
                    if (dir[i] == (3 << a.arc_)) {
                        // The outgoing arcs are a.arc_, a.arc_+1.
                        l.components_.emplace_back(l.crossings_[i], 1);
                    } else {
                        // The outgoing arcs are a.arc_, a.arc_-1.
                        l.components_.emplace_back(l.crossings_[i], 0);
                    }
                }
            }

            action(std::move(l), std::forward<Args>(args)...);

            // Backtrack!
            --curr;
            // Here: 0 <= curr < size (since the model graph is non-empty).
        }

        // Here: 0 <= curr < size.
        if (sign[curr] == 0)
            sign[curr] = 1;
        else if (sign[curr] == 1)
            sign[curr] = -1;
        else {
            // We have exhausted our options here.
            sign[curr--] = 0;
            continue;
        }

        // Move on to the next crossing.
        ++curr;
    }

    // All done!
}

template <typename Action, typename... Args>
void ModelLinkGraph::generateAllEmbeddings(const FacetPairing<3>& pairing,
        bool allowReflection, Action&& action, Args&&... args) {
    const size_t n = pairing.size();
    if (n == 0) {
        // Generate a single empty graph.
        action(ModelLinkGraph(), std::forward<Args>(args)...);
        return;
    }

    if (n > 52)
        throw InvalidArgument("generateAllEmbeddings() can only work with "
            "facet pairings with ≤ 52 simplices");
    if (! pairing.isConnected())
        throw InvalidArgument("generateAllEmbeddings() can only work with "
            "connected facet pairings");
    if (! pairing.isClosed())
        throw InvalidArgument("generateAllEmbeddings() can only work with "
            "closed facet pairings");

    // Prepare a set of found solutions, so we can avoid outputting the same
    // canonical labelling more than once.
    //
    // This is where the algorithm becomes memory-hungry: a better solution
    // would be to take the automorphisms of the facet pairing and use those
    // to avoid duplicates before they are generated.
    //
    // For now, we will store the text output of the found solutions.
    // Again, we could do better if we need to by implementing some kind of
    // packed representation of the graph (or even just a tight encoding).
    std::set<std::string> found;

    // The array perm[] maps facet numbers to arc numbers.
    // It stores second-generation permutation codes for Perm<4>.
    // WLOG, we insist that perm[0] == 0 always, so the permutation codes
    // are all in the range [0,6).
    // If we consider reflections to be the same then for node 0 we only need
    // codes 0,2,4.
    FixedArray<int> perm(n, 0);
    for (perm[0] = 0; perm[0] < 6; perm[0] += (allowReflection ? 2 : 1)) {
        while (true) {
            // Process this set of permutations.
            ModelLinkGraph g;
            for (size_t i = 0; i < n; ++i)
                g.nodes_.push_back(new ModelLinkGraphNode());
            for (size_t i = 0; i < n; ++i) // i == simplex == node
                for (int j = 0; j < 4; ++j) { // j == facet
                    const auto& dest = pairing.dest(i, j);
                    g.nodes_[i]->adj_[Perm<4>::fromPermCode2(perm[i])[j]] =
                        ModelLinkGraphArc(g.nodes_[dest.simp],
                            Perm<4>::fromPermCode2(perm[dest.simp])[dest.facet]);
                }
            ModelLinkGraph canonical = g.canonical(allowReflection);
            if (found.insert(canonical.extendedPlantri()).second)
                action(std::move(canonical), std::forward<Args>(args)...);

            // Advance to the next set of permutations.
            size_t pos = n - 1;
            for ( ; pos > 0; --pos)
                if (perm[pos] < 5) {
                    ++perm[pos];
                    for (size_t i = pos + 1; i < n; ++i)
                        perm[i] = 0;
                    break;
                }
            if (pos == 0) {
                for (size_t i = 1; i < n; ++i)
                    perm[i] = 0;
                break;
            }
        }
    }
}

} // namespace regina

#endif

