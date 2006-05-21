
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

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
            NSurfaceFilter* filter;

        public:
            NPlainFilterReader() : filter(new NSurfaceFilter()) {
            }

            virtual NSurfaceFilter* getFilter() {
                return filter;
            }
    };

    /**
     * Reads an NSurfaceFilterCombination filter.
     */
    class NCombinationReader : public NXMLFilterReader {
        private:
            NSurfaceFilterCombination* filter;

        public:
            NCombinationReader() : filter(0) {
            }

            virtual NSurfaceFilter* getFilter() {
                return filter;
            }

            NXMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (! filter)
                    if (subTagName == "op") {
                        std::string type = props.lookup("type");
                        if (type == "and") {
                            filter = new NSurfaceFilterCombination();
                            filter->setUsesAnd(true);
                        } else if (type == "or") {
                            filter = new NSurfaceFilterCombination();
                            filter->setUsesAnd(false);
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
            NSurfaceFilterProperties* filter;

        public:
            NPropertiesReader() : filter(new NSurfaceFilterProperties()) {
            }

            virtual NSurfaceFilter* getFilter() {
                return filter;
            }

            NXMLElementReader* startSubElement(const std::string& subTagName,
                    const regina::xml::XMLPropertyDict& props) {
                if (subTagName == "euler") {
                    return new NXMLCharsReader();
                } else if (subTagName == "orbl") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter->setOrientability(b);
                } else if (subTagName == "compact") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter->setCompactness(b);
                } else if (subTagName == "realbdry") {
                    NBoolSet b;
                    if (valueOf(props.lookup("value"), b))
                        filter->setRealBoundary(b);
                }
                return new NXMLElementReader();
            }

            void endSubElement(
                    const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "euler") {
                    std::list<std::string> tokens;
                    basicTokenise(back_inserter(tokens),
                        dynamic_cast<NXMLCharsReader*>(subReader)->getChars());

                    NLargeInteger val;
                    for (std::list<std::string>::const_iterator it =
                            tokens.begin(); it != tokens.end(); it++)
                        if (valueOf(*it, val))
                            filter->addEC(val);
                }
            }
    };
}

NXMLFilterReader* NSurfaceFilter::getXMLFilterReader(NPacket*) {
    return new NPlainFilterReader();
}

NXMLFilterReader* NSurfaceFilterCombination::getXMLFilterReader(NPacket*) {
    return new NCombinationReader();
}

NXMLFilterReader* NSurfaceFilterProperties::getXMLFilterReader(NPacket*) {
    return new NPropertiesReader();
}

} // namespace regina

