
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
#include "manifold/sfs.h"
#include "maths/matrix.h"
#include "subcomplex/layeredchainpair.h"
#include "triangulation/dim3.h"

namespace regina {

LayeredChainPair* LayeredChainPair::clone() const {
    LayeredChainPair* ans = new LayeredChainPair();
    if (chain_[0])
        ans->chain_[0] = new LayeredChain(*chain_[0]);
    if (chain_[1])
        ans->chain_[1] = new LayeredChain(*chain_[1]);
    return ans;
}

LayeredChainPair* LayeredChainPair::isLayeredChainPair(
        const Component<3>* comp) {
    // Basic property check.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;

    unsigned long nTet = comp->size();
    if (nTet < 2)
        return 0;
    if (comp->countVertices() != 1)
        return 0;

    // We have at least two tetrahedra and precisely 1 vertex.
    // The component is closed and orientable (and connected, since it's
    // a component).

    // Start with tetrahedron 0.  This must belong to *some* chain.
    Tetrahedron<3>* base = comp->tetrahedron(0);

    LayeredChain* first;
    LayeredChain* second;

    // Note that we only need check permutations in S3 since we can
    // arbitrarily assign the role of one vertex in the tetrahedron.
    Tetrahedron<3>* firstBottom;
    Tetrahedron<3>* firstTop;
    Tetrahedron<3>* secondBottom;
    Tetrahedron<3>* secondTop;
    Perm<4> firstBottomRoles, firstTopRoles, secondBottomRoles, secondTopRoles;

    for (int p = 0; p < 6; p++) {
        first = new LayeredChain(base, Perm<4>::S3[p]);
        first->extendMaximal();

        firstTop = first->top();
        firstBottom = first->bottom();
        firstTopRoles = first->topVertexRoles();
        firstBottomRoles = first->bottomVertexRoles();

        // Check to see if the first chain fills the entire component.
        if (first->index() == nTet) {
            // The only success here will be if we have a chain pair of
            // indices (n-1) and 1, which is in fact a layered loop.

            LayeredChain* longChain = new LayeredChain(
                firstBottom, firstBottomRoles);
            if (longChain->extendBelow())
                if (longChain->bottom() == firstTop &&
                        longChain->bottomVertexRoles() ==
                        firstTopRoles * Perm<4>(3, 2, 1, 0)) {
                    // We've got a layered loop!
                    LayeredChainPair* ans = new LayeredChainPair();
                    if (nTet == 2) {
                        // The new chain is already too long.
                        delete longChain;
                        longChain = new LayeredChain(
                            firstBottom, firstBottomRoles);
                    }

                    // Extend longChain to (n-1) tetrahedra.
                    while (longChain->index() + 1 < nTet)
                        longChain->extendBelow();
                    ans->chain_[1] = longChain;
                    ans->chain_[0] = new LayeredChain(
                        firstBottom->adjacentTetrahedron(
                            firstBottomRoles[0]),
                        firstBottom->adjacentGluing(
                            firstBottomRoles[0]) * firstBottomRoles *
                            Perm<4>(0, 2, 1, 3));

                    delete first;
                    return ans;
                }

            delete longChain;
            delete first;
            continue;
        }

        // At this point we must have run into the second chain.
        secondBottom = firstTop->adjacentTetrahedron(firstTopRoles[3]);
        if (secondBottom == firstTop || secondBottom == firstBottom ||
                secondBottom == 0) {
            delete first;
            continue;
        }

        second = new LayeredChain(secondBottom,
            firstTop->adjacentGluing(firstTopRoles[3]) *
            firstTopRoles * Perm<4>(1, 3, 0, 2));
        while (second->extendAbove())
            ;

        if (second->index() + first->index() != nTet) {
            delete first;
            delete second;
            continue;
        }

        secondTop = second->top();
        secondTopRoles = second->topVertexRoles();
        secondBottomRoles = second->bottomVertexRoles();

        // At this point we have two chains that together have the
        // correct number of tetrahedra.  All we need do is check the
        // remaining three between-chain gluings.
        if (secondTop == firstTop->adjacentTetrahedron(firstTopRoles[0]) &&
                secondBottom == firstBottom->adjacentTetrahedron(
                    firstBottomRoles[2]) &&
                secondTop == firstBottom->adjacentTetrahedron(
                    firstBottomRoles[1]) &&
                secondTopRoles == firstTop->adjacentGluing(
                    firstTopRoles[0]) * firstTopRoles * Perm<4>(0, 2, 1, 3) &&
                secondBottomRoles == firstBottom->adjacentGluing(
                    firstBottomRoles[2]) * firstBottomRoles *
                    Perm<4>(3, 1, 2, 0) &&
                secondTopRoles == firstBottom->adjacentGluing(
                    firstBottomRoles[1]) * firstBottomRoles *
                    Perm<4>(2, 0, 3, 1)) {
            // We found one!
            LayeredChainPair* ans = new LayeredChainPair();
            if (first->index() > second->index()) {
                ans->chain_[0] = second;
                ans->chain_[1] = first;
            } else {
                ans->chain_[0] = first;
                ans->chain_[1] = second;
            }
            return ans;
        } else {
            delete first;
            delete second;
        }
    }

    // Nothing was found.  Sigh.
    return 0;
}

Manifold* LayeredChainPair::manifold() const {
    SFSpace* ans = new SFSpace();

    ans->insertFibre(2, -1);
    ans->insertFibre(chain_[0]->index() + 1, 1);
    ans->insertFibre(chain_[1]->index() + 1, 1);

    ans->reduce();
    return ans;
}

AbelianGroup* LayeredChainPair::homology() const {
    // The first homology group can be obtained from the matrix:
    //
    //   [  1  -1   1 ]
    //   [ n_1  1   1 ]
    //   [  1  n_2 -1 ]
    //
    // This is established simply by examining the edges on the boundary
    // of each layered chain.
    AbelianGroup* ans = new AbelianGroup();
    MatrixInt mat(3, 3);
    mat.initialise(1);
    mat.entry(0, 1) = mat.entry(2, 2) = -1;
    mat.entry(1, 0) = chain_[0]->index();
    mat.entry(2, 1) = chain_[1]->index();
    ans->addGroup(mat);
    return ans;
}

} // namespace regina

