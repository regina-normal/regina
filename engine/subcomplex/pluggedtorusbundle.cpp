
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "manifold/graphloop.h"
#include "manifold/sfs.h"
#include "subcomplex/layering.h"
#include "subcomplex/pluggedtorusbundle.h"
#include "subcomplex/satregion.h"
#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"

namespace regina {

namespace {
    const TxIDiagonalCore core_T_6_1(6, 1);
    const TxIDiagonalCore core_T_7_1(7, 1);
    const TxIDiagonalCore core_T_8_1(8, 1);
    const TxIDiagonalCore core_T_8_2(8, 2);
    const TxIDiagonalCore core_T_9_1(9, 1);
    const TxIDiagonalCore core_T_9_2(9, 2);
    const TxIDiagonalCore core_T_10_1(10, 1);
    const TxIDiagonalCore core_T_10_2(10, 2);
    const TxIDiagonalCore core_T_10_3(10, 3);
    const TxIParallelCore core_T_p;
}

PluggedTorusBundle::~PluggedTorusBundle() {
    delete bundleIso_;
    delete region_;
}

Manifold* PluggedTorusBundle::manifold() const {
    SFSpace* sfs = region_->createSFS(false);
    if (! sfs)
        return 0;
    if (sfs->punctures() == 1) {
        // The region has one larger boundary, but we pinch it to create
        // two smaller boundaries.
        sfs->addPuncture();
    }

    sfs->reduce(false);

    return new GraphLoop(sfs, matchingReln_);
}

std::ostream& PluggedTorusBundle::writeName(std::ostream& out) const {
    out << "Plugged Torus Bundle [";
    bundle_.writeName(out);
    out << " | ";
    region_->writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& PluggedTorusBundle::writeTeXName(std::ostream& out) const {
    out << "\\mathrm{PTB}\\left[";
    bundle_.writeTeXName(out);
    out << "\\,|\\n";
    region_->writeBlockAbbrs(out, true);
    return out << "\\right]";
}

void PluggedTorusBundle::writeTextLong(std::ostream& out) const {
    out << "Plugged torus bundle, fibre/orbifold relation " << matchingReln_
        << '\n';
    out << "Thin I-bundle: ";
    bundle_.writeName(out);
    out << '\n';
    region_->writeDetail(out, "Saturated region");
}

PluggedTorusBundle* PluggedTorusBundle::isPluggedTorusBundle(
        Triangulation<3>* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->countComponents() > 1)
        return 0;

    // The smallest non-trivial examples of these have nine tetrahedra
    // (six for the TxI core and another three for a non-trivial region).
    if (tri->size() < 9)
        return 0;

    // We have a closed and connected triangulation with at least
    // nine tetrahedra.

    // Hunt for the thin torus bundle.
    PluggedTorusBundle* ans;
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

PluggedTorusBundle* PluggedTorusBundle::hunt(Triangulation<3>* triang,
        const TxICore& bundle) {
    std::list<Isomorphism<3>*> isos;
    if (! bundle.core().findAllSubcomplexesIn(*triang, back_inserter(isos)))
        return 0;

    int regionPos;
    Perm<4> annulusToUpperLayer;
    SatAnnulus upperAnnulus, lowerAnnulus, bdryAnnulus;
    SatBlock::TetList avoidTets;
    SatBlock* starter;
    SatRegion* region;
    bool bdryRefVert, bdryRefHoriz;

    // Run through each isomorphism and look for the corresponding layering.
    for (std::list<Isomorphism<3>*>::const_iterator it = isos.begin();
            it != isos.end(); it++) {
        // Apply layerings to the upper and lower boundaries.
        Layering layerUpper(
            triang->tetrahedron((*it)->tetImage(bundle.bdryTet(0,0))),
            (*it)->facePerm(bundle.bdryTet(0,0)) * bundle.bdryRoles(0,0),
            triang->tetrahedron((*it)->tetImage(bundle.bdryTet(0,1))),
            (*it)->facePerm(bundle.bdryTet(0,1)) * bundle.bdryRoles(0,1));
        layerUpper.extend();

        Layering layerLower(
            triang->tetrahedron((*it)->tetImage(bundle.bdryTet(1,0))),
            (*it)->facePerm(bundle.bdryTet(1,0)) * bundle.bdryRoles(1,0),
            triang->tetrahedron((*it)->tetImage(bundle.bdryTet(1,1))),
            (*it)->facePerm(bundle.bdryTet(1,1)) * bundle.bdryRoles(1,1));
        layerLower.extend();

        // Count tetrahedra to ensure that the layerings haven't crossed.
        // In fact, we should have at least three spare tetrahedra for
        // housing a non-trivial saturated region.
        if (layerLower.size() + layerUpper.size() +
                bundle.core().size() + 3 > triang->size()) {
            // No good.  Move on.
            delete *it;
            continue;
        }

        lowerAnnulus.tet[0] = layerLower.newBoundaryTet(0);
        lowerAnnulus.tet[1] = layerLower.newBoundaryTet(1);
        lowerAnnulus.roles[0] = layerLower.newBoundaryRoles(0);
        lowerAnnulus.roles[1] = layerLower.newBoundaryRoles(1);

        // Look for the saturated region.
        for (regionPos = 0; regionPos < 3; regionPos++) {
            // Construct the permutation from 0/1/2 markings on the
            // first saturated annulus boundary to 0/1/2 markings on the
            // first boundary triangle above the layering.
            annulusToUpperLayer = Perm<4>(regionPos, (regionPos + 1) % 3,
                (regionPos + 2) % 3, 3);

            upperAnnulus.tet[0] = layerUpper.newBoundaryTet(0);
            upperAnnulus.tet[1] = layerUpper.newBoundaryTet(1);
            upperAnnulus.roles[0] = layerUpper.newBoundaryRoles(0)
                * annulusToUpperLayer;
            upperAnnulus.roles[1] = layerUpper.newBoundaryRoles(1)
                * annulusToUpperLayer;

            // Recall that we already know the triangulation to be closed.
            upperAnnulus.switchSides();

            // Construct the list of tetrahedra to avoid when searching for the
            // saturated region.  Don't worry about all the internal tetrahedra
            // within the layerings or the thin I-bundle; as long as we've got
            // the boundary tetrahedra we'll be fine.
            avoidTets.clear();
            avoidTets.insert(layerUpper.newBoundaryTet(0));
            avoidTets.insert(layerUpper.newBoundaryTet(1));
            avoidTets.insert(layerLower.newBoundaryTet(0));
            avoidTets.insert(layerLower.newBoundaryTet(1));

            starter = SatBlock::isBlock(upperAnnulus, avoidTets);
            if (! starter)
                continue;

            // We have a starter block.  Make a region out of it, and
            // ensure that region has precisely two boundary annuli.
            region = new SatRegion(starter);
            region->expand(avoidTets, false);

            if (region->numberOfBoundaryAnnuli() != 2) {
                delete region;
                continue;
            }

            // From the SatRegion specifications we know that the first
            // boundary annulus will be upperAnnulus.  Find the second.
            bdryAnnulus = region->boundaryAnnulus(1, bdryRefVert, bdryRefHoriz);

            // Hope like hell that this meets up with the lower layering
            // boundary.  Note that this will force it to be a torus also.
            Matrix2 upperRolesToLower;
            if (! lowerAnnulus.isJoined(bdryAnnulus, upperRolesToLower)) {
                delete region;
                continue;
            }

            // All good!
            // Better work out what we've got here.

            // Mapping from fibre/base curves (f0, o0) to upperAnnulus
            // edges (first triangle: 01, first triangle: 02).
            Matrix2 curvesToUpperAnnulus(-1, 0, 0, 1);

            // Mapping from upperAnnulus edges (first: 01, first: 02) to
            // upper layering boundary roles (first: 01, first: 02).
            Matrix2 upperAnnulusToUpperLayer;
            if (regionPos == 0)
                upperAnnulusToUpperLayer = Matrix2(1, 0, 0, 1);
            else if (regionPos == 1)
                upperAnnulusToUpperLayer = Matrix2(0, -1, 1, -1);
            else
                upperAnnulusToUpperLayer = Matrix2(-1, 1, -1, 0);

            // Mapping from upper layering boundary roles
            // (first: 01, first: 02) to the bundle boundary 0 roles
            // (first: 01, first: 02) is layerUpper.boundaryReln().inverse().

            // Mapping from bundle boundary 0 roles (first: 01, first: 02) to
            // bundle boundary 0 (alpha, beta) is bundle.bdryReln(0).

            // Mapping from bundle boundary 0 (alpha, beta) to bundle boundary 1
            // (alpha, beta) is bundle.parallelReln().

            // Mapping from bundle boundary 1 (alpha, beta) to bundle boundary 1
            // roles (first: 01, first: 02) is bundle.bdryReln(1).inverse().

            // Mapping from bundle boundary 1 roles (first: 01, first: 02) to
            // lower layering boundary roles (first: 01, first: 02) is
            // layerLower.boundaryReln().

            // Mapping from lower layering boundary roles (first: 01, first: 02)
            // to lower annulus boundary roles (first: 01, first: 02) is the
            // identity.

            // SO: Here comes the mapping from fibre/base curves (f0, o0)
            // to lower annulus boundary roles (first: 01, first: 02):
            Matrix2 curvesToLowerAnnulus =
                layerLower.boundaryReln() *
                bundle.bdryReln(1).inverse() *
                bundle.parallelReln() *
                bundle.bdryReln(0) *
                layerUpper.boundaryReln().inverse() *
                upperAnnulusToUpperLayer *
                curvesToUpperAnnulus;

            // Now let's work out the mapping from fibre/base curves (f1, o1)
            // to bdryAnnulus roles (first: 01, first: 02).  This is
            // rather simpler.
            Matrix2 curvesToBdryAnnulus(bdryRefVert ? 1 : -1, 0, 0,
                bdryRefHoriz ? -1 : 1);

            // Finally, we already know how the two annuli are joined
            // together -- we worked this out earlier as upperRolesToLower.
            // Note that curvesToBdryAnnulus is self-inverse, so we won't
            // bother inverting it even though we should.
            PluggedTorusBundle* ans = new PluggedTorusBundle(bundle, *it,
                region, curvesToBdryAnnulus * upperRolesToLower.inverse() *
                curvesToLowerAnnulus);

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
