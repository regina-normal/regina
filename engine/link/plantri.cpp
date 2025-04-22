
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

#include "modellinkgraph.h"

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

} // namespace regina

