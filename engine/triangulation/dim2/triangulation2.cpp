
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "triangulation/dim2.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<2>::Triangulation(const std::string& description) {
    Triangulation<2>* attempt;

    if ((attempt = fromIsoSig(description))) {
        swapContents(*attempt);
        setLabel(description);
    }

    delete attempt;
}

bool Triangulation<2>::isMinimal() const {
    // 2-sphere:
    if (eulerChar() == 2)
        return (simplices_.size() == 2);

    // Projective plane and disc:
    if (eulerChar() == 1)
        return (simplices_.size() == (isClosed() ? 2 : 1));

    // All other closed manifolds:
    if (isClosed())
        return (countVertices() == 1);

    // All other bounded manifolds:
    return (countVertices() == countBoundaryComponents());
}

void Triangulation<2>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Triangles: " << simplices_.size() << '\n';
    out << "  Edges: " << countEdges() << '\n';
    out << "  Vertices: " << countVertices() << '\n';
    out << '\n';

    Triangle<2>* tri;
    Triangle<2>* adjTri;
    unsigned triPos;
    int i, j;
    Perm<3> adjPerm;

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
                out << std::setw(3) << adjTri->index() << " (";
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
            out << ' ' << std::setw(3) << tri->vertex(i)->index();
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
            out << ' ' << std::setw(3) << tri->edge(i)->index();
        out << '\n';
    }
    out << '\n';
}

void Triangulation<2>::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the triangle gluings.
    TriangleIterator it;
    Triangle<2>* adjTri;
    int edge;

    out << "  <triangles ntriangles=\"" << simplices_.size() << "\">\n";
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        out << "    <triangle desc=\"" <<
            xmlEncodeSpecialChars((*it)->description()) << "\"> ";
        for (edge = 0; edge < 3; ++edge) {
            adjTri = (*it)->adjacentTriangle(edge);
            if (adjTri) {
                out << adjTri->index() << ' '
                    << static_cast<int>((*it)->
                        adjacentGluing(edge).permCode()) << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</triangle>\n";
    }
    out << "  </triangles>\n";

    writeXMLBaseProperties(out);
}

} // namespace regina
