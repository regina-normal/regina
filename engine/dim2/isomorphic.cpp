
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
    if (triangles_.empty()) {
        if (completeIsomorphism && ! other.triangles_.empty())
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
        if (triangles_.size() != other.triangles_.size())
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
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components_.begin();
                    it != other.components_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
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
        if (triangles_.size() > other.triangles_.size())
            return 0;
        if ((! orientable_) && other.orientable_)
            return 0;
    }

    // Start searching for the isomorphism.
    // From the tests above, we are guaranteed that both triangulations
    // have at least one triangle.
    unsigned long nResults = 0;
    unsigned long nTriangles = triangles_.size();
    unsigned long nDestTriangles = other.triangles_.size();
    unsigned long nComponents = components_.size();
    unsigned i;

    Dim2Isomorphism iso(nTriangles);
    for (i = 0; i < nTriangles; i++)
        iso.simpImage(i) = -1;

    // Which source component does each destination triangle correspond to?
    long* whichComp = new long[nDestTriangles];
    std::fill(whichComp, whichComp + nDestTriangles, -1);

    // The image of the first source triangle of each component.  The
    // remaining images can be derived by following gluings.
    unsigned long* startTri = new unsigned long[nComponents];
    std::fill(startTri, startTri + nComponents, 0);

    int* startPerm = new int[nComponents];
    std::fill(startPerm, startPerm + nComponents, 0);

    // The triangles whose neighbours must be processed when filling
    // out the current component.
    std::queue<long> toProcess;

    // Temporary variables.
    unsigned long compSize;
    Dim2Triangle* tri;
    Dim2Triangle* adj;
    Dim2Triangle* destTri;
    Dim2Triangle* destAdj;
    unsigned long myTriIndex, adjIndex;
    unsigned long destTriIndex, destAdjIndex;
    NPerm3 triPerm, adjPerm;
    int edge;
    bool broken;

    long comp = 0;
    while (comp >= 0) {
        // Continue trying to find a mapping for the current component.
        // The next mapping to try is the one that starts with
        // startTri[comp] and startPerm[comp].
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            results.push_back(new Dim2Isomorphism(iso));

            if (firstOnly) {
                delete[] whichComp;
                delete[] startTri;
                delete[] startPerm;
                return 1;
            } else
                nResults++;

            // Back down to the previous component, and clear the
            // mapping for that previous component so we can make way
            // for a new one.
            // Since nComponents > 0, we are guaranteed that comp > 0 also.
            comp--;

            for (i = 0; i < nTriangles; i++)
                if (iso.simpImage(i) >= 0 &&
                        whichComp[iso.simpImage(i)] == comp) {
                    whichComp[iso.simpImage(i)] = -1;
                    iso.simpImage(i) = -1;
                }
            startPerm[comp]++;

            continue;
        }

        // Sort out the results of any previous startPerm++.
        if (startPerm[comp] == 6) {
            // Move on to the next destination triangle.
            startTri[comp]++;
            startPerm[comp] = 0;
        }

        // Be sure we're looking at a triangle we can use.
        compSize = components_[comp]->getNumberOfTriangles();
        if (completeIsomorphism) {
            // Conditions:
            // 1) The destination triangle is unused.
            // 2) The component sizes match precisely.
            while (startTri[comp] < nDestTriangles &&
                    (whichComp[startTri[comp]] >= 0 ||
                     other.triangles_[startTri[comp]]->getComponent()->
                     getNumberOfTriangles() != compSize))
                startTri[comp]++;
        } else {
            // Conditions:
            // 1) The destination triangle is unused.
            // 2) The destination component is at least as large as
            // the source component.
            while (startTri[comp] < nDestTriangles &&
                    (whichComp[startTri[comp]] >= 0 ||
                     other.triangles_[startTri[comp]]->getComponent()->
                     getNumberOfTriangles() < compSize))
                startTri[comp]++;
        }

        // Have we run out of possibilities?
        if (startTri[comp] == nDestTriangles) {
            // No more possibilities for filling this component.
            // Move back to the previous component, and clear the
            // mapping for that previous component.
            startTri[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nTriangles; i++)
                    if (iso.simpImage(i) >= 0 &&
                            whichComp[iso.simpImage(i)] == comp) {
                        whichComp[iso.simpImage(i)] = -1;
                        iso.simpImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source triangle.
        // Note that there is only one way of doing this (as seen by
        // following adjacent triangle gluings).  It either works or
        // it doesn't.
        myTriIndex = triangleIndex(components_[comp]->getTriangle(0));

        whichComp[startTri[comp]] = comp;
        iso.simpImage(myTriIndex) = startTri[comp];
        iso.facetPerm(myTriIndex) = NPerm3::S3[startPerm[comp]];
        toProcess.push(myTriIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            myTriIndex = toProcess.front();
            toProcess.pop();
            tri = triangles_[myTriIndex];
            triPerm = iso.facetPerm(myTriIndex);
            destTriIndex = iso.simpImage(myTriIndex);
            destTri = other.triangles_[destTriIndex];

            // If we are after a complete isomorphism, we might as well
            // test whether the lower-dimensional face degrees match.
            if (completeIsomorphism && ! compatibleTriangles(tri, destTri,
                    triPerm)) {
                broken = true;
                break;
            }

            for (edge = 0; edge < 3; ++edge) {
                adj = tri->adjacentTriangle(edge);
                if (adj) {
                    // There is an adjacent source triangle.
                    // Is there an adjacent destination triangle?
                    destAdj = destTri->adjacentTriangle(triPerm[edge]);
                    if (! destAdj) {
                        broken = true;
                        break;
                    }
                    // Work out what the isomorphism *should* say.
                    adjIndex = triangleIndex(adj);
                    destAdjIndex = other.triangleIndex(destAdj);
                    adjPerm =
                        destTri->adjacentGluing(triPerm[edge]) *
                        triPerm *
                        tri->adjacentGluing(edge).inverse();

                    if (iso.simpImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source triangle.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso.simpImage(adjIndex) ||
                                adjPerm != iso.facetPerm(adjIndex)) {
                            broken = true;
                            break;
                        }
                    } else if (whichComp[destAdjIndex] >= 0) {
                        // We haven't decided upon an image for this
                        // source triangle but the destination
                        // triangle has already been used.
                        broken = true;
                        break;
                    } else {
                        // We haven't seen either the source or the
                        // destination triangle.
                        whichComp[destAdjIndex] = comp;
                        iso.simpImage(adjIndex) = destAdjIndex;
                        iso.facetPerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                } else if (completeIsomorphism) {
                    // There is no adjacent source triangle, and we
                    // are after a boundary complete isomorphism.
                    // There had better be no adjacent destination
                    // triangle also.
                    if (destTri->adjacentTriangle(triPerm[edge])) {
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

            for (i = 0; i < nTriangles; i++)
                if (iso.simpImage(i) >= 0 &&
                        whichComp[iso.simpImage(i)] == comp) {
                    whichComp[iso.simpImage(i)] = -1;
                    iso.simpImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // All out of options.
    delete[] whichComp;
    delete[] startTri;
    delete[] startPerm;
    return nResults;
}

bool Dim2Triangulation::compatibleTriangles(
        Dim2Triangle* src, Dim2Triangle* dest, NPerm3 p) {
    for (int vertex = 0; vertex < 3; vertex++)
        if (src->getVertex(vertex)->getNumberOfEmbeddings() !=
                dest->getVertex(p[vertex])->getNumberOfEmbeddings())
            return false;

    return true;
}

} // namespace regina

