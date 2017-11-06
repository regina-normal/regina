
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

ModelLinkGraph::ModelLinkGraph(const ModelLinkGraph& cloneMe) :
        cells_(nullptr) {
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

    // The cellular decomposition takes linear time to copy and linear
    // time to compute, so just recompute it on demand and don't attempt
    // to copy it here.
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

    out << "Outgoing arcs:\n";
    out << "  Node  |  adjacent:      (0)      (1)      (2)      (3)\n";
    out << "  ------+-----------------------------------------------\n";

    size_t i;
    int j;
    ModelLinkGraphNode* n;
    for (i = 0; i < nodes_.size(); ++i) {
        n = nodes_[i];
        out << std::setw(6) << i << "  |           ";
        for (j = 0; j < 4; ++j)
            out << "  " << std::setw(3) << n->adj_[j].node()->index() << " ("
                << n->adj_[j].arc() << ')';
        out << '\n';
    }
    out << std::endl;

    cells().writeTextLong(out);
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

ModelLinkGraphCells::ModelLinkGraphCells(const ModelLinkGraphCells& c) :
        arcs_(new ModelLinkGraphArc[4 * (c.nCells_ == 0 ? 0 : c.nCells_ - 2)]),
        start_(new size_t[1 + c.nCells_]),
        cell_(new size_t[4 * (c.nCells_ == 0 ? 0 : c.nCells_ - 2)]),
        step_(new size_t[4 * (c.nCells_ == 0 ? 0 : c.nCells_ - 2)]),
        nCells_(c.nCells_) {
    if (nCells_ == 0) {
        start_[0] = 0;
        return;
    }

    size_t nArcs = 4 * (nCells_ - 2);
    std::copy(c.arcs_, c.arcs_ + nArcs, arcs_);
    std::copy(c.start_, c.start_ + nCells_ + 1, start_);
    std::copy(c.cell_, c.cell_ + nArcs, cell_);
    std::copy(c.step_, c.step_ + nArcs, step_);
}


ModelLinkGraphCells::ModelLinkGraphCells(const ModelLinkGraph& g) :
        arcs_(new ModelLinkGraphArc[4 * g.size()]),
        start_(new size_t[3 + g.size()]),
        cell_(new size_t[4 * g.size()]),
        step_(new size_t[4 * g.size()]),
        nCells_(2 + g.size()) {
    if (g.size() == 0) {
        nCells_ = 0;
        start_[0] = 0;
        return;
    }

    // Euler characteristic: vertices - arcs + cells = 2
    // Since arcs = 2 * vertices, we have cells = 2 + vertices.

    std::fill(cell_, cell_ + 4 * g.size(), nCells_);

    size_t cell = 0;
    size_t nextArc = 0;
    size_t nextPos = 0;
    start_[0] = 0;
    while (nextArc < 4 * g.size() && cell < nCells_) {
        ModelLinkGraphArc from(g.node(nextArc >> 2), nextArc & 3);
        ModelLinkGraphArc curr(from);
        do {
            cell_[(curr.node()->index() << 2) | curr.arc()] = cell;
            step_[(curr.node()->index() << 2) | curr.arc()] =
                nextPos - start_[cell];
            arcs_[nextPos++] = curr;
            curr = curr.traverse();
            ++curr;
        } while (curr != from);

        while (nextArc < 4 * g.size() && cell_[nextArc] != nCells_)
            ++nextArc;

        start_[++cell] = nextPos;
    }
    if (nextArc < 4 * g.size()) {
        // We found too many cells.
        nCells_ = 0;
        /*
        while (next < 4 * g.size())
            step_[next++] = -1;
        */
    } else if (cell < nCells_) {
        // We did not find enough cells.
        nCells_ = 0;
        /*
        while (cell < 2 + g.size())
            start_[++cell] = 4 * g.size();
        */
    }
}

void ModelLinkGraphCells::writeTextShort(std::ostream& out) const {
    if (nCells_ == 0)
        out << "invalid cell structure";
    else {
        // Must have nCells_ >= 3, so use the plural.
        out << "cell structure with " << nCells_ << " cells";
    }
}

void ModelLinkGraphCells::writeTextLong(std::ostream& out) const {
    if (nCells_ == 0) {
        out << "Invalid cell structure" << std::endl;
        return;
    }

    // Must have nCells_ >= 3, so use the plural.
    // out << "Cell structure with " << nCells_ << " cells\n\n";

    out << "Cell boundaries:\n";
    out << "  Cell  |  node (arc) - (arc) node (arc) - ... - (arc) node\n";
    out << "  ------+--------------------------------------------------\n";
    size_t i;
    const ModelLinkGraphArc* a;
    for (i = 0; i < nCells_; ++i) {
        out << std::setw(6) << i << "  |  ";
        for (a = begin(i); a != end(i); ++a) {
            if (a != begin(i))
                out << " - (" << (a->arc() + 3) % 4 << ") ";
            out << a->node()->index() << " (" << a->arc() << ')';
        }
        a = begin(i);
        out << " - (" << (a->arc() + 3) % 4 << ") " << a->node()->index()
            << '\n';
    }
    out << '\n';
    out << "Cells around each node:\n";
    out << "  Node  |  (arc)  cell_pos  (arc)  cell_pos  ...\n";
    out << "  ------+----------------------------------------\n";

    int j;
    for (i = 0; i < nCells_ - 2; ++i) {
        out << std::setw(6) << i << "  |";
        j = 0;
        do {
            out << "  (" << j << ")  ";
            j = (j + 1) % 4;
            out << cell_[(i << 2) | j] << '_' << step_[(i << 2) | j];
        } while (j != 0);
        out << '\n';
    }

    out << std::endl;
}

std::pair<ModelLinkGraphArc, ModelLinkGraphArc> ModelLinkGraphCells::findFlype(
        const ModelLinkGraphArc& from) const {
    /*

             Cell A

          __   __upper
            \ /                    ----> result.first
             X         Cell B
      back__/ \__from              ----> result.second

             Cell C

    */

    ModelLinkGraphArc upper = from;
    --upper;

    ModelLinkGraphArc back = from;
    ++back;

    if (cell(upper) == cell(back)) {
        // Following upper must return back to from.
        // This means that the crossing (X) is redundant, and can be
        // undone by twisting everything from upper around to from.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    }

    // For each cell adjacent to C, we identify the first arc of C in a
    // clockwise direction from the vertex (X) that borders it.  A null arc
    // means the cell is not adjacent to C at all.
    ModelLinkGraphArc* adjC = new ModelLinkGraphArc[nCells_];
    ModelLinkGraphArc a = back;
    do {
        a = a.traverse();
        adjC[cell(a)] = a;
        ++a;
    } while (a != back);

    // Now walk anticlockwise around cell A from vertex (X) and see if
    // we are ever adjacent to one of the cells that was also adjacent to C.
    // However, to avoid the do-nothing flype, we must pass at least one
    // crossing from X first.
    a = upper.traverse();
    ++a;
    ModelLinkGraphArc b;
    size_t common;
    while (a != upper) {
        b = a.traverse();
        common = cell(b);
        if (adjC[common])
            break;
        a = b;
        ++a;
    }

    delete[] adjC;

    if (a == upper) {
        // The strand upper comes straight back to (X), with no
        // crossings in between.  Note that this means there is a
        // trivial twist that we can undo.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    } else
        return std::make_pair(a, adjC[common]);
}

} // namespace regina

