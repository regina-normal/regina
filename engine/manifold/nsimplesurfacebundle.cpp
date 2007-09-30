
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "algebra/nabeliangroup.h"
#include "manifold/nsimplesurfacebundle.h"
#include "triangulation/ntriangulation.h"

namespace regina {

const int NSimpleSurfaceBundle::S2xS1 = 1;
const int NSimpleSurfaceBundle::S2xS1_TWISTED = 2;
const int NSimpleSurfaceBundle::RP2xS1 = 3;

NTriangulation* NSimpleSurfaceBundle::construct() const {
    NTriangulation* ans = new NTriangulation();

    if (type == S2xS1) {
        ans->insertLayeredLensSpace(0, 1);
    } else if (type == S2xS1_TWISTED) {
        // Taken from section 3.5.1 of Ben Burton's PhD thesis.
        NTetrahedron* r = new NTetrahedron();
        NTetrahedron* s = new NTetrahedron();

        r->joinTo(1, s, NPerm());
        r->joinTo(3, s, NPerm());
        r->joinTo(2, s, NPerm(3, 2, 0, 1));
        s->joinTo(2, r, NPerm(3, 2, 0, 1));

        ans->addTetrahedron(r);
        ans->addTetrahedron(s);
    } else if (type == RP2xS1) {
        // Taken from section 3.5.1 of Ben Burton's PhD thesis.
        NTetrahedron* r = new NTetrahedron();
        NTetrahedron* s = new NTetrahedron();
        NTetrahedron* t = new NTetrahedron();

        s->joinTo(0, r, NPerm(0, 1, 2, 3));
        s->joinTo(3, r, NPerm(3, 0, 1, 2));
        s->joinTo(1, t, NPerm(3, 0, 1, 2));
        s->joinTo(2, t, NPerm(0, 1, 2, 3));
        r->joinTo(1, t, NPerm(2, 3, 0, 1));
        r->joinTo(3, t, NPerm(2, 3, 0, 1));

        ans->addTetrahedron(r);
        ans->addTetrahedron(s);
        ans->addTetrahedron(t);
    }

    return ans;
}

NAbelianGroup* NSimpleSurfaceBundle::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank();
    if (type == RP2xS1)
        ans->addTorsionElement(2);
    return ans;
}

std::ostream& NSimpleSurfaceBundle::writeName(std::ostream& out) const {
    if (type == S2xS1)
        out << "S2 x S1";
    else if (type == S2xS1_TWISTED)
        out << "S2 x~ S1";
    else if (type == RP2xS1)
        out << "RP2 x S1";
    return out;
}

std::ostream& NSimpleSurfaceBundle::writeTeXName(std::ostream& out) const {
    if (type == S2xS1)
        out << "S^2 \\times S^1";
    else if (type == S2xS1_TWISTED)
        out << "S^2 \\twisted S^1";
    else if (type == RP2xS1)
        out << "\\mathbb{R}P^2 \\times S^1";
    return out;
}

} // namespace regina

