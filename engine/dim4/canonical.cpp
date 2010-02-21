
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
     * On entry to this routine, \a current should have all pentachoron
     * images set to -1, aside from the selected preimage of pentachoron 0.
     *
     * This routine currently only works for connected triangulations.
     */
    bool extendIsomorphism(const Dim4Triangulation* tri,
            Dim4Isomorphism& current, Dim4Isomorphism& currentInv,
            const Dim4Isomorphism& best, const Dim4Isomorphism& bestInv) {
        bool better = false;

        unsigned pent;
        int facet;

        unsigned origPent, origPentBest;
        int origFacet, origFacetBest;

        unsigned lastAssigned = 0;
        for (pent = 0; pent < tri->getNumberOfPentachora(); ++pent) {
            // INV: We have already selected the preimage of pent and
            // the corresponding facet permutation by the time we reach
            // this point.
            origPent = currentInv.pentImage(pent);
            origPentBest = bestInv.pentImage(pent);

            for (facet = 0; facet < 5; ++facet) {
                origFacet = current.facetPerm(origPent).preImageOf(facet);
                origFacetBest = best.facetPerm(origPentBest).preImageOf(facet);

                // Check out the adjacency along pent/facet.
                // TODO: Down to here.
                // TODO: Don't forget to update better, lastAssigned.
            }
        }
        /*
        myPent = tri->getPentachoron(origP);
        adjPent = myPent->adjacentPentachoron(origF);
        adjPentIndex = tri->pentachoronIndex(adjPent);
        gluingPerm = myPent->adjacentGluing(origF);

        pent[p]->joinTo(f,
            pent[pentImage_[adjPentIndex]],
            facetPerm_[adjPentIndex] *
                gluingPerm *
                facetPerm_[origP].inverse());
        */

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
    unsigned pent;
    for (pent = 0; pent < nPents; ++pent) {
        best.pentImage(pent) = bestInv.pentImage(pent) = pent;
        best.facetPerm(pent) = bestInv.facetPerm(pent) = NPerm5();
    }

    // Run through potential preimages of pentachoron 0.
    int perm;
    for (pent = 0; pent < nPents; ++pent) {
        current.pentImage(pent) = 0;
        currentInv.pentImage(0) = pent;
        for (perm = 0; perm < 120; ++pent) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of pentachoron 0.
            for (pent = 0; pent < nPents; ++pent)
                current.pentImage(pent) = -1;

            current.facetPerm(pent) = NPerm5::S5[NPerm5::invS5[perm]];
            currentInv.facetPerm(0) = NPerm5::S5[perm];

            if (extendIsomorphism(this, current, currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (pent = 0; pent < nPents; ++pent) {
                    best.pentImage(pent) = current.pentImage(pent);
                    best.facetPerm(pent) = current.facetPerm(pent);
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

