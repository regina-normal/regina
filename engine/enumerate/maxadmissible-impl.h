
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file enumerate/maxadmissible-impl.h
 *  \brief Contains implementations of template functions from maxadmissible.h.
 *
 *  This file is automatically included from maxadmissible.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __MAXADMISSIBLE_IMPL_H
#ifndef __DOXYGEN
#define __MAXADMISSIBLE_IMPL_H
#endif

#include "enumerate/enumconstraints.h"
#include "enumerate/maxadmissible.h"
#include <algorithm>
#include <list>

namespace regina {

template <class BitmaskType, class RayIterator>
std::vector<BitmaskType>* MaxAdmissible::enumerate(
        RayIterator beginExtremalRays, RayIterator endExtremalRays,
        const EnumConstraints* constraints) {
    if (beginExtremalRays == endExtremalRays) {
        // Input is empty, so output is empty.
        return new std::vector<BitmaskType>();
    }

    size_t dim = (*beginExtremalRays).size();
    BitmaskType b(dim);
    int i;

    // Rewrite the constraints as bitmasks.
    std::vector<BitmaskType> constMasks;
    if (constraints) {
        EnumConstraints::const_iterator cit;
        std::set<unsigned long>::const_iterator sit; 
        for (cit = constraints->begin(); cit != constraints->end(); ++cit) {
            b.reset();
            for (sit = cit->begin(); sit != cit->end(); ++sit)
                b.set(*sit, true);
            constMasks.push_back(b);
        }
    }

    // Create a set of bitmasks representing the admissible 1-faces of
    // the cone, i.e., the set of admissible extremal rays.
    std::vector<BitmaskType> rays;
    for (RayIterator rit = beginExtremalRays; rit != endExtremalRays; ++rit) {
        for (i = 0; i < dim; ++i)
            b.set(i, (*rit)[i] != 0);
        rays.push_back(b);
    }

    // Create a working set of admissible faces.
    // We initialise this to the set of all admissible 1-faces, as above.
    std::list<BitmaskType> faces(rays.begin(), rays.end());

    // Create the final set of faces to return.
    std::vector<BitmaskType>* maxFaces = new std::vector<BitmaskType>();

    // Keep expanding the faces using additional extremal rays until we can
    // expand no more.
    // The ith iteration of the following loop enumerates _all_ admissible
    // faces of dimension i+1, and identifies all _maximal_ admissible
    // faces of dimension i.
    std::list<BitmaskType> nextDim;
    typename std::vector<BitmaskType>::const_iterator r, c;
    typename std::list<BitmaskType>::const_iterator f;
    typename std::list<BitmaskType>::iterator n, next;
    bool isMax, broken;
    while (! faces.empty()) {
        for (f = faces.begin(); f != faces.end(); ++f) {
            // Expand this face by combining with other extremal rays.
            isMax = true;
            for (r = rays.begin(); r != rays.end(); ++r) {
                BitmaskType comb(*f);
                comb |= *r;

                // Ignore rays already in this face.
                if (comb == *f)
                    continue;

                // Ignore rays that will break admissibility.
                broken = false;
                for (c = constMasks.begin(); c != constMasks.end(); ++c) {
                    b = comb;
                    b &= *c;
                    if (! b.atMostOneBit()) {
                        broken = true;
                        break;
                    }
                }
                if (broken)
                    continue;

                // comb expands *f to a higher-dimensional superface.
                isMax = false;

                // Strip out duplicates, and also strip out superfaces of
                // too high a dimension (since we only want to step up one
                // dimension at a time).
                broken = false;
                n = nextDim.begin();
                while (n != nextDim.end()) {
                    next = n;
                    ++next;
                    if (*n <= comb) {
                        // comb has too high a dimension, or is a duplicate.
                        broken = true;
                        break;
                    }
                    if (comb <= *n) {
                        // *n has too high a dimension.
                        nextDim.erase(n);
                    }
                    n = next;
                }
                if (! broken)
                    nextDim.push_back(comb);
            }
            if (isMax)
                maxFaces->push_back(*f);
        }
        std::swap(nextDim, faces);
        nextDim.clear();
    }

    // All done!
    return maxFaces;
}

} // namespace regina

#endif
