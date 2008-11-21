
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "manifold/nlensspace.h"
#include "maths/numbertheory.h"
#include "subcomplex/nlayeredlensspace.h"
#include "triangulation/nedge.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

NLayeredLensSpace* NLayeredLensSpace::clone() const {
    NLayeredLensSpace* ans = new NLayeredLensSpace();
    ans->torus = torus->clone();
    ans->mobiusBoundaryGroup = mobiusBoundaryGroup;
    ans->p = p;
    ans->q = q;
    return ans;
}

NLayeredLensSpace* NLayeredLensSpace::isLayeredLensSpace(
        const NComponent* comp) {
    // Basic property check.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;
    if (comp->getNumberOfVertices() > 1)
        return 0;

    unsigned long nTet = comp->getNumberOfTetrahedra();
    NLayeredSolidTorus* torus;
    for (unsigned long i = 0; i < nTet; i++) {
        torus = NLayeredSolidTorus::formsLayeredSolidTorusBase(
            comp->getTetrahedron(i));
        if (torus) {
            // We have found a layered solid torus; either this makes the
            // layered lens space or nothing makes the layered lens space.
            NTetrahedron* tet = torus->getTopLevel();
            int tf0 = torus->getTopFace(0);
            int tf1 = torus->getTopFace(1);
            if (tet->adjacentTetrahedron(tf0) != tet) {
                delete torus;
                return 0;
            }

            /* We already know the component is orientable; no need
               to check orientation!
            if (perm.sign() == 1) {
                delete torus;
                return 0;
            }*/

            // This is the real thing!
            NLayeredLensSpace* ans = new NLayeredLensSpace();
            ans->torus = torus;

            NPerm perm = tet->adjacentGluing(tf0);
            if (perm[tf1] == tf0) {
                // Snapped shut.
                ans->mobiusBoundaryGroup = torus->getTopEdgeGroup(
                    5 - edgeNumber[tf0][tf1]);
            } else {
                // Twisted shut.
                ans->mobiusBoundaryGroup = torus->getTopEdgeGroup(
                    edgeNumber[perm[tf1]][tf0]);
            }

            // Work out p and q.
            switch (ans->mobiusBoundaryGroup) {
                // For layered solid torus (x < y < z):
                case 0:
                    // L( x + 2y, y )
                    ans->p =
                        torus->getMeridinalCuts(1) + torus->getMeridinalCuts(2);
                    ans->q = torus->getMeridinalCuts(1);
                    break;
                case 1:
                    // L( 2x + y, x )
                    ans->p =
                        torus->getMeridinalCuts(0) + torus->getMeridinalCuts(2);
                    ans->q = torus->getMeridinalCuts(0);
                    break;
                case 2:
                    // L( y - x, x )
                    ans->p =
                        torus->getMeridinalCuts(1) - torus->getMeridinalCuts(0);
                    if (ans->p == 0)
                        ans->q = 1;
                    else
                        ans->q = torus->getMeridinalCuts(0) % ans->p;
                    break;
            }

            // Find the nicest possible value for q.
            // Choices are +/- q, +/- 1/q.
            if (ans->p > 0) {
                if (2 * ans->q > ans->p)
                    ans->q = ans->p - ans->q;
                if (ans->q > 0) {
                    unsigned long qAlt = modularInverse(ans->p, ans->q);
                    if (2 * qAlt > ans->p)
                        qAlt = ans->p - qAlt;
                    if (qAlt < ans->q)
                        ans->q = qAlt;
                }
            }

            return ans;
        }
    }
    return 0;
}

NManifold* NLayeredLensSpace::getManifold() const {
    return new NLensSpace(p, q);
}

NAbelianGroup* NLayeredLensSpace::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    if (p == 0)
        ans->addRank();
    else if (p > 1)
        ans->addTorsionElement(p);
    return ans;
}

std::ostream& NLayeredLensSpace::writeName(std::ostream& out) const {
    if (p == 3 && q == 1) {
        out << "L(3,1)";
        if (torus->getNumberOfTetrahedra() != 2)
            return out;
        else if (isSnapped())
            return out << " (1)";
        else
            return out << " (2)";
    } else
        return out << "L(" << p << ',' << q << ')';
}

std::ostream& NLayeredLensSpace::writeTeXName(std::ostream& out) const {
    if (p == 3 && q == 1) {
        out << "L_{3,1}";
        if (torus->getNumberOfTetrahedra() != 2)
            return out;
        else if (isSnapped())
            return out << "^{(1)}";
        else
            return out << "^{(2)}";
    } else
        return out << "L_{" << p << ',' << q << '}';
}

} // namespace regina
