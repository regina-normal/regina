
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <vector>
#include "surfaces/nxmlsurfacereader.h"
#include "surfaces/flavourregistry.h"
#include "triangulation/ntriangulation.h"
#include "utilities/stringutils.h"

#define __FLAVOUR_REGISTRY_BODY

namespace regina {

void NXMLNormalSurfaceReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        NXMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen))
        vecLen = -1;
    name = props.lookup("name");
}

#define REGISTER_FLAVOUR(id_name, class, n, a, t) \
    if (flavour == NNormalSurfaceList::id_name) \
        vec = new class(vecLen); \
    else

void NXMLNormalSurfaceReader::initialChars(const std::string& chars) {
    if (vecLen < 0 || tri == 0)
        return;

    std::vector<std::string> tokens;
    if (basicTokenise(back_inserter(tokens), chars) % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    // Bring in cases from the flavour registry.
    NNormalSurfaceVector* vec;
    #include "surfaces/flavourregistry.h"
        return; // Final else statement.

    long pos;
    NLargeInteger value;
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

    surface = new NNormalSurface(tri, vec);
    if (! name.empty())
        surface->setName(name);
}

NXMLElementReader* NXMLNormalSurfaceReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! surface)
        return new NXMLElementReader();

    if (subTagName == "euler") {
        if (valueOf(props.lookup("value"), surface->eulerChar))
            surface->calculatedEulerChar = true;
    } else if (subTagName == "orbl") {
        if (valueOf(props.lookup("value"), surface->orientable))
            surface->calculatedOrientable = true;
    } else if (subTagName == "twosided") {
        if (valueOf(props.lookup("value"), surface->twoSided))
            surface->calculatedTwoSided = true;
    } else if (subTagName == "connected") {
        if (valueOf(props.lookup("value"), surface->connected))
            surface->calculatedConnected = true;
    } else if (subTagName == "realbdry") {
        if (valueOf(props.lookup("value"), surface->realBoundary))
            surface->calculatedRealBoundary = true;
    } else if (subTagName == "compact") {
        if (valueOf(props.lookup("value"), surface->compact))
            surface->calculatedCompact = true;
    } else if (subTagName == "cancrush") {
        if (valueOf(props.lookup("value"), surface->canCrush))
            surface->calculatedCanCrush = true;
    }
    return new NXMLElementReader();
}

NXMLElementReader* NXMLNormalSurfaceListReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list) {
        // The surface list has already been created.
        if (subTagName == "surface")
            return new NXMLNormalSurfaceReader(tri, list->flavour);
    } else {
        // The surface list has not yet been created.
        if (subTagName == "params") {
            long flavour;
            bool embedded;
            if (valueOf(props.lookup("flavourid"), flavour))
                if (valueOf(props.lookup("embedded"), embedded)) {
                    // Parameters look sane; create the empty list.
                    list = new NNormalSurfaceList();
                    list->flavour = flavour;
                    list->embedded = embedded;
                }
        }
    }
    return new NXMLElementReader();
}

void NXMLNormalSurfaceListReader::endContentSubElement(
        const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (list)
        if (subTagName == "surface")
            if (NNormalSurface* s =
                    ((NXMLNormalSurfaceReader*)subReader)->getSurface())
                list->surfaces.push_back(s);
}

NXMLPacketReader* NNormalSurfaceList::getXMLReader(NPacket* parent) {
    return new NXMLNormalSurfaceListReader((NTriangulation*)parent);
}

} // namespace regina

