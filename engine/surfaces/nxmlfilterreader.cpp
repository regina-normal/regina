
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "surfaces/nxmlfilterreader.h"
#include "surfaces/filterregistry.h"
#include "utilities/stringutils.h"

namespace regina {

namespace {
    struct XMLReaderFunction : public Returns<NXMLElementReader*> {
        template <typename Filter>
        inline NXMLElementReader* operator() (NPacket* parent) {
            return Filter::Class::xmlFilterReader(parent);
        }
    };
}

NXMLElementReader* NXMLFilterPacketReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! filter_)
        if (subTagName == "filter") {
            int type;
            if (valueOf(props.lookup("typeid"), type)) {
                NXMLElementReader* ans = forFilter(
                    static_cast<SurfaceFilterType>(type),
                    XMLReaderFunction(), 0, parent_);
                if (ans)
                    return ans;
                else
                    return new NXMLFilterReader();
            }
        }
    return new NXMLElementReader();
}

void NXMLFilterPacketReader::endContentSubElement(
        const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (! filter_)
        if (subTagName == "filter")
            filter_ = dynamic_cast<NXMLFilterReader*>(subReader)->filter();
}

NXMLPacketReader* NSurfaceFilter::xmlReader(NPacket* parent,
        NXMLTreeResolver& resolver) {
    return new NXMLFilterPacketReader(parent, resolver);
}

} // namespace regina

