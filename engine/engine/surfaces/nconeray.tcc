
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/* To be included from nconeray.h. */

#include <list>
#include "maths/nvectormatrix.h"
#include "maths/nmatrixint.h"
#include "utilities/nmiscutils.h"

template <class OutputIterator, class RayIterator, class FaceIterator>
void intersectCone(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NConeRay& hyperplane, bool testCompatibility) {
    if (oldRaysFirst == oldRaysLast)
        return;

    std::list<NConeRay*> pos;
    std::list<NConeRay*> neg;

    // Run through the old rays and determine which side of the
    // hyperplane they lie on.
    // Rays lying within the hyperplane will be added to the new
    // solution set.
    RayIterator it;
    NLargeInteger dot;
    for (it = oldRaysFirst; it != oldRaysLast; it++) {
        dot = hyperplane * (**it);
        if (dot == NConeRay::zero)
            *results++ = (NConeRay*)(*it)->clone();
        else if (dot < NConeRay::zero)
            neg.push_back(*it);
        else
            pos.push_back(*it);
    }

    // Run through the pairs of positive and negative rays.
    // Rays that are joined by an edge will result in a new ray
    // being added to the solution set.
    // One can prove that no ray will ever have been added to the
    // solution set twice.
    std::list<NConeRay*>::const_iterator posit, negit;
    FaceIterator faceit;
    NVector<NLargeInteger>* face;
    bool adjacent, hasCommonFaces;
    for (posit = pos.begin(); posit != pos.end(); posit++)
        for (negit = neg.begin(); negit != neg.end(); negit++) {
            // Are we supposed to check for compatibility?
            if (testCompatibility)
                if (! (*posit)->isCompatibleWith(**negit))
                    continue;

            // Two rays are adjacent if and only if there is no
            // other ray belonging to all of their common faces.
            adjacent = true;
            for (it = oldRaysFirst; it != oldRaysLast; it++) {
                if (*it == *posit || *it == *negit)
                    continue;
                hasCommonFaces = true;
                for (faceit = facesFirst; faceit != facesLast; faceit++) {
                    face = *faceit;
                    if ((*face) * (**posit) == 0 && (*face) * (**negit) == 0 &&
                            (*face) * (**it) != 0) {
                        hasCommonFaces = false;
                        break;
                    }
                }
                if (hasCommonFaces) {
                    adjacent = false;
                    break;
                }
            }

            // If the rays are adjacent then join them and put the
            // corresponding intersection with the hyperplane in the
            // results set.
            if (adjacent)
                *results++ = intersectLine(**posit, **negit, hyperplane);
        }
}

template <class OutputIterator, class RayIterator, class FaceIterator>
void intersectCone(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NMatrixInt& subspace, bool testCompatibility) {
    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // There are no hyperplanes in the subspace!
        // We will have to clone the list of extremal rays.
        transform(oldRaysFirst, oldRaysLast, results,
            FuncNewClonePtr<NConeRay>());
        return;
    }
    
    // Create the two vector lists with which we will work.
    // Fill the first list with the intersection with the first
    // hyperplane.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    std::list<NConeRay*> list[2];
    std::back_insert_iterator<std::list<NConeRay*> > inserter[] =
        { back_inserter(list[0]), back_inserter(list[1]) };
    int workingList = 0;
    intersectCone(inserter[0], oldRaysFirst, oldRaysLast,
        facesFirst, facesLast, NVectorMatrixRow<NLargeInteger>(subspace, 0),
        testCompatibility);
    
    // Now run around intersecting each extra hyperplane as it comes.
    FuncDelete<NConeRay> funcDelete;
    for (unsigned i=1; i<nEqns; i++) {
        intersectCone(inserter[1-workingList], list[workingList].begin(),
            list[workingList].end(), facesFirst, facesLast,
            NVectorMatrixRow<NLargeInteger>(subspace, i), testCompatibility);
        for_each(list[workingList].begin(), list[workingList].end(),
            funcDelete);
        list[workingList].clear();
        workingList = 1 - workingList;
    }
    
    // We're done!
    copy(list[workingList].begin(), list[workingList].end(), results);
}

