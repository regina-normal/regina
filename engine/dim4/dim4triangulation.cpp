
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

#include <iomanip>
#include <iostream>
#include <sstream>
#include "dim4/dim4triangulation.h"
#include "utilities/xmlutils.h"

namespace regina {

void Dim4Triangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton_)
        calculateSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Pentachora: " << pentachora_.size() << '\n';
    out << "  Tetrahedra: " << tetrahedra_.size() << '\n';
    out << "  Faces: " << faces_.size() << '\n';
    out << "  Edges: " << edges_.size() << '\n';
    out << "  Vertices: " << vertices_.size() << '\n';
    out << '\n';

    Dim4Pentachoron* pent;
    Dim4Pentachoron* adjPent;
    unsigned pentPos;
    int i, j, k;
    NPerm5 adjPerm;

    out << "Pentachoron gluing:\n";
    out << "  Pent  |  glued to:     (0123)     (0124)     (0134)     (0234)     (1234)\n";
    out << "  ------+------------------------------------------------------------------\n";
    for (pentPos=0; pentPos < pentachora_.size(); pentPos++) {
        pent = pentachora_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |           ";
        for (i = 4; i >= 0; --i) {
            out << " ";
            adjPent = pent->adjacentPentachoron(i);
            if (! adjPent)
                out << "  boundary";
            else {
                adjPerm = pent->adjacentGluing(i);
                out << std::setw(3) << pentachoronIndex(adjPent) << " (";
                for (j = 0; j < 5; ++j) {
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
    out << "  Pent  |  vertex:    0   1   2   3   4\n";
    out << "  ------+------------------------------\n";
    for (pentPos = 0; pentPos < pentachora_.size(); ++pentPos) {
        pent = pentachora_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |          ";
        for (i = 0; i < 5; ++i)
            out << ' ' << std::setw(3) <<
                vertexIndex(pent->getVertex(i));
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Pent  |  edge:   01  02  03  04  12  13  14  23  24  34\n";
    out << "  ------+------------------------------------------------\n";
    for (pentPos = 0; pentPos < pentachora_.size(); ++pentPos) {
        pent = pentachora_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |        ";
        for (i = 0; i < 5; ++i)
            for (j = i + 1; j < 5; ++j)
                out << ' ' << std::setw(3)
                    << edgeIndex(pent->getEdge(Dim4Edge::edgeNumber[i][j]));
        out << '\n';
    }
    out << '\n';

    out << "Faces:\n";
    out << "  Pent  |  face:  012 013 014 023 024 034 123 124 134 234\n";
    out << "  ------+------------------------------------------------\n";
    for (pentPos = 0; pentPos < pentachora_.size(); ++pentPos) {
        pent = pentachora_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |        ";
        for (i = 0; i < 5; ++i)
            for (j = i + 1; j < 5; ++j)
                for (k = j + 1; j < 5; ++k)
                    out << ' ' << std::setw(3)
                        << faceIndex(pent->getFace(
                            Dim4Face::faceNumber[i][j][k]));
        out << '\n';
    }
    out << '\n';

    out << "Tetrahedra:\n";
    out << "  Pent  |  facet:  0123 0124 0134 0234 1234\n";
    out << "  ------+----------------------------------\n";
    for (pentPos = 0; pentPos < pentachora_.size(); ++pentPos) {
        pent = pentachora_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |         ";
        for (i = 4; i >= 0; --i)
            out << ' ' << std::setw(4) << tetrahedronIndex(
                pent->getTetrahedron(i));
        out << '\n';
    }
    out << '\n';
}

void Dim4Triangulation::insertTriangulation(const Dim4Triangulation& X) {
    ChangeEventBlock(this);

    unsigned long nOrig = getNumberOfPentachora();

    PentachoronIterator it;
    for (it = X.pentachora_.begin(); it != X.pentachora_.end(); ++it)
        addPentachoron(new Dim4Pentachoron((*it)->getDescription()));

    // Make the gluings.
    long pentPos, adjPos;
    Dim4Pentachoron* pent;
    Dim4Pentachoron* adjPent;
    NPerm5 adjPerm;
    int facet;
    pentPos = 0;
    for (it = X.pentachora_.begin(); it != X.pentachora_.end(); ++it) {
        pent = *it;
        for (facet = 0; facet < 5; ++facet) {
            adjPent = pent->adjacentPentachoron(facet);
            if (adjPent) {
                adjPos = X.pentachoronIndex(adjPent);
                adjPerm = pent->adjacentGluing(facet);
                if (adjPos > pentPos ||
                        (adjPos == pentPos && adjPerm[facet] > facet)) {
                    pentachora_[nOrig + pentPos]->joinTo(facet,
                        pentachora_[nOrig + adjPos], adjPerm);
                }
            }
        }
        ++pentPos;
    }

    gluingsHaveChanged();
}

void Dim4Triangulation::insertConstruction(unsigned long nPentachora,
        const int adjacencies[][5], const int gluings[][5][5]) {
    if (nPentachora == 0)
        return;

    Dim4Pentachoron** pent = new Dim4Pentachoron*[nPentachora];

    unsigned i, j;
    NPerm5 p;

    for (i = 0; i < nPentachora; ++i)
        pent[i] = new Dim4Pentachoron();

    for (i = 0; i < nPentachora; ++i)
        for (j = 0; j < 5; ++j)
            if (adjacencies[i][j] >= 0 &&
                    ! pent[i]->adjacentPentachoron(j)) {
                p = NPerm5(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2], gluings[i][j][3], gluings[i][j][4]);
                pent[i]->joinTo(j, pent[adjacencies[i][j]], p);
            }

    // It's not until here that we actually modify this triangulation.
    ChangeEventBlock(this);

    for (i = 0; i < nPentachora; ++i)
        addPentachoron(pent[i]);

    delete[] pent;
}

std::string Dim4Triangulation::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! getPacketLabel().empty())
        ans <<
" * 4-manifold triangulation: " << getPacketLabel() << "\n";
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (pentachora_.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * pentachoron facets.\n"
" */\n"
"\n";

    unsigned long nPentachora = pentachora_.size();
    Dim4Pentachoron* pent;
    NPerm5 perm;
    unsigned long p;
    int f, i;

    ans << "const int adjacencies[" << nPentachora << "][5] = {\n";
    for (p = 0; p < nPentachora; ++p) {
        pent = pentachora_[p];

        ans << "    { ";
        for (f = 0; f < 5; ++f) {
            if (pent->adjacentPentachoron(f)) {
                ans << pentachoronIndex(pent->adjacentPentachoron(f));
            } else
                ans << "-1";

            if (f < 4)
                ans << ", ";
            else if (p != nPentachora - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nPentachora << "][5][5] = {\n";
    for (p = 0; p < nPentachora; ++p) {
        pent = pentachora_[p];

        ans << "    { ";
        for (f = 0; f < 5; ++f) {
            if (pent->adjacentPentachoron(f)) {
                perm = pent->adjacentGluing(f);
                ans << "{ ";
                for (i = 0; i < 5; ++i) {
                    ans << perm[i];
                    if (i < 4)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else
                ans << "{ 0, 0, 0, 0, 0 }";

            if (f < 4)
                ans << ", ";
            else if (p != nPentachora - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code actually constructs a 4-manifold triangulation\n"
" * based on the information stored in the arrays above.\n"
" */\n"
"\n"
"Dim4Triangulation tri;\n"
"tri.insertConstruction(" << nPentachora << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}

void Dim4Triangulation::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the pentachoron gluings.
    PentachoronIterator it;
    Dim4Pentachoron* adjPent;
    int facet;

    out << "  <pentachora npent=\"" << pentachora_.size() << "\">\n";
    for (it = pentachora_.begin(); it != pentachora_.end(); ++it) {
        out << "    <pent desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (facet = 0; facet < 5; ++facet) {
            adjPent = (*it)->adjacentPentachoron(facet);
            if (adjPent) {
                out << pentachoronIndex(adjPent) << ' '
                    << (*it)->adjacentGluing(facet).getPermCode() << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</pent>\n";
    }
    out << "  </pentachora>\n";

    if (H1_.known()) {
        out << "  <H1>";
        H1_.value()->writeXMLData(out);
        out << "</H1>\n";
    }
    if (H2_.known()) {
        out << "  <H2>";
        H2_.value()->writeXMLData(out);
        out << "</H2>\n";
    }
}

void Dim4Triangulation::cloneFrom(const Dim4Triangulation& X) {
    ChangeEventBlock(this);

    removeAllPentachora();

    PentachoronIterator it;
    for (it = X.pentachora_.begin(); it != X.pentachora_.end(); ++it)
        addPentachoron(new Dim4Pentachoron((*it)->getDescription()));

    // Make the gluings.
    long pentPos, adjPos;
    Dim4Pentachoron* pent;
    Dim4Pentachoron* adjPent;
    NPerm5 adjPerm;
    int facet;
    pentPos = 0;
    for (it = X.pentachora_.begin(); it != X.pentachora_.end(); ++it) {
        pent = *it;
        for (facet = 0; facet < 5; ++facet) {
            adjPent = pent->adjacentPentachoron(facet);
            if (adjPent) {
                adjPos = X.pentachoronIndex(adjPent);
                adjPerm = pent->adjacentGluing(facet);
                if (adjPos > pentPos ||
                        (adjPos == pentPos && adjPerm[facet] > facet)) {
                    pentachora_[pentPos]->joinTo(facet,
                        pentachora_[adjPos], adjPerm);
                }
            }
        }
        ++pentPos;
    }
    gluingsHaveChanged();

    // Properties:
    if (X.H1_.known())
        H1_ = new NAbelianGroup(*(X.H1_.value()));
    if (X.H2_.known())
        H2_ = new NAbelianGroup(*(X.H2_.value()));
}

void Dim4Triangulation::deletePentachora() {
    for (PentachoronIterator it = pentachora_.begin();
            it != pentachora_.end(); ++it)
        delete *it;
    pentachora_.clear();
}

void Dim4Triangulation::deleteSkeleton() {
    for (VertexIterator it = vertices_.begin(); it != vertices_.end(); ++it)
        delete *it;
    for (EdgeIterator it = edges_.begin(); it != edges_.end(); ++it)
        delete *it;
    for (FaceIterator it = faces_.begin(); it != faces_.end(); ++it)
        delete *it;
    for (TetrahedronIterator it = tetrahedra_.begin();
            it != tetrahedra_.end(); ++it)
        delete *it;
    for (ComponentIterator it = components_.begin();
            it != components_.end(); ++it)
        delete *it;
    for (BoundaryComponentIterator it = boundaryComponents_.begin();
            it != boundaryComponents_.end(); ++it)
        delete *it;

    vertices_.clear();
    edges_.clear();
    faces_.clear();
    tetrahedra_.clear();
    components_.clear();
    boundaryComponents_.clear();
}

void Dim4Triangulation::clearAllProperties() {
    if (calculatedSkeleton_) {
        deleteSkeleton();
        calculatedSkeleton_ = false;
    }

    H1_.clear();
    H2_.clear();
}

} // namespace regina
