
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
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/boostutils.h"
#include "utilities/memutils.h"

// Sanity checking for the fast-vs-slow algorithm macros.
#ifdef NO_VERTEX_LINK_PRUNING
#define NO_VERTEX_COUNT_PRUNING
#endif

#ifdef NO_EDGE_CLASS_PRUNING
#define NO_EDGE_COUNT_PRUNING
#define NO_CONE_PRUNING
#define NO_L31_PRUNING
#endif

// Simplify the many splitVertexClasses() and splitEdgeClasses() calls.
#ifdef NO_VERTEX_LINK_PRUNING
#define SPLIT_VERTEX_CLASSES
#else
#define SPLIT_VERTEX_CLASSES splitVertexClasses();
#endif

#ifdef NO_EDGE_CLASS_PRUNING
#define SPLIT_EDGE_CLASSES
#else
#define SPLIT_EDGE_CLASSES splitEdgeClasses();
#endif

#define SPLIT_VERTEX_EDGE_CLASSES SPLIT_VERTEX_CLASSES SPLIT_EDGE_CLASSES

namespace regina {

const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_END = 1;
const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_FIRST = 2;
const unsigned NClosedPrimeMinSearcher::EDGE_CHAIN_INTERNAL_SECOND = 3;
const unsigned NClosedPrimeMinSearcher::EDGE_DOUBLE_FIRST = 4;
const unsigned NClosedPrimeMinSearcher::EDGE_DOUBLE_SECOND = 5;
const unsigned NClosedPrimeMinSearcher::EDGE_MISC = 6;

#ifndef NO_VERTEX_LINK_PRUNING
const char NClosedPrimeMinSearcher::VLINK_CLOSED = 1;
const char NClosedPrimeMinSearcher::VLINK_NON_ORBL = 2;
#endif

#ifndef NO_EDGE_CLASS_PRUNING
const char NClosedPrimeMinSearcher::ECLASS_TWISTED = 1;
const char NClosedPrimeMinSearcher::ECLASS_LOWDEG = 2;
const char NClosedPrimeMinSearcher::ECLASS_CONE = 4;
const char NClosedPrimeMinSearcher::ECLASS_L31 = 8;
#endif

const char NClosedPrimeMinSearcher::dataTag_ = 'c';

#ifndef NO_VERTEX_LINK_PRUNING
void NClosedPrimeMinSearcher::TetVertexState::dumpData(std::ostream& out)
        const {
    // Be careful with twistUp, which is a char but which should be
    // written as an int.
    out << parent << ' ' << rank << ' ' << bdry << ' '
        << (twistUp ? 1 : 0) << ' ' << (hadEqualRank ? 1 : 0);
}

bool NClosedPrimeMinSearcher::TetVertexState::readData(std::istream& in,
        unsigned long nStates) {
    in >> parent >> rank >> bdry;

    // twistUp is a char, but we need to read it as an int.
    int twist;
    in >> twist;
    twistUp = twist;

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    if (parent < -1 || parent >= static_cast<long>(nStates))
        return false;
    if (rank >= nStates)
        return false;
    if (bdry > 3 * nStates)
        return false;
    if (twist != 1 && twist != 0)
        return false;
    if (bRank != 1 && bRank != 0)
        return false;

    return true;
}
#endif

#ifndef NO_EDGE_CLASS_PRUNING
void NClosedPrimeMinSearcher::TetEdgeState::dumpData(std::ostream& out) {
    // Be careful with twistUp, which is a char but which should be
    // written as an int.
    out << parent << ' ' << rank << ' ' << size << ' '
        << (bounded ? 1 : 0) << ' ' << (twistUp ? 1 : 0) << ' '
        << (hadEqualRank ? 1 : 0);
}

bool NClosedPrimeMinSearcher::TetEdgeState::readData(std::istream& in,
        unsigned long nStates) {
    in >> parent >> rank >> size;

    // bounded is a bool, but we need to read it as an int.
    int bBounded;
    in >> bBounded;
    bounded = bBounded;

    // twistUp is a char, but we need to read it as an int.
    int twist;
    in >> twist;
    twistUp = twist;

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    if (parent < -1 || parent >= static_cast<long>(nStates))
        return false;
    if (rank >= nStates)
        return false;
    if (size >= nStates)
        return false;
    if (bBounded != 1 && bBounded != 0)
        return false;
    if (twist != 1 && twist != 0)
        return false;
    if (bRank != 1 && bRank != 0)
        return false;

    return true;
}
#endif

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

