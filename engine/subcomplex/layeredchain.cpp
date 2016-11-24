
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
#include "manifold/handlebody.h"
#include "triangulation/dim3.h"
#include "subcomplex/layeredchain.h"

namespace regina {

bool LayeredChain::extendAbove() {
    Tetrahedron<3>* adj = top_->adjacentTetrahedron(topVertexRoles_[0]);
    if (adj == bottom_ || adj == top_ || adj == 0)
        return false;
    if (adj != top_->adjacentTetrahedron(topVertexRoles_[3]))
        return false;

    // Check the gluings.
    Perm<4> adjRoles = top_->adjacentGluing(topVertexRoles_[0]) *
        topVertexRoles_ * Perm<4>(0, 1);
    if (adjRoles != top_->adjacentGluing(topVertexRoles_[3]) *
            topVertexRoles_ * Perm<4>(2, 3))
        return false;

    // We can extend the layered chain.
    top_ = adj;
    topVertexRoles_ = adjRoles;
    index_++;
    return true;
}

bool LayeredChain::extendBelow() {
    Tetrahedron<3>* adj = bottom_->adjacentTetrahedron(bottomVertexRoles_[1]);
    if (adj == bottom_ || adj == top_ || adj == 0)
        return false;
    if (adj != bottom_->adjacentTetrahedron(bottomVertexRoles_[2]))
        return false;

    // Check the gluings.
    Perm<4> adjRoles = bottom_->adjacentGluing(bottomVertexRoles_[1])
        * bottomVertexRoles_ * Perm<4>(0, 1);
    if (adjRoles != bottom_->adjacentGluing(bottomVertexRoles_[2])
            * bottomVertexRoles_ * Perm<4>(2, 3))
        return false;

    // We can extend the layered chain.
    bottom_ = adj;
    bottomVertexRoles_ = adjRoles;
    index_++;
    return true;
}

bool LayeredChain::extendMaximal() {
    bool changed = false;
    while (extendAbove())
        changed = true;
    while (extendBelow())
        changed = true;
    return changed;
}

void LayeredChain::reverse() {
    Tetrahedron<3>* tmp = top_;
    top_ = bottom_;
    bottom_ = tmp;

    Perm<4> pTmp = topVertexRoles_ * Perm<4>(1, 0, 3, 2);
    topVertexRoles_ = bottomVertexRoles_ * Perm<4>(1, 0, 3, 2);
    bottomVertexRoles_ = pTmp;
}

void LayeredChain::invert() {
    topVertexRoles_ = topVertexRoles_ * Perm<4>(3, 2, 1, 0);
    bottomVertexRoles_ = bottomVertexRoles_ * Perm<4>(3, 2, 1, 0);
}

Manifold* LayeredChain::manifold() const {
    return new Handlebody(index_ <= 1 ? 0 : 1, true);
}

AbelianGroup* LayeredChain::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    if (index_ > 1)
        ans->addRank();
    return ans;
}

} // namespace regina

