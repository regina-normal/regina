
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

#include "modellinkgraph.h"
#include <algorithm>

namespace regina {

std::pair<ModelLinkGraphArc, ModelLinkGraphArc> ModelLinkGraph::findFlype(
        const ModelLinkGraphArc& from) const {
    // Ensure that the cellular decomposition has been computed.
    cells();

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

    if (cells_->cell(upper) == cells_->cell(back)) {
        // Following upper must return back to from.
        // This means that the crossing (X) is redundant, and can be
        // undone by twisting everything from upper around to from.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    }

    // For each cell adjacent to C, we identify the first arc of C in a
    // clockwise direction from the vertex (X) that borders it.  A null arc
    // means the cell is not adjacent to C at all.
    ModelLinkGraphArc* adjC = new ModelLinkGraphArc[cells_->nCells_];
    ModelLinkGraphArc a = back;
    do {
        a = a.traverse();
        adjC[cells_->cell(a)] = a;
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
        common = cells_->cell(b);
        if (adjC[common])
            break;
        a = b;
        ++a;
    }

    ModelLinkGraphArc right = adjC[common];
    delete[] adjC;

    if (a == upper) {
        // The strand upper comes straight back to (X), with no
        // crossings in between.  In other words, cell A is a 1-gon.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    } else if (right == from) {
        // The common cell is in fact the cell immediately between the
        // arcs upper and from (i.e., immediately to the right of (X)).
        // The flype() routine will refuse to work with this, so return
        // null now.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    } else if (a.traverse().node() == from.node() ||
            right.traverse().node() == from.node()) {
        // One of the two return arcs ends at (X).  Again, flype() will
        // refuse to work with this, so return null now.
        return std::make_pair(ModelLinkGraphArc(), ModelLinkGraphArc());
    } else
        return std::make_pair(a, right);
}

ModelLinkGraph* ModelLinkGraph::flype(const ModelLinkGraphArc& from,
        const ModelLinkGraphArc& left, const ModelLinkGraphArc& right) const {
    // Some basic sanity checking.
    if (left.traverse().node() == from.node() ||
            right.traverse().node() == from.node())
        return nullptr;

    // Ensure that the cellular decomposition has been computed.
    cells();

    // We do a depth-first search through cells to work out which nodes
    // to flip.  There are three "barrier" cells that surround the
    // region to flip, which we find now:
    size_t upper = cells_->cell(left);
    size_t centre = cells_->cell(right);
    size_t lower = cells_->cell(right.traverse());

    // The cell from which we start the depth-first search:
    size_t inner = cells_->cell(from);

    // Some more sanity checking, now that we have cell data.
    if (upper == lower || centre == inner)
        return nullptr;

    bool* flip = new bool[size()];
    std::fill(flip, flip + size(), false);

    bool* visited = new bool[cells_->nCells_];
    std::fill(visited, visited + cells_->nCells_, false);
    visited[upper] = visited[lower] = visited[centre] = true;

    size_t* process = new size_t[cells_->nCells_]; // DFS stack
    visited[inner] = true;
    process[0] = inner;
    size_t nProcess = 1;

    size_t curr, adj;
    while (nProcess) {
        curr = process[--nProcess];
        for (auto p = cells_->begin(curr); p != cells_->end(curr); ++p) {
            flip[p->node()->index()] = true;

            adj = cells_->cell_[(p->node()->index() << 2) |
                ((p->arc() + 1) & 3)];
            if (! visited[adj]) {
                process[nProcess++] = adj;
                visited[adj] = true;
            }
        }
    }

    delete[] process;
    delete[] visited;

    // We now know exactly which nodes to flip.
    // Remove the start node from this list, which is a special case -
    // we will untwist this later.
    flip[from.node()->index()] = false;

    // Off we go!  Prepare a new graph and perform the flype.
    ModelLinkGraph* ans = new ModelLinkGraph(*this);

    ModelLinkGraphNode* n;
    for (size_t i = 0; i < size(); ++i)
        if (flip[i]) {
            // Swap arcs 1 and 3 of the ith node.
            // This code does not work if arcs 1 and 3 are joined to
            // each other, but such an arrangement is impossible for a
            // planar graph.
            n = ans->nodes_[i];
            std::swap(n->adj_[1], n->adj_[3]);
            n->adj_[1].node()->adj_[n->adj_[1].arc()].arc_ = 1;
            n->adj_[3].node()->adj_[n->adj_[3].arc()].arc_ = 3;
        }

    // Create the arcs in the new graph that correspond to the old graph's
    // left and right arcs.  Since the source nodes for both left and right
    // were caught up in the flips, we must account for this also.
    ModelLinkGraphArc newLeft(ans->nodes_[left.node()->index()],
        left.arc_ % 2 ? left.arc_ ^ 2 : left.arc_);
    ModelLinkGraphArc newRight(ans->nodes_[right.node()->index()],
        right.arc_ % 2 ? right.arc_ ^ 2 : right.arc_);

    // Undo the crossing at from, and make a new crossing from left and right.
    // The node n starts as the old crossing that we undo, and we will
    // then reuse it for the new crossing that we create.
    //
    // Note that, when undoing the crossing at from, we know that from
    // is not connected immediately to itself due to the sanity checks
    // that we have already run.
    n = ans->nodes_[from.node()->index()];
    ModelLinkGraphArc a, b;

    a = n->adj(from.arc());
    b = n->adj(from.arc() ^ 2);
    a.node()->adj_[a.arc()] = b;
    b.node()->adj_[b.arc()] = a;

    a = n->adj((from.arc() + 1) % 4);
    b = n->adj((from.arc() + 3) % 4);
    a.node()->adj_[a.arc()] = b;
    b.node()->adj_[b.arc()] = a;

    // Note that the nodes at newLeft.traverse() and newRight.traverse()
    // are not flipped, and are not the node that we just untwisted.
    n->adj_[0] = newLeft.traverse();
    n->adj_[1] = newRight.traverse();
    n->adj_[2] = newLeft;
    n->adj_[3] = newRight;
    for (int i = 0; i < 4; ++i)
        n->adj_[i].node()->adj_[n->adj_[i].arc()] = ModelLinkGraphArc(n, i);

    delete[] flip;
    return ans;
}

} // namespace regina

