
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

#include "manifold/graphpair.h"
#include "manifold/sfs.h"
#include "subcomplex/blockedsfspair.h"
#include "subcomplex/layering.h"
#include "subcomplex/satregion-impl.h"

namespace regina {

std::unique_ptr<Manifold> BlockedSFSPair::manifold() const {
    try {
        SFSpace sfs0 = region_[0].createSFS(false);
        SFSpace sfs1 = region_[1].createSFS(false);

        // Reduce the Seifert fibred space representations and finish up.
        sfs0.reduce(false);
        sfs1.reduce(false);

        if (sfs1 < sfs0)
            return std::make_unique<GraphPair>(std::move(sfs1), std::move(sfs0),
                matchingReln_.inverse());
        else
            return std::make_unique<GraphPair>(std::move(sfs0), std::move(sfs1),
                matchingReln_);
    } catch (const regina::NotImplemented&) {
        return nullptr;
    }
}

std::ostream& BlockedSFSPair::writeName(std::ostream& out) const {
    out << "Blocked SFS Pair [";
    region_[0].writeBlockAbbrs(out, false);
    out << " | ";
    region_[1].writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& BlockedSFSPair::writeTeXName(std::ostream& out) const {
    out << R"(\mathrm{BSFS\_Pair}\left[)";
    region_[0].writeBlockAbbrs(out, true);
    out << R"(\,|\,)";
    region_[1].writeBlockAbbrs(out, true);
    return out << R"(\right])";
}

void BlockedSFSPair::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS pair, matching relation " << matchingReln_ << "\n";

    region_[0].writeDetail(out, "First region");
    region_[1].writeDetail(out, "Second region");
}

std::unique_ptr<BlockedSFSPair> BlockedSFSPair::recognise(
        const Triangulation<3>& tri) {
    // Basic property checks.
    if (! tri.isClosed())
        return nullptr;
    if (tri.countComponents() > 1)
        return nullptr;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  Also watch for the boundary between blocks
    // being an annulus on one side and a Klein bottle on the other (or
    // two incompatible Klein bottles for that matter).
    //
    // These will result in edges joined to themselves in reverse.
    if (! tri.isValid())
        return nullptr;

    // Hunt for a starting block.
    std::unique_ptr<SatRegion> r0;
    std::unique_ptr<SatRegion> r1;
    Matrix2 matchingReln;
    bool found = SatRegion::find(tri, false,
            [&](std::unique_ptr<SatRegion> r, SatBlock::TetList& usedTets) {
        if (r->countBoundaryAnnuli() != 1)
            return false;

        // Insist on this boundary being untwisted.
        auto [bdryBlock, bdryAnnulus, bdryVert, bdryHoriz] =
            r->boundaryAnnulus(0);

        bool firstRegionReflected =
            ((bdryVert && ! bdryHoriz) || (bdryHoriz && ! bdryVert));

        auto [tmpBlock, tmpAnnulus, tmpVert, tmpHoriz] =
            bdryBlock->nextBoundaryAnnulus(bdryAnnulus, false);
        if (tmpVert)
            return false;

        SatAnnulus bdry = bdryBlock->annulus(bdryAnnulus);

        // We have a boundary annulus for the first region.

        // Hunt for a layering.
        Layering layering(bdry.tet[0], bdry.roles[0],
            bdry.tet[1], bdry.roles[1]);
        layering.extend();

        // Relation from fibre/orbifold to layering first
        // triangle markings 01/02:
        Matrix2 curves0ToLayering = layering.boundaryReln() *
            Matrix2(-1, 0, 0, firstRegionReflected ? -1 : 1);

        // We make the shell of an other-side boundary annulus; we will fill
        // in the precise vertex role permutations later on.
        SatAnnulus otherSide(layering.newBoundaryTet(0), Perm<4>(),
            layering.newBoundaryTet(1), Perm<4>());

        if (otherSide.meetsBoundary())
            return false;

        // Mapping from (layering first triangle markings 01/02) to
        // (other side annulus first triangle markings 01/02).  Like the other
        // side vertex roles, this mapping will be filled in later.
        Matrix2 layeringToAnnulus1;

        // Try the three possible orientations for fibres on the other side.
        for (int plugPos = 0; plugPos < 3; plugPos++) {
            // Construct the boundary annulus for the second region.
            // Refresh the tetrahedra as well as the vertex roles, since
            // it may have switched sides since our last run through the loop.
            otherSide.tet[0] = layering.newBoundaryTet(0);
            otherSide.tet[1] = layering.newBoundaryTet(1);

            if (plugPos == 0) {
                otherSide.roles[0] = layering.newBoundaryRoles(0);
                otherSide.roles[1] = layering.newBoundaryRoles(1);
                layeringToAnnulus1 = Matrix2(1, 0, 0, 1);
            } else if (plugPos == 1) {
                otherSide.roles[0] = layering.newBoundaryRoles(0) *
                    Perm<4>(1, 2, 0, 3);
                otherSide.roles[1] = layering.newBoundaryRoles(1) *
                    Perm<4>(1, 2, 0, 3);
                layeringToAnnulus1 = Matrix2(-1, 1, -1, 0);
            } else {
                otherSide.roles[0] = layering.newBoundaryRoles(0) *
                    Perm<4>(2, 0, 1, 3);
                otherSide.roles[1] = layering.newBoundaryRoles(1) *
                    Perm<4>(2, 0, 1, 3);
                layeringToAnnulus1 = Matrix2(0, -1, 1, -1);
            }

            // Clear out the used tetrahedron list.  Everything before the new
            // layering boundary is self-contained, so we won't run into it
            // again on the other side.  We'll just re-insert the layering
            // boundary tetrahedra.
            usedTets.clear();
            usedTets.insert(layering.newBoundaryTet(0));
            usedTets.insert(layering.newBoundaryTet(1));

            // See if we can flesh the other side out to an entire region.
            otherSide.switchSides();

            if ((r1 = SatRegion::beginsRegion(otherSide, usedTets))) {
                if (r1->countBoundaryAnnuli() == 1) {
                    // This is it!  Stop searching.
                    r0 = std::move(r);

                    // Do a final conversion from annulus first triangle
                    // markings 01/02 and exit.
                    matchingReln = Matrix2(-1, 0, 0, 1) * layeringToAnnulus1 *
                        curves0ToLayering;
                    return true;
                }

                // Nup, this one didn't work.
                r1.reset();
            }
        }

        // Sigh, nothing works.
        return false;
    });

    // Any luck?
    if (found) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        //
        // Note: we cannot use make_unique here, since the class constructor
        // is private.
        return std::unique_ptr<BlockedSFSPair>(new BlockedSFSPair(
            std::move(*r0), std::move(*r1), matchingReln));
    }

    // Nope.
    return nullptr;
}

} // namespace regina

