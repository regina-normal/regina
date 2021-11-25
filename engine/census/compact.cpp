
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

#include "census/gluingpermsearcher3.h"
#include "triangulation/dim3.h"
#include "triangulation/facepair.h"

namespace regina {

const int CompactSearcher::vertexLinkNextFace[4][4] = {
    { -1, 2, 3, 1},
    { 3, -1, 0, 2},
    { 1, 3, -1, 0},
    { 1, 2, 0, -1}
};

const int CompactSearcher::vertexLinkPrevFace[4][4] = {
    { -1, 3, 1, 2},
    { 2, -1, 3, 0},
    { 3, 0, -1, 1},
    { 2, 0, 1, -1}
};

void CompactSearcher::TetVertexState::dumpData(std::ostream& out)
        const {
    // Be careful with twistUp, which is a char but which should be
    // written as an int.
    out << parent << ' ' << rank << ' ' << bdry << ' '
        << (twistUp ? 1 : 0) << ' ' << (hadEqualRank ? 1 : 0) << ' '
        << static_cast<int>(bdryEdges) << ' '
        << bdryNext[0] << ' ' << bdryNext[1] << ' '
        << static_cast<int>(bdryTwist[0]) << ' '
        << static_cast<int>(bdryTwist[1]) << ' '
        << bdryNextOld[0] << ' ' << bdryNextOld[1] << ' '
        << static_cast<int>(bdryTwistOld[0]) << ' '
        << static_cast<int>(bdryTwistOld[1]);
}

bool CompactSearcher::TetVertexState::readData(std::istream& in,
        unsigned long nStates) {
    in >> parent >> rank >> bdry;

    // twistUp is a char, but we need to read it as an int.
    int twist;
    in >> twist;
    twistUp = twist;

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
    if (twist != 1 && twist != 0)
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

void CompactSearcher::TetEdgeState::dumpData(std::ostream& out, unsigned nTets)
        const {
    // Be careful with twistUp, which is a char but which should be
    // written as an int.
    out << parent << ' ' << rank << ' ' << size << ' '
        << (bounded ? 1 : 0) << ' ' << (twistUp ? 1 : 0) << ' '
        << (hadEqualRank ? 1 : 0) << ' ';
    unsigned i;
    for (i = 0; i < nTets * 4 && i < 64; ++i)
        out << char(facesPos.get(i) + '0');
    out << ' ';
    for (i = 0; i < nTets * 4 && i < 64; ++i)
        out << char(facesNeg.get(i) + '0');
}

bool CompactSearcher::TetEdgeState::readData(std::istream& in, unsigned nTets) {
    in >> parent >> rank >> size;

    // bounded is a bool, but we need to read it as an int.
    int bBounded;
    in >> bBounded;
    bounded = bBounded;

    // twistUp is a char, but we need to read it as an int.
    int twist;
    in >> twist;
    twistUp = twist;

    // hadEqualRank is a bool, but we need to read it as an int.
    int bRank;
    in >> bRank;
    hadEqualRank = bRank;

    char cFaces;
    bool facesBroken = false;
    unsigned i;
    for (i = 0; i < nTets * 4 && i < 64; ++i) {
        in >> cFaces;
        if (cFaces >= '0' && cFaces <= '3')
            facesPos.set(i, cFaces - '0');
        else
            facesBroken = true;
    }
    for (i = 0; i < nTets * 4 && i < 64; ++i) {
        in >> cFaces;
        if (cFaces >= '0' && cFaces <= '3')
            facesNeg.set(i, cFaces - '0');
        else
            facesBroken = true;
    }

    if (parent < -1 || parent >= 6 * static_cast<long>(nTets))
        return false;
    if (rank >= 6 * nTets)
        return false;
    if (size >= 6 * nTets)
        return false;
    if (bBounded != 1 && bBounded != 0)
        return false;
    if (twist != 1 && twist != 0)
        return false;
    if (bRank != 1 && bRank != 0)
        return false;
    if (facesBroken)
        return false;

    return true;
}

CompactSearcher::CompactSearcher(FacetPairing<3> pairing,
        FacetPairing<3>::IsoList autos, bool orientableOnly,
        CensusPurge whichPurge) :
        GluingPermSearcher<3>(std::move(pairing), std::move(autos),
            orientableOnly, true /* finiteOnly */, whichPurge) {
    // Initialise the internal arrays to accurately reflect the underlying
    // face pairing.

    unsigned nTets = perms_.size();

    // ---------- Tracking of vertex / edge equivalence classes ----------

    unsigned i;

    nVertexClasses = nTets * 4;
    vertexState = new TetVertexState[nTets * 4];
    vertexStateChanged = new int[nTets * 8];
    std::fill(vertexStateChanged, vertexStateChanged + nTets * 8, -1);
    for (i = 0; i < nTets * 4; i++) {
        vertexState[i].bdryEdges = 3;
        vertexState[i].bdryNext[0] = vertexState[i].bdryNext[1] = i;
        vertexState[i].bdryTwist[0] = vertexState[i].bdryTwist[1] = 0;
        // Initialise the backup members also so we're not writing
        // uninitialised data via dumpData().
        vertexState[i].bdryNextOld[0] = vertexState[i].bdryNextOld[1] = -1;
        vertexState[i].bdryTwistOld[0] = vertexState[i].bdryTwistOld[1] = 0;
    }

    nEdgeClasses = nTets * 6;
    edgeState = new TetEdgeState[nTets * 6];
    edgeStateChanged = new int[nTets * 8];
    std::fill(edgeStateChanged, edgeStateChanged + nTets * 8, -1);

    // Since our hard-coded Qitmask classes only support 64 faces, only work
    // with the first 16 tetrahedra.  If n > 16, this just weakens the
    // optimisation; however, this is no great loss since for n > 16 the
    // census code is at present infeasibly slow anyway.
    for (i = 0; i < nTets && i < 16; ++i) {
        /* 01 on +012, +013             */
        edgeState[6 * i    ].facesPos.set(4 * i + 3, 1);
        edgeState[6 * i    ].facesPos.set(4 * i + 2, 1);
        /* 02 on -012        +023       */
        edgeState[6 * i + 1].facesNeg.set(4 * i + 3, 1);
        edgeState[6 * i + 1].facesPos.set(4 * i + 1, 1);
        /* 03 on       -013, -023       */
        edgeState[6 * i + 2].facesNeg.set(4 * i + 2, 1);
        edgeState[6 * i + 2].facesNeg.set(4 * i + 1, 1);
        /* 12 on +012,             +123 */
        edgeState[6 * i + 3].facesPos.set(4 * i + 3, 1);
        edgeState[6 * i + 3].facesPos.set(4 * i + 0, 1);
        /* 13 on       +013        -123 */
        edgeState[6 * i + 4].facesPos.set(4 * i + 2, 1);
        edgeState[6 * i + 4].facesNeg.set(4 * i + 0, 1);
        /* 23 on             +023, +123 */
        edgeState[6 * i + 5].facesPos.set(4 * i + 1, 1);
        edgeState[6 * i + 5].facesPos.set(4 * i + 0, 1);
    }
}

// TODO (net): See what was removed when we brought in vertex link checking.
void CompactSearcher::searchImpl(long maxDepth, ActionWrapper&& action_) {
    unsigned nTets = perms_.size();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTets * 4 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 || perms_.pairing().dest(0, 0).isBoundary(nTets)) {
            action_(perms_);
            return;
        }

        orderElt = 0;
        orientation[0] = 1;
    }

