
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

#include <list>
#include "surfaces/nxmlfilterreader.h"
#include "surfaces/sfcombination.h"
#include "surfaces/sfproperties.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing XML readers for specific filter types.
 */
namespace {
    /**
     * Reads a plain (non-subclassed) NSurfaceFilter.
     */
    class NPlainFilterReader : public NXMLFilterReader {
        private:
            NSurfaceFilter* filter_;

        public:
            NPlainFilterReader() : filter_(new NSurfaceFilter()) {
            }

            virtual NSurfaceFilter* filter() {
                return filter_;
            }
    };

    /**
     * Reads an NSurfaceFilterCombination filter.
     */
    class NCombinationReader : public NXMLFilterReader {
        private:
            NSurfaceFilterCombination* filter_;

        public:
            NCombinationReader() : filter_(0) {
            }

            virtual NSurfaceFilter* filter() {
                return filter_;
            }

            NXMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (! filter_)
                    if (subTagName == "op") {
                        std::string type = props.lookup("type");
                        if (type == "and") {
                            filter_ = new NSurfaceFilterCombination();
                            filter_->setUsesAnd(true);
                        } else if (type == "or") {
                            filter_ = new NSurfaceFilterCombination();
                            filter_->setUsesAnd(false);
                        }
                    }
                return new NXMLElementReader();
            }
    };

    /**
     * Reads an NSurfaceFilterProperties filter.
     */
    class NPropertiesReader : public NXMLFilterReader {
        private:
            NSurfaceFilterProperties* filter_;

        public:
            NPropertiesReader() : filter_(new NSurfaceFilterProperties()) {
            }

            virtual NSurfaceFilter* filter() {
                return filter_;
            }

            NXMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (subTagName == "euler") {
                    return new NXMLCharsReader();
                } else if (subTagName == "orbl") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setOrientability(b);
                } else if (subTagName == "compact") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setCompactness(b);
                } else if (subTagName == "realbdry") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setRealBoundary(b);
                }
                return new NXMLElementReader();
            }

            void endSubElement(
                    const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "euler") {
                    std::list<std::string> tokens;
                    basicTokenise(back_inserter(tokens),
                        dynamic_cast<NXMLCharsReader*>(subReader)->chars());

                    NLargeInteger val;
                    for (std::list<std::string>::const_iterator it =
                            tokens.begin(); it != tokens.end(); it++)
                        if (valueOf(*it, val))
                            filter_->addEulerChar(val);
                }
            }
    };
}

NXMLFilterReader* NSurfaceFilter::xmlFilterReader(Packet*) {
    return new NPlainFilterReader();
}

NXMLFilterReader* NSurfaceFilterCombination::xmlFilterReader(Packet*) {
    return new NCombinationReader();
}

NXMLFilterReader* NSurfaceFilterProperties::xmlFilterReader(Packet*) {
    return new NPropertiesReader();
}

} // namespace regina

