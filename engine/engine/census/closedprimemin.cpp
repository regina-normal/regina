
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

#include <sstream>
#include "census/ncensus.h"
#include "census/ngluingperms.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

void NGluingPerms::findAllPermsClosedPrimeMin(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        UseGluingPerms use, void* useArgs) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    // ---------- Tests for trivial solutions ----------

    // Begin by testing for face pairings that can never lead to such a
    // triangulation.

    if (orientableOnly)
        if (pairing->hasTripleEdge() || pairing->hasBrokenDoubleEndedChain()
                || pairing->hasOneEndedChainWithDoubleHandle()) {
            use(0, useArgs);
            return;
        }

    // ---------- Selecting an ordering of faces ----------

    // We fill permutations in the order:
    //     1. One-ended chains (== layered solid tori)
    //     2. Double edges (== restricted possibilities)
    //     3. Everything else

    unsigned nTets = getNumberOfTetrahedra();

    /**
     * The order in which gluing permutations are assigned to faces is
     * order[0], order[1], ..., order[2n-1].  Note that each element of
     * this array corresponds to a single edge of the underlying face
     * pairing graph, which in turn represents a tetrahedron face and
     * its image under the given face pairing.
     */
    NTetFace* order = new NTetFace[nTets * 2];

    /**
     * For each edge in the face pairing graph stored in the order[]
     * array, a corresponding category for this edge is stored in the
     * orderType[] array.  Categories are described by the EDGE_...
     * constants defined below.
     */
    unsigned* orderType = new unsigned[nTets * 2];

    /**
     * Have we placed a tetrahedron face or its partner in the order[]
     * array yet?
     */
    bool* orderAssigned = new bool[nTets * 4];

    /**
     * The end of a one-ended chain.
     */
    static const unsigned EDGE_CHAIN_END = 1;

    /**
     * The first edge of a double edge within a one-ended chain.
     * The corresponding element of order[] stores the face closest to
     * the loop at the end of this chain.
     */
    static const unsigned EDGE_CHAIN_INTERNAL_FIRST = 2;

    /**
     * The second edge of a double edge within a one-ended chain.
     * See EDGE_CHAIN_INTERNAL_FIRST for further details.
     */
    static const unsigned EDGE_CHAIN_INTERNAL_SECOND = 3;

    /**
     * The first edge of a miscellaneous double edge.
     */
    static const unsigned EDGE_DOUBLE_FIRST = 4;

    /**
     * The second edge of a miscellaneous double edge.
     * Note that the same tetrahedron is stored in the order[] array
     * for both the first and second edge of this double edge.
     */
    static const unsigned EDGE_DOUBLE_SECOND = 5;

    /**
     * A miscellaneous edge in the face pairing graph.
     */
    static const unsigned EDGE_MISC = 6;

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

    // Continue by following each one-ended chain whose base was
    // identified in the previous loop.

    unsigned nChains = orderDone;
    unsigned i, j;
    unsigned tet;
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
        while (dest1.tet == dest2.tet && dest1.tet != (int)tet &&
                (! orderAssigned[tet * 4 + faces.lower()]) &&
                (! orderAssigned[tet * 4 + faces.upper()])) {
            // Insert this pair of edges into the ordering and follow
            // the chain.
            order[orderDone] = NTetFace(tet, faces.lower());
            order[orderDone + 1] = NTetFace(tet, faces.upper());
            orderType[orderDone] = EDGE_CHAIN_INTERNAL_FIRST;
            orderType[orderDone + 1] = EDGE_CHAIN_INTERNAL_SECOND;
            orderAssigned[tet * 4 + faces.lower()] = true;
            orderAssigned[tet * 4 + faces.upper()] = true;
            orderDone += 2;

            tet = dest1.tet;
            faces = NFacePair(dest1.face, dest2.face);
            orderAssigned[tet * 4 + faces.lower()] = true;
            orderAssigned[tet * 4 + faces.upper()] = true;
            faces = faces.complement();
        }
    }

    // Search now for miscellaneous double edges.
    for (tet = 0; tet < nTets; tet++) {
        for (i = 0; i < 4; i++) {
            if (orderAssigned[tet * 4 + i])
                continue;
            dest1 = pairing->dest(tet, i);

            for (j = i + 1; j < 4; j++) {
                if (orderAssigned[tet * 4 + j])
                    continue;
                dest2 = pairing->dest(tet, j);

                if (dest1.tet == dest2.tet && dest1.tet != (int)tet) {
                    // Insert this double edge.
                    order[orderDone] = NTetFace(tet, i);
                    order[orderDone + 1] = NTetFace(tet, j);
                    orderType[orderDone] = EDGE_DOUBLE_FIRST;
                    orderType[orderDone + 1] = EDGE_DOUBLE_SECOND;
                    orderDone += 2;

                    orderAssigned[tet * 4 + i] = true;
                    orderAssigned[tet * 4 + j] = true;
                    orderAssigned[dest1.tet * 4 + dest1.face] = true;
                    orderAssigned[dest2.tet * 4 + dest2.face] = true;
                }
            }
        }
    }

    // Finally run through the remaining faces.
    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++)
        if (! orderAssigned[face.tet * 4 + face.face]) {
            order[orderDone] = face;
            orderType[orderDone] = EDGE_MISC;
            orderDone++;

            adj = (*pairing)[face];
            orderAssigned[face.tet * 4 + face.face] = true;
            orderAssigned[adj.tet * 4 + adj.face] = true;
        }

    // ---------- Selecting the individual gluing permutations ----------

    // Initialise the internal arrays.
    //
    // In this generation algorithm, the orientation array counts
    // forwards or backwards from zero according to how many times the
    // orientation has been set/verified.
    std::fill(orientation, orientation + nTets, 0);
    std::fill(permIndices, permIndices + nTets * 4, -1);

    int orderElt = 0;
    orientation[order[0].tet] = 1;
    bool canonical;
    std::list<NIsomorphismDirect*>::const_iterator it;
    while (orderElt >= 0) {
        face = order[orderElt];
        adj = (*pairing)[face];

        // TODO: Check for cancellation.

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if (orientation[(*pairing)[face].tet] == 0)
            permIndex(face)++;
        else
            permIndex(face) += 2;

        if (permIndex(face) >= 6) {
            // Out of ideas for this face.
            // Head back down to the previous face.
            permIndex(face) = -1;
            face--;
            while ((! face.isBeforeStart()) && pairing->dest(face) < face)
                face--;
            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = allPermsS3Inv[permIndex(face)];

        // Is this going to lead to an unwanted triangulation?
        if (mayPurge(face, NCensus::PURGE_NON_MINIMAL_PRIME, true, true))
            continue;

        // Fix the orientation if appropriate.
        if (adj.face == 0) {
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
        if (face.tet == (int)nTets) {
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
            while ((! face.isBeforeStart()) && pairing->dest(face) < face)
                face--;
        } else if (pairing->dest(face).face > 0) {
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

    delete[] order;
    delete[] orderType;
    delete[] orderAssigned;

    use(0, useArgs);
}

} // namespace regina

