
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

#include <algorithm>
#include "file/nfile.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/flavourregistry.h"
#include "triangulation/ntriangulation.h"
#include "utilities/xmlutils.h"

#define __FLAVOUR_REGISTRY_BODY

// Property IDs:
#define PROPID_EULERCHARACTERISTIC 1
#define PROPID_REALBOUNDARY 5
#define PROPID_COMPACT 6
#define PROPID_ORIENTABILITY 7
#define PROPID_TWOSIDEDNESS 8
#define PROPID_CONNECTEDNESS 9
#define PROPID_CANCRUSH 10
#define PROPID_SURFACENAME 100

namespace regina {

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
        dynamic_cast<NNormalSurfaceVector*>(vector->clone()));

    ans->eulerChar = eulerChar;
    ans->orientable = orientable;
    ans->twoSided = twoSided;
    ans->connected = connected;
    ans->realBoundary = realBoundary;
    ans->compact = compact;
    ans->canCrush = canCrush;

    return ans;
}

NNormalSurface* NNormalSurface::doubleSurface() const {
    NNormalSurface* ans = new NNormalSurface(triangulation,
        dynamic_cast<NNormalSurfaceVector*>(vector->clone()));

    (*(ans->vector)) *= 2;

    // Some properties can be copied straight across.
    ans->realBoundary = realBoundary;
    ans->compact = compact;
    if (eulerChar.known())
        ans->eulerChar = eulerChar.value() * 2;

    // The following three properties can be used together to deduce how
    // they change in the clone.  However, until we sit down and check
    // through all possible cases we'll just leave them marked unknown.

    // TODO: ans->orientable, ans->twoSided, ans->connected

    // And some other properties are best left recalculated.

    return ans;
}

void NNormalSurface::readIndividualProperty(NFile& infile,
        unsigned propType) {
    if (propType == PROPID_EULERCHARACTERISTIC)
        eulerChar = infile.readLarge();
    else if (propType == PROPID_ORIENTABILITY) {
        int val = infile.readInt();
        if (val == 1)
            orientable = true;
        else if (val == -1)
            orientable = false;
        else
            orientable = NTriBool::Unknown;
    } else if (propType == PROPID_TWOSIDEDNESS) {
        int val = infile.readInt();
        if (val == 1)
            twoSided = true;
        else if (val == -1)
            twoSided = false;
        else
            twoSided = NTriBool::Unknown;
    } else if (propType == PROPID_CONNECTEDNESS) {
        int val = infile.readInt();
        if (val == 1)
            connected = true;
        else if (val == -1)
            connected = false;
        else
            connected = NTriBool::Unknown;
    } else if (propType == PROPID_REALBOUNDARY)
        realBoundary = infile.readBool();
    else if (propType == PROPID_COMPACT)
        compact = infile.readBool();
    else if (propType == PROPID_CANCRUSH)
        canCrush = infile.readBool();
    else if (propType == PROPID_SURFACENAME)
        name = infile.readString();
}

