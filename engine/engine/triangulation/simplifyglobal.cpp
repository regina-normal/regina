
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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
    #include "ntriangulation.h"
#else
    #include "engine/triangulation/ntriangulation.h"
#endif

bool NTriangulation::intelligentSimplify() {
    return simplifyToLocalMinimum();
}

bool NTriangulation::simplifyToLocalMinimum() {
    bool changed = false;
    bool changedNow = true;
    EdgeIterator eit;
    FaceIterator fit;
    TetrahedronIterator tit;
    NEdge* edge;
    NFace* face;
    NTetrahedron* tet;
    while (changedNow) {
        changedNow = false;
        if (! calculatedSkeleton) {
            calculateSkeleton();
        }
        if (vertices.size() > components.size()) {
            if (crushMaximalForest()) {
                if (! calculatedSkeleton)
                    calculateSkeleton();
                // Keep trying to simplify in this iteration of the loop.
                // Thus changedNow will remain false.
                changed = true;
            }
        }
        if (hasBoundaryFaces()) {
            for (tit.init(tetrahedra); !(tit.done()); tit++) {
                tet = *tit;
                if (shellBoundary(tet)) {
                    changedNow = true;
                    changed = true;
                    break;
                }
            }
            if (changedNow)
                continue;
            for (fit.init(faces); ! (fit.done()); fit++) {
                face = *fit;
                if (openBook(face)) {
                    changedNow = true;
                    changed = true;
                    break;
                }
            }
        }
        if (changedNow)
            continue;
        for (eit.init(edges); ! (eit.done()); eit++) {
            edge = *eit;
            if (threeTwoMove(edge)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoZeroMove(edge)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoOneMove(edge, 0)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoOneMove(edge, 1)) {
                changedNow = true;
                changed = true;
                break;
            }
        }
    }
    return changed;
}

