
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "triangulation/facepair.h"
#include "triangulation/dim3.h"
#include "subcomplex/layeredsolidtorus.h"

namespace regina {

void LayeredSolidTorus::transform(const Triangulation<3>& /* originalTri */,
        const Isomorphism<3>& iso, const Triangulation<3>& newTri) {
    size_t baseTetID = base_->index();
    size_t topTetID = top_->index();

    // Data members size_ and meridinalCuts remain unchanged.

    // Transform edge numbers:
    baseEdge_ = iso.facePerm(baseTetID).pairs() * baseEdge_;
    topEdge_ = iso.facePerm(topTetID).pairs() * topEdge_;

    // Transform face numbers:
    FacePair newBaseFace = FacePair(
        iso.facePerm(baseTetID)[baseFace_.lower()],
        iso.facePerm(baseTetID)[baseFace_.upper()]);
    if (newBaseFace.lower() != iso.facePerm(baseTetID)[baseFace_.lower()]) {
        // Swap images of 1,2 and images of 3,5.
        baseEdge_ = baseEdge_ * Perm<6>(0, 2, 1, 5, 4, 3);
    }
    baseFace_ = newBaseFace;

    FacePair newTopFace = FacePair(
        iso.facePerm(topTetID)[topFace_.lower()],
        iso.facePerm(topTetID)[topFace_.upper()]);
    if (newTopFace.lower() != iso.facePerm(topTetID)[topFace_.lower()]) {
        // Swap images of 0,1, images of 2,3 and images of 4,5, except for
        // the pair that only contains one top edge.
        // We do this by swapping all three pairs, and then swapping the
        // unwanted pair back again.
        topEdge_ = Perm<6>(newTopFace.oppositeEdge(), newTopFace.commonEdge()) *
            topEdge_ * Perm<6>(1,0, 3,2, 5,4);
    }
    topFace_ = newTopFace;

    // Transform tetrahedra:
    base_ = newTri.tetrahedron(iso.tetImage(baseTetID));
    top_ = newTri.tetrahedron(iso.tetImage(topTetID));
}

std::unique_ptr<LayeredSolidTorus> LayeredSolidTorus::recogniseFromBase(
        const Tetrahedron<3>* tet) {
    int baseFace1;
    int baseFace2 = -1;
    Perm<4> basePerm;
    bool okay;
    int i;
    for (baseFace1 = 0; baseFace1 < 3; baseFace1++)
        if (tet->adjacentTetrahedron(baseFace1) == tet) {
            // This tetrahedron is glued to itself.
            baseFace2 = tet->adjacentFace(baseFace1);
            basePerm = tet->adjacentGluing(baseFace1);
            okay = true;
            for (i = 0; i < 4; i++) {
                if (i == baseFace1 || i == baseFace2)
                    continue;
                // No vertex should be glued to itself!
                if (basePerm[i] == i) {
                    okay = false;
                    break;
                }
            }
            if (okay && basePerm[baseFace2] != baseFace1)
                break;
            else
                baseFace2 = -1;
        }

    if (baseFace2 == -1)
        return nullptr;

    // We have a layered solid torus!!
    // Fill in the details for the bottom layer.
    std::unique_ptr<LayeredSolidTorus> ans(new LayeredSolidTorus());
    ans->size_ = 1;
    ans->base_ = tet;

    ans->baseFace_ = FacePair(baseFace1, baseFace2);
    // Currently basePerm maps baseFace1 -> baseFace2.
    // Make it so that basePerm maps baseFace_.lower() -> baseFace_.upper().
    if (baseFace1 > baseFace2)
        basePerm = basePerm.inverse();

    ans->topFace_ = ans->baseFace_.complement();
    ans->top_ = tet;

    if (basePerm[ans->baseFace_.upper()] == ans->topFace_.lower()) {
        ans->baseEdge_ = Perm<6>(
            ans->baseFace_.oppositeEdge(),
            Edge<3>::edgeNumber[ans->topFace_.upper()][ans->baseFace_.upper()],
            Edge<3>::edgeNumber[ans->topFace_.lower()][ans->baseFace_.lower()],
            Edge<3>::edgeNumber[ans->topFace_.lower()][ans->baseFace_.upper()],
            ans->baseFace_.commonEdge(),
            Edge<3>::edgeNumber[ans->topFace_.upper()][ans->baseFace_.lower()]);

        ans->topEdge_ = ans->baseEdge_ * Perm<6>(5, 3, 1, 2, 0, 4);
    } else {
        ans->baseEdge_ = Perm<6>(
            ans->baseFace_.oppositeEdge(),
            Edge<3>::edgeNumber[ans->topFace_.lower()][ans->baseFace_.upper()],
            Edge<3>::edgeNumber[ans->topFace_.upper()][ans->baseFace_.lower()],
            Edge<3>::edgeNumber[ans->topFace_.upper()][ans->baseFace_.upper()],
            ans->baseFace_.commonEdge(),
            Edge<3>::edgeNumber[ans->topFace_.lower()][ans->baseFace_.lower()]);

        ans->topEdge_ = ans->baseEdge_ * Perm<6>(3, 5, 2, 1, 0, 4);
    }

    ans->meridinalCuts_[0] = 1;
    ans->meridinalCuts_[1] = 2;
    ans->meridinalCuts_[2] = 3;

    // Now run through and look for layers to add to the torus.
    int adjFace[2]; // Faces of adjacent tetrahedron glued to the torus.
    int adjEdge; // Layering edge of the adjacent tetrahedron.
    int layerOnEdge[2]; // The two edges of the current top tetrahedron
                        // corresponding to adjEdge.
    int newTopEdge; // New boundary edge of degree 1 in the torus.
    Perm<4> adjPerm[2];
    int layerOnGroup;
    while (true) {
        // Is there a new layer?
        tet = ans->top_->adjacentTetrahedron(ans->topFace_.lower());
        if (tet == nullptr || tet == ans->top_ ||
                tet != ans->top_->adjacentTetrahedron(ans->topFace_.upper()))
            break;

        // There is a new tetrahedron glued to both torus boundary triangles.
        adjPerm[0] = ans->top_->adjacentGluing(ans->topFace_.lower());
        adjPerm[1] = ans->top_->adjacentGluing(ans->topFace_.upper());
        if (adjPerm[0].sign() != adjPerm[1].sign())
            break;

        // See what the new boundary edge would be.
        adjFace[0] = ans->top_->adjacentFace(ans->topFace_.lower());
        adjFace[1] = ans->top_->adjacentFace(ans->topFace_.upper());
        newTopEdge = Edge<3>::edgeNumber[adjFace[0]][adjFace[1]];
        adjEdge = 5 - newTopEdge;

        // See which edges of the current top tetrahedron are being
        // layered upon.
        layerOnEdge[0] = adjPerm[0].inverse().pairs()[adjEdge];
        layerOnEdge[1] = adjPerm[1].inverse().pairs()[adjEdge];
        if (layerOnEdge[0] != layerOnEdge[1] &&
                layerOnEdge[0] + layerOnEdge[1] != 5)
            break;

        // We have a new layer!

        // Before changing anything else, rearrange the topEdge and
        // meridinalCuts arrays.
        layerOnGroup = ans->topEdge_.pre(layerOnEdge[0]) / 2;
        Perm<6> edgePerm = adjPerm[0].pairs();
        switch(layerOnGroup) {
            case 0:
                // p q r  ->  q r q+r
                ans->meridinalCuts_[0] = ans->meridinalCuts_[1];
                ans->meridinalCuts_[1] = ans->meridinalCuts_[2];
                ans->meridinalCuts_[2] = ans->meridinalCuts_[0] +
                    ans->meridinalCuts_[1];
                ans->topEdge_ = edgePerm * ans->topEdge_ *
                    Perm<6>(2, 3, 4, 5, 1, 0);
                break;
            case 1:
                // p q r  ->  p r p+r
                ans->meridinalCuts_[1] = ans->meridinalCuts_[2];
                ans->meridinalCuts_[2] = ans->meridinalCuts_[0] +
                    ans->meridinalCuts_[1];
                ans->topEdge_ = edgePerm * ans->topEdge_ *
                    Perm<6>(0, 1, 4, 5, 3, 2);
                break;
            case 2:
                if (ans->meridinalCuts_[1] - ans->meridinalCuts_[0] <
                        ans->meridinalCuts_[0]) {
                    // p q r  ->  q-p p q
                    ans->meridinalCuts_[2] = ans->meridinalCuts_[1];
                    ans->meridinalCuts_[1] = ans->meridinalCuts_[0];
                    ans->meridinalCuts_[0] = ans->meridinalCuts_[2] -
                        ans->meridinalCuts_[1];
                    ans->topEdge_ = edgePerm * ans->topEdge_ *
                        Perm<6>(5, 4, 0, 1, 2, 3);
                } else {
                    // p q r  ->  p q-p q
                    ans->meridinalCuts_[2] = ans->meridinalCuts_[1];
                    ans->meridinalCuts_[1] = ans->meridinalCuts_[2] -
                        ans->meridinalCuts_[0];
                    ans->topEdge_ = edgePerm * ans->topEdge_ *
                        Perm<6>(0, 1, 5, 4, 2, 3);
                }
                break;
        }

        ans->topFace_ = FacePair(
            Edge<3>::edgeVertex[adjEdge][0],
            Edge<3>::edgeVertex[adjEdge][1]);

        // Massage the indices in topEdge to match topFace.
        for (i = 0; i < 6; i += 2) {
            // Make sure ans->topEdge[i] is in face ans->topFace.lower().
            // That is: neither of the edge vertices can match the face number.
            if (    ans->topFace_.lower() ==
                        Edge<3>::edgeVertex[ans->topEdge_[i]][0] ||
                    ans->topFace_.lower() ==
                        Edge<3>::edgeVertex[ans->topEdge_[i]][1]) {
                ans->topEdge_ = ans->topEdge_ * Perm<6>(i, i+1);
            }
        }

        ans->top_ = tet;
        ++ans->size_;
    }

    return ans;
}

std::unique_ptr<LayeredSolidTorus> LayeredSolidTorus::recogniseFromTop(
        const Tetrahedron<3>* tet, unsigned topFace1, unsigned topFace2) {
    const Tetrahedron<3>* top = tet;
    const Tetrahedron<3>* next;
    Perm<4> cross1, cross2;
    Perm<4> canon1, canon2;
    FacePair pair = FacePair(topFace1, topFace2).complement();
    Perm<4> vRoles(pair.upper(), topFace1, topFace2, pair.lower());
    Perm<4> topRoles(vRoles);
    Perm<4> nextRoles;
    long w = 1, x = 0, y = 0, z = 1;
    long w_, x_, y_, z_;
    int rotation;
    size_t nTets = 1;

    Perm<4> rot180(3, 2, 1, 0);

    while (true) {
        // INVARIANT:
        //
        // We are currently looking at tetrahedron tet.
        // The faces of tet closest to the top of the layered solid
        // torus are vRoles[1] and vRoles[2].  The faces we have not yet
        // looked at are vRoles[0] and vRoles[3].
        //
        // Denote directed edges a = vRoles[01], b = vRoles[02], and
        // similarly let p = topRoles[01], q = topRoles[02] (where topRoles
        // was the original permutation vRoles for the original top-level
        // tetrahedron top).  Then these edges are related as follows:
        //
        //     p == w.a + x.b
        //     q == y.a + z.b
        //
        // The count nTets reflects the number of tetrahedra seen so
        // far, including this one.

        // Verify that both new faces go to the same tetrahedron (which
        // exists).
        next = tet->adjacentTetrahedron(vRoles[0]);
        if (! next)
            return nullptr;
        if (next != tet->adjacentTetrahedron(vRoles[3]))
            return nullptr;

        // Are we folding over?
        cross1 = tet->adjacentGluing(vRoles[0]);
        cross2 = tet->adjacentGluing(vRoles[3]);
        if (next == tet && cross1[vRoles[0]] == vRoles[3]) {
            // Could be.  Certainly faces vRoles[0,3] are joined to
            // each other.  This is our final iteration -- either it
            // works or it doesn't.

            // Find the permutation that maps canonical vertices 123 to 012.
            canon1 = vRoles.inverse() * cross1 * vRoles;

            // Run through the three orientation-preserving permutations.
            // Note that canon1[0] == 3.
            if (canon1 == Perm<4>(3, 1, 2, 0)) {
                // Tetrahedron is folded shut over edge vRoles[12].
                // This does not give an LST(3,2,1) base, so we are not
                // interested.
                return nullptr;
            } else if (canon1 == Perm<4>(3, 0, 1, 2)) {
                rotation = 1;
                // a, b have weights 1, 2.
            } else if (canon1 == Perm<4>(3, 2, 0, 1)) {
                rotation = 2;
                // a, b have weights 2, 1.
            } else {
                // We have an orientation-reversing permutation.
                return nullptr;
            }

            // We got one!
            std::unique_ptr<LayeredSolidTorus> ans(new LayeredSolidTorus());
            ans->size_ = nTets;

            ans->base_ = tet;
            ans->baseFace_ = FacePair(vRoles[0], vRoles[3]);
            if ((rotation == 1 && vRoles[3] < vRoles[0]) ||
                    (rotation == 2 && vRoles[0] < vRoles[3])) {
                ans->baseEdge_ = Perm<6>(
                    ans->baseFace_.oppositeEdge(),
                    Edge<3>::edgeNumber[ans->baseFace_.upper()][vRoles[2]],
                    Edge<3>::edgeNumber[vRoles[1]][ans->baseFace_.lower()],
                    Edge<3>::edgeNumber[ans->baseFace_.upper()][vRoles[1]],
                    ans->baseFace_.commonEdge(),
                    Edge<3>::edgeNumber[vRoles[2]][ans->baseFace_.lower()]);
            } else {
                ans->baseEdge_ = Perm<6>(
                    ans->baseFace_.oppositeEdge(),
                    Edge<3>::edgeNumber[ans->baseFace_.upper()][vRoles[1]],
                    Edge<3>::edgeNumber[vRoles[2]][ans->baseFace_.lower()],
                    Edge<3>::edgeNumber[ans->baseFace_.upper()][vRoles[2]],
                    ans->baseFace_.commonEdge(),
                    Edge<3>::edgeNumber[vRoles[1]][ans->baseFace_.lower()]);
            }

            long cuts01, cuts13, cuts30;
            if (rotation == 1) {
                // (a,b) == (1,2).
                cuts01 = w + 2 * x;  // w.a + x.b
                cuts13 = y + 2 * z;  // y.a + z.b
            } else {
                // (a,b) == (2,1).
                cuts01 = 2 * w + x;  // w.a + x.b
                cuts13 = 2 * y + z;  // y.a + z.b
            }
            cuts30 = - cuts01 - cuts13;

            if (cuts01 < 0) cuts01 = -cuts01;
            if (cuts13 < 0) cuts13 = -cuts13;
            if (cuts30 < 0) cuts30 = -cuts30;

            ans->top_ = top;
            ans->topFace_ = FacePair(topRoles[2], topRoles[1]);

            // Run through all six possible orderings.
            int group01, group13, group30;
            if (cuts01 <= cuts13) {
                // 01 13
                if (cuts13 <= cuts30) {
                    // 01 13 30
                    group01 = 0; group13 = 1; group30 = 2;
                } else if (cuts30 <= cuts01) {
                    // 30 01 13
                    group30 = 0; group01 = 1; group13 = 2;
                } else {
                    // 01 30 13
                    group01 = 0; group30 = 1; group13 = 2;
                }
            } else {
                // 13 01
                if (cuts30 <= cuts13) {
                    // 30 13 01
                    group30 = 0; group13 = 1; group01 = 2;
                } else if (cuts01 <= cuts30) {
                    // 13 01 30
                    group13 = 0; group01 = 1; group30 = 2;
                } else {
                    // 13 30 01
                    group13 = 0; group30 = 1; group01 = 2;
                }
            }
            ans->meridinalCuts_[group01] = cuts01;
            ans->meridinalCuts_[group13] = cuts13;
            ans->meridinalCuts_[group30] = cuts30;
            if (topRoles[2] < topRoles[1]) {
                ans->topEdge_ = Perm<6>(
                    2 * group01,
                    Edge<3>::edgeNumber[topRoles[0]][ans->topFace_.upper()],
                    2 * group01 + 1,
                    Edge<3>::edgeNumber[ans->topFace_.lower()][topRoles[3]],
                    2 * group13,
                    Edge<3>::edgeNumber[ans->topFace_.upper()][topRoles[3]],
                    2 * group13 + 1,
                    Edge<3>::edgeNumber[topRoles[0]][ans->topFace_.lower()],
                    2 * group30,
                    ans->topFace_.commonEdge(),
                    2 * group30 + 1,
                    ans->topFace_.oppositeEdge());
            } else {
                ans->topEdge_ = Perm<6>(
                    2 * group01,
                    Edge<3>::edgeNumber[topRoles[3]][ans->topFace_.upper()],
                    2 * group01 + 1,
                    Edge<3>::edgeNumber[ans->topFace_.lower()][topRoles[0]],
                    2 * group13,
                    Edge<3>::edgeNumber[ans->topFace_.upper()][topRoles[0]],
                    2 * group13 + 1,
                    Edge<3>::edgeNumber[topRoles[3]][ans->topFace_.lower()],
                    2 * group30,
                    ans->topFace_.commonEdge(),
                    2 * group30 + 1,
                    ans->topFace_.oppositeEdge());
            }

            // All done!
            return ans;
        }

        // We're looking for an entirely new tetrahedron.
        // Make sure we're not looping back in a cycle or anything kinky.
        if (next == tet || next == top)
            return nullptr;

        // Set up nextRoles so that faces tet/vRoles[0,3] are joined to
        // faces next/nextRoles[1,2] respectively.
        pair = FacePair(cross1[vRoles[0]], cross2[vRoles[3]]).complement();
        nextRoles = Perm<4>(pair.upper(), cross1[vRoles[0]], cross2[vRoles[3]],
            pair.lower());

        // Find the mapping between the canonical 0123 as described by
        // vRoles and the canonical 0123 as described by nextRoles.
        // There are two such mappings, for the gluings on faces
        // vRoles[0,3] respectively.
        canon1 = nextRoles.inverse() * cross1 * vRoles;
        canon2 = nextRoles.inverse() * cross2 * vRoles;

        // Make sure it's actually a layering, i.e., canon1 and canon2 are
        // compatible.
        if (rot180 * canon1 * rot180 != canon2)
            return nullptr;

        // Update the matrix [ a,b | c,d ].
        // It seems sanest to take cases based on the six possible
        // permutations.  Use canon2, which starts at face 3 (012).
        // Note that canon2[3] == 2.
        // Old a, b : 01, 02.
        // New a, b : 01, 13.
        if (canon2 == Perm<4>(0, 1, 3, 2)) {
            // 012 -> 013.
            // old a = a
            // old b = a+b
            // p = w.(old_a) + x.(old_b) = (w+x).a + x.b
            // q = y.(old_a) + z.(old_b) = (y+z).a + z.b
            w_ = w + x;
            x_ = x;
            y_ = y + z;
            z_ = z;
        } else if (canon2 == Perm<4>(0, 3, 1, 2)) {
            // 012 -> 031.
            // old a = a+b
            // old b = a
            // p = w.(old_a) + x.(old_b) = (w+x).a + w.b
            // q = y.(old_a) + z.(old_b) = (y+z).a + y.b
            w_ = w + x;
            x_ = w;
            y_ = y + z;
            z_ = y;
        } else if (canon2 == Perm<4>(1, 0, 3, 2)) {
            // 012 -> 103.
            // old a = -a
            // old b = b
            // p = w.(old_a) + x.(old_b) = -w.a + x.b
            // q = y.(old_a) + z.(old_b) = -y.a + z.b
            w_ = -w;
            x_ = x;
            y_ = -y;
            z_ = z;
        } else if (canon2 == Perm<4>(1, 3, 0, 2)) {
            // 012 -> 130.
            // old a = b
            // old b = -a
            // p = w.(old_a) + x.(old_b) = -x.a + w.b
            // q = y.(old_a) + z.(old_b) = -z.a + y.b
            w_ = -x;
            x_ = w;
            y_ = -z;
            z_ = y;
        } else if (canon2 == Perm<4>(3, 0, 1, 2)) {
            // 012 -> 301.
            // old a = -(a+b)
            // old b = -b
            // p = w.(old_a) + x.(old_b) = -w.a - (w+x).b
            // q = y.(old_a) + z.(old_b) = -y.a - (y+z).b
            w_ = -w;
            x_ = -w - x;
            y_ = -y;
            z_ = -y - z;
        } else if (canon2 == Perm<4>(3, 1, 0, 2)) {
            // 012 -> 310.
            // old a = -b
            // old b = -(a+b)
            // p = w.(old_a) + x.(old_b) = -x.a - (w+x).b
            // q = y.(old_a) + z.(old_b) = -z.a - (y+z).b
            w_ = -x;
            x_ = -w - x;
            y_ = -z;
            z_ = -y - z;
        } else {
            // Impossible.  We should never get to this point.
            std::cerr << "ERROR: Bad permutation canon2." << std::endl;
            return nullptr;
        }

        w = w_; x = x_; y = y_; z = z_;

        // Adjust the other variables in preparation for the next loop
        // iteration.
        tet = next;
        vRoles = nextRoles;
        nTets++;
    }

    // The loop has no break so we should never get here, but what the hell.
    return nullptr;
}

std::unique_ptr<LayeredSolidTorus> LayeredSolidTorus::recognise(
        Component<3>* comp) {
    // Start with some basic property checks.
    if (! comp->isOrientable())
        return nullptr;
    if (comp->countBoundaryComponents() != 1)
        return nullptr;
    if (comp->boundaryComponent(0)->countTriangles() != 2)
        return nullptr;

    TriangleEmbedding<3> f0 = comp->boundaryComponent(0)->triangle(0)->
        embedding(0);
    TriangleEmbedding<3> f1 = comp->boundaryComponent(0)->triangle(1)->
        embedding(0);

    Tetrahedron<3>* top = f0.tetrahedron();
    if (f1.tetrahedron() != top)
        return nullptr;

    // We have precisely one boundary component, which consists of two
    // triangular faces belonging to the same tetrahedron.

    // Follow the adjacent tetrahedra down to what should be the base
    // tetrahedron.  Don't worry about gluing permutations for now.

    // Run a full search.

    // We use recogniseFromBase(), which works out the full structure
    // for us.  It would be faster to just follow straight down from
    // the top level tetrahedron (which we already know), but this would
    // also require us to code up the entire structure again.

    FacePair underFaces = FacePair(f0.triangle(), f1.triangle()).complement();
    Tetrahedron<3>* currTet = top;
    Tetrahedron<3>* nextTet;
    while (true) {
        // INV: Thus far we have seen a chain of tetrahedra, with each
        // tetrahedron glued to the next along two faces.

        // See where the next two faces lead.
        // Note that they cannot lead back to a previous tetrahedron,
        // since each previous tetrahedron already has all four faces
        // accounted for (thus showing this loop will terminate).
        // They also cannot be boundary faces, since there are only two
        // boundary faces and these have already been seen.
        nextTet = currTet->adjacentTetrahedron(underFaces.lower());
        if (nextTet != currTet->adjacentTetrahedron(underFaces.upper()))
            return nullptr;

        // They both lead to the same adjacent tetrahedron.
        // Have we reached the end?
        if (nextTet == currTet)
            break;

        // No; we have simply moved on to the next tetrahedron.
        underFaces = FacePair(currTet->adjacentFace(underFaces.lower()),
            currTet->adjacentFace(underFaces.upper())).complement();
        currTet = nextTet;
    }

    // Here we are at the bottom.  Now check the individual permutations
    // and fill in the structural details.
    //
    // Awkwardly, we need to convert from a std::optional to a std::unique_ptr
    // at this point.
    auto ans = recogniseFromBase(currTet);
    return (ans ? std::make_unique<LayeredSolidTorus>(std::move(*ans)) :
        nullptr);
}

std::unique_ptr<Manifold> LayeredSolidTorus::manifold() const {
    return std::make_unique<Handlebody>(1);
}

AbelianGroup LayeredSolidTorus::homology() const {
    return AbelianGroup(1);
}

Triangulation<3> LayeredSolidTorus::flatten(int mobiusBandBdry) const {
    // Create a new triangulation and identify the top-level and
    // base tetrahedra.
    Triangulation<3> ans(top_->triangulation(), false);

    Tetrahedron<3>* newTop = ans.tetrahedron(top_->index());
    Tetrahedron<3>* newBase = ans.tetrahedron(base_->index());

    // Ensure only one event pair is fired in this sequence of changes.
    Triangulation<3>::ChangeEventSpan span(ans);

    // Reglue the top faces before deleting the layered solid torus.
    Tetrahedron<3>* adj0 = newTop->adjacentTetrahedron(topFace_.lower());
    Tetrahedron<3>* adj1 = newTop->adjacentTetrahedron(topFace_.upper());

    if (adj0 && adj1 && (adj0 != newTop)) {
        // A permutation for each adjacent tetrahedron.
        // These permutations map:
        //   1,2 -> vertices corresponding to top edge group 0
        //   0,2 -> vertices corresponding to top edge group 1
        //   0,1 -> vertices corresponding to top edge group 2

        // Start by representing vertices of this tetrahedron instead.
        Perm<4> groups0 = Perm<4>(
            6 - Edge<3>::edgeVertex[topEdge_[0]][0] -
                Edge<3>::edgeVertex[topEdge_[0]][1] - topFace_.lower(),
            6 - Edge<3>::edgeVertex[topEdge_[2]][0] -
                Edge<3>::edgeVertex[topEdge_[2]][1] - topFace_.lower(),
            6 - Edge<3>::edgeVertex[topEdge_[4]][0] -
                Edge<3>::edgeVertex[topEdge_[4]][1] - topFace_.lower(),
            topFace_.lower());

        FacePair underFaces = topFace_.complement();
        Perm<4> groups1 = Perm<4>(topFace_.lower(), topFace_.upper()) *
            Perm<4>(underFaces.lower(), underFaces.upper()) * groups0;

        // Move these to vertices of the adjacent tetrahedra.
        groups0 = newTop->adjacentGluing(topFace_.lower()) * groups0;
        groups1 = newTop->adjacentGluing(topFace_.upper()) * groups1;

        // And do the regluing.
        adj0->unjoin(groups0[3]);
        adj1->unjoin(groups1[3]);

        adj0->join(groups0[3], adj1, groups1 *
            Perm<4>((mobiusBandBdry + 1) % 3, (mobiusBandBdry + 2) % 3) *
            groups0.inverse());
    }

    // Delete the layered solid torus tetrahedra.
    Tetrahedron<3>* curr;
    Tetrahedron<3>* next;
    FacePair currBdryFaces;

    curr = newBase;
    currBdryFaces = baseFace_.complement();
    while (curr) {
        next = curr->adjacentTetrahedron(currBdryFaces.lower());

        currBdryFaces = FacePair(curr->adjacentFace(currBdryFaces.lower()),
            curr->adjacentFace(currBdryFaces.upper())).complement();
        ans.removeTetrahedron(curr);
        curr = next;
    }

    // And we're done.
    return ans;
}

} // namespace regina
