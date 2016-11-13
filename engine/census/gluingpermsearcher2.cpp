
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
#include "census/gluingpermsearcher2.h"
#include "triangulation/dim2.h"
#include "utilities/memutils.h"

namespace regina {

const char GluingPermSearcher<2>::dataTag_ = 'g';

GluingPermSearcher<2>::GluingPermSearcher(
        const FacetPairing<2>* pairing, const FacetPairing<2>::IsoList* autos,
        bool orientableOnly, GluingPermSearcher<2>::Use use, void* useArgs) :
        GluingPerms<2>(pairing), autos_(autos), autosNew(autos == 0),
        orientableOnly_(orientableOnly), use_(use), useArgs_(useArgs),
        started(false),
        orientation(new int[pairing->size()]) {
    // Generate the list of edge pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autosNew) {
        const_cast<GluingPermSearcher<2>*>(this)->autos_ =
            new FacetPairing<2>::IsoList();
        pairing->findAutomorphisms(
            const_cast<FacetPairing<2>::IsoList&>(*autos_));
    }

    // Initialise arrays.
    unsigned nTris = size();

    std::fill(orientation, orientation + nTris, 0);
    std::fill(permIndices_, permIndices_ + nTris* 3, -1);

    // Just fill the order[] array in a default left-to-right fashion.
    // Subclasses can rearrange things if they choose.
    order = new FacetSpec<2>[(nTris * 3) / 2];
    orderElt = orderSize = 0;

    FacetSpec<2> edge, adj;
    for (edge.setFirst(); ! edge.isPastEnd(nTris, true); edge++)
        if (! pairing->isUnmatched(edge))
            if (edge < pairing->dest(edge))
                order[orderSize++] = edge;
}

GluingPermSearcher<2>::~GluingPermSearcher() {
    delete[] orientation;
    delete[] order;
    if (autosNew) {
        // We made them, so we'd better remove the const again and
        // delete them.
        FacetPairing<2>::IsoList* autos =
            const_cast<FacetPairing<2>::IsoList*>(autos_);
        std::for_each(autos->begin(), autos->end(),
            FuncDelete<Isomorphism<2>>());
        delete autos;
    }
}

GluingPermSearcher<2>* GluingPermSearcher<2>::bestSearcher(
        const FacetPairing<2>* pairing, const FacetPairing<2>::IsoList* autos,
        bool orientableOnly, GluingPermSearcher<2>::Use use, void* useArgs) {
    // We only have one algorithm for now.
    return new GluingPermSearcher<2>(pairing, autos, orientableOnly,
        use, useArgs);
}

void GluingPermSearcher<2>::findAllPerms(const FacetPairing<2>* pairing,
        const FacetPairing<2>::IsoList* autos, bool orientableOnly,
        GluingPermSearcher<2>::Use use, void* useArgs) {
    GluingPermSearcher<2>* searcher = bestSearcher(pairing, autos,
        orientableOnly, use, useArgs);
    searcher->runSearch();
    delete searcher;
}

void GluingPermSearcher<2>::runSearch(long maxDepth) {
    // In this generation algorithm, each orientation is simply +/-1.

    unsigned nTriangles = size();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTriangles * 3 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 || pairing_->dest(0, 0).isBoundary(nTriangles)) {
            use_(this, useArgs_);
            use_(0, useArgs_);
            return;
        }

        orderElt = 0;
        orientation[0] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt == orderSize) {
        if (isCanonical())
            use_(this, useArgs_);
        use_(0, useArgs_);
        return;
    }

    // ---------- Selecting the individual gluing permutations ----------

    int minOrder = orderElt;
    int maxOrder = orderElt + maxDepth;

    FacetSpec<2> edge, adj;

    while (orderElt >= minOrder) {
        edge = order[orderElt];
        adj = (*pairing_)[edge];

        // TODO: Check for cancellation.

        // Move to the next permutation.

        // Be sure to preserve the orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            permIndex(edge)++;
        else
            permIndex(edge) += 2;

        // Are we out of ideas for this edge?
        if (permIndex(edge) >= 2) {
            // Yep.  Head back down to the previous edge.
            permIndex(edge) = -1;
            permIndex(adj) = -1;
            orderElt--;
            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = permIndex(edge); // S2 elements are their own inverses.

        // Fix the orientation if appropriate.
        if (adj.facet == 0 && orientableOnly_) {
            // It's the first time we've hit this triangle.
            if ((permIndex(edge) + (edge.facet == 2 ? 0 : 1) +
                    (adj.facet == 2 ? 0 : 1)) % 2 == 0)
                orientation[adj.simp] = -orientation[edge.simp];
            else
                orientation[adj.simp] = orientation[edge.simp];
        }

        // Move on to the next edge.
        orderElt++;

        // If we're at the end, try the solution and step back.
        if (orderElt == orderSize) {
            // We in fact have an entire triangulation.
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            if (isCanonical())
                use_(this, useArgs_);

            // Back to the previous face.
            orderElt--;
        } else {
            // Not a full triangulation; just one level deeper.

            // We've moved onto a new edge.
            // Be sure to get the orientation right.
            edge = order[orderElt];
            if (orientableOnly_ && pairing_->dest(edge).facet > 0) {
                // permIndex(edge) will be set to -1 or -2 as appropriate.
                adj = (*pairing_)[edge];
                if (orientation[edge.simp] == orientation[adj.simp])
                    permIndex(edge) = 1;
                else
                    permIndex(edge) = 0;

                if ((edge.facet == 2 ? 0 : 1) + (adj.facet == 2 ? 0 : 1) == 1)
                    permIndex(edge) = (permIndex(edge) + 1) % 2;

                permIndex(edge) -= 2;
            }

            if (orderElt == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                use_(this, useArgs_);

                // Back to the previous edge.
                permIndex(edge) = -1;
                orderElt--;
            }
        }
    }

    // And the search is over.
    use_(0, useArgs_);
}

