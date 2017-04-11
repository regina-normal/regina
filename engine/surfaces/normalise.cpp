
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

#include "triangulation/dim3.h"
#include "surfaces/nsvectoranstandard.h"

namespace regina {

/**
 * This file contains methods that "normalise" the various types of normal
 * surface classes. The most obvious of these is the removeOctagons() function,
 * which will create a new triangulation that contains an isotopy of this
 * normal surface which does not contain any octogons.
 */

// ------------------------------------------------------------------------
// Implementation of removeOctagons()
// ------------------------------------------------------------------------
Triangulation<3>* NormalSurface::removeOctagons() const {
    Triangulation<3>* ans = new Triangulation<3>(*triangulation_, false);
    unsigned long nTet = ans->size();

    if ((! systemAllowsAlmostNormal()) || (nTet == 0))
        return ans;

    for (long whichTet = 0; whichTet < static_cast<long>(nTet); whichTet++) {
        for (int i = 0; i < 3; ++i) {
            if (octs(whichTet, i) != 0)
                ans->zeroTwoMove(ans->tetrahedron(whichTet), i);
        }
    }
    return ans;
}

} // namespace regina
