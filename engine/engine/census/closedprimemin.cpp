
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
#include "census/ngluingpermsearcher.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/boostutils.h"
#include "utilities/memutils.h"

namespace regina {

const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_END = 1;
const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_FIRST = 2;
const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_SECOND = 3;
const unsigned NClosedPrimeMinSearcher::EDGE_DOUBLE_FIRST = 4;
const unsigned NClosedPrimeMinSearcher::EDGE_DOUBLE_SECOND = 5;
const unsigned NClosedPrimeMinSearcher::EDGE_MISC = 6;

NClosedPrimeMinSearcher::NClosedPrimeMinSearcher(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        UseGluingPerms use, void* useArgs) :
        NGluingPermSearcher(pairing, autos, orientableOnly,
            true /* finiteOnly */,
            NCensus::PURGE_NON_MINIMAL_PRIME | NCensus::PURGE_P2_REDUCIBLE,
            use, useArgs) {
    initOrder();
}

void NClosedPrimeMinSearcher::initOrder() {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    // ---------- Selecting an ordering of faces ----------

    // We fill permutations in the order:
    //     1. One-ended chains (== layered solid tori) from loop to
    //        boundary, though chains may be interlaced in the
    //        processing order;
    //     2. Everything else ordered by tetrahedron faces.
    //
    // Both permutations for each double edge will be processed
    // consecutively, the permutation for the smallest face involved
    // in the double edge being processed first.
    //
    // Note from the tests above that there are no triple edges.

    unsigned nTets = getNumberOfTetrahedra();

    order = new NTetFace[nTets * 2];
    orderType = new unsigned[nTets * 2];

    bool* orderAssigned = new bool[nTets * 4];
        /**< Have we placed a tetrahedron face or its partner in the
             order[] array yet? */

    // Hunt for structures within the face pairing graph.

    NTetFace face, adj;
    unsigned orderDone = 0;
    std::fill(orderAssigned, orderAssigned + 4 * nTets, false);

    // Begin by searching for tetrahedra that are joined to themselves.
    // Note that each tetrahedra can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.

    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++) {
        if (orderAssigned[face.tet * 4 + face.face])
            continue;

        adj = (*pairing)[face];
        if (adj.tet != face.tet)
            continue;

        order[orderDone] = face;
        orderType[orderDone] = EDGE_CHAIN_END;
        orderAssigned[face.tet * 4 + face.face] = true;
        orderAssigned[adj.tet * 4 + adj.face] = true;
        orderDone++;
    }

    // Record the number of one-ended chains.

    unsigned nChains = orderDone;

    // Continue by following each one-ended chain whose base was
    // identified in the previous loop.

    unsigned i;
    int tet;
    NTetFace dest1, dest2;
    NFacePair faces;
    for (i = 0; i < nChains; i++) {
        tet = order[i].tet;
        faces = NFacePair(order[i].face,
            (*pairing)[order[i]].face).complement();
        dest1 = pairing->dest(tet, faces.lower());
        dest2 = pairing->dest(tet, faces.upper());

        // Currently tet and faces refer to the two faces of the base
        // tetrahedron that are pointing outwards.
        while (dest1.tet == dest2.tet && dest1.tet != tet &&
                (! orderAssigned[tet * 4 + faces.lower()]) &&
                (! orderAssigned[tet * 4 + faces.upper()])) {
            // Insert this pair of edges into the ordering and follow
            // the chain.
            orderType[orderDone] = EDGE_CHAIN_INTERNAL_FIRST;
            orderType[orderDone + 1] = EDGE_CHAIN_INTERNAL_SECOND;

            if (tet < dest1.tet) {
                order[orderDone] = NTetFace(tet, faces.lower());
                order[orderDone + 1] = NTetFace(tet, faces.upper());
            }

            orderAssigned[tet * 4 + faces.lower()] = true;
            orderAssigned[tet * 4 + faces.upper()] = true;
            orderAssigned[dest1.tet * 4 + dest1.face] = true;
            orderAssigned[dest2.tet * 4 + dest2.face] = true;

            faces = NFacePair(dest1.face, dest2.face);

            if (dest1.tet < tet) {
                order[orderDone] = NTetFace(dest1.tet, faces.lower());
                order[orderDone + 1] = NTetFace(dest1.tet, faces.upper());
            }

            faces = faces.complement();
            tet = dest1.tet;

            dest1 = pairing->dest(tet, faces.lower());
            dest2 = pairing->dest(tet, faces.upper());

            orderDone += 2;
        }
    }

    // Record the number of edges in the face pairing graph
    // belonging to one-ended chains.
    nChainEdges = orderDone;

