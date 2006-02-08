
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

#include "subcomplex/nngblockedsfspair.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * TODO: Document this class.
 */
class NNGBlockedSFSPairSearcher : public NSatBlockStarterSearcher {
    private:
        NSatRegion* region_[2];
        bool horizontal_;

    public:
        NNGBlockedSFSPairSearcher() {
            region_[0] = region_[1] = 0;
        }

        NSatRegion* region(unsigned which) {
            return region_[which];
        }

        bool horizontal() {
            return horizontal_;
        }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NNGBlockedSFSPair::~NNGBlockedSFSPair() {
    if (region_[0])
        delete region_[0];
    if (region_[1])
        delete region_[1];
}

NManifold* NNGBlockedSFSPair::getManifold() const {
    // TODO, later
    return 0;
}

std::ostream& NNGBlockedSFSPair::writeName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS Pair";
}

std::ostream& NNGBlockedSFSPair::writeTeXName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS Pair";
}

void NNGBlockedSFSPair::writeTextLong(std::ostream& out) const {
    // TODO, later
    out << "Blocked SFS Pair";
}

NNGBlockedSFSPair* NNGBlockedSFSPair::isNGBlockedSFSPair(NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  These will result in edges joined to
    // themselves in reverse.
    if (! tri->isValid())
        return 0;

    // Hunt for a starting block.
    NNGBlockedSFSPairSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region(0)) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new NNGBlockedSFSPair(searcher.region(0), searcher.region(1),
            searcher.horizontal());
    }

    // Nope.
    return 0;
}

bool NNGBlockedSFSPairSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointers should be null, but just in case...
    if (region_[0] || region_[1]) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // just one boundary annulus remaining.
    // Note that the starter block will now be owned by region_[0].
    region_[0] = new NSatRegion(starter);
    region_[0]->expand(avoidTets);

    if (region_[0]->numberOfBoundaryAnnuli() != 1) {
        delete region_[0];
        region_[0] = 0;
        return true;
    }

    NSatAnnulus bdry = region_[0]->boundaryAnnulus(0);

    // Try expanding it with vertical fibres running both horizontally
    // and diagonally.
    if (bdry.meetsBoundary()) {
        delete region_[0];
        region_[0] = 0;
        return true;
    }

    bdry.switchSides();
    NSatAnnulus otherSideHoriz(bdry.tet[1], bdry.roles[1] * NPerm(1, 2),
        bdry.tet[0], bdry.roles[0] * NPerm(1, 2));
    NSatAnnulus otherSideDiag(bdry.tet[0], bdry.roles[0] * NPerm(0, 2),
        bdry.tet[1], bdry.roles[1] * NPerm(0, 2));

    // I fear we shall need to back up the tetrahedron list.  Sigh.
    NSatBlock::TetList avoidTets2 = avoidTets;

    NSatBlock* otherStarter;

    // Try horizontal:
    if ((otherStarter = NSatBlock::isBlock(otherSideHoriz, avoidTets))) {
        region_[1] = new NSatRegion(otherStarter);
        region_[1]->expand(avoidTets);

        if (region_[1]->numberOfBoundaryAnnuli() == 1) {
            // This is it!  Stop searching.
            return false;
        }

        // Nup, this one didn't work.
        delete region_[1];
        region_[1] = 0;
    }

    // Try diagonal:
    if ((otherStarter = NSatBlock::isBlock(otherSideDiag, avoidTets2))) {
        region_[1] = new NSatRegion(otherStarter);
        region_[1]->expand(avoidTets2);

        if (region_[1]->numberOfBoundaryAnnuli() == 1) {
            // This is it!  Stop searching.
            // Switch the tetrahedron lists before we go.
            avoidTets = avoidTets2;
            return false;
        }

        // Nup, not this one either.
        delete region_[1];
        region_[1] = 0;
    }

    // Nothing works.
    delete region_[0];
    region_[0] = 0;
    return true;
}

} // namespace regina

