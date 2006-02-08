
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
 * TODO: Document this class.
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
    NSFSpace::classType baseClass;
    if (region_->baseOrientable())
        baseClass = (region_->hasTwist() ? NSFSpace::o2 : NSFSpace::o1);
    else if (! region_->hasTwist())
        baseClass = NSFSpace::n1;
    else if (region_->twistsMatchOrientation())
        baseClass = NSFSpace::n2;
    else {
        // It could be n3 or n4; if we work out later that we can't tell
        // them apart, we'll return 0.
        baseClass = NSFSpace::n3;
    }

    NSFSpace* ans = new NSFSpace(baseClass,
        (region_->baseOrientable() ? (2 - region_->baseEuler()) / 2 :
            (2 - region_->baseEuler())), 0, 0);

    region_->adjustSFS(*ans, false);

    // TODO: At the moment we cannot distinguish between n3 and n4.
    // If there is a possibility that we have n4 on our hands then
    // return 0.  Better no answer than a wrong answer. :/
    if ((ans->getBaseGenus() >= 3) &&
            (ans->getBaseClass() == NSFSpace::n3 ||
             ans->getBaseClass() == NSFSpace::n4)) {
        // Sigh.  All that work.
        delete ans;
        return 0;
    }

    ans->reduce();
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
    // TODO, later
    out << "Blocked SFS";
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
    if (! region_->expand(avoidTets, true)) {
        // Nup.  Destroy the temporary structures and keep searching.
        delete region_;
        region_ = 0;
        return true;
    }

    // Got one!  Stop the search.
    return false;
}

} // namespace regina

