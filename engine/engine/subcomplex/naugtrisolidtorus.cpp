
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nlayeredchain.h"

namespace regina {

NAugTriSolidTorus::~NAugTriSolidTorus() {
    if (core)
        delete core;
    for (int i = 0; i < 3; i++)
        if (augTorus[i])
            delete augTorus[i];
}

NAugTriSolidTorus* NAugTriSolidTorus::clone() const {
    NAugTriSolidTorus* ans = new NAugTriSolidTorus();
    ans->core = core->clone();
    for (int i = 0; i < 3; i++) {
        if (augTorus[i])
            ans->augTorus[i] = augTorus[i]->clone();
        ans->edgeGroupRoles[i] = edgeGroupRoles[i];
    }
    ans->chainIndex = chainIndex;
    ans->torusAnnulus = torusAnnulus;
    ans->seifertStructure = seifertStructure;
    return ans;
}

void NAugTriSolidTorus::writeTextShort(std::ostream& out) const {
    out << "Augmented triangular solid torus " <<
        (torusAnnulus == -1 ? "(three tori): " : "(torus + chain): ");
    seifertStructure.writeTextShort(out);
}

void NAugTriSolidTorus::findExceptionalFibres() {
    if (chainIndex > 0) {
        // Layered solid torus + layered chain.
        seifertStructure.insertFibre(NExceptionalFibre(2,1));
        seifertStructure.insertFibre(NExceptionalFibre(chainIndex + 1, 1));

        long q, r;
        if (edgeGroupRoles[torusAnnulus][2] == 2) {
            if (augTorus[torusAnnulus]) {
                r = augTorus[torusAnnulus]->getMeridinalCuts(
                    edgeGroupRoles[torusAnnulus][0]);
                q = augTorus[torusAnnulus]->getMeridinalCuts(
                    edgeGroupRoles[torusAnnulus][1]);
            } else {
                r = 1;
                q = 1;
            }
        } else {
            if (augTorus[torusAnnulus]) {
                r = augTorus[torusAnnulus]->getMeridinalCuts(
                    edgeGroupRoles[torusAnnulus][0]);
                q = -augTorus[torusAnnulus]->getMeridinalCuts(
                    edgeGroupRoles[torusAnnulus][1]);
            } else {
                r = (edgeGroupRoles[torusAnnulus][0] == 2 ? 2 : 1);
                q = -(edgeGroupRoles[torusAnnulus][1] == 2 ? 2 : 1);
            }
        }
        r = r - q;
        if (r < 0) {
            r = -r;
            q = -q;
        }
        seifertStructure.insertFibre(NExceptionalFibre(r, q));
    } else {
        // Three layered solid tori.
        seifertStructure.insertFibre(NExceptionalFibre(1,1));

        long alpha, beta;
        for (int i = 0; i < 3; i++) {
            if (edgeGroupRoles[i][2] == 2) {
                if (augTorus[i]) {
                    alpha = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][0]);
                    beta = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][1]);
                } else {
                    alpha = 1;
                    beta = 1;
                }
            } else {
                if (augTorus[i]) {
                    alpha = augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][0]);
                    beta = -augTorus[i]->getMeridinalCuts(edgeGroupRoles[i][1]);
                } else {
                    alpha = (edgeGroupRoles[i][0] == 2 ? 2 : 1);
                    beta = -(edgeGroupRoles[i][1] == 2 ? 2 : 1);
                }
            }
            seifertStructure.insertFibre(NExceptionalFibre(alpha, beta));
        }
    }

    seifertStructure.reduce();
}

