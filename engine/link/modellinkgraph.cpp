
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

#include "link.h"
#include "modellinkgraph.h"
#include <iomanip>
#include <sstream>

namespace {
    // These routines support plantri encoding/decoding.

    // These encLess() routines compare case-sensitive letters in the order:
    // a < b < ... < z < A < B < ... < Z.
    // The awkwardness here of course comes from the fact that lower-case
    // letters have higher integer ASCII values than upper-case letters.

    // PRE: a, b both in [a..zA..Z]
    inline bool encLess(char a, char b) {
        if (a >= 'a') {
            // a is lower-case
            return (b > a || b < 'a');
        } else {
            // a is upper-case
            return (b > a && b < 'a');
        }
    }

    // PRE: all characters of a, b in [a..zA..Z]
    inline bool encLess(const char* a, const char* b) {
        while (true) {
            if (! (*b))
                return false;
            if (! (*a))
                return true;
            if ((*a) == (*b)) {
                ++a;
                ++b;
                continue;
            }
            return encLess(*a, *b);
        }
    }

    // PRE: nodes <= 52
    inline bool encInRange(char c, size_t nodes) {
        if (nodes <= 26)
            return (c >= 'a' && c < static_cast<char>('a' + nodes));
        else
            return ((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c < static_cast<char>('A' + nodes - 26)));
    }

    // PRE: c in [a..zA..Z]
    inline int encToIndex(char c) {
        return (c >= 'a' ? c - 'a' : c - 'A' + 26);
    }
}

