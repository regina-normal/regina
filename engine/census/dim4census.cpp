
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include <algorithm>
#include <sstream>
#include "census/dim4census.h"
#include "census/dim4gluingpermsearcher.h"
#include "dim4/dim4triangulation.h"
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"
#include "utilities/memutils.h"

namespace regina {

unsigned long Dim4Census::formCensus(NPacket* parent, unsigned nPentachora,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFacets, AcceptTriangulation sieve,
        void* sieveArgs, NProgressManager* manager) {
    // If obviously nothing is going to happen but we won't realise
    // it until we've actually generated the facet pairings, change
    // nPentachora to 0 so we'll realise it immediately once the new
    // thread starts.
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone)
        nPentachora = 0;

    // Start the census!
    NProgressMessage* progress;
    if (manager) {
        progress = new NProgressMessage("Starting census generation...");
        manager->setProgress(progress);
    } else
        progress = 0;

    Dim4Census* census = new Dim4Census(parent, finiteness, orientability,
        sieve, sieveArgs, progress);

    if (manager) {
        Dim4FacetPairing::findAllPairings(nPentachora, boundary, nBdryFacets,
            Dim4Census::foundFacetPairing, census, true);
        return 0;
    } else {
        Dim4FacetPairing::findAllPairings(nPentachora, boundary, nBdryFacets,
            Dim4Census::foundFacetPairing, census, false);
        unsigned long ans = census->whichSoln_ - 1;
        delete census;
        return ans;
    }
}

unsigned long Dim4Census::formPartialCensus(const Dim4FacetPairing* pairing,
        NPacket* parent, NBoolSet finiteness, NBoolSet orientability,
        AcceptTriangulation sieve, void* sieveArgs) {
    // Is it obvious that nothing will happen?
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone)
        return 0;

    // Make a list of automorphisms.
    Dim4FacetPairingIsoList autos;
    pairing->findAutomorphisms(autos);

    // Select the individual gluing permutations.
    Dim4Census census(parent, finiteness, orientability, sieve, sieveArgs, 0);
    Dim4GluingPermSearcher::findAllPerms(pairing, &autos,
        ! census.orientability_.hasFalse(), ! census.finiteness_.hasFalse(),
        Dim4Census::foundGluingPerms, &census);

    // Clean up.
    std::for_each(autos.begin(), autos.end(), FuncDelete<Dim4Isomorphism>());
    return census.whichSoln_ - 1;
}

Dim4Census::Dim4Census(NPacket* parent, const NBoolSet& finiteness,
        const NBoolSet& orientability, AcceptTriangulation sieve,
        void* sieveArgs, NProgressMessage* progress) :
        parent_(parent), finiteness_(finiteness), orientability_(orientability),
        sieve_(sieve), sieveArgs_(sieveArgs), progress_(progress),
        whichSoln_(1) {
}

void Dim4Census::foundFacetPairing(const Dim4FacetPairing* pairing,
        const Dim4FacetPairingIsoList* autos, void* census) {
    Dim4Census* realCensus = static_cast<Dim4Census*>(census);
    if (pairing) {
        // We've found another facet pairing.
        if (realCensus->progress_)
            realCensus->progress_->setMessage(pairing->toString());

        // Select the individual gluing permutations.
        Dim4GluingPermSearcher::findAllPerms(pairing, autos,
            ! realCensus->orientability_.hasFalse(),
            ! realCensus->finiteness_.hasFalse(),
            Dim4Census::foundGluingPerms, census);
    } else {
        // Census generation has finished.
        if (realCensus->progress_) {
            realCensus->progress_->setMessage("Finished.");
            realCensus->progress_->setFinished();
            delete realCensus;
        }
    }
}

void Dim4Census::foundGluingPerms(const Dim4GluingPermSearcher* perms,
        void* census) {
    if (perms) {
        // We've found another permutation set.
        // Triangulate and see what we've got.
        Dim4Triangulation* tri = perms->triangulate();
        Dim4Census* realCensus = static_cast<Dim4Census*>(census);

        bool ok = true;
        if (! tri->isValid())
            ok = false;
        else if ((! realCensus->finiteness_.hasFalse()) && tri->isIdeal())
            ok = false;
        else if ((! realCensus->finiteness_.hasTrue()) && (! tri->isIdeal()))
            ok = false;
        else if ((! realCensus->orientability_.hasTrue()) &&
                tri->isOrientable())
            ok = false;
        else if (realCensus->sieve_ &&
                ! realCensus->sieve_(tri, realCensus->sieveArgs_))
            ok = false;

        if (ok) {
            // Put it in the census!
            // Make sure it has a charming label.
            std::ostringstream out;
            out << "Item " << realCensus->whichSoln_;
            tri->setPacketLabel(realCensus->parent_->makeUniqueLabel(
                out.str()));
            realCensus->parent_->insertChildLast(tri);
            realCensus->whichSoln_++;
        } else {
            // Bad triangulation.
            delete tri;
        }
    }
}

} // namespace regina

