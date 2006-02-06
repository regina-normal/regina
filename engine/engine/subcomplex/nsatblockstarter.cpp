
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

#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatblocktypes.h"

namespace regina {

const NSatBlockStarterSet NSatBlockStarterSet::blocks;

void NSatBlockStarterSet::initialise() {
    NSatBlockStarter* starter;

    starter = new NSatBlockStarter;
    starter->block_ = NSatTriPrism::insertBlock(starter->triangulation_, true);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatCube::insertBlock(starter->triangulation_);
    insert(starter);

    // Try various reflector strips of small length.
    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        1, false);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        1, true);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        2, false);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        2, true);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        3, false);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        3, true);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        4, false);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block_ = NSatReflectorStrip::insertBlock(starter->triangulation_,
        4, true);
    insert(starter);
}

} // namespace regina

