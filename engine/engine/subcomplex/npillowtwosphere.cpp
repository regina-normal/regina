
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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
    #include "nface.h"
    #include "npillowtwosphere.h"
#else
    #include "engine/triangulation/nface.h"
    #include "engine/subcomplex/npillowtwosphere.h"
#endif

NPillowTwoSphere* NPillowTwoSphere::clone() const {
    NPillowTwoSphere* ans = new NPillowTwoSphere();
    ans->face[0] = face[0];
    ans->face[1] = face[1];
    ans->faceMapping = faceMapping;
    return ans;
}

NPillowTwoSphere* NPillowTwoSphere::formsPillowTwoSphere(
        NFace* face1, NFace* face2) {
    if (face1 == face2 || face1->isBoundary() || face2->isBoundary())
        return 0;
    NEdge* edge[2][3];
    int i;
    for (i = 0; i < 3; i++) {
        edge[0][i] = face1->getEdge(i);
        edge[1][i] = face2->getEdge(i);
    }
    if (edge[0][0] == edge[0][1] || edge[0][0] == edge[0][2] ||
            edge[0][1] == edge[0][2])
        return 0;

    // The first face has three distinct edges.  See if it matches to the
    // second face.
    int joinTo0 = -1;
    for (i = 0; i < 3; i++)
        if (edge[0][0] == edge[1][i]) {
            joinTo0 = i;
            break;
        }
    if (joinTo0 == -1)
        return 0;
    
    // Now make sure the edges all match up and with the correct
    // permutations.
    NPerm perm = face2->getEdgeMapping(joinTo0) *
        face1->getEdgeMapping(0).inverse();
    for (i = 1; i < 3; i++) {
        if (edge[0][i] != edge[1][perm[i]])
            return 0;
        if (! (face2->getEdgeMapping(perm[i]) ==
                perm * face1->getEdgeMapping(i)))
            return 0;
    }
    
    // We have an answer.
    NPillowTwoSphere* ans = new NPillowTwoSphere();
    ans->face[0] = face1;
    ans->face[1] = face2;
    ans->faceMapping = perm;
    return ans;
}

void NPillowTwoSphere::reduceTriangulation() const {
}

NTriangulation* NPillowTwoSphere::getReducedTriangulation(
        const NTriangulation* original) const {
    return 0;
}

