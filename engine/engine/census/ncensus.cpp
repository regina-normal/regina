
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

#include "census/ncensus.h"
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"

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
            NCensus::selectGluingPerms, census, true);
        return 0;
    } else {
        NFacePairing::findAllPairings(nTetrahedra, boundary, nBdryFaces,
            NCensus::selectGluingPerms, census, false);
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
    // Initialise the triangulation and dynamic arrays.
    tet = new (NTetrahedron*)[nTetrahedra];
    orientation = new int[nTetrahedra];

    unsigned i;
    for (i=0; i<nTetrahedra; i++) {
        tet[i] = new NTetrahedron();
        working.addTetrahedron(tet[i]);
        orientation[i] = 0;
    }

    joinPermIndices = new int[4 * nTetrahedra];
    std::fill(joinPermIndices, joinPermIndices + nTetrahedra * 4, -1);
}

NCensus::~NCensus() {
    working.removeAllTetrahedra();
    delete[] tet;
    delete[] orientation;
    delete[] joinPermIndices;
}

} // namespace regina

