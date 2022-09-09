
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file triangulation/detail/facetpairing-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  FacetPairing class template.
 *
 *  This file is automatically included from facetpairing.h; there is
 *  no need for end users to include it explicitly.
 */

#ifndef __REGINA_FACETPAIRING_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_FACETPAIRING_IMPL_H
#endif

#include <algorithm>
#include <sstream>
#include <thread>
#include <vector>
#include "triangulation/generic/facetpairing.h"
#include "maths/perm.h"
#include "utilities/exception.h"
#include "utilities/stringutils.h"
#include "utilities/typeutils.h"

namespace regina::detail {

template <int dim>
FacetPairingBase<dim>::FacetPairingBase(const Triangulation<dim>& tri) :
        size_(tri.size()),
        pairs_(new FacetSpec<dim>[tri.size() * (dim + 1)]) {
    size_t p, index;
    unsigned f;
    const Simplex<dim> *simp, *adj;
    for (index = 0, p = 0; p < size_; ++p) {
        simp = tri.simplex(p);
        for (f = 0; f <= dim; ++f) {
            adj = simp->adjacentSimplex(f);
            if (adj) {
                pairs_[index].simp = adj->index();
                pairs_[index].facet = simp->adjacentFacet(f);
            } else
                pairs_[index].setBoundary(size_);

            ++index;
        }
    }
}

template <int dim>
FacetPairingBase<dim>::FacetPairingBase(std::istream& in) :
        size_(0), pairs_(nullptr) {
    // Skip initial whitespace to find the facet pairing.
    std::string line;
    while (true) {
        std::getline(in, line);
        if (in.eof()) {
            throw InvalidInput("Unexpected end of input stream "
                "while attempting to read a FacetPairing");
        }
        line = regina::stripWhitespace(line);
        if (line.length() > 0)
            break;
    }

    try {
        *this = fromTextRep(line);
    } catch (const InvalidArgument& exc) {
        throw InvalidInput(
            "Incorrect formatted FacetPairing text representation");
    }
}

template <int dim>
bool FacetPairingBase<dim>::isClosed() const {
    for (FacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f)
        if (isUnmatched(f))
            return false;
    return true;
}

template <int dim>
bool FacetPairingBase<dim>::operator == (const FacetPairing<dim>& other) const {
    return size_ == other.size_ &&
        std::equal(pairs_, pairs_ + (size_ * (dim + 1)), other.pairs_);
}

template <int dim>
bool FacetPairingBase<dim>::operator != (const FacetPairing<dim>& other) const {
    return size_ != other.size_ ||
        ! std::equal(pairs_, pairs_ + (size_ * (dim + 1)), other.pairs_);
}

template <int dim>
bool FacetPairingBase<dim>::isConnected() const {
    if (size_ <= 1)
        return true;

    bool* seen = new bool[size_];
    auto* stack = new size_t[size_];

    seen[0] = true;
    std::fill(seen + 1, seen + size_, false);
    stack[0] = 0;
    size_t stackSize = 1;
    size_t nSeen = 1;

    while (stackSize > 0) {
        size_t top = stack[--stackSize];
        for (int i = 0; i <= dim; ++i) {
            ssize_t adj = dest(top, i).simp;
            if (adj >= 0 && adj < static_cast<ssize_t>(size_) && ! seen[adj]) {
                seen[adj] = true;
                stack[stackSize++] = adj;
                ++nSeen;
                if (nSeen == size_) {
                    delete[] stack;
                    delete[] seen;
                    return true;
                }
            }
        }
    }

    delete[] stack;
    delete[] seen;
    return false;
}

template <int dim>
void FacetPairingBase<dim>::writeTextShort(std::ostream& out) const {
    for (FacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.facet == 0 && f.simp > 0)
            out << " | ";
        else if (f.simp || f.facet)
            out << ' ';

        if (dest(f).isBoundary(size_))
            out << "bdry";
        else
            out << dest(f).simp << ':' << dest(f).facet;
    }
}

