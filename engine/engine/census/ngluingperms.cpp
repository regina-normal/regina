
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include <sstream>
#include "census/ncensus.h"
#include "census/ngluingperms.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

NGluingPerms::NGluingPerms(const NGluingPerms& cloneMe) :
        pairing(cloneMe.pairing) {
    unsigned nTet = cloneMe.getNumberOfTetrahedra();

    orientation = new int[nTet];
    std::copy(cloneMe.orientation, cloneMe.orientation + nTet, orientation);

    permIndices = new int[nTet * 4];
    std::copy(cloneMe.permIndices, cloneMe.permIndices + nTet * 4, permIndices);
}

NTriangulation* NGluingPerms::triangulate() const {
    unsigned nTet = getNumberOfTetrahedra();

    NTriangulation* ans = new NTriangulation;
    NTetrahedron** tet = new (NTetrahedron*)[nTet];
    std::generate(tet, tet + nTet, FuncNew<NTetrahedron>());

    unsigned t, face;
    for (t = 0; t < nTet; t++)
        for (face = 0; face < 4; face++)
            if ((! pairing->isUnmatched(t, face)) &&
                    (! tet[t]->getAdjacentTetrahedron(face)))
                tet[t]->joinTo(face, tet[pairing->dest(t, face).tet],
                    gluingPerm(t, face));

    for (t = 0; t < nTet; t++)
        ans->addTetrahedron(tet[t]);

    delete[] tet;
    return ans;
}

int NGluingPerms::cmpPermsWithPreImage(const NFacePairing* pairing,
        const NIsomorphism& automorph) {
    NTetFace faceDest, faceImage;
    int order;
    for (NTetFace face(0, 0); face.tet < (int)pairing->getNumberOfTetrahedra();
            face++) {
        faceDest = pairing->dest(face);
        if (pairing->isUnmatched(face) || faceDest < face)
            continue;

        faceImage = automorph[face];
        order = gluingPerm(face).compareWith(
            automorph.facePerm(faceDest.tet).inverse() * gluingPerm(faceImage)
            * automorph.facePerm(face.tet));
        if (order < 0)
            return -1;
        else if (order > 0)
            return 1;
    }
    return 0;
}

void NGluingPerms::findAllPerms(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        int whichPurge, UseGluingPerms use, void* useArgs) {
    NGluingPerms perms(pairing);
    perms.findAllPermsInternal(pairing, autos, orientableOnly, whichPurge,
        use, useArgs);
}

void NGluingPerms::findAllPermsInternal(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        int whichPurge, UseGluingPerms use, void* useArgs) {
    unsigned nTetrahedra = getNumberOfTetrahedra();

    // See if we can guarantee no solutions without even searching for
    // permutations.
    /**
     * TODO: Enable this code once we've proven the corresponding
     * theorems!
    if (nTetrahedra > 2 && orientableOnly &&
            (whichPurge & NCensus::PURGE_NON_MINIMAL) &&
            (whichPurge & NCensus::PURGE_NON_PRIME) &&
            pairing->isClosed()) {
        if (pairing->hasTripleEdge() || pairing->hasBrokenDoubleChain() ||
                pairing->hasChainWithDoubleHandle()) {
            use(0, useArgs);
            return;
        }
    }
    */

    // Initialise the internal arrays.
    std::fill(orientation, orientation + nTetrahedra, 0);
    std::fill(permIndices, permIndices + nTetrahedra * 4, -1);

    // Do we in fact have no permutation at all to choose?
    if (pairing->dest(0, 0).isBoundary(nTetrahedra)) {
        use(this, useArgs);
        use(0, useArgs);
        return;
    }

    NTetFace face(0, 0);
    orientation[0] = 1;
    bool canonical;
    std::list<NIsomorphismDirect*>::const_iterator it;
    while (! face.isBeforeStart()) {
        // TODO: Check for cancellation.

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if ((! orientableOnly) || pairing->dest(face).face == 0)
            permIndex(face)++;
        else
            permIndex(face) += 2;

        if (permIndex(face) >= 6) {
            // Out of ideas for this face.
            // Head back down to the previous face.
            permIndex(face) = -1;
            face--;
            while ((! face.isBeforeStart()) &&
                    (pairing->isUnmatched(face) || pairing->dest(face) < face))
                face--;
            continue;
        }

        // We are sitting on a new permutation to try.

        // Is this going to lead to an unwanted triangulation?
        if (mayPurge(face, whichPurge, orientableOnly))
            continue;

        // Fix the orientation if appropriate.
        if (pairing->dest(face).face == 0) {
            // It's the first time we've hit this tetrahedron.
            if ((permIndex(face) + (face.face == 3 ? 0 : 1) +
                    (pairing->dest(face).face == 3 ? 0 : 1)) % 2 == 0)
                orientation[pairing->dest(face).tet] = -orientation[face.tet];
            else
                orientation[pairing->dest(face).tet] = orientation[face.tet];
        }

        // Move on to the next face.
        for (face++; face.tet < (int)nTetrahedra; face++) {
            if (pairing->isUnmatched(face))
                continue;
            if (face < pairing->dest(face))
                break;

            // We've already decided on this gluing permutation; don't
            // forget to store the corresponding inverse permutation.
            permIndex(face) = allPermsS3Inv[permIndex(pairing->dest(face))];
        }

        // If we're at the end, try the solution and step back.
        if (face.tet == (int)nTetrahedra) {
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            canonical = true;
            for (it = autos->begin(); it != autos->end(); it++) {
                // TODO: Check for cancellation.

                if (cmpPermsWithPreImage(pairing, **it) > 0) {
                    canonical = false;
                    break;
                }
            }

            if (canonical)
                use(this, useArgs);

            // Back to the previous face.
            face--;
            while ((! face.isBeforeStart()) &&
                    (pairing->isUnmatched(face) || pairing->dest(face) < face))
                face--;
        } else if (orientableOnly && pairing->dest(face).face > 0) {
            // Be sure to get the orientation right.
            if (orientation[face.tet] == orientation[pairing->dest(face).tet])
                permIndex(face) = 1;
            else
                permIndex(face) = 0;
            
            if ((face.face == 3 ? 0 : 1) +
                    (pairing->dest(face).face == 3 ? 0 : 1) == 1)
                permIndex(face) = (permIndex(face) + 1) % 2;

            permIndex(face) -= 2;
        }
    }

    // And the search is over.
    use(0, useArgs);
}

