
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

#include "dim4/dim4edge.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "triangulation/nedge.h"

namespace regina {

namespace detail {
    const NPerm5 FaceNumbering<4, 3>::ordering_[5] = {
        NPerm5(1,2,3,4,0),
        NPerm5(0,2,3,4,1),
        NPerm5(0,1,3,4,2),
        NPerm5(0,1,2,4,3),
        NPerm5(0,1,2,3,4)
    };
}

Dim4Edge* Dim4Tetrahedron::getEdge(int edge) const {
    NPerm5 p = front().getVertices();
    return front().getPentachoron()->getEdge(
        Dim4Edge::edgeNumber
            [p[NEdge::edgeVertex[edge][0]]]
            [p[NEdge::edgeVertex[edge][1]]]);
}

Dim4Triangle* Dim4Tetrahedron::getTriangle(int tri) const {
    NPerm5 p = front().getVertices();
    return front().getPentachoron()->getTriangle(
        Dim4Triangle::triangleNumber
            [p[(tri + 1) % 4]]
            [p[(tri + 2) % 4]]
            [p[(tri + 3) % 4]]);
}

NPerm5 Dim4Tetrahedron::getEdgeMapping(int edge) const {
    NPerm5 tetPerm = front().getVertices();
    NPerm5 edgePerm = front().getPentachoron()->getEdgeMapping(
        Dim4Edge::edgeNumber[tetPerm[NEdge::edgeVertex[edge][0]]]
                            [tetPerm[NEdge::edgeVertex[edge][1]]]);

    return NPerm5(
        tetPerm.preImageOf(edgePerm[0]),
        tetPerm.preImageOf(edgePerm[1]),
        NEdge::edgeVertex[5 - edge][0],
        NEdge::edgeVertex[5 - edge][1],
        4);
}

NPerm5 Dim4Tetrahedron::getTriangleMapping(int tri) const {
    NPerm5 tetPerm = front().getVertices();
    NPerm5 trianglePerm = front().getPentachoron()->getTriangleMapping(
        Dim4Triangle::triangleNumber[tetPerm[(tri + 1) % 4]]
                            [tetPerm[(tri + 2) % 4]]
                            [tetPerm[(tri + 3) % 4]]);

    return NPerm5(
        tetPerm.preImageOf(trianglePerm[0]),
        tetPerm.preImageOf(trianglePerm[1]),
        tetPerm.preImageOf(trianglePerm[2]),
        tri, 4);
}

void Dim4Tetrahedron::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (auto& emb : *this)
        out << "  " << emb << std::endl;
}

} // namespace regina

