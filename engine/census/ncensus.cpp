
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "regina-config.h" // For QDBM-related macros

#include <algorithm>
#include <cstdlib>
#include <sstream>
#ifdef QDBM_AS_TOKYOCABINET
#include <depot.h>
#include <cabin.h>
#include <villa.h>
#else
#include <stdbool.h> // cstdbool only works for c++11
#include <stdint.h> // cstdint only works for c++11
#include <tcbdb.h>
#include <tcutil.h>
#endif
#include "census/ncensus.h"
#include "census/ngluingpermsearcher.h"
#include "file/nglobaldirs.h"
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
    return new NCensusDB(NGlobalDirs::census() + "/" + filename, desc);
}

const int NCensus::PURGE_NON_MINIMAL = NGluingPermSearcher::PURGE_NON_MINIMAL;
const int NCensus::PURGE_NON_PRIME = NGluingPermSearcher::PURGE_NON_PRIME;
const int NCensus::PURGE_NON_MINIMAL_PRIME =
    NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME;
const int NCensus::PURGE_P2_REDUCIBLE = NGluingPermSearcher::PURGE_P2_REDUCIBLE;

unsigned long NCensus::formCensus(NPacket* parent, unsigned nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces, int whichPurge, AcceptTriangulation sieve,
        void* sieveArgs) {
    // If obviously nothing is going to happen but we won't realise
    // it until we've actually generated the face pairings, change
    // nTetrahedra to 0 so we'll realise it immediately once the new
    // thread starts.
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone)
        nTetrahedra = 0;

    // Start the census!
    NCensus* census = new NCensus(parent, finiteness, orientability,
        whichPurge, sieve, sieveArgs);

    NFacePairing::findAllPairings(nTetrahedra, boundary, nBdryFaces,
        NCensus::foundFacePairing, census);
    unsigned long ans = census->whichSoln - 1;
    delete census;
    return ans;
}

unsigned long NCensus::formPartialCensus(const NFacePairing* pairing,
        NPacket* parent, NBoolSet finiteness, NBoolSet orientability,
        int whichPurge, AcceptTriangulation sieve, void* sieveArgs) {
    // Is it obvious that nothing will happen?
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone)
        return 0;

    // Make a list of automorphisms.
    NFacePairing::IsoList autos;
    pairing->findAutomorphisms(autos);

    // Select the individual gluing permutations.
    NCensus census(parent, finiteness, orientability, whichPurge,
        sieve, sieveArgs);
    NGluingPermSearcher::findAllPerms(pairing, &autos,
        ! census.orientability.hasFalse(), ! census.finiteness.hasFalse(),
        census.whichPurge, NCensus::foundGluingPerms, &census);

    // Clean up.
    std::for_each(autos.begin(), autos.end(), FuncDelete<NIsomorphism>());
    return census.whichSoln - 1;
}

NCensus::NCensus(NPacket* newParent, const NBoolSet& newFiniteness,
        const NBoolSet& newOrientability, int newWhichPurge,
        AcceptTriangulation newSieve, void* newSieveArgs) :
        parent(newParent),
        finiteness(newFiniteness), orientability(newOrientability),
        whichPurge(newWhichPurge), sieve(newSieve), sieveArgs(newSieveArgs),
        whichSoln(1) {
}

void NCensus::foundFacePairing(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, void* census) {
    NCensus* realCensus = static_cast<NCensus*>(census);
    if (pairing) {
        // We've found another face pairing.
        // Select the individual gluing permutations.
        NGluingPermSearcher::findAllPerms(pairing, autos,
            ! realCensus->orientability.hasFalse(),
            ! realCensus->finiteness.hasFalse(),
            realCensus->whichPurge, NCensus::foundGluingPerms, census);
    } else {
        // Census generation has finished.
    }
}

void NCensus::foundGluingPerms(const NGluingPermSearcher* perms, void* census) {
    if (perms) {
        // We've found another permutation set.
        // Triangulate and see what we've got.
        NTriangulation* tri = perms->triangulate();
        NCensus* realCensus = static_cast<NCensus*>(census);

        bool ok = true;
        if (! tri->isValid())
            ok = false;
        else if ((! realCensus->finiteness.hasFalse()) && tri->isIdeal())
            ok = false;
        else if ((! realCensus->finiteness.hasTrue()) && (! tri->isIdeal()))
            ok = false;
        else if ((! realCensus->orientability.hasTrue()) && tri->isOrientable())
            ok = false;
        else if (realCensus->sieve &&
                ! realCensus->sieve(tri, realCensus->sieveArgs))
            ok = false;

        if (ok) {
            // Put it in the census!
            // Make sure it has a charming label.
            // Don't insist on unique labels, since this requirement
            // will soon be dropped and it multiplies the running time
            // by a factor of #triangulations.
            std::ostringstream out;
            out << "Item " << realCensus->whichSoln;
            tri->setLabel(out.str());
            realCensus->parent->insertChildLast(tri);
            realCensus->whichSoln++;
        } else {
            // Bad triangulation.
            delete tri;
        }
    }
}

bool NCensus::mightBeMinimal(NTriangulation* tri, void*) {
    if (! tri->hasBoundaryTriangles()) {
        // No boundary faces.

        // Tests specific to closed finite orientable triangulations:
        if (tri->isOrientable() && (! tri->isIdeal())) {
            // Check for too many vertices.
            if (tri->countVertices() > 1 && tri->size() > 2)
                return false;
        }

        // Check for obvious simplifications.
        if (tri->simplifyToLocalMinimum(false))
            return false;
    }
    return true;
}

} // namespace regina

