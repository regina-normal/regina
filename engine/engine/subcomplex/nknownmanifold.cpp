
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "triangulation/ntriangulation.h"
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nlayeredchainpair.h"
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/nplugtrisolidtorus.h"

namespace regina {

NSFS* isKnownSFS(NTriangulation* tri) {
    if (tri->getNumberOfComponents() != 1)
        return 0;

    // We have just one component.
    NComponent* comp = tri->getComponents().front();

    // Layered lens space?
    {
        NLayeredLensSpace* lens = NLayeredLensSpace::isLayeredLensSpace(comp);
        if (lens) {
            NSFS* ans = new NSFS();
            ans->insertFibre(NExceptionalFibre(lens->getQ(), lens->getP()));
            delete lens;
            return ans;
        }
    }

    // Layered loop?
    {
        NLayeredLoop* loop = NLayeredLoop::isLayeredLoop(comp);
        if (loop) {
            NSFS* ans = new NSFS(loop->getSeifertStructure());
            delete loop;
            return ans;
        }
    }

    // Layered chain pair?
    {
        NLayeredChainPair* pair = NLayeredChainPair::isLayeredChainPair(comp);
        if (pair) {
            NSFS* ans = new NSFS(pair->getSeifertStructure());
            delete pair;
            return ans;
        }
    }

    // Augmented triangular solid torus?
    {
        NAugTriSolidTorus* aug = NAugTriSolidTorus::isAugTriSolidTorus(comp);
        if (aug) {
            NSFS* ans = new NSFS(aug->getSeifertStructure());
            delete aug;
            return ans;
        }
    }

    // Plugged triangular solid torus?
    {
        NPlugTriSolidTorus* plug = NPlugTriSolidTorus::isPlugTriSolidTorus(tri);
        if (plug) {
            NSFS* ans = new NSFS(plug->getSeifertStructure());
            delete plug;
            return ans;
        }
    }

    // Nothing recognised.
    return 0;
}

} // namespace regina

