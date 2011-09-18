
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

#include "snappea/nsnappeatriangulation.h"
#include "snappea/kernel/triangulation.h"

int* get_cusp_equation(Triangulation* manifold, int cusp_num, int m, int l, int* numRows);
void free_cusp_equation(int *equation);


NMatrixInt NSnapPeaTriangulation::slopeEquations() const {
    int i,j;
    if (! snappeaData)
        return 0;
    NMatrixInt matrix(2*manifold->num_cusps, 3*manifold->num_tetrahedra);
    for(i=0; i< manifold->num_cusps; i++) {
        int numRows;
        int *equations = get_cusp_equation(snappeaData, cusp, 1, 0, &numRows);
        for(j=0; j< manifold->num_tetrahedra; j++) {
            matrix.entry(2*cusp,j) = equations[j];
        }
        free_cusp_equation(equations)
        *equations = get_cusp_equation(snappeaData, cusp, 0, 1, &numRows);
        for(j=0; j< manifold->num_tetrahedra; j++) {
            matrix.entry(2*cusp+1,j) = equations[j];
        }
        free_cusp_equation(equations)
    }
}
