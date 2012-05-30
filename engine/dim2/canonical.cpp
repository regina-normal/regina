
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
     * the preimage of face 0 has been fixed (along with the
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

        unsigned nFaces = tri->getNumberOfFaces();
        unsigned face;

        for (face = 0; face < nFaces; ++face)
            if (face != currentInv.faceImage(0))
                current.faceImage(face) = -1;

        int edge;

        unsigned origFace, origFaceBest;
        int origEdge, origEdgeBest;

        Dim2Face *adjFace, *adjFaceBest;
        unsigned adjFaceIndex, adjFaceIndexBest;
        unsigned finalImage, finalImageBest;

        NPerm3 gluingPerm, gluingPermBest;
        NPerm3 finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        unsigned lastAssigned = 0;
        for (face = 0; face < nFaces; ++face) {
            // INV: We have already selected the preimage of face and
            // the corresponding edge permutation by the time we reach
            // this point.
            origFace = currentInv.faceImage(face);
            origFaceBest = bestInv.faceImage(face);

            for (edge = 0; edge < 3; ++edge) {
                origEdge = current.edgePerm(origFace).preImageOf(edge);
                origEdgeBest = best.edgePerm(origFaceBest).preImageOf(edge);

                // Check out the adjacency along face/edge.
                adjFace = tri->getFace(origFace)->adjacentFace(origEdge);
                adjFaceIndex = (adjFace ?
                    tri->faceIndex(adjFace) : nFaces);
                adjFaceBest = tri->getFace(origFaceBest)->
                    adjacentFace(origEdgeBest);
                adjFaceIndexBest = (adjFaceBest ?
                    tri->faceIndex(adjFaceBest) : nFaces);

                justAssigned = false;
                if (adjFace && current.faceImage(adjFaceIndex) < 0) {
                    // We have a new face that needs assignment.
                    ++lastAssigned;
                    current.faceImage(adjFaceIndex) = lastAssigned;
                    currentInv.faceImage(lastAssigned) = adjFaceIndex;
                    justAssigned = true;
                }

                finalImage = (adjFace ?
                    current.faceImage(adjFaceIndex) : nFaces);
                finalImageBest = (adjFaceBest ?
                    best.faceImage(adjFaceIndexBest) : nFaces);

                // We now have a gluing (but possibly not a gluing
                // permutation).  Compare adjacent face indices.
                if ((! better) && finalImage > finalImageBest)
                    return false; // Worse than best-so-far.
                if (finalImage < finalImageBest)
                    better = true;

                // Time now to look at the gluing permutation.
                if (! adjFace)
                    continue;

                gluingPerm = tri->getFace(origFace)->adjacentGluing(origEdge);
                gluingPermBest = tri->getFace(origFaceBest)->
                    adjacentGluing(origEdgeBest);

                if (justAssigned) {
                    // We can choose the permutation ourselves.
                    // Make it so that the final gluing (computed later
                    // below) becomes the identity.
                    current.edgePerm(adjFaceIndex) =
                        current.edgePerm(origFace) * gluingPerm.inverse();
                    currentInv.edgePerm(lastAssigned) =
                        current.edgePerm(adjFaceIndex).inverse();
                }

                // Although adjFace is guaranteed to exist, adjFaceBest is
                // not.  However, if adjFaceBest does not exist then our
                // isomorphism-under-construction must already be an
                // improvement over best.
                if (better)
                    continue;

                // Now we are guaranteed that adjFaceBest exists.
                finalGluing = current.edgePerm(adjFaceIndex) *
                    gluingPerm * current.edgePerm(origFace).inverse();
                finalGluingBest = best.edgePerm(adjFaceIndexBest) *
                    gluingPermBest * best.edgePerm(origFaceBest).inverse();

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
    unsigned nFaces = getNumberOfFaces();

    // Get the empty triangulation out of the way.
    if (nFaces == 0)
        return false;

    // Prepare to search for isomorphisms.
    Dim2Isomorphism current(nFaces), currentInv(nFaces);
    Dim2Isomorphism best(nFaces), bestInv(nFaces);

    // The thing to best is the identity isomorphism.
    unsigned face, inner;
    for (face = 0; face < nFaces; ++face) {
        best.faceImage(face) = bestInv.faceImage(face) = face;
        best.edgePerm(face) = bestInv.edgePerm(face) = NPerm3();
    }

    // Run through potential preimages of face 0.
    int perm;
    for (face = 0; face < nFaces; ++face) {
        for (perm = 0; perm < 6; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of face 0.
            current.faceImage(face) = 0;
            currentInv.faceImage(0) = face;

            current.edgePerm(face) = NPerm3::S3[NPerm3::invS3[perm]];
            currentInv.edgePerm(0) = NPerm3::S3[perm];

            if (extendIsomorphism(this, current, currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (inner = 0; inner < nFaces; ++inner) {
                    best.faceImage(inner) = current.faceImage(inner);
                    best.edgePerm(inner) = current.edgePerm(inner);
                    bestInv.faceImage(inner) = currentInv.faceImage(inner);
                    bestInv.edgePerm(inner) = currentInv.edgePerm(inner);
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

