
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
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "utilities/memutils.h"

// If this symbol is uncommented, then the algorithm will become fairly
// brute-force, with no fast union-find code at all.  The only reason for
// offering this at all is to verify that the union-find code gives the
// same results.
// #define DIM4_NO_UNION_FIND

namespace regina {

const int Dim4GluingPermSearcher::edgeLinkNextFacet[10][5] = {
    // TODO: UFIND
};

const int Dim4GluingPermSearcher::edgeLinkPrevFacet[10][5] = {
    // TODO: UFIND
};

#ifdef DIM4_NO_UNION_FIND
const char Dim4GluingPermSearcher::dataTag_ = 'b';
#else
const char Dim4GluingPermSearcher::dataTag_ = 'g';
#endif

void Dim4GluingPermSearcher::PentEdgeState::dumpData(std::ostream& out) const {
    // TODO: UFIND
}

bool Dim4GluingPermSearcher::PentEdgeState::readData(std::istream& in,
        unsigned long nStates) {
    // TODO: UFIND
}

void Dim4GluingPermSearcher::PentFaceState::dumpData(std::ostream& out) const {
    // Be careful with the permutation code, which is an unsigned char
    // but which should be written as an int.
    out << parent << ' ' << rank << ' ' << size << ' '
        << (bounded ? 1 : 0) << ' '
        << static_cast<unsigned int>(twistUp.getPermCode()) << ' '
        << (hadEqualRank ? 1 : 0);
}

bool Dim4GluingPermSearcher::PentFaceState::readData(std::istream& in,
        unsigned long nStates) {
    in >> parent >> rank >> size;

    // bounded is a bool, but we need to read it as an int.
    int bBounded;
    in >> bBounded;
    bounded = bBounded;

    // The twistUp permutation code is an unsigned char, but we need to
    // read it as an unsigned int.  Don't actually set the permutation
    // code in twistUp until we know that the code is valid (which we
    // test later on).
    unsigned int twist;
    in >> twist;

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    if (parent < -1 || parent >= static_cast<long>(nStates))
        return false;
    if (rank >= nStates)
        return false;
    if (size >= nStates)
        return false;
    if (bBounded != 1 && bBounded != 0)
        return false;
    if (! NPerm::isPermCode(static_cast<unsigned char>(twist)))
        return false;
    if (bRank != 1 && bRank != 0)
        return false;

    twistUp.setPermCode(static_cast<unsigned char>(twist));

    return true;
}

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

    // ---------- Tracking of edge / face equivalence classes ----------

    // TODO: UFIND

    nFaceClasses_ = nPent * 10;
    faceState_ = new PentFaceState[nPent * 10];
    // The length of faceStateChanged_[] needs to be at least 5 * orderSize_.
    // Just be conservative here -- we know that orderSize_ <= 5 * nPent / 2.
    faceStateChanged_ = new int[25 * nPent / 2];
    std::fill(faceStateChanged_, faceStateChanged_ + (25 * nPent / 2), -1);
}

