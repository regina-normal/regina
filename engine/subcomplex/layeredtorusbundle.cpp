
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

#include "manifold/torusbundle.h"
#include "subcomplex/layeredtorusbundle.h"
#include "subcomplex/layering.h"
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
    const TxIDiagonalCore core_T_11_1(11, 1);
    const TxIDiagonalCore core_T_11_2(11, 2);
    const TxIDiagonalCore core_T_11_3(11, 3);
    const TxIDiagonalCore core_T_12_1(12, 1);
    const TxIDiagonalCore core_T_12_2(12, 2);
    const TxIDiagonalCore core_T_12_3(12, 3);
    const TxIDiagonalCore core_T_12_4(12, 4);
    const TxIParallelCore core_T_p;
}

std::unique_ptr<LayeredTorusBundle> LayeredTorusBundle::recognise(
        const Triangulation<3>& tri) {
    // Basic property checks.
    if (! tri.isClosed())
        return nullptr;
    if (tri.countVertices() > 1)
        return nullptr;
    if (tri.countComponents() > 1)
        return nullptr;
    if (tri.size() < 6)
        return nullptr;

    // We have a 1-vertex 1-component closed triangulation with at least
    // six tetrahedra.

    // Hunt for the core thin torus bundle.
    if (auto ans = hunt(tri, core_T_6_1))
        return ans;
    if (auto ans = hunt(tri, core_T_7_1))
        return ans;
    if (auto ans = hunt(tri, core_T_8_1))
        return ans;
    if (auto ans = hunt(tri, core_T_8_2))
        return ans;
    if (auto ans = hunt(tri, core_T_9_1))
        return ans;
    if (auto ans = hunt(tri, core_T_9_2))
        return ans;
    if (auto ans = hunt(tri, core_T_10_1))
        return ans;
    if (auto ans = hunt(tri, core_T_10_2))
        return ans;
    if (auto ans = hunt(tri, core_T_10_3))
        return ans;
    if (auto ans = hunt(tri, core_T_11_1))
        return ans;
    if (auto ans = hunt(tri, core_T_11_2))
        return ans;
    if (auto ans = hunt(tri, core_T_11_3))
        return ans;
    if (auto ans = hunt(tri, core_T_12_1))
        return ans;
    if (auto ans = hunt(tri, core_T_12_2))
        return ans;
    if (auto ans = hunt(tri, core_T_12_3))
        return ans;
    if (auto ans = hunt(tri, core_T_12_4))
        return ans;
    if (auto ans = hunt(tri, core_T_p))
        return ans;

    return nullptr;
}

std::unique_ptr<LayeredTorusBundle> LayeredTorusBundle::hunt(
        const Triangulation<3>& tri, const TxICore& core) {
    std::unique_ptr<LayeredTorusBundle> ans;
    core.core().findAllSubcomplexesIn(tri,
            [&ans, &core, &tri](const Isomorphism<3>& iso) {
        // Look for the corresponding layering.
        Matrix2 matchReln;

        // Apply the layering to the lower boundary and see if it
        // matches nicely with the upper.
        Layering layering(
            tri.tetrahedron(iso.tetImage(core.bdryTet(1,0))),
            iso.facePerm(core.bdryTet(1,0)) * core.bdryRoles(1,0),
            tri.tetrahedron(iso.tetImage(core.bdryTet(1,1))),
            iso.facePerm(core.bdryTet(1,1)) * core.bdryRoles(1,1));
        layering.extend();

        if (layering.matchesTop(
                tri.tetrahedron(iso.tetImage(core.bdryTet(0,0))),
                iso.facePerm(core.bdryTet(0,0)) * core.bdryRoles(0,0),
                tri.tetrahedron(iso.tetImage(core.bdryTet(0,1))),
                iso.facePerm(core.bdryTet(0,1)) * core.bdryRoles(0,1),
                matchReln)) {
            // It's a match!
            //
            // Note: we cannot use make_unique here, since the class
            // constructor is private.
            ans.reset(new LayeredTorusBundle(core, iso,
                core.bdryReln(0) * matchReln * core.bdryReln(1).inverse()));
            return true;
        }

        // No match.
        return false;
    });
    return ans;
}

std::unique_ptr<Manifold> LayeredTorusBundle::manifold() const {
    // Note that this one-liner appears again in homology(), where
    // we use the underlying TorusBundle for homology calculations.
    return std::make_unique<TorusBundle>(core_->parallelReln() * reln_);
}

AbelianGroup LayeredTorusBundle::homology() const {
    // It's implemented in TorusBundle, so ride on that for now.
    // We'll implement it directly here in good time.
    return TorusBundle(core_->parallelReln() * reln_).homology();
}

std::ostream& LayeredTorusBundle::writeCommonName(std::ostream& out,
        bool tex) const {
    if (tex) {
        out << "B_{";
        core_->writeTeXName(out);
    } else {
        out << "B(";
        core_->writeName(out);
    }

    out << " | " << reln_[0][0] << ',' << reln_[0][1];
    out << " | " << reln_[1][0] << ',' << reln_[1][1];

    return out << (tex ? "}" : ")");
}

void LayeredTorusBundle::writeTextLong(std::ostream& out) const {
    out << "Layered torus bundle: ";
    writeName(out);
}

} // namespace regina
