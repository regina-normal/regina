
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
    #include "ntrisolidtorus.h"
#else
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/ntrisolidtorus.h"
#endif

NTriSolidTorus* NTriSolidTorus::clone() const {
    NTriSolidTorus* ans = new NTriSolidTorus();
    for (int i = 0; i < 3; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles[i] = vertexRoles[i];
    }
    return ans;
}

NTriSolidTorus* NTriSolidTorus::isTriSolidTorus(NTetrahedron* tet,
        NPerm useVertexRoles) {
    NTriSolidTorus* ans = new NTriSolidTorus();
    ans->tet[0] = tet;
    ans->vertexRoles[0] = useVertexRoles;
    
    // Find the adjacent tetrahedra.
    ans->tet[1] = tet->getAdjacentTetrahedron(useVertexRoles[0]);
    ans->tet[2] = tet->getAdjacentTetrahedron(useVertexRoles[1]);

    // Check that we have three distinct tetrahedra.
    if (ans->tet[1] == 0 || ans->tet[2] == 0 || ans->tet[1] == tet ||
            ans->tet[2] == tet || ans->tet[1] == ans->tet[2]) {
        delete ans;
        return 0;
    }

    // Find the vertex roles for tetrahedra 1 and 2.
    ans->vertexRoles[1] = tet->getAdjacentTetrahedronGluing(useVertexRoles[0])
        * useVertexRoles * NPerm(2, 0, 3, 1);
    ans->vertexRoles[2] = tet->getAdjacentTetrahedronGluing(useVertexRoles[1])
        * useVertexRoles * NPerm(1, 3, 0, 2);
    
    // Finally, check that tetrahedra 1 and 2 are glued together
    // properly.
    NPerm roles1 = ans->vertexRoles[1];
    if (ans->tet[1]->getAdjacentTetrahedron(roles1[0]) != ans->tet[2]) {
        delete ans;
        return 0;
    }

    if (ans->tet[1]->getAdjacentTetrahedronGluing(roles1[0]) * roles1 *
            NPerm(2, 0, 3, 1) != ans->vertexRoles[2]) {
        delete ans;
        return 0;
    }

    // We have the desired structure!
    return ans;
}

