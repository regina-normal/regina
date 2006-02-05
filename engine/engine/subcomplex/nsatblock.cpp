
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

bool NSatBlock::isBad(NTetrahedron* t, const TetList& list) {
    for (TetList::const_iterator it = list.begin(); it != list.end(); it++)
        if (*it == t)
            return true;
    return false;
}

bool NSatBlock::isBad(NTetrahedron* t, const TetList& list1,
        const TetList& list2) {
    TetList::const_iterator it;

    for (it = list1.begin(); it != list1.end(); it++)
        if (*it == t)
            return true;
    for (it = list2.begin(); it != list2.end(); it++)
        if (*it == t)
            return true;

    return false;
}

} // namespace regina