template <int dim>
std::string FacetPairingBase<dim>::dotHeader(const char* graphName) {
    std::ostringstream ans;
    writeDotHeader(ans, graphName);
    return ans.str();
}

template <int dim>
void FacetPairingBase<dim>::writeDotHeader(
        std::ostream& out, const char* graphName) {
    static const char defaultGraphName[] = "G";

    if ((! graphName) || (! *graphName))
        graphName = defaultGraphName;

    out << "graph " << graphName << " {" << std::endl;
    out << "edge [color=black];" << std::endl;
    out << R"(node [shape=circle,style=filled,height=0.15,fixedsize=true,label="",fontsize=9,fontcolor="#751010"];)" << std::endl;
}

template <int dim>
std::string FacetPairingBase<dim>::dot(
        const char* prefix, bool subgraph, bool labels) const {
    std::ostringstream ans;
    writeDot(ans, prefix, subgraph, labels);
    return ans.str();
}

template <int dim>
void FacetPairingBase<dim>::writeDot(std::ostream& out,
        const char* prefix, bool subgraph, bool labels) const {
    static const char defaultPrefix[] = "g";

    if ((! prefix) || (! *prefix))
        prefix = defaultPrefix;

    // We are guaranteed that prefix is a non-empty string.

    if (subgraph)
        out << "subgraph pairing_" << prefix << " {" << std::endl;
    else
        writeDotHeader(out, (prefix + std::string("_graph")).c_str());

    // Ancient versions of graphviz seem to ignore the default label="".
    // Make this explicit for each node.
    for (size_t p = 0; p < size_; ++p) {
        out << prefix << '_' << p << " [label=\"";
        if (labels)
            out << p;
        out << "\"]" << std::endl;
    }

    FacetSpec<dim> adj;
    for (ssize_t p = 0; p < static_cast<ssize_t>(size_); ++p)
        for (int f = 0; f < (dim + 1); ++f) {
            adj = dest(p, f);
            if (adj.isBoundary(size_) || adj.simp < p ||
                    (adj.simp == p && adj.facet < f))
                continue;
            out << prefix << '_' << p << " -- " << prefix << '_'
                << adj.simp << ';' << std::endl;
        }

    out << '}' << std::endl;
}

template <int dim>
std::string FacetPairingBase<dim>::textRep() const {
    std::ostringstream ans;

    for (FacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.simp || f.facet)
            ans << ' ';
        ans << dest(f).simp << ' ' << dest(f).facet;
    }

    return ans.str();
}

template <int dim>
FacetPairing<dim> FacetPairingBase<dim>::fromTextRep(const std::string& rep) {
    std::vector<std::string> tokens = basicTokenise(rep);

    if (tokens.empty() || tokens.size() % (2 * (dim + 1)) != 0)
        throw InvalidArgument("fromTextRep(): invalid number of tokens");

    // We use ssize_t, not size_t, to avoid signed/unsigned comparisons below.
    ssize_t nSimp = tokens.size() / (2 * (dim + 1));
    FacetPairing<dim> ans(nSimp);

    // Read the raw values.
    // Check the range of each value while we're at it.
    long val;
    for (ssize_t i = 0; i < nSimp * (dim + 1); ++i) {
        if (! valueOf(tokens[2 * i], val))
            throw InvalidArgument(
                "fromTextRep(): contains non-integer simplex");
        if (val < 0 || val > nSimp)
            throw InvalidArgument("fromTextRep(): simplex out of range");
        ans.pairs_[i].simp = val;

        if (! valueOf(tokens[2 * i + 1], val))
            throw InvalidArgument("fromTextRep(): contains non-integer facet");
        if (val < 0 || val > dim)
            throw InvalidArgument("fromTextRep(): facet out of range");
        ans.pairs_[i].facet = static_cast<int>(val);
    }

    // Run a sanity check.
    // Note: all destination simplices are known to be in the range [0..nSimp],
    // and all destination facets are known to be in the range [0..dim].
    FacetSpec<dim> destFacet;
    bool broken = false;
    for (FacetSpec<dim> f(0, 0); ! f.isPastEnd(nSimp, true); ++f) {
        destFacet = ans.dest(f);
        if (destFacet.simp == nSimp && destFacet.facet != 0)
            broken = true;
        else if (destFacet.simp < nSimp && ! (ans.dest(destFacet) == f))
            broken = true;
        else
            continue;
        break;
    }

    if (broken)
        throw InvalidArgument("fromTextRep(): mismatched facet pairings");

    // All is well.
    return ans;
}

