
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
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"
#include "subcomplex/nlayeredchainpair.h"

namespace regina {

NLayeredChainPair* NLayeredChainPair::clone() const {
    NLayeredChainPair* ans = new NLayeredChainPair();
    if (chain[0])
        ans->chain[0] = new NLayeredChain(*chain[0]);
    if (chain[1])
        ans->chain[1] = new NLayeredChain(*chain[1]);
    return ans;
}

NLayeredChainPair* NLayeredChainPair::isLayeredChainPair(
        const NComponent* comp) {
    // Basic property check.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;

    unsigned long nTet = comp->getNumberOfTetrahedra();
    if (nTet < 2)
        return 0;
    if (comp->getNumberOfVertices() != 1)
        return 0;

    // We have at least two tetrahedra and precisely 1 vertex.
    // The component is closed and orientable (and connected, since it's
    // a component).

    // Start with tetrahedron 0.  This must belong to *some* chain.
    NTetrahedron* base = comp->getTetrahedron(0);

    NLayeredChain* first;
    NLayeredChain* second;

    // Note that we only need check permutations in S3 since we can
    // arbitrarily assign the role of one vertex in the tetrahedron.
    NTetrahedron* firstBottom;
    NTetrahedron* firstTop;
    NTetrahedron* secondBottom;
    NTetrahedron* secondTop;
    NPerm firstBottomRoles, firstTopRoles, secondBottomRoles, secondTopRoles;

    for (int p = 0; p < 6; p++) {
        first = new NLayeredChain(base, allPermsS3[p]);
        first->extendMaximal();

        firstTop = first->getTop();
        firstBottom = first->getBottom();
        firstTopRoles = first->getTopVertexRoles();
        firstBottomRoles = first->getBottomVertexRoles();

        // Check to see if the first chain fills the entire component.
        if (first->getIndex() == nTet) {
            // The only success here will be if we have a chain pair of
            // indices (n-1) and 1, which is in fact a layered loop.

            NLayeredChain* longChain = new NLayeredChain(
                firstBottom, firstBottomRoles);
            if (longChain->extendBelow())
                if (longChain->getBottom() == firstTop &&
                        longChain->getBottomVertexRoles() ==
                        firstTopRoles * NPerm(3, 2, 1, 0)) {
                    // We've got a layered loop!
                    NLayeredChainPair* ans = new NLayeredChainPair();
                    if (nTet == 2) {
                        // The new chain is already too long.
                        delete longChain;
                        longChain = new NLayeredChain(
                            firstBottom, firstBottomRoles);
                    }

                    // Extend longChain to (n-1) tetrahedra.
                    while (longChain->getIndex() + 1 < nTet)
                        longChain->extendBelow();
                    ans->chain[1] = longChain;
                    ans->chain[0] = new NLayeredChain(
                        firstBottom->getAdjacentTetrahedron(
                            firstBottomRoles[0]),
                        firstBottom->getAdjacentTetrahedronGluing(
                            firstBottomRoles[0]) * firstBottomRoles *
                            NPerm(0, 2, 1, 3));

                    delete first;
                    return ans;
                }

            delete longChain;
            delete first;
            continue;
        }

        // At this point we must have run into the second chain.
        secondBottom = firstTop->getAdjacentTetrahedron(firstTopRoles[3]);
        if (secondBottom == firstTop || secondBottom == firstBottom ||
                secondBottom == 0) {
            delete first;
            continue;
        }

        second = new NLayeredChain(secondBottom,
            firstTop->getAdjacentTetrahedronGluing(firstTopRoles[3]) *
            firstTopRoles * NPerm(1, 3, 0, 2));
        while (second->extendAbove())
            ;

        if (second->getIndex() + first->getIndex() != nTet) {
            delete first;
            delete second;
            continue;
        }

        secondTop = second->getTop();
        secondTopRoles = second->getTopVertexRoles();
        secondBottomRoles = second->getBottomVertexRoles();

        // At this point we have two chains that together have the
        // correct number of tetrahedra.  All we need do is check the
        // remaining three between-chain gluings.
        if (secondTop == firstTop->getAdjacentTetrahedron(firstTopRoles[0]) &&
                secondBottom == firstBottom->getAdjacentTetrahedron(
                    firstBottomRoles[2]) &&
                secondTop == firstBottom->getAdjacentTetrahedron(
                    firstBottomRoles[1]) &&
                secondTopRoles == firstTop->getAdjacentTetrahedronGluing(
                    firstTopRoles[0]) * firstTopRoles * NPerm(0, 2, 1, 3) &&
                secondBottomRoles == firstBottom->getAdjacentTetrahedronGluing(
                    firstBottomRoles[2]) * firstBottomRoles *
                    NPerm(3, 1, 2, 0) &&
                secondTopRoles == firstBottom->getAdjacentTetrahedronGluing(
                    firstBottomRoles[1]) * firstBottomRoles *
                    NPerm(2, 0, 3, 1)) {
            // We found one!
            NLayeredChainPair* ans = new NLayeredChainPair();
            if (first->getIndex() > second->getIndex()) {
                ans->chain[0] = second;
                ans->chain[1] = first;
            } else {
                ans->chain[0] = first;
                ans->chain[1] = second;
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

NManifold* NLayeredChainPair::getManifold() const {
    NSFSpace* ans = new NSFSpace();

    ans->insertFibre(2, -1);
    ans->insertFibre(chain[0]->getIndex() + 1, 1);
    ans->insertFibre(chain[1]->getIndex() + 1, 1);

    ans->reduce();
    return ans;
}

NAbelianGroup* NLayeredChainPair::getHomologyH1() const {
    // The first homology group can be obtained from the matrix:
    //
    //   [  1  -1   1 ]
    //   [ n_1  1   1 ]
    //   [  1  n_2 -1 ]
    //
    // This is established simply by examining the edges on the boundary
    // of each layered chain.
    NAbelianGroup* ans = new NAbelianGroup();
    NMatrixInt mat(3, 3);
    mat.initialise(1);
    mat.entry(0, 1) = mat.entry(2, 2) = -1;
    mat.entry(1, 0) = chain[0]->getIndex();
    mat.entry(2, 1) = chain[1]->getIndex();
    ans->addGroup(mat);
    return ans;
}

} // namespace regina

