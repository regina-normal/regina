
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

#include "file/nfile.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/flavourregistry.h"
#include "triangulation/ntriangulation.h"

#define __FLAVOUR_REGISTRY_BODY

// Property IDs:
#define PROPID_EULERCHARACTERISTIC 1
#define PROPID_REALBOUNDARY 5
#define PROPID_COMPACT 6
#define PROPID_ORIENTABILITY 7
#define PROPID_TWOSIDEDNESS 8
#define PROPID_CONNECTEDNESS 9
#define PROPID_SURFACENAME 100

const int vertexSplit[4][4] = {
    { -1, 0, 1, 2 },
    {  0,-1, 2, 1 },
    {  1, 2,-1, 0 },
    {  2, 1, 0,-1 }
};

const int vertexSplitMeeting[4][4][2] = {
    { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
    { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
    { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
    { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
};

const int vertexSplitDefn[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 }
};

const int vertexSplitPartner[3][4] = {
    { 1, 0, 3, 2},
    { 2, 3, 0, 1},
    { 3, 2, 1, 0}
};

const char vertexSplitString[3][6] = { "01/23", "02/13", "03/12" };

// The following three arrays cannot be made 2-D because of a g++-2.95 bug.

const NPerm __triDiscArcs[12] = {
    NPerm(0,1,2,3), NPerm(0,2,3,1), NPerm(0,3,1,2),
    NPerm(1,0,3,2), NPerm(1,3,2,0), NPerm(1,2,0,3),
    NPerm(2,3,0,1), NPerm(2,0,1,3), NPerm(2,1,3,0),
    NPerm(3,2,1,0), NPerm(3,1,0,2), NPerm(3,0,2,1)
};

const NPerm __quadDiscArcs[12] = {
    NPerm(0,2,3,1), NPerm(3,0,1,2), NPerm(1,3,2,0), NPerm(2,1,0,3),
    NPerm(0,3,1,2), NPerm(1,0,2,3), NPerm(2,1,3,0), NPerm(3,2,0,1),
    NPerm(0,1,2,3), NPerm(2,0,3,1), NPerm(3,2,1,0), NPerm(1,3,0,2)
};

const NPerm __octDiscArcs[24] = {
    NPerm(0,3,1,2), NPerm(0,1,2,3), NPerm(2,0,3,1), NPerm(2,3,1,0),
    NPerm(1,2,0,3), NPerm(1,0,3,2), NPerm(3,1,2,0), NPerm(3,2,0,1),
    NPerm(0,1,2,3), NPerm(0,2,3,1), NPerm(3,0,1,2), NPerm(3,1,2,0),
    NPerm(2,3,0,1), NPerm(2,0,1,3), NPerm(1,2,3,0), NPerm(1,3,0,2),
    NPerm(0,2,3,1), NPerm(0,3,1,2), NPerm(1,0,2,3), NPerm(1,2,3,0),
    NPerm(3,1,0,2), NPerm(3,0,2,1), NPerm(2,3,1,0), NPerm(2,1,0,3)
};

NNormalSurface* NNormalSurface::clone() const {
    NNormalSurface* ans = new NNormalSurface(triangulation,
        (NNormalSurfaceVector*)vector->clone());
    if (calculatedEulerChar) {
        ans->eulerChar = eulerChar;
        ans->calculatedEulerChar = true;
    }
    if (calculatedOrientable) {
        ans->orientable = orientable;
        ans->calculatedOrientable = true;
    }
    if (calculatedTwoSided) {
        ans->twoSided = twoSided;
        ans->calculatedTwoSided = true;
    }
    if (calculatedConnected) {
        ans->connected = connected;
        ans->calculatedConnected = true;
    }
    if (calculatedRealBoundary) {
        ans->realBoundary = realBoundary;
        ans->calculatedRealBoundary = true;
    }
    if (calculatedCompact) {
        ans->compact = compact;
        ans->calculatedCompact = true;
    }
    return ans;
}

void NNormalSurface::readIndividualProperty(NFile& infile,
        unsigned propType) {
    if (propType == PROPID_EULERCHARACTERISTIC) {
        eulerChar = infile.readLarge();
        calculatedEulerChar = true;
    }
    else if (propType == PROPID_ORIENTABILITY) {
        orientable = infile.readInt();
        calculatedOrientable = true;
    }
    else if (propType == PROPID_TWOSIDEDNESS) {
        twoSided = infile.readInt();
        calculatedTwoSided = true;
    }
    else if (propType == PROPID_CONNECTEDNESS) {
        connected = infile.readInt();
        calculatedConnected = true;
    }
    else if (propType == PROPID_REALBOUNDARY) {
        realBoundary = infile.readBool();
        calculatedRealBoundary = true;
    }
    else if (propType == PROPID_COMPACT) {
        compact = infile.readBool();
        calculatedCompact = true;
    }
    else if (propType == PROPID_SURFACENAME)
        name = infile.readString();
}

void NNormalSurface::initialiseAllProperties() {
    calculatedEulerChar = false;
    calculatedOrientable = false;
    calculatedTwoSided = false;
    calculatedConnected = false;
    calculatedRealBoundary = false;
    calculatedCompact = false;
}

void NNormalSurface::writeTextShort(std::ostream& out) const {
    unsigned long nTets = triangulation->getNumberOfTetrahedra();
    unsigned long tet;
    unsigned j;
    bool almostNormal = vector->allowsAlmostNormal();
    for (tet=0; tet<nTets; tet++) {
        if (tet > 0)
            out << " || ";
        for (j=0; j<4; j++)
            out << getTriangleCoord(tet, j) << ' ';
        out << ';';
        for (j=0; j<3; j++)
            out << ' ' << getQuadCoord(tet, j);
        if (almostNormal) {
            out << " ;";
            for (j=0; j<3; j++)
                out << ' ' << getOctCoord(tet, j);
        }
    }
}

bool NNormalSurfaceVector::hasMultipleOctDiscs(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    int oct;
    NLargeInteger coord;
    for (unsigned long tet=0; tet<nTets; tet++)
        for (oct=0; oct<3; oct++) {
            coord = getOctCoord(tet, oct, triang);
            if (coord == 0)
                continue;
            // We have found our one and only oct type!
            if (coord == 1)
                return false;
            return true;
        }
    // There are no octahedral types at all.
    return false;
}

bool NNormalSurfaceVector::isCompact(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (getTriangleCoord(tet, type, triang).isInfinite())
                return false;
        for (type = 0; type < 3; type++)
            if (getQuadCoord(tet, type, triang).isInfinite())
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang).isInfinite())
                    return false;
    return true;
}

bool NNormalSurfaceVector::isVertexLinking(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 3; type++)
            if (getQuadCoord(tet, type, triang) != 0)
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return false;
    return true;
}

