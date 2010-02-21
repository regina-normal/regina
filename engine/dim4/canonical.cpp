
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

namespace regina {

namespace {
    /**
     * For internal use by makeCanonical().  This routines assumes that
     * the preimage of pentachronn 0 has been fixed (along with the
     * corresponding facet permutation), and tries to extend
     * this to a "possibly canonical" isomorphism.
     *
     * If it becomes clear that the isomorphism cannot be made canonical
     * and/or cannot be made better (i.e., lexicographically smaller) than
     * the best isomorphism found so far, this routine returns \c false
     * (possibly before the full isomorphism has been constructed).
     * Otherwise it returns \c true (and it is guaranteed that the
     * isomorphism is both fully constructed and moreover a strict
     * improvement upon the best found so far).
     *
     * This routine currently only works for connected triangulations.
     */
    bool extendIsomorphism(const Dim4Triangulation* tri,
            Dim4Isomorphism& current, Dim4Isomorphism& currentInv,
            const Dim4Isomorphism& best, const Dim4Isomorphism& bestInv) {
        bool better = false;

        unsigned nPents = tri->getNumberOfPentachora();
        unsigned pent;

        for (pent = 0; pent < nPents; ++pent)
            if (pent != currentInv.pentImage(0))
                current.pentImage(pent) = -1;

        int facet;

        unsigned origPent, origPentBest;
        int origFacet, origFacetBest;

        Dim4Pentachoron *adjPent, *adjPentBest;
        unsigned adjPentIndex, adjPentIndexBest;

        NPerm5 gluingPerm, gluingPermBest;
        NPerm5 finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        unsigned lastAssigned = 0;
        for (pent = 0; pent < nPents; ++pent) {
            // INV: We have already selected the preimage of pent and
            // the corresponding facet permutation by the time we reach
            // this point.
            origPent = currentInv.pentImage(pent);
            origPentBest = bestInv.pentImage(pent);

            for (facet = 0; facet < 5; ++facet) {
                origFacet = current.facetPerm(origPent).preImageOf(facet);
                origFacetBest = best.facetPerm(origPentBest).preImageOf(facet);

                // Check out the adjacency along pent/facet.
                adjPent = tri->getPentachoron(origPent)->
                    adjacentPentachoron(origFacet);
                adjPentIndex = (adjPent ?
                    tri->pentachoronIndex(adjPent) : nPents);
                adjPentBest = tri->getPentachoron(origPentBest)->
                    adjacentPentachoron(origFacetBest);
                adjPentIndexBest = (adjPentBest ?
                    tri->pentachoronIndex(adjPentBest) : nPents);

                justAssigned = false;
                if (adjPent && current.pentImage(adjPentIndex) < 0) {
                    // We have a new pentachoron that needs assignment.
                    ++lastAssigned;
                    current.pentImage(adjPentIndex) = lastAssigned;
                    currentInv.pentImage(lastAssigned) = adjPentIndex;
                    justAssigned = true;
                }

                // We now have a gluing (but possibly not a gluing
                // permutation).  Compare adjacent pentachoron indices.
                if ((! better) && current.pentImage(adjPentIndex) >
                        best.pentImage(adjPentIndexBest))
                    return false; // Worse than best-so-far.
                if (current.pentImage(adjPentIndex) <
                        best.pentImage(adjPentIndexBest))
                    better = true;

                // Time now to look at the gluing permutation.
                if (! adjPent)
                    continue;

                gluingPerm = tri->getPentachoron(origPent)->
                    adjacentGluing(origFacet);
                gluingPermBest = tri->getPentachoron(origPentBest)->
                    adjacentGluing(origFacetBest);

                if (justAssigned) {
                    // We can choose the permutation ourselves.
                    // Make it so that the final gluing (computed later
                    // below) becomes the identity.
                    current.facetPerm(adjPentIndex) =
                        current.facetPerm(origPent) * gluingPerm.inverse();
                    currentInv.facetPerm(lastAssigned) =
                        current.facetPerm(adjPentIndex).inverse();
                }

                finalGluing = current.facetPerm(adjPentIndex) *
                    gluingPerm * current.facetPerm(origPent).inverse();
                finalGluingBest = best.facetPerm(adjPentIndexBest) *
                    gluingPermBest * best.facetPerm(origPentBest).inverse();

                comp = finalGluing.compareWith(finalGluingBest);
                if ((! better) && comp > 0)
                    return false; // Worse than best-so-far.
                if (comp < 0)
                    better = true;
            }
        }

        return better;
    }
}

bool Dim4Triangulation::makeCanonical() {
    unsigned nPents = getNumberOfPentachora();

    // Get the empty triangulation out of the way.
    if (nPents == 0)
        return false;

    // Prepare to search for isomorphisms.
    Dim4Isomorphism current(nPents), currentInv(nPents);
    Dim4Isomorphism best(nPents), bestInv(nPents);

    // The thing to best is the identity isomorphism.
    unsigned pent, inner;
    for (pent = 0; pent < nPents; ++pent) {
        best.pentImage(pent) = bestInv.pentImage(pent) = pent;
        best.facetPerm(pent) = bestInv.facetPerm(pent) = NPerm5();
    }

    // Run through potential preimages of pentachoron 0.
    int perm;
    for (pent = 0; pent < nPents; ++pent) {
        for (perm = 0; perm < 120; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of pentachoron 0.
            current.pentImage(pent) = 0;
            currentInv.pentImage(0) = pent;

            current.facetPerm(pent) = NPerm5::S5[NPerm5::invS5[perm]];
            currentInv.facetPerm(0) = NPerm5::S5[perm];

            if (extendIsomorphism(this, current, currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (inner = 0; inner < nPents; ++inner) {
                    best.pentImage(inner) = current.pentImage(inner);
                    best.facetPerm(inner) = current.facetPerm(inner);
                    bestInv.pentImage(inner) = currentInv.pentImage(inner);
                    bestInv.facetPerm(inner) = currentInv.facetPerm(inner);
                }
            }
        }
    }

    // Is there anything to do?
    if (best.isIdentity())
        return false;

    // Do it.
    best.applyInPlace(this);
    return true;
}

} // namespace regina

