
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nedge.h"
    #include "nface.h"
#else
    #include "engine/triangulation/nedge.h"
    #include "engine/triangulation/nface.h"
#endif

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
        edgeNumber[p[(edge + 1) % 3]][p[(edge + 2) % 3]]);
}

NPerm NFace::getEdgeMapping(int edge) const {
    NPerm facePerm = embeddings[0]->getVertices();
        // Maps face -> tetrahedron
    NPerm edgePerm = embeddings[0]->getTetrahedron()->getEdgeMapping(
        edgeNumber[facePerm[(edge + 1) % 3]][facePerm[(edge + 2) % 3]]);
        // Maps edge -> tetrahedron
    return NPerm(facePerm.preImageOf(edgePerm[0]),
        facePerm.preImageOf(edgePerm[1]), edge, 3);
}

