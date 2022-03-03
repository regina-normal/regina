
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

#include <algorithm>
#include <utility>
#include "algebra/abeliangroup.h"
#include "manifold/sfs.h"
#include "subcomplex/augtrisolidtorus.h"
#include "subcomplex/layeredchain.h"
#include "triangulation/dim3.h"

namespace regina {

std::unique_ptr<Manifold> AugTriSolidTorus::manifold() const {
    std::unique_ptr<SFSpace> ans(new SFSpace());
    if (chainType_ == CHAIN_MAJOR) {
        // Layered solid torus + layered chain.
        ans->insertFibre(2, 1);
        ans->insertFibre(chainIndex_ + 1, 1);

        long q, r;
        if (edgeGroupRoles_[torusAnnulus_][2] == 2) {
            if (augTorus_[torusAnnulus_]) {
                r = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][0]);
                q = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][1]);
            } else {
                r = 1;
                q = 1;
            }
        } else {
            if (augTorus_[torusAnnulus_]) {
                r = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][0]);
                q = -augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][1]);
            } else {
                r = (edgeGroupRoles_[torusAnnulus_][0] == 2 ? 2 : 1);
                q = -(edgeGroupRoles_[torusAnnulus_][1] == 2 ? 2 : 1);
            }
        }
        r = r - q;
        if (r < 0) {
            r = -r;
            q = -q;
        }
        if (r == 0)
            return nullptr;
        else
            ans->insertFibre(r, q);
    } else if (chainType_ == CHAIN_AXIS) {
        // Layered solid torus + layered chain.
        ans->insertFibre(2, 1);
        ans->insertFibre(2, -1);

        long q, r;
        if (edgeGroupRoles_[torusAnnulus_][2] == 2) {
            if (augTorus_[torusAnnulus_]) {
                r = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][0]);
                q = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][1]);
            } else {
                r = 1;
                q = 1;
            }
        } else {
            if (augTorus_[torusAnnulus_]) {
                r = augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][0]);
                q = -augTorus_[torusAnnulus_]->meridinalCuts(
                    edgeGroupRoles_[torusAnnulus_][1]);
            } else {
                r = (edgeGroupRoles_[torusAnnulus_][0] == 2 ? 2 : 1);
                q = -(edgeGroupRoles_[torusAnnulus_][1] == 2 ? 2 : 1);
            }
        }
        long alpha = q - chainIndex_ * r;
        long beta = -r;
        if (alpha < 0) {
            alpha = -alpha;
            beta = -beta;
        }
        if (alpha == 0)
            return nullptr;
        else
            ans->insertFibre(alpha, beta);
    } else {
        // Three layered solid tori.
        ans->insertFibre(1, 1);

        long alpha, beta;
        for (int i = 0; i < 3; i++) {
            if (edgeGroupRoles_[i][2] == 2) {
                if (augTorus_[i]) {
                    alpha = augTorus_[i]->meridinalCuts(edgeGroupRoles_[i][0]);
                    beta = augTorus_[i]->meridinalCuts(edgeGroupRoles_[i][1]);
                } else {
                    alpha = 1;
                    beta = 1;
                }
            } else {
                if (augTorus_[i]) {
                    alpha = augTorus_[i]->meridinalCuts(edgeGroupRoles_[i][0]);
                    beta = -augTorus_[i]->meridinalCuts(edgeGroupRoles_[i][1]);
                } else {
                    alpha = (edgeGroupRoles_[i][0] == 2 ? 2 : 1);
                    beta = -(edgeGroupRoles_[i][1] == 2 ? 2 : 1);
                }
            }
            if (alpha == 0)
                return nullptr;
            else
                ans->insertFibre(alpha, beta);
        }
    }

    ans->reduce();
    return ans;
}

