
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
#include "subcomplex/ntxicore.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    const NTxIDiagonalCore core_T_6_1(6, 1);
    const NTxIDiagonalCore core_T_7_1(7, 1);
    const NTxIDiagonalCore core_T_8_1(8, 1);
    const NTxIDiagonalCore core_T_8_2(8, 2);
    const NTxIDiagonalCore core_T_9_1(9, 1);
    const NTxIDiagonalCore core_T_9_2(9, 2);
    const NTxIDiagonalCore core_T_10_1(10, 1);
    const NTxIDiagonalCore core_T_10_2(10, 2);
    const NTxIDiagonalCore core_T_10_3(10, 3);
    const NTxIParallelCore core_T_p;
}

NLayeredTorusBundle::~NLayeredTorusBundle() {
    delete coreIso_;
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
    if ((ans = hunt(tri, core_T_6_1)))
        return ans;
    if ((ans = hunt(tri, core_T_7_1)))
        return ans;
    if ((ans = hunt(tri, core_T_8_1)))
        return ans;
    if ((ans = hunt(tri, core_T_8_2)))
        return ans;
    if ((ans = hunt(tri, core_T_9_1)))
        return ans;
    if ((ans = hunt(tri, core_T_9_2)))
        return ans;
    if ((ans = hunt(tri, core_T_10_1)))
        return ans;
    if ((ans = hunt(tri, core_T_10_2)))
        return ans;
    if ((ans = hunt(tri, core_T_10_3)))
        return ans;
    if ((ans = hunt(tri, core_T_p)))
        return ans;

    return 0;
}

NLayeredTorusBundle* NLayeredTorusBundle::hunt(NTriangulation* tri,
        const NTxICore& core) {
    std::list<NIsomorphism*> isos;
    if (! core.core().findAllSubcomplexesIn(*tri, isos))
        return 0;

    // Run through each isomorphism and look for the corresponding layering.
    NMatrix2 matchReln;
    for (std::list<NIsomorphism*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        // Apply the layering to the lower boundary and see if it
        // matches nicely with the upper.
        NLayering layering(
            tri->getTetrahedron((*it)->tetImage(core.bdryTet(1,0))),
            (*it)->facePerm(core.bdryTet(1,0)) * core.bdryRoles(1,0),
            tri->getTetrahedron((*it)->tetImage(core.bdryTet(1,1))),
            (*it)->facePerm(core.bdryTet(1,1)) * core.bdryRoles(1,1));
        layering.extend();

        if (layering.matchesTop(
                tri->getTetrahedron((*it)->tetImage(core.bdryTet(0,0))),
                (*it)->facePerm(core.bdryTet(0,0)) * core.bdryRoles(0,0),
                tri->getTetrahedron((*it)->tetImage(core.bdryTet(0,1))),
                (*it)->facePerm(core.bdryTet(0,1)) * core.bdryRoles(0,1),
                matchReln)) {
            // It's a match!
            NLayeredTorusBundle* ans = new NLayeredTorusBundle(core);
            ans->coreIso_ = *it;
            ans->reln_ = core.bdryReln(0) * matchReln *
                core.bdryReln(1).inverse();

            // Delete the remaining isomorphisms that we never even
            // looked at.
            for (it++; it != isos.end(); it++)
                delete *it;

            return ans;
        }

        // No match.  Delete this isomorphism; we won't need it any more.
        delete *it;
        continue;
    }

    // Nothing found.
    return 0;
}

NManifold* NLayeredTorusBundle::getManifold() const {
    return new NTorusBundle(core_.parallelReln() * reln_);
}

std::ostream& NLayeredTorusBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    if (tex) {
        out << "B_{";
        core_.writeTeXName(out);
    } else {
        out << "B(";
        core_.writeName(out);
    }

    out << " | " << reln_[0][0] << ',' << reln_[0][1];
    out << " | " << reln_[1][0] << ',' << reln_[1][1];

    return out << (tex ? "}" : ")");
}

void NLayeredTorusBundle::writeTextLong(std::ostream& out) const {
    out << "Layered torus bundle: ";
    writeName(out);
}

} // namespace regina
