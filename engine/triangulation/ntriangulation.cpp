
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include <iostream>
#include <iomanip>
#include <sstream>

#include "file/nfile.h"
#include "triangulation/ntriangulation.h"
#include "utilities/xmlutils.h"

// Property IDs:
// #define PROPID_EXTRA_TOPOLOGY 1 -- Do not use!
#define PROPID_H1 10
#define PROPID_H1REL 11
#define PROPID_H1BDRY 12
#define PROPID_H2 13
#define PROPID_FUNDAMENTALGROUP 14

// Property IDs for properties relating to normal surfaces:
#define PROPID_ZEROEFFICIENT 201
#define PROPID_SPLITTINGSURFACE 202

namespace regina {

void NTriangulation::clearAllProperties() {
    if (calculatedSkeleton) {
        deleteSkeleton();
        calculatedSkeleton = false;
    }

    fundamentalGroup.clear();
    H1.clear();
    H1Rel.clear();
    H1Bdry.clear();
    H2.clear();
    zeroEfficient.clear();
    splittingSurface.clear();
    twoSphereBoundaryComponents.clear();
    negativeIdealBoundaryComponents.clear();
    threeSphere.clear();

    turaevViroCache.clear();
}

void NTriangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton)
        calculateSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Tetrahedra: " << tetrahedra.size() << '\n';
    out << "  Faces: " << faces.size() << '\n';
    out << "  Edges: " << edges.size() << '\n';
    out << "  Vertices: " << vertices.size() << '\n';
    out << '\n';

    NTetrahedron* tet;
    NTetrahedron* adjTet;
    unsigned tetPos;
    int face, vertex, start, end;
    NPerm adjPerm;

    out << "Tetrahedron gluing:\n";
    out << "  Tet  |  glued to:      (012)      (013)      (023)      (123)\n";
    out << "  -----+-------------------------------------------------------\n";
    for (tetPos=0; tetPos<tetrahedra.size(); tetPos++) {
        tet = tetrahedra[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |           ";
        for (face=3; face>=0; face--) {
            out << "  ";
            adjTet = tet->getAdjacentTetrahedron(face);
            if (! adjTet)
                out << " boundary";
            else {
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
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
    for (tetPos=0; tetPos<tetrahedra.size(); tetPos++) {
        tet = tetrahedra[tetPos];
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
    for (tetPos=0; tetPos<tetrahedra.size(); tetPos++) {
        tet = tetrahedra[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |        ";
        for (start=0; start<4; start++)
            for (end=start+1; end<4; end++)
                out << ' ' << std::setw(3)
                    << edgeIndex(tet->getEdge(edgeNumber[start][end]));
        out << '\n';
    }
    out << '\n';

    out << "Faces:\n";
    out << "  Tet  |  face:  012 013 023 123\n";
    out << "  -----+------------------------\n";
    for (tetPos=0; tetPos<tetrahedra.size(); tetPos++) {
        tet = tetrahedra[tetPos];
        out << "  " << std::setw(3) << tetPos << "  |        ";
        for (face=3; face>=0; face--)
            out << ' ' << std::setw(3) << faceIndex(tet->getFace(face));
        out << '\n';
    }
    out << '\n';
}

void NTriangulation::writePacket(NFile& out) const {
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NPerm adjPerm;
    int face;
    long tetPos, adjPos;

    // Write the number of tetrahedra.
    out.writeULong(tetrahedra.size());

    // Write the name of each tetrahedron.
    TetrahedronIterator it;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++)
        out.writeString((*it)->getDescription());

    // Write the joins to take place.
    tetPos = 0;
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++) {
            adjTet = tet->getAdjacentTetrahedron(face);
            if (adjTet) {
                adjPos = tetrahedronIndex(adjTet);
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                if (adjPos > tetPos ||
                        (adjPos == tetPos && adjPerm[face] > face)) {
                    // Write this gluing to file.
                    out.writeLong(tetPos);
                    out.writeInt(face);
                    out.writeLong(adjPos);
                    out.writeChar(adjPerm.getPermCode());
                }
            }
        }
        tetPos++;
    }

    // Signify no more joins to take place.
    out.writeLong(-1);

    // Write the properties.
    std::streampos bookmark(0);

    if (fundamentalGroup.known()) {
        bookmark = out.writePropertyHeader(PROPID_FUNDAMENTALGROUP);
        fundamentalGroup.value()->writeToFile(out);
        out.writePropertyFooter(bookmark);
    }
    if (H1.known()) {
        bookmark = out.writePropertyHeader(PROPID_H1);
        H1.value()->writeToFile(out);
        out.writePropertyFooter(bookmark);
    }
    if (H1Rel.known()) {
        bookmark = out.writePropertyHeader(PROPID_H1REL);
        H1Rel.value()->writeToFile(out);
        out.writePropertyFooter(bookmark);
    }
    if (H1Bdry.known()) {
        bookmark = out.writePropertyHeader(PROPID_H1BDRY);
        H1Bdry.value()->writeToFile(out);
        out.writePropertyFooter(bookmark);
    }
    if (H2.known()) {
        bookmark = out.writePropertyHeader(PROPID_H2);
        H2.value()->writeToFile(out);
        out.writePropertyFooter(bookmark);
    }
    if (zeroEfficient.known()) {
        bookmark = out.writePropertyHeader(PROPID_ZEROEFFICIENT);
        out.writeBool(zeroEfficient.value());
        out.writePropertyFooter(bookmark);
    }
    if (splittingSurface.known()) {
        bookmark = out.writePropertyHeader(PROPID_SPLITTINGSURFACE);
        out.writeBool(splittingSurface.value());
        out.writePropertyFooter(bookmark);
    }

    out.writeAllPropertiesFooter();
}

NTriangulation* NTriangulation::readPacket(NFile& in, NPacket* /* parent */) {
    NTriangulation* triang = new NTriangulation();
    NTetrahedron* tet;

    // Create new tetrahedra.
    unsigned long nTet = in.readULong();
    for (unsigned long i=0; i<nTet; i++) {
        tet = new NTetrahedron();
        tet->setDescription(in.readString());
        triang->addTetrahedron(tet);
    }

    // Read in the joins.
    long tetPos, altPos;
    int face;
    NPerm adjPerm;

    tetPos = in.readLong();
    while (tetPos >= 0) {
        face = in.readInt();
        altPos = in.readLong();
        adjPerm = in.readChar();
        triang->tetrahedra[tetPos]->joinTo(face,
            triang->tetrahedra[altPos], NPerm(adjPerm));
        tetPos = in.readLong();
    }

    // Read in properties.
    in.readProperties(triang);

    // Return the completed triangulation.
    return triang;
}

void NTriangulation::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROPID_FUNDAMENTALGROUP)
        fundamentalGroup = NGroupPresentation::readFromFile(infile);
    if (propType == PROPID_H1)
        H1 = NAbelianGroup::readFromFile(infile);
    if (propType == PROPID_H1REL)
        H1Rel = NAbelianGroup::readFromFile(infile);
    if (propType == PROPID_H1BDRY)
        H1Bdry = NAbelianGroup::readFromFile(infile);
    if (propType == PROPID_H2)
        H2 = NAbelianGroup::readFromFile(infile);
    if (propType == PROPID_ZEROEFFICIENT)
        zeroEfficient = infile.readBool();
    if (propType == PROPID_SPLITTINGSURFACE)
        splittingSurface = infile.readBool();
}