NNormalSurface::NNormalSurface(NTriangulation* triang,
        NNormalSurfaceVector* newVector) :
        vector(newVector),
        triangulation(triang) {
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
    // There are no octagonal types at all.
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

bool NNormalSurfaceVector::isSplitting(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    NLargeInteger tot;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (getTriangleCoord(tet, type, triang) != 0)
                return false;
        tot = 0L;
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

NLargeInteger NNormalSurfaceVector::isCentral(NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    NLargeInteger tot, tetTot;
    for (tet = 0; tet < nTets; tet++) {
        tetTot = 0L;
        for (type = 0; type < 4; type++)
            tetTot += getTriangleCoord(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += getQuadCoord(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += getOctCoord(tet, type, triang);
        if (tetTot > 1)
            return NLargeInteger::zero;
        tot += tetTot;
    }
    return tot;
}

bool NNormalSurface::locallyCompatible(const NNormalSurface& other) const {
    unsigned long nTets = triangulation->getNumberOfTetrahedra();

    int type;
    int found;
    for (unsigned long tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (getQuadCoord(tet, type) > 0 ||
                    other.getQuadCoord(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (getOctCoord(tet, type) > 0 ||
                    other.getOctCoord(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

void NNormalSurface::calculateEulerCharacteristic() const {
    unsigned long index, tot;
    int type;
    NLargeInteger ans = NLargeInteger::zero;

    // Add vertices.
    tot = triangulation->getNumberOfEdges();
    for (index = 0; index < tot; index++)
        ans += getEdgeWeight(index);

    // Subtract edges.
    tot = triangulation->getNumberOfFaces();
    for (index = 0; index < tot; index++)
        for (type = 0; type < 3; type++)
            ans -= getFaceArcs(index, type);

    // Add faces.
    tot = triangulation->getNumberOfTetrahedra();
    for (index = 0; index < tot; index++) {
        for (type=0; type<4; type++)
            ans += getTriangleCoord(index, type);
        for (type=0; type<3; type++)
            ans += getQuadCoord(index, type);
        for (type=0; type<3; type++)
            ans += getOctCoord(index, type);
    }

    // Done!
    eulerChar = ans;
}

void NNormalSurface::calculateRealBoundary() const {
    if (triangulation->isClosed()) {
        realBoundary = false;
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
                    return;
                }
            }
            for (type=0; type<3; type++) {
                if (getOctCoord(index, type) > 0) {
                    realBoundary = true;
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
                            return;
                        }
                    }
                }
        }
    }
    realBoundary = false;
}

void NNormalSurface::writeXMLData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the opening tag including vector length.
    unsigned vecLen = vector->size();
    out << "  <surface len=\"" << vecLen << "\" name=\""
        << xmlEncodeSpecialChars(name) << "\">";

    // Write all non-zero entries.
    NLargeInteger entry;
    for (unsigned i = 0; i < vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0)
            out << ' ' << i << ' ' << entry;
    }

    // Write properties.
    if (eulerChar.known())
        out << "\n\t" << xmlValueTag("euler", eulerChar.value());
    if (orientable.known())
        out << "\n\t" << xmlValueTag("orbl", orientable.value());
    if (twoSided.known())
        out << "\n\t" << xmlValueTag("twosided", twoSided.value());
    if (connected.known())
        out << "\n\t" << xmlValueTag("connected", connected.value());
    if (realBoundary.known())
        out << "\n\t" << xmlValueTag("realbdry", realBoundary.value());
    if (compact.known())
        out << "\n\t" << xmlValueTag("compact", compact.value());
    if (canCrush.known())
        out << "\n\t" << xmlValueTag("cancrush", canCrush.value());

    // Write the closing tag.
    out << " </surface>\n";
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

    bookmark = out.writePropertyHeader(PROPID_SURFACENAME);
    out.writeString(name);
    out.writePropertyFooter(bookmark);

    if (eulerChar.known()) {
        bookmark = out.writePropertyHeader(PROPID_EULERCHARACTERISTIC);
        out.writeLarge(eulerChar.value());
        out.writePropertyFooter(bookmark);
    }
    if (orientable.known()) {
        bookmark = out.writePropertyHeader(PROPID_ORIENTABILITY);
        NTriBool val = orientable.value();
        if (val.isTrue())
            out.writeInt(1);
        else if (val.isFalse())
            out.writeInt(-1);
        else
            out.writeInt(0);
        out.writePropertyFooter(bookmark);
    }
    if (twoSided.known()) {
        bookmark = out.writePropertyHeader(PROPID_TWOSIDEDNESS);
        NTriBool val = twoSided.value();
        if (val.isTrue())
            out.writeInt(1);
        else if (val.isFalse())
            out.writeInt(-1);
        else
            out.writeInt(0);
        out.writePropertyFooter(bookmark);
    }
    if (connected.known()) {
        bookmark = out.writePropertyHeader(PROPID_CONNECTEDNESS);
        NTriBool val = connected.value();
        if (val.isTrue())
            out.writeInt(1);
        else if (val.isFalse())
            out.writeInt(-1);
        else
            out.writeInt(0);
        out.writePropertyFooter(bookmark);
    }
    if (realBoundary.known()) {
        bookmark = out.writePropertyHeader(PROPID_REALBOUNDARY);
        out.writeBool(realBoundary.value());
        out.writePropertyFooter(bookmark);
    }
    if (compact.known()) {
        bookmark = out.writePropertyHeader(PROPID_COMPACT);
        out.writeBool(compact.value());
        out.writePropertyFooter(bookmark);
    }
    if (canCrush.known()) {
        bookmark = out.writePropertyHeader(PROPID_CANCRUSH);
        out.writeBool(canCrush.value());
        out.writePropertyFooter(bookmark);
    }

    out.writeAllPropertiesFooter();
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
    in.readProperties(ans);

    return ans;
}

// Tidy up.
#undef REGISTER_FLAVOUR
#undef __FLAVOUR_REGISTRY_BODY

} // namespace regina