namespace regina {

int ModelLinkGraphNode::loops() const {
    int ans = 0;
    for (int i = 0; i < 4; ++i)
        if (adj_[i].node() == this)
            ++ans;
    return ans >> 1; // each loop is counted twice, once from each end
}

int ModelLinkGraphNode::bigons() const {
    int ans = 0;
    for (int i = 0; i < 4; ++i)
        if (adj_[i].node() != this) {
            auto next = (i+1) % 4;
            if (adj_[i].node() == adj_[next].node()) {
                auto left = adj_[i];
                --left;
                if (left == adj_[next])
                    ++ans;
            }
        }
    return ans;
}

int ModelLinkGraphNode::triangles() const {
    int ans = 0;
    for (int i = 0; i < 4; ++i) {
        if (adj_[i].node() != this) {
            auto next = (i+1) % 4;
            if (adj_[i].node() != adj_[next].node()) {
                auto left = adj_[i];
                --left;
                auto right = adj_[next];
                ++right;
                if (left.traverse() == right)
                    ++ans;
            }
        }
    }
    return ans;
}

ModelLinkGraph::ModelLinkGraph(const ModelLinkGraph& copy) :
        nComponents_(copy.nComponents_), cells_(nullptr) {
    nodes_.reserve(copy.nodes_.size());
    for (size_t i = 0; i < copy.nodes_.size(); ++i)
        nodes_.push_back(new ModelLinkGraphNode());

    auto it = copy.nodes_.begin();
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

ModelLinkGraph::ModelLinkGraph(const Link& link) :
        nComponents_(-1), cells_(nullptr) {
    nodes_.reserve(link.size());
    for (size_t i = 0; i < link.size(); ++i)
        nodes_.push_back(new ModelLinkGraphNode());

    for (Crossing* c : link.crossings_) {
        for (int strand = 0; strand < 2; ++strand) {
            ModelLinkGraphArc out = outgoingArc(c->strand(strand));
            ModelLinkGraphArc in = incomingArc(c->next(strand));
            out.node_->adj_[out.arc_] = in;
            in.node_->adj_[in.arc_] = out;
        }
    }
}

ModelLinkGraph& ModelLinkGraph::operator = (const ModelLinkGraph& src) {
    if (std::addressof(src) == this)
        return *this;

    for (ModelLinkGraphNode* n : nodes_)
        delete n;
    nodes_.clear();

    nodes_.reserve(src.nodes_.size());
    for (size_t i = 0; i < src.nodes_.size(); ++i)
        nodes_.push_back(new ModelLinkGraphNode());

    auto it = src.nodes_.begin();
    for (ModelLinkGraphNode* n : nodes_) {
        for (int i = 0; i < 4; ++i) {
            n->adj_[i].node_ = nodes_[(*it)->adj_[i].node_->index()];
            n->adj_[i].arc_ = (*it)->adj_[i].arc_;
        }
        ++it;
    }

    nComponents_ = src.nComponents_;

    // The cellular decomposition takes linear time to copy and linear
    // time to compute, so just recompute it on demand and don't attempt
    // to copy it here.
    if (cells_) {
        delete cells_;
        cells_ = nullptr;
    }

    return *this;
}

bool ModelLinkGraph::operator == (const ModelLinkGraph& other) const {
    if (nodes_.size() != other.nodes_.size())
        return false;

    auto a = nodes_.begin();
    auto b = other.nodes_.begin();
    for ( ; a != nodes_.end(); ++a, ++b) {
        for (int i = 0; i < 4; ++i) {
            if ((*a)->adj_[i].node_->index() != (*b)->adj_[i].node_->index())
                return false;
            if ((*a)->adj_[i].arc_ != (*b)->adj_[i].arc_)
                return false;
        }
    }
    return true;
}

void ModelLinkGraph::reflect() {
    for (ModelLinkGraphNode* n : nodes_) {
        std::swap(n->adj_[1], n->adj_[3]);
        for (auto& a : n->adj_)
            if (a.arc_ % 2)
                a.arc_ ^= 2;
    }

    if (cells_) {
        // The cellular decomposition takes linear time to reflect and
        // linear time to rebuild.  Just rebuild it.
        delete cells_;
        cells_ = nullptr;
    }
}

void ModelLinkGraph::computeComponents() const {
    if (nodes_.size() <= 1) {
        const_cast<ModelLinkGraph*>(this)->nComponents_ = nodes_.size();
        return;
    }

    // Just another depth-first search.

    FixedArray<bool> seen(nodes_.size(), false);
    FixedArray<const ModelLinkGraphNode*> stack(nodes_.size());
    size_t stackSize = 0;

    auto nextComponent = nodes_.begin();
    size_t foundComponents = 0;

    while (nextComponent != nodes_.end()) {
        stack[0] = *nextComponent++;
        stackSize = 1;
        seen[stack[0]->index()] = true;
        ++foundComponents;

        while (stackSize > 0) {
            auto curr = stack[--stackSize];

            for (int i = 0; i < 4; ++i) {
                auto adj = curr->adj_[i].node_;
                if (! seen[adj->index()]) {
                    seen[adj->index()] = true;
                    stack[stackSize++] = adj;
                }
            }
        }

        while (nextComponent != nodes_.end() && seen[(*nextComponent)->index()])
            ++nextComponent;
    }

    const_cast<ModelLinkGraph*>(this)->nComponents_ = foundComponents;
}

bool ModelLinkGraph::isSimple() const {
    for (auto n : nodes_)
        for (int i = 0; i < 4; ++i)
            if (n->adj_[i].node_ == n ||
                    n->adj_[i].node_ == n->adj_[(i + 1) % 4].node_)
                return false;
    return true;
}

Link ModelLinkGraph::generateAnyLink() const {
    if (size() == 0)
        return Link();

    // First work out the orientation of the link components as they pass
    // through each node.
    FixedArray<char> dir(size(), 0); // Bits 0,1,2,3 are 1/0 for forward/back.

    std::vector<ModelLinkGraphArc> componentArcs;

    size_t steps = 0;
    for (size_t i = 0; i < size(); ++i) {
        auto node = nodes_[i];

        // Look for a new component passing through arcs 0 and 2:
        if ((dir[node->index()] & 5 /* 0101 */) == 0) {
            ModelLinkGraphArc a(node, 0);
            componentArcs.push_back(a);
            do {
                dir[a.node()->index()] |= (1 << a.arc());
                a = a.next();
                ++steps;
            } while (a.node() != node || a.arc() != 0);
        }

        // Look for a new component passing through arcs 1 and 3:
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

    // Go ahead and build the link.
    // We will make every crossing positive.
    Link l;
    for (size_t i = 0; i < size(); ++i)
        l.crossings_.push_back(new Crossing(1));
    for (size_t i = 0; i < size(); ++i) {
        // Upper outgoing arc:
        ModelLinkGraphArc a = nodes_[i]->adj_[upperOutArc[1][dir[i]]];
        size_t adj = a.node_->index();
        int adjStrand = (a.arc_ == (upperOutArc[1][dir[adj]] ^ 2) ? 1 : 0);
        l.crossings_[i]->next_[1].crossing_ = l.crossings_[adj];
        l.crossings_[i]->next_[1].strand_ = adjStrand;

        l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
        l.crossings_[adj]->prev_[adjStrand].strand_ = 1;

        // Lower outgoing arc:
        a = nodes_[i]->adj_[upperOutArc[0][dir[i]]];
        adj = a.node_->index();
        adjStrand = (a.arc_ == (upperOutArc[1][dir[adj]] ^ 2) ? 1 : 0);
        l.crossings_[i]->next_[0].crossing_ = l.crossings_[adj];
        l.crossings_[i]->next_[0].strand_ = adjStrand;

        l.crossings_[adj]->prev_[adjStrand].crossing_ = l.crossings_[i];
        l.crossings_[adj]->prev_[adjStrand].strand_ = 0;
    }
    for (const auto& a : componentArcs) {
        size_t i = a.node_->index();
        // We know from above that a.arc_ is either 0 or 1,
        // and that dir[i] sets the bit for a.arc_.
        //
        // Since all crossings are positive: if the outgoing arcs are j, j+1
        // then j is lower.
        if (dir[i] == (3 << a.arc_)) {
            // The outgoing arcs are a.arc_, a.arc_+1, so a.arc_ is lower.
            l.components_.emplace_back(l.crossings_[i], 0);
        } else {
            // The outgoing arcs are a.arc_, a.arc_-1, so a.arc_ is upper.
            l.components_.emplace_back(l.crossings_[i], 1);
        }
    }

    return l;
}

void ModelLinkGraph::writeTextShort(std::ostream& out) const {
    if (nodes_.empty()) {
        out << "Empty graph";
        return;
    }

    size_t g = cells().genus();
    if (g == 0)
        out << nodes_.size() << "-node planar graph: ";
    else
        out << nodes_.size() << "-node genus " << g << " graph: ";
    for (size_t i = 0; i < nodes_.size(); ++i) {
        if (i > 0)
            out << ' ';

        auto n = nodes_[i];
        out << '[';
        for (int j = 0; j < 4; ++j) {
            if (j > 0)
                out << ' ';
            out << n->adj_[j].node()->index() << ':' << n->adj_[j].arc();
        }
        out << ']';
    }
}

void ModelLinkGraph::writeTextLong(std::ostream& out) const {
    if (nodes_.empty()) {
        out << "Empty model link graph" << std::endl;
        return;
    }

    size_t g = cells().genus();
    if (g == 0)
        out << nodes_.size() << "-node planar model link graph\n\n";
    else
        out << nodes_.size() << "-node genus " << g << " model link graph: ";

    out << "Outgoing arcs:\n";
    out << "  Node  |  adjacent:      (0)      (1)      (2)      (3)\n";
    out << "  ------+-----------------------------------------------\n";

    for (size_t i = 0; i < nodes_.size(); ++i) {
        auto n = nodes_[i];
        out << std::setw(6) << i << "  |           ";
        for (const auto& arc : n->adj_)
            out << "  " << std::setw(3) << arc.node()->index() << " ("
                << arc.arc() << ')';
        out << '\n';
    }
    out << std::endl;

    cells().writeTextLong(out);
}

std::string ModelLinkGraph::plantri() const {
    if (size() == 0 || size() > 52)
        throw FailedPrecondition("plantri() can only work with "
            "graphs with between 1 and 52 nodes inclusive");

    std::string ans;
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
        if (it != nodes_.begin())
            ans += ',';
        for (const auto& arc : (*it)->adj_) {
            auto idx = arc.node()->index();
            if (idx < 26)
                ans += static_cast<char>('a' + idx);
            else
                ans += static_cast<char>('A' + idx - 26);
        }
    }
    return ans;
}

std::string ModelLinkGraph::canonicalPlantri(bool useReflection,
        bool tight) const {
    if (size() == 0 || size() > 52)
        throw FailedPrecondition("canonicalPlantri() can only work with "
            "graphs with between 1 and 52 nodes inclusive");

    std::string best;

    // The image and preimage for each node, and the image of arc 0
    // for each node:
    auto* image = new ssize_t[size()];
    auto* preimage = new ssize_t[size()];
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

                        if (image[adjSrcNode] < 26)
                            curr += static_cast<char>('a' + image[adjSrcNode]);
                        else
                            curr += static_cast<char>('A' + image[adjSrcNode]
                                - 26);

                        if (! currBetter) {
                            // curr == best for the characters seen so far.
                            if (encLess(curr[curr.length() - 1],
                                    best[curr.length() - 1]))
                                currBetter = true;
                            else if (encLess(best[curr.length() - 1],
                                    curr[curr.length() - 1])) {
                                // There is no chance of this being canonical.
                                goto noncanonical;
                            }
                        }
                    }
                }

