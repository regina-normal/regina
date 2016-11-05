
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
#include "census/gluingpermsearcher3.h"
#include "triangulation/dim3.h"
#include "utilities/memutils.h"

namespace regina {

const char GluingPermSearcher<3>::dataTag_ = 'g';

GluingPermSearcher<3>::GluingPermSearcher(
        const FacetPairing<3>* pairing, const FacetPairing<3>::IsoList* autos,
        bool orientableOnly, bool finiteOnly, int whichPurge,
        GluingPermSearcher<3>::Use use, void* useArgs) :
        GluingPerms<3>(pairing), autos_(autos), autosNew(autos == 0),
        orientableOnly_(orientableOnly), finiteOnly_(finiteOnly),
        whichPurge_(whichPurge), use_(use), useArgs_(useArgs),
        started(false),
        orientation(new int[pairing->size()]) {
    // Generate the list of face pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autosNew) {
        const_cast<GluingPermSearcher<3>*>(this)->autos_ =
            new FacetPairing<3>::IsoList();
        pairing->findAutomorphisms(const_cast<FacetPairing<3>::IsoList&>(*autos_));
    }

    // Initialise arrays.
    unsigned nTets = size();

    std::fill(orientation, orientation + nTets, 0);
    std::fill(permIndices_, permIndices_ + nTets* 4, -1);

    // Just fill the order[] array in a default left-to-right fashion.
    // Subclasses can rearrange things if they choose.
    order = new FacetSpec<3>[nTets * 2];
    orderElt = orderSize = 0;

    FacetSpec<3> face, adj;
    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++)
        if (! pairing->isUnmatched(face))
            if (face < pairing->dest(face))
                order[orderSize++] = face;
}

GluingPermSearcher<3>::~GluingPermSearcher() {
    delete[] orientation;
    delete[] order;
    if (autosNew) {
        // We made them, so we'd better remove the const again and
        // delete them.
        FacetPairing<3>::IsoList* autos =
            const_cast<FacetPairing<3>::IsoList*>(autos_);
        std::for_each(autos->begin(), autos->end(), FuncDelete<Isomorphism<3>>());
        delete autos;
    }
}

GluingPermSearcher<3>* GluingPermSearcher<3>::bestSearcher(
        const FacetPairing<3>* pairing, const FacetPairing<3>::IsoList* autos,
        bool orientableOnly, bool finiteOnly, int whichPurge,
        GluingPermSearcher<3>::Use use, void* useArgs) {
    // Use an optimised algorithm if possible.
    if (finiteOnly) {
        if (pairing->isClosed() && pairing->size() >= 3 &&
                (whichPurge & PURGE_NON_MINIMAL) &&
                (whichPurge & PURGE_NON_PRIME) &&
                (orientableOnly || (whichPurge & PURGE_P2_REDUCIBLE))) {
                // Closed prime minimal P2-irreducible triangulations with >= 3
                // tetrahedra.
                return new ClosedPrimeMinSearcher(pairing, autos,
                    orientableOnly, use, useArgs);
            }
        return new CompactSearcher(pairing, autos, orientableOnly,
            whichPurge, use, useArgs);
    }

    if (pairing->isClosed() && ((whichPurge & PURGE_NON_MINIMAL_HYP) ==
            PURGE_NON_MINIMAL_HYP))
        return new HyperbolicMinSearcher(pairing, autos, orientableOnly,
            use, useArgs);

    return new GluingPermSearcher<3>(pairing, autos, orientableOnly, finiteOnly,
        whichPurge, use, useArgs);
}

void GluingPermSearcher<3>::findAllPerms(const FacetPairing<3>* pairing,
        const FacetPairing<3>::IsoList* autos, bool orientableOnly,
        bool finiteOnly, int whichPurge, GluingPermSearcher<3>::Use use,
        void* useArgs) {
    GluingPermSearcher<3>* searcher = bestSearcher(pairing, autos,
        orientableOnly, finiteOnly, whichPurge, use, useArgs);
    searcher->runSearch();
    delete searcher;
}

