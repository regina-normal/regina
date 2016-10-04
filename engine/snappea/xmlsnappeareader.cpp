
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

#include "snappea/snappeatriangulation.h"
#include "snappea/xmlsnappeareader.h"
#include "snappea/kernel/unix_file_io.h"

namespace regina {

void XMLSnapPeaReader::endContentSubElement(
        const std::string& subTagName, XMLElementReader* subReader) {
    if (subTagName == "snappea") {
        if (snappea_->data_) {
            // We can't have two <snappea>..</snappea> blocks.
            return;
        }

        try {
            regina::snappea::Triangulation* data =
                regina::snappea::read_triangulation_from_string(
                dynamic_cast<XMLCharsReader*>(subReader)->chars().c_str());
            if (data) {
                regina::snappea::find_complete_hyperbolic_structure(data);
                regina::snappea::do_Dehn_filling(data);
                snappea_->reset(data);
            }
        } catch (regina::SnapPeaFatalError& err) {
            if (snappea_->data_)
                snappea_->reset(0);
        }
    }
}

XMLPacketReader* SnapPeaTriangulation::xmlReader(Packet*,
        XMLTreeResolver& resolver) {
    return new XMLSnapPeaReader(resolver);
}

} // namespace regina

