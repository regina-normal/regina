
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "manifold/ngraphtriple.h"
#include "manifold/nsfs.h"
#include "subcomplex/nblockedsfstriple.h"
#include "subcomplex/nlayering.h"
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"
#include <memory>

namespace regina {

/**
 * A subclass of NSatBlockStarterSearcher that, upon finding a starter
 * block, attempts to flesh this out to a group of three saturated regions
 * joined along their torus boundaries, as desribed by the
 * NBlockedSFSTriple class.
 *
 * The starter block will be assumed to belong to the central region (not
 * one of the end regions).
 */
struct NBlockedSFSTripleSearcher : public NSatBlockStarterSearcher {
    NSatRegion* end[2];
        /**< The two end regions of the NBlockedSFSTriple structure,
             if such a structure has been successfully found; otherwise,
             two null pointers if we are still searching. */
    NSatRegion* centre;
        /**< The central region of the NBlockedSFSTriple structure,
             if such a structure has been successfully found; otherwise,
             a null pointer if we are still searching. */
    NMatrix2 matchingReln[2];
        /**< The matrices describing how the various region boundaries are
             joined together.  Here matrix \a matchingReln[i] expresses the
             fibre/base curves on region \a end[i] in terms of the fibre/base
             curves on the corresponding central region boundary.  See
             NBlockedSFSTriple::matchingReln() for further details. */

    /**
     * Creates a new searcher whose \a end and \a centre region pointers
     * are all null.
     */
    NBlockedSFSTripleSearcher() {
        end[0] = end[1] = centre = 0;
    }

