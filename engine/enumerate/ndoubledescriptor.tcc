
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

#include <algorithm>
#include "regina-config.h"
#include "enumerate/nenumconstraint.h"
#include "maths/matrixops.h"
#include "maths/nvectormatrix.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstypes.h"
#include "utilities/boostutils.h"
#include "utilities/memutils.h"
#include "utilities/nbitmask.h"

namespace regina {

template <class BitmaskType>
NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(
        const RaySpec<BitmaskType>& first,
        const RaySpec<BitmaskType>& second,
        const NVector<NLargeInteger>& hyperplane) :
        NFastVector<NLargeInteger>(second), facets_(second.facets_) {
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

    // Compute the new set of facets.
    facets_ &= first.facets_;
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

template <class OutputIterator, class RayIterator, class FacetIterator>
void NDoubleDescriptor::enumerateExtremalRays(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FacetIterator facetsFirst, FacetIterator facetsLast,
        const NMatrixInt& subspace,
        const NEnumConstraintList* constraints,
        NProgressNumber* progress) {
    unsigned nFacets = std::distance(facetsFirst, facetsLast);

    if (nFacets == 0) {
        // Something is wrong.. every one of the initial rays should
        // belong to at least one facet.  Return no results.
        return;
    }

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask1<unsigned> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask1<unsigned long> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
#ifdef HAVE_LONG_LONG
    else if (nFacets <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask1<unsigned long long> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned long> >(
            results, oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask2<unsigned long long> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
#else
    else if (nFacets <= 8 * sizeof(unsigned long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask2<unsigned long, unsigned> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
    else if (nFacets <= 16 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask2<unsigned long> >(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
#endif
    else
        enumerateUsingBitmask<NBitmask>(results,
            oldRaysFirst, oldRaysLast, facetsFirst, facetsLast,
            subspace, constraints, progress);
}

template <class BitmaskType,
          class OutputIterator, class RayIterator, class FacetIterator>
void NDoubleDescriptor::enumerateUsingBitmask(OutputIterator results,
        RayIterator oldRaysFirst, RayIterator oldRaysLast,
        FacetIterator facetsFirst, FacetIterator facetsLast,
        const NMatrixInt& subspace,
        const NEnumConstraintList* constraints,
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
    typedef std::vector<RaySpec<BitmaskType>*> RaySpecList;
    RaySpecList list[2];

    for (RayIterator rit = oldRaysFirst; rit != oldRaysLast; ++rit)
        list[0].push_back(new RaySpec<BitmaskType>(
            **rit, facetsFirst, facetsLast));

    // Convert the set of constraints into a bitmask, where for every original
    // facet listed in the constraint the corresponding bit is set to 1.
    BitmaskType* constraintsBegin = 0;
    BitmaskType* constraintsEnd = 0;
    if (constraints && ! constraints->empty()) {
        constraintsBegin = new BitmaskType[constraints->size()];

        unsigned nFacets = std::distance(facetsFirst, facetsLast);
        NEnumConstraintList::const_iterator cit;
        std::set<unsigned>::const_iterator sit;
        for (cit = constraints->begin(), constraintsEnd = constraintsBegin;
                cit != constraints->end(); ++cit, ++constraintsEnd) {
            constraintsEnd->reset(nFacets);
            constraintsEnd->set(cit->begin(), cit->end(), true);
        }
    }

    // We want to process the hyperplanes in a good order;
    // Fukuda and Prodon (1996) recommend this, and experimental
    // experience with Regina agrees.  The ordering we use here is
    // described in the notes for the inner class NDoubleDescriptor::LexComp.
    //
    // Sort the integers 0..(nEqns-1) into the order in which we plan to
    // process the hyperplanes.
    int* hyperplanes = new int[nEqns];
    unsigned i;
    for (i = 0; i < nEqns; ++i)
        hyperplanes[i] = i;

    std::sort(hyperplanes, hyperplanes + nEqns,
        LexComp<FacetIterator>(subspace, facetsFirst, facetsLast));

    // Intersect the hyperplanes one at a time.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    int workingList = 0;
    for (i=0; i<nEqns; i++) {
        intersectHyperplane(list[workingList], list[1 - workingList],
            NVectorMatrixRow<NLargeInteger>(subspace, hyperplanes[i]), i,
            constraintsBegin, constraintsEnd);

        workingList = 1 - workingList;

        if (progress) {
            progress->incCompleted();
            if (progress->isCancelled())
                break;
        }
    }

    // We're done!
    delete[] hyperplanes;
    if (constraintsBegin)
        delete[] constraintsBegin;

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
        std::vector<RaySpec<BitmaskType>*>& src,
        std::vector<RaySpec<BitmaskType>*>& dest,
        const NVector<NLargeInteger>& hyperplane,
        unsigned prevHyperplanes,
        const BitmaskType* constraintsBegin,
        const BitmaskType* constraintsEnd) {
    if (src.empty())
        return;

    typedef std::vector<RaySpec<BitmaskType>*> RayList;
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

    const BitmaskType* constraint;
    bool adjacent, broken;
    for (posit = pos.begin(); posit != pos.end(); ++posit)
        for (negit = neg.begin(); negit != neg.end(); ++negit) {
            BitmaskType join((*posit)->facets());
            join &= ((*negit)->facets());

            // We only care about adjacent rays, i.e., rays joined by an edge.
            // For the rays to be adjacent, the number of original facets that
            // both belong to must be >= dimension(subspace) - 2,
            // which is >= hyperplane.size() - prevHyperplanes - 2.
            // See Fukuda and Prodon (1996), Proposition 9.
            if (join.bits() + prevHyperplanes + 2 < hyperplane.size())
                continue;

            // Are we supposed to check for compatibility?
            if (constraintsBegin) {
                join.flip();

                broken = false;
                for (constraint = constraintsBegin;
                        constraint != constraintsEnd; ++constraint) {
                    BitmaskType mask(join);
                    mask &= *constraint;
                    if (! mask.atMostOneBit()) {
                        broken = true;
                        break;
                    }
                }
                if (broken)
                    continue;
            }

            // Two rays are joined by an edge if and only if there is no
            // other ray belonging to all of their common facets.
            adjacent = true;
            for (otherit = src.begin(); otherit != src.end(); ++otherit)
                if (*otherit != *posit && *otherit != *negit &&
                        (*otherit)->onAllCommonFacets(**posit, **negit)) {
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

