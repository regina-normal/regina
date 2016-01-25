
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
#include <cassert>
#include <sstream>
#include "census/enumerationdb.h"
#include "census/collapsedchain.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

const char CollapsedChainSearcher::dataTag_ = 'h';

CollapsedChainSearcher::CollapsedChainSearcher(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, bool orientableOnly,
        char* enumDBfile, UseGluingPerms use, void* useArgs) :
        NGluingPermSearcher(pairing, autos, orientableOnly, true, // finiteOnly
            PURGE_NON_MINIMAL_PRIME | PURGE_P2_REDUCIBLE,
            use, useArgs), maxOrder(0) {

    // Create enumeration database if the file path is given, else mark the
    // database as not present.
    if (enumDBfile)
        enumDB = new EnumerationDB(enumDBfile);
    else
        enumDB = 0;
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.

    unsigned nTets = getNumberOfTetrahedra();

    modified = new NFacePairing(*pairing);
    chainPermIndices = new int[4 * nTets];
    orderType = new EdgeType[2 * nTets];

    int numChains = 0;
    shortChain = new bool [nTets] {false};
    collapse = true;
    empty = false;
    NTetFace face;
    if (pairing_->hasTripleEdge() ||
            pairing_->hasBrokenDoubleEndedChain() ||
            pairing_->hasOneEndedChainWithDoubleHandle() ||
            pairing_->hasOneEndedChainWithStrayBigon() ||
            pairing_->hasWedgedDoubleEndedChain() ||
            pairing_->hasTripleOneEndedChain()) {
        // Empty anyway
        iso = NULL;
        isoInv = NULL;
        collapse = false;
        empty = true;
    } else {
        for (face.setFirst(); collapse && ! face.isPastEnd(nTets, true); face++) {
            NTetFace adj = (*pairing_)[face];
            if (adj.simp != face.simp)
                continue; // Not a loop
            if (face.facet > adj.facet)
                continue; // Only traverse loops once
            if (collapseChain(NFacePair(face.facet, adj.facet), adj.simp))
                numChains++;
        }
        if (numChains > 0) {
            maxOrder = orderElt;
            iso = modified->makeCanonical();
            isoInv = iso->inverse();
            if (pairing_->size() == modified->size())
                collapse = false;
            if (modified->size() < 3)
                collapse = false;
        } else {
            collapse = false;
            iso = NULL;
            isoInv = NULL;
        }
    }
}

CollapsedChainSearcher::~CollapsedChainSearcher() {
    if (iso)
        delete iso;
    if (isoInv)
        delete isoInv;
    delete modified;
    delete[] orderType;
    delete[] chainPermIndices;
    delete[] shortChain;
    if (!automorphs.empty()) {
        std::for_each(automorphs.begin(), automorphs.end(),
                FuncDelete<NIsomorphism>());
    }
}

void CollapsedChainSearcher::runSearch(long maxDepth) {
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
    if (empty) {
        use_(0, useArgs_);
        return;
    }

    if (collapse) {
        // TODO remove loops that will be replaced from modified, to cut down
        // number of automorphisms.
        modified->findAutomorphisms(automorphs);
        if (enumDB) {
            std::list<NTriangulation*> results = enumDB->lookup(*modified);
            for(auto tri: results)
                extendTri(tri);
        } else {
            UseGluingPerms func = &CollapsedChainSearcher::extendTriHelper;
            NClosedPrimeMinSearcher s(modified, NULL, orientableOnly_, func,
                    this);
            s.runSearch();
            use_(0, useArgs_);
        }
    } else {
        NClosedPrimeMinSearcher s(pairing_, autos_, orientableOnly_, use_,
                useArgs_);
        s.runSearch();
    }
}

void CollapsedChainSearcher::extendTriHelper(const NGluingPermSearcher *s, void *
        useArgs) {
    // End of search
    // Note that we can't call use_ here as this function is static.
    if (s == 0) {
        return;
    }

    CollapsedChainSearcher *c = static_cast<CollapsedChainSearcher*>(useArgs);
    c->extendTri(s);
}

