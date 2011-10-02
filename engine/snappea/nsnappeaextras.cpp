
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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


#include <cassert>

#include "maths/nmatrixint.h"
#include "triangulation/ntriangulation.h"
#include "snappea/nsnappeatriangulation.h"
#include "snappea/kernel/triangulation.h"
#include "snappea/kernel/kernel_prototypes.h"

namespace regina {


NMatrixInt NSnapPeaTriangulation::slopeEquations() const {
    int i,j;
    if (! snappeaData)
        return NMatrixInt(0,0);
    NMatrixInt matrix(2*snappeaData->num_cusps, 3*snappeaData->num_tetrahedra);
    ::peripheral_curves(snappeaData);
    for(i=0; i< snappeaData->num_cusps; i++) {
        int numRows;
        // SnapPea returns "a b c" for each tetrahedra where the slope is given
        // as
        //   a log (z_0) + b log ( 1/(1-z_0)) + c log ((z_0 - 1)/z_0) + ... = 1
        //
        // The equation for slopes in terms of quads of types q, q' and q'' becomes
        //   nu = (b-c)q + (-a + c)q' + (a-b)q''
        //   
        int *equations = ::get_cusp_equation(snappeaData, i, 1, 0, &numRows);
        for(j=0; j< snappeaData->num_tetrahedra; j++) {
            matrix.entry(2*i,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix.entry(2*i,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix.entry(2*i,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        ::free_cusp_equation(equations);
        equations = ::get_cusp_equation(snappeaData, i, 0, 1, &numRows);
        for(j=0; j< snappeaData->num_tetrahedra; j++) {
            matrix.entry(2*i+1,3*j) = equations[3*j+1] - equations[3*j+2];
            matrix.entry(2*i+1,3*j+1) = equations[3*j+2] - equations[3*j];
            matrix.entry(2*i+1,3*j+2) = equations[3*j] - equations[3*j+1];
        }
        ::free_cusp_equation(equations);
    }
    return matrix;
}


bool NSnapPeaTriangulation::verifyTriangulation(const NTriangulation& tri) const {
    if (! snappeaData)
        return false;

    ::TriangulationData *data;
    ::triangulation_to_data(snappeaData, &data);

    int tet, face, i, j, k, l;
    assert ( data->num_tetrahedra == tri.getNumberOfTetrahedra() );

    NTriangulation::TetrahedronIterator it = tri.getTetrahedra().begin();
    for (tet = 0; tet < data->num_tetrahedra; tet++) {
        for (face = 0; face < 4; face++) {
            assert (data->tetrahedron_data[tet].neighbor_index[face] ==
                tri.tetrahedronIndex((*it)->adjacentTetrahedron(face)) );
            for (i = 0; i < 4; i++)
                assert (data->tetrahedron_data[tet].gluing[face][i] ==
                    (*it)->adjacentGluing(face)[i] );
        }
        it++;
    }
    return true;
}



} // namespace
