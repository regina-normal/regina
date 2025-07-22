
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

#include "packet/text.h"
#include "utilities/xmlutils.h"

namespace regina {

void Text::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    writeXMLHeader(out, "textdata", format, anon, refs,
        format == FileFormat::XmlGen2);
    if (format == FileFormat::XmlGen2) {
        out << "  <text>" << regina::xml::xmlEncodeSpecialChars(text_)
            << "</text>\n";
    } else {
        out << regina::xml::xmlEncodeSpecialChars(text_);
    }
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "textdata", format);
}

} // namespace regina

