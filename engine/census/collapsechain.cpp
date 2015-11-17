
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

const char NCollapsedChainSearcher::dataTag_ = 'h';

NCollapsedChainSearcher::NCollapsedChainSearcher(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, bool orientableOnly,
        UseGluingPerms use, void* useArgs) :
        NGluingPermSearcher(pairing, autos, orientableOnly, true, // finiteOnly
            PURGE_NON_MINIMAL_PRIME | PURGE_P2_REDUCIBLE,
            use, useArgs), maxOrder(0) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    unsigned nTets = getNumberOfTetrahedra();

    modified = new NFacePairing(*pairing);
    chainPermIndices = new int[8 * nTets];
    orderType = new edgeType[2 * nTets];
    // Begin by searching for tetrahedra that are joined to themselves.
    // Note that each tetrahedra can be joined to itself at most once,
    // since we are guaranteed that the face pairing is connected with
    // order >= 3.

    int numChains = 0;
    chainNo = new int [2*nTets];
    shortChain = new bool [nTets] {false};
    collapse = true;
    NTetFace face;
    std::cout << "From " << pairing_->toString() << std::endl;
    if (pairing_->hasTripleEdge() ||
            pairing_->hasBrokenDoubleEndedChain() ||
            pairing_->hasOneEndedChainWithDoubleHandle() ||
            pairing_->hasOneEndedChainWithStrayBigon() ||
            pairing_->hasWedgedDoubleEndedChain() ||
            pairing_->hasTripleOneEndedChain()) {
        // Empty anyway
        chainSym = NULL;
        iso = NULL;
        isoInv = NULL;
    } else {
        for (face.setFirst(); collapse && ! face.isPastEnd(nTets, true); face++) {
            NTetFace adj = (*pairing_)[face];
            if (adj.simp != face.simp)
                continue; // Not a loop
            if (face.facet > adj.facet)
                continue; // Only traverse loops once
            if (collapseChain(NFacePair(face.facet, adj.facet), adj.simp,
                        numChains))
                numChains++;
        }
        if (numChains > 0) {
            maxOrder = orderElt;
            chainSym = new bool [numChains] {false};
            iso = modified->makeCanonical();
            isoInv = iso->inverse();
            if (pairing_->size() == modified->size())
                collapse = false;
            if (modified->size() < 3)
                collapse = false;
        } else {
            collapse = false;
            chainSym = NULL;
            iso = NULL;
            isoInv = NULL;
        }
    }
//    std::cout << "maxOrder = " << maxOrder << std::endl;
//    std::cout << "numChains= " << numChains << std::endl;
//    for (int i=0; i<orderElt; i++) {
//        std::cout << order[i] << "  ";
//    }
//    std::cout << std::endl;
//    for (int i=0; i<orderElt; i++) {
//        for (int j=0; j<4; j++) {
//            std::cout << indexToGluing(order[i], chainPermIndices[4*i+j]).str() << "  ";
//        }
//    }
//    std::cout << std::endl;
}

NCollapsedChainSearcher::~NCollapsedChainSearcher() {
    if (iso)
        delete iso;
    if (isoInv)
        delete isoInv;
    delete modified;
    delete[] chainNo;
    if (chainSym)
        delete[] chainSym;
    delete[] chainPermIndices;
    delete[] shortChain;
    delete[] orderType;
}

void NCollapsedChainSearcher::runSearch(long maxDepth) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3
    //     The given face pairing contains a chain of length >= 1

    // Is it a partial search that has already finished?
    // TODO
//    if (orderElt == static_cast<int>(nTets) * 2) {
//        if (isCanonical())
//            use_(this, useArgs_);
//        use_(0, useArgs_);
//        return;
//    }

    if (collapse) {
        std::cout << "From " << pairing_->toString() << std::endl;
        std::cout << "To   " << modified->toString() << std::endl;
//        for (int i = 0; i < maxOrder; i++) {
//            NTetFace face = order[i];
//            NTetFace adj = (*pairing_)[face];
//            std::cout << face << " to " << adj << " ";
//            std::cout << indexToGluing(face, chainPermIndices[2*i]).str() << " and ";
//            std::cout << indexToGluing(face, chainPermIndices[2*i+ 1]).str() << std::endl;
//        }
        UseGluingPerms func = &NCollapsedChainSearcher::extendTriHelper;
        NClosedPrimeMinSearcher s(modified, NULL, orientableOnly_, func,
                this);
        s.runSearch();
    } else {
        NClosedPrimeMinSearcher s(pairing_, autos_, orientableOnly_, use_,
                useArgs_);
        s.runSearch();
    }
    use_(0, useArgs_);
}

