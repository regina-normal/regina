
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
#include <iterator>
#include "regina-config.h"
#include "enumerate/nenumconstraint.h"
#include "maths/matrixops.h"
#include "maths/nray.h"
#include "progress/nprogresstypes.h"
#include "utilities/boostutils.h"
#include "utilities/memutils.h"
#include "utilities/nbitmask.h"

namespace regina {

template <class BitmaskType>
NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(unsigned axis,
        const NMatrixInt& subspace, const int* hypOrder) :
        NFastRay(subspace.rows()),
        facets_(subspace.columns()) {
    unsigned i;

    for (i = 0; i < subspace.columns(); ++i)
        if (i != axis)
            facets_.set(i, true);

    for (i = 0; i < size(); ++i)
        elements[i] = subspace.entry(hypOrder[i], axis);
}

template <class BitmaskType>
NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(
        const RaySpec<BitmaskType>& first,
        const RaySpec<BitmaskType>& second) :
        NFastRay(second.size() - 1),
        facets_(second.facets_) {
    for (unsigned i = 0; i < size(); ++i)
        elements[i] = second.elements[i + 1] * (*first.elements) -
            first.elements[i + 1] * (*second.elements);
    scaleDown();

    if (*first.elements < zero)
        negate();

    // Compute the new set of facets.
    facets_ &= first.facets_;
}

template <class BitmaskType>
void NDoubleDescriptor::RaySpec<BitmaskType>::recover(
        NRay& dest, const NMatrixInt& subspace) const {
    unsigned i, j;

    unsigned rows = subspace.rows();
    unsigned cols = subspace.columns() - facets_.bits();

    // Extract the set of columns that we actually care about.
    unsigned* use = new unsigned[cols];
    for (i = 0, j = 0; i < subspace.columns(); ++i)
        if (facets_.get(i)) {
            // We know in advance that this coordinate will be zero.
            dest.setElement(i, NLargeInteger::zero);
        } else {
            use[j++] = i;
        }

    // We know the solution space has dimension one.
    // If there are no equations then there must be only one non-zero
    // coordinate, and vice versa.
    if (cols == 1) {
        dest.setElement(*use, 1);
        delete[] use;
        return;
    }

    // We have several non-zero coordinates with at least one
    // non-trivial equation relating them.

    // Form a submatrix for the equations, looking only at non-zero coordinates.
    NLargeInteger* m = new NLargeInteger[rows * cols];
    for (i = 0; i < rows; ++i)
        for (j = 0; j < cols; ++j)
            m[i * cols + j] = subspace.entry(i, use[j]);

    // Put this submatrix in echelon form; moreover, for the leading
    // entry in each row, set all other entries in the corresponding
    // column to zero.
    unsigned* lead = new unsigned[cols];
    for (i = 0; i < cols; ++i)
        lead[i] = i;

    // More or less a stripped-down copy of rowBasisAndOrthComp() from here.
    // See rowBasisAndOrthComp() for further details on how this works.
    unsigned done = 0;
    unsigned tmp;
    NLargeInteger coeff1, coeff2, common;
    while (done < rows) {
        // Find the first non-zero entry in row done.
        for (i = done; i < cols; ++i)
            if (m[done * cols + lead[i]] != NLargeInteger::zero)
                break;

        if (i == cols) {
            // We have a zero row.  Trash it and replace it with something else.
            --rows;
            if (done < rows) {
                for (j = 0; j < cols; ++j)
                    m[done * cols + j] = m[rows * cols + j];
            }
        } else {
            // We have a non-zero row.
            // Save the column in which we found our non-zero entry.
            tmp = lead[done]; lead[done] = lead[i]; lead[i] = tmp;

            // Make all other entries in column lead[done] equal to zero.
            coeff1 = m[done * cols + lead[done]];
            for (i = 0; i < rows; ++i) {
                if (i == done)
                    continue;

                coeff2 = m[i * cols + lead[done]];
                common = NLargeInteger::zero;
                if (coeff2 != NLargeInteger::zero) {
                    for (j = 0; j < cols; ++j) {
                        m[i * cols + j] = m[i * cols + j] * coeff1
                            - m[done * cols + j] * coeff2;
                        common = common.gcd(m[i * cols + j]);
                    }
                }
                if (common < NLargeInteger::zero)
                    common.negate();
                if (common > NLargeInteger::one)
                    for (j = 0; j < cols; ++j)
                        m[i * cols + j].divByExact(common);
            }
            ++done;
        }
    }

    // At this point we should have rows == (cols - 1), and the column
    // that is *not* zeroed out almost everywhere is lead[rows].  (We
    // know this because we know that our solution must be one-dimensional).
    //
    // Form a solution!
    common = NLargeInteger::one;
    for (i = 0; i < rows; ++i)
        common = common.lcm(m[i * cols + lead[i]]);
    if (common < NLargeInteger::zero)
        common.negate();

    for (i = 0; i < rows; ++i)
        dest.setElement(use[lead[i]], - (common * m[i * cols + lead[rows]]).
            divExact(m[i * cols + lead[i]]));
    dest.setElement(use[lead[rows]], common);

    dest.scaleDown();

    // All done!
    delete[] lead;
    delete[] m;
    delete[] use;
}

template <class OutputIterator>
void NDoubleDescriptor::enumerateExtremalRays(OutputIterator results,
        const NRay& rayBase, const NMatrixInt& subspace,
        const NEnumConstraintList* constraints, NProgressNumber* progress) {
    unsigned nFacets = subspace.columns();

    // If the space has dimension zero, return no results.
    if (nFacets == 0)
        return;

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask1<unsigned> >(results,
            rayBase, subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask1<unsigned long> >(results,
            rayBase, subspace, constraints, progress);
#ifdef HAVE_LONG_LONG
    else if (nFacets <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask1<unsigned long long> >(results,
            rayBase, subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned> >(results,
            rayBase, subspace, constraints, progress);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask2<unsigned long long, unsigned long> >(
            results, rayBase, subspace, constraints, progress);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<NBitmask2<unsigned long long> >(results,
            rayBase, subspace, constraints, progress);
#else
    else if (nFacets <= 8 * sizeof(unsigned long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<NBitmask2<unsigned long, unsigned> >(results,
            rayBase, subspace, constraints, progress);
    else if (nFacets <= 16 * sizeof(unsigned long))
        enumerateUsingBitmask<NBitmask2<unsigned long> >(results,
            rayBase, subspace, constraints, progress);
#endif
    else
        enumerateUsingBitmask<NBitmask>(results,
            rayBase, subspace, constraints, progress);
}

template <class BitmaskType, class OutputIterator>
void NDoubleDescriptor::enumerateUsingBitmask(OutputIterator results,
        const NRay& rayBase, const NMatrixInt& subspace,
        const NEnumConstraintList* constraints,
        NProgressNumber* progress) {
    typedef typename std::iterator_traits<OutputIterator>::value_type
        RayClassPtr;

    // Get the dimension of the entire space in which we are working.
    unsigned dim = subspace.columns();

    // Are there any hyperplanes at all in the subspace?
    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // No!  Just send back the vertices of the non-negative orthant.
        if (progress)
            progress->setOutOf(progress->getOutOf() + 1);

        RayClassPtr ans;
        for (unsigned i = 0; i < dim; ++i) {
            ans = static_cast<RayClassPtr>(rayBase.clone());
            ans->setElement(i, NLargeInteger::one);
            *results++ = ans;
        }

        if (progress)
            progress->incCompleted();
        return;
    }

    // We actually have some work to do.
    if (progress)
        progress->setOutOf(progress->getOutOf() + nEqns + 1);

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

    std::sort(hyperplanes, hyperplanes + nEqns, LexComp(subspace));

    // Create the two vector lists with which we will work.
    // Fill the first with the initial set of rays.
    typedef std::vector<RaySpec<BitmaskType>*> RaySpecList;
    RaySpecList list[2];

    for (i = 0; i < dim; ++i)
        list[0].push_back(new RaySpec<BitmaskType>(i, subspace, hyperplanes));

    // Convert the set of constraints into a bitmask, where for every original
    // facet listed in the constraint the corresponding bit is set to 1.
    BitmaskType* constraintsBegin = 0;
    BitmaskType* constraintsEnd = 0;
    if (constraints && ! constraints->empty()) {
        constraintsBegin = new BitmaskType[constraints->size()];

        NEnumConstraintList::const_iterator cit;
        for (cit = constraints->begin(), constraintsEnd = constraintsBegin;
                cit != constraints->end(); ++cit, ++constraintsEnd) {
            constraintsEnd->reset(dim);
            constraintsEnd->set(cit->begin(), cit->end(), true);
        }
    }

    // Intersect the hyperplanes one at a time.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    int workingList = 0;
    unsigned used = 0;
    for (i=0; i<nEqns; i++) {
        // Do not increment used if the old solution set sits entirely in
        // and/or to only one side of the new hyperplane.  This gives the
        // dimensional filtering in intersectHyperplane() greater strength.
        // The reason this works is because any vertex of the solution
        // space *with* this hyperplane is also a vertex of the solution
        // space *without* this hyperplane (and therefore satisfies the
        // relevant dimensional constraints without this hyperplane).
        if (intersectHyperplane(list[workingList], list[1 - workingList],
                dim, used, constraintsBegin, constraintsEnd))
            ++used;

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
    RayClassPtr ans;
    for (typename RaySpecList::iterator it = list[workingList].begin();
            it != list[workingList].end(); ++it) {
        ans = static_cast<RayClassPtr>(rayBase.clone());
        (*it)->recover(*ans, subspace);
        *results++ = ans;

        delete *it;
    }

    // All done!
    if (progress)
        progress->incCompleted();
}

template <class BitmaskType>
bool NDoubleDescriptor::intersectHyperplane(
        std::vector<RaySpec<BitmaskType>*>& src,
        std::vector<RaySpec<BitmaskType>*>& dest,
        unsigned dim, unsigned prevHyperplanes,
        const BitmaskType* constraintsBegin,
        const BitmaskType* constraintsEnd) {
    if (src.empty())
        return false;

    typedef std::vector<RaySpec<BitmaskType>*> RayList;
    RayList pos, neg;

    // Run through the old rays and determine which side of the
    // new hyperplane they lie on.
    // Rays lying within the new hyperplane will be added directly to
    // the new solution set.
    typename RayList::iterator it;
    int sign;
    for (it = src.begin(); it != src.end(); it++) {
        sign = (*it)->sign();
        if (sign == 0)
            dest.push_back(new RaySpec<BitmaskType>(**it));
        else if (sign < 0)
            neg.push_back(*it);
        else
            pos.push_back(*it);
    }

    // Does one of the closed half-spaces defined by the hyperplane contain the
    // entire old solution set?  If so, there will be no new vertices.
    if (pos.empty() || neg.empty()) {
        for (typename RayList::iterator it = src.begin(); it != src.end(); ++it)
            delete *it;
        src.clear();
        return false;
    }

    // Run through the pairs of positive and negative rays.  For every
    // pair of rays that are adjacent in the current solution space,
    // add the corresponding intersection with the new hyperplane to the
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
            // which is >= dimension(entire space) - prevHyperplanes - 2.
            // See Fukuda and Prodon (1996), Proposition 9.
            if (join.bits() + prevHyperplanes + 2 < dim)
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
                dest.push_back(new RaySpec<BitmaskType>(**posit, **negit));
        }

    // Clean up.
    for (otherit = src.begin(); otherit != src.end(); ++otherit)
        delete *otherit;
    src.clear();
    return true;
}

} // namespace regina

