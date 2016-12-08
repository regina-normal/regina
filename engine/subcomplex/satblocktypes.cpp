
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

#include "manifold/sfs.h"
#include "subcomplex/satblocktypes.h"
#include "subcomplex/layeredsolidtorus.h"
#include "triangulation/facepair.h"
#include "triangulation/dim3.h"
#include <algorithm>
#include <cstdlib> // For exit().
#include <iterator>
#include <list>

namespace regina {

bool SatBlock::operator < (const SatBlock& compare) const {
    const SatTriPrism* tri1 = dynamic_cast<const SatTriPrism*>(this);
    const SatTriPrism* tri2 = dynamic_cast<const SatTriPrism*>(&compare);
    if (tri1 && ! tri2)
        return true;
    if (tri2 && ! tri1)
        return false;
    if (tri1 && tri2) {
        // Major first, then minor.
        return (tri1->isMajor() && ! tri2->isMajor());
    }

    const SatCube* cube1 = dynamic_cast<const SatCube*>(this);
    const SatCube* cube2 = dynamic_cast<const SatCube*>(&compare);
    if (cube1 && ! cube2)
        return true;
    if (cube2 && ! cube1)
        return false;
    if (cube1 && cube2) {
        // All cubes are considered equal.
        return false;
    }

    const SatReflectorStrip* ref1 =
        dynamic_cast<const SatReflectorStrip*>(this);
    const SatReflectorStrip* ref2 =
        dynamic_cast<const SatReflectorStrip*>(&compare);
    if (ref1 && ! ref2)
        return true;
    if (ref2 && ! ref1)
        return false;
    if (ref1 && ref2) {
        // Always put untwisted before twisted.
        if (ref1->twistedBoundary() && ! ref2->twistedBoundary())
            return false;
        if (ref2->twistedBoundary() && ! ref1->twistedBoundary())
            return true;
        return (ref1->nAnnuli() < ref2->nAnnuli());
    }

    const SatLST* lst1 = dynamic_cast<const SatLST*>(this);
    const SatLST* lst2 = dynamic_cast<const SatLST*>(&compare);
    if (lst1 && ! lst2)
        return true;
    if (lst2 && ! lst1)
        return false;
    if (lst1 && lst2) {
        // Order first by LST parameters, then by roles.
        if (lst1->lst()->meridinalCuts(2) < lst2->lst()->meridinalCuts(2))
            return true;
        if (lst1->lst()->meridinalCuts(2) > lst2->lst()->meridinalCuts(2))
            return false;
        if (lst1->lst()->meridinalCuts(1) < lst2->lst()->meridinalCuts(1))
            return true;
        if (lst1->lst()->meridinalCuts(1) > lst2->lst()->meridinalCuts(1))
            return false;
        if (lst1->lst()->meridinalCuts(0) < lst2->lst()->meridinalCuts(0))
            return true;
        if (lst1->lst()->meridinalCuts(0) > lst2->lst()->meridinalCuts(0))
            return false;

        // Sorts by which edge group is joined to the vertical annulus
        // edges, then horizontal, then diagonal (though we won't bother
        // testing diagonal, since by that stage we will know the roles
        // permutations to be equal).
        if (lst1->roles()[0] < lst2->roles()[0])
            return true;
        if (lst1->roles()[0] > lst2->roles()[0])
            return false;
        if (lst1->roles()[1] < lst2->roles()[1])
            return true;
        if (lst1->roles()[1] > lst2->roles()[1])
            return false;

        // All equal.
        return false;
    }

    const SatMobius* mob1 = dynamic_cast<const SatMobius*>(this);
    const SatMobius* mob2 = dynamic_cast<const SatMobius*>(&compare);
    if (mob1 && ! mob2)
        return true;
    if (mob2 && ! mob1)
        return false;
    if (mob1 && mob2) {
        // Order by position in descending order (vertical first, then
        // horizontal, then finally diagonal).
        return (mob1->position() > mob2->position());
    }

    const SatLayering* layer1 = dynamic_cast<const SatLayering*>(this);
    const SatLayering* layer2 = dynamic_cast<const SatLayering*>(&compare);
    if (layer1 && ! layer2)
        return true;
    if (layer2 && ! layer1)
        return false;
    if (layer1 && layer2) {
        // Horizontal, then diagonal.
        return (layer1->overHorizontal() && ! layer2->overHorizontal());
    }

    return false;
}

SatBlock* SatBlock::isBlock(const SatAnnulus& annulus, TetList& avoidTets) {
    SatBlock* ans;

    // Run through the types of blocks that we know about.
    if ((ans = SatMobius::isBlockMobius(annulus, avoidTets)))
        return ans;
    if ((ans = SatLST::isBlockLST(annulus, avoidTets)))
        return ans;
    if ((ans = SatTriPrism::isBlockTriPrism(annulus, avoidTets)))
        return ans;
    if ((ans = SatCube::isBlockCube(annulus, avoidTets)))
        return ans;
    if ((ans = SatReflectorStrip::isBlockReflectorStrip(annulus, avoidTets)))
        return ans;

    // As a last attempt, try a single layering.  We don't have to worry
    // about the degeneracy, since we'll never get a loop of these
    // things (since that would form a disconnected component, and we
    // never use one as a starting block).
    if ((ans = SatLayering::isBlockLayering(annulus, avoidTets)))
        return ans;

    // Nothing was found.
    return 0;
}

void SatMobius::adjustSFS(SFSpace& sfs, bool reflect) const {
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

void SatMobius::writeTextShort(std::ostream& out) const {
    out << "Saturated Mobius band, boundary on ";
    if (position_ == 0)
        out << "diagonal";
    else if (position_ == 1)
        out << "horizontal";
    else if (position_ == 2)
        out << "vertical";
    else
        out << "invalid";
    out << " edge";
}

void SatMobius::writeAbbr(std::ostream& out, bool tex) const {
    out << (tex ? "M_" : "Mob(");
    if (position_ == 0)
        out << 'd';
    else if (position_ == 1)
        out << 'h';
    else if (position_ == 2)
        out << 'v';
    if (! tex)
        out << ')';
}

SatMobius* SatMobius::isBlockMobius(const SatAnnulus& annulus, TetList&) {
    // The two tetrahedra must be joined together along the annulus triangles.

    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][3]) !=
            annulus.tet[1])
        return 0;

    Perm<4> annulusGluing = annulus.roles[1].inverse() *
        annulus.tet[0]->adjacentGluing(annulus.roles[0][3]) *
        annulus.roles[0];

    if (annulusGluing[3] != 3)
        return 0;

    // The triangles are glued together.  Is it one of the allowable
    // (orientable) permutations?

    int position = -1;
    if (annulusGluing == Perm<4>(0, 1))
        position = 2; // Vertical
    else if (annulusGluing == Perm<4>(0, 2))
        position = 1; // Horizontal
    else if (annulusGluing == Perm<4>(1, 2))
        position = 0; // Diagonal

    if (position < 0) {
        // Nope.  It must be a non-orientable permutation.
        return 0;
    }

    // Got it!

    SatMobius* ans = new SatMobius(position);
    ans->annulus_[0] = annulus;
    return ans;
}

