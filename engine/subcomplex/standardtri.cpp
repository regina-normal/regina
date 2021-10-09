
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

#include <sstream>
#include "subcomplex/augtrisolidtorus.h"
#include "subcomplex/blockedsfs.h"
#include "subcomplex/blockedsfsloop.h"
#include "subcomplex/blockedsfspair.h"
#include "subcomplex/blockedsfstriple.h"
#include "subcomplex/l31pillow.h"
#include "subcomplex/layeredchainpair.h"
#include "subcomplex/layeredlensspace.h"
#include "subcomplex/layeredloop.h"
#include "subcomplex/layeredtorusbundle.h"
#include "subcomplex/pluggedtorusbundle.h"
#include "subcomplex/plugtrisolidtorus.h"
#include "subcomplex/snappeacensustri.h"
#include "subcomplex/trivialtri.h"
#include "triangulation/dim3.h"

namespace regina {

std::string StandardTriangulation::name() const {
    std::ostringstream ans;
    writeName(ans);
    return ans.str();
}

std::string StandardTriangulation::TeXName() const {
    std::ostringstream ans;
    writeTeXName(ans);
    return ans.str();
}

std::unique_ptr<StandardTriangulation> StandardTriangulation::recognise(
        Component<3>* comp) {
    if (auto ans = TrivialTri::recognise(comp))
        return std::make_unique<TrivialTri>(std::move(*ans));
    if (auto ans = L31Pillow::recognise(comp))
        return std::make_unique<L31Pillow>(std::move(*ans));
    if (auto ans = LayeredLensSpace::recognise(comp))
        return std::make_unique<LayeredLensSpace>(std::move(*ans));
    if (auto ans = LayeredLoop::recognise(comp))
        return std::make_unique<LayeredLoop>(std::move(*ans));
    if (auto ans = LayeredChainPair::recognise(comp))
        return std::make_unique<LayeredChainPair>(std::move(*ans));
    if (auto ans = AugTriSolidTorus::recognise(comp))
        return std::make_unique<AugTriSolidTorus>(std::move(*ans));
    if (auto ans = PlugTriSolidTorus::recognise(comp))
        return std::make_unique<PlugTriSolidTorus>(std::move(*ans));
    if (auto ans = LayeredSolidTorus::recognise(comp))
        return std::make_unique<LayeredSolidTorus>(std::move(*ans));
    if (auto ans = SnapPeaCensusTri::recognise(comp))
        return std::make_unique<SnapPeaCensusTri>(std::move(*ans));

    return nullptr;
}

std::unique_ptr<StandardTriangulation> StandardTriangulation::recognise(
        const Triangulation<3>& tri) {
    if (tri.countComponents() != 1)
        return nullptr;

    // Do what we can through components.
    StandardTriangulation* ans;
    if (auto ans = recognise(tri.component(0)))
        return ans;

    // Run tests that require entire triangulations.
    if (auto ans = BlockedSFS::recognise(tri))
        return std::make_unique<BlockedSFS>(std::move(*ans));
    if (auto ans = LayeredTorusBundle::recognise(tri))
        return std::make_unique<LayeredTorusBundle>(std::move(*ans));

    // Save non-geometric graph manifolds until last.
    if (auto ans = BlockedSFSLoop::recognise(tri))
        return std::make_unique<BlockedSFSLoop>(std::move(*ans));
    if (auto ans = BlockedSFSPair::recognise(tri))
        return std::make_unique<BlockedSFSPair>(std::move(*ans));
    if (auto ans = BlockedSFSTriple::recognise(tri))
        return std::make_unique<BlockedSFSTriple>(std::move(*ans));
    if (auto ans = PluggedTorusBundle::recognise(tri))
        return std::make_unique<PluggedTorusBundle>(std::move(*ans));

    // Nup.
    return nullptr;
}

} // namespace regina

