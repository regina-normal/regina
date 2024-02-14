
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

#ifdef GUDHI_FOUND
#include <gudhi/distance_functions.h>
#include <gudhi/Persistent_cohomology.h>
#include <gudhi/Rips_complex.h>
#include <gudhi/Simplex_tree.h>
#endif

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
    if (isEmpty())
        defaultRadius_ = 1.0; /* The actual value is irrelevant. */

    auto r = range();

    auto min = std::min(std::min(r.second.x - r.first.x,
        r.second.y - r.first.y), r.second.z - r.first.z);

#ifndef GUDHI_FOUND
    defaultRadius_ = min / 20;
#else
    // TODO: This is very much work in progress.
    std::cerr << "RANGE: " << min << std::endl;

    using Point = std::array<double, 3>;
    using Simplex_tree = Gudhi::Simplex_tree<Gudhi::Simplex_tree_options_fast_persistence>;
    using Filtration_value = Simplex_tree::Filtration_value;
    using Rips_complex = Gudhi::rips_complex::Rips_complex<Filtration_value>;

    std::vector<Point> points;
    for (const auto& c : components_) {
        for (const auto& n : c) {
            points.push_back(n);
        }
    }

    double threshold = min / 2; // TODO: shrink this if I can.
    Rips_complex rips_complex_from_points(points, threshold,
        Gudhi::Euclidean_distance());

    Simplex_tree stree;
    rips_complex_from_points.create_complex(stree, 2);

    std::cerr << "Rips complex is of dimension " << stree.dimension() <<
               " - " << stree.num_simplices() << " simplices - " <<
               stree.num_vertices() << " vertices." << std::endl;

    std::cerr << "Iterator on Rips complex simplices in the filtration order, with [filtration value]:" <<
               std::endl;
    /*
    for (auto f_simplex : stree.filtration_simplex_range()) {
        std::cerr << "   ( ";
        for (auto vertex : stree.simplex_vertex_range(f_simplex)) {
            std::cerr << vertex << " ";
        }
        std::cerr << ") -> " << "[" << stree.filtration(f_simplex) << "] ";
        std::cerr << std::endl;
    }
    */

    Gudhi::persistent_cohomology::Persistent_cohomology< Simplex_tree, Gudhi::persistent_cohomology::Field_Zp > pcoh(stree);
    pcoh.init_coefficients(2);
    pcoh.compute_persistent_cohomology(0);
    for (const auto& pair : pcoh.get_persistent_pairs()) {
        if (stree.dimension(get<0>(pair)) == 1) {
        std::cerr << stree.filtration(get<0>(pair)) << " "
            << stree.filtration(get<1>(pair)) << " " << std::endl;
        }
    }

    defaultRadius_ = min / 20;
#endif
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

            for (int i = 1; i < sub; ++i) {
                double u = double(i) / double(sub);
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

