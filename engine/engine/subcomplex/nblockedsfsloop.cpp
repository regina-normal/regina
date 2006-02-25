
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

#include "manifold/ngraphloop.h"
#include "manifold/nsfs.h"
#include "subcomplex/nblockedsfsloop.h"
#include "subcomplex/nlayering.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * A subclass of NSatBlockStarterSearcher that, upon finding a starter
 * block, attempts to flesh this out to an entire saturated region with
 * two identified torus boundaries, as described by the NNGBlockedSFSLoop
 * class.
 */
struct NNGBlockedSFSLoopSearcher : public NSatBlockStarterSearcher {
    NSatRegion* region;
        /**< The bounded saturated region, if the entire NNGBlockedSFSLoop
             structure has been successfully found; otherwise, 0 if we are
             still searching. */
    NMatrix2 matchingReln;
        /**< The matrix describing how the two boundary annuli of the
             saturated region are joined together.  This matrix expresses
             the fibre/base curves on one boundary annulus in terms of the
             fibre/base curves on the other, as described by
             NGraphLoop::matchingReln(). */

    /**
     * Creates a new searcher whose \a region pointer is null.
     */
    NNGBlockedSFSLoopSearcher() : region(0) {
    }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NNGBlockedSFSLoop::~NNGBlockedSFSLoop() {
    if (region_)
        delete region_;
}

NManifold* NNGBlockedSFSLoop::getManifold() const {
    NSFSpace* sfs = region_->createSFS(2, false);
    if (! sfs)
        return 0;

    sfs->reduce(false);

    return new NGraphLoop(sfs, matchingReln_);
}

std::ostream& NNGBlockedSFSLoop::writeName(std::ostream& out) const {
    // TODO: output
    return out << "Blocked SFS Loop";
}

std::ostream& NNGBlockedSFSLoop::writeTeXName(std::ostream& out) const {
    // TODO: output (tex)
    return out << "Blocked SFS Loop";
}

void NNGBlockedSFSLoop::writeTextLong(std::ostream& out) const {
    // TODO: output (detailed)
    out << "Blocked SFS Loop";
}

NNGBlockedSFSLoop* NNGBlockedSFSLoop::isNGBlockedSFSLoop(NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  Also watch for saturated tori being joined
    // to saturated Klein bottles.  Any of these issues will result in
    // edges joined to themselves in reverse.
    if (! tri->isValid())
        return 0;

    // Hunt for a starting block.
    NNGBlockedSFSLoopSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region) {
        // The expansion and self-adjacency worked, and the triangulation
        // is known to be closed and connected.
        // This means we've got one!
        return new NNGBlockedSFSLoop(searcher.region, searcher.matchingReln);
    }

    // Nope.
    return 0;
}

bool NNGBlockedSFSLoopSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointer should be null, but just in case...
    if (region) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // precisely two boundary annuli remaining.
    // Note that the starter block will now be owned by region.
    region = new NSatRegion(starter);
    region->expand(usedTets);

    if (region->numberOfBoundaryAnnuli() != 2) {
        delete region;
        region = 0;
        return true;
    }

    NSatBlock* bdryBlock[2];
    unsigned bdryAnnulus[2];
    bool bdryRefVert[2], bdryRefHoriz[2];
    region->boundaryAnnulus(0, bdryBlock[0], bdryAnnulus[0],
        bdryRefVert[0], bdryRefHoriz[0]);
    region->boundaryAnnulus(1, bdryBlock[1], bdryAnnulus[1],
        bdryRefVert[1], bdryRefHoriz[1]);

    // We either want two disjoint one-annulus torus boundaries, or else a
    // single two-annulus boundary that is pinched to turn each annulus into
    // a two-sided torus.  The following test will handle all cases.  We
    // don't worry about the degenerate case of fibres mapping to fibres
    // through the layering in the pinched case, since this will fail
    // our test anyway (either boundaries do not form tori, or they are
    // not two-sided).
    NSatAnnulus bdry0 = bdryBlock[0]->annulus(bdryAnnulus[0]);
    NSatAnnulus bdry1 = bdryBlock[1]->annulus(bdryAnnulus[1]);

    if (! (bdry0.isTwoSidedTorus() && bdry1.isTwoSidedTorus())) {
        delete region;
        region = 0;
        return true;
    }

    // Look for a layering on the first boundary annulus.
    // Extend the layering one tetrahedron at a time, to make sure we
    // don't loop back onto ourselves.
    NLayering layering(bdry0.tet[0], bdry0.roles[0],
        bdry0.tet[1], bdry0.roles[1]);

    NSatAnnulus layerTop;
    NMatrix2 layerToBdry1;
    while (true) {
        layerTop.tet[0] = layering.getNewBoundaryTet(0);
        layerTop.tet[1] = layering.getNewBoundaryTet(1);
        layerTop.roles[0] = layering.getNewBoundaryRoles(0);
        layerTop.roles[1] = layering.getNewBoundaryRoles(1);

        // Have we reached the second boundary?
        if (bdry1.isJoined(layerTop, layerToBdry1))
            break;

        // We haven't joined up yet.  Either extend or die.
        if (! layering.extendOne()) {
            // The layering dried up and we didn't make it.
            delete region;
            region = 0;
            return true;
        }

        if (usedTets.find(layering.getNewBoundaryTet(0)) !=
                usedTets.end() ||
                usedTets.find(layering.getNewBoundaryTet(1)) !=
                usedTets.end()) {
            // Gone too far -- we've looped back upon ourselves.
            delete region;
            region = 0;
            return true;
        }

        usedTets.insert(layering.getNewBoundaryTet(0));
        usedTets.insert(layering.getNewBoundaryTet(1));
    }

    // This is it!  Build the matching matrix and stop searching.

    // First find mappings from the fibre/base curves (fi, oi) to
    // annulus #i edges (first face: 01, first face: 02).
    // Note that each of these matrices is self-inverse.
    NMatrix2 curves0ToAnnulus0(bdryRefVert[0] ? 1 : -1, 0, 0,
        bdryRefHoriz[0] ? -1 : 1);
    NMatrix2 curves1ToAnnulus1(bdryRefVert[1] ? 1 : -1, 0, 0,
        bdryRefHoriz[1] ? -1 : 1);

    // Put it all together.
    // Remember that curves1ToAnnulus1 is self-inverse.
    matchingReln = curves1ToAnnulus1 * layerToBdry1 *
        layering.boundaryReln() * curves0ToAnnulus0;

    return false;
}

} // namespace regina

