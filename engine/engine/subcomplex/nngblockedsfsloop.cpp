
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

#include "manifold/nngsfsloop.h"
#include "manifold/nsfs.h"
#include "subcomplex/nngblockedsfsloop.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * A subclass of NSatBlockStarterSearcher that, upon finding a starter
 * block, attempts to flesh this out to an entire saturated region with
 * two identified torus boundaries, as described by the NNGBlockedSFSLoop
 * class.
 *
 * Wherever the member documentation refers to boundary annulus #0 and #1,
 * this corresponds to region->boundaryAnnulus(0, ...) and
 * region->boundaryAnnulus(1, ...) respectively.
 */
struct NNGBlockedSFSLoopSearcher : public NSatBlockStarterSearcher {
    NSatRegion* region;
        /**< The bounded saturated region, if the entire NNGBlockedSFSLoop
             structure has been successfully found; otherwise, 0 if we are
             still searching. */
    bool bdryRefVert[2];
        /**< Indicates for each boundary of the region whether the
             corresponding saturated block is vertically reflected within
             the region.  See NSatBlockSpec for details. */
    bool bdryRefHoriz[2];
        /**< Indicates for each boundary of the region whether the
             corresponding saturated block is horizontally reflected within
             the region.  See NSatBlockSpec for details. */
    bool swapFaces;
        /**< Contains details of how the two boundary annuli are identified.
             This is set to \c true if the first face of boundary annulus #0
             is identified with the second face of boundary annulus #1 (and
             vice versa), or \c false if the two first faces are joined
             together and the two second faces are joined together. */
    NPerm facePerm;
        /**< Contains details of how the two boundary annuli are identified.
             This permutation shows how the identification maps markings
             0/1/2 on boundary annulus #0 to markings 0/1/2 on boundary
             annulus #1. */

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

    // Prepare the matching matrix.

    // First find mappings from the fibre/base curves (fi, oi) to
    // annulus #i edges (first face: 10, first face: 02).
    // Note that each of these matrices is self-inverse.
    NMatrix2 curves0ToAnnulus(bdryRefVert_[0] ? -1 : 1, 0, 0,
        bdryRefHoriz_[0] ? -1 : 1);
    NMatrix2 curves1ToAnnulus(bdryRefVert_[1] ? -1 : 1, 0, 0,
        bdryRefHoriz_[1] ? -1 : 1);

    // Now we have the twelve cases of how the two sets of annulus
    // curves are mapped.
    NMatrix2 ann0ToAnn1;
    if (swapFaces_) {
        // First and second faces are swapped.
        if      (facePerm_ == NPerm(0, 1, 2, 3))
            ann0ToAnn1 = NMatrix2(-1, 0, 0, -1);
        else if (facePerm_ == NPerm(1, 2, 0, 3))
            ann0ToAnn1 = NMatrix2(0, -1, 1, 1);
        else if (facePerm_ == NPerm(2, 0, 1, 3))
            ann0ToAnn1 = NMatrix2(1, 1, -1, 0);
        else if (facePerm_ == NPerm(0, 2, 1, 3))
            ann0ToAnn1 = NMatrix2(0, 1, 1, 0);
        else if (facePerm_ == NPerm(1, 0, 2, 3))
            ann0ToAnn1 = NMatrix2(1, 0, -1, -1);
        else if (facePerm_ == NPerm(2, 1, 0, 3))
            ann0ToAnn1 = NMatrix2(-1, -1, 0, 1);
    } else {
        // First and second faces are not swapped.
        if      (facePerm_ == NPerm(0, 1, 2, 3))
            ann0ToAnn1 = NMatrix2(1, 0, 0, 1);
        else if (facePerm_ == NPerm(1, 2, 0, 3))
            ann0ToAnn1 = NMatrix2(0, 1, -1, -1);
        else if (facePerm_ == NPerm(2, 0, 1, 3))
            ann0ToAnn1 = NMatrix2(-1, -1, 1, 0);
        else if (facePerm_ == NPerm(0, 2, 1, 3))
            ann0ToAnn1 = NMatrix2(0, -1, -1, 0);
        else if (facePerm_ == NPerm(1, 0, 2, 3))
            ann0ToAnn1 = NMatrix2(-1, 0, 1, 1);
        else if (facePerm_ == NPerm(2, 1, 0, 3))
            ann0ToAnn1 = NMatrix2(1, 1, 0, -1);
    }

    // Remember that curves1ToAnnulus is self-inverse.
    return new NNGSFSLoop(sfs,
        curves1ToAnnulus * ann0ToAnn1 * curves0ToAnnulus);
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

    // TODO: Check on this.
    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  Also watch for annuli being joined to Klein
    // bottles and the like.  Any of these issues will result in edges
    // joined to themselves in reverse.
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
        return new NNGBlockedSFSLoop(searcher.region,
            searcher.bdryRefVert[0], searcher.bdryRefHoriz[0],
            searcher.bdryRefVert[1], searcher.bdryRefHoriz[1],
            searcher.swapFaces, searcher.facePerm);
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
    region->boundaryAnnulus(0, bdryBlock[0], bdryAnnulus[0],
        bdryRefVert[0], bdryRefHoriz[0]);
    region->boundaryAnnulus(1, bdryBlock[1], bdryAnnulus[1],
        bdryRefVert[1], bdryRefHoriz[1]);

    // We either want two disjoint one-annulus boundaries, or else a single
    // two-annulus boundary that is pinched to turn each annulus into a
    // two-sided torus.  The following test will handle all cases.  We
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

    // See whether the two boundary annuli are joined.
    if (bdry0.meetsBoundary() || bdry1.meetsBoundary()) {
        delete region;
        region = 0;
        return true;
    }

    bdry1.switchSides();

    if (bdry1.tet[0] == bdry0.tet[0] && bdry1.tet[1] == bdry0.tet[1] &&
            bdry1.roles[0][3] == bdry0.roles[0][3] &&
            bdry1.roles[1][3] == bdry0.roles[1][3]) {
        // Could be one with first/second faces identified in the same order.

        // Construct the mapping of 0/1/2 markings from the first
        // annulus to the second.
        facePerm = bdry1.roles[0].inverse() * bdry0.roles[0];
        if (facePerm != bdry1.roles[1].inverse() * bdry0.roles[1]) {
            delete region;
            region = 0;
            return true;
        }

        // This is it!  Stop searching.
        swapFaces = false;
        return false;
    }

    if (bdry1.tet[0] == bdry0.tet[1] && bdry1.tet[1] == bdry0.tet[0] &&
            bdry1.roles[0][3] == bdry0.roles[1][3] &&
            bdry1.roles[1][3] == bdry0.roles[0][3]) {
        // Could be one with first/second faces switched.

        // Construct the mapping of 0/1/2 markings from the first
        // annulus to the second.
        facePerm = bdry1.roles[1].inverse() * bdry0.roles[0];
        if (facePerm != bdry1.roles[0].inverse() * bdry0.roles[1]) {
            delete region;
            region = 0;
            return true;
        }

        // This is it!  Stop searching.
        swapFaces = true;
        return false;
    }

    // Nah, it's nothing.
    delete region;
    region = 0;
    return true;
}

} // namespace regina

