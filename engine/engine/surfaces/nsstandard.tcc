
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/* To be included from nsstandard.h. */

#include "maths/nvectorunit.h"

namespace regina {

template <class RayOutputIterator, class FaceOutputIterator>
void NNormalSurfaceVectorStandard::createNonNegativeCone(
        NTriangulation* triangulation,
        RayOutputIterator rays, FaceOutputIterator faces) {
    unsigned long nCoords = 7 * triangulation->getNumberOfTetrahedra();

    NNormalSurfaceVector* vector;
    for (unsigned long i=0; i<nCoords; i++) {
        vector = new NNormalSurfaceVectorStandard(nCoords);
        vector->setElement(i, NLargeInteger::one);
        *rays++ = vector;

        *faces++ = new NVectorUnit<NLargeInteger>(nCoords, i);
    }
}

} // namespace regina

