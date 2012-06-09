
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "dim2/dim2triangulation.h"
#include "utilities/xmlutils.h"

namespace regina {

bool Dim2Triangulation::isMinimal() const {
    // 2-sphere:
    if (getEulerChar() == 2)
        return (triangles_.size() == 2);

    // Projective plane and disc:
    if (getEulerChar() == 1)
        return (triangles_.size() == (isClosed() ? 2 : 1));

    // All other closed manifolds:
    if (isClosed())
        return (vertices_.size() == 1);

    // All other bounded manifolds:
    return (vertices_.size() == boundaryComponents_.size());
}

void Dim2Triangulation::swapContents(Dim2Triangulation& other) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&other);

    clearAllProperties();
    other.clearAllProperties();

    triangles_.swap(other.triangles_);

    TriangleIterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it)
        (*it)->tri_ = this;
    for (it = other.triangles_.begin(); it != other.triangles_.end(); ++it)
        (*it)->tri_ = &other;
}

void Dim2Triangulation::moveContentsTo(Dim2Triangulation& dest) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&dest);

    clearAllProperties();
    dest.clearAllProperties();

    TriangleIterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        // This is an abuse of NMarkedVector, since for a brief moment
        // each triangle belongs to both vectors triangles_ and dest.triangles_.
        // However, the subsequent clear() operation does not touch the
        // triangle markings (indices), and so we end up with the
        // correct result (i.e., the markings are correct for dest).
        (*it)->tri_ = &dest;
        dest.triangles_.push_back(*it);
    }
    triangles_.clear();
}

void Dim2Triangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Triangles: " << triangles_.size() << '\n';
    out << "  Edges: " << edges_.size() << '\n';
    out << "  Vertices: " << vertices_.size() << '\n';
    out << '\n';

    Dim2Triangle* tri;
    Dim2Triangle* adjTri;
    unsigned triPos;
    int i, j, k;
    NPerm3 adjPerm;

    out << "Triangle gluing:\n";
    out << "  Triangle  |  glued to:     (01)     (02)     (12)\n";
    out << "  ----------+--------------------------------------\n";
    for (triPos=0; triPos < triangles_.size(); triPos++) {
        tri = triangles_[triPos];
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
    for (triPos = 0; triPos < triangles_.size(); ++triPos) {
        tri = triangles_[triPos];
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
    for (triPos = 0; triPos < triangles_.size(); ++triPos) {
        tri = triangles_[triPos];
        out << "      " << std::setw(4) << triPos << "  |        ";
        for (i = 2; i >= 0; --i)
            out << ' ' << std::setw(3) << edgeIndex(tri->getEdge(i));
        out << '\n';
    }
    out << '\n';
}

void Dim2Triangulation::insertTriangulation(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    unsigned long nOrig = getNumberOfTriangles();

    TriangleIterator it;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it)
        newTriangle((*it)->getDescription());

    // Make the gluings.
    long triPos, adjPos;
    Dim2Triangle* tri;
    Dim2Triangle* adjTri;
    NPerm3 adjPerm;
    int edge;
    triPos = 0;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it) {
        tri = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentTriangle(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    triangles_[nOrig + triPos]->joinTo(edge,
                        triangles_[nOrig + adjPos], adjPerm);
                }
            }
        }
        ++triPos;
    }
}

void Dim2Triangulation::insertConstruction(unsigned long nTriangles,
        const int adjacencies[][3], const int gluings[][3][3]) {
    if (nTriangles == 0)
        return;

    Dim2Triangle** tri = new Dim2Triangle*[nTriangles];

    unsigned i, j;
    NPerm3 p;

    ChangeEventSpan span(this);

    for (i = 0; i < nTriangles; ++i)
        tri[i] = newTriangle();

    for (i = 0; i < nTriangles; ++i)
        for (j = 0; j < 3; ++j)
            if (adjacencies[i][j] >= 0 &&
                    ! tri[i]->adjacentTriangle(j)) {
                p = NPerm3(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2]);
                tri[i]->joinTo(j, tri[adjacencies[i][j]], p);
            }

    delete[] tri;
}