void NCollapsedChainSearcher::extendTriHelper(const NGluingPermSearcher *s, void *
        useArgs) {
    // End of search
    if (s == 0)
        return;

    NCollapsedChainSearcher *c = static_cast<NCollapsedChainSearcher*>(useArgs);
    c->extendTri(s);
}

void NCollapsedChainSearcher::extendTri(const NGluingPermSearcher *s) {
    orderElt = 0;
    // For each triangulation found.
    // copy pairing_ and apply reverse of iso
    // Note that iso is the isomorphism from this to s

    NTriangulation *tri = s->triangulate();
    std::string testCase("gLLMQacdefefjkaknkr");
    std::string thisCase = tri->isoSig();
    bool debug = (testCase.compare(thisCase) == 0);
    if (debug) {
        std::cout << "Original " << pairing_->str() << std::endl;
        std::cout << "Modified " << modified->str() << std::endl;
        std::cout << "Started with " << thisCase << std::endl;
//        for (NTetFace f(0,0); ! f.isPastEnd(s->size(), true); f++) {
//            NTetFace adj = (*modified)[f];
//            if (f < adj) {
//                std::cout << "From " << f << " to " << adj.simp << " (" <<
//                    (s->gluingPerm(f)*NTriangle::ordering[f.facet]).trunc3() <<
//                    ")";
//                std::cout << "\t\tFrom " << f << " to " << adj << " : " <<
//                    s->gluingPerm(f) << std::endl;
//            }
//        }
//        for (int simp = 0; simp < s->size(); simp++) {
//            std::cout << simp << " isoInv -> " << isoInv->simpImage(simp)
//                << " : " << isoInv->facetPerm(simp) << std::endl;
//        }
    }

    for (NTetFace f(0,0); ! f.isPastEnd(s->size(), true); f++) {
        NTetFace adj = (*(s->getFacetPairing()))[f];
        if ((adj.simp == f.simp)) {
            // This is a loop
            continue;
        }
        int mySimp = isoInv->simpImage(f.simp);
        NPerm4 perm = isoInv->facetPerm(f.simp);
        int myFacet = perm[f.facet];
        NTetFace my(mySimp, myFacet);
        if (permIndex(my) != -1)
            continue; // Already done.
        int myAdjSimp = isoInv->simpImage(adj.simp);
        NPerm4 adjPerm = isoInv->facetPerm(adj.simp);
        int myAdjFacet = adjPerm[adj.facet];
        NTetFace myAdj(myAdjSimp, myAdjFacet);

        NPerm4 gluing = (adjPerm.inverse() * s->gluingPerm(f) * perm);
        if (debug) {
            if (f < adj) {
                std::cout << "From " << f << " to " << adj.simp << " (" <<
                    (s->gluingPerm(f)*NTriangle::ordering[f.facet]).trunc3() <<
                    ")";
                std::cout << "\t\tFrom " << f << " to " << adj << " : " <<
                    s->gluingPerm(f) << std::endl;
            }
        }
        if ( gluing[myFacet] != myAdjFacet ) {
            std::cerr << "Error: Invalid gluing " << std::endl;
        }
        permIndex(my) = gluingToIndex(my, gluing);
        permIndex(myAdj) = gluingToIndex(myAdj, gluing.inverse());
    }
    orderElt = 0;
    while (orderElt >= 0) {
        if (orderElt >= maxOrder) {
//            if (debug) {
//                NTriangulation *newTri = triangulate();
//                std::cout << "Found " << newTri->isoSig() << std::endl;
//            }
            use_(this, useArgs_);
            orderElt -= 1;
            continue;
        }
        NTetFace face = order[orderElt];
        NTetFace adj = (*pairing_)[face];
        if (orderType[orderElt] == EDGE_CHAIN_END ) {
            if (permIndex(face) < 0) {
                permIndex(face) = chainPermIndices[4*orderElt];
            } else if (permIndex(face) == chainPermIndices[4*orderElt]) {
                permIndex(face) = chainPermIndices[4*orderElt+1];
            } else {
                // Try flipping the whole chain.
                int chain = chainNo[orderElt];
                if (chainSym[chain] == false) {
                    permIndex(face) = chainPermIndices[4*orderElt];
                    chainSym[chain] = true;
                } else {
                    chainSym[chain] = false;
                    permIndex(face) = -1;
                    permIndex(adj) = -1;
                    orderElt--;
                    continue;
                }
            }
            permIndex(adj) = NPerm4::invS3[permIndex(face)];
            orderElt++;
            continue;
        }
        // If chainSym is true, add 2 to each index into chainPermIndices
        int symAdd = chainSym[chainNo[orderElt]] ? 2 : 0;
        if (orderType[orderElt] == EDGE_CHAIN_INTERNAL_FIRST) {
            if (permIndex(face) < 0) {
                permIndex(face) = chainPermIndices[4*orderElt + symAdd];
            } else if (permIndex(face) == chainPermIndices[4*orderElt + symAdd]) {
                permIndex(face) = chainPermIndices[4*orderElt+1 + symAdd];
            } else {
                permIndex(face) = -1;
                permIndex(adj) = -1;
                orderElt--;
                continue;
            }
        } else { // orderType[orderElt] == EDGE_CHAIN_INTERNAL_SECOND
            if (permIndex(face) == -1) {
                permIndex(face) = chainPermIndices[4*orderElt + symAdd];
            } else if (permIndex(face) == chainPermIndices[4*orderElt + symAdd]) {
                permIndex(face) = chainPermIndices[4*orderElt+1 + symAdd];
            } else {
                permIndex(face) = -1;
                permIndex(adj) = -1;
                orderElt--;
                continue;
            }
        }
        permIndex(adj) = NPerm4::invS3[permIndex(face)];
        orderElt++;
    }
    // Reset
    for (NTetFace f(0,0); ! f.isPastEnd(pairing_->size(), true); f++) {
        permIndex(f) = -1;
    }
}


