
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
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>

#include "foreign/snappea.h"
#include "triangulation/ntriangulation.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<3>::Triangulation(const std::string& description) {
    NTriangulation* attempt;

    if ((attempt = fromIsoSig(description))) {
        cloneFrom(*attempt);
        setPacketLabel(description);
    } else if ((attempt = rehydrate(description))) {
        cloneFrom(*attempt);
        setPacketLabel(description);
    } else if ((attempt = fromSnapPea(description))) {
        cloneFrom(*attempt);
        setPacketLabel(attempt->getPacketLabel());
    }

    delete attempt;
}

void Triangulation<3>::clearAllProperties() {
    if (calculatedSkeleton())
        deleteSkeleton();

    fundamentalGroup_.clear();
    H1_.clear();
    H1Rel_.clear();
    H1Bdry_.clear();
    H2_.clear();
    zeroEfficient_.clear();
    splittingSurface_.clear();
    twoSphereBoundaryComponents_.clear();
    negativeIdealBoundaryComponents_.clear();
    threeSphere_.clear();
    threeBall_.clear();
    solidTorus_.clear();
    irreducible_.clear();
    compressingDisc_.clear();
    haken_.clear();
    strictAngleStructure_.clear();

    niceTreeDecomposition_.clear();
    turaevViroCache_.clear();
}

