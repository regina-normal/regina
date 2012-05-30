
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

void Dim2Triangulation::swapContents(Dim2Triangulation& other) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&other);

    clearAllProperties();
    other.clearAllProperties();

    faces_.swap(other.faces_);

    FaceIterator it;
    for (it = faces_.begin(); it != faces_.end(); ++it)
        (*it)->tri_ = this;
    for (it = other.faces_.begin(); it != other.faces_.end(); ++it)
        (*it)->tri_ = &other;
}

void Dim2Triangulation::moveContentsTo(Dim2Triangulation& dest) {
    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&dest);

    clearAllProperties();
    dest.clearAllProperties();

    FaceIterator it;
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        // This is an abuse of NMarkedVector, since for a brief moment
        // each face belongs to both vectors faces_ and dest.faces_.
        // However, the subsequent clear() operation does not touch the
        // face markings (indices), and so we end up with the
        // correct result (i.e., the markings are correct for dest).
        (*it)->tri_ = &dest;
        dest.faces_.push_back(*it);
    }
    faces_.clear();
}

void Dim2Triangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Faces: " << faces_.size() << '\n';
    out << "  Edges: " << edges_.size() << '\n';
    out << "  Vertices: " << vertices_.size() << '\n';
    out << '\n';

    Dim2Face* face;
    Dim2Face* adjFace;
    unsigned facePos;
    int i, j, k;
    NPerm3 adjPerm;

    out << "Face gluing:\n";
    out << "  Face  |  glued to:     (01)     (02)     (12)\n";
    out << "  ------+--------------------------------------\n";
    for (facePos=0; facePos < faces_.size(); facePos++) {
        face = faces_[facePos];
        out << "  " << std::setw(4) << facePos << "  |           ";
        for (i = 2; i >= 0; --i) {
            out << " ";
            adjFace = face->adjacentFace(i);
            if (! adjFace)
                out << "boundary";
            else {
                adjPerm = face->adjacentGluing(i);
                out << std::setw(3) << faceIndex(adjFace) << " (";
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
    out << "  Face  |  vertex:    0   1   2\n";
    out << "  ------+----------------------\n";
    for (facePos = 0; facePos < faces_.size(); ++facePos) {
        face = faces_[facePos];
        out << "  " << std::setw(4) << facePos << "  |          ";
        for (i = 0; i < 3; ++i)
            out << ' ' << std::setw(3) <<
                vertexIndex(face->getVertex(i));
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Face  |  edge:   01  02  12\n";
    out << "  ------+--------------------\n";
    for (facePos = 0; facePos < faces_.size(); ++facePos) {
        face = faces_[facePos];
        out << "  " << std::setw(4) << facePos << "  |        ";
        for (i = 2; i >= 0; --i)
            out << ' ' << std::setw(3) << edgeIndex(face->getEdge(i));
        out << '\n';
    }
    out << '\n';
}

void Dim2Triangulation::insertTriangulation(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    unsigned long nOrig = getNumberOfFaces();

    FaceIterator it;
    for (it = X.faces_.begin(); it != X.faces_.end(); ++it)
        newFace((*it)->getDescription());

    // Make the gluings.
    long facePos, adjPos;
    Dim2Face* face;
    Dim2Face* adjFace;
    NPerm3 adjPerm;
    int edge;
    facePos = 0;
    for (it = X.faces_.begin(); it != X.faces_.end(); ++it) {
        face = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjFace = face->adjacentFace(edge);
            if (adjFace) {
                adjPos = X.faceIndex(adjFace);
                adjPerm = face->adjacentGluing(edge);
                if (adjPos > facePos ||
                        (adjPos == facePos && adjPerm[edge] > edge)) {
                    faces_[nOrig + facePos]->joinTo(edge,
                        faces_[nOrig + adjPos], adjPerm);
                }
            }
        }
        ++facePos;
    }
}

void Dim2Triangulation::insertConstruction(unsigned long nFaces,
        const int adjacencies[][3], const int gluings[][3][3]) {
    if (nFaces == 0)
        return;

    Dim2Face** face = new Dim2Face*[nFaces];

    unsigned i, j;
    NPerm3 p;

    ChangeEventSpan span(this);

    for (i = 0; i < nFaces; ++i)
        face[i] = newFace();

    for (i = 0; i < nFaces; ++i)
        for (j = 0; j < 3; ++j)
            if (adjacencies[i][j] >= 0 &&
                    ! face[i]->adjacentFace(j)) {
                p = NPerm3(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2]);
                face[i]->joinTo(j, face[adjacencies[i][j]], p);
            }

    delete[] face;
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

    if (faces_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * face edges.\n"
" */\n"
"\n";

    unsigned long nFaces = faces_.size();
    Dim2Face* face;
    NPerm3 perm;
    unsigned long p;
    int e, i;

    ans << "const int adjacencies[" << nFaces << "][3] = {\n";
    for (p = 0; p < nFaces; ++p) {
        face = faces_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (face->adjacentFace(e)) {
                ans << faceIndex(face->adjacentFace(e));
            } else
                ans << "-1";

            if (e < 2)
                ans << ", ";
            else if (p != nFaces - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nFaces << "][3][3] = {\n";
    for (p = 0; p < nFaces; ++p) {
        face = faces_[p];

        ans << "    { ";
        for (e = 0; e < 3; ++e) {
            if (face->adjacentFace(e)) {
                perm = face->adjacentGluing(e);
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
            else if (p != nFaces - 1)
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
"tri.insertConstruction(" << nFaces << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}

void Dim2Triangulation::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the face gluings.
    FaceIterator it;
    Dim2Face* adjFace;
    int edge;

    out << "  <faces nfaces=\"" << faces_.size() << "\">\n";
    for (it = faces_.begin(); it != faces_.end(); ++it) {
        out << "    <face desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (edge = 0; edge < 3; ++edge) {
            adjFace = (*it)->adjacentFace(edge);
            if (adjFace) {
                out << faceIndex(adjFace) << ' '
                    << (*it)->adjacentGluing(edge).getPermCode() << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</face>\n";
    }
    out << "  </faces>\n";
}

void Dim2Triangulation::cloneFrom(const Dim2Triangulation& X) {
    ChangeEventSpan span(this);

    removeAllFaces();

    FaceIterator it;
    for (it = X.faces_.begin(); it != X.faces_.end(); ++it)
        newFace((*it)->getDescription());

    // Make the gluings.
    long facePos, adjPos;
    Dim2Face* face;
    Dim2Face* adjFace;
    NPerm3 adjPerm;
    int edge;
    facePos = 0;
    for (it = X.faces_.begin(); it != X.faces_.end(); ++it) {
        face = *it;
        for (edge = 0; edge < 3; ++edge) {
            adjFace = face->adjacentFace(edge);
            if (adjFace) {
                adjPos = X.faceIndex(adjFace);
                adjPerm = face->adjacentGluing(edge);
                if (adjPos > facePos ||
                        (adjPos == facePos && adjPerm[edge] > edge)) {
                    faces_[facePos]->joinTo(edge, faces_[adjPos], adjPerm);
                }
            }
        }
        ++facePos;
    }

    // Properties:
    // None yet for 2-manifold triangulations.
}

void Dim2Triangulation::deleteFaces() {
    for (FaceIterator it = faces_.begin(); it != faces_.end(); ++it)
        delete *it;
    faces_.clear();
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
