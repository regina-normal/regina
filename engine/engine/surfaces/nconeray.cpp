
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
    #include "nconeray.h"
    #include "nvectorunit.h"
    #include "nvectormatrix.h"
    #include "nmatrixint.h"
#else
    #include "engine/surfaces/nconeray.h"
    #include "engine/maths/nvectorunit.h"
    #include "engine/maths/nvectormatrix.h"
    #include "engine/maths/nmatrixint.h"
#endif

NConeRay* intersectLine(const NConeRay& pos, const NConeRay& neg,
        const NConeRay& hyperplane) {
    NConeRay* ans = (NConeRay*)neg.clone();

    (*ans) *= (hyperplane * pos);
    (*ans).subtractCopies(pos, hyperplane * neg);
    (*ans).scaleDown();

    return ans;
}

void intersectCone(NDoubleList<NConeRay*>& results,
        const NDoubleList<NConeRay*>& oldRays,
        const NConeRay& hyperplane,
        bool embeddedOnly) {
    if (oldRays.size() == 0)
        return;

    NDoubleList<NConeRay*> pos;
    NDoubleList<NConeRay*> neg;

    // Run through the old rays and determine which side of the
    // hyperplane they lie on.
    // Rays lying within the hyperplane will be added to the new
    // solution set.
    NDoubleListIterator<NConeRay*> it(oldRays);
    unsigned nCoords = (*it)->size();
    NLargeInteger dot;
    while (! it.done()) {
        dot = hyperplane * (**it);
        if (dot == NConeRay::zero)
            results.addLast((NConeRay*)(*it)->clone());
        else if (dot < NConeRay::zero)
            neg.addLast(*it);
        else
            pos.addLast(*it);
        it++;
    }

    // Run through the pairs of positive and negative rays.
    // Rays that are joined by an edge will result in a new ray
    // being added to the solution set.
    // One can prove that no ray will ever have been added to the
    // solution set twice.
    NDoubleListIterator<NConeRay*> posit, negit;
    bool adjacent, hasCommonFaces;
    unsigned i;
    posit.init(pos);
    while (! posit.done()) {
        negit.init(neg);
        while (! negit.done()) {
            // Are we supposed to check for compatibility?
            if (embeddedOnly)
                if (! (*posit)->isCompatibleWith(**negit)) {
                    negit++;
                    continue;
                }

            // Two rays are adjacent if and only if there is no
            // other ray belonging to all of their common faces.
            adjacent = true;
            it.init(oldRays);
            while (! it.done()) {
                if (*it == *posit || *it == *negit) {
                    it++;
                    continue;
                }
                hasCommonFaces = true;
                for (i=0; i<nCoords; i++)
                    if ((**posit)[i] == 0 && (**negit)[i] == 0
                            && (**it)[i] != 0) {
                        hasCommonFaces = false;
                        break;
                    }
                if (hasCommonFaces) {
                    adjacent = false;
                    break;
                }
                it++;
            }

            // If the rays are adjacent then join them and put the
            // corresponding intersection with the hyperplane in the
            // results set.
            if (adjacent)
                results.addLast(intersectLine(**posit, **negit, hyperplane));
            negit++;
        }
        posit++;
    }
}

NDoubleList<NConeRay*>* intersectCone(
        const NDoubleList<NConeRay*>& oldRays,
        const NMatrixInt& subspace,
        bool embeddedOnly) {
    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // There are no hyperplanes in the subspace!
        // We will have to clone the list of extremal rays.
        NDoubleList<NConeRay*>* ans = new NDoubleList<NConeRay*>;
        NDoubleListIterator<NConeRay*> it(oldRays);
        while (! it.done()) {
            ans->addLast((NConeRay*)(*it)->clone());
            it++;
        }
        return ans;
    }
    
    // Create the two vector lists with which we will work.
    // Fill the first list with the intersection with the first
    // hyperplane.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    NDoubleList<NConeRay*>* list[2];
    list[0] = new NDoubleList<NConeRay*>;
    list[1] = new NDoubleList<NConeRay*>;
    int workingList = 0;
    intersectCone(*list[0], oldRays,
        NVectorMatrixRow<NLargeInteger>(subspace, 0), embeddedOnly);
    
    // Now run around intersecting each extra hyperplane as it comes.
    for (unsigned i=1; i<nEqns; i++) {
        intersectCone(*list[1-workingList], *list[workingList], 
            NVectorMatrixRow<NLargeInteger>(subspace, i), embeddedOnly);
        list[workingList]->flushAndDelete();
        workingList = 1 - workingList;
    }
    
    // We're done!
    delete list[1-workingList];
    return list[workingList];
}

