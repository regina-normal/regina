
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

void SatBlockStarterSearcher::findStarterBlocks(Triangulation<3>* tri) {
    // Clean up usedTets if required.
    if (! usedTets.empty())
        usedTets.clear();

    // Hunt for a starting block.
    for (const SatBlockStarter* model : SatBlockStarterSet()) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! model->triangulation().isOrientable())
            continue;
        if (tri->size() < model->triangulation().size())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        // In the lambda below, all our captures are pointers (hence [=]).
        bool terminate = model->triangulation().findAllSubcomplexesIn(*tri,
                [=](const Isomorphism<3>& iso) {
            // See if this isomorphism leads somewhere useful.
            SatBlock* starter = model->block()->clone();
            starter->transform(&model->triangulation(), &iso, tri);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting block.
            for (unsigned long i = 0; i < model->triangulation().size(); i++)
                usedTets.insert(tri->tetrahedron(iso.tetImage(i)));

            // And process!
            // Note that useStarterBlock() passes ownership of the starter
            // block elsewhere.
            if (! useStarterBlock(starter)) {
                // The search ends now.
                usedTets.clear();
                return true;
            }

            // Keep on searching.
            usedTets.clear();
            return false;
        });
        if (terminate)
            return;
    }

    // Search over.  Nothing here to see.
}

} // namespace regina

