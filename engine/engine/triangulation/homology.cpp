
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// ntriangulationhomology.cpp
//        Implements homology as a property of a triangulation.

#include "triangulation/ntriangulation.h"
#include "maths/nmatrixint.h"

const NAbelianGroup& NTriangulation::getHomologyH1() {
    if (calculatedH1)
        return *H1;

    if (getNumberOfTetrahedra() == 0) {
        H1 = new NAbelianGroup();
        calculatedH1 = true;
        return *H1;
    }

    // Calculate the first homology.
    // Find a maximal forest in the dual 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    NPointerSet<NFace> forest;
    maximalForestInDualSkeleton(forest);
    
    // Build a presentation matrix.
    // Each non-boundary not-in-forest face is a generator.
    // Each non-boundary edge is a relation.
    unsigned long nBdryEdges = 0;
    unsigned long nBdryFaces = 0;
    BoundaryComponentIterator bit(boundaryComponents);
    while (! bit.done()) {
        nBdryEdges += (*bit)->getNumberOfEdges();
        nBdryFaces += (*bit)->getNumberOfFaces();
        bit++;
    }
    long nGens = getNumberOfFaces() - nBdryFaces - forest.size();
    long nRels = getNumberOfEdges() - nBdryEdges;
    NMatrixInt pres(nRels, nGens);

    // Find out which face corresponds to which generator.
    long* genIndex = new long[getNumberOfFaces()];
    FaceIterator fit(faces);
    long i = 0;
    while (! fit.done()) {
        if ((*fit)->isBoundary())
            genIndex[fit.getArrayIndex()] = -1;
        else if (forest.contains(*fit))
            genIndex[fit.getArrayIndex()] = -1;
        else {
            genIndex[fit.getArrayIndex()] = i;
            i++;
        }
        fit++;
    }

    // Run through each edge and put the relations in the matrix.
    EdgeIterator eit(edges);
    NDynamicArrayIterator<NEdgeEmbedding> embit;
    NTetrahedron* currTet;
    NFace* face;
    int currTetFace;
    long faceGenIndex;
    i = 0;
    while (! eit.done()) {
        if (! (*eit)->isBoundary()) {
            // Put in the relation corresponding to this edge.
            embit.init((*eit)->getEmbeddings());
            while (! embit.done()) {
                currTet = (*embit).getTetrahedron();
                currTetFace = (*embit).getVertices()[2];
                face = currTet->getFace(currTetFace);
                faceGenIndex = genIndex[faces.position(face)];
                if (faceGenIndex >= 0) {
                    if ((face->getEmbedding(0).getTetrahedron() == currTet) &&
                            (face->getEmbedding(0).getFace() == currTetFace))
                        pres.entry(i, faceGenIndex) += 1;
                    else
                        pres.entry(i, faceGenIndex) -= 1;
                }
                embit++;
            }
            i++;
        }
        eit++;
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    H1 = new NAbelianGroup();
    H1->addGroup(pres);
    calculatedH1 = true;
    return *H1;
}

const NAbelianGroup& NTriangulation::getHomologyH1Rel() {
    if (calculatedH1Rel)
        return *H1Rel;

    if (getNumberOfBoundaryComponents() == 0) {
        H1Rel = new NAbelianGroup(getHomologyH1());
        calculatedH1Rel = true;
        return *H1Rel;
    }

    // Calculate the relative first homology wrt the boundary.

    // Find a maximal forest in the 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    NPointerSet<NEdge> forest;
    maximalForestInSkeleton(forest, false);

    // Build a presentation matrix.
    // Each non-boundary not-in-forest edge is a generator.
    // Each non-boundary face is a relation.
    unsigned long nBdryVertices = 0;
    unsigned long nBdryEdges = 0;
    unsigned long nBdryFaces = 0;
    unsigned long nClosedComponents = 0;
    BoundaryComponentIterator bit(boundaryComponents);
    while (! bit.done()) {
        nBdryVertices += (*bit)->getNumberOfVertices();
        nBdryEdges += (*bit)->getNumberOfEdges();
        nBdryFaces += (*bit)->getNumberOfFaces();
        bit++;
    }
    ComponentIterator cit(components);
    while (! cit.done()) {
        if ((*cit)->isClosed())
            nClosedComponents++;
        cit++;
    }
    long nGens = getNumberOfEdges() - nBdryEdges
        - getNumberOfVertices() + nBdryVertices
        + nClosedComponents;
    long nRels = getNumberOfFaces() - nBdryFaces;
    NMatrixInt pres(nRels, nGens);

    // Find out which edge corresponds to which generator.
    long* genIndex = new long[getNumberOfEdges()];
    EdgeIterator eit(edges);
    long i = 0;
    while (! eit.done()) {
        if ((*eit)->isBoundary())
            genIndex[eit.getArrayIndex()] = -1;
        else if (forest.contains(*eit))
            genIndex[eit.getArrayIndex()] = -1;
        else {
            genIndex[eit.getArrayIndex()] = i;
            i++;
        }
        eit++;
    }

    // Run through each face and put the relations in the matrix.
    FaceIterator fit(faces);
    NTetrahedron* currTet;
    NPerm currTetVertices;
    long edgeGenIndex;
    i = 0;
    int faceEdge, currEdgeStart, currEdgeEnd, currEdge;
    while (! fit.done()) {
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
                edgeGenIndex = genIndex[edges.position(
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
        fit++;
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    H1Rel = new NAbelianGroup();
    H1Rel->addGroup(pres);
    calculatedH1Rel = true;
    return *H1Rel;
}

const NAbelianGroup& NTriangulation::getHomologyH1Bdry() {
    if (calculatedH1Bdry)
        return *H1Bdry;

    // Run through the individual boundary components and add the
    // appropriate pieces to the homology group.
    unsigned long rank = 0;
    unsigned long z2rank = 0;

    // The following line ensures the skeleton has been calculated.
    BoundaryComponentIterator bit(getBoundaryComponents());
    while (! bit.done()) {
        if ((*bit)->isOrientable()) {
            rank += (2 - (*bit)->getEulerCharacteristic());
        } else {
            rank += (1 - (*bit)->getEulerCharacteristic());
            z2rank++;
        }
        bit++;
    }

    // Build the group and tidy up.
    H1Bdry = new NAbelianGroup();
    H1Bdry->addRank(rank);
    H1Bdry->addTorsionElement(2, z2rank);
    calculatedH1Bdry = true;
    return *H1Bdry;
}

const NAbelianGroup& NTriangulation::getHomologyH2() {
    if (calculatedH2)
        return *H2;

    if (getNumberOfTetrahedra() == 0) {
        H2 = new NAbelianGroup();
        calculatedH2 = true;
        return *H2;
    }

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
        ComponentIterator cit(components);
        while (! cit.done()) {
            if ((*cit)->isClosed())
                if (! ((*cit)->isOrientable()))
                    z2rank++;
            cit++;
        }

        // Find rank(Z_2) + rank(Z) and take off z2rank.
        rank = getHomologyH1Rel().getRank() +
            getHomologyH1Rel().getTorsionRank(2) -
            getHomologyH1().getTorsionRank(2) -
            z2rank;
    }
    
    // Build the new group and tidy up.
    H2 = new NAbelianGroup();
    H2->addRank(rank);
    if (z2rank)
        H2->addTorsionElement(2, z2rank);
    calculatedH2 = true;
    return *H2;
}