SatLST::SatLST(const SatLST& cloneMe) : SatBlock(cloneMe),
        lst_(cloneMe.lst_->clone()), roles_(cloneMe.roles_) {
}

SatLST::~SatLST() {
    delete lst_;
}

void SatLST::adjustSFS(SFSpace& sfs, bool reflect) const {
    long cutsVert = lst_->meridinalCuts(roles_[0]);
    long cutsHoriz = lst_->meridinalCuts(roles_[1]);
    if (roles_[2] == 2) {
        // Most cuts are on the diagonal, which means the meridinal
        // curve is negative.
        cutsHoriz = -cutsHoriz;
    }

    sfs.insertFibre(cutsVert, reflect ? -cutsHoriz : cutsHoriz);
}

void SatLST::writeTextShort(std::ostream& out) const {
    out << "Saturated ("
        << lst_->meridinalCuts(0) << ", "
        << lst_->meridinalCuts(1) << ", "
        << lst_->meridinalCuts(2) << ") layered solid torus";
}

void SatLST::writeAbbr(std::ostream& out, bool tex) const {
    out << (tex ? "\\mathrm{LST}_{" : "LST(")
        << lst_->meridinalCuts(0) << ", "
        << lst_->meridinalCuts(1) << ", "
        << lst_->meridinalCuts(2) << (tex ? '}' : ')');
}

