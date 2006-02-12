
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

void NSatBlockStarterSearcher::findStarterBlocks(NTriangulation* tri) {
    // Clean up usedTets if required.
    if (! usedTets.empty())
        usedTets.clear();

    // Hunt for a starting block.
    unsigned long i;
    NSatBlockStarterSet::iterator it;
    std::list<NIsomorphism*> isos;
    std::list<NIsomorphism*>::iterator isoIt;
    NSatBlock* starter;
    for (it = NSatBlockStarterSet::begin(); it != NSatBlockStarterSet::end();
            it++) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! (*it)->triangulation().isOrientable())
            continue;
        if (tri->getNumberOfTetrahedra() <
                (*it)->triangulation().getNumberOfTetrahedra())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        if (! (*it)->triangulation().findAllSubcomplexesIn(*tri, isos))
            continue;

        // Run through each isomorphism in the list and see if it leads
        // somewhere useful.
        //
        // All of the isomorphisms in this list _must_ be destroyed at
        // some point before we loop back to the next starter block.
        for (isoIt = isos.begin(); isoIt != isos.end(); isoIt++) {
            starter = (*it)->block()->clone();
            starter->transform(&(*it)->triangulation(), *isoIt, tri);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting block.
            for (i = 0; i < (*it)->triangulation().getNumberOfTetrahedra(); i++)
                usedTets.insert(tri->getTetrahedron((*isoIt)->tetImage(i)));

            // And process!
            // Note that useStarterBlock() passes ownership of the starter
            // block elsewhere.
            if (! useStarterBlock(starter)) {
                // The search ends now.
                // Don't forget to destroy the remaining isomorphisms
                // that we never looked at.
                usedTets.clear();

                for (isoIt++; isoIt != isos.end(); isoIt++)
                    delete *isoIt;

                return;
            }

            // Keep on searching.
            // Destroy this isomorphism and make things ready for the next one.
            usedTets.clear();
            delete *isoIt;
        }

        // Make sure the list is empty again for the next time around.
        isos.clear();
    }

    // Search over.  Nothing here to see.
}

} // namespace regina