                if (best.empty() || encLess(curr.c_str(), best.c_str()))
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

ModelLinkGraph ModelLinkGraph::fromPlantri(const std::string& plantri) {
    bool tight = plantri.size() == 3 ||
        (plantri.size() > 4 && plantri[4] != ',');

    // Extract the graph size and run some basic sanity checks.
    size_t n;
    if (tight) {
        if (plantri.size() % 3 != 0)
            throw InvalidArgument("fromPlantri(): "
                "invalid string length for a tight encoding");
        n = plantri.size() / 3;
    } else {
        if (plantri.size() % 5 != 4)
            throw InvalidArgument("fromPlantri(): "
                "invalid string length for a standard encoding");
        n = (plantri.size() + 1) / 5;
    }
    if (n > 52)
        throw InvalidArgument("fromPlantri(): more than 52 nodes");

    size_t i;
    for (i = 0; i < plantri.size(); ++i)
        if ((! tight) && i % 5 == 4) {
            if (plantri[i] != ',')
                throw InvalidArgument("fromPlantri(): missing comma");
        } else {
            if (! encInRange(plantri[i], n))
                throw InvalidArgument("fromPlantri(): invalid node letter");
        }

    ModelLinkGraph g;
    for (i = 0; i < n; ++i)
        g.nodes_.push_back(new ModelLinkGraphNode());

    // First set up adj_[..].node_.
    if (tight) {
        // Node 0, arc 0 is a special case.
        if (n == 1) {
            // (0, 0) links to node 0 - there is no other option.
            g.nodes_[0]->adj_[0].node_ = g.nodes_[0];
        } else {
            // The dual quadrangulation is simple, and this means we
            // cannot have loops for n > 1.  Therefore (0, 0) links to node 1.
            // Since node 1 is new, make the link in both directions.
            g.nodes_[0]->adj_[0].node_ = g.nodes_[1];
            g.nodes_[1]->adj_[0].node_ = g.nodes_[0];
            g.nodes_[1]->adj_[0].arc_ = -1;
        }
        g.nodes_[0]->adj_[0].arc_ = -1;

        for (i = 0; i < n; ++i)
            for (int j = 1; j < 4; ++j) {
                g.nodes_[i]->adj_[j].node_ =
                    g.nodes_[encToIndex(plantri[3 * i + j - 1])];
                if (! g.nodes_[i]->adj_[j].node_->adj_[0].node_) {
                    // This is the first time we've seen this adjacent node.
                    // Make the link in the reverse direction also.
                    g.nodes_[i]->adj_[j].node_->adj_[0].node_ = g.nodes_[i];
                    g.nodes_[i]->adj_[j].node_->adj_[0].arc_ = -1;
                }
                g.nodes_[i]->adj_[j].arc_ = -1;
            }
    } else {
        for (i = 0; i < n; ++i)
            for (int j = 0; j < 4; ++j) {
                g.nodes_[i]->adj_[j].node_ =
                    g.nodes_[encToIndex(plantri[5 * i + j])];
                g.nodes_[i]->adj_[j].arc_ = -1;
            }
    }

    // Now set up adj_[..].arc_.
    // For each pair of adjacent nodes, we guarantee to set up all edges
    // between those nodes, in both directions, at the same time.
    int count;
    int k;
    ModelLinkGraphNode *src, *dest;
    for (i = 0; i < n; ++i) {
        src = g.nodes_[i];
        for (int j = 0; j < 4; ++j) {
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
            if (src == dest && count % 2 != 0)
                throw InvalidArgument("fromPlantri(): invalid loop");

            // In the code below, we use the precondition that the graph is
            // dual to a simple quadrangulation of the surface in which it
            // embeds.
            if (count == 1) {
                // This is just a single edge.  Find the matching arc from dest.
                for (k = 0; k < 4; ++k)
                    if (dest->adj_[k].node_ == src) {
                        if (dest->adj_[k].arc_ >= 0)
                            throw InvalidArgument("fromPlantri(): "
                                "single edge has multiple endpoints");
                        src->adj_[j].arc_ = k;
                        dest->adj_[k].arc_ = j;
                        break;
                    }
                if (k == 4)
                    throw InvalidArgument("fromPlantri(): single edge "
                        "has no endpoint");
            } else if (count == 2) {
                // We have a double edge.
                // The only configuration dual to a simple quadrangulation is
                // the one that produces a bigon.  In particular, the two
                // endpoints of the parallel edges must be adjacent at both
                // src and dest.
                if (src->adj_[j ^ 2].node_ == dest)
                    throw InvalidArgument("fromPlantri(): invalid "
                        "non-adjacent double edge");

                // Since our two parallel edges must bound a bigon, we can
                // follow the corresponding arcs clockwise around one node
                // and anticlockwise around the other.

                // We already have j as the first of the two arcs around src.
                // Find the "clockwise first" arc around dest.
                for (k = 0; k < 4; ++k)
                    if (dest->adj_[k].node_ == src &&
                            dest->adj_[(k + 1) % 4].node_ == src) {
                        if (dest->adj_[k].arc_ >= 0 ||
                                dest->adj_[(k + 1) % 4].arc_ >= 0)
                            throw InvalidArgument("fromPlantri(): "
                                "double edge has too many endpoints");
                        break;
                    }
                if (k == 4)
                    throw InvalidArgument("fromPlantri(): double edge "
                        "missing its endpoints");

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
            } else if (count == 3) {
                // A triple edge will never appear in a graph whose dual
                // quadrangulation is simple.
                throw InvalidArgument("fromPlantri(): invalid triple edge");
            } else {
                // A quadruple edge.
                // The only configuration whose dual quadrangulation is simple
                // is the one in which, as we walk clockwise around one node,
                // we walk anticlockwise around the other.  (This is a
                // standalone graph component that models the Hopf link.)
                //
                // We will match up (0,1,2,3) <-> (3,2,1,0).
                // Note that this scheme also works if src == dest.
                for (k = 0; k < 4; ++k) {
                    if (dest->adj_[3 - k].node_ != src)
                        throw InvalidArgument("fromPlantri(): "
                            "quadruple edge has a missing endpoint");
                    if (dest != src && dest->adj_[3 - k].arc_ >= 0)
                        throw InvalidArgument("fromPlantri(): "
                            "quadruple edge has too many endpoints");
                    src->adj_[k].arc_ = 3 - k;
                    dest->adj_[3 - k].arc_ = k;
                }
            }
        }
    }

    return g;
}

std::string ModelLinkGraph::extendedPlantri() const {
    if (size() == 0 || size() > 52)
        throw FailedPrecondition("extendedPlantri() can only work with "
            "graphs with between 1 and 52 nodes inclusive");

    std::string ans;
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
        if (it != nodes_.begin())
            ans += ',';
        for (const auto& arc : (*it)->adj_) {
            auto idx = arc.node()->index();
            if (idx < 26)
                ans += static_cast<char>('a' + idx);
            else
                ans += static_cast<char>('A' + idx - 26);
            ans += static_cast<char>('0' + arc.arc());
        }
    }
    return ans;
}

