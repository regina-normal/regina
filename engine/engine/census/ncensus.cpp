
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
    // Bail if obviously nothing is going to happen.
    if (finiteness == NBoolSet::sNone || orientability == NBoolSet::sNone ||
            boundary == NBoolSet::sNone || nTetrahedra == 0) {
        if (manager)
            manager->setProgress(new NProgressFinished());
        return 0;
    }
    if (boundary.hasTrue() && nBdryFaces >= 0 &&
            (nBdryFaces % 2 == 1 || nBdryFaces > 2 * (int)nTetrahedra + 2
            || (nBdryFaces == 0 && ! boundary.hasFalse()))) {
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
        orientability, boundary, nBdryFaces, progress);
    
    if (manager) {
        census->start(0, true);
        return 0;
    } else {
        census->run(0);
        unsigned long ans = census->whichSoln - 1;
        delete census;
        return ans;
    }
}

NCensus::NCensus(NPacket* newParent, unsigned newNTetrahedra,
        const NBoolSet& newFiniteness, const NBoolSet& newOrientability,
        const NBoolSet& newBoundary, int newNBdryFaces,
        NProgressMessage* newProgress) : parent(newParent),
        nTetrahedra(newNTetrahedra), finiteness(newFiniteness),
        orientability(newOrientability), boundary(newBoundary),
        nBdryFaces(newNBdryFaces), progress(newProgress),
        whichSoln(1), automorphism(newNTetrahedra) {
    // Initialise the triangulation and dynamic arrays.
    tet = new (NTetrahedron*)[nTetrahedra];
    orientation = new int[nTetrahedra];

    unsigned i;
    for (i=0; i<nTetrahedra; i++) {
        tet[i] = new NTetrahedron();
        working.addTetrahedron(tet[i]);
        orientation[i] = 0;
    }

    joins = new NTetFace[4 * nTetrahedra];
    joinPermIndices = new int[4 * nTetrahedra];
    for (NTetFace f(0,0); f.tet < (int)nTetrahedra; f++) {
        dest(f) = f;
        joinPermIndex(f) = -1;
    }

    // Initialise the automorphism and related members.
    autoPreImage = new int[nTetrahedra];
    for (i = 0; i < nTetrahedra; i++) {
        automorphism.tetImage(i) = 0;
        automorphism.facePermIndex(i) = -1;
        autoPreImage[i] = -1;
    }
}

NCensus::~NCensus() {
    working.removeAllTetrahedra();
    delete[] tet;
    delete[] orientation;
    delete[] joins;
    delete[] joinPermIndices;
    delete[] autoPreImage;
}

void* NCensus::run(void*) {
    matchFacePairs();
    return 0;
}

} // namespace regina

