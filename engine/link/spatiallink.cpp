
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "link/spatiallink.h"
#include "utilities/exception.h"
#include <fstream>

namespace regina {

SpatialLink& SpatialLink::operator = (const SpatialLink& src) {
    if (std::addressof(src) == this)
        return *this;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties (if/when these are
    // ever implemented).  Our intention here is to clone them, not clear them.
    PacketChangeSpan span(*this);

    components_ = src.components_;
    return *this;
}

SpatialLink& SpatialLink::operator = (SpatialLink&& src) {
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties (if/when these are
    // ever implemented).  Our intention here is to move them, not clear them.
    PacketChangeSpan span(*this);

    components_ = std::move(src.components_);
    return *this;
}

std::pair<SpatialLink::Node, SpatialLink::Node> SpatialLink::range()
        const {
    std::pair<SpatialLink::Node, SpatialLink::Node> ans;

    bool found = false;
    for (const auto& c : components_)
        for (const auto& n : c) {
            if (! found) {
                ans.first = ans.second = n;
                found = true;
            } else {
                if (n.x < ans.first.x)
                    ans.first.x = n.x;
                if (n.y < ans.first.y)
                    ans.first.y = n.y;
                if (n.z < ans.first.z)
                    ans.first.z = n.z;
                if (n.x > ans.second.x)
                    ans.second.x = n.x;
                if (n.y > ans.second.y)
                    ans.second.y = n.y;
                if (n.z > ans.second.z)
                    ans.second.z = n.z;
            }
        }

    if (found)
        return ans;
    else
        return {{ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }};
}

void SpatialLink::writeTextShort(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty spatial link";
        return;
    }

    if (components_.size() == 1)
        out << components_.front().size() << "-crossing spatial knot";
    else
        out << size() << "-node, "
            << components_.size() << "-component spatial link";
}

void SpatialLink::writeTextLong(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty spatial link" << std::endl;
        return;
    }

    if (components_.size() == 1)
        out << components_.front().size() << "-crossing spatial knot";
    else
        out << components_.size() << "-component spatial link";
    out << "\n\n";

    int comp = 0;
    for (const auto& c : components_) {
        out << "Component " << comp++ << " (";
        if (c.size() == 1)
            out << "1 node):\n";
        else
            out << c.size() << " nodes):\n";
        for (const auto& n : c)
            out << "    (" << n.x << ", " << n.y << ", " << n.z << ")\n";
    }
}

void SpatialLink::swap(SpatialLink& other) {
    if (&other == this)
        return;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties (if/when these are
    // ever implemented).  Our intention here is to swap them, not clear them.
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    components_.swap(other.components_);
}

void SpatialLink::scale(double factor) {
    for (auto& c : components_)
        for (auto& n : c) {
            n.x *= factor;
            n.y *= factor;
            n.z *= factor;
        }
}

void SpatialLink::translate(const Node& vector) {
    for (auto& c : components_)
        for (auto& n : c) {
            n.x += vector.x;
            n.y += vector.y;
            n.z += vector.z;
        }
}

void SpatialLink::reflect(int axis) {
    switch (axis) {
        case 0:
            for (auto& c : components_)
                for (auto& n : c)
                    n.x = -n.x;
            break;
        case 1:
            for (auto& c : components_)
                for (auto& n : c)
                    n.y = -n.y;
            break;
        case 2:
            for (auto& c : components_)
                for (auto& n : c)
                    n.z = -n.z;
            break;
        default:
            throw InvalidInput("reflect(): the given axis must be 0, 1 or 2");
    }
}

} // namespace regina

