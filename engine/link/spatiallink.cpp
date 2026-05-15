
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "link/spatiallink.h"
#include "utilities/exception.h"
#include <fstream>

namespace regina {

SpatialLink::SpatialLink(std::initializer_list<std::initializer_list<Node>>
        components) {
    for (auto c : components) {
        auto& next = components_.emplace_back();
        next.reserve(c.size());
        for (auto n : c) {
            next.push_back(n);
        }
    }
}

SpatialLink& SpatialLink::operator = (const SpatialLink& src) {
    if (std::addressof(src) == this)
        return *this;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties (if/when these are
    // ever implemented).  Our intention here is to clone them, not clear them.
    PacketChangeSpan span(*this);

    components_ = src.components_;
    radius_ = src.radius_;

    // Clone properties:
    defaultRadius_ = src.defaultRadius_;

    return *this;
}

SpatialLink& SpatialLink::operator = (SpatialLink&& src) {
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties (if/when these are
    // ever implemented).  Our intention here is to move them, not clear them.
    PacketChangeSpan span(*this);

    components_ = std::move(src.components_);
    radius_ = src.radius_;

    // Move properties:
    defaultRadius_ = src.defaultRadius_;

    return *this;
}

std::pair<SpatialLink::Node, SpatialLink::Node> SpatialLink::range() const {
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

void SpatialLink::computeDefaultRadius() {
    if (isEmpty()) {
        defaultRadius_ = 1.0; /* The actual value is irrelevant. */
        return;
    }

    // We have at least one component, which must be non-empty.
    //
    // The current algorithm uses the _relative neighbourhood graph_ (RNG)
    // (thanks to Kate Turner for talking me through this).
    // The RNG connects two nodes X,Y iff their lune does not contain any
    // _other_ nodes (the lune is the intersection of the ball centred at X
    // passing through Y with the ball centred at Y passing through X).
    //
    // We expect adjacent nodes in the link to be connected in the RNG.
    // What we do here is find the closest pair of _non-adjacent_ nodes X,Y
    // that are connected in the RNG, and choose the rendering radius
    // dist(X,Y) / 4 (which means the rendered link should fill roughly
    // half of the space between X and Y).
    //
    // TODO: The algorithm can almost certainly be improved.  Currently the
    // running time is best-case quadratic (since we examine all pairs X,Y),
    // worst-case cubic (due to the test for an empty lune), and extremely
    // parallelisable (though we are not parallelising it here).

    defaultRadius_ = -1;

    for (auto a = beginNodes(); a != endNodes(); ++a) {
        for (auto b = std::next(a); b != endNodes(); ++b) {
            // In most cases, the first value of b in this loop will be
            // adjacent to a; however, this will not be true if a is the last
            // node in its component.  At any rate, this is a minor
            // inefficiency: we will only consider O(n) adjacent pairs,
            // which is negligible for a best-case O(n^2) algorithm.
            if (a.adjacent(b))
                continue;

            auto dist = a->distance(*b);
            if (defaultRadius_ >= 0 && dist >= defaultRadius_)
                continue;

            // If the lune is empty, then this will become our new closest
            // non-adjacent pair of nodes connected in the RNG.
            bool emptyLune = true;
            for (auto c = beginNodes(); c != endNodes(); ++c) {
                if (c == a || c == b)
                    continue;
                if (a->distance(*c) < dist && b->distance(*c) < dist) {
                    emptyLune = false;
                    break;
                }
            }
            if (emptyLune)
                defaultRadius_ = dist;
        }
    }

    if (defaultRadius_ < 0) {
        // There were no non-adjacent pairs connected in the RNG at all.
        // Fall back to a simple heuristic based on the size of the
        // bounding box.
        std::cerr << "WARNING: no usable non-adjacent nodes, using "
            "bounding box to compute radius" << std::endl;
        auto r = range();
        auto min = std::min(std::min(r.second.x - r.first.x,
            r.second.y - r.first.y), r.second.z - r.first.z);
        defaultRadius_ = min / 20;
    } else {
        // We identified the closest pair of non-adjacent nodes connected in
        // the RNG; now we use dist / 4, as discussed above.
        defaultRadius_ /= 4;
    }
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

    if (hasRadius())
        out << "\nRendering radius: " << radius_;

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
    std::swap(radius_, other.radius_);

    // Swap properties:
    std::swap(defaultRadius_, other.defaultRadius_);
}

void SpatialLink::scale(double factor) {
    ChangeAndClearSpan<> span(*this);

    for (auto& c : components_)
        for (auto& n : c) {
            n.x *= factor;
            n.y *= factor;
            n.z *= factor;
        }

    if (hasRadius())
        radius_ *= factor;
}

void SpatialLink::translate(const Vector3D<double>& vector) {
    ChangeAndClearSpan<> span(*this);

    for (auto& c : components_)
        for (auto& n : c) {
            n.x += vector.x;
            n.y += vector.y;
            n.z += vector.z;
        }
}

void SpatialLink::reflect(int axis) {
    ChangeAndClearSpan<> span(*this);

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

void SpatialLink::refine() {
    ChangeAndClearSpan<> span(*this);

    // See the comments in the implementation of refine(int) for where these
    // coefficients 9/16 and -1/16 come from (they just fix u = 1/2).
    static constexpr double inner = 9.0 / 16.0;
    static constexpr double outer = -1.0 / 16.0;

    for (auto& c : components_) {
        Component refined;
        refined.reserve(c.size() * 2);

        for (size_t i = 0; i < c.size(); ++i) {
            const Node& n1 = c[i == 0 ? c.size() - 1 : i - 1];
            const Node& n2 = c[i];
            const Node& n3 = c[i < c.size() - 1 ? i + 1 : 0];
            const Node& n4 = c[i < c.size() - 2 ? i + 2 : i + 2 - c.size()];

            refined.push_back(n2);
            refined.push_back((n2 + n3) * inner + (n1 + n4) * outer);
        }

        c.swap(refined);
    }
}

void SpatialLink::refine(int sub) {
    ChangeAndClearSpan<> span(*this);

    for (auto& c : components_) {
        Component refined;
        refined.reserve(c.size() * sub);

        for (size_t i = 0; i < c.size(); ++i) {
            const Node& n1 = c[i == 0 ? c.size() - 1 : i - 1];
            const Node& n2 = c[i];
            const Node& n3 = c[i < c.size() - 1 ? i + 1 : 0];
            const Node& n4 = c[i < c.size() - 2 ? i + 2 : i + 2 - c.size()];

            // In general, the Catmull-Rom spline with tension τ=0.5 follows
            // the following path where 0 ≤ u ≤ 1:
            //
            // - n1 * u * (1-u)^2 / 2 +
            // n2 * (1-u) * (-2 u^2 - (1-u)^2 + 3) / 2 +
            // n3 * u * (-2 (1-u)^2 - u^2 + 3) / 2 +
            // - n4 * u^2 * (1-u) / 2
            //
            // There is a simple write-up of this by Christopher Twigg at:
            // http://www.cs.cmu.edu/~fp/courses/graphics/asst5/catmullRom.pdf
            //
            // For u = 1/2, these coefficients become -1/16, 9/16, 9/16, -1/16,
            // hence the values of the inner and outer constants above.

            refined.push_back(n2);

            for (int j = 1; j < sub; ++j) {
                double u = double(j) / double(sub);
                double coeff[4] = {
                    -u * (1-u) * (1-u) / 2.0,
                    (1-u) * (3 - 2 * u * u - (1-u) * (1-u)) / 2.0,
                    u * (3 - 2 * (1-u) * (1-u) - u * u) / 2.0,
                    -u * u * (1-u) / 2.0
                };
                refined.push_back(n1 * coeff[0] + n2 * coeff[1] +
                    n3 * coeff[2] + n4 * coeff[3]);
            }
        }

        c.swap(refined);
    }
}

} // namespace regina

