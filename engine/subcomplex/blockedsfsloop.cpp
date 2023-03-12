
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "manifold/graphloop.h"
#include "manifold/sfs.h"
#include "subcomplex/blockedsfsloop.h"
#include "subcomplex/layering.h"
#include "subcomplex/satregion-impl.h"

namespace regina {

std::unique_ptr<Manifold> BlockedSFSLoop::manifold() const {
    try {
        SFSpace sfs = region_.createSFS(false);
        if (sfs.punctures() == 1) {
            // The region has one larger boundary, but we pinch it to create
            // two smaller boundaries.
            sfs.addPuncture();
        }

        sfs.reduce(false);

        return std::make_unique<GraphLoop>(std::move(sfs), matchingReln_);
    } catch (const regina::NotImplemented&) {
        return nullptr;
    }
}

std::ostream& BlockedSFSLoop::writeName(std::ostream& out) const {
    out << "Blocked SFS Loop [";
    region_.writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& BlockedSFSLoop::writeTeXName(std::ostream& out) const {
    out << R"(\mathrm{BSFS\_Loop}\left[)";
    region_.writeBlockAbbrs(out, true);
    return out << R"(\right])";
}

void BlockedSFSLoop::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS Loop, matching relation " << matchingReln_ << '\n';
    region_.writeDetail(out, "Internal region");
}

std::unique_ptr<BlockedSFSLoop> BlockedSFSLoop::recognise(
        const Triangulation<3>& tri) {
    // Basic property checks.
    if (! tri.isClosed())
        return nullptr;
    if (tri.countComponents() > 1)
        return nullptr;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  Also watch for saturated tori being joined
    // to saturated Klein bottles.  Any of these issues will result in
    // edges joined to themselves in reverse.
    if (! tri.isValid())
        return nullptr;

    // Hunt for a starting block.
    std::unique_ptr<SatRegion> region;
    Matrix2 matchingReln;
    bool found = SatRegion::find(tri, false,
            [&](std::unique_ptr<SatRegion> r, SatBlock::TetList& usedTets) {
        if (r->countBoundaryAnnuli() != 2)
            return false;

        auto [bdryBlock0, bdryAnnulus0, bdryRefVert0, bdryRefHoriz0] =
            r->boundaryAnnulus(0);
        auto [bdryBlock1, bdryAnnulus1, bdryRefVert1, bdryRefHoriz1] =
            r->boundaryAnnulus(1);

        // We either want two disjoint one-annulus torus boundaries, or else a
        // single two-annulus boundary that is pinched to turn each annulus into
        // a two-sided torus.  The following test will handle all cases.  We
        // don't worry about the degenerate case of fibres mapping to fibres
        // through the layering in the pinched case, since this will fail
        // our test anyway (either boundaries do not form tori, or they are
        // not two-sided).
        SatAnnulus bdry0 = bdryBlock0->annulus(bdryAnnulus0);
        SatAnnulus bdry1 = bdryBlock1->annulus(bdryAnnulus1);

        if (! (bdry0.isTwoSidedTorus() && bdry1.isTwoSidedTorus()))
            return false;

        // Look for a layering on the first boundary annulus.
        // Extend the layering one tetrahedron at a time, to make sure we
        // don't loop back onto ourselves.
        Layering layering(bdry0.tet[0], bdry0.roles[0],
            bdry0.tet[1], bdry0.roles[1]);

        SatAnnulus layerTop;
        Matrix2 layerToBdry1;
        while (true) {
            layerTop.tet[0] = layering.newBoundaryTet(0);
            layerTop.tet[1] = layering.newBoundaryTet(1);
            layerTop.roles[0] = layering.newBoundaryRoles(0);
            layerTop.roles[1] = layering.newBoundaryRoles(1);

            // Have we reached the second boundary?
            if (bdry1.isJoined(layerTop, layerToBdry1))
                break;

            // We haven't joined up yet.  Either extend or die.
            if (! layering.extendOne()) {
                // The layering dried up and we didn't make it.
                return false;
            }

            if (usedTets.find(layering.newBoundaryTet(0)) !=
                    usedTets.end() ||
                    usedTets.find(layering.newBoundaryTet(1)) !=
                    usedTets.end()) {
                // Gone too far -- we've looped back upon ourselves.
                return false;
            }

            usedTets.insert(layering.newBoundaryTet(0));
            usedTets.insert(layering.newBoundaryTet(1));
        }

        // This is it!  Build the matching matrix and stop searching.
        region = std::move(r);

        // First find mappings from the fibre/base curves (fi, oi) to
        // annulus #i edges (first triangle: 01, first triangle: 02).
        // Note that each of these matrices is self-inverse.
        Matrix2 curves0ToAnnulus0(bdryRefVert0 ? 1 : -1, 0, 0,
            bdryRefHoriz0 ? -1 : 1);
        Matrix2 curves1ToAnnulus1(bdryRefVert1 ? 1 : -1, 0, 0,
            bdryRefHoriz1 ? -1 : 1);

        // Put it all together.
        // Remember that curves1ToAnnulus1 is self-inverse.
        matchingReln = curves1ToAnnulus1 * layerToBdry1 *
            layering.boundaryReln() * curves0ToAnnulus0;

        return true;
    });

    if (found) {
        // The expansion and self-adjacency worked, and the triangulation
        // is known to be closed and connected.
        // This means we've got one!
        //
        // Note: we cannot use make_unique here, since the class constructor
        // is private.
        return std::unique_ptr<BlockedSFSLoop>(new BlockedSFSLoop(
            std::move(*region), matchingReln));
    }

    // Nope.
    return nullptr;
}

} // namespace regina

