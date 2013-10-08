
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <fstream>

#include "foreign/recogniser.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    // Anonymous routine to write to the given output stream.
    //
    // PRE: All preconditions for writeRecogniser() have already been
    // tested, and are known to be met.
    bool writeRecogniser(std::ostream& out, NTriangulation& tri) {
        // Write the header.
        out << "triangulation" << std::endl;

        // Write face gluings.
        NTriangle* f;
        NTetrahedron* tet;
        NPerm4 vert;
        for (unsigned i = 0; i < tri.getNumberOfTriangles(); ++i) {
            f = tri.getFace(i);

            tet = f->getEmbedding(0).getTetrahedron();
            vert = f->getEmbedding(0).getVertices();
            out << 't' << (tri.tetrahedronIndex(tet) + 1)
                << '(' << (vert[0] + 1)
                << ',' << (vert[1] + 1)
                << ',' << (vert[2] + 1) << ") - ";

            tet = f->getEmbedding(1).getTetrahedron();
            vert = f->getEmbedding(1).getVertices();
            out << 't' << (tri.tetrahedronIndex(tet) + 1)
                << '(' << (vert[0] + 1)
                << ',' << (vert[1] + 1)
                << ',' << (vert[2] + 1) << ')';

            if (i != tri.getNumberOfTriangles() - 1)
                out << ',';
            out << std::endl;
        }

        // Write the footer.
        out << "end" << std::endl;

        return true;
    }
}

bool writeRecogniser(const char* filename, NTriangulation& tri) {
    // Sanity checks.
    if (! tri.isValid())
        return false;
    if (tri.hasBoundaryFaces())
        return false;

    // Write to file or stdout as appropriate.
    if (filename && *filename) {
        std::ofstream out(filename);
        if (! out)
            return 0;
        return writeRecogniser(out, tri);
    } else {
        return writeRecogniser(std::cout, tri);
    }
}

} // namespace regina