void GluingPermSearcher<3>::runSearch(long maxDepth) {
    // In this generation algorithm, each orientation is simply +/-1.

    unsigned nTetrahedra = size();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTetrahedra * 4 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 || pairing_->dest(0, 0).isBoundary(nTetrahedra)) {
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

    FacetSpec<3> face, adj;

    while (orderElt >= minOrder) {
        face = order[orderElt];
        adj = (*pairing_)[face];

        // TODO: Check for cancellation.

        // Move to the next permutation.

        // Be sure to preserve the orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            permIndex(face)++;
        else
            permIndex(face) += 2;

        // Are we out of ideas for this face?
        if (permIndex(face) >= 6) {
            // Yep.  Head back down to the previous face.
            permIndex(face) = -1;
            permIndex(adj) = -1;
            orderElt--;
            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = Perm<3>::invS3[permIndex(face)];

        // Is this going to lead to an unwanted triangulation?
        //
        // Don't test for degree 1 or 2 edges here - for situations
        // where these can be purged, we will be using a specialised
        // subclass of GluingPermSearcher<3> with its own custom
        // implementation of runSearch().
        if (lowDegreeEdge(face, false /* degree 1,2 */,
                whichPurge_ & PURGE_NON_MINIMAL))
            continue;
        if (! orientableOnly_)
            if (badEdgeLink(face))
                continue;

        // Fix the orientation if appropriate.
        if (adj.facet == 0 && orientableOnly_) {
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

            if (orderElt == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                use_(this, useArgs_);

                // Back to the previous face.
                permIndex(face) = -1;
                orderElt--;
            }
        }
    }

    // And the search is over.
    use_(0, useArgs_);
}

void GluingPermSearcher<3>::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
}

GluingPermSearcher<3>* GluingPermSearcher<3>::readTaggedData(std::istream& in,
        GluingPermSearcher<3>::Use use, void* useArgs) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        return 0;

    GluingPermSearcher<3>* ans;
    if (c == GluingPermSearcher<3>::dataTag_)
        ans = new GluingPermSearcher<3>(in, use, useArgs);
    else if (c == CompactSearcher::dataTag_)
        ans = new CompactSearcher(in, use, useArgs);
    else if (c == ClosedPrimeMinSearcher::dataTag_)
        ans = new ClosedPrimeMinSearcher(in, use, useArgs);
    else if (c == HyperbolicMinSearcher::dataTag_)
        ans = new HyperbolicMinSearcher(in, use, useArgs);
    else
        return 0;

    if (ans->inputError()) {
        delete ans;
        return 0;
    }

    return ans;
}