void CollapsedChainSearcher::extendTri(const NTriangulation *tri) {
    // tri's face pairing is probably not canonical.
    NFacePairing fpg(*tri);
    // We don't need fpg to be canonical, strictly speaking, but we do need the
    // isomorphism which will make fpg canonical.
    NIsomorphism* newIso = fpg.makeCanonical();
    NIsomorphism* newInv = newIso->inverse();

    // For each triangulation found.
    // copy pairing_ and apply reverse of iso
    // Note that iso is the isomorphism from this to s

    for (auto automorph : automorphs) {
        orderElt = 0;
        // f will be a facet of tri.
        for (NTetFace f(0,0); ! f.isPastEnd(tri->getNumberOfTetrahedra(), true); f++) {
            const NTetrahedron *tet = tri->getTetrahedron(f.simp);
            int adjSimp = tet->adjacentSimplex(f.facet)->index();
            int adjFacet = (tet->adjacentGluing(f.facet))[f.facet];
            // adj is adjacent (i.e. glued to) f in tri
            NTetFace adj(adjSimp,adjFacet);
            if ((f.simp == adj.simp) &&
                    (!shortChain[newInv->simpImage(iso->simpImage(f.simp))])) {
                continue;
            }

            // tri is a canonical triangulation, but that means that its face
            // pairing graph is probably not canonical. we need to apply newIso
            // to translate f and adj.
            // modF is f, but translated to the modified face pairing graph
            // modAdj is adj, translated to the modified face pairing graph
            NTetFace modF(newIso->simpImage(f.simp),
                    newIso->facetPerm(f.simp)[f.facet]);
            NTetFace modAdj(newIso->simpImage(adj.simp),
                    newIso->facetPerm(adj.simp)[adj.facet]);
            // Now we translate modF and modAdj onto pairing_, the actual fpg
            // we are searching on.
            // my is f, translated onto pairing_
            // myAdj is adj, translated onto pairing_
            int mySimp = isoInv->simpImage(newIso->simpImage(f.simp));
            NPerm4 perm = isoInv->facetPerm(newIso->simpImage(f.simp))
                * newIso->facetPerm(f.simp);
            int myFacet = perm[f.facet];
            NTetFace my(mySimp, myFacet);
            if (permIndex(my) != -1)
                continue; // Already done.
            int myAdjSimp = isoInv->simpImage(newIso->simpImage(adj.simp));
            NPerm4 adjPerm = isoInv->facetPerm(newIso->simpImage(adj.simp))
                * newIso->facetPerm(adj.simp);
            int myAdjFacet = adjPerm[adj.facet];
            NTetFace myAdj(myAdjSimp, myAdjFacet);

            // We need to allow for automorphisms of the triangulation
            // aut will be the image of modF under this automorphism
            NTetFace aut(automorph->simpImage(modF.simp),
                    automorph->facetPerm(modF.simp)[modF.facet]);
            // autAdj will be the image of modAdj under this automorphism
            NTetFace autAdj(automorph->simpImage(modAdj.simp),
                    automorph->facetPerm(modAdj.simp)[modAdj.facet]);
            NPerm4 autPerm = automorph->facetPerm(modF.simp);
            NPerm4 autAdjPerm = automorph->facetPerm(modAdj.simp);

            // And then translate aut and autAdj onto tri
            // autT is aut translated onto tri
            // autTadj is autAdj translated onto tri
            NTetFace autT(newInv->simpImage(aut.simp),
                    newInv->facetPerm(aut.simp)[aut.facet]);
            NTetFace autTadj(newInv->simpImage(autAdj.simp),
                    newInv->facetPerm(autAdj.simp)[autAdj.facet]);
            // Lastly, be careful that we translate permutations correctly
            // between these various settings.
            NPerm4 autGluing = newIso->facetPerm(autTadj.simp) *
                tri->getTetrahedron(autT.simp)->adjacentGluing(autT.facet) *
                newInv->facetPerm(aut.simp);
            NPerm4 gluing = isoInv->facetPerm(modAdj.simp).inverse() * autAdjPerm.inverse() *
                            autGluing *
                            autPerm * isoInv->facetPerm(modF.simp);
            assert(gluing[myFacet] == myAdjFacet);
            permIndex(my) = gluingToIndex(my, gluing);
            permIndex(myAdj) = gluingToIndex(myAdj, gluing.inverse());
        }
        buildUp(); // Builds actual triangulation
    }
    delete newInv;
    delete newIso;
}

