
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

#include <sstream>
#include "census/ncensus.h"
#include "census/ngluingpermsearcher.h"
#include "triangulation/nedge.h"
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

const char NClosedPrimeMinSearcher::ECLASS_TWISTED = 1;
const char NClosedPrimeMinSearcher::ECLASS_LOWDEG = 2;
const char NClosedPrimeMinSearcher::ECLASS_HIGHDEG = 4;
const char NClosedPrimeMinSearcher::ECLASS_CONE = 8;
const char NClosedPrimeMinSearcher::ECLASS_L31 = 16;

const unsigned NClosedPrimeMinSearcher::coneEdge[12][2] = {
    { 0, 1 }, { 0, 2 }, { 1, 2 }, { 0, 3 }, { 0, 4 }, { 3, 4 },
    { 1, 3 }, { 1, 5 }, { 3, 5 }, { 2, 4 }, { 2, 5 }, { 4, 5 },
};

const char NClosedPrimeMinSearcher::coneNoTwist[12] = {
    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1
};

const char NClosedPrimeMinSearcher::dataTag_ = 'c';

NClosedPrimeMinSearcher::NClosedPrimeMinSearcher(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        UseGluingPerms use, void* useArgs) :
        NCompactSearcher(pairing, autos, orientableOnly,
            NCensus::PURGE_NON_MINIMAL_PRIME | NCensus::PURGE_P2_REDUCIBLE,
            use, useArgs) {
    // Initialise internal arrays, specifically those relating to face
    // orderings and properties of chains, to accurately reflect the
    // underlying face pairing.
    //
    // Although the NGluingPermSearcher constructor initialises the order[]
    // array in a vanilla fashion (front to back), we reorder things now
    // to match specific structures that are found in the face pairing graph.

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

    // ---------- Tracking of vertex / edge equivalence classes ----------

#if PRUNE_HIGH_DEG_EDGE_SET
    highDegSum = 0;
    highDegBound = 3 * nTets - 3;
#endif
}

