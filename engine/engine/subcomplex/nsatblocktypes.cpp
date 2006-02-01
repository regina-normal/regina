
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "manifold/nsfs.h"
#include "subcomplex/nsatblocktypes.h"
#include "subcomplex/nlayeredsolidtorus.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntetrahedron.h"
#include <cstdlib> // For exit().

namespace regina {

void NSatLST::adjustSFS(NSFSpace& sfs, bool reflect) const {
    long cutsVert = lst_->getMeridinalCuts(roles_[0]);
    long cutsHoriz = lst_->getMeridinalCuts(roles_[1]);
    if (roles_[2] == 2) {
        // Most cuts are on the diagonal, which means the meridinal
        // curve is negative.
        cutsHoriz = -cutsHoriz;
    }

    sfs.insertFibre(cutsVert, reflect ? -cutsHoriz : cutsHoriz);
}

NSatLST* NSatLST::isBlockLST(const NSatAnnulus& annulus, TetList& avoidTets) {
    // Do we move to a common usable tetrahedron?

    if (annulus.tet[0] != annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets))
        return 0;

    // Is it a layering?

    // Here we find the endpoints of the edge from which the two layered
    // faces fold out.
    NFacePair centralEdge =
        NFacePair(annulus.roles[0][3], annulus.roles[1][3]).complement();

    if (annulus.roles[1] !=
            NPerm(annulus.roles[0][3], annulus.roles[1][3]) *
            NPerm(centralEdge.upper(), centralEdge.lower()) *
            annulus.roles[0])
        return 0;

    // Find the layered solid torus.
    NLayeredSolidTorus* lst = NLayeredSolidTorus::formsLayeredSolidTorusTop(
        annulus.tet[0], annulus.roles[0][3], annulus.roles[1][3]);
    if (! lst)
        return 0;

    // Make sure we're not about to create a (0,k) curve.
    NPerm lstRoles(
        lst->getTopEdgeGroup(
            edgeNumber[annulus.roles[0][0]][annulus.roles[0][1]]),
        lst->getTopEdgeGroup(
            edgeNumber[annulus.roles[0][0]][annulus.roles[0][2]]),
        lst->getTopEdgeGroup(
            edgeNumber[annulus.roles[0][1]][annulus.roles[0][2]]),
        3);

    if (lst->getMeridinalCuts(lstRoles[0]) == 0)
        return 0;

    // Verify that all of the tetrahedra are usable, and add them to the
    // list if they are.
    NTetrahedron* current = annulus.tet[0];
    NFacePair nextPair;
    avoidTets.push_back(current);
    unsigned long addedTets = 1;
    while (addedTets < lst->getNumberOfTetrahedra()) {
        // INV: The current tetrahedron is usable.
        // INV: We have checked addedTets tetrahedra thus far.
        // INV: The next two faces to push through are in centralEdge.

        // Start by pushing through to the next tetrahedron.
        nextPair = NFacePair(
            current->getAdjacentFace(centralEdge.upper()),
            current->getAdjacentFace(centralEdge.lower())
            ).complement();
        current = current->getAdjacentTetrahedron(centralEdge.upper());
        centralEdge = nextPair;

        // Make sure this next tetrahedron is usable.
        if (isBad(current, avoidTets)) {
            // No!  Run back and pull off all the tetrahedra we've added
            // so far, and then bail.
            for ( ; addedTets > 0; addedTets--)
                avoidTets.pop_back();
            return 0;
        }

        avoidTets.push_back(current);
        addedTets++;
    }
    // We had better be at the bottom of the LST by now.
    if (current->getAdjacentTetrahedron(centralEdge.upper()) != current) {
        // THIS SHOULD NEVER HAPPEN!
        std::cerr <<
            "ERROR: Failure in the NSatLST tetrahedron usability test.\n";
        std::cerr.flush();
        exit(0);
    }

    // All good!
    NSatLST* ans = new NSatLST(lst, lstRoles);
    ans->annulus_[0] = annulus;
    return ans;
}

void NSatTriPrism::adjustSFS(NSFSpace& sfs, bool reflect) const {
    if (major_)
        sfs.insertFibre(1, reflect ? -1 : 1);
    else
        sfs.insertFibre(1, reflect ? -2 : 2);
}

NSatTriPrism* NSatTriPrism::isBlockTriPrism(const NSatAnnulus& annulus,
        TetList& avoidTets) {
    NSatTriPrism* ans;

    // First try for one of major type.
    if ((ans = isBlockTriPrismMajor(annulus, avoidTets)))
        return ans;

    // Now try the reflected version.
    NSatAnnulus altAnnulus = annulus.verticalReflection();
    if ((ans = isBlockTriPrismMajor(altAnnulus, avoidTets))) {
        // Reflect it back again but mark it as a minor variant.
        ans->major_ = false;

        ans->annulus_[0].reflectVertical();
        ans->annulus_[1].reflectVertical();
        ans->annulus_[2].reflectVertical();

        return ans;
    }

    // Neither variant was found.
    return 0;
}

NSatTriPrism* NSatTriPrism::isBlockTriPrismMajor(const NSatAnnulus& annulus,
        TetList& avoidTets) {
    if (annulus.tet[0] == annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets) || isBad(annulus.tet[1], avoidTets))
        return 0;
    if (annulus.tet[0]->getAdjacentTetrahedron(annulus.roles[0][0]) !=
            annulus.tet[1])
        return 0;
    if (annulus.tet[0]->getAdjacentTetrahedronGluing(annulus.roles[0][0]) *
            annulus.roles[0] * NPerm(1, 2) != annulus.roles[1])
        return 0;

    // The two tetrahedra forming the annulus are joined together as
    // expected.  Look for the third tetrahedron.

    NTetrahedron* adj = annulus.tet[0]->getAdjacentTetrahedron(
        annulus.roles[0][1]);
    if (adj == 0 || adj == annulus.tet[0] || adj == annulus.tet[1])
        return 0;
    if (isBad(adj, avoidTets))
        return 0;

    NPerm adjRoles =
        annulus.tet[0]->getAdjacentTetrahedronGluing(annulus.roles[0][1]) *
        annulus.roles[0] * NPerm(0, 3);

    if (annulus.tet[1]->getAdjacentTetrahedronGluing(annulus.roles[1][1]) *
            annulus.roles[1] * NPerm(1, 3, 0, 2) != adjRoles)
        return 0;

    // All three tetrahedra are joined together as expected!

    NSatTriPrism* ans = new NSatTriPrism(true);

    const NPerm pairSwap(1, 0, 3, 2);
    ans->annulus_[0] = annulus;
    ans->annulus_[1].tet[0] = annulus.tet[1];
    ans->annulus_[1].tet[1] = adj;
    ans->annulus_[1].roles[0] = annulus.roles[1] * pairSwap;
    ans->annulus_[1].roles[1] = adjRoles;
    ans->annulus_[2].tet[0] = adj;
    ans->annulus_[2].tet[1] = annulus.tet[0];
    ans->annulus_[2].roles[0] = adjRoles * pairSwap;
    ans->annulus_[2].roles[1] = annulus.roles[0] * pairSwap;

    avoidTets.push_back(annulus.tet[0]);
    avoidTets.push_back(annulus.tet[1]);
    avoidTets.push_back(adj);

    return ans;
}

} // namespace regina

