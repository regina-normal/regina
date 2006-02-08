
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

#include "subcomplex/nsatblock.h"

namespace regina {

NSatBlock::NSatBlock(const NSatBlock& cloneMe) : ShareableObject(),
        nAnnuli_(cloneMe.nAnnuli_),
        annulus_(new NSatAnnulus[cloneMe.nAnnuli_]),
        twistedBoundary_(cloneMe.twistedBoundary_),
        adjBlock_(new NSatBlock*[cloneMe.nAnnuli_]),
        adjAnnulus_(new unsigned[cloneMe.nAnnuli_]),
        adjReflected_(new bool[cloneMe.nAnnuli_]),
        adjBackwards_(new bool[cloneMe.nAnnuli_]) {
    for (unsigned i = 0; i < nAnnuli_; i++) {
        annulus_[i] = cloneMe.annulus_[i];
        adjBlock_[i] = cloneMe.adjBlock_[i];
        adjAnnulus_[i] = cloneMe.adjAnnulus_[i];
        adjReflected_[i] = cloneMe.adjReflected_[i];
        adjBackwards_[i] = cloneMe.adjBackwards_[i];
    }
}

void NSatBlock::transform(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) {
    for (unsigned i = 0; i < nAnnuli_; i++)
        annulus_[i].transform(originalTri, iso, newTri);
}

void NSatBlock::nextBoundaryAnnulus(unsigned thisAnnulus,
        NSatBlock*& nextBlock, unsigned& nextAnnulus, bool& refVert,
        bool& refHoriz) {
    // Don't worry about testing the precondition (this annulus has no
    // adjacency) -- things won't break even if it's false.

    nextBlock = this;
    nextAnnulus = (thisAnnulus + 1 == nAnnuli_ ? 0 : thisAnnulus + 1);
    refVert = refHoriz = false;

    unsigned tmp;
    while (nextBlock->hasAdjacentBlock(nextAnnulus)) {
        // Push through to the next block...
        if (nextBlock->adjReflected_[nextAnnulus])
            refVert = ! refVert;
        if (! nextBlock->adjBackwards_[nextAnnulus])
            refHoriz = ! refHoriz;

        tmp = nextBlock->adjAnnulus_[nextAnnulus];
        nextBlock = nextBlock->adjBlock_[nextAnnulus];
        nextAnnulus = tmp;

        if (refHoriz) {
            // ... and step to the previous annulus around.
            nextAnnulus = (nextAnnulus == 0 ? nextBlock->nAnnuli_ - 1 :
                nextAnnulus - 1);
        } else {
            // ... and step to the next annulus around.
            nextAnnulus = (nextAnnulus + 1 == nextBlock->nAnnuli_ ? 0 :
                nextAnnulus + 1);
        }
    }
}

bool NSatBlock::isBad(NTetrahedron* t, const TetList& list) {
    if (list.find(t) != list.end())
        return true;
    return false;
}

} // namespace regina

