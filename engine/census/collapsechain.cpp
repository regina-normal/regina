
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include <boost/next_prior.hpp>
#include <sstream>
#include "census/ngluingpermsearcher.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

const unsigned CollapsedChainSearcher::EDGE_CHAIN_END = 1;
const unsigned CollapsedChainSearcher::EDGE_CHAIN_INTERNAL_FIRST = 2;
const unsigned CollapsedChainSearcher::EDGE_CHAIN_INTERNAL_SECOND = 3;
const unsigned CollapsedChainSearcher::EDGE_DOUBLE_FIRST = 4;
const unsigned CollapsedChainSearcher::EDGE_DOUBLE_SECOND = 5;
const unsigned CollapsedChainSearcher::EDGE_MISC = 6;

const char CollapsedChainSearcher::ECLASS_TWISTED = 1;
const char CollapsedChainSearcher::ECLASS_LOWDEG = 2;
const char CollapsedChainSearcher::ECLASS_HIGHDEG = 4;
const char CollapsedChainSearcher::ECLASS_CONE = 8;
const char CollapsedChainSearcher::ECLASS_L31 = 16;

const unsigned CollapsedChainSearcher::coneEdge[12][2] = {
    { 0, 1 }, { 0, 2 }, { 1, 2 }, { 0, 3 }, { 0, 4 }, { 3, 4 },
    { 1, 3 }, { 1, 5 }, { 3, 5 }, { 2, 4 }, { 2, 5 }, { 4, 5 },
};

const char CollapsedChainSearcher::coneNoTwist[12] = {
    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1
};

const char CollapsedChainSearcher::dataTag_ = 'h';