void GluingPermSearcher<2>::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
}

GluingPermSearcher<2>* GluingPermSearcher<2>::readTaggedData(std::istream& in,
        GluingPermSearcher<2>::Use use, void* useArgs) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        return 0;

    GluingPermSearcher<2>* ans;
    if (c == GluingPermSearcher<2>::dataTag_)
        ans = new GluingPermSearcher<2>(in, use, useArgs);
    else
        return 0;

    if (ans->inputError()) {
        delete ans;
        return 0;
    }

    return ans;
}

void GluingPermSearcher<2>::dumpData(std::ostream& out) const {
    GluingPerms<2>::dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (started ? 's' : '.');
    out << std::endl;

    int nTris = size();
    int i;

    for (i = 0; i < nTris; i++) {
        if (i)
            out << ' ';
        out << orientation[i];
    }
    out << std::endl;

    out << orderElt << ' ' << orderSize << std::endl;
    for (i = 0; i < orderSize; i++) {
        if (i)
            out << ' ';
        out << order[i].simp << ' ' << order[i].facet;
    }
    out << std::endl;
}

GluingPermSearcher<2>::GluingPermSearcher(std::istream& in,
        GluingPermSearcher<2>::Use use, void* useArgs) :
        GluingPerms<2>(in), autos_(0), autosNew(false),
        use_(use), useArgs_(useArgs), orientation(0),
        order(0), orderSize(0), orderElt(0) {
    if (inputError_)
        return;

    // Recontruct the face pairing automorphisms.
    const_cast<GluingPermSearcher<2>*>(this)->autos_ =
        new FacetPairing<2>::IsoList();
    pairing_->findAutomorphisms(const_cast<FacetPairing<2>::IsoList&>(*autos_));
    autosNew = true;

    // Keep reading.
    char c;

    in >> c;
    if (c == 'o')
        orientableOnly_ = true;
    else if (c == '.')
        orientableOnly_ = false;
    else {
        inputError_ = true; return;
    }

    in >> c;
    if (c == 's')
        started = true;
    else if (c == '.')
        started = false;
    else {
        inputError_ = true; return;
    }

    int nTris = pairing_->size();
    int t;

    orientation = new int[nTris];
    for (t = 0; t < nTris; t++)
        in >> orientation[t];

    order = new FacetSpec<2>[(nTris * 3) / 2];
    in >> orderElt >> orderSize;
    for (t = 0; t < orderSize; t++) {
        in >> order[t].simp >> order[t].facet;
        if (order[t].simp >= nTris || order[t].simp < 0 ||
                order[t].facet >= 3 || order[t].facet < 0) {
            inputError_ = true; return;
        }
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

bool GluingPermSearcher<2>::isCanonical() const {
    FacetSpec<2> edge, edgeDest, edgeImage;
    int ordering;

    for (FacetPairing<2>::IsoList::const_iterator it = autos_->begin();
            it != autos_->end(); it++) {
        // Compare the current set of gluing permutations with its
        // preimage under each edge pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (edge.setFirst(); edge.simp <
                static_cast<int>(pairing_->size()); edge++) {
            edgeDest = pairing_->dest(edge);
            if (pairing_->isUnmatched(edge) || edgeDest < edge)
                continue;

            edgeImage = (**it)[edge];
            ordering = gluingPerm(edge).compareWith(
                (*it)->edgePerm(edgeDest.simp).inverse() * gluingPerm(edgeImage)
                * (*it)->edgePerm(edge.simp));
            if (ordering < 0) {
                // This permutation set is closer.
                break;
            } else if (ordering > 0) {
                // The transformed permutation set is closer.
                return false;
            }

            // So far it's an automorphism of gluing permutations also.
            // Keep running through edges.
        }
        // Nothing broke with this automorphism.  On to the next one.
    }

    // Nothing broke at all.
    return true;
}

} // namespace regina

