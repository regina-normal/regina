
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "manifold/sfs.h"
#include "subcomplex/satblocktypes.h"
#include "subcomplex/satregion.h"
#include "triangulation/dim3.h"
#include <map>
#include <mutex>
#include <set>
#include <sstream>

namespace regina {

namespace {
    std::mutex startersMutex;

    /**
     * An anonymous inline boolean xor.  I'm always afraid to use ^ with
     * bool, since I'm never sure if this bitwise operator will do the
     * right thing on all platforms.
     *
     * (Update, 24/08/21: I'm pretty sure (bool != bool) does it. - Ben.)
     */
    inline bool regXor(bool a, bool b) {
        return ((a && ! b) || (b && ! a));
    }
}

std::list<SatBlockModel> SatRegion::starters_;

void SatBlockSpec::writeTextShort(std::ostream& out) const {
    block_->writeTextShort(out);
    if (refVert_) {
        if (refHoriz_)
            out << ", rotated";
        else
            out << ", reflected(V)";
    } else if (refHoriz_) {
        out << ", reflected(H)";
    }
}

void SatRegion::initStarters() {
    std::scoped_lock lock(startersMutex);
    if (starters_.empty()) {
        starters_.push_back(SatTriPrism::model(true));
        starters_.push_back(SatCube::model());

        // Try various reflector strips of small length.
        starters_.push_back(SatReflectorStrip::model(1, false));
        starters_.push_back(SatReflectorStrip::model(1, true));
        starters_.push_back(SatReflectorStrip::model(2, false));
        starters_.push_back(SatReflectorStrip::model(2, true));
        starters_.push_back(SatReflectorStrip::model(3, false));
        starters_.push_back(SatReflectorStrip::model(3, true));
        starters_.push_back(SatReflectorStrip::model(4, false));
        starters_.push_back(SatReflectorStrip::model(4, true));
    }
}

SatBlock* SatRegion::hasBlock(const SatAnnulus& annulus,
        SatBlock::TetList& avoidTets) {
    SatBlock* ans;

    // Run through the types of blocks that we know about.
    if ((ans = SatMobius::beginsRegion(annulus, avoidTets)))
        return ans;
    if ((ans = SatLST::beginsRegion(annulus, avoidTets)))
        return ans;
    if ((ans = SatTriPrism::beginsRegion(annulus, avoidTets)))
        return ans;
    if ((ans = SatCube::beginsRegion(annulus, avoidTets)))
        return ans;
    if ((ans = SatReflectorStrip::beginsRegion(annulus, avoidTets)))
        return ans;

    // As a last attempt, try a single layering.  We don't have to worry
    // about the degeneracy, since we'll never get a loop of these
    // things (since that would form a disconnected component, and we
    // never use one as a starting block).
    if ((ans = SatLayering::beginsRegion(annulus, avoidTets)))
        return ans;

    // Nothing was found.
    return nullptr;
}

SatRegion::SatRegion(SatBlock* starter) :
        baseEuler_(1),
        baseOrbl_(true),
        hasTwist_(false),
        twistsMatchOrientation_(true),
        shiftedAnnuli_(0),
        twistedBlocks_(0),
        nBdryAnnuli_(starter->countAnnuli()) {
    blocks_.push_back(SatBlockSpec(starter, false, false));

    if (starter->twistedBoundary()) {
        hasTwist_ = true;
        twistsMatchOrientation_ = false;
        twistedBlocks_ = 1;
    }
}

SatRegion::SatRegion(const SatRegion& src) :
        baseEuler_(src.baseEuler_),
        baseOrbl_(src.baseOrbl_),
        hasTwist_(src.hasTwist_),
        twistsMatchOrientation_(src.twistsMatchOrientation_),
        shiftedAnnuli_(src.shiftedAnnuli_),
        twistedBlocks_(src.twistedBlocks_),
        nBdryAnnuli_(src.nBdryAnnuli_)
        {
    // First clone the blocks, and keep a map from original blocks to
    // their clones.
    std::map<const SatBlock*, SatBlock*> clones;
    for (const SatBlockSpec& b : src.blocks_) {
        SatBlock* clone = b.block_->clone();
        clones.insert(std::make_pair(b.block_, clone));
        blocks_.push_back(SatBlockSpec(clone, b.refVert_, b.refHoriz_));
    }

    // Now fix the adjacencies in the cloned blocks.
    for (const SatBlockSpec& b : blocks_) {
        for (unsigned i = 0; i < b.block_->nAnnuli_; ++i)
            if (b.block_->adjBlock_[i]) {
                auto it = clones.find(b.block_->adjBlock_[i]);
                if (it == clones.end()) {
                    // This should *not* happen.
                    b.block_->adjBlock_[i] = nullptr;
                } else
                    b.block_->adjBlock_[i] = it->second;
            }
    }
}

std::tuple<const SatBlock*, unsigned, bool, bool>
        SatRegion::boundaryAnnulus(size_t which) const {
    for (const SatBlockSpec& b : blocks_)
        for (unsigned ann = 0; ann < b.block()->countAnnuli(); ++ann)
            if (! b.block()->hasAdjacentBlock(ann)) {
                if (which == 0)
                    return { b.block(), ann, b.refVert(), b.refHoriz() };
                --which;
            }

    throw InvalidArgument(
        "SatRegion::boundaryAnnulus(): Invalid boundary annulus index");
}

SFSpace SatRegion::createSFS(bool reflect) const {
    // Count boundary components.
    unsigned untwisted, twisted;
    countBoundaries(untwisted, twisted);

    // Go ahead and build the Seifert fibred space.
    SFSpace::ClassType baseClass;

    bool bdry = (twisted || untwisted || twistedBlocks_);
    if (baseOrbl_) {
        if (hasTwist_)
            baseClass = (bdry ? SFSpace::bo2 : SFSpace::o2);
        else
            baseClass = (bdry ? SFSpace::bo1 : SFSpace::o1);
    } else if (! hasTwist_)
        baseClass = (bdry ? SFSpace::bn1 : SFSpace::n1);
    else if (twistsMatchOrientation_)
        baseClass = (bdry ? SFSpace::bn2 : SFSpace::n2);
    else {
        // In the no-boundary case, we might not be able to distinguish
        // between n3 and n4.  Just call it n3 for now, and if we discover
        // it might have been n4 instead then we call it off and throw
        // an exception.
        baseClass = (bdry ? SFSpace::bn3 : SFSpace::n3);
    }

    // Recall that baseEuler_ assumes that each block contributes a plain
    // old disc to the base orbifold (and, in particular, it ignores any
    // reflector boundaries arising from twistedBlocks_).  This lets us
    // calculate genus just by looking at baseEuler_, orientability and
    // the number of punctures.

    SFSpace sfs(baseClass,
        (baseOrbl_ ? ((2 - baseEuler_) - twisted - untwisted) / 2 :
            ((2 - baseEuler_) - twisted - untwisted)),
        untwisted /* untwisted punctures */, twisted /* twisted punctures */,
        0 /* untwisted reflectors */, twistedBlocks_ /* twisted reflectors */);

    for (const auto& b : blocks_)
        b.block()->adjustSFS(sfs, ! regXor(reflect,
            regXor(b.refVert(), b.refHoriz())));

    if (shiftedAnnuli_)
        sfs.insertFibre(1, reflect ? -shiftedAnnuli_ : shiftedAnnuli_);

    if ((sfs.baseGenus() >= 3) &&
            (sfs.baseClass() == SFSpace::n3 ||
             sfs.baseClass() == SFSpace::n4)) {
        // Could still be either n3 or n4.
        // Shrug, give up.
        throw NotImplemented("SatRegion::createSFS() cannot yet "
            "distinguish between the closed non-orientable classes n3 and n4 "
            "for large base orbifold genus");
    }

    return sfs;
}

bool SatRegion::expand(SatBlock::TetList& avoidTets, bool stopIfIncomplete) {
    unsigned ann, adjAnn;
    size_t adjPos;
    bool currTwisted, currNor;
    unsigned annBdryTriangles;

    // Try to push past the boundary annuli of all blocks present and future.
    // We rely on a vector data type for blocks_ here, since this
    // will keep the loop doing exactly what it should do even if new
    // blocks are added and blockFound.size() increases.
    for (size_t pos = 0; pos < blocks_.size(); pos++) {
        const SatBlockSpec& currBlockSpec = blocks_[pos];

        // Keep a local copy of the contents of currBlockSpec, since
        // our additions to the blocks_ vector might cause reallocation
        // and therefore invalidate our reference.
        SatBlock* currBlock = currBlockSpec.block_;
        bool currVert = currBlockSpec.refVert();
        bool currHoriz = currBlockSpec.refHoriz();

        // Run through each boundary annulus for this block.
        for (ann = 0; ann < currBlock->countAnnuli(); ann++) {
            if (currBlock->hasAdjacentBlock(ann))
                continue;

            // Do we have one or two boundary triangles?
            annBdryTriangles = currBlock->annulus(ann).meetsBoundary();
            if (annBdryTriangles == 2) {
                // The annulus lies completely on the triangulation
                // boundary.  Just skip it.
                continue;
            } else if (annBdryTriangles == 1) {
                // The annulus lies half on the boundary.  No chance of
                // extending it from here, but we have no chance of
                // filling the entire triangulation.
                if (stopIfIncomplete)
                    return false;
                continue;
            }

            // We can happily jump to the other side, since we know
            // there are tetrahedra present.
            // Is there a new block there?
            if (SatBlock* adjBlock = hasBlock(
                    currBlock->annulus(ann).otherSide(), avoidTets)) {
                // We found a new adjacent block that we haven't seen before.

                // Note that, since the annuli are not horizontally
                // reflected, the blocks themselves will be.
                currBlock->setAdjacent(ann, adjBlock, 0, false, false);
                blocks_.push_back(SatBlockSpec(adjBlock, false, ! currHoriz));
                nBdryAnnuli_ = nBdryAnnuli_ + adjBlock->countAnnuli() - 2;

                // Note whether the new block has twisted boundary.
                if (adjBlock->twistedBoundary()) {
                    hasTwist_ = true;
                    twistsMatchOrientation_ = false;
                    twistedBlocks_++;
                }

                // On to the next annulus!
                continue;
            }

            // No adjacent block.
            // Perhaps it's joined to something we've already seen?
            // Only search forwards from this annulus.
            if (ann + 1 < currBlock->countAnnuli()) {
                adjPos = pos;
                adjAnn = ann + 1;
            } else {
                adjPos = pos + 1;
                adjAnn = 0;
            }
            while (adjPos < blocks_.size()) {
                SatBlock* adjBlock = blocks_[adjPos].block_;
                if (! adjBlock->hasAdjacentBlock(adjAnn)) {
                    auto [isAdj, adjVert, adjHoriz] = currBlock->annulus(ann).
                        isAdjacent(adjBlock->annulus(adjAnn));
                    if (isAdj) {
                        // They match!
                        currBlock->setAdjacent(ann, adjBlock, adjAnn,
                            adjVert, adjHoriz);
                        nBdryAnnuli_ -= 2;

                        // See what kinds of inconsistencies this
                        // rejoining has caused.
                        currNor = regXor(regXor(currHoriz,
                            blocks_[adjPos].refHoriz()), ! adjHoriz);
                        currTwisted = regXor(regXor(currVert,
                            blocks_[adjPos].refVert()), adjVert);

                        if (currNor)
                            baseOrbl_ = false;
                        if (currTwisted)
                            hasTwist_ = true;
                        if (regXor(currNor, currTwisted))
                            twistsMatchOrientation_ = false;

                        // See if we need to add a (1,1) shift before
                        // the annuli can be identified.
                        if (regXor(adjHoriz, adjVert)) {
                            if (regXor(currHoriz, currVert))
                                shiftedAnnuli_--;
                            else
                                shiftedAnnuli_++;
                        }

                        break;
                    }
                }

                if (adjAnn + 1 < adjBlock->countAnnuli())
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
            if (stopIfIncomplete)
                return false;
        }
    }

    // Well, we got as far as we got.
    calculateBaseEuler();
    return true;
}

long SatRegion::blockIndex(const SatBlock* block) const {
    std::vector<SatBlockSpec>::const_iterator it;
    size_t id;

    for (id = 0, it = blocks_.begin(); it != blocks_.end(); it++, id++)
        if (block == it->block())
            return id;

    return -1;
}

void SatRegion::calculateBaseEuler() {
    unsigned ann;

    long faces = blocks_.size();

    long edgesBdry = 0;
    long edgesInternalDoubled = 0;

    for (const SatBlockSpec& b : blocks_)
        for (ann = 0; ann < b.block()->countAnnuli(); ann++)
            if (b.block()->hasAdjacentBlock(ann))
                edgesInternalDoubled++;
            else
                edgesBdry++;

    // When counting vertices, don't just count unique edges in the
    // triangulation -- we could run into strife with edge identifications
    // outside the region.  Count the boundary vertices separately (this
    // is easy, since it's the same as the number of boundary edges).

    std::set<Edge<3>*> baseVerticesAll;
    std::set<Edge<3>*> baseVerticesBdry;
    SatAnnulus annData;

    for (const SatBlockSpec& b : blocks_)
        for (ann = 0; ann < b.block()->countAnnuli(); ann++) {
            annData = b.block()->annulus(ann);
            baseVerticesAll.insert(annData.tet[0]->edge(
                Edge<3>::edgeNumber[annData.roles[0][0]][annData.roles[0][1]]));

            if (! b.block()->hasAdjacentBlock(ann)) {
                baseVerticesBdry.insert(annData.tet[0]->edge(
                    Edge<3>::edgeNumber[annData.roles[0][0]][annData.roles[0][1]]));
                baseVerticesBdry.insert(annData.tet[1]->edge(
                    Edge<3>::edgeNumber[annData.roles[1][0]][annData.roles[1][1]]));
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

void SatRegion::writeBlockAbbrs(std::ostream& out, bool tex) const {
    // Here we temporarily sort the blocks, just for the purpose of output.
    // TODO: Possibly we could keep them permanently in sorted order,
    // but we should check that won't break anything else by forgetting
    // the order of insertion/creation.
    //
    // Note: creating this secondary array should be cheap, since the
    // number of blocks is typically small.
    std::vector<const SatBlockSpec*> sorted(blocks_.size());
    std::transform(blocks_.begin(), blocks_.end(), sorted.begin(),
            [](const SatBlockSpec& s) {
        return &s;
    });

    std::sort(sorted.begin(), sorted.end(),
            [](const SatBlockSpec* a, const SatBlockSpec* b) {
        return (*a->block()) < (*b->block());
    });

    bool first = true;
    for (const SatBlockSpec* b : sorted) {
        if (first)
            first = false;
        else
            out << ", ";
        b->block()->writeAbbr(out, tex);
    }
}

void SatRegion::writeDetail(std::ostream& out, const std::string& title)
        const {
    out << title << ":\n";

    std::vector<SatBlockSpec>::const_iterator it;
    size_t id, nAnnuli, ann;
    bool ref, back;

    out << "  Blocks:\n";
    for (id = 0, it = blocks_.begin(); it != blocks_.end(); it++, id++) {
        out << "    " << id << ". ";
        it->writeTextShort(out);
        nAnnuli = it->block()->countAnnuli();
        out << ", " << nAnnuli << (nAnnuli == 1 ? " annulus\n" : " annuli\n");
    }

    out << "  Adjacencies:\n";
    for (id = 0, it = blocks_.begin(); it != blocks_.end(); it++, id++)
        for (ann = 0; ann < it->block()->countAnnuli(); ann++) {
            out << "    " << id << '/' << ann << " --> ";
            if (! it->block()->hasAdjacentBlock(ann))
                out << "bdry";
            else {
                out << blockIndex(it->block()->adjacentBlock(ann)) << '/'
                    << it->block()->adjacentAnnulus(ann);
                ref = it->block()->adjacentReflected(ann);
                back = it->block()->adjacentBackwards(ann);
                if (ref || back) {
                    if (ref && back)
                        out << " (reflected, backwards)";
                    else if (ref)
                        out << " (reflected)";
                    else
                        out << " (backwards)";
                }
            }
            out << "\n";
        }
}

void SatRegion::writeTextShort(std::ostream& out) const {
    out << "[ ";
    bool first = true;
    for (const auto& b : blocks_) {
        if (first)
            first = false;
        else
            out << " | ";
        b.writeTextShort(out);
    }
    out << " ]";
}

void SatRegion::countBoundaries(unsigned& untwisted, unsigned& twisted) const {
    untwisted = twisted = 0;

    // Just trace around each boundary component in turn.
    // Note that we are guaranteed that blocks_ is non-empty.
    unsigned i, j;

    // Count annuli in each block, and work out how to index all annuli
    // from all blocks into a single monolithic array.
    auto* nAnnuli = new unsigned[blocks_.size()];
    auto* indexAnnuliFrom = new unsigned[blocks_.size()];
    for (i = 0; i < blocks_.size(); ++i) {
        nAnnuli[i] = blocks_[i].block()->countAnnuli();
        indexAnnuliFrom[i] = (i == 0 ? 0 : indexAnnuliFrom[i-1] + nAnnuli[i-1]);
    }
    unsigned totAnnuli = indexAnnuliFrom[blocks_.size() - 1] +
        nAnnuli[blocks_.size() - 1];

    // Prepare to keep track of which annuli we've processed.
    bool* used = new bool[totAnnuli];
    std::fill(used, used + totAnnuli, false);

    // Off we go!
    const SatBlock *currBlock;
    unsigned currBlockIndex, currAnnulus;
    bool hTwist, vTwist;
    for (i = 0; i < blocks_.size(); ++i) {
        const SatBlock* b = blocks_[i].block();
        for (j = 0; j < nAnnuli[i]; ++j) {
            // Here's our next annulus to examine.
            if (used[indexAnnuliFrom[i] + j]) {
                // Ignore: we've already processed this before.
                continue;
            }
            if (b->hasAdjacentBlock(j)) {
                // Ignore: this annulus is internal.
                used[indexAnnuliFrom[i] + j] = true;
                continue;
            }

            // This annulus is on the boundary, and not yet processed.
            // Run around the entire boundary component, marking annuli
            // as processed, and testing whether we close with a twist.
            currBlock = b;
            currBlockIndex = i;
            currAnnulus = j;
            hTwist = false;
            vTwist = false;

            while (true) {
                used[indexAnnuliFrom[currBlockIndex] + currAnnulus] = true;

                auto [tmpBlock, tmpAnnulus, tmpVTwist, tmpHTwist] =
                    currBlock->nextBoundaryAnnulus(currAnnulus, hTwist);
                if (tmpVTwist)
                    vTwist = ! vTwist;
                if (tmpHTwist)
                    hTwist = ! hTwist;
                currBlock = tmpBlock;
                currAnnulus = tmpAnnulus;
                // Gaa.  We need a block pointer -> index lookup.
                // Use a slow search for now.
                for (currBlockIndex = 0; currBlockIndex < blocks_.size();
                        ++currBlockIndex)
                    if (blocks_[currBlockIndex].block() == currBlock)
                        break;
                if (currBlockIndex >= blocks_.size()) {
                    std::cerr << "ERROR: Could not index current block."
                        << std::endl;
                }

                if (currBlock == b && currAnnulus == j)
                    break;
            }

            // See how the boundary component closed itself off.
            if (hTwist) {
                std::cerr
                    << "ERROR: Unexpected hTwist in boundary tracing."
                    << std::endl;
            }
            if (vTwist)
                ++twisted;
            else
                ++untwisted;
        }
    }

    //std::cout << "Region: " << twisted << " twisted, " << untwisted
    //    << " untwisted." << std::endl;

    delete[] nAnnuli;
    delete[] indexAnnuliFrom;
    delete[] used;
}

} // namespace regina