CollapsedChainSearcher::CollapsedChainSearcher(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, bool orientableOnly,
        UseGluingPerms use, void* useArgs) :
        NCompactSearcher(pairing, autos, orientableOnly,
            PURGE_NON_MINIMAL_PRIME | PURGE_P2_REDUCIBLE,
            use, useArgs), modified(pairing) nChains(0) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    unsigned nTets = getNumberOfTetrahedra();

    // Taken from closedprimemin.cpp
    // Begin by searching for tetrahedra that are joined to themselves.
    // Note that each tetrahedra can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.

    nChains = 0;
    for (NTetFace face.setFirst(); ! face.isPastEnd(nTets, true); face++) {

        NTetFace adj = (*pairing)[face];
        if (adj.simp != face.simp)
            continue;
        collapseChain(NFacePair(face.face, adj.face));
    }

    // Begin by searching for tetrahedra that are joined to themselves.
    // Note that each tetrahedra can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.

    chainFaces = new NTetFace[nChains];
    chainLength = new int[nChains];


    // Continue by following each one-ended chain whose base was
    // identified in the previous loop.

    unsigned i;
    int tet;
    NTetFace dest1, dest2;
    NFacePair faces;
    for (i = 0; i < nChains; i++) {
        tet = order[i].simp;
        faces = NFacePair(order[i].facet,
            (*pairing)[order[i]].facet).complement();
        dest1 = pairing->dest(tet, faces.lower());
        dest2 = pairing->dest(tet, faces.upper());

        // Currently tet and faces refer to the two faces of the base
        // tetrahedron that are pointing outwards.
        while (dest1.simp == dest2.simp && dest1.simp != tet &&
                (! orderAssigned[tet * 4 + faces.lower()]) &&
                (! orderAssigned[tet * 4 + faces.upper()])) {
            // Insert this pair of edges into the ordering and follow
            // the chain.
            orderType[orderDone] = EDGE_CHAIN_INTERNAL_FIRST;
            orderType[orderDone + 1] = EDGE_CHAIN_INTERNAL_SECOND;

            if (tet < dest1.simp) {
                order[orderDone] = NTetFace(tet, faces.lower());
                order[orderDone + 1] = NTetFace(tet, faces.upper());
            }

            orderAssigned[tet * 4 + faces.lower()] = true;
            orderAssigned[tet * 4 + faces.upper()] = true;
            orderAssigned[dest1.simp * 4 + dest1.facet] = true;
            orderAssigned[dest2.simp * 4 + dest2.facet] = true;

            faces = NFacePair(dest1.facet, dest2.facet);

            if (dest1.simp < tet) {
                order[orderDone] = NTetFace(dest1.simp, faces.lower());
                order[orderDone + 1] = NTetFace(dest1.simp, faces.upper());
            }

            faces = faces.complement();
            tet = dest1.simp;

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
        if (! orderAssigned[face.simp * 4 + face.facet]) {
            order[orderDone] = face;
            if (face.facet < 3 && pairing->dest(boost::next(face)).simp ==
                    pairing->dest(face).simp)
                orderType[orderDone] = EDGE_DOUBLE_FIRST;
            else if (face.facet > 0 && pairing->dest(boost::prior(face)).simp ==
                    pairing->dest(face).simp)
                orderType[orderDone] = EDGE_DOUBLE_SECOND;
            else
                orderType[orderDone] = EDGE_MISC;
            orderDone++;

            adj = (*pairing)[face];
            orderAssigned[face.simp * 4 + face.facet] = true;
            orderAssigned[adj.simp * 4 + adj.facet] = true;
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
    NPerm4 trial1, trial2;
    for (i = 0; i < nChainEdges; i++) {
        if (orderType[i] == EDGE_CHAIN_END) {
            faces = NFacePair(order[i].facet, pairing->dest(order[i]).facet);
            comp = faces.complement();

            // order[i].facet == faces.lower(),
            // pairing->dest(order[i]).facet == faces.upper().
            chainPermIndices[2 * i] = gluingToIndex(order[i],
                NPerm4(faces.lower(), faces.upper(),
                       faces.upper(), comp.lower(),
                       comp.lower(), comp.upper(),
                       comp.upper(), faces.lower()));
            chainPermIndices[2 * i + 1] = gluingToIndex(order[i],
                NPerm4(faces.lower(), faces.upper(),
                       faces.upper(), comp.upper(),
                       comp.upper(), comp.lower(),
                       comp.lower(), faces.lower()));
        } else if (orderType[i] == EDGE_CHAIN_INTERNAL_FIRST) {
            faces = NFacePair(order[i].facet, order[i + 1].facet);
            comp = faces.complement();
            facesAdj = NFacePair(pairing->dest(order[i]).facet,
                pairing->dest(order[i + 1]).facet);
            compAdj = facesAdj.complement();

            // order[i].facet == faces.lower(),
            // order[i + 1].facet == faces.upper(),
            // pairing->dest(order[i]).facet == facesAdj.lower().
            // pairing->dest(order[i + 1]).facet == facesAdj.upper().
            trial1 = NPerm4(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.lower(),
                            comp.lower(), compAdj.upper(),
                            comp.upper(), facesAdj.upper());
            trial2 = NPerm4(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.upper(),
                            comp.lower(), compAdj.lower(),
                            comp.upper(), facesAdj.upper());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    NPerm4(faces.lower(), compAdj.upper(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), facesAdj.lower(),
                           comp.upper(), compAdj.lower()));
            } else {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    NPerm4(faces.lower(), compAdj.lower(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), facesAdj.lower(),
                           comp.upper(), compAdj.upper()));
            }

            trial1 = NPerm4(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.lower(),
                            comp.lower(), facesAdj.upper(),
                            comp.upper(), compAdj.upper());
            trial2 = NPerm4(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.upper(),
                            comp.lower(), facesAdj.upper(),
                            comp.upper(), compAdj.lower());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    NPerm4(faces.lower(), compAdj.upper(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), compAdj.lower(),
                           comp.upper(), facesAdj.lower()));
            } else {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    NPerm4(faces.lower(), compAdj.lower(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), compAdj.upper(),
                           comp.upper(), facesAdj.lower()));
            }
        }
    }

    // ---------- Tracking of vertex / edge equivalence classes ----------

#if PRUNE_HIGH_DEG_EDGE_SET
    // Only allow degree three edges if the face pairing graph supports
    // a (1,3,4) layered solid torus.  We can test this easily using the
    // precondition that the face pairing graph must be in canonical form.
    if (pairing->dest(0, 0).simp == 0 && pairing->dest(0, 2).simp == 1 &&
            pairing->dest(0, 3).simp == 1)
        highDegLimit = 3;
    else
        highDegLimit = 4;
    highDegSum = 0;
    highDegBound = (6 - highDegLimit) * nTets - highDegLimit;
#endif
}

