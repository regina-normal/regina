
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <map>
#include <queue>
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

template <>
bool TriangulationBase<3>::compatible(
        const Triangulation<3>& other, bool complete) const {
    const NTriangulation* me = static_cast<const NTriangulation*>(this);

    // Unfortunately, if we allow boundary incomplete isomorphisms then
    // we can't test that many properties.
    if (complete) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (simplices_.size() != other.simplices_.size())
            return false;
        if (me->getNumberOfTriangles() != other.getNumberOfTriangles())
            return false;
        if (me->getNumberOfEdges() != other.getNumberOfEdges())
            return false;
        if (me->getNumberOfVertices() != other.getNumberOfVertices())
            return false;
        if (components().size() != other.components().size())
            return false;
        if (me->boundaryComponents_.size() != other.boundaryComponents_.size())
            return false;
        if (isOrientable() ^ other.isOrientable())
            return false;

        // Test degree sequences and the like.
        std::map<unsigned long, unsigned long> map1;
        std::map<unsigned long, unsigned long> map2;
        std::map<unsigned long, unsigned long>::iterator mapIt;

        {
            for (auto e : me->getEdges()) {
                // Find this degree, or insert it with frequency 0 if it's
                // not already present.
                mapIt = map1.insert(std::make_pair(e->degree(), 0)).first;
                (*mapIt).second++;
            }
            for (auto e : other.getEdges()) {
                mapIt = map2.insert(std::make_pair(e->degree(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return false;
            map1.clear();
            map2.clear();
        }
        {
            for (auto v : me->getVertices()) {
                mapIt = map1.insert(std::make_pair(v->degree(), 0)).first;
                (*mapIt).second++;
            }
            for (auto v : other.getVertices()) {
                mapIt = map2.insert(std::make_pair(v->degree(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return false;
            map1.clear();
            map2.clear();
        }
        {
            ComponentIterator it;
            for (it = components().begin(); it != components().end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfTetrahedra(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components().begin();
                    it != other.components().end(); it++) {
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
            NTriangulation::BoundaryComponentIterator it;
            for (it = me->boundaryComponents_.begin();
                    it != me->boundaryComponents_.end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.boundaryComponents_.begin();
                    it != other.boundaryComponents_.end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return false;
            map1.clear();
            map2.clear();
        }
    } else {
        // May be boundary incomplete, and need not be onto.
        // Not much we can test for unfortunately.
        if (simplices_.size() > other.simplices_.size())
            return false;
        if ((! isOrientable()) && other.isOrientable())
            return false;
    }

    return true;
}

template <>
bool TriangulationBase<3>::compatible(
        Simplex<3>* src, Simplex<3>* dest, NPerm<4> p) {
    for (int edge = 0; edge < 6; edge++) {
        if (src->getEdge(edge)->getDegree() !=
                dest->getEdge(NEdge::edgeNumber[p[NEdge::edgeVertex[edge][0]]]
                    [p[NEdge::edgeVertex[edge][1]]])->getDegree())
            return false;
    }

    for (int vertex = 0; vertex < 4; vertex++) {
        if (src->getVertex(vertex)->getDegree() !=
                dest->getVertex(p[vertex])->getDegree())
            return false;
        if (src->getVertex(vertex)->getLink() !=
                dest->getVertex(p[vertex])->getLink())
            return false;
    }

    return true;
}

} // namespace regina

