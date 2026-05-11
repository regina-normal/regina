
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

#include <algorithm>
#include <cstdlib>
#include <functional>
#include "census/census-impl.h"
#include "file/globaldirs.h"
#include "triangulation/dim3.h"

namespace regina {

CensusDB* Census::closedOr_ = nullptr;
CensusDB* Census::closedNor_ = nullptr;
CensusDB* Census::closedHyp_ = nullptr;
CensusDB* Census::cuspedHypOr_ = nullptr;
CensusDB* Census::cuspedHypNor_ = nullptr;
CensusDB* Census::christy_ = nullptr;
bool Census::dbInit_ = false;

// Instantiate this template, which we use in the python bindings.
template bool CensusDB::lookupKey<1, const std::function<void(CensusHit&&)>&>(
    const std::string&, const std::function<void(CensusHit&&)>&) const;

std::list<CensusHit> Census::lookup(const Triangulation<3>& tri) {
    return lookup(tri.isoSig());
}

std::list<CensusHit> Census::lookup(const std::string& isoSig) {
    // For now, the census databases all store first-generation signatures.
    switch (IsoSigPrintable::generation(isoSig)) {
        case 1:
            // Go ahead and perform the database lookups.
            break;
        case 2:
            // We need to do the lookup with a first-generation signature.
            // Note: "a" is both a first-generation and second-generation
            // signature, so in that case we should fall through and treat it
            // as first-generation (otherwise we will get an infinite loop).
            if (isoSig != "a")
                return lookup(Triangulation<3>::fromSig(isoSig).isoSig());
            break;
        default:
            // The given string is not an isomorphism signature.
            // There will be no hits.
            return {};
    }

    if (! dbInit_) {
        closedOr_ = standardDB("closed-or-census-11." REGINA_DB_EXT,
            "Closed census (orientable)");
        closedNor_ = standardDB("closed-nor-census-11." REGINA_DB_EXT,
            "Closed census (non-orientable)");
        closedHyp_ = standardDB("closed-hyp-census-full." REGINA_DB_EXT,
            "Hodgson-Weeks closed hyperbolic census");
        cuspedHypOr_ = standardDB("cusped-hyp-or-census-9." REGINA_DB_EXT,
            "Cusped hyperbolic census (orientable)");
        cuspedHypNor_ = standardDB("cusped-hyp-nor-census-9." REGINA_DB_EXT,
            "Cusped hyperbolic census (non-orientable)");
        christy_ = standardDB("christy-knots-links." REGINA_DB_EXT,
            "Christy's collection of knot/link complements");
        dbInit_ = true;
    }

    std::list<CensusHit> hits;

    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };

    closedOr_->lookupKey<1>(isoSig, push);
    closedNor_->lookupKey<1>(isoSig, push);
    closedHyp_->lookupKey<1>(isoSig, push);
    cuspedHypOr_->lookupKey<1>(isoSig, push);
    cuspedHypNor_->lookupKey<1>(isoSig, push);
    christy_->lookupKey<1>(isoSig, push);

    return hits;
}

CensusDB* Census::standardDB(const char* filename, const char* desc) {
    return new CensusDB(GlobalDirs::census() + "/" + filename, desc);
}

} // namespace regina