void GluingPermSearcher<3>::dumpData(std::ostream& out) const {
    GluingPerms<3>::dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (finiteOnly_ ? 'f' : '.');
    out << (started ? 's' : '.');
    out << ' ' << whichPurge_ << std::endl;

    int nTets = size();
    int i;

    for (i = 0; i < nTets; i++) {
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

GluingPermSearcher<3>::GluingPermSearcher(std::istream& in,
        GluingPermSearcher<3>::Use use, void* useArgs) :
        GluingPerms<3>(in), autos_(0), autosNew(false),
        use_(use), useArgs_(useArgs), orientation(0),
        order(0), orderSize(0), orderElt(0) {
    if (inputError_)
        return;

    // Recontruct the face pairing automorphisms.
    const_cast<GluingPermSearcher<3>*>(this)->autos_ =
        new FacetPairing<3>::IsoList();
    pairing_->findAutomorphisms(const_cast<FacetPairing<3>::IsoList&>(*autos_));
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
    if (c == 'f')
        finiteOnly_ = true;
    else if (c == '.')
        finiteOnly_ = false;
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

    in >> whichPurge_;

    int nTets = pairing_->size();
    int t;

    orientation = new int[nTets];
    for (t = 0; t < nTets; t++)
        in >> orientation[t];

    order = new FacetSpec<3>[2 * nTets];
    in >> orderElt >> orderSize;
    for (t = 0; t < orderSize; t++) {
        in >> order[t].simp >> order[t].facet;
        if (order[t].simp >= nTets || order[t].simp < 0 ||
                order[t].facet >= 4 || order[t].facet < 0) {
            inputError_ = true; return;
        }
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

bool GluingPermSearcher<3>::isCanonical() const {
    FacetSpec<3> face, faceDest, faceImage;
    int ordering;

    for (FacetPairing<3>::IsoList::const_iterator it = autos_->begin();
            it != autos_->end(); it++) {
        // Compare the current set of gluing permutations with its
        // preimage under each face pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (face.setFirst(); face.simp <
                static_cast<int>(pairing_->size()); face++) {
            faceDest = pairing_->dest(face);
            if (pairing_->isUnmatched(face) || faceDest < face)
                continue;

            faceImage = (**it)[face];
            ordering = gluingPerm(face).compareWith(
                (*it)->facePerm(faceDest.simp).inverse() * gluingPerm(faceImage)
                * (*it)->facePerm(face.simp));
            if (ordering < 0) {
                // This permutation set is closer.
                break;
            } else if (ordering > 0) {
                // The transformed permutation set is closer.
                return false;
            }

            // So far it's an automorphism of gluing permutations also.
            // Keep running through faces.
        }
        // Nothing broke with this automorphism.  On to the next one.
    }

    // Nothing broke at all.
    return true;
}

bool GluingPermSearcher<3>::badEdgeLink(const FacetSpec<3>& face) const {
    // Run around all three edges bounding the face.
    FacetSpec<3> adj;
    unsigned tet;
    Perm<4> current;
    Perm<4> start(face.facet, 3);
    bool started, incomplete;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * Perm<4>(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.simp;

        started = false;
        incomplete = false;

        while ((! started) || (static_cast<int>(tet) != face.simp) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            // Test for a return to the original tetrahedron with the
            // orientation reversed; this either means a bad edge link
            // or a bad vertex link.
            if (started && finiteOnly_ && static_cast<int>(tet) == face.simp)
                if (start[3] == current[3] && start.sign() != current.sign())
                    return true;

            // Push through the current tetrahedron.
            started = true;
            current = current * Perm<4>(2, 3);

            // Push across a face.
            if (pairing_->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing_->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.simp;
        }

        // Did we meet the original edge in reverse?
        if ((! incomplete) && (start != current))
            return true;
    }

    // No bad edge links were found.
    return false;
}

bool GluingPermSearcher<3>::lowDegreeEdge(const FacetSpec<3>& face,
        bool testDegree12, bool testDegree3) const {
    // Run around all three edges bounding the face.
    FacetSpec<3> adj;
    unsigned tet;
    Perm<4> current;
    Perm<4> start(face.facet, 3);
    bool started, incomplete;
    unsigned size;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * Perm<4>(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.simp;

        started = false;
        incomplete = false;
        size = 0;

        while ((! started) || (static_cast<int>(tet) != face.simp) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            started = true;

            // We're about to push through the current tetrahedron; see
            // if we've already exceeded the size of edge links that we
            // care about.
            if (size >= 3) {
                incomplete = true;
                break;
            }

            // Push through the current tetrahedron.
            current = current * Perm<4>(2, 3);

            // Push across a face.
            if (pairing_->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing_->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.simp;
            size++;
        }

        if (! incomplete) {
            if (testDegree12 && size < 3)
                return true;
            if (testDegree3 && size == 3) {
                // Only throw away a degree three edge if it involves
                // three distinct tetrahedra.
                int tet1 = pairing_->dest(face.simp, start[2]).simp;
                int tet2 = pairing_->dest(face.simp, start[3]).simp;
                if (face.simp != tet1 && tet1 != tet2 && tet2 != face.simp)
                    return true;
            }
        }
    }

    // No bad low-degree edges were found.
    return false;
}

} // namespace regina