    // ---------- Tracking of vertex / edge equivalence classes ----------

#ifndef NO_VERTEX_LINK_PRUNING
    nVertexClasses = nTets * 4;
    vertexState = new TetVertexState[nTets * 4];
    vertexStateChanged = new int[nTets * 8];
    std::fill(vertexStateChanged, vertexStateChanged + nTets * 8, -1);
#endif

#ifndef NO_EDGE_CLASS_PRUNING
    nEdgeClasses = nTets * 6;
    edgeState = new TetEdgeState[nTets * 6];
    edgeStateChanged = new int[nTets * 8];
    std::fill(edgeStateChanged, edgeStateChanged + nTets * 8, -1);
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
        if (pairing->hasTripleEdge() || pairing->hasBrokenDoubleEndedChain() ||
                pairing->hasOneEndedChainWithDoubleHandle()) {
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
    NFacePair faces;
    NPerm trial1, trial2;
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

            // Pull apart vertex and edge links at the previous level.
            if (orderElt >= minOrder) {
                SPLIT_VERTEX_EDGE_CLASSES
            }

            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = allPermsS3Inv[permIndex(face)];

#ifdef NO_EDGE_CLASS_PRUNING
        // Simple tests that help if we're not already tracking
        // equivalence classes of tetrahedron edges.
        if (lowDegreeEdge(face, true, true))
            continue;
        if (! orientableOnly_)
            if (badEdgeLink(face))
                continue;
#endif

#if (defined(NO_EDGE_CLASS_PRUNING) || defined(NO_CONE_PRUNING))
        // Exploit our results regarding double edges in a face pairing graph.
        //
        // If we're already tracking equivalence classes of tetrahedron
        // edges and purging on (i) low degree edges, (ii) invalid edges
        // and (iii) conical faces, then these results add nothing new
        // (in which case there is no need for these additional tests).
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
            // low degree edge and invalid edge tests already performed
            // elsewhere.

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
#endif

#ifndef NO_EDGE_CLASS_PRUNING
        // Merge edge links and run corresponding tests.
        mergeResult = mergeEdgeClasses();
        if (mergeResult &
                (ECLASS_TWISTED | ECLASS_LOWDEG | ECLASS_CONE | ECLASS_L31)) {
            // We closed off an edge link, resulting in a low-degree or
            // invalid edge.
            SPLIT_EDGE_CLASSES
            continue;
        }
#ifndef NO_EDGE_COUNT_PRUNING
        // The final triangulation should have precisely (nTets + 1) edges
        // (since it must have precisely one vertex).
        if (nEdgeClasses < nTets + 1) {
            // We already have too few edge classes, and the count can
            // only get smaller.
            // Note that the triangulations we are pruning include ideal
            // triangulations (with vertex links of Euler characteristic < 2).
            SPLIT_EDGE_CLASSES
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
            SPLIT_EDGE_CLASSES
            continue;
        }
#endif
#endif

#ifndef NO_VERTEX_LINK_PRUNING
        // Merge vertex links and run corresponding tests.
        mergeResult = mergeVertexClasses();
#ifndef NO_VERTEX_COUNT_PRUNING
        if (mergeResult & VLINK_CLOSED) {
            // We closed off a vertex link, which means we will end up
            // with more than one vertex (unless this was our very last
            // gluing).
            if (orderElt + 1 < static_cast<int>(nTets) * 2) {
                SPLIT_VERTEX_EDGE_CLASSES
                continue;
            }
        }
#endif
        if (mergeResult & VLINK_NON_ORBL) {
            // We made the vertex link non-orientable.  Stop now.
            SPLIT_VERTEX_EDGE_CLASSES
            continue;
        }
#ifndef NO_VERTEX_COUNT_PRUNING
        if (nVertexClasses > 1 + 3 * (nTets * 2 - orderElt - 1)) {
            // We have (2n - orderElt - 1) more gluings to choose.
            // Since each merge can reduce the number of vertex classes
            // by at most 3, there is no way we can end up with just one
            // vertex at the end.
            SPLIT_VERTEX_EDGE_CLASSES
            continue;
        }
#endif
#endif

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
                SPLIT_VERTEX_EDGE_CLASSES
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
                    SPLIT_VERTEX_EDGE_CLASSES
                }
            }
        }
    }

    // And the search is over.

    // Some extra sanity checking.
    if (minOrder == 0) {
#ifndef NO_VERTEX_LINK_PRUNING
        // Our vertex classes had better be 4n standalone vertices.
        if (nVertexClasses != 4 * nTets)
            std::cerr << "ERROR: nVertexClasses == "
                << nVertexClasses << " at end of search!" << std::endl;
        for (unsigned i = 0; i < nTets * 4; i++) {
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
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (vertexStateChanged[i] != -1)
                std::cerr << "ERROR: vertexStateChanged[" << i << "] == "
                    << vertexStateChanged[i] << " at end of search!"
                    << std::endl;
#endif

#ifndef NO_EDGE_CLASS_PRUNING
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
                    << edgeState[i].bounded << " at end of search!"
                    << std::endl;
            if (edgeState[i].hadEqualRank)
                std::cerr << "ERROR: edgeState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (edgeStateChanged[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged[i] << " at end of search!"
                    << std::endl;
#endif
    }

    use_(0, useArgs_);
}

