
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
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

namespace regina {

template <>
bool detail::TriangulationBase<2>::compatible(
        const Dim2Triangulation& other, bool complete) const {
    const Dim2Triangulation* me = static_cast<const Dim2Triangulation*>(this);

    // Unfortunately, if we allow boundary incomplete isomorphisms then
    // we can't test that many properties.
    if (complete) {
        // Must be boundary complete, 1-to-1 and onto.
        // That is, combinatorially the two triangulations must be
        // identical.
        if (simplices_.size() != other.simplices_.size())
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
        std::map<size_t, size_t> map1;
        std::map<size_t, size_t> map2;
        std::map<size_t, size_t>::iterator mapIt;

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
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
                (*mapIt).second++;
            }
            for (it = other.components().begin();
                    it != other.components().end(); it++) {
                mapIt = map2.insert(
                    std::make_pair((*it)->getNumberOfTriangles(), 0)).first;
                (*mapIt).second++;
            }
            if (! (map1 == map2))
                return false;
            map1.clear();
            map2.clear();
        }
        {
            Dim2Triangulation::BoundaryComponentIterator it;
            for (it = me->boundaryComponents_.begin();
                    it != me->boundaryComponents_.end(); it++) {
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
bool detail::TriangulationBase<2>::compatible(
        Simplex<2>* src, Simplex<2>* dest, NPerm<3> p) {
    for (int vertex = 0; vertex < 3; vertex++)
        if (src->getVertex(vertex)->degree() !=
                dest->getVertex(p[vertex])->degree())
            return false;

    return true;
}

} // namespace regina

