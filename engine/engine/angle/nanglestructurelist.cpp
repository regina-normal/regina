
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

#include "angle/nanglestructurelist.h"
#include "triangulation/ntriangulation.h"
#include "maths/nmatrixint.h"
#include "maths/nvectorunit.h"
#include "surfaces/nnormalsurface.h"
#include "file/nfile.h"

// Property IDs:
#define PROPID_ALLOWSTRICT 1
#define PROPID_ALLOWTAUT 2

NAngleStructureList::NAngleStructureList(NTriangulation* owner) {
    NAngleStructureList::initialiseAllProperties();
    owner->insertChildLast(this);

    // Form the matching equations (one per non-boundary edge plus
    // one per tetrahedron).
    unsigned long nTetrahedra = owner->getNumberOfTetrahedra();
    unsigned long nCoords = 3 * nTetrahedra + 1;

    long nEquations = long(owner->getNumberOfEdges()) +
        long(owner->getNumberOfTetrahedra());
    for (NTriangulation::BoundaryComponentIterator bit(owner->
            getBoundaryComponents()); ! bit.done(); bit++)
        nEquations -= (*bit)->getNumberOfEdges();

    NMatrixInt eqns(nEquations, nCoords);
    unsigned long row = 0;

    NDynamicArrayIterator<NEdgeEmbedding> embit;
    NPerm perm;
    unsigned long index;
    for (NTriangulation::EdgeIterator eit(owner->getEdges()); ! eit.done();
            eit++) {
        if ((*eit)->isBoundary())
            continue;
        for (embit.init((*eit)->getEmbeddings()); ! embit.done(); embit++) {
            index = owner->getTetrahedronIndex((*embit).getTetrahedron());
            perm = (*embit).getVertices();
            eqns.entry(row, 3 * index + vertexSplit[perm[0]][perm[1]]) += 1;
        }
        eqns.entry(row, nCoords - 1) = -2;
        row++;
    }
    for (index = 0; index < nTetrahedra; index++) {
        eqns.entry(row, 3 * index) = 1;
        eqns.entry(row, 3 * index + 1) = 1;
        eqns.entry(row, 3 * index + 2) = 1;
        eqns.entry(row, nCoords - 1) = -1;
        row++;
    }

    // Form the starting cone.
    NDoubleList<NConeRay*> originalCone;
    NAngleStructureVector* vector;
    NLargeInteger startValue(nCoords - 1);
    for (index = 0; index < nCoords - 1; index++) {
        vector = new NAngleStructureVector(nCoords);
        vector->setElement(index, startValue);
        vector->setElement(nCoords - 1, NLargeInteger::one);
        originalCone.addLast(vector);
    }
    vector = new NAngleStructureVector(nCoords);
    vector->setElement(nCoords - 1, NLargeInteger::one);
    originalCone.addLast(vector);

    // Form the face list.
    NDoubleList<NVector<NLargeInteger>*> faces;
    for (index = 0; index < nCoords - 1; index++)
        faces.addLast(new NVectorUnit<NLargeInteger>(nCoords, index));
    NVectorDense<NLargeInteger>* finalFace =
        new NVectorDense<NLargeInteger>(nCoords, NLargeInteger::one);
    finalFace->setElement(nCoords - 1, -startValue);
    faces.addLast(finalFace);

    // Find the angle structures.
    NDoubleList<NConeRay*>* ans = intersectCone(originalCone, faces,
        eqns, false);
    for (NDoubleListIterator<NConeRay*> it(*ans); ! it.done(); it++) {
        vector = (NAngleStructureVector*)(*it);
        structures.addLast(new NAngleStructure(owner, vector));
    }

    // Tidy up.
    originalCone.flushAndDelete();
    faces.flushAndDelete();
    delete ans;
}

void NAngleStructureList::writeTextShort(ostream& o) const {
    o << structures.size() << " vertex angle structure";
    if (structures.size() != 1)
        o << 's';
}

void NAngleStructureList::writeTextLong(ostream& o) const {
    writeTextShort(o);
    o << ":\n";

    NDynamicArrayIterator<NAngleStructure*> it(structures);
    while (! it.done()) {
        (*it)->writeTextShort(o);
        o << '\n';
        it++;
    }
}

