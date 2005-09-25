
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "manifold/ntorusbundle.h"
#include "subcomplex/nlayeredsurfacebundle.h"
#include "subcomplex/nlayering.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    const NTxICore core_T_6_2(NTxICore::T_6_2);
    const NTxICore core_T_7(NTxICore::T_7);
}

NTxICore::NTxICore(type whichCoreType) : coreType(whichCoreType) {
    if (coreType == T_6_2) {
        const int adj[6][4] = {
            { 1, 3, 2, -1},
            { 0, 3, 2, -1},
            { 4, 5, 1, 0},
            { 1, 0, 4, 5},
            { 5, 2, 3, -1},
            { 4, 2, 3, -1}
        };

        const int glu[6][4][4] = {
            { { 0, 2, 1, 3 }, { 2, 1, 3, 0 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 0, 2, 1 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 3, 1, 0, 2 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } },
            { { 1, 3, 2, 0 }, { 3, 1, 0, 2 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } },
            { { 0, 2, 1, 3 }, { 3, 0, 2, 1 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 2, 1, 3, 0 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } }
        };

        core.insertConstruction(6, adj, glu);

        bdryTet[0][0] = 0;
        bdryTet[0][1] = 1;
        bdryTet[1][0] = 4;
        bdryTet[1][1] = 5;

        // The bdryRoles permutations are all identities.

        bdryReln[0] = NMatrix2(0, 1, -1, 0);
        bdryReln[1] = NMatrix2(0, 1, 1, 0);
        parallelReln = NMatrix2(1, 0, 0, 1);

        namePlain = "T6^2";
        nameTeX = "T_6^2";
    } else if (coreType == T_7) {
        const int adj[7][4] = {
            { 1, 3, 2, -1},
            { 0, 4, 2, -1},
            { 5, 4, 1, 0},
            { 4, 0, 5, 6},
            { 3, 1, 6, 2},
            { 6, 2, 3, -1},
            { 5, 4, 3, -1}
        };

        const int glu[7][4][4] = {
            { { 0, 2, 1, 3 }, { 2, 1, 3, 0 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 3, 1, 2, 0 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 1, 3, 2, 0 }, { 2, 3, 0, 1 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } },
            { { 0, 1, 2, 3 }, { 3, 1, 0, 2 }, { 0, 1, 2, 3 }, { 1, 0, 3, 2 } },
            { { 0, 1, 2, 3 }, { 3, 1, 2, 0 }, { 0, 2, 1, 3 }, { 2, 3, 0, 1 } },
            { { 0, 2, 1, 3 }, { 3, 0, 2, 1 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
            { { 0, 2, 1, 3 }, { 0, 2, 1, 3 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 } }
        };

        core.insertConstruction(7, adj, glu);

        bdryTet[0][0] = 0;
        bdryTet[0][1] = 1;
        bdryTet[1][0] = 5;
        bdryTet[1][1] = 6;

        // The bdryRoles permutations are all identities.

        bdryReln[0] = NMatrix2(0, 1, 1, 0);
        bdryReln[1] = NMatrix2(0, 1, -1, 0);
        parallelReln = NMatrix2(1, 0, 1, 1);

        namePlain = "T7";
        nameTeX = "T_7";
    }
}

NLayeredTorusBundle::~NLayeredTorusBundle() {
    delete coreIso;
}

NLayeredTorusBundle* NLayeredTorusBundle::isLayeredTorusBundle(
        NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfVertices() > 1)
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;
    if (tri->getNumberOfTetrahedra() < 6)
        return 0;

    // We have a 1-vertex 1-component closed triangulation with at least
    // six tetrahedra.

    // Hunt for the core thin torus bundle.
    NLayeredTorusBundle* ans;
    if ((ans = hunt(tri, core_T_6_2)))
        return ans;
    if ((ans = hunt(tri, core_T_7)))
        return ans;

    return 0;
}

NLayeredTorusBundle* NLayeredTorusBundle::hunt(NTriangulation* tri,
        const NTxICore& core) {
    std::list<NIsomorphism*> isos;
    if (! core.core.findAllSubcomplexesIn(*tri, isos))
        return 0;

    // Run through each isomorphism and look for the corresponding layering.
    NMatrix2 matchReln;
    for (std::list<NIsomorphism*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        // Apply the layering to the lower boundary and see if it
        // matches nicely with the upper.
        NLayering layering(
            tri->getTetrahedron((*it)->tetImage(core.bdryTet[1][0])),
            (*it)->facePerm(core.bdryTet[1][0]) * core.bdryRoles[1][0],
            tri->getTetrahedron((*it)->tetImage(core.bdryTet[1][1])),
            (*it)->facePerm(core.bdryTet[1][1]) * core.bdryRoles[1][1]);
        layering.extend();

        if (! layering.matchesTop(
                tri->getTetrahedron((*it)->tetImage(core.bdryTet[0][0])),
                (*it)->facePerm(core.bdryTet[0][0]) * core.bdryRoles[0][0],
                tri->getTetrahedron((*it)->tetImage(core.bdryTet[0][1])),
                (*it)->facePerm(core.bdryTet[0][1]) * core.bdryRoles[0][1],
                matchReln)) {
            // Delete this isomorphism; we won't need it any more.
            delete *it;
            continue;
        }

        // It's a match!
        NLayeredTorusBundle* ans = new NLayeredTorusBundle(core);
        ans->coreIso = *it;
        ans->reln = core.bdryReln[0] * matchReln * core.bdryReln[1].inverse();

        // Delete the remaining isomorphisms that we never even
        // looked at.
        for (it++; it != isos.end(); it++)
            delete *it;

        return ans;
    }

    // Nothing found.
    return 0;
}

NManifold* NLayeredTorusBundle::getManifold() const {
    return new NTorusBundle(core.parallelReln * reln);
}

std::ostream& NLayeredTorusBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    if (tex)
        out << "B_{" << core.nameTeX;
    else
        out << "B(" << core.namePlain;

    out << " | " << reln[0][0] << ',' << reln[0][1];
    out << " | " << reln[1][0] << ',' << reln[1][1];

    return out << (tex ? "}" : ")");
}

void NLayeredTorusBundle::writeTextLong(std::ostream& out) const {
    out << "Layered torus bundle: ";
    writeName(out);
}

} // namespace regina
