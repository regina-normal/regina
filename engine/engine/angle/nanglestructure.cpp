
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

#include "angle/nanglestructure.h"
#include "triangulation/ntriangulation.h"
#include "file/nfile.h"

NAngleStructure* NAngleStructure::clone() const {
    return new NAngleStructure(triangulation,
        (NAngleStructureVector*)vector->clone());
}

NRational NAngleStructure::getAngle(unsigned long tetIndex, int edgePair)
        const {
    const NLargeInteger& num = (*vector)[3 * tetIndex + edgePair];
    const NLargeInteger& den =
        (*vector)[3 * triangulation->getNumberOfTetrahedra()];

    NLargeInteger gcd = den.gcd(num);
    if (gcd < 0)
        gcd.negate();
    return NRational(num.divExact(gcd), den.divExact(gcd));
}

void NAngleStructure::writeTextShort(ostream& out) const {
    unsigned long nTets = triangulation->getNumberOfTetrahedra();
    unsigned j;
    for (unsigned long tet = 0; tet < nTets; tet++) {
        if (tet > 0)
            out << " ; ";
        for (j=0; j<3; j++) {
            if (j > 0)
                out << ' ';
            out << getAngle(tet, j);
        }
    }
}

void NAngleStructure::writeToFile(NFile& out) const {
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
    
    // Write properties (of which there are currently none).
    writeAllPropertiesFooter(out);
}

NAngleStructure* NAngleStructure::readFromFile(NFile& in,
        NTriangulation* triangulation) {
    // Read the vector length and make a new vector.
    unsigned vecLen = in.readUInt();
    NAngleStructureVector* vector = new NAngleStructureVector(vecLen);

    // Read all non-zero vector entries.
    int vecPos = in.readInt();
    while (vecPos != -1) {
        vector->setElement(vecPos, in.readLarge());
        vecPos = in.readInt();
    }

    NAngleStructure* ans = new NAngleStructure(triangulation, vector);

    // Read in properties.
    ans->readProperties(in);

    return ans;
}

void NAngleStructure::readIndividualProperty(NFile& infile, unsigned propType) {
}

void NAngleStructure::initialiseAllProperties() {
}

