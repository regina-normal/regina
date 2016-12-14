
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

StandardTriangulation* StandardTriangulation::isStandardTriangulation(
        Component<3>* comp) {
    StandardTriangulation* ans;
    if ((ans = TrivialTri::isTrivialTriangulation(comp)))
        return ans;
    if ((ans = L31Pillow::isL31Pillow(comp)))
        return ans;
    if ((ans = LayeredLensSpace::isLayeredLensSpace(comp)))
        return ans;
    if ((ans = LayeredLoop::isLayeredLoop(comp)))
        return ans;
    if ((ans = LayeredChainPair::isLayeredChainPair(comp)))
        return ans;
    if ((ans = AugTriSolidTorus::isAugTriSolidTorus(comp)))
        return ans;
    if ((ans = PlugTriSolidTorus::isPlugTriSolidTorus(comp)))
        return ans;
    if ((ans = LayeredSolidTorus::isLayeredSolidTorus(comp)))
        return ans;
    if ((ans = SnapPeaCensusTri::isSmallSnapPeaCensusTri(comp)))
        return ans;

    return 0;
}

StandardTriangulation* StandardTriangulation::isStandardTriangulation(
        Triangulation<3>* tri) {
    if (tri->countComponents() != 1)
        return 0;

    // Do what we can through components.
    StandardTriangulation* ans;
    if ((ans = isStandardTriangulation(tri->component(0))))
        return ans;

    // Run tests that require entire triangulations.
    if ((ans = BlockedSFS::isBlockedSFS(tri)))
        return ans;
    if ((ans = LayeredTorusBundle::isLayeredTorusBundle(tri)))
        return ans;

    // Save non-geometric graph manifolds until last.
    if ((ans = BlockedSFSLoop::isBlockedSFSLoop(tri)))
        return ans;
    if ((ans = BlockedSFSPair::isBlockedSFSPair(tri)))
        return ans;
    if ((ans = BlockedSFSTriple::isBlockedSFSTriple(tri)))
        return ans;
    if ((ans = PluggedTorusBundle::isPluggedTorusBundle(tri)))
        return ans;

    // Nup.
    return 0;
}

} // namespace regina

