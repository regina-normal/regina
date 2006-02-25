
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

#include <sstream>
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nblockedsfs.h"
#include "subcomplex/nl31pillow.h"
#include "subcomplex/nlayeredchainpair.h"
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/nlayeredsurfacebundle.h"
#include "subcomplex/nngblockedsfsloop.h"
#include "subcomplex/nngblockedsfspair.h"
#include "subcomplex/nngblockedsfstriple.h"
#include "subcomplex/nngpluggedtorusbundle.h"
#include "subcomplex/nplugtrisolidtorus.h"
#include "subcomplex/nsnappeacensustri.h"
#include "subcomplex/ntrivialtri.h"
#include "triangulation/ntriangulation.h"

namespace regina {

std::string NStandardTriangulation::getName() const {
    std::ostringstream ans;
    writeName(ans);
    return ans.str();
}

std::string NStandardTriangulation::getTeXName() const {
    std::ostringstream ans;
    writeTeXName(ans);
    return ans.str();
}

NStandardTriangulation* NStandardTriangulation::isStandardTriangulation(
        NComponent* comp) {
    NStandardTriangulation* ans;
    if ((ans = NTrivialTri::isTrivialTriangulation(comp)))
        return ans;
    if ((ans = NL31Pillow::isL31Pillow(comp)))
        return ans;
    if ((ans = NLayeredLensSpace::isLayeredLensSpace(comp)))
        return ans;
    if ((ans = NLayeredLoop::isLayeredLoop(comp)))
        return ans;
    if ((ans = NLayeredChainPair::isLayeredChainPair(comp)))
        return ans;
    if ((ans = NAugTriSolidTorus::isAugTriSolidTorus(comp)))
        return ans;
    if ((ans = NPlugTriSolidTorus::isPlugTriSolidTorus(comp)))
        return ans;
    if ((ans = NLayeredSolidTorus::isLayeredSolidTorus(comp)))
        return ans;
    if ((ans = NSnapPeaCensusTri::isSmallSnapPeaCensusTri(comp)))
        return ans;

    return 0;
}

NStandardTriangulation* NStandardTriangulation::isStandardTriangulation(
        NTriangulation* tri) {
    if (tri->getNumberOfComponents() != 1)
        return 0;

    // Do what we can through components.
    NStandardTriangulation* ans;
    if ((ans = isStandardTriangulation(tri->getComponent(0))))
        return ans;

    // Run tests that require entire triangulations.
    if ((ans = NBlockedSFS::isBlockedSFS(tri)))
        return ans;
    if ((ans = NLayeredTorusBundle::isLayeredTorusBundle(tri)))
        return ans;

    // Save non-geometric graph manifolds until last.
    if ((ans = NNGBlockedSFSLoop::isNGBlockedSFSLoop(tri)))
        return ans;
    if ((ans = NNGBlockedSFSPair::isNGBlockedSFSPair(tri)))
        return ans;
    if ((ans = NNGBlockedSFSTriple::isNGBlockedSFSTriple(tri)))
        return ans;
    if ((ans = NNGPluggedTorusBundle::isNGPluggedTorusBundle(tri)))
        return ans;

    // Nup.
    return 0;
}

} // namespace regina

