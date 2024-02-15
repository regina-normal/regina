
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

namespace {
    // Simplex tree options based on GUDHI's fast_persistence options,
    // but using double instead of float.
    struct ReginaSimplexTreeOptions {
        using Indexing_tag = Gudhi::linear_indexing_tag;
        using Vertex_handle = int;
        using Filtration_value = double /* not float */;
        using Simplex_key = uint32_t;
        static constexpr bool store_key = true;
        static constexpr bool store_filtration = true;
        static constexpr bool contiguous_vertices = true;
        static constexpr bool link_nodes_by_label = false;
        static constexpr bool stable_simplex_handles = false;
    };
}

void SpatialLink::computeDefaultRadius(int subdivisions) {
    if (isEmpty()) {
        defaultRadius_ = 1.0; /* The actual value is irrelevant. */
        return;
    }

    auto r = range();
    auto min = std::min(std::min(r.second.x - r.first.x,
        r.second.y - r.first.y), r.second.z - r.first.z);
    std::cerr << "x range: [" << r.first.x << ", " << r.second.x << ']' << std::endl;
    std::cerr << "y range: [" << r.first.y << ", " << r.second.y << ']' << std::endl;
    std::cerr << "z range: [" << r.first.z << ", " << r.second.z << ']' << std::endl;
    std::cerr << "Min range: " << min << std::endl;

#ifdef GUDHI_FOUND
    // TODO: Probably we should choose a better default subdivision.
    std::vector<std::array<double, 3>> points;
    points.reserve(size() * subdivisions);

    const double slice = 1.0 / subdivisions;
    for (const auto& c : components_) {
        for (size_t i = 0; i < c.size(); ++i) {
            const Node& a = c[i];
            const Node& b = c[i < c.size() - 1 ? i + 1 : 0];

            points.emplace_back(a);
            for (int j = 1; j < subdivisions; ++j)
                points.emplace_back(
                    b * (j * slice) + a * ((subdivisions - j) * slice));
        }
    }

    // TODO: Choose a good threshold.
    double threshold = min;
    Gudhi::rips_complex::Rips_complex<double> rips(points, threshold,
        Gudhi::Euclidean_distance());

    using Simplex_tree = Gudhi::Simplex_tree<ReginaSimplexTreeOptions>;
    Simplex_tree stree;
    rips.create_complex(stree, 2);
    std::cerr << "Rips complex: dim = " << stree.dimension()
        << ", simplices = " << stree.num_simplices()
        << ", vertices = " << stree.num_vertices() << std::endl;

    Gudhi::persistent_cohomology::Persistent_cohomology<Simplex_tree,
        Gudhi::persistent_cohomology::Field_Zp> pcoh(stree);
    pcoh.init_coefficients(2);
    pcoh.compute_persistent_cohomology(0);

    // Persistence pairs are sorted by death time.
    // Both dimension and birth time are ordered arbitrarily.

    // TODO: Look at 0-dimensional persistence to verify that the subdivision
    // was enough.

    // Extract the 1-D pairs.
    std::vector<std::pair<double, double>> dim1;
    for (const auto& pair : pcoh.get_persistent_pairs()) {
        auto dim = stree.dimension(get<0>(pair));
        if (dim == 1) {
            auto birth = stree.filtration(get<0>(pair));
            auto death = stree.filtration(get<1>(pair));
            std::cerr << "1-D: " << birth << " --> " << death << std::endl;
            dim1.emplace_back(birth, death);
        }
    }

    // Sort by _birth_ time, and then by death time.
    // If this link has c components, then the first c cycles should describe
    // these components, and breakage occurs at min(next birth, first death).
    if (dim1.size() >= components_.size()) {
        std::sort(dim1.begin(), dim1.end(), [](const auto& x, const auto& y) {
            if (x.first < y.first) return true;
            if (x.first > y.first) return false;
            return (x.second < y.second);
        });

        defaultRadius_ = dim1.front().second; // first death
        std::cerr << "First death: " << defaultRadius_ << std::endl;
        if (dim1.size() > components_.size()) {
            double nextBirth = dim1[components_.size()].first;
            std::cerr << "Next birth: " << nextBirth << std::endl;
            if (nextBirth < defaultRadius_)
                defaultRadius_ = nextBirth;
        } else if (std::isinf(defaultRadius_)) {
            // We never managed to kill off all our cycles.
            std::cerr << "Cycles not all killed" << std::endl;
            defaultRadius_ = threshold;
        }
        std::cerr << "Final radius: " << defaultRadius_ << std::endl;

        // TODO: Explain away this.
        defaultRadius_ = defaultRadius_ / 2;

        // NOW: We have the radius at which things first go wrong.
        // This is not actually the radius that we want to use for rendering.
        // TODO: Make a good default choice.
        return;
    }

    // There were not enough 1-cycles found: something has gone wrong.
    // Presumably the subdivision was not enough.
    std::cerr << "Not all link components were found" << std::endl;
    // TODO: Rerun with a higher subdivision.
#endif

    // Fall through to an unintelligent default.
    defaultRadius_ = min / 20;
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