void SatLST::transform(const Triangulation<3>* originalTri,
        const Isomorphism<3>* iso, Triangulation<3>* newTri) {
    // Start with the parent implementation.
    SatBlock::transform(originalTri, iso, newTri);

    // Transform the layered solid torus also.
    lst_->transform(originalTri, iso, newTri);
}

SatLST* SatLST::isBlockLST(const SatAnnulus& annulus, TetList& avoidTets) {
    // Do we move to a common usable tetrahedron?

    if (annulus.tet[0] != annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets))
        return 0;

    // Is it a layering?

    // Here we find the endpoints of the edge from which the two layered
    // triangles fold out.
    FacePair centralEdge =
        FacePair(annulus.roles[0][3], annulus.roles[1][3]).complement();

    if (annulus.roles[1] !=
            Perm<4>(annulus.roles[0][3], annulus.roles[1][3]) *
            Perm<4>(centralEdge.upper(), centralEdge.lower()) *
            annulus.roles[0])
        return 0;

    // Find the layered solid torus.
    LayeredSolidTorus* lst = LayeredSolidTorus::formsLayeredSolidTorusTop(
        annulus.tet[0], annulus.roles[0][3], annulus.roles[1][3]);
    if (! lst)
        return 0;

    // Make sure we're not about to create a (0,k) curve.
    Perm<4> lstRoles(
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][0]][annulus.roles[0][1]]),
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][0]][annulus.roles[0][2]]),
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][1]][annulus.roles[0][2]]),
        3);

    if (lst->meridinalCuts(lstRoles[0]) == 0)
        return 0;

    // Make two runs through the full set of tetrahedra.
    // The first run verifies that each tetrahedron is usable.
    // The second run inserts the tetrahedra into avoidTets.
    Tetrahedron<3>* current = annulus.tet[0];
    FacePair currPair = centralEdge;
    FacePair nextPair;
    while (current != lst->base()) {
        // INV: The current tetrahedron is usable.
        // INV: The next two faces to push through are in currPair.

        // Push through to the next tetrahedron.
        nextPair = FacePair(
            current->adjacentFace(currPair.upper()),
            current->adjacentFace(currPair.lower())
            ).complement();
        current = current->adjacentTetrahedron(currPair.upper());
        currPair = nextPair;

        // Make sure this next tetrahedron is usable.
        if (isBad(current, avoidTets))
            return 0;
    }

    // All good!
    current = annulus.tet[0];
    currPair = centralEdge;
    avoidTets.insert(current);
    while (current != lst->base()) {
        // INV: All tetrahedra up to and including current have been added.
        // INV: The next two faces to push through are in currPair.

        // Push through to the next tetrahedron.
        nextPair = FacePair(
            current->adjacentFace(currPair.upper()),
            current->adjacentFace(currPair.lower())
            ).complement();
        current = current->adjacentTetrahedron(currPair.upper());
        currPair = nextPair;

        // Add this next tetrahedron to the list.
        avoidTets.insert(current);
    }

    SatLST* ans = new SatLST(lst, lstRoles);
    ans->annulus_[0] = annulus;
    return ans;
}

void SatTriPrism::adjustSFS(SFSpace& sfs, bool reflect) const {
    if (major_)
        sfs.insertFibre(1, reflect ? -1 : 1);
    else
        sfs.insertFibre(1, reflect ? -2 : 2);
}