    // Is it a partial search that has already finished?
    if (orderElt == orderSize) {
        if (isCanonical())
            action_(perms_);
        return;
    }

    // ---------- Selecting the individual gluing permutations ----------

    int minOrder = orderElt;
    int maxOrder = orderElt + maxDepth;

    FacetSpec<3> face, adj;
    int mergeResult;
    while (orderElt >= minOrder) {
        face = order[orderElt];
        adj = perms_.pairing()[face];

        // TODO (long-term): Check for cancellation.

        // Move to the next permutation.

        // Be sure to preserve the orientation of the permutation if necessary.
        if ((! orientableOnly_) || adj.facet == 0)
            perms_.permIndex(face)++;
        else
            perms_.permIndex(face) += 2;

        // Are we out of ideas for this face?
        if (perms_.permIndex(face) >= 6) {
            // Yep.  Head back down to the previous face.
            perms_.permIndex(face) = -1;
            perms_.permIndex(adj) = -1;
            orderElt--;

            // Pull apart vertex and edge links at the previous level.
            if (orderElt >= minOrder) {
                splitVertexClasses();
                splitEdgeClasses();
            }

            continue;
        }

        // We are sitting on a new permutation to try.
        perms_.permIndex(adj) =
            Perm<3>::S3[perms_.permIndex(face)].inverse().S3Index();

        // Merge edge links and run corresponding tests.
        if (mergeEdgeClasses()) {
            // We created an invalid edge.
            splitEdgeClasses();
            continue;
        }

        // Merge vertex links and run corresponding tests.
        mergeResult = mergeVertexClasses();
        if (mergeResult & VLINK_NON_SPHERE) {
            // Our vertex link will never be a 2-sphere.  Stop now.
            splitVertexClasses();
            splitEdgeClasses();
            continue;
        }

        // Fix the orientation if appropriate.
        if (adj.facet == 0 && orientableOnly_) {
            // It's the first time we've hit this tetrahedron.
            if ((perms_.permIndex(face) + (face.facet == 3 ? 0 : 1) +
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
                action_(perms_);

            // Back to the previous face.
            orderElt--;

            // Pull apart vertex and edge links at the previous level.
            if (orderElt >= minOrder) {
                splitVertexClasses();
                splitEdgeClasses();
            }
        } else {
            // Not a full triangulation; just one level deeper.

            // We've moved onto a new face.
            // Be sure to get the orientation right.
            face = order[orderElt];
            if (orientableOnly_ && perms_.pairing().dest(face).facet > 0) {
                // permIndex(face) will be set to -1 or -2 as appropriate.
                adj = perms_.pairing()[face];
                if (orientation[face.simp] == orientation[adj.simp])
                    perms_.permIndex(face) = 1;
                else
                    perms_.permIndex(face) = 0;

                if ((face.facet == 3 ? 0 : 1) + (adj.facet == 3 ? 0 : 1) == 1)
                    perms_.permIndex(face) = (perms_.permIndex(face) + 1) % 2;

                perms_.permIndex(face) -= 2;
            }

            if (orderElt == maxOrder) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                action_(perms_);

                // Back to the previous face.
                perms_.permIndex(face) = -1;
                orderElt--;

                // Pull apart vertex and edge links at the previous level.
                if (orderElt >= minOrder) {
                    splitVertexClasses();
                    splitEdgeClasses();
                }
            }
        }
    }

    // And the search is over.

    // Some extra sanity checking.
    if (minOrder == 0) {
        // Our vertex classes had better be 4n standalone vertices.
        if (nVertexClasses != 4 * nTets)
            std::cerr << "ERROR: nVertexClasses == "
                << nVertexClasses << " at end of search!" << std::endl;
        for (int i = 0; i < static_cast<int>(nTets) * 4; i++) {
            if (vertexState[i].parent != -1)
                std::cerr << "ERROR: vertexState[" << i << "].parent == "
                    << vertexState[i].parent << " at end of search!"
                    << std::endl;
            if (vertexState[i].rank != 0)
                std::cerr << "ERROR: vertexState[" << i << "].rank == "
                    << vertexState[i].rank << " at end of search!" << std::endl;
            if (vertexState[i].bdry != 3)
                std::cerr << "ERROR: vertexState[" << i << "].bdry == "
                    << vertexState[i].bdry << " at end of search!" << std::endl;
            if (vertexState[i].hadEqualRank)
                std::cerr << "ERROR: vertexState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
            if (vertexState[i].bdryEdges != 3)
                std::cerr << "ERROR: vertexState[" << i << "].bdryEdges == "
                    << static_cast<int>(vertexState[i].bdryEdges)
                    << " at end of search!" << std::endl;
            if (vertexState[i].bdryNext[0] != i)
                std::cerr << "ERROR: vertexState[" << i << "].bdryNext[0] == "
                    << vertexState[i].bdryNext[0] << " at end of search!"
                    << std::endl;
            if (vertexState[i].bdryNext[1] != i)
                std::cerr << "ERROR: vertexState[" << i << "].bdryNext[1] == "
                    << vertexState[i].bdryNext[1] << " at end of search!"
                    << std::endl;
            if (vertexState[i].bdryTwist[0])
                std::cerr << "ERROR: vertexState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
            if (vertexState[i].bdryTwist[1])
                std::cerr << "ERROR: vertexState[" << i << "].bdryTwist == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (vertexStateChanged[i] != -1)
                std::cerr << "ERROR: vertexStateChanged[" << i << "] == "
                    << vertexStateChanged[i] << " at end of search!"
                    << std::endl;

        // And our edge classes had better be 6n standalone edges.
        if (nEdgeClasses != 6 * nTets)
            std::cerr << "ERROR: nEdgeClasses == "
                << nEdgeClasses << " at end of search!" << std::endl;
        for (unsigned i = 0; i < nTets * 6; i++) {
            if (edgeState[i].parent != -1)
                std::cerr << "ERROR: edgeState[" << i << "].parent == "
                    << edgeState[i].parent << " at end of search!"
                    << std::endl;
            if (edgeState[i].rank != 0)
                std::cerr << "ERROR: edgeState[" << i << "].rank == "
                    << edgeState[i].rank << " at end of search!" << std::endl;
            if (edgeState[i].size != 1)
                std::cerr << "ERROR: edgeState[" << i << "].size == "
                    << edgeState[i].size << " at end of search!" << std::endl;
            if (! edgeState[i].bounded)
                std::cerr << "ERROR: edgeState[" << i << "].bounded == "
                    "false at end of search!" << std::endl;
            if (edgeState[i].hadEqualRank)
                std::cerr << "ERROR: edgeState[" << i << "].hadEqualRank == "
                    "true at end of search!" << std::endl;
        }
        for (unsigned i = 0; i < nTets * 8; i++)
            if (edgeStateChanged[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged[i] << " at end of search!"
                    << std::endl;
    }
}

void CompactSearcher::dumpData(std::ostream& out) const {
    GluingPermSearcher<3>::dumpData(out);

    unsigned nTets = perms_.size();
    unsigned i;

    out << nVertexClasses << std::endl;
    for (i = 0; i < 4 * nTets; i++) {
        vertexState[i].dumpData(out);
        out << std::endl;
    }
    for (i = 0; i < 8 * nTets; i++) {
        if (i)
            out << ' ';
        out << vertexStateChanged[i];
    }
    out << std::endl;

    out << nEdgeClasses << std::endl;
    for (i = 0; i < 6 * nTets; i++) {
        edgeState[i].dumpData(out, nTets);
        out << std::endl;
    }
    for (i = 0; i < 8 * nTets; i++) {
        if (i)
            out << ' ';
        out << edgeStateChanged[i];
    }
    out << std::endl;
}

CompactSearcher::CompactSearcher(std::istream& in) :
        GluingPermSearcher<3>(in),
        nVertexClasses(0), vertexState(nullptr), vertexStateChanged(nullptr),
        nEdgeClasses(0), edgeState(nullptr), edgeStateChanged(nullptr) {
    unsigned nTets = perms_.size();
    unsigned i;

    in >> nVertexClasses;
    if (nVertexClasses > 4 * nTets)
        throw InvalidInput("Vertex classes out of range "
            "while attempting to read CompactSearcher");

    vertexState = new TetVertexState[4 * nTets];
    for (i = 0; i < 4 * nTets; i++)
        if (! vertexState[i].readData(in, 4 * nTets))
            throw InvalidInput("Invalid vertex state "
                "while attempting to read CompactSearcher");

    vertexStateChanged = new int[8 * nTets];
    for (i = 0; i < 8 * nTets; i++) {
        in >> vertexStateChanged[i];
        if (vertexStateChanged[i] < -1 ||
                 vertexStateChanged[i] >= 4 * static_cast<int>(nTets))
            throw InvalidInput("Invalid vertex state changed "
                "while attempting to read CompactSearcher");
    }

    in >> nEdgeClasses;
    if (nEdgeClasses > 6 * nTets)
        throw InvalidInput("Edge classes out of range "
            "while attempting to read CompactSearcher");

    edgeState = new TetEdgeState[6 * nTets];
    for (i = 0; i < 6 * nTets; i++)
        if (! edgeState[i].readData(in, nTets))
            throw InvalidInput("Invalid edge state "
                "while attempting to read CompactSearcher");

    edgeStateChanged = new int[8 * nTets];
    for (i = 0; i < 8 * nTets; i++) {
        in >> edgeStateChanged[i];
        if (edgeStateChanged[i] < -1 ||
                 edgeStateChanged[i] >= 6 * static_cast<int>(nTets))
            throw InvalidInput("Invalid edge state changed "
                "while attempting to read CompactSearcher");
    }

    // Did we hit an unexpected EOF?
    if (in.eof())
        throw InvalidInput("Unexpected end of input stream "
            "while attempting to read CompactSearcher");
}

int CompactSearcher::mergeVertexClasses() {
    // Merge all three vertex pairs for the current face.
    FacetSpec<3> face = order[orderElt];
    FacetSpec<3> adj = perms_.pairing()[face];

    int retVal = 0;

    int v, w;
    int vIdx, wIdx, tmpIdx, nextIdx;
    unsigned orderIdx;
    int vRep, wRep;
    int vNext[2], wNext[2];
    char vTwist[2], wTwist[2];
    Perm<4> p = perms_.perm(face);
    char parentTwists, hasTwist, tmpTwist;
    for (v = 0; v < 4; v++) {
        if (v == face.facet)
            continue;

        w = p[v];
        vIdx = v + 4 * face.simp;
        wIdx = w + 4 * adj.simp;
        orderIdx = v + 4 * orderElt;

        // Are the natural 012 representations of the two faces joined
        // with reversed orientations?
        // Here we combine the sign of permutation p with the mappings
        // from 012 to the native tetrahedron vertices, i.e., v <-> 3 and
        // w <-> 3.
        hasTwist = (p.sign() < 0 ? 0 : 1);
        if ((v == 3 && w != 3) || (v != 3 && w == 3))
            hasTwist ^= 1;

        parentTwists = 0;
        for (vRep = vIdx; vertexState[vRep].parent >= 0;
                vRep = vertexState[vRep].parent)
            parentTwists ^= vertexState[vRep].twistUp;
        for (wRep = wIdx; vertexState[wRep].parent >= 0;
                wRep = vertexState[wRep].parent)
            parentTwists ^= vertexState[wRep].twistUp;

        if (vRep == wRep) {
            vertexState[vRep].bdry -= 2;
            if (vertexState[vRep].bdry == 0)
                retVal |= VLINK_CLOSED;

            // Have we made the vertex link non-orientable?
            if (hasTwist ^ parentTwists)
                retVal |= VLINK_NON_SPHERE;

            vertexStateChanged[orderIdx] = -1;

            // Examine the cycles of boundary components.
            if (vIdx == wIdx) {
                // Either we are folding together two adjacent edges of the
                // vertex link, or we are making the vertex link
                // non-orientable.

                // The possible cases are:
                //
                // 1) hasTwist is true.  The vertex becomes
                // non-orientable, but we should already have flagged
                // this above.  Don't touch anything.
                //
                // 2) hasTwist is false, and vertexState[vIdx].bdryEdges is 3.
                // Here we are taking a stand-alone triangle and folding
                // two of its edges together.  Nothing needs to change.
                //
                // 3) hasTwist is false, and vertexState[vIdx].bdryEdges is 2.
                // This means we are folding together two edges of a
                // triangle whose third edge is already joined elsewhere.
                // We deal with this as follows:
                //
                if ((! hasTwist) && vertexState[vIdx].bdryEdges < 3) {
                    // Although bdryEdges is 2, we don't bother keeping
                    // a backup in bdryTwistOld[].  This is because
                    // bdryEdges jumps straight from 2 to 0, and the
                    // neighbours in bdryNext[] / bdryTwist[] never get
                    // overwritten.
                    if (vertexState[vIdx].bdryNext[0] == vIdx) {
                        // We are closing off a single boundary of length
                        // two.  All good.
                    } else {
                        // Adjust each neighbour to point to the other.
                        vtxBdryJoin(vertexState[vIdx].bdryNext[0],
                            1 ^ vertexState[vIdx].bdryTwist[0],
                            vertexState[vIdx].bdryNext[1],
                            vertexState[vIdx].bdryTwist[1] ^
                                vertexState[vIdx].bdryTwist[0]);
                    }
                }

                vertexState[vIdx].bdryEdges -= 2;
            } else {
                // We are joining two distinct tetrahedron vertices that
                // already contribute to the same vertex link.
                if (vertexState[vIdx].bdryEdges == 2)
                    vtxBdryBackup(vIdx);
                if (vertexState[wIdx].bdryEdges == 2)
                    vtxBdryBackup(wIdx);

                if (vtxBdryLength1(vIdx) && vtxBdryLength1(wIdx)) {
                    // We are joining together two boundaries of length one.
                    // Do nothing and mark the non-trivial genus.
                    // std::cerr << "NON-SPHERE: 1 >-< 1" << std::endl;
                    retVal |= VLINK_NON_SPHERE;
                } else if (vtxBdryLength2(vIdx, wIdx)) {
                    // We are closing off a single boundary of length two.
                    // All good.
                } else {
                    vtxBdryNext(vIdx, face.simp, v, face.facet, vNext, vTwist);
                    vtxBdryNext(wIdx, adj.simp, w, adj.facet, wNext, wTwist);

                    if (vNext[0] == wIdx && wNext[1 ^ vTwist[0]] == vIdx) {
                        // We are joining two adjacent edges of the vertex link.
                        // Simply eliminate them.
                        vtxBdryJoin(vNext[1], 0 ^ vTwist[1],
                            wNext[0 ^ vTwist[0]],
                            (vTwist[0] ^ wTwist[0 ^ vTwist[0]]) ^ vTwist[1]);
                    } else if (vNext[1] == wIdx &&
                            wNext[0 ^ vTwist[1]] == vIdx) {
                        // Again, joining two adjacent edges of the vertex link.
                        vtxBdryJoin(vNext[0], 1 ^ vTwist[0],
                            wNext[1 ^ vTwist[1]],
                            (vTwist[1] ^ wTwist[1 ^ vTwist[1]]) ^ vTwist[0]);
                    } else {
                        // See if we are joining two different boundary cycles
                        // together; if so, we have created non-trivial genus in
                        // the vertex link.
                        tmpIdx = vertexState[vIdx].bdryNext[0];
                        tmpTwist = vertexState[vIdx].bdryTwist[0];
                        while (tmpIdx != vIdx && tmpIdx != wIdx) {
                            nextIdx = vertexState[tmpIdx].
                                bdryNext[0 ^ tmpTwist];
                            tmpTwist ^= vertexState[tmpIdx].
                                bdryTwist[0 ^ tmpTwist];
                            tmpIdx = nextIdx;
                        }

                        if (tmpIdx == vIdx) {
                            // Different boundary cycles.
                            // Don't touch anything; just flag a
                            // high genus error.
                            // std::cerr << "NON-SPHERE: (X)" << std::endl;
                            retVal |= VLINK_NON_SPHERE;
                        } else {
                            // Same boundary cycle.
                            vtxBdryJoin(vNext[0], 1 ^ vTwist[0],
                                wNext[1 ^ hasTwist],
                                vTwist[0] ^ (hasTwist ^ wTwist[1 ^ hasTwist]));
                            vtxBdryJoin(vNext[1], 0 ^ vTwist[1],
                                wNext[0 ^ hasTwist],
                                vTwist[1] ^ (hasTwist ^ wTwist[0 ^ hasTwist]));
                        }
                    }
                }

                vertexState[vIdx].bdryEdges--;
                vertexState[wIdx].bdryEdges--;
            }
        } else {
            // We are joining two distinct vertices together and merging
            // their vertex links.
            if (vertexState[vRep].rank < vertexState[wRep].rank) {
                // Join vRep beneath wRep.
                vertexState[vRep].parent = wRep;
                vertexState[vRep].twistUp = hasTwist ^ parentTwists;

                vertexState[wRep].bdry = vertexState[wRep].bdry +
                    vertexState[vRep].bdry - 2;
                if (vertexState[wRep].bdry == 0)
                    retVal |= VLINK_CLOSED;

                vertexStateChanged[orderIdx] = vRep;
            } else {
                // Join wRep beneath vRep.
                vertexState[wRep].parent = vRep;
                vertexState[wRep].twistUp = hasTwist ^ parentTwists;
                if (vertexState[vRep].rank == vertexState[wRep].rank) {
                    vertexState[vRep].rank++;
                    vertexState[wRep].hadEqualRank = true;
                }

                vertexState[vRep].bdry = vertexState[vRep].bdry +
                    vertexState[wRep].bdry - 2;
                if (vertexState[vRep].bdry == 0)
                    retVal |= VLINK_CLOSED;

                vertexStateChanged[orderIdx] = wRep;
            }

            nVertexClasses--;

            // Adjust the cycles of boundary components.
            if (vertexState[vIdx].bdryEdges == 2)
                vtxBdryBackup(vIdx);
            if (vertexState[wIdx].bdryEdges == 2)
                vtxBdryBackup(wIdx);

            if (vtxBdryLength1(vIdx)) {
                if (vtxBdryLength1(wIdx)) {
                    // Both vIdx and wIdx form entire boundary components of
                    // length one; these are joined together and the vertex
                    // link is closed off.
                    // No changes to make for the boundary cycles.
                } else {
                    // Here vIdx forms a boundary component of length one,
                    // and wIdx does not.  Ignore vIdx, and simply excise the
                    // relevant edge from wIdx.
                    // There is nothing to do here unless wIdx only has one
                    // boundary edge remaining (in which case we know it
                    // joins to some different tetrahedron vertex).
                    if (vertexState[wIdx].bdryEdges == 1) {
                        wNext[0] = vertexState[wIdx].bdryNext[0];
                        wNext[1] = vertexState[wIdx].bdryNext[1];
                        wTwist[0] = vertexState[wIdx].bdryTwist[0];
                        wTwist[1] = vertexState[wIdx].bdryTwist[1];

                        vtxBdryJoin(wNext[0], 1 ^ wTwist[0], wNext[1],
                            wTwist[0] ^ wTwist[1]);
                    }
                }
            } else if (vtxBdryLength1(wIdx)) {
                // As above, but with the two vertices the other way around.
                if (vertexState[vIdx].bdryEdges == 1) {
                    vNext[0] = vertexState[vIdx].bdryNext[0];
                    vNext[1] = vertexState[vIdx].bdryNext[1];
                    vTwist[0] = vertexState[vIdx].bdryTwist[0];
                    vTwist[1] = vertexState[vIdx].bdryTwist[1];

                    vtxBdryJoin(vNext[0], 1 ^ vTwist[0], vNext[1],
                        vTwist[0] ^ vTwist[1]);
                }
            } else {
                // Each vertex belongs to a boundary component of length
                // at least two.  Merge the components together.
                vtxBdryNext(vIdx, face.simp, v, face.facet, vNext, vTwist);
                vtxBdryNext(wIdx, adj.simp, w, adj.facet, wNext, wTwist);

                vtxBdryJoin(vNext[0], 1 ^ vTwist[0], wNext[1 ^ hasTwist],
                    vTwist[0] ^ (hasTwist ^ wTwist[1 ^ hasTwist]));
                vtxBdryJoin(vNext[1], 0 ^ vTwist[1], wNext[0 ^ hasTwist],
                    vTwist[1] ^ (hasTwist ^ wTwist[0 ^ hasTwist]));
            }

            vertexState[vIdx].bdryEdges--;
            vertexState[wIdx].bdryEdges--;
        }
    }

    return retVal;
}

void CompactSearcher::splitVertexClasses() {
    // Split all three vertex pairs for the current face.
    FacetSpec<3> face = order[orderElt];
    FacetSpec<3> adj = perms_.pairing()[face];

    int v, w;
    int vIdx, wIdx;
    unsigned orderIdx;
    int rep, subRep;
    Perm<4> p = perms_.perm(face);
    // Do everything in reverse.  This includes the loop over vertices.
    for (v = 3; v >= 0; v--) {
        if (v == face.facet)
            continue;

        w = p[v];
        vIdx = v + 4 * face.simp;
        wIdx = w + 4 * adj.simp;
        orderIdx = v + 4 * orderElt;

        if (vertexStateChanged[orderIdx] < 0) {
            for (rep = vIdx; vertexState[rep].parent >= 0;
                    rep = vertexState[rep].parent)
                ;
            vertexState[rep].bdry += 2;
        } else {
            subRep = vertexStateChanged[orderIdx];
            rep = vertexState[subRep].parent;

            vertexState[subRep].parent = -1;
            if (vertexState[subRep].hadEqualRank) {
                vertexState[subRep].hadEqualRank = false;
                vertexState[rep].rank--;
            }

            vertexState[rep].bdry = vertexState[rep].bdry + 2 -
                vertexState[subRep].bdry;

            vertexStateChanged[orderIdx] = -1;
            nVertexClasses++;
        }

        // Restore cycles of boundary components.
        if (vIdx == wIdx) {
            vertexState[vIdx].bdryEdges += 2;

            // Adjust neighbours to point back to vIdx if required.
            if (vertexState[vIdx].bdryEdges == 2)
                vtxBdryFixAdj(vIdx);
        } else {
            vertexState[wIdx].bdryEdges++;
            vertexState[vIdx].bdryEdges++;

            switch (vertexState[wIdx].bdryEdges) {
                case 3: vertexState[wIdx].bdryNext[0] =
                            vertexState[wIdx].bdryNext[1] = wIdx;
                        vertexState[wIdx].bdryTwist[0] =
                            vertexState[wIdx].bdryTwist[1] = 0;
                        break;

                case 2: vtxBdryRestore(wIdx);
                        // Fall through to the next case, so we can
                        // adjust the neighbours.

                case 1: // Nothing was changed for wIdx during the merge,
                        // so there is nothing there to restore.

                        // Adjust neighbours to point back to wIdx.
                        vtxBdryFixAdj(wIdx);
            }

            switch (vertexState[vIdx].bdryEdges) {
                case 3: vertexState[vIdx].bdryNext[0] =
                            vertexState[vIdx].bdryNext[1] = vIdx;
                        vertexState[vIdx].bdryTwist[0] =
                            vertexState[vIdx].bdryTwist[1] = 0;
                        break;

                case 2: vtxBdryRestore(vIdx);
                        // Fall through to the next case, so we can
                        // adjust the neighbours.

                case 1: // Nothing was changed for vIdx during the merge,
                        // so there is nothing there to restore.

                        // Adjust neighbours to point back to vIdx.
                        vtxBdryFixAdj(vIdx);
            }
        }
    }
}

bool CompactSearcher::mergeEdgeClasses() {
    FacetSpec<3> face = order[orderElt];
    FacetSpec<3> adj = perms_.pairing()[face];

    bool retVal = false;

    Perm<4> p = perms_.perm(face);
    int v1, w1, v2, w2;
    int e, f;
    int orderIdx;
    int eRep, fRep;

    v1 = face.facet;
    w1 = p[v1];

    char parentTwists, hasTwist;
    for (v2 = 0; v2 < 4; v2++) {
        if (v2 == v1)
            continue;

        w2 = p[v2];

        // Look at the edge opposite v1-v2.
        e = 5 - Edge<3>::edgeNumber[v1][v2];
        f = 5 - Edge<3>::edgeNumber[w1][w2];

        orderIdx = v2 + 4 * orderElt;

        // We declare the natural orientation of an edge to be smaller
        // vertex to larger vertex.
        hasTwist = (p[Edge<3>::edgeVertex[e][0]] > p[Edge<3>::edgeVertex[e][1]] ?
            1 : 0);

        parentTwists = 0;
        eRep = findEdgeClass(e + 6 * face.simp, parentTwists);
        fRep = findEdgeClass(f + 6 * adj.simp, parentTwists);

        if (eRep == fRep) {
            edgeState[eRep].bounded = false;

            if (hasTwist ^ parentTwists)
                retVal = true;

            edgeStateChanged[orderIdx] = -1;
        } else {
            if (edgeState[eRep].rank < edgeState[fRep].rank) {
                // Join eRep beneath fRep.
                edgeState[eRep].parent = fRep;
                edgeState[eRep].twistUp = hasTwist ^ parentTwists;
                edgeState[fRep].size += edgeState[eRep].size;

                edgeStateChanged[orderIdx] = eRep;
            } else {
                // Join fRep beneath eRep.
                edgeState[fRep].parent = eRep;
                edgeState[fRep].twistUp = hasTwist ^ parentTwists;
                if (edgeState[eRep].rank == edgeState[fRep].rank) {
                    edgeState[eRep].rank++;
                    edgeState[fRep].hadEqualRank = true;
                }
                edgeState[eRep].size += edgeState[fRep].size;

                edgeStateChanged[orderIdx] = fRep;
            }
            nEdgeClasses--;
        }
    }

    return retVal;
}

void CompactSearcher::splitEdgeClasses() {
    FacetSpec<3> face = order[orderElt];

    int v1, v2;
    int e;
    int eIdx, orderIdx;
    int rep, subRep;

    v1 = face.facet;

    for (v2 = 3; v2 >= 0; v2--) {
        if (v2 == v1)
            continue;

        // Look at the edge opposite v1-v2.
        e = 5 - Edge<3>::edgeNumber[v1][v2];

        eIdx = e + 6 * face.simp;
        orderIdx = v2 + 4 * orderElt;

        if (edgeStateChanged[orderIdx] < 0)
            edgeState[findEdgeClass(eIdx)].bounded = true;
        else {
            subRep = edgeStateChanged[orderIdx];
            rep = edgeState[subRep].parent;

            edgeState[subRep].parent = -1;
            if (edgeState[subRep].hadEqualRank) {
                edgeState[subRep].hadEqualRank = false;
                edgeState[rep].rank--;
            }

            edgeState[rep].size -= edgeState[subRep].size;

            edgeStateChanged[orderIdx] = -1;
            nEdgeClasses++;
        }
    }
}

void CompactSearcher::vtxBdryConsistencyCheck() {
    int adj, id, end;
    for (id = 0; id < static_cast<int>(perms_.size()) * 4; id++)
        if (vertexState[id].bdryEdges > 0)
            for (end = 0; end < 2; end++) {
                adj = vertexState[id].bdryNext[end];
                if (vertexState[adj].bdryEdges == 0)
                    std::cerr << "CONSISTENCY ERROR: Vertex link boundary "
                        << id << '/' << end
                        << " runs into an internal vertex." << std::endl;
                if (vertexState[adj].bdryNext[(1 ^ end) ^
                        vertexState[id].bdryTwist[end]] != id)
                    std::cerr << "CONSISTENCY ERROR: Vertex link boundary "
                        << id << '/' << end
                        << " has a mismatched adjacency." << std::endl;
                if (vertexState[adj].bdryTwist[(1 ^ end) ^
                        vertexState[id].bdryTwist[end]] !=
                        vertexState[id].bdryTwist[end])
                    std::cerr << "CONSISTENCY ERROR: Vertex link boundary "
                        << id << '/' << end
                        << " has a mismatched twist." << std::endl;
            }
}

void CompactSearcher::vtxBdryDump(std::ostream& out) {
    for (unsigned id = 0; id < perms_.size() * 4; id++) {
        if (id > 0)
            out << ' ';
        out << vertexState[id].bdryNext[0]
            << (vertexState[id].bdryTwist[0] ? '~' : '-')
            << id
            << (vertexState[id].bdryTwist[1] ? '~' : '-')
            << vertexState[id].bdryNext[1]
            << " [" << int(vertexState[id].bdryEdges) << ']';
    }
    out << std::endl;
}

} // namespace regina