void CollapsedChainSearcher::extendTri(const NGluingPermSearcher *s) {
    // For each triangulation found.
    // copy pairing_ and apply reverse of iso
    // Note that iso is the isomorphism from this to s

    for (auto automorph : automorphs) {
        orderElt = 0;
        // f will be a facet of s.
        for (NTetFace f(0,0); ! f.isPastEnd(s->size(), true); f++) {
            // adj is adjacent (i.e. glued to) f in s
            NTetFace adj = (*(s->getFacetPairing()))[f];
            if ((f.simp == adj.simp) &&
                    (!shortChain[isoInv->simpImage(f.simp)])) {
                // This is a loop, but not a short one. Don't copy the loop
                // gluing.
                continue;
            }
            // Now we translate f and adj onto pairing_, the actual fpg
            // we are searching on.
            // my is f, translated onto pairing_
            // myAdj is adj, translated onto pairing_
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

            // And then translate aut and autAdj onto s
            // autT is aut translated onto s
            // autTadj is autAdj translated onto s
            NTetFace aut(automorph->simpImage(f.simp),
                    automorph->facetPerm(f.simp)[f.facet]);
            NPerm4 autPerm = automorph->facetPerm(f.simp);
            NPerm4 autAdjPerm = automorph->facetPerm(adj.simp);

            // Lastly, be careful that we translate permutations correctly
            // between these various settings.
            NPerm4 gluing = adjPerm.inverse() * autAdjPerm.inverse() *
                            s->gluingPerm(aut) *
                            autPerm * perm;
            assert(gluing[myFacet] == myAdjFacet);
            permIndex(my) = gluingToIndex(my, gluing);
            permIndex(myAdj) = gluingToIndex(myAdj, gluing.inverse());
        }
        buildUp(); // Builds actual triangulation
    }
}

void NCollapsedChainSearcher::buildUp() {
    orderElt = 0;
    while (orderElt >= 0) {
        if (orderElt >= maxOrder) {
            use_(this, useArgs_);
            orderElt -= 1;
            continue;
        }
        NTetFace face = order[orderElt];
        NTetFace adj = (*pairing_)[face];
        if (orderType[orderElt] == EDGE_CHAIN_INTERNAL_SECOND) {
            if (permIndex(face) < 0) {
                if (permIndex(order[orderElt - 1]) ==
                        chainPermIndices[2 * orderElt - 2])
                    permIndex(face) = chainPermIndices[2 * orderElt];
                else
                    permIndex(face) = chainPermIndices[2 * orderElt + 1];
            } else {
                permIndex(face) = -1;
                permIndex(face) = -1;
                permIndex(adj) = -1;
                orderElt--;
                continue;
            }
        } else { // EGE_CHAIN_END or EDGE_CHAIN_INTERNAL_FIRST
            if (permIndex(face) < 0) {
                permIndex(face) = chainPermIndices[2*orderElt];
            } else if (permIndex(face) == chainPermIndices[2*orderElt]) {
                permIndex(face) = chainPermIndices[2*orderElt+1];
            } else {
                permIndex(face) = -1;
                permIndex(adj) = -1;
                orderElt--;
                continue;
            }
        }
        permIndex(adj) = NPerm3::invS3[permIndex(face)];
        orderElt++;
    }
    // Reset
    for (NTetFace f(0,0); ! f.isPastEnd(pairing_->size(), true); f++) {
        permIndex(f) = -1;
    }
}


void CollapsedChainSearcher::dumpData(std::ostream& out) const {
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

CollapsedChainSearcher::CollapsedChainSearcher(std::istream& in,
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

bool CollapsedChainSearcher::collapseChain(NFacePair faces, int tet) {
    NFacePair comp = faces.complement();
    order[orderElt] = NTetFace(tet, faces.lower());
    chainPermIndices[2 * orderElt] = gluingToIndex(order[orderElt],
        NPerm4(faces.lower(), faces.upper(),
                faces.upper(), comp.lower(),
                comp.lower(), comp.upper(),
                comp.upper(), faces.lower()));
    chainPermIndices[2 * orderElt + 1] = gluingToIndex(order[orderElt],
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
        // order, orderType and other arrays will be overwritten, if required, so
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
            chainPermIndices[2 * orderElt] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[2 * orderElt + 2] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), facesAdj.lower(),
                        comp.upper(), compAdj.lower()));
        } else {
            chainPermIndices[2 * orderElt] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[2 * orderElt + 2] = gluingToIndex(order[orderElt + 1],
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
            chainPermIndices[2 * orderElt + 1] = gluingToIndex(order[orderElt], trial1);
            chainPermIndices[2 * orderElt + 3] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.upper(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.lower(),
                        comp.upper(), facesAdj.lower()));
        } else {
            chainPermIndices[2 * orderElt + 1] = gluingToIndex(order[orderElt], trial2);
            chainPermIndices[2 * orderElt + 3] = gluingToIndex(order[orderElt + 1],
                NPerm4(faces.lower(), compAdj.lower(),
                        faces.upper(), facesAdj.upper(),
                        comp.lower(), compAdj.upper(),
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

