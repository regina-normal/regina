
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "file/xml/xmlanglestructreader.h"
#include "file/xml/xmltreeresolver.h"
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
    if (vecLen < 0)
        return;

    std::vector<std::string> tokens = basicTokenise(chars);
    if (tokens.size() % 2 != 0)
        return;

    // Create a new vector and read all non-zero entries.
    VectorInt vec(vecLen);

    long pos;
    Integer value;
    for (size_t i = 0; i < tokens.size(); i += 2) {
        if (! valueOf(tokens[i], pos))
            return;
        if (pos < 0 || pos >= vecLen)
            return;
        try {
            vec[pos] = tokens[i + 1];
        } catch (const regina::InvalidArgument&) {
            return;
        }
    }

    angles_ = AngleStructure(tri_, std::move(vec));
}

XMLAngleStructuresReader::XMLAngleStructuresReader(XMLTreeResolver& res,
        std::shared_ptr<Packet> parent, bool anon, std::string label,
        std::string id, const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        list_(nullptr), tri_(resolver_.resolveTri3(props.lookup("tri"))) {
    if (! tri_)
        return;

    // Extract the list parameters from the attributes.
    // We will (unnecessarily) allow the algorithm to be missing.
    bool tautOnly;
    if (! valueOf(props.lookup("tautonly"), tautOnly))
        return;

    int algorithm;
    if (! valueOf(props.lookup("algorithm"), algorithm))
        algorithm = AngleAlg::Legacy;

    list_ = make_packet<AngleStructures>(std::in_place, tautOnly,
        Flags<AngleAlg>::fromInt(algorithm), *tri_);
}

XMLElementReader* XMLAngleStructuresReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! list_) {
        // We are ignoring this <angles> element because it was missing the
        // required attributes.
        return new XMLElementReader();
    }

    if (subTagName == "struct")
        return new XMLAngleStructureReader(list_->triangulation_);

    bool b;
    if (subTagName == "spanstrict") {
        if (valueOf(props.lookup("value"), b))
            list_->doesSpanStrict_ = b;
    } else if (subTagName == "spantaut") {
        if (valueOf(props.lookup("value"), b))
            list_->doesSpanTaut_ = b;
    }
    return new XMLElementReader();
}

void XMLAngleStructuresReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "struct")
        if (auto& s = static_cast<XMLAngleStructureReader*>(subReader)->
                structure())
            list_->structures_.push_back(std::move(*s));
}

XMLElementReader* XMLLegacyAngleStructuresReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (list_) {
        bool b;
        // The angle structure list has already been created.
        if (subTagName == "struct") {
            return new XMLAngleStructureReader(list_->triangulation_);
        } else if (subTagName == "spanstrict") {
            if (valueOf(props.lookup("value"), b))
                list_->doesSpanStrict_ = b;
        } else if (subTagName == "spantaut") {
            if (valueOf(props.lookup("value"), b))
                list_->doesSpanTaut_ = b;
        } else if (subTagName == "allowstrict") {
            if (valueOf(props.lookup("value"), b))
                list_->doesSpanStrict_ = b;
        } else if (subTagName == "allowtaut") {
            if (valueOf(props.lookup("value"), b))
                list_->doesSpanTaut_ = b;
        }
    } else {
        // The angle structure list has not yet been created.
        if (subTagName == "angleparams") {
            // All of these parameters are optional, to support older
            // file formats.
            bool tautOnly;
            int algorithm;
            if (! valueOf(props.lookup("tautonly"), tautOnly))
                tautOnly = false;
            if (! valueOf(props.lookup("algorithm"), algorithm))
                algorithm = AngleAlg::Legacy;
            list_ = make_packet<AngleStructures>(std::in_place, tautOnly,
                Flags<AngleAlg>::fromInt(algorithm), tri_);
        } else if (subTagName == "struct") {
            // Eep, we are getting angle structures but no parameters were
            // ever specified.  This was how data files looked in
            // Regina 4.6 and earlier, when there were no parameters to select.
            // Set up a new list containing all default values, before
            // reading the first angle structure that we just bumped into.
            list_ = make_packet<AngleStructures>(std::in_place, false,
                AngleAlg::Legacy, tri_);
            return new XMLAngleStructureReader(list_->triangulation_);
        }

        // If the file format is old *and* the list is empty, we could
        // conceivably jump straight to a property (spansstrict, etc.),
        // which means we would see them here, before the list is created.
        // However, we silently ignore such properties in this case, since
        // they are trivial to recreate (given that the list is empty).
    }
    return new XMLElementReader();
}

void XMLLegacyAngleStructuresReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (list_ && subTagName == "struct")
        if (auto& s = static_cast<XMLAngleStructureReader*>(subReader)->
                structure())
            list_->structures_.push_back(std::move(*s));
}

void XMLLegacyAngleStructuresReader::endElement() {
    // If we have an empty angle structure list and the file was saved
    // in an ancient version of Regina, the XML content for the packet
    // could legitimately contain no content at all - technically,
    // everything in this XML element is optional.
    if (! list_)
        list_ = make_packet<AngleStructures>(std::in_place, false,
            AngleAlg::Legacy, tri_);

    XMLPacketReader::endElement();
}

} // namespace regina

