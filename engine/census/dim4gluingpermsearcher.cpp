
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "census/dim4gluingpermsearcher.h"
#include "utilities/memutils.h"

namespace regina {

const char Dim4GluingPermSearcher::dataTag_ = 'g';

Dim4GluingPermSearcher::Dim4GluingPermSearcher(
        const Dim4FacetPairing* pairing, const Dim4FacetPairingIsoList* autos,
        bool orientableOnly, bool finiteOnly,
        UseDim4GluingPerms use, void* useArgs) :
        Dim4GluingPerms(pairing), autos_(autos), autosNew_(autos == 0),
        orientableOnly_(orientableOnly), finiteOnly_(finiteOnly),
        use_(use), useArgs_(useArgs),
        started_(false),
        orientation_(new int[pairing->getNumberOfPentachora()]) {
    // Generate the list of facet pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autosNew_) {
        const_cast<Dim4GluingPermSearcher*>(this)->autos_ =
            new Dim4FacetPairingIsoList();
        pairing->findAutomorphisms(
            const_cast<Dim4FacetPairingIsoList&>(*autos_));
    }

    // Initialise arrays.
    unsigned nPent = getNumberOfPentachora();

    std::fill(orientation_, orientation_ + nPent, 0);
    std::fill(permIndices_, permIndices_ + nPent * 5, -1);

    // Just fill the order_[] array in a default left-to-right fashion.
    // Subclasses can rearrange things if they choose.
    order_ = new Dim4PentFacet[(nPent * 5) / 2];
    orderElt_ = orderSize_ = 0;

    Dim4PentFacet facet, adj;
    for (facet.setFirst(); ! facet.isPastEnd(nPent, true); facet++)
        if (! pairing->isUnmatched(facet))
            if (facet < pairing->dest(facet))
                order_[orderSize_++] = facet;
}

Dim4GluingPermSearcher::~Dim4GluingPermSearcher() {
    delete[] orientation_;
    delete[] order_;
    if (autosNew_) {
        // We made them, so we'd better remove the const again and
        // delete them.
        Dim4FacetPairingIsoList* autos =
            const_cast<Dim4FacetPairingIsoList*>(autos_);
        std::for_each(autos->begin(), autos->end(),
            FuncDelete<Dim4Isomorphism>());
        delete autos;
    }
}

Dim4GluingPermSearcher* Dim4GluingPermSearcher::bestSearcher(
        const Dim4FacetPairing* pairing, const Dim4FacetPairingIsoList* autos,
        bool orientableOnly, bool finiteOnly,
        UseDim4GluingPerms use, void* useArgs) {
    // Do everything by brute force for now.
    return new Dim4GluingPermSearcher(pairing, autos,
        orientableOnly, finiteOnly, use, useArgs);
}

void Dim4GluingPermSearcher::findAllPerms(const Dim4FacetPairing* pairing,
        const Dim4FacetPairingIsoList* autos, bool orientableOnly,
        bool finiteOnly, UseDim4GluingPerms use, void* useArgs) {
    Dim4GluingPermSearcher* searcher = bestSearcher(pairing, autos,
        orientableOnly, finiteOnly, use, useArgs);
    searcher->runSearch();
    delete searcher;
}

void Dim4GluingPermSearcher::runSearch(long maxDepth) {
    // In this generation algorithm, each orientation is simply +/-1.

    unsigned nPentachora = getNumberOfPentachora();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nPentachora * 5 + 1;
    }

    if (! started_) {
        // Search initialisation.
        started_ = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 || pairing_->dest(0, 0).isBoundary(nPentachora)) {
            use_(this, useArgs_);
            use_(0, useArgs_);
            return;
        }

        orderElt_ = 0;
        orientation_[0] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt_ == orderSize_) {
        if (isCanonical())
            use_(this, useArgs_);
        use_(0, useArgs_);
        return;
    }

    int minOrder = orderElt_;
    int maxOrder = orderElt_ + maxDepth;

    Dim4PentFacet facet, adj;

    while (orderElt_ >= minOrder) {
        facet = order_[orderElt_];
        adj = (*pairing_)[facet];

        // TODO: Check for cancellation.

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            permIndex(facet)++;
        else
            permIndex(facet) += 2;

        if (permIndex(facet) >= 24) {
            // Out of ideas for this facet.
            // Head back down to the previous facet.
            permIndex(facet) = -1;
            permIndex(adj) = -1;
            orderElt_--;
            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = NPerm::invS4[permIndex(facet)];

        // Is this going to lead to an unwanted triangulation?
        if (badFaceLink(facet))
            continue;

        // Fix the orientation if appropriate.
        if (adj.facet == 0) {
            // It's the first time we've hit this pentachoron.
            if ((permIndex(facet) + (facet.facet == 4 ? 0 : 1) +
                    (adj.facet == 4 ? 0 : 1)) % 2 == 0)
                orientation_[adj.pent] = -orientation_[facet.pent];
            else
                orientation_[adj.pent] = orientation_[facet.pent];
        }

        // Move on to the next facet.
        orderElt_++;

        // If we're at the end, try the solution and step back.
        if (orderElt_ == orderSize_) {
            // We in fact have an entire triangulation.
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            if (isCanonical())
                use_(this, useArgs_);

            // Back to the previous facet.
            orderElt_--;
        } else {
            // Not a full triangulation; just one level deeper.

            // We've moved onto a new facet.
            // Be sure to get the orientation right.
            facet = order_[orderElt_];
            if (orientableOnly_ && pairing_->dest(facet).facet > 0) {
                adj = (*pairing_)[facet];
                if (orientation_[facet.pent] == orientation_[adj.pent])
                    permIndex(facet) = 1;
                else
                    permIndex(facet) = 0;

                if ((facet.facet == 4 ? 0 : 1) + (adj.facet == 4 ? 0 : 1) == 1)
                    permIndex(facet) = (permIndex(facet) + 1) % 2;

                permIndex(facet) -= 2;
            }

            if (orderElt_ == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                use_(this, useArgs_);

                // Back to the previous facet.
                permIndex(facet) = -1;
                orderElt_--;
            }
        }
    }

    // And the search is over.
    use_(0, useArgs_);
}