std::string Dim2Triangulation::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! getPacketLabel().empty())
        ans <<
" * 2-manifold triangulation: " << getPacketLabel() << "\n";
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (triangles_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * triangle edges.\n"
" */\n"
"\n";

    unsigned long nTriangles = triangles_.size();
    Dim2Triangle* tri;
    NPerm3 perm;
    unsigned long p;
    int e, i;

    ans << "const int adjacencies[" << nTriangles << "][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = triangles_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentTriangle(e)) {
                ans << triangleIndex(tri->adjacentTriangle(e));
            } else
                ans << "-1";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nTriangles << "][3][3] = {\n";
    for (p = 0; p < nTriangles; ++p) {
        tri = triangles_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (tri->adjacentTriangle(e)) {
                perm = tri->adjacentGluing(e);
                ans << "{ ";
                for (i = 0; i < 3; ++i) {
                    ans << perm[i];
                    if (i < 2)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else
                ans << "{ 0, 0, 0 }";

            if (e < 2)
                ans << ", ";
            else if (p != nTriangles - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code actually constructs a 2-manifold triangulation\n"
" * based on the information stored in the arrays above.\n"
" */\n"
"\n"
"Dim2Triangulation tri;\n"
"tri.insertConstruction(" << nTriangles << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}

void Dim2Triangulation::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the triangle gluings.
    TriangleIterator it;
    Dim2Triangle* adjTri;
    int edge;

    out << "  <triangles ntriangles=\"" << triangles_.size() << "\">\n";
    for (it = triangles_.begin(); it != triangles_.end(); ++it) {
        out << "    <triangle desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (edge = 0; edge < 3; ++edge) {
            adjTri = (*it)->adjacentTriangle(edge);
            if (adjTri) {
                out << triangleIndex(adjTri) << ' '
                    << (*it)->adjacentGluing(edge).getPermCode() << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</triangle>\n";
    }
    out << "  </triangles>\n";
}

void Dim2Triangulation::cloneFrom(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    removeAllTriangles();

    TriangleIterator it;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it)
        newTriangle((*it)->getDescription());

    // Make the gluings.
    long triPos, adjPos;
    Dim2Triangle* tri;
    Dim2Triangle* adjTri;
    NPerm3 adjPerm;
    int edge;
    triPos = 0;
    for (it = X.triangles_.begin(); it != X.triangles_.end(); ++it) {
        tri = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjTri = tri->adjacentTriangle(edge);
            if (adjTri) {
                adjPos = X.triangleIndex(adjTri);
                adjPerm = tri->adjacentGluing(edge);
                if (adjPos > triPos ||
                        (adjPos == triPos && adjPerm[edge] > edge)) {
                    triangles_[triPos]->joinTo(edge,
                        triangles_[adjPos], adjPerm);
                }
            }
        }
        ++triPos;
    }

    // Properties:
    // None yet for 2-manifold triangulations.
}

void Dim2Triangulation::deleteTriangles() {
    for (TriangleIterator it = triangles_.begin(); it != triangles_.end(); ++it)
        delete *it;
    triangles_.clear();
}

void Dim2Triangulation::deleteSkeleton() {
    for (VertexIterator it = vertices_.begin(); it != vertices_.end(); ++it)
        delete *it;
    for (EdgeIterator it = edges_.begin(); it != edges_.end(); ++it)
        delete *it;
    for (ComponentIterator it = components_.begin();
            it != components_.end(); ++it)
        delete *it;
    for (BoundaryComponentIterator it = boundaryComponents_.begin();
            it != boundaryComponents_.end(); ++it)
        delete *it;

    vertices_.clear();
    edges_.clear();
    components_.clear();
    boundaryComponents_.clear();

    calculatedSkeleton_ = false;
}

void Dim2Triangulation::clearAllProperties() {
    if (calculatedSkeleton_)
        deleteSkeleton();
}

} // namespace regina
