
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

/**
 * Stores orientation and sides A/B for a normal disc.
 */
struct OrientData {
    int orient;
        /**< Specifies the orientation of the disc.
             1 represents with the natural boundary orientation.
             -1 represents against the natural boundary orientation.
             0 means orientation is not yet determined. */
    int sides;
        /**< Specifies which sides of the disc are sides A/B.
             If sides is 1, discs are numbered from side A to B.
             If sides is -1, discs are numbered from side B to A.
             A value of 0 means sides are not yet determined. */

    /**
     * Create a new structure with all values initialised to 0.
     */
    OrientData() : orient(0), sides(0) {
    }
};

void NNormalSurface::calculateOrientable() {
    // This is going to be ghastly.
    // We will create an orientation and side selection for every disc.

    // TODO: First check that there aren't too many discs!

    // All right.  Off we go.
    calculatedOrientable = false;
    calculatedTwoSided = false;
    
    NDiscSetSurfaceData<OrientData> orients(*this);
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

    NDiscSpec* adjDisc;  // The disc to which the current disc is glued.
    NPerm arc[8];        // Holds each gluing arc for the current disc.
    NPerm adjArc;        // Represents the corresponding gluing arc on the
                         //     adjacent disc.

    bool myOrient, yourOrient, sameOrient;
    bool mySides, yourSides, sameSides;
    int i;

    while (true) {
        // If there's no discs to propagate from, choose the next
        // unoriented one.
        while (queue.empty() && (! it.done())) {
            if (orients.data(*it).orient == 0) {
                orients.data(*it).orient = 1;
                orients.data(*it).sides = 1;
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
        if (use.type < 4) {
            // Current disc is a triangle.
            nGluingArcs = 3;
            for (i = 0; i < 3; i++)
                arc[i] = triDiscArcs(use.type, i);
        } else if (use.type < 7) {
            // Current disc is a quad.
            nGluingArcs = 4;
            for (i = 0; i < 4; i++)
                arc[i] = quadDiscArcs(use.type - 4, i);
        } else {
            // Current disc is an octahedron.
            nGluingArcs = 8;
            for (i = 0; i < 8; i++)
                arc[i] = octDiscArcs(use.type - 7, i);
        }

        // Process any discs that might be adjacent to each of these
        // gluing arcs.
        for (i = 0; i < nGluingArcs; i++) {
            // Establish which is the adjacent disc.
            adjDisc = orients.adjacentDisc(use, arc[i], adjArc);
            if (adjDisc == 0)
                continue;

            // There is actually a disc glued along this arc.
            // Determine the desired properties of the adjacent disc.

            if (! calculatedOrientable) {
                myOrient = discOrientationFollowsEdge(use.type,
                    arc[i][0], arc[i][1], arc[i][2]);
                yourOrient = discOrientationFollowsEdge(adjDisc->type,
                    adjArc[0], adjArc[2], adjArc[1]);
                sameOrient = (myOrient && yourOrient) ||
                    ((! myOrient) && (! yourOrient));
            } else
                sameOrient = true;

            if (! calculatedTwoSided) {
                mySides = numberDiscsAwayFromVertex(use.type, arc[i][0]);
                yourSides = numberDiscsAwayFromVertex(
                    adjDisc->type, adjArc[0]);
                sameSides = (mySides && yourSides) ||
                    ((! mySides) && (! yourSides));
            } else
                sameSides = true;

            // Propagate these properties.

            if (orients.data(*adjDisc).orient == 0) {
                orients.data(*adjDisc).orient = (sameOrient ?
                    orients.data(use).orient : -orients.data(use).orient);
                orients.data(*adjDisc).sides = (sameSides ?
                    orients.data(use).sides : -orients.data(use).sides);
                queue.insert(*adjDisc);
            } else {
                if (! calculatedOrientable) {
                    if (sameOrient) {
                        if (orients.data(*adjDisc).orient !=
                                orients.data(use).orient) {
                            orientable = -1;
                            calculatedOrientable = true;
                        }
                    } else {
                        if (orients.data(*adjDisc).orient ==
                                orients.data(use).orient) {
                            orientable = -1;
                            calculatedOrientable = true;
                        }
                    }
                }
                if (! calculatedTwoSided) {
                    if (sameSides) {
                        if (orients.data(*adjDisc).sides !=
                                orients.data(use).sides) {
                            twoSided = -1;
                            calculatedTwoSided = true;
                        }
                    } else {
                        if (orients.data(*adjDisc).sides ==
                                orients.data(use).sides) {
                            twoSided = -1;
                            calculatedTwoSided = true;
                        }
                    }
                }
            }

            // Tidy up.
            delete adjDisc;
            if (calculatedOrientable && calculatedTwoSided)
                return;
        }
    }

    // We made it through!  Any properties that weren't proven false
    // must be true.

    if (! calculatedOrientable) {
        orientable = 1;
        calculatedOrientable = true;
    }
    if (! calculatedTwoSided) {
        twoSided = 1;
        calculatedTwoSided = true;
    }
}

