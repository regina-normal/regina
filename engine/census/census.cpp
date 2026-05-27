
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
#include "file/globaldirs.h"
#include "triangulation/dim3.h"

namespace regina {

CensusDB Census::closedOr_ = CensusDB::global("closed-or-census-11",
    "Closed census (orientable)", 11);
CensusDB Census::closedNor_ = CensusDB::global("closed-nor-census-11",
    "Closed census (non-orientable)", 11);
CensusDB Census::closedHyp_ = CensusDB::global("closed-hyp-census-full",
    "Hodgson-Weeks closed hyperbolic census", 32);
CensusDB Census::cuspedHypOr_ = CensusDB::global("cusped-hyp-or-census-9",
    "Cusped hyperbolic census (orientable)", 9);
CensusDB Census::cuspedHypNor_ = CensusDB::global("cusped-hyp-nor-census-9",
    "Cusped hyperbolic census (non-orientable)", 9);
CensusDB Census::christy_ = CensusDB::global("christy-knots-links",
    "Christy's collection of knot/link complements", 22);
CensusDB Census::classicalKnots_ = CensusDB::global("classical",
    "Prime classical knots", 16);
CensusDB Census::virtualKnots_ = CensusDB::global("virtual",
    "Prime virtual knots", 6);

// Instantiate this template, which we use in the python bindings.
template void CensusDB::lookupKey<2, const std::function<void(CensusHit&&)>&>(
    const std::string&, const std::function<void(CensusHit&&)>&) const;

std::list<CensusHit> Census::lookup(const Triangulation<3>& tri) {
    return lookup(tri.neoSig());
}

std::list<CensusHit> Census::lookup(const std::string& sig) {
    // The census databases currently use second-generation signatures as keys.
    switch (IsoSigPrintable::generation(sig)) {
        case 2:
            // Go ahead and perform the database lookups.
            // Note: "a" is both a first-generation and second-generation
            // signature.  However, generation("a") guarantees to return 2
            // and so it will land in this case as we would like it to.
            break;
        case 1:
            // We need to do the lookup with a second-generation signature.
            try {
                return lookup(Triangulation<3>::fromSig(sig).neoSig());
            } catch (const InvalidArgument&) {
                // The given string is not an isomorphism signature.
                // There will be no hits.
                return {};
            }
        default:
            // The given string is not an isomorphism signature.
            // There will be no hits.
            return {};
    }

    std::list<CensusHit> hits;

    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };

    // Any of these calls to lookupKey() might throw a FileError exception.
    closedOr_.lookupKey<2>(sig, push);
    closedNor_.lookupKey<2>(sig, push);
    closedHyp_.lookupKey<2>(sig, push);
    cuspedHypOr_.lookupKey<2>(sig, push);
    cuspedHypNor_.lookupKey<2>(sig, push);
    christy_.lookupKey<2>(sig, push);

    return hits;
}

} // namespace regina

