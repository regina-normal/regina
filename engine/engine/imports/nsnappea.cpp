
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
#include <fstream.h>
#include <iomanip.h>

#ifdef __NO_INCLUDE_PATHS
    #include "nsnappea.h"
    #include "nresources.h"
    #include "ntriangulation.h"
#else
    #include "engine/imports/nsnappea.h"
    #include "engine/file/nresources.h"
    #include "engine/triangulation/ntriangulation.h"
#endif


NTriangulation* readSnapPea(const char* filename) {
    // Open the file.
    ifstream in(filename, NLocalFileResource::MODE_READ);
    if (!in)
        return 0;
    
    // Check that this is a SnapPea triangulation.
    if (in.peek() != '%')
        return 0;
    
    // Read in junk.
    NString tempStr;
    double tempDbl;

    in.ignore(1001, '\n');
    in.ignore(1001, '\n'); // Name line
    in >> tempStr;         // Solution type
    in >> tempDbl;         // Volume
    in >> tempStr;         // Orientability
    in >> tempStr;         // Chern-Simmon
    if (tempStr[3] == 'k')
        in >> tempDbl;     // Chern-Simmon is known

    unsigned i,j,k;
        
    // Read in cusp details and ignore them.
    unsigned numOrientCusps, numNonOrientCusps;
    in >> numOrientCusps >> numNonOrientCusps;

    for (i=0; i<numOrientCusps+numNonOrientCusps; i++) {
        in >> tempStr;             // Cusp type
        in >> tempDbl >> tempDbl;  // Filling information
    }
    
    // Create the new tetrahedra.
    unsigned numTet;
    in >> numTet;
    NTetrahedron **tet = new (NTetrahedron*)[numTet];
    for (i=0; i<numTet; i++)
        tet[i] = new NTetrahedron();
        
    int g[4];
    int p[4][4];
    
    for (i=0; i<numTet; i++) {
        // Test the state of the input stream.
        if (! in.good()) {
            for (j=0; j<numTet; j++)
                delete tet[j];
            delete[] tet;
            return 0;
        }
        
        // Read in adjacent tetrahedra.
        for (j=0; j<4; j++)
            in >> g[j];
        
        // Read in gluing permutations.
        for (j=0; j<4; j++) {
            in >> tempStr;
            for (k=0; k<4; k++)
                switch( tempStr[k] ) {
                    case '0': p[j][k] = 0; break;
                    case '1': p[j][k] = 1; break;
                    case '2': p[j][k] = 2; break;
                    case '3': p[j][k] = 3; break;
                    default:
                        for (j=0; j<numTet; j++)
                            delete tet[j];
                        delete[] tet;
                        return 0;
                }
        }
        
        // Perform the gluings.
        for (j=0; j<4; j++)
            tet[i]->joinTo(j, tet[g[j]],
                NPerm(p[j][0], p[j][1], p[j][2], p[j][3]));
        
        // Read in junk.
        for (j=0; j<4; j++)
            in >> tempStr;
        for (j=0; j<64; j++)
            in >> tempStr;
        for (j=0; j<2; j++)
            in >> tempStr;
    }
    
    // Build the acutal triangulation.
    NTriangulation* triang = new NTriangulation();
    for (i=0; i<numTet; i++)        
        triang->addTetrahedron(tet[i]);
    delete[] tet;
    return triang;
}

