
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
#include "subcomplex/nsatblockstarter.h"
#include "subcomplex/nsatregion.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include <set>

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

NSatRegion::NSatRegion(NSatBlock* starter) :
        baseEuler_(1),
        baseOrbl_(true),
        hasTwist_(false),
        twistsMatchOrientation_(true),
        shiftedAnnuli_(0),
        extraReflectors_(0),
        nBdryAnnuli_(starter->nAnnuli()) {
    blocks_.push_back(NSatBlockSpec(starter, false, false));

    if (starter->twistedBoundary()) {
        hasTwist_ = true;
        twistsMatchOrientation_ = false;
        extraReflectors_ = 1;
    }
}

NSatRegion::~NSatRegion() {
    for (BlockSet::iterator it = blocks_.begin(); it != blocks_.end(); it++)
        delete it->block;
}

const NSatAnnulus& NSatRegion::boundaryAnnulus(unsigned long which) const {
    unsigned ann;
    for (BlockSet::const_iterator it = blocks_.begin(); it != blocks_.end();
            it++)
        for (ann = 0; ann < it->block->nAnnuli(); ann++)
            if (! it->block->hasAdjacentBlock(ann)) {
                if (which == 0)
                    return it->block->annulus(ann);

                which--;
            }

    // Given the precondition, we should never reach this point.
    // TODO: Return junk.
    return NSatAnnulus();
}

void NSatRegion::boundaryAnnulus(unsigned long which,
        NSatBlock*& block, unsigned& annulus) const {
    unsigned ann;
    for (BlockSet::const_iterator it = blocks_.begin(); it != blocks_.end();
            it++)
        for (ann = 0; ann < it->block->nAnnuli(); ann++)
            if (! it->block->hasAdjacentBlock(ann)) {
                if (which == 0) {
                    block = it->block;
                    annulus = ann;
                    return;
                }

                which--;
            }

    // Given the precondition, we should never reach this point.
}

void NSatRegion::adjustSFS(NSFSpace& sfs, bool reflect) const {
    sfs.addReflector(extraReflectors_);

    for (BlockSet::const_iterator it = blocks_.begin(); it != blocks_.end();
            it++)
        it->block->adjustSFS(sfs, regXor(reflect,
            regXor(it->refVert, it->refHoriz)));

    if (shiftedAnnuli_)
        sfs.insertFibre(1, shiftedAnnuli_);
}