    // Run through the remaining faces.
    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++)
        if (! orderAssigned[face.tet * 4 + face.face]) {
            order[orderDone] = face;
            if (face.face < 3 && pairing->dest(boost::next(face)).tet ==
                    pairing->dest(face).tet)
                orderType[orderDone] = EDGE_DOUBLE_FIRST;
            else if (face.face > 0 && pairing->dest(boost::prior(face)).tet ==
                    pairing->dest(face).tet)
                orderType[orderDone] = EDGE_DOUBLE_SECOND;
            else
                orderType[orderDone] = EDGE_MISC;
            orderDone++;

            adj = (*pairing)[face];
            orderAssigned[face.tet * 4 + face.face] = true;
            orderAssigned[adj.tet * 4 + adj.face] = true;
        }

    // All done for the order[] array.  Tidy up.
    delete[] orderAssigned;

    // ---------- Calculating the possible gluing permutations ----------

    // For each face in the order[] array of type EDGE_CHAIN_END or
    // EDGE_CHAIN_INTERNAL_FIRST, we calculate the two gluing permutations
    // that must be tried.
    //
    // For the remaining faces we try all possible permutations.

    chainPermIndices = (nChainEdges == 0 ? 0 : new int[nChainEdges * 2]);

    NFacePair facesAdj, comp, compAdj;
    NPerm trial1, trial2;
    for (i = 0; i < nChainEdges; i++) {
        if (orderType[i] == EDGE_CHAIN_END) {
            faces = NFacePair(order[i].face, pairing->dest(order[i]).face);
            comp = faces.complement();

            // order[i].face == faces.lower(),
            // pairing->dest(order[i]).face == faces.upper().
            chainPermIndices[2 * i] = gluingToIndex(order[i],
                NPerm(faces.lower(), faces.upper(),
                      faces.upper(), comp.lower(),
                      comp.lower(), comp.upper(),
                      comp.upper(), faces.lower()));
            chainPermIndices[2 * i + 1] = gluingToIndex(order[i],
                NPerm(faces.lower(), faces.upper(),
                      faces.upper(), comp.upper(),
                      comp.upper(), comp.lower(),
                      comp.lower(), faces.lower()));
        } else if (orderType[i] == EDGE_CHAIN_INTERNAL_FIRST) {
            faces = NFacePair(order[i].face, order[i + 1].face);
            comp = faces.complement();
            facesAdj = NFacePair(pairing->dest(order[i]).face,
                pairing->dest(order[i + 1]).face);
            compAdj = facesAdj.complement();

            // order[i].face == faces.lower(),
            // order[i + 1].face == faces.upper(),
            // pairing->dest(order[i]).face == facesAdj.lower().
            // pairing->dest(order[i + 1]).face == facesAdj.upper().
            trial1 = NPerm(faces.lower(), facesAdj.lower(),
                           faces.upper(), compAdj.lower(),
                           comp.lower(), compAdj.upper(),
                           comp.upper(), facesAdj.upper());
            trial2 = NPerm(faces.lower(), facesAdj.lower(),
                           faces.upper(), compAdj.upper(),
                           comp.lower(), compAdj.lower(),
                           comp.upper(), facesAdj.upper());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    NPerm(faces.lower(), compAdj.upper(),
                          faces.upper(), facesAdj.upper(),
                          comp.lower(), facesAdj.lower(),
                          comp.upper(), compAdj.lower()));
            } else {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    NPerm(faces.lower(), compAdj.lower(),
                          faces.upper(), facesAdj.upper(),
                          comp.lower(), facesAdj.lower(),
                          comp.upper(), compAdj.upper()));
            }

            trial1 = NPerm(faces.lower(), facesAdj.lower(),
                           faces.upper(), compAdj.lower(),
                           comp.lower(), facesAdj.upper(),
                           comp.upper(), compAdj.upper());
            trial2 = NPerm(faces.lower(), facesAdj.lower(),
                           faces.upper(), compAdj.upper(),
                           comp.lower(), facesAdj.upper(),
                           comp.upper(), compAdj.lower());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    NPerm(faces.lower(), compAdj.upper(),
                          faces.upper(), facesAdj.upper(),
                          comp.lower(), compAdj.lower(),
                          comp.upper(), facesAdj.lower()));
            } else {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    NPerm(faces.lower(), compAdj.lower(),
                          faces.upper(), facesAdj.upper(),
                          comp.lower(), compAdj.upper(),
                          comp.upper(), facesAdj.lower()));
            }
        }
    }
}

