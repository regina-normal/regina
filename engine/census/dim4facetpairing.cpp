
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <algorithm>
#include <sstream>
#include <vector>
#include "census/dim4facetpairing.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"
#include "utilities/memutils.h"
#include "utilities/stringutils.h"

namespace regina {

namespace {
    /**
     * Holds the arguments passed to Dim4FacetPairing::findAllPairings().
     */
    struct Dim4FacetPairingArgs {
        NBoolSet boundary;
        int nBdryFacets;
        UseDim4FacetPairing use;
        void* useArgs;
    };
}

Dim4FacetPairing::Dim4FacetPairing(const Dim4FacetPairing& cloneMe) : NThread(),
        nPentachora_(cloneMe.nPentachora_),
        pairs_(new Dim4PentFacet[cloneMe.nPentachora_ * 5]) {
    std::copy(cloneMe.pairs_, cloneMe.pairs_ + (nPentachora_ * 5), pairs_);
}

Dim4FacetPairing::Dim4FacetPairing(const Dim4Triangulation& tri) :
        nPentachora_(tri.getNumberOfPentachora()),
        pairs_(new Dim4PentFacet[tri.getNumberOfPentachora() * 5]) {
    unsigned p, f, index;
    const Dim4Pentachoron *pent, *adj;
    for (index = 0, p = 0; p < nPentachora_; ++p) {
        pent = tri.getPentachoron(p);
        for (f = 0; f < 5; ++f) {
            adj = pent->adjacentPentachoron(f);
            if (adj) {
                pairs_[index].pent = tri.pentachoronIndex(adj);
                pairs_[index].facet = pent->adjacentFacet(f);
            } else
                pairs_[index].setBoundary(nPentachora_);

            ++index;
        }
    }
}

bool Dim4FacetPairing::isClosed() const {
    for (Dim4PentFacet f(0, 0); ! f.isPastEnd(nPentachora_, true); ++f)
        if (isUnmatched(f))
            return false;
    return true;
}

std::string Dim4FacetPairing::toString() const {
    std::ostringstream ans;

    for (Dim4PentFacet f(0, 0); ! f.isPastEnd(nPentachora_, true); ++f) {
        if (f.facet == 0 && f.pent > 0)
            ans << " | ";
        else if (f.pent || f.facet)
            ans << ' ';

        if (dest(f).isBoundary(nPentachora_))
            ans << "bdry";
        else
            ans << dest(f).pent << ':' << dest(f).facet;
    }
    return ans.str();
}

void Dim4FacetPairing::writeDotHeader(std::ostream& out, const char* graphName) {
    static const char defaultGraphName[] = "G";

    if ((! graphName) || (! *graphName))
        graphName = defaultGraphName;

    out << "graph " << graphName << " {" << std::endl;
    out << "graph [bgcolor=white];" << std::endl;
    out << "edge [color=black];" << std::endl;
    out << "node [shape=circle,style=filled,height=0.15,fixedsize=true,label=\"\"];" << std::endl;
}

void Dim4FacetPairing::writeDot(std::ostream& out, const char* prefix,
        bool subgraph) const {
    static const char defaultPrefix[] = "g";

    if ((! prefix) || (! *prefix))
        prefix = defaultPrefix;

    // We are guaranteed that prefix is a non-empty string.

    if (subgraph)
        out << "subgraph cluster_" << prefix << " {" << std::endl;
    else
        writeDotHeader(out, (prefix + std::string("_graph")).c_str());

    // Ancient versions of graphviz seem to ignore the default label="".
    // Make this explicit for each node.
    unsigned p;
    for (p = 0; p < nPentachora_; ++p)
        out << prefix << '_' << p << " [label=\"\"]" << std::endl;

    int f;
    Dim4PentFacet adj;
    for (p = 0; p < nPentachora_; ++p)
        for (f = 0; f < 5; ++f) {
            adj = dest(p, f);
            if (adj.isBoundary(nPentachora_) ||
                    adj.pent < static_cast<int>(p) ||
                    (adj.pent == static_cast<int>(p) && adj.facet < f))
                continue;
            out << prefix << '_' << p << " -- " << prefix << '_'
                << adj.pent << ';' << std::endl;
        }

    out << '}' << std::endl;
}

std::string Dim4FacetPairing::toTextRep() const {
    std::ostringstream ans;

    for (Dim4PentFacet f(0, 0); ! f.isPastEnd(nPentachora_, true); ++f) {
        if (f.pent || f.facet)
            ans << ' ';
        ans << dest(f).pent << ' ' << dest(f).facet;
    }

    return ans.str();
}

Dim4FacetPairing* Dim4FacetPairing::fromTextRep(const std::string& rep) {
    std::vector<std::string> tokens;
    unsigned nTokens = basicTokenise(back_inserter(tokens), rep);

    if (nTokens == 0 || nTokens % 10 != 0)
        return 0;

    long nPent = nTokens / 10;
    Dim4FacetPairing* ans = new Dim4FacetPairing(nPent);

    // Read the raw values.
    // Check the range of each value while we're at it.
    long val;
    for (long i = 0; i < nPent * 5; ++i) {
        if (! valueOf(tokens[2 * i], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val > nPent) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].pent = val;

        if (! valueOf(tokens[2 * i + 1], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val >= 5) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].facet = val;
    }

    // Run a sanity check.
    Dim4PentFacet destFacet;
    bool broken = false;
    for (Dim4PentFacet f(0, 0); ! f.isPastEnd(nPent, true); ++f) {
        destFacet = ans->dest(f);
        if (destFacet.pent == nPent && destFacet.facet != 0)
            broken = true;
        else if (destFacet.pent < nPent && ! (ans->dest(destFacet) == f))
            broken = true;
        else
            continue;
        break;
    }

    if (broken) {
        delete ans;
        return 0;
    }

    // All is well.
    return ans;
}

bool Dim4FacetPairing::findAllPairings(unsigned nPentachora_,
        NBoolSet boundary, int nBdryFacets, UseDim4FacetPairing use,
        void* useArgs, bool newThread) {
    // Create a set of arguments.
    Dim4FacetPairingArgs* args = new Dim4FacetPairingArgs();
    args->boundary = boundary;
    args->nBdryFacets = nBdryFacets;
    args->use = use;
    args->useArgs = useArgs;

    // Start the facet pairing generation.
    Dim4FacetPairing* pairing = new Dim4FacetPairing(nPentachora_);
    if (newThread)
        return pairing->start(args, true);
    else {
        pairing->run(args);
        delete pairing;
        return true;
    }
}

void* Dim4FacetPairing::run(void* param) {
    Dim4FacetPairingArgs* args = static_cast<Dim4FacetPairingArgs*>(param);

    // Bail if it's obvious that nothing will happen.
    if (args->boundary == NBoolSet::sNone || nPentachora_ == 0) {
        args->use(0, 0, args->useArgs);
        delete args;
        return 0;
    }
    if (args->boundary.hasTrue() && args->nBdryFacets >= 0 &&
            (args->nBdryFacets % 2 != static_cast<int>(nPentachora_) % 2 ||
            args->nBdryFacets > 3 * static_cast<int>(nPentachora_) + 2
            || (args->nBdryFacets == 0 && ! args->boundary.hasFalse()))) {
        args->use(0, 0, args->useArgs);
        delete args;
        return 0;
    }

    // Initialise the pairings to unspecified (i.e., facet -> itself).
    for (Dim4PentFacet f(0,0); f.pent < static_cast<int>(nPentachora_); ++f)
        dest(f) = f;

    // Note that we have at least one pentachoron.
    Dim4PentFacet trying(0, 0);
        /**< The facet we're currently trying to match. */
    int boundaryFacets = 0;
        /**< How many (deliberately) unmatched facets do we currently have? */
    int usedFacets = 0;
        /**< How many facets have we already determined matchings for? */
    Dim4FacetPairingIsoList allAutomorphisms;
        /**< The set of all automorphisms of the current facet pairing. */

    // Run through and find all possible matchings.
    Dim4PentFacet oldTrying, tmpFacet;
    while (true) {
        // TODO: Check for cancellation,

        // INVARIANT: Facet trying needs to be joined to something.
        // dest(trying) represents the last tried destination for the
        // join, and there is no reciprocal join from dest(trying) back
        // to trying.
        // The current value of dest(trying) is >= trying.

        // Move to the next destination.
        ++dest(trying);

        // If we're about to close off the current set of of pentachora
        // and it's not all the pentachora, we will have something
        // disconnected!
        // We will now avoid tying the last two facets in a set together,
        // and later we will avoid sending the last facet of a set to the
        // boundary.
        if (usedFacets % 5 == 3 &&
                usedFacets < 5 * static_cast<int>(nPentachora_) - 2 &&
                noDest((usedFacets / 5) + 1, 0) &&
                dest(trying).pent <= (usedFacets / 5)) {
            // Move to the first unused pentachoron.
            dest(trying).pent = (usedFacets / 5) + 1;
            dest(trying).facet = 0;
        }

        // We'd better make sure we're not going to glue together so
        // many facets that there is no room for the required number of
        // boundary facets.
        if (args->boundary.hasTrue()) {
            // We're interested in triangulations with boundary.
            if (args->nBdryFacets < 0) {
                // We don't care how many boundary facets.
                if (! args->boundary.hasFalse()) {
                    // We must have some boundary though.
                    if (boundaryFacets == 0 &&
                            usedFacets ==
                                5 * static_cast<int>(nPentachora_) - 2 &&
                            dest(trying).pent <
                                static_cast<int>(nPentachora_))
                        dest(trying).setBoundary(nPentachora_);
                }
            } else {
                // We're specific about the number of boundary facets.
                if (usedFacets - boundaryFacets + args->nBdryFacets ==
                        5 * static_cast<int>(nPentachora_) &&
                        dest(trying).pent < static_cast<int>(nPentachora_))
                    // We've used our entire quota of non-boundary facets.
                    dest(trying).setBoundary(nPentachora_);
            }
        }

        // dest(trying) is now the first remaining candidate destination.
        // We still don't know whether this destination is valid however.
        while(true) {
            // Move onwards to the next free destination.
            while (dest(trying).pent < static_cast<int>(nPentachora_) &&
                    ! noDest(dest(trying)))
                ++dest(trying);

            // If we are past facet 0 of a pentachoron and the previous facet
            // was not used, we can't do anything with this pentachoron.
            // Move to the next pentachoron.
            if (dest(trying).pent < static_cast<int>(nPentachora_) &&
                    dest(trying).facet > 0 &&
                    noDest(dest(trying).pent, dest(trying).facet - 1)) {
                ++dest(trying).pent;
                dest(trying).facet = 0;
                continue;
            }

            break;
        }

        // If we're still at an illegitimate destination, it must be
        // facet 0 of a pentachoron where the previous pentachoron is
        // unused.  Note that facet == 0 implies pent > 0.
        // In this case, we've passed the last sane choice; head
        // straight to the boundary.
        if (dest(trying).pent < static_cast<int>(nPentachora_) &&
                dest(trying).facet == 0 &&
                noDest(dest(trying).pent - 1, 0))
            dest(trying).setBoundary(nPentachora_);

        // Finally, return to the issue of prematurely closing off a
        // set of pentachora.  This time we will avoid sending the last
        // facet of a set of pentachora to the boundary.
        if (usedFacets % 5 == 4 &&
                usedFacets < 5 * static_cast<int>(nPentachora_) - 1 &&
                noDest((usedFacets / 5) + 1, 0) && isUnmatched(trying)) {
            // Can't use the boundary; all we can do is push past the
            // end.
            ++dest(trying);
        }

        // And so we're finally looking at the next real candidate for
        // dest(trying) that we know we're actually allowed to use.

        // Check if after all that we've been pushed past the end.
        if (dest(trying).isPastEnd(nPentachora_,
                (! args->boundary.hasTrue()) ||
                boundaryFacets == args->nBdryFacets)) {
            // We can't join trying to anything else.  Step back.
            dest(trying) = trying;
            --trying;

            // Keep heading back until we find a facet that joins
            // forwards or to the boundary.
            while (! trying.isBeforeStart()) {
                if (dest(trying) < trying)
                    --trying;
                else
                    break;
            }

            // Is the search over?
            if (trying.isBeforeStart())
                break;

            // Otherwise undo the previous gluing and prepare to loop
            // again trying the next option.
            if (isUnmatched(trying)) {
                --usedFacets;
                --boundaryFacets;
            } else {
                usedFacets -= 2;
                dest(dest(trying)) = dest(trying);
            }

            continue;
        }

        // Let's match it up and head to the next free facet!
        if (isUnmatched(trying)) {
            ++usedFacets;
            ++boundaryFacets;
        } else {
            usedFacets += 2;
            dest(dest(trying)) = trying;
        }

        // Now we increment trying to move to the next unmatched facet.
        oldTrying = trying;
        ++trying;
        while (trying.pent < static_cast<int>(nPentachora_) && ! noDest(trying))
            ++trying;

        // Have we got a solution?
        if (trying.pent == static_cast<int>(nPentachora_)) {
            // Deal with the solution!
            if (isCanonicalInternal(allAutomorphisms)) {
                args->use(this, &allAutomorphisms, args->useArgs);
                for_each(allAutomorphisms.begin(), allAutomorphisms.end(),
                    FuncDelete<Dim4Isomorphism>());
                allAutomorphisms.clear();
            }

            // Head back down to the previous gluing and undo it, ready
            // for the next loop.
            trying = oldTrying;
            if (isUnmatched(trying)) {
                --usedFacets;
                --boundaryFacets;
            } else {
                usedFacets -= 2;
                dest(dest(trying)) = dest(trying);
            }
        } else {
            // We're about to start working on a new unmatched facet.
            // Set dest(trying) to one step *before* the first feasible
            // destination.

            // Note that currently the destination is set to trying.

            // Ensure the destination is at least the
            // previous forward destination from an earlier facet of this
            // pentachoron.
            if (trying.facet > 0) {
                tmpFacet = trying;
                for (--tmpFacet; tmpFacet.pent == trying.pent; --tmpFacet)
                    if (tmpFacet < dest(tmpFacet)) {
                        // Here is the previous forward destination in
                        // this pentachoron.
                        if (dest(trying) < dest(tmpFacet)) {
                            dest(trying) = dest(tmpFacet);

                            // Remember that dest(trying) will be
                            // incremented before it is used.  This
                            // should not happen if we're already on the
                            // boundary, so we need to move back one
                            // step so we will be pushed back onto the
                            // boundary.
                            if (isUnmatched(trying))
                                --dest(trying);
                        }
                        break;
                    }
            }

            // If the first pentachoron doesn't glue to itself and this
            // is not the first pentachoron, it can't glue to itself either.
            // (Note that we already know there is at least 1 pentachoron.)
            if (dest(trying).pent == trying.pent && dest(trying).facet < 4 &&
                    trying.pent > 0)
                if (dest(0, 0).pent != 0)
                    dest(trying).facet = 4;
        }
    }

    args->use(0, 0, args->useArgs);
    delete args;
    return 0;
}

bool Dim4FacetPairing::isCanonical() const {
    // Check the preconditions for isCanonicalInternal().
    unsigned pent, facet;
    for (pent = 0; pent < nPentachora_; ++pent) {
        for (facet = 0; facet < 4; ++facet)
            if (dest(pent, facet + 1) < dest(pent, facet))
                if (! (dest(pent, facet + 1) == Dim4PentFacet(pent, facet)))
                    return false;
        if (pent > 0)
            if (dest(pent, 0).pent >= static_cast<int>(pent))
                return false;
        if (pent > 1)
            if (dest(pent, 0) <= dest(pent - 1, 0))
                return false;
    }

    // We've met all the preconditions, so we can now run
    // isCanonicalInternal().
    Dim4FacetPairingIsoList list;
    return isCanonicalInternal(list);
}

bool Dim4FacetPairing::isCanonicalInternal(
        Dim4FacetPairingIsoList& list) const {
    // Create the automorphisms one pentachoron at a time, selecting the
    // preimage of 0 first, then the preimage of 1 and so on.

    // We want to cycle through all possible first facet gluings, so we'll
    // special-case the situation in which there are no facet gluings at all.
    if (isUnmatched(0, 0)) {
        // We must have just one pentachoron with no facet gluings at all.
        Dim4Isomorphism* ans;
        for (int i = 0; i < 120 /* size of S5 */; ++i) {
            ans = new Dim4Isomorphism(1);
            ans->pentImage(0) = 0;
            ans->facetPerm(0) = NPerm5::S5[i];
            list.push_back(ans);
        }
        return true;
    }

    // Now we know that facet 0 of pentachoron 0 is glued to something.

    Dim4PentFacet* image = new Dim4PentFacet[nPentachora_ * 5];
        /**< The automorphism currently under construction. */
    Dim4PentFacet* preImage = new Dim4PentFacet[nPentachora_ * 5];
        /**< The inverse of this automorphism. */

    unsigned i;
    for (i = 0; i < nPentachora_ * 5; ++i) {
        image[i].setBeforeStart();
        preImage[i].setBeforeStart();
    }

    // Note that we know nPentachora_ >= 1.
    // For the preimage of facet 0 of pentachoron 0 we simply cycle
    // through all possibilities.
    const Dim4PentFacet firstFace(0, 0);
    const Dim4PentFacet firstFaceDest(dest(firstFace));
    Dim4PentFacet firstDestPre;
    Dim4PentFacet trying;
    Dim4PentFacet fImg, fPre;
    bool stepDown;
    int pent, facet;
    for (preImage[0] = firstFace ; ! preImage[0].isPastEnd(nPentachora_, true);
            ++preImage[0]) {
        // Note that we know firstFace is not unmatched.
        if (isUnmatched(preImage[0]))
            continue;

        // If firstFace glues to the same pentachoron and this facet
        // doesn't, we can ignore this permutation.
        firstDestPre = dest(preImage[0]);
        if (firstFaceDest.pent == 0 && firstDestPre.pent != preImage[0].pent)
            continue;

        // If firstFace doesn't glue to the same pentachoron but this
        // facet does, we're not in canonical form.
        if (firstFaceDest.pent != 0 && firstDestPre.pent == preImage[0].pent) {
            for_each(list.begin(), list.end(),
                FuncDelete<Dim4Isomorphism>());
            list.clear();
            delete[] image;
            delete[] preImage;
            return false;
        }

        // We can use this facet.  Set the corresponding reverse mapping
        // and off we go.
        image[preImage[0].pent * 5 + preImage[0].facet] = firstFace;
        preImage[firstFaceDest.pent * 5 + firstFaceDest.facet] = firstDestPre;
        image[firstDestPre.pent * 5 + firstDestPre.facet] = firstFaceDest;

        // Step forwards to the next facet whose preimage is undetermined.
        trying = firstFace;
        ++trying;
        if (trying == firstFaceDest)
            ++trying;
        while (! (trying == firstFace)) {
            // INV: We've successfully selected preimages for all facets
            // before trying.  We're currently looking at the last
            // attempted candidate for the preimage of trying.

            // Note that if preimage facet A is glued to preimage facet B
            // and the image of A is earlier than the image of B, then
            // the image of A will be selected whereas the image of B
            // will be automatically derived.

            stepDown = false;
            Dim4PentFacet& pre = preImage[trying.pent * 5 + trying.facet];

            if (trying.isPastEnd(nPentachora_, true)) {
                // We have a complete automorphism!
                Dim4Isomorphism* ans = new Dim4Isomorphism(nPentachora_);
                for (i = 0; i < nPentachora_; i++) {
                    ans->pentImage(i) = image[i * 5].pent;
                    ans->facetPerm(i) = NPerm5(image[i * 5].facet,
                        image[i * 5 + 1].facet, image[i * 5 + 2].facet,
                        image[i * 5 + 3].facet, image[i * 5 + 4].facet);
                }
                list.push_back(ans);
                stepDown = true;
            } else {
                // Move to the next candidate.
                if (pre.pent >= 0 && pre.facet == 4) {
                    // We're all out of candidates.
                    pre.setBeforeStart();
                    stepDown = true;
                } else {
                    if (pre.isBeforeStart()) {
                        // Which pentachoron must we look in?
                        // Note that this pentachoron will already have been
                        // determined.
                        pre.pent = preImage[trying.pent * 5].pent;
                        pre.facet = 0;
                    } else
                        ++pre.facet;

                    // Step forwards until we have a preimage whose image
                    // has not already been set.
                    // If the preimage is unmatched and trying isn't,
                    // we'll also skip it.
                    // If trying is unmatched and the preimage isn't,
                    // we're not in canonical form.
                    for ( ; pre.facet < 5; ++pre.facet) {
                        if (! image[pre.pent * 5 + pre.facet].isBeforeStart())
                            continue;
                        if ((! isUnmatched(trying)) && isUnmatched(pre))
                            continue;
                        if (isUnmatched(trying) && (! isUnmatched(pre))) {
                            // We're not in canonical form.
                            for_each(list.begin(), list.end(),
                                FuncDelete<Dim4Isomorphism>());
                            list.clear();
                            delete[] image;
                            delete[] preImage;
                            return false;
                        }
                        break;
                    }
                    while (pre.facet < 5 &&
                            ! image[pre.pent * 5 + pre.facet].isBeforeStart())
                        ++pre.facet;
                    if (pre.facet == 5) {
                        pre.setBeforeStart();
                        stepDown = true;
                    }
                }
            }

            if (! stepDown) {
                // We found a candidate.
                // We also know that trying is unmatched iff the preimage
                // is unmatched.
                image[pre.pent * 5 + pre.facet] = trying;
                if (! isUnmatched(pre)) {
                    fPre = dest(pre);
                    if (image[fPre.pent * 5 + fPre.facet].isBeforeStart()) {
                        // The image of fPre (the partner of the preimage
                        // facet) can be determined at this point.
                        // Specifically, it should go into the next
                        // available slot.

                        // Do we already know which pentachoron we should
                        // be looking into?
                        for (i = 0; i < 5; i++)
                            if (! image[fPre.pent * 5 + i].isBeforeStart()) {
                                // Here's the pentachoron!
                                // Find the first available facet.
                                pent = image[fPre.pent * 5 + i].pent;
                                for (facet = 0; ! preImage[pent * 5 + facet].
                                        isBeforeStart(); ++facet)
                                    ;
                                image[fPre.pent * 5 + fPre.facet].pent = pent;
                                image[fPre.pent * 5 + fPre.facet].facet = facet;
                                break;
                            }
                        if (i == 5) {
                            // We need to map to a new pentachoron.
                            // Find the first available pentachoron.
                            for (pent = trying.pent + 1;
                                    ! preImage[pent * 5].isBeforeStart();
                                    ++pent)
                                ;
                            image[fPre.pent * 5 + fPre.facet].pent = pent;
                            image[fPre.pent * 5 + fPre.facet].facet = 0;
                        }

                        // Set the corresponding preimage.
                        fImg = image[fPre.pent * 5 + fPre.facet];
                        preImage[fImg.pent * 5 + fImg.facet] = fPre;
                    }
                }

                // Do a lexicographical comparison and shunt trying up
                // if need be.
                do {
                    fImg = dest(trying);
                    fPre = dest(preImage[trying.pent * 5 + trying.facet]);
                    if (! fPre.isBoundary(nPentachora_))
                        fPre = image[fPre.pent * 5 + fPre.facet];

                    // Currently trying is glued to fImg.
                    // After applying our isomorphism, trying will be
                    // glued to fPre.

                    if (fImg < fPre) {
                        // This isomorphism will lead to a
                        // lexicographically greater representation.
                        // Ignore it.
                        stepDown = true;
                    } else if (fPre < fImg) {
                        // Whapow, we're not in canonical form.
                        for_each(list.begin(), list.end(),
                            FuncDelete<Dim4Isomorphism>());
                        list.clear();
                        delete[] image;
                        delete[] preImage;
                        return false;
                    }

                    // What we have so far is consistent with an automorphism.
                    ++trying;
                } while (! (stepDown || trying.isPastEnd(nPentachora_, true) ||
                        preImage[trying.pent * 5 + trying.facet].isBeforeStart()));
            }

            if (stepDown) {
                // We're shunting trying back down.
                --trying;
                while (true) {
                    fPre = preImage[trying.pent * 5 + trying.facet];
                    if (! isUnmatched(fPre)) {
                        fPre = dest(fPre);
                        if (image[fPre.pent * 5 + fPre.facet] < trying) {
                            // This preimage/image was automatically
                            // derived.
                            --trying;
                            continue;
                        }
                    }
                    break;
                }

                // Note that this resetting of facets that follows will
                // also take place when trying makes it all the way back
                // down to firstFace.
                fPre = preImage[trying.pent * 5 + trying.facet];
                image[fPre.pent * 5 + fPre.facet].setBeforeStart();
                if (! isUnmatched(fPre)) {
                    fPre = dest(fPre);
                    fImg = image[fPre.pent * 5 + fPre.facet];
                    preImage[fImg.pent * 5 + fImg.facet].setBeforeStart();
                    image[fPre.pent * 5 + fPre.facet].setBeforeStart();
                }
            }
        }
    }

    // The pairing is in canonical form and we have all our automorphisms.
    // Tidy up and return.
    delete[] image;
    delete[] preImage;
    return true;
}

} // namespace regina

