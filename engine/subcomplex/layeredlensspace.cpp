
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
#include "manifold/lensspace.h"
#include "maths/numbertheory.h"
#include "subcomplex/layeredlensspace.h"
#include "triangulation/dim3.h"

namespace regina {

LayeredLensSpace* LayeredLensSpace::clone() const {
    LayeredLensSpace* ans = new LayeredLensSpace();
    ans->torus_ = torus_->clone();
    ans->mobiusBoundaryGroup_ = mobiusBoundaryGroup_;
    ans->p_ = p_;
    ans->q_ = q_;
    return ans;
}

LayeredLensSpace* LayeredLensSpace::isLayeredLensSpace(
        const Component<3>* comp) {
    // Basic property check.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;
    if (comp->countVertices() > 1)
        return 0;

    unsigned long nTet = comp->size();
    LayeredSolidTorus* torus;
    for (unsigned long i = 0; i < nTet; i++) {
        torus = LayeredSolidTorus::formsLayeredSolidTorusBase(
            comp->tetrahedron(i));
        if (torus) {
            // We have found a layered solid torus; either this makes the
            // layered lens space or nothing makes the layered lens space.
            Tetrahedron<3>* tet = torus->topLevel();
            int tf0 = torus->topFace(0);
            int tf1 = torus->topFace(1);
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
            LayeredLensSpace* ans = new LayeredLensSpace();
            ans->torus_ = torus;

            Perm<4> perm = tet->adjacentGluing(tf0);
            if (perm[tf1] == tf0) {
                // Snapped shut.
                ans->mobiusBoundaryGroup_ = torus->topEdgeGroup(
                    5 - Edge<3>::edgeNumber[tf0][tf1]);
            } else {
                // Twisted shut.
                ans->mobiusBoundaryGroup_ = torus->topEdgeGroup(
                    Edge<3>::edgeNumber[perm[tf1]][tf0]);
            }

            // Work out p and q.
            switch (ans->mobiusBoundaryGroup_) {
                // For layered solid torus (x < y < z):
                case 0:
                    // L( x + 2y, y )
                    ans->p_ =
                        torus->meridinalCuts(1) + torus->meridinalCuts(2);
                    ans->q_ = torus->meridinalCuts(1);
                    break;
                case 1:
                    // L( 2x + y, x )
                    ans->p_ =
                        torus->meridinalCuts(0) + torus->meridinalCuts(2);
                    ans->q_ = torus->meridinalCuts(0);
                    break;
                case 2:
                    // L( y - x, x )
                    ans->p_ =
                        torus->meridinalCuts(1) - torus->meridinalCuts(0);
                    if (ans->p_ == 0)
                        ans->q_ = 1;
                    else
                        ans->q_ = torus->meridinalCuts(0) % ans->p_;
                    break;
            }

            // Find the nicest possible value for q.
            // Choices are +/- q, +/- 1/q.
            if (ans->p_ > 0) {
                if (2 * ans->q_ > ans->p_)
                    ans->q_ = ans->p_ - ans->q_;
                if (ans->q_ > 0) {
                    unsigned long qAlt = modularInverse(ans->p_, ans->q_);
                    if (2 * qAlt > ans->p_)
                        qAlt = ans->p_ - qAlt;
                    if (qAlt < ans->q_)
                        ans->q_ = qAlt;
                }
            }

            return ans;
        }
    }
    return 0;
}

Manifold* LayeredLensSpace::manifold() const {
    return new LensSpace(p_, q_);
}

AbelianGroup* LayeredLensSpace::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    if (p_ == 0)
        ans->addRank();
    else if (p_ > 1)
        ans->addTorsionElement(p_);
    return ans;
}

std::ostream& LayeredLensSpace::writeName(std::ostream& out) const {
    if (p_ == 3 && q_ == 1) {
        out << "L(3,1)";
        if (torus_->size() != 2)
            return out;
        else if (isSnapped())
            return out << " (1)";
        else
            return out << " (2)";
    } else
        return out << "L(" << p_ << ',' << q_ << ')';
}

std::ostream& LayeredLensSpace::writeTeXName(std::ostream& out) const {
    if (p_ == 3 && q_ == 1) {
        out << "L_{3,1}";
        if (torus_->size() != 2)
            return out;
        else if (isSnapped())
            return out << "^{(1)}";
        else
            return out << "^{(2)}";
    } else
        return out << "L_{" << p_ << ',' << q_ << '}';
}

} // namespace regina
