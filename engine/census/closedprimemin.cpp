
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/next_prior.hpp>
#include <sstream>
#include "census/gluingpermsearcher3.h"
#include "triangulation/dim3.h"
#include "triangulation/facepair.h"
#include "utilities/memutils.h"

namespace regina {

const unsigned ClosedPrimeMinSearcher::EDGE_CHAIN_END = 1;
const unsigned ClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_FIRST = 2;
const unsigned ClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_SECOND = 3;
const unsigned ClosedPrimeMinSearcher::EDGE_DOUBLE_FIRST = 4;
const unsigned ClosedPrimeMinSearcher::EDGE_DOUBLE_SECOND = 5;
const unsigned ClosedPrimeMinSearcher::EDGE_MISC = 6;

const char ClosedPrimeMinSearcher::ECLASS_TWISTED = 1;
const char ClosedPrimeMinSearcher::ECLASS_LOWDEG = 2;
const char ClosedPrimeMinSearcher::ECLASS_HIGHDEG = 4;
const char ClosedPrimeMinSearcher::ECLASS_CONE = 8;
const char ClosedPrimeMinSearcher::ECLASS_L31 = 16;

const unsigned ClosedPrimeMinSearcher::coneEdge[12][2] = {
    { 0, 1 }, { 0, 2 }, { 1, 2 }, { 0, 3 }, { 0, 4 }, { 3, 4 },
    { 1, 3 }, { 1, 5 }, { 3, 5 }, { 2, 4 }, { 2, 5 }, { 4, 5 },
};

const char ClosedPrimeMinSearcher::coneNoTwist[12] = {
    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1
};

const char ClosedPrimeMinSearcher::dataTag_ = 'c';

ClosedPrimeMinSearcher::ClosedPrimeMinSearcher(const FacetPairing<3>* pairing,
        const FacetPairing<3>::IsoList* autos, bool orientableOnly,
        GluingPermSearcher<3>::Use use, void* useArgs) :
        CompactSearcher(pairing, autos, orientableOnly,
            PURGE_NON_MINIMAL_PRIME | PURGE_P2_REDUCIBLE,
            use, useArgs) {
    // Initialise internal arrays, specifically those relating to face
    // orderings and properties of chains, to accurately reflect the
    // underlying face pairing.
    //
    // Although the GluingPermSearcher<3> constructor initialises the order[]
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

    unsigned nTets = size();

    orderType = new unsigned[nTets * 2];

    bool* orderAssigned = new bool[nTets * 4];
        /**< Have we placed a tetrahedron face or its partner in the
             order[] array yet? */

    // Hunt for structures within the face pairing graph.

    FacetSpec<3> face, adj;
    unsigned orderDone = 0;
    std::fill(orderAssigned, orderAssigned + 4 * nTets, false);

    // Begin by searching for tetrahedra that are joined to themselves.
    // Note that each tetrahedra can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.

    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++) {
        if (orderAssigned[face.simp * 4 + face.facet])
            continue;

        adj = (*pairing)[face];
        if (adj.simp != face.simp)
            continue;

        order[orderDone] = face;
        orderType[orderDone] = EDGE_CHAIN_END;
        orderAssigned[face.simp * 4 + face.facet] = true;
        orderAssigned[adj.simp * 4 + adj.facet] = true;
        orderDone++;
    }

    // Record the number of one-ended chains.

    unsigned nChains = orderDone;

    // Continue by following each one-ended chain whose base was
    // identified in the previous loop.

