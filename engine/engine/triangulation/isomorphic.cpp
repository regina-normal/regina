
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <map>
#include <queue>
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

#define NO_ISOMORPHISM (std::auto_ptr<NIsomorphism>(0))

namespace regina {

std::auto_ptr<NIsomorphism> NTriangulation::isIsomorphicTo(
        const NTriangulation& other) const {
    if (! calculatedSkeleton)
        calculateSkeleton();
    if (! other.calculatedSkeleton)
        other.calculateSkeleton();

    if (tetrahedra.size() != other.tetrahedra.size())
        return NO_ISOMORPHISM;
    if (tetrahedra.empty())
        return std::auto_ptr<NIsomorphism>(new NIsomorphismDirect(0));
    if (faces.size() != other.faces.size())
        return NO_ISOMORPHISM;
    if (edges.size() != other.edges.size())
        return NO_ISOMORPHISM;
    if (vertices.size() != other.vertices.size())
        return NO_ISOMORPHISM;
    if (components.size() != other.components.size())
        return NO_ISOMORPHISM;
    if (boundaryComponents.size() != other.boundaryComponents.size())
        return NO_ISOMORPHISM;
    if (orientable ^ other.orientable)
        return NO_ISOMORPHISM;

    std::map<unsigned long, unsigned long> map1;
    std::map<unsigned long, unsigned long> map2;
    std::map<unsigned long, unsigned long>::iterator mapIt;

    {
        EdgeIterator it;
        for (it = edges.begin(); it != edges.end(); it++) {
            // Find this degree, or insert it with frequency 0 if it's
            // not already present.
            mapIt = map1.insert(
                std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
            (*mapIt).second++;
        }
        for (it = other.edges.begin(); it != other.edges.end(); it++) {
            mapIt = map2.insert(
                std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
            (*mapIt).second++;
        }
        if (! (map1 == map2))
            return NO_ISOMORPHISM;
        map1.clear();
        map2.clear();
    }
    {
        VertexIterator it;
        for (it = vertices.begin(); it != vertices.end(); it++) {
            mapIt = map1.insert(
                std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
            (*mapIt).second++;
        }
        for (it = other.vertices.begin(); it != other.vertices.end(); it++) {
            mapIt = map2.insert(
                std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
            (*mapIt).second++;
        }
        if (! (map1 == map2))
            return NO_ISOMORPHISM;
        map1.clear();
        map2.clear();
    }
    {
        ComponentIterator it;
        for (it = components.begin(); it != components.end(); it++) {
            mapIt = map1.insert(
                std::make_pair((*it)->getNumberOfTetrahedra(), 0)).first;
            (*mapIt).second++;
        }
        for (it = other.components.begin();
                it != other.components.end(); it++) {
            mapIt = map2.insert(
                std::make_pair((*it)->getNumberOfTetrahedra(), 0)).first;
            (*mapIt).second++;
        }
        if (! (map1 == map2))
            return NO_ISOMORPHISM;
        map1.clear();
        map2.clear();
    }
    {
        BoundaryComponentIterator it;
        for (it = boundaryComponents.begin();
                it != boundaryComponents.end(); it++) {
            mapIt = map1.insert(
                std::make_pair((*it)->getNumberOfFaces(), 0)).first;
            (*mapIt).second++;
        }
        for (it = other.boundaryComponents.begin();
                it != other.boundaryComponents.end(); it++) {
            mapIt = map2.insert(
                std::make_pair((*it)->getNumberOfFaces(), 0)).first;
            (*mapIt).second++;
        }
        if (! (map1 == map2))
            return NO_ISOMORPHISM;
        map1.clear();
        map2.clear();
    }

    // Try to make an exact matching.
    unsigned long nTetrahedra = tetrahedra.size();
    std::auto_ptr<NIsomorphismIndexed> iso(
        new NIsomorphismIndexed(nTetrahedra));
    bool* used = new bool[nTetrahedra];
    for (unsigned long i=0; i<nTetrahedra; i++) {
        used[i] = false;
        iso->tetImage(i) = 0;
        iso->facePermIndex(i) = 0;
    }

    bool canJoin;
    int edge, vertex, face;
    long tetIndex;
    NPerm tetPerm, adjPerm;
    NTetrahedron* myNbr;
    NTetrahedron* yourNbr;
    long mustMatch = 0;
    used[0] = true;
    while (1) {
        if (iso->facePermIndex(mustMatch) == 24) {
            iso->facePermIndex(mustMatch) = 0;
            used[iso->tetImage(mustMatch)] = false;
            iso->tetImage(mustMatch)++;
            while (iso->tetImage(mustMatch) < static_cast<long>(nTetrahedra))
                if (used[iso->tetImage(mustMatch)])
                    iso->tetImage(mustMatch)++;
                else
                    break;
            if (iso->tetImage(mustMatch) == static_cast<long>(nTetrahedra)) {
                iso->tetImage(mustMatch) = 0;
                if (mustMatch == 0)
                    break;
                mustMatch--;
                iso->facePermIndex(mustMatch)++;
                continue;
            } else {
                used[iso->tetImage(mustMatch)] = true;
            }
        }

        // Try to join mustMatch with iso->tetImage(mustMatch) according to
        // permutation iso->facePermIndex(mustMatch).

        canJoin = true;
        tetPerm = allPermsS4[iso->facePermIndex(mustMatch)];

        for (edge = 0; edge < 6; edge++) {
            if (tetrahedra[mustMatch]->getEdge(edge)
                    ->getNumberOfEmbeddings() !=
                    other.tetrahedra[iso->tetImage(mustMatch)]
                    ->getEdge(edgeNumber[tetPerm[edgeStart[edge]]]
                    [tetPerm[edgeEnd[edge]]])
                    ->getNumberOfEmbeddings()) {
                canJoin = false;
                break;
            }
        }

        if (canJoin) {
            for (vertex = 0; vertex < 4; vertex++) {
                if (tetrahedra[mustMatch]->getVertex(vertex)
                        ->getNumberOfEmbeddings() !=
                        other.tetrahedra[iso->tetImage(mustMatch)]
                        ->getVertex(tetPerm[vertex])
                        ->getNumberOfEmbeddings()) {
                    canJoin = false;
                    break;
                }
                if (tetrahedra[mustMatch]->getVertex(vertex)
                        ->getLink() !=
                        other.tetrahedra[iso->tetImage(mustMatch)]
                        ->getVertex(tetPerm[vertex])
                        ->getLink()) {
                    canJoin = false;
                    break;
                }
            }
        }

        if (canJoin) {
            for (face = 0; face < 4; face++) {
                myNbr = tetrahedra[mustMatch]->
                    getAdjacentTetrahedron(face);
                yourNbr = other.tetrahedra[iso->tetImage(mustMatch)]->
                    getAdjacentTetrahedron(tetPerm[face]);
                if ((myNbr && (! yourNbr)) || (yourNbr && (! myNbr))) {
                    // One of the triangulations has a boundary face here
                    // and the other does not!
                    canJoin = false;
                    break;
                }
                if (myNbr) {
                    // Both tetrahedra have adjacent tetrahedra; check
                    // these map to each other also.
                    tetIndex = tetrahedra.index(myNbr);
                    if (tetIndex <= mustMatch) {
                        adjPerm = allPermsS4[iso->facePermIndex(tetIndex)];
                        if (yourNbr !=
                                other.tetrahedra[iso->tetImage(tetIndex)]) {
                            canJoin = false;
                            break;
                        }
                        if (! ((adjPerm * tetrahedra[mustMatch]->
                                getAdjacentTetrahedronGluing(face)) ==
                                (other.tetrahedra[iso->tetImage(mustMatch)]->
                                getAdjacentTetrahedronGluing(tetPerm[face]) *
                                tetPerm))) {
                            canJoin = false;
                            break;
                        }
                    }
                }
            }
        }

        if (canJoin) {
            mustMatch++;
            if (mustMatch == static_cast<long>(nTetrahedra)) {
                delete[] used;
                // Explicitly do the base class conversion since
                // otherwise we seem to get compile errors. *sigh*
                return std::auto_ptr<NIsomorphism>(iso);
            }
            used[iso->tetImage(mustMatch)] = true;
        } else
            iso->facePermIndex(mustMatch)++;
    }

    delete[] used;
    return NO_ISOMORPHISM;
}

std::auto_ptr<NIsomorphism> NTriangulation::isContainedIn(
        const NTriangulation& other) const {
    // Basic property checks.  Unfortunately, since the isomorphism need
    // not be boundary complete we can't test that many properties.
    if (! calculatedSkeleton)
        calculateSkeleton();
    if (! other.calculatedSkeleton)
        other.calculateSkeleton();

    if (tetrahedra.size() > other.tetrahedra.size())
        return NO_ISOMORPHISM;
    if (tetrahedra.empty())
        return std::auto_ptr<NIsomorphism>(new NIsomorphismDirect(0));
    if ((! orientable) && other.orientable)
        return NO_ISOMORPHISM;

    // Start searching for the isomorphism.
    unsigned long nTetrahedra = tetrahedra.size();
    unsigned long nDestTetrahedra = other.tetrahedra.size();
    unsigned long nComponents = components.size();
    unsigned i;

    std::auto_ptr<NIsomorphismDirect> iso(new NIsomorphismDirect(nTetrahedra));
    for (i = 0; i < nTetrahedra; i++)
        iso->tetImage(i) = -1;

    // Which source component does each destination component correspond to?
    long* whichComp = new long[nDestTetrahedra];
    std::fill(whichComp, whichComp + nDestTetrahedra, -1);

    // The image of the first source tetrahedron of each component.  The
    // remaining images can be derived by following gluings.
    unsigned long* startTet = new unsigned long[nComponents];
    std::fill(startTet, startTet + nComponents, 0);

    int* startPerm = new int[nComponents];
    std::fill(startPerm, startPerm + nComponents, 0);

    // The tetrahedra whose neighbours must be processed when filling
    // out the current component.
    std::queue<long> toProcess;

    // Temporary variables.
    NTetrahedron* tet;
    NTetrahedron* adj;
    NTetrahedron* destTet;
    NTetrahedron* destAdj;
    unsigned long tetIndex, adjIndex;
    unsigned long destTetIndex, destAdjIndex;
    NPerm tetPerm, adjPerm;
    int face;
    bool broken;

    long comp = 0;
    while (comp >= 0) {
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            delete[] whichComp;
            delete[] startTet;
            delete[] startPerm;
            return std::auto_ptr<NIsomorphism>(iso);
        }
        if (startPerm[comp] == 24) {
            // Move on to the next destination tetrahedron.
            startTet[comp]++;
            startPerm[comp] = 0;
        }
        // Make sure we're looking at an unused destination tetrahedron.
        while (startTet[comp] < nDestTetrahedra &&
                whichComp[startTet[comp]] >= 0)
            startTet[comp]++;
        if (startTet[comp] == nDestTetrahedra) {
            // We can't fill this component.
            // Move back to the previous component.
            startTet[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nTetrahedra; i++)
                    if (iso->tetImage(i) >= 0 &&
                            whichComp[iso->tetImage(i)] == comp) {
                        whichComp[iso->tetImage(i)] = -1;
                        iso->tetImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source tetrahedron.
        tetIndex = getTetrahedronIndex(components[comp]->getTetrahedron(0));

        whichComp[startTet[comp]] = comp;
        iso->tetImage(tetIndex) = startTet[comp];
        iso->facePerm(tetIndex) = allPermsS4[startPerm[comp]];
        toProcess.push(tetIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            tetIndex = toProcess.front();
            toProcess.pop();
            tet = tetrahedra[tetIndex];
            tetPerm = iso->facePerm(tetIndex);
            destTetIndex = iso->tetImage(tetIndex);
            destTet = other.tetrahedra[destTetIndex];

            for (face = 0; face < 4; face++) {
                adj = tet->getAdjacentTetrahedron(face);
                if (adj) {
                    // There is an adjacent source tetrahedron.
                    // Is there an adjacent destination tetrahedron?
                    destAdj = destTet->getAdjacentTetrahedron(tetPerm[face]);
                    if (! destAdj) {
                        broken = true;
                        break;
                    }
                    // Work out what the isomorphism *should* say.
                    adjIndex = getTetrahedronIndex(adj);
                    destAdjIndex = other.getTetrahedronIndex(destAdj);
                    adjPerm =
                        destTet->getAdjacentTetrahedronGluing(tetPerm[face]) *
                        tetPerm *
                        tet->getAdjacentTetrahedronGluing(face).inverse();

                    if (iso->tetImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source tetrahedron.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso->tetImage(adjIndex) ||
                                adjPerm != iso->facePerm(adjIndex)) {
                            broken = true;
                            break;
                        }
                    } else if (whichComp[destAdjIndex] >= 0) {
                        // We haven't decided upon an image for this
                        // source tetrahedron but the destination
                        // tetrahedron has already been used.
                        broken = true;
                        break;
                    } else {
                        // We haven't seen either the source or the
                        // destination tetrahedron.
                        whichComp[destAdjIndex] = comp;
                        iso->tetImage(adjIndex) = destAdjIndex;
                        iso->facePerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                }
            }
        }

        if (! broken) {
            // Therefore toProcess is empty.
            // The image for this component was successfully filled out.
            // Move on to the next component.
            comp++;
        } else {
            // The image for this component was not successfully filled out.
            // Undo our partially created image, and then try another
            // starting image for this component.
            while (! toProcess.empty())
                toProcess.pop();

            for (i = 0; i < nTetrahedra; i++)
                if (iso->tetImage(i) >= 0 &&
                        whichComp[iso->tetImage(i)] == comp) {
                    whichComp[iso->tetImage(i)] = -1;
                    iso->tetImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // We ran out of options.
    delete[] whichComp;
    delete[] startTet;
    delete[] startPerm;
    return NO_ISOMORPHISM;
}

} // namespace regina

