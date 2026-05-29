
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

#include <algorithm>
#include <cstdlib>
#include <functional>
#include "census/census-impl.h"
#include "link/link.h"
#include "triangulation/dim3.h"

namespace regina {

std::list<CensusHit> Census::lookup(const std::string& sig) {
    auto tri3 = lookupAs<Triangulation<3>>(sig);
    auto links = lookupAs<Link>(sig);

    if (tri3.empty())
        return links;
    else if (links.empty())
        return tri3;
    else {
        // I'm not sure this can even occur with the census databases we
        // currently have..?
        tri3.splice(tri3.end(), std::move(links));
        return tri3;
    }
}

template <>
void CensusCollection<Triangulation<3>>::init() {
    // IMPORTANT: Do not forget to update the maximum sizes (both in each
    // CensusDB creation and also at the end of this list) if/when the
    // databases change!
    databases_.reserve(6);
    databases_.push_back(CensusDB::global("closed-or-census-11",
        "Closed census (orientable)", 11));
    databases_.push_back(CensusDB::global("closed-nor-census-11",
        "Closed census (non-orientable)", 11));
    databases_.push_back(CensusDB::global("closed-hyp-census-full",
        "Hodgson-Weeks closed hyperbolic census", 32));
    databases_.push_back(CensusDB::global("cusped-hyp-or-census-9",
        "Cusped hyperbolic census (orientable)", 9));
    databases_.push_back(CensusDB::global("cusped-hyp-nor-census-9",
        "Cusped hyperbolic census (non-orientable)", 9));
    databases_.push_back(CensusDB::global("christy-knots-links",
        "Christy's collection of knot/link complements", 22));
    maxSize_ = 32;
}

template <>
void CensusCollection<Link>::init() {
    // IMPORTANT: Do not forget to update the maximum sizes (both in each
    // CensusDB creation and also at the end of this list) if/when the
    // databases change!
    databases_.reserve(3);
    databases_.push_back(CensusDB::global("classical",
        "Prime classical knots", "classical", 16));
    databases_.push_back(CensusDB::global("virtual",
        "Virtual knots", "virtual", 6));
    databases_.push_back(CensusDB::global("green",
        "Green's virtual knots", "Green", 6));
    maxSize_ = 16;
}

// Instantiate this template, which we use in the python bindings.
template void CensusDB::lookupKey<2, const std::function<void(CensusHit&&)>&>(
    const std::string&, const std::function<void(CensusHit&&)>&) const;

// Other instantiations that we need:
template std::list<CensusHit> CensusCollection<Triangulation<3>>::lookup(
    const Triangulation<3>&);
template std::list<CensusHit> CensusCollection<Triangulation<3>>::lookup(
    const std::string&);

template std::list<CensusHit> CensusCollection<Link>::lookup(const Link&);
template std::list<CensusHit> CensusCollection<Link>::lookup(
    const std::string&);

} // namespace regina

