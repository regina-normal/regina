
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

#include "link.h"
#include "modellinkgraph.h"
#include <iomanip>
#include <sstream>

namespace regina {

namespace {
    const int upperOutArc[2 /* 0,1 for -,+ */][13 /* dir */] = {
        { -1, -1, -1, 0, -1, -1, 1, -1, -1, 3, -1, -1, 2 },
        { -1, -1, -1, 1, -1, -1, 2, -1, -1, 0, -1, -1, 3 }
    };
}

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

void ModelLinkGraph::reflect() {
    for (ModelLinkGraphNode* n : nodes_) {
        std::swap(n->adj_[1], n->adj_[3]);
        for (int i = 0; i < 4; ++i)
            if (n->adj_[i].arc_ % 2)
                n->adj_[i].arc_ ^= 2;
    }

    if (cells_) {
        // The cellular decomposition takes linear time to reflect and
        // linear time to rebuild.  Just rebuild it.
        delete cells_;
        cells_ = nullptr;
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

std::string ModelLinkGraph::plantri() const {
    std::string ans;
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
        if (it != nodes_.begin())
            ans += ',';
        for (int arc = 0; arc < 4; ++arc)
            ans += ('a' + (*it)->adj_[arc].node()->index());
    }
    return ans;
}

std::string ModelLinkGraph::canonicalPlantri(bool useReflection,
        bool tight) const {
    std::string best;

    // The image and preimage for each node, and the image of arc 0
    // for each node:
    ptrdiff_t* image = new ptrdiff_t[size()];
    ptrdiff_t* preimage = new ptrdiff_t[size()];
    int* arcOffset = new int[size()];

    size_t nextUnusedNode, nodeImg, nodeSrc, adjSrcNode;
    int arcImg;
    ModelLinkGraphArc adjSrc;
    bool currBetter;
    for (int reflect = 0; reflect < 2; ++reflect) {
        for (auto start : nodes_)
            for (int offset = 0; offset < 4; ++offset) {
                std::string curr;
                currBetter = best.empty();

                // Map arc (start, offset) -> (0, 0).
                std::fill(image, image + size(), -1);
                std::fill(preimage, preimage + size(), -1);
                nextUnusedNode = 1;

                image[start->index()] = 0;
                preimage[0] = start->index();
                arcOffset[start->index()] = (offset == 0 ? 0 : 4 - offset);

                for (nodeImg = 0; nodeImg < size(); ++nodeImg) {
                    if ((! tight) && nodeImg > 0)
                        curr += ',';

                    // In the image, work out who the neighbours of nodeImg are.
                    nodeSrc = preimage[nodeImg];

                    for (arcImg = (tight && nodeImg > 0 ? 1 : 0);
                            arcImg < 4; ++arcImg) {
                        adjSrc = (reflect ?
                            nodes_[nodeSrc]->
                                adj_[(8 - arcOffset[nodeSrc] - arcImg) % 4] :
                            nodes_[nodeSrc]->
                                adj_[(arcImg + 4 - arcOffset[nodeSrc]) % 4]);
                        adjSrcNode = adjSrc.node()->index();

                        // Is it a new node?
                        if (image[adjSrcNode] < 0) {
                            // Yes.
                            // Map it to the next available image node, and
                            // make the corresponding source arc map to 0.
                            image[adjSrcNode] = nextUnusedNode++;
                            preimage[image[adjSrcNode]] = adjSrcNode;
                            arcOffset[adjSrcNode] =
                                (adjSrc.arc() == 0 ? 0 : 4 - adjSrc.arc());
                        }

                        if (tight && arcImg == 0) {
                            // For node 0, arc 0, we did need to sort
                            // out images and preimages above, but we do not
                            // need to write the corresponding output.
                            continue;
                        }

                        curr += ('a' + image[adjSrcNode]);

                        if (! currBetter) {
                            // curr == best for the characters seen so far.
                            if (curr[curr.length() - 1] <
                                    best[curr.length() - 1])
                                currBetter = true;
                            else if (curr[curr.length() - 1] >
                                    best[curr.length() - 1]) {
                                // There is no chance of this being canonical.
                                goto noncanonical;
                            }
                        }
                    }
                }

                if (best.empty() || curr < best)
                    best.swap(curr);

                noncanonical:
                    ;
            }

        if (! useReflection)
            break;
    }

    delete[] image;
    delete[] preimage;
    delete[] arcOffset;

    return best;
}

ModelLinkGraph* ModelLinkGraph::fromPlantri(const std::string& plantri) {
    bool tight = plantri.size() == 3 ||
        (plantri.size() > 4 && plantri[4] != ',');

    // Extract the graph size and run some basic sanity checks.
    size_t n;
    if (tight) {
        if (plantri.size() % 3 != 0)
            return 0;
        n = plantri.size() / 3;
    } else {
        if (plantri.size() % 5 != 4)
            return 0;
        n = (plantri.size() + 1) / 5;
    }
    if (n > 26)
        return 0;

    size_t i, j;
    for (i = 0; i < plantri.size(); ++i)
        if ((! tight) && i % 5 == 4) {
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
    if (tight) {
        // Node 0, arc 0 is a special case.
        if (n == 1) {
            // (0, 0) links to node 0 - there is no other option.
            g->nodes_[0]->adj_[0].node_ = g->nodes_[0];
        } else {
            // The dual quadrangulation is simple, and this means we
            // cannot have loops for n > 1.  Therefore (0, 0) links to node 1.
            // Since node 1 is new, make the link in both directions.
            g->nodes_[0]->adj_[0].node_ = g->nodes_[1];
            g->nodes_[1]->adj_[0].node_ = g->nodes_[0];
            g->nodes_[1]->adj_[0].arc_ = -1;
        }
        g->nodes_[0]->adj_[0].arc_ = -1;

        for (i = 0; i < n; ++i)
            for (j = 1; j < 4; ++j) {
                g->nodes_[i]->adj_[j].node_ =
                    g->nodes_[plantri[3 * i + j - 1] - 'a'];
                if (! g->nodes_[i]->adj_[j].node_->adj_[0].node_) {
                    // This is the first time we've seen this adjacent node.
                    // Make the link in the reverse direction also.
                    g->nodes_[i]->adj_[j].node_->adj_[0].node_ = g->nodes_[i];
                    g->nodes_[i]->adj_[j].node_->adj_[0].arc_ = -1;
                }
                g->nodes_[i]->adj_[j].arc_ = -1;
            }
    } else {
        for (i = 0; i < n; ++i)
            for (j = 0; j < 4; ++j) {
                g->nodes_[i]->adj_[j].node_ =
                    g->nodes_[plantri[5 * i + j] - 'a'];
                g->nodes_[i]->adj_[j].arc_ = -1;
            }
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
                    if (dest->adj_[3 - k].node_ != src) {
                        delete g;
                        return 0;
                    }
                    if (dest != src && dest->adj_[3 - k].arc_ >= 0) {
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

void ModelLinkGraph::generateMinimalLinks(Use use, void* useArgs) const {
    if (size() == 0) {
        // Generate a single empty link.
        use(new Link(), useArgs);
        use(nullptr, useArgs);
        return;
    }

    // First work out the orientation of the knot as it passes through
    // each node.
    char* dir = new char[size()]; // Bits 0,1,2,3 are 1/0 for forward/backward.
    std::fill(dir, dir + size(), 0);

    size_t steps = 0;
    ModelLinkGraphArc a(nodes_[0], 0);
    do {
        dir[a.node()->index()] |= (1 << a.arc());
        a = a.next();
        ++steps;
    } while (a.node()->index() != 0 || a.arc() != 0);

    if (steps != 2 * size()) {
        std::cerr << "ERROR: Not a knot graph!" << std::endl;
        use(nullptr, useArgs);
        delete[] dir;
        return;
    }

    // Next work out which relationships we may assume between different
    // crossing signs.  This will be a quadratic-time "poor man's union-find" -
    // the criterion for how to join subtrees is not depth (to keep the
    // worst-case depth logarithmic), but rather the insistence that
    // parent[i] < i (so we can easily choose the parent sign before the child).

    cells(); // force computation of cell structure

    // If parent[i] >= 0, then the sign of crossing i is tied to the
    // sign of crossing parent[i].  The signs are the same if flip[i] is false,
    // and the signs are different if flip[i] is true.
    // We guarantee for all nodes that parent[i] < i.
    int* parent = new int[size()];
    bool* flip = new bool[size()];

    std::fill(parent, parent + size(), -1);

    int i;
    ModelLinkGraphArc a1, a2;
    int n1, n2, n3;
    bool flip1, flip2, flip3;
    for (i = 0; i < cells_->nCells_; ++i)
        if (cells_->size(i) == 2) {
            // Both crossings on the bigon should have the same sign.
            a1 = cells_->arc(i, 0);
            a2 = cells_->arc(i, 1);
            n1 = a1.node()->index();
            n2 = a2.node()->index();
            flip1 = flip2 = false;
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
            }

            // At this point we will modify arcs a1 and a2, but not their nodes.
            ++a1;
            ++a2;
            if (cells_->size(cells_->cell(a1)) == 3) {
                // We have a triangle beside the original arc a1.
                // The third crossing of the triangle has its sign forced also.
                n3 = a1.traverse().node()->index();
                flip3 = (
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
                }
            }
            if (cells_->size(cells_->cell(a2)) == 3) {
                // We have a triangle beside the original arc a2.
                // As above.
                n3 = a2.traverse().node()->index();
                flip3 = (
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
                }
            }
        }

    // Now choose the signs of the crossings!
    int* sign = new int[size()];
    std::fill(sign, sign + size(), 0);

    int curr = 0;
    int adj;
    int adjStrand;
    while (curr >= 0) {
        // We have selected the signs for all crossings < curr, and we
        // need to move to the next available sign at crossing curr.
        if (curr == size()) {
            // We have a complete selection of crossings.
            Link* l = new Link;
            for (i = 0; i < size(); ++i)
                l->crossings_.push_back(new Crossing(sign[i]));
            for (i = 0; i < size(); ++i) {
                // Upper outgoing arc:
                a = nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 1 : 0][dir[i]]];
                adj = a.node_->index();
                adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l->crossings_[i]->next_[1].crossing_ = l->crossings_[adj];
                l->crossings_[i]->next_[1].strand_ = adjStrand;

                l->crossings_[adj]->prev_[adjStrand].crossing_ =
                    l->crossings_[i];
                l->crossings_[adj]->prev_[adjStrand].strand_ = 1;

                // Lower outgoing arc:
                a = nodes_[i]->adj_[upperOutArc[sign[i] > 0 ? 0 : 1][dir[i]]];
                adj = a.node_->index();
                adjStrand = (a.arc_ ==
                    (upperOutArc[sign[adj] > 0 ? 1 : 0][dir[adj]] ^ 2) ? 1 : 0);
                l->crossings_[i]->next_[0].crossing_ = l->crossings_[adj];
                l->crossings_[i]->next_[0].strand_ = adjStrand;

                l->crossings_[adj]->prev_[adjStrand].crossing_ =
                    l->crossings_[i];
                l->crossings_[adj]->prev_[adjStrand].strand_ = 0;
            }
            l->components_.push_back(StrandRef(*l->crossings_.begin(), 1));

            use(l, useArgs);

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
    use(nullptr, useArgs);

    delete[] sign;
    delete[] flip;
    delete[] parent;
    delete[] dir;
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

} // namespace regina

