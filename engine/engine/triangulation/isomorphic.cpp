
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

namespace regina {

bool NTriangulation::isIsomorphicTo(NTriangulation& other) {
    if (! calculatedSkeleton)
        calculateSkeleton();
    if (! other.calculatedSkeleton)
        other.calculateSkeleton();

    if (tetrahedra.size() != other.tetrahedra.size())
        return false;
    if (tetrahedra.empty())
        return true;
    if (faces.size() != other.faces.size())
        return false;
    if (edges.size() != other.edges.size())
        return false;
    if (vertices.size() != other.vertices.size())
        return false;
    if (components.size() != other.components.size())
        return false;
    if (boundaryComponents.size() != other.boundaryComponents.size())
        return false;
    if (isOrientable() ^ other.isOrientable())
        return false;

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
            return false;
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
            return false;
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
            return false;
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
            return false;
        map1.clear();
        map2.clear();
    }

    // Try to make an exact matching.
    unsigned long nTetrahedra = tetrahedra.size();
    unsigned long* join = new unsigned long[nTetrahedra];
    bool* used = new bool[nTetrahedra];
    int* perm = new int[nTetrahedra];
    for (unsigned long i=0; i<nTetrahedra; i++) {
        used[i] = false;
        join[i] = 0;
        perm[i] = 0;
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
        if (perm[mustMatch] == 24) {
            perm[mustMatch] = 0;
            used[join[mustMatch]] = false;
            join[mustMatch]++;
            while (join[mustMatch] < nTetrahedra)
                if (used[join[mustMatch]])
                    join[mustMatch]++;
                else
                    break;
            if (join[mustMatch] == nTetrahedra) {
                join[mustMatch] = 0;
                if (mustMatch == 0)
                    break;
                mustMatch--;
                perm[mustMatch]++;
                continue;
            } else {
                used[join[mustMatch]] = true;
            }
        }

        // Try to join mustMatch with join[mustMatch] according to
        // permutation perm[mustMatch].

        canJoin = true;
        tetPerm = allPermsS4[perm[mustMatch]];

        for (edge = 0; edge < 6; edge++) {
            if (tetrahedra[mustMatch]->getEdge(edge)
                    ->getNumberOfEmbeddings() !=
                    other.tetrahedra[join[mustMatch]]
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
                        other.tetrahedra[join[mustMatch]]
                        ->getVertex(tetPerm[vertex])
                        ->getNumberOfEmbeddings()) {
                    canJoin = false;
                    break;
                }
                if (tetrahedra[mustMatch]->getVertex(vertex)
                        ->getLink() !=
                        other.tetrahedra[join[mustMatch]]
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
                yourNbr = other.tetrahedra[join[mustMatch]]->
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
                        adjPerm = allPermsS4[perm[tetIndex]];
                        if (yourNbr != other.tetrahedra[join[tetIndex]]) {
                            canJoin = false;
                            break;
                        }
                        if (! ((adjPerm * tetrahedra[mustMatch]->
                                getAdjacentTetrahedronGluing(face)) ==
                                (other.tetrahedra[join[mustMatch]]->
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
                delete[] join;
                delete[] perm;
                delete[] used;
                return true;
            }
            used[join[mustMatch]] = true;
        } else
            perm[mustMatch]++;
    }

    delete[] join;
    delete[] perm;
    delete[] used;
    return false;
}

} // namespace regina

