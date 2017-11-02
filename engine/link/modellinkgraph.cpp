
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
#include <sstream>

namespace regina {

ModelLinkGraph::ModelLinkGraph(const ModelLinkGraph& cloneMe) {
    for (ModelLinkGraphNode* n : cloneMe.nodes_)
        nodes_.push_back(new ModelLinkGraphNode());

    auto it = cloneMe.nodes_.begin();
    for (ModelLinkGraphNode* n : nodes_) {
        for (int i = 0; i < 4; ++i) {
            n->adj_[i].node_ = nodes_[(*it)->adj_[i].node_->index()];
            n->adj_[i].arc_ = (*it)->adj_[i].arc;
        }
        ++it;
    }
}

void ModelLinkGraph::writeTextShort(std::ostream& out) const {
    if (components_.empty())
        out << "empty link";
    else if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
}

void ModelLinkGraph::writeTextLong(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty link" << std::endl;
        return;
    }

    if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
    out << "\n\n";

    int comp = 0;
    ModelLinkGraphArc s;
    for (ModelLinkGraphArc start : components_) {
        out << "Component " << comp++ << ": ";
        if (! start) {
            out << "no crossings (separate unknot)" << std::endl;
            continue;
        }
        s = start;
        out << s;
        for (++s; s != start; ++s)
            out << ' ' << s;
        out << '\n';
    }

    out << "\nModelLinkGraphNodes:";
    for (ModelLinkGraphNode* c : crossings_)
        out << ' ' << (c->sign() > 0 ? '+' : '-') << c->index();
    out << std::endl;
}

void ModelLinkGraph::writeTextShort(std::ostream& out) const {
    // TODO
}

void ModelLinkGraph::writeTextLong(std::ostream& out) const {
    // TODO
}

ModelLinkGraph* ModelLinkGraph::fromPlantri(const std::string& plantri) {
    // TODO
}

} // namespace regina

