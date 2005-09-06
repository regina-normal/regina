
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/npluggedibundle.h"
#include "subcomplex/nlayeredsolidtorus.h"
#include "triangulation/nfacepair.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntetface.h"
#include "triangulation/ntriangulation.h"

namespace regina {

const int NPluggedIBundleCore::T_5_1 = 501;
const int NPluggedIBundleCore::T_5_2 = 502;
const int NPluggedIBundleCore::T_5_3 = 503;
const int NPluggedIBundleCore::T_5_4 = 504;
const int NPluggedIBundleCore::T_6_1 = 601;
const int NPluggedIBundleCore::T_6_2 = 602;
const int NPluggedIBundleCore::T_6_3 = 603;
const int NPluggedIBundleCore::T_6_4 = 604;

namespace {
    const NPluggedIBundleCore core_T_5_1(NPluggedIBundleCore::T_5_1);
    const NPluggedIBundleCore core_T_5_2(NPluggedIBundleCore::T_5_2);
    const NPluggedIBundleCore core_T_5_3(NPluggedIBundleCore::T_5_3);
    const NPluggedIBundleCore core_T_5_4(NPluggedIBundleCore::T_5_4);
    const NPluggedIBundleCore core_T_6_1(NPluggedIBundleCore::T_6_1);
    const NPluggedIBundleCore core_T_6_2(NPluggedIBundleCore::T_6_2);
    const NPluggedIBundleCore core_T_6_3(NPluggedIBundleCore::T_6_3);
    const NPluggedIBundleCore core_T_6_4(NPluggedIBundleCore::T_6_4);
}

NPluggedIBundleCore::NPluggedIBundleCore(int whichCoreType) :
        coreType(whichCoreType) {
    if (coreType == T_6_1) {
        const int adj[6][4] = {
            { 1, 5, 3, -1},
            { 0, 5, 4, -1},
            { 3, 4, 5, -1},
            { 2, 4, 0, -1},
            { 5, 1, 2, 3},
            { 2, 4, 0, 1}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 3, 2, 1, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 3, 2, 1, 0 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 2, 3, 0, 1 }, { 1, 0, 2, 3 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_2) {
        const int adj[6][4] = {
            { 1, 5, 3, -1},
            { 0, 5, 4, -1},
            { 3, 4, 5, -1},
            { 2, 4, 0, -1},
            { 5, 1, 2, 3},
            { 2, 4, 1, 0}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 0, 3, 1, 2 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 2, 0, 3 }, { 2, 3, 1, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 1, 3, 0, 2 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 3, 2, 0, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 2, 3, 0, 1 }, { 1, 0, 2, 3 }, { 2, 0, 1, 3 }, { 0, 2, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_3) {
        const int adj[6][4] = {
            { 4, 5, 3, -1},
            { 4, 5, 2, -1},
            { 5, 4, 1, -1},
            { 5, 4, 0, -1},
            { 1, 0, 2, 3},
            { 3, 2, 0, 1}
        };

        const int glu[6][4][4] = {
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_6_4) {
        const int adj[6][4] = {
            { 4, 4, 3, -1},
            { 4, 4, 2, -1},
            { 5, 5, 1, -1},
            { 5, 5, 0, -1},
            { 1, 0, 0, 1},
            { 3, 2, 2, 3}
        };

        const int glu[6][4][4] = {
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 0, 2, 1, 3 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 3, 1 }, { 1, 3, 0, 2 }, { 1, 0, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 0, 2, 1, 3 }, { 2, 0, 3, 1 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 0; topTet[0][1] = 1; topTet[1][0] = 2; topTet[1][1] = 3;
        // topVertices consists entirely of identity permutations.
    } else if (coreType == T_5_1) {
        const int adj[6][4] = {
            { 3, 4, -1, -1},
            { 3, 4, 5, -1},
            { 3, 4, 5, -1},
            { 2, 1, 5, 0},
            { 2, 1, 5, 0},
            { 4, 3, 2, 1}
        };

        const int glu[6][4][4] = {
            { { 3, 2, 0, 1 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
            { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 3, 2, 1, 0 }, { 2, 3, 1, 0 } },
            { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 3, 2, 0, 1 }, { 2, 3, 0, 1 } },
            { { 2, 3, 1, 0 }, { 3, 2, 1, 0 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 1; topTet[0][1] = 0; topTet[1][0] = 0; topTet[1][1] = 2;
        topVertices[1][0] = NPerm(2, 3);
    } else if (coreType == T_5_2) {
        const int adj[6][4] = {
            { 2, 1, -1, -1},
            { 2, 0, 3, -1},
            { 0, 1, 4, -1},
            { 4, 5, 5, 1},
            { 3, 5, 5, 2},
            { 4, 3, 4, 3}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 3, 1 }, { 2, 1, 0, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 2, 1, 0, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 3, 1, 2 }, { 3, 0, 2, 1 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 1, 2, 0 }, { 1, 0, 3, 2 }, { 0, 1, 3, 2 } },
            { { 0, 2, 1, 3 }, { 2, 0, 3, 1 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } },
            { { 1, 3, 0, 2 }, { 3, 1, 2, 0 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 1; topTet[0][1] = 0; topTet[1][0] = 0; topTet[1][1] = 2;
        topVertices[1][0] = NPerm(2, 3);
    } else if (coreType == T_5_3) {
        const int adj[6][4] = {
            { 1, 2, -1, -1},
            { 0, 2, 3, -1},
            { 0, 1, 4, -1},
            { 4, 5, 5, 1},
            { 3, 5, 5, 2},
            { 4, 3, 4, 3}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 2, 0, 3, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 1, 2, 0 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 1, 3, 0, 2 }, { 3, 1, 2, 0 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 1, 2, 0 }, { 1, 0, 3, 2 }, { 0, 1, 3, 2 } },
            { { 0, 2, 1, 3 }, { 2, 0, 3, 1 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 } },
            { { 1, 3, 0, 2 }, { 3, 1, 2, 0 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 1; topTet[0][1] = 0; topTet[1][0] = 0; topTet[1][1] = 2;
        topVertices[1][0] = NPerm(1, 0, 3, 2);
    } else if (coreType == T_5_4) {
        const int adj[6][4] = {
            { 1, 2, -1, -1},
            { 0, 2, 3, -1},
            { 1, 0, 4, -1},
            { 5, 5, 4, 1},
            { 5, 5, 3, 2},
            { 3, 4, 3, 4}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 2, 1, 3, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 0, 2, 1 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 3, 1, 0, 2 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 1, 2, 3 }, { 3, 2, 0, 1 }, { 1, 0, 2, 3 }, { 1, 0, 3, 2 } },
            { { 3, 2, 1, 0 }, { 0, 1, 3, 2 }, { 1, 0, 2, 3 }, { 0, 1, 3, 2 } },
            { { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 2, 3, 1, 0 }, { 3, 2, 1, 0 } }
        };

        core.insertConstruction(6, adj, glu);

        topTet[0][0] = 1; topTet[0][1] = 0; topTet[1][0] = 0; topTet[1][1] = 2;
        topVertices[1][0] = NPerm(2, 3);
    }
}

NPluggedIBundle::~NPluggedIBundle() {
    if (core)
        delete core;
    if (plug[0])
        delete plug[0];
    if (plug[1])
        delete plug[1];
}

NPluggedIBundle* NPluggedIBundle::clone() const {
    NPluggedIBundle* ans = new NPluggedIBundle();

    ans->core = new NIsomorphismDirect(*core);
    ans->coreType = coreType;

    for (int i = 0; i < 2; i++) {
        if (plug[i])
            ans->plug[i] = plug[i]->clone();
        ans->edgeGroupRoles[i] = edgeGroupRoles[i];
    }

    return ans;
}

NPluggedIBundle* NPluggedIBundle::isPluggedIBundle(NTriangulation* tri) {
    // Basic property checks.
    if ((! tri->isClosed()) || tri->isOrientable())
        return 0;
    if (tri->getNumberOfVertices() > 1)
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;
    if (tri->getNumberOfTetrahedra() < 6)
        return 0;

    // We have a 1-vertex 1-component closed non-orientable triangulation
    // with at least six tetrahedra.

    // Hunt for the core thin I-bundle.
    NPluggedIBundle* ans;
    if ((ans = hunt(tri, core_T_6_1)))
        return ans;
    if ((ans = hunt(tri, core_T_6_2)))
        return ans;
    if ((ans = hunt(tri, core_T_6_3)))
        return ans;
    if ((ans = hunt(tri, core_T_6_4)))
        return ans;
    if ((ans = hunt(tri, core_T_5_1)))
        return ans;
    if ((ans = hunt(tri, core_T_5_2)))
        return ans;
    if ((ans = hunt(tri, core_T_5_3)))
        return ans;
    if ((ans = hunt(tri, core_T_5_4)))
        return ans;

    return 0;
}

NPluggedIBundle* NPluggedIBundle::hunt(NTriangulation* tri,
        const NPluggedIBundleCore& core) {
    std::list<NIsomorphism*> isos;
    unsigned long found = core.core.findAllSubcomplexesIn(*tri, isos);
    if (! found)
        return 0;

    NTetFace top[2][2];
    NTetrahedron* adj[2][2];
    NPerm adjPerm[2][2];
    NLayeredSolidTorus* lst[2];
    NPerm p, canon[2], roles[2];
    NFacePair faces, tmpFaces;
    int i, j;
    bool broken;
    std::list<NIsomorphism*>::const_iterator it;
    for (it = isos.begin(); it != isos.end(); it++) {
        // Look for the layered solid tori.
        lst[0] = lst[1] = 0;
        broken = false;

        for (i = 0; i < 2; i++)
            for (j = 0; j < 2; j++) {
                top[i][j].tet = (*it)->tetImage(core.topTet[i][j]);
                top[i][j].face = (*it)->facePerm(core.topTet[i][j])
                    [core.topVertices[i][j][3]];
                adj[i][j] = tri->getTetrahedron(top[i][j].tet)->
                    getAdjacentTetrahedron(top[i][j].face);
                adjPerm[i][j] = tri->getTetrahedron(top[i][j].tet)->
                    getAdjacentTetrahedronGluing(top[i][j].face);
            }

        for (i = 0; i < 2; i++) {
            if (adj[i][0] == tri->getTetrahedron(top[i][1].tet)) {
                // Our only hope is for a degenerate LST.
                if (adjPerm[i][0][top[i][0].face] == top[i][1].face) {
                    // It glues to the right face.  We still have a chance.
                    // Find the permutation describing how the canonical 012
                    // representations of the two faces are identified.
                    p = core.topVertices[i][1].inverse() *
                        (*it)->facePerm(core.topTet[i][1]).inverse() *
                        adjPerm[i][0] *
                        (*it)->facePerm(core.topTet[i][0]) *
                        core.topVertices[i][0];
                    if (p == NPerm(0, 2, 1, 3)) {
                        // Boundary of the Mobius strip is a diagonal edge.
                        roles[i] = NPerm(0, 1, 2, 3);
                    } else if (p == NPerm(2, 1, 0, 3)) {
                        // Boundary of the Mobius strip is a horizontal edge.
                        roles[i] = NPerm(1, 2, 0, 3);
                    } else if (p == NPerm(1, 0, 2, 3)) {
                        // Boundary of the Mobius strip is a vertical edge.
                        roles[i] = NPerm(2, 0, 1, 3);
                    } else {
                        // Not the right kind of permutation.
                        broken = true;
                        break;
                    }
                } else {
                    broken = true;
                    break;
                }
            } else if (adj[i][0] == tri->getTetrahedron(top[1 - i][0].tet) ||
                       adj[i][0] == tri->getTetrahedron(top[1 - i][1].tet)) {
                // We're folding back into the core, and it's not a
                // degenerate LST.  Nothing good can come of this.
                broken = true;
                break;
            } else if (adj[i][0] == adj[i][1]) {
                // We're heading outside the core, and both adj[i][*]
                // faces point to the same tetrahedron.  Hunt for a real
                // attached LST.
                lst[i] = NLayeredSolidTorus::formsLayeredSolidTorusTop
                    (adj[i][0], adjPerm[i][0][top[i][0].face],
                    adjPerm[i][1][top[i][1].face]);
                if (! lst[i])
                    break;

                // We have an LST.
                // All that's left is to ensure that the gluings match
                // up properly.
                // The following permutations map canonical 012
                // representations of the core boundary faces to the
                // vertices of the top level LST tetrahedron.
                canon[0] = adjPerm[i][0] *
                    (*it)->facePerm(core.topTet[i][0]) *
                    core.topVertices[i][0];
                canon[1] = adjPerm[i][1] *
                    (*it)->facePerm(core.topTet[i][1]) *
                    core.topVertices[i][1];

                roles[i] = NPerm(
                    lst[i]->getTopEdgeGroup(
                        edgeNumber[canon[0][0]][canon[0][1]]),
                    lst[i]->getTopEdgeGroup(
                        edgeNumber[canon[0][0]][canon[0][2]]),
                    lst[i]->getTopEdgeGroup(
                        edgeNumber[canon[0][1]][canon[0][2]]),
                    3);

                if (roles[i][0] == lst[i]->getTopEdgeGroup(
                            edgeNumber[canon[1][0]][canon[1][1]]) &&
                        roles[i][1] == lst[i]->getTopEdgeGroup(
                            edgeNumber[canon[1][0]][canon[1][2]]) &&
                        roles[i][2] == lst[i]->getTopEdgeGroup(
                            edgeNumber[canon[1][1]][canon[1][2]])) {
                    // It's a layering!  W00t.  All done.
                } else {
                    // Not a layering.
                    broken = true;
                    break;
                }
            } else {
                // No hope.
                broken = true;
                break;
            }
        }

        if (broken) {
            if (lst[0])
                delete lst[0];
            if (lst[1])
                delete lst[1];

            // Delete this isomorphism; we won't need it any more.
            delete *it;
            continue;
        }

        // Found one!
        NPluggedIBundle* ans = new NPluggedIBundle();
        ans->core = *it;
        ans->coreType = core.coreType;
        ans->plug[0] = lst[0];
        ans->plug[1] = lst[1];
        ans->edgeGroupRoles[0] = roles[0];
        ans->edgeGroupRoles[1] = roles[1];

        // Delete the remaining isomorphisms that we never even
        // looked at.
        for (it++; it != isos.end(); it++)
            delete *it;

        return ans;
    }

    // Nothing found.
    return 0;
}

std::ostream& NPluggedIBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    if (tex) {
        switch(coreType) {
            case NPluggedIBundleCore::T_6_1: out << "$H_{\\tilde{T}_6^1"; break;
            case NPluggedIBundleCore::T_6_2: out << "$H_{\\tilde{T}_6^2"; break;
            case NPluggedIBundleCore::T_6_3: out << "$H_{\\tilde{T}_6^3"; break;
            case NPluggedIBundleCore::T_6_4: out << "$H_{\\tilde{T}_6^4"; break;
            case NPluggedIBundleCore::T_5_1: out << "$K_{\\tilde{T}_5^1"; break;
            case NPluggedIBundleCore::T_5_2: out << "$K_{\\tilde{T}_5^2"; break;
            case NPluggedIBundleCore::T_5_3: out << "$K_{\\tilde{T}_5^3"; break;
            case NPluggedIBundleCore::T_5_4: out << "$K_{\\tilde{T}_5^4"; break;
        }
    } else {
        switch(coreType) {
            case NPluggedIBundleCore::T_6_1: out << "H(T~6^1"; break;
            case NPluggedIBundleCore::T_6_2: out << "H(T~6^2"; break;
            case NPluggedIBundleCore::T_6_3: out << "H(T~6^3"; break;
            case NPluggedIBundleCore::T_6_4: out << "H(T~6^4"; break;
            case NPluggedIBundleCore::T_5_1: out << "K(T~5^1"; break;
            case NPluggedIBundleCore::T_5_2: out << "K(T~5^2"; break;
            case NPluggedIBundleCore::T_5_3: out << "K(T~5^3"; break;
            case NPluggedIBundleCore::T_5_4: out << "K(T~5^4"; break;
        }
    }

    // TODO: Normalise parameters.
    long horiz[2], vert[2];
    for (int i = 0; i < 2; i++) {
        // Calculate vert[i] and horiz[i], and ensure that
        // vert[i] is non-negative.
        if (plug[i]) {
            vert[i] = plug[i]->getMeridinalCuts(edgeGroupRoles[i][0]);
            horiz[i] = plug[i]->getMeridinalCuts(edgeGroupRoles[i][1]);
            if (edgeGroupRoles[i][2] != 2)
                horiz[i] = -horiz[i];
        } else {
            // We have a degenerate LST, i.e., a Mobius band.
            if (edgeGroupRoles[i][0] == 2) {
                // Group 2 is glued to a vertical edge.
                vert[i] = 2;
                horiz[i] = -1;
            } else if (edgeGroupRoles[i][1] == 2) {
                // Group 2 is glued to a horizontal edge.
                vert[i] = 1;
                horiz[i] = -2;
            } else {
                // Group 2 is glued to a diagonal edge.
                vert[i] = horiz[i] = 1;
            }
        }
    }

    if (! (vert[1] == 2 && horiz[1] == -1)) {
        // Both sets of parameters must be output.
        out << " | " << vert[0] << ',' << horiz[0];
        out << " | " << vert[1] << ',' << horiz[1];
    } else {
        // The second set of parameters may be omitted.
        // Perhaps the first also.
        if (! (vert[0] == 2 && horiz[0] == -1))
            out << " | " << vert[0] << ',' << horiz[0];
    }

    return out << (tex ? "}$" : ")");
}

std::ostream& NPluggedIBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

std::ostream& NPluggedIBundle::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

void NPluggedIBundle::writeTextLong(std::ostream& out) const {
    out << "Plugged ";
    switch (coreType) {
        case NPluggedIBundleCore::T_5_1:
        case NPluggedIBundleCore::T_5_2:
        case NPluggedIBundleCore::T_5_3:
        case NPluggedIBundleCore::T_5_4:
            out << "thick"; break;
        case NPluggedIBundleCore::T_6_1:
        case NPluggedIBundleCore::T_6_2:
        case NPluggedIBundleCore::T_6_3:
        case NPluggedIBundleCore::T_6_4:
            out << "thin"; break;
    }
    out << " I-bundle: ";
    writeName(out);
}

} // namespace regina
