
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

#include "link/link.h"
#include "link/xmllinkreader.h"
#include <sstream>

namespace regina {

XMLPacketReader* Link::xmlReader(Packet*, XMLTreeResolver& resolver) {
    return new XMLLinkReader(resolver);
}

void XMLLinkCrossingsReader::initialChars(const std::string& chars) {
    if (! link_)
        return;

    std::istringstream in(chars);

    char c;
    for (size_t read = 0; read < size_; ++read) {
        in >> c;

        if (! in) {
            link_ = 0;
            return;
        }

        if (c == '+')
            link_->crossings_.push_back(new Crossing(1));
        else if (c == '-')
            link_->crossings_.push_back(new Crossing(-1));
        else {
            link_ = 0;
            return;
        }
    }
}

void XMLLinkConnectionsReader::initialChars(const std::string& chars) {
    if (! link_)
        return;

    std::istringstream in(chars);

    std::string s;
    int adjSide;
    size_t crossing;
    Crossing* adj;
    for (size_t read = 0; read < link_->size(); ++read)
        for (int side = 1; side >= 0; --side) {
            in >> s;

            if ((! in) || s.length() < 2) {
                link_ = 0;
                return;
            }

            if (s[0] == '^')
                adjSide = 1;
            else if (s[0] == '_')
                adjSide = 0;
            else {
                link_ = 0;
                return;
            }

            if (! valueOf(s.c_str() + 1, crossing)) {
                link_ = 0;
                return;
            }
            if (/* crossing < 0 || */ crossing >= link_->size()) {
                link_ = 0;
                return;
            }

            adj = link_->crossing(crossing);
            if (adj->prev_[adjSide]) {
                link_ = 0;
                return;
            }
            link_->crossing(read)->next_[side] = adj->strand(adjSide);
            adj->prev_[adjSide] = link_->crossing(read)->strand(side);
        }
}

void XMLLinkComponentsReader::initialChars(const std::string& chars) {
    if (! link_)
        return;

    std::istringstream in(chars);

    std::string s;
    int strand;
    size_t crossing;
    for (size_t read = 0; read < size_; ++read) {
        in >> s;

        if ((! in) || s.length() < 2) {
            link_ = 0;
            return;
        }
        
        if (s == "(null)") {
            link_->components_.push_back(StrandRef(nullptr, 0));
            continue;
        }

        if (s[0] == '^')
            strand = 1;
        else if (s[0] == '_')
            strand = 0;
        else {
            link_ = 0;
            return;
        }

        if (! valueOf(s.c_str() + 1, crossing)) {
            link_ = 0;
            return;
        }
        if (/* crossing < 0 || */ crossing >= link_->size()) {
            link_ = 0;
            return;
        }

        link_->components_.push_back(link_->crossing(crossing)->strand(strand));
    }
}

} // namespace regina