bool NNormalSurfaceVector::isSplitting(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    NLargeInteger tot;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (getTriangleCoord(tet, type, triang) != 0)
                return false;
        tot = (long)0;
        for (type = 0; type < 3; type++)
            tot += getQuadCoord(tet, type, triang);
        if (tot != 1)
            return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return false;
    return true;
}

void NNormalSurface::calculateEulerCharacteristic() {
    unsigned long index, tot;
    int type;
    eulerChar = NLargeInteger::zero;

    // Add vertices.
    tot = triangulation->getNumberOfEdges();
    for (index = 0; index < tot; index++)
        eulerChar += getEdgeWeight(index);

    // Subtract edges.
    tot = triangulation->getNumberOfFaces();
    for (index = 0; index < tot; index++)
        for (type = 0; type < 3; type++)
            eulerChar -= getFaceArcs(index, type);
    
    // Add faces.
    tot = triangulation->getNumberOfTetrahedra();
    for (index = 0; index < tot; index++) {
        for (type=0; type<4; type++)
            eulerChar += getTriangleCoord(index, type);
        for (type=0; type<3; type++)
            eulerChar += getQuadCoord(index, type);
        for (type=0; type<3; type++)
            eulerChar += getOctCoord(index, type);
    }

    // Done!
    calculatedEulerChar = true;
}