void NCollapsedChainSearcher::dumpData(std::ostream& out) const {
    NGluingPermSearcher::dumpData(out);
//
//    int i;
//    for (i = 0; i < orderSize; i++) {
//        if (i)
//            out << ' ';
//        out << orderType[i];
//    }
//    out << std::endl;
//
//    out << nChainEdges << std::endl;
//    if (nChainEdges) {
//        for (i = 0; i < 2 * static_cast<int>(nChainEdges); i++) {
//            if (i)
//                out << ' ';
//            out << chainPermIndices[i];
//        }
//        out << std::endl;
//    }
}

NCollapsedChainSearcher::NCollapsedChainSearcher(std::istream& in,
        UseGluingPerms use, void* useArgs) :
        NGluingPermSearcher(in, use, useArgs)  {
//    if (inputError_)
//        return;
//
//    unsigned nTets = getNumberOfTetrahedra();
//    int i;
//
//    orderType = new unsigned[2 * nTets];
//    for (i = 0; i < orderSize; i++)
//        in >> orderType[i];
//
//    in >> nChainEdges;
//    /* Unnecessary since nChainEdges is unsigned.
//    if (nChainEdges < 0) {
//        inputError_ = true; return;
//    } */
//    if (nChainEdges) {
//        chainPermIndices = new int[nChainEdges * 2];
//        for (i = 0; i < 2 * static_cast<int>(nChainEdges); i++) {
//            in >> chainPermIndices[i];
//            if (chainPermIndices[i] < 0 || chainPermIndices[i] >= 6) {
//                inputError_ = true; return;
//            }
//        }
//    }
//
//    // Did we hit an unexpected EOF?
//    if (in.eof())
//        inputError_ = true;
}