SatTriPrism* SatTriPrism::isBlockTriPrism(const SatAnnulus& annulus,
        TetList& avoidTets) {
    SatTriPrism* ans;

    // First try for one of major type.
    if ((ans = isBlockTriPrismMajor(annulus, avoidTets)))
        return ans;

    // Now try the reflected version.
    SatAnnulus altAnnulus = annulus.verticalReflection();
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

SatTriPrism* SatTriPrism::isBlockTriPrismMajor(const SatAnnulus& annulus,
        TetList& avoidTets) {
    if (annulus.tet[0] == annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets) || isBad(annulus.tet[1], avoidTets))
        return 0;
    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][0]) !=
            annulus.tet[1])
        return 0;
    if (annulus.tet[0]->adjacentGluing(annulus.roles[0][0]) *
            annulus.roles[0] * Perm<4>(1, 2) != annulus.roles[1])
        return 0;

    // The two tetrahedra forming the annulus are joined together as
    // expected.  Look for the third tetrahedron.

    Tetrahedron<3>* adj = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][1]);
    if (adj == 0 || adj == annulus.tet[0] || adj == annulus.tet[1])
        return 0;
    if (isBad(adj, avoidTets))
        return 0;

    Perm<4> adjRoles =
        annulus.tet[0]->adjacentGluing(annulus.roles[0][1]) *
        annulus.roles[0] * Perm<4>(0, 3);

    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][1]) != adj)
        return 0;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][1]) *
            annulus.roles[1] * Perm<4>(1, 3, 0, 2) != adjRoles)
        return 0;

    // All three tetrahedra are joined together as expected!

    SatTriPrism* ans = new SatTriPrism(true);

    const Perm<4> pairSwap(1, 0, 3, 2);
    ans->annulus_[0] = annulus;
    ans->annulus_[1].tet[0] = annulus.tet[1];
    ans->annulus_[1].tet[1] = adj;
    ans->annulus_[1].roles[0] = annulus.roles[1] * pairSwap;
    ans->annulus_[1].roles[1] = adjRoles;
    ans->annulus_[2].tet[0] = adj;
    ans->annulus_[2].tet[1] = annulus.tet[0];
    ans->annulus_[2].roles[0] = adjRoles * pairSwap;
    ans->annulus_[2].roles[1] = annulus.roles[0] * pairSwap;

    avoidTets.insert(annulus.tet[0]);
    avoidTets.insert(annulus.tet[1]);
    avoidTets.insert(adj);

    return ans;
}

