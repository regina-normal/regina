
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "dim2/dim2triangulation.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<2>::Triangulation(const std::string& description) {
    Dim2Triangulation* attempt;

    if ((attempt = fromIsoSig(description))) {
        cloneFrom(*attempt);
        setPacketLabel(description);
    }

    delete attempt;
}

bool Triangulation<2>::isMinimal() const {
    // 2-sphere:
    if (getEulerChar() == 2)
        return (simplices_.size() == 2);

    // Projective plane and disc:
    if (getEulerChar() == 1)
        return (simplices_.size() == (isClosed() ? 2 : 1));

    // All other closed manifolds:
    if (isClosed())
        return (countVertices() == 1);

    // All other bounded manifolds:
    return (countVertices() == boundaryComponents_.size());
}

void Triangulation<2>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Triangles: " << simplices_.size() << '\n';
    out << "  Edges: " << countEdges() << '\n';
    out << "  Vertices: " << countVertices() << '\n';
    out << '\n';

    Dim2Triangle* tri;
    Dim2Triangle* adjTri;
    unsigned triPos;
    int i, j;
    NPerm3 adjPerm;

    out << "Triangle gluing:\n";
    out << "  Triangle  |  glued to:     (01)     (02)     (12)\n";
    out << "  ----------+--------------------------------------\n";
    for (triPos=0; triPos < simplices_.size(); triPos++) {
        tri = simplices_[triPos];
        out << "      " << std::setw(4) << triPos << "  |           ";
        for (i = 2; i >= 0; --i) {
            out << " ";
            adjTri = tri->adjacentTriangle(i);
            if (! adjTri)
                out << "boundary";
            else {
                adjPerm = tri->adjacentGluing(i);
                out << std::setw(3) << triangleIndex(adjTri) << " (";
                for (j = 0; j < 3; ++j) {
                    if (j == i) continue;
                    out << adjPerm[j];
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';

    out << "Vertices:\n";
    out << "  Triangle  |  vertex:    0   1   2\n";
    out << "  ----------+----------------------\n";
    for (triPos = 0; triPos < simplices_.size(); ++triPos) {
        tri = simplices_[triPos];
        out << "      " << std::setw(4) << triPos << "  |          ";
        for (i = 0; i < 3; ++i)
            out << ' ' << std::setw(3) <<
                vertexIndex(tri->getVertex(i));
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Triangle  |  edge:   01  02  12\n";
    out << "  ----------+--------------------\n";
    for (triPos = 0; triPos < simplices_.size(); ++triPos) {
        tri = simplices_[triPos];
        out << "      " << std::setw(4) << triPos << "  |        ";
        for (i = 2; i >= 0; --i)
            out << ' ' << std::setw(3) << edgeIndex(tri->getEdge(i));
        out << '\n';
    }
    out << '\n';
}

void Triangulation<2>::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the triangle gluings.
    TriangleIterator it;
    Dim2Triangle* adjTri;
    int edge;

    out << "  <triangles ntriangles=\"" << simplices_.size() << "\">\n";
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        out << "    <triangle desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (edge = 0; edge < 3; ++edge) {
            adjTri = (*it)->adjacentTriangle(edge);
            if (adjTri) {
                out << triangleIndex(adjTri) << ' '
                    << static_cast<int>((*it)->
                        adjacentGluing(edge).getPermCode()) << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</triangle>\n";
    }
    out << "  </triangles>\n";
}

void Triangulation<2>::cloneFrom(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    removeAllTriangles();

    TriangleIterator it;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); ++it)
        newTriangle((*it)->getDescription());

    // Make the gluings.
    long triPos, adjPos;
    Dim2Triangle* tri;
    Dim2Triangle* adjTri;
    NPerm3 adjPerm;
    int edge;
    triPos = 0;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); ++it) {
        tri = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentTriangle(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    simplices_[triPos]->joinTo(edge,
                        simplices_[adjPos], adjPerm);
                }
            }
        }
        ++triPos;
    }

    // Properties:
    // None yet for 2-manifold triangulations.
}

void Triangulation<2>::deleteSkeleton() {
    for (auto b : boundaryComponents_)
        delete b;
    boundaryComponents_.clear();

    TriangulationBase<2>::deleteSkeleton();
}

void Triangulation<2>::clearAllProperties() {
    if (calculatedSkeleton())
        deleteSkeleton();
}

} // namespace regina
