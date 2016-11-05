
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
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim4.h"
#include "utilities/memutils.h"

// If this symbol is uncommented, then the algorithm will become fairly
// brute-force, with no fast union-find code at all.  The only reason for
// offering this at all is to verify that the union-find code gives the
// same results.
// #define DIM4_NO_UNION_FIND

namespace regina {

const int GluingPermSearcher<4>::edgeLinkNextFacet[10][5] = {
    { -1, -1,  3,  4,  2 },
    { -1,  3, -1,  4,  1 },
    { -1,  2,  4, -1,  1 },
    { -1,  2,  3,  1, -1 },
    {  3, -1, -1,  4,  0 },
    {  2, -1,  4, -1,  0 },
    {  2, -1,  3,  0, -1 },
    {  1,  4, -1, -1,  0 },
    {  1,  3, -1,  0, -1 },
    {  1,  2,  0, -1, -1 }
};

const int GluingPermSearcher<4>::edgeLinkPrevFacet[10][5] = {
    { -1, -1,  4,  2,  3 },
    { -1,  4, -1,  1,  3 },
    { -1,  4,  1, -1,  2 },
    { -1,  3,  1,  2, -1 },
    {  4, -1, -1,  0,  3 },
    {  4, -1,  0, -1,  2 },
    {  3, -1,  0,  2, -1 },
    {  4,  0, -1, -1,  1 },
    {  3,  0, -1,  1, -1 },
    {  2,  0,  1, -1, -1 }
};

#ifdef DIM4_NO_UNION_FIND
const char GluingPermSearcher<4>::dataTag_ = 'b';
#else
const char GluingPermSearcher<4>::dataTag_ = 'g';
#endif

void GluingPermSearcher<4>::PentEdgeState::dumpData(std::ostream& out) const {
    // Be careful with the twisting fields, which are chars but which should
    // be written as ints.
    out << parent << ' ' << rank << ' ' << bdry << ' '
        << (twistUpEdge ? 1 : 0) << ' ' << (twistUpTriangle ? 1 : 0) << ' '
        << (hadEqualRank ? 1 : 0) << ' '
        << static_cast<int>(bdryEdges) << ' '
        << bdryNext[0] << ' ' << bdryNext[1] << ' '
        << static_cast<int>(bdryTwist[0]) << ' '
        << static_cast<int>(bdryTwist[1]) << ' '
        << bdryNextOld[0] << ' ' << bdryNextOld[1] << ' '
        << static_cast<int>(bdryTwistOld[0]) << ' '
        << static_cast<int>(bdryTwistOld[1]);
}

bool GluingPermSearcher<4>::PentEdgeState::readData(std::istream& in,
        unsigned long nStates) {
    in >> parent >> rank >> bdry;

    // The twist fields are chars, but we need to read them as ints.
    int twistEdge;
    in >> twistEdge;
    twistUpEdge = twistEdge;

    int twistTriangle;
    in >> twistTriangle;
    twistUpTriangle = twistTriangle;

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    // More chars to ints coming.
    int bVal;

    in >> bVal; bdryEdges = bVal;
    in >> bdryNext[0] >> bdryNext[1];
    in >> bVal; bdryTwist[0] = bVal;
    in >> bVal; bdryTwist[1] = bVal;
    in >> bdryNextOld[0] >> bdryNextOld[1];
    in >> bVal; bdryTwistOld[0] = bVal;
    in >> bVal; bdryTwistOld[1] = bVal;

    if (parent < -1 || parent >= static_cast<long>(nStates))
        return false;
    if (rank >= nStates)
        return false;
    if (bdry > 3 * nStates)
        return false;
    if (twistEdge != 1 && twistEdge != 0)
        return false;
    if (twistTriangle != 1 && twistTriangle != 0)
        return false;
    if (bRank != 1 && bRank != 0)
        return false;
    if (bdryEdges > 3) /* Never < 0 since this is unsigned. */
        return false;
    if (bdryNext[0] < 0 || bdryNext[0] >= static_cast<long>(nStates))
        return false;
    if (bdryNext[1] < 0 || bdryNext[1] >= static_cast<long>(nStates))
        return false;
    if (bdryNextOld[0] < -1 || bdryNext[0] >= static_cast<long>(nStates))
        return false;
    if (bdryNextOld[1] < -1 || bdryNextOld[1] >= static_cast<long>(nStates))
        return false;
    if (bdryTwist[0] < 0 || bdryTwist[0] > 1)
        return false;
    if (bdryTwist[1] < 0 || bdryTwist[1] > 1)
        return false;
    if (bdryTwistOld[0] < 0 || bdryTwistOld[0] > 1)
        return false;
    if (bdryTwistOld[1] < 0 || bdryTwistOld[1] > 1)
        return false;

    return true;
}

void GluingPermSearcher<4>::PentTriangleState::dumpData(std::ostream& out)
        const {
    // Be careful with the permutation code, which is an unsigned char
    // but which should be written as an int.
    out << parent << ' ' << rank << ' ' << size << ' '
        << (bounded ? 1 : 0) << ' '
        << static_cast<unsigned int>(twistUp.permCode()) << ' '
        << (hadEqualRank ? 1 : 0);
}

bool GluingPermSearcher<4>::PentTriangleState::readData(std::istream& in,
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
    if (! Perm<3>::isPermCode(static_cast<unsigned char>(twist)))
        return false;
    if (bRank != 1 && bRank != 0)
        return false;

    twistUp.setPermCode(static_cast<unsigned char>(twist));

    return true;
}

GluingPermSearcher<4>::GluingPermSearcher(
        const FacetPairing<4>* pairing, const FacetPairing<4>::IsoList* autos,
        bool orientableOnly, bool finiteOnly,
        GluingPermSearcher<4>::Use use, void* useArgs) :
        GluingPerms<4>(pairing), autos_(autos), autosNew_(autos == 0),
        orientableOnly_(orientableOnly), finiteOnly_(finiteOnly),
        use_(use), useArgs_(useArgs),
        started_(false),
        orientation_(new int[pairing->size()]) {
    // Generate the list of facet pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autosNew_) {
        const_cast<GluingPermSearcher<4>*>(this)->autos_ =
            new FacetPairing<4>::IsoList();
        pairing->findAutomorphisms(
            const_cast<FacetPairing<4>::IsoList&>(*autos_));
    }

