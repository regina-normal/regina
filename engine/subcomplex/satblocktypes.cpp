
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
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

std::weak_ordering SatBlock::operator <=> (const SatBlock& rhs) const {
    const auto* tri1 = dynamic_cast<const SatTriPrism*>(this);
    const auto* tri2 = dynamic_cast<const SatTriPrism*>(&rhs);
    if (tri1 && ! tri2)
        return std::weak_ordering::less;
    if (tri2 && ! tri1)
        return std::weak_ordering::greater;
    if (tri1 && tri2) {
        // Major first, then minor.
        if (tri1->isMajor() && ! tri2->isMajor())
            return std::weak_ordering::less;
        if (tri2->isMajor() && ! tri1->isMajor())
            return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }

    const auto* cube1 = dynamic_cast<const SatCube*>(this);
    const auto* cube2 = dynamic_cast<const SatCube*>(&rhs);
    if (cube1 && ! cube2)
        return std::weak_ordering::less;
    if (cube2 && ! cube1)
        return std::weak_ordering::greater;
    if (cube1 && cube2) {
        // All cubes are considered equal.
        return std::weak_ordering::equivalent;
    }

    const auto* ref1 = dynamic_cast<const SatReflectorStrip*>(this);
    const auto* ref2 = dynamic_cast<const SatReflectorStrip*>(&rhs);
    if (ref1 && ! ref2)
        return std::weak_ordering::less;
    if (ref2 && ! ref1)
        return std::weak_ordering::greater;
    if (ref1 && ref2) {
        // Always put untwisted before twisted.
        if (ref1->twistedBoundary() && ! ref2->twistedBoundary())
            return std::weak_ordering::greater;
        if (ref2->twistedBoundary() && ! ref1->twistedBoundary())
            return std::weak_ordering::less;
        return (ref1->countAnnuli() <=> ref2->countAnnuli());
    }

    const auto* lst1 = dynamic_cast<const SatLST*>(this);
    const auto* lst2 = dynamic_cast<const SatLST*>(&rhs);
    if (lst1 && ! lst2)
        return std::weak_ordering::less;
    if (lst2 && ! lst1)
        return std::weak_ordering::greater;
    if (lst1 && lst2) {
        // Order first by LST parameters, then by roles.
        if (lst1->lst().meridinalCuts(2) < lst2->lst().meridinalCuts(2))
            return std::weak_ordering::less;
        if (lst1->lst().meridinalCuts(2) > lst2->lst().meridinalCuts(2))
            return std::weak_ordering::greater;
        if (lst1->lst().meridinalCuts(1) < lst2->lst().meridinalCuts(1))
            return std::weak_ordering::less;
        if (lst1->lst().meridinalCuts(1) > lst2->lst().meridinalCuts(1))
            return std::weak_ordering::greater;
        if (lst1->lst().meridinalCuts(0) < lst2->lst().meridinalCuts(0))
            return std::weak_ordering::less;
        if (lst1->lst().meridinalCuts(0) > lst2->lst().meridinalCuts(0))
            return std::weak_ordering::greater;

        // Sorts by which edge group is joined to the vertical annulus
        // edges, then horizontal, then diagonal (though we won't bother
        // testing diagonal, since by that stage we will know the roles
        // permutations to be equal).
        if (lst1->roles()[0] < lst2->roles()[0])
            return std::weak_ordering::less;
        if (lst1->roles()[0] > lst2->roles()[0])
            return std::weak_ordering::greater;
        if (lst1->roles()[1] < lst2->roles()[1])
            return std::weak_ordering::less;
        if (lst1->roles()[1] > lst2->roles()[1])
            return std::weak_ordering::greater;

        // All equal.
        return std::weak_ordering::equivalent;
    }

    const auto* mob1 = dynamic_cast<const SatMobius*>(this);
    const auto* mob2 = dynamic_cast<const SatMobius*>(&rhs);
    if (mob1 && ! mob2)
        return std::weak_ordering::less;
    if (mob2 && ! mob1)
        return std::weak_ordering::greater;
    if (mob1 && mob2) {
        // Order by position in _descending_ order (vertical first, then
        // horizontal, then finally diagonal).
        return (mob2->position() <=> mob1->position()); // note reversal
    }

    const auto* layer1 = dynamic_cast<const SatLayering*>(this);
    const auto* layer2 = dynamic_cast<const SatLayering*>(&rhs);
    if (layer1 && ! layer2)
        return std::weak_ordering::less;
    if (layer2 && ! layer1)
        return std::weak_ordering::greater;
    if (layer1 && layer2) {
        // Horizontal, then diagonal.
        if (layer1->overHorizontal() && ! layer2->overHorizontal())
            return std::weak_ordering::less;
        if (layer2->overHorizontal() && ! layer1->overHorizontal())
            return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }

    return std::weak_ordering::equivalent;
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
    // Format was like: Saturated Mobius band, boundary on vertical edge
    out << "Mobius(";
    if (position_ == 0)
        out << "diag"; // roles swap 1,2
    else if (position_ == 1)
        out << "horiz"; // roles swap 0,2
    else if (position_ == 2)
        out << "vert"; // roles swap 0,1
    else
        out << "invalid";
    out << ") {triangle "
        << annulus_[0].tet[0]->triangle(annulus_[0].roles[0][3])->index()
        << '}';
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

SatMobius* SatMobius::beginsRegion(const SatAnnulus& annulus, TetList&) {
    // The two tetrahedra must be joined together along the annulus triangles.

    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][3]) !=
            annulus.tet[1])
        return nullptr;

    Perm<4> annulusGluing = annulus.roles[1].inverse() *
        annulus.tet[0]->adjacentGluing(annulus.roles[0][3]) *
        annulus.roles[0];

    if (annulusGluing[3] != 3)
        return nullptr;

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
        return nullptr;
    }

    // Got it!

    auto* ans = new SatMobius(position);
    ans->annulus_[0] = annulus;
    return ans;
}

