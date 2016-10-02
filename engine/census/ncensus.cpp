
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "regina-config.h" // For QDBM-related macros

#include <algorithm>
#include <cstdlib>
#include <sstream>
#ifdef QDBM_AS_TOKYOCABINET
#include <depot.h>
#include <cabin.h>
#include <villa.h>
#else
#include <cstdbool>
#include <cstdint>
#include <tcbdb.h>
#include <tcutil.h>
#endif
#include "census/ncensus.h"
#include "file/globaldirs.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

NCensusDB* NCensus::closedOr_ = 0;
NCensusDB* NCensus::closedNor_ = 0;
NCensusDB* NCensus::closedHyp_ = 0;
NCensusDB* NCensus::cuspedHypOr_ = 0;
NCensusDB* NCensus::cuspedHypNor_ = 0;
NCensusDB* NCensus::hypKnotLink_ = 0;
bool NCensus::dbInit_ = false;

bool NCensusDB::lookup(const std::string& isoSig, NCensusHits* hits) const {
#ifdef QDBM_AS_TOKYOCABINET
    VILLA* db;
    if (! (db = vlopen(filename_.c_str(), VL_OREADER, VL_CMPLEX))) {
        std::cerr << "ERROR: Could not open QDBM database: "
            << filename_ << std::endl;
        return false;
    }

    CBLIST* records = vlgetlist(db, isoSig.c_str(), isoSig.length());
    if (records) {
        int n = cblistnum(records);
        for (int i = 0; i < n; ++i)
            hits->append(new NCensusHit(cblistval(records, i, 0), this));
        cblistclose(records);
    }

    vlclose(db);
#else
    TCBDB* db = tcbdbnew();
    if (! tcbdbopen(db, filename_.c_str(), BDBOREADER)) {
        std::cerr << "ERROR: Could not open Tokyo Cabinet database: "
            << filename_ << std::endl;
        return false;
    }

    TCLIST* records = tcbdbget4(db, isoSig.c_str(), isoSig.length());
    if (records) {
        int n = tclistnum(records);
        for (int i = 0; i < n; ++i)
            hits->append(new NCensusHit(tclistval2(records, i), this));
        tclistdel(records);
    }

    tcbdbclose(db);
    tcbdbdel(db);
#endif

    return true;
}

NCensusHits* NCensus::lookup(const NTriangulation& tri) {
    return lookup(tri.isoSig());
}

NCensusHits* NCensus::lookup(const std::string& isoSig) {
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
        hypKnotLink_ = standardDB("hyp-knot-link-census." REGINA_DB_EXT,
            "Hyperbolic knot and link complements");
        dbInit_ = true;
    }

    NCensusHits* hits = new NCensusHits;

    closedOr_->lookup(isoSig, hits);
    closedNor_->lookup(isoSig, hits);
    closedHyp_->lookup(isoSig, hits);
    cuspedHypOr_->lookup(isoSig, hits);
    cuspedHypNor_->lookup(isoSig, hits);
    hypKnotLink_->lookup(isoSig, hits);

    return hits;
}

NCensusDB* NCensus::standardDB(const char* filename, const char* desc) {
    return new NCensusDB(GlobalDirs::census() + "/" + filename, desc);
}

} // namespace regina

