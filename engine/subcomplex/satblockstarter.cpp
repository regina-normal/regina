
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

#include <mutex>
#include "subcomplex/satblockstarter.h"
#include "subcomplex/satblocktypes.h"
#include "subcomplex/satregion.h"

namespace regina {

namespace {
    std::mutex starterBlocksMutex;
}

std::list<const SatBlockStarter*> SatBlockStarterSet::blocks;

SatBlockStarterSet::SatBlockStarterSet() {
    std::scoped_lock lock(starterBlocksMutex);
    if (blocks.empty()) {
        SatBlockStarter* s;

        s = new SatBlockStarter;
        s->block_ = SatTriPrism::insertBlock(s->triangulation_, true);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatCube::insertBlock(s->triangulation_);
        blocks.push_back(s);

        // Try various reflector strips of small length.
        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            1, false);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            1, true);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            2, false);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            2, true);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            3, false);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            3, true);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            4, false);
        blocks.push_back(s);

        s = new SatBlockStarter;
        s->block_ = SatReflectorStrip::insertBlock(s->triangulation_,
            4, true);
        blocks.push_back(s);
    }
}

} // namespace regina