bool NSatRegion::expand(NSatBlock::TetList& avoidTets, bool stopIfBounded) {
    NSatBlockSpec currBlockSpec;
    NSatBlock *currBlock, *adjBlock;
    unsigned ann, adjAnn;
    unsigned long adjPos;
    bool adjVert, adjHoriz;
    bool currTwisted, currNor;
    unsigned annBdryFaces;

    // Try to push past the boundary annuli of all blocks present and future.
    // We rely on a vector data type for BlockSet here, since this
    // will keep the loop doing exactly what it should do even if new
    // blocks are added and blockFound.size() increases.
    for (unsigned long pos = 0; pos < blocks_.size(); pos++) {
        currBlockSpec = blocks_[pos];
        currBlock = currBlockSpec.block;

        // Run through each boundary annulus for this block.
        for (ann = 0; ann < currBlock->nAnnuli(); ann++) {
            if (currBlock->hasAdjacentBlock(ann))
                continue;

            // Do we have one or two boundary faces?
            annBdryFaces = currBlock->annulus(ann).meetsBoundary();
            if (annBdryFaces == 2) {
                // The annulus lies completely on the triangulation
                // boundary.  Just skip it.
                continue;
            } else if (annBdryFaces == 1) {
                // The annulus lies half on the boundary.  No chance of
                // extending it from here, but we have no chance of
                // filling the entire triangulation.
                if (stopIfBounded)
                    return false;
                continue;
            }

            // We can happily jump to the other side, since we know
            // there are tetrahedra present.
            // Is there a new block there?
            if ((adjBlock = NSatBlock::isBlock(
                    currBlock->annulus(ann).otherSide(), avoidTets))) {
                // We found a new adjacent block that we haven't seen before.

                // Note that, since the annuli are not horizontally
                // reflected, the blocks themselves will be.
                currBlock->setAdjacent(ann, adjBlock, 0, false, false);
                blocks_.push_back(NSatBlockSpec(adjBlock, false,
                    ! currBlockSpec.refHoriz));
                nBdryAnnuli_ = nBdryAnnuli_ + adjBlock->nAnnuli() - 2;

                // Note whether the new block has twisted boundary.
                if (adjBlock->twistedBoundary()) {
                    hasTwist_ = true;
                    twistsMatchOrientation_ = false;
                    extraReflectors_++;
                }

                // On to the next annulus!
                continue;
            }

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
            while (adjPos < blocks_.size()) {
                adjBlock = blocks_[adjPos].block;
                if ((! adjBlock->hasAdjacentBlock(adjAnn)) &&
                        currBlock->annulus(ann).isAdjacent(
                        adjBlock->annulus(adjAnn), &adjVert, &adjHoriz)) {
                    // They match!
                    currBlock->setAdjacent(ann, adjBlock, adjAnn,
                        adjVert, adjHoriz);
                    nBdryAnnuli_ -= 2;

                    // See what kinds of inconsistencies this
                    // rejoining has caused.
                    currNor = regXor(regXor(currBlockSpec.refHoriz,
                        blocks_[adjPos].refHoriz), ! adjHoriz);
                    currTwisted = regXor(regXor(currBlockSpec.refVert,
                        blocks_[adjPos].refVert), adjVert);

                    if (currNor)
                        baseOrbl_ = false;
                    if (currTwisted)
                        hasTwist_ = true;
                    if (regXor(currNor, currTwisted))
                        twistsMatchOrientation_ = false;

                    // See if we need to add a (1,-1) shift before
                    // the annuli can be identified.
                    if (regXor(adjHoriz, adjVert)) {
                        if (regXor(currBlockSpec.refHoriz,
                                currBlockSpec.refVert))
                            shiftedAnnuli_++;
                        else
                            shiftedAnnuli_--;
                    }

                    break;
                }

                if (adjAnn + 1 < adjBlock->nAnnuli())
                    adjAnn++;
                else {
                    adjPos++;
                    adjAnn = 0;
                }
            }

            // If we found a match, we're done.  Move on to the next annulus.
            if (adjPos < blocks_.size())
                continue;

            // We couldn't match the annulus to anything.
            if (stopIfBounded)
                return false;
        }
    }

    // Well, we got as far as we got.
    calculateBaseEuler();
    return true;
}

void NSatRegion::calculateBaseEuler() {
    BlockSet::const_iterator it;
    unsigned ann;

    long faces = blocks_.size();

    long edgesBdry = 0;
    long edgesInternalDoubled = 0;

    for (it = blocks_.begin(); it != blocks_.end(); it++)
        for (ann = 0; ann < it->block->nAnnuli(); ann++)
            if (it->block->hasAdjacentBlock(ann))
                edgesInternalDoubled++;
            else
                edgesBdry++;

    // When counting vertices, don't just count unique edges in the
    // triangulation -- we could run into strife with edge identifications
    // outside the region.  Count the boundary vertices separately (this
    // is easy, since it's the same as the number of boundary edges).

    std::set<NEdge*> baseVerticesAll;
    std::set<NEdge*> baseVerticesBdry;
    NSatAnnulus annData;

    for (it = blocks_.begin(); it != blocks_.end(); it++)
        for (ann = 0; ann < it->block->nAnnuli(); ann++) {
            annData = it->block->annulus(ann);
            baseVerticesAll.insert(annData.tet[0]->getEdge(
                edgeNumber[annData.roles[0][0]][annData.roles[0][1]]));

            if (! it->block->hasAdjacentBlock(ann)) {
                baseVerticesBdry.insert(annData.tet[0]->getEdge(
                    edgeNumber[annData.roles[0][0]][annData.roles[0][1]]));
                baseVerticesBdry.insert(annData.tet[1]->getEdge(
                    edgeNumber[annData.roles[1][0]][annData.roles[1][1]]));
            }
        }

    // To summarise what was said above: the internal vertices are
    // guaranteed to give distinct elements in the baseVertices sets,
    // but the boundary vertices are not.  Thus we calculate internal
    // vertices via the sets, but boundary vertices via edgesBdry instead.

    long vertices = baseVerticesAll.size() - baseVerticesBdry.size()
        + edgesBdry;

    baseEuler_ = faces - edgesBdry - (edgesInternalDoubled / 2) + vertices;
}

void NSatRegion::writeTextShort(std::ostream& out) const {
    unsigned long size = blocks_.size();
    out << "Saturated region with " << size <<
        (size == 1 ? " block" : " blocks");
}

} // namespace regina

