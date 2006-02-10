
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "subcomplex/nlayering.h"
#include "subcomplex/nngpluggedtorusbundle.h"
#include "subcomplex/nsatregion.h"
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

NNGPluggedTorusBundle::~NNGPluggedTorusBundle() {
    delete coreIso_;
    delete plug_;
}

NManifold* NNGPluggedTorusBundle::getManifold() const {
    // TODO
    return 0;
}

std::ostream& NNGPluggedTorusBundle::writeName(std::ostream& out) const {
    // TODO
    return out << "Non-geometric plugged torus bundle";
}

std::ostream& NNGPluggedTorusBundle::writeTeXName(std::ostream& out) const {
    // TODO
    return out << "Non-geometric plugged torus bundle";
}

void NNGPluggedTorusBundle::writeTextLong(std::ostream& out) const {
    // TODO
    out << "Non-geometric plugged torus bundle";
}

NNGPluggedTorusBundle* NNGPluggedTorusBundle::isNGPluggedTorusBundle(
        NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;

    // The smallest non-trivial examples of these have nine tetrahedra
    // (six for the TxI core and another three for a non-trivial plug).
    if (tri->getNumberOfTetrahedra() < 9)
        return 0;

    // We have a closed and connected triangulation with at least
    // nine tetrahedra.

    // Hunt for the core thin torus bundle.
    NNGPluggedTorusBundle* ans;
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

NNGPluggedTorusBundle* NNGPluggedTorusBundle::hunt(NTriangulation* triang,
        const NTxICore& core) {
    std::list<NIsomorphism*> isos;
    if (! core.core().findAllSubcomplexesIn(*triang, isos))
        return 0;

    int i;
    NPerm upperLayerToAnnulus, upperBdryToLower;
    NSatAnnulus upperAnnulus, lowerAnnulus, bdryAnnulus;
    NSatBlock::TetList avoidTets;
    NSatBlock* starter;
    NSatRegion* region;
    bool swapFaces;

    // Run through each isomorphism and look for the corresponding layering.
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
        // housing a non-trivial plug.
        if (layerLower.getSize() + layerUpper.getSize() +
                core.core().getNumberOfTetrahedra() + 3 >
                triang->getNumberOfTetrahedra()) {
            // No good.  Move on.
            delete *it;
            continue;
        }

        lowerAnnulus.tet[0] = layerLower.getNewBoundaryTet(0);
        lowerAnnulus.tet[1] = layerLower.getNewBoundaryTet(1);
        lowerAnnulus.roles[0] = layerLower.getNewBoundaryRoles(0);
        lowerAnnulus.roles[1] = layerLower.getNewBoundaryRoles(1);

        // Look for the SFS plug.
        for (i = 0; i < 3; i++) {
            // Construct the permutation from 0/1/2 markings on the
            // first boundary face above the layering to the 0/1/2
            // markings of the first saturated annulus boundary.
            upperLayerToAnnulus = NPerm(i, (i + 1) % 3, (i + 2) % 3, 3);

            upperAnnulus.tet[0] = layerUpper.getNewBoundaryTet(0);
            upperAnnulus.tet[1] = layerUpper.getNewBoundaryTet(1);
            upperAnnulus.roles[0] = layerUpper.getNewBoundaryRoles(0)
                * upperLayerToAnnulus.inverse();
            upperAnnulus.roles[1] = layerUpper.getNewBoundaryRoles(1)
                * upperLayerToAnnulus.inverse();

            // Recall that we already know the triangulation to be closed.
            upperAnnulus.switchSides();

            // Construct the list of tetrahedra to avoid when searching for
            // the plug.  Don't worry about all the internal tetrahedra
            // within the layerings or the core; as long as we've got the
            // boundary tetrahedra we'll be fine.
            avoidTets.clear();
            avoidTets.insert(layerUpper.getNewBoundaryTet(0));
            avoidTets.insert(layerUpper.getNewBoundaryTet(1));
            avoidTets.insert(layerLower.getNewBoundaryTet(0));
            avoidTets.insert(layerLower.getNewBoundaryTet(1));

            starter = NSatBlock::isBlock(upperAnnulus, avoidTets);
            if (! starter)
                continue;

            // We have a starter block.  Make a region out of it, and
            // ensure that region has precisely two boundary annuli.
            region = new NSatRegion(starter);
            region->expand(avoidTets, false);

            if (region->numberOfBoundaryAnnuli() != 2) {
                delete region;
                continue;
            }

            // From the NSatRegion specifications we know that the first
            // boundary annulus will be upperAnnulus.  Find the second.
            bdryAnnulus = region->boundaryAnnulus(1);

            // Hope like hell that this meets up with the lower layering
            // boundary.
            bdryAnnulus.switchSides();
            if (bdryAnnulus.tet[0] == lowerAnnulus.tet[0] &&
                    bdryAnnulus.tet[1] == lowerAnnulus.tet[1] &&
                    bdryAnnulus.roles[0][3] == lowerAnnulus.roles[0][3] &&
                    bdryAnnulus.roles[1][3] == lowerAnnulus.roles[1][1]) {
                // Construct the mapping of 0/1/2 markings from the
                // upper boundary annulus to the lower.
                upperBdryToLower = lowerAnnulus.roles[0].inverse() *
                    bdryAnnulus.roles[0];
                if (upperBdryToLower != lowerAnnulus.roles[1].inverse() *
                    bdryAnnulus.roles[1]) {
                    delete region;
                    continue;
                }

                // Yup!
                swapFaces = false;
            } else if (bdryAnnulus.tet[0] == lowerAnnulus.tet[1] &&
                    bdryAnnulus.tet[1] == lowerAnnulus.tet[0] &&
                    bdryAnnulus.roles[0][3] == lowerAnnulus.roles[1][3] &&
                    bdryAnnulus.roles[1][3] == lowerAnnulus.roles[0][1]) {
                // Construct the mapping of 0/1/2 markings from the
                // upper boundary annulus to the lower.
                upperBdryToLower = lowerAnnulus.roles[0].inverse() *
                    bdryAnnulus.roles[1];
                if (upperBdryToLower != lowerAnnulus.roles[1].inverse() *
                    bdryAnnulus.roles[0]) {
                    delete region;
                    continue;
                }

                // Yup!
                swapFaces = true;
            }

            // All good!
            NNGPluggedTorusBundle* ans = new NNGPluggedTorusBundle(core, *it,
                region);

            // Before we head home, delete the remaining isomorphisms
            // that we never looked at.
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

} // namespace regina
