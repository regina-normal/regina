
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

#include "manifold/nsnappeacensusmfd.h"
#include "subcomplex/nsnappeacensustri.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"

namespace regina {

const char NSnapPeaCensusManifold::SEC_5 = 'm';
const char NSnapPeaCensusManifold::SEC_6_OR = 's';
const char NSnapPeaCensusManifold::SEC_6_NOR = 'x';
const char NSnapPeaCensusManifold::SEC_7_OR = 'v';
const char NSnapPeaCensusManifold::SEC_7_NOR = 'y';

NTriangulation* NSnapPeaCensusManifold::construct() const {
    NTriangulation* ans = 0;

    if (section == SEC_5) {
        if (index == 0) {
            ans = NExampleTriangulation::gieseking();
            ans->setPacketLabel("");
        } else if (index == 1) {
            NTetrahedron* r = new NTetrahedron();
            NTetrahedron* s = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 1, 3, 2));
            r->joinTo(1, s, NPerm(2, 3, 1, 0));
            r->joinTo(2, s, NPerm(3, 2, 1, 0));
            r->joinTo(3, s, NPerm(1, 0, 3, 2));
            ans = new NTriangulation();
            ans->addTetrahedron(r);
            ans->addTetrahedron(s);
        } else if (index == 2) {
            NTetrahedron* r = new NTetrahedron();
            NTetrahedron* s = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 1, 3, 2));
            r->joinTo(1, s, NPerm(3, 1, 2, 0));
            r->joinTo(2, s, NPerm(2, 1, 3, 0));
            r->joinTo(3, s, NPerm(3, 1, 0, 2));
            ans = new NTriangulation();
            ans->addTetrahedron(r);
            ans->addTetrahedron(s);
        } else if (index == 3) {
            NTetrahedron* r = new NTetrahedron();
            NTetrahedron* s = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 1, 3, 2));
            r->joinTo(1, s, NPerm(2, 1, 0, 3));
            r->joinTo(2, s, NPerm(0, 3, 2, 1));
            r->joinTo(3, s, NPerm(1, 0, 2, 3));
            ans = new NTriangulation();
            ans->addTetrahedron(r);
            ans->addTetrahedron(s);
        } else if (index == 4) {
            ans = NExampleTriangulation::figureEightKnotComplement();
            ans->setPacketLabel("");
        }
    }

    return ans;
}

NAbelianGroup* NSnapPeaCensusManifold::getHomologyH1() const {
    return NSnapPeaCensusTri(section, index).getHomologyH1();
}

std::ostream& NSnapPeaCensusManifold::writeName(std::ostream& out) const {
    // Some manifolds will get special names, and will have their usual
    // SnapPea names written in writeStructure() instead.
    if (section == SEC_5) {
        if (index == 0)
            return out << "Gieseking manifold";
        if (index == 4)
            return out << "Figure eight knot complement";
        if (index == 129)
            return out << "Whitehead link complement";
    }

    // No special names, just the usual SnapPea notation.
    return NSnapPeaCensusTri(section, index).writeName(out);
}

std::ostream& NSnapPeaCensusManifold::writeTeXName(std::ostream& out) const {
    return NSnapPeaCensusTri(section, index).writeTeXName(out);
}

std::ostream& NSnapPeaCensusManifold::writeStructure(std::ostream& out) const {
    // If we didn't give the usual SnapPea name in writeName(), give it here.
    if (section == SEC_5) {
        if (index == 0 || index == 4 || index == 129)
            return NSnapPeaCensusTri(section, index).writeName(out);
    }

    return out;
}

} // namespace regina

