
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include <iostream>
#include <iomanip>

#include "algebra/ngrouppresentation.h"
#include "file/nfile.h"
#include "triangulation/ntriangulation.h"

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
    if (calculatedSkeleton)
        deleteSkeleton();
    if (calculatedFundamentalGroup)
        delete fundamentalGroup;
    if (calculatedH1)
        delete H1;
    if (calculatedH1Rel)
        delete H1Rel;
    if (calculatedH1Bdry)
        delete H1Bdry;
    if (calculatedH2)
        delete H2;
        
    NPropertyHolder::clearAllProperties();
}

void NTriangulation::initialiseAllProperties() {
    calculatedSkeleton = false;
    calculatedFundamentalGroup = false;
    calculatedH1 = false;
    calculatedH1Rel = false;
    calculatedH1Bdry = false;
    calculatedH2 = false;
    calculatedZeroEfficient = false;
    calculatedSplittingSurface = false;
}

void NTriangulation::writeTextLong(std::ostream& out) const {
    if (! calculatedSkeleton)
        ((NTriangulation*)this)->calculateSkeleton();

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
                out << std::setw(3) << tetrahedra.index(adjTet) << " (";
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
                vertices.index(tet->getVertex(vertex));
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
                    << edges.index(tet->getEdge(edgeNumber[start][end]));
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
            out << ' ' << std::setw(3) << faces.index(tet->getFace(face));
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
                adjPos = tetrahedra.index(adjTet);
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

    if (calculatedFundamentalGroup) {
        bookmark = writePropertyHeader(out, PROPID_FUNDAMENTALGROUP);
        fundamentalGroup->writeToFile(out);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedH1) {
        bookmark = writePropertyHeader(out, PROPID_H1);
        H1->writeToFile(out);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedH1Rel) {
        bookmark = writePropertyHeader(out, PROPID_H1REL);
        H1Rel->writeToFile(out);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedH1Bdry) {
        bookmark = writePropertyHeader(out, PROPID_H1BDRY);
        H1Bdry->writeToFile(out);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedH2) {
        bookmark = writePropertyHeader(out, PROPID_H2);
        H2->writeToFile(out);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedZeroEfficient) {
        bookmark = writePropertyHeader(out, PROPID_ZEROEFFICIENT);
        out.writeBool(zeroEfficient);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedSplittingSurface) {
        bookmark = writePropertyHeader(out, PROPID_SPLITTINGSURFACE);
        out.writeBool(splittingSurface);
        writePropertyFooter(out, bookmark);
    }

    writeAllPropertiesFooter(out);
}

NTriangulation* NTriangulation::readPacket(NFile& in, NPacket* parent) {
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
    triang->readProperties(in);

    // Return the completed triangulation.
    return triang;
}

void NTriangulation::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROPID_FUNDAMENTALGROUP) {
        fundamentalGroup = NGroupPresentation::readFromFile(infile);
        calculatedFundamentalGroup = true;
    }
    if (propType == PROPID_H1) {
        H1 = NAbelianGroup::readFromFile(infile);
        calculatedH1 = true;
    }
    if (propType == PROPID_H1REL) {
        H1Rel = NAbelianGroup::readFromFile(infile);
        calculatedH1Rel = true;
    }
    if (propType == PROPID_H1BDRY) {
        H1Bdry = NAbelianGroup::readFromFile(infile);
        calculatedH1Bdry = true;
    }
    if (propType == PROPID_H2) {
        H2 = NAbelianGroup::readFromFile(infile);
        calculatedH2 = true;
    }
    if (propType == PROPID_ZEROEFFICIENT) {
        zeroEfficient = infile.readBool();
        calculatedZeroEfficient = true;
    }
    if (propType == PROPID_SPLITTINGSURFACE) {
        splittingSurface = infile.readBool();
        calculatedSplittingSurface = true;
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
    clearAllProperties();
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
                adjPos = X.tetrahedra.index(adjTet);
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
    if (X.calculatedFundamentalGroup) {
        fundamentalGroup= new NGroupPresentation(*(X.fundamentalGroup));
        calculatedFundamentalGroup = true;
    }
    if (X.calculatedH1) {
        H1 = new NAbelianGroup(*(X.H1));
        calculatedH1 = true;
    }
    if (X.calculatedH1Rel) {
        H1Rel = new NAbelianGroup(*(X.H1Rel));
        calculatedH1Rel = true;
    }
    if (X.calculatedH1Bdry) {
        H1Bdry = new NAbelianGroup(*(X.H1Bdry));
        calculatedH1Bdry = true;
    }
    if (X.calculatedH2) {
        H2 = new NAbelianGroup(*(X.H2));
        calculatedH2 = true;
    }
    if (X.calculatedZeroEfficient) {
        zeroEfficient = X.zeroEfficient;
        calculatedZeroEfficient = true;
    }
    if (X.calculatedSplittingSurface) {
        splittingSurface = X.splittingSurface;
        calculatedSplittingSurface = true;
    }
}

void NTriangulation::insertTriangulation(const NTriangulation& X) {
    clearAllProperties();
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
                adjPos = X.tetrahedra.index(adjTet);
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

} // namespace regina

