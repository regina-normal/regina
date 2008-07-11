
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/* To be included from ndoubledescriptor.h. */

#include "enumerate/ncompconstraint.h"
#include "maths/nvectormatrix.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstypes.h"
#include "utilities/boostutils.h"
#include "utilities/nbitmask.h"

namespace regina {

template <class BitmaskType>
NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(
        const RaySpec<BitmaskType>& first,
        const RaySpec<BitmaskType>& second,
        const NVector<NLargeInteger>& hyperplane) :
        NFastVector<NLargeInteger>(second), faces(second.faces) {
    // We are currently a perfect clone of second.
    // Convert ourselves into
    // (hyperplane * first) second - (hyperplane * second) first.
    // Negate this if (hyperplane * first) is negative.

    NLargeInteger firstCoeff(first * hyperplane);
    (*this) *= firstCoeff;
    subtractCopies(first, second * hyperplane);
    scaleDown();

    if (firstCoeff < zero)
        negate();

    // Compute the new set of faces.
    faces &= first.faces;
}

template <class BitmaskType>
void NDoubleDescriptor::RaySpec<BitmaskType>::scaleDown() {
    NLargeInteger gcd; // Initialised to 0.
    NLargeInteger* e;
    for (e = elements; e < end; ++e) {
        if (e->isInfinite() || (*e) == zero)
            continue;
        gcd = gcd.gcd(*e);
        if (gcd < 0)
            gcd.negate();
        if (gcd == one)
            return;
    }
    if (gcd == zero)
        return;
    for (e = elements; e < end; ++e)
        if ((! e->isInfinite()) && (*e) != zero)
            e->divByExact(gcd);
}

template <class OutputIterator, class RayIterator, class FaceIterator>
void NDoubleDescriptor::enumerateVertices(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NMatrixInt& subspace, const NCompConstraintSet* constraints,
        NProgressNumber* progress) {
    unsigned nFaces = std::distance(facesFirst, facesLast);

    if (nFaces == 0) {
        // Something is wrong.. every one of the initial rays should
        // belong to at least one face.  Return no results.
        return;
    }

    // Choose a bitmask type for representing the set of faces that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (nFaces <= sizeof(unsigned))
        enumerateUsingBitmask<NBitmask1<unsigned> >(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else if (nFaces <= sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask1<unsigned long> >(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else if (nFaces <= sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask1<unsigned long long> >(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else if (nFaces <= sizeof(unsigned long long) + sizeof(unsigned))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned> >(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else if (nFaces <= sizeof(unsigned long long) + sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned long> >(
            results, oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else if (nFaces <= 2 * sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask2<unsigned long long> >(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
    else
        enumerateUsingBitmask<NBitmask>(results,
            oldRaysFirst, oldRaysLast, facesFirst, facesLast,
            subspace, constraints, progress);
}

template <class BitmaskType,
          class OutputIterator, class RayIterator, class FaceIterator>
void NDoubleDescriptor::enumerateUsingBitmask(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FaceIterator facesFirst, FaceIterator facesLast,
        const NMatrixInt& subspace, const NCompConstraintSet* constraints,
        NProgressNumber* progress) {
    typedef typename std::iterator_traits<RayIterator>::value_type RayClassPtr;
    typedef typename regina::boost::remove_pointer<RayClassPtr>::type RayClass;

    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // There are no hyperplanes in the subspace!
        // We can just clone the list of extremal rays.
        if (progress)
            progress->setOutOf(progress->getOutOf() + 1);

        transform(oldRaysFirst, oldRaysLast, results,
            FuncNewClonePtr<RayClass>());

        if (progress)
            progress->incCompleted();
        return;
    }

    // We actually have some work to do.
    if (progress)
        progress->setOutOf(progress->getOutOf() + nEqns);

    // Create the two vector lists with which we will work.
    // Fill the first with the initial set of rays.
    typedef std::list<RaySpec<BitmaskType>*> RaySpecList;
    RaySpecList list[2];

    FaceIterator fit;
    for (RayIterator rit = oldRaysFirst; rit != oldRaysLast; ++rit)
        list[0].push_back(new RaySpec<BitmaskType>(
            **rit, facesFirst, facesLast));

    // Intersect the hyperplanes one at a time.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    int workingList = 0;
    unsigned i;
    for (i=0; i<nEqns; i++) {
        intersectHyperplane(list[workingList], list[1 - workingList],
            NVectorMatrixRow<NLargeInteger>(subspace, i), constraints);

        workingList = 1 - workingList;

        if (progress) {
            progress->incCompleted();
            if (progress->isCancelled())
                break;
        }
    }

    // We're done!
    // Use the first input ray as a factory for creating output rays of
    // the correct class.
    RayClass* ans;
    for (typename RaySpecList::iterator it = list[workingList].begin();
            it != list[workingList].end(); ++it) {
        ans = static_cast<RayClass*>((*oldRaysFirst)->clone());
        for (i = 0; i < ans->size(); ++i)
            ans->setElement(i, (**it)[i]);
        *results++ = ans;

        delete *it;
    }
}

template <class BitmaskType>
void NDoubleDescriptor::intersectHyperplane(
        std::list<RaySpec<BitmaskType>*>& src,
        std::list<RaySpec<BitmaskType>*>& dest,
        const NVector<NLargeInteger>& hyperplane,
        const NCompConstraintSet* constraints) {
    if (src.empty())
        return;

    typedef std::list<RaySpec<BitmaskType>*> RayList;
    RayList pos, neg;

    // Run through the old rays and determine which side of the
    // hyperplane they lie on.
    // Rays lying within the hyperplane will be added directly to the new
    // solution set.
    typename RayList::iterator it;
    NLargeInteger dot;
    for (it = src.begin(); it != src.end(); it++) {
        dot = (**it) * hyperplane;
        if (dot == NRay::zero)
            dest.push_back(*it);
        else if (dot < NRay::zero)
            neg.push_back(*it);
        else
            pos.push_back(*it);
    }

    // At this point every ray is owned by pos, neg, or dest.
    // When we are finished with the src list we can just empty it out.

    // Run through the pairs of positive and negative rays.  For every
    // pair of rays that are adjacent in the current solution space,
    // add the corresponding intersection with the hyperplane to the
    // new solution set.
    typename RayList::iterator posit, negit, otherit;

    bool adjacent;
    for (posit = pos.begin(); posit != pos.end(); ++posit)
        for (negit = neg.begin(); negit != neg.end(); ++negit) {
            // Are we supposed to check for compatibility?
            if (constraints)
                if (! constraints->isSatisfied(**posit, **negit))
                    continue;

            // Two rays are joined by an edge if and only if there is no
            // other ray belonging to all of their common faces.
            adjacent = true;
            for (otherit = src.begin(); otherit != src.end(); ++otherit)
                if (*otherit != *posit && *otherit != *negit &&
                        (*otherit)->onAllCommonFaces(**posit, **negit)) {
                    adjacent = false;
                    break;
                }

            // If the rays are adjacent then join them and put the
            // corresponding intersection in the results set.
            if (adjacent)
                dest.push_back(new RaySpec<BitmaskType>(
                    **posit, **negit, hyperplane));
        }

    // Clean up.
    for (posit = pos.begin(); posit != pos.end(); ++posit)
        delete *posit;
    for (negit = neg.begin(); negit != neg.end(); ++negit)
        delete *negit;
    src.clear();
}

} // namespace regina