bool NCollapsedChainSearcher::collapseChain(NFacePair faces, int tet, int
        numChains) {
    NFacePair comp = faces.complement();
    order[orderElt] = NTetFace(tet, faces.lower());
    chainNo[orderElt] = numChains;
    chainPermIndices[4 * orderElt] = gluingToIndex(order[orderElt],
        NPerm4(faces.lower(), faces.upper(),
                faces.upper(), comp.lower(),
                comp.lower(), comp.upper(),
                comp.upper(), faces.lower()));
    chainPermIndices[4 * orderElt + 1] = gluingToIndex(order[orderElt],
        NPerm4(faces.lower(), faces.upper(),
                faces.upper(), comp.upper(),
                comp.upper(), comp.lower(),
                comp.lower(), faces.lower()));
    orderType[orderElt] = EDGE_CHAIN_END;
    orderElt+=1;
    faces = faces.complement();
    NTetFace dest1, dest2;
    dest1 = modified->dest(tet, faces.lower());
    dest2 = modified->dest(tet, faces.upper());
    if (dest1.simp != dest2.simp) {
        // Short chain here. Bail early, reset things.
        shortChain[tet] = true;
        orderElt-=1;
        // order, chainNo and other arrays will be overwritten, if required, so
        // no need to reset them.
        return false;
    }
    // Currently tet and faces refer to the two faces of the base
    // tetrahedron that are pointing outwards.
    while (dest1.simp == dest2.simp && dest1.simp != tet) {
        order[orderElt] = NTetFace(tet, faces.lower());
        order[orderElt+1] = NTetFace(tet, faces.upper());
        orderType[orderElt] = EDGE_CHAIN_INTERNAL_FIRST;
        orderType[orderElt+1] = EDGE_CHAIN_INTERNAL_SECOND;
        chainNo[orderElt] = numChains;
        chainNo[orderElt+1] = numChains;

        comp = faces.complement();
        NFacePair facesAdj = NFacePair(dest1.facet, dest2.facet);
        NFacePair compAdj = facesAdj.complement();

        NPerm4 trial1 = NPerm4(faces.lower(), facesAdj.lower(),
                        faces.upper(), compAdj.lower(),
                        comp.lower(), compAdj.upper(),
                        comp.upper(), facesAdj.upper());
        NPerm4 trial2 = NPerm4(faces.lower(), facesAdj.lower(),
                        faces.upper(), compAdj.upper(),
                        comp.lower(), compAdj.lower(),
                        comp.upper(), facesAdj.upper());
        if (trial1.compareWith(trial2) < 0) {
            chainPermIndices[4 * orderElt] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[4 * orderElt + 4] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), facesAdj.lower(),
                        comp.upper(), compAdj.lower()));
            chainPermIndices[4 * orderElt + 2] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[4 * orderElt + 6] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.lower(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), facesAdj.lower(),
                        comp.upper(), compAdj.upper()));
        } else {
            chainPermIndices[4 * orderElt] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[4 * orderElt + 4] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.lower(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), facesAdj.lower(),
                        comp.upper(), compAdj.upper()));
            chainPermIndices[4 * orderElt + 2] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[4 * orderElt + 6] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), facesAdj.lower(),
                        comp.upper(), compAdj.lower()));
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
            chainPermIndices[4 * orderElt + 1] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[4 * orderElt + 5] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.lower(),
                        comp.upper(), facesAdj.lower()));
            chainPermIndices[4 * orderElt + 3] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[4 * orderElt + 7] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.lower(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.upper(),
                        comp.upper(), facesAdj.lower()));
        } else {
            chainPermIndices[4 * orderElt + 1] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[4 * orderElt + 5] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.lower(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.upper(),
                        comp.upper(), facesAdj.lower()));
            chainPermIndices[4 * orderElt + 3] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[4 * orderElt + 7] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.lower(),
                        comp.upper(), facesAdj.lower()));
        }
        // If we are not unmatching a loop, unmatch the extra edge
        if (modified->dest(tet, comp.lower()) != NTetFace(tet, comp.upper())) {
            modified->unMatch(tet,comp.lower());
        }
        modified->unMatch(tet,comp.upper());
        faces = NFacePair(dest1.facet, dest2.facet);
        faces = faces.complement();
        orderElt += 2;

        tet = dest1.simp;
        dest1 = modified->dest(tet, faces.lower());
        dest2 = modified->dest(tet, faces.upper());

    }
    if (dest1.simp == tet)  // Chain ended on loop, aka this face pairing is a
        collapse = false;   // double-chain
    faces = faces.complement();
    // Need to unmatch the final pair of parallel edges (as long as its not a loop).
    if (modified->dest(tet, faces.lower()) != NTetFace(tet, faces.upper())) {
        dest1 = NTetFace(tet, faces.lower());
        dest2 = NTetFace(tet, faces.upper());
        modified->unMatch(dest1);
        modified->unMatch(dest2);
        modified->match(dest1, dest2); // Add back the original loop.
    }
    return true;
}

} // namespace regina

