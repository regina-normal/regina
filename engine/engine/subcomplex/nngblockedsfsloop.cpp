
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
 * TODO: Document this class.
 */
class NNGBlockedSFSLoopSearcher : public NSatBlockStarterSearcher {
    private:
        NSatRegion* region_;
        bool bdryRefVert_[2];
        bool bdryRefHoriz_[2];
        bool swapFaces_;
        NPerm facePerm_;

    public:
        NNGBlockedSFSLoopSearcher() {
            region_ = 0;
        }

        NSatRegion* region() {
            return region_;
        }

        bool bdryRefVert(unsigned which) const {
            return bdryRefVert_[which];
        }

        bool bdryRefHoriz(unsigned which) const {
            return bdryRefHoriz_[which];
        }

        bool swapFaces() const {
            return swapFaces_;
        }

        NPerm facePerm() const {
            return facePerm_;
        }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NNGBlockedSFSLoop::~NNGBlockedSFSLoop() {
    if (region_)
        delete region_;
}

NManifold* NNGBlockedSFSLoop::getManifold() const {
    NSFSpace::classType baseClass;

    // As with NBlockedSFS, we might not be able to distinguish between
    // n3 and n4.  Just call it n3 for now, and if we discover it might
    // have been n4 instead then we call it off and return 0.

    if (region_->baseOrientable())
        baseClass = (region_->hasTwist() ? NSFSpace::o2 : NSFSpace::o1);
    else if (! region_->hasTwist())
        baseClass = NSFSpace::n1;
    else if (region_->twistsMatchOrientation())
        baseClass = NSFSpace::n2;
    else
        baseClass = NSFSpace::n3;

    NSFSpace* sfs = new NSFSpace(baseClass,
        (region_->baseOrientable() ? (- region_->baseEuler()) / 2 :
            (- region_[0]->baseEuler())), 2, 0);

    region_->adjustSFS(*sfs, false);

    if ((sfs->getBaseGenus() >= 3) &&
            (sfs->getBaseClass() == NSFSpace::n3 ||
             sfs->getBaseClass() == NSFSpace::n4)) {
        delete sfs;
        return 0;
    }

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
    // curves are mapped.  TODO: Finish
    NMatrix2 ann0ToAnn1;
    if (swapFaces_) {
        // First and second faces are swapped.
        if      (facePerm_ == NPerm(0, 1, 2, 3))
            ann0ToAnn1 = NMatrix2(-1, 0, 0, -1);
        else if (facePerm_ == NPerm(1, 2, 0, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(2, 0, 1, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(0, 2, 1, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(1, 0, 2, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(2, 1, 0, 3))
            ann0ToAnn1 = NMatrix2();
    } else {
        // First and second faces are not swapped.
        if      (facePerm_ == NPerm(0, 1, 2, 3))
            ann0ToAnn1 = NMatrix2(1, 0, 0, 1);
        else if (facePerm_ == NPerm(1, 2, 0, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(2, 0, 1, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(0, 2, 1, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(1, 0, 2, 3))
            ann0ToAnn1 = NMatrix2();
        else if (facePerm_ == NPerm(2, 1, 0, 3))
            ann0ToAnn1 = NMatrix2();
    }

    // Remember that curves1ToAnnulus is self-inverse.
    return NNGSFSLoop(sfs, curves1ToAnnulus * ann0ToAnn1 * curves0ToAnnulus);
}

std::ostream& NNGBlockedSFSLoop::writeName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS Loop";
}

std::ostream& NNGBlockedSFSLoop::writeTeXName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS Loop";
}

void NNGBlockedSFSLoop::writeTextLong(std::ostream& out) const {
    // TODO, later
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
    if (searcher.region()) {
        // The expansion and self-adjacency worked, and the triangulation
        // is known to be closed and connected.
        // This means we've got one!
        return new NNGBlockedSFSLoop(searcher.region(),
            searcher.bdryRefVert(0), searcher.bdryRefHoriz(0),
            searcher.bdryRefVert(1), searcher.bdryRefHoriz(1),
            searcher.swapFaces(), searcher.facePerm());
    }

    // Nope.
    return 0;
}

bool NNGBlockedSFSLoopSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointer should be null, but just in case...
    if (region_) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // precisely two disjoint boundary annuli remaining.
    // Note that the starter block will now be owned by region_.
    region_ = new NSatRegion(starter);
    region_->expand(avoidTets);

    if (region_->numberOfBoundaryAnnuli() != 2) {
        delete region_;
        region_ = 0;
        return true;
    }

    NSatBlock* bdryBlock[2];
    unsigned bdryAnnulus[2];
    region_->boundaryAnnulus(0, bdryBlock[0], bdryAnnulus[0],
        bdryRefVert_[0], bdryRefHoriz_[0]);
    region_->boundaryAnnulus(1, bdryBlock[1], bdryAnnulus[1],
        bdryRefVert_[1], bdryRefHoriz_[1]);

    NSatBlock* tmpBlock;
    unsigned tmpAnnulus;
    bool tmpVert, tmpHoriz;

    bdryBlock[0]->nextBoundaryAnnulus(bdryAnnulus[0], tmpBlock, tmpAnnulus,
        tmpVert, tmpHoriz);
    if (tmpVert || tmpBlock != bdryBlock[0]) {
        delete region_;
        region_ = 0;
        return true;
    }

    bdryBlock[1]->nextBoundaryAnnulus(bdryAnnulus[1], tmpBlock, tmpAnnulus,
        tmpVert, tmpHoriz);
    if (tmpVert || tmpBlock != bdryBlock[1]) {
        delete region_;
        region_ = 0;
        return true;
    }

    // See whether the two boundary annuli are joined.
    NSatAnnulus bdry0 = bdryBlock[0]->annulus(bdryAnnulus[0]);
    NSatAnnulus bdry1 = bdryBlock[1]->annulus(bdryAnnulus[1]);

    if (bdry0.meetsBoundary() || bdry1.meetsBoundary()) {
        delete region_;
        region_ = 0;
        return true;
    }

    bdry1.switchSides();

    if (bdry1.tet[0] == bdry0.tet[0] && bdry1.tet[1] == bdry0.tet[1] &&
            bdry1.roles[0][3] == bdry0.roles[0][3] &&
            bdry1.roles[1][3] == bdry0.roles[1][3]) {
        // Could be one with first/second faces identified in the same order.

        // Construct the mapping of 0/1/2 markings from the first
        // annulus to the second.
        facePerm_ = bdry1.roles[0].inverse() * bdry0.roles[0];
        if (facePerm_ != bdry1.roles[1].inverse() * bdry0.roles[1]) {
            delete region_;
            region_ = 0;
            return true;
        }

        // This is it!  Stop searching.
        swapFaces_ = false;
        return false;
    }

    if (bdry1.tet[0] == bdry0.tet[0] && bdry1.tet[1] == bdry0.tet[1] &&
            bdry1.roles[0][3] == bdry0.roles[0][3] &&
            bdry1.roles[1][3] == bdry0.roles[1][3]) {
        // Could be one with first/second faces switched.

        // Construct the mapping of 0/1/2 markings from the first
        // annulus to the second.
        facePerm_ = bdry1.roles[1].inverse() * bdry0.roles[0];
        if (facePerm_ != bdry1.roles[0].inverse() * bdry0.roles[1]) {
            delete region_;
            region_ = 0;
            return true;
        }

        // This is it!  Stop searching.
        swapFaces_ = true;
        return false;
    }

    // Nah, it's nothing.
    delete region_;
    region_ = 0;
    return true;
}

} // namespace regina