    unsigned i;
    int tet;
    FacetSpec<3> dest1, dest2;
    FacePair faces;
    for (i = 0; i < nChains; i++) {
        tet = order[i].simp;
        faces = FacePair(order[i].facet,
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
                order[orderDone] = FacetSpec<3>(tet, faces.lower());
                order[orderDone + 1] = FacetSpec<3>(tet, faces.upper());
            }

            orderAssigned[tet * 4 + faces.lower()] = true;
            orderAssigned[tet * 4 + faces.upper()] = true;
            orderAssigned[dest1.simp * 4 + dest1.facet] = true;
            orderAssigned[dest2.simp * 4 + dest2.facet] = true;

            faces = FacePair(dest1.facet, dest2.facet);

            if (dest1.simp < tet) {
                order[orderDone] = FacetSpec<3>(dest1.simp, faces.lower());
                order[orderDone + 1] = FacetSpec<3>(dest1.simp, faces.upper());
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

    FacePair facesAdj, comp, compAdj;
    Perm<4> trial1, trial2;
    for (i = 0; i < nChainEdges; i++) {
        if (orderType[i] == EDGE_CHAIN_END) {
            faces = FacePair(order[i].facet, pairing->dest(order[i]).facet);
            comp = faces.complement();

            // order[i].facet == faces.lower(),
            // pairing->dest(order[i]).facet == faces.upper().
            chainPermIndices[2 * i] = gluingToIndex(order[i],
                Perm<4>(faces.lower(), faces.upper(),
                       faces.upper(), comp.lower(),
                       comp.lower(), comp.upper(),
                       comp.upper(), faces.lower()));
            chainPermIndices[2 * i + 1] = gluingToIndex(order[i],
                Perm<4>(faces.lower(), faces.upper(),
                       faces.upper(), comp.upper(),
                       comp.upper(), comp.lower(),
                       comp.lower(), faces.lower()));
        } else if (orderType[i] == EDGE_CHAIN_INTERNAL_FIRST) {
            faces = FacePair(order[i].facet, order[i + 1].facet);
            comp = faces.complement();
            facesAdj = FacePair(pairing->dest(order[i]).facet,
                pairing->dest(order[i + 1]).facet);
            compAdj = facesAdj.complement();

            // order[i].facet == faces.lower(),
            // order[i + 1].facet == faces.upper(),
            // pairing->dest(order[i]).facet == facesAdj.lower().
            // pairing->dest(order[i + 1]).facet == facesAdj.upper().
            trial1 = Perm<4>(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.lower(),
                            comp.lower(), compAdj.upper(),
                            comp.upper(), facesAdj.upper());
            trial2 = Perm<4>(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.upper(),
                            comp.lower(), compAdj.lower(),
                            comp.upper(), facesAdj.upper());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    Perm<4>(faces.lower(), compAdj.upper(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), facesAdj.lower(),
                           comp.upper(), compAdj.lower()));
            } else {
                chainPermIndices[2 * i] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 2] = gluingToIndex(order[i + 1],
                    Perm<4>(faces.lower(), compAdj.lower(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), facesAdj.lower(),
                           comp.upper(), compAdj.upper()));
            }

            trial1 = Perm<4>(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.lower(),
                            comp.lower(), facesAdj.upper(),
                            comp.upper(), compAdj.upper());
            trial2 = Perm<4>(faces.lower(), facesAdj.lower(),
                            faces.upper(), compAdj.upper(),
                            comp.lower(), facesAdj.upper(),
                            comp.upper(), compAdj.lower());
            if (trial1.compareWith(trial2) < 0) {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial1);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    Perm<4>(faces.lower(), compAdj.upper(),
                           faces.upper(), facesAdj.upper(),
                           comp.lower(), compAdj.lower(),
                           comp.upper(), facesAdj.lower()));
            } else {
                chainPermIndices[2 * i + 1] = gluingToIndex(order[i], trial2);
                chainPermIndices[2 * i + 3] = gluingToIndex(order[i + 1],
                    Perm<4>(faces.lower(), compAdj.lower(),
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
void ClosedPrimeMinSearcher::runSearch(long maxDepth) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    unsigned nTets = size();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTets * 4 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Begin by testing for face pairings that can never lead to such a
        // triangulation.
        if (pairing_->hasTripleEdge() ||
                pairing_->hasBrokenDoubleEndedChain() ||
                pairing_->hasOneEndedChainWithDoubleHandle() ||
                pairing_->hasOneEndedChainWithStrayBigon() ||
                pairing_->hasWedgedDoubleEndedChain() ||
                pairing_->hasTripleOneEndedChain()) {
            use_(0, useArgs_);
            return;
        }

        orderElt = 0;
        if (nChainEdges < nTets * 2)
            orientation[order[nChainEdges].simp] = 1;
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
    // order[nChainEdges].simp), face 0 of this tetrahedron is not
    // involved in a one-ended chain.

    // In this generation algorithm, each orientation is simply +/-1.
    // We won't bother assigning orientations to the tetrahedra internal
    // to the one-ended chains.

    int minOrder = orderElt;
    int maxOrder = orderElt + maxDepth;

    FacetSpec<3> face, adj;
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
        permIndex(adj) = Perm<3>::invS3[permIndex(face)];

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

void ClosedPrimeMinSearcher::dumpData(std::ostream& out) const {
    CompactSearcher::dumpData(out);

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

ClosedPrimeMinSearcher::ClosedPrimeMinSearcher(std::istream& in,
        GluingPermSearcher<3>::Use use, void* useArgs) :
        CompactSearcher(in, use, useArgs),
        orderType(0), nChainEdges(0), chainPermIndices(0) {
    if (inputError_)
        return;

    unsigned nTets = size();
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

int ClosedPrimeMinSearcher::mergeEdgeClasses() {
    FacetSpec<3> face = order[orderElt];
    FacetSpec<3> adj = (*pairing_)[face];

    int retVal = 0;

    Perm<4> p = gluingPerm(face);
    int v1, w1, v2, w2;
    int e, f;
    int orderIdx;
    int eRep, fRep;
    int middleTet;

    v1 = face.facet;
    w1 = p[v1];

    char parentTwists, hasTwist;
    for (v2 = 0; v2 < 4; v2++) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        // Look at the edge opposite v1-v2.
        e = 5 - Edge<3>::edgeNumber[v1][v2];
        f = 5 - Edge<3>::edgeNumber[w1][w2];

        orderIdx = v2 + 4 * orderElt;

        // We declare the natural orientation of an edge to be smaller
        // vertex to larger vertex.
        hasTwist = (p[Edge<3>::edgeVertex[e][0]] > p[Edge<3>::edgeVertex[e][1]] ?
            1 : 0);

        parentTwists = 0;
        eRep = findEdgeClass(e + 6 * face.simp, parentTwists);
        fRep = findEdgeClass(f + 6 * adj.simp, parentTwists);

        if (eRep == fRep) {
            edgeState[eRep].bounded = false;

            if (edgeState[eRep].size <= 2)
                retVal |= ECLASS_LOWDEG;
            else if (edgeState[eRep].size == 3) {
                // Flag as LOWDEG only if three distinct tetrahedra are used.
                middleTet = pairing_->dest(face.simp, v2).simp;
                if (face.simp != adj.simp && adj.simp != middleTet &&
                        middleTet != face.simp)
                    retVal |= ECLASS_LOWDEG;
            }
            if (hasTwist ^ parentTwists)
                retVal |= ECLASS_TWISTED;

            edgeStateChanged[orderIdx] = -1;
        } else {
#if PRUNE_HIGH_DEG_EDGE_SET
            if (edgeState[eRep].size >= highDegLimit) {
                if (edgeState[fRep].size >= highDegLimit)
                    highDegSum += highDegLimit;
                else
                    highDegSum += edgeState[fRep].size;
            } else if (edgeState[fRep].size >= highDegLimit)
                highDegSum += edgeState[eRep].size;
            else if (edgeState[eRep].size + edgeState[fRep].size >
                    highDegLimit)
                highDegSum += (edgeState[eRep].size + edgeState[fRep].size -
                    highDegLimit);
#endif

            if (edgeState[eRep].rank < edgeState[fRep].rank) {
                // Join eRep beneath fRep.
                edgeState[eRep].parent = fRep;
                edgeState[eRep].twistUp = hasTwist ^ parentTwists;

                edgeState[fRep].size += edgeState[eRep].size;
#if PRUNE_HIGH_DEG_EDGE_SET
#else
                if (edgeState[fRep].size > 3 * size())
                    retVal |= ECLASS_HIGHDEG;
#endif

                if (edgeState[eRep].twistUp) {
                    edgeState[fRep].facesPos += edgeState[eRep].facesNeg;
                    edgeState[fRep].facesNeg += edgeState[eRep].facesPos;
                } else {
                    edgeState[fRep].facesPos += edgeState[eRep].facesPos;
                    edgeState[fRep].facesNeg += edgeState[eRep].facesNeg;
                }
                if (edgeState[fRep].facesPos.hasNonZeroMatch(
                        edgeState[fRep].facesNeg))
                    retVal |= ECLASS_CONE;
                if (edgeState[fRep].facesPos.has3() ||
                        edgeState[fRep].facesNeg.has3())
                    retVal |= ECLASS_L31;

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
                if (edgeState[eRep].size > 3 * size())
                    retVal |= ECLASS_HIGHDEG;
#endif

                if (edgeState[fRep].twistUp) {
                    edgeState[eRep].facesPos += edgeState[fRep].facesNeg;
                    edgeState[eRep].facesNeg += edgeState[fRep].facesPos;
                } else {
                    edgeState[eRep].facesPos += edgeState[fRep].facesPos;
                    edgeState[eRep].facesNeg += edgeState[fRep].facesNeg;
                }
                if (edgeState[eRep].facesPos.hasNonZeroMatch(
                        edgeState[eRep].facesNeg))
                    retVal |= ECLASS_CONE;
                if (edgeState[eRep].facesPos.has3() ||
                        edgeState[eRep].facesNeg.has3())
                    retVal |= ECLASS_L31;

                edgeStateChanged[orderIdx] = fRep;
            }

#if PRUNE_HIGH_DEG_EDGE_SET
            if (highDegSum > highDegBound)
                retVal |= ECLASS_HIGHDEG;
#endif

            nEdgeClasses--;
        }
    }

    return retVal;
}

void ClosedPrimeMinSearcher::splitEdgeClasses() {
    FacetSpec<3> face = order[orderElt];

    int v1, v2;
    int e;
    int eIdx, orderIdx;
    int rep, subRep;

    v1 = face.facet;

    for (v2 = 3; v2 >= 0; v2--) {
        if (v2 == v1)
            continue;

        // Look at the edge opposite v1-v2.
        e = 5 - Edge<3>::edgeNumber[v1][v2];

        eIdx = e + 6 * face.simp;
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
            if (edgeState[rep].size >= highDegLimit) {
                if (edgeState[subRep].size >= highDegLimit)
                    highDegSum -= highDegLimit;
                else
                    highDegSum -= edgeState[subRep].size;
            } else if (edgeState[subRep].size >= highDegLimit)
                highDegSum -= edgeState[rep].size;
            else if (edgeState[rep].size + edgeState[subRep].size >
                    highDegLimit)
                highDegSum -= (edgeState[rep].size + edgeState[subRep].size
                    - highDegLimit);
#endif

            if (edgeState[subRep].twistUp) {
                edgeState[rep].facesPos -= edgeState[subRep].facesNeg;
                edgeState[rep].facesNeg -= edgeState[subRep].facesPos;
            } else {
                edgeState[rep].facesPos -= edgeState[subRep].facesPos;
                edgeState[rep].facesNeg -= edgeState[subRep].facesNeg;
            }

            edgeStateChanged[orderIdx] = -1;
            nEdgeClasses++;
        }
    }
}

} // namespace regina

