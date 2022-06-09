
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "file/xml/xmlsurfacereader.h"
#include "file/xml/xmltreeresolver.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

namespace regina {

void XMLNormalSurfaceReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        XMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen_))
        vecLen_ = -1;
    if (! valueOf(props.lookup("enc"), vecEnc_))
        vecEnc_ = 0;
    name_ = props.lookup("name");
}

void XMLNormalSurfaceReader::initialChars(const std::string& chars) {
    // If this file was created before Regina 7.0, then it will not include
    // a vector encoding.  For those earlier versions of Regina, the encoding
    // is meant to be deduced from the enclosing list's coordinate system.
    if (vecLen_ < 0)
        return;

    std::vector<std::string> tokens = basicTokenise(chars);
    if (tokens.size() % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    Vector<LargeInteger> vec(vecLen_);

    long pos;
    LargeInteger value;
    for (size_t i = 0; i < tokens.size(); i += 2) {
        if (! valueOf(tokens[i], pos))
            return;
        if (pos < 0 || pos >= vecLen_)
            return;
        try {
            vec[pos] = tokens[i + 1];
        } catch (const regina::InvalidArgument&) {
            return;
        }
    }

    if (vecEnc_ != 0)
        surface_ = NormalSurface(tri_, NormalEncoding::fromIntValue(vecEnc_),
            std::move(vec));
    else
        surface_ = NormalSurface(tri_, coords_, std::move(vec));

    if (! name_.empty())
        surface_->setName(name_);
}

XMLElementReader* XMLNormalSurfaceReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! surface_)
        return new XMLElementReader();

    if (subTagName == "euler") {
        try {
            surface_->eulerChar_ = props.lookup("value");
        } catch (const regina::InvalidArgument&) {
        }
    } else if (subTagName == "orbl") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->orientable_ = val;
    } else if (subTagName == "twosided") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->twoSided_ = val;
    } else if (subTagName == "connected") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->connected_ = val;
    } else if (subTagName == "realbdry") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->realBoundary_ = val;
    } else if (subTagName == "compact") {
        bool val;
        if (valueOf(props.lookup("value"), val))
            surface_->compact_ = val;
    }
    return new XMLElementReader();
}

XMLNormalSurfacesReader::XMLNormalSurfacesReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id,
        const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        list_(nullptr), tri_(resolver_.resolveTri3(props.lookup("tri"))) {
    if (! tri_)
        return;

    // Extract the list parameters from the attributes.
    int coords, listType, algorithm;
    if (valueOf(props.lookup("coords"), coords) &&
            valueOf(props.lookup("type"), listType) &&
            valueOf(props.lookup("algorithm"), algorithm)) {
        // Parameters look sane; create the empty list.
        list_ = make_packet<NormalSurfaces>(std::in_place,
            static_cast<NormalCoords>(coords),
            NormalList::fromInt(listType),
            NormalAlg::fromInt(algorithm),
            *tri_);
    }
}

XMLElementReader* XMLNormalSurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (list_ && subTagName == "surface")
        return new XMLNormalSurfaceReader(
            list_->triangulation_, list_->coords_);
    else
        return new XMLElementReader();
}

void XMLNormalSurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "surface")
        if (auto& s = static_cast<XMLNormalSurfaceReader*>(subReader)->
                surface())
            list_->surfaces_.push_back(std::move(*s));
}

XMLElementReader* XMLLegacyNormalSurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list_) {
        // The surface list has already been created.
        if (subTagName == "surface")
            return new XMLNormalSurfaceReader(
                list_->triangulation_, list_->coords_);
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
                    list_ = make_packet<NormalSurfaces>(std::in_place,
                        static_cast<NormalCoords>(coords),
                        NormalList::fromInt(listType),
                        NormalAlg::fromInt(algorithm),
                        tri_);
                } else if (valueOf(props.lookup("embedded"), embedded)) {
                    // Parameters look sane but use the old format.
                    list_ = make_packet<NormalSurfaces>(std::in_place,
                        static_cast<NormalCoords>(coords),
                        NS_LEGACY | (embedded ?
                            NS_EMBEDDED_ONLY : NS_IMMERSED_SINGULAR),
                        NS_ALG_LEGACY,
                        tri_);
                }
            }
        }
    }
    return new XMLElementReader();
}

void XMLLegacyNormalSurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "surface")
        if (auto& s = static_cast<XMLNormalSurfaceReader*>(subReader)->
                surface())
            list_->surfaces_.push_back(std::move(*s));
}

} // namespace regina

