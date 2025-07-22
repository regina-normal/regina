
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "link/link.h"
#include "triangulation/cut.h"

namespace regina {

bool Cut::isTrivial() const {
    if (size_ == 0)
        return true;
    for (size_t i = 1; i < size_; ++i)
        if (side_[i] != side_[0])
            return false;
    return true;
}

size_t Cut::weight(const Link& link) const {
    if (link.size() != size_)
        throw InvalidArgument("Cut::weight() requires a link diagram "
            "with the same size as the cut.");

    size_t ans = 0;
    for (size_t i = 0; i < size_; ++i)
        if (side(i) == 0) {
            auto c = link.crossing(i);
            for (int j = 0; j < 2; ++j) {
                if (side_[c->next(j).crossing()->index()] == 1)
                    ++ans;
                if (side_[c->prev(j).crossing()->index()] == 1)
                    ++ans;
            }
        }

    return ans;
}

size_t Cut::weight(const ModelLinkGraph& graph) const {
    if (graph.size() != size_)
        throw InvalidArgument("Cut::weight() requires a model link graph "
            "with the same size as the cut.");

    size_t ans = 0;
    for (size_t i = 0; i < size_; ++i)
        if (side(i) == 0) {
            auto n = graph.node(i);
            for (int j = 0; j < 4; ++j) {
                if (side_[n->adj(j).node()->index()] == 1)
                    ++ans;
            }
        }

    return ans;
}

bool Cut::inc() {
    if (size_ == 0)
        return false;

    for (ssize_t pos = size_ - 1; pos >= 0; --pos) {
        if (side_[pos] == 0) {
            side_[pos] = 1;
            return true;
        } else
            side_[pos] = 0;
    }

    return false;
}

void Cut::writeTextShort(std::ostream& out) const {
    out << '(';
    for (size_t i = 0; i < size_; ++i)
        if (side_[i] == 0)
            out << ' ' << i;
    out << " |";
    for (size_t i = 0; i < size_; ++i)
        if (side_[i] == 1)
            out << ' ' << i;
    out << " )";
}

} // namespace regina

