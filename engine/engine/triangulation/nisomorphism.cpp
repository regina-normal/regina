
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/nisomorphism.h"

namespace regina {

void NIsomorphism::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between triangulations";
}

void NIsomorphism::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nTetrahedra; i++)
        out << i << " -> " << tetImage(i) << " (" << facePerm(i) << ")\n";
}

NIsomorphismDirect::NIsomorphismDirect(const NIsomorphism& cloneMe) :
        NIsomorphism(cloneMe.getSourceTetrahedra()),
        mFacePerm(cloneMe.getSourceTetrahedra()> 0 ?
            new NPerm[cloneMe.getSourceTetrahedra()] : 0) {
    for (unsigned i = 0; i < nTetrahedra; i++) {
        mTetImage[i] = cloneMe.tetImage(i);
        mFacePerm[i] = cloneMe.facePerm(i);
    }
}

NIsomorphismIndexed::NIsomorphismIndexed(const NIsomorphismIndexed& cloneMe) :
        NIsomorphism(cloneMe.nTetrahedra),
        mIndex(cloneMe.nTetrahedra > 0 ?
            new int[cloneMe.nTetrahedra] : 0) {
    for (unsigned i = 0; i < nTetrahedra; i++) {
        mTetImage[i] = cloneMe.mTetImage[i];
        mIndex[i] = cloneMe.mIndex[i];
    }
}

} // namespace regina