NAugTriSolidTorus* NAugTriSolidTorus::isAugTriSolidTorus(
        const NComponent* comp) {
    // Basic property checks.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;
    if (comp->getNumberOfVertices() > 1)
        return 0;

    // We have a 1-vertex closed orientable triangulation.

    unsigned long nTet = comp->getNumberOfTetrahedra();
    if (nTet < 3)
        return 0;

    // Handle the 3-tetrahedron case separately.
    if (nTet == 3) {
        // Note that there cannot be a layered chain.
        NTetrahedron* base = comp->getTetrahedron(0);
        NTriSolidTorus* core;
        NPerm annulusMap[3];
        // Check every possible choice of vertex roles in tetrahedron 0.
        // Note that (a,b,c,d) gives an equivalent core to (d,c,b,a).
        int i, j;
        for (i = 0; i < 24; i++) {
            // Make sure we don't check each possible core twice.
            if (allPermsS4[i][0] > allPermsS4[i][3])
                continue;

            core = NTriSolidTorus::isTriSolidTorus(base, allPermsS4[i]);
            if (core) {
                // Check that the annuli are being glued to themselves.
                // Since the component is orientable, that's all we need
                // to know.
                for (j = 0; j < 3; j++) {
                    if (! core->isAnnulusSelfIdentified(j, annulusMap + j)) {
                        delete core;
                        core = 0;
                        break;
                    }
                }

                if (core) {
                    // We got one!
                    NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                    ans->core = core;

                    // Work out how the mobius strip is glued onto each
                    // annulus.
                    for (j = 0; j < 3; j++) {
                        switch (annulusMap[j][0]) {
                            case 0:
                                ans->edgeGroupRoles[j] = NPerm(2, 0, 1, 3);
                                break;
                            case 2:
                                ans->edgeGroupRoles[j] = NPerm(1, 2, 0, 3);
                                break;
                            case 3:
                                ans->edgeGroupRoles[j] = NPerm(0, 1, 2, 3);
                                break;
                        }
                    }

                    ans->chainIndex = 0;
                    ans->torusAnnulus = -1;

                    ans->findExceptionalFibres();
                    return ans;
                }
            }
        }

        // Didn't find anything.
        return 0;
    }

    // We have strictly more than three tetrahedra.
    // There must be bewteen 0 and 3 layered solid tori (note that there
    // will be no layered solid tori other than the (0-3) glued to the
    // boundary annuli on the core, since no other tetrahedron is glued
    // to itself.
    int nLayered = 0;
    NLayeredSolidTorus* layered[4];
    unsigned long usedTets = 0;
    for (unsigned long t = 0; t < nTet; t++) {
        layered[nLayered] = NLayeredSolidTorus::isLayeredSolidTorusBase(
            comp->getTetrahedron(t));
        if (layered[nLayered]) {
            usedTets += layered[nLayered]->getNumberOfTetrahedra();
            nLayered++;
            if (nLayered == 4) {
                // Too many layered solid tori.
                for (int i = 0; i < nLayered; i++)
                    delete layered[i];
                return 0;
            }
        }
    }

    if (nLayered == 0) {
        // Our only chance now is a layered chain plus a degenerate
        // layered solid torus.
        
        // Start with tetrahedron 0.  Either it belongs to the chain or
        // it belongs to the core.
        NTetrahedron* tet = comp->getTetrahedron(0);

        // Run through all possible cores to which it might belong.
        int i;
        NPerm p, annulusPerm;
        NTriSolidTorus* core;
        int torusAnnulus;
        unsigned long chainLen;
        for (i = 0; i < 24; i++) {
            p = allPermsS4[i];
            if (p[0] > p[3])
                continue;
            core = NTriSolidTorus::isTriSolidTorus(tet, p);
            if (! core)
                continue;

            // Let's try this core.
            // Look for an identified annulus.
            for (torusAnnulus = 0; torusAnnulus < 3; torusAnnulus++)
                if (core->isAnnulusSelfIdentified(torusAnnulus, &annulusPerm)) {
                    // Look now for a layered chain.
                    // If we don't find it, the entire core must be wrong.
                    chainLen = core->areAnnuliLinkedMajor(torusAnnulus);
                    if (chainLen + 3 != nTet)
                        break;

                    // We have the entire structure!
                    NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                    ans->core = core;
                    switch (annulusPerm[0]) {
                        case 0:
                            ans->edgeGroupRoles[torusAnnulus] = NPerm(2,0,1,3);
                            break;
                        case 2:
                            ans->edgeGroupRoles[torusAnnulus] = NPerm(1,2,0,3);
                            break;
                        case 3:
                            ans->edgeGroupRoles[torusAnnulus] = NPerm(0,1,2,3);
                            break;
                    }
                    ans->chainIndex = chainLen;
                    ans->torusAnnulus = torusAnnulus;

                    ans->findExceptionalFibres();
                    return ans;
                }

            // Didn't find anything.
            delete core;
        }

        // Wasn't the core.  Must have been the chain.
        NTetrahedron* top;
        NPerm topRoles;
        int j;
        for (i = 0; i < 6; i++) {
            p = allPermsS3[i];
            NLayeredChain chain(tet, p);
            chain.extendMaximal();

            // Note that the chain will run into one of the core tetrahedra.
            if (chain.getIndex() + 2 != nTet)
                continue;

            // Look for the corresponding core.
            // The identified annulus on the core will have to be annulus 0.
            // Test the chain at both ends (bottom / top).
            for (j = 0; j < 2; j++) {
                core = NTriSolidTorus::isTriSolidTorus(chain.getBottom(),
                    chain.getBottomVertexRoles() * NPerm(2, 3, 0, 1));
                if (core) {
                    // Test that everything is put together properly.
                    top = chain.getTop();
                    topRoles = chain.getTopVertexRoles();

                    if ((top->getAdjacentTetrahedron(topRoles[0]) ==
                                core->getTetrahedron(1)) &&
                            (top->getAdjacentTetrahedron(topRoles[3]) ==
                                core->getTetrahedron(2)) &&
                            (top->getAdjacentTetrahedronGluing(topRoles[0]) *
                                topRoles * NPerm(1, 0, 2, 3) ==
                                core->getVertexRoles(1)) &&
                            (top->getAdjacentTetrahedronGluing(topRoles[3]) *
                                topRoles * NPerm(0, 1, 3, 2) ==
                                core->getVertexRoles(2)) &&
                            core->isAnnulusSelfIdentified(0, &annulusPerm)) {
                        // We have the entire structure!
                        NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                        ans->core = core;
                        switch (annulusPerm[0]) {
                            case 0:
                                ans->edgeGroupRoles[0] = NPerm(2, 0, 1, 3);
                                break;
                            case 2:
                                ans->edgeGroupRoles[0] = NPerm(1, 2, 0, 3);
                                break;
                            case 3:
                                ans->edgeGroupRoles[0] = NPerm(0, 1, 2, 3);
                                break;
                        }
                        ans->chainIndex = chain.getIndex() - 1;
                        ans->torusAnnulus = 0;
        
                        ans->findExceptionalFibres();
                        return ans;
                    }

                    delete core;
                }

                // If we just tested the bottom, prepare to test the top.
                if (j == 0)
                    chain.reverse();
            }
        }

        // Didn't find anything.
        return 0;
    }

    // We now know nLayered >= 1.

    // Determine whether or not this augmented solid torus must contain a
    // layered chain.
    bool needChain = (usedTets + 3 != nTet);
    if (needChain && nLayered != 1)
        return 0;

    // Examine each layered solid torus.
    NTetrahedron* top[3];
    int i, j;
    for (i = 0; i < nLayered; i++) {
        top[i] = layered[i]->getTopLevel();
        if (top[i]->getAdjacentTetrahedron(layered[i]->getTopFace(0)) ==
                top[i]->getAdjacentTetrahedron(layered[i]->getTopFace(1))) {
            // These two top faces should be glued to different
            // tetrahedra.
            for (j = 0; j < nLayered; j++)
                delete layered[j];
            return 0;
        }
    }

    // Run to the top of the first layered solid torus; this should give
    // us our core.
    int topFace = layered[0]->getTopFace(0);
    NTetrahedron* coreTet = top[0]->getAdjacentTetrahedron(topFace);

    // We will declare that this face hooks onto vertex roles 0, 1 and 3
    // of the first core tetrahedron.  Thus the vertex roles permutation
    // should map 0, 1 and 3 (in some order) to all vertices except for
    // topCoreFace.
    int topCoreFace = top[0]->getAdjacentFace(topFace);
    NPerm swap3Top(3, topCoreFace);
    NPerm swap23(2, 3);
    NTriSolidTorus* core;
    NTetrahedron* coreTets[3];
    NPerm coreVertexRoles[3];
    int whichLayered[3];
    int usedLayered;
    NPerm edgeGroupRoles[3];
    int torusAnnulus;
    NPerm q;
    for (int p = 0; p < 6; p++) {
        core = NTriSolidTorus::isTriSolidTorus(coreTet,
            swap3Top * allPermsS3[p] * swap23);
        if (core) {
            // We have a potential core.
            // Now all that remains is to ensure that the layered solid
            // tori hang from it accordingly.
            for (j = 0; j < 3; j++) {
                coreTets[j] = core->getTetrahedron(j);
                coreVertexRoles[j] = core->getVertexRoles(j);
            }
            usedLayered = 0;
            torusAnnulus = -1;
            for (j = 0; j < 3; j++) {
                // Check annulus j.
                // Recall that the 3-manifold is orientable so we don't
                // have to check for wacky reversed gluings.
                if (core->isAnnulusSelfIdentified(j, &q)) {
                    // We have a degenerate (2,1,1) glued in here.
                    if (needChain) {
                        // We already know there is a non-degenerate
                        // layered solid torus floating about, and the
                        // other two annuli are reserved for the layered
                        // chain.
                        delete core;
                        core = 0;
                        break;
                    }
                    whichLayered[j] = -1;
                    switch (q[0]) {
                        case 0:
                            edgeGroupRoles[j] = NPerm(2, 0, 1, 3); break;
                        case 2:
                            edgeGroupRoles[j] = NPerm(1, 2, 0, 3); break;
                        case 3:
                            edgeGroupRoles[j] = NPerm(0, 1, 2, 3); break;
                    }
                } else {
                    // There should be a layered solid torus glued in here.
                    for (whichLayered[j] = 0; whichLayered[j] < nLayered;
                            whichLayered[j]++)
                        if (coreTets[(j+1)%3]->getAdjacentTetrahedron(
                                coreVertexRoles[(j+1)%3][2]) ==
                                top[whichLayered[j]] &&
                                coreTets[(j+2)%3]->getAdjacentTetrahedron(
                                coreVertexRoles[(j+2)%3][1]) ==
                                top[whichLayered[j]]) {
                            // Annulus j is glued to torus whichLayered[j].
                            q = coreTets[(j+1)%3]->
                                getAdjacentTetrahedronGluing(
                                coreVertexRoles[(j+1)%3][2]) *
                                coreVertexRoles[(j+1)%3];
                            // q maps vertex roles in core tetrahedron j+1 to
                            // vertices of the top tetrahedron in
                            // layered[whichLayered[j]].
                            edgeGroupRoles[j] = NPerm(
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[0]][q[3]]),
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[0]][q[1]]),
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[1]][q[3]]),
                                3);
                            usedLayered++;
                            break;
                        }
                    if (whichLayered[j] >= nLayered) {
                        // This annulus was glued neither to itself nor
                        // to a layered solid torus.
                        if (needChain)
                            whichLayered[i] = -1;
                        else {
                            delete core;
                            core = 0;
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
                delete core;
                continue;
            }

            if (needChain) {
                // We found our one layered solid torus.  The other two
                // boundary annuli *must* be linked via a layered chain.
                unsigned long chainLen =
                    core->areAnnuliLinkedMajor(torusAnnulus);
                if (usedTets + chainLen + 3 != nTet) {
                    delete core;
                    continue;
                }

                // We've got one!
                NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                ans->core = core;
                for (j = 0; j < 3; j++) {
                    if (whichLayered[j] >= 0) {
                        ans->augTorus[j] = layered[whichLayered[j]];
                        ans->edgeGroupRoles[j] = edgeGroupRoles[j];
                    }
                }
                ans->chainIndex = chainLen;
                ans->torusAnnulus = torusAnnulus;

                ans->findExceptionalFibres();
                return ans;
            } else {
                // We're not looking for a layered chain.
                // This means we have found the entire structure!
                NAugTriSolidTorus* ans = new NAugTriSolidTorus();
                ans->core = core;
                for (j = 0; j < 3; j++) {
                    ans->edgeGroupRoles[j] = edgeGroupRoles[j];
                    if (whichLayered[j] >= 0)
                        ans->augTorus[j] = layered[whichLayered[j]];
                }
                ans->chainIndex = 0;
                ans->torusAnnulus = -1;

                ans->findExceptionalFibres();
                return ans;
            }
        }
    }
        
    // Nothing was found.
    for (i = 0; i < nLayered; i++)
        delete layered[i];
    return 0;
}

} // namespace regina
