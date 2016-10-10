
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

#include "surfaces/surfacefilter.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "surfaces/filterregistry.h"
#include "utilities/xmlutils.h"

#define TYPE_AND 1
#define TYPE_OR 2

#define PROPSF_EULER 1001
#define PROPSF_ORIENT 1002
#define PROPSF_COMPACT 1003
#define PROPSF_REALBDRY 1004

namespace regina {

void SurfaceFilter::writeXMLPacketData(std::ostream& out) const {
    out << "  <filter type=\""
        << regina::xml::xmlEncodeSpecialChars(filterTypeName())
        << "\" typeid=\"" << filterType() << "\">\n";

    writeXMLFilterData(out);

    out << "  </filter>\n";
}

bool SurfaceFilterCombination::accept(const NormalSurface& surface) const {
    if (usesAnd_) {
        // Combine all child filters using AND.
        for (Packet* child = firstChild(); child; child = child->nextSibling())
            if (child->type() == PACKET_SURFACEFILTER)
                if (! (dynamic_cast<SurfaceFilter*>(child)->accept(surface)))
                    return false;
        return true;
    } else {
        // Combine all child filters using OR.
        for (Packet* child = firstChild(); child; child = child->nextSibling())
            if (child->type() == PACKET_SURFACEFILTER)
                if (dynamic_cast<SurfaceFilter*>(child)->accept(surface))
                    return true;
        return false;
    }
}

void SurfaceFilterCombination::writeXMLFilterData(std::ostream& out) const {
    out << "    <op type=\"" << (usesAnd_ ? "and" : "or") << "\"/>\n";
}

LargeInteger SurfaceFilterProperties::eulerChar(size_t index) const {
    std::set<LargeInteger>::const_iterator it = eulerChar_.begin();
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

void SurfaceFilterProperties::writeTextLong(std::ostream& o) const {
    o << "Filter normal surfaces with restrictions:\n";

    if (eulerChar_.size() > 0) {
        o << "    Euler characteristic:";
        for (auto it = eulerChar_.rbegin(); it != eulerChar_.rend(); ++it)
            o << ' ' << *it;
        o << '\n';
    }
    if (orientability_ != BoolSet::sBoth)
        o << "    Orientability: " << orientability_ << '\n';
    if (compactness_ != BoolSet::sBoth)
        o << "    Compactness: " << compactness_ << '\n';
    if (realBoundary_ != BoolSet::sBoth)
        o << "    Has real boundary: " << realBoundary_ << '\n';
}

void SurfaceFilterProperties::writeXMLFilterData(std::ostream& out) const {
    using regina::xml::xmlValueTag;

    if (! eulerChar_.empty()) {
        out << "    <euler> ";
        for (auto it = eulerChar_.begin(); it != eulerChar_.end(); ++it)
            out << (*it) << ' ';
        out << "</euler>\n";
    }

    if (orientability_ != BoolSet::sBoth)
        out << "    " << xmlValueTag("orbl", orientability_) << '\n';
    if (compactness_ != BoolSet::sBoth)
        out << "    " << xmlValueTag("compact", compactness_) << '\n';
    if (realBoundary_ != BoolSet::sBoth)
        out << "    " << xmlValueTag("realbdry", realBoundary_) << '\n';
}

} // namespace regina

