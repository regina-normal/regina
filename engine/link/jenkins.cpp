
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

#include "link.h"
#include <climits>
#include <iterator>
#include <sstream>

namespace regina {

Link Link::fromJenkins(std::istream& in) {
    // Work with the largest integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;
    return fromJenkins(std::istream_iterator<Int>(in),
        std::istream_iterator<Int>());
}

Link Link::fromJenkins(const std::string& str) {
    // Work with the largest integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;
    std::istringstream in(str);
    return fromJenkins(std::istream_iterator<Int>(in),
        std::istream_iterator<Int>());
}

std::string Link::jenkins() const {
    std::ostringstream out;
    jenkins(out);
    return out.str();
}

void Link::jenkins(std::ostream& out) const {
    out << components_.size() << std::endl;

    StrandRef s;
    size_t len;
    for (StrandRef start : components_) {
        if (! start)
            out << '0';
        else {
            // Get the length of the component.
            s = start;
            len = 0;
            do {
                ++s; ++len;
            } while (s != start);

            // Output the component.
            // Note that s == start at this point.
            out << len;
            do {
                out << "   " << s.crossing()->index()
                    << ' ' << (s.strand() == 1 ? 1 : -1);
                ++s;
            } while (s != start);
        }
        out << std::endl;
    }

    if (! crossings_.empty()) {
        auto c = crossings_.begin();
        out << (*c)->index() << ' ' << (*c)->sign();
        for (++c; c != crossings_.end(); ++c)
            out << "   " << (*c)->index() << ' ' << (*c)->sign();
        out << std::endl;
    }
}

std::vector<int> Link::jenkinsData() const {
    // Note: we explicitly write #components, but we only write indices
    // of individual crossings, not #crossings.
    if (components_.size() > INT_MAX ||
            ((! crossings_.empty()) && crossings_.size() - 1 > INT_MAX))
        throw NotImplemented("This Jenkins format has entries that cannot "
            "fit into a C++ int");

    std::vector<int> ans;
    ans.push_back(static_cast<int>(components_.size()));

    StrandRef s;
    size_t len;
    for (StrandRef start : components_) {
        if (! start)
            ans.push_back(0);
        else {
            // Get the length of the component.
            s = start;
            len = 0;
            do {
                ++s; ++len;
            } while (s != start);

            // Output the component.
            // Note that s == start at this point.
            if (len > INT_MAX)
                throw NotImplemented("This Jenkins format has entries "
                    "that cannot fit into a C++ int");
            ans.push_back(static_cast<int>(len));
            do {
                ans.push_back(static_cast<int>(s.crossing()->index()));
                ans.push_back(s.strand() == 1 ? 1 : -1);
                ++s;
            } while (s != start);
        }
    }

    for (auto c : crossings_) {
        ans.push_back(static_cast<int>(c->index()));
        ans.push_back(c->sign());
    }

    return ans;
}

} // namespace regina