void SatLST::adjustSFS(SFSpace& sfs, bool reflect) const {
    long cutsVert = lst_.meridinalCuts(roles_[0]);
    long cutsHoriz = lst_.meridinalCuts(roles_[1]);
    if (roles_[2] == 2) {
        // Most cuts are on the diagonal, which means the meridinal
        // curve is negative.
        cutsHoriz = -cutsHoriz;
    }

    sfs.insertFibre(cutsVert, reflect ? -cutsHoriz : cutsHoriz);
}

void SatLST::writeTextShort(std::ostream& out) const {
    // Format was like: Saturated (1, 2, 3) layered solid torus
    out << "LST("
        << lst_.meridinalCuts(0) << ','
        << lst_.meridinalCuts(1) << ','
        << lst_.meridinalCuts(2) << ") {"
        << lst_.topLevel()->index();
    if (lst_.topLevel() != lst_.base())
        out << ".." << lst_.base()->index();
    out << '}';
}

void SatLST::writeAbbr(std::ostream& out, bool tex) const {
    out << (tex ? "\\mathrm{LST}_{" : "LST(")
        << lst_.meridinalCuts(0) << ", "
        << lst_.meridinalCuts(1) << ", "
        << lst_.meridinalCuts(2) << (tex ? '}' : ')');
}

void SatLST::transform(const Triangulation<3>& originalTri,
        const Isomorphism<3>& iso, const Triangulation<3>& newTri) {
    // Start with the parent implementation.
    SatBlock::transform(originalTri, iso, newTri);

    // Transform the layered solid torus also.
    lst_.transform(originalTri, iso, newTri);
}

SatLST* SatLST::beginsRegion(const SatAnnulus& annulus, TetList& avoidTets) {
    // Do we move to a common usable tetrahedron?

    if (annulus.tet[0] != annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(annulus.tet[0]))
        return nullptr;

    // Is it a layering?

    // Here we find the endpoints of the edge from which the two layered
    // triangles fold out.
    FacePair centralEdge =
        FacePair(annulus.roles[0][3], annulus.roles[1][3]).complement();

    if (annulus.roles[1] !=
            Perm<4>(annulus.roles[0][3], annulus.roles[1][3]) *
            Perm<4>(centralEdge.upper(), centralEdge.lower()) *
            annulus.roles[0])
        return nullptr;

    // Find the layered solid torus.
    auto lst = LayeredSolidTorus::recogniseFromTop(
        annulus.tet[0], annulus.roles[0][3], annulus.roles[1][3]);
    if (! lst)
        return nullptr;

    // Make sure we're not about to create a (0,k) curve.
    Perm<3> lstRoles(
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][0]][annulus.roles[0][1]]),
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][0]][annulus.roles[0][2]]),
        lst->topEdgeGroup(
            Edge<3>::edgeNumber[annulus.roles[0][1]][annulus.roles[0][2]]));

    if (lst->meridinalCuts(lstRoles[0]) == 0)
        return nullptr;

    // Make two runs through the full set of tetrahedra.
    // The first run verifies that each tetrahedron is usable.
    // The second run inserts the tetrahedra into avoidTets.
    const Tetrahedron<3>* current = annulus.tet[0];
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
        if (avoidTets.contains(current))
            return nullptr;
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

    auto* ans = new SatLST(*lst, lstRoles);
    ans->annulus_[0] = annulus;
    return ans;
}

