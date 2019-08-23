
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2019, Alex He                                           *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <map>
#include <set>
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"

namespace regina {

void NormalSurface::calculateBoundaries() const {
    /*
    Counts the number of boundary curves by transforming the boundary curves
    into a collection of "interval isometries", and counting the number of
    orbits of these isometries.

    This is based on the algorithm given by Agol, Hass and Thurston (2006) for
    counting connected components of normal curves and normal surfaces. However,
    since this routine works only with boundary curves, which are necessarily
    closed, this implementation is dramatically simpler than the much more
    general algorithm originally given by Agol, Hass and Thurston.
    */

    const Triangulation<3>* tri = triangulation();

    // Assign intervals to each boundary edge of tri, where the length of the
    // assigned interval corresponds to the weight of the given surf at edge i.
    std::map<long, std::pair<long, long>> intervals;
    long weight, totalWeight = 0;
    for (const Edge<3>* e : tri->edges())
        if (e->isBoundary()) {
            weight = edgeWeight(e->index()).longValue();
            intervals[e->index()] =
                std::make_pair(totalWeight, totalWeight + weight);
            totalWeight += weight;
        }

    // Encode interval isometries as a vector mapping each number k to the
    // set of all images of k under the isometries.
    std::vector<std::vector<long>> mappings(totalWeight);
    for (const Triangle<3>* face : tri->triangles()) {
        if (! face->isBoundary())
            continue;

        // There is a collection of parallel normal arcs corresponding to each
        // vertex of the current boundary face. Assign an interval isometry to
        // each such collection of parallel arcs.
        for (int v = 0; v < 3; ++v) {
            // interval0 is a list of points on edge(index0) emanating out from
            // vertex v.
            int index0 = (v + 1) % 3;
            Perm<4> edgeMap0 = face->edgeMapping(index0);
            auto interval0 = intervals[ face->edge(index0)->index() ];
            bool reverseInterval0 = (edgeMap0[0] != v);

            // interval1 is a list of points on edge(index1) emanating out from
            // vertex v.
            int index1 = (v + 2) % 3;
            Perm<4> edgeMap1 = face->edgeMapping(index1);
            auto interval1 = intervals[ face->edge(index1)->index() ];
            bool reverseInterval1 = (edgeMap1[0] != v);

            long nArcs = arcs(face->index(), v).longValue();
            for (long j = 0; j < nArcs; ++j) {
                long num0 = (reverseInterval0 ?
                    interval0.second - 1 - j : interval0.first + j);
                long num1 = (reverseInterval1 ?
                    interval1.second - 1 - j : interval1.first + j);
                mappings[num0].push_back(num1);
                mappings[num1].push_back(num0);
            }
        }
    }

    // Since the boundary curves of surf are all closed curves, we can count the
    // orbits in the interval isometries by simply traversing the orbits, and
    // "marking" points which have already been visited. This is dramatically
    // simpler than the much more general algorithm given by Agol, Hass and
    // Thurston (2006).
    bool* marked = new bool[totalWeight];
    std::fill(marked, marked + totalWeight, false);

    long orbits = 0;
    long markNext = 0;
    while (true) {
        while (markNext < totalWeight && marked[markNext])
            ++markNext;
        if (markNext == totalWeight)
            break;

        long currentPoint = markNext++;

        // Traverse the orbit containing currentPoint until all points in the
        // orbit have been "marked".
        while (true) {
            marked[currentPoint] = true;

            // mappings[currentPoint] should always have exactly two elements.
            if (! marked[mappings[currentPoint].front()])
                currentPoint = mappings[currentPoint].front();
            else if (! marked[mappings[currentPoint].back()])
                currentPoint = mappings[currentPoint].back();
            else
                break;
        }

        ++orbits;
    }

    delete[] marked;
    boundaries_ = orbits;
}

} // namespace regina