void NTriangulation::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the tetrahedron gluings.
    TetrahedronIterator it;
    NTetrahedron* adjTet;
    int face;

    out << "  <tetrahedra ntet=\"" << tetrahedra.size() << "\">\n";
    for (it = tetrahedra.begin(); it != tetrahedra.end(); it++) {
        out << "    <tet desc=\"" <<
            xmlEncodeSpecialChars((*it)->getDescription()) << "\"> ";
        for (face = 0; face < 4; face++) {
            adjTet = (*it)->getAdjacentTetrahedron(face);
            if (adjTet) {
                out << tetrahedronIndex(adjTet) << ' '
                    << static_cast<int>((*it)->
                        getAdjacentTetrahedronGluing(face).getPermCode())
                    << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</tet>\n";
    }
    out << "  </tetrahedra>\n";

    if (fundamentalGroup.known()) {
        out << "  <fundgroup>\n";
        fundamentalGroup.value()->writeXMLData(out);
        out << "  </fundgroup>\n";
    }
    if (H1.known()) {
        out << "  <H1>";
        H1.value()->writeXMLData(out);
        out << "</H1>\n";
    }
    if (H1Rel.known()) {
        out << "  <H1Rel>";
        H1Rel.value()->writeXMLData(out);
        out << "</H1Rel>\n";
    }
    if (H1Bdry.known()) {
        out << "  <H1Bdry>";
        H1Bdry.value()->writeXMLData(out);
        out << "</H1Bdry>\n";
    }
    if (H2.known()) {
        out << "  <H2>";
        H2.value()->writeXMLData(out);
        out << "</H2>\n";
    }
    if (twoSphereBoundaryComponents.known())
        out << "  " << xmlValueTag("twosphereboundarycomponents",
            twoSphereBoundaryComponents.value()) << '\n';
    if (negativeIdealBoundaryComponents.known())
        out << "  " << xmlValueTag("negativeidealboundarycomponents",
            negativeIdealBoundaryComponents.value()) << '\n';
    if (zeroEfficient.known())
        out << "  " << xmlValueTag("zeroeff", zeroEfficient.value())
            << '\n';
    if (splittingSurface.known())
        out << "  " << xmlValueTag("splitsfce", splittingSurface.value())
            << '\n';
    if (threeSphere.known())
        out << "  " << xmlValueTag("threesphere", threeSphere.value()) << '\n';
    if (! turaevViroCache.empty()) {
        for (TuraevViroSet::const_iterator it = turaevViroCache.begin();
                it != turaevViroCache.end(); it++)
            out << "  <turaevviro r=\"" << (*it).first.first
                << "\" root=\"" << (*it).first.second
                << "\" value=\"" << (*it).second << "\"/>\n";
    }
}

NTriangulation* NTriangulation::enterTextTriangulation(std::istream& in,
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
        triang->addTetrahedron(new NTetrahedron());

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
        tet = triang->tetrahedra[tetPos];
        altTet = triang->tetrahedra[altPos];
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
        if (tet->getAdjacentTetrahedron(face) ||
                altTet->getAdjacentTetrahedron(altFace)) {
            out << "One of these faces is already glued to something else.\n";
            continue;
        }

        tet->joinTo(face, altTet,
            NPerm(vertices[0], vertices[3], vertices[1], vertices[4],
                vertices[2], vertices[5], face, altFace));
        out << '\n';
    }
    triang->gluingsHaveChanged();

    out << "Finished reading gluings.\n";
    out << "The triangulation has been successfully created.\n";
    out << '\n';

    // Return the completed triangulation.
    return triang;
}