ModelLinkGraph ModelLinkGraph::fromExtendedPlantri(const std::string& text) {
    // Extract the graph size and run some basic sanity checks.
    if (text.size() % 9 != 8)
        throw InvalidArgument("fromExtendedPlantri(): "
            "invalid string length for a standard encoding");
    size_t n = (text.size() + 1) / 9;
    if (n > 52)
        throw InvalidArgument("fromExtendedPlantri(): more than 52 nodes");

    size_t i;
    for (i = 0; i < text.size(); ++i) {
        size_t offset = i % 9;
        if (offset == 8) {
            if (text[i] != ',')
                throw InvalidArgument("fromExtendedPlantri(): missing comma");
        } else if (offset % 2 == 0) {
            if (! encInRange(text[i], n))
                throw InvalidArgument("fromExtendedPlantri(): "
                    "invalid node letter");
        } else {
            if (text[i] < '0' || text[i] > '3')
                throw InvalidArgument("fromExtendedPlantri(): "
                    "invalid arc number");
        }
    }

    ModelLinkGraph g;
    for (i = 0; i < n; ++i)
        g.nodes_.push_back(new ModelLinkGraphNode());

    for (i = 0; i < n; ++i) {
        auto srcNode = g.nodes_[i];
        for (int j = 0; j < 4; ++j) {
            ModelLinkGraphArc src(srcNode, j);
            ModelLinkGraphArc dest(
                g.nodes_[encToIndex(text[9 * i + 2 * j])],
                text[9 * i + 2 * j + 1] - '0');
            srcNode->adj_[j] = dest;

            if (auto back = dest.node()->adj_[dest.arc()])
                if (back.node() != srcNode || back.arc() != j)
                    throw InvalidArgument("fromExtendedPlantri(): "
                        "mismatched connections between arcs");
        }
    }

    return g;
}

