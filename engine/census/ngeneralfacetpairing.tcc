
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/* Template definitions for ngeneralfacetpairing.h. */

#include <algorithm>
#include <sstream>
#include <vector>
#include "census/ngeneralfacetpairing.h"
#include "utilities/memutils.h"
#include "utilities/stringutils.h"

namespace regina {

template <int dim>
NGeneralFacetPairing<dim>::NGeneralFacetPairing(
        const NGeneralFacetPairing<dim>& cloneMe) :
        /* TODO NThread(), */
        size_(cloneMe.size_),
        pairs_(new NFacetSpec<dim>[cloneMe.size_ * (dim + 1)]) {
    std::copy(cloneMe.pairs_, cloneMe.pairs_ + (size_ * (dim + 1)), pairs_);
}

template <int dim>
bool NGeneralFacetPairing<dim>::isClosed() const {
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f)
        if (isUnmatched(f))
            return false;
    return true;
}

template <int dim>
std::string NGeneralFacetPairing<dim>::toString() const {
    std::ostringstream ans;

    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.facet == 0 && f.simp > 0)
            ans << " | ";
        else if (f.simp || f.facet)
            ans << ' ';

        if (dest(f).isBoundary(size_))
            ans << "bdry";
        else
            ans << dest(f).simp << ':' << dest(f).facet;
    }
    return ans.str();
}

template <int dim>
std::string NGeneralFacetPairing<dim>::dotHeader(const char* graphName) {
    std::ostringstream ans;
    writeDotHeader(ans, graphName);
    return ans.str();
}

template <int dim>
void NGeneralFacetPairing<dim>::writeDotHeader(
        std::ostream& out, const char* graphName) {
    static const char defaultGraphName[] = "G";

    if ((! graphName) || (! *graphName))
        graphName = defaultGraphName;

    out << "graph " << graphName << " {" << std::endl;
    out << "graph [bgcolor=white];" << std::endl;
    out << "edge [color=black];" << std::endl;
    out << "node [shape=circle,style=filled,height=0.15,fixedsize=true,label=\"\",fontsize=9,fontcolor=\"#751010\"];" << std::endl;
}

template <int dim>
std::string NGeneralFacetPairing<dim>::dot(
        const char* prefix, bool subgraph, bool labels) const {
    std::ostringstream ans;
    writeDot(ans, prefix, subgraph, labels);
    return ans.str();
}

template <int dim>
void NGeneralFacetPairing<dim>::writeDot(std::ostream& out,
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
    unsigned p;
    for (p = 0; p < size_; ++p) {
        out << prefix << '_' << p << " [label=\"";
        if (labels)
            out << p;
        out << "\"]" << std::endl;
    }

    int f;
    NFacetSpec<dim> adj;
    for (p = 0; p < size_; ++p)
        for (f = 0; f < (dim + 1); ++f) {
            adj = dest(p, f);
            if (adj.isBoundary(size_) ||
                    adj.simp < static_cast<int>(p) ||
                    (adj.simp == static_cast<int>(p) && adj.facet < f))
                continue;
            out << prefix << '_' << p << " -- " << prefix << '_'
                << adj.simp << ';' << std::endl;
        }

    out << '}' << std::endl;
}

template <int dim>
std::string NGeneralFacetPairing<dim>::toTextRep() const {
    std::ostringstream ans;

    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(size_, true); ++f) {
        if (f.simp || f.facet)
            ans << ' ';
        ans << dest(f).simp << ' ' << dest(f).facet;
    }

    return ans.str();
}

/*
template <int dim>
NGeneralFacetPairing* NGeneralFacetPairing::fromTextRep(const std::string& rep) {
    std::vector<std::string> tokens;
    unsigned nTokens = basicTokenise(back_inserter(tokens), rep);

    if (nTokens == 0 || nTokens % 10 != 0)
        return 0;

    long nPent = nTokens / 10;
    NGeneralFacetPairing* ans = new NGeneralFacetPairing(nPent);

    // Read the raw values.
    // Check the range of each value while we're at it.
    long val;
    for (long i = 0; i < nPent * (dim + 1); ++i) {
        if (! valueOf(tokens[2 * i], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val > nPent) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].simp = val;

        if (! valueOf(tokens[2 * i + 1], val)) {
            delete ans;
            return 0;
        }
        if (val < 0 || val >= (dim + 1)) {
            delete ans;
            return 0;
        }
        ans->pairs_[i].facet = val;
    }

    // Run a sanity check.
    NFacetSpec<dim> destFacet;
    bool broken = false;
    for (NFacetSpec<dim> f(0, 0); ! f.isPastEnd(nPent, true); ++f) {
        destFacet = ans->dest(f);
        if (destFacet.simp == nPent && destFacet.facet != 0)
            broken = true;
        else if (destFacet.simp < nPent && ! (ans->dest(destFacet) == f))
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
*/

} // namespace regina

