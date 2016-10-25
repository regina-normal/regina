
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
#include "hypersurface/xmlhypersurfacereader.h"
#include "hypersurface/hscoordregistry.h"
#include "triangulation/dim4.h"
#include "utilities/stringutils.h"

namespace regina {

void XMLNormalHypersurfaceReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props,
        XMLElementReader*) {
    if (! valueOf(props.lookup("len"), vecLen_))
        vecLen_ = -1;
    name_ = props.lookup("name");
}

void XMLNormalHypersurfaceReader::initialChars(const std::string& chars) {
    if (vecLen_ < 0 || tri_ == 0)
        return;

    std::vector<std::string> tokens;
    if (basicTokenise(back_inserter(tokens), chars) % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    // Bring in cases from the coordinate system registry...
    NormalHypersurfaceVector* vec = forCoords(coords_,
        NewFunction<NormalHypersurfaceVector>(), 0, vecLen_);
    if (! vec)
        return;

    long pos;
    LargeInteger value;
    for (size_t i = 0; i < tokens.size(); i += 2) {
        if (valueOf(tokens[i], pos))
            if (valueOf(tokens[i + 1], value))
                if (pos >= 0 && pos < vecLen_) {
                    // All looks valid.
                    vec->setElement(pos, value);
                    continue;
                }

        // Found something invalid.
        delete vec;
        return;
    }

    surface_ = new NormalHypersurface(tri_, vec);
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

XMLElementReader* XMLNormalHypersurfacesReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list_) {
        // The hypersurface list has already been created.
        if (subTagName == "hypersurface")
            return new XMLNormalHypersurfaceReader(tri_, list_->coords_);
    } else {
        // The hypersurface list has not yet been created.
        if (subTagName == "params") {
            long coords;
            int listType, algorithm;
            bool embedded;
            if (valueOf(props.lookup("flavourid"), coords)) {
                if (valueOf(props.lookup("type"), listType) &&
                        valueOf(props.lookup("algorithm"), algorithm)) {
                    // Parameters look sane; create the empty list.
                    list_ = new NormalHypersurfaces(
                        static_cast<HyperCoords>(coords),
                        HyperList::fromInt(listType),
                        HyperAlg::fromInt(algorithm));
                } else if (valueOf(props.lookup("embedded"), embedded)) {
                    // Parameters look sane but use the old prerelease format.
                    list_ = new NormalHypersurfaces(
                        static_cast<HyperCoords>(coords),
                        HS_LEGACY | (embedded ?
                            HS_EMBEDDED_ONLY : HS_IMMERSED_SINGULAR),
                        HS_ALG_LEGACY);
                }
            }
        }
    }
    return new XMLElementReader();
}

void XMLNormalHypersurfacesReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_)
        if (subTagName == "hypersurface")
            if (NormalHypersurface* s =
                    dynamic_cast<XMLNormalHypersurfaceReader*>(subReader)->
                    hypersurface())
                list_->surfaces_.push_back(s);
}

XMLPacketReader* NormalHypersurfaces::xmlReader(Packet* parent,
        XMLTreeResolver& resolver) {
    return new XMLNormalHypersurfacesReader(
        dynamic_cast<Triangulation<4>*>(parent), resolver);
}

} // namespace regina

