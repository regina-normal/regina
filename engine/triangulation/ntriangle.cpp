
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

#include "triangulation/nedge.h"
#include "triangulation/ntriangle.h"

namespace regina {

const int NTriangle::TRIANGLE = 1;
const int NTriangle::SCARF = 2;
const int NTriangle::PARACHUTE = 3;
const int NTriangle::CONE = 4;
const int NTriangle::MOBIUS = 5;
const int NTriangle::HORN = 6;
const int NTriangle::DUNCEHAT = 7;
const int NTriangle::L31 = 8;

const NPerm4 NTriangle::ordering[4] = {
    NPerm4(1, 2, 3, 0),
    NPerm4(0, 2, 3, 1),
    NPerm4(0, 1, 3, 2),
    NPerm4(0, 1, 2, 3)
};

int NTriangle::getType() {
    if (type)
        return type;

    subtype = -1;

    // Determine the triangle type.
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

NEdge* NTriangle::getEdge(int edge) const {
    NPerm4 p = embeddings[0]->getVertices();
    return embeddings[0]->getTetrahedron()->getEdge(
        NEdge::edgeNumber[p[(edge + 1) % 3]][p[(edge + 2) % 3]]);
}

NPerm4 NTriangle::getEdgeMapping(int edge) const {
    NPerm4 triPerm = embeddings[0]->getVertices();
        // Maps triangle -> tetrahedron
    NPerm4 edgePerm = embeddings[0]->getTetrahedron()->getEdgeMapping(
        NEdge::edgeNumber[triPerm[(edge + 1) % 3]][triPerm[(edge + 2) % 3]]);
        // Maps edge -> tetrahedron
    return NPerm4(triPerm.preImageOf(edgePerm[0]),
        triPerm.preImageOf(edgePerm[1]), edge, 3);
}

void NTriangle::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (int i = 0; i < nEmbeddings; ++i)
        out << "  " << embeddings[i]->getTetrahedron()->markedIndex()
            << " (" << embeddings[i]->getVertices().trunc3() << ')'
            << std::endl;
}

} // namespace regina

