
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "surface/surfacefilter.h"
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

#define TYPE_AND 1
#define TYPE_OR 2

#define PROPSF_EULER 1001
#define PROPSF_ORIENT 1002
#define PROPSF_COMPACT 1003
#define PROPSF_REALBDRY 1004

namespace regina {

bool SurfaceFilterCombination::accept(const NormalSurface& surface) const {
    if (usesAnd_) {
        // Combine all child filters using AND.
        for (auto child = firstChild(); child; child = child->nextSibling())
            if (child->type() == PACKET_SURFACEFILTER)
                if (! (std::static_pointer_cast<SurfaceFilter>(child)->accept(
                        surface)))
                    return false;
        return true;
    } else {
        // Combine all child filters using OR.
        for (auto child = firstChild(); child; child = child->nextSibling())
            if (child->type() == PACKET_SURFACEFILTER)
                if (std::static_pointer_cast<SurfaceFilter>(child)->accept(
                        surface))
                    return true;
        return false;
    }
}

void SurfaceFilterCombination::writeXMLPacketData(std::ostream& out,
        FileFormat format, bool anon, PacketRefs& refs) const {
    writeXMLHeader(out, "filtercomb", format, anon, refs,
        true, std::pair("op", (usesAnd_ ? "and" : "or")));
    if (format == REGINA_XML_GEN_2) {
        out << "  <filter type=\""
            << regina::xml::xmlEncodeSpecialChars(filterTypeName())
            << "\" typeid=\"" << filterType() << "\">\n"
               "    <op type=\"" << (usesAnd_ ? "and" : "or") << "\"/>\n"
               "  </filter>\n";
    }
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "filtercomb", format);
}

LargeInteger SurfaceFilterProperties::eulerChar(size_t index) const {
    auto it = eulerChar_.begin();
    advance(it, index);
    return *it;
}

bool SurfaceFilterProperties::accept(const NormalSurface& surface) const {
    if (! realBoundary_.contains(surface.hasRealBoundary()))
        return false;
    if (! compactness_.contains(surface.isCompact()))
        return false;

    // Some properties may only be calculated for compact surfaces.
    if (surface.isCompact()) {
        if (! orientability_.contains(surface.isOrientable()))
            return false;

        if (! eulerChar_.empty())
            if (! eulerChar_.count(surface.eulerChar()))
                return false;
    }

    // All tests passed.
    return true;
}

void SurfaceFilterProperties::writeTextShort(std::ostream& o) const {
    bool first = true;

    if (eulerChar_.size() > 0) {
        first = false;

        o << "Euler in {";
        for (auto it = eulerChar_.rbegin(); it != eulerChar_.rend(); ++it)
            o << ' ' << *it;
        o << " }";
    }
    if (orientability_ != BoolSet(true, true)) {
        if (first)
            first = false;
        else
            o << ", ";

        if (orientability_.hasTrue())
            o << "orientable only";
        else if (orientability_.hasFalse())
            o << "non-orientable only";
        else
            o << "reject all orientabilities";
    }
    if (compactness_ != BoolSet(true, true)) {
        if (first)
            first = false;
        else
            o << ", ";

        if (compactness_.hasTrue())
            o << "compact only";
        else if (compactness_.hasFalse())
            o << "non-compact only";
        else
            o << "reject all compactnesses";
    }
    if (realBoundary_ != BoolSet(true, true)) {
        if (first)
            first = false;
        else
            o << ", ";

        if (realBoundary_.hasTrue())
            o << "real boundary only";
        else if (realBoundary_.hasFalse())
            o << "no real boundary only";
        else
            o << "reject all boundary types";
    }

    if (first)
        o << "Accept all surfaces";
}

void SurfaceFilterProperties::writeXMLPacketData(std::ostream& out,
        FileFormat format, bool anon, PacketRefs& refs) const {
    using regina::xml::xmlValueTag;

    if (format == REGINA_XML_GEN_2) {
        writeXMLHeader(out, "filterprop", format, anon, refs, true);
        out << "  <filter type=\""
            << regina::xml::xmlEncodeSpecialChars(filterTypeName())
            << "\" typeid=\"" << filterType() << "\">\n";

        if (! eulerChar_.empty()) {
            out << "    <euler> ";
            for (const auto& e : eulerChar_)
                out << e << ' ';
            out << "</euler>\n";
        }

        if (orientability_ != BoolSet(true, true))
            out << "    " << xmlValueTag("orbl", orientability_) << '\n';
        if (compactness_ != BoolSet(true, true))
            out << "    " << xmlValueTag("compact", compactness_) << '\n';
        if (realBoundary_ != BoolSet(true, true))
            out << "    " << xmlValueTag("realbdry", realBoundary_) << '\n';

        out << "  </filter>\n";
    } else if (eulerChar_.empty()) {
        writeXMLHeader(out, "filterprop", format, anon, refs, true,
            std::pair("orbl", orientability_.stringCode()),
            std::pair("compact", compactness_.stringCode()),
            std::pair("realbdry", realBoundary_.stringCode()));
    } else {
        std::ostringstream euler;
        bool first = true;
        for (const auto& e : eulerChar_) {
            if (first)
                first = false;
            else
                euler << ',';
            euler << e;
        }
        writeXMLHeader(out, "filterprop", format, anon, refs, true,
            std::pair("orbl", orientability_.stringCode()),
            std::pair("compact", compactness_.stringCode()),
            std::pair("realbdry", realBoundary_.stringCode()),
            std::pair("euler", euler.str()));
    }
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "filterprop", format);
}

} // namespace regina

