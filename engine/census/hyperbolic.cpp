
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "triangulation/facepair.h"
#include "triangulation/dim3.h"

namespace regina {

HyperbolicMinSearcher::HyperbolicMinSearcher(FacetPairing<3> pairing,
        FacetPairing<3>::IsoList autos, bool orientableOnly) :
        EulerSearcher(0, std::move(pairing), std::move(autos), orientableOnly,
            CensusPurge::NonMinimalHyp) {
}

void HyperbolicMinSearcher::searchImpl(long maxDepth, ActionWrapper&& action_) {
    size_t nTets = perms_.size();
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
    if (orderElt == static_cast<ssize_t>(orderSize)) {
        if (isCanonical())
            action_(perms_);
        return;
    }

    // ---------- Selecting the individual gluing permutations ----------

    ssize_t minOrder = orderElt;
    ssize_t maxOrder = orderElt + maxDepth;

    while (orderElt >= minOrder) {
        FacetSpec<3> face = order[orderElt];
        FacetSpec<3> adj = perms_.pairing()[face];

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
            Perm<3>::Sn[perms_.permIndex(face)].inverse().S3Index();

        // Merge edge links and run corresponding tests.
        if (mergeEdgeClasses()) {
            // We created an invalid edge.
            splitEdgeClasses();
            continue;
        }

        // Merge vertex links and run corresponding tests.
        int mergeResult = mergeVertexClasses();
        if (mergeResult & VLINK_BAD_EULER) {
            // Our vertex link can never obtain the correct
            // Euler characteristic.  Stop now.
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
        if (orderElt == static_cast<ssize_t>(orderSize)) {
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
        for (size_t i = 0; i < nTets * 4; i++) {
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
            if (vertexState[i].euler != 2)
                std::cerr << "ERROR: vertexState[" << i << "].euler == "
                    << vertexState[i].euler << " at end of search!"
                    << std::endl;
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
        for (size_t i = 0; i < nTets * 8; i++)
            if (vertexStateChanged[i] != VLINK_JOIN_INIT)
                std::cerr << "ERROR: vertexStateChanged[" << i << "] == "
                    << vertexStateChanged[i] << " at end of search!"
                    << std::endl;

        // And our edge classes had better be 6n standalone edges.
        if (nEdgeClasses != 6 * nTets)
            std::cerr << "ERROR: nEdgeClasses == "
                << nEdgeClasses << " at end of search!" << std::endl;
        for (size_t i = 0; i < nTets * 6; i++) {
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
        for (size_t i = 0; i < nTets * 8; i++)
            if (edgeStateChanged[i] != -1)
                std::cerr << "ERROR: edgeStateChanged[" << i << "] == "
                    << edgeStateChanged[i] << " at end of search!"
                    << std::endl;
    }
}

void HyperbolicMinSearcher::dumpData(std::ostream& out) const {
    EulerSearcher::dumpData(out);
}

int HyperbolicMinSearcher::mergeEdgeClasses() {
    /**
     * As well as detecting edges that are self-identified in reverse,
     * we strip out low-degree edges here.  Although we are also interested
     * in non-geometric triangulations, we can still ignore triangulations
     * with low-degree edges because (with a little work) they can be
     * proven to be non-minimal.  For details see:
     * "The cusped hyperbolic census is complete", B.B.
     */
    FacetSpec<3> face = order[orderElt];
    FacetSpec<3> adj = perms_.pairing()[face];

    int retVal = 0;

    Perm<4> p = perms_.perm(face);

    int v1 = face.facet;
    int w1 = p[v1];

    char parentTwists, hasTwist;
    for (int v2 = 0; v2 < 4; v2++) {
        if (v2 == v1)
            continue;

        int w2 = p[v2];

        // Look at the edge opposite v1-v2.
        int e = 5 - Edge<3>::edgeNumber[v1][v2];
        int f = 5 - Edge<3>::edgeNumber[w1][w2];

        size_t orderIdx = v2 + 4 * orderElt;

        // We declare the natural orientation of an edge to be smaller
        // vertex to larger vertex.
        hasTwist = (p[Edge<3>::edgeVertex[e][0]] > p[Edge<3>::edgeVertex[e][1]] ?
            1 : 0);

        parentTwists = 0;
        size_t eRep = findEdgeClass(e + 6 * face.simp, parentTwists);
        size_t fRep = findEdgeClass(f + 6 * adj.simp, parentTwists);

        if (eRep == fRep) {
            edgeState[eRep].bounded = false;

            if (edgeState[eRep].size <= 2)
                retVal |= ECLASS_LOWDEG;
            else if (edgeState[eRep].size == 3) {
                // Flag as LOWDEG only if three distinct tetrahedra are used.
                auto middleTet = perms_.pairing().dest(face.simp, v2).simp;
                if (face.simp != adj.simp && adj.simp != middleTet &&
                        middleTet != face.simp)
                    retVal |= ECLASS_LOWDEG;
            }
            if (hasTwist ^ parentTwists)
                retVal |= ECLASS_TWISTED;

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

void HyperbolicMinSearcher::splitEdgeClasses() {
    FacetSpec<3> face = order[orderElt];

    int v1 = face.facet;

    for (int v2 = 3; v2 >= 0; v2--) {
        if (v2 == v1)
            continue;

        // Look at the edge opposite v1-v2.
        int e = 5 - Edge<3>::edgeNumber[v1][v2];

        size_t eIdx = e + 6 * face.simp;
        size_t orderIdx = v2 + 4 * orderElt;

        if (edgeStateChanged[orderIdx] < 0)
            edgeState[findEdgeClass(eIdx)].bounded = true;
        else {
            size_t subRep = edgeStateChanged[orderIdx];
            size_t rep = edgeState[subRep].parent;

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

} // namespace regina