void NAngleStructureList::writePacket(NFile& out) const {
    out.writeULong(structures.size());
    NDynamicArrayIterator<NAngleStructure*> it(structures);
    while (! it.done()) {
        (*it)->writeToFile(out);
        it++;
    }

    // Write the properties.
    streampos bookmark(0);

    if (calculatedAllowStrict) {
        bookmark = writePropertyHeader(out, PROPID_ALLOWSTRICT);
        out.writeBool(doesAllowStrict);
        writePropertyFooter(out, bookmark);
    }
    if (calculatedAllowTaut) {
        bookmark = writePropertyHeader(out, PROPID_ALLOWTAUT);
        out.writeBool(doesAllowTaut);
        writePropertyFooter(out, bookmark);
    }

    writeAllPropertiesFooter(out);
}

NAngleStructureList* NAngleStructureList::readPacket(NFile& in,
        NPacket* parent) {
    NAngleStructureList* ans = new NAngleStructureList();

    unsigned long nStructures = in.readULong();
    for (unsigned long i=0; i<nStructures; i++)
        ans->structures.addLast(NAngleStructure::readFromFile(in,
            (NTriangulation*)parent));

    // Read the properties.
    ans->readProperties(in);

    return ans;
}

NPacket* NAngleStructureList::internalClonePacket(NPacket* parent) const {
    NAngleStructureList* ans = new NAngleStructureList();
    NDynamicArrayIterator<NAngleStructure*> it(structures);
    while (! it.done()) {
        ans->structures.addLast((*it)->clone());
        it++;
    }

    if (calculatedAllowStrict) {
        ans->doesAllowStrict = doesAllowStrict;
        ans->calculatedAllowStrict = true;
    }
    if (calculatedAllowTaut) {
        ans->doesAllowTaut = doesAllowTaut;
        ans->calculatedAllowTaut = true;
    }

    return ans;
}

void NAngleStructureList::initialiseAllProperties() {
    calculatedAllowStrict = false;
    calculatedAllowTaut = false;
}

void NAngleStructureList::readIndividualProperty(NFile& infile,
        unsigned propType) {
    if (propType == PROPID_ALLOWSTRICT) {
        doesAllowStrict = infile.readBool();
        calculatedAllowStrict = true;
    } else if (propType == PROPID_ALLOWTAUT) {
        doesAllowTaut = infile.readBool();
        calculatedAllowTaut = true;
    }
}

void NAngleStructureList::calculateAllowStrict() {
    calculatedAllowStrict = true;

    if (structures.size() == 0) {
        doesAllowStrict = false;
        return;
    }

    unsigned long nTets = getTriangulation()->getNumberOfTetrahedra();
    if (nTets == 0) {
        doesAllowStrict = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    NRational* fixedAngles = new NRational[nTets * 3];
    unsigned long nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    StructureIterator it(structures);
    NAngleStructure* s = *it;

    NRational angle;
    unsigned long tet;
    int edges;
    for (tet = 0; tet < nTets; tet++)
        for (edges = 0; edges < 3; edges++) {
            angle = s->getAngle(tet, edges);
            if (angle == NRational::zero || angle == NRational::one) {
                fixedAngles[3 * tet + edges] = angle;
                nFixed++;
            } else
                fixedAngles[3 * tet + edges] = NRational::undefined;
        }

    if (nFixed == 0) {
        doesAllowStrict = true;
        delete[] fixedAngles;
        return;
    }

    // Run through the rest of the structures to see if these bad angles
    // do ever change.
    for (it++; ! it.done(); it++) {
        s = *it;
        for (tet = 0; tet < nTets; tet++)
            for (edges = 0; edges < 3; edges++) {
                if (fixedAngles[3 * tet + edges] == NRational::undefined)
                    continue;
                if (s->getAngle(tet, edges) != fixedAngles[3 * tet + edges]) {
                    // Here's a bad angle that finally changed.
                    fixedAngles[3 * tet + edges] = NRational::undefined;
                    nFixed--;
                    if (nFixed == 0) {
                        doesAllowStrict = true;
                        delete[] fixedAngles;
                        return;
                    }
                }
            }
    }

    // Some of the bad angles never changed.
    doesAllowStrict = false;
    delete[] fixedAngles;
}

void NAngleStructureList::calculateAllowTaut() {
    calculatedAllowTaut = true;

    for (StructureIterator it(structures); ! it.done(); it++)
        if ((*it)->isTaut()) {
            doesAllowTaut = true;
            return;
        }
    doesAllowTaut = false;
}