void SatTriPrism::adjustSFS(SFSpace& sfs, bool reflect) const {
    if (major_)
        sfs.insertFibre(1, reflect ? -1 : 1);
    else
        sfs.insertFibre(1, reflect ? -2 : 2);
}

SatTriPrism* SatTriPrism::beginsRegion(const SatAnnulus& annulus,
        TetList& avoidTets) {
    SatTriPrism* ans;

    // First try for one of major type.
    if ((ans = beginsRegionMajor(annulus, avoidTets)))
        return ans;

    // Now try the reflected version.
    SatAnnulus altAnnulus = annulus.verticalReflection();
    if ((ans = beginsRegionMajor(altAnnulus, avoidTets))) {
        // Reflect it back again but mark it as a minor variant.
        ans->major_ = false;

        ans->annulus_[0].reflectVertical();
        ans->annulus_[1].reflectVertical();
        ans->annulus_[2].reflectVertical();

        return ans;
    }

    // Neither variant was found.
    return nullptr;
}

SatTriPrism* SatTriPrism::beginsRegionMajor(const SatAnnulus& annulus,
        TetList& avoidTets) {
    if (annulus.tet[0] == annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(annulus.tet[0]) ||
            avoidTets.contains(annulus.tet[1]))
        return nullptr;
    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][0]) !=
            annulus.tet[1])
        return nullptr;
    if (annulus.tet[0]->adjacentGluing(annulus.roles[0][0]) *
            annulus.roles[0] * Perm<4>(1, 2) != annulus.roles[1])
        return nullptr;

    // The two tetrahedra forming the annulus are joined together as
    // expected.  Look for the third tetrahedron.

    Tetrahedron<3>* adj = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][1]);
    if ((! adj) || adj == annulus.tet[0] || adj == annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(adj))
        return nullptr;

    Perm<4> adjRoles =
        annulus.tet[0]->adjacentGluing(annulus.roles[0][1]) *
        annulus.roles[0] * Perm<4>(0, 3);

    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][1]) != adj)
        return nullptr;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][1]) *
            annulus.roles[1] * Perm<4>(1, 3, 0, 2) != adjRoles)
        return nullptr;

    // All three tetrahedra are joined together as expected!

    auto* ans = new SatTriPrism(true);

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

SatBlockModel SatTriPrism::model(bool major) {
    auto* tri = new Triangulation<3>;
    auto [a, b, c] = tri->newTetrahedra<3>();
    a->join(1, c, Perm<4>(2, 0, 3, 1));
    b->join(1, a, Perm<4>(2, 0, 3, 1));
    c->join(1, b, Perm<4>(2, 0, 3, 1));

    auto* ans = new SatTriPrism(major);

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

    return ans->modelWith(tri);
}

void SatCube::adjustSFS(SFSpace& sfs, bool reflect) const {
    sfs.insertFibre(1, reflect ? -2 : 2);
}

