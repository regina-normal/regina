
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "manifold/ngraphpair.h"
#include "manifold/nsfs.h"
#include "subcomplex/nblockedsfspair.h"
#include "subcomplex/nlayering.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * A subclass of NSatBlockStarterSearcher that, upon finding a starter
 * block, attempts to flesh this out to a pair of saturated regions
 * joined along their single torus boundaries, as desribed by the
 * NBlockedSFSPair class.
 */
struct NBlockedSFSPairSearcher : public NSatBlockStarterSearcher {
    NSatRegion* region[2];
        /**< The two bounded saturated regions that are joined together,
             if the entire NBlockedSFSPair structure has been successfully
             found; otherwise, two null pointers if we are still searching. */
    NMatrix2 matchingReln;
        /**< The matrix describing how the region boundaries are joined
             together.  This matrix expresses the fibre/base curves on the
             second region boundary in terms of the fibre/base curves on
             the first, as described by NGraphPair::matchingReln(). */

    /**
     * Creates a new searcher whose \a region pointers are both null.
     */
    NBlockedSFSPairSearcher() {
        region[0] = region[1] = 0;
    }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NBlockedSFSPair::~NBlockedSFSPair() {
    if (region_[0])
        delete region_[0];
    if (region_[1])
        delete region_[1];
}

NManifold* NBlockedSFSPair::getManifold() const {
    NSFSpace* sfs0 = region_[0]->createSFS(1, false);
    if (! sfs0)
        return 0;

    NSFSpace* sfs1 = region_[1]->createSFS(1, false);
    if (! sfs1) {
        delete sfs0;
        return 0;
    }

    // Reduce the Seifert fibred space representations and finish up.
    sfs0->reduce(false);
    sfs1->reduce(false);

    if (*sfs1 < *sfs0)
        return new NGraphPair(sfs1, sfs0, matchingReln_.inverse());
    else
        return new NGraphPair(sfs0, sfs1, matchingReln_);
}

std::ostream& NBlockedSFSPair::writeName(std::ostream& out) const {
    out << "Blocked SFS Pair [";
    region_[0]->writeBlockAbbrs(out, false);
    out << " | ";
    region_[1]->writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& NBlockedSFSPair::writeTeXName(std::ostream& out) const {
    out << "\\mathrm{BSFS\\_Pair}\\left[";
    region_[0]->writeBlockAbbrs(out, true);
    out << "\\,|\\,";
    region_[1]->writeBlockAbbrs(out, true);
    return out << "\\right]";
}

void NBlockedSFSPair::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS pair, matching relation " << matchingReln_ << "\n";

    region_[0]->writeDetail(out, "First region");
    region_[1]->writeDetail(out, "Second region");
}

NBlockedSFSPair* NBlockedSFSPair::isBlockedSFSPair(NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfComponents() > 1)
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
    NBlockedSFSPairSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region[0]) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new NBlockedSFSPair(searcher.region[0], searcher.region[1],
            searcher.matchingReln);
    }

    // Nope.
    return 0;
}

bool NBlockedSFSPairSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointers should be null, but just in case...
    if (region[0] || region[1]) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // just one boundary annulus remaining.
    // Note that the starter block will now be owned by region[0].
    region[0] = new NSatRegion(starter);
    region[0]->expand(usedTets);

    if (region[0]->numberOfBoundaryAnnuli() != 1) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    // Insist on this boundary being untwisted.
    NSatBlock* bdryBlock;
    unsigned bdryAnnulus;
    bool bdryVert, bdryHoriz;
    region[0]->boundaryAnnulus(0, bdryBlock, bdryAnnulus,
        bdryVert, bdryHoriz);

    bool firstRegionReflected =
        ((bdryVert && ! bdryHoriz) || (bdryHoriz && ! bdryVert));

    NSatBlock* tmpBlock;
    unsigned tmpAnnulus;
    bool tmpVert, tmpHoriz;
    bdryBlock->nextBoundaryAnnulus(bdryAnnulus, tmpBlock, tmpAnnulus,
        tmpVert, tmpHoriz);
    if (tmpVert) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    NSatAnnulus bdry = bdryBlock->annulus(bdryAnnulus);

    // We have a boundary annulus for the first region.

    // Hunt for a layering.
    NLayering layering(bdry.tet[0], bdry.roles[0], bdry.tet[1], bdry.roles[1]);
    layering.extend();

    // Relation from fibre/orbifold to layering first face markings 01/02:
    NMatrix2 curves0ToLayering = layering.boundaryReln() *
        NMatrix2(-1, 0, 0, firstRegionReflected ? -1 : 1);

    // We make the shell of an other-side boundary annulus; we will fill
    // in the precise vertex role permutations later on.
    NSatAnnulus otherSide(layering.getNewBoundaryTet(0), NPerm(),
        layering.getNewBoundaryTet(1), NPerm());

    if (otherSide.meetsBoundary()) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    // Mapping from (layering first face markings 01/02) to
    // (other side annulus first face markings 01/02).  Like the other
    // side vertex roles, this mapping will be filled in later.
    NMatrix2 layeringToAnnulus1;

    // Try the three possible orientations for fibres on the other side.
    NSatBlock* otherStarter;
    for (int plugPos = 0; plugPos < 3; plugPos++) {
        // Construct the boundary annulus for the second region.
        // Refresh the tetrahedra as well as the vertex roles, since
        // it may have switched sides since our last run through the loop.
        otherSide.tet[0] = layering.getNewBoundaryTet(0);
        otherSide.tet[1] = layering.getNewBoundaryTet(1);

        if (plugPos == 0) {
            otherSide.roles[0] = layering.getNewBoundaryRoles(0);
            otherSide.roles[1] = layering.getNewBoundaryRoles(1);
            layeringToAnnulus1 = NMatrix2(1, 0, 0, 1);
        } else if (plugPos == 1) {
            otherSide.roles[0] = layering.getNewBoundaryRoles(0) *
                NPerm(1, 2, 0, 3);
            otherSide.roles[1] = layering.getNewBoundaryRoles(1) *
                NPerm(1, 2, 0, 3);
            layeringToAnnulus1 = NMatrix2(-1, 1, -1, 0);
        } else {
            otherSide.roles[0] = layering.getNewBoundaryRoles(0) *
                NPerm(2, 0, 1, 3);
            otherSide.roles[1] = layering.getNewBoundaryRoles(1) *
                NPerm(2, 0, 1, 3);
            layeringToAnnulus1 = NMatrix2(0, -1, 1, -1);
        }

        // Clear out the used tetrahedron list.  Everything before the new
        // layering boundary is self-contained, so we won't run into it
        // again on the other side.  We'll just re-insert the layering
        // boundary tetrahedra.
        usedTets.clear();
        usedTets.insert(layering.getNewBoundaryTet(0));
        usedTets.insert(layering.getNewBoundaryTet(1));

        // See if we can flesh the other side out to an entire region.
        otherSide.switchSides();

        if ((otherStarter = NSatBlock::isBlock(otherSide, usedTets))) {
            region[1] = new NSatRegion(otherStarter);
            region[1]->expand(usedTets);

            if (region[1]->numberOfBoundaryAnnuli() == 1) {
                // This is it!  Stop searching.
                // Do a final conversion from annulus first face markings
                // 01/02 and exit.
                matchingReln = NMatrix2(-1, 0, 0, 1) * layeringToAnnulus1 *
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

