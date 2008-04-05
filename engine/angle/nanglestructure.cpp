
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

#include "angle/nanglestructure.h"
#include "triangulation/ntriangulation.h"
#include "file/nfile.h"
#include "utilities/xmlutils.h"

// Property IDs:
#define PROPID_FLAGS 1

namespace regina {

const unsigned long NAngleStructure::flagStrict = 1;
const unsigned long NAngleStructure::flagTaut = 2;
const unsigned long NAngleStructure::flagCalculatedType = 4;

NAngleStructure* NAngleStructure::clone() const {
    NAngleStructure* ans = new NAngleStructure(triangulation,
        dynamic_cast<NAngleStructureVector*>(vector->clone()));
    ans->flags = flags;
    return ans;
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

void NAngleStructure::writeTextShort(std::ostream& out) const {
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

    // Write properties.
    std::streampos bookmark(0);

    bookmark = out.writePropertyHeader(PROPID_FLAGS);
    out.writeULong(flags);
    out.writePropertyFooter(bookmark);

    out.writeAllPropertiesFooter();
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
    in.readProperties(ans);

    return ans;
}

void NAngleStructure::writeXMLData(std::ostream& out) const {
    // Write the vector length.
    unsigned vecLen = vector->size();
    out << "  <struct len=\"" << vecLen << "\"> ";

    // Write the non-zero elements.
    NLargeInteger entry;
    for (unsigned i = 0; i < vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0)
            out << i << ' ' << entry << ' ';
    }

    // Write properties.
    out << regina::xml::xmlValueTag("flags", flags);

    // Write the closing tag.
    out << "</struct>\n";
}

void NAngleStructure::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROPID_FLAGS) {
        flags = infile.readULong();
    }
}

void NAngleStructure::calculateType() const {
    unsigned long size = vector->size();
    if (size == 1) {
        // We have no tetrahedra at all; this angle structure is both
        // strict and taut.
        flags |= flagStrict;
        flags |= flagTaut;
        flags |= flagCalculatedType;
        return;
    }

    bool taut = true;
    bool strict = true;

    // Run through the tetrahedra one by one.
    const NLargeInteger& scale = (*vector)[size - 1];
    unsigned long pair;
    for (unsigned long base = 0; base < size - 1; base += 3) {
        for (pair = 0; pair < 3; pair++) {
            if ((*vector)[base + pair] == scale) {
                // We have a pi; thus all three angles in this
                // tetrahedron are pi or zero.
                strict = false;
                break;
            } else if ((*vector)[base + pair] == NLargeInteger::zero)
                strict = false;
            else
                taut = false;
        }
        if ((! strict) && (! taut))
            break;
    }

    // Update the flags as appropriate.
    if (strict)
        flags |= flagStrict;
    else
        flags &= (~flagStrict);

    if (taut)
        flags |= flagTaut;
    else
        flags &= (~flagTaut);

    flags |= flagCalculatedType;
}

} // namespace regina