bool NGluingPerms::mayPurge(const NTetFace& face, int whichPurge,
        bool orientableOnly) {
    if (! whichPurge)
        return false;

    // Are we allowed to purge on edges of degree 1/2?
    bool mayPurgeDeg12 = (whichPurge & NCensus::PURGE_NON_MINIMAL) &&
        (whichPurge & NCensus::PURGE_NON_PRIME) && orientableOnly &&
        (getNumberOfTetrahedra() > 2);

    // Are we allowed to purge on edges of degree 3?
    bool mayPurgeDeg3 = (whichPurge & NCensus::PURGE_NON_MINIMAL);

    // Currently look for edges of degree 1, 2 or 3.
    // Edges of degree 3 either lead to simplification or imply an
    // invalid triangulation.
    // Edges of degrees 2 and 1 in the orientable case lead to simplification,
    // S2 reduction, disc reduction (for bounded triangulations),
    // loss of # L(3,1) or loss of an entire 2 tetrahedron space.
    // Edges of degrees 2 and 1 lead to all sorts of nasties in the
    // non-orientable case and will not be avoided.

    // For edges of degree 2 or 3 we find out once the larger face of the
    // second largest tetrahedron is glued to the larger face of the
    // largest tetrahedron.
    // For edges of degree 1 we find out once the smaller face is glued
    // to the larger.

    // We know face has a partner since we just chose its permutation.
    NTetFace destFace = pairing->dest(face);
    if (destFace.tet == face.tet) {
        // Check for an edge of degree 1.
        // This corresponds to a gluing permutation of order 2
        // (this includes both the orientable and non-orientable gluings).
        if (mayPurgeDeg12) {
            NPerm map(gluingPerm(face));
            if ((map * map).isIdentity())
                return true;
        }
    } else if (destFace.tet > face.tet) {
        // Check for an edge of degree 2 or 3.
        NTetFace destOther;
        NPerm faceMap, otherMap;
        for (NTetFace other(face.tet, 0); other < face; other++) {
            // Examine the edge between faces [other] and [face].
            destOther = pairing->dest(other);
            if (destOther.tet == destFace.tet) {
                // Could be an edge of degree 2.
                if (mayPurgeDeg12) {
                    // The tetrahedra are correct; check the permutation.
                    // We only care about the edge having degree 2; we'll
                    // catch both the valid and invalid gluings.
                    faceMap = gluingPerm(face) *
                        NPerm(face.face, other.face);
                    otherMap = gluingPerm(other) *
                        NPerm(face.face, other.face);
                    if (faceMap[face.face] == otherMap[face.face] &&
                            faceMap[other.face] == otherMap[other.face])
                        return true;
                }
            } else if (destOther.tet < face.tet) {
                // Could be an edge of degree 3.
                if (mayPurgeDeg3) {
                    // We now know that [other] has a partner as well and that
                    // we're looking at three different tetrahedra.

                    // We also know that [other] heads to an earlier
                    // tetrahedron and [face] heads to a later tetrahedron.

                    // Establish how these destination tetrahedra are glued up
                    // at the back.
                    faceMap = gluingPerm(face) * NPerm(face.face, other.face);
                    otherMap = gluingPerm(other) * NPerm(face.face, other.face);

                    if (! (pairing->dest(destOther.tet, otherMap[other.face])
                            == NTetFace(destFace.tet, faceMap[face.face])))
                        continue;

                    // The two faces are glued together.
                    // Check whether the corresponding edge has degree three;
                    // if so then the triangulation will either be non-minimal
                    // or invalid.
                    if (gluingPerm(destOther.tet,
                            otherMap[other.face])[otherMap[face.face]] ==
                            faceMap[other.face])
                        return true;
                }
            }
        }
    }

    return false;
}

} // namespace regina