long NTriangulation::getEulerCharManifold() const {
    // Begin with V - E + F - T.
    // This call to getEulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = getEulerCharTri();

    // Truncate any ideal vertices.
    for (BoundaryComponentIterator it = boundaryComponents.begin();
            it != boundaryComponents.end(); ++it)
        if ((*it)->isIdeal())
            ans += (*it)->getEulerCharacteristic() - 1;

    // If we have an invalid triangulation, we need to locate non-standard
    // boundary vertices and invalid edges, and truncate those unwanted bits
    // also.
    if (! valid) {
        for (VertexIterator it = vertices.begin(); it != vertices.end(); ++it)
            if ((*it)->getLink() == NVertex::NON_STANDARD_BDRY)
                ans += (*it)->getLinkEulerCharacteristic() - 1;
        for (EdgeIterator it = edges.begin(); it != edges.end(); ++it)
            if (! (*it)->isValid())
                ++ans;
    }

    return ans;
}

void NTriangulation::deleteTetrahedra() {
    for_each(tetrahedra.begin(), tetrahedra.end(), FuncDelete<NTetrahedron>());
    tetrahedra.clear();
}

void NTriangulation::deleteSkeleton() {
    for_each(vertices.begin(), vertices.end(), FuncDelete<NVertex>());
    for_each(edges.begin(), edges.end(), FuncDelete<NEdge>());
    for_each(faces.begin(), faces.end(), FuncDelete<NFace>());
    for_each(components.begin(), components.end(), FuncDelete<NComponent>());
    for_each(boundaryComponents.begin(), boundaryComponents.end(),
        FuncDelete<NBoundaryComponent>());

    vertices.clear();
    edges.clear();
    faces.clear();
    components.clear();
    boundaryComponents.clear();
}

