
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/* To be included from ndoubledescriptor.h. */

#include <iterator>
#include <list>
#include <vector>
#include "enumerate/ncompconstraint.h"
#include "maths/nvectormatrix.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstypes.h"
#include "utilities/boostutils.h"
#include "utilities/memutils.h"

namespace regina {

template <class OutputIterator, class RayIterator, class FaceIterator>
void NDoubleDescriptor::enumerateVertices(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NMatrixInt& subspace, const NCompConstraintSet* constraints,
        NProgressNumber* progress) const {
    typedef typename std::iterator_traits<RayIterator>::value_type RayClassPtr;
    typedef typename regina::boost::remove_pointer<RayClassPtr>::type RayClass;

    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // There are no hyperplanes in the subspace!
        // We will have to clone the list of extremal rays.
        if (progress)
            progress->setOutOf(progress->getOutOf() + 1);

        transform(oldRaysFirst, oldRaysLast, results,
            FuncNewClonePtr<RayClass>());

        if (progress)
            progress->incCompleted();
        return;
    }

    if (progress)
        progress->setOutOf(progress->getOutOf() + nEqns);

    // Create the two vector lists with which we will work.
    // Fill the first list with the intersection with the first
    // hyperplane.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    std::list<RayClassPtr> list[2];
    std::back_insert_iterator<std::list<RayClassPtr> > inserter[] =
        { back_inserter(list[0]), back_inserter(list[1]) };
    int workingList = 0;
    enumerateVertices(inserter[0], oldRaysFirst, oldRaysLast,
        facesFirst, facesLast, NVectorMatrixRow<NLargeInteger>(subspace, 0),
        constraints);

    if (progress) {
        progress->incCompleted();
        if (progress->isCancelled()) {
            copy(list[workingList].begin(), list[workingList].end(), results);
            return;
        }
    }

    // Now run around intersecting each extra hyperplane as it comes.
    FuncDelete<RayClass> funcDelete;
    for (unsigned i=1; i<nEqns; i++) {
        enumerateVertices(inserter[1-workingList], list[workingList].begin(),
            list[workingList].end(), facesFirst, facesLast,
            NVectorMatrixRow<NLargeInteger>(subspace, i), constraints);
        for_each(list[workingList].begin(), list[workingList].end(),
            funcDelete);
        list[workingList].clear();
        workingList = 1 - workingList;

        if (progress) {
            progress->incCompleted();
            if (progress->isCancelled()) {
                copy(list[workingList].begin(), list[workingList].end(),
                    results);
                return;
            }
        }
    }

    // We're done!
    copy(list[workingList].begin(), list[workingList].end(), results);
}

template <class OutputIterator, class RayIterator, class FaceIterator>
void NDoubleDescriptor::enumerateVertices(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NVector<NLargeInteger>& hyperplane,
        const NCompConstraintSet* constraints) const {
    if (oldRaysFirst == oldRaysLast)
        return;

    typedef typename std::iterator_traits<RayIterator>::value_type RayClassPtr;

    // Establish the number of faces.
    typename std::iterator_traits<FaceIterator>::difference_type nFaces =
        std::distance(facesFirst, facesLast);

    // The following table will store which rays lie on which faces.
    // We'll begin by reserving enough space for the entire table so
    // iterators won't be invalidated as the table is filled in.
    typedef std::vector<bool> RayFaceTable;
    typedef typename RayFaceTable::iterator RayFaceTableIterator;
    RayFaceTable rayOnFace(std::distance(oldRaysFirst, oldRaysLast) * nFaces);

    // Each of the following lists stores a ray plus the beginning
    // of the block in table rayOnFace corresponding to that
    // particular ray.
    typedef std::list<std::pair<RayClassPtr, RayFaceTableIterator> >
        ExtendedRayList;
    typedef typename ExtendedRayList::const_iterator ExtendedRayListIterator;
    ExtendedRayList pos, neg;

    // Run through the old rays and determine which side of the
    // hyperplane they lie on.
    // Also precalculate which rays lie on which faces.
    // Rays lying within the hyperplane will be added to the new
    // solution set.
    RayIterator it;
    FaceIterator faceIt;
    RayFaceTableIterator rayFaceIt = rayOnFace.begin();
    NLargeInteger dot;
    for (it = oldRaysFirst; it != oldRaysLast; it++) {
        dot = hyperplane * (**it);
        if (dot == NRay::zero)
            *results++ = dynamic_cast<RayClassPtr>((*it)->clone());
        else if (dot < NRay::zero)
            neg.push_back(make_pair(*it, rayFaceIt));
        else
            pos.push_back(make_pair(*it, rayFaceIt));

        for (faceIt = facesFirst; faceIt != facesLast; faceIt++)
            *rayFaceIt++ = ((**faceIt) * (**it) == 0);
    }

    // Run through the pairs of positive and negative rays.
    // Rays that are joined by an edge will result in a new ray
    // being added to the solution set.
    // One can prove that no ray will ever have been added to the
    // solution set twice.
    ExtendedRayListIterator posit, negit;
    RayFaceTableIterator posFaceIt, negFaceIt, baseFaceIt;
    bool adjacent, hasCommonFaces;
    for (posit = pos.begin(); posit != pos.end(); posit++)
        for (negit = neg.begin(); negit != neg.end(); negit++) {
            // Are we supposed to check for compatibility?
            if (constraints)
                if (! constraints->isSatisfied(*(*posit).first,
                        *(*negit).first))
                    continue;

            // Two rays are adjacent if and only if there is no
            // other ray belonging to all of their common faces.
            adjacent = true;
            baseFaceIt = rayOnFace.begin();
            for (it = oldRaysFirst; it != oldRaysLast; it++) {
                if (*it != (*posit).first && *it != (*negit).first) {
                    hasCommonFaces = true;

                    posFaceIt = (*posit).second;
                    negFaceIt = (*negit).second;
                    rayFaceIt = baseFaceIt;
                    for (faceIt = facesFirst; faceIt != facesLast; faceIt++) {
                        if ((*posFaceIt) && (*negFaceIt) && ! (*rayFaceIt)) {
                            hasCommonFaces = false;
                            break;
                        }
                        posFaceIt++;
                        negFaceIt++;
                        rayFaceIt++;
                    }

                    if (hasCommonFaces) {
                        adjacent = false;
                        break;
                    }
                }

                baseFaceIt += nFaces;
            }

            // If the rays are adjacent then join them and put the
            // corresponding intersection with the hyperplane in the
            // results set.
            if (adjacent)
                *results++ = dynamic_cast<RayClassPtr>(intersect(
                    *(*posit).first, *(*negit).first, hyperplane));
        }
}

} // namespace regina