SatTriPrism* SatTriPrism::insertBlock(Triangulation<3>& tri, bool major) {
    Tetrahedron<3>* a = tri.newTetrahedron();
    Tetrahedron<3>* b = tri.newTetrahedron();
    Tetrahedron<3>* c = tri.newTetrahedron();
    a->join(1, c, Perm<4>(2, 0, 3, 1));
    b->join(1, a, Perm<4>(2, 0, 3, 1));
    c->join(1, b, Perm<4>(2, 0, 3, 1));

    SatTriPrism* ans = new SatTriPrism(major);

    const Perm<4> id;
    const Perm<4> pairSwap(1, 0, 3, 2);
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

void SatCube::adjustSFS(SFSpace& sfs, bool reflect) const {
    sfs.insertFibre(1, reflect ? -2 : 2);
}

SatCube* SatCube::isBlockCube(const SatAnnulus& annulus,
        TetList& avoidTets) {
    if (annulus.tet[0] == annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets) || isBad(annulus.tet[1], avoidTets))
        return 0;

    Tetrahedron<3>* central0 = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][0]);
    Tetrahedron<3>* central1 = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][1]);

    if (central0 == 0 || central0 == annulus.tet[0] ||
            central0 == annulus.tet[1] || isBad(central0, avoidTets))
        return 0;
    if (central1 == 0 || central1 == annulus.tet[0] ||
            central1 == annulus.tet[1] || central1 == central0 ||
            isBad(central0, avoidTets))
        return 0;

    Perm<4> roles0 = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][0]) * annulus.roles[0];
    Perm<4> roles1 = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][1]) * annulus.roles[0];

    // We've got the two central tetrahedra.  Now look for the remaining
    // three boundary tetrahedra.

    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][0]) !=
            central0)
        return 0;
    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][1]) !=
            central1)
        return 0;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][0]) *
            annulus.roles[1] * Perm<4>(3, 2, 1, 0) != roles0)
        return 0;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][1]) *
            annulus.roles[1] * Perm<4>(2, 3, 0, 1) != roles1)
        return 0;

    // We've got the two tetrahedra from the annulus boundary completely
    // sorted out.  Just the two new boundary tetrahedra to go.

    Tetrahedron<3>* bdry2 = central0->adjacentTetrahedron(roles0[1]);
    Perm<4> roles2 = central0->adjacentGluing(roles0[1]) * roles0;

    Tetrahedron<3>* bdry3 = central0->adjacentTetrahedron(roles0[2]);
    Perm<4> roles3 = central0->adjacentGluing(roles0[2]) * roles0;

    if (bdry2 == 0 || bdry2 == annulus.tet[0] || bdry2 == annulus.tet[1] ||
            bdry2 == central0 || bdry2 == central1 ||
            isBad(bdry2, avoidTets))
        return 0;
    if (bdry3 == 0 || bdry3 == annulus.tet[0] || bdry3 == annulus.tet[1] ||
            bdry3 == central0 || bdry3 == central1 || bdry3 == bdry2 ||
            isBad(bdry3, avoidTets))
        return 0;
    if (central1->adjacentTetrahedron(roles1[0]) != bdry2)
        return 0;
    if (central1->adjacentTetrahedron(roles1[2]) != bdry3)
        return 0;
    if (central1->adjacentGluing(roles1[0]) * roles1 != roles2)
        return 0;
    if (central1->adjacentGluing(roles1[2]) * roles1 *
            Perm<4>(1, 0, 3, 2) != roles3)
        return 0;

    // All looking good!

    SatCube* ans = new SatCube();

    const Perm<4> id;
    ans->annulus_[0] = annulus;
    ans->annulus_[1].tet[0] = annulus.tet[1];
    ans->annulus_[1].tet[1] = bdry2;
    ans->annulus_[1].roles[0] = annulus.roles[1] * Perm<4>(1, 0, 3, 2);
    ans->annulus_[1].roles[1] = roles2;
    ans->annulus_[2].tet[0] = bdry2;
    ans->annulus_[2].tet[1] = bdry3;
    ans->annulus_[2].roles[0] = roles2 * Perm<4>(1, 0, 3, 2);
    ans->annulus_[2].roles[1] = roles3 * Perm<4>(2, 3, 0, 1);
    ans->annulus_[3].tet[0] = bdry3;
    ans->annulus_[3].tet[1] = annulus.tet[0];
    ans->annulus_[3].roles[0] = roles3 * Perm<4>(3, 2, 1, 0);
    ans->annulus_[3].roles[1] = annulus.roles[0] * Perm<4>(1, 0, 3, 2);

    avoidTets.insert(annulus.tet[0]);
    avoidTets.insert(annulus.tet[1]);
    avoidTets.insert(bdry2);
    avoidTets.insert(bdry3);
    avoidTets.insert(central0);
    avoidTets.insert(central1);

    return ans;
}

