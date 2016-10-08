
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
#include "surfaces/xmlfilterreader.h"
#include "surfaces/filterregistry.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing XML readers for specific filter types.
 */
namespace {
    /**
     * Reads a plain (non-subclassed) SurfaceFilter.
     */
    class PlainFilterReader : public XMLFilterReader {
        private:
            SurfaceFilter* filter_;

        public:
            PlainFilterReader() : filter_(new SurfaceFilter()) {
            }

            virtual SurfaceFilter* filter() {
                return filter_;
            }
    };

    /**
     * Reads a SurfaceFilterCombination filter.
     */
    class CombinationReader : public XMLFilterReader {
        private:
            SurfaceFilterCombination* filter_;

        public:
            CombinationReader() : filter_(0) {
            }

            virtual SurfaceFilter* filter() {
                return filter_;
            }

            XMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (! filter_)
                    if (subTagName == "op") {
                        std::string type = props.lookup("type");
                        if (type == "and") {
                            filter_ = new SurfaceFilterCombination();
                            filter_->setUsesAnd(true);
                        } else if (type == "or") {
                            filter_ = new SurfaceFilterCombination();
                            filter_->setUsesAnd(false);
                        }
                    }
                return new XMLElementReader();
            }
    };

    /**
     * Reads a SurfaceFilterProperties filter.
     */
    class PropertiesReader : public XMLFilterReader {
        private:
            SurfaceFilterProperties* filter_;

        public:
            PropertiesReader() : filter_(new SurfaceFilterProperties()) {
            }

            virtual SurfaceFilter* filter() {
                return filter_;
            }

            XMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (subTagName == "euler") {
                    return new XMLCharsReader();
                } else if (subTagName == "orbl") {
                    BoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setOrientability(b);
                } else if (subTagName == "compact") {
                    BoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setCompactness(b);
                } else if (subTagName == "realbdry") {
                    BoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter_->setRealBoundary(b);
                }
                return new XMLElementReader();
            }

            void endSubElement(
                    const std::string& subTagName,
                    XMLElementReader* subReader) {
                if (subTagName == "euler") {
                    std::list<std::string> tokens;
                    basicTokenise(back_inserter(tokens),
                        dynamic_cast<XMLCharsReader*>(subReader)->chars());

                    LargeInteger val;
                    for (std::list<std::string>::const_iterator it =
                            tokens.begin(); it != tokens.end(); it++)
                        if (valueOf(*it, val))
                            filter_->addEulerChar(val);
                }
            }
    };

    /**
     * Helper class for using forFilter().
     */
    struct XMLReaderFunction : public Returns<XMLElementReader*> {
        template <typename Filter>
        inline XMLElementReader* operator() (Packet* parent) {
            return Filter::Class::xmlFilterReader(parent);
        }
    };
}

XMLElementReader* XMLFilterPacketReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! filter_)
        if (subTagName == "filter") {
            int type;
            if (valueOf(props.lookup("typeid"), type)) {
                XMLElementReader* ans = forFilter(
                    static_cast<SurfaceFilterType>(type),
                    XMLReaderFunction(), 0, parent_);
                if (ans)
                    return ans;
                else
                    return new XMLFilterReader();
            }
        }
    return new XMLElementReader();
}

void XMLFilterPacketReader::endContentSubElement(
        const std::string& subTagName,
        XMLElementReader* subReader) {
    if (! filter_)
        if (subTagName == "filter")
            filter_ = dynamic_cast<XMLFilterReader*>(subReader)->filter();
}

XMLPacketReader* SurfaceFilter::xmlReader(Packet* parent,
        XMLTreeResolver& resolver) {
    return new XMLFilterPacketReader(parent, resolver);
}

XMLFilterReader* SurfaceFilter::xmlFilterReader(Packet*) {
    return new PlainFilterReader();
}

XMLFilterReader* SurfaceFilterCombination::xmlFilterReader(Packet*) {
    return new CombinationReader();
}

XMLFilterReader* SurfaceFilterProperties::xmlFilterReader(Packet*) {
    return new PropertiesReader();
}

} // namespace regina

