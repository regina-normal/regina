
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <algorithm>
#include <array>
#include <sstream>
#include <vector>
#include "triangulation/facepair.h"
#include "triangulation/facetpairing.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

// This file includes the implementations of graph structure queries that are
// specific to dimension 3.
//
// We keep these as generic functions with `requires (dim == 3)` as opposed to
// specialisations, since this avoids instantiating the entire class
// FacetPairing<3> every time someone imports the header facetpairing.h.
//
// We keep the implementation quarantined here in facetpairing.cpp (and not in
// any header at all) since they really are only 3-dimensional functions.
// However, this also means we must explicitly instantiate each function after
// defining it, so that the linker can find it.

namespace {
    /**
     * Identifies whether the given list contains any duplicates.
     *
     * \pre The given list is sorted.
     */
    template <std::forward_iterator Iterator>
    bool sortedListHasDuplicates(Iterator begin, Iterator end) {
        if (begin == end)
            return false;
        for (Iterator it = begin, next = std::next(it); next != end;
                it = next++)
            if (*it == *next)
                return true;
        return false;
    }

    /**
     * If the given list has exactly one duplicate pair, returns the location
     * of the first occurrence.  Otherwise returns *end*.
     *
     * \pre The given list is sorted.
     */
    template <std::forward_iterator Iterator>
    Iterator singleDuplicateInSortedList(Iterator begin, Iterator end) {
        Iterator ans = end;

        if (begin == end)
            return ans;
        for (Iterator it = begin, next = std::next(it); next != end;
                it = next++)
            if (*it == *next) {
                if (ans == end)
                    ans = it; // we found our first duplicate pair
                else
                    return end; // there are multiple duplicate pairs
            }
        return ans;
    }
}

namespace regina {

template <int dim> requires (supportedDim(dim))
void FacetPairing<dim>::followChain(ssize_t& tet, FacePair& faces) const
        requires (dim == 3) {
    FacetSpec<dim> dest1, dest2;
    while (true) {
        // Does the first face lead to a real simplex?
        if (isUnmatched(tet, faces.lower()))
            return;

        // Does the second face lead to the same simplex as the first?
        dest1 = dest(tet, faces.lower());
        dest2 = dest(tet, faces.upper());
        if (dest1.simp != dest2.simp)
            return;

        // Do the two faces lead to a *different* simplex?
        if (dest1.simp == tet)
            return;

        // Follow the chain along.
        tet = dest1.simp;
        faces = FacePair(dest1.facet, dest2.facet).complement();
    }
}

template void FacetPairing<3>::followChain(ssize_t&, FacePair&) const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasBrokenDoubleEndedChain(size_t baseTet,
        int baseFace) const requires (dim == 3) {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    auto bdryTet = static_cast<ssize_t>(baseTet);
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and move into the second chain.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == bdryTet)
        return false;

    // Try each possible direction away from the working faces into the
    // second chain.
    for (int i = 0; i < 2; i++) {
        FacetSpec<dim> destFace = dest(bdryTet,
            i == 0 ? bdryFaces.lower() : bdryFaces.upper());
        if (destFace.isBoundary(size_))
            continue;

        for (int ignoreFace = 0; ignoreFace <= dim; ignoreFace++) {
            if (destFace.facet == ignoreFace)
                continue;
            // Try to follow the chain along from simplex
            // destFace.simp, using the two faces that are *not*
            // destFace.facet or ignoreFace.
            ssize_t chainTet = destFace.simp;
            FacePair chainFaces =
                FacePair(destFace.facet, ignoreFace).complement();
            followChain(chainTet, chainFaces);

            // Did we reach an end edge of the second chain?
            if (dest(chainTet, chainFaces.lower()).simp == chainTet)
                return true;
        }
    }

    // Nup.  Nothing found.
    return false;
}

