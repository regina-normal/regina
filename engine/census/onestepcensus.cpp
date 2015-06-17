
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

#include <sstream>
#include <cstring>
#include <boost/next_prior.hpp>
#include "census/ngluingpermsearcher.h"
#include "census/partialcensusdb.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"


namespace regina {

const char OneStepSearcher::dataTag_ = 'o';

OneStepSearcher::OneStepSearcher(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, PartialCensusDB* db,
        bool orientableOnly, UseGluingPerms use, void* useArgs) :
        NClosedPrimeMinSearcher(pairing, autos, orientableOnly, use, useArgs),
        db_(db), useDB(false), childPairing(NULL) {

    unsigned nTets = size();

    // First check the database to see if results are available.
    childPairing = new NFacePairing(*pairing);

    pairingStrings = new std::string[nTets];
    viable = new bool[nTets];
    for(unsigned i=0; i< nTets; i++)
        viable[i] = false;

    minOrder = size();
    PartialCensusDB::DBStatus status;
    do {
        childPairing->removeSimplex(childPairing->size()-1);
        pairingStrings[childPairing->size()] = childPairing->str();
        if (db_->viable(childPairing))
            viable[childPairing->size()] = true;
        status = db_->request(pairingStrings[childPairing->size()]);
    }
    while (status != PartialCensusDB::DBStatus::Found && childPairing->size() > 1);

    if (status == PartialCensusDB::DBStatus::Found) {
        useDB = true;
    }

    orderDone = 0;
    bool* orderAssigned = new bool[nTets * 4];
        /**< Have we placed a tetrahedron face or its partner in the
             order[] array yet? */
    std::fill(orderAssigned, orderAssigned + 4 * nTets, false);

    // Hunt for structures within the face pairing graph.

    NTetFace face, adj;

    // Check if a chain starts on tetrahedron 0.
    // Note that this tetrahedron can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.
    // Since we want to visit tetrahedra in order and store all possible
    // partial triangulations, we cannot track any other chains.

    face.setFirst();
    adj = (*pairing)[face];
    if (adj.simp == face.simp) {
        order[orderDone] = face;
        orderType[orderDone] = EDGE_CHAIN_END;
        orderAssigned[face.simp * 4 + face.facet] = true;
        orderAssigned[adj.simp * 4 + adj.facet] = true;
        orderDone++;

        // Now try to follow said chain as far as possible.
        unsigned i = 0;
        int tet;
        NTetFace dest1, dest2;
        tet = order[i].simp;
        NFacePair faces = NFacePair(order[i].facet,
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
    unsigned lastChainEdge = orderDone;

    NFacePair faces, facesAdj, comp, compAdj;
    NPerm4 trial1, trial2;
    for (unsigned i = 0; i < orderDone; i++) {
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

    orientation[order[lastChainEdge].simp] = 1;

    if (useDB) {
        // Find where we have to start from
        minOrder = 0;
        NTetFace face = order[minOrder];
        NTetFace adj = (*pairing_)[face];
        while (adj.simp < childPairing->size()) {
            minOrder++;
            face = order[minOrder];
            adj = (*pairing_)[face];
        }
        std::cout << "Restarting at minOrder = " << minOrder << std::endl;
        std::cout << "child FPG = " << pairingStrings[childPairing->size()] << std::endl;
        std::cout << "Face: " << face.simp << "." << face.facet <<std::endl;
        std::cout << "adj: " << adj.simp << "." << adj.facet <<std::endl;
    }
}

OneStepSearcher::~OneStepSearcher() {
    if (childPairing)
        delete childPairing;
}

void OneStepSearcher::runSearch(long maxDepth) {
    if (pairing_->hasTripleEdge() ||
            pairing_->hasBrokenDoubleEndedChain() ||
            pairing_->hasOneEndedChainWithDoubleHandle() ||
            pairing_->hasOneEndedChainWithStrayBigon() ||
            pairing_->hasWedgedDoubleEndedChain() ||
            pairing_->hasTripleOneEndedChain()) {
        // Immediate fall through to use_(0, useArgs_);
    } else if (useDB) {
        // TODO use each result from DB
        PartialCensusHits * results = db_->retrieve(childPairing->str());
        PartialCensusHit * h;
        for ( h = results->begin(); h != results->end(); h = h->next() ) {
            const PartialTriangulationData *d = h->data();
            buildUp(d);
        }
    } else {
        orderElt = minOrder = 0;
        glue();
    }
    use_(0, useArgs_);
}

void OneStepSearcher::buildUp(const PartialTriangulationData *data) {
    int nTets = size();
    std::memcpy(permIndices_, data->permIndices_,
            sizeof(data->permIndices_));
    std::memcpy(vertexState, data->vertexState,
            sizeof(data->vertexState));
    std::memcpy(vertexStateChanged, data->vertexStateChanged,
            sizeof(data->vertexStateChanged));
    std::memcpy(edgeState, data->edgeState,
            sizeof(data->edgeState));
    std::memcpy(edgeStateChanged, data->edgeStateChanged,
            sizeof(data->edgeStateChanged));
    if (orientableOnly_) {
        std::memcpy(orientation, data->orientation, sizeof(data->orientation));
        //std::fill(orientation + childPairing->size(), orientation + nTets, 0);
    }
    // Note that anything stored in the database is still only relevant to nTet
    // tetrahedra. That is, when doing a 6-tetrahedra census anything in the DB
    // must already assume 6 tetrahedra are in the triangulation.
    nVertexClasses = data->nVertexClasses;
    nEdgeClasses = data->nEdgeClasses;

    // Set up edge + vertex states for the remaining tetrahedra
    // Vertex states
    //std::fill(vertexStateChanged + childPairing->size()*8, vertexStateChanged + nTets * 8, -1);
    //for (unsigned i = childPairing->size() * 4; i < nTets * 4; ++i) {
    //    vertexState[i].bdryEdges = 3;
    //    vertexState[i].bdryNext[0] = vertexState[i].bdryNext[1] = i;
    //    vertexState[i].bdryTwist[0] = vertexState[i].bdryTwist[1] = 0;
    //    // Initialise the backup members also so we're not writing
    //    // uninitialised data via dumpData().
    //    vertexState[i].bdryNextOld[0] = vertexState[i].bdryNextOld[1] = -1;
    //    vertexState[i].bdryTwistOld[0] = vertexState[i].bdryTwistOld[1] = 0;
    //}
    //unsigned tetDiff = nTets - childPairing->size();
    ////nVertexClasses += 4 * tetDiff;

    //// Edge states
    //std::fill(edgeStateChanged + childPairing->size()*8, edgeStateChanged + nTets * 8, -1);

    //// Since NQitmaskLen64 only supports 64 faces, only work with
    //// the first 16 tetrahedra.  If n > 16, this just weakens the
    //// optimisation; however, this is no great loss since for n > 16 the
    //// census code is at present infeasibly slow anyway.
    //for (unsigned i = childPairing->size(); i < nTets && i < 16 ; ++i) {
    //    /* 01 on +012, +013             */
    //    edgeState[6 * i    ].facesPos.set(4 * i + 3, 1);
    //    edgeState[6 * i    ].facesPos.set(4 * i + 2, 1);
    //    /* 02 on -012        +023       */
    //    edgeState[6 * i + 1].facesNeg.set(4 * i + 3, 1);
    //    edgeState[6 * i + 1].facesPos.set(4 * i + 1, 1);
    //    /* 03 on       -013, -023       */
    //    edgeState[6 * i + 2].facesNeg.set(4 * i + 2, 1);
    //    edgeState[6 * i + 2].facesNeg.set(4 * i + 1, 1);
    //    /* 12 on +012,             +123 */
    //    edgeState[6 * i + 3].facesPos.set(4 * i + 3, 1);
    //    edgeState[6 * i + 3].facesPos.set(4 * i + 0, 1);
    //    /* 13 on       +013        -123 */
    //    edgeState[6 * i + 4].facesPos.set(4 * i + 2, 1);
    //    edgeState[6 * i + 4].facesNeg.set(4 * i + 0, 1);
    //    /* 23 on             +023, +123 */
    //    edgeState[6 * i + 5].facesPos.set(4 * i + 1, 1);
    //    edgeState[6 * i + 5].facesPos.set(4 * i + 0, 1);
    //}
    //nEdgeClasses += 6 * tetDiff; // What if nTet >= 16?

    orderElt = minOrder;
    glue(); // Attempt first gluing
}

void OneStepSearcher::glue() {
    NTetFace face, adj;
    bool generic;
    unsigned nTets = size();
    int mergeResult;
    while ( orderElt >= minOrder ) {
        face = order[orderElt];
        NTetFace adj = (*pairing_)[face];

        // If we're about to identify onto a new tetrahedron, store the partial
        // triangulation we have at the moment. But only if it's only
        // canonical up to this point. Also no point calling database if it's
        // the same size as what we're already pulling from the database
        if (viable[adj.simp] && (orderElt > minOrder) && (adj.facet == 0)) {
            db_->store(this, nTets, pairingStrings[adj.simp]);
            std::cout << "Storing at orderElt = " << orderElt << std::endl;
            std::cout << "child FPG = " << pairingStrings[adj.simp] << std::endl;
            std::cout << "permIndices: ";
            NTetFace f;
            for (f.setFirst(); ! f.isPastEnd(nTets, true); f++) {
                std::cout << indexToGluing(f, permIndex(f));
                if (f.facet == 3)
                    std::cout << " | ";
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
            //std::cout << "Face: " << face.simp << "." << face.facet <<std::endl;
            //std::cout << "adj: " << adj.simp << "." << adj.facet <<std::endl;
        }

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
        // If we have identified with the last tetrahedron, we can check a
        // bound on the number of remaining edges.
        if ((face.simp == nTets) || (adj.simp == nTets)) {
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
        }

        // If we have identified with the last tetrahedron, we can check a
        // bound on the number of remaining edges.
        if ((face.simp == nTets) || (adj.simp == nTets)) {
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

            // Remnant from NClosedPrimeMin, not needed? TODO
//            if (orderElt == maxOrder) {
//                // We haven't found an entire triangulation, but we've
//                // gone as far as we need to.
//                // Process it, then step back.
//                use_(this, useArgs_);
//
//                // Back to the previous face.
//                permIndex(face) = -1;
//                orderElt--;
//
//                // Pull apart vertex links at the previous level.
//                if (orderElt >= minOrder) {
//                    splitVertexClasses();
//                    splitEdgeClasses();
//                }
//            }
        }
    }
}

// TODO Adjust (maybe)?
void OneStepSearcher::dumpData(std::ostream& out) const {
    NClosedPrimeMinSearcher::dumpData(out);

}

//NCompactSearcher::NCompactSearcher(std::istream& in,
//        UseGluingPerms use, void* useArgs) :
//        NGluingPermSearcher(in, use, useArgs),
//        nVertexClasses(0), vertexState(0), vertexStateChanged(0),
//        nEdgeClasses(0), edgeState(0), edgeStateChanged(0) {
//    if (inputError_)
//        return;
//
//    unsigned nTets = getNumberOfTetrahedra();
//    unsigned i;
//
//    in >> nVertexClasses;
//    if (nVertexClasses > 4 * nTets) {
//        inputError_ = true; return;
//    }
//
//    vertexState = new TetVertexState[4 * nTets];
//    for (i = 0; i < 4 * nTets; i++)
//        if (! vertexState[i].readData(in, 4 * nTets)) {
//            inputError_ = true; return;
//        }
//
//    vertexStateChanged = new int[8 * nTets];
//    for (i = 0; i < 8 * nTets; i++) {
//        in >> vertexStateChanged[i];
//        if (vertexStateChanged[i] < -1 ||
//                 vertexStateChanged[i] >= 4 * static_cast<int>(nTets)) {
//            inputError_ = true; return;
//        }
//    }
//
//    in >> nEdgeClasses;
//    if (nEdgeClasses > 6 * nTets) {
//        inputError_ = true; return;
//    }
//
//    edgeState = new TetEdgeState[6 * nTets];
//    for (i = 0; i < 6 * nTets; i++)
//        if (! edgeState[i].readData(in, nTets)) {
//            inputError_ = true; return;
//        }
//
//    edgeStateChanged = new int[8 * nTets];
//    for (i = 0; i < 8 * nTets; i++) {
//        in >> edgeStateChanged[i];
//        if (edgeStateChanged[i] < -1 ||
//                 edgeStateChanged[i] >= 6 * static_cast<int>(nTets)) {
//            inputError_ = true; return;
//        }
//    }
//
//    // Did we hit an unexpected EOF?
//    if (in.eof())
//        inputError_ = true;
//}


} // namespace regina