SatCube* SatCube::beginsRegion(const SatAnnulus& annulus, TetList& avoidTets) {
    if (annulus.tet[0] == annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(annulus.tet[0]) ||
            avoidTets.contains(annulus.tet[1]))
        return nullptr;

    Tetrahedron<3>* central0 = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][0]);
    Tetrahedron<3>* central1 = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][1]);

    if ((! central0) || central0 == annulus.tet[0] ||
            central0 == annulus.tet[1] || avoidTets.contains(central0))
        return nullptr;
    if ((! central1) || central1 == annulus.tet[0] ||
            central1 == annulus.tet[1] || central1 == central0 ||
            avoidTets.contains(central0))
        return nullptr;

    Perm<4> roles0 = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][0]) * annulus.roles[0];
    Perm<4> roles1 = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][1]) * annulus.roles[0];

    // We've got the two central tetrahedra.  Now look for the remaining
    // three boundary tetrahedra.

    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][0]) !=
            central0)
        return nullptr;
    if (annulus.tet[1]->adjacentTetrahedron(annulus.roles[1][1]) !=
            central1)
        return nullptr;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][0]) *
            annulus.roles[1] * Perm<4>(3, 2, 1, 0) != roles0)
        return nullptr;
    if (annulus.tet[1]->adjacentGluing(annulus.roles[1][1]) *
            annulus.roles[1] * Perm<4>(2, 3, 0, 1) != roles1)
        return nullptr;

    // We've got the two tetrahedra from the annulus boundary completely
    // sorted out.  Just the two new boundary tetrahedra to go.

    Tetrahedron<3>* bdry2 = central0->adjacentTetrahedron(roles0[1]);
    Perm<4> roles2 = central0->adjacentGluing(roles0[1]) * roles0;

    Tetrahedron<3>* bdry3 = central0->adjacentTetrahedron(roles0[2]);
    Perm<4> roles3 = central0->adjacentGluing(roles0[2]) * roles0;

    if ((! bdry2) || bdry2 == annulus.tet[0] || bdry2 == annulus.tet[1] ||
            bdry2 == central0 || bdry2 == central1 ||
            avoidTets.contains(bdry2))
        return nullptr;
    if ((! bdry3) || bdry3 == annulus.tet[0] || bdry3 == annulus.tet[1] ||
            bdry3 == central0 || bdry3 == central1 || bdry3 == bdry2 ||
            avoidTets.contains(bdry3))
        return nullptr;
    if (central1->adjacentTetrahedron(roles1[0]) != bdry2)
        return nullptr;
    if (central1->adjacentTetrahedron(roles1[2]) != bdry3)
        return nullptr;
    if (central1->adjacentGluing(roles1[0]) * roles1 != roles2)
        return nullptr;
    if (central1->adjacentGluing(roles1[2]) * roles1 *
            Perm<4>(1, 0, 3, 2) != roles3)
        return nullptr;

    // All looking good!

    auto* ans = new SatCube();

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

SatBlockModel SatCube::model() {
    auto* tri = new Triangulation<3>;
    auto bdry = tri->newTetrahedra<4>();
    auto central = tri->newTetrahedra<2>();

    const Perm<4> id;
    bdry[0]->join(1, central[0], id);
    bdry[0]->join(0, central[1], Perm<4>(0, 1));
    bdry[1]->join(2, central[0], Perm<4>(2, 1, 3, 0));
    bdry[1]->join(0, central[1], Perm<4>(0, 3));
    bdry[2]->join(0, central[0], id);
    bdry[2]->join(1, central[1], Perm<4>(0, 1));
    bdry[3]->join(3, central[0], Perm<4>(0, 3, 1, 2));
    bdry[3]->join(1, central[1], Perm<4>(1, 2));

    auto* ans = new SatCube();

    ans->annulus_[0].tet[0] = bdry[0];
    ans->annulus_[0].tet[1] = bdry[1];
    ans->annulus_[1].tet[0] = bdry[1];
    ans->annulus_[1].tet[1] = bdry[2];
    ans->annulus_[2].tet[0] = bdry[2];
    ans->annulus_[2].tet[1] = bdry[3];
    ans->annulus_[3].tet[0] = bdry[3];
    ans->annulus_[3].tet[1] = bdry[0];

    ans->annulus_[0].roles[0] = Perm<4>(0, 1);
    ans->annulus_[0].roles[1] = Perm<4>(2, 0, 3, 1);
    ans->annulus_[1].roles[0] = Perm<4>(1, 2);
    ans->annulus_[1].roles[1] = Perm<4>(0, 1);
    ans->annulus_[2].roles[0] = Perm<4>(2, 3);
    ans->annulus_[2].roles[1] = Perm<4>(0, 3);
    ans->annulus_[3].roles[0] = Perm<4>(1, 3, 0, 2);
    ans->annulus_[3].roles[1] = Perm<4>(2, 3);

    return ans->modelWith(tri);
}

