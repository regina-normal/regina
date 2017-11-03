
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

#include "modellinkgraph.h"
#include <iomanip>
#include <sstream>

namespace regina {

ModelLinkGraph::ModelLinkGraph(const ModelLinkGraph& cloneMe) {
    for (ModelLinkGraphNode* n : cloneMe.nodes_)
        nodes_.push_back(new ModelLinkGraphNode());

    auto it = cloneMe.nodes_.begin();
    for (ModelLinkGraphNode* n : nodes_) {
        for (int i = 0; i < 4; ++i) {
            n->adj_[i].node_ = nodes_[(*it)->adj_[i].node_->index()];
            n->adj_[i].arc_ = (*it)->adj_[i].arc_;
        }
        ++it;
    }
}

void ModelLinkGraph::writeTextShort(std::ostream& out) const {
    if (nodes_.empty())
        out << "empty model link graph";
    else
        out << nodes_.size() << "-node model link graph";
}

void ModelLinkGraph::writeTextLong(std::ostream& out) const {
    if (nodes_.empty()) {
        out << "Empty model link graph" << std::endl;
        return;
    }

    out << nodes_.size() << "-node model link graph\n\n";

    out << "Node  |  adjacent to:      (0)      (1)      (2)      (3)\n";
    out << "------+--------------------------------------------------\n";

    size_t i;
    int j;
    ModelLinkGraphNode* n;
    for (i = 0; i < nodes_.size(); ++i) {
        n = nodes_[i];
        out << std::setw(4) << i << "  |              ";
        for (j = 0; j < 4; ++j)
            out << "  " << std::setw(3) << n->adj_[j].node()->index() << " ("
                << n->adj_[j].arc() << ')';
        out << '\n';
    }
    out << std::endl;
}

ModelLinkGraph* ModelLinkGraph::fromPlantri(const std::string& plantri) {
    // Extract the graph size and run some basic sanity checks.
    if (plantri.size() % 5 != 4)
        return 0;
    size_t n = (plantri.size() + 1) / 5;
    if (n > 26)
        return 0;

    size_t i, j;
    for (i = 0; i < plantri.size(); ++i)
        if (i % 5 == 4) {
            if (plantri[i] != ',')
                return 0;
        } else {
            if (plantri[i] < 'a' || plantri[i] >= ('a' + n))
                return 0;
        }

    ModelLinkGraph* g = new ModelLinkGraph();
    for (i = 0; i < n; ++i)
        g->nodes_.push_back(new ModelLinkGraphNode());

    // First set up adj_[..].node_.
    for (i = 0; i < n; ++i)
        for (j = 0; j < 4; ++j) {
            g->nodes_[i]->adj_[j].node_ =
                g->nodes_[plantri[5 * i + j] - 'a'];
            g->nodes_[i]->adj_[j].arc_ = -1;
        }

    // Now set up adj_[..].arc_.
    // For each pair of adjacent nodes, we guarantee to set up all edges
    // between those nodes, in both directions, at the same time.
    int count;
    int k;
    ModelLinkGraphNode *src, *dest;
    for (i = 0; i < n; ++i) {
        src = g->nodes_[i];
        for (j = 0; j < 4; ++j) {
            if (src->adj_[j].arc_ >= 0)
                continue;

            // Examine node i, arc j.
            dest = src->adj_[j].node_;

            // Is this one of a double / triple / quadruple edge?
            count = 1;
            for (k = j + 1; k < 4; ++k)
                if (src->adj_[k].node_ == dest)
                    ++count;

            // Be careful about when we can have loops.
            if (src == dest && count % 2 != 0) {
                delete g;
                return 0;
            }

            // In the code below, we use the fact that plantri only produces
            // 4-valent graphs whose dual quadrangulations are simple.
            if (count == 1) {
                // This is just a single edge.  Find the matching arc
                // from dest.
                for (k = 0; k < 4; ++k)
                    if (dest->adj_[k].node_ == src) {
                        if (dest->adj_[k].arc_ >= 0) {
                            delete g;
                            return 0;
                        }
                        src->adj_[j].arc_ = k;
                        dest->adj_[k].arc_ = j;
                        break;
                    }
                if (k == 4) {
                    delete g;
                    return 0;
                }
            } else if (count == 2) {
                if (src->adj_[j ^ 2].node_ == dest) {
                    // We have two parallel edges that are not adjacent
                    // around src.
                    //
                    // Because the dual quadrangulation must be simple,
                    // it follows that any double edge of this type must
                    // actually be part of a quadruple edge.
                    delete g;
                    return 0;
                } else {
                    // We have two parallel edges that are adjacent around src.
                    //
                    // Because the dual quadrangulation must be simple,
                    // these parallel edges must bound a bigon.
                    // This means that we follow them clockwise around one node
                    // and anticlockwise around the other.

                    // We already have j as the first of the two arcs
                    // around src.
                    // Find the "clockwise first" arc around dest.
                    for (k = 0; k < 4; ++k)
                        if (dest->adj_[k].node_ == src &&
                                dest->adj_[(k + 1) % 4].node_ == src) {
                            if (dest->adj_[k].arc_ >= 0 ||
                                    dest->adj_[(k + 1) % 4].arc_ >= 0) {
                                delete g;
                                return 0;
                            }
                            break;
                        }
                    if (k == 4) {
                        delete g;
                        return 0;
                    }

                    if (j < 3 && src->adj_[j + 1].node_ == dest) {
                        src->adj_[j].arc_ = (k + 1) % 4;
                        src->adj_[j + 1].arc_ = k;
                        dest->adj_[k].arc_ = j + 1;
                        dest->adj_[(k + 1) % 4].arc_ = j;
                    } else {
                        // The arcs from src must be 0 and 3.
                        src->adj_[3].arc_ = (k + 1) % 4;
                        src->adj_[0].arc_ = k;
                        dest->adj_[k].arc_ = 0;
                        dest->adj_[(k + 1) % 4].arc_ = 3;
                    }
                }
            } else if (count == 3) {
                // Because the dual quadrangulation must be simple, it
                // follows that any triple edge must actually be part
                // of a quadruple edge.
                delete g;
                return 0;
            } else {
                // A quadruple edge.
                // As we walk clockwise around one node, we must walk
                // anticlockwise around the other.
                //
                // We will match up (0,1,2,3) <-> (3,2,1,0).
                // Note that this scheme also works if src == dest.
                for (k = 0; k < 4; ++k) {
                    if (dest->adj_[k].node_ != src || dest->adj_[k].arc_ >= 0) {
                        delete g;
                        return 0;
                    }
                    src->adj_[k].arc_ = 3 - k;
                    dest->adj_[3 - k].arc_ = k;
                }
            }
        }
    }

    return g;
}

} // namespace regina

