
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
    // Not particularly intelligent.
    return simplifyToLocalMinimum(true);
}

bool NTriangulation::simplifyToLocalMinimum(bool perform) {
    EdgeIterator eit;
    VertexIterator vit;
    BoundaryComponentIterator bit;
    NEdge* edge;
    NBoundaryComponent* bc;
    unsigned long nEdges;
    unsigned long nFaces;
    unsigned long iEdge;
    unsigned long iFace;
    NDynamicArrayIterator<NEdgeEmbedding> embit;

    bool changed = false;
    bool changedNow = true;
    while (changedNow) {
        changedNow = false;
        if (! calculatedSkeleton) {
            calculateSkeleton();
        }

        // Crush a maximal skeleton.
        /* Don't crush a maximal skeleton until we know what the
         * routine does!
        if (vertices.size() > components.size()) {
            if (crushMaximalForest()) {
                if (! calculatedSkeleton)
                    calculateSkeleton();
                // Keep trying to simplify in this iteration of the loop.
                // Thus changedNow will remain false.
                changed = true;
            }
        }
        */

        // Look for boundary simplifications.
        if (hasBoundaryFaces()) {
            for (bit.init(boundaryComponents); !(bit.done()); bit++) {
                bc = *bit;

                // Run through faces of this boundary component looking
                // for shell boundary moves.
                nFaces = (*bit)->getNumberOfFaces();
                for (iFace = 0; iFace < nFaces; iFace++) {
                    if (shellBoundary((*bit)->getFace(iFace)->
                            getEmbedding(0).getTetrahedron(), true, perform)) {
                        changedNow = true;
                        changed = true;
                        break;
                    }
                }
                if (changedNow)
                    break;

                // Run through edges of this boundary component looking
                // for open book moves.
                nEdges = (*bit)->getNumberOfEdges();
                for (iEdge = 0; iEdge < nEdges; iEdge++) {
                    for (embit.init((*bit)->getEdge(iEdge)->getEmbeddings());
                            !(embit.done()); embit++) {
                        if (openBook((*embit).getTetrahedron()->getFace(
                                ((*embit).getVertices())[2]), true, perform)) {
                            changedNow = true;
                            changed = true;
                            break;
                        }
                    }
                    if (changedNow)
                        break;
                }
                if (changedNow)
                    break;
            }
        }
        if (changedNow) {
            if (perform)
                continue;
            else
                return true;
        }

        // Look for internal simplifications.
        for (eit.init(edges); ! (eit.done()); eit++) {
            edge = *eit;
            if (threeTwoMove(edge, true, perform)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoZeroMove(edge, true, perform)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoOneMove(edge, 0, true, perform)) {
                changedNow = true;
                changed = true;
                break;
            }
            if (twoOneMove(edge, 1, true, perform)) {
                changedNow = true;
                changed = true;
                break;
            }
        }
        if (changedNow) {
            if (perform)
                continue;
            else
                return true;
        }
        for (vit.init(vertices); ! (vit.done()); vit++) {
            if (twoZeroMove(*vit, true, perform)) {
                changedNow = true;
                changed = true;
                break;
            }
        }
        if (changedNow) {
            if (perform)
                continue;
            else
                return true;
        }
    }
    return changed;
}

