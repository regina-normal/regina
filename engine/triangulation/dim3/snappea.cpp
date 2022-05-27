
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>

#include "triangulation/dim3.h"
#include "utilities/stringutils.h"

namespace regina {

Triangulation<3> Triangulation<3>::fromSnapPea(const std::string& snapPeaData) {
    std::istringstream in(snapPeaData);

    // Check that this is a SnapPea triangulation.
    char name[1001];
    unsigned len;
    in.getline(name, 1000);
    if (in.fail() || in.eof())
        throw InvalidArgument("fromSnapPea(): unexpected end of string");
    // Allow junk on the same line following the triangulation marker.
    if (strncmp(name, "% Triangulation", 15) != 0 &&
            strncmp(name, "% triangulation", 15) != 0)
        throw InvalidArgument("fromSnapPea(): missing triangulation marker");

    // Read in the manifold name.
    // Unfortunately Triangulation<3> has nowhere to put this, so for
    // now we just read it and then forget about it.
    in.getline(name, 1000);
    if (in.fail() || in.eof())
        throw InvalidArgument("fromSnapPea(): unexpected end of string");
    if ((len = strlen(name)) > 0 && name[len - 1] == '\r')
        name[len - 1] = 0;

    // Read in junk.
    std::string tempStr;
    double tempDbl;

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
    Triangulation<3> triang;

    unsigned numTet;
    in >> numTet;
    auto* tet = new Tetrahedron<3>*[numTet];
    for (i=0; i<numTet; i++)
        tet[i] = triang.newTetrahedron();

    int g[4];
    int p[4][4];

    for (i=0; i<numTet; i++) {
        // Test the state of the input stream.
        if (! in.good()) {
            delete[] tet;
            throw InvalidArgument("fromSnapPea(): string not in the "
                "correct format");
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
                        delete[] tet;
                        throw InvalidArgument("fromSnapPea(): "
                            "invalid permutation");
                }
        }

        // Perform the gluings.
        for (j=0; j<4; j++) {
            Perm<4> gluing(p[j][0], p[j][1], p[j][2], p[j][3]);
            if (auto adj = tet[i]->adjacentSimplex(j)) {
                // This gluing has already been made from the other side.
                if (adj != tet[g[j]] || tet[i]->adjacentGluing(j) != gluing) {
                    delete[] tet;
                    throw InvalidArgument("fromSnapPea(): "
                        "inconsistent tetrahedron gluings");
                }
            } else
                tet[i]->join(j, tet[g[j]], gluing);
        }

        // Read in junk.
        for (j=0; j<4; j++)
            in >> tempStr;
        for (j=0; j<64; j++)
            in >> tempStr;
        for (j=0; j<2; j++)
            in >> tempStr;
    }

    // All done!
    delete[] tet;
    return triang;
}

} // namespace regina