void NTriangulation::cloneFrom(const NTriangulation& X) {
    ChangeEventBlock(this);

    removeAllTetrahedra();

    TetrahedronIterator it;
    for (it = X.tetrahedra.begin(); it != X.tetrahedra.end(); it++)
        addTetrahedron(new NTetrahedron((*it)->getDescription()));

    // Make the gluings.
    long tetPos, adjPos;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NPerm adjPerm;
    int face;
    tetPos = 0;
    for (it = X.tetrahedra.begin(); it != X.tetrahedra.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++) {
            adjTet = tet->getAdjacentTetrahedron(face);
            if (adjTet) {
                adjPos = X.tetrahedronIndex(adjTet);
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                if (adjPos > tetPos ||
                        (adjPos == tetPos && adjPerm[face] > face)) {
                    tetrahedra[tetPos]->joinTo(face,
                        tetrahedra[adjPos], adjPerm);
                }
            }
        }
        tetPos++;
    }
    gluingsHaveChanged();

    // Properties:
    if (X.fundamentalGroup.known())
        fundamentalGroup= new NGroupPresentation(*X.fundamentalGroup.value());
    if (X.H1.known())
        H1 = new NAbelianGroup(*(X.H1.value()));
    if (X.H1Rel.known())
        H1Rel = new NAbelianGroup(*(X.H1Rel.value()));
    if (X.H1Bdry.known())
        H1Bdry = new NAbelianGroup(*(X.H1Bdry.value()));
    if (X.H2.known())
        H2 = new NAbelianGroup(*(X.H2.value()));

    twoSphereBoundaryComponents = X.twoSphereBoundaryComponents;
    negativeIdealBoundaryComponents = X.negativeIdealBoundaryComponents;
    zeroEfficient = X.zeroEfficient;
    splittingSurface = X.splittingSurface;
    threeSphere = X.threeSphere;

    turaevViroCache = X.turaevViroCache;
}

void NTriangulation::insertTriangulation(const NTriangulation& X) {
    ChangeEventBlock(this);

    unsigned long norig = getNumberOfTetrahedra();

    TetrahedronIterator it;
    for (it = X.tetrahedra.begin(); it != X.tetrahedra.end(); it++)
        addTetrahedron(new NTetrahedron((*it)->getDescription()));

    // Make the gluings.
    long tetPos, adjPos;
    NTetrahedron* tet;
    NTetrahedron* adjTet;
    NPerm adjPerm;
    int face;
    tetPos = 0;
    for (it = X.tetrahedra.begin(); it != X.tetrahedra.end(); it++) {
        tet = *it;
        for (face=0; face<4; face++) {
            adjTet = tet->getAdjacentTetrahedron(face);
            if (adjTet) {
                adjPos = X.tetrahedronIndex(adjTet);
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                if (adjPos > tetPos ||
                        (adjPos == tetPos && adjPerm[face] > face)) {
                    tetrahedra[norig + tetPos]->joinTo(face,
                        tetrahedra[norig + adjPos], adjPerm);
                }
            }
        }
        tetPos++;
    }

    gluingsHaveChanged();
}

