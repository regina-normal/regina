
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
    #include "ndisc.h"
    #include "nqueue.h"
#else
    #include "engine/surfaces/ndisc.h"
    #include "engine/utilities/nqueue.h"
#endif

void NNormalSurface::calculateOrientable() {
    // This is going to be ghastly.
    // We will create an orientation for every disc.

    // First check that there aren't too many discs!


    // All right.  Off we go.
    calculatedOrientable = false;
    
    NDiscSetSurfaceData<int> orients(*this, 0);
        // Stores the orientation of each disc.
    NQueue<NDiscSpec> queue;
        // A queue of discs whose orientations must be propagated.
    NDiscSpecIterator it(orients);
        // Runs through the discs whose orientations might not have yet
        // been determined.
    NDiscSpec use;
        // The disc that currently holds our interest.

    int nGluingArcs;     // The number of arcs on the current disc to
                         //     which an adjacent disc might may be glued.
    int face[8];         // The faces of this tetrahedron on which the
                         //     gluing arcs lie.
    int tmpFace;         // As above for one particular face.
    int vertex[8];       // The vertices of this tetrahedron about which
                         //     the gluing arcs run.
    int tmpVertex;       // As above for one particular vertex.

    NDiscSpec* adjDisc;  // The disc to which the current disc is glued.
    int adjFace;         // The face of the adjacent tetrahedron on which
                         //     the gluing arc lies.
    int adjVertex;       // The vertex of the adjacent tetrahedron about
                         //     which the gluing arc runs.

    bool myOrient, yourOrient, sameOrient;
    int i;

    while (true) {
        // If there's no discs to propagate from, choose the next
        // unoriented one.
        while (queue.empty() && (! it.done())) {
            if (orients.data(*it) == 0) {
                orients.data(*it) = 1;
                queue.insert(*it);
            }
            it++;
        }
        if (queue.empty())
            break;

        // At the head of the queue is the next already-oriented disc
        // whose orientation must be propagated.
        use = queue.remove();

        // Determine along which arcs we may glue other discs.
        nGluingArcs = 0;

        if (use.type < 4) {
            // Current disc is a triangle.
            for (tmpFace = 0; tmpFace < 4; tmpFace++) {
                if (tmpFace == use.type)
                    continue;
                face[nGluingArcs] = tmpFace;
                vertex[nGluingArcs] = use.type;
                nGluingArcs++;
            }
        } else if (use.type < 7) {
            // Current disc is a quad.
            for (tmpFace = 0; tmpFace < 4; tmpFace++) {
                face[nGluingArcs] = tmpFace;
                vertex[nGluingArcs] =
                    vertexSplitPartner[use.type - 4][tmpFace];
                nGluingArcs++;
            }
        } else {
            // Current disc is an octahedron.
            for (tmpFace = 0; tmpFace < 4; tmpFace++)
                for (tmpVertex = 0; tmpVertex < 4; tmpVertex++) {
                    if (tmpVertex == tmpFace)
                        continue;
                    if (vertexSplit[tmpVertex][tmpFace] == use.type - 7)
                        continue;
                    face[nGluingArcs] = tmpFace;
                    vertex[nGluingArcs] = tmpVertex;
                    nGluingArcs++;
                }
        }

        // Process any discs that might be adjacent to each of these
        // gluing arcs.
        for (i = 0; i < nGluingArcs; i++) {
            // Establish which is the adjacent disc.
            adjDisc = orients.adjacentDisc(use, face[i], vertex[i],
                adjFace, adjVertex);
            if (adjDisc == 0)
                continue;

            // There is actually a disc glued along this arc.
            // Determine the desired orientation of the adjacent disc.
            myOrient = numberDiscsAwayFromVertex(use.type, vertex[i]);
            yourOrient = numberDiscsAwayFromVertex(adjDisc->type, adjVertex);
            sameOrient = (myOrient && yourOrient) ||
                ((! myOrient) && (! yourOrient));

            // Check the orientation of the adjacent disc.
            if (orients.data(*adjDisc) == 0) {
                orients.data(*adjDisc) = (sameOrient ?
                    orients.data(use) : -orients.data(use));
                queue.insert(*adjDisc);
            } else if (sameOrient) {
                if (orients.data(*adjDisc) != orients.data(use)) {
                    orientable = -1;
                    calculatedOrientable = true;
                }
            } else {
                if (orients.data(*adjDisc) == orients.data(use)) {
                    orientable = -1;
                    calculatedOrientable = true;
                }
            }

            // Tidy up.
            delete adjDisc;
            if (calculatedOrientable)
                return;
        }
    }

    // We made it through! - the surface must be orientable.
    orientable = 1;
    calculatedOrientable = true;
}

