
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
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
    try {
        vecLen_ = parse<ssize_t>(props.lookup("len"));
    } catch (const InvalidArgument&) {
        vecLen_ = -1;
    }
    try {
        vecEnc_ = parse<int>(props.lookup("enc"));
    } catch (const InvalidArgument&) {
        vecEnc_ = 0;
    }
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
    try {
        for (size_t i = 0; i < tokens.size(); i += 2) {
            size_t pos = parse<size_t>(tokens[i]);
            if (pos >= vecLen_)
                return;
            vec[pos] = tokens[i + 1];
        }
    } catch (const InvalidArgument&) {
        return;
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

    try {
        if (subTagName == "euler") {
            surface_->eulerChar_ = props.lookup("value");
        } else if (subTagName == "orbl") {
            surface_->orientable_ = parse<bool>(props.lookup("value"));
        } else if (subTagName == "twosided") {
            surface_->twoSided_ = parse<bool>(props.lookup("value"));
        } else if (subTagName == "connected") {
            surface_->connected_ = parse<bool>(props.lookup("value"));
        } else if (subTagName == "realbdry") {
            surface_->realBoundary_ = parse<bool>(props.lookup("value"));
        } else if (subTagName == "compact") {
            surface_->compact_ = parse<bool>(props.lookup("value"));
        }
    } catch (const InvalidArgument&) {
        // Fall through to the default XMLElementReader below.
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

    // Extract the list parameters from the attributes and create an empty list.
    try {
        list_ = make_packet<NormalSurfaces>(std::in_place,
            static_cast<NormalCoords>(parse<int>(props.lookup("coords"))),
            Flags<NormalList>::fromBase(
                parse<Flags<NormalList>::BaseInt>(props.lookup("type"))),
            Flags<NormalAlg>::fromBase(
                parse<Flags<NormalAlg>::BaseInt>(props.lookup("algorithm"))),
            *tri_);
    } catch (const InvalidArgument&) {
        // Do not create the surface list.
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
            NormalCoords coords;
            try {
                coords = static_cast<NormalCoords>(
                    parse<long>(props.lookup("flavourid")));
            } catch (const InvalidArgument&) {
                // A coordinate system is mandatory.  Abort.
                return new XMLElementReader();
            }

            // First try for the new format.
            try {
                list_ = make_packet<NormalSurfaces>(std::in_place, coords,
                    Flags<NormalList>::fromBase(
                        parse<Flags<NormalList>::BaseInt>(
                        props.lookup("type"))),
                    Flags<NormalAlg>::fromBase(
                        parse<Flags<NormalAlg>::BaseInt>(
                        props.lookup("algorithm"))),
                    tri_);
            } catch (const InvalidArgument&) {
                // No go.  See if we have the old format instead.
                try {
                    bool embedded = parse<bool>(props.lookup("embedded"));
                    list_ = make_packet<NormalSurfaces>(std::in_place, coords,
                        NormalList::Legacy | (embedded ?
                            NormalList::EmbeddedOnly :
                            NormalList::ImmersedSingular),
                        NormalAlg::Legacy,
                        tri_);
                } catch (const InvalidArgument&) {
                    // Neither format worked.  Do not create an empty list.
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