void Triangulation<3>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Tetrahedra: " << simplices_.size() << '\n';
    out << "  Triangles: " << countTriangles() << '\n';
    out << "  Edges: " << countEdges() << '\n';
    out << "  Vertices: " << countVertices() << '\n';
    out << '\n';

    NTetrahedron* tet;
    NTetrahedron* adjTet;
    unsigned tetPos;
    int face, vertex, start, end;
    NPerm4 adjPerm;

    out << "Tetrahedron gluing:\n";
    out << "  Tet  |  glued to:      (012)      (013)      (023)      (123)\n";
    out << "  -----+-------------------------------------------------------\n";
    for (tetPos=0; tetPos<simplices_.size(); tetPos++) {
        tet = simplices_[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |           ";
        for (face=3; face>=0; face--) {
            out << "  ";
            adjTet = tet->adjacentTetrahedron(face);
            if (! adjTet)
                out << " boundary";
            else {
                adjPerm = tet->adjacentGluing(face);
                out << std::setw(3) << tetrahedronIndex(adjTet) << " (";
                for (vertex=0; vertex<4; vertex++) {
                    if (vertex == face) continue;
                    out << adjPerm[vertex];
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';

    out << "Vertices:\n";
    out << "  Tet  |  vertex:    0   1   2   3\n";
    out << "  -----+--------------------------\n";
    for (tetPos=0; tetPos<simplices_.size(); tetPos++) {
        tet = simplices_[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |          ";
        for (vertex=0; vertex<4; vertex++)
            out << ' ' << std::setw(3) <<
                vertexIndex(tet->getVertex(vertex));
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Tet  |  edge:   01  02  03  12  13  23\n";
    out << "  -----+--------------------------------\n";
    for (tetPos=0; tetPos<simplices_.size(); tetPos++) {
        tet = simplices_[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |        ";
        for (start=0; start<4; start++)
            for (end=start+1; end<4; end++)
                out << ' ' << std::setw(3)
                    << edgeIndex(tet->getEdge(NEdge::edgeNumber[start][end]));
        out << '\n';
    }
    out << '\n';

    out << "Triangles:\n";
    out << "  Tet  |  face:  012 013 023 123\n";
    out << "  -----+------------------------\n";
    for (tetPos=0; tetPos<simplices_.size(); tetPos++) {
        tet = simplices_[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |        ";
        for (face=3; face>=0; face--)
            out << ' ' << std::setw(3) << triangleIndex(tet->getTriangle(face));
        out << '\n';
    }
    out << '\n';
}

void Triangulation<3>::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the tetrahedron gluings.
    TetrahedronIterator it;
    NTetrahedron* adjTet;
    int face;

    out << "  <tetrahedra ntet=\"" << simplices_.size() << "\">\n";
    for (it = simplices_.begin(); it != simplices_.end(); it++) {
        out << "    <tet desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (face = 0; face < 4; face++) {
            adjTet = (*it)->adjacentTetrahedron(face);
            if (adjTet) {
                out << tetrahedronIndex(adjTet) << ' '
                    << static_cast<int>((*it)->
                        adjacentGluing(face).getPermCode())
                    << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</tet>\n";
    }
    out << "  </tetrahedra>\n";

    if (fundamentalGroup_.known()) {
        out << "  <fundgroup>\n";
        fundamentalGroup_.value()->writeXMLData(out);
        out << "  </fundgroup>\n";
    }
    if (H1_.known()) {
        out << "  <H1>";
        H1_.value()->writeXMLData(out);
        out << "</H1>\n";
    }
    if (H1Rel_.known()) {
        out << "  <H1Rel>";
        H1Rel_.value()->writeXMLData(out);
        out << "</H1Rel>\n";
    }
    if (H1Bdry_.known()) {
        out << "  <H1Bdry>";
        H1Bdry_.value()->writeXMLData(out);
        out << "</H1Bdry>\n";
    }
    if (H2_.known()) {
        out << "  <H2>";
        H2_.value()->writeXMLData(out);
        out << "</H2>\n";
    }
    if (twoSphereBoundaryComponents_.known())
        out << "  " << xmlValueTag("twosphereboundarycomponents",
            twoSphereBoundaryComponents_.value()) << '\n';
    if (negativeIdealBoundaryComponents_.known())
        out << "  " << xmlValueTag("negativeidealboundarycomponents",
            negativeIdealBoundaryComponents_.value()) << '\n';
    if (zeroEfficient_.known())
        out << "  " << xmlValueTag("zeroeff", zeroEfficient_.value())
            << '\n';
    if (splittingSurface_.known())
        out << "  " << xmlValueTag("splitsfce", splittingSurface_.value())
            << '\n';
    if (threeSphere_.known())
        out << "  " << xmlValueTag("threesphere", threeSphere_.value()) << '\n';
    if (threeBall_.known())
        out << "  " << xmlValueTag("threeball", threeBall_.value()) << '\n';
    if (solidTorus_.known())
        out << "  " << xmlValueTag("solidtorus", solidTorus_.value()) << '\n';
    if (irreducible_.known())
        out << "  " << xmlValueTag("irreducible", irreducible_.value())
            << '\n';
    if (compressingDisc_.known())
        out << "  " << xmlValueTag("compressingdisc", compressingDisc_.value())
            << '\n';
    if (haken_.known())
        out << "  " << xmlValueTag("haken", haken_.value()) << '\n';
}

NTriangulation* Triangulation<3>::enterTextTriangulation(std::istream& in,
        std::ostream& out) {
    NTriangulation* triang = new NTriangulation();
    NTetrahedron* tet;
    long nTet;

    // Create new tetrahedra.
    out << "Number of tetrahedra: ";
    in >> nTet;
    while (nTet < 0) {
        out << "The number of tetrahedra must be non-negative.\n";
        out << "Number of tetrahedra: ";
        in >> nTet;
    }
    out << '\n';

    for (long i=0; i<nTet; i++)
        triang->newTetrahedron();

    // Read in the joins.
    long tetPos, altPos;
    int face, altFace;
    NTetrahedron* altTet;
    int vertices[6];

    out << "Tetrahedra are numbered from 0 to " << nTet-1 << ".\n";
    out << "Vertices are numbered from 0 to 3.\n";
    out << "Enter in the face gluings one at a time.\n";
    out << '\n';
    while(1) {
        out << "Enter two tetrahedra to glue, separated by a space, or ";
        out << "-1 if finished: ";
        in >> tetPos;
        if (tetPos < 0) break;
        in >> altPos;
        if (altPos < 0) break;
        if (tetPos >= nTet || altPos >= nTet) {
            out << "Tetrahedron identifiers must be between 0 and "
                << nTet-1 << " inclusive.\n";
            continue;
        }
        tet = triang->simplices_[tetPos];
        altTet = triang->simplices_[altPos];
        out << "Enter the three vertices of the first tetrahedron ("
            << tetPos << "), separated by spaces,\n";
        out << "    that will form one face of the gluing: ";
        in >> vertices[0] >> vertices[1] >> vertices[2];
        out << "Enter the corresponding three vertices of the second tetrahedron ("
            << altPos << "): ";
        in >> vertices[3] >> vertices[4] >> vertices[5];

        if (vertices[3] < 0 || vertices[3] > 3 || vertices[4] < 0
                || vertices[4] > 3 || vertices[5] < 0 || vertices[5] > 3
                || vertices[0] < 0 || vertices[0] > 3 || vertices[1] < 0
                || vertices[1] > 3 || vertices[2] < 0 || vertices[2] > 3) {
            out << "Vertices must be between 0 and 3 inclusive.\n";
            continue;
        }
        if (vertices[0] == vertices[1] || vertices[1] == vertices[2]
                || vertices[2] == vertices[0]) {
            out << "The three vertices for tetrahedron " << tetPos
                << " must be different.\n";
            continue;
        }
        if (vertices[3] == vertices[4] || vertices[4] == vertices[5]
                || vertices[5] == vertices[3]) {
            out << "The three vertices for tetrahedron " << altPos
                << " must be different.\n";
            continue;
        }

        face = 6 - vertices[0] - vertices[1] - vertices[2];
        altFace = 6 - vertices[3] - vertices[4] - vertices[5];

        if (face == altFace && tetPos == altPos) {
            out << "You cannot glue a face to itself.\n";
            continue;
        }
        if (tet->adjacentTetrahedron(face) ||
                altTet->adjacentTetrahedron(altFace)) {
            out << "One of these faces is already glued to something else.\n";
            continue;
        }

        tet->joinTo(face, altTet,
            NPerm4(vertices[0], vertices[3], vertices[1], vertices[4],
                vertices[2], vertices[5], face, altFace));
        out << '\n';
    }

    out << "Finished reading gluings.\n";
    out << "The triangulation has been successfully created.\n";
    out << '\n';

    // Return the completed triangulation.
    return triang;
}

long Triangulation<3>::getEulerCharManifold() const {
    // Begin with V - E + F - T.
    // This call to getEulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = getEulerCharTri();

    // Truncate any ideal vertices.
    for (BoundaryComponentIterator it = boundaryComponents_.begin();
            it != boundaryComponents_.end(); ++it)
        if ((*it)->isIdeal())
            ans += (*it)->getEulerChar() - 1;

    // If we have an invalid triangulation, we need to locate non-standard
    // boundary vertices and invalid edges, and truncate those unwanted bits
    // also.
    if (! valid_) {
        for (NVertex* v : getVertices())
            if (v->getLink() == NVertex::NON_STANDARD_BDRY)
                ans += v->getLinkEulerChar() - 1;
        for (NEdge* e : getEdges())
            if (! e->isValid())
                ++ans;
    }

    return ans;
}

void Triangulation<3>::deleteSkeleton() {
    for (auto b : boundaryComponents_)
        delete b;
    boundaryComponents_.clear();

    TriangulationBase<3>::deleteSkeleton();
}

void Triangulation<3>::cloneFrom(const NTriangulation& X) {
    ChangeEventSpan span(this);

    removeAllTetrahedra();

    TetrahedronIterator it;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); it++)
        newTetrahedron((*it)->getDescription());

    // Make the gluings.
    long tetPos, adjPos;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NPerm4 adjPerm;
    int face;
    tetPos = 0;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++) {
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet) {
                adjPos = X.tetrahedronIndex(adjTet);
                adjPerm = tet->adjacentGluing(face);
                if (adjPos > tetPos ||
                        (adjPos == tetPos && adjPerm[face] > face)) {
                    simplices_[tetPos]->joinTo(face,
                        simplices_[adjPos], adjPerm);
                }
            }
        }
        tetPos++;
    }

    // Properties:
    if (X.fundamentalGroup_.known())
        fundamentalGroup_= new NGroupPresentation(*X.fundamentalGroup_.value());
    if (X.H1_.known())
        H1_ = new NAbelianGroup(*(X.H1_.value()));
    if (X.H1Rel_.known())
        H1Rel_ = new NAbelianGroup(*(X.H1Rel_.value()));
    if (X.H1Bdry_.known())
        H1Bdry_ = new NAbelianGroup(*(X.H1Bdry_.value()));
    if (X.H2_.known())
        H2_ = new NAbelianGroup(*(X.H2_.value()));

    twoSphereBoundaryComponents_ = X.twoSphereBoundaryComponents_;
    negativeIdealBoundaryComponents_ = X.negativeIdealBoundaryComponents_;
    zeroEfficient_ = X.zeroEfficient_;
    splittingSurface_ = X.splittingSurface_;
    threeSphere_ = X.threeSphere_;
    threeBall_ = X.threeBall_;
    solidTorus_ = X.solidTorus_;
    irreducible_ = X.irreducible_;
    compressingDisc_ = X.compressingDisc_;
    haken_ = X.haken_;

    if (X.strictAngleStructure_.known()) {
        if (X.strictAngleStructure_.value())
            strictAngleStructure_ = new NAngleStructure(this,
                new NAngleStructureVector(
                *X.strictAngleStructure_.value()->rawVector()));
        else
            strictAngleStructure_ = 0;
    }

    turaevViroCache_ = X.turaevViroCache_;
}

std::string Triangulation<3>::snapPea() const {
    std::ostringstream out;
    snapPea(out);
    return out.str();
}

void Triangulation<3>::snapPea(std::ostream& out) const {
    // Sanity checks.
    if ((! isValid()) || hasBoundaryTriangles() || simplices_.empty())
        return;

    // Write header information.
    out << "% Triangulation\n";
    if (getPacketLabel().empty())
        out << "Regina_Triangulation\n";
    else
        out << stringToToken(getPacketLabel()) << '\n';

    // Write general details.
    out << "not_attempted 0.0\n";
    out << "unknown_orientability\n";
    out << "CS_unknown\n";

    // Write cusps.
    out << "0 0\n";

    // Write tetrahedra.
    out << size() << '\n';

    int i, j;
    for (Triangulation<3>::TetrahedronIterator it = getTetrahedra().begin();
            it != getTetrahedra().end(); it++) {
        // Although our precondition states that there are no boundary
        // triangles, we test for this anyway.  If somebody makes a mistake and
        // calls this routine with a bounded triangulation, we don't want
        // to wind up calling tetrahedronIndex(0) and crashing.
        for (i = 0; i < 4; i++)
            if ((*it)->adjacentTetrahedron(i))
                out << "   " << tetrahedronIndex(
                    (*it)->adjacentTetrahedron(i)) << ' ';
            else
                out << "   -1 ";
        out << '\n';
        for (i = 0; i < 4; i++)
            out << ' ' << (*it)->adjacentGluing(i).str();
        out << '\n';

        // Incident cusps.
        for (i = 0; i < 4; i++)
            out << "  -1 ";
        out << '\n';

        // Meridians and longitudes.
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 16; j++)
                out << "  0";
            out << '\n';
        }

        // Tetrahedron shape.
        out << "0.0 0.0\n";
    }
}

bool Triangulation<3>::saveSnapPea(const char* filename) const {
    // Sanity checks.
    if ((! isValid()) || hasBoundaryTriangles() || simplices_.empty())
        return false;

    std::ofstream out(filename);
    if (!out)
        return false;
    snapPea(out);
    return true;
}

std::string Triangulation<3>::recogniser() const {
    std::ostringstream out;
    recogniser(out);
    return out.str();
}

std::string Triangulation<3>::recognizer() const {
    std::ostringstream out;
    recogniser(out);
    return out.str();
}

void Triangulation<3>::recogniser(std::ostream& out) const {
    // Sanity checks.
    if ((! isValid()) || hasBoundaryTriangles())
        return;

    // Write the header.
    out << "triangulation" << std::endl;

    // Write face gluings.
    NTriangle* f;
    NTetrahedron* tet;
    NPerm4 vert;
    for (unsigned i = 0; i < countTriangles(); ++i) {
        f = getTriangle(i);

        tet = f->getEmbedding(0).getTetrahedron();
        vert = f->getEmbedding(0).getVertices();
        out << 't' << (tetrahedronIndex(tet) + 1)
            << '(' << (vert[0] + 1)
            << ',' << (vert[1] + 1)
            << ',' << (vert[2] + 1) << ") - ";

        tet = f->getEmbedding(1).getTetrahedron();
        vert = f->getEmbedding(1).getVertices();
        out << 't' << (tetrahedronIndex(tet) + 1)
            << '(' << (vert[0] + 1)
            << ',' << (vert[1] + 1)
            << ',' << (vert[2] + 1) << ')';

        if (i != countTriangles() - 1)
            out << ',';
        out << std::endl;
    }

    // Write the footer.
    out << "end" << std::endl;
}

bool Triangulation<3>::saveRecogniser(const char* filename) const {
    // Sanity checks.
    if ((! isValid()) || hasBoundaryTriangles())
        return false;

    // Write to file or stdout as appropriate.
    std::ofstream out(filename);
    if (! out)
        return false;
    recogniser(out);
    return true;
}

NTriangulation* Triangulation<3>::fromSnapPea(const std::string& snapPeaData) {
    std::istringstream in(snapPeaData);
    return readSnapPea(in);
}

} // namespace regina

