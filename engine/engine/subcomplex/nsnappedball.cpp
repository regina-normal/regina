
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntetrahedron.h"
    #include "nsnappedball.h"
#else
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/nsnappedball.h"
#endif

NSnappedBall* NSnappedBall::clone() const {
    NSnappedBall* ans = new NSnappedBall();
    ans->tet = tet;
    ans->equator = equator;
    return ans;
}

NSnappedBall* NSnappedBall::isSnappedBall(NTetrahedron* tet) {
    int inFace1, inFace2;
    NPerm perm;
    for (inFace1 = 0; inFace1 < 3; inFace1++)
        if (tet->getAdjacentTetrahedron(inFace1) == tet) {
            perm = tet->getAdjacentTetrahedronGluing(inFace1);
            inFace2 = perm[inFace1];
            if (perm == NPerm(inFace1, inFace2)) {
                // This is it!
                NSnappedBall* ans = new NSnappedBall();
                ans->tet = tet;
                ans->equator = edgeNumber[inFace1][inFace2];
                return ans;
            }
        }

    return 0;
}

