
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ncensus.h"
    #include "nprogresstypes.h"
#else
    #include "engine/census/ncensus.h"
    #include "engine/progress/nprogresstypes.h"
#endif

bool NCensus::isCanonicalInternal(int whichTet, const NTetFace& equalUpTo) {
    if (whichTet == (int)nTetrahedra) {
        allAutomorphisms.addLast(new NIsomorphismIndexed(automorphism));
        return true;
    }

    // Have we been cancelled?
    if (progress && progress->isCancelled())
        return false;
    
    // Play nicely with the other children.
    yield();

    int& image(automorphism.tetImage(whichTet));
    int& index(automorphism.facePermIndex(whichTet));
    int order;
    NTetFace newEqualUpTo;
    for (image = 0; image < (int)nTetrahedra; image++) {
        if (autoPreImage[image] >= 0)
            continue;

        autoPreImage[image] = whichTet;
        for (index = 0; index < 24; index++) {
            newEqualUpTo = equalUpTo;
            order = cmpWithPreImage(whichTet + 1, newEqualUpTo);
            if (order < 0)
                continue;
            if (order > 0) {
                allAutomorphisms.flushAndDelete();
                autoPreImage[image] = -1;
                image = -1;
                index = -1;
                return false;
            }
            if (! isCanonicalInternal(whichTet + 1, newEqualUpTo)) {
                autoPreImage[image] = -1;
                image = -1;
                index = -1;
                return false;
            }
        }
        index = -1;
        autoPreImage[image] = -1;
    }
    image = -1;
    return true;
}

int NCensus::cmpWithPreImage(unsigned compareTets, NTetFace& equalUpTo) {
    NTetFace myDest, imageOfYourDest, yourDest;
    int yourDestTet;

    while (equalUpTo.tet < (int)compareTets) {
        myDest = dest(equalUpTo);
        imageOfYourDest = dest(automorphism[equalUpTo]);

        if (imageOfYourDest.isBoundary(nTetrahedra))
            yourDest.setBoundary(nTetrahedra);
        else {
            if (autoPreImage[imageOfYourDest.tet] == -1)
                return 0;
            yourDestTet = autoPreImage[imageOfYourDest.tet];
            yourDest = NTetFace(yourDestTet, automorphism.facePerm(yourDestTet).
                preImageOf(imageOfYourDest.face));
        }

        if (myDest < yourDest)
            return -1;
        if (yourDest < myDest)
            return 1;

        equalUpTo++;
    }

    return 0;
}