std::unique_ptr<AugTriSolidTorus> AugTriSolidTorus::recognise(
        const Component<3>* comp) {
    // Basic property checks.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return nullptr;
    if (comp->countVertices() > 1)
        return nullptr;

    // We have a 1-vertex closed orientable triangulation.

    unsigned long nTet = comp->size();
    if (nTet < 3)
        return nullptr;

    // Handle the 3-tetrahedron case separately.
    if (nTet == 3) {
        // Note that there cannot be a layered chain.
        Tetrahedron<3>* base = comp->tetrahedron(0);
        std::unique_ptr<TriSolidTorus> core;
        Perm<4> annulusMap[3];
        // Check every possible choice of vertex roles in tetrahedron 0.
        // Note that (a,b,c,d) gives an equivalent core to (d,c,b,a).
        int i, j;
        for (i = 0; i < 24; i++) {
            // Make sure we don't check each possible core twice.
            if (Perm<4>::S4[i][0] > Perm<4>::S4[i][3])
                continue;

            core = TriSolidTorus::recognise(base, Perm<4>::S4[i]);
            if (core) {
                // Check that the annuli are being glued to themselves.
                // Since the component is orientable, that's all we need
                // to know.
                for (j = 0; j < 3; j++) {
                    if (auto id = core->isAnnulusSelfIdentified(j)) {
                        annulusMap[j] = *id;
                    } else {
                        core.reset();
                        break;
                    }
                }

                if (core) {
                    // We got one!
                    std::unique_ptr<AugTriSolidTorus> ans(
                        new AugTriSolidTorus(*core));

                    // Work out how the mobius strip is glued onto each
                    // annulus.
                    for (j = 0; j < 3; j++) {
                        switch (annulusMap[j][0]) {
                            case 0:
                                ans->edgeGroupRoles_[j] = Perm<3>(2, 0, 1);
                                break;
                            case 2:
                                ans->edgeGroupRoles_[j] = Perm<3>(1, 2, 0);
                                break;
                            case 3:
                                ans->edgeGroupRoles_[j] = Perm<3>(0, 1, 2);
                                break;
                        }
                    }

                    ans->chainIndex_ = 0;
                    ans->chainType_ = CHAIN_NONE;
                    ans->torusAnnulus_ = -1;
                    return ans;
                }
            }
        }

        // Didn't find anything.
        return nullptr;
    }

    // We have strictly more than three tetrahedra.
    // There must be bewteen 0 and 3 layered solid tori (note that there
    // will be no layered solid tori other than the (0-3) glued to the
    // boundary annuli on the core, since no other tetrahedron is glued
    // to itself.
    int nLayered = 0;
    std::unique_ptr<LayeredSolidTorus> layered[4];
    unsigned long usedTets = 0;
    for (unsigned long t = 0; t < nTet; t++) {
        layered[nLayered] = LayeredSolidTorus::recogniseFromBase(
            comp->tetrahedron(t));
        if (layered[nLayered]) {
            usedTets += layered[nLayered]->size();
            nLayered++;
            if (nLayered == 4) {
                // Too many layered solid tori.
                return nullptr;
            }
        }
    }

    if (nLayered == 0) {
        // Our only chance now is a layered chain plus a degenerate
        // layered solid torus.

        // Start with tetrahedron 0.  Either it belongs to the chain or
        // it belongs to the core.
        Tetrahedron<3>* tet = comp->tetrahedron(0);

        // Run through all possible cores to which it might belong.
        int i;
        Perm<4> p;
        std::optional<Perm<4>> gluing;
        std::unique_ptr<TriSolidTorus> core;
        int torusAnnulus;
        unsigned long chainLen;
        for (i = 0; i < 24; i++) {
            p = Perm<4>::S4[i];
            if (p[0] > p[3])
                continue;
            core = TriSolidTorus::recognise(tet, p);
            if (! core)
                continue;

            // Let's try this core.
            // Look for an identified annulus.
            for (torusAnnulus = 0; torusAnnulus < 3; torusAnnulus++)
                if ((gluing = core->isAnnulusSelfIdentified(torusAnnulus))) {
                    // Look now for a layered chain.
                    // If we don't find it, the entire core must be wrong.
                    int chainType = CHAIN_NONE;

                    if ((chainLen = core->areAnnuliLinkedMajor(torusAnnulus)))
                        chainType = CHAIN_MAJOR;
                    else if ((chainLen =
                            core->areAnnuliLinkedAxis(torusAnnulus)))
                        chainType = CHAIN_AXIS;

                    if (chainType == CHAIN_NONE || chainLen + 3 != nTet)
                        break;

                    // We have the entire structure!
                    std::unique_ptr<AugTriSolidTorus> ans(
                        new AugTriSolidTorus(*core));
                    switch ((*gluing)[0]) {
                        case 0:
                            ans->edgeGroupRoles_[torusAnnulus] = Perm<3>(2,0,1);
                            break;
                        case 2:
                            ans->edgeGroupRoles_[torusAnnulus] = Perm<3>(1,2,0);
                            break;
                        case 3:
                            ans->edgeGroupRoles_[torusAnnulus] = Perm<3>(0,1,2);
                            break;
                    }
                    ans->chainIndex_ = chainLen;
                    ans->chainType_ = chainType;
                    ans->torusAnnulus_ = torusAnnulus;
                    return ans;
                }

            // Didn't find anything.
        }

        // Wasn't the core.  Must have been the chain.
        Tetrahedron<3>* top;
        Tetrahedron<3>* bottom;
        Perm<4> topRoles;
        Perm<4> bottomRoles;
        int j;
        int chainType;
        for (i = 0; i < 6; i++) {
            p = Perm<4>::S3[i];
            LayeredChain chain(tet, p);
            chain.extendMaximal();

            // Note that the chain will run into one of the core tetrahedra.
            if (chain.index() + 2 == nTet)
                chainType = CHAIN_MAJOR;
            else if (chain.index() + 3 == nTet)
                chainType = CHAIN_AXIS;
            else
                continue;

            // Look for the corresponding core.
            // The identified annulus on the core will have to be annulus 0.
            // Test the chain at both ends (bottom / top).
            for (j = 0; j < 2; j++) {
                if (chainType == CHAIN_MAJOR) {
                    core = TriSolidTorus::recognise(chain.bottom(),
                        chain.bottomVertexRoles() * Perm<4>(2, 3, 0, 1));
                    if (core) {
                        // Test that everything is put together properly.
                        top = chain.top();
                        topRoles = chain.topVertexRoles();

                        if ((top->adjacentTetrahedron(topRoles[0]) ==
                                    core->tetrahedron(1)) &&
                                (top->adjacentTetrahedron(topRoles[3]) ==
                                    core->tetrahedron(2)) &&
                                (top->adjacentGluing(topRoles[0])
                                    * topRoles * Perm<4>(1, 0, 2, 3) ==
                                    core->vertexRoles(1)) &&
                                (top->adjacentGluing(topRoles[3])
                                    * topRoles * Perm<4>(0, 1, 3, 2) ==
                                    core->vertexRoles(2)) &&
                                (gluing = core->isAnnulusSelfIdentified(0))) {
                            // We have the entire structure!
                            std::unique_ptr<AugTriSolidTorus> ans(
                                new AugTriSolidTorus(*core));
                            switch ((*gluing)[0]) {
                                case 0:
                                    ans->edgeGroupRoles_[0] = Perm<3>(2, 0, 1);
                                    break;
                                case 2:
                                    ans->edgeGroupRoles_[0] = Perm<3>(1, 2, 0);
                                    break;
                                case 3:
                                    ans->edgeGroupRoles_[0] = Perm<3>(0, 1, 2);
                                    break;
                            }
                            ans->chainIndex_ = chain.index() - 1;
                            ans->chainType_ = chainType;
                            ans->torusAnnulus_ = 0;
                            return ans;
                        }
                    }
                } else if (chainType == CHAIN_AXIS) {
                    bottom = chain.bottom();
                    bottomRoles = chain.bottomVertexRoles();

                    Tetrahedron<3>* startCore = bottom->adjacentTetrahedron(
                        bottomRoles[2]);

                    if (startCore)
                        core = TriSolidTorus::recognise(startCore,
                            bottom->adjacentGluing(bottomRoles[2]) *
                            bottomRoles * Perm<4>(0, 3, 2, 1));
                    else
                        core.reset();

                    if (core) {
                        // Test that everything is put together properly.
                        top = chain.top();
                        topRoles = chain.topVertexRoles();

                        if ((bottom->adjacentTetrahedron(bottomRoles[1]) ==
                                    core->tetrahedron(1)) &&
                                (top->adjacentTetrahedron(topRoles[0]) ==
                                    core->tetrahedron(0)) &&
                                (top->adjacentTetrahedron(topRoles[3]) ==
                                    core->tetrahedron(2)) &&
                                (bottom->adjacentGluing(
                                        bottomRoles[1])
                                    * bottomRoles * Perm<4>(2, 1, 0, 3) ==
                                    core->vertexRoles(1)) &&
                                (top->adjacentGluing(topRoles[0])
                                    * topRoles * Perm<4>(3, 0, 1, 2) ==
                                    core->vertexRoles(0)) &&
                                (top->adjacentGluing(topRoles[3])
                                    * topRoles * Perm<4>(1, 2, 3, 0) ==
                                    core->vertexRoles(2)) &&
                                (gluing = core->isAnnulusSelfIdentified(0))) {
                            // We have the entire structure!
                            std::unique_ptr<AugTriSolidTorus> ans(
                                new AugTriSolidTorus(*core));
                            switch ((*gluing)[0]) {
                                case 0:
                                    ans->edgeGroupRoles_[0] = Perm<3>(2, 0, 1);
                                    break;
                                case 2:
                                    ans->edgeGroupRoles_[0] = Perm<3>(1, 2, 0);
                                    break;
                                case 3:
                                    ans->edgeGroupRoles_[0] = Perm<3>(0, 1, 2);
                                    break;
                            }
                            ans->chainIndex_ = chain.index();
                            ans->chainType_ = chainType;
                            ans->torusAnnulus_ = 0;
                            return ans;
                        }
                    }
                }

                // If we just tested the bottom, prepare to test the top.
                if (j == 0)
                    chain.reverse();
            }
        }

        // Didn't find anything.
        return nullptr;
    }

    // We now know nLayered >= 1.
    int i, j;

    // Determine whether or not this augmented solid torus must contain a
    // layered chain.
    bool needChain = (usedTets + 3 != nTet);
    if (needChain && nLayered != 1)
        return nullptr;

    // Examine each layered solid torus.
    const Tetrahedron<3>* top[3];
    for (i = 0; i < nLayered; i++) {
        top[i] = layered[i]->topLevel();
        if (top[i]->adjacentTetrahedron(layered[i]->topFace(0)) ==
                top[i]->adjacentTetrahedron(layered[i]->topFace(1))) {
            // These two top triangles should be glued to different tetrahedra.
            return nullptr;
        }
    }

    // Run to the top of the first layered solid torus; this should give
    // us our core.
    int topFace = layered[0]->topFace(0);
    Tetrahedron<3>* coreTet = top[0]->adjacentTetrahedron(topFace);

    // We will declare that this triangle hooks onto vertex roles 0, 1 and 3
    // of the first core tetrahedron.  Thus the vertex roles permutation
    // should map 0, 1 and 3 (in some order) to all vertices except for
    // topCoreFace.
    int topCoreFace = top[0]->adjacentFace(topFace);
    Perm<4> swap3Top(3, topCoreFace);
    Perm<4> swap23(2, 3);
    std::unique_ptr<TriSolidTorus> core;
    Tetrahedron<3>* coreTets[3];
    Perm<4> coreVertexRoles[3];
    int whichLayered[3];
    int usedLayered;
    Perm<3> edgeGroupRoles[3];
    int torusAnnulus;
    for (int p = 0; p < 6; p++) {
        core = TriSolidTorus::recognise(coreTet,
            swap3Top * Perm<4>::S3[p] * swap23);
        if (core) {
            // We have a potential core.
            // Now all that remains is to ensure that the layered solid
            // tori hang from it accordingly.
            for (j = 0; j < 3; j++) {
                coreTets[j] = core->tetrahedron(j);
                coreVertexRoles[j] = core->vertexRoles(j);
            }
            usedLayered = 0;
            torusAnnulus = -1;
            for (j = 0; j < 3; j++) {
                // Check annulus j.
                // Recall that the 3-manifold is orientable so we don't
                // have to check for wacky reversed gluings.
                if (auto q = core->isAnnulusSelfIdentified(j)) {
                    // We have a degenerate (2,1,1) glued in here.
                    if (needChain) {
                        // We already know there is a non-degenerate
                        // layered solid torus floating about, and the
                        // other two annuli are reserved for the layered
                        // chain.
                        core.reset();
                        break;
                    }
                    whichLayered[j] = -1;
                    switch ((*q)[0]) {
                        case 0:
                            edgeGroupRoles[j] = Perm<3>(2, 0, 1); break;
                        case 2:
                            edgeGroupRoles[j] = Perm<3>(1, 2, 0); break;
                        case 3:
                            edgeGroupRoles[j] = Perm<3>(0, 1, 2); break;
                    }
                } else {
                    // There should be a layered solid torus glued in here.
                    for (whichLayered[j] = 0; whichLayered[j] < nLayered;
                            whichLayered[j]++)
                        if (coreTets[(j+1)%3]->adjacentTetrahedron(
                                coreVertexRoles[(j+1)%3][2]) ==
                                top[whichLayered[j]] &&
                                coreTets[(j+2)%3]->adjacentTetrahedron(
                                coreVertexRoles[(j+2)%3][1]) ==
                                top[whichLayered[j]]) {
                            // Annulus j is glued to torus whichLayered[j].
                            Perm<4> q = coreTets[(j+1)%3]->
                                adjacentGluing(
                                coreVertexRoles[(j+1)%3][2]) *
                                coreVertexRoles[(j+1)%3];
                            // q maps vertex roles in core tetrahedron j+1 to
                            // vertices of the top tetrahedron in
                            // layered[whichLayered[j]].
                            edgeGroupRoles[j] = Perm<3>(
                                layered[whichLayered[j]]->topEdgeGroup(
                                    Edge<3>::edgeNumber[q[0]][q[3]]),
                                layered[whichLayered[j]]->topEdgeGroup(
                                    Edge<3>::edgeNumber[q[0]][q[1]]),
                                layered[whichLayered[j]]->topEdgeGroup(
                                    Edge<3>::edgeNumber[q[1]][q[3]]));
                            usedLayered++;
                            break;
                        }
                    if (whichLayered[j] >= nLayered) {
                        // This annulus was glued neither to itself nor
                        // to a layered solid torus.
                        if (needChain)
                            whichLayered[j] = -1;
                        else {
                            core.reset();
                            break;
                        }
                    } else {
                        // This annulus was glued to a layered solid torus.
                        if (needChain)
                            torusAnnulus = j;
                    }
                }
            }
            if (! core)
                continue;

            if (usedLayered < nLayered) {
                // We didn't use all our layered solid tori.
                continue;
            }

            if (needChain) {
                // We found our one layered solid torus.  The other two
                // boundary annuli *must* be linked via a layered chain.
                int chainType = CHAIN_NONE;
                unsigned long chainLen;

                if ((chainLen = core->areAnnuliLinkedMajor(torusAnnulus)))
                    chainType = CHAIN_MAJOR;
                else if ((chainLen = core->areAnnuliLinkedAxis(torusAnnulus)))
                    chainType = CHAIN_AXIS;

                if (chainType == CHAIN_NONE || usedTets + chainLen + 3 != nTet)
                    continue;

                // We've got one!
                std::unique_ptr<AugTriSolidTorus> ans(
                    new AugTriSolidTorus(*core));
                for (j = 0; j < 3; j++) {
                    if (whichLayered[j] >= 0) {
                        ans->augTorus_[j] =
                            std::move(*layered[whichLayered[j]]);
                        ans->edgeGroupRoles_[j] = edgeGroupRoles[j];
                    }
                }
                ans->chainIndex_ = chainLen;
                ans->chainType_ = chainType;
                ans->torusAnnulus_ = torusAnnulus;
                return ans;
            } else {
                // We're not looking for a layered chain.
                // This means we have found the entire structure!
                std::unique_ptr<AugTriSolidTorus> ans(
                    new AugTriSolidTorus(*core));
                for (j = 0; j < 3; j++) {
                    ans->edgeGroupRoles_[j] = edgeGroupRoles[j];
                    if (whichLayered[j] >= 0)
                        ans->augTorus_[j] =
                            std::move(*layered[whichLayered[j]]);
                }
                ans->chainIndex_ = 0;
                ans->chainType_ = CHAIN_NONE;
                ans->torusAnnulus_ = -1;
                return ans;
            }
        }
    }

    // Nothing was found.
    return nullptr;
}

