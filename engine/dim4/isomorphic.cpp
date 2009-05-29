
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

namespace regina {

std::auto_ptr<Dim4Isomorphism> Dim4Triangulation::isIsomorphicTo(
        const Dim4Triangulation& other) const {
    std::list<Dim4Isomorphism*> results;
    if (findIsomorphisms(other, results, true, true))
        return std::auto_ptr<Dim4Isomorphism>(results.front());
    else
        return std::auto_ptr<Dim4Isomorphism>(0);
}

std::auto_ptr<Dim4Isomorphism> Dim4Triangulation::isContainedIn(
        const Dim4Triangulation& other) const {
    std::list<Dim4Isomorphism*> results;
    if (findIsomorphisms(other, results, false, true))
        return std::auto_ptr<Dim4Isomorphism>(results.front());
    else
        return std::auto_ptr<Dim4Isomorphism>(0);
}

unsigned long Dim4Triangulation::findAllSubcomplexesIn(
        const Dim4Triangulation& other, std::list<Dim4Isomorphism*>& results)
        const {
    return findIsomorphisms(other, results, false, false);
}

unsigned long Dim4Triangulation::findIsomorphisms(
        const Dim4Triangulation& other, std::list<Dim4Isomorphism*>& results,
        bool completeIsomorphism, bool firstOnly) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();
    if (! other.calculatedSkeleton_)
        other.calculateSkeleton();

    // Deal with the empty triangulation first.
    if (pentachora_.empty()) {
        if (completeIsomorphism && ! other.pentachora_.empty())
            return 0;
        results.push_back(new Dim4Isomorphism(0));
        return 1;
    }