Dim4GluingPermSearcher::~Dim4GluingPermSearcher() {
    delete[] faceState_;
    delete[] faceStateChanged_;
    delete[] edgeState_;
    delete[] edgeStateChanged_;

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

    // ---------- Selecting the individual gluing permutations ----------

    int minOrder = orderElt_;
    int maxOrder = orderElt_ + maxDepth;

    Dim4PentFacet facet, adj;

    while (orderElt_ >= minOrder) {
        facet = order_[orderElt_];
        adj = (*pairing_)[facet];

        // TODO: Check for cancellation.

        // Move to the next permutation.

        // Be sure to preserve the orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            permIndex(facet)++;
        else
            permIndex(facet) += 2;

        // Are we out of ideas for this facet?
        if (permIndex(facet) >= 24) {
            // Yep.  Head back down to the previous facet.
            permIndex(facet) = -1;
            permIndex(adj) = -1;
            orderElt_--;

#ifndef DIM4_NO_UNION_FIND
            // Pull apart edge and face links at the previous level.
            if (orderElt_ >= minOrder) {
                splitEdgeClasses();
                splitFaceClasses();
            }
#endif

            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = NPerm::invS4[permIndex(facet)];

#ifndef DIM4_NO_UNION_FIND
        // Merge face links and run corresponding tests.
        if (mergeFaceClasses()) {
            // We created an invalid face.
            splitFaceClasses();
            continue;
        }

        // Merge edge links and run corresponding tests.
        if (mergeEdgeClasses()) {
            // We created an invalid edge.
            splitEdgeClasses();
            splitFaceClasses();
            continue;
        }
#else
        // Is this going to lead to an unwanted triangulation?
        if (badFaceLink(facet))
            continue;
#endif

        // Fix the orientation if appropriate.
        if (adj.facet == 0 && orientableOnly_) {
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

#ifndef DIM4_NO_UNION_FIND
            // Pull apart edge and face links at the previous level.
            if (orderElt_ >= minOrder) {
                splitEdgeClasses();
                splitFaceClasses();
            }
#endif
        } else {
            // Not a full triangulation; just one level deeper.

            // We've moved onto a new facet.
            // Be sure to get the orientation right.
            facet = order_[orderElt_];
            if (orientableOnly_ && pairing_->dest(facet).facet > 0) {
                // permIndex(facet) will be set to -1 or -2 as appropriate.
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

#ifndef DIM4_NO_UNION_FIND
                // Pull apart edge and face links at the previous level.
                if (orderElt_ >= minOrder) {
                    splitEdgeClasses();
                    splitFaceClasses();
                }
#endif
            }
        }
    }

    // And the search is over.

#ifndef DIM4_NO_UNION_FIND
    // Some extra sanity checking.
    if (minOrder == 0) {
        // Our edge classes had better be 10n standalone edges.
        // TODO: UFIND

        // And our face classes had better be 10n standalone faces.
        if (nFaceClasses_ != 10 * nPentachora)
            std::cerr << "ERROR: nFaceClasses == "
                << nFaceClasses_ << " at end of search!" << std::endl;
        for (unsigned i = 0; i < nPentachora * 10; ++i) {
            if (faceState_[i].parent != -1)
                std::cerr << "ERROR: faceState[" << i << "].parent == "
                    << faceState_[i].parent << " at end of search!"
                    << std::endl;
            if (faceState_[i].rank != 0)
                std::cerr << "ERROR: faceState[" << i << "].rank == "
                    << faceState_[i].rank << " at end of search!" << std::endl;
            if (faceState_[i].size != 1)
                std::cerr << "ERROR: faceState[" << i << "].size == "
                    << faceState_[i].size << " at end of search!" << std::endl;
            if (! faceState_[i].bounded)
                std::cerr << "ERROR: faceState[" << i << "].bounded == "
                    "false at end of search!" << std::endl;
            if (faceState_[i].hadEqualRank)
                std::cerr << "ERROR: faceState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nPentachora * 25 / 2; ++i)
            if (faceStateChanged_[i] != -1)
                std::cerr << "ERROR: faceStateChanged[" << i << "] == "
                    << faceStateChanged_[i] << " at end of search!"
                    << std::endl;
    }
#endif

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

    // ---------- Tracking of edge / face equivalence classes ----------

    // TODO: UFIND

    out << nFaceClasses_ << std::endl;
    for (i = 0; i < 10 * nPent; ++i) {
        faceState_[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 25 * nPent / 2; ++i) {
        if (i)
            out << ' ';
        out << faceStateChanged_[i];
    }
    out << std::endl;
}

Dim4GluingPermSearcher::Dim4GluingPermSearcher(std::istream& in,
        UseDim4GluingPerms use, void* useArgs) :
        Dim4GluingPerms(in), autos_(0), autosNew_(false),
        use_(use), useArgs_(useArgs), orientation_(0),
        order_(0), orderSize_(0), orderElt_(0),
        nEdgeClasses_(0), edgeState_(0), edgeStateChanged_(0),
        nFaceClasses_(0), faceState_(0), faceStateChanged_(0) {
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
    if (in.eof()) {
        inputError_ = true; return;
    }

    // ---------- Tracking of edge / face equivalence classes ----------

    unsigned i;

    // TODO: UFIND

    in >> nFaceClasses_;
    if (nFaceClasses_ > 10 * nPent) {
        inputError_ = true; return;
    }

    faceState_ = new PentFaceState[10 * nPent];
    for (i = 0; i < 10 * nPent; ++i)
        if (! faceState_[i].readData(in, 10 * nPent)) {
            inputError_ = true; return;
        }

    faceStateChanged_ = new int[25 * nPent / 2];
    for (i = 0; i < 25 * nPent / 2; ++i) {
        in >> faceStateChanged_[i];
        if (faceStateChanged_[i] < -1 ||
                 faceStateChanged_[i] >= 10 * static_cast<int>(nPent)) {
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

bool Dim4GluingPermSearcher::mergeEdgeClasses() {
    // TODO: UFIND
}

void Dim4GluingPermSearcher::splitEdgeClasses() {
    // TODO: UFIND
}

bool Dim4GluingPermSearcher::mergeFaceClasses() {
    Dim4PentFacet facet = order_[orderElt_];
    Dim4PentFacet adj = (*pairing_)[facet];

    bool retVal = false;

    NPerm5 p = gluingPerm(facet);
    int v1, w1, v2, w2;
    int e, f;
    int orderIdx;
    int eRep, fRep;

    v1 = facet.facet;
    w1 = p[v1];

    NPerm3 directTwist;
    for (v2 = 0; v2 < 5; ++v2) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        // Look at the face opposite edge v1-v2.
        e = Dim4Edge::edgeNumber[v1][v2];
        f = Dim4Edge::edgeNumber[w1][w2];

        orderIdx = v2 + 5 * orderElt_;

        // Vertices of a face are labelled in order from smallest to largest.
        if (p[Dim4Face::faceVertex[e][0]] == Dim4Face::faceVertex[f][0]) {
            if (p[Dim4Face::faceVertex[e][1]] == Dim4Face::faceVertex[f][1])
                directTwist.setPermCode(NPerm3::code012);
            else
                directTwist.setPermCode(NPerm3::code021);
        } else if (p[Dim4Face::faceVertex[e][0]] == Dim4Face::faceVertex[f][1]) {
            if (p[Dim4Face::faceVertex[e][1]] == Dim4Face::faceVertex[f][0])
                directTwist.setPermCode(NPerm3::code102);
            else
                directTwist.setPermCode(NPerm3::code120);
        } else {
            if (p[Dim4Face::faceVertex[e][1]] == Dim4Face::faceVertex[f][0])
                directTwist.setPermCode(NPerm3::code201);
            else
                directTwist.setPermCode(NPerm3::code210);
        }

        NPerm3 eTwist, fTwist; /* Initialise to identity permutations. */
        eRep = findFaceClass(e + 10 * facet.pent, eTwist);
        fRep = findFaceClass(f + 10 * adj.pent, fTwist);

        if (eRep == fRep) {
            faceState_[eRep].bounded = false;

            if (eTwist != fTwist * directTwist)
                retVal = true;

            faceStateChanged_[orderIdx] = -1;
        } else {
            if (faceState_[eRep].rank < faceState_[fRep].rank) {
                // Join eRep beneath fRep.
                faceState_[eRep].parent = fRep;
                faceState_[eRep].twistUp =
                    fTwist * directTwist * eTwist.inverse();
                faceState_[fRep].size += faceState_[eRep].size;

                faceStateChanged_[orderIdx] = eRep;
            } else {
                // Join fRep beneath eRep.
                faceState_[fRep].parent = eRep;
                faceState_[fRep].twistUp =
                    eTwist * directTwist.inverse() * fTwist.inverse();
                if (faceState_[eRep].rank == faceState_[fRep].rank) {
                    faceState_[eRep].rank++;
                    faceState_[fRep].hadEqualRank = true;
                }
                faceState_[eRep].size += faceState_[fRep].size;

                faceStateChanged_[orderIdx] = fRep;
            }
            --nFaceClasses_;
        }
    }

    return retVal;
}

void Dim4GluingPermSearcher::splitFaceClasses() {
    Dim4PentFacet facet = order_[orderElt_];

    int v1, v2;
    int f;
    int fIdx, orderIdx;
    int rep, subRep;

    v1 = facet.facet;

    for (v2 = 4; v2 >= 0; --v2) {
        if (v2 == v1)
            continue;

        // Look at the face opposite edge v1-v2.
        f = Dim4Edge::edgeNumber[v1][v2];

        fIdx = f + 10 * facet.pent;
        orderIdx = v2 + 5 * orderElt_;

        if (faceStateChanged_[orderIdx] < 0)
            faceState_[findFaceClass(fIdx)].bounded = true;
        else {
            subRep = faceStateChanged_[orderIdx];
            rep = faceState_[subRep].parent;

            faceState_[subRep].parent = -1;
            if (faceState_[subRep].hadEqualRank) {
                faceState_[subRep].hadEqualRank = false;
                faceState_[rep].rank--;
            }

            faceState_[rep].size -= faceState_[subRep].size;

            faceStateChanged_[orderIdx] = -1;
            ++nFaceClasses_;
        }
    }
}

void Dim4GluingPermSearcher::edgeBdryNext(int edgeID, int pent, int edge,
        int bdryFacet, int next[2], char twist[2]) {
    switch (edgeState_[edgeID].bdryEdges) {
        case 3: next[0] = next[1] = edgeID;
                twist[0] = twist[1] = 0;
                break;
        case 2: if (permIndex(pent, edgeLinkNextFacet[edge][bdryFacet]) < 0) {
                    next[0] = edgeState_[edgeID].bdryNext[0];
                    twist[0] = edgeState_[edgeID].bdryTwist[0];
                    next[1] = edgeID;
                    twist[1] = 0;
                } else if (permIndex(pent,
                        edgeLinkPrevFacet[edge][bdryFacet]) < 0) {
                    next[0] = edgeID;
                    twist[0] = 0;
                    next[1] = edgeState_[edgeID].bdryNext[1];
                    twist[1] = edgeState_[edgeID].bdryTwist[1];
                } else {
                    // We must be in the process of gluing a pentachoron
                    // to itself, and one of the gluings hasn't happened
                    // yet (hence bdryEdges == 2 but only one boundary
                    // edge shows up in the gluing permutations).
                    // The boundary that we're not seeing must belong
                    // to either the pentachoron face we are currently
                    // working with or its adjacent partner.
                    int ghostFace = (bdryFacet == order_[orderElt_].facet ?
                        (*pairing_)[order_[orderElt_]].facet :
                        order_[orderElt_].facet);
                    if (edgeLinkNextFacet[edge][bdryFacet] == ghostFace) {
                        next[0] = edgeState_[edgeID].bdryNext[0];
                        twist[0] = edgeState_[edgeID].bdryTwist[0];
                        next[1] = edgeID;
                        twist[1] = 0;
                    } else {
                        // Sanity check.
                        if (edgeLinkPrevFacet[edge][bdryFacet] != ghostFace)
                            std::cerr << "ERROR: Inconsistent edge link "
                                "boundary information!" << std::endl;
                        next[0] = edgeID;
                        twist[0] = 0;
                        next[1] = edgeState_[edgeID].bdryNext[1];
                        twist[1] = edgeState_[edgeID].bdryTwist[1];
                    }
                }
                break;
        case 1: next[0] = edgeState_[edgeID].bdryNext[0];
                next[1] = edgeState_[edgeID].bdryNext[1];
                twist[0] = edgeState_[edgeID].bdryTwist[0];
                twist[1] = edgeState_[edgeID].bdryTwist[1];
                break;
    }
}

void Dim4GluingPermSearcher::edgeBdryConsistencyCheck() {
    int adj, id, end;
    for (id = 0; id < static_cast<int>(getNumberOfPentachora()) * 5; ++id)
        if (edgeState_[id].bdryEdges > 0)
            for (end = 0; end < 2; ++end) {
                adj = edgeState_[id].bdryNext[end];
                if (edgeState_[adj].bdryEdges == 0)
                    std::cerr << "CONSISTENCY ERROR: Edge link boundary "
                        << id << '/' << end
                        << " runs into an internal edge." << std::endl;
                if (edgeState_[adj].bdryNext[(1 ^ end) ^
                        edgeState_[id].bdryTwist[end]] != id)
                    std::cerr << "CONSISTENCY ERROR: Edge link boundary "
                        << id << '/' << end
                        << " has a mismatched adjacency." << std::endl;
                if (edgeState_[adj].bdryTwist[(1 ^ end) ^
                        edgeState_[id].bdryTwist[end]] !=
                        edgeState_[id].bdryTwist[end])
                    std::cerr << "CONSISTENCY ERROR: Edge link boundary "
                        << id << '/' << end
                        << " has a mismatched twist." << std::endl;
            }
}

void Dim4GluingPermSearcher::edgeBdryDump(std::ostream& out) {
    for (unsigned id = 0; id < getNumberOfPentachora() * 5; ++id) {
        if (id > 0)
            out << ' ';
        out << edgeState_[id].bdryNext[0]
            << (edgeState_[id].bdryTwist[0] ? '~' : '-')
            << id
            << (edgeState_[id].bdryTwist[1] ? '~' : '-')
            << edgeState_[id].bdryNext[1]
            << " [" << int(edgeState_[id].bdryEdges) << ']';
    }
    out << std::endl;
}

} // namespace regina

