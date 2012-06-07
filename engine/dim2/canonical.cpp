
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

#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

namespace regina {

namespace {
    /**
     * For internal use by makeCanonical().  This routines assumes that
     * the preimage of triangle 0 has been fixed (along with the
     * corresponding edge permutation), and tries to extend
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
    bool extendIsomorphism(const Dim2Triangulation* tri,
            Dim2Isomorphism& current, Dim2Isomorphism& currentInv,
            const Dim2Isomorphism& best, const Dim2Isomorphism& bestInv) {
        bool better = false;

        unsigned nTriangles = tri->getNumberOfTriangles();
        unsigned triangle;

        for (triangle = 0; triangle < nTriangles; ++triangle)
            if (triangle != currentInv.simpImage(0))
                current.simpImage(triangle) = -1;

        int edge;

        unsigned origTri, origTriBest;
        int origEdge, origEdgeBest;

        Dim2Triangle *adjTri, *adjTriBest;
        unsigned adjTriIndex, adjTriIndexBest;
        unsigned finalImage, finalImageBest;

        NPerm3 gluingPerm, gluingPermBest;
        NPerm3 finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        unsigned lastAssigned = 0;
        for (triangle = 0; triangle < nTriangles; ++triangle) {
            // INV: We have already selected the preimage of triangle and
            // the corresponding edge permutation by the time we reach
            // this point.
            origTri = currentInv.simpImage(triangle);
            origTriBest = bestInv.simpImage(triangle);

            for (edge = 0; edge < 3; ++edge) {
                origEdge = current.facetPerm(origTri).preImageOf(edge);
                origEdgeBest = best.facetPerm(origTriBest).preImageOf(edge);

                // Check out the adjacency along triangle/edge.
                adjTri = tri->getTriangle(origTri)->adjacentTriangle(
                    origEdge);
                adjTriIndex = (adjTri ?
                    tri->triangleIndex(adjTri) : nTriangles);
                adjTriBest = tri->getTriangle(origTriBest)->
                    adjacentTriangle(origEdgeBest);
                adjTriIndexBest = (adjTriBest ?
                    tri->triangleIndex(adjTriBest) : nTriangles);

                justAssigned = false;
                if (adjTri && current.simpImage(adjTriIndex) < 0) {
                    // We have a new triangle that needs assignment.
                    ++lastAssigned;
                    current.simpImage(adjTriIndex) = lastAssigned;
                    currentInv.simpImage(lastAssigned) = adjTriIndex;
                    justAssigned = true;
                }

                finalImage = (adjTri ?
                    current.simpImage(adjTriIndex) : nTriangles);
                finalImageBest = (adjTriBest ?
                    best.simpImage(adjTriIndexBest) : nTriangles);

                // We now have a gluing (but possibly not a gluing
                // permutation).  Compare adjacent triangle indices.
                if ((! better) && finalImage > finalImageBest)
                    return false; // Worse than best-so-far.
                if (finalImage < finalImageBest)
                    better = true;

                // Time now to look at the gluing permutation.
                if (! adjTri)
                    continue;

                gluingPerm = tri->getTriangle(origTri)->adjacentGluing(
                    origEdge);
                gluingPermBest = tri->getTriangle(origTriBest)->
                    adjacentGluing(origEdgeBest);

                if (justAssigned) {
                    // We can choose the permutation ourselves.
                    // Make it so that the final gluing (computed later
                    // below) becomes the identity.
                    current.facetPerm(adjTriIndex) =
                        current.facetPerm(origTri) * gluingPerm.inverse();
                    currentInv.facetPerm(lastAssigned) =
                        current.facetPerm(adjTriIndex).inverse();
                }

                // Although adjTri is guaranteed to exist, adjTriBest is
                // not.  However, if adjTriBest does not exist then our
                // isomorphism-under-construction must already be an
                // improvement over best.
                if (better)
                    continue;

                // Now we are guaranteed that adjTriBest exists.
                finalGluing = current.facetPerm(adjTriIndex) *
                    gluingPerm * current.facetPerm(origTri).inverse();
                finalGluingBest = best.facetPerm(adjTriIndexBest) *
                    gluingPermBest * best.facetPerm(origTriBest).inverse();

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

bool Dim2Triangulation::makeCanonical() {
    unsigned nTriangles = getNumberOfTriangles();

    // Get the empty triangulation out of the way.
    if (nTriangles == 0)
        return false;

    // Prepare to search for isomorphisms.
    Dim2Isomorphism current(nTriangles), currentInv(nTriangles);
    Dim2Isomorphism best(nTriangles), bestInv(nTriangles);

    // The thing to best is the identity isomorphism.
    unsigned tri, inner;
    for (tri = 0; tri < nTriangles; ++tri) {
        best.simpImage(tri) = bestInv.simpImage(tri) = tri;
        best.facetPerm(tri) = bestInv.facetPerm(tri) = NPerm3();
    }

    // Run through potential preimages of triangle 0.
    int perm;
    for (tri = 0; tri < nTriangles; ++tri) {
        for (perm = 0; perm < 6; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of triangle 0.
            current.simpImage(tri) = 0;
            currentInv.simpImage(0) = tri;

            current.facetPerm(tri) = NPerm3::S3[NPerm3::invS3[perm]];
            currentInv.facetPerm(0) = NPerm3::S3[perm];

            if (extendIsomorphism(this, current, currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (inner = 0; inner < nTriangles; ++inner) {
                    best.simpImage(inner) = current.simpImage(inner);
                    best.facetPerm(inner) = current.facetPerm(inner);
                    bestInv.simpImage(inner) = currentInv.simpImage(inner);
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