ModelLinkGraphArc ModelLinkGraph::outgoingArc(const StrandRef& s) {
    if (s.strand() == 0)
        return { nodes_[s.crossing()->index()], 0 };
    else if (s.crossing()->sign() > 0)
        return { nodes_[s.crossing()->index()], 1 };
    else
        return { nodes_[s.crossing()->index()], 3 };
}

ModelLinkGraphArc ModelLinkGraph::incomingArc(const StrandRef& s) {
    if (s.strand() == 0)
        return { nodes_[s.crossing()->index()], 2 };
    else if (s.crossing()->sign() > 0)
        return { nodes_[s.crossing()->index()], 3 };
    else
        return { nodes_[s.crossing()->index()], 1 };
}

ModelLinkGraphCells::ModelLinkGraphCells(const ModelLinkGraph& g) :
        nCells_(0),
        nComponents_(g.countComponents()),
        arcs_(4 * g.size()),
        start_(1 + g.size() + 2 * nComponents_), // 1 + upper bound on #cells
        cell_(4 * g.size()),
        step_(4 * g.size()) {
    if (g.size() == 0) {
        start_[0] = 0;
        return;
    }

    // We need a value for the cell number that means "not yet computed".
    // For this we will use the maximum possible number of cells, which is
    // what we would get in the planar case:
    const size_t maxCells = g.size() + 2 * nComponents_;
    std::fill(cell_.begin(), cell_.end(), maxCells);

    size_t nextArc = 0;
    size_t nextPos = 0;
    start_[0] = 0;
    while (nextArc < 4 * g.size()) {
        // Explore the boundary of the next cell.
        if (nCells_ == maxCells)
            throw InvalidArgument("ModelLinkGraph has more cells than should "
                "be possible");

        ModelLinkGraphArc from(g.node(nextArc >> 2), nextArc & 3);
        ModelLinkGraphArc curr(from);
        do {
            cell_[(curr.node()->index() << 2) | curr.arc()] = nCells_;
            step_[(curr.node()->index() << 2) | curr.arc()] =
                nextPos - start_[nCells_];
            arcs_[nextPos++] = curr;
            curr = curr.traverse();
            ++curr;
        } while (curr != from);

        while (nextArc < 4 * g.size() && cell_[nextArc] != maxCells)
            ++nextArc;

        start_[++nCells_] = nextPos;
    }
}

