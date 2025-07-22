
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "manifold/graphtriple.h"
#include "manifold/sfs.h"
#include "subcomplex/blockedsfstriple.h"
#include "subcomplex/layering.h"
#include "subcomplex/satregion-impl.h"

namespace regina {

std::unique_ptr<Manifold> BlockedSFSTriple::manifold() const {
    try {
        // Go ahead and create the Seifert fibred spaces.
        SFSpace end0 = end_[0].createSFS(false);
        SFSpace end1 = end_[1].createSFS(false);
        SFSpace hub = centre_.createSFS(false);

        if (hub.punctures() == 1) {
            // The region has one larger boundary, but we pinch it to create
            // two smaller boundaries.
            hub.addPuncture();
        }

        // Reduce the Seifert fibred space representations and finish up.
        end0.reduce(false);
        end1.reduce(false);
        hub.reduce(false);

        return std::make_unique<GraphTriple>(
            std::move(end0), std::move(hub), std::move(end1),
            matchingReln_[0], matchingReln_[1]);
    } catch (const regina::NotImplemented&) {
        return nullptr;
    }
}

std::ostream& BlockedSFSTriple::writeName(std::ostream& out) const {
    out << "Blocked SFS Triple [";
    end_[0].writeBlockAbbrs(out, false);
    out << " | ";
    centre_.writeBlockAbbrs(out, false);
    out << " | ";
    end_[1].writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& BlockedSFSTriple::writeTeXName(std::ostream& out) const {
    out << R"(\mathrm{BSFS\_Triple}\left[)";
    end_[0].writeBlockAbbrs(out, true);
    out << R"(\,|\,)";
    centre_.writeBlockAbbrs(out, true);
    out << R"(\,|\,)";
    end_[1].writeBlockAbbrs(out, true);
    return out << R"(\right])";
}

void BlockedSFSTriple::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS triple\n";
    out << "Matching relation (centre -> end #1): " << matchingReln_[0] << '\n';
    out << "Matching relation (centre -> end #2): " << matchingReln_[1] << '\n';

    centre_.writeDetail(out, "Central region");
    end_[0].writeDetail(out, "First end region");
    end_[1].writeDetail(out, "Second end region");
}

std::unique_ptr<BlockedSFSTriple> BlockedSFSTriple::recognise(
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
    std::unique_ptr<SatRegion> end[2];
    std::unique_ptr<SatRegion> centre;
    Matrix2 matchingReln[2];
    bool found = SatRegion::find(tri, false,
            [&](std::unique_ptr<SatRegion> r, SatBlock::TetList& usedTets) {
        if (r->countBoundaryAnnuli() != 2)
            return false;

        // Insist on the boundary annuli being disjoint and untwisted.
        auto [bdryBlock0, bdryAnnulus0, bdryVert0, bdryHoriz0] =
            r->boundaryAnnulus(0);
        auto [bdryBlock1, bdryAnnulus1, bdryVert1, bdryHoriz1] =
            r->boundaryAnnulus(1);

        bool bdryRef[2] = {
            ((bdryVert0 && ! bdryHoriz0) || (bdryHoriz0 && ! bdryVert0)),
            ((bdryVert1 && ! bdryHoriz1) || (bdryHoriz1 && ! bdryVert1)) };

        // We either want two disjoint one-annulus boundaries, or else a
        // single two-annulus boundary that is pinched to turn each annulus
        // into a two-sided torus.  The following test handles all cases.
        SatAnnulus bdry[2] = {
            bdry[0] = bdryBlock0->annulus(bdryAnnulus0),
            bdry[1] = bdryBlock1->annulus(bdryAnnulus1) };

        if (! (bdry[0].isTwoSidedTorus() && bdry[1].isTwoSidedTorus()))
            return false;

        // Hunt for layerings, but gently gently -- we don't want to loop
        // from one boundary back onto the other.
        std::unique_ptr<Layering> layering[2];

        int e;
        for (e = 0; e < 2; e++) {
            layering[e] = std::make_unique<Layering>(bdry[e].tet[0],
                bdry[e].roles[0], bdry[e].tet[1], bdry[e].roles[1]);

            while (layering[e]->extendOne()) {
                if (usedTets.find(layering[e]->newBoundaryTet(0)) !=
                        usedTets.end() ||
                        usedTets.find(layering[e]->newBoundaryTet(1)) !=
                        usedTets.end()) {
                    // Oops, we've run back into something we've already seen.
                    return false;
                }
                usedTets.insert(layering[e]->newBoundaryTet(0));
                usedTets.insert(layering[e]->newBoundaryTet(1));
            }
        }

        // Start looking for the end regions.
        int plugPos;
        Matrix2 curvesCentreToLayering, layeringToEndAnnulus;

        for (e = 0; e < 2; e++) {
            // Relation from centre fibre/orbifold to layering first triangle
            // markings 01/02:
            curvesCentreToLayering = layering[e]->boundaryReln() *
                Matrix2(-1, 0, 0, bdryRef[e] ? -1 : 1);

            // We make the shell of an other-side boundary annulus; we will fill
            // in the precise vertex role permutations later on.
            SatAnnulus otherSide(layering[e]->newBoundaryTet(0), Perm<4>(),
                layering[e]->newBoundaryTet(1), Perm<4>());

            if (otherSide.meetsBoundary()) {
                if (e == 1)
                    end[0].reset();
                return false;
            }

            // Try the three possible orientations for fibres on the other side.
            for (plugPos = 0; plugPos < 3; plugPos++) {
                // Construct the boundary annulus for the end region.
                // Refresh the tetrahedra as well as the vertex roles, since it
                // may have switched sides since our last run through the loop.
                otherSide.tet[0] = layering[e]->newBoundaryTet(0);
                otherSide.tet[1] = layering[e]->newBoundaryTet(1);

                // In each case, also fill in the mapping from (layering first
                // triangle markings 01/02) to (other side annulus first
                // triangle markings 01/02).  This is stored in
                // layeringToEndAnnulus.
                if (plugPos == 0) {
                    otherSide.roles[0] = layering[e]->newBoundaryRoles(0);
                    otherSide.roles[1] = layering[e]->newBoundaryRoles(1);
                    layeringToEndAnnulus = Matrix2(1, 0, 0, 1);
                } else if (plugPos == 1) {
                    otherSide.roles[0] = layering[e]->newBoundaryRoles(0) *
                        Perm<4>(1, 2, 0, 3);
                    otherSide.roles[1] = layering[e]->newBoundaryRoles(1) *
                        Perm<4>(1, 2, 0, 3);
                    layeringToEndAnnulus = Matrix2(-1, 1, -1, 0);
                } else {
                    otherSide.roles[0] = layering[e]->newBoundaryRoles(0) *
                        Perm<4>(2, 0, 1, 3);
                    otherSide.roles[1] = layering[e]->newBoundaryRoles(1) *
                        Perm<4>(2, 0, 1, 3);
                    layeringToEndAnnulus = Matrix2(0, -1, 1, -1);
                }

                // Clear out the used tetrahedron list.  Everything between the
                // two layering boundaries is self-contained, so we won't run
                // into any of it again on the other side.  We'll just re-insert
                // the layering boundary tetrahedra.
                usedTets.clear();
                usedTets.insert(layering[0]->newBoundaryTet(0));
                usedTets.insert(layering[0]->newBoundaryTet(1));
                usedTets.insert(layering[1]->newBoundaryTet(0));
                usedTets.insert(layering[1]->newBoundaryTet(1));

                // See if we can flesh the other side out to an entire region.
                otherSide.switchSides();

                if ((end[e] = SatRegion::beginsRegion(otherSide, usedTets))) {
                    if (end[e]->countBoundaryAnnuli() == 1) {
                        // Got it!
                        // Do a final conversion from annulus first triangle
                        // markings 01/02 and move onto the next end space.
                        matchingReln[e] = Matrix2(-1, 0, 0, 1) *
                            layeringToEndAnnulus * curvesCentreToLayering;
                        break;
                    }

                    // Nup, this one didn't work.
                    end[e].reset();
                }
            }

            // Did we manage to fill in this end space?
            if (! end[e]) {
                // Nope.  Keep searching.
                if (e == 1)
                    end[0].reset();
                return false;
            }
        }

        // w00t!  It all worked out.
        // Stop searching, we're done.
        centre = std::move(r);
        return true;
    });

    // Any luck?
    if (found) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        //
        // Note: we cannot use make_unique here, since the class constructor
        // is private.
        return std::unique_ptr<BlockedSFSTriple>(new BlockedSFSTriple(
            std::move(*end[0]), std::move(*centre), std::move(*end[1]),
            matchingReln[0], matchingReln[1]));
    }

    // Nope.
    return nullptr;
}

} // namespace regina

