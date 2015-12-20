
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

namespace regina {

template <>
bool detail::TriangulationBase<4>::compatible(
        const Dim4Triangulation& other, bool complete) const {
    const Dim4Triangulation* me = static_cast<const Dim4Triangulation*>(this);

    // Unfortunately, if we allow boundary incomplete isomorphisms then
    // we can't test that many properties.
    if (complete) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (size() != other.size())
            return 0;
        if (me->countTetrahedra() != other.countTetrahedra())
            return 0;
        if (me->countTriangles() != other.countTriangles())
            return 0;
        if (me->countEdges() != other.countEdges())
            return 0;
        if (me->countVertices() != other.countVertices())
            return 0;
        if (countComponents() != other.countComponents())
            return 0;
        if (me->boundaryComponents_.size() != other.boundaryComponents_.size())
            return 0;
        if (isOrientable() ^ other.isOrientable())
            return 0;

        // Test degree sequences and the like.
        std::map<unsigned long, unsigned long> map1;
        std::map<unsigned long, unsigned long> map2;
        std::map<unsigned long, unsigned long>::iterator mapIt;

        {
            for (auto f : me->triangles()) {
                // Find this degree, or insert it with frequency 0 if it's
                // not already present.
                mapIt = map1.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            for (auto f : other.triangles()) {
                mapIt = map2.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return 0;
            map1.clear();
            map2.clear();
        }
        {
            for (auto f : me->edges()) {
                // Find this degree, or insert it with frequency 0 if it's
                // not already present.
                mapIt = map1.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            for (auto f : other.edges()) {
                mapIt = map2.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return 0;
            map1.clear();
            map2.clear();
        }
        {
            for (auto f : me->vertices()) {
                mapIt = map1.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            for (auto f : other.vertices()) {
                mapIt = map2.insert(std::make_pair(f->degree(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return 0;
            map1.clear();
            map2.clear();
        }
        {
            ComponentIterator it;
            for (it = components().begin(); it != components().end(); it++) {
                mapIt = map1.insert(
                    std::make_pair((*it)->getNumberOfPentachora(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components().begin();
                    it != other.components().end(); it++) {
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
            Dim4Triangulation::BoundaryComponentIterator it;
            for (it = me->boundaryComponents_.begin();
                    it != me->boundaryComponents_.end(); it++) {
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
        if (size() > other.size())
            return 0;
        if ((! isOrientable()) && other.isOrientable())
            return 0;
    }

    return true;
}

template <>
bool detail::TriangulationBase<4>::compatible(
        Simplex<4>* src, Simplex<4>* dest, NPerm<5> p) {
    for (int triangle = 0; triangle < 10; triangle++)
        if (src->getTriangle(triangle)->degree() !=
                dest->getTriangle(Dim4Triangle::triangleNumber
                    [p[Dim4Triangle::triangleVertex[triangle][0]]]
                    [p[Dim4Triangle::triangleVertex[triangle][1]]]
                    [p[Dim4Triangle::triangleVertex[triangle][2]]])
                ->degree())
            return false;

    for (int edge = 0; edge < 10; edge++)
        if (src->getEdge(edge)->degree() !=
                dest->getEdge(Dim4Edge::edgeNumber
                    [p[Dim4Edge::edgeVertex[edge][0]]]
                    [p[Dim4Edge::edgeVertex[edge][1]]])
                ->degree())
            return false;

    for (int vertex = 0; vertex < 5; vertex++)
        if (src->getVertex(vertex)->degree() !=
                dest->getVertex(p[vertex])->degree())
            return false;

    return true;
}

} // namespace regina

