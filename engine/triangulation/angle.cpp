
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include "angle/nanglestructure.h"
#include "enumerate/ntreeconstraint.h"
#include "enumerate/ntreelp.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NAngleStructure* NTriangulation::hasStrictAngleStructure() {
    // Knock off the empty triangulation first.
    if (tetrahedra.empty())
        return 0;

    LPInitialTableaux<LPConstraintNone> eqns(this, NS_ANGLE, false);

    LPData<LPConstraintNone, NInteger> lp;
    lp.reserve(&eqns);

    // Find an initial basis.
    lp.initStart();

    // Set all angles to be strictly positive.
    unsigned i;
    for (i = 0; i < eqns.columns(); ++i)
        lp.constrainPositive(i);

    // Test for a solution!
    if (! lp.isFeasible())
        return 0;

    // We have a strict angle structure: reconstruct it.
    unsigned long len = 3 * tetrahedra.size() + 1;
    NAngleStructureVector* v = new NAngleStructureVector(len);
    lp.extractSolution(*v, 0 /* type vector */);
    return new NAngleStructure(this, v);
}

} // namespace regina