    // Initialise arrays.
    unsigned nPent = size();

    std::fill(orientation_, orientation_ + nPent, 0);
    std::fill(permIndices_, permIndices_ + nPent * 5, -1);

    // Just fill the order_[] array in a default left-to-right fashion.
    // Subclasses can rearrange things if they choose.
    order_ = new FacetSpec<4>[(nPent * 5) / 2];
    orderElt_ = orderSize_ = 0;

    FacetSpec<4> facet, adj;
    for (facet.setFirst(); ! facet.isPastEnd(nPent, true); facet++)
        if (! pairing->isUnmatched(facet))
            if (facet < pairing->dest(facet))
                order_[orderSize_++] = facet;

    // ---------- Tracking of edge / triangle equivalence classes ----------

    nEdgeClasses_ = nPent * 10;
    edgeState_ = new PentEdgeState[nPent * 10];
    // The length of triStateChanged_[] needs to be at least 10 * orderSize_.
    // Just be conservative here -- we know that orderSize_ <= 5 * nPent / 2.
    edgeStateChanged_ = new int[nPent * 25];
    std::fill(edgeStateChanged_, edgeStateChanged_ + nPent * 25, -1);
    for (unsigned i = 0; i < nPent * 10; ++i) {
        edgeState_[i].bdryEdges = 3;
        edgeState_[i].bdryNext[0] = edgeState_[i].bdryNext[1] = i;
        edgeState_[i].bdryTwist[0] = edgeState_[i].bdryTwist[1] = 0;
        // Initialise the backup members also so we're not writing
        // uninitialised data via dumpData().
        edgeState_[i].bdryNextOld[0] = edgeState_[i].bdryNextOld[1] = -1;
        edgeState_[i].bdryTwistOld[0] = edgeState_[i].bdryTwistOld[1] = 0;
    }

    nTriangleClasses_ = nPent * 10;
    triState_ = new PentTriangleState[nPent * 10];
    // The length of triStateChanged_[] needs to be at least 5 * orderSize_.
    // Just be conservative here -- we know that orderSize_ <= 5 * nPent / 2.
    triStateChanged_ = new int[25 * nPent / 2];
    std::fill(triStateChanged_, triStateChanged_ + (25 * nPent / 2), -1);
}

GluingPermSearcher<4>::~GluingPermSearcher() {
    delete[] triState_;
    delete[] triStateChanged_;
    delete[] edgeState_;
    delete[] edgeStateChanged_;

    delete[] orientation_;
    delete[] order_;
    if (autosNew_) {
        // We made them, so we'd better remove the const again and
        // delete them.
        FacetPairing<4>::IsoList* autos =
            const_cast<FacetPairing<4>::IsoList*>(autos_);
        std::for_each(autos->begin(), autos->end(),
            FuncDelete<Isomorphism<4>>());
        delete autos;
    }
}

GluingPermSearcher<4>* GluingPermSearcher<4>::bestSearcher(
        const FacetPairing<4>* pairing, const FacetPairing<4>::IsoList* autos,
        bool orientableOnly, bool finiteOnly,
        GluingPermSearcher<4>::Use use, void* useArgs) {
    // Do everything by brute force for now.
    return new GluingPermSearcher<4>(pairing, autos,
        orientableOnly, finiteOnly, use, useArgs);
}

void GluingPermSearcher<4>::findAllPerms(const FacetPairing<4>* pairing,
        const FacetPairing<4>::IsoList* autos, bool orientableOnly,
        bool finiteOnly, GluingPermSearcher<4>::Use use, void* useArgs) {
    GluingPermSearcher<4>* searcher = bestSearcher(pairing, autos,
        orientableOnly, finiteOnly, use, useArgs);
    searcher->runSearch();
    delete searcher;
}

void GluingPermSearcher<4>::runSearch(long maxDepth) {
    // In this generation algorithm, each orientation is simply +/-1.

    unsigned nPentachora = size();
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

    FacetSpec<4> facet, adj;

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
            // Pull apart edge and triangle links at the previous level.
            if (orderElt_ >= minOrder) {
                splitEdgeClasses();
                splitTriangleClasses();
            }
#endif

            continue;
        }

        // We are sitting on a new permutation to try.
        permIndex(adj) = Perm<4>::invS4[permIndex(facet)];

#ifndef DIM4_NO_UNION_FIND
        // Merge triangle links and run corresponding tests.
        if (mergeTriangleClasses()) {
            // We created an invalid triangle.
            splitTriangleClasses();
            continue;
        }

        // Merge edge links and run corresponding tests.
        if (mergeEdgeClasses()) {
            // We created an invalid edge.
            splitEdgeClasses();
            splitTriangleClasses();
            continue;
        }
#else
        // Is this going to lead to an unwanted triangulation?
        if (badTriangleLink(facet))
            continue;
