
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

#include "link/link.h"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <iterator>

namespace {
    // Computes abs(a-b) without using negative numbers.
    size_t absdiff(size_t a, size_t b) {
        return (a >= b ? a - b : b - a);
    }
}

namespace regina {

std::string Link::dt(bool alpha) const {
    std::ostringstream out;
    dt(out, alpha);
    return out.str();
}

void Link::dt(std::ostream& out, bool alpha) const {
    if (components_.size() != 1)
        throw NotImplemented("Dowker-Thistlethwaite notation is "
            "only implemented for single-component links");
    if (crossings_.empty())
        return;
    if (alpha && size() > 26)
        throw NotImplemented("Alphabetical Dowker-Thistlethwaite notation is "
            "only implemented for links with at most 26 crossings");
    if (! isClassical())
        throw NotImplemented("Dowker-Thistlethwaite notation is only "
            "implemented for classical link diagrams, not virtual diagrams");

    // Dowker-Thistlethwaite notation requires us to start on the lower strand.
    StrandRef start = components_.front();
    if (start.strand() == 1)
        start.jump();

    size_t n = size();

    // Work with the largest signed integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;

    // Odd steps in traversal -> crossing index
    FixedArray<size_t> oddCrossing(n);

    // Crossing index -> even steps in traversal, negated if passing under
    FixedArray<Int> evenStep(n);

    StrandRef s = start;
    Int step = 0;
    do {
        ++step;
        if (step % 2 == 1) {
            oddCrossing[step >> 1] = s.crossing()->index();
        } else {
            // TODO: It would be kind to do some sanity checking here,
            // to make sure that every crossing is seen here exactly once.
            evenStep[s.crossing()->index()] = (s.strand() == 1 ? step : -step);
        }
        ++s;
    } while (s != start);
    assert(static_cast<size_t>(step) == 2 * n);

    if (alpha) {
        for (size_t i = 0; i < n; ++i)
            out << (evenStep[oddCrossing[i]] > 0 ?
                (char('a' + evenStep[oddCrossing[i]] / 2 - 1)) :
                (char('A' - evenStep[oddCrossing[i]] / 2 - 1)));
    } else {
        for (size_t i = 0; i < n; ++i) {
            if (i > 0)
                out << ' ';
            out << evenStep[oddCrossing[i]];
        }
    }
}

std::vector<int> Link::dtData() const {
    if (components_.size() != 1)
        throw NotImplemented("Dowker-Thistlethwaite notation is "
            "only implemented for single-component links");
    if (crossings_.empty())
        return {};
    if (2 * size() > INT_MAX)
        throw NotImplemented("This Dowker-Thistlethwaite notation has "
            "entries that cannot fit into a C++ int");

    // Dowker-Thistlethwaite notation requires us to start on the lower strand.
    StrandRef start = components_.front();
    if (start.strand() == 1)
        start.jump();

    int n = static_cast<int>(size());

    // Odd steps in traversal -> crossing index
    FixedArray<int> oddCrossing(n);

    // Crossing index -> even steps in traversal, negated if passing under
    FixedArray<int> evenStep(n);

    StrandRef s = start;
    int step = 0;
    do {
        ++step;
        if (step % 2 == 1) {
            oddCrossing[step >> 1] = static_cast<int>(s.crossing()->index());
        } else {
            evenStep[s.crossing()->index()] = (s.strand() == 1 ? step : -step);
        }
        ++s;
    } while (s != start);
    assert(step == 2 * n);

    std::vector<int> ans;
    ans.reserve(n);

    for (int i = 0; i < n; ++i)
        ans.push_back(evenStep[oddCrossing[i]]);

    return ans;
}

Link Link::fromDT(const std::string& s) {
    // Do we have an alphabetical or numerical string?
    auto it = s.begin();
    while (it != s.end() && ::isspace(*it))
        ++it;
    if (it == s.end()) {
        // Empty string, so return the unknot.
        return { 1 };
    }

    // Work with the largest integer type that we could possibly need.
    using Int = std::make_signed_t<size_t>;
    std::vector<Int> terms;

    if ((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z')) {
        // We have the alphabetical variant.
        for ( ; it != s.end(); ++it) {
            if (*it >= 'a' && *it <= 'z')
                terms.push_back(2 * (*it - 'a' + 1));
            else if (*it >= 'A' && *it <= 'Z')
                terms.push_back(-2 * (*it - 'A' + 1));
            else if (! ::isspace(*it)) {
                // Invalid character.
                throw InvalidArgument(
                    "fromDT(): invalid alphabetical character");
            }
        }
    } else if ((*it >= '0' && *it <= '9') || *it == '-') {
        // We have the numerical variant.
        std::istringstream in(s);

        Int i;
        while (true) {
            in >> i;
            if (! in) {
                if (in.eof())
                    break;
                throw InvalidArgument("fromDT(): invalid numerical character");
            }
            terms.push_back(i);
        }
    } else
        throw InvalidArgument("fromDT(): invalid character");

    return fromDT(terms.begin(), terms.end());
}

} // namespace regina

