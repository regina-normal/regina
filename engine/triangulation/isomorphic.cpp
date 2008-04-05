
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <map>
#include <queue>
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

std::auto_ptr<NIsomorphism> NTriangulation::isIsomorphicTo(
        const NTriangulation& other) const {
    std::list<NIsomorphism*> results;
    if (findIsomorphisms(other, results, true, true))
        return std::auto_ptr<NIsomorphism>(results.front());
    else
        return std::auto_ptr<NIsomorphism>(0);
}

std::auto_ptr<NIsomorphism> NTriangulation::isContainedIn(
        const NTriangulation& other) const {
    std::list<NIsomorphism*> results;
    if (findIsomorphisms(other, results, false, true))
        return std::auto_ptr<NIsomorphism>(results.front());
    else
        return std::auto_ptr<NIsomorphism>(0);
}

unsigned long NTriangulation::findAllSubcomplexesIn(
        const NTriangulation& other, std::list<NIsomorphism*>& results) const {
    return findIsomorphisms(other, results, false, false);
}

unsigned long NTriangulation::findIsomorphisms(
        const NTriangulation& other, std::list<NIsomorphism*>& results,
        bool completeIsomorphism, bool firstOnly) const {
    if (! calculatedSkeleton)
        calculateSkeleton();
    if (! other.calculatedSkeleton)
        other.calculateSkeleton();

    // Deal with the empty triangulation first.
    if (tetrahedra.empty()) {
        if (completeIsomorphism && ! other.tetrahedra.empty())
            return 0;
        results.push_back(new NIsomorphismDirect(0));
        return 1;
    }

    // Basic property checks.  Unfortunately, if we allow boundary
    // incomplete isomorphisms then we can't test that many properties.
    if (completeIsomorphism) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (tetrahedra.size() != other.tetrahedra.size())
            return 0;
        if (faces.size() != other.faces.size())
            return 0;
        if (edges.size() != other.edges.size())
            return 0;
        if (vertices.size() != other.vertices.size())
            return 0;
        if (components.size() != other.components.size())
            return 0;
        if (boundaryComponents.size() != other.boundaryComponents.size())
            return 0;
        if (orientable ^ other.orientable)
            return 0;

        // Test degree sequences and the like.
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
                return 0;
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
            for (it = other.vertices.begin();
                    it != other.vertices.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return 0;
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
                return 0;
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
                return 0;
            map1.clear();
            map2.clear();
        }
    } else {
        // May be boundary incomplete, and need not be onto.
        // Not much we can test for unfortunately.
        if (tetrahedra.size() > other.tetrahedra.size())
            return 0;
        if ((! orientable) && other.orientable)
            return 0;
    }

    // Start searching for the isomorphism.
    // From the tests above, we are guaranteed that both triangulations
    // have at least one tetrahedron.
    unsigned long nResults = 0;
    unsigned long nTetrahedra = tetrahedra.size();
    unsigned long nDestTetrahedra = other.tetrahedra.size();
    unsigned long nComponents = components.size();
    unsigned i;

    NIsomorphismDirect iso(nTetrahedra);
    for (i = 0; i < nTetrahedra; i++)
        iso.tetImage(i) = -1;

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
    unsigned long compSize;
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
        // Continue trying to find a mapping for the current component.
        // The next mapping to try is the one that starts with
        // startTet[comp] and startPerm[comp].
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            results.push_back(new NIsomorphismDirect(
                static_cast<NIsomorphism&>(iso)));

            if (firstOnly) {
                delete[] whichComp;
                delete[] startTet;
                delete[] startPerm;
                return 1;
            } else
                nResults++;

            // Back down to the previous component, and clear the
            // mapping for that previous component so we can make way
            // for a new one.
            // Since nComponents > 0, we are guaranteed that comp > 0 also.
            comp--;

            for (i = 0; i < nTetrahedra; i++)
                if (iso.tetImage(i) >= 0 &&
                        whichComp[iso.tetImage(i)] == comp) {
                    whichComp[iso.tetImage(i)] = -1;
                    iso.tetImage(i) = -1;
                }
            startPerm[comp]++;

            continue;
        }

        // Sort out the results of any previous startPerm++.
        if (startPerm[comp] == 24) {
            // Move on to the next destination tetrahedron.
            startTet[comp]++;
            startPerm[comp] = 0;
        }

        // Be sure we're looking at a tetrahedron we can use.
        compSize = components[comp]->getNumberOfTetrahedra();
        if (completeIsomorphism) {
            // Conditions:
            // 1) The destination tetrahedron is unused.
            // 2) The component sizes match precisely.
            while (startTet[comp] < nDestTetrahedra &&
                    (whichComp[startTet[comp]] >= 0 ||
                     other.tetrahedra[startTet[comp]]->getComponent()->
                     getNumberOfTetrahedra() != compSize))
                startTet[comp]++;
        } else {
            // Conditions:
            // 1) The destination tetrahedron is unused.
            // 2) The destination component is at least as large as
            // the source component.
            while (startTet[comp] < nDestTetrahedra &&
                    (whichComp[startTet[comp]] >= 0 ||
                     other.tetrahedra[startTet[comp]]->getComponent()->
                     getNumberOfTetrahedra() < compSize))
                startTet[comp]++;
        }

        // Have we run out of possibilities?
        if (startTet[comp] == nDestTetrahedra) {
            // No more possibilities for filling this component.
            // Move back to the previous component, and clear the
            // mapping for that previous component.
            startTet[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nTetrahedra; i++)
                    if (iso.tetImage(i) >= 0 &&
                            whichComp[iso.tetImage(i)] == comp) {
                        whichComp[iso.tetImage(i)] = -1;
                        iso.tetImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source tetrahedron.
        // Note that there is only one way of doing this (as seen by
        // following adjacent tetrahedron gluings).  It either works or
        // it doesn't.
        tetIndex = tetrahedronIndex(components[comp]->getTetrahedron(0));

        whichComp[startTet[comp]] = comp;
        iso.tetImage(tetIndex) = startTet[comp];
        iso.facePerm(tetIndex) = allPermsS4[startPerm[comp]];
        toProcess.push(tetIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            tetIndex = toProcess.front();
            toProcess.pop();
            tet = tetrahedra[tetIndex];
            tetPerm = iso.facePerm(tetIndex);
            destTetIndex = iso.tetImage(tetIndex);
            destTet = other.tetrahedra[destTetIndex];

            // If we are after a complete isomorphism, we might as well
            // test whether the edge and vertex degrees match.
            if (completeIsomorphism && ! compatibleTets(tet, destTet,
                    tetPerm)) {
                broken = true;
                break;
            }

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
                    adjIndex = tetrahedronIndex(adj);
                    destAdjIndex = other.tetrahedronIndex(destAdj);
                    adjPerm =
                        destTet->getAdjacentTetrahedronGluing(tetPerm[face]) *
                        tetPerm *
                        tet->getAdjacentTetrahedronGluing(face).inverse();

                    if (iso.tetImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source tetrahedron.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso.tetImage(adjIndex) ||
                                adjPerm != iso.facePerm(adjIndex)) {
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
                        iso.tetImage(adjIndex) = destAdjIndex;
                        iso.facePerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                } else if (completeIsomorphism) {
                    // There is no adjacent source tetrahedron, and we
                    // are after a boundary complete isomorphism.
                    // There had better be no adjacent destination
                    // tetrahedron also.
                    if (destTet->getAdjacentTetrahedron(tetPerm[face])) {
                        broken = true;
                        break;
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
                if (iso.tetImage(i) >= 0 &&
                        whichComp[iso.tetImage(i)] == comp) {
                    whichComp[iso.tetImage(i)] = -1;
                    iso.tetImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // All out of options.
    delete[] whichComp;
    delete[] startTet;
    delete[] startPerm;
    return nResults;
}

bool NTriangulation::compatibleTets(NTetrahedron* src, NTetrahedron* dest,
        NPerm p) {
    for (int edge = 0; edge < 6; edge++) {
        if (src->getEdge(edge)->getNumberOfEmbeddings() !=
                dest->getEdge(edgeNumber[p[edgeStart[edge]]][p[edgeEnd[edge]]])
                ->getNumberOfEmbeddings())
            return false;
    }

    for (int vertex = 0; vertex < 4; vertex++) {
        if (src->getVertex(vertex)->getNumberOfEmbeddings() !=
                dest->getVertex(p[vertex])->getNumberOfEmbeddings())
            return false;
        if (src->getVertex(vertex)->getLink() !=
                dest->getVertex(p[vertex])->getLink())
            return false;
    }

    return true;
}

} // namespace regina