// TODO (net): See what was removed when we brought in vertex link checking.
void CollapsedChainSearcher::runSearch(long maxDepth) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3
    //     The given face pairing contains a chain of length >= 1

    // Is it a partial search that has already finished?
    if (orderElt == static_cast<int>(nTets) * 2) {
        if (isCanonical())
            use_(this, useArgs_);
        use_(0, useArgs_);
        return;
    }

    signed int orderElt = 0; // Which chain we are up to
    signed int chainElt = 0; // Which element in the chain

    // TODO How to store permutation indices?
    signed int *permIndices[nChains * biggestChain];

    // for each triangulation found.
    // apply reverse isomorphism
    // for each chain, move the gluing on the loop of the collapsed chain to the
    // loop of the full chain. then:
    while (orderElt <= nChains) {
        // If we've finished this chain, move onto the next.
        if (chainTet.length() == chainElt) {
            orderElt += 1;
            continue;
        }
        // We've tried everything for this chain, go back a chain.
        if (chainElt == -1) {
            orderElt -= 1;
            continue;
        }

        permIndices[orderElt][chainElt] += 1;
        if (permIndices[orderElt][chainElt] == 3) {
            chainElt -= 1;
            continue;
        }

        glueUsing(permIndices[orderElt][chainElt]);
        chainElt += 1;
    }




    // ---------- Selecting the individual gluing permutations ----------

    // Observe that in a canonical face pairing, one-ended chains always
    // follow an increasing sequence of tetrahedra from boundary to end,
    // or follow the sequence of tetrahedra 0, 1, ..., k from end to
    // boundary.
    //
    // In particular, this means that for any tetrahedron not internal
    // to a one-ended chain (with the possible exception of tetrahedron
    // order[nChainEdges].simp), face 0 of this tetrahedron is not
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
        adj = (*pairing_)[face];

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
            if ((! orientableOnly_) || adj.facet == 0)
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
        permIndex(adj) = NPerm4::invS3[permIndex(face)];

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
        if (generic && adj.facet == 0 && orientableOnly_) {
            // It's the first time we've hit this tetrahedron.
            if ((permIndex(face) + (face.facet == 3 ? 0 : 1) +
                    (adj.facet == 3 ? 0 : 1)) % 2 == 0)
                orientation[adj.simp] = -orientation[face.simp];
            else
                orientation[adj.simp] = orientation[face.simp];
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
            if (orientableOnly_ && pairing_->dest(face).facet > 0) {
                // permIndex(face) will be set to -1 or -2 as appropriate.
                adj = (*pairing_)[face];
                if (orientation[face.simp] == orientation[adj.simp])
                    permIndex(face) = 1;
                else
                    permIndex(face) = 0;

                if ((face.facet == 3 ? 0 : 1) + (adj.facet == 3 ? 0 : 1) == 1)
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

void CollapsedChainSearcher::dumpData(std::ostream& out) const {
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
    out << highDegLimit << ' '
        << highDegSum << ' '
        << highDegBound << std::endl;
#endif
}

CollapsedChainSearcher::CollapsedChainSearcher(std::istream& in,
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
    in >> highDegLimit >> highDegSum >> highDegBound;
    if (highDegLimit < 3 || highDegLimit > 4 || highDegSum < 0 ||
            highDegSum > 6 * static_cast<int>(nTets) || highDegBound !=
                (6 - highDegLimit) * static_cast<int>(nTets) - highDegLimit) {
        inputError_ = true; return;
    }
#endif

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

void NCollapsedChainSearcher::collapseChain(NFacePair faces, int tet) {

    pairing->unMatch(tet,faces,lower()); // Unmatch loop
    faces = faces.complement();

    NTetFace dest1, dest2;
    dest1 = pairing->dest(tet, faces.lower());
    dest2 = pairing->dest(tet, faces.upper());
    // Currently tet and faces refer to the two faces of the base
    // tetrahedron that are pointing outwards.
    while (dest1.simp == dest2.simp && dest1.simp != tet) {
        pairing->unMatch(tet,faces,lower());
        pairing->unMatch(tet,faces,upper());
        faces = NFacePair(dest1.facet, dest2.facet).complement;

        // Store this tetrahedron as part of the chain.
        chainTet[nChains].append(tet);

        tet = dest1.simp;
        dest1 = pairing->dest(tet, faces.lower());
        dest2 = pairing->dest(tet, faces.upper());

    }
    pairing->match(dest1, dest2); // Add back the original loop.
    nChains+=1;
}

} // namespace regina