void NNormalSurface::calculateRealBoundary() {
    if (triangulation->isClosed()) {
        realBoundary = false;
        calculatedRealBoundary = true;
        return;
    }
    
    unsigned long index;
    unsigned long tot = triangulation->getNumberOfTetrahedra();
    NTetrahedron* tet;
    int type, face;

    for (index = 0; index < tot; index++) {
        tet = triangulation->getTetrahedron(index);
        if (tet->hasBoundary()) {
            // Check for disk types with boundary
            for (type=0; type<3; type++) {
                if (getQuadCoord(index, type) > 0) {
                    realBoundary = true;
                    calculatedRealBoundary = true;
                    return;
                }
            }
            for (type=0; type<3; type++) {
                if (getOctCoord(index, type) > 0) {
                    realBoundary = true;
                    calculatedRealBoundary = true;
                    return;
                }
            }
            for (type=0; type<4; type++)
                if (getTriangleCoord(index, type) > 0) {
                    // Make sure the triangle actually hits the
                    // boundary.
                    for (face=0; face<4; face++) {
                        if (face == type)
                            continue;
                        if (tet->getAdjacentTetrahedron(face) == 0) {
                            realBoundary = true;
                            calculatedRealBoundary = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary = false;
    calculatedRealBoundary = true;
}

void NNormalSurface::writeToFile(NFile& out) const {
    // Write the vector length.
    unsigned vecLen = vector->size();
    out.writeUInt(vecLen);

    // Write all non-zero entries.
    NLargeInteger entry;
    for (unsigned i=0; i<vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0) {
            out.writeInt(i);
            out.writeLarge(entry);
        }
    }
    out.writeInt(-1);
    
    // Write properties.
    std::streampos bookmark(0);

    bookmark = writePropertyHeader(out, PROPID_SURFACENAME);
    out.writeString(name);
    writePropertyFooter(out, bookmark);

    if (calculatedEulerChar) {
        bookmark = writePropertyHeader(out, PROPID_EULERCHARACTERISTIC);
        out.writeLarge(eulerChar);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedOrientable) {
        bookmark = writePropertyHeader(out, PROPID_ORIENTABILITY);
        out.writeInt(orientable);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedTwoSided) {
        bookmark = writePropertyHeader(out, PROPID_TWOSIDEDNESS);
        out.writeInt(twoSided);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedConnected) {
        bookmark = writePropertyHeader(out, PROPID_CONNECTEDNESS);
        out.writeInt(connected);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedRealBoundary) {
        bookmark = writePropertyHeader(out, PROPID_REALBOUNDARY);
        out.writeBool(realBoundary);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedCompact) {
        bookmark = writePropertyHeader(out, PROPID_COMPACT);
        out.writeBool(compact);
        writePropertyFooter(out, bookmark);
    }

    writeAllPropertiesFooter(out);
}

#define REGISTER_FLAVOUR(id_name, class, n, a, t) \
    if (flavour == NNormalSurfaceList::id_name) \
        vector = new class(vecLen); \
    else

NNormalSurface* NNormalSurface::readFromFile(NFile& in, int flavour,
        NTriangulation* triangulation) {
    // Read the vector length and make a new vector.
    unsigned vecLen = in.readUInt();
    NNormalSurfaceVector* vector;

    // Bring in cases from the flavour registry.
    #include "surfaces/flavourregistry.h"
        return 0; // Final else statement.

    // Read all non-zero vector entries.
    int vecPos = in.readInt();
    while (vecPos != -1) {
        vector->setElement(vecPos, in.readLarge());
        vecPos = in.readInt();
    }

    NNormalSurface* ans = new NNormalSurface(triangulation, vector);

    // Read in properties.
    ans->readProperties(in);

    return ans;
}

