
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <sstream>
#include "census/ncensus.h"
#include "census/ngluingperms.h"
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"
#include "triangulation/ntriangulation.h"

namespace regina {

unsigned long formCensus(NPacket* parent, unsigned nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces, NProgressManager* manager) {
    // Bail if obviously nothing is going to happen but we won't realise
    // it until we've actually generated the face pairings.
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone) {
        if (manager)
            manager->setProgress(new NProgressFinished());
        return 0;
    }

    // Start the census!
    NProgressMessage* progress;
    if (manager) {
        progress = new NProgressMessage("Starting census generation...",
            true);
        manager->setProgress(progress);
    } else
        progress = 0;

    NCensus* census = new NCensus(parent, nTetrahedra, finiteness,
        orientability, progress);
    
    if (manager) {
        NFacePairing::findAllPairings(nTetrahedra, boundary, nBdryFaces,
            NCensus::foundFacePairing, census, true);
        return 0;
    } else {
        NFacePairing::findAllPairings(nTetrahedra, boundary, nBdryFaces,
            NCensus::foundFacePairing, census, false);
        unsigned long ans = census->whichSoln - 1;
        delete census;
        return ans;
    }
}

NCensus::NCensus(NPacket* newParent, unsigned nTetrahedra,
        const NBoolSet& newFiniteness, const NBoolSet& newOrientability,
        NProgressMessage* newProgress) : parent(newParent),
        finiteness(newFiniteness), orientability(newOrientability),
        progress(newProgress), whichSoln(1) {
}

void NCensus::foundFacePairing(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, void* census) {
    NCensus* realCensus = (NCensus*)census;
    if (pairing) {
        // We've found another face pairing.
        if (realCensus->progress)
            realCensus->progress->setMessage(pairing->toString());

        // Select the individual gluing permutations.
        NGluingPerms::findAllPerms(pairing, autos,
            ! realCensus->orientability.hasFalse(), NCensus::foundGluingPerms,
            census);
    } else {
        // Census generation has finished.
        if (realCensus->progress) {
            realCensus->progress->setMessage("Finished.");
            realCensus->progress->setFinished();
            delete realCensus;
        }
    }
}

void NCensus::foundGluingPerms(const NGluingPerms* perms, void* census) {
    if (perms) {
        // We've found another permutation set.
        // Triangulate and see what we've got.
        NTriangulation* tri = perms->triangulate();
        NCensus* realCensus = (NCensus*)census;

        bool ok = true;
        if (! tri->isValid())
            ok = false;
        else if ((! realCensus->finiteness.hasFalse()) && tri->isIdeal())
            ok = false;
        else if ((! realCensus->finiteness.hasTrue()) && (! tri->isIdeal()))
            ok = false;
        else if ((! realCensus->orientability.hasTrue()) && tri->isOrientable())
            ok = false;

        if (ok) {
            // Put it in the census!
            // Make sure it has a charming label.
            std::ostringstream out;
            out << "Item " << realCensus->whichSoln;
            tri->setPacketLabel(realCensus->parent->makeUniqueLabel(
                out.str()));
            realCensus->parent->insertChildLast(tri);
            realCensus->whichSoln++;
        } else {
            // Bad triangulation.
            delete tri;
        }
    }
}

} // namespace regina

