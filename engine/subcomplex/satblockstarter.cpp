
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

#include "subcomplex/satblockstarter.h"
#include "subcomplex/satblocktypes.h"

namespace regina {

const SatBlockStarterSet SatBlockStarterSet::blocks;

void SatBlockStarterSet::initialise() {
    SatBlockStarter* starter;

    starter = new SatBlockStarter;
    starter->block_ = SatTriPrism::insertBlock(starter->triangulation_, true);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatCube::insertBlock(starter->triangulation_);
    insert(starter);

    // Try various reflector strips of small length.
    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        1, false);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        1, true);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        2, false);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        2, true);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        3, false);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        3, true);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        4, false);
    insert(starter);

    starter = new SatBlockStarter;
    starter->block_ = SatReflectorStrip::insertBlock(starter->triangulation_,
        4, true);
    insert(starter);
}

void SatBlockStarterSearcher::findStarterBlocks(Triangulation<3>* tri) {
    // Clean up usedTets if required.
    if (! usedTets.empty())
        usedTets.clear();

    // Hunt for a starting block.
    unsigned long i;
    SatBlockStarterSet::iterator it;
    std::list<Isomorphism<3>*> isos;
    std::list<Isomorphism<3>*>::iterator isoIt;
    SatBlock* starter;
    for (it = SatBlockStarterSet::begin(); it != SatBlockStarterSet::end();
            it++) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! (*it)->triangulation().isOrientable())
            continue;
        if (tri->size() < (*it)->triangulation().size())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        if (! (*it)->triangulation().findAllSubcomplexesIn(*tri,
                back_inserter(isos)))
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
            for (i = 0; i < (*it)->triangulation().size(); i++)
                usedTets.insert(tri->tetrahedron((*isoIt)->tetImage(i)));

            // And process!
            // Note that useStarterBlock() passes ownership of the starter
            // block elsewhere.
            if (! useStarterBlock(starter)) {
                // The search ends now.
                // Don't forget to destroy all remaining isomorphisms.
                usedTets.clear();

                while (isoIt != isos.end())
                    delete *isoIt++;

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

