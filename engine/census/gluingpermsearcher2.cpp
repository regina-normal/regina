
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

namespace regina {

GluingPermSearcher<2>::GluingPermSearcher(
        FacetPairing<2>&& pairing, const FacetPairing<2>::IsoList* autos,
        bool orientableOnly, ActionWrapper&& action) :
        GluingPerms<2>(std::move(pairing)),
        // pairing is no longer usable, must use pairing_ instead
        autos_(autos), autosNew(autos == 0),
        orientableOnly_(orientableOnly), action_(std::move(action)),
        started(false), orientation(new int[pairing_.size()]) {
    // Generate the list of edge pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autosNew) {
        const_cast<GluingPermSearcher<2>*>(this)->autos_ =
            new FacetPairing<2>::IsoList(pairing_.findAutomorphisms());
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
        if (! pairing_.isUnmatched(edge))
            if (edge < pairing_.dest(edge))
                order[orderSize++] = edge;
}

GluingPermSearcher<2>::~GluingPermSearcher() {
    delete[] orientation;
    delete[] order;
    if (autosNew) {
        // We made them, so we'd better remove the const again and
        // delete them.
        delete const_cast<FacetPairing<2>::IsoList*>(autos_);
    }
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
        if (maxDepth == 0 || pairing_.dest(0, 0).isBoundary(nTriangles)) {
            action_(*this);
            return;
        }

        orderElt = 0;
        orientation[0] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt == orderSize) {
        if (isCanonical())
            action_(*this);
        return;
    }

    // ---------- Selecting the individual gluing permutations ----------

    int minOrder = orderElt;
    int maxOrder = orderElt + maxDepth;

    FacetSpec<2> edge, adj;

    while (orderElt >= minOrder) {
        edge = order[orderElt];
        adj = pairing_[edge];

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
                action_(*this);

            // Back to the previous face.
            orderElt--;
        } else {
            // Not a full triangulation; just one level deeper.

            // We've moved onto a new edge.
            // Be sure to get the orientation right.
            edge = order[orderElt];
            if (orientableOnly_ && pairing_.dest(edge).facet > 0) {
                // permIndex(edge) will be set to -1 or -2 as appropriate.
                adj = pairing_[edge];
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
                action_(*this);

                // Back to the previous edge.
                permIndex(edge) = -1;
                orderElt--;
            }
        }
    }

    // And the search is over.
}

void GluingPermSearcher<2>::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
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
        ActionWrapper&& action) :
        GluingPerms<2>(in), autos_(0), autosNew(false),
        action_(std::move(action)), orientation(0), order(0), orderSize(0),
        orderElt(0) {
    // Recontruct the face pairing automorphisms.
    const_cast<GluingPermSearcher<2>*>(this)->autos_ =
        new FacetPairing<2>::IsoList(pairing_.findAutomorphisms());
    autosNew = true;

    // Keep reading.
    char c;

    in >> c;
    if (c == 'o')
        orientableOnly_ = true;
    else if (c == '.')
        orientableOnly_ = false;
    else
        throw InvalidInput("Invalid orientability tag "
            "while attempting to read GluingPermSearcher<2>");

    in >> c;
    if (c == 's')
        started = true;
    else if (c == '.')
        started = false;
    else
        throw InvalidInput("Invalid started tag "
            "while attempting to read GluingPermSearcher<2>");

    int nTris = pairing_.size();
    int t;

    orientation = new int[nTris];
    for (t = 0; t < nTris; t++)
        in >> orientation[t];

    order = new FacetSpec<2>[(nTris * 3) / 2];
    in >> orderElt >> orderSize;
    for (t = 0; t < orderSize; t++) {
        in >> order[t].simp >> order[t].facet;
        if (order[t].simp >= nTris || order[t].simp < 0 ||
                order[t].facet >= 3 || order[t].facet < 0)
            throw InvalidInput("Edge gluing out of range "
                "while attempting to read GluingPermSearcher<2>");
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        throw InvalidInput("Unexpected end of input stream "
            "while attempting to read GluingPermSearcher<2>");
}

bool GluingPermSearcher<2>::isCanonical() const {
    FacetSpec<2> edge, edgeDest, edgeImage;
    int ordering;

    for (const auto& iso : *autos_) {
        // Compare the current set of gluing permutations with its
        // preimage under each edge pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (edge.setFirst(); edge.simp <
                static_cast<int>(pairing_.size()); edge++) {
            edgeDest = pairing_.dest(edge);
            if (pairing_.isUnmatched(edge) || edgeDest < edge)
                continue;

            edgeImage = iso[edge];
            ordering = gluingPerm(edge).compareWith(
                iso.edgePerm(edgeDest.simp).inverse() * gluingPerm(edgeImage)
                * iso.edgePerm(edge.simp));
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

