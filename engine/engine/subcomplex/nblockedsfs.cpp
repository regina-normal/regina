
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

#include "manifold/nsfs.h"
#include "subcomplex/nblockedsfs.h"
#include "subcomplex/nsatblocktypes.h"

namespace regina {

const NSatBlockStarterSet NSatBlockStarterSet::blocks;

NBlockedSFS::~NBlockedSFS() {
    for (BlockSet::iterator it = blocks.begin(); it != blocks.end(); it++)
        delete it->block;
}

NManifold* NBlockedSFS::getManifold() const {
    NSFSpace::classType baseClass;
    if (baseOrbl)
        baseClass = (hasTwist ? NSFSpace::o2 : NSFSpace::o1);
    else if (! hasTwist)
        baseClass = NSFSpace::n1;
    else if (twistsMatchOrientation)
        baseClass = NSFSpace::n2;
    else {
        // TODO: Distinguish between n3 and n4!
        baseClass = NSFSpace::n3;
    }

    NSFSpace* ans = new NSFSpace(baseClass,
        (baseOrbl ? (2 - baseEuler) / 2 : (2 - baseEuler)),
        0 /* punctures */, 0 /* reflectors */);

    bool reflect;
    for (BlockSet::const_iterator it = blocks.begin(); it != blocks.end();
            it++) {
        reflect = (it->refVert && ! it->refHoriz) ||
            (it->refHoriz && ! it->refVert);
        it->block->adjustSFS(*ans, reflect);
    }

    return ans;
}

std::ostream& NBlockedSFS::writeName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS";
}

std::ostream& NBlockedSFS::writeTeXName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS";
}

void NBlockedSFS::writeTextLong(std::ostream& out) const {
    // TODO, later
    out << "Blocked SFS";
}

NBlockedSFS* NBlockedSFS::isBlockedSFS(NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;

    // Our triangulation is closed with one component.
    // Hunt for a starting block.
    NSatBlockStarterSet::iterator it;
    std::list<NIsomorphism*> isos;
    std::list<NIsomorphism*>::iterator isoIt;
    NSatBlock* starter;
    for (it = NSatBlockStarterSet::begin(); it != NSatBlockStarterSet::end();
            it++) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! (*it)->triangulation.isOrientable())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        if (! (*it)->triangulation.findAllSubcomplexesIn(*tri, isos))
            continue;

        // Run through each isomorhpism in the list and see if it leads
        // somewhere useful.
        for (isoIt = isos.begin(); isoIt != isos.end(); isoIt++) {
            starter = (*it)->block->clone();
            // TODO: starter->transform(), etc
            // TODO: delete starter, don't forget to delete isomorphisms also
        }

        // Make sure the list is empty again for the next time around.
        isos.clear();
    }

    // Nothing found.
    return 0;
}

void NSatBlockStarterSet::initialise() {
    // TODO: More types of starter block!
    NSatBlockStarter* triPrism = new NSatBlockStarter;
    triPrism->block = NSatTriPrism::insertBlock(triPrism->triangulation, true);
    insert(triPrism);
}

} // namespace regina