#endif

        // Fix the orientation if appropriate.
        if (adj.facet == 0 && orientableOnly_) {
            // It's the first time we've hit this pentachoron.
            if ((permIndex(facet) + (facet.facet == 4 ? 0 : 1) +
                    (adj.facet == 4 ? 0 : 1)) % 2 == 0)
                orientation_[adj.simp] = -orientation_[facet.simp];
            else
                orientation_[adj.simp] = orientation_[facet.simp];
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
            // Pull apart edge and triangle links at the previous level.
            if (orderElt_ >= minOrder) {
                splitEdgeClasses();
                splitTriangleClasses();
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
                if (orientation_[facet.simp] == orientation_[adj.simp])
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
                // Pull apart edge and triangle links at the previous level.
                if (orderElt_ >= minOrder) {
                    splitEdgeClasses();
                    splitTriangleClasses();
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
        if (nEdgeClasses_ != 10 * nPentachora)
            std::cerr << "ERROR: nEdgeClasses == "
                << nEdgeClasses_ << " at end of search!" << std::endl;
        for (int i = 0; i < static_cast<int>(nPentachora) * 10; ++i) {
            if (edgeState_[i].parent != -1)
                std::cerr << "ERROR: edgeState[" << i << "].parent == "
                    << edgeState_[i].parent << " at end of search!"
                    << std::endl;
            if (edgeState_[i].rank != 0)
                std::cerr << "ERROR: edgeState[" << i << "].rank == "
                    << edgeState_[i].rank << " at end of search!" << std::endl;
            if (edgeState_[i].bdry != 3)
                std::cerr << "ERROR: edgeState[" << i << "].bdry == "
                    << edgeState_[i].bdry << " at end of search!" << std::endl;
            if (edgeState_[i].hadEqualRank)
                std::cerr << "ERROR: edgeState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
            if (edgeState_[i].bdryEdges != 3)
                std::cerr << "ERROR: edgeState[" << i << "].bdryEdges == "
                    << static_cast<int>(edgeState_[i].bdryEdges)
                    << " at end of search!" << std::endl;
            if (edgeState_[i].bdryNext[0] != i)
                std::cerr << "ERROR: edgeState[" << i << "].bdryNext[0] == "
                    << edgeState_[i].bdryNext[0] << " at end of search!"
                    << std::endl;
            if (edgeState_[i].bdryNext[1] != i)
                std::cerr << "ERROR: edgeState[" << i << "].bdryNext[1] == "
                    << edgeState_[i].bdryNext[1] << " at end of search!"
                    << std::endl;
            if (edgeState_[i].bdryTwist[0])
                std::cerr << "ERROR: edgeState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
            if (edgeState_[i].bdryTwist[1])
                std::cerr << "ERROR: edgeState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nPentachora * 25; ++i)
            if (edgeStateChanged_[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged_[i] << " at end of search!"
                    << std::endl;

        // And our triangle classes had better be 10n standalone triangles.
        if (nTriangleClasses_ != 10 * nPentachora)
            std::cerr << "ERROR: nTriangleClasses == "
                << nTriangleClasses_ << " at end of search!" << std::endl;
        for (unsigned i = 0; i < nPentachora * 10; ++i) {
            if (triState_[i].parent != -1)
                std::cerr << "ERROR: triState[" << i << "].parent == "
                    << triState_[i].parent << " at end of search!"
                    << std::endl;
            if (triState_[i].rank != 0)
                std::cerr << "ERROR: triState[" << i << "].rank == "
                    << triState_[i].rank << " at end of search!" << std::endl;
            if (triState_[i].size != 1)
                std::cerr << "ERROR: triState[" << i << "].size == "
                    << triState_[i].size << " at end of search!" << std::endl;
            if (! triState_[i].bounded)
                std::cerr << "ERROR: triState[" << i << "].bounded == "
                    "false at end of search!" << std::endl;
            if (triState_[i].hadEqualRank)
                std::cerr << "ERROR: triState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nPentachora * 25 / 2; ++i)
            if (triStateChanged_[i] != -1)
                std::cerr << "ERROR: triStateChanged[" << i << "] == "
                    << triStateChanged_[i] << " at end of search!"
                    << std::endl;
    }
#endif

    use_(0, useArgs_);
}

void GluingPermSearcher<4>::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
}

GluingPermSearcher<4>* GluingPermSearcher<4>::readTaggedData(std::istream& in,
        GluingPermSearcher<4>::Use use, void* useArgs) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        return 0;

    GluingPermSearcher<4>* ans;
    if (c == GluingPermSearcher<4>::dataTag_)
        ans = new GluingPermSearcher<4>(in, use, useArgs);
    else
        return 0;

    if (ans->inputError()) {
        delete ans;
        return 0;
    }

    return ans;
}

void GluingPermSearcher<4>::dumpData(std::ostream& out) const {
    GluingPerms<4>::dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (finiteOnly_ ? 'f' : '.');
    out << (started_ ? 's' : '.');
    out << std::endl;

    int nPent = size();
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
        out << order_[i].simp << ' ' << order_[i].facet;
    }
    out << std::endl;

    // ---------- Tracking of edge / triangle equivalence classes ----------

    out << nEdgeClasses_ << std::endl;
    for (i = 0; i < 10 * nPent; ++i) {
        edgeState_[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 25 * nPent; ++i) {
        if (i)
            out << ' ';
        out << edgeStateChanged_[i];
    }
    out << std::endl;

    out << nTriangleClasses_ << std::endl;
    for (i = 0; i < 10 * nPent; ++i) {
        triState_[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 25 * nPent / 2; ++i) {
        if (i)
            out << ' ';
        out << triStateChanged_[i];
    }
    out << std::endl;
}

GluingPermSearcher<4>::GluingPermSearcher(std::istream& in,
        GluingPermSearcher<4>::Use use, void* useArgs) :
        GluingPerms<4>(in), autos_(0), autosNew_(false),
        use_(use), useArgs_(useArgs), orientation_(0),
        order_(0), orderSize_(0), orderElt_(0),
        nEdgeClasses_(0), edgeState_(0), edgeStateChanged_(0),
        nTriangleClasses_(0), triState_(0), triStateChanged_(0) {
    if (inputError_)
        return;

    // Recontruct the facet pairing automorphisms.
    const_cast<GluingPermSearcher<4>*>(this)->autos_ =
        new FacetPairing<4>::IsoList();
    pairing_->findAutomorphisms(const_cast<FacetPairing<4>::IsoList&>(
        *autos_));
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

    int nPent = pairing_->size();
    int p;

    orientation_ = new int[nPent];
    for (p = 0; p < nPent; ++p)
        in >> orientation_[p];

    order_ = new FacetSpec<4>[(nPent * 5) / 2];
    in >> orderElt_ >> orderSize_;
    for (p = 0; p < orderSize_; ++p) {
        in >> order_[p].simp >> order_[p].facet;
        if (order_[p].simp >= nPent || order_[p].simp < 0 ||
                order_[p].facet >= 5 || order_[p].facet < 0) {
            inputError_ = true; return;
        }
    }

    // Did we hit an unexpected EOF?
    if (in.eof()) {
        inputError_ = true; return;
    }

    // ---------- Tracking of edge / triangle equivalence classes ----------

    unsigned i;

    in >> nEdgeClasses_;
    if (nEdgeClasses_ > 10 * nPent) {
        inputError_ = true; return;
    }

    edgeState_ = new PentEdgeState[10 * nPent];
    for (i = 0; i < 10 * nPent; ++i)
        if (! edgeState_[i].readData(in, 10 * nPent)) {
            inputError_ = true; return;
        }

    edgeStateChanged_ = new int[25 * nPent];
    for (i = 0; i < 25 * nPent; ++i) {
        in >> edgeStateChanged_[i];
        if (edgeStateChanged_[i] < -1 ||
                 edgeStateChanged_[i] >= 10 * static_cast<int>(nPent)) {
            inputError_ = true; return;
        }
    }

    in >> nTriangleClasses_;
    if (nTriangleClasses_ > 10 * nPent) {
        inputError_ = true; return;
    }

    triState_ = new PentTriangleState[10 * nPent];
    for (i = 0; i < 10 * nPent; ++i)
        if (! triState_[i].readData(in, 10 * nPent)) {
            inputError_ = true; return;
        }

    triStateChanged_ = new int[25 * nPent / 2];
    for (i = 0; i < 25 * nPent / 2; ++i) {
        in >> triStateChanged_[i];
        if (triStateChanged_[i] < -1 ||
                 triStateChanged_[i] >= 10 * static_cast<int>(nPent)) {
            inputError_ = true; return;
        }
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

bool GluingPermSearcher<4>::isCanonical() const {
    FacetSpec<4> facet, facetDest, facetImage;
    int ordering;

    for (FacetPairing<4>::IsoList::const_iterator it = autos_->begin();
            it != autos_->end(); ++it) {
        // Compare the current set of gluing permutations with its
        // preimage under each facet pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (facet.setFirst(); facet.simp <
                static_cast<int>(pairing_->size()); facet++) {
            facetDest = pairing_->dest(facet);
            if (pairing_->isUnmatched(facet) || facetDest < facet)
                continue;

            facetImage = (**it)[facet];
            ordering = gluingPerm(facet).compareWith(
                (*it)->facetPerm(facetDest.simp).inverse()
                * gluingPerm(facetImage) * (*it)->facetPerm(facet.simp));
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

bool GluingPermSearcher<4>::badTriangleLink(const FacetSpec<4>& facet) const {
    // Run around all four triangles bounding the facet.
    FacetSpec<4> adj;
    unsigned pent;
    Perm<5> current;
    Perm<5> start(facet.facet, 4);
    bool started, incomplete;
    for (unsigned permIdx = 0; permIdx < 4; ++permIdx) {
        start = start * Perm<5>(1, 2, 3, 0, 4);

        // start maps (0,1,2,3) to the four vertices of facet, with
        // (0,1,2) mapped to the 2-dimensional triangle that we wish to examine.

        // Continue to push through a pentachoron and then across a
        // facet, until either we hit a boundary or we return to the
        // original facet.

        current = start;
        pent = facet.simp;

        started = false;
        incomplete = false;

        while ((! started) || (static_cast<int>(pent) != facet.simp) ||
                (start[3] != current[3]) || (start[4] != current[4])) {
            // Push through the current pentachoron.
            started = true;
            current = current * Perm<5>(3, 4);

            // Push across a facet.
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

            pent = adj.simp;
        }

        // Did we meet the original facet with a rotation or reflection?
        if ((! incomplete) && (start != current))
            return true;
    }

    // No bad triangle links were found.
    return false;
}

bool GluingPermSearcher<4>::mergeEdgeClasses() {
    // Merge all six edge pairs for the current facet.
    FacetSpec<4> facet = order_[orderElt_];
    FacetSpec<4> adj = (*pairing_)[facet];

    bool retVal = false;

    int v1, w1, v2, w2, v3, w3;
    int e, f;
    int eIdx, fIdx, tmpIdx, nextIdx;
    int orderIdx;
    int eRep, fRep;
    int eNext[2], fNext[2];
    char eTwistTriangle[2], fTwistTriangle[2];

    Perm<5> p = gluingPerm(facet);

    int tmpInvariant;
    char parentTwistEdge, hasTwistEdge;
    char parentTwistTriangle, hasTwistTriangle;
    char tmpTwistTriangle;

    v1 = facet.facet;
    w1 = p[v1];

    for (v2 = 0; v2 < 4; ++v2) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        for (v3 = v2 + 1; v3 < 5; ++v3) {
            if (v3 == v1)
                continue;

            w3 = p[v3];

            // Look at the edge opposite v1, v2 and v3.
            e = Triangle<4>::triangleNumber[v1][v2][v3];
            f = Triangle<4>::triangleNumber[w1][w2][w3];
            eIdx = e + 10 * facet.simp;
            fIdx = f + 10 * adj.simp;
            orderIdx = e + 10 * orderElt_;

            // We declare the natural orientation of an edge to be
            // smaller vertex to larger vertex.
            hasTwistEdge = (p[Edge<4>::edgeVertex[e][0]] >
                p[Edge<4>::edgeVertex[e][1]] ?  1 : 0);

            // Are the natural 012 representations of the two triangles
            // joined with reverse orientations?
            // Here we label triangles 012 by running through the
            // three vertices of the opposite pentachoron triangle in
            // ascending numerical order.
            tmpInvariant = 0;
            if (p[Triangle<4>::triangleVertex[e][0]] ==
                    Triangle<4>::triangleVertex[f][0])
                ++tmpInvariant;
            if (p[Triangle<4>::triangleVertex[e][1]] ==
                    Triangle<4>::triangleVertex[f][1])
                ++tmpInvariant;
            if (p[Triangle<4>::triangleVertex[e][2]] ==
                    Triangle<4>::triangleVertex[f][2])
                ++tmpInvariant;
            hasTwistTriangle = (tmpInvariant == 1 ? 0 : 1);

            parentTwistEdge = parentTwistTriangle = 0;
            for (eRep = eIdx; edgeState_[eRep].parent >= 0;
                    eRep = edgeState_[eRep].parent) {
                parentTwistEdge ^= edgeState_[eRep].twistUpEdge;
                parentTwistTriangle ^= edgeState_[eRep].twistUpTriangle;
            }
            for (fRep = fIdx; edgeState_[fRep].parent >= 0;
                    fRep = edgeState_[fRep].parent) {
                parentTwistEdge ^= edgeState_[fRep].twistUpEdge;
                parentTwistTriangle ^= edgeState_[fRep].twistUpTriangle;
            }

            if (eRep == fRep) {
                edgeState_[eRep].bdry -= 2;

                // Have we identified an edge with itself in reverse?
                if (hasTwistEdge ^ parentTwistEdge)
                    retVal = true;

                // Have we made the edge link non-orientable?
                if (hasTwistTriangle ^ parentTwistTriangle)
                    retVal = true;

                edgeStateChanged_[orderIdx] = -1;

                // Examine the cycles of boundary components.
                if (eIdx == fIdx) {
                    // Either we are folding together two adjacent edges of the
                    // edge link, or we are making the edge link non-orientable.

                    // The possible cases are:
                    //
                    // 1) hasTwistTriangle is true.  The edge link becomes
                    // non-orientable, but we should already have flagged
                    // this above.  Don't touch anything.
                    //
                    // 2) hasTwistTriangle is false, and
                    // edgeState_[eIdx].bdryEdges is 3.
                    // Here we are taking a stand-alone triangle and folding
                    // two of its edges together.  Nothing needs to change.
                    //
                    // 3) hasTwistTriangle is false, and
                    // edgeState_[eIdx].bdryEdges is 2.
                    // This means we are folding together two edges of a
                    // triangle whose third edge is already joined elsewhere.
                    // We deal with this as follows:
                    //
                    if ((! hasTwistTriangle) &&
                            edgeState_[eIdx].bdryEdges < 3) {
                        // Although bdryEdges is 2, we don't bother keeping
                        // a backup in bdryTwistOld[].  This is because
                        // bdryEdges jumps straight from 2 to 0, and the
                        // neighbours in bdryNext[] / bdryTwist[] never get
                        // overwritten.
                        if (edgeState_[eIdx].bdryNext[0] == eIdx) {
                            // We are closing off a single boundary of length
                            // two.  All good.
                        } else {
                            // Adjust each neighbour to point to the other.
                            edgeBdryJoin(edgeState_[eIdx].bdryNext[0],
                                1 ^ edgeState_[eIdx].bdryTwist[0],
                                edgeState_[eIdx].bdryNext[1],
                                edgeState_[eIdx].bdryTwist[1] ^
                                    edgeState_[eIdx].bdryTwist[0]);
                        }
                    }

                    edgeState_[eIdx].bdryEdges -= 2;
                } else {
                    // We are joining two distinct pentachoron edges that
                    // already contribute to the same edge link.
                    if (edgeState_[eIdx].bdryEdges == 2)
                        edgeBdryBackup(eIdx);
                    if (edgeState_[fIdx].bdryEdges == 2)
                        edgeBdryBackup(fIdx);

                    if (edgeBdryLength1(eIdx) && edgeBdryLength1(fIdx)) {
                        // We are joining together two boundaries of length one.
                        // Do nothing and mark the non-trivial genus.
                        // std::cerr << "NON-SPHERE: 1 >-< 1" << std::endl;
                        retVal = true;
                    } else if (edgeBdryLength2(eIdx, fIdx)) {
                        // We are closing off a single boundary of length two.
                        // All good.
                    } else {
                        edgeBdryNext(eIdx, facet.simp, e, facet.facet,
                            eNext, eTwistTriangle);
                        edgeBdryNext(fIdx, adj.simp, f, adj.facet,
                            fNext, fTwistTriangle);

                        if (eNext[0] == fIdx &&
                                fNext[1 ^ eTwistTriangle[0]] == eIdx) {
                            // We are joining two adjacent edges of the
                            // edge link.  Simply eliminate them.
                            edgeBdryJoin(eNext[1], 0 ^ eTwistTriangle[1],
                                fNext[0 ^ eTwistTriangle[0]],
                                (eTwistTriangle[0] ^
                                    fTwistTriangle[0 ^ eTwistTriangle[0]]) ^
                                    eTwistTriangle[1]);
                        } else if (eNext[1] == fIdx &&
                                fNext[0 ^ eTwistTriangle[1]] == eIdx) {
                            // Again, joining two adjacent edges of the
                            // edge link.
                            edgeBdryJoin(eNext[0], 1 ^ eTwistTriangle[0],
                                fNext[1 ^ eTwistTriangle[1]],
                                (eTwistTriangle[1] ^
                                    fTwistTriangle[1 ^ eTwistTriangle[1]]) ^
                                    eTwistTriangle[0]);
                        } else {
                            // See if we are joining two different boundary
                            // cycles together; if so, we have created
                            // non-trivial genus in the edge link.
                            tmpIdx = edgeState_[eIdx].bdryNext[0];
                            tmpTwistTriangle = edgeState_[eIdx].bdryTwist[0];
                            while (tmpIdx != eIdx && tmpIdx != fIdx) {
                                nextIdx = edgeState_[tmpIdx].
                                    bdryNext[0 ^ tmpTwistTriangle];
                                tmpTwistTriangle ^= edgeState_[tmpIdx].
                                    bdryTwist[0 ^ tmpTwistTriangle];
                                tmpIdx = nextIdx;
                            }

                            if (tmpIdx == eIdx) {
                                // Different boundary cycles.
                                // Don't touch anything; just flag a
                                // high genus error.
                                // std::cerr << "NON-SPHERE: (X)" << std::endl;
                                retVal = true;
                            } else {
                                // Same boundary cycle.
                                edgeBdryJoin(eNext[0], 1 ^ eTwistTriangle[0],
                                    fNext[1 ^ hasTwistTriangle],
                                    eTwistTriangle[0] ^ (hasTwistTriangle ^
                                        fTwistTriangle[1 ^ hasTwistTriangle]));
                                edgeBdryJoin(eNext[1], 0 ^ eTwistTriangle[1],
                                    fNext[0 ^ hasTwistTriangle],
                                    eTwistTriangle[1] ^ (hasTwistTriangle ^
                                        fTwistTriangle[0 ^ hasTwistTriangle]));
                            }
                        }
                    }

                    edgeState_[eIdx].bdryEdges--;
                    edgeState_[fIdx].bdryEdges--;
                }
            } else {
                // We are joining two distinct edges together and merging
                // their edge links.
                if (edgeState_[eRep].rank < edgeState_[fRep].rank) {
                    // Join eRep beneath fRep.
                    edgeState_[eRep].parent = fRep;
                    edgeState_[eRep].twistUpEdge =
                        hasTwistEdge ^ parentTwistEdge;
                    edgeState_[eRep].twistUpTriangle =
                        hasTwistTriangle ^ parentTwistTriangle;
                    edgeState_[fRep].bdry = edgeState_[fRep].bdry +
                        edgeState_[eRep].bdry - 2;

                    edgeStateChanged_[orderIdx] = eRep;
                } else {
                    // Join fRep beneath eRep.
                    edgeState_[fRep].parent = eRep;
                    edgeState_[fRep].twistUpEdge =
                        hasTwistEdge ^ parentTwistEdge;
                    edgeState_[fRep].twistUpTriangle =
                        hasTwistTriangle ^ parentTwistTriangle;
                    if (edgeState_[eRep].rank == edgeState_[fRep].rank) {
                        edgeState_[eRep].rank++;
                        edgeState_[fRep].hadEqualRank = true;
                    }
                    edgeState_[eRep].bdry = edgeState_[eRep].bdry +
                        edgeState_[fRep].bdry - 2;

                    edgeStateChanged_[orderIdx] = fRep;
                }
                --nEdgeClasses_;

                // Adjust the cycles of boundary components.
                if (edgeState_[eIdx].bdryEdges == 2)
                    edgeBdryBackup(eIdx);
                if (edgeState_[fIdx].bdryEdges == 2)
                    edgeBdryBackup(fIdx);

                if (edgeBdryLength1(eIdx)) {
                    if (edgeBdryLength1(fIdx)) {
                        // Both eIdx and fIdx form entire boundary components
                        // of length one; these are joined together and the
                        // edge link is closed off.
                        // No changes to make for the boundary cycles.
                    } else {
                        // Here eIdx forms a boundary component of length one,
                        // and fIdx does not.  Ignore eIdx, and simply excise
                        // the relevant edge from fIdx.
                        // There is nothing to do here unless fIdx only has one
                        // boundary edge remaining (in which case we know it
                        // joins to some different pentachoron edge).
                        if (edgeState_[fIdx].bdryEdges == 1) {
                            fNext[0] = edgeState_[fIdx].bdryNext[0];
                            fNext[1] = edgeState_[fIdx].bdryNext[1];
                            fTwistTriangle[0] = edgeState_[fIdx].bdryTwist[0];
                            fTwistTriangle[1] = edgeState_[fIdx].bdryTwist[1];

                            edgeBdryJoin(fNext[0], 1 ^ fTwistTriangle[0],
                                fNext[1],
                                fTwistTriangle[0] ^ fTwistTriangle[1]);
                        }
                    }
                } else if (edgeBdryLength1(fIdx)) {
                    // As above, but with the two edges the other way around.
                    if (edgeState_[eIdx].bdryEdges == 1) {
                        eNext[0] = edgeState_[eIdx].bdryNext[0];
                        eNext[1] = edgeState_[eIdx].bdryNext[1];
                        eTwistTriangle[0] = edgeState_[eIdx].bdryTwist[0];
                        eTwistTriangle[1] = edgeState_[eIdx].bdryTwist[1];

                        edgeBdryJoin(eNext[0], 1 ^ eTwistTriangle[0], eNext[1],
                            eTwistTriangle[0] ^ eTwistTriangle[1]);
                    }
                } else {
                    // Each edge belongs to a boundary component of length
                    // at least two.  Merge the components together.
                    edgeBdryNext(eIdx, facet.simp, e, facet.facet, eNext,
                        eTwistTriangle);
                    edgeBdryNext(fIdx, adj.simp, f, adj.facet, fNext,
                        fTwistTriangle);

                    edgeBdryJoin(eNext[0], 1 ^ eTwistTriangle[0],
                        fNext[1 ^ hasTwistTriangle],
                        eTwistTriangle[0] ^ (hasTwistTriangle ^
                            fTwistTriangle[1 ^ hasTwistTriangle]));
                    edgeBdryJoin(eNext[1], 0 ^ eTwistTriangle[1],
                        fNext[0 ^ hasTwistTriangle],
                        eTwistTriangle[1] ^ (hasTwistTriangle ^
                            fTwistTriangle[0 ^ hasTwistTriangle]));
                }

                edgeState_[eIdx].bdryEdges--;
                edgeState_[fIdx].bdryEdges--;
            }
        }
    }

    return retVal;
}

void GluingPermSearcher<4>::splitEdgeClasses() {
    FacetSpec<4> facet = order_[orderElt_];
    FacetSpec<4> adj = (*pairing_)[facet];

    int v1, v2, v3, w1, w2, w3;
    int e, f;
    int eIdx, fIdx, orderIdx;
    int rep, subRep;

    Perm<5> p = gluingPerm(facet);

    v1 = facet.facet;
    w1 = p[v1];

    // TODO: UFIND
    // Do everything in reverse.  This includes the nested loops over vertices.
    for (v2 = 3; v2 >= 0; --v2) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        for (v3 = 4; v3 > v2; --v3) {
            if (v3 == v1)
                continue;

            w3 = p[v3];

            // Look at the edge opposite v1, v2 and v3.
            e = Triangle<4>::triangleNumber[v1][v2][v3];
            f = Triangle<4>::triangleNumber[w1][w2][w3];
            eIdx = e + 10 * facet.simp;
            fIdx = f + 10 * adj.simp;
            orderIdx = e + 10 * orderElt_;

            if (edgeStateChanged_[orderIdx] < 0) {
                for (rep = eIdx; edgeState_[rep].parent >= 0;
                        rep = edgeState_[rep].parent)
                    ;
                edgeState_[rep].bdry += 2;
            } else {
                // Separate a two trees that had been grafted together.
                subRep = edgeStateChanged_[orderIdx];
                rep = edgeState_[subRep].parent;

                edgeState_[subRep].parent = -1;
                if (edgeState_[subRep].hadEqualRank) {
                    edgeState_[subRep].hadEqualRank = false;
                    edgeState_[rep].rank--;
                }

                edgeState_[rep].bdry = edgeState_[rep].bdry + 2 -
                    edgeState_[subRep].bdry;

                edgeStateChanged_[orderIdx] = -1;
                ++nEdgeClasses_;
            }

            // Restore cycles of boundary components.
            if (eIdx == fIdx) {
                edgeState_[eIdx].bdryEdges += 2;

                // Adjust neighbours to point back to eIdx if required.
                if (edgeState_[eIdx].bdryEdges == 2)
                    edgeBdryFixAdj(eIdx);
            } else {
                edgeState_[fIdx].bdryEdges++;
                edgeState_[eIdx].bdryEdges++;

                switch (edgeState_[fIdx].bdryEdges) {
                    case 3: edgeState_[fIdx].bdryNext[0] =
                                edgeState_[fIdx].bdryNext[1] = fIdx;
                            edgeState_[fIdx].bdryTwist[0] =
                                edgeState_[fIdx].bdryTwist[1] = 0;
                            break;

                    case 2: edgeBdryRestore(fIdx);
                            // Fall through to the next case, so we can
                            // adjust the neighbours.

                    case 1: // Nothing was changed for fIdx during the merge,
                            // so there is nothing there to restore.

                            // Adjust neighbours to point back to fIdx.
                            edgeBdryFixAdj(fIdx);
                }

                switch (edgeState_[eIdx].bdryEdges) {
                    case 3: edgeState_[eIdx].bdryNext[0] =
                                edgeState_[eIdx].bdryNext[1] = eIdx;
                            edgeState_[eIdx].bdryTwist[0] =
                                edgeState_[eIdx].bdryTwist[1] = 0;
                            break;

                    case 2: edgeBdryRestore(eIdx);
                            // Fall through to the next case, so we can
                            // adjust the neighbours.

                    case 1: // Nothing was changed for eIdx during the merge,
                            // so there is nothing there to restore.

                            // Adjust neighbours to point back to eIdx.
                            edgeBdryFixAdj(eIdx);
                }
            }
        }
    }
}

bool GluingPermSearcher<4>::mergeTriangleClasses() {
    FacetSpec<4> facet = order_[orderElt_];
    FacetSpec<4> adj = (*pairing_)[facet];

    bool retVal = false;

    Perm<5> p = gluingPerm(facet);
    int v1, w1, v2, w2;
    int e, f;
    int orderIdx;
    int eRep, fRep;

    v1 = facet.facet;
    w1 = p[v1];

    Perm<3> directTwist;
    for (v2 = 0; v2 < 5; ++v2) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        // Look at the triangle opposite edge v1-v2.
        e = Edge<4>::edgeNumber[v1][v2];
        f = Edge<4>::edgeNumber[w1][w2];

        orderIdx = v2 + 5 * orderElt_;

        // Vertices of a triangle are labelled in order from smallest to
        // largest.
        if (p[Triangle<4>::triangleVertex[e][0]] ==
                Triangle<4>::triangleVertex[f][0]) {
            if (p[Triangle<4>::triangleVertex[e][1]] ==
                    Triangle<4>::triangleVertex[f][1])
                directTwist.setPermCode(Perm<3>::code012);
            else
                directTwist.setPermCode(Perm<3>::code021);
        } else if (p[Triangle<4>::triangleVertex[e][0]] ==
                Triangle<4>::triangleVertex[f][1]) {
            if (p[Triangle<4>::triangleVertex[e][1]] ==
                    Triangle<4>::triangleVertex[f][0])
                directTwist.setPermCode(Perm<3>::code102);
            else
                directTwist.setPermCode(Perm<3>::code120);
        } else {
            if (p[Triangle<4>::triangleVertex[e][1]] ==
                    Triangle<4>::triangleVertex[f][0])
                directTwist.setPermCode(Perm<3>::code201);
            else
                directTwist.setPermCode(Perm<3>::code210);
        }

        Perm<3> eTwist, fTwist; /* Initialise to identity permutations. */
        eRep = findTriangleClass(e + 10 * facet.simp, eTwist);
        fRep = findTriangleClass(f + 10 * adj.simp, fTwist);

        if (eRep == fRep) {
            triState_[eRep].bounded = false;

            if (eTwist != fTwist * directTwist)
                retVal = true;

            triStateChanged_[orderIdx] = -1;
        } else {
            if (triState_[eRep].rank < triState_[fRep].rank) {
                // Join eRep beneath fRep.
                triState_[eRep].parent = fRep;
                triState_[eRep].twistUp =
                    fTwist * directTwist * eTwist.inverse();
                triState_[fRep].size += triState_[eRep].size;

                triStateChanged_[orderIdx] = eRep;
            } else {
                // Join fRep beneath eRep.
                triState_[fRep].parent = eRep;
                triState_[fRep].twistUp =
                    eTwist * directTwist.inverse() * fTwist.inverse();
                if (triState_[eRep].rank == triState_[fRep].rank) {
                    triState_[eRep].rank++;
                    triState_[fRep].hadEqualRank = true;
                }
                triState_[eRep].size += triState_[fRep].size;

                triStateChanged_[orderIdx] = fRep;
            }
            --nTriangleClasses_;
        }
    }

    return retVal;
}

void GluingPermSearcher<4>::splitTriangleClasses() {
    FacetSpec<4> facet = order_[orderElt_];

    int v1, v2;
    int f;
    int fIdx, orderIdx;
    int rep, subRep;

    v1 = facet.facet;

    for (v2 = 4; v2 >= 0; --v2) {
        if (v2 == v1)
            continue;

        // Look at the triangle opposite edge v1-v2.
        f = Edge<4>::edgeNumber[v1][v2];

        fIdx = f + 10 * facet.simp;
        orderIdx = v2 + 5 * orderElt_;

        if (triStateChanged_[orderIdx] < 0)
            triState_[findTriangleClass(fIdx)].bounded = true;
        else {
            subRep = triStateChanged_[orderIdx];
            rep = triState_[subRep].parent;

            triState_[subRep].parent = -1;
            if (triState_[subRep].hadEqualRank) {
                triState_[subRep].hadEqualRank = false;
                triState_[rep].rank--;
            }

            triState_[rep].size -= triState_[subRep].size;

            triStateChanged_[orderIdx] = -1;
            ++nTriangleClasses_;
        }
    }
}

void GluingPermSearcher<4>::edgeBdryNext(int edgeID, int pent, int edge,
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
                    // to either the pentachoron triangle we are currently
                    // working with or its adjacent partner.
                    int ghostTriangle = (bdryFacet == order_[orderElt_].facet ?
                        (*pairing_)[order_[orderElt_]].facet :
                        order_[orderElt_].facet);
                    if (edgeLinkNextFacet[edge][bdryFacet] == ghostTriangle) {
                        next[0] = edgeState_[edgeID].bdryNext[0];
                        twist[0] = edgeState_[edgeID].bdryTwist[0];
                        next[1] = edgeID;
                        twist[1] = 0;
                    } else {
                        // Sanity check.
                        if (edgeLinkPrevFacet[edge][bdryFacet] != ghostTriangle)
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

void GluingPermSearcher<4>::edgeBdryConsistencyCheck() {
    int adj, id, end;
    for (id = 0; id < static_cast<int>(size()) * 5; ++id)
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

void GluingPermSearcher<4>::edgeBdryDump(std::ostream& out) {
    for (unsigned id = 0; id < size() * 5; ++id) {
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