// TODO (net): See what was removed when we brought in vertex link checking.
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
        if (pairing->hasTripleEdge() ||
                pairing->hasBrokenDoubleEndedChain() ||
                pairing->hasOneEndedChainWithDoubleHandle() ||
                pairing->hasOneEndedChainWithStrayBigon() ||
                pairing->hasWedgedDoubleEndedChain() ||
                pairing->hasTripleOneEndedChain()) {
            use_(0, useArgs_);
            return;
        }

        orderElt = 0;
        if (nChainEdges < nTets * 2)
            orientation[order[nChainEdges].tet] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt == static_cast<int>(nTets) * 2) {
        if (isCanonical())
            use_(this, useArgs_);
        use_(0, useArgs_);
        return;
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
    bool generic;
    int mergeResult;
    while (orderElt >= minOrder) {
        face = order[orderElt];
        adj = (*pairing)[face];

        // TODO (long-term): Check for cancellation.

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
            if ((! orientableOnly_) || adj.face == 0)
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

            // Pull apart vertex and edge links at the previous level.
            if (orderElt >= minOrder) {
                splitVertexClasses();
                splitEdgeClasses();
            }

            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = NPerm::invS3[permIndex(face)];

        // In the following code we use several results from
        // "Face pairing graphs and 3-manifold enumeration", B. A. Burton,
        // J. Knot Theory Ramifications 13 (2004).
        //
        // These include:
        //
        // - We cannot have an edge of degree <= 2, or an edge of degree 3
        //   meeting three distinct tetrahedra (section 2.1);
        // - We must have exactly one vertex (lemma 2.6);
        // - We cannot have a face with two edges identified to form a
        //   cone (lemma 2.8);
        // - We cannot have a face with all three edges identified to
        //   form an L(3,1) spine (lemma 2.5).

        // Merge edge links and run corresponding tests.
        if (mergeEdgeClasses()) {
            // We created a structure that should not appear in a final
            // census triangulation (e.g., a low-degree or invalid edge,
            // or a face whose edges are identified in certain ways).
            splitEdgeClasses();
            continue;
        }
        // The final triangulation should have precisely (nTets + 1) edges
        // (since it must have precisely one vertex).
        if (nEdgeClasses < nTets + 1) {
            // We already have too few edge classes, and the count can
            // only get smaller.
            // Note that the triangulations we are pruning include ideal
            // triangulations (with vertex links of Euler characteristic < 2).
            splitEdgeClasses();
            continue;
        }
        // In general, one can prove that (assuming no invalid edges or
        // boundary faces) we will end up with (<= nTets + nVertices) edges
        // (with strictly fewer edges if some vertex links are non-spherical).
        // If we must end up with (> nTets + 1) edges we can therefore
        // prune since we won't have a one-vertex triangulation.
        if (nEdgeClasses > nTets + 1 + 3 * (nTets * 2 - orderElt - 1)) {
            // We have (2n - orderElt - 1) more gluings to choose.
            // Since each merge can reduce the number of edge classes
            // by at most 3, there is no way we can end up with just
            // (nTets + 1) edges at the end.
            splitEdgeClasses();
            continue;
        }

        // Merge vertex links and run corresponding tests.
        mergeResult = mergeVertexClasses();
        if (mergeResult & VLINK_CLOSED) {
            // We closed off a vertex link, which means we will end up
            // with more than one vertex (unless this was our very last
            // gluing).
            if (orderElt + 1 < static_cast<int>(nTets) * 2) {
                splitVertexClasses();
                splitEdgeClasses();
                continue;
            }
        }
        if (mergeResult & VLINK_NON_SPHERE) {
            // Our vertex link will never be a 2-sphere.  Stop now.
            splitVertexClasses();
            splitEdgeClasses();
            continue;
        }
        if (nVertexClasses > 1 + 3 * (nTets * 2 - orderElt - 1)) {
            // We have (2n - orderElt - 1) more gluings to choose.
            // Since each merge can reduce the number of vertex classes
            // by at most 3, there is no way we can end up with just one
            // vertex at the end.
            splitVertexClasses();
            splitEdgeClasses();
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
        if (orderElt == static_cast<int>(nTets) * 2) {
            // We in fact have an entire triangulation.
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            if (isCanonical())
                use_(this, useArgs_);

            // Back to the previous face.
            orderElt--;

            // Pull apart vertex and edge links at the previous level.
            if (orderElt >= minOrder) {
                splitVertexClasses();
                splitEdgeClasses();
            }
        } else {
            // Not a full triangulation; just one level deeper.

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

            if (orderElt == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                use_(this, useArgs_);

                // Back to the previous face.
                permIndex(face) = -1;
                orderElt--;

                // Pull apart vertex links at the previous level.
                if (orderElt >= minOrder) {
                    splitVertexClasses();
                    splitEdgeClasses();
                }
            }
        }
    }

    // And the search is over.

    // Some extra sanity checking.
    if (minOrder == 0) {
        // Our vertex classes had better be 4n standalone vertices.
        if (nVertexClasses != 4 * nTets)
            std::cerr << "ERROR: nVertexClasses == "
                << nVertexClasses << " at end of search!" << std::endl;
        for (int i = 0; i < static_cast<int>(nTets) * 4; i++) {
            if (vertexState[i].parent != -1)
                std::cerr << "ERROR: vertexState[" << i << "].parent == "
                    << vertexState[i].parent << " at end of search!"
                    << std::endl;
            if (vertexState[i].rank != 0)
                std::cerr << "ERROR: vertexState[" << i << "].rank == "
                    << vertexState[i].rank << " at end of search!" << std::endl;
            if (vertexState[i].bdry != 3)
                std::cerr << "ERROR: vertexState[" << i << "].bdry == "
                    << vertexState[i].bdry << " at end of search!" << std::endl;
            if (vertexState[i].hadEqualRank)
                std::cerr << "ERROR: vertexState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
            if (vertexState[i].bdryEdges != 3)
                std::cerr << "ERROR: vertexState[" << i << "].bdryEdges == "
                    << static_cast<int>(vertexState[i].bdryEdges)
                    << " at end of search!" << std::endl;
            if (vertexState[i].bdryNext[0] != i)
                std::cerr << "ERROR: vertexState[" << i << "].bdryNext[0] == "
                    << vertexState[i].bdryNext[0] << " at end of search!"
                    << std::endl;
            if (vertexState[i].bdryNext[1] != i)
                std::cerr << "ERROR: vertexState[" << i << "].bdryNext[1] == "
                    << vertexState[i].bdryNext[1] << " at end of search!"
                    << std::endl;
            if (vertexState[i].bdryTwist[0])
                std::cerr << "ERROR: vertexState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
            if (vertexState[i].bdryTwist[1])
                std::cerr << "ERROR: vertexState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (vertexStateChanged[i] != -1)
                std::cerr << "ERROR: vertexStateChanged[" << i << "] == "
                    << vertexStateChanged[i] << " at end of search!"
                    << std::endl;

        // And our edge classes had better be 6n standalone edges.
        if (nEdgeClasses != 6 * nTets)
            std::cerr << "ERROR: nEdgeClasses == "
                << nEdgeClasses << " at end of search!" << std::endl;
        for (unsigned i = 0; i < nTets * 6; i++) {
            if (edgeState[i].parent != -1)
                std::cerr << "ERROR: edgeState[" << i << "].parent == "
                    << edgeState[i].parent << " at end of search!"
                    << std::endl;
            if (edgeState[i].rank != 0)
                std::cerr << "ERROR: edgeState[" << i << "].rank == "
                    << edgeState[i].rank << " at end of search!" << std::endl;
            if (edgeState[i].size != 1)
                std::cerr << "ERROR: edgeState[" << i << "].size == "
                    << edgeState[i].size << " at end of search!" << std::endl;
            if (! edgeState[i].bounded)
                std::cerr << "ERROR: edgeState[" << i << "].bounded == "
                    "false at end of search!" << std::endl;
            if (edgeState[i].hadEqualRank)
                std::cerr << "ERROR: edgeState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (edgeStateChanged[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged[i] << " at end of search!"
                    << std::endl;

#if PRUNE_HIGH_DEG_EDGE_SET
        if (highDegSum != 0)
            std::cerr << "ERROR: highDegSum == " << highDegSum
                << " at end of search!" << std::endl;
#endif
    }

    use_(0, useArgs_);
}

void NClosedPrimeMinSearcher::dumpData(std::ostream& out) const {
    NCompactSearcher::dumpData(out);

    int i;
    for (i = 0; i < orderSize; i++) {
        if (i)
            out << ' ';
        out << orderType[i];
    }
    out << std::endl;

    out << nChainEdges << std::endl;
    if (nChainEdges) {
        for (i = 0; i < 2 * static_cast<int>(nChainEdges); i++) {
            if (i)
                out << ' ';
            out << chainPermIndices[i];
        }
        out << std::endl;
    }

#if PRUNE_HIGH_DEG_EDGE_SET
    out << highDegSum << ' ' << highDegBound << std::endl;
#endif
}

NClosedPrimeMinSearcher::NClosedPrimeMinSearcher(std::istream& in,
        UseGluingPerms use, void* useArgs) :
        NCompactSearcher(in, use, useArgs),
        orderType(0), nChainEdges(0), chainPermIndices(0) {
    if (inputError_)
        return;

    unsigned nTets = getNumberOfTetrahedra();
    int i;

    orderType = new unsigned[2 * nTets];
    for (i = 0; i < orderSize; i++)
        in >> orderType[i];

    in >> nChainEdges;
    /* Unnecessary since nChainEdges is unsigned.
    if (nChainEdges < 0) {
        inputError_ = true; return;
    } */
    if (nChainEdges) {
        chainPermIndices = new int[nChainEdges * 2];
        for (i = 0; i < 2 * static_cast<int>(nChainEdges); i++) {
            in >> chainPermIndices[i];
            if (chainPermIndices[i] < 0 || chainPermIndices[i] >= 6) {
                inputError_ = true; return;
            }
        }
    }

#if PRUNE_HIGH_DEG_EDGE_SET
    in >> highDegSum >> highDegBound;
    if (highDegSum < 0 || highDegSum > 6 * static_cast<int>(nTets) ||
            highDegBound != 3 * static_cast<int>(nTets) - 3) {
        inputError_ = true; return;
    }
#endif

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

int NClosedPrimeMinSearcher::mergeEdgeClasses() {
    NTetFace face = order[orderElt];
    NTetFace adj = (*pairing)[face];

    int retVal = 0;

    NPerm p = gluingPerm(face);
    int v1, w1, v2, w2;
    int e, f;
    int orderIdx;
    int eRep, fRep;
    int middleTet;

    v1 = face.face;
    w1 = p[v1];

    char parentTwists, hasTwist;
    for (v2 = 0; v2 < 4; v2++) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        // Look at the edge opposite v1-v2.
        e = 5 - NEdge::edgeNumber[v1][v2];
        f = 5 - NEdge::edgeNumber[w1][w2];

        orderIdx = v2 + 4 * orderElt;

        // We declare the natural orientation of an edge to be smaller
        // vertex to larger vertex.
        hasTwist = (p[NEdge::edgeVertex[e][0]] > p[NEdge::edgeVertex[e][1]] ?
            1 : 0);

        parentTwists = 0;
        eRep = findEdgeClass(e + 6 * face.tet, parentTwists);
        fRep = findEdgeClass(f + 6 * adj.tet, parentTwists);

        if (eRep == fRep) {
            edgeState[eRep].bounded = false;

            if (edgeState[eRep].size <= 2)
                retVal |= ECLASS_LOWDEG;
            else if (edgeState[eRep].size == 3) {
                // Flag as LOWDEG only if three distinct tetrahedra are used.
                middleTet = pairing->dest(face.tet, v2).tet;
                if (face.tet != adj.tet && adj.tet != middleTet &&
                        middleTet != face.tet)
                    retVal |= ECLASS_LOWDEG;
            }
            if (hasTwist ^ parentTwists)
                retVal |= ECLASS_TWISTED;

            edgeStateChanged[orderIdx] = -1;
        } else {
#if PRUNE_HIGH_DEG_EDGE_SET
            if (edgeState[eRep].size >= 3) {
                if (edgeState[fRep].size >= 3)
                    highDegSum += 3;
                else
                    highDegSum += edgeState[fRep].size;
            } else if (edgeState[fRep].size >= 3)
                highDegSum += edgeState[eRep].size;
            else if (edgeState[eRep].size == 2 && edgeState[fRep].size == 2)
                ++highDegSum;
#endif

            if (edgeState[eRep].rank < edgeState[fRep].rank) {
                // Join eRep beneath fRep.
                edgeState[eRep].parent = fRep;
                edgeState[eRep].twistUp = hasTwist ^ parentTwists;

                edgeState[fRep].size += edgeState[eRep].size;
#if PRUNE_HIGH_DEG_EDGE_SET
#else
                if (edgeState[fRep].size > 3 * getNumberOfTetrahedra())
                    retVal |= ECLASS_HIGHDEG;
#endif

                edgeStateChanged[orderIdx] = eRep;
            } else {
                // Join fRep beneath eRep.
                edgeState[fRep].parent = eRep;
                edgeState[fRep].twistUp = hasTwist ^ parentTwists;
                if (edgeState[eRep].rank == edgeState[fRep].rank) {
                    edgeState[eRep].rank++;
                    edgeState[fRep].hadEqualRank = true;
                }

                edgeState[eRep].size += edgeState[fRep].size;
#if PRUNE_HIGH_DEG_EDGE_SET
#else
                if (edgeState[eRep].size > 3 * getNumberOfTetrahedra())
                    retVal |= ECLASS_HIGHDEG;
#endif

                edgeStateChanged[orderIdx] = fRep;
            }

#if PRUNE_HIGH_DEG_EDGE_SET
            if (highDegSum > highDegBound)
                retVal |= ECLASS_HIGHDEG;
#endif

            nEdgeClasses--;
        }
    }

    // If we've already found something bad, exit now.  No sense in
    // looking for even more bad structures, since we're only going to
    // discard the triangulation anyway.
    if (retVal)
        return retVal;

    // Find representatives of the equivalence classes for all six edges
    // of the current tetrahedron (instead of calculating them each time
    // we want them).
    int tRep[6];
    char tTwist[6];
    for (e = 0; e < 6; e++)
        tRep[e] = findEdgeClass(e + 6 * face.tet, tTwist[e] = 0);

    // Test for cones in all possible positions on all possible faces.
    // Apologies for the tightness of the code; this part is being
    // micro-optimised since it is run so very frequently.  The old,
    // more readable version of this code is in the commented block below.
    for (e = 0; e < 12; e++)
        if (tRep[coneEdge[e][0]] == tRep[coneEdge[e][1]] && (coneNoTwist[e] ^
            (tTwist[coneEdge[e][0]] ^ tTwist[coneEdge[e][1]])))
                return ECLASS_CONE;

    /*
    // Test for cones on edges v1->w1->v2.
    for (w1 = 0; w1 < 4; w1++)
        for (v1 = 0; v1 < 3; v1++) {
            if (v1 == w1)
                continue;
            for (v2 = v1 + 1; v2 < 4; v2++) {
                if (v2 == w1)
                    continue;

                parentTwists = tTwist[NEdge::edgeNumber[v1][w1]] ^
                    tTwist[NEdge::edgeNumber[v2][w1]];

                if (tRep[NEdge::edgeNumber[v1][w1]] ==
                        tRep[NEdge::edgeNumber[v2][w1]]) {
                    hasTwist = (v1 < w1 && w1 < v2 ? 0 : 1);
                    if (hasTwist ^ parentTwists) {
                        return ECLASS_CONE;
                    }
                }
            }
        }
    */

    // Test for L(3,1) spines.
    // Don't bother checking the directions of the edges -- if it's not an
    // L(3,1) spine then it includes a cone, which we've already tested for.

    // L(3,1) on face 012:
    if (tRep[0] == tRep[1] && tRep[1] == tRep[3])
        return ECLASS_L31;
    // L(3,1) on face 013:
    if (tRep[0] == tRep[2] && tRep[2] == tRep[4])
        return ECLASS_L31;
    // L(3,1) on face 023:
    if (tRep[1] == tRep[2] && tRep[2] == tRep[5])
        return ECLASS_L31;
    // L(3,1) on face 123:
    if (tRep[3] == tRep[4] && tRep[4] == tRep[5])
        return ECLASS_L31;

    // Nothing bad was found.
    return 0;
}

void NClosedPrimeMinSearcher::splitEdgeClasses() {
    NTetFace face = order[orderElt];

    int v1, v2;
    int e;
    int eIdx, orderIdx;
    int rep, subRep;

    v1 = face.face;

    for (v2 = 3; v2 >= 0; v2--) {
        if (v2 == v1)
            continue;

        // Look at the edge opposite v1-v2.
        e = 5 - NEdge::edgeNumber[v1][v2];

        eIdx = e + 6 * face.tet;
        orderIdx = v2 + 4 * orderElt;

        if (edgeStateChanged[orderIdx] < 0)
            edgeState[findEdgeClass(eIdx)].bounded = true;
        else {
            subRep = edgeStateChanged[orderIdx];
            rep = edgeState[subRep].parent;

            edgeState[subRep].parent = -1;
            if (edgeState[subRep].hadEqualRank) {
                edgeState[subRep].hadEqualRank = false;
                edgeState[rep].rank--;
            }

            edgeState[rep].size -= edgeState[subRep].size;
#if PRUNE_HIGH_DEG_EDGE_SET
            if (edgeState[rep].size >= 3) {
                if (edgeState[subRep].size >= 3)
                    highDegSum -= 3;
                else
                    highDegSum -= edgeState[subRep].size;
            } else if (edgeState[subRep].size >= 3)
                highDegSum -= edgeState[rep].size;
            else if (edgeState[rep].size == 2 && edgeState[subRep].size == 2)
                --highDegSum;
#endif

            edgeStateChanged[orderIdx] = -1;
            nEdgeClasses++;
        }
    }
}

} // namespace regina

