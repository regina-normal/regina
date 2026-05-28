
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

std::vector<CensusDB> Census::knots_;
std::vector<CensusDB> Census::tri3_;

// Instantiate this template, which we use in the python bindings.
template void CensusDB::lookupKey<2, const std::function<void(CensusHit&&)>&>(
    const std::string&, const std::function<void(CensusHit&&)>&) const;

// The census databases currently use second-generation signatures as keys.
// Note: "a" is both a first-generation and second-generation signature
// (for both triangulations and link diagrams).  However, generation("a")
// guarantees to return 2 and so it will be treated as we hope in the code
// below.

template <>
std::list<CensusHit> Census::lookupAs<Triangulation<3>>(
        const std::string& sig) {
    switch (IsoSigPrintable::generation(sig)) {
        case 2:
            // Go ahead and perform the database lookups.
            break;
        case 1:
            // We need to do the lookup with a second-generation signature.
            try {
                return lookupAs<Triangulation<3>>(
                    Triangulation<3>::fromSig(sig).neoSig());
            } catch (const InvalidArgument&) {
                return {}; // not an isomorphism signature.
            }
        default:
            return {}; // not an isomorphism signature.
    }

    if (tri3_.empty()) {
        tri3_.reserve(6);
        tri3_.push_back(CensusDB::global("closed-or-census-11",
            "Closed census (orientable)", 11));
        tri3_.push_back(CensusDB::global("closed-nor-census-11",
            "Closed census (non-orientable)", 11));
        tri3_.push_back(CensusDB::global("closed-hyp-census-full",
            "Hodgson-Weeks closed hyperbolic census", 32));
        tri3_.push_back(CensusDB::global("cusped-hyp-or-census-9",
            "Cusped hyperbolic census (orientable)", 9));
        tri3_.push_back(CensusDB::global("cusped-hyp-nor-census-9",
            "Cusped hyperbolic census (non-orientable)", 9));
        tri3_.push_back(CensusDB::global("christy-knots-links",
            "Christy's collection of knot/link complements", 22));
    }

    size_t size;
    try {
        size = Triangulation<3>::isoSigComponentSize(sig);
    } catch (const InvalidArgument&) {
        return {}; // not an isomorphism signature.
    }

    std::list<CensusHit> hits;
    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };
    for (const auto& db : tri3_) {
        // Any of these calls to lookupKey() might throw a FileError exception.
        if (size <= db.maxSize())
            db.lookupKey<2>(sig, push);
    }
    return hits;
}

template <>
std::list<CensusHit> Census::lookupAs<Link>(const std::string& sig) {
    switch (LinkSigPrintable::generation(sig)) {
        case 2:
            // Go ahead and perform the database lookups.
            break;
        case 1:
            // We need to do the lookup with a second-generation signature.
            try {
                return lookupAs<Link>(Link::fromSig(sig).neoSig());
            } catch (const InvalidArgument&) {
                return {}; // not an isomorphism signature.
            }
        default:
            return {}; // not an isomorphism signature.
    }

    if (knots_.empty()) {
        knots_.reserve(3);
        knots_.push_back(CensusDB::global("classical",
            "Prime classical knots", "classical", 16));
        knots_.push_back(CensusDB::global("virtual",
            "Virtual knots", "virtual", 6));
        knots_.push_back(CensusDB::global("green",
            "Green's virtual knots", "Green", 6));
    }

    size_t size;
    try {
        size = Link::sigDiagramComponentSize(sig);
    } catch (const InvalidArgument&) {
        return {}; // not a knot/link signature.
    }

    std::list<CensusHit> hits;
    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };
    for (const auto& db : knots_) {
        // Any of these calls to lookupKey() might throw a FileError exception.
        if (size <= db.maxSize())
            db.lookupKey<2>(sig, push);
    }
    return hits;
}

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

} // namespace regina

