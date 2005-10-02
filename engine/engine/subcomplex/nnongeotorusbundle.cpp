
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

#include <memory>
#include "subcomplex/nnongeotorusbundle.h"
#include "subcomplex/nlayering.h"
#include "subcomplex/npluggedsfs.h"
#include "subcomplex/ntrisolidtorus.h"
#include "subcomplex/ntxicore.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/npermit.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    const NTxIDiagonalCore core_T_6_1(6, 1);
    const NTxIDiagonalCore core_T_7_1(7, 1);
    const NTxIParallelCore core_T_p;
}

NNonGeoTorusBundle::NNonGeoTorusBundle(const NTxICore& whichCore,
        NIsomorphism* coreIso, NSFSPlug* plug,
        const NMatrix2& fibreRelnUpper, const NMatrix2& fibreRelnLower) :
        core_(whichCore), coreIso_(coreIso), plug_(plug) {
    fibreReln_[0] = fibreRelnUpper;
    fibreReln_[1] = fibreRelnLower;

    matchingReln_ = fibreRelnLower * core_.parallelReln() *
        fibreRelnUpper.inverse();
}

NNonGeoTorusBundle::~NNonGeoTorusBundle() {
    delete coreIso_;
    delete plug_;
}

NNonGeoTorusBundle* NNonGeoTorusBundle::isNonGeoTorusBundle(
        NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfVertices() > 1)
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;
    if (tri->getNumberOfTetrahedra() < 9)
        return 0;

    // We have a 1-vertex 1-component closed triangulation with at least
    // nine tetrahedra.

    // Hunt for the core thin torus bundle.
    NNonGeoTorusBundle* ans;
    if ((ans = hunt(tri, core_T_6_1)))
        return ans;
    if ((ans = hunt(tri, core_T_7_1)))
        return ans;
    if ((ans = hunt(tri, core_T_p)))
        return ans;

    return 0;
}

NNonGeoTorusBundle* NNonGeoTorusBundle::hunt(NTriangulation* triang,
        const NTxICore& core) {
    std::list<NIsomorphism*> isos;
    if (! core.core().findAllSubcomplexesIn(*triang, isos))
        return 0;

    // Run through each isomorphism and look for the corresponding layering.
    NMatrix2 layerRelnUpper, layerRelnLower;
    NPermItS4 pit;
    std::auto_ptr<NTriSolidTorus> tri;
    std::auto_ptr<NSFSPlug> plug;
    NTetrahedron* t;
    NPerm p;
    for (std::list<NIsomorphism*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        // Apply layerings to the upper and lower boundaries.
        NLayering layerUpper(
            triang->getTetrahedron((*it)->tetImage(core.bdryTet(0,0))),
            (*it)->facePerm(core.bdryTet(0,0)) * core.bdryRoles(0,0),
            triang->getTetrahedron((*it)->tetImage(core.bdryTet(0,1))),
            (*it)->facePerm(core.bdryTet(0,1)) * core.bdryRoles(0,1));
        layerUpper.extend();

        NLayering layerLower(
            triang->getTetrahedron((*it)->tetImage(core.bdryTet(1,0))),
            (*it)->facePerm(core.bdryTet(1,0)) * core.bdryRoles(1,0),
            triang->getTetrahedron((*it)->tetImage(core.bdryTet(1,1))),
            (*it)->facePerm(core.bdryTet(1,1)) * core.bdryRoles(1,1));
        layerLower.extend();

        // Count tetrahedra to ensure that the layerings haven't crossed.
        // In fact, we should have at least three spare tetrahedra for
        // housing the exceptional fibre.
        if (layerLower.getSize() + layerUpper.getSize() +
                core.core().getNumberOfTetrahedra() + 3 >
                triang->getNumberOfTetrahedra()) {
            // No good.  Move on.
            delete *it;
            continue;
        }

        // Remember: from isNonGeoTorusBundle() we know that the
        // triangulation has no boundary faces.

        // Look for the triangular solid torus at the centre.
        for (pit.init(); ! pit.done(); pit++) {
            // Examine *pit as a potential map from the (0,1,2,3) roles
            // on the triangular solid torus to the (0,1,2) torus boundary.

            // Make sure the torus boundary contains an axis edge.
            if ((*pit)[0] == 3 || (*pit)[3] == 3)
                continue;

            t = layerUpper.getNewBoundaryTet(0);
            p = layerUpper.getNewBoundaryRoles(0);
            tri.reset(NTriSolidTorus::formsTriSolidTorus(
                t->getAdjacentTetrahedron(p[3]),
                t->getAdjacentTetrahedronGluing(p[3]) * p * (*pit)));
            if (! tri.get())
                continue;

            // See if the triangular solid torus matches completely on
            // both sides.
            // To NLayering::matchesTop(), we present axis edges as 01 and
            // major edges as 02.
            if (! layerUpper.matchesTop(
                    tri->getTetrahedron(0),
                    tri->getVertexRoles(0) * NPerm(0, 3, 1, 2),
                    tri->getTetrahedron(1),
                    tri->getVertexRoles(1) * NPerm(3, 0, 2, 1),
                    layerRelnUpper))
                continue;
            if (! layerLower.matchesTop(
                    tri->getTetrahedron(1),
                    tri->getVertexRoles(1) * NPerm(0, 3, 1, 2),
                    tri->getTetrahedron(2),
                    tri->getVertexRoles(2) * NPerm(3, 0, 2, 1),
                    layerRelnLower))
                continue;

            // Looking good.  Finally, hunt for the plug.
            // For the annulus boundary, 01 is an axis edge and 02 is a
            // minor edge.
            NSFSAnnulus plugBdry(
                tri->getTetrahedron(0),
                tri->getVertexRoles(0) * NPerm(0, 3, 2, 1),
                tri->getTetrahedron(2),
                tri->getVertexRoles(2) * NPerm(3, 0, 1, 2));

            // TODO: Check diagonals?
            plug.reset(NSFSPlug::isPlugged(plugBdry));
            if (! plug.get())
                continue;

            // We have it, folks!
            NNonGeoTorusBundle* ans = new NNonGeoTorusBundle(core, *it,
                plug.release(),
                layerRelnUpper * core.bdryReln(0).inverse(),
                layerRelnLower * core.bdryReln(1).inverse());

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

NManifold* NNonGeoTorusBundle::getManifold() const {
    // TODO
    return 0;
}

std::ostream& NNonGeoTorusBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    // TODO
    if (tex) {
        out << "NGB_{";
        core_.writeTeXName(out);
    } else {
        out << "NGB(";
        core_.writeName(out);
    }

    out << " | " << matchingReln_[0][0] << ',' << matchingReln_[0][1];
    out << " | " << matchingReln_[1][0] << ',' << matchingReln_[1][1];

    out << " | ";
    if (tex)
        plug_->writeTeXName(out);
    else
        plug_->writeName(out);

    return out << (tex ? "}" : ")");
}

void NNonGeoTorusBundle::writeTextLong(std::ostream& out) const {
    out << "Non-geometric torus bundle: ";
    writeName(out);
}

} // namespace regina