void SatReflectorStrip::adjustSFS(SFSpace& sfs, bool) const {
    if (! twistedBoundary_)
        sfs.addReflector(false);
}

SatReflectorStrip* SatReflectorStrip::beginsRegion(const SatAnnulus& annulus,
        TetList& avoidTets) {
    // Hunt for the initial segment of the reflector strip that lies
    // behind the given annulus.

    if (annulus.tet[0] == annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(annulus.tet[0]) ||
            avoidTets.contains(annulus.tet[1]))
        return nullptr;

    Tetrahedron<3>* middle = annulus.tet[0]->adjacentTetrahedron(
        annulus.roles[0][0]);
    Perm<4> middleRoles = annulus.tet[0]->adjacentGluing(
        annulus.roles[0][0]) * annulus.roles[0] * Perm<4>(3, 1, 0, 2);

    if (notUnique(middle, annulus.tet[0], annulus.tet[1]) ||
            avoidTets.contains(middle))
        return nullptr;
    if (middle != annulus.tet[0]->adjacentTetrahedron(
            annulus.roles[0][1]))
        return nullptr;
    if (middle != annulus.tet[1]->adjacentTetrahedron(
            annulus.roles[1][0]))
        return nullptr;
    if (middle != annulus.tet[1]->adjacentTetrahedron(
            annulus.roles[1][1]))
        return nullptr;
    if (middleRoles != annulus.tet[0]->adjacentGluing(
            annulus.roles[0][1]) * annulus.roles[0] * Perm<4>(1, 3))
        return nullptr;
    if (middleRoles != annulus.tet[1]->adjacentGluing(
            annulus.roles[1][0]) * annulus.roles[1] * Perm<4>(0, 2, 3, 1))
        return nullptr;
    if (middleRoles != annulus.tet[1]->adjacentGluing(
            annulus.roles[1][1]) * annulus.roles[1] * Perm<4>(0, 2))
        return nullptr;

    // We've found the initial segment.
    // Do we just have a segment of length one?
    if (annulus.tet[0]->adjacentTetrahedron(annulus.roles[0][2]) ==
            annulus.tet[1]) {
        // It's either length one or nothing.
        if (annulus.roles[1] == annulus.tet[0]->adjacentGluing(
                annulus.roles[0][2]) * annulus.roles[0] * Perm<4>(0, 1)) {
            // Got one that's untwisted.
            auto* ans = new SatReflectorStrip(1, false);
            ans->annulus_[0] = annulus;

            avoidTets.insert(annulus.tet[0]);
            avoidTets.insert(middle);
            avoidTets.insert(annulus.tet[1]);

            return ans;
        }

        if (annulus.roles[1] == annulus.tet[0]->adjacentGluing(
                annulus.roles[0][2]) * annulus.roles[0]) {
            // Got one that's twisted.
            auto* ans = new SatReflectorStrip(1, true);
            ans->annulus_[0] = annulus;

            avoidTets.insert(annulus.tet[0]);
            avoidTets.insert(middle);
            avoidTets.insert(annulus.tet[1]);

            return ans;
        }
        // Nup.  Nothing.
        return nullptr;
    }

    // If anything, we have a segment of length >= 2.  Start following
    // it around.

    // Make a list storing the tetrahedra from left to right around the
    // boundary ring.  We must use a list and not a set, since we will
    // rely on the tetrahedra being stored in a particular order.
    std::list<const Tetrahedron<3>*> foundSoFar;
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

    while (true) {
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
                return nullptr;
            }

            auto* ans = new SatReflectorStrip(length, twisted);

            std::copy(foundSoFar.begin(), foundSoFar.end(),
                std::inserter(avoidTets, avoidTets.begin()));

            auto tit = foundSoFar.begin();
            auto pit = rolesSoFar.begin();
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
        if (notUnique(nextLeft) || avoidTets.contains(nextLeft) ||
                std::find(foundSoFar.begin(), foundSoFar.end(), nextLeft) !=
                foundSoFar.end())
            return nullptr;

        nextMiddle = nextLeft->adjacentTetrahedron(nextLeftRoles[0]);
        nextMiddleRoles = nextLeft->adjacentGluing(
            nextLeftRoles[0]) * nextLeftRoles * Perm<4>(3, 1, 0, 2);

        if (notUnique(nextMiddle, nextLeft) || avoidTets.contains(nextMiddle) ||
                std::find(foundSoFar.begin(), foundSoFar.end(), nextMiddle) !=
                foundSoFar.end())
            return nullptr;

        if (nextMiddle != nextLeft->adjacentTetrahedron(nextLeftRoles[1]))
            return nullptr;
        if (nextMiddleRoles != nextLeft->adjacentGluing(
                nextLeftRoles[1]) * nextLeftRoles * Perm<4>(1, 3))
            return nullptr;

        nextRight = nextMiddle->adjacentTetrahedron(nextMiddleRoles[0]);
        nextRightRoles = nextMiddle->adjacentGluing(
            nextMiddleRoles[0]) * nextMiddleRoles * Perm<4>(0, 3, 1, 2);

        if (notUnique(nextRight, nextLeft, nextMiddle) ||
                avoidTets.contains(nextRight) ||
                std::find(foundSoFar.begin(), foundSoFar.end(), nextRight) !=
                foundSoFar.end())
            return nullptr;

        if (nextRight != nextMiddle->adjacentTetrahedron(nextMiddleRoles[1]))
            return nullptr;
        if (nextRightRoles != nextMiddle->adjacentGluing(
                nextMiddleRoles[1]) * nextMiddleRoles * Perm<4>(0, 2))
            return nullptr;

        // Yup, we have a new block.
        foundSoFar.push_back(nextLeft);
        foundSoFar.push_back(nextMiddle);
        foundSoFar.push_back(nextRight);

        rolesSoFar.push_back(nextLeftRoles);
        rolesSoFar.push_back(nextRightRoles);

        length++;
    }

    // We should never get out of the loop this way.
    return nullptr;
}