SatCube* SatCube::insertBlock(Triangulation<3>& tri) {
    Tetrahedron<3>* bdry0 = tri.newTetrahedron();
    Tetrahedron<3>* bdry1 = tri.newTetrahedron();
    Tetrahedron<3>* bdry2 = tri.newTetrahedron();
    Tetrahedron<3>* bdry3 = tri.newTetrahedron();
    Tetrahedron<3>* central0 = tri.newTetrahedron();
    Tetrahedron<3>* central1 = tri.newTetrahedron();

    const Perm<4> id;
    bdry0->join(1, central0, id);
    bdry0->join(0, central1, Perm<4>(0, 1));
    bdry1->join(2, central0, Perm<4>(2, 1, 3, 0));
    bdry1->join(0, central1, Perm<4>(0, 3));
    bdry2->join(0, central0, id);
    bdry2->join(1, central1, Perm<4>(0, 1));
    bdry3->join(3, central0, Perm<4>(0, 3, 1, 2));
    bdry3->join(1, central1, Perm<4>(1, 2));

    SatCube* ans = new SatCube();

    ans->annulus_[0].tet[0] = bdry0;
    ans->annulus_[0].tet[1] = bdry1;
    ans->annulus_[1].tet[0] = bdry1;
    ans->annulus_[1].tet[1] = bdry2;
    ans->annulus_[2].tet[0] = bdry2;
    ans->annulus_[2].tet[1] = bdry3;
    ans->annulus_[3].tet[0] = bdry3;
    ans->annulus_[3].tet[1] = bdry0;

    ans->annulus_[0].roles[0] = Perm<4>(0, 1);
    ans->annulus_[0].roles[1] = Perm<4>(2, 0, 3, 1);
    ans->annulus_[1].roles[0] = Perm<4>(1, 2);
    ans->annulus_[1].roles[1] = Perm<4>(0, 1);
    ans->annulus_[2].roles[0] = Perm<4>(2, 3);
    ans->annulus_[2].roles[1] = Perm<4>(0, 3);
    ans->annulus_[3].roles[0] = Perm<4>(1, 3, 0, 2);
    ans->annulus_[3].roles[1] = Perm<4>(2, 3);

    return ans;
}

void SatReflectorStrip::adjustSFS(SFSpace& sfs, bool) const {
    if (! twistedBoundary_)
        sfs.addReflector(false);
}

