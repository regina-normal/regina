
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ncomponent.h"
    #include "nedge.h"
    #include "ntetrahedron.h"
    #include "naugtrisolidtorus.h"
#else
    #include "engine/triangulation/ncomponent.h"
    #include "engine/triangulation/nedge.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/naugtrisolidtorus.h"
#endif

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
    return ans;
}

void NAugTriSolidTorus::writeTextShort(ostream& out) const {
    out << "Augmented triangular solid torus: ";
    seifertStructure.writeTextShort(out);
}

void NAugTriSolidTorus::findExceptionalFibres() {
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
        NTetrahedron* base = comp->getTetrahedron(0);
        NTriSolidTorus* core;
        NPerm annulusMap[3];
        // Check every possible choice of vertex roles in tetrahedron 0.
        // Note that we do twice the checks we need to since (a,b,c,d)
        // gives an equivalent core to (b,a,d,c).
        int i, j;
        for (i = 0; i < 24; i++) {
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
                            case 1:
                                ans->edgeGroupRoles[j] = NPerm(0, 1, 2, 3);
                                break;
                            case 3:
                                ans->edgeGroupRoles[j] = NPerm(1, 2, 0, 3);
                                break;
                        }
                    }

                    ans->findExceptionalFibres();
                    return ans;
                }
            }
        }

        // Didn't find anything.
        return 0;
    }

    // We have more than three tetrahedra.
    // There must be bewteen 1 and 3 layered solid tori (note that there
    // will be no layered solid tori other than the (1-3) glued to the
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

    if (usedTets + 3 != nTet) {
        // Should only have the three core tetrahedra leftover.
        return 0;
    }

    // We now know nLayered >= 1 since usedTets == nTet - 3.

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

    // We will declare that this face hooks onto vertex roles 0, 1 and 2
    // of the first core tetrahedron.  Thus the vertex roles permutation
    // should map 0, 1 and 2 (in some order) to all vertices except for
    // topCoreFace.
    int topCoreFace = top[0]->getAdjacentFace(topFace);
    NPerm swap(3, topCoreFace);
    NTriSolidTorus* core;
    NTetrahedron* coreTets[3];
    NPerm coreVertexRoles[3];
    int whichLayered[3];
    int usedLayered;
    NPerm edgeGroupRoles[3];
    NPerm q;
    for (int p = 0; p < 6; p++) {
        core = NTriSolidTorus::isTriSolidTorus(coreTet, swap * allPermsS3[p]);
        if (core) {
            // We have a potential core.
            // Now all that remains is to ensure that the layered solid
            // tori hang from it accordingly.
            for (j = 0; j < 3; j++) {
                coreTets[j] = core->getTetrahedron(j);
                coreVertexRoles[j] = core->getVertexRoles(j);
            }
            usedLayered = 0;
            for (j = 0; j < 3; j++) {
                // Check annulus j.
                // Recall that the 3-manifold is orientable so we don't
                // have to check for wacky reversed gluings.
                if (core->isAnnulusSelfIdentified(j, &q)) {
                    // We have a degenerate (2,1,1) glued in here.
                    whichLayered[j] = -1;
                    switch (q[0]) {
                        case 0:
                            edgeGroupRoles[j] = NPerm(2, 0, 1, 3); break;
                        case 1:
                            edgeGroupRoles[j] = NPerm(0, 1, 2, 3); break;
                        case 3:
                            edgeGroupRoles[j] = NPerm(1, 2, 0, 3); break;
                    }
                } else {
                    // There should be a layered solid torus glued in here.
                    for (whichLayered[j] = 0; whichLayered[j] < nLayered;
                            whichLayered[j]++)
                        if (coreTets[(j+1)%3]->getAdjacentTetrahedron(
                                coreVertexRoles[(j+1)%3][3]) ==
                                top[whichLayered[j]] &&
                                coreTets[(j+2)%3]->getAdjacentTetrahedron(
                                coreVertexRoles[(j+2)%3][2]) ==
                                top[whichLayered[j]]) {
                            // Annulus j is glued to torus whichLayered[j].
                            q = coreTets[(j+1)%3]->
                                getAdjacentTetrahedronGluing(
                                coreVertexRoles[(j+1)%3][3]) *
                                coreVertexRoles[(j+1)%3];
                            // q maps vertex roles in core tetrahedron j+1 to
                            // vertices of the top tetrahedron in
                            // layered[whichLayered[j]].
                            edgeGroupRoles[j] = NPerm(
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[0]][q[1]]),
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[0]][q[2]]),
                                layered[whichLayered[j]]->getTopEdgeGroup(
                                    edgeNumber[q[1]][q[2]]),
                                3);
                            usedLayered++;
                            break;
                        }
                    if (whichLayered[j] >= nLayered) {
                        // This annulus was glued neither to itself nor
                        // to a layered solid torus.
                        delete core;
                        core = 0;
                        break;
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

            // We've got one!!
            NAugTriSolidTorus* ans = new NAugTriSolidTorus();
            ans->core = core;
            for (j = 0; j < 3; j++) {
                ans->edgeGroupRoles[j] = edgeGroupRoles[j];
                if (whichLayered[j] >= 0)
                    ans->augTorus[j] = layered[whichLayered[j]];
            }

            ans->findExceptionalFibres();
            return ans;
        }
    }
        
    // Nothing was found.
    for (i = 0; i < nLayered; i++)
        delete layered[i];
    return 0;
}