SatBlockModel SatReflectorStrip::model(unsigned length, bool twisted) {
    auto* tri = new Triangulation<3>;
    auto* ans = new SatReflectorStrip(length, twisted);

    const Perm<4> id;
    Tetrahedron<3> *prevRight = nullptr, *firstLeft = nullptr;
    for (unsigned i = 0; i < length; i++) {
        // Create the three tetrahedra behind boundary annulus #i.
        auto [upper, lower, middle] = tri->newTetrahedra<3>();

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

    return ans->modelWith(tri);
}

void SatReflectorStrip::writeTextShort(std::ostream& out) const {
    // Format was like: Saturated reflector strip of length 1
    out << "Reflector(" << countAnnuli();
    if (twistedBoundary())
        out << ", twisted";
    out << ") {";
    for (size_t i = 0; i < countAnnuli(); ++i) {
        if (i > 0)
            out << '|';
        out << annulus_[i].tet[0]->index() << ','
            << annulus_[i].tet[0]->adjacentTetrahedron(
                annulus_[i].roles[0][0])->index() << ','
            << annulus_[i].tet[1]->index();
    }
    out << '}';
}

void SatLayering::adjustSFS(SFSpace& sfs, bool reflect) const {
    if (overHorizontal_)
        sfs.insertFibre(1, reflect ? -2 : 2);

    // Over the diagonal, there is no change at all.
}

SatLayering* SatLayering::beginsRegion(const SatAnnulus& annulus,
        TetList& avoidTets) {
    // Must be a common usable tetrahedron.
    if (annulus.tet[0] != annulus.tet[1])
        return nullptr;
    if (avoidTets.contains(annulus.tet[0]))
        return nullptr;

    // Is it a layering over the horizontal edge?
    if (annulus.roles[0][0] == annulus.roles[1][2] &&
            annulus.roles[0][2] == annulus.roles[1][0]) {
        avoidTets.insert(annulus.tet[0]);

        auto* ans = new SatLayering(true);
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

        auto* ans = new SatLayering(false);
        ans->annulus_[0] = annulus;
        ans->annulus_[1].tet[0] = ans->annulus_[1].tet[1] = annulus.tet[0];
        ans->annulus_[1].roles[0] = annulus.roles[1] * Perm<4>(1, 0, 3, 2);
        ans->annulus_[1].roles[1] = annulus.roles[0] * Perm<4>(1, 0, 3, 2);

        return ans;
    }

    // No layering.
    return nullptr;
}

} // namespace regina

