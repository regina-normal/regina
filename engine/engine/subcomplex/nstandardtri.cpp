
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include <sstream>
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nl31pillow.h"
#include "subcomplex/nlayeredchainpair.h"
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/nplugtrisolidtorus.h"
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

    return 0;
}

NStandardTriangulation* NStandardTriangulation::isStandardTriangulation(
        NTriangulation* tri) {
    if (tri->getNumberOfComponents() != 1)
        return 0;
    return isStandardTriangulation(tri->getComponent(0));
}

} // namespace regina