    protected:
        bool useStarterBlock(NSatBlock* starter);
};

NBlockedSFSTriple::~NBlockedSFSTriple() {
    if (end_[0])
        delete end_[0];
    if (end_[1])
        delete end_[1];
    if (centre_)
        delete centre_;
}

NManifold* NBlockedSFSTriple::getManifold() const {
    NSFSpace* end0 = end_[0]->createSFS(1, false);
    if (! end0)
        return 0;

    NSFSpace* end1 = end_[1]->createSFS(1, false);
    if (! end1) {
        delete end0;
        return 0;
    }

    NSFSpace* hub = centre_->createSFS(2, false);
    if (! hub) {
        delete end0;
        delete end1;
        return 0;
    }

    // Reduce the Seifert fibred space representations and finish up.
    end0->reduce(false);
    end1->reduce(false);
    hub->reduce(false);

    return new NGraphTriple(end0, hub, end1,
        matchingReln_[0], matchingReln_[1]);
}

std::ostream& NBlockedSFSTriple::writeName(std::ostream& out) const {
    out << "Blocked SFS Triple [";
    end_[0]->writeBlockAbbrs(out, false);
    out << " | ";
    centre_->writeBlockAbbrs(out, false);
    out << " | ";
    end_[1]->writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& NBlockedSFSTriple::writeTeXName(std::ostream& out) const {
    out << "\\mathrm{BSFS\\_Triple}\\left[";
    end_[0]->writeBlockAbbrs(out, true);
    out << "\\,|\\,";
    centre_->writeBlockAbbrs(out, true);
    out << "\\,|\\,";
    end_[1]->writeBlockAbbrs(out, true);
    return out << "\\right]";
}

void NBlockedSFSTriple::writeTextLong(std::ostream& out) const {
    out << "Blocked SFS triple\n";
    out << "Matching relation (centre -> end #1): " << matchingReln_[0] << '\n';
    out << "Matching relation (centre -> end #2): " << matchingReln_[1] << '\n';

    centre_->writeDetail(out, "Central region");
    end_[0]->writeDetail(out, "First end region");
    end_[1]->writeDetail(out, "Second end region");
}

NBlockedSFSTriple* NBlockedSFSTriple::isBlockedSFSTriple(
        NTriangulation* tri) {
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
    NBlockedSFSTripleSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.centre) {
        // The full expansion worked, and the triangulation is known
        // to be closed and connected.
        // This means we've got one!
        return new NBlockedSFSTriple(searcher.end[0], searcher.centre,
            searcher.end[1], searcher.matchingReln[0],
            searcher.matchingReln[1]);
    }

    // Nope.
    return 0;
}

bool NBlockedSFSTripleSearcher::useStarterBlock(NSatBlock* starter) {
    // The region pointers should be null, but just in case...
    if (end[0] || end[1] || centre) {
        delete starter;
        return false;
    }

    // Flesh out the triangulation as far as we can.  We're aiming for
    // precisely two disjoint boundary annuli remaining.
    // Note that the starter block will now be owned by centre.
    centre = new NSatRegion(starter);
    centre->expand(usedTets);

    if (centre->numberOfBoundaryAnnuli() != 2) {
        delete centre;
        centre = 0;
        return true;
    }

    // Insist on the boundary annuli being disjoint and untwisted.
    NSatBlock* bdryBlock[2];
    unsigned bdryAnnulus[2];
    bool bdryVert[2], bdryHoriz[2], bdryRef[2];

    centre->boundaryAnnulus(0, bdryBlock[0], bdryAnnulus[0],
        bdryVert[0], bdryHoriz[0]);
    centre->boundaryAnnulus(1, bdryBlock[1], bdryAnnulus[1],
        bdryVert[1], bdryHoriz[1]);

    bdryRef[0] =
        ((bdryVert[0] && ! bdryHoriz[0]) || (bdryHoriz[0] && ! bdryVert[0]));
    bdryRef[1] =
        ((bdryVert[1] && ! bdryHoriz[1]) || (bdryHoriz[1] && ! bdryVert[1]));

    // We either want two disjoint one-annulus boundaries, or else a
    // single two-annulus boundary that is pinched to turn each annulus
    // into a two-sided torus.  The following test handles all cases.
    NSatAnnulus bdry[2];
    bdry[0] = bdryBlock[0]->annulus(bdryAnnulus[0]);
    bdry[1] = bdryBlock[1]->annulus(bdryAnnulus[1]);

    if (! (bdry[0].isTwoSidedTorus() && bdry[1].isTwoSidedTorus())) {
        delete centre;
        centre = 0;
        return true;
    }

    // Hunt for layerings, but gently gently -- we don't want to loop
    // from one boundary back onto the other.
    std::auto_ptr<NLayering> layering[2];

    int e;
    for (e = 0; e < 2; e++) {
        layering[e].reset(new NLayering(bdry[e].tet[0], bdry[e].roles[0],
            bdry[e].tet[1], bdry[e].roles[1]));

        while (layering[e]->extendOne()) {
            if (usedTets.find(layering[e]->getNewBoundaryTet(0)) !=
                    usedTets.end() ||
                    usedTets.find(layering[e]->getNewBoundaryTet(1)) !=
                    usedTets.end()) {
                // Oops, we've run back into something we've already seen.
                delete centre;
                centre = 0;
                return true;
            }
            usedTets.insert(layering[e]->getNewBoundaryTet(0));
            usedTets.insert(layering[e]->getNewBoundaryTet(1));
        }
    }

    // Start looking for the end regions.
    int plugPos;
    NSatBlock* otherStarter;
    NMatrix2 curvesCentreToLayering, layeringToEndAnnulus;

    for (e = 0; e < 2; e++) {
        // Relation from centre fibre/orbifold to layering first face
        // markings 01/02:
        curvesCentreToLayering = layering[e]->boundaryReln() *
            NMatrix2(-1, 0, 0, bdryRef[e] ? -1 : 1);

        // We make the shell of an other-side boundary annulus; we will fill
        // in the precise vertex role permutations later on.
        NSatAnnulus otherSide(layering[e]->getNewBoundaryTet(0), NPerm4(),
            layering[e]->getNewBoundaryTet(1), NPerm4());

        if (otherSide.meetsBoundary()) {
            delete centre;
            centre = 0;
            if (e == 1) {
                delete end[0];
                end[0] = 0;
            }
            return true;
        }

        // Try the three possible orientations for fibres on the other side.
        for (plugPos = 0; plugPos < 3; plugPos++) {
            // Construct the boundary annulus for the end region.
            // Refresh the tetrahedra as well as the vertex roles, since
            // it may have switched sides since our last run through the loop.
            otherSide.tet[0] = layering[e]->getNewBoundaryTet(0);
            otherSide.tet[1] = layering[e]->getNewBoundaryTet(1);

            // In each case, also fill in the mapping from (layering first
            // face markings 01/02) to (other side annulus first face
            // markings 01/02).  This is stored in layeringToEndAnnulus.
            if (plugPos == 0) {
                otherSide.roles[0] = layering[e]->getNewBoundaryRoles(0);
                otherSide.roles[1] = layering[e]->getNewBoundaryRoles(1);
                layeringToEndAnnulus = NMatrix2(1, 0, 0, 1);
            } else if (plugPos == 1) {
                otherSide.roles[0] = layering[e]->getNewBoundaryRoles(0) *
                    NPerm4(1, 2, 0, 3);
                otherSide.roles[1] = layering[e]->getNewBoundaryRoles(1) *
                    NPerm4(1, 2, 0, 3);
                layeringToEndAnnulus = NMatrix2(-1, 1, -1, 0);
            } else {
                otherSide.roles[0] = layering[e]->getNewBoundaryRoles(0) *
                    NPerm4(2, 0, 1, 3);
                otherSide.roles[1] = layering[e]->getNewBoundaryRoles(1) *
                    NPerm4(2, 0, 1, 3);
                layeringToEndAnnulus = NMatrix2(0, -1, 1, -1);
            }

            // Clear out the used tetrahedron list.  Everything between the
            // two layering boundaries is self-contained, so we won't run
            // into any of it again on the other side.  We'll just re-insert
            // the layering boundary tetrahedra.
            usedTets.clear();
            usedTets.insert(layering[0]->getNewBoundaryTet(0));
            usedTets.insert(layering[0]->getNewBoundaryTet(1));
            usedTets.insert(layering[1]->getNewBoundaryTet(0));
            usedTets.insert(layering[1]->getNewBoundaryTet(1));

            // See if we can flesh the other side out to an entire region.
            otherSide.switchSides();

            if ((otherStarter = NSatBlock::isBlock(otherSide, usedTets))) {
                end[e] = new NSatRegion(otherStarter);
                end[e]->expand(usedTets);

                if (end[e]->numberOfBoundaryAnnuli() == 1) {
                    // Got it!
                    // Do a final conversion from annulus first face markings
                    // 01/02 and move onto the next end space.
                    matchingReln[e] = NMatrix2(-1, 0, 0, 1) *
                        layeringToEndAnnulus * curvesCentreToLayering;
                    break;
                }

                // Nup, this one didn't work.
                delete end[e];
                end[e] = 0;
            }
        }

        // Did we manage to fill in this end space?
        if (! end[e]) {
            // Nope.  Keep searching.
            delete centre;
            centre = 0;

            if (e == 1) {
                delete end[0];
                end[0] = 0;
            }

            return true;
        }
    }

    // w00t!  It all worked out.
    // Stop searching, we're done.
    return false;
}

} // namespace regina

