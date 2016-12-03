
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <vector>
#include "angle/xmlanglestructreader.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

namespace regina {

void XMLAngleStructureReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        XMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen))
        vecLen = -1;
}

void XMLAngleStructureReader::initialChars(const std::string& chars) {
    if (vecLen < 0 || tri == 0)
        return;

    std::vector<std::string> tokens;
    if (basicTokenise(back_inserter(tokens), chars) % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    AngleStructureVector* vec = new AngleStructureVector(vecLen);

    long pos;
    LargeInteger value;
    for (unsigned long i = 0; i < tokens.size(); i += 2) {
        if (valueOf(tokens[i], pos))
            if (valueOf(tokens[i + 1], value))
                if (pos >= 0 && pos < vecLen) {
                    // All looks valid.
                    vec->setElement(pos, value);
                    continue;
                }

        // Found something invalid.
        delete vec;
        return;
    }

    angles = new AngleStructure(tri, vec);
}

XMLElementReader* XMLAngleStructureReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! angles)
        return new XMLElementReader();

    /** Flags in data files are deprecated as of Regina 4.93.
    if (subTagName == "flags") {
        if (! valueOf(props.lookup("value"), angles->flags))
            angles->flags = 0;
    }
    */
    return new XMLElementReader();
}

XMLElementReader* XMLAngleStructuresReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    bool b;
    if (subTagName == "angleparams") {
        if (valueOf(props.lookup("tautonly"), b))
            list->tautOnly_ = b;
    } else if (subTagName == "struct") {
        return new XMLAngleStructureReader(tri);
    } else if (subTagName == "spanstrict") {
        if (valueOf(props.lookup("value"), b))
            list->doesSpanStrict = b;
    } else if (subTagName == "spantaut") {
        if (valueOf(props.lookup("value"), b))
            list->doesSpanTaut = b;
    } else if (subTagName == "allowstrict") {
        if (valueOf(props.lookup("value"), b))
            list->doesSpanStrict = b;
    } else if (subTagName == "allowtaut") {
        if (valueOf(props.lookup("value"), b))
            list->doesSpanTaut = b;
    }
    return new XMLElementReader();
}

void XMLAngleStructuresReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "struct")
        if (AngleStructure* s =
                dynamic_cast<XMLAngleStructureReader*>(subReader)->structure())
            list->structures.push_back(s);
}

XMLPacketReader* AngleStructures::xmlReader(Packet* parent,
        XMLTreeResolver& resolver) {
    return new XMLAngleStructuresReader(
        dynamic_cast<Triangulation<3>*>(parent), resolver);
}

} // namespace regina

