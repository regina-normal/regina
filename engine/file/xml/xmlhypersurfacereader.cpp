
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "file/xml/xmlhypersurfacereader.h"
#include "file/xml/xmltreeresolver.h"
#include "triangulation/dim4.h"
#include "utilities/stringutils.h"

namespace regina {

void XMLNormalHypersurfaceReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        XMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen_))
        vecLen_ = -1;
    if (! valueOf(props.lookup("enc"), vecEnc_))
        vecEnc_ = 0;
    name_ = props.lookup("name");
}

void XMLNormalHypersurfaceReader::initialChars(const std::string& chars) {
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

    size_t pos;
    LargeInteger value;
    for (size_t i = 0; i < tokens.size(); i += 2) {
        if (! valueOf(tokens[i], pos)) // note: this ensures pos >= 0
            return;
        if (pos >= vecLen_)
            return;
        try {
            vec[pos] = tokens[i + 1];
        } catch (const regina::InvalidArgument&) {
            return;
        }
    }

    if (vecEnc_ != 0)
        surface_ = NormalHypersurface(tri_,
            HyperEncoding::fromIntValue(vecEnc_), std::move(vec));
    else
        surface_ = NormalHypersurface(tri_, coords_, std::move(vec));
    if (! name_.empty())
        surface_->setName(name_);
}

XMLElementReader* XMLNormalHypersurfaceReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! surface_)
        return new XMLElementReader();

    if (subTagName == "realbdry") {
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

XMLNormalHypersurfacesReader::XMLNormalHypersurfacesReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id,
        const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label), std::move(id)),
        list_(nullptr),
        tri_(resolver_.resolvePacketData<Triangulation<4>>(
            props.lookup("tri"))) {
    if (! tri_)
        return;

    // Extract the list parameters from the attributes.
    int coords;
    Flags<HyperList>::BaseInt listType;
    Flags<HyperAlg>::BaseInt algorithm;
    if (valueOf(props.lookup("coords"), coords) &&
            valueOf(props.lookup("type"), listType) &&
            valueOf(props.lookup("algorithm"), algorithm)) {
        // Parameters look sane; create the empty list.
        list_ = make_packet<NormalHypersurfaces>(std::in_place,
            static_cast<HyperCoords>(coords),
            Flags<HyperList>::fromBase(listType),
            Flags<HyperAlg>::fromBase(algorithm),
            *tri_);
    }
}

XMLElementReader* XMLNormalHypersurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (list_ && subTagName == "hypersurface")
        return new XMLNormalHypersurfaceReader(
            list_->triangulation_, list_->coords_);
    else
        return new XMLElementReader();
}

void XMLNormalHypersurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "hypersurface")
        if (auto& s = static_cast<XMLNormalHypersurfaceReader*>(
                subReader)->hypersurface())
            list_->surfaces_.push_back(std::move(*s));
}

XMLElementReader* XMLLegacyNormalHypersurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list_) {
        // The hypersurface list has already been created.
        if (subTagName == "hypersurface")
            return new XMLNormalHypersurfaceReader(
                list_->triangulation_, list_->coords_);
    } else {
        // The hypersurface list has not yet been created.
        if (subTagName == "params") {
            long coords;
            Flags<HyperList>::BaseInt listType;
            Flags<HyperAlg>::BaseInt algorithm;
            bool embedded;
            if (valueOf(props.lookup("flavourid"), coords)) {
                if (valueOf(props.lookup("type"), listType) &&
                        valueOf(props.lookup("algorithm"), algorithm)) {
                    // Parameters look sane; create the empty list.
                    list_ = make_packet<NormalHypersurfaces>(std::in_place,
                        static_cast<HyperCoords>(coords),
                        Flags<HyperList>::fromBase(listType),
                        Flags<HyperAlg>::fromBase(algorithm),
                        tri_);
                } else if (valueOf(props.lookup("embedded"), embedded)) {
                    // Parameters look sane but use the old prerelease format.
                    list_ = make_packet<NormalHypersurfaces>(std::in_place,
                        static_cast<HyperCoords>(coords),
                        HyperList::Legacy | (embedded ?
                            HyperList::EmbeddedOnly :
                            HyperList::ImmersedSingular),
                        HyperAlg::Legacy,
                        tri_);
                }
            }
        }
    }
    return new XMLElementReader();
}

void XMLLegacyNormalHypersurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "hypersurface")
        if (auto& s = static_cast<XMLNormalHypersurfaceReader*>(
                subReader)->hypersurface())
            list_->surfaces_.push_back(std::move(*s));
}

} // namespace regina