bool ModelLinkGraphCells::operator == (const ModelLinkGraphCells& other) const {
    if (nCells_ != other.nCells_)
        return false;

    // Don't compare the full start_ arrays, since these might contain unused
    // space at the end.  Instead just compare the sections of the arrays that
    // are used.
    if (! std::equal(start_.begin(), start_.begin() + nCells_ + 1,
            other.start_.begin()))
        return false;

    for (size_t i = 0; i < start_[nCells_]; ++i) {
        if (arcs_[i].node()->index() != other.arcs_[i].node()->index())
            return false;
        if (arcs_[i].arc() != other.arcs_[i].arc())
            return false;
    }

    return true;
}

void ModelLinkGraphCells::writeTextShort(std::ostream& out) const {
    if (nCells_ == 0)
        out << "Empty cell structure";
    else {
        if (nCells_ == 1)
            out << "1 cell:";
        else
            out << nCells_ << " cells:";

        for (size_t i = 0; i < nCells_; ++i) {
            out << " (";
            for (auto a = begin(i); a != end(i); ++a) {
                if (a != begin(i)) {
                    out << ' ' << a->node()->index() << ':'
                        << (a->arc() + 3) % 4 << '-' << a->arc();
                } else {
                    out << a->node()->index() << ':' << a->arc();
                }
            }
            auto a = begin(i);
            out << ' ' << a->node()->index() << ':'
                << (a->arc() + 3) % 4 << ')';
        }
    }
}

void ModelLinkGraphCells::writeTextLong(std::ostream& out) const {
    if (nCells_ == 0) {
        out << "Empty cell structure" << std::endl;
        return;
    }

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
    for (i = 0; i < countNodes(); ++i) {
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

