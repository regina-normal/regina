
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

#include "algebra/abeliangroup.h"
#include "manifold/simplesurfacebundle.h"
#include "triangulation/dim3.h"

namespace regina {

const int SimpleSurfaceBundle::S2xS1 = 1;
const int SimpleSurfaceBundle::S2xS1_TWISTED = 2;
const int SimpleSurfaceBundle::RP2xS1 = 3;

Triangulation<3>* SimpleSurfaceBundle::construct() const {
    Triangulation<3>* ans = new Triangulation<3>();

    if (type_ == S2xS1) {
        ans->insertLayeredLensSpace(0, 1);
    } else if (type_ == S2xS1_TWISTED) {
        // Taken from section 3.5.1 of Ben Burton's PhD thesis.
        Tetrahedron<3>* r = ans->newTetrahedron();
        Tetrahedron<3>* s = ans->newTetrahedron();

        r->join(1, s, Perm<4>());
        r->join(3, s, Perm<4>());
        r->join(2, s, Perm<4>(3, 2, 0, 1));
        s->join(2, r, Perm<4>(3, 2, 0, 1));
    } else if (type_ == RP2xS1) {
        // Taken from section 3.5.1 of Ben Burton's PhD thesis.
        Tetrahedron<3>* r = ans->newTetrahedron();
        Tetrahedron<3>* s = ans->newTetrahedron();
        Tetrahedron<3>* t = ans->newTetrahedron();

        s->join(0, r, Perm<4>(0, 1, 2, 3));
        s->join(3, r, Perm<4>(3, 0, 1, 2));
        s->join(1, t, Perm<4>(3, 0, 1, 2));
        s->join(2, t, Perm<4>(0, 1, 2, 3));
        r->join(1, t, Perm<4>(2, 3, 0, 1));
        r->join(3, t, Perm<4>(2, 3, 0, 1));
    }

    return ans;
}

AbelianGroup* SimpleSurfaceBundle::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    ans->addRank();
    if (type_ == RP2xS1)
        ans->addTorsionElement(2);
    return ans;
}

std::ostream& SimpleSurfaceBundle::writeName(std::ostream& out) const {
    if (type_ == S2xS1)
        out << "S2 x S1";
    else if (type_ == S2xS1_TWISTED)
        out << "S2 x~ S1";
    else if (type_ == RP2xS1)
        out << "RP2 x S1";
    return out;
}

std::ostream& SimpleSurfaceBundle::writeTeXName(std::ostream& out) const {
    if (type_ == S2xS1)
        out << "S^2 \\times S^1";
    else if (type_ == S2xS1_TWISTED)
        out << "S^2 \\twisted S^1";
    else if (type_ == RP2xS1)
        out << "\\mathbb{R}P^2 \\times S^1";
    return out;
}

} // namespace regina

