
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
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim4.h"

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
        size_t nStates) {
    in >> parent >> rank >> bdry;

    // The twist fields are chars, but we need to read them as ints.
    int twistEdge;
    in >> twistEdge;
    twistUpEdge = static_cast<char>(twistEdge);

    int twistTriangle;
    in >> twistTriangle;
    twistUpTriangle = static_cast<char>(twistTriangle);

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    // More chars to ints coming.
    int bVal;

    in >> bVal; bdryEdges = static_cast<uint8_t>(bVal);
    in >> bdryNext[0] >> bdryNext[1];
    in >> bVal; bdryTwist[0] = static_cast<char>(bVal);
    in >> bVal; bdryTwist[1] = static_cast<char>(bVal);
    in >> bdryNextOld[0] >> bdryNextOld[1];
    in >> bVal; bdryTwistOld[0] = static_cast<char>(bVal);
    in >> bVal; bdryTwistOld[1] = static_cast<char>(bVal);

    if (parent < -1 || parent >= static_cast<ssize_t>(nStates))
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
    // Since bdryNext[...] is size_t, it is always non-negative.
    if (bdryNext[0] >= nStates || bdryNext[1] >= nStates)
        return false;
    // Since bdryNextOld[...] is ssize_t, we must be careful with
    // signed/unsigned comparisons.
    if (bdryNextOld[0] < -1 || bdryNextOld[0] >= static_cast<ssize_t>(nStates))
        return false;
    if (bdryNextOld[1] < -1 || bdryNextOld[1] >= static_cast<ssize_t>(nStates))
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
        size_t nStates) {
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

    if (parent < -1 || parent >= static_cast<ssize_t>(nStates))
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
        FacetPairing<4> pairing, FacetPairing<4>::IsoList autos,
        bool orientableOnly, bool finiteOnly) :
        perms_(std::move(pairing)), autos_(std::move(autos)),
        // pairing and autos are no longer usable
        orientableOnly_(orientableOnly), finiteOnly_(finiteOnly),
        started_(false), orientation_(new int[perms_.size()]) {
    // Initialise arrays.
    size_t nPent = perms_.size();

    std::fill(orientation_, orientation_ + nPent, 0);

    // Just fill the order_[] array in a default left-to-right fashion.
    // Subclasses can rearrange things if they choose.
    order_ = new FacetSpec<4>[(nPent * 5) / 2];
    orderElt_ = orderSize_ = 0;

    FacetSpec<4> facet;
    for (facet.setFirst(); ! facet.isPastEnd(nPent, true); facet++)
        if (! perms_.pairing().isUnmatched(facet))
            if (facet < perms_.pairing().dest(facet))
                order_[orderSize_++] = facet;

    // ---------- Tracking of edge / triangle equivalence classes ----------

    nEdgeClasses_ = nPent * 10;
    edgeState_ = new PentEdgeState[nPent * 10];
    // The length of triStateChanged_[] needs to be at least 10 * orderSize_.
    // Just be conservative here -- we know that orderSize_ <= 5 * nPent / 2.
    edgeStateChanged_ = new ssize_t[nPent * 25];
    std::fill(edgeStateChanged_, edgeStateChanged_ + nPent * 25, -1);
    for (size_t i = 0; i < nPent * 10; ++i) {
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
    triStateChanged_ = new ssize_t[25 * nPent / 2];
    std::fill(triStateChanged_, triStateChanged_ + (25 * nPent / 2), -1);
}

GluingPermSearcher<4>::~GluingPermSearcher() {
    delete[] triState_;
    delete[] triStateChanged_;
    delete[] edgeState_;
    delete[] edgeStateChanged_;

    delete[] orientation_;
    delete[] order_;
}

void GluingPermSearcher<4>::searchImpl(long maxDepth, ActionWrapper&& action_) {
    // In this generation algorithm, each orientation is simply +/-1.

    size_t nPentachora = perms_.size();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nPentachora * 5 + 1;
    }

    if (! started_) {
        // Search initialisation.
        started_ = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 ||
                perms_.pairing().dest(0, 0).isBoundary(nPentachora)) {
            action_(perms_);
            return;
        }

        orderElt_ = 0;
        orientation_[0] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt_ == static_cast<ssize_t>(orderSize_)) {
        if (isCanonical())
            action_(perms_);
        return;
    }

    // ---------- Selecting the individual gluing permutations ----------

    ssize_t minOrder = orderElt_;
    ssize_t maxOrder = orderElt_ + maxDepth;

    while (orderElt_ >= minOrder) {
        FacetSpec<4> facet = order_[orderElt_];
        FacetSpec<4> adj = perms_.pairing()[facet];

        // TODO: Check for cancellation.

        // Move to the next permutation.

        // Be sure to preserve the orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            perms_.permIndex(facet)++;
        else
            perms_.permIndex(facet) += 2;

        // Are we out of ideas for this facet?
        if (perms_.permIndex(facet) >= 24) {
            // Yep.  Head back down to the previous facet.
            perms_.permIndex(facet) = -1;
            perms_.permIndex(adj) = -1;
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
        perms_.permIndex(adj) =
            Perm<4>::S4[perms_.permIndex(facet)].inverse().S4Index();

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
            if ((perms_.permIndex(facet) + (facet.facet == 4 ? 0 : 1) +
                    (adj.facet == 4 ? 0 : 1)) % 2 == 0)
                orientation_[adj.simp] = -orientation_[facet.simp];
            else
                orientation_[adj.simp] = orientation_[facet.simp];
        }

        // Move on to the next facet.
        orderElt_++;

        // If we're at the end, try the solution and step back.
        if (orderElt_ == static_cast<ssize_t>(orderSize_)) {
            // We in fact have an entire triangulation.
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            if (isCanonical())
                action_(perms_);

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
            if (orientableOnly_ && perms_.pairing().dest(facet).facet > 0) {
                // permIndex(facet) will be set to -1 or -2 as appropriate.
                adj = perms_.pairing()[facet];
                if (orientation_[facet.simp] == orientation_[adj.simp])
                    perms_.permIndex(facet) = 1;
                else
                    perms_.permIndex(facet) = 0;

                if ((facet.facet == 4 ? 0 : 1) + (adj.facet == 4 ? 0 : 1) == 1)
                    perms_.permIndex(facet) = (perms_.permIndex(facet) + 1) % 2;

                perms_.permIndex(facet) -= 2;
            }

            if (orderElt_ == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                action_(perms_);

                // Back to the previous facet.
                perms_.permIndex(facet) = -1;
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
        for (size_t i = 0; i < nPentachora * 10; ++i) {
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
        for (size_t i = 0; i < nPentachora * 25; ++i)
            if (edgeStateChanged_[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged_[i] << " at end of search!"
                    << std::endl;

        // And our triangle classes had better be 10n standalone triangles.
        if (nTriangleClasses_ != 10 * nPentachora)
            std::cerr << "ERROR: nTriangleClasses == "
                << nTriangleClasses_ << " at end of search!" << std::endl;
        for (size_t i = 0; i < nPentachora * 10; ++i) {
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
        for (size_t i = 0; i < nPentachora * 25 / 2; ++i)
            if (triStateChanged_[i] != -1)
                std::cerr << "ERROR: triStateChanged[" << i << "] == "
                    << triStateChanged_[i] << " at end of search!"
                    << std::endl;
    }
#endif
}

void GluingPermSearcher<4>::dumpData(std::ostream& out) const {
    perms_.dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (finiteOnly_ ? 'f' : '.');
    out << (started_ ? 's' : '.');
    out << std::endl;

    size_t nPent = perms_.size();

    for (size_t i = 0; i < nPent; ++i) {
        if (i)
            out << ' ';
        out << orientation_[i];
    }
    out << std::endl;

    out << orderElt_ << ' ' << orderSize_ << std::endl;
    for (size_t i = 0; i < orderSize_; ++i) {
        if (i)
            out << ' ';
        out << order_[i].simp << ' ' << order_[i].facet;
    }
    out << std::endl;

    // ---------- Tracking of edge / triangle equivalence classes ----------

    out << nEdgeClasses_ << std::endl;
    for (size_t i = 0; i < 10 * nPent; ++i) {
        edgeState_[i].dumpData(out);
        out << std::endl;
    }
    for (size_t i = 0; i < 25 * nPent; ++i) {
        if (i)
            out << ' ';
        out << edgeStateChanged_[i];
    }
    out << std::endl;

    out << nTriangleClasses_ << std::endl;
    for (size_t i = 0; i < 10 * nPent; ++i) {
        triState_[i].dumpData(out);
        out << std::endl;
    }
    for (size_t i = 0; i < 25 * nPent / 2; ++i) {
        if (i)
            out << ' ';
        out << triStateChanged_[i];
    }
    out << std::endl;
}

void GluingPermSearcher<4>::writeTextShort(std::ostream& out) const {
    if (started_)
        out << "Running search";
    else
        out << "New search";

    if (orientableOnly_)
        out << ", orientable only";
    if (finiteOnly_)
        out << ", finite only";

    out << ": stage " << orderElt_ << ", order:";
    for (size_t i = 0; i < orderSize_; ++i)
        out << ' ' << order_[i].simp << ':' << order_[i].facet;
}

GluingPermSearcher<4>::GluingPermSearcher(std::istream& in) :
        perms_(in), autos_(perms_.pairing().findAutomorphisms()),
        orientation_(nullptr), order_(nullptr), orderSize_(0), orderElt_(0),
        nEdgeClasses_(0), edgeState_(nullptr), edgeStateChanged_(nullptr),
        nTriangleClasses_(0), triState_(nullptr), triStateChanged_(nullptr) {
    // Keep reading.
    char c;

    in >> c;
    if (c == 'o')
        orientableOnly_ = true;
    else if (c == '.')
        orientableOnly_ = false;
    else
        throw InvalidInput("Invalid orientability tag "
            "while attempting to read GluingPermSearcher<4>");

    in >> c;
    if (c == 'f')
        finiteOnly_ = true;
    else if (c == '.')
        finiteOnly_ = false;
    else
        throw InvalidInput("Invalid finiteness tag "
            "while attempting to read GluingPermSearcher<4>");

    in >> c;
    if (c == 's')
        started_ = true;
    else if (c == '.')
        started_ = false;
    else
        throw InvalidInput("Invalid started tag "
            "while attempting to read GluingPermSearcher<4>");

    size_t nPent = perms_.size();

    orientation_ = new int[nPent];
    for (size_t p = 0; p < nPent; ++p)
        in >> orientation_[p];

    order_ = new FacetSpec<4>[(nPent * 5) / 2];
    in >> orderElt_ >> orderSize_;
    for (size_t p = 0; p < orderSize_; ++p) {
        in >> order_[p].simp >> order_[p].facet;
        if (order_[p].simp >= static_cast<ssize_t>(nPent) ||
                order_[p].simp < 0 ||
                order_[p].facet >= 5 || order_[p].facet < 0)
            throw InvalidInput("Facet gluing out of range "
                "while attempting to read GluingPermSearcher<4>");
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        throw InvalidInput("Unexpected end of input stream "
            "while attempting to read GluingPermSearcher<4>");

    // ---------- Tracking of edge / triangle equivalence classes ----------

    in >> nEdgeClasses_;
    if (nEdgeClasses_ > 10 * nPent)
        throw InvalidInput("Edge classes out of range "
            "while attempting to read GluingPermSearcher<4>");

    edgeState_ = new PentEdgeState[10 * nPent];
    for (size_t i = 0; i < 10 * nPent; ++i)
        if (! edgeState_[i].readData(in, 10 * nPent))
            throw InvalidInput("Invalid edge state "
                "while attempting to read GluingPermSearcher<4>");

    edgeStateChanged_ = new ssize_t[25 * nPent];
    for (size_t i = 0; i < 25 * nPent; ++i) {
        in >> edgeStateChanged_[i];
        if (edgeStateChanged_[i] < -1 ||
                 edgeStateChanged_[i] >= 10 * static_cast<ssize_t>(nPent))
            throw InvalidInput("Invalid edge state changed "
                "while attempting to read GluingPermSearcher<4>");
    }

    in >> nTriangleClasses_;
    if (nTriangleClasses_ > 10 * nPent)
        throw InvalidInput("Triangle classes out of range "
            "while attempting to read GluingPermSearcher<4>");

    triState_ = new PentTriangleState[10 * nPent];
    for (size_t i = 0; i < 10 * nPent; ++i)
        if (! triState_[i].readData(in, 10 * nPent))
            throw InvalidInput("Invalid triangle state "
                "while attempting to read GluingPermSearcher<4>");

    triStateChanged_ = new ssize_t[25 * nPent / 2];
    for (size_t i = 0; i < 25 * nPent / 2; ++i) {
        in >> triStateChanged_[i];
        if (triStateChanged_[i] < -1 ||
                 triStateChanged_[i] >= 10 * static_cast<ssize_t>(nPent))
            throw InvalidInput("Invalid triangle state changed "
                "while attempting to read GluingPermSearcher<4>");
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        throw InvalidInput("Unexpected end of input stream "
            "while attempting to read GluingPermSearcher<4>");
}

bool GluingPermSearcher<4>::isCanonical() const {
    FacetSpec<4> facet, facetDest, facetImage;
    int ordering;

    for (const auto& iso : autos_) {
        // Compare the current set of gluing permutations with its
        // preimage under each facet pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (facet.setFirst(); facet.simp < static_cast<ssize_t>(perms_.size());
                ++facet) {
            facetDest = perms_.pairing().dest(facet);
            if (perms_.pairing().isUnmatched(facet) || facetDest < facet)
                continue;

            facetImage = iso[facet];
            ordering = perms_.perm(facet).compareWith(
                iso.facetPerm(facetDest.simp).inverse()
                * perms_.perm(facetImage) * iso.facetPerm(facet.simp));
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
    Perm<5> start(facet.facet, 4);
    for (int permIdx = 0; permIdx < 4; ++permIdx) {
        start = start * Perm<5>(1, 2, 3, 0, 4);

        // start maps (0,1,2,3) to the four vertices of facet, with
        // (0,1,2) mapped to the 2-dimensional triangle that we wish to examine.

        // Continue to push through a pentachoron and then across a
        // facet, until either we hit a boundary or we return to the
        // original facet.

        Perm<5> current = start;
        ssize_t pent = facet.simp;

        bool started = false;
        bool incomplete = false;

        while ((! started) || (pent != facet.simp) ||
                (start[3] != current[3]) || (start[4] != current[4])) {
            // Push through the current pentachoron.
            started = true;
            current = current * Perm<5>(3, 4);

            // Push across a facet.
            if (perms_.pairing().isUnmatched(pent, current[4])) {
                incomplete = true;
                break;
            }
            FacetSpec<4> adj = perms_.pairing().dest(pent, current[4]);

            if (perms_.permIndex(pent, current[4]) >= 0) {
                current = perms_.perm(pent, current[4]) * current;
            } else if (perms_.permIndex(adj) >= 0) {
                current = perms_.perm(adj).inverse() * current;
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
    FacetSpec<4> adj = perms_.pairing()[facet];

    bool retVal = false;

    int v1, w1, v2, w2, v3, w3;
    int e, f;
    ssize_t eRep, fRep;
    size_t eNext[2], fNext[2];
    char eTwistTriangle[2], fTwistTriangle[2];

    Perm<5> p = perms_.perm(facet);

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
            size_t eIdx = e + 10 * facet.simp;
            size_t fIdx = f + 10 * adj.simp;
            size_t orderIdx = e + 10 * orderElt_;

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
                            size_t tmpIdx = edgeState_[eIdx].bdryNext[0];
                            tmpTwistTriangle = edgeState_[eIdx].bdryTwist[0];
                            while (tmpIdx != eIdx && tmpIdx != fIdx) {
                                size_t nextIdx = edgeState_[tmpIdx].
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
    FacetSpec<4> adj = perms_.pairing()[facet];

    int v1, v2, v3, w1, w2, w3;
    int e, f;

    Perm<5> p = perms_.perm(facet);

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
            size_t eIdx = e + 10 * facet.simp;
            size_t fIdx = f + 10 * adj.simp;
            size_t orderIdx = e + 10 * orderElt_;

            if (edgeStateChanged_[orderIdx] < 0) {
                size_t rep;
                for (rep = eIdx; edgeState_[rep].parent >= 0;
                        rep = edgeState_[rep].parent)
                    ;
                edgeState_[rep].bdry += 2;
            } else {
                // Separate a two trees that had been grafted together.
                size_t subRep = edgeStateChanged_[orderIdx];
                size_t rep = edgeState_[subRep].parent;

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
    FacetSpec<4> adj = perms_.pairing()[facet];

    bool retVal = false;

    Perm<5> p = perms_.perm(facet);
    int v1, w1, v2, w2;
    int e, f;

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

        size_t orderIdx = v2 + 5 * orderElt_;

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
        size_t eRep = findTriangleClass(e + 10 * facet.simp, eTwist);
        size_t fRep = findTriangleClass(f + 10 * adj.simp, fTwist);

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

    int v1 = facet.facet;

    for (int v2 = 4; v2 >= 0; --v2) {
        if (v2 == v1)
            continue;

        // Look at the triangle opposite edge v1-v2.
        int f = Edge<4>::edgeNumber[v1][v2];

        size_t fIdx = f + 10 * facet.simp;
        size_t orderIdx = v2 + 5 * orderElt_;

        if (triStateChanged_[orderIdx] < 0)
            triState_[findTriangleClass(fIdx)].bounded = true;
        else {
            size_t subRep = triStateChanged_[orderIdx];
            size_t rep = triState_[subRep].parent;

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

void GluingPermSearcher<4>::edgeBdryNext(size_t edgeID, size_t pent, int edge,
        int bdryFacet, size_t next[2], char twist[2]) {
    switch (edgeState_[edgeID].bdryEdges) {
        case 3: next[0] = next[1] = edgeID;
                twist[0] = twist[1] = 0;
                break;
        case 2: if (perms_.permIndex(pent,
                        edgeLinkNextFacet[edge][bdryFacet]) < 0) {
                    next[0] = edgeState_[edgeID].bdryNext[0];
                    twist[0] = edgeState_[edgeID].bdryTwist[0];
                    next[1] = edgeID;
                    twist[1] = 0;
                } else if (perms_.permIndex(pent,
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
                        perms_.pairing()[order_[orderElt_]].facet :
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
    for (size_t id = 0; id < perms_.size() * 5; ++id)
        if (edgeState_[id].bdryEdges > 0)
            for (int end = 0; end < 2; ++end) {
                size_t adj = edgeState_[id].bdryNext[end];
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
    for (size_t id = 0; id < perms_.size() * 5; ++id) {
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

