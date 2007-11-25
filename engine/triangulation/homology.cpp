
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

// ntriangulationhomology.cpp
//        Implements homology as a property of a triangulation.

#include "triangulation/ntriangulation.h"
#include "maths/nmatrixint.h"

namespace regina {

const NAbelianGroup& NTriangulation::getHomologyH1() const {
    if (H1.known())
        return *H1.value();

    if (getNumberOfTetrahedra() == 0)
        return *(H1 = new NAbelianGroup());

    // Calculate the first homology.
    // Find a maximal forest in the dual 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    stdhash::hash_set<NFace*, HashPointer> forest;
    maximalForestInDualSkeleton(forest);

    // Build a presentation matrix.
    // Each non-boundary not-in-forest face is a generator.
    // Each non-boundary edge is a relation.
    unsigned long nBdryEdges = 0;
    unsigned long nBdryFaces = 0;
    for (BoundaryComponentIterator bit = boundaryComponents.begin();
            bit != boundaryComponents.end(); bit++) {
        nBdryEdges += (*bit)->getNumberOfEdges();
        nBdryFaces += (*bit)->getNumberOfFaces();
    }
    long nGens = getNumberOfFaces() - nBdryFaces - forest.size();
    long nRels = getNumberOfEdges() - nBdryEdges;
    NMatrixInt pres(nRels, nGens);

    // Find out which face corresponds to which generator.
    long* genIndex = new long[getNumberOfFaces()];
    long i = 0;
    for (FaceIterator fit = faces.begin(); fit != faces.end(); fit++) {
        if ((*fit)->isBoundary())
            genIndex[fit - faces.begin()] = -1;
        else if (forest.count(*fit))
            genIndex[fit - faces.begin()] = -1;
        else {
            genIndex[fit - faces.begin()] = i;
            i++;
        }
    }

    // Run through each edge and put the relations in the matrix.
    std::deque<NEdgeEmbedding>::const_iterator embit;
    NTetrahedron* currTet;
    NFace* face;
    int currTetFace;
    long faceGenIndex;
    i = 0;
    for (EdgeIterator eit = edges.begin(); eit != edges.end(); eit++) {
        if (! (*eit)->isBoundary()) {
            // Put in the relation corresponding to this edge.
            for (embit = (*eit)->getEmbeddings().begin();
                    embit != (*eit)->getEmbeddings().end(); embit++) {
                currTet = (*embit).getTetrahedron();
                currTetFace = (*embit).getVertices()[2];
                face = currTet->getFace(currTetFace);
                faceGenIndex = genIndex[getFaceIndex(face)];
                if (faceGenIndex >= 0) {
                    if ((face->getEmbedding(0).getTetrahedron() == currTet) &&
                            (face->getEmbedding(0).getFace() == currTetFace))
                        pres.entry(i, faceGenIndex) += 1;
                    else
                        pres.entry(i, faceGenIndex) -= 1;
                }
            }
            i++;
        }
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(pres);
    return *(H1 = ans);
}

const NAbelianGroup& NTriangulation::getHomologyH1Rel() const {
    if (H1Rel.known())
        return *H1Rel.value();

    if (getNumberOfBoundaryComponents() == 0)
        return *(H1Rel = new NAbelianGroup(getHomologyH1()));

    // Calculate the relative first homology wrt the boundary.

    // Find a maximal forest in the 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    stdhash::hash_set<NEdge*, HashPointer> forest;
    maximalForestInSkeleton(forest, false);

    // Build a presentation matrix.
    // Each non-boundary not-in-forest edge is a generator.
    // Each non-boundary face is a relation.
    unsigned long nBdryVertices = 0;
    unsigned long nBdryEdges = 0;
    unsigned long nBdryFaces = 0;
    unsigned long nClosedComponents = 0;
    for (BoundaryComponentIterator bit = boundaryComponents.begin();
            bit != boundaryComponents.end(); bit++) {
        nBdryVertices += (*bit)->getNumberOfVertices();
        nBdryEdges += (*bit)->getNumberOfEdges();
        nBdryFaces += (*bit)->getNumberOfFaces();
    }
    for (ComponentIterator cit = components.begin();
            cit != components.end(); cit++)
        if ((*cit)->isClosed())
            nClosedComponents++;
    long nGens = getNumberOfEdges() - nBdryEdges
        - getNumberOfVertices() + nBdryVertices
        + nClosedComponents;
    long nRels = getNumberOfFaces() - nBdryFaces;
    NMatrixInt pres(nRels, nGens);

    // Find out which edge corresponds to which generator.
    long* genIndex = new long[getNumberOfEdges()];
    long i = 0;
    for (EdgeIterator eit = edges.begin(); eit != edges.end(); eit++) {
        if ((*eit)->isBoundary())
            genIndex[eit - edges.begin()] = -1;
        else if (forest.count(*eit))
            genIndex[eit - edges.begin()] = -1;
        else {
            genIndex[eit - edges.begin()] = i;
            i++;
        }
    }

    // Run through each face and put the relations in the matrix.
    NTetrahedron* currTet;
    NPerm currTetVertices;
    long edgeGenIndex;
    i = 0;
    int faceEdge, currEdgeStart, currEdgeEnd, currEdge;
    for (FaceIterator fit = faces.begin(); fit != faces.end(); fit++) {
        if (! (*fit)->isBoundary()) {
            // Put in the relation corresponding to this face.
            currTet = (*fit)->getEmbedding(0).getTetrahedron();
            currTetVertices = (*fit)->getEmbedding(0).getVertices();
            for (faceEdge = 0; faceEdge < 3; faceEdge++) {
                currEdgeStart = currTetVertices[faceEdge];
                currEdgeEnd = currTetVertices[(faceEdge + 1) % 3];
                // Examine the edge from vertex edgeStart to edgeEnd
                // in tetrahedron currTet.
                currEdge = edgeNumber[currEdgeStart][currEdgeEnd];
                edgeGenIndex = genIndex[getEdgeIndex(
                    currTet->getEdge(currEdge))];
                if (edgeGenIndex >= 0) {
                    if (currTet->getEdgeMapping(currEdge)[0] == currEdgeStart)
                        pres.entry(i, edgeGenIndex) += 1;
                    else
                        pres.entry(i, edgeGenIndex) -= 1;
                }
            }
            i++;
        }
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(pres);
    return *(H1Rel = ans);
}

const NAbelianGroup& NTriangulation::getHomologyH1Bdry() const {
    if (H1Bdry.known())
        return *H1Bdry.value();

    // Run through the individual boundary components and add the
    // appropriate pieces to the homology group.
    unsigned long rank = 0;
    unsigned long z2rank = 0;

    // Ensure that the skeleton has been calculated.
    if (! calculatedSkeleton)
        calculateSkeleton();

    for (BoundaryComponentIterator bit = boundaryComponents.begin();
            bit != boundaryComponents.end(); bit++) {
        if ((*bit)->isOrientable()) {
            rank += (2 - (*bit)->getEulerCharacteristic());
        } else {
            rank += (1 - (*bit)->getEulerCharacteristic());
            z2rank++;
        }
    }

    // Build the group and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank(rank);
    ans->addTorsionElement(2, z2rank);
    return *(H1Bdry = ans);
}

const NAbelianGroup& NTriangulation::getHomologyH2() const {
    if (H2.known())
        return *H2.value();

    if (getNumberOfTetrahedra() == 0)
        return *(H2 = new NAbelianGroup());

    // Calculations are different for orientable vs non-orientable
    // components.
    // We know the only components will be Z and Z_2.
    long rank, z2rank;
    if (isOrientable()) {
        // Same as H1Rel without the torsion elements.
        rank = getHomologyH1Rel().getRank();
        z2rank = 0;
    } else {
        // Non-orientable!
        // z2rank = # closed cmpts - # closed orientable cmpts
        z2rank = 0;
        for (ComponentIterator cit = components.begin();
                cit != components.end(); cit++)
            if ((*cit)->isClosed())
                if (! ((*cit)->isOrientable()))
                    z2rank++;

        // Find rank(Z_2) + rank(Z) and take off z2rank.
        rank = getHomologyH1Rel().getRank() +
            getHomologyH1Rel().getTorsionRank(2) -
            getHomologyH1().getTorsionRank(2) -
            z2rank;
    }

    // Build the new group and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank(rank);
    if (z2rank)
        ans->addTorsionElement(2, z2rank);
    return *(H2 = ans);
}

} // namespace regina

