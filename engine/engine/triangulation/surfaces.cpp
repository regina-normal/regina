
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "triangulation/ntriangulation.h"
#include "surfaces/nnormalsurfacelist.h"

namespace regina {

void NTriangulation::calculateSurfaceProperties() {
    // Check for 2-sphere boundary components.
    if (! calculatedZeroEfficient)
        for (BoundaryComponentIterator it = boundaryComponents.begin();
                it != boundaryComponents.end(); it++)
            if ((*it)->getEulerCharacteristic() == 2) {
                zeroEfficient = false;
                calculatedZeroEfficient = true;
                break;
            }

    if (calculatedZeroEfficient && calculatedSplittingSurface)
        return;

    // Create a normal surface list.
    NNormalSurfaceList surfaces(this, NNormalSurfaceList::STANDARD);

    if (! calculatedZeroEfficient)
        zeroEfficient = true;
    if (! calculatedSplittingSurface)
        splittingSurface = false;

    unsigned long nSurfaces = surfaces.getNumberOfSurfaces();
    NNormalSurface* s;
    NLargeInteger chi;
    for (unsigned long i = 0; i < nSurfaces; i++) {
        s = const_cast<NNormalSurface*>(surfaces.getSurface(i));

        if (! calculatedSplittingSurface)
            if (s->isSplitting()) {
                splittingSurface = true;
                calculatedSplittingSurface = true;
            }

        if (! calculatedZeroEfficient)
            if (! s->isVertexLinking()) {
                // No need to test for connectedness since these are
                // vertex normal surfaces.
                // No need to test for compactness since we're using
                // standard tri-quad coordinates.
                chi = s->getEulerCharacteristic();
                if (s->hasRealBoundary()) {
                    // Hunt for discs.
                    if (chi == 1) {
                        zeroEfficient = false;
                        calculatedZeroEfficient = true;
                    }
                } else {
                    // Hunt for spheres.
                    if (chi == 2) {
                        zeroEfficient = false;
                        calculatedZeroEfficient = true;
                    } else if (chi == 1 && s->isTwoSided() == -1) {
                        zeroEfficient = false;
                        calculatedZeroEfficient = true;
                    }
                }
            }

        // See if there is no use running through the rest of the list.
        if (calculatedZeroEfficient && calculatedSplittingSurface)
            break;
    }

    // Done!
    calculatedZeroEfficient = true;
    calculatedSplittingSurface = true;

    // Clean up.
    surfaces.makeOrphan();
}

} // namespace regina

