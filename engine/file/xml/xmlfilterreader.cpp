
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

#include <list>
#include "file/xml/xmlfilterreader.h"
#include "utilities/stringutils.h"

namespace regina {

XMLElementReader* XMLLegacyFilterReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! filter_)
        if (subTagName == "filter") {
            int type;
            // Run through all the filter types that the file format supports.
            if (valueOf(props.lookup("typeid"), type)) {
                switch (static_cast<SurfaceFilterType>(type)) {
                    case SurfaceFilterType::LegacyDefault:
                        return dataReader_ =
                            new XMLPlainFilterReader(resolver_,
                                parent_, anon_, label_, id_);
                    case SurfaceFilterType::Properties:
                        return dataReader_ =
                            new XMLLegacyPropertiesFilterReader(resolver_,
                                parent_, anon_, label_, id_);
                    case SurfaceFilterType::Combination:
                        return dataReader_ =
                            new XMLLegacyCombinationFilterReader(resolver_,
                                parent_, anon_, label_, id_);
                    default:
                        return new XMLPacketReader(resolver_,
                            parent_, anon_, label_, id_);
                }
            }
        }
    return new XMLElementReader();
}

void XMLLegacyFilterReader::endContentSubElement(const std::string&, XMLElementReader*) {
    if (dataReader_)
        filter_ = dataReader_->packetToCommit();
}

XMLCombinationFilterReader::XMLCombinationFilterReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id,
        const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label), std::move(id)),
        filter_(nullptr) {
    std::string type = props.lookup("op");
    if (type == "and") {
        filter_ = std::make_shared<SurfaceFilterCombination>();
        filter_->setUsesAnd(true);
    } else if (type == "or") {
        filter_ = std::make_shared<SurfaceFilterCombination>();
        filter_->setUsesAnd(false);
    }
}

XMLElementReader* XMLLegacyCombinationFilterReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! filter_)
        if (subTagName == "op") {
            std::string type = props.lookup("type");
            if (type == "and") {
                filter_ = std::make_shared<SurfaceFilterCombination>();
                filter_->setUsesAnd(true);
            } else if (type == "or") {
                filter_ = std::make_shared<SurfaceFilterCombination>();
                filter_->setUsesAnd(false);
            }
        }
    return new XMLElementReader();
}

XMLPropertiesFilterReader::XMLPropertiesFilterReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id,
        const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label), std::move(id)),
        filter_(nullptr) {
    BoolSet orbl, compact, realbdry;

    try {
        auto prop = props.find("orbl");
        if (prop != props.end())
            orbl = BoolSet::fromStringCode(prop->second);
        else
            orbl.fill();

        prop = props.find("compact");
        if (prop != props.end())
            compact = BoolSet::fromStringCode(prop->second);
        else
            compact.fill();

        prop = props.find("realbdry");
        if (prop != props.end())
            realbdry = BoolSet::fromStringCode(prop->second);
        else
            realbdry.fill();
    } catch (const InvalidArgument&) {
        return;
    }

    // We have successfully parsed the BoolSet attributes.
    filter_ = std::make_shared<SurfaceFilterProperties>();
    filter_->setOrientability(orbl);
    filter_->setCompactness(compact);
    filter_->setRealBoundary(realbdry);

    std::string euler = props.lookup("euler");
    if (! euler.empty()) {
        std::istringstream str(euler);
        std::string val;
        try {
            while (std::getline(str, val, ','))
                filter_->addEulerChar(LargeInteger(val));
        } catch (const regina::InvalidArgument&) {
            // We cannot parse the list of Euler characteristics.  Abort.
            filter_.reset();
        }
    }
}

XMLElementReader* XMLLegacyPropertiesFilterReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (filter_) {
        if (subTagName == "euler") {
            return new XMLCharsReader();
        } else try {
            std::string value = props.lookup("value");
            if (subTagName == "orbl")
                filter_->setOrientability(BoolSet::fromStringCode(value));
            else if (subTagName == "compact")
                filter_->setCompactness(BoolSet::fromStringCode(value));
            else if (subTagName == "realbdry")
                filter_->setRealBoundary(BoolSet::fromStringCode(value));
        } catch (const InvalidArgument&) {
            filter_.reset();
        }
    }
    return new XMLElementReader();
}

void XMLLegacyPropertiesFilterReader::endContentSubElement(
        const std::string& subTagName, XMLElementReader* subReader) {
    if (filter_ && subTagName == "euler") {
        std::vector<std::string> tokens = basicTokenise(
            static_cast<XMLCharsReader*>(subReader)->chars());

        for (const auto& t : tokens) {
            try {
                filter_->addEulerChar(LargeInteger(t));
            } catch (const regina::InvalidArgument&) {
                filter_.reset();
                return;
            }
        }
    }
}

} // namespace regina

