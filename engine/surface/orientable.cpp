
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <queue>
#include "surface/disc.h"

namespace regina {

namespace {
    /**
     * Stores orientation and sides A/B for a normal disc.
     */
    struct OrientData {
        int orient { 0 };
            /**< Specifies the orientation of the disc.
                 1 represents with the natural boundary orientation.
                 -1 represents against the natural boundary orientation.
                 0 means orientation is not yet determined. */
        int sides { 0 };
            /**< Specifies which sides of the disc are sides A/B.
                 If sides is 1, discs are numbered from side A to B.
                 If sides is -1, discs are numbered from side B to A.
                 A value of 0 means sides are not yet determined. */
    };
}

void NormalSurface::calculateOrientable() const try {
    // This is going to be ghastly.
    // We will create an orientation and side selection for every disc.

    orientable_.reset();
    twoSided_.reset();
    connected_.reset();

    // First check that the precondition (compactness) holds, since if
    // it doesn't we'll have a rather nasty crash (thanks Nathan).
    if (! isCompact())
        return;

    // All right.  Off we go.
    DiscSetSurfaceData<OrientData> orients(*this);
        // Stores the orientation of each disc.
        // Could throw an IntegerOverflow.
    std::queue<DiscSpec> discQueue;
        // A queue of discs whose orientations must be propagated.
    DiscSpecIterator it(orients);
        // Runs through the discs whose orientations might not have yet
        // been determined.
    DiscSpec use;
        // The disc that currently holds our interest.

    int nGluingArcs;     // The number of arcs on the current disc to
                         //     which an adjacent disc might may be glued.
    Perm<4> arc[8];       // Holds each gluing arc for the current disc.

    bool myOrient, yourOrient, sameOrient;
    bool mySides, yourSides, sameSides;
    int i;

    bool noComponents = true;

    while (true) {
        // If there's no discs to propagate from, choose the next
        // unoriented one.
        while (discQueue.empty() && (! it.done())) {
            if (orients.data(*it).orient == 0) {
                orients.data(*it).orient = 1;
                orients.data(*it).sides = 1;
                discQueue.push(*it);
                if (noComponents)
                    noComponents = false;
                else
                    connected_ = false;
            }
            ++it;
        }
        if (discQueue.empty())
            break;

        // At the head of the queue is the next already-oriented disc
        // whose orientation must be propagated.
        use = discQueue.front();
        discQueue.pop();

        // Determine along which arcs we may glue other discs.
        if (use.type < 4) {
            // Current disc is a triangle.
            nGluingArcs = 3;
            for (i = 0; i < 3; i++)
                arc[i] = triDiscArcs[use.type][i];
        } else if (use.type < 7) {
            // Current disc is a quad.
            nGluingArcs = 4;
            for (i = 0; i < 4; i++)
                arc[i] = quadDiscArcs[use.type - 4][i];
        } else {
            // Current disc is an octagon.
            nGluingArcs = 8;
            for (i = 0; i < 8; i++)
                arc[i] = octDiscArcs[use.type - 7][i];
        }

        // Process any discs that might be adjacent to each of these
        // gluing arcs.
        for (i = 0; i < nGluingArcs; i++) {
            // Establish which is the adjacent disc.
            auto adjDisc = orients.adjacentDisc(use, arc[i]);
            if (! adjDisc)
                continue;

            // There is actually a disc glued along this arc.
            // Determine the desired properties of the adjacent disc.
            Perm<4> adjArc = adjDisc->second;

            if (! orientable_.has_value()) {
                myOrient = discOrientationFollowsEdge(use.type,
                    arc[i][0], arc[i][1], arc[i][2]);
                yourOrient = discOrientationFollowsEdge(adjDisc->first.type,
                    adjArc[0], adjArc[2], adjArc[1]);
                sameOrient = (myOrient && yourOrient) ||
                    ((! myOrient) && (! yourOrient));
            } else
                sameOrient = true;

            if (! twoSided_.has_value()) {
                mySides = numberDiscsAwayFromVertex(use.type, arc[i][0]);
                yourSides = numberDiscsAwayFromVertex(
                    adjDisc->first.type, adjArc[0]);
                sameSides = (mySides && yourSides) ||
                    ((! mySides) && (! yourSides));
            } else
                sameSides = true;

            // Propagate these properties.

            if (orients.data(adjDisc->first).orient == 0) {
                orients.data(adjDisc->first).orient = (sameOrient ?
                    orients.data(use).orient : -orients.data(use).orient);
                orients.data(adjDisc->first).sides = (sameSides ?
                    orients.data(use).sides : -orients.data(use).sides);
                discQueue.push(adjDisc->first);
            } else {
                if (! orientable_.has_value()) {
                    if (sameOrient) {
                        if (orients.data(adjDisc->first).orient !=
                                orients.data(use).orient)
                            orientable_ = false;
                    } else {
                        if (orients.data(adjDisc->first).orient ==
                                orients.data(use).orient)
                            orientable_ = false;
                    }
                }
                if (! twoSided_.has_value()) {
                    if (sameSides) {
                        if (orients.data(adjDisc->first).sides !=
                                orients.data(use).sides)
                            twoSided_ = false;
                    } else {
                        if (orients.data(adjDisc->first).sides ==
                                orients.data(use).sides)
                            twoSided_ = false;
                    }
                }
            }
            if (orientable_.has_value() && twoSided_.has_value() &&
                    connected_.has_value())
                return;
        }
    }

    // We made it through!  Any properties that weren't proven false
    // must be true.

    if (! orientable_.has_value())
        orientable_ = true;
    if (! twoSided_.has_value())
        twoSided_ = true;
    if (! connected_.has_value())
        connected_ = true;
} catch (const IntegerOverflow&) {
    throw UnsolvedCase("This surface has too many normal discs "
        "for this computation to proceed");
}

} // namespace regina

