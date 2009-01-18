
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

#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4tetrahedron.h"
#include "triangulation/nedge.h"

namespace regina {

const NPerm5 Dim4Tetrahedron::ordering[5] = {
    NPerm5(1,2,3,4,0),
    NPerm5(0,2,3,4,1),
    NPerm5(0,1,3,4,2),
    NPerm5(0,1,2,4,3),
    NPerm5(0,1,2,3,4)
};

Dim4Edge* Dim4Tetrahedron::getEdge(int edge) const {
    NPerm5 p = emb_[0].getVertices();
    return emb_[0].getPentachoron()->getEdge(
        Dim4Edge::edgeNumber
            [p[NEdge::edgeVertex[edge][0]]]
            [p[NEdge::edgeVertex[edge][1]]]);
}

Dim4Face* Dim4Tetrahedron::getFace(int face) const {
    NPerm5 p = emb_[0].getVertices();
    return emb_[0].getPentachoron()->getFace(
        Dim4Face::faceNumber
            [p[(face + 1) % 4]]
            [p[(face + 2) % 4]]
            [p[(face + 3) % 4]]);
}

NPerm5 Dim4Tetrahedron::getEdgeMapping(int edge) const {
    NPerm5 tetPerm = emb_[0].getVertices();
    NPerm5 edgePerm = emb_[0].getPentachoron()->getEdgeMapping(
        Dim4Edge::edgeNumber[tetPerm[NEdge::edgeVertex[edge][0]]]
                            [tetPerm[NEdge::edgeVertex[edge][1]]]);

    return NPerm5(
        tetPerm.preImageOf(edgePerm[0]),
        tetPerm.preImageOf(edgePerm[1]),
        NEdge::edgeVertex[5 - edge][0],
        NEdge::edgeVertex[5 - edge][1],
        4);
}

NPerm5 Dim4Tetrahedron::getFaceMapping(int face) const {
    NPerm5 tetPerm = emb_[0].getVertices();
    NPerm5 facePerm = emb_[0].getPentachoron()->getFaceMapping(
        Dim4Face::faceNumber[tetPerm[(face + 1) % 4]]
                            [tetPerm[(face + 2) % 4]]
                            [tetPerm[(face + 3) % 4]]);

    return NPerm5(
        tetPerm.preImageOf(facePerm[0]),
        tetPerm.preImageOf(facePerm[1]),
        tetPerm.preImageOf(facePerm[2]),
        face, 4);
}

} // namespace regina

