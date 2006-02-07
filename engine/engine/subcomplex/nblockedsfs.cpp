
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

    // Our triangulation is closed and connected.
    // Hunt for a starting block.
    unsigned long i;
    NSatBlockStarterSet::iterator it;
    std::list<NIsomorphism*> isos;
    std::list<NIsomorphism*>::iterator isoIt;
    NSatBlock::TetList avoidTets;
    NSatBlock* starter;
    NSatRegion* region;
    for (it = NSatBlockStarterSet::begin(); it != NSatBlockStarterSet::end();
            it++) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! (*it)->triangulation().isOrientable())
            continue;
        if (tri->getNumberOfTetrahedra() <
                (*it)->triangulation().getNumberOfTetrahedra())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        if (! (*it)->triangulation().findAllSubcomplexesIn(*tri, isos))
            continue;

        // Run through each isomorphism in the list and see if it leads
        // somewhere useful.
        //
        // All of the isomorphisms in this list _must_ be destroyed at
        // some point before we loop back to the next starter block.
        for (isoIt = isos.begin(); isoIt != isos.end(); isoIt++) {
            starter = (*it)->block()->clone();
            starter->transform(&(*it)->triangulation(), *isoIt, tri);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting
            // block.
            for (i = 0; i < (*it)->triangulation().getNumberOfTetrahedra(); i++)
                avoidTets.insert(tri->getTetrahedron((*isoIt)->tetImage(i)));

            // See if we can flesh out the entire triangulation from the
            // starter block.
            region = new NSatRegion(starter);
            if (! region->expand(avoidTets, true)) {
                // Clean up our temporary structures, and also destroy the
                // isomorphism that didn't work.
                avoidTets.clear();
                delete region;
                delete *isoIt;
                continue;
            }

            // Since expand() worked and the triangulation is known to
            // be closed and connected, we've got one!

            // Before we return, delete the remaining isomorphisms that
            // we never even looked at.
            for (isoIt++; isoIt != isos.end(); isoIt++)
                delete *isoIt;

            return new NBlockedSFS(region);
        }

        // Make sure the list is empty again for the next time around.
        isos.clear();
    }

    // Nothing found.
    return 0;
}

} // namespace regina