template bool FacetPairing<3>::hasBrokenDoubleEndedChain(size_t, int) const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasBrokenDoubleEndedChain() const requires (dim == 3) {
    // Search for the end edge of the first chain.
    // Skip the last simplex -- any of the two ends will do.
    for (size_t baseTet = 0; baseTet + 1 < size_; baseTet++)
        for (int baseFace = 0; baseFace < dim; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<ssize_t>(baseTet)) {
                // Here's a face that matches to the same simplex.
                if (hasBrokenDoubleEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more self-identifications
                // in this simplex, since if there's another (different) one
                // it must be a one-simplex component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

template bool FacetPairing<3>::hasBrokenDoubleEndedChain() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasOneEndedChainWithDoubleHandle(size_t baseTet,
        int baseFace) const requires (dim == 3) {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    auto bdryTet = static_cast<ssize_t>(baseTet);
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and create the double handle.
    FacetSpec<dim> dest1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<dim> dest2 = dest(bdryTet, bdryFaces.upper());

    // These two faces must be joined to two distinct simplices.
    if (dest1.simp == dest2.simp)
        return false;

    // They also cannot be boundary.
    if (dest1.isBoundary(size_) || dest2.isBoundary(size_))
        return false;

    // Since they're joined to two distinct simplices, they cannot be
    // joined to each other.  So we can start hunting for the double handle.
    int handle = 0;
    for (int i = 0; i <= dim; i++)
        if (dest(dest1.simp, i).simp == dest2.simp)
            handle++;

    // Did we find our double handle?
    return (handle >= 2);
}

template bool FacetPairing<3>::hasOneEndedChainWithDoubleHandle(size_t, int)
    const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasOneEndedChainWithDoubleHandle() const
        requires (dim == 3) {
    // Search for the end edge of the chain.
    for (size_t baseTet = 0; baseTet < size_; baseTet++)
        for (int baseFace = 0; baseFace < dim; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<ssize_t>(baseTet)) {
                // Here's a face that matches to the same simplex.
                if (hasOneEndedChainWithDoubleHandle(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more self-identifications
                // in this simplex, since if there's another (different) one
                // it must be a one-simplex component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

template bool FacetPairing<3>::hasOneEndedChainWithDoubleHandle() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasWedgedDoubleEndedChain(size_t baseTet,
        int baseFace) const requires (dim == 3) {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    auto bdryTet = static_cast<ssize_t>(baseTet);
    followChain(bdryTet, bdryFaces);

    // Here we expect to find the wedge.
    FacetSpec<dim> dest1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<dim> dest2 = dest(bdryTet, bdryFaces.upper());

    if (dest1.isBoundary(size_) || dest2.isBoundary(size_) ||
            dest1.simp == dest2.simp)
        return false;

    // We are joined to two new and distinct graph vertices.
    // Hunt for the edge joining them, and also see where they follow
    // through to beyond these two new vertices.
    // Drawing a diagram whilst reading this code will certainly help. :)
    FacetSpec<dim> throughFace[2][dim];
    int nThroughFaces[2] = { 0, 0 };

    FacetSpec<dim> nextDest;
    bool foundCrossEdge = false;
    for (int i = 0; i <= dim; i++) {
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
    // unseen simplices.
    // Hunt for the other half of the double-ended chain.
    for (int i = 0; i < nThroughFaces[0]; i++)
        for (int j = 0; j < nThroughFaces[1]; j++)
            if (throughFace[0][i].simp == throughFace[1][j].simp) {
                // Bingo.
                // Follow the chain and see if it ends in a loop.
                ssize_t chainTet = throughFace[0][i].simp;
                FacePair chainFaces = FacePair(throughFace[0][i].facet,
                    throughFace[1][j].facet).complement();
                followChain(chainTet, chainFaces);

                if (dest(chainTet, chainFaces.lower()).simp == chainTet)
                    return true;
            }

    // Nothing found.
    return false;
}

template bool FacetPairing<3>::hasWedgedDoubleEndedChain(size_t, int) const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasWedgedDoubleEndedChain() const requires (dim == 3) {
    // Search for the end edge of the first chain.
    // Skip the last simplex -- any of the two ends will do.
    for (size_t baseTet = 0; baseTet + 1 < size_; baseTet++)
        for (int baseFace = 0; baseFace < dim; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<ssize_t>(baseTet)) {
                // Here's a face that matches to the same simplex.
                if (hasWedgedDoubleEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more self-identifications
                // in this simplex, since if there's another (different) one
                // it must be a one-simplex component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

template bool FacetPairing<3>::hasWedgedDoubleEndedChain() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasOneEndedChainWithStrayBigon(size_t baseTet,
        int baseFace) const requires (dim == 3) {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    auto bdryTet = static_cast<ssize_t>(baseTet);
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and create the stray bigon.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == bdryTet)
        return false;

    // Try each possible direction away from the working faces into the bigon.
    for (int i = 0; i < 2; i++) {
        FacetSpec<dim> destFace = dest(bdryTet,
            i == 0 ? bdryFaces.lower() : bdryFaces.upper());
        if (destFace.isBoundary(size_))
            continue;
        ssize_t bigonTet = destFace.simp;

        for (int ignoreFace = 0; ignoreFace <= dim; ignoreFace++) {
            if (destFace.facet == ignoreFace)
                continue;
            // Look for a bigon running away from simplex
            // destFace.simp, using the two faces that are *not*
            // destFace.facet or ignoreFace.
            FacePair bigonFaces =
                FacePair(destFace.facet, ignoreFace).complement();

            ssize_t farTet = dest(bigonTet, bigonFaces.upper()).simp;
            if (farTet != bigonTet &&
                    farTet < static_cast<ssize_t>(size_) /* non-bdry */ &&
                    farTet == dest(bigonTet, bigonFaces.lower()).simp) {
                // We have the bigon!
                // We know that bdryTet != bigonTet != farTet, and we
                // can prove that bdryTet != farTet using 4-valency.

                // Ensure that we don't have one of our special exceptions.
                ssize_t extraTet = dest(bdryTet,
                    i == 0 ? bdryFaces.upper() : bdryFaces.lower()).simp;
                // We know extraTet != bigonTet, since otherwise our
                // one-ended chain would not have stopped when it did.
                // We also know extraTet != bdryTet by 4-valency.
                if (extraTet == farTet ||
                        extraTet >= static_cast<ssize_t>(size_) /* bdry */)
                    return true;
                if (extraTet == dest(bigonTet, ignoreFace).simp) {
                    // Could be the special case where extraTet joins to
                    // all of bdryTet, bigonTet and farTet.
                    // We already have it joined to bdryTet and bigonTet.
                    // Check farTet.
                    bool found = false;
                    for (int f = 0; f <= dim; ++f)
                        if (extraTet == dest(farTet, f).simp) {
                            found = true;
                            break;
                        }
                    if (! found)
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

template bool FacetPairing<3>::hasOneEndedChainWithStrayBigon(size_t, int)
    const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasOneEndedChainWithStrayBigon() const
        requires (dim == 3) {
    // Search for the end edge of the chain.
    for (size_t baseTet = 0; baseTet < size_; baseTet++)
        for (int baseFace = 0; baseFace < dim; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<ssize_t>(baseTet)) {
                // Here's a face that matches to the same simplex.
                if (hasOneEndedChainWithStrayBigon(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more self-identifications
                // in this simplex, since if there's another (different) one
                // it must be a one-simplex component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

template bool FacetPairing<3>::hasOneEndedChainWithStrayBigon() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasTripleOneEndedChain(size_t baseTet,
        int baseFace) const requires (dim == 3) {
    // Follow the chain along and see how far we get.
    FacePair bdryFaces =
        FacePair(baseFace, dest(baseTet, baseFace).facet).complement();
    auto bdryTet = static_cast<ssize_t>(baseTet);
    followChain(bdryTet, bdryFaces);

    // Here's where we must diverge and hunt for the other two chains.

    // We cannot glue the working pair of faces to each other.
    if (dest(bdryTet, bdryFaces.lower()).simp == bdryTet)
        return false;

    FacetSpec<dim> axis1 = dest(bdryTet, bdryFaces.lower());
    FacetSpec<dim> axis2 = dest(bdryTet, bdryFaces.upper());
    if (axis1.isBoundary(size_) || axis2.isBoundary(size_))
        return false;

    // We know axis1.simp != axis2.simp because the chain stopped, but
    // just in case..
    if (axis1.simp == axis2.simp)
        return false;

    // Count the number of other chains coming from axis1 and axis2.
    int nChains = 1;
    for (int exit1 = 0; exit1 <= dim; exit1++) {
        if (exit1 == axis1.facet)
            continue;
        FacetSpec<dim> arrive1 = dest(axis1.simp, exit1);
        if (arrive1.simp == bdryTet || arrive1.simp == axis1.simp ||
                arrive1.simp == axis2.simp || arrive1.isBoundary(size_))
            continue;

        for (int exit2 = 0; exit2 <= dim; exit2++) {
            if (exit2 == axis2.facet)
                continue;
            FacetSpec<dim> arrive2 = dest(axis2.simp, exit2);
            if (arrive2.simp != arrive1.simp)
                continue;

            // We have graph edges from axis1 and axis2 to a common vertex,
            // which is not part of our original chain and is neither axis1
            // nor axis2.

            // See if there's a (possibly zero-length) chain we can
            // follow to a loop.
            ssize_t newChainTet = arrive1.simp;
            FacePair newChainFaces = FacePair(arrive1.facet, arrive2.facet).
                complement();
            followChain(newChainTet, newChainFaces);

            if (dest(newChainTet, newChainFaces.lower()).simp == newChainTet) {
                // Got one!
                if (++nChains == 3)
                    return true;
            }
        }
    }

    // Nope.  Not enough chains were found.
    return false;
}

template bool FacetPairing<3>::hasTripleOneEndedChain(size_t, int) const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasTripleOneEndedChain() const requires (dim == 3) {
    // Search for the end edge of the first chain.
    // Skip the last two simplices -- any of the three chains will do.
    for (size_t baseTet = 0; baseTet + 2 < size_; baseTet++)
        for (int baseFace = 0; baseFace < dim; baseFace++)
            if (dest(baseTet, baseFace).simp == static_cast<ssize_t>(baseTet)) {
                // Here's a face that matches to the same simplex.
                if (hasTripleOneEndedChain(baseTet, baseFace))
                    return true;

                // There's no sense in looking for more self-identifications
                // in this simplex, since if there's another (different) one
                // it must be a one-simplex component (and so not applicable).
                break;
            }

    // Nothing found.  Boring.
    return false;
}

template bool FacetPairing<3>::hasTripleOneEndedChain() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasSingleStar() const requires (dim == 3) {
    static constexpr int nHalf = dim + 1;
    std::array<size_t, nHalf> half;
    std::array<size_t, 2 * nHalf> all;
    int f1, f2, i;

    // Skip the last simplex, since we're already testing every possibility
    // from both sides.
    for (size_t first = 0; first + 1 < size_; first++) {
        // All potential neighbours of the first centre simplex must exist
        // and be distinct.
        for (f1 = 0; f1 <= dim; f1++) {
            half[f1] = dest(first, f1).simp;
            if (half[f1] >= size_ /* bdry */)
                break;
        }
        if (f1 <= dim)
            continue;

        std::sort(half.begin(), half.end());
        if (sortedListHasDuplicates(half.begin(), half.end()))
            continue;

        // Look for an adjacent neighbour to be the second centre simplex.
        for (f1 = 0; f1 <= dim; f1++) {
            size_t second = dest(first, f1).simp;

            // Now ensure that all potential neighbours of _both_ centre
            // simplices exist and are distinct.
            for (f2 = 0; f2 <= dim; f2++) {
                all[f2 + nHalf] = dest(second, f2).simp;
                if (all[f2 + nHalf] >= size_ /* bdry */)
                    break;
            }
            if (f2 <= dim)
                continue;

            // We have to refresh the first half of the all[] array each
            // time, since every time we sort all[] we mix the first
            // centre's neighbours in with the second centre's neighbours.
            std::copy(half.begin(), half.end(), all.begin());
            std::sort(all.begin(), all.end());
            if (! sortedListHasDuplicates(all.begin(), all.end()))
                return true;
        }
    }

    return false;
}

template bool FacetPairing<3>::hasSingleStar() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasDoubleStar() const requires (dim == 3) {
    static constexpr int nHalf = dim + 1;
    std::array<size_t, 2 * nHalf - 1> all;

    // Skip the last simplex, since we're already testing every possibility
    // from both sides.
    for (size_t first = 0; first + 1 < size_; first++) {
        int f;

        // All potential neighbours of the first centre simplex must exist,
        // and there should be exactly one duplicate pair.
        for (f = 0; f <= dim; f++) {
            all[f] = dest(first, f).simp;
            if (all[f] >= size_ /* bdry */)
                break;
        }
        if (f <= dim)
            continue;

        auto halfway = all.begin() + nHalf;
        std::sort(all.begin(), halfway);
        auto dup = singleDuplicateInSortedList(all.begin(), halfway);
        if (dup == halfway)
            continue;
        size_t second = *dup;

        // Compress the neighbours into a smaller list of size (halfway - 1)
        // whose entries are unique (overwriting the second copy of *second*).
        // We are happy to break sorted order when doing this.
        ++dup; // move to the second copy of *second*, which we will overwrite
        if (dup != halfway - 1)
            *dup = *(halfway - 1); // move the final list element into its place

        // Now look at neighbours of the second centre simplex.
        for (f = 0; f <= dim; f++) {
            all[f + nHalf - 1] = dest(second, f).simp;
            if (all[f + nHalf - 1] >= size_ /* bdry */)
                break;
        }
        if (f <= dim)
            continue;

        // Look for duplicates in the combined set of neighbours (recalling
        // that we already removed the duplicate copy of *second*).  We should
        // only have a single duplicate pair remaining, this being two copies
        // of *first*.
        std::sort(all.begin(), all.end());
        dup = singleDuplicateInSortedList(all.begin(), all.end());
        if (dup != all.end() && *dup == first)
            return true;
    }

    return false;
}

template bool FacetPairing<3>::hasDoubleStar() const;

template <int dim> requires (supportedDim(dim))
bool FacetPairing<dim>::hasDoubleSquare() const requires (dim == 3) {
    // We use ssize_t everywhere here because it makes for fewer
    // signed/unsigned comparisons that need to be cast away.
    ssize_t adj1 = 0, adj2 = 0;

    // Skip the last three simplices -- any of the four starting points will do.
    for (ssize_t t1 = 0; t1 + 3 < static_cast<ssize_t>(size_); t1++)
        for (int join = 0; join <= dim; join++) {
            FacetSpec<dim> t2 = dest(t1, join);
            if (t2.simp == t1 || t2.isBoundary(size_))
                continue;

            // We have distinct t1, t2 adjacent.
            // Search for double edges leaving t1 and t2 for two new simplices.
            bool found = false;
            for (int fa = 0; fa < dim && ! found; fa++) {
                if (fa == join)
                    continue;
                adj1 = dest(t1, fa).simp;
                if (adj1 >= static_cast<ssize_t>(size_) /* bdry */)
                    continue;
                if (adj1 == t1 || adj1 == t2.simp)
                    continue;
                for (int fb = fa + 1; fb <= dim; fb++) {
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
            for (int fa = 0; fa < dim && ! found; fa++) {
                if (fa == t2.facet)
                    continue;
                adj2 = dest(t2.simp, fa).simp;
                if (adj2 >= static_cast<ssize_t>(size_) /* bdry */)
                    continue;
                if (adj2 == t1 || adj2 == t2.simp || adj2 == adj1)
                    continue;
                for (int fb = fa + 1; fb <= dim; fb++) {
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
            for (int fa = 0; fa <= dim; fa++)
                if (dest(adj1, fa).simp == adj2)
                    return true;
        }

    // Nothing found.
    return false;
}

template bool FacetPairing<3>::hasDoubleSquare() const;

} // namespace regina

