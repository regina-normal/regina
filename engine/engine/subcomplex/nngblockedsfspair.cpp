
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

#include "manifold/nngsfspair.h"
#include "manifold/nsfs.h"
#include "subcomplex/nngblockedsfspair.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * TODO: Document NNGBlockedSFSPairSearcher.
 */
struct NNGBlockedSFSPairSearcher : public NSatBlockStarterSearcher {
    NSatRegion* region[2];
    bool horizontal;
    bool firstRegionReflected;

    NNGBlockedSFSPairSearcher() {
        region[0] = region[1] = 0;
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
    NSFSpace* sfs0 = region_[0]->createSFS(1, firstRegionReflected_);
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

    // Since both matching matrices are self-inverse, we can happily
    // switch the two Seifert fibred spaces without changing the overall
    // manifold.
    if (*sfs1 < *sfs0) {
        NSFSpace* tmp = sfs0;
        sfs0 = sfs1;
        sfs1 = tmp;
    }

    NNGSFSPair* ans;
    if (horizontal_)
        ans = new NNGSFSPair(sfs0, sfs1, 0, 1, 1, 0);
    else
        ans = new NNGSFSPair(sfs0, sfs1, 1, 1, 0, -1);

    return ans;
}

std::ostream& NNGBlockedSFSPair::writeName(std::ostream& out) const {
    // TODO: output
    return out << "Blocked SFS Pair";
}

std::ostream& NNGBlockedSFSPair::writeTeXName(std::ostream& out) const {
    // TODO: output (tex)
    return out << "Blocked SFS Pair";
}

void NNGBlockedSFSPair::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS pair (vertical <--> "
        << (horizontal_ ? "horizontal" : "diagonal") << ")\n";

    region_[0]->writeDetail(out, "First region");
    region_[1]->writeDetail(out, "Second region");
}

NNGBlockedSFSPair* NNGBlockedSFSPair::isNGBlockedSFSPair(NTriangulation* tri) {
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
    NNGBlockedSFSPairSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region[0]) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new NNGBlockedSFSPair(searcher.region[0], searcher.region[1],
            searcher.horizontal, searcher.firstRegionReflected);
    }

    // Nope.
    return 0;
}

bool NNGBlockedSFSPairSearcher::useStarterBlock(NSatBlock* starter) {
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

    firstRegionReflected =
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

    // Try expanding it with vertical fibres running both horizontally
    // and diagonally.
    NSatAnnulus bdry = bdryBlock->annulus(bdryAnnulus);

    if (bdry.meetsBoundary()) {
        delete region[0];
        region[0] = 0;
        return true;
    }

    bdry.switchSides();
    NSatAnnulus otherSideHoriz(bdry.tet[1], bdry.roles[1] * NPerm(1, 2),
        bdry.tet[0], bdry.roles[0] * NPerm(1, 2));
    NSatAnnulus otherSideDiag(bdry.tet[0], bdry.roles[0] * NPerm(0, 2),
        bdry.tet[1], bdry.roles[1] * NPerm(0, 2));

    // I fear we shall need to back up the tetrahedron list.  Sigh.
    NSatBlock::TetList usedTets2 = usedTets;

    NSatBlock* otherStarter;

    // Try horizontal:
    if ((otherStarter = NSatBlock::isBlock(otherSideHoriz, usedTets))) {
        region[1] = new NSatRegion(otherStarter);
        region[1]->expand(usedTets);

        if (region[1]->numberOfBoundaryAnnuli() == 1) {
            // This is it!  Stop searching.
            horizontal = true;
            return false;
        }

        // Nup, this one didn't work.
        delete region[1];
        region[1] = 0;
    }

    // Try diagonal:
    if ((otherStarter = NSatBlock::isBlock(otherSideDiag, usedTets2))) {
        region[1] = new NSatRegion(otherStarter);
        region[1]->expand(usedTets2);

        if (region[1]->numberOfBoundaryAnnuli() == 1) {
            // This is it!  Stop searching.
            // Switch the tetrahedron lists before we go.
            usedTets = usedTets2;
            horizontal = false;
            return false;
        }

        // Nup, not this one either.
        delete region[1];
        region[1] = 0;
    }

    // Nothing works.
    delete region[0];
    region[0] = 0;
    return true;
}

} // namespace regina

