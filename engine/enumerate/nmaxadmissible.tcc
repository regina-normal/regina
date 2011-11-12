
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/* To be included from nmaxadmissible.h. */

#include "enumerate/nenumconstraint.h"
#include <algorithm>
#include <deque>

namespace regina {

template <class BitmaskType, class RayIterator>
std::vector<BitmaskType>* NMaxAdmissible::enumerate(
        RayIterator beginExtremalRays, RayIterator endExtremalRays,
        const NEnumConstraintList* constraints) {
    if (beginExtremalRays == endExtremalRays) {
        // Input is empty, so output is empty.
        return new std::vector<BitmaskType>();
    }

    unsigned dim = (*beginExtremalRays)->size();
    BitmaskType b(dim);
    int i;

    // Rewrite the constraints as bitmasks.
    std::deque<BitmaskType> constMasks;
    if (constraints) {
        NEnumConstraintList::const_iterator cit;
        std::set<unsigned>::const_iterator sit; 
        for (cit = constraints->begin(); cit != constraints->end(); ++cit) {
            b.reset();
            for (sit = cit->begin(); sit != cit->end(); ++sit)
                b.set(*sit, true);
            constMasks.push_back(b);
        }
    }

    // Create a set of bitmasks representing the admissible 1-faces of
    // the cone, i.e., the set of admissible extremal rays.
    std::deque<BitmaskType> rays;
    for (RayIterator rit = beginExtremalRays; rit != endExtremalRays; ++rit) {
        for (i = 0; i < dim; ++i)
            b.set(i, (**rit)[i] != 0);
        rays.push_back(b);
    }

    // Create a working set of admissible faces.
    // We initialise this to the set of all admissible 1-faces, as above.
    std::deque<BitmaskType> faces(rays);

    // Create the final set of faces to return.
    std::vector<BitmaskType>* maxFaces = new std::vector<BitmaskType>();

    // Keep expanding the faces using additional extremal rays until we can
    // expand no more.
    // TODO: Change this to one dimension at a time.
    typename std::deque<BitmaskType>::const_iterator r, c;
    BitmaskType tmp(dim);
    bool expanded, broken;
    while (! faces.empty()) {
        b = faces.front();
        faces.pop_front();

        // If this is a copy of some other face already in the queue or
        // in the solution set, just ignore it.
        bool found = false;
        if (std::find(maxFaces->begin(), maxFaces->end(), b)
                != maxFaces->end() ||
                std::find(faces.begin(), faces.end(), b) != faces.end())
            continue;

        // Expand this face by combining with other extremal rays.
        expanded = false;
        for (r = rays.begin(); r != rays.end(); ++r) {
            // Ignore rays already in this face.
            if (*r <= b)
                continue;

            // Ignore rays that will break admissibility.
            BitmaskType comb(b);
            comb |= *r;
            broken = false;
            for (c = constMasks.begin(); c != constMasks.end(); ++c) {
                tmp = comb;
                tmp &= *c;
                if (! tmp.atMostOneBit()) {
                    broken = true;
                    break;
                }
            }
            if (broken)
                continue;

            // We have a higher-dimensional face.
            faces.push_back(comb);
            expanded = true;
        }
        if (! expanded)
            maxFaces->push_back(b);
    }

    // All done!
    return maxFaces;
}

} // namespace regina

