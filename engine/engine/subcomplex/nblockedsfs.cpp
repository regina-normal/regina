
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

#include "manifold/nsfs.h"
#include "subcomplex/nblockedsfs.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"

namespace regina {

/**
 * TODO: Document NBlockedSFSSearcher.
 */
class NBlockedSFSSearcher : public NSatBlockStarterSearcher {
    private:
        NSatRegion* region_;

    public:
        NBlockedSFSSearcher() : region_(0) {
        }

        NSatRegion* region() {
            return region_;
        }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NBlockedSFS::~NBlockedSFS() {
    if (region_)
        delete region_;
}

NManifold* NBlockedSFS::getManifold() const {
    NSFSpace* ans = region_->createSFS(0, false);
    if (! ans)
        return 0;

    ans->reduce();

    // If we have SFS(RP2/n2) with one exceptional fibre, rewrite it as
    // SFS(S2) with three exceptional fibres.

    if (ans->getBaseClass() == NSFSpace::n2 &&
            ans->getBaseGenus() == 1 &&
            (! ans->isBaseOrientable()) &&
            ans->getBasePunctures() == 0 &&
            ans->getBaseReflectors() == 0 &&
            ans->getFibreCount() <= 1) {
        NSFSpace* altAns = new NSFSpace(NSFSpace::o1, 0, 0, 0);
        altAns->insertFibre(2, 1);
        altAns->insertFibre(2, -1);

        NSFSFibre rp2Fibre;
        if (ans->getFibreCount() == 0) {
            rp2Fibre.alpha = 1;
            rp2Fibre.beta = ans->getObstruction();
        } else {
            rp2Fibre = ans->getFibre(0);
            rp2Fibre.beta += rp2Fibre.alpha * ans->getObstruction();
        }

        // Make sure we're not going to try inserting (0,k).
        if (rp2Fibre.beta != 0) {
            altAns->insertFibre(rp2Fibre.beta, rp2Fibre.alpha);
            altAns->reduce();

            delete ans;
            return altAns;
        } else
            delete altAns;
    }

    return ans;
}

std::ostream& NBlockedSFS::writeName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS";
}

std::ostream& NBlockedSFS::writeTeXName(std::ostream& out) const {
    // TODO, later
    return out << "Blocked SFS";
}

void NBlockedSFS::writeTextLong(std::ostream& out) const {
    region_->writeDetail(out, "Blocked SFS");
}

NBlockedSFS* NBlockedSFS::isBlockedSFS(NTriangulation* tri) {
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
    NBlockedSFSSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region()) {
        // The region expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new NBlockedSFS(searcher.region());
    }

    // Nope.
    return 0;
}

bool NBlockedSFSSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointer should be null, but just in case...
    if (region_) {
        delete starter;
        return false;
    }

    // See if we can flesh out an entire triangulation component from
    // the starter block.  At this point the region will own the given
    // starter block.
    region_ = new NSatRegion(starter);
    if (! region_->expand(usedTets, true)) {
        // Nup.  Destroy the temporary structures and keep searching.
        delete region_;
        region_ = 0;
        return true;
    }

    // Got one!  Stop the search.
    return false;
}

} // namespace regina