void NClosedPrimeMinSearcher::runSearch(long maxDepth) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    unsigned nTets = getNumberOfTetrahedra();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTets * 4 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Begin by testing for face pairings that can never lead to such a
        // triangulation.
        if (pairing->hasTripleEdge() || pairing->hasBrokenDoubleEndedChain() ||
                pairing->hasOneEndedChainWithDoubleHandle()) {
            use_(0, useArgs_);
            return;
        }

        orderElt = 0;
        if (nChainEdges < nTets * 2)
            orientation[order[nChainEdges].tet] = 1;
    }

    // ---------- Selecting the individual gluing permutations ----------

    // Observe that in a canonical face pairing, one-ended chains always
    // follow an increasing sequence of tetrahedra from boundary to end,
    // or follow the sequence of tetrahedra 0, 1, ..., k from end to
    // boundary.
    //
    // In particular, this means that for any tetrahedron not internal
    // to a one-ended chain (with the possible exception of tetrahedron
    // order[nChainEdges].tet), face 0 of this tetrahedron is not
    // involved in a one-ended chain.

    // In this generation algorithm, each orientation is simply +/-1.
    // We won't bother assigning orientations to the tetrahedra internal
    // to the one-ended chains.

    int minOrder = orderElt;
    int maxOrder = orderElt + maxDepth;

    NTetFace face, adj;
    NFacePair faces;
    NPerm trial1, trial2;
    bool generic;
    while (orderElt >= minOrder) {
        face = order[orderElt];
        adj = (*pairing)[face];

        // TODO: Check for cancellation.

        // Move to the next permutation.
        if (orderType[orderElt] == EDGE_CHAIN_END ||
                orderType[orderElt] == EDGE_CHAIN_INTERNAL_FIRST) {
            // Choose from one of the two permutations stored in array
            // chainPermIndices[].
            generic = false;
            if (permIndex(face) < 0)
                permIndex(face) = chainPermIndices[2 * orderElt];
            else if (permIndex(face) == chainPermIndices[2 * orderElt])
                permIndex(face) = chainPermIndices[2 * orderElt + 1];
            else
                permIndex(face) = 6;
        } else if (orderType[orderElt] == EDGE_CHAIN_INTERNAL_SECOND) {
            // The permutation is predetermined.
            generic = false;
            if (permIndex(face) < 0) {
                if (permIndex(order[orderElt - 1]) ==
                        chainPermIndices[2 * orderElt - 2])
                    permIndex(face) = chainPermIndices[2 * orderElt];
                else
                    permIndex(face) = chainPermIndices[2 * orderElt + 1];
            } else
                permIndex(face) = 6;
        } else {
            // Generic case.
            generic = true;

            // Be sure to preserve the orientation of the permutation if
            // necessary.
            if ((! orientableOnly_) || pairing->dest(face).face == 0)
                permIndex(face)++;
            else
                permIndex(face) += 2;
        }

        // Are we out of ideas for this face?
        if (permIndex(face) >= 6) {
            // Head back down to the previous face.
            permIndex(face) = -1;
            permIndex(adj) = -1;
            orderElt--;
            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = allPermsS3Inv[permIndex(face)];

        // Is this going to lead to an unwanted triangulation?
        if (lowDegreeEdge(face, true, true))
            continue;
        if (! orientableOnly_)
            if (badEdgeLink(face))
                continue;

        if (orderType[orderElt] == EDGE_DOUBLE_SECOND) {
            // We can use our double edge results to limit the possible
            // choices for the second permutation of a double edge.

            // These results boil down to the following:
            //
            // 1) You cannot create an edge of degree two;
            //
            // 2) Each vertex of the equator edge of the first
            // tetrahedron must be mapped to two distinct vertices of
            // the second tetrahedron by the two gluing permutations.
            //
            // Note that condition (1) is already verified by the
            // lowDegreeEdge() and badEdgeLink() tests above.

            // The two vertices of the equator:
            faces = NFacePair(order[orderElt - 1].face,
                order[orderElt].face).complement();

            // The two gluing permutations:
            trial1 = gluingPerm(order[orderElt - 1]);
            trial2 = gluingPerm(order[orderElt]);

            if (trial1[faces.lower()] == trial2[faces.lower()] ||
                    trial1[faces.upper()] == trial2[faces.upper()])
                continue;
        }

        // Fix the orientation if appropriate.
        if (generic && adj.face == 0 && orientableOnly_) {
            // It's the first time we've hit this tetrahedron.
            if ((permIndex(face) + (face.face == 3 ? 0 : 1) +
                    (adj.face == 3 ? 0 : 1)) % 2 == 0)
                orientation[adj.tet] = -orientation[face.tet];
            else
                orientation[adj.tet] = orientation[face.tet];
        }

        // Move on to the next face.
        orderElt++;

        // If we're at the end, try the solution and step back.
        if (orderElt == maxOrder || orderElt == static_cast<int>(nTets) * 2) {
            // We've gone as far as we need to.
            if (orderElt == static_cast<int>(nTets) * 2) {
                // We in fact have an entire triangulation.
                // Run through the automorphisms and check whether our
                // permutations are in canonical form.
                if (isCanonical())
                    use_(this, useArgs_);
            } else {
                // No triangulation, just hit the max depth.
                use_(this, useArgs_);
            }

            // Back to the previous face.
            orderElt--;
        } else {
            // We've moved onto a new face.
            // Be sure to get the orientation right.
            face = order[orderElt];
            if (orientableOnly_ && pairing->dest(face).face > 0) {
                // permIndex(face) will be set to -1 or -2 as appropriate.
                adj = (*pairing)[face];
                if (orientation[face.tet] == orientation[adj.tet])
                    permIndex(face) = 1;
                else
                    permIndex(face) = 0;

                if ((face.face == 3 ? 0 : 1) + (adj.face == 3 ? 0 : 1) == 1)
                    permIndex(face) = (permIndex(face) + 1) % 2;

                permIndex(face) -= 2;
            }
        }
    }

    // And the search is over.

    use_(0, useArgs_);
}

} // namespace regina