void NClosedPrimeMinSearcher::dumpData(std::ostream& out) const {
    NGluingPermSearcher::dumpData(out);

    unsigned nTets = getNumberOfTetrahedra();
    unsigned i;

    for (i = 0; i < 2 * nTets; i++) {
        if (i)
            out << ' ';
        out << order[i].tet << ' ' << order[i].face << ' ' << orderType[i];
    }
    out << std::endl;

    out << nChainEdges << std::endl;
    if (nChainEdges) {
        for (i = 0; i < 2 * nChainEdges; i++) {
            if (i)
                out << ' ';
            out << chainPermIndices[i];
        }
        out << std::endl;
    }

    out << orderElt << std::endl;

#ifndef NO_VERTEX_LINK_PRUNING
    out << nVertexClasses << std::endl;
    for (i = 0; i < 4 * nTets; i++) {
        vertexState[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 8 * nTets; i++) {
        if (i)
            out << ' ';
        out << vertexStateChanged[i];
    }
    out << std::endl;
#endif

#ifndef NO_EDGE_CLASS_PRUNING
    out << nEdgeClasses << std::endl;
    for (i = 0; i < 6 * nTets; i++) {
        edgeState[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 8 * nTets; i++) {
        if (i)
            out << ' ';
        out << edgeStateChanged[i];
    }
    out << std::endl;
#endif
}

NClosedPrimeMinSearcher::NClosedPrimeMinSearcher(std::istream& in,
        UseGluingPerms use, void* useArgs) :
        NGluingPermSearcher(in, use, useArgs),
        order(0), orderType(0), nChainEdges(0), chainPermIndices(0),
#ifndef NO_VERTEX_LINK_PRUNING
        nVertexClasses(0), vertexState(0), vertexStateChanged(0),
#endif
#ifndef NO_EDGE_CLASS_PRUNING
        nEdgeClasses(0), edgeState(0), edgeStateChanged(0),
#endif
        orderElt(0) {
    if (inputError_)
        return;

    unsigned nTets = getNumberOfTetrahedra();
    unsigned i;

    order = new NTetFace[2 * nTets];
    orderType = new unsigned[nTets * 2];
    for (i = 0; i < 2 * nTets; i++) {
        in >> order[i].tet >> order[i].face >> orderType[i];
        if (order[i].tet >= static_cast<int>(nTets) || order[i].tet < 0 ||
                order[i].face >= 4 || order[i].face < 0) {
            inputError_ = true; return;
        }
    }

    in >> nChainEdges;
    /* Unnecessary since nChainEdges is unsigned.
    if (nChainEdges < 0) {
        inputError_ = true; return;
    } */
    if (nChainEdges) {
        chainPermIndices = new int[nChainEdges * 2];
        for (i = 0; i < 2 * nChainEdges; i++) {
            in >> chainPermIndices[i];
            if (chainPermIndices[i] < 0 || chainPermIndices[i] >= 6) {
                inputError_ = true; return;
            }
        }
    }

    in >> orderElt;

#ifndef NO_VERTEX_LINK_PRUNING
    in >> nVertexClasses;
    if (nVertexClasses > 4 * nTets) {
        inputError_ = true; return;
    }

    vertexState = new TetVertexState[4 * nTets];
    for (i = 0; i < 4 * nTets; i++)
        if (! vertexState[i].readData(in, 4 * nTets)) {
            inputError_ = true; return;
        }

    vertexStateChanged = new int[8 * nTets];
    for (i = 0; i < 8 * nTets; i++) {
        in >> vertexStateChanged[i];
        if (vertexStateChanged[i] < -1 ||
                 vertexStateChanged[i] >= 4 * static_cast<int>(nTets)) {
            inputError_ = true; return;
        }
    }
#endif

#ifndef NO_EDGE_CLASS_PRUNING
    in >> nEdgeClasses;
    if (nEdgeClasses > 6 * nTets) {
        inputError_ = true; return;
    }

    edgeState = new TetEdgeState[6 * nTets];
    for (i = 0; i < 6 * nTets; i++)
        if (! edgeState[i].readData(in, 6 * nTets)) {
            inputError_ = true; return;
        }

    edgeStateChanged = new int[8 * nTets];
    for (i = 0; i < 8 * nTets; i++) {
        in >> edgeStateChanged[i];
        if (edgeStateChanged[i] < -1 ||
                 edgeStateChanged[i] >= 6 * static_cast<int>(nTets)) {
            inputError_ = true; return;
        }
    }
#endif

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

#ifndef NO_VERTEX_LINK_PRUNING
int NClosedPrimeMinSearcher::mergeVertexClasses() {
    // Merge all three vertex pairs for the current face.
    NTetFace face = order[orderElt];
    NTetFace adj = (*pairing)[face];

    int retVal = 0;

    int v, w;
    unsigned vIdx, wIdx, orderIdx;
    int vRep, wRep;
    NPerm p = gluingPerm(face);
    char parentTwists, hasTwist;
    for (v = 0; v < 4; v++) {
        if (v == face.face)
            continue;

        w = p[v];
        vIdx = v + 4 * face.tet;
        wIdx = w + 4 * adj.tet;
        orderIdx = v + 4 * orderElt;

        // Are the natural 012 representations of the two faces joined
        // with reversed orientations?
        // Here we combine the sign of permutation p with the mappings
        // from 012 to the native tetrahedron vertices, i.e., v <-> 3 and
        // w <-> 3.
        hasTwist = (p.sign() < 0 ? 0 : 1);
        if ((v == 3 && w != 3) || (v != 3 && w == 3))
            hasTwist ^= 1;

        parentTwists = 0;
        for (vRep = vIdx; vertexState[vRep].parent >= 0;
                vRep = vertexState[vRep].parent)
            parentTwists ^= vertexState[vRep].twistUp;
        for (wRep = wIdx; vertexState[wRep].parent >= 0;
                wRep = vertexState[wRep].parent)
            parentTwists ^= vertexState[wRep].twistUp;

        if (vRep == wRep) {
            vertexState[vRep].bdry -= 2;
            if (vertexState[vRep].bdry == 0)
                retVal |= VLINK_CLOSED;
            if (hasTwist ^ parentTwists)
                retVal |= VLINK_NON_ORBL;

            vertexStateChanged[orderIdx] = -1;
        } else {
            if (vertexState[vRep].rank < vertexState[wRep].rank) {
                // Join vRep beneath wRep.
                vertexState[vRep].parent = wRep;
                vertexState[vRep].twistUp = hasTwist ^ parentTwists;

                vertexState[wRep].bdry = vertexState[wRep].bdry +
                    vertexState[vRep].bdry - 2;
                if (vertexState[wRep].bdry == 0)
                    retVal |= VLINK_CLOSED;

                vertexStateChanged[orderIdx] = vRep;
            } else {
                // Join wRep beneath vRep.
                vertexState[wRep].parent = vRep;
                vertexState[wRep].twistUp = hasTwist ^ parentTwists;
                if (vertexState[vRep].rank == vertexState[wRep].rank) {
                    vertexState[vRep].rank++;
                    vertexState[wRep].hadEqualRank = true;
                }

                vertexState[vRep].bdry = vertexState[vRep].bdry +
                    vertexState[wRep].bdry - 2;
                if (vertexState[vRep].bdry == 0)
                    retVal |= VLINK_CLOSED;

                vertexStateChanged[orderIdx] = wRep;
            }

            nVertexClasses--;
        }
    }

    return retVal;
}

void NClosedPrimeMinSearcher::splitVertexClasses() {
    // Split all three vertex pairs for the current face.
    NTetFace face = order[orderElt];

    int v;
    unsigned vIdx, orderIdx;
    int rep, subRep;
    // Do everything in reverse.  This includes the loop over vertices.
    for (v = 3; v >= 0; v--) {
        if (v == face.face)
            continue;

        vIdx = v + 4 * face.tet;
        orderIdx = v + 4 * orderElt;

        if (vertexStateChanged[orderIdx] < 0) {
            for (rep = vIdx; vertexState[rep].parent >= 0;
                    rep = vertexState[rep].parent)
                ;
            vertexState[rep].bdry += 2;
        } else {
            subRep = vertexStateChanged[orderIdx];
            rep = vertexState[subRep].parent;

            vertexState[subRep].parent = -1;
            if (vertexState[subRep].hadEqualRank) {
                vertexState[subRep].hadEqualRank = false;
                vertexState[rep].rank--;
            }

            vertexState[rep].bdry = vertexState[rep].bdry + 2 -
                vertexState[subRep].bdry;

            vertexStateChanged[orderIdx] = -1;
            nVertexClasses++;
        }
    }
}
#endif

#ifndef NO_EDGE_CLASS_PRUNING
int NClosedPrimeMinSearcher::mergeEdgeClasses() {
    NTetFace face = order[orderElt];
    NTetFace adj = (*pairing)[face];

    int retVal = 0;

    NPerm p = gluingPerm(face);
    int v1, w1, v2, w2;
    int e, f;
    int eIdx, fIdx, orderIdx;
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
        e = 5 - edgeNumber[v1][v2];
        f = 5 - edgeNumber[w1][w2];

        eIdx = e + 6 * face.tet;
        fIdx = f + 6 * adj.tet;
        orderIdx = v2 + 4 * orderElt;

        // We declare the natural orientation of an edge to be smaller
        // vertex to larger vertex.
        hasTwist = (p[edgeStart[e]] > p[edgeEnd[e]] ? 1 : 0);

        parentTwists = 0;
        for (eRep = eIdx; edgeState[eRep].parent >= 0;
                eRep = edgeState[eRep].parent)
            parentTwists ^= edgeState[eRep].twistUp;
        for (fRep = fIdx; edgeState[fRep].parent >= 0;
                fRep = edgeState[fRep].parent)
            parentTwists ^= edgeState[fRep].twistUp;

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
            if (edgeState[eRep].rank < edgeState[fRep].rank) {
                // Join eRep beneath fRep.
                edgeState[eRep].parent = fRep;
                edgeState[eRep].twistUp = hasTwist ^ parentTwists;

                edgeState[fRep].size += edgeState[eRep].size;

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

                edgeStateChanged[orderIdx] = fRep;
            }

            nEdgeClasses--;
        }
    }

#if (! (defined(NO_CONE_PRUNING) && defined(NO_L31_PRUNING)))
    // Find representatives of the equivalence classes for all six edges
    // of the current tetrahedron (instead of calculating them each time
    // we want them).
    int tRep[6];
    char tTwist[6];
    for (e = 0; e < 6; e++) {
        tTwist[e] = 0;
        for (tRep[e] = e + 6 * face.tet; edgeState[tRep[e]].parent >= 0;
                tRep[e] = edgeState[tRep[e]].parent)
            tTwist[e] ^= edgeState[tRep[e]].twistUp;
    }
#endif

#ifndef NO_CONE_PRUNING
    // Test for cones on edges v1->w1->v2.
    for (w1 = 0; w1 < 4; w1++)
        for (v1 = 0; v1 < 3; v1++) {
            if (v1 == w1)
                continue;
            for (v2 = v1 + 1; v2 < 4; v2++) {
                if (v2 == w1)
                    continue;

                parentTwists = tTwist[edgeNumber[v1][w1]] ^
                    tTwist[edgeNumber[v2][w1]];

                if (tRep[edgeNumber[v1][w1]] == tRep[edgeNumber[v2][w1]]) {
                    hasTwist = (v1 < w1 && w1 < v2 ? 0 : 1);
                    if (hasTwist ^ parentTwists) {
                        retVal |= ECLASS_CONE;
                        goto coneDone;
                    }
                }
            }
        }
    coneDone:
#endif

#ifndef NO_L31_PRUNING
    // Test for L(3,1) spines.
    // L(3,1) on face 012:
    if (tRep[0] == tRep[1] && tRep[1] == tRep[3])
        if ((tTwist[0] ^ tTwist[1]) && (tTwist[1] ^ tTwist[3]))
            retVal |= ECLASS_L31;
    // L(3,1) on face 013:
    if (tRep[0] == tRep[2] && tRep[2] == tRep[4])
        if ((tTwist[0] ^ tTwist[2]) && (tTwist[2] ^ tTwist[4]))
            retVal |= ECLASS_L31;
    // L(3,1) on face 023:
    if (tRep[1] == tRep[2] && tRep[2] == tRep[5])
        if ((tTwist[1] ^ tTwist[2]) && (tTwist[2] ^ tTwist[5]))
            retVal |= ECLASS_L31;
    // L(3,1) on face 123:
    if (tRep[3] == tRep[4] && tRep[4] == tRep[5])
        if ((tTwist[3] ^ tTwist[4]) && (tTwist[4] ^ tTwist[5]))
            retVal |= ECLASS_L31;
#endif

    return retVal;
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
        e = 5 - edgeNumber[v1][v2];

        eIdx = e + 6 * face.tet;
        orderIdx = v2 + 4 * orderElt;

        if (edgeStateChanged[orderIdx] < 0) {
            for (rep = eIdx; edgeState[rep].parent >= 0;
                    rep = edgeState[rep].parent)
                ;
            edgeState[rep].bounded = true;
        } else {
            subRep = edgeStateChanged[orderIdx];
            rep = edgeState[subRep].parent;

            edgeState[subRep].parent = -1;
            if (edgeState[subRep].hadEqualRank) {
                edgeState[subRep].hadEqualRank = false;
                edgeState[rep].rank--;
            }

            edgeState[rep].size -= edgeState[subRep].size;

            edgeStateChanged[orderIdx] = -1;
            nEdgeClasses++;
        }
    }
}
#endif

} // namespace regina