    // Basic property checks.  Unfortunately, if we allow boundary
    // incomplete isomorphisms then we can't test that many properties.
    if (completeIsomorphism) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (pentachora_.size() != other.pentachora_.size())
            return 0;
        if (tetrahedra_.size() != other.tetrahedra_.size())
            return 0;
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
            FaceIterator it;
            for (it = faces_.begin(); it != faces_.end(); it++) {
                // Find this degree, or insert it with frequency 0 if it's
                // not already present.
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.faces_.begin(); it != other.faces_.end(); it++) {
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
            EdgeIterator it;
            for (it = edges_.begin(); it != edges_.end(); it++) {
                // Find this degree, or insert it with frequency 0 if it's
                // not already present.
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfEmbeddings(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.edges_.begin(); it != other.edges_.end(); it++) {
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
                    std::make_pair((*it)->getNumberOfPentachora(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components_.begin();
                    it != other.components_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfPentachora(), 0)).first;
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
                    std::make_pair((*it)->getNumberOfTetrahedra(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.boundaryComponents_.begin();
                    it != other.boundaryComponents_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfTetrahedra(), 0)).first;
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
        if (pentachora_.size() > other.pentachora_.size())
            return 0;
        if ((! orientable_) && other.orientable_)
            return 0;
    }

    // Start searching for the isomorphism.
    // From the tests above, we are guaranteed that both triangulations
    // have at least one pentachoron.
    unsigned long nResults = 0;
    unsigned long nPentachora = pentachora_.size();
    unsigned long nDestPentachora = other.pentachora_.size();
    unsigned long nComponents = components_.size();
    unsigned i;

    Dim4Isomorphism iso(nPentachora);
    for (i = 0; i < nPentachora; i++)
        iso.pentImage(i) = -1;

    // Which source component does each destination pentachoron correspond to?
    long* whichComp = new long[nDestPentachora];
    std::fill(whichComp, whichComp + nDestPentachora, -1);

    // The image of the first source pentachoron of each component.  The
    // remaining images can be derived by following gluings.
    unsigned long* startPent = new unsigned long[nComponents];
    std::fill(startPent, startPent + nComponents, 0);

    int* startPerm = new int[nComponents];
    std::fill(startPerm, startPerm + nComponents, 0);

    // The pentachora whose neighbours must be processed when filling
    // out the current component.
    std::queue<long> toProcess;

    // Temporary variables.
    unsigned long compSize;
    Dim4Pentachoron* pent;
    Dim4Pentachoron* adj;
    Dim4Pentachoron* destPent;
    Dim4Pentachoron* destAdj;
    unsigned long pentIndex, adjIndex;
    unsigned long destPentIndex, destAdjIndex;
    NPerm5 pentPerm, adjPerm;
    int facet;
    bool broken;

    long comp = 0;
    while (comp >= 0) {
        // Continue trying to find a mapping for the current component.
        // The next mapping to try is the one that starts with
        // startPent[comp] and startPerm[comp].
        if (comp == static_cast<long>(nComponents)) {
            // We have an isomorphism!!!
            results.push_back(new Dim4Isomorphism(iso));

            if (firstOnly) {
                delete[] whichComp;
                delete[] startPent;
                delete[] startPerm;
                return 1;
            } else
                nResults++;

            // Back down to the previous component, and clear the
            // mapping for that previous component so we can make way
            // for a new one.
            // Since nComponents > 0, we are guaranteed that comp > 0 also.
            comp--;

            for (i = 0; i < nPentachora; i++)
                if (iso.pentImage(i) >= 0 &&
                        whichComp[iso.pentImage(i)] == comp) {
                    whichComp[iso.pentImage(i)] = -1;
                    iso.pentImage(i) = -1;
                }
            startPerm[comp]++;

            continue;
        }

        // Sort out the results of any previous startPerm++.
        if (startPerm[comp] == 120) {
            // Move on to the next destination pentachoron.
            startPent[comp]++;
            startPerm[comp] = 0;
        }

        // Be sure we're looking at a pentachoron we can use.
        compSize = components_[comp]->getNumberOfPentachora();
        if (completeIsomorphism) {
            // Conditions:
            // 1) The destination pentachoron is unused.
            // 2) The component sizes match precisely.
            while (startPent[comp] < nDestPentachora &&
                    (whichComp[startPent[comp]] >= 0 ||
                     other.pentachora_[startPent[comp]]->getComponent()->
                     getNumberOfPentachora() != compSize))
                startPent[comp]++;
        } else {
            // Conditions:
            // 1) The destination pentachoron is unused.
            // 2) The destination component is at least as large as
            // the source component.
            while (startPent[comp] < nDestPentachora &&
                    (whichComp[startPent[comp]] >= 0 ||
                     other.pentachora_[startPent[comp]]->getComponent()->
                     getNumberOfPentachora() < compSize))
                startPent[comp]++;
        }

        // Have we run out of possibilities?
        if (startPent[comp] == nDestPentachora) {
            // No more possibilities for filling this component.
            // Move back to the previous component, and clear the
            // mapping for that previous component.
            startPent[comp] = 0;
            startPerm[comp] = 0;

            comp--;
            if (comp >= 0) {
                for (i = 0; i < nPentachora; i++)
                    if (iso.pentImage(i) >= 0 &&
                            whichComp[iso.pentImage(i)] == comp) {
                        whichComp[iso.pentImage(i)] = -1;
                        iso.pentImage(i) = -1;
                    }
                startPerm[comp]++;
            }

            continue;
        }

        // Try to fill the image of this component based on the selected
        // image of its first source pentachoron.
        // Note that there is only one way of doing this (as seen by
        // following adjacent pentachoron gluings).  It either works or
        // it doesn't.
        pentIndex = pentachoronIndex(components_[comp]->getPentachoron(0));

        whichComp[startPent[comp]] = comp;
        iso.pentImage(pentIndex) = startPent[comp];
        iso.facetPerm(pentIndex) = NPerm5::S5[startPerm[comp]];
        toProcess.push(pentIndex);

        broken = false;
        while ((! broken) && (! toProcess.empty())) {
            pentIndex = toProcess.front();
            toProcess.pop();
            pent = pentachora_[pentIndex];
            pentPerm = iso.facetPerm(pentIndex);
            destPentIndex = iso.pentImage(pentIndex);
            destPent = other.pentachora_[destPentIndex];

            // If we are after a complete isomorphism, we might as well
            // test whether the lower-dimensional face degrees match.
            if (completeIsomorphism && ! compatiblePents(pent, destPent,
                    pentPerm)) {
                broken = true;
                break;
            }

            for (facet = 0; facet < 5; ++facet) {
                adj = pent->adjacentPentachoron(facet);
                if (adj) {
                    // There is an adjacent source pentachoron.
                    // Is there an adjacent destination pentachoron?
                    destAdj = destPent->adjacentPentachoron(pentPerm[facet]);
                    if (! destAdj) {
                        broken = true;
                        break;
                    }
                    // Work out what the isomorphism *should* say.
                    adjIndex = pentachoronIndex(adj);
                    destAdjIndex = other.pentachoronIndex(destAdj);
                    adjPerm =
                        destPent->adjacentGluing(pentPerm[facet]) *
                        pentPerm *
                        pent->adjacentGluing(facet).inverse();

                    if (iso.pentImage(adjIndex) >= 0) {
                        // We've already decided upon an image for this
                        // source pentachoron.  Does it match?
                        if (static_cast<long>(destAdjIndex) !=
                                iso.pentImage(adjIndex) ||
                                adjPerm != iso.facetPerm(adjIndex)) {
                            broken = true;
                            break;
                        }
                    } else if (whichComp[destAdjIndex] >= 0) {
                        // We haven't decided upon an image for this
                        // source pentachoron but the destination
                        // pentachoron has already been used.
                        broken = true;
                        break;
                    } else {
                        // We haven't seen either the source or the
                        // destination pentachoron.
                        whichComp[destAdjIndex] = comp;
                        iso.pentImage(adjIndex) = destAdjIndex;
                        iso.facetPerm(adjIndex) = adjPerm;
                        toProcess.push(adjIndex);
                    }
                } else if (completeIsomorphism) {
                    // There is no adjacent source pentachoron, and we
                    // are after a boundary complete isomorphism.
                    // There had better be no adjacent destination
                    // pentachoron also.
                    if (destPent->adjacentPentachoron(pentPerm[facet])) {
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

            for (i = 0; i < nPentachora; i++)
                if (iso.pentImage(i) >= 0 &&
                        whichComp[iso.pentImage(i)] == comp) {
                    whichComp[iso.pentImage(i)] = -1;
                    iso.pentImage(i) = -1;
                }

            startPerm[comp]++;
        }
    }

    // All out of options.
    delete[] whichComp;
    delete[] startPent;
    delete[] startPerm;
    return nResults;
}

bool Dim4Triangulation::compatiblePents(
        Dim4Pentachoron* src, Dim4Pentachoron* dest, NPerm5 p) {
    for (int face = 0; face < 10; face++)
        if (src->getFace(face)->getNumberOfEmbeddings() !=
                dest->getFace(Dim4Face::faceNumber
                    [p[Dim4Face::faceVertex[face][0]]]
                    [p[Dim4Face::faceVertex[face][1]]]
                    [p[Dim4Face::faceVertex[face][2]]])
                ->getNumberOfEmbeddings())
            return false;

    for (int edge = 0; edge < 10; edge++)
        if (src->getEdge(edge)->getNumberOfEmbeddings() !=
                dest->getEdge(Dim4Edge::edgeNumber
                    [p[Dim4Edge::edgeVertex[edge][0]]]
                    [p[Dim4Edge::edgeVertex[edge][1]]])
                ->getNumberOfEmbeddings())
            return false;

    for (int vertex = 0; vertex < 5; vertex++)
        if (src->getVertex(vertex)->getNumberOfEmbeddings() !=
                dest->getVertex(p[vertex])->getNumberOfEmbeddings())
            return false;

    return true;
}

} // namespace regina

