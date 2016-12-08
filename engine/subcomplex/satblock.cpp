
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "subcomplex/satblock.h"
#include <sstream>

namespace regina {

SatBlock::SatBlock(const SatBlock& cloneMe) : nAnnuli_(cloneMe.nAnnuli_),
        annulus_(new SatAnnulus[cloneMe.nAnnuli_]),
        twistedBoundary_(cloneMe.twistedBoundary_),
        adjBlock_(new SatBlock*[cloneMe.nAnnuli_]),
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

void SatBlock::transform(const Triangulation<3>* originalTri,
        const Isomorphism<3>* iso, Triangulation<3>* newTri) {
    for (unsigned i = 0; i < nAnnuli_; i++)
        annulus_[i].transform(originalTri, iso, newTri);
}

void SatBlock::nextBoundaryAnnulus(unsigned thisAnnulus,
        SatBlock*& nextBlock, unsigned& nextAnnulus, bool& refVert,
        bool& refHoriz, bool followPrev) {
    // Don't worry about testing the precondition (this annulus has no
    // adjacency) -- things won't break even if it's false.

    nextBlock = this;
    if (followPrev)
        nextAnnulus = (thisAnnulus == 0 ? nAnnuli_ - 1 : thisAnnulus - 1);
    else
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

        // ... and step to the previous/next annulus around.
        if (refHoriz == followPrev) {
            nextAnnulus = (nextAnnulus + 1 == nextBlock->nAnnuli_ ? 0 :
                nextAnnulus + 1);
        } else {
            nextAnnulus = (nextAnnulus == 0 ? nextBlock->nAnnuli_ - 1 :
                nextAnnulus - 1);
        }
    }
}

std::string SatBlock::abbr(bool tex) const {
    std::ostringstream s;
    writeAbbr(s, tex);
    return s.str();
}

bool SatBlock::isBad(Tetrahedron<3>* t, const TetList& list) {
    if (list.find(t) != list.end())
        return true;
    return false;
}

} // namespace regina