SatReflectorStrip* SatReflectorStrip::isBlockReflectorStrip(
        const SatAnnulus& annulus, TetList& avoidTets) {
    // Hunt for the initial segment of the reflector strip that lies
    // behind the given annulus.

    if (annulus.tet[0] == annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets) || isBad(annulus.tet[1], avoidTets))
        return 0;

    Tetrahedron<3>* middle = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][0]);
    Perm<4> middleRoles = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][0]) * annulus.roles[0] * Perm<4>(3, 1, 0, 2);

    if (notUnique(middle, annulus.tet[0], annulus.tet[1]) ||
            isBad(middle, avoidTets))

    if (middle != annulus.tet[0]->adjacentTetrahedron(
            annulus.roles[0][1]))
        return 0;
    if (middle != annulus.tet[1]->adjacentTetrahedron(
            annulus.roles[1][0]))
        return 0;
    if (middle != annulus.tet[1]->adjacentTetrahedron(
            annulus.roles[1][1]))
        return 0;
    if (middleRoles != annulus.tet[0]->adjacentGluing(
            annulus.roles[0][1]) * annulus.roles[0] * Perm<4>(1, 3))
        return 0;
    if (middleRoles != annulus.tet[1]->adjacentGluing(
            annulus.roles[1][0]) * annulus.roles[1] * Perm<4>(0, 2, 3, 1))
        return 0;
    if (middleRoles != annulus.tet[1]->adjacentGluing(
            annulus.roles[1][1]) * annulus.roles[1] * Perm<4>(0, 2))
        return 0;

    // We've found the initial segment.
    // Do we just have a segment of length one?
    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][2]) ==
            annulus.tet[1]) {
        // It's either length one or nothing.
        if (annulus.roles[1] == annulus.tet[0]->adjacentGluing(
                annulus.roles[0][2]) * annulus.roles[0] * Perm<4>(0, 1)) {
            // Got one that's untwisted.
            SatReflectorStrip* ans = new SatReflectorStrip(1, false);
            ans->annulus_[0] = annulus;

            avoidTets.insert(annulus.tet[0]);
            avoidTets.insert(middle);
            avoidTets.insert(annulus.tet[1]);

            return ans;
        }

        if (annulus.roles[1] == annulus.tet[0]->adjacentGluing(
                annulus.roles[0][2]) * annulus.roles[0]) {
            // Got one that's twisted.
            SatReflectorStrip* ans = new SatReflectorStrip(1, true);
            ans->annulus_[0] = annulus;

            avoidTets.insert(annulus.tet[0]);
            avoidTets.insert(middle);
            avoidTets.insert(annulus.tet[1]);

            return ans;
        }
        // Nup.  Nothing.
        return 0;
    }

    // If anything, we have a segment of length >= 2.  Start following
    // it around.

    // Make a list storing the tetrahedra from left to right around the
    // boundary ring.  We must use a list and not a set, since we will
    // rely on the tetrahedra being stored in a particular order.
    std::list<Tetrahedron<3>*> foundSoFar;
    foundSoFar.push_back(annulus.tet[0]);
    foundSoFar.push_back(middle);
    foundSoFar.push_back(annulus.tet[1]);

    // Also make a list of tetrahedron vertex roles for the two
    // tetrahedra in each segment that meet the boundary annuli.
    std::list<Perm<4>> rolesSoFar;
    rolesSoFar.push_back(annulus.roles[0]);
    rolesSoFar.push_back(annulus.roles[1]);

    unsigned length = 1;
    bool twisted = false;

    Tetrahedron<3> *nextLeft, *nextMiddle, *nextRight;
    Perm<4> nextLeftRoles, nextMiddleRoles, nextRightRoles;

    while (1) {
        // Run off the right hand side looking for the next tetrahedron.
        nextLeft = foundSoFar.back()->adjacentTetrahedron(
            rolesSoFar.back()[2]);
        nextLeftRoles = foundSoFar.back()->adjacentGluing(
            rolesSoFar.back()[2]) * rolesSoFar.back() * Perm<4>(0, 1);

        if (nextLeft == annulus.tet[0]) {
            // The right _might_ have completed!
            if (nextLeftRoles == annulus.roles[0]) {
                // All good!  An untwisted strip.
            } else if (nextLeftRoles == annulus.roles[0] * Perm<4>(0, 1)) {
                // A complete twisted strip.
                twisted = true;
            } else {
                // Nothing.
                return 0;
            }

            SatReflectorStrip* ans = new SatReflectorStrip(length, twisted);

            std::copy(foundSoFar.begin(), foundSoFar.end(),
                std::inserter(avoidTets, avoidTets.begin()));

            std::list<Tetrahedron<3>*>::const_iterator tit = foundSoFar.begin();
            std::list<Perm<4>>::const_iterator pit = rolesSoFar.begin();
            for (unsigned i = 0; i < length; i++) {
                ans->annulus_[i].tet[0] = *tit++;
                tit++; // Skip the middle tetrahedron from each block.
                ans->annulus_[i].tet[1] = *tit++;

                ans->annulus_[i].roles[0] = *pit++;
                ans->annulus_[i].roles[1] = *pit++;
            }

            return ans;
        }

        // Look for a new adjacent block.
        if (notUnique(nextLeft) ||
                isBad(nextLeft, avoidTets) || isBad(nextLeft, foundSoFar))
            return 0;

        nextMiddle = nextLeft->adjacentTetrahedron(nextLeftRoles[0]);
        nextMiddleRoles = nextLeft->adjacentGluing(
            nextLeftRoles[0]) * nextLeftRoles * Perm<4>(3, 1, 0, 2);

        if (notUnique(nextMiddle, nextLeft) ||
                isBad(nextMiddle, avoidTets) || isBad(nextMiddle, foundSoFar))
            return 0;

        if (nextMiddle != nextLeft->adjacentTetrahedron(nextLeftRoles[1]))
            return 0;
        if (nextMiddleRoles != nextLeft->adjacentGluing(
                nextLeftRoles[1]) * nextLeftRoles * Perm<4>(1, 3))
            return 0;

        nextRight = nextMiddle->adjacentTetrahedron(nextMiddleRoles[0]);
        nextRightRoles = nextMiddle->adjacentGluing(
            nextMiddleRoles[0]) * nextMiddleRoles * Perm<4>(0, 3, 1, 2);

        if (notUnique(nextRight, nextLeft, nextMiddle) ||
                isBad(nextRight, avoidTets) || isBad(nextRight, foundSoFar))
            return 0;

        if (nextRight != nextMiddle->adjacentTetrahedron(nextMiddleRoles[1]))
            return 0;
        if (nextRightRoles != nextMiddle->adjacentGluing(
                nextMiddleRoles[1]) * nextMiddleRoles * Perm<4>(0, 2))
            return 0;

        // Yup, we have a new block.
        foundSoFar.push_back(nextLeft);
        foundSoFar.push_back(nextMiddle);
        foundSoFar.push_back(nextRight);

        rolesSoFar.push_back(nextLeftRoles);
        rolesSoFar.push_back(nextRightRoles);

        length++;
    }

    // We should never get out of the loop this way.
    return 0;
}