void NTriangulation::insertConstruction(unsigned long nTetrahedra,
        const int adjacencies[][4], const int gluings[][4][4]) {
    if (nTetrahedra == 0)
        return;

    NTetrahedron** tet = new NTetrahedron*[nTetrahedra];

    unsigned i, j;
    NPerm p;

    for (i = 0; i < nTetrahedra; i++)
        tet[i] = new NTetrahedron();

    for (i = 0; i < nTetrahedra; i++)
        for (j = 0; j < 4; j++)
            if (adjacencies[i][j] >= 0 &&
                    ! tet[i]->getAdjacentTetrahedron(j)) {
                p = NPerm(gluings[i][j][0], gluings[i][j][1],
                    gluings[i][j][2], gluings[i][j][3]);
                tet[i]->joinTo(j, tet[adjacencies[i][j]], p);
            }

    // It's not until here that we actually modify this triangulation.
    ChangeEventBlock(this);

    for (i = 0; i < nTetrahedra; i++)
        addTetrahedron(tet[i]);

    delete[] tet;
}

std::string NTriangulation::dumpConstruction() const {
    std::ostringstream ans;
    ans <<
"/**\n";
    if (! getPacketLabel().empty())
        ans <<
" * Triangulation: " << getPacketLabel() << "\n";
    ans <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n";

    if (tetrahedra.empty()) {
        ans <<
"/* This triangulation is empty.  No code is being generated. */\n";
        return ans.str();
    }

    ans <<
"/**\n"
" * The following arrays describe the individual gluings of\n"
" * tetrahedron faces.\n"
" */\n"
"\n";

    unsigned long nTetrahedra = tetrahedra.size();
    NTetrahedron* tet;
    NPerm p;
    unsigned long t;
    int f, i;

    ans << "const int adjacencies[" << nTetrahedra << "][4] = {\n";
    for (t = 0; t < nTetrahedra; t++) {
        tet = tetrahedra[t];

        ans << "    { ";
        for (f = 0; f < 4; f++) {
            if (tet->getAdjacentTetrahedron(f)) {
                ans << tetrahedronIndex(tet->getAdjacentTetrahedron(f));
            } else
                ans << "-1";

            if (f < 3)
                ans << ", ";
            else if (t != nTetrahedra - 1)
                ans << "},\n";
            else
                ans << "}\n";
        }
    }
    ans << "};\n\n";

    ans << "const int gluings[" << nTetrahedra << "][4][4] = {\n";
    for (t = 0; t < nTetrahedra; t++) {
        tet = tetrahedra[t];

        ans << "    { ";
        for (f = 0; f < 4; f++) {
            if (tet->getAdjacentTetrahedron(f)) {
                p = tet->getAdjacentTetrahedronGluing(f);
                ans << "{ ";
                for (i = 0; i < 4; i++) {
                    ans << p[i];
                    if (i < 3)
                        ans << ", ";
                    else
                        ans << " }";
                }
            } else
                ans << "{ 0, 0, 0, 0 }";

            if (f < 3)
                ans << ", ";
            else if (t != nTetrahedra - 1)
                ans << " },\n";
            else
                ans << " }\n";
        }
    }
    ans << "};\n\n";

    ans <<
"/**\n"
" * The following code actually constructs a triangulation based on\n"
" * the information stored in the arrays above.\n"
" */\n"
"\n"
"NTriangulation tri;\n"
"tri.insertConstruction(" << nTetrahedra << ", adjacencies, gluings);\n"
"\n";

    return ans.str();
}

} // namespace regina

