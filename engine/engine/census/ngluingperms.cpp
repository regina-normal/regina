
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

#include <algorithm>
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
    NTetrahedron** tet = new NTetrahedron*[nTet];
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

int NGluingPerms::gluingToIndex(const NTetFace& source,
        const NPerm& gluing) const {
    NPerm permS3 = NPerm(pairing->dest(source).face, 3) * gluing *
        NPerm(source.face, 3);
    return (std::find(allPermsS3, allPermsS3 + 6, permS3) - allPermsS3);
}

int NGluingPerms::gluingToIndex(unsigned tet, unsigned face,
        const NPerm& gluing) const {
    NPerm permS3 = NPerm(pairing->dest(tet, face).face, 3) * gluing *
        NPerm(face, 3);
    return (std::find(allPermsS3, allPermsS3 + 6, permS3) - allPermsS3);
}

int NGluingPerms::cmpPermsWithPreImage(const NIsomorphism& automorph) {
    NTetFace faceDest, faceImage;
    int order;
    for (NTetFace face(0, 0); face.tet <
            static_cast<int>(pairing->getNumberOfTetrahedra()); face++) {
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
        bool finiteOnly, int whichPurge, UseGluingPerms use, void* useArgs) {
    NGluingPerms perms(pairing);

    // Generate the list of face pairing automorphisms if necessary.
    NFacePairingIsoList* newAutos =
        (autos == 0 ? new NFacePairingIsoList() : 0);
    if (newAutos) {
        pairing->findAutomorphisms(*newAutos);
        autos = newAutos;
    }

    // Call an optimised internal generation routine if possible.
    if (pairing->getNumberOfTetrahedra() >= 3) {
        if (finiteOnly && pairing->isClosed() &&
                (whichPurge & NCensus::PURGE_NON_MINIMAL) &&
                (whichPurge & NCensus::PURGE_NON_PRIME) &&
                (orientableOnly ||
                    (whichPurge & NCensus::PURGE_P2_REDUCIBLE))) {
            // Closed prime minimal P2-irreducible triangulations with >= 3
            // tetrahedra.
            perms.findAllPermsClosedPrimeMin(autos, orientableOnly, use,
                useArgs);

            if (newAutos)
                delete newAutos;
            return;
        }
    }

    perms.findAllPermsInternal(autos, orientableOnly, finiteOnly, whichPurge,
        use, useArgs);

    if (newAutos)
        delete newAutos;
}

void NGluingPerms::findAllPermsInternal(const NFacePairingIsoList* autos,
        bool orientableOnly, bool finiteOnly, int whichPurge,
        UseGluingPerms use, void* useArgs) {
    unsigned nTetrahedra = getNumberOfTetrahedra();

    // Initialise the internal arrays.
    //
    // In this generation algorithm, each orientation is simply +/-1.
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
            while ((! face.isBeforeStart()) && (pairing->isUnmatched(face) ||
                    pairing->dest(face) < face)) {
                permIndex(face) = -1;
                face--;
            }
            continue;
        }

        // We are sitting on a new permutation to try.

        // Is this going to lead to an unwanted triangulation?
        if (mayPurge(face, whichPurge, orientableOnly, finiteOnly))
            continue;
        if (! orientableOnly)
            if (badEdgeLink(face))
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
        for (face++; face.tet < static_cast<int>(nTetrahedra); face++) {
            if (pairing->isUnmatched(face))
                continue;
            if (face < pairing->dest(face))
                break;

            // We've already decided on this gluing permutation; don't
            // forget to store the corresponding inverse permutation.
            permIndex(face) = allPermsS3Inv[permIndex(pairing->dest(face))];
        }

        // If we're at the end, try the solution and step back.
        if (face.tet == static_cast<int>(nTetrahedra)) {
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            canonical = true;
            for (it = autos->begin(); it != autos->end(); it++) {
                // TODO: Check for cancellation.

                if (cmpPermsWithPreImage(**it) > 0) {
                    canonical = false;
                    break;
                }
            }

            if (canonical)
                use(this, useArgs);

            // Back to the previous face.
            face--;
            while ((! face.isBeforeStart()) && (pairing->isUnmatched(face) ||
                    pairing->dest(face) < face)) {
                permIndex(face) = -1;
                face--;
            }
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

bool NGluingPerms::badEdgeLink(const NTetFace& face) const {
    // Run around all three edges bounding the face.
    NTetFace adj;
    unsigned tet;
    NPerm current;
    NPerm start(face.face, 3);
    bool started, incomplete;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * NPerm(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.tet;

        started = false;
        incomplete = false;

        while ((! started) || (static_cast<int>(tet) != face.tet) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            started = true;

            // Push through the current tetrahedron.
            current = current * NPerm(2, 3);

            // Push across a face.
            if (pairing->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.tet;
        }

        // Did we meet the original edge in reverse?
        if ((! incomplete) && (start != current))
            return true;
    }

    // No bad edge links were found.
    return false;
}

bool NGluingPerms::lowDegreeEdge(const NTetFace& face,
        bool testDegree12, bool testDegree3) const {
    // Run around all three edges bounding the face.
    NTetFace adj;
    unsigned tet;
    NPerm current;
    NPerm start(face.face, 3);
    bool started, incomplete;
    unsigned size;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * NPerm(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.tet;

        started = false;
        incomplete = false;
        size = 0;

        while ((! started) || (static_cast<int>(tet) != face.tet) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            started = true;

            // We're about to push through the current tetrahedron; see
            // if we've already exceeded the size of edge links that we
            // care about.
            if (size >= 3) {
                incomplete = true;
                break;
            }

            // Push through the current tetrahedron.
            current = current * NPerm(2, 3);

            // Push across a face.
            if (pairing->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.tet;
            size++;
        }

        if (! incomplete) {
            if (testDegree12 && size < 3)
                return true;
            if (testDegree3 && size == 3) {
                // Only throw away a degree three edge if it involves
                // three distinct tetrahedra.
                int tet1 = pairing->dest(face.tet, start[2]).tet;
                int tet2 = pairing->dest(face.tet, start[3]).tet;
                if (face.tet != tet1 && tet1 != tet2 && tet2 != face.tet)
                    return true;
            }
        }
    }

    // No bad low-degree edges were found.
    return false;
}

bool NGluingPerms::mayPurge(const NTetFace& face, int whichPurge,
        bool orientableOnly, bool finiteOnly) const {
    // Are we allowed to purge on edges of degree 3?
    bool mayPurgeDeg3 = (whichPurge & NCensus::PURGE_NON_MINIMAL);

    // Are we allowed to purge on edges of degree 1 or 2?
    //
    // A 2-0 edge move or a 2-1 edge move can result in one or more of
    // the following topological changes.
    //
    // Bigon squashing:
    //   - Disc reduction;
    //   - Sphere decomposition or reduction;
    //   - Crushing embedded RP2 to an invalid edge.
    //
    // Pillow squashing:
    //   - Loss of 3-ball;
    //   - Loss of 3-sphere;
    //   - Loss of L(3,1).
    //
    bool mayPurgeDeg12 = (whichPurge & NCensus::PURGE_NON_MINIMAL) &&
        (whichPurge & NCensus::PURGE_NON_PRIME) &&
        ((whichPurge & NCensus::PURGE_P2_REDUCIBLE) || orientableOnly) &&
        finiteOnly && (getNumberOfTetrahedra() > 2);

    if (mayPurgeDeg12 || mayPurgeDeg3)
        return lowDegreeEdge(face, mayPurgeDeg12, mayPurgeDeg3);
    else
        return false;
}

} // namespace regina

