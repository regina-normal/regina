
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

#include "triangulation/nedge.h"
#include "triangulation/nface.h"

namespace regina {

const int NFace::TRIANGLE = 1;
const int NFace::SCARF = 2;
const int NFace::PARACHUTE = 3;
const int NFace::CONE = 4;
const int NFace::MOBIUS = 5;
const int NFace::HORN = 6;
const int NFace::DUNCEHAT = 7;
const int NFace::L31 = 8;

const NPerm NFace::ordering[4] = {
    NPerm(1, 2, 3, 0),
    NPerm(0, 2, 3, 1),
    NPerm(0, 1, 3, 2),
    NPerm(0, 1, 2, 3)
};

int NFace::getType() {
    if (type)
        return type;

    subtype = -1;

    // Determine the face type.
    NVertex* v[3];
    NEdge* e[3];
    int i;
    for (i = 0; i < 3; i++) {
        v[i] = getVertex(i);
        e[i] = getEdge(i);
    }

    if (e[0] != e[1] && e[1] != e[2] && e[2] != e[0]) {
        // Three distinct edges.
        if (v[0] == v[1] && v[1] == v[2])
            return (type = PARACHUTE);
        for (i = 0; i < 3; i++)
            if (v[(i+1)%3] == v[(i+2)%3]) {
                subtype = i;
                return (type = SCARF);
            }
        return (type = TRIANGLE);
    }

    if (e[0] == e[1] && e[1] == e[2]) {
        // All edges identified.
        if (getEdgeMapping(0).sign() == getEdgeMapping(1).sign() &&
                getEdgeMapping(1).sign() == getEdgeMapping(2).sign())
            return (type = L31);

        for (i = 0; i < 3; i++)
            if (getEdgeMapping((i+1)%3).sign() ==
                    getEdgeMapping((i+2)%3).sign()) {
                subtype = i;
                return (type = DUNCEHAT);
            }
    }

    // Two edges identified.
    for (i = 0; i < 3; i++)
        if (e[(i+1)%3] == e[(i+2)%3]) {
            subtype = i;

            if (getEdgeMapping((i+1)%3).sign() ==
                    getEdgeMapping((i+2)%3).sign())
                return (type = MOBIUS);

            if (v[0] == v[1] && v[1] == v[2])
                return (type = HORN);

            return (type = CONE);
        }

    // We should never reach this point.
    return 0;
}

NEdge* NFace::getEdge(int edge) const {
    NPerm p = embeddings[0]->getVertices();
    return embeddings[0]->getTetrahedron()->getEdge(
        NEdge::edgeNumber[p[(edge + 1) % 3]][p[(edge + 2) % 3]]);
}

NPerm NFace::getEdgeMapping(int edge) const {
    NPerm facePerm = embeddings[0]->getVertices();
        // Maps face -> tetrahedron
    NPerm edgePerm = embeddings[0]->getTetrahedron()->getEdgeMapping(
        NEdge::edgeNumber[facePerm[(edge + 1) % 3]][facePerm[(edge + 2) % 3]]);
        // Maps edge -> tetrahedron
    return NPerm(facePerm.preImageOf(edgePerm[0]),
        facePerm.preImageOf(edgePerm[1]), edge, 3);
}

} // namespace regina

