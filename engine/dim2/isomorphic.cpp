
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

namespace regina {

std::auto_ptr<Dim2Isomorphism> Dim2Triangulation::isIsomorphicTo(
        const Dim2Triangulation& other) const {
    std::list<Dim2Isomorphism*> results;
    if (findIsomorphisms(other, results, true, true))
        return std::auto_ptr<Dim2Isomorphism>(results.front());
    else
        return std::auto_ptr<Dim2Isomorphism>(0);
}

std::auto_ptr<Dim2Isomorphism> Dim2Triangulation::isContainedIn(
        const Dim2Triangulation& other) const {
    std::list<Dim2Isomorphism*> results;
    if (findIsomorphisms(other, results, false, true))
        return std::auto_ptr<Dim2Isomorphism>(results.front());
    else
        return std::auto_ptr<Dim2Isomorphism>(0);
}

unsigned long Dim2Triangulation::findAllSubcomplexesIn(
        const Dim2Triangulation& other, std::list<Dim2Isomorphism*>& results)
        const {
    return findIsomorphisms(other, results, false, false);
}

unsigned long Dim2Triangulation::findIsomorphisms(
        const Dim2Triangulation& other, std::list<Dim2Isomorphism*>& results,
        bool completeIsomorphism, bool firstOnly) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    if (! other.calculatedSkeleton_)
        other.calculateSkeleton();

    // Deal with the empty triangulation first.
    if (faces_.empty()) {
        if (completeIsomorphism && ! other.faces_.empty())
            return 0;
        results.push_back(new Dim2Isomorphism(0));
        return 1;
    }

    // Basic property checks.  Unfortunately, if we allow boundary
    // incomplete isomorphisms then we can't test that many properties.
    if (completeIsomorphism) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (faces_.size() != other.faces_.size())
            return 0;
        if (edges_.size() != other.edges_.size())
            return 0;
        if (vertices_.size() != other.vertices_.size())
            return 0;
        if (components_.size() != other.components_.size())
            return 0;
        if (boundaryComponents_.size() != other.boundaryComponents_.size())
            return 0;
        if (orientable_ ^ other.orientable_)
            return 0;

        // Test degree sequences and the like.
        std::map<unsigned long, unsigned long> map1;
        std::map<unsigned long, unsigned long> map2;
        std::map<unsigned long, unsigned long>::iterator mapIt;

