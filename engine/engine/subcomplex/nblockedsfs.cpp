
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
#include "subcomplex/nsatblocktypes.h"
#include <stack>

namespace regina {

namespace {
    /**
     * An anonymous inline boolean xor.  I'm always afraid to use ^ with
     * bool, since I'm never sure if this bitwise operator will do the
     * right thing on all platforms.
     */
    inline bool regXor(bool a, bool b) {
        return ((a && ! b) || (b && ! a));
    }
}

const NSatBlockStarterSet NSatBlockStarterSet::blocks;

void NSatBlockStarterSet::initialise() {
    // TODO: More types of starter block!
    NSatBlockStarter* starter;

    starter = new NSatBlockStarter;
    starter->block = NSatTriPrism::insertBlock(starter->triangulation, true);
    insert(starter);

    starter = new NSatBlockStarter;
    starter->block = NSatCube::insertBlock(starter->triangulation);
    insert(starter);
}

NBlockedSFS::~NBlockedSFS() {
    for (BlockSet::iterator it = blocks.begin(); it != blocks.end(); it++)
        delete it->block;
}

NManifold* NBlockedSFS::getManifold() const {
    NSFSpace::classType baseClass;
    if (baseOrbl)
        baseClass = (hasTwist ? NSFSpace::o2 : NSFSpace::o1);
    else if (! hasTwist)
        baseClass = NSFSpace::n1;
    else if (twistsMatchOrientation)
        baseClass = NSFSpace::n2;
    else {
        // TODO: Distinguish between n3 and n4!
        baseClass = NSFSpace::n3;
    }

    NSFSpace* ans = new NSFSpace(baseClass,
        (baseOrbl ? (2 - baseEuler) / 2 : (2 - baseEuler)),
        0 /* punctures */, 0 /* reflectors */);

    bool reflect;
    for (BlockSet::const_iterator it = blocks.begin(); it != blocks.end();
            it++) {
        reflect = (it->refVert && ! it->refHoriz) ||
            (it->refHoriz && ! it->refVert);
        it->block->adjustSFS(*ans, reflect);
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

    // Our triangulation is closed and connected.
    // Hunt for a starting block.
    unsigned long i;
    NSatBlockStarterSet::iterator it;
    std::list<NIsomorphism*> isos;
    std::list<NIsomorphism*>::iterator isoIt;
    NSatBlock::TetList avoidTets;
    NSatBlock* starter;
    NBlockedSFS* ans;
    for (it = NSatBlockStarterSet::begin(); it != NSatBlockStarterSet::end();
            it++) {
        // Look for this particular starting block.
        // Get trivialities out of the way first.
        if (tri->isOrientable() && ! (*it)->triangulation.isOrientable())
            continue;

        // Find all isomorphisms of the starter block within the given
        // triangulation.
        if (! (*it)->triangulation.findAllSubcomplexesIn(*tri, isos))
            continue;

        // Run through each isomorphism in the list and see if it leads
        // somewhere useful.
        //
        // All of the isomorphisms in this list _must_ be destroyed at
        // some point before we loop back to the next starter block.
        for (isoIt = isos.begin(); isoIt != isos.end(); isoIt++) {
            starter = (*it)->block->clone();
            starter->transform(&(*it)->triangulation, *isoIt, tri);

            // Create an initial blacklist of tetrahedra consisting of
            // those in the isomorphic image of the initial starting
            // block.
            for (i = 0; i < (*it)->triangulation.getNumberOfTetrahedra(); i++)
                avoidTets.push_back(tri->getTetrahedron((*isoIt)->tetImage(i)));

            // See if we can flesh out the entire triangulation from the
            // starter block.
            if (! (ans = hunt(starter, avoidTets))) {
                // Clean up our temporary structures, and also destroy the
                // isomorphism that didn't work.
                avoidTets.clear();
                delete starter;
                delete *isoIt;
                continue;
            }

            // We got one!
            // Before we return, delete the remaining isomorphisms that
            // we never even looked at.
            for (isoIt++; isoIt != isos.end(); isoIt++)
                delete *isoIt;

            return ans;
        }

        // Make sure the list is empty again for the next time around.
        isos.clear();
    }

    // Nothing found.
    return 0;
}

NBlockedSFS* NBlockedSFS::hunt(NSatBlock* starter,
        NSatBlock::TetList& avoidTets) {
    BlockSet blocksFound;
    blocksFound.push_back(NSatBlockSpec(starter, false, false));

    unsigned ann, adjAnn;
    unsigned long adjPos;
    bool adjVert, adjHoriz;
    NSatBlockSpec currBlockSpec;
    NSatBlock* currBlock;
    NSatBlock* adjBlock;

    bool baseOrbl = true;
    bool hasTwist = false;
    bool twistsMatchOrientation = true;
    bool currTwisted, currNor;

    for (unsigned long pos = 0; pos < blocksFound.size(); pos++) {
        currBlockSpec = blocksFound[pos];
        currBlock = currBlockSpec.block;
        for (ann = 0; ann < currBlock->nAnnuli(); ann++) {
            if (currBlock->hasAdjacentBlock(ann))
                continue;

            // Note that we can happily jump to the other side because
            // we know the triangulation is closed.
            if (! (adjBlock = NSatBlock::isBlock(
                    currBlock->annulus(ann).otherSide(), avoidTets))) {
                // No adjacent block.
                // Perhaps it's joined to something we've already seen?
                // Only search forwards from this annulus.
                if (ann + 1 < currBlock->nAnnuli()) {
                    adjPos = pos;
                    adjAnn = ann + 1;
                } else {
                    adjPos = pos + 1;
                    adjAnn = 0;
                }
                while (adjPos < blocksFound.size()) {
                    adjBlock = blocksFound[adjPos].block;
                    if ((! adjBlock->hasAdjacentBlock(adjAnn)) &&
                            currBlock->annulus(ann).isAdjacent(
                            adjBlock->annulus(adjAnn), &adjVert, &adjHoriz)) {
                        // They match!
                        currBlock->setAdjacent(ann, adjBlock, adjAnn,
                            adjHoriz, adjVert);

                        // See what kinds of inconsistencies this
                        // rejoining has caused.
                        currNor = regXor(regXor(currBlockSpec.refHoriz,
                            blocksFound[adjPos].refHoriz), ! adjHoriz);
                        currTwisted = regXor(regXor(currBlockSpec.refVert,
                            blocksFound[adjPos].refVert), adjVert);

                        if (currNor)
                            baseOrbl = false;
                        if (currTwisted)
                            hasTwist = true;
                        if (regXor(currNor, currTwisted))
                            twistsMatchOrientation = false;

                        break;
                    }

                    if (adjAnn + 1 < adjBlock->nAnnuli())
                        adjAnn++;
                    else {
                        adjPos++;
                        adjAnn = 0;
                    }
                }

                if (adjPos < blocksFound.size())
                    continue;

                // We couldn't match the annulus to anything.
                // Guess it's all over.

                // Destroy any new blocks that we added (but not starter),
                // and bail.
                for (BlockSet::iterator it = blocksFound.begin();
                        it != blocksFound.end(); it++)
                    if (it->block != starter)
                        delete it->block;

                return 0;
            }

            // We found a new adjacent block that we haven't seen before.

            // Note that, since the annuli are not horizontally
            // reflected, the blocks themselves will be.
            currBlock->setAdjacent(ann, adjBlock, 0, false, false);
            blocksFound.push_back(NSatBlockSpec(adjBlock, false,
                ! currBlockSpec.refHoriz));
        }
    }

    // We joined everything together!
    // Since it's known to be a connected closed triangulation, this
    // must be all!

    NBlockedSFS* ans = new NBlockedSFS();
    ans->blocks = blocksFound;
    ans->baseEuler = (baseOrbl ? 2 : 1);
        /* TODO: Work out the Euler characteristic! */
    ans->baseOrbl = baseOrbl;
    ans->hasTwist = hasTwist;
    ans->twistsMatchOrientation = twistsMatchOrientation;

    return ans;
}

} // namespace regina

