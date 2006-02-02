
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
#include "triangulation/ntriangulation.h"
#include <cstdlib> // For exit().

namespace regina {

NSatBlock* NSatBlock::isBlock(const NSatAnnulus& annulus, TetList& avoidTets) {
    NSatBlock* ans;

    // Run through the types of blocks that we know about.
    if ((ans = NSatMobius::isBlockMobius(annulus, avoidTets)))
        return ans;
    if ((ans = NSatLST::isBlockLST(annulus, avoidTets)))
        return ans;
    if ((ans = NSatTriPrism::isBlockTriPrism(annulus, avoidTets)))
        return ans;

    // Nothing was found.
    return 0;
}

void NSatMobius::adjustSFS(NSFSpace& sfs, bool reflect) const {
    if (position_ == 0) {
        // Diagonal:
        sfs.insertFibre(1, reflect ? 1 : -1);
    } else if (position_ == 1) {
        // Horizontal:
        sfs.insertFibre(1, reflect ? -2 : 2);
    } else {
        // Vertical:
        sfs.insertFibre(2, reflect ? -1 : 1);
    }
}

NSatMobius* NSatMobius::isBlockMobius(const NSatAnnulus& annulus, TetList&) {
    // The two tetrahedra must be joined together along the annulus faces.

    if (annulus.tet[0]->getAdjacentTetrahedron(annulus.roles[0][3]) !=
            annulus.tet[1])
        return 0;

    NPerm annulusGluing = annulus.roles[1].inverse() *
        annulus.tet[0]->getAdjacentTetrahedronGluing(annulus.roles[0][3]) *
        annulus.roles[0];

    if (annulusGluing[3] != 3)
        return 0;

    // The faces are glued together.  Is it one of the allowable
    // (orientable) permutations?
    if (annulusGluing == NPerm(0, 1))
        return new NSatMobius(2); // Vertical
    if (annulusGluing == NPerm(0, 2))
        return new NSatMobius(1); // Horizontal
    if (annulusGluing == NPerm(1, 2))
        return new NSatMobius(0); // Diagonal

    // Nope.  It must be a non-orientable permutation.
    return 0;
}

NSatLST::NSatLST(const NSatLST& cloneMe) : NSatBlock(cloneMe),
        lst_(cloneMe.lst_->clone()), roles_(cloneMe.roles_) {
}

NSatLST::~NSatLST() {
    delete lst_;
}

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

void NSatLST::transform(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) {
    // Start with the parent implementation.
    NSatBlock::transform(originalTri, iso, newTri);

    // Transform the layered solid torus also.
    lst_->transform(originalTri, iso, newTri);
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

NSatTriPrism* NSatTriPrism::insertBlock(NTriangulation& tri, bool major) {
    NTetrahedron* a = new NTetrahedron();
    NTetrahedron* b = new NTetrahedron();
    NTetrahedron* c = new NTetrahedron();
    a->joinTo(1, c, NPerm(2, 0, 3, 1));
    b->joinTo(1, a, NPerm(2, 0, 3, 1));
    c->joinTo(1, b, NPerm(2, 0, 3, 1));
    tri.addTetrahedron(a);
    tri.addTetrahedron(b);
    tri.addTetrahedron(c);

    NSatTriPrism* ans = new NSatTriPrism(major);

    const NPerm id;
    const NPerm pairSwap(1, 0, 3, 2);
    ans->annulus_[0].tet[0] = a;
    ans->annulus_[0].tet[1] = b;
    ans->annulus_[0].roles[0] = id;
    ans->annulus_[0].roles[1] = pairSwap;
    ans->annulus_[1].tet[0] = b;
    ans->annulus_[1].tet[1] = c;
    ans->annulus_[1].roles[0] = id;
    ans->annulus_[1].roles[1] = pairSwap;
    ans->annulus_[2].tet[0] = c;
    ans->annulus_[2].tet[1] = a;
    ans->annulus_[2].roles[0] = id;
    ans->annulus_[2].roles[1] = pairSwap;

    if (! major) {
        ans->annulus_[0].reflectVertical();
        ans->annulus_[1].reflectVertical();
        ans->annulus_[2].reflectVertical();
    }

    return ans;
}

void NSatCube::adjustSFS(NSFSpace& sfs, bool reflect) const {
    sfs.insertFibre(1, reflect ? -2 : 2);
}

NSatCube* NSatCube::isBlockCube(const NSatAnnulus& annulus,
        TetList& avoidTets) {
    // TODO: Implement cube recognition.
    return 0;
}

NSatCube* NSatCube::insertBlock(NTriangulation& tri) {
    NTetrahedron* bdry0 = new NTetrahedron();
    NTetrahedron* bdry1 = new NTetrahedron();
    NTetrahedron* bdry2 = new NTetrahedron();
    NTetrahedron* bdry3 = new NTetrahedron();
    NTetrahedron* central0 = new NTetrahedron();
    NTetrahedron* central1 = new NTetrahedron();

    const NPerm id;
    bdry0->joinTo(1, central0, id);
    bdry0->joinTo(0, central1, NPerm(0, 1));
    bdry1->joinTo(2, central0, NPerm(2, 1, 3, 0));
    bdry1->joinTo(0, central1, NPerm(0, 3));
    bdry2->joinTo(0, central0, id);
    bdry2->joinTo(1, central1, NPerm(0, 1));
    bdry3->joinTo(3, central0, NPerm(0, 3, 1, 2));
    bdry3->joinTo(1, central1, NPerm(1, 2));

    tri.addTetrahedron(bdry0);
    tri.addTetrahedron(bdry1);
    tri.addTetrahedron(bdry2);
    tri.addTetrahedron(bdry3);
    tri.addTetrahedron(central0);
    tri.addTetrahedron(central1);

    NSatCube* ans = new NSatCube();

    ans->annulus_[0].tet[0] = bdry0;
    ans->annulus_[0].tet[1] = bdry1;
    ans->annulus_[1].tet[0] = bdry1;
    ans->annulus_[1].tet[1] = bdry2;
    ans->annulus_[2].tet[0] = bdry2;
    ans->annulus_[2].tet[1] = bdry3;
    ans->annulus_[3].tet[0] = bdry3;
    ans->annulus_[3].tet[1] = bdry0;

    ans->annulus_[0].roles[0] = NPerm(0, 1);
    ans->annulus_[0].roles[1] = NPerm(2, 0, 3, 1);
    ans->annulus_[1].roles[0] = NPerm(1, 2);
    ans->annulus_[1].roles[1] = NPerm(0, 1);
    ans->annulus_[2].roles[0] = NPerm(2, 3);
    ans->annulus_[2].roles[1] = NPerm(0, 3);
    ans->annulus_[3].roles[0] = NPerm(1, 3, 0, 2);
    ans->annulus_[3].roles[1] = NPerm(2, 3);

    return ans;
}

} // namespace regina

