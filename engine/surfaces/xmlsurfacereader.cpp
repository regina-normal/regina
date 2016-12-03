
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
#include "surfaces/coordregistry.h"
#include "surfaces/xmlsurfacereader.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

namespace regina {

void XMLNormalSurfaceReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        XMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen))
        vecLen = -1;
    name = props.lookup("name");
}

void XMLNormalSurfaceReader::initialChars(const std::string& chars) {
    if (vecLen < 0 || tri == 0)
        return;

    std::vector<std::string> tokens;
    if (basicTokenise(back_inserter(tokens), chars) % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    // Bring in cases from the coordinate system registry...
    NormalSurfaceVector* vec;
    if (coords == NS_AN_LEGACY)
        vec = new NSVectorANStandard(vecLen);
    else
        vec = forCoords(coords, NewFunction<NormalSurfaceVector>(), 0, vecLen);
    if (! vec)
        return;

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

    surface_ = new NormalSurface(tri, vec);
    if (! name.empty())
        surface_->setName(name);
}

XMLElementReader* XMLNormalSurfaceReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! surface_)
        return new XMLElementReader();

    if (subTagName == "euler") {
        LargeInteger val;
        if (valueOf(props.lookup("value"), val))
            surface_->eulerChar_ = val;
    } else if (subTagName == "orbl") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->orientable = val;
    } else if (subTagName == "twosided") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->twoSided = val;
    } else if (subTagName == "connected") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->connected = val;
    } else if (subTagName == "realbdry") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->realBoundary = val;
    } else if (subTagName == "compact") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->compact = val;
    }
    return new XMLElementReader();
}

XMLElementReader* XMLNormalSurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list) {
        // The surface list has already been created.
        if (subTagName == "surface")
            return new XMLNormalSurfaceReader(tri, list->coords_);
    } else {
        // The surface list has not yet been created.
        if (subTagName == "params") {
            long coords;
            int listType, algorithm;
            bool embedded;
            if (valueOf(props.lookup("flavourid"), coords)) {
                if (valueOf(props.lookup("type"), listType) &&
                        valueOf(props.lookup("algorithm"), algorithm)) {
                    // Parameters look sane; create the empty list.
                    list = new NormalSurfaces(
                        static_cast<NormalCoords>(coords),
                        NormalList::fromInt(listType),
                        NormalAlg::fromInt(algorithm));
                } else if (valueOf(props.lookup("embedded"), embedded)) {
                    // Parameters look sane but use the old format.
                    list = new NormalSurfaces(
                        static_cast<NormalCoords>(coords),
                        NS_LEGACY | (embedded ?
                            NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
                        NS_ALG_LEGACY);
                }
            }
        }
    }
    return new XMLElementReader();
}

void XMLNormalSurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list)
        if (subTagName == "surface")
            if (NormalSurface* s =
                    dynamic_cast<XMLNormalSurfaceReader*>(subReader)->
                    surface())
                list->surfaces.push_back(s);
}

XMLPacketReader* NormalSurfaces::xmlReader(Packet* parent,
        XMLTreeResolver& resolver) {
    return new XMLNormalSurfacesReader(
        dynamic_cast<Triangulation<3>*>(parent), resolver);
}

} // namespace regina

