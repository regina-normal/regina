
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

#include "manifold/graphpair.h"
#include "manifold/sfs.h"
#include "subcomplex/blockedsfspair.h"
#include "subcomplex/layering.h"
#include "subcomplex/satblockstarter.h"
#include "subcomplex/satregion.h"

namespace regina {

/**
 * A subclass of SatBlockStarterSearcher that, upon finding a starter
 * block, attempts to flesh this out to a pair of saturated regions
 * joined along their single torus boundaries, as desribed by the
 * BlockedSFSPair class.
 */
struct BlockedSFSPairSearcher : public SatBlockStarterSearcher {
    SatRegion* region[2];
        /**< The two bounded saturated regions that are joined together,
             if the entire BlockedSFSPair structure has been successfully
             found; otherwise, two null pointers if we are still searching. */
    Matrix2 matchingReln;
        /**< The matrix describing how the region boundaries are joined
             together.  This matrix expresses the fibre/base curves on the
             second region boundary in terms of the fibre/base curves on
             the first, as described by GraphPair::matchingReln(). */

    /**
     * Creates a new searcher whose \a region pointers are both null.
     */
    BlockedSFSPairSearcher() {
        region[0] = region[1] = 0;
    }

    protected:
        bool useStarterBlock(SatBlock* starter);
};

BlockedSFSPair::~BlockedSFSPair() {
    if (region_[0])
        delete region_[0];
    if (region_[1])
        delete region_[1];
}

Manifold* BlockedSFSPair::manifold() const {
    SFSpace* sfs0 = region_[0]->createSFS(false);
    if (! sfs0)
        return 0;

    SFSpace* sfs1 = region_[1]->createSFS(false);
    if (! sfs1) {
        delete sfs0;
        return 0;
    }

    // Reduce the Seifert fibred space representations and finish up.
    sfs0->reduce(false);
    sfs1->reduce(false);

    if (*sfs1 < *sfs0)
        return new GraphPair(sfs1, sfs0, matchingReln_.inverse());
    else
        return new GraphPair(sfs0, sfs1, matchingReln_);
}

std::ostream& BlockedSFSPair::writeName(std::ostream& out) const {
    out << "Blocked SFS Pair [";
    region_[0]->writeBlockAbbrs(out, false);
    out << " | ";
    region_[1]->writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& BlockedSFSPair::writeTeXName(std::ostream& out) const {
    out << "\\mathrm{BSFS\\_Pair}\\left[";
    region_[0]->writeBlockAbbrs(out, true);
    out << "\\,|\\,";
    region_[1]->writeBlockAbbrs(out, true);
    return out << "\\right]";
}

void BlockedSFSPair::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS pair, matching relation " << matchingReln_ << "\n";

    region_[0]->writeDetail(out, "First region");
    region_[1]->writeDetail(out, "Second region");
}

BlockedSFSPair* BlockedSFSPair::isBlockedSFSPair(Triangulation<3>* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->countComponents() > 1)
        return 0;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  Also watch for the boundary between blocks
    // being an annulus on one side and a Klein bottle on the other (or
    // two incompatible Klein bottles for that matter).
    //
    // These will result in edges joined to themselves in reverse.
    if (! tri->isValid())
        return 0;

    // Hunt for a starting block.
    BlockedSFSPairSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region[0]) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new BlockedSFSPair(searcher.region[0], searcher.region[1],
            searcher.matchingReln);
    }

    // Nope.
    return 0;
}

bool BlockedSFSPairSearcher::useStarterBlock(SatBlock* starter) {
    // The region pointers should be null, but just in case...
    if (region[0] || region[1]) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // just one boundary annulus remaining.
    // Note that the starter block will now be owned by region[0].
    region[0] = new SatRegion(starter);
    region[0]->expand(usedTets);

    if (region[0]->numberOfBoundaryAnnuli() != 1) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    // Insist on this boundary being untwisted.
    SatBlock* bdryBlock;
    unsigned bdryAnnulus;
    bool bdryVert, bdryHoriz;
    region[0]->boundaryAnnulus(0, bdryBlock, bdryAnnulus,
        bdryVert, bdryHoriz);

    bool firstRegionReflected =
        ((bdryVert && ! bdryHoriz) || (bdryHoriz && ! bdryVert));

    SatBlock* tmpBlock;
    unsigned tmpAnnulus;
    bool tmpVert, tmpHoriz;
    bdryBlock->nextBoundaryAnnulus(bdryAnnulus, tmpBlock, tmpAnnulus,
        tmpVert, tmpHoriz, false);
    if (tmpVert) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    SatAnnulus bdry = bdryBlock->annulus(bdryAnnulus);

    // We have a boundary annulus for the first region.

    // Hunt for a layering.
    Layering layering(bdry.tet[0], bdry.roles[0], bdry.tet[1], bdry.roles[1]);
    layering.extend();

    // Relation from fibre/orbifold to layering first triangle markings 01/02:
    Matrix2 curves0ToLayering = layering.boundaryReln() *
        Matrix2(-1, 0, 0, firstRegionReflected ? -1 : 1);

    // We make the shell of an other-side boundary annulus; we will fill
    // in the precise vertex role permutations later on.
    SatAnnulus otherSide(layering.newBoundaryTet(0), Perm<4>(),
        layering.newBoundaryTet(1), Perm<4>());

    if (otherSide.meetsBoundary()) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    // Mapping from (layering first triangle markings 01/02) to
    // (other side annulus first triangle markings 01/02).  Like the other
    // side vertex roles, this mapping will be filled in later.
    Matrix2 layeringToAnnulus1;

    // Try the three possible orientations for fibres on the other side.
    SatBlock* otherStarter;
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

        if ((otherStarter = SatBlock::isBlock(otherSide, usedTets))) {
            region[1] = new SatRegion(otherStarter);
            region[1]->expand(usedTets);

            if (region[1]->numberOfBoundaryAnnuli() == 1) {
                // This is it!  Stop searching.
                // Do a final conversion from annulus first triangle markings
                // 01/02 and exit.
                matchingReln = Matrix2(-1, 0, 0, 1) * layeringToAnnulus1 *
                    curves0ToLayering;
                return false;
            }

            // Nup, this one didn't work.
            delete region[1];
            region[1] = 0;
        }
    }

    // Sigh, nothing works.
    delete region[0];
    region[0] = 0;
    return true;
}

} // namespace regina