SatReflectorStrip* SatReflectorStrip::insertBlock(Triangulation<3>& tri,
        unsigned length, bool twisted) {
    SatReflectorStrip* ans = new SatReflectorStrip(length, twisted);

    const Perm<4> id;
    Tetrahedron<3> *upper, *lower, *middle;
    Tetrahedron<3> *prevRight = 0, *firstLeft = 0;
    for (unsigned i = 0; i < length; i++) {
        // Create the three tetrahedra behind boundary annulus #i.
        upper = tri.newTetrahedron();
        lower = tri.newTetrahedron();
        middle = tri.newTetrahedron();

        upper->join(0, middle, Perm<4>(2, 1, 3, 0));
        lower->join(0, middle, Perm<4>(0, 3, 1, 2));
        upper->join(1, middle, Perm<4>(1, 3));
        lower->join(1, middle, Perm<4>(0, 2));

        if (i == 0)
            firstLeft = upper;
        else
            upper->join(2, prevRight, Perm<4>(0, 1));

        prevRight = lower;

        ans->annulus_[i].tet[0] = upper;
        ans->annulus_[i].tet[1] = lower;
        ans->annulus_[i].roles[0] = id;
        ans->annulus_[i].roles[1] = id;
    }

    if (twisted)
        firstLeft->join(2, prevRight, id);
    else
        firstLeft->join(2, prevRight, Perm<4>(0, 1));

    return ans;
}

void SatLayering::adjustSFS(SFSpace& sfs, bool reflect) const {
    if (overHorizontal_)
        sfs.insertFibre(1, reflect ? -2 : 2);

    // Over the diagonal, there is no change at all.
}

SatLayering* SatLayering::isBlockLayering(const SatAnnulus& annulus,
        TetList& avoidTets) {
    // Must be a common usable tetrahedron.
    if (annulus.tet[0] != annulus.tet[1])
        return 0;
    if (isBad(annulus.tet[0], avoidTets))
        return 0;

    // Is it a layering over the horizontal edge?
    if (annulus.roles[0][0] == annulus.roles[1][2] &&
            annulus.roles[0][2] == annulus.roles[1][0]) {
        avoidTets.insert(annulus.tet[0]);

        SatLayering* ans = new SatLayering(true);
        ans->annulus_[0] = annulus;
        ans->annulus_[1].tet[0] = ans->annulus_[1].tet[1] = annulus.tet[0];
        ans->annulus_[1].roles[0] = annulus.roles[1] * Perm<4>(1, 0, 3, 2);
        ans->annulus_[1].roles[1] = annulus.roles[0] * Perm<4>(1, 0, 3, 2);

        return ans;
    }

    // Is it a layering over the diagonal edge?
    if (annulus.roles[0][1] == annulus.roles[1][2] &&
            annulus.roles[0][2] == annulus.roles[1][1]) {
        avoidTets.insert(annulus.tet[0]);

        SatLayering* ans = new SatLayering(false);
        ans->annulus_[0] = annulus;
        ans->annulus_[1].tet[0] = ans->annulus_[1].tet[1] = annulus.tet[0];
        ans->annulus_[1].roles[0] = annulus.roles[1] * Perm<4>(1, 0, 3, 2);
        ans->annulus_[1].roles[1] = annulus.roles[0] * Perm<4>(1, 0, 3, 2);

        return ans;
    }

    // No layering.
    return 0;
}

} // namespace regina