        {
            VertexIterator it;
            for (it = vertices_.begin(); it != vertices_.end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.vertices_.begin();
                    it != other.vertices_.end(); it++) {
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
            for (it = components_.begin(); it != components_.end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfFaces(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components_.begin();
                    it != other.components_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfFaces(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return 0;
            map1.clear();
            map2.clear();
        }
        {
            BoundaryComponentIterator it;
            for (it = boundaryComponents_.begin();
                    it != boundaryComponents_.end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfEdges(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.boundaryComponents_.begin();
                    it != other.boundaryComponents_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfEdges(), 0)).first;
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
        if (faces_.size() > other.faces_.size())
            return 0;
        if ((! orientable_) && other.orientable_)
            return 0;
    }

    // Start searching for the isomorphism.
    // From the tests above, we are guaranteed that both triangulations
    // have at least one face.
    unsigned long nResults = 0;
    unsigned long nFaces = faces_.size();
    unsigned long nDestFaces = other.faces_.size();
    unsigned long nComponents = components_.size();
    unsigned i;

    Dim2Isomorphism iso(nFaces);
    for (i = 0; i < nFaces; i++)
        iso.faceImage(i) = -1;

    // Which source component does each destination face correspond to?
    long* whichComp = new long[nDestFaces];
    std::fill(whichComp, whichComp + nDestFaces, -1);

    // The image of the first source face of each component.  The
    // remaining images can be derived by following gluings.
    unsigned long* startFace = new unsigned long[nComponents];
    std::fill(startFace, startFace + nComponents, 0);

    int* startPerm = new int[nComponents];
    std::fill(startPerm, startPerm + nComponents, 0);

    // The faces whose neighbours must be processed when filling
    // out the current component.
    std::queue<long> toProcess;

    // Temporary variables.
    unsigned long compSize;
    Dim2Face* face;
    Dim2Face* adj;
    Dim2Face* destFace;
    Dim2Face* destAdj;
    unsigned long myFaceIndex, adjIndex;
    unsigned long destFaceIndex, destAdjIndex;
    NPerm3 facePerm, adjPerm;
    int edge;
    bool broken;

    long comp = 0;
    while (comp >= 0) {
        // Continue trying to find a mapping for the current component.
        // The next mapping to try is the one that starts with
        // startFace[comp] and startPerm[comp].
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            results.push_back(new Dim2Isomorphism(iso));

            if (firstOnly) {
                delete[] whichComp;
                delete[] startFace;
                delete[] startPerm;
                return 1;
            } else
                nResults++;

            // Back down to the previous component, and clear the
            // mapping for that previous component so we can make way
            // for a new one.
            // Since nComponents > 0, we are guaranteed that comp > 0 also.
            comp--;

            for (i = 0; i < nFaces; i++)
                if (iso.faceImage(i) >= 0 &&
                        whichComp[iso.faceImage(i)] == comp) {
                    whichComp[iso.faceImage(i)] = -1;
                    iso.faceImage(i) = -1;
                }
            startPerm[comp]++;

            continue;
        }

        // Sort out the results of any previous startPerm++.
        if (startPerm[comp] == 6) {
            // Move on to the next destination face.
            startFace[comp]++;
            startPerm[comp] = 0;
        }

        // Be sure we're looking at a face we can use.
        compSize = components_[comp]->getNumberOfFaces();
        if (completeIsomorphism) {
            // Conditions:
            // 1) The destination face is unused.
            // 2) The component sizes match precisely.
            while (startFace[comp] < nDestFaces &&
                    (whichComp[startFace[comp]] >= 0 ||
                     other.faces_[startFace[comp]]->getComponent()->
                     getNumberOfFaces() != compSize))
                startFace[comp]++;
        } else {
            // Conditions:
            // 1) The destination face is unused.
            // 2) The destination component is at least as large as
            // the source component.
            while (startFace[comp] < nDestFaces &&
                    (whichComp[startFace[comp]] >= 0 ||
                     other.faces_[startFace[comp]]->getComponent()->
                     getNumberOfFaces() < compSize))
                startFace[comp]++;
        }

        // Have we run out of possibilities?
        if (startFace[comp] == nDestFaces) {
            // No more possibilities for filling this component.
            // Move back to the previous component, and clear the
            // mapping for that previous component.
            startFace[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nFaces; i++)
                    if (iso.faceImage(i) >= 0 &&
                            whichComp[iso.faceImage(i)] == comp) {
                        whichComp[iso.faceImage(i)] = -1;
                        iso.faceImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source face.
        // Note that there is only one way of doing this (as seen by
        // following adjacent face gluings).  It either works or
        // it doesn't.
        myFaceIndex = faceIndex(components_[comp]->getFace(0));

        whichComp[startFace[comp]] = comp;
        iso.faceImage(myFaceIndex) = startFace[comp];
        iso.edgePerm(myFaceIndex) = NPerm3::S3[startPerm[comp]];
        toProcess.push(myFaceIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            myFaceIndex = toProcess.front();
            toProcess.pop();
            face = faces_[myFaceIndex];
            facePerm = iso.edgePerm(myFaceIndex);
            destFaceIndex = iso.faceImage(myFaceIndex);
            destFace = other.faces_[destFaceIndex];

            // If we are after a complete isomorphism, we might as well
            // test whether the lower-dimensional face degrees match.
            if (completeIsomorphism && ! compatibleFaces(face, destFace,
                    facePerm)) {
                broken = true;
                break;
            }

            for (edge = 0; edge < 3; ++edge) {
                adj = face->adjacentFace(edge);
                if (adj) {
                    // There is an adjacent source face.
                    // Is there an adjacent destination face?
                    destAdj = destFace->adjacentFace(facePerm[edge]);
                    if (! destAdj) {
                        broken = true;
                        break;
                    }
                    // Work out what the isomorphism *should* say.
                    adjIndex = faceIndex(adj);
                    destAdjIndex = other.faceIndex(destAdj);
                    adjPerm =
                        destFace->adjacentGluing(facePerm[edge]) *
                        facePerm *
                        face->adjacentGluing(edge).inverse();

                    if (iso.faceImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source face.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso.faceImage(adjIndex) ||
                                adjPerm != iso.edgePerm(adjIndex)) {
                            broken = true;
                            break;
                        }
                    } else if (whichComp[destAdjIndex] >= 0) {
                        // We haven't decided upon an image for this
                        // source face but the destination
                        // face has already been used.
                        broken = true;
                        break;
                    } else {
                        // We haven't seen either the source or the
                        // destination face.
                        whichComp[destAdjIndex] = comp;
                        iso.faceImage(adjIndex) = destAdjIndex;
                        iso.edgePerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                } else if (completeIsomorphism) {
                    // There is no adjacent source face, and we
                    // are after a boundary complete isomorphism.
                    // There had better be no adjacent destination
                    // face also.
                    if (destFace->adjacentFace(facePerm[edge])) {
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

            for (i = 0; i < nFaces; i++)
                if (iso.faceImage(i) >= 0 &&
                        whichComp[iso.faceImage(i)] == comp) {
                    whichComp[iso.faceImage(i)] = -1;
                    iso.faceImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // All out of options.
    delete[] whichComp;
    delete[] startFace;
    delete[] startPerm;
    return nResults;
}

bool Dim2Triangulation::compatibleFaces(
        Dim2Face* src, Dim2Face* dest, NPerm3 p) {
    for (int vertex = 0; vertex < 3; vertex++)
        if (src->getVertex(vertex)->getNumberOfEmbeddings() !=
                dest->getVertex(p[vertex])->getNumberOfEmbeddings())
            return false;

    return true;
}

} // namespace regina

