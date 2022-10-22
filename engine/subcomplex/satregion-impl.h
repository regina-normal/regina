
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file subcomplex/satregion-impl.h
 *  \brief Full implementation details for SatRegion::find().
 *
 *  This file is _not_ included automatically by satregion.h.
 */

#ifndef __REGINA_SATREGION_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_SATREGION_IMPL_H
#endif

#include "subcomplex/satregion.h"

namespace regina {

template <typename Action, typename... Args>
bool SatRegion::find(const Triangulation<3>& tri, bool mustBeComplete,
        Action&& action, Args&&... args) {
    initStarters();

    SatBlock::TetList usedTets;

    // Hunt for a starting block.
    for (const SatBlockModel& model : starters_) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri.isOrientable() && ! model.triangulation().isOrientable())
            continue;
        if (tri.size() < model.triangulation().size())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        bool terminate = model.triangulation().findAllSubcomplexesIn(tri,
                [&](const Isomorphism<3>& iso) {
            // See if this isomorphism leads somewhere useful.
            SatBlock* starter = model.block().clone();
            starter->transform(model.triangulation(), iso, tri);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting block.
            for (size_t i = 0; i < model.triangulation().size(); i++)
                usedTets.insert(tri.tetrahedron(iso.tetImage(i)));

            // Wrap an initial region around the block, and expand.
            auto* region = new SatRegion(starter);
            if (! region->expand(usedTets, mustBeComplete)) {
                // Nope.  Keep on searching.
                delete region;
                usedTets.clear();
                return false;
            }

            // We have a saturated region: give it to the caller to process.
            bool terminate = action(std::unique_ptr<SatRegion>(region),
                usedTets, std::forward<Args>(args)...);

            usedTets.clear();
            return terminate;
        });
        if (terminate)
            return true;
    }

    // Search over.  Nothing here to see.
    return false;
}

} // namespace regina

#endif

