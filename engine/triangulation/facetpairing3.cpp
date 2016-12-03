
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

#include <algorithm>
#include <sstream>
#include <vector>
#include "triangulation/facepair.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim3.h"
#include "utilities/memutils.h"
#include "utilities/stringutils.h"

namespace regina {

bool FacetPairing<3>::hasTripleEdge() const {
    unsigned equal, i, j;
    for (size_t tet = 0; tet < size_; tet++) {
        // Is there a triple edge coming from this tetrahedron?
        equal = 0;
        for (i = 0; i < 4; i++)
            if ((! isUnmatched(tet, i)) &&
                    dest(tet, i).simp > static_cast<int>(tet)) {
                // This face joins to a real face of a later tetrahedron.
                for (j = i + 1; j < 4; j++)
                    if (dest(tet, i).simp == dest(tet, j).simp)
                        equal++;
            }

        // Did we find at least three pairs (i,j) joining to the same
        // real later tetrahedron?  A little case analysis shows that the
        // only way we can achieve this is through a triple edge.
        if (equal >= 3)
            return true;
    }
    return false;
}

void FacetPairing<3>::followChain(size_t& tet, FacePair& faces) const {
    FacetSpec<3> dest1, dest2;
    while (true) {
        // Does the first face lead to a real tetrahedron?
        if (isUnmatched(tet, faces.lower()))
            return;

        // Does the second face lead to the same tetrahedron as the first?
        dest1 = dest(tet, faces.lower());
        dest2 = dest(tet, faces.upper());
        if (dest1.simp != dest2.simp)
            return;

        // Do the two faces lead to a *different* tetrahedron?
        if (dest1.simp == static_cast<int>(tet))
            return;

        // Follow the chain along.
        tet = dest1.simp;
        faces = FacePair(dest1.facet, dest2.facet).complement();
    }
}

bool FacetPairing<3>::hasBrokenDoubleEndedChain() const {
    // Search for the end edge of the first chain.
    size_t baseTet;
    unsigned baseFace;
    // Skip the last tetrahedron -- any of the two ends will do.
    for (baseTet = 0; baseTet + 1 < size_; baseTet++)
        for (baseFace = 0; baseFace < 3; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<int>(baseTet)) {
                // Here's a face that matches to the same tetrahedron.
                if (hasBrokenDoubleEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more
                // self-identifications in this tetrahedron, since if
                // there's another (different) one it must be a
                // one-tetrahedron component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

bool FacetPairing<3>::hasBrokenDoubleEndedChain(size_t baseTet,
        unsigned baseFace) const {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    size_t bdryTet = baseTet;
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and move into the second chain.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == static_cast<int>(bdryTet))
        return false;

    // Try each possible direction away from the working faces into the
    // second chain.
    FacePair chainFaces;
    size_t chainTet;
    FacetSpec<3> destFace;
    unsigned ignoreFace;
    int i;
    for (i = 0; i < 2; i++) {
        destFace = dest(bdryTet,
            i == 0 ? bdryFaces.lower() : bdryFaces.upper());
        if (destFace.isBoundary(size_))
            continue;

        for (ignoreFace = 0; ignoreFace < 4; ignoreFace++) {
            if (destFace.facet == static_cast<int>(ignoreFace))
                continue;
            // Try to follow the chain along from tetrahedron
            // destFace.simp, using the two faces that are *not*
            // destFace.facet or ignoreFace.
            chainTet = destFace.simp;
            chainFaces = FacePair(destFace.facet, ignoreFace).complement();
            followChain(chainTet, chainFaces);

            // Did we reach an end edge of the second chain?
            if (dest(chainTet, chainFaces.lower()).simp ==
                    static_cast<int>(chainTet))
                return true;
        }
    }

    // Nup.  Nothing found.
    return false;
}

bool FacetPairing<3>::hasOneEndedChainWithDoubleHandle() const {
    // Search for the end edge of the chain.
    size_t baseTet;
    unsigned baseFace;
    for (baseTet = 0; baseTet < size_; baseTet++)
        for (baseFace = 0; baseFace < 3; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<int>(baseTet)) {
                // Here's a face that matches to the same tetrahedron.
                if (hasOneEndedChainWithDoubleHandle(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more
                // self-identifications in this tetrahedron, since if
                // there's another (different) one it must be a
                // one-tetrahedron component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

bool FacetPairing<3>::hasOneEndedChainWithDoubleHandle(size_t baseTet,
        unsigned baseFace) const {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    size_t bdryTet = baseTet;
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and create the double handle.
    FacetSpec<3> dest1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<3> dest2 = dest(bdryTet, bdryFaces.upper());

    // These two faces must be joined to two distinct tetrahedra.
    if (dest1.simp == dest2.simp)
        return false;

    // They also cannot be boundary.
    if (dest1.isBoundary(size_) || dest2.isBoundary(size_))
        return false;

    // Since they're joined to two distinct tetrahedra, they cannot be
    // joined to each other.  So we can start hunting for the double handle.
    int handle = 0;
    for (int i = 0; i < 4; i++)
        if (dest(dest1.simp, i).simp == dest2.simp)
            handle++;

    // Did we find our double handle?
    return (handle >= 2);
}

bool FacetPairing<3>::hasWedgedDoubleEndedChain() const {
    // Search for the end edge of the first chain.
    size_t baseTet;
    unsigned baseFace;
    // Skip the last tetrahedron -- any of the two ends will do.
    for (baseTet = 0; baseTet + 1 < size_; baseTet++)
        for (baseFace = 0; baseFace < 3; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<int>(baseTet)) {
                // Here's a face that matches to the same tetrahedron.
                if (hasWedgedDoubleEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more
                // self-identifications in this tetrahedron, since if
                // there's another (different) one it must be a
                // one-tetrahedron component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

bool FacetPairing<3>::hasWedgedDoubleEndedChain(size_t baseTet,
        unsigned baseFace) const {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    size_t bdryTet = baseTet;
    followChain(bdryTet, bdryFaces);

    // Here we expect to find the wedge.
    FacetSpec<3> dest1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<3> dest2 = dest(bdryTet, bdryFaces.upper());

    if (dest1.isBoundary(size_) || dest2.isBoundary(size_) ||
            dest1.simp == dest2.simp)
        return false;

    // We are joined to two new and distinct graph vertices.
    // Hunt for the edge joining them, and also see where they follow
    // through to beyond these two new vertices.
    // Drawing a diagram whilst reading this code will certainly help. :)
    FacetSpec<3> throughFace[2][3];
    int nThroughFaces[2];
    nThroughFaces[0] = nThroughFaces[1] = 0;

    int i, j;
    FacetSpec<3> nextDest;
    bool foundCrossEdge = false;
    for (i = 0; i < 4; i++) {
        if (i != dest1.facet) {
            nextDest = dest(dest1.simp, i);
            if (nextDest.simp == dest2.simp)
                foundCrossEdge = true;
            else if (nextDest.simp != dest1.simp &&
                    ! nextDest.isBoundary(size_))
                throughFace[0][nThroughFaces[0]++] = nextDest;
        }
        if (i != dest2.facet) {
            nextDest = dest(dest2.simp, i);
            if (nextDest.simp != dest1.simp && nextDest.simp != dest2.simp &&
                    ! nextDest.isBoundary(size_))
                throughFace[1][nThroughFaces[1]++] = nextDest;
        }
    }

    if (! foundCrossEdge)
        return false;

    // We have our cross edge.
    // Moreover, all of the faces in throughFace[] belong to previously
    // unseen tetrahedra.
    // Hunt for the other half of the double-ended chain.
    FacePair chainFaces;
    size_t chainTet;
    for (i = 0; i < nThroughFaces[0]; i++)
        for (j = 0; j < nThroughFaces[1]; j++)
            if (throughFace[0][i].simp == throughFace[1][j].simp) {
                // Bingo.
                // Follow the chain and see if it ends in a loop.
                chainTet = throughFace[0][i].simp;
                chainFaces = FacePair(throughFace[0][i].facet,
                    throughFace[1][j].facet).complement();
                followChain(chainTet, chainFaces);

                if (dest(chainTet, chainFaces.lower()).simp ==
                        static_cast<int>(chainTet))
                    return true;
            }

    // Nothing found.
    return false;
}

bool FacetPairing<3>::hasOneEndedChainWithStrayBigon() const {
    // Search for the end edge of the chain.
    size_t baseTet;
    unsigned baseFace;
    for (baseTet = 0; baseTet < size_; baseTet++)
        for (baseFace = 0; baseFace < 3; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<int>(baseTet)) {
                // Here's a face that matches to the same tetrahedron.
                if (hasOneEndedChainWithStrayBigon(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more
                // self-identifications in this tetrahedron, since if
                // there's another (different) one it must be a
                // one-tetrahedron component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

bool FacetPairing<3>::hasOneEndedChainWithStrayBigon(size_t baseTet,
        unsigned baseFace) const {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    size_t bdryTet = baseTet;
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and create the stray bigon.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == static_cast<int>(bdryTet))
        return false;

    // Try each possible direction away from the working faces into the bigon.
    FacePair bigonFaces;
    int bigonTet, farTet, extraTet;
    FacetSpec<3> destFace;
    unsigned ignoreFace;
    int i;
    for (i = 0; i < 2; i++) {
        destFace = dest(bdryTet,
            i == 0 ? bdryFaces.lower() : bdryFaces.upper());
        if (destFace.isBoundary(size_))
            continue;
        bigonTet = destFace.simp;

        for (ignoreFace = 0; ignoreFace < 4; ignoreFace++) {
            if (destFace.facet == static_cast<int>(ignoreFace))
                continue;
            // Look for a bigon running away from tetrahedron
            // destFace.simp, using the two faces that are *not*
            // destFace.facet or ignoreFace.
            bigonFaces = FacePair(destFace.facet, ignoreFace).complement();

            farTet = dest(bigonTet, bigonFaces.upper()).simp;
            if (farTet != bigonTet &&
                    farTet < static_cast<int>(size_) /* non-bdry */ &&
                    farTet == dest(bigonTet, bigonFaces.lower()).simp) {
                // We have the bigon!
                // We know that bdryTet != bigonTet != farTet, and we
                // can prove that bdryTet != farTet using 4-valency.

                // Ensure that we don't have one of our special exceptions.
                extraTet = dest(bdryTet,
                    i == 0 ? bdryFaces.upper() : bdryFaces.lower()).simp;
                // We know extraTet != bigonTet, since otherwise our
                // one-ended chain would not have stopped when it did.
                // We also know extraTet != bdryTet by 4-valency.
                if (extraTet == farTet ||
                        extraTet >= static_cast<int>(size_) /* bdry */)
                    return true;
                if (extraTet == dest(bigonTet, ignoreFace).simp) {
                    // Could be the special case where extraTet joins to
                    // all of bdryTet, bigonTet and farTet.
                    // We already have it joined to bdryTet and bigonTet.
                    // Check farTet.
                    if (extraTet != dest(farTet, 0).simp &&
                            extraTet != dest(farTet, 1).simp &&
                            extraTet != dest(farTet, 2).simp &&
                            extraTet != dest(farTet, 3).simp)
                        return true;
                } else {
                    // Could be the special case where extraTet joins
                    // twice to farTet.  If not, we have the type of
                    // graph we're looking for.
                    bigonFaces = FacePair(
                        dest(bigonTet, bigonFaces.upper()).facet,
                        dest(bigonTet, bigonFaces.lower()).facet).complement();
                    if (extraTet != dest(farTet, bigonFaces.upper()).simp ||
                            extraTet != dest(farTet, bigonFaces.lower()).simp)
                        return true;
                }
            }
        }
    }

    // Nup.  Nothing found.
    return false;
}

bool FacetPairing<3>::hasTripleOneEndedChain() const {
    // Search for the end edge of the first chain.
    size_t baseTet;
    unsigned baseFace;
    // Skip the last two tetrahedra -- any of the three chains will do.
    for (baseTet = 0; baseTet + 2 < size_; baseTet++)
        for (baseFace = 0; baseFace < 3; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<int>(baseTet)) {
                // Here's a face that matches to the same tetrahedron.
                if (hasTripleOneEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more
                // self-identifications in this tetrahedron, since if
                // there's another (different) one it must be a
                // one-tetrahedron component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

bool FacetPairing<3>::hasTripleOneEndedChain(size_t baseTet,
        unsigned baseFace) const {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    size_t bdryTet = baseTet;
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and hunt for the other two chains.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == static_cast<int>(bdryTet))
        return false;

    FacetSpec<3> axis1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<3> axis2 = dest(bdryTet, bdryFaces.upper());
    if (axis1.isBoundary(size_) || axis2.isBoundary(size_))
        return false;

    // We know axis1.simp != axis2.simp because the chain stopped, but
    // just in case..
    if (axis1.simp == axis2.simp)
        return false;

    // Count the number of other chains coming from axis1 and axis2.
    int exit1, exit2;
    FacetSpec<3> arrive1, arrive2;
    int nChains = 1;
    size_t newChainTet;
    FacePair newChainFaces;
    for (exit1 = 0; exit1 < 4; exit1++) {
        if (exit1 == axis1.facet)
            continue;
        arrive1 = dest(axis1.simp, exit1);
        if (arrive1.simp == static_cast<int>(bdryTet) ||
                arrive1.simp == axis1.simp || arrive1.simp == axis2.simp ||
                arrive1.isBoundary(size_))
            continue;

        for (exit2 = 0; exit2 < 4; exit2++) {
            if (exit2 == axis2.facet)
                continue;
            arrive2 = dest(axis2.simp, exit2);
            if (arrive2.simp != arrive1.simp)
                continue;

            // We have graph edges from axis1 and axis2 to a common vertex,
            // which is not part of our original chain and is neither axis1
            // nor axis2.

            // See if there's a (possibly zero-length) chain we can
            // follow to a loop.
            newChainTet = arrive1.simp;
            newChainFaces = FacePair(arrive1.facet, arrive2.facet).
                complement();
            followChain(newChainTet, newChainFaces);

            if (dest(newChainTet, newChainFaces.lower()).simp ==
                    static_cast<int>(newChainTet)) {
                // Got one!
                if (++nChains == 3)
                    return true;
            }
        }
    }

    // Nope.  Not enough chains were found.
    return false;
}

bool FacetPairing<3>::hasSingleStar() const {
    int half[4], all[8];

    size_t first, second;
    unsigned f1, f2;
    int i;

    // Skip the last tetrahedron, since we're already testing every
    // possibility from both sides.
    for (first = 0; first + 1 < size_; first++) {
        // All four neighbours must be non-boundary and distinct.
        for (f1 = 0; f1 < 4; f1++) {
            half[f1] = dest(first, f1).simp;
            if (half[f1] >= static_cast<int>(size_) /* bdry */)
                break;
        }
        if (f1 < 4)
            continue;

        std::sort(half, half + 4);
        if (half[0] == half[1] || half[1] == half[2] || half[2] == half[3])
            continue;

        // Look for the adjacent neighbour.
        for (f1 = 0; f1 < 4; f1++) {
            second = dest(first, f1).simp;

            // Now ensure that all eight faces are non-boundary and distinct.
            for (f2 = 0; f2 < 4; f2++) {
                all[f2 + 4] = dest(second, f2).simp;
                if (all[f2 + 4] >= static_cast<int>(size_) /* bdry */)
                    break;
            }
            if (f2 < 4)
                continue;

            // We have to refresh the first half of the all[] array each
            // time, since every time we sort all[] we mix the first
            // tetrahedron's neighbours in with the second tetrahedron's
            // neighbours.
            std::copy(half, half + 4, all);
            std::sort(all, all + 8);
            for (i = 0; i < 7; i++)
                if (all[i] == all[i + 1])
                    break;
            if (i >= 7)
                return true;
        }
    }

    return false;
}

bool FacetPairing<3>::hasDoubleStar() const {
    int all[7];

    size_t first, second;
    int f, i;

    // Skip the last tetrahedron, since we're already testing every
    // possibility from both sides.
    for (first = 0; first + 1 < size_; first++) {
        // All four neighbours must be non-boundary, and three must be
        // distinct.
        for (f = 0; f < 4; f++) {
            all[f] = dest(first, f).simp;
            if (all[f] >= static_cast<int>(size_) /* bdry */)
                break;
        }
        if (f < 4)
            continue;

        std::sort(all, all + 4);

        // Find the double edge, and move the three distinct tetrahedra
        // to the beginning of the array.
        if (all[0] == all[1] && all[1] != all[2] && all[2] != all[3]) {
            second = all[0];
            all[0] = all[3];
        } else if (all[0] != all[1] && all[1] == all[2] && all[2] != all[3]) {
            second = all[1];
            all[1] = all[3];
        } else if (all[0] != all[1] && all[1] != all[2] && all[2] == all[3]) {
            second = all[2];
        } else
            continue;

        // Now look at the edges coming out from the second tetrahedron.
        for (f = 0; f < 4; f++) {
            all[f + 3] = dest(second, f).simp;
            if (all[f + 3] >= static_cast<int>(size_) /* bdry */)
                break;
        }
        if (f < 4)
            continue;

        // Look for duplicates.  We should only have a single duplicate
        // pair, this being two copies of first.
        std::sort(all, all + 7);
        for (i = 0; i < 6; i++)
            if (all[i] == all[i + 1]) {
                if (all[i] != static_cast<int>(first))
                    break;
                if (i < 5 && all[i] == all[i + 2])
                    break;
            }
        if (i >= 6)
            return true;
    }

    return false;
}

bool FacetPairing<3>::hasDoubleSquare() const {
    size_t t1;
    FacetSpec<3> t2;
    int join, fa, fb;
    int adj1 = 0, adj2 = 0;
    bool found;

    // Skip the last three tetrahedra -- any of the four starting points
    // will do.
    for (t1 = 0; t1 + 3 < size_; t1++)
        for (join = 0; join < 4; join++) {
            t2 = dest(t1, join);
            if (t2.simp == static_cast<int>(t1) || t2.isBoundary(size_))
                continue;

            // We have distinct t1, t2 adjacent.
            // Search for double edges leaving t1 and t2 for two new
            // tetrahedra.
            found = false;
            for (fa = 0; fa < 3 && ! found; fa++) {
                if (fa == join)
                    continue;
                adj1 = dest(t1, fa).simp;
                if (adj1 >= static_cast<int>(size_) /* bdry */)
                    continue;
                if (adj1 == static_cast<int>(t1) || adj1 == t2.simp)
                    continue;
                for (fb = fa + 1; fb < 4; fb++) {
                    if (fb == join)
                        continue;
                    if (adj1 == dest(t1, fb).simp) {
                        found = true;
                        break;
                    }
                }
            }
            if (! found)
                continue;

            found = false;
            for (fa = 0; fa < 3 && ! found; fa++) {
                if (fa == t2.facet)
                    continue;
                adj2 = dest(t2.simp, fa).simp;
                if (adj2 >= static_cast<int>(size_) /* bdry */)
                    continue;
                if (adj2 == static_cast<int>(t1) || adj2 == t2.simp ||
                        adj2 == adj1)
                    continue;
                for (fb = fa + 1; fb < 4; fb++) {
                    if (fb == t2.facet)
                        continue;
                    if (adj2 == dest(t2.simp, fb).simp) {
                        found = true;
                        break;
                    }
                }
            }
            if (! found)
                continue;

            // All we need now is a link between adj1 and adj2.
            for (fa = 0; fa < 4; fa++)
                if (dest(adj1, fa).simp == adj2)
                    return true;
        }

    // Nothing found.
    return false;
}

} // namespace regina