std::ostream& AugTriSolidTorus::writeCommonName(std::ostream& out,
        bool tex) const {
    if (chainIndex_) {
        // We have a layered solid torus and a layered chain.
        Perm<3> roles = edgeGroupRoles_[torusAnnulus_];
        const auto& torus = augTorus_[torusAnnulus_];

        long params[3];
        if (torus) {
            params[0] = torus->meridinalCuts(0);
            params[1] = torus->meridinalCuts(1);
            params[2] = - torus->meridinalCuts(2);
        } else {
            params[0] = 1;
            params[1] = 1;
            params[2] = -2;
        }

        if (params[roles[0]] < 0) {
            params[0] = - params[0];
            params[1] = - params[1];
            params[2] = - params[2];
        }

        if (chainType_ == CHAIN_MAJOR)
            out << (tex ? "J_{" : "J(");
        else
            out << (tex ? "X_{" : "X(");
        return out << chainIndex_ << " | " << params[roles[0]] << ','
            << params[roles[1]] << (tex ? '}' : ')');
    } else {
        // We have three layered solid tori.
        std::pair<long, long> allParams[3];
        int nAllParams = 0;
        long params[3];
        std::pair<long, long> lstParams;
        int i;
        for (i = 0; i < 3; i++) {
            Perm<3> roles = edgeGroupRoles_[i];
            const auto& torus = augTorus_[i];
            if (torus) {
                params[0] = torus->meridinalCuts(0);
                params[1] = torus->meridinalCuts(1);
                params[2] = - torus->meridinalCuts(2);
            } else {
                params[0] = 1;
                params[1] = 1;
                params[2] = -2;
            }
            lstParams = std::make_pair(params[roles[0]], params[roles[1]]);
            if (lstParams.first < 0) {
                lstParams.first = - lstParams.first;
                lstParams.second = - lstParams.second;
            }
            if (! (lstParams.first == 2 && lstParams.second == -1))
                allParams[nAllParams++] = lstParams;
        }
        sort(allParams, allParams + nAllParams);

        out << (tex ? "A_{" : "A(");
        for (i = 0; i < nAllParams; i++) {
            if (i > 0)
                out << " | ";
            out << allParams[i].first << ',' << allParams[i].second;
        }
        return out << (tex ? '}' : ')');
    }
}

std::ostream& AugTriSolidTorus::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

std::ostream& AugTriSolidTorus::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

void AugTriSolidTorus::writeTextLong(std::ostream& out) const {
    out << (chainIndex_ ? "Chained " : "Augmented ")
        << "triangular solid torus "
        << (torusAnnulus_ == -1 ? "(three tori): " : "(torus + chain): ");
    writeName(out);
}

} // namespace regina
