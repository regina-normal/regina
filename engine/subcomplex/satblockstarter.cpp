
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

SatBlock* SatBlockStarter::isomorphicCopy(Triangulation<3>& tri,
        const Isomorphism<3>& iso) const {
    SatBlock* ans = block_->clone();
    ans->transform(triangulation_, iso, tri);
    return ans;
}

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

void SatBlockStarterSearcher::findStarterBlocks(Triangulation<3>* tri,
        bool mustBeComplete) {
    SatBlock::TetList usedTets;

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
        // In the lambda below, most of our captures are pointers (hence [=]).
        bool terminate = model->triangulation().findAllSubcomplexesIn(*tri,
                [=, &usedTets](const Isomorphism<3>& iso) {
            // See if this isomorphism leads somewhere useful.
            SatBlock* starter = model->isomorphicCopy(*tri, iso);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting block.
            for (unsigned long i = 0; i < model->triangulation().size(); i++)
                usedTets.insert(tri->tetrahedron(iso.tetImage(i)));

            // Wrap an initial region around the block, and expand.
            SatRegion* region = new SatRegion(starter);
            if (! region->expand(usedTets, mustBeComplete)) {
                // Nope.  Keep on searching.
                delete region;
                usedTets.clear();
                return false;
            }

            // We have a saturated region: give it to the caller to process.
            // Note that useStarterBlock() passes ownership of the region
            // elsewhere, regardless of whether it ultimately succeeded.
            bool success = ! useStarterBlock(region, usedTets);

            usedTets.clear();
            return success; // terminate the subcomplex search upon success
        });
        if (terminate)
            return;
    }

    // Search over.  Nothing here to see.
}

} // namespace regina