template <int dim>
std::optional<Cut> FacetPairingBase<dim>::divideConnected(size_t minSide)
        const {
    std::optional<Cut> ans;
    size_t bestWeight = 0 /* Unnecessary, but silences warnings */;

    for (size_t left = size() / 2; left >= minSide; --left) {
        Cut c(left, size() - left);
        while (true) {
            size_t w = c.weight(static_cast<const FacetPairing<dim>&>(*this));
            if ((! ans) || w < bestWeight) {
                auto pieces = c(static_cast<const FacetPairing<dim>&>(*this));
                if (pieces.first.isConnected() && pieces.second.isConnected()) {
                    ans = c;
                    bestWeight = w;
                }
            }
            if (! c.incFixedSizes())
                break;
        }
    }

    return ans;
}

template <int dim>
template <typename Action, typename... Args>
void FacetPairingBase<dim>::enumerateInternal(BoolSet boundary,
        int nBdryFacets, Action&& action, Args&&... args) {
    // Bail if it's obvious that nothing will happen.
    if (boundary == BoolSet() || size_ == 0) {
        return;
    }
    if (boundary.hasTrue() && nBdryFacets >= 0 &&
            (nBdryFacets % 2 != ((dim+1) * static_cast<int>(size_)) % 2 ||
            nBdryFacets > (dim - 1) * static_cast<int>(size_) + 2
            || (nBdryFacets == 0 && ! boundary.hasFalse()))) {
        return;
    }

    // Initialise the pairings to unspecified (i.e., facet -> itself).
    for (FacetSpec<dim> f(0,0); f.simp < static_cast<ssize_t>(size_); ++f)
        dest(f) = f;

    // Note that we have at least one simplex.
    FacetSpec<dim> trying(0, 0);
        /**< The facet we're currently trying to match. */
    int boundaryFacets = 0;
        /**< How many (deliberately) unmatched facets do we currently have? */
    size_t usedFacets = 0;
        /**< How many facets have we already determined matchings for? */

    // Run through and find all possible matchings.
    FacetSpec<dim> oldTrying, tmpFacet;
    while (true) {
        // TODO: Check for cancellation,

        // INVARIANT: Facet trying needs to be joined to something.
        // dest(trying) represents the last tried destination for the
        // join, and there is no reciprocal join from dest(trying) back
        // to trying.
        // The current value of dest(trying) is >= trying.

        // Move to the next destination.
        ++dest(trying);

        // If we're about to close off the current set of simplices
        // and it's not all the simplices, we will have something
        // disconnected!
        // We will now avoid tying the last two facets in a set together,
        // and later we will avoid sending the last facet of a set to the
        // boundary.
        if (usedFacets % (dim + 1) == (dim - 1) &&
                usedFacets < (dim + 1) * size_ - 2 &&
                noDest((usedFacets / (dim + 1)) + 1, 0) &&
                static_cast<size_t>(dest(trying).simp) <=
                    (usedFacets / (dim + 1))) {
            // Move to the first unused simplex.
            dest(trying).simp = (usedFacets / (dim + 1)) + 1;
            dest(trying).facet = 0;
        }

        // We'd better make sure we're not going to glue together so
        // many facets that there is no room for the required number of
        // boundary facets.
        if (boundary.hasTrue()) {
            // We're interested in triangulations with boundary.
            if (nBdryFacets < 0) {
                // We don't care how many boundary facets.
                if (! boundary.hasFalse()) {
                    // We must have some boundary though.
                    if (boundaryFacets == 0 &&
                            usedFacets == (dim + 1) * size_ - 2 &&
                            dest(trying).simp < static_cast<ssize_t>(size_))
                        dest(trying).setBoundary(size_);
                }
            } else {
                // We're specific about the number of boundary facets.
                if (usedFacets - boundaryFacets + nBdryFacets ==
                        (dim + 1) * size_ &&
                        dest(trying).simp < static_cast<ssize_t>(size_))
                    // We've used our entire quota of non-boundary facets.
                    dest(trying).setBoundary(size_);
            }
        }

        // dest(trying) is now the first remaining candidate destination.
        // We still don't know whether this destination is valid however.
        while(true) {
            // Move onwards to the next free destination.
            while (dest(trying).simp < static_cast<ssize_t>(size_) &&
                    ! noDest(dest(trying)))
                ++dest(trying);

            // If we are past facet 0 of a simplex and the previous facet
            // was not used, we can't do anything with this simplex.
            // Move to the next simplex.
            if (dest(trying).simp < static_cast<ssize_t>(size_) &&
                    dest(trying).facet > 0 &&
                    noDest(dest(trying).simp, dest(trying).facet - 1)) {
                ++dest(trying).simp;
                dest(trying).facet = 0;
                continue;
            }

            break;
        }

        // If we're still at an illegitimate destination, it must be
        // facet 0 of a simplex where the previous simplex is
        // unused.  Note that facet == 0 implies simp > 0.
        // In this case, we've passed the last sane choice; head
        // straight to the boundary.
        if (dest(trying).simp < static_cast<ssize_t>(size_) &&
                dest(trying).facet == 0 && noDest(dest(trying).simp - 1, 0))
            dest(trying).setBoundary(size_);

        // Finally, return to the issue of prematurely closing off a
        // set of simplices.  This time we will avoid sending the last
        // facet of a set of simplices to the boundary.
        if (usedFacets % (dim + 1) == dim &&
                usedFacets < (dim + 1) * size_ - 1 &&
                noDest((usedFacets / (dim + 1)) + 1, 0) &&
                isUnmatched(trying)) {
            // Can't use the boundary; all we can do is push past the
            // end.
            ++dest(trying);
        }

        // And so we're finally looking at the next real candidate for
        // dest(trying) that we know we're actually allowed to use.

        // Check if after all that we've been pushed past the end.
        if (dest(trying).isPastEnd(size_, (! boundary.hasTrue()) ||
                boundaryFacets == nBdryFacets)) {
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
        while (trying.simp < static_cast<ssize_t>(size_) && ! noDest(trying))
            ++trying;

        // Have we got a solution?
        if (trying.simp == static_cast<ssize_t>(size_)) {
            // Deal with the solution!
            if constexpr (std::is_same_v<
                    typename std::remove_cv_t<typename std::remove_reference_t<
                        typename CallableArg<Action, 1>::type>>,
                    IsoList>) {
                IsoList allAutomorphisms;
                if (isCanonicalInternal(std::addressof(allAutomorphisms))) {
                    action(static_cast<FacetPairing<dim>&>(*this),
                        std::move(allAutomorphisms),
                        std::forward<Args>(args)...);
                }
            } else {
                if (isCanonicalInternal()) {
                    action(static_cast<FacetPairing<dim>&>(*this),
                        std::forward<Args>(args)...);
                }
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
            // simplex.
            if (trying.facet > 0) {
                tmpFacet = trying;
                for (--tmpFacet; tmpFacet.simp == trying.simp; --tmpFacet)
                    if (tmpFacet < dest(tmpFacet)) {
                        // Here is the previous forward destination in
                        // this simplex.
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

            // If the first simplex doesn't glue to itself and this
            // is not the first simplex, it can't glue to itself either.
            // (Note that we already know there is at least 1 simplex.)
            if (dest(trying).simp == trying.simp && dest(trying).facet < dim &&
                    trying.simp > 0)
                if (dest(0, 0).simp != 0)
                    dest(trying).facet = dim;
        }
    }

    return;
}

} // namespace regina::detail

#endif
