
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

#include <algorithm>
#include "algebra/nabeliangroup.h"
#include "manifold/nsimplesurfacebundle.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "subcomplex/ntrivialtri.h"

namespace regina {

const int NTrivialTri::N2 = 200;
const int NTrivialTri::N3_1 = 301;
const int NTrivialTri::N3_2 = 302;

NTrivialTri* NTrivialTri::isTrivialTriangulation(const NComponent* comp) {
    // Since the triangulations are so small we can use census results
    // to recognise the triangulations by properties alone.

    // We only recognise closed triangulations for now.
    if (! (comp->isClosed()))
        return 0;

    // Before we do our validity check, make sure the number of
    // tetrahedra is in the supported range.
    if (comp->getNumberOfTetrahedra() > 3)
        return 0;

    // Is the triangulation valid?
    // Since the triangulations is closed we know that the vertices are
    // valid; all that remains is to check the edges.
    unsigned long nEdges = comp->getNumberOfEdges();
    unsigned long i;
    for (i = 0; i < nEdges; i++)
        if (! comp->getEdge(i)->isValid())
            return 0;

    // Test for the specific triangulations that we know about.

    if (comp->getNumberOfTetrahedra() == 2) {
        if (! comp->isOrientable())
            return new NTrivialTri(N2);
        return 0;
    }

    if (comp->getNumberOfTetrahedra() == 3) {
        if (! comp->isOrientable()) {
            // If the triangulation is valid and the edge degrees
            // are 2,4,4,6 then we have N(3,1) or N(3,2).
            // All of the vertices are valid since there are no boundary
            // faces; we thus only need to check the edges.
            if (comp->getNumberOfEdges() != 4)
                return 0;

            int degree[4];
            for (i = 0; i < 4; i++)
                degree[i] = comp->getEdge(i)->getDegree();
            std::sort(degree, degree + 4);

            if (degree[0] == 2 && degree[1] == 4 && degree[2] == 6 &&
                    degree[3] == 6) {
                // We have N(3,1) or N(3,2)!
                // Search for Mobius band faces.
                unsigned long nFaces = comp->getNumberOfFaces();
                for (i = 0; i < nFaces; i++)
                    if (comp->getFace(i)->isMobiusBand())
                        return new NTrivialTri(N3_2);
                return new NTrivialTri(N3_1);
            }
        }
    }

    return 0;
}

NManifold* NTrivialTri::getManifold() const {
    if (type == N2)
        return new NSimpleSurfaceBundle(NSimpleSurfaceBundle::S2xS1_TWISTED);
    else if (type == N3_1 || type == N3_2)
        return new NSimpleSurfaceBundle(NSimpleSurfaceBundle::RP2xS1);
    return 0;
}

NAbelianGroup* NTrivialTri::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank();
    if (type == N3_1 || type == N3_2)
        ans->addTorsionElement(2);
    return ans;
}

inline std::ostream& NTrivialTri::writeName(std::ostream& out) const {
    if (type == N2)
        out << "N(2)";
    else if (type == N3_1)
        out << "N(3,1)";
    else if (type == N3_2)
        out << "N(3,2)";
    return out;
}
inline std::ostream& NTrivialTri::writeTeXName(std::ostream& out) const {
    if (type == N2)
        out << "$N_{2}$";
    else if (type == N3_1)
        out << "$N_{3,1}$";
    else if (type == N3_2)
        out << "$N_{3,2}$";
    return out;
}
inline void NTrivialTri::writeTextLong(std::ostream& out) const {
    if (type == N2)
        out << "Non-orientable triangulation N(2)";
    else if (type == N3_1)
        out << "Non-orientable triangulation N(3,1)";
    else if (type == N3_2)
        out << "Non-orientable triangulation N(3,2)";
}

} // namespace regina