void Dim4GluingPermSearcher::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
}

Dim4GluingPermSearcher* Dim4GluingPermSearcher::readTaggedData(std::istream& in,
        UseDim4GluingPerms use, void* useArgs) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        return 0;

    Dim4GluingPermSearcher* ans;
    if (c == Dim4GluingPermSearcher::dataTag_)
        ans = new Dim4GluingPermSearcher(in, use, useArgs);
    else
        return 0;

    if (ans->inputError()) {
        delete ans;
        return 0;
    }

    return ans;
}

void Dim4GluingPermSearcher::dumpData(std::ostream& out) const {
    Dim4GluingPerms::dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (finiteOnly_ ? 'f' : '.');
    out << (started_ ? 's' : '.');
    out << std::endl;

    int nPent = getNumberOfPentachora();
    int i;

    for (i = 0; i < nPent; ++i) {
        if (i)
            out << ' ';
        out << orientation_[i];
    }
    out << std::endl;

    out << orderElt_ << ' ' << orderSize_ << std::endl;
    for (i = 0; i < orderSize_; ++i) {
        if (i)
            out << ' ';
        out << order_[i].pent << ' ' << order_[i].facet;
    }
    out << std::endl;
}

Dim4GluingPermSearcher::Dim4GluingPermSearcher(std::istream& in,
        UseDim4GluingPerms use, void* useArgs) :
        Dim4GluingPerms(in), autos_(0), autosNew_(false),
        use_(use), useArgs_(useArgs), orientation_(0),
        order_(0), orderSize_(0), orderElt_(0) {
    if (inputError_)
        return;

    // Recontruct the facet pairing automorphisms.
    const_cast<Dim4GluingPermSearcher*>(this)->autos_ =
        new Dim4FacetPairingIsoList();
    pairing_->findAutomorphisms(const_cast<Dim4FacetPairingIsoList&>(*autos_));
    autosNew_ = true;

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
        started_ = true;
    else if (c == '.')
        started_ = false;
    else {
        inputError_ = true; return;
    }

    int nPent = pairing_->getNumberOfPentachora();
    int p;

    orientation_ = new int[nPent];
    for (p = 0; p < nPent; ++p)
        in >> orientation_[p];

    order_ = new Dim4PentFacet[(nPent * 5) / 2];
    in >> orderElt_ >> orderSize_;
    for (p = 0; p < orderSize_; ++p) {
        in >> order_[p].pent >> order_[p].facet;
        if (order_[p].pent >= nPent || order_[p].pent < 0 ||
                order_[p].facet >= 5 || order_[p].facet < 0) {
            inputError_ = true; return;
        }
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

bool Dim4GluingPermSearcher::isCanonical() const {
    Dim4PentFacet facet, facetDest, facetImage;
    int ordering;

    for (Dim4FacetPairingIsoList::const_iterator it = autos_->begin();
            it != autos_->end(); ++it) {
        // Compare the current set of gluing permutations with its
        // preimage under each facet pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (facet.setFirst(); facet.pent <
                static_cast<int>(pairing_->getNumberOfPentachora()); facet++) {
            facetDest = pairing_->dest(facet);
            if (pairing_->isUnmatched(facet) || facetDest < facet)
                continue;

            facetImage = (**it)[facet];
            ordering = gluingPerm(facet).compareWith(
                (*it)->facetPerm(facetDest.pent).inverse()
                * gluingPerm(facetImage) * (*it)->facetPerm(facet.pent));
            if (ordering < 0) {
                // This permutation set is closer.
                break;
            } else if (ordering > 0) {
                // The transformed permutation set is closer.
                return false;
            }

            // So far it's an automorphism of gluing permutations also.
            // Keep running through facets.
        }
        // Nothing broke with this automorphism.  On to the next one.
    }

    // Nothing broke at all.
    return true;
}

bool Dim4GluingPermSearcher::badFaceLink(const Dim4PentFacet& facet) const {
    // Run around all four faces bounding the facet.
    Dim4PentFacet adj;
    unsigned pent;
    NPerm5 current;
    NPerm5 start(facet.facet, 4);
    bool started, incomplete;
    for (unsigned permIdx = 0; permIdx < 4; ++permIdx) {
        start = start * NPerm5(1, 2, 3, 0, 4);

        // start maps (0,1,2,3) to the four vertices of facet, with
        // (0,1,2) mapped to the 2-dimensional face that we wish to examine.

        // Continue to push through a pentachoron and then across a
        // facet, until either we hit a boundary or we return to the
        // original face.

        current = start;
        pent = facet.pent;

        started = false;
        incomplete = false;

        while ((! started) || (static_cast<int>(pent) != facet.pent) ||
                (start[3] != current[3]) || (start[4] != current[4])) {
            // Push through the current pentachoron.
            started = true;
            current = current * NPerm5(3, 4);

            // Push across a face.
            if (pairing_->isUnmatched(pent, current[4])) {
                incomplete = true;
                break;
            }
            adj = pairing_->dest(pent, current[4]);

            if (permIndex(pent, current[4]) >= 0) {
                current = gluingPerm(pent, current[4]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            pent = adj.pent;
        }

        // Did we meet the original face with a rotation or reflection?
        if ((! incomplete) && (start != current))
            return true;
    }

    // No bad face links were found.
    return false;
}

} // namespace regina

