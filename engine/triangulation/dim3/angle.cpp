
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

#include "angle/anglestructure.h"
#include "enumerate/treeconstraint.h"
#include "enumerate/treelp.h"
#include "triangulation/dim3.h"

namespace regina {

bool Triangulation<3>::knowsStrictAngleStructure() const {
    // There are some simple cases for which we can deduce the answer
    // automatically.
    if (simplices_.empty()) {
        strictAngleStructure_ = 0;
        return true;
    }

    if (! hasBoundaryTriangles()) {
        // It is easy to prove that, if an angle structure exists,
        // then we must have #edges = #tetrahedra.
        if (countEdges() != simplices_.size()) {
            strictAngleStructure_ = 0;
            return true;
        }
    }

    return strictAngleStructure_.known();
}

const AngleStructure* Triangulation<3>::findStrictAngleStructure() const {
    // The following test also catches any easy cases.
    if (knowsStrictAngleStructure())
        return strictAngleStructure_.value();

    LPInitialTableaux<LPConstraintNone> eqns(this, NS_ANGLE, false);

    LPData<LPConstraintNone, Integer> lp;
    lp.reserve(&eqns);

    // Find an initial basis.
    lp.initStart();

    // Set all angles to be strictly positive.
    unsigned i;
    for (i = 0; i < eqns.columns(); ++i)
        lp.constrainPositive(i);

    // Test for a solution!
    if (! lp.isFeasible())
        return (strictAngleStructure_ = 0);

    // We have a strict angle structure: reconstruct it.
    unsigned long len = 3 * simplices_.size() + 1;
    AngleStructureVector* v = new AngleStructureVector(len);
    lp.extractSolution(*v, 0 /* type vector */);
    return (strictAngleStructure_ = new AngleStructure(this, v));
}

} // namespace regina

